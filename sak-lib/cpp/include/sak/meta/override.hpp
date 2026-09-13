//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_773859171
#define header_guard_773859171


#include <meta>
#include <string_view>
#include <vector>
#include <sak/using.hpp>


namespace sak {
namespace meta {


//	--------------------------------------------------
__using( ::std::
	,define_static_array
	,define_static_string
	,string_view
	,vector
)
__using( ::std::meta::
	,access_context
	,has_identifier
	,identifier_of
	,info
	,is_function
	,is_override
	,is_special_member_function
	,is_virtual
	,members_of
)
//	--------------------------------------------------


//	every virtual method declared by the base interface, in declaration order
template< typename t_base >
consteval auto virtual_methods( )
{
	vector< info > result;
	for( auto method : members_of( ^^t_base, access_context::current( ) ) )
		if( is_function( method ) and not is_special_member_function( method ) and is_virtual( method ) )
			result.push_back( method );
	return	result;
}


template< typename t_base, typename t_derived >
consteval auto overridden_methods( )
{
	vector< info > result;
	for( auto method : virtual_methods< t_base >( ) )
		for( auto candidate : members_of( ^^t_derived, access_context::current( ) ) )
		{
			if( not is_function( candidate ) or not is_override( candidate ) )
				continue;
			if( has_identifier( method ) and has_identifier( candidate ) and identifier_of( method ) == identifier_of( candidate ) )
			{
				result.push_back( method );
				break;
			}
		}
	return	result;
}


template< typename t_base, typename t_derived >
consteval auto is_overridden( const string_view name ) -> bool
{
	for( auto method : overridden_methods< t_base, t_derived >( ) )
		if( identifier_of( method ) == name )
			return	true;
	return	false;
}


template< typename t_base, typename t_derived >
consteval auto overridden_names( )
{
	vector< const char* > result;
	for( auto method : overridden_methods< t_base, t_derived >( ) )
		result.push_back( define_static_string( identifier_of( method ) ) );
	return	define_static_array( result );
}


template< info t_method >
consteval auto method_pointer( )
{
	return	&[: t_method :];
}


template< typename t_base, typename t_derived, info t_method >
consteval auto is_overridden( )
{
	return	is_overridden< t_base, t_derived >( identifier_of( t_method ) );
}


template< info t_method, typename t_dispatcher, typename... t_args >
auto dispatch_reflected( t_dispatcher& dispatcher_instance, t_args&&... arguments )
{
	return	dispatcher_instance.template operator ( )< t_method >( arguments... );
}


} } 


#endif


