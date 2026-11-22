//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_938700816
#define header_guard_938700816


#include <sak/sak.hpp>
#include <bitset>
#include <meta>
#include <string_view>
#include <vector>
#include <sak/ranges/index_of.hpp>


namespace sak {
namespace meta {


//	--------------------------------------------------
__using( ::std::
	,bitset
	,define_static_array
	,define_static_string
	,is_enum_v
	,is_trivially_copyable_v
	,ptrdiff_t
	,string
	,string_view
	,vector
)
__using( ::std::meta::
	,access_context
	,enumerators_of
	,has_identifier
	,identifier_of
	,info
	,is_function
	,is_override
	,is_special_member_function
	,is_virtual
	,members_of
	,nonstatic_data_members_of
	,offset_of
	,size_of
	,type_of
)
__using( ::sak::, ensure )
__using( ::sak::ranges::, index_of )
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


//	reflective model pairing an interface with an implementation
//	answers which methods exist, each one's position
//	and the mask of methods the implementation overrides
template< typename t_interface, typename t_implementation = t_interface >
struct override_model
{
	using	override_mask	=	bitset< virtual_methods< t_interface >( ).size( ) >;

	static consteval auto methods( ) { return virtual_methods< t_interface >( ); }
	static consteval auto position( const info t_method ) { return *index_of( methods( ), t_method ); }
	static consteval auto overridden( ) { return overridden_methods< t_interface, t_implementation >( ); }

	static consteval auto mask( )
	{
		override_mask	result;
		for( const auto& method : overridden( ) )
			result.set( position( method ) );
		return	result;
	}
};


template< info t_method >
consteval auto method_pointer( ) { return &[: t_method :]; }

template< typename t_base, typename t_derived, info t_method >
consteval auto is_overridden( ) { return is_overridden< t_base, t_derived >( identifier_of( t_method ) ); }


//	map an enumerator name to the enumerator it declares
template< typename t_enum >
	requires is_enum_v< t_enum >
auto kind_of( const string_view name ) -> t_enum
{
	t_enum kind{ };
	bool found = false;
	template for( constexpr auto enumerator : define_static_array( enumerators_of( ^^t_enum ) ) )
		if( not found and identifier_of( enumerator ) == name )
		{
			kind = [: enumerator :];
			found = true;
		}
	ensure( found, "unknown enumerator: " + string( name ) );
	return	kind;
}


//	name of an enumerator as declared by the enumerator itself
template< typename t_enum >
	requires is_enum_v< t_enum >
auto name_of( const t_enum kind ) -> string_view
{
	string_view name;
	template for( constexpr auto enumerator : define_static_array( enumerators_of( ^^t_enum ) ) )
		if( [: enumerator :] == kind )
			name = identifier_of( enumerator );
	ensure( not name.empty( ), "unknown enumerator value" );
	return	name;
}


//	a type whose bytes are a contiguous payload with no padding
consteval auto has_no_padding( const info type ) -> bool
{
	ptrdiff_t offset = 0;
	for( const auto member : nonstatic_data_members_of( type, access_context::current( ) ) )
	{
		if( offset_of( member ).bytes not_eq offset )
			return	false;
		offset += size_of( type_of( member ) );
	}
	return	offset == static_cast< ptrdiff_t >( size_of( type ) );
}

template< typename t_type >
concept is_flat = has_no_padding( ^^t_type ) and is_trivially_copyable_v< t_type >;


} } 


#endif


