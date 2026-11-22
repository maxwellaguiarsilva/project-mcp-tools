//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_574051120
#define header_guard_574051120


#include <sak/sak.hpp>
#include <meta>
#include <string_view>


namespace sak {
namespace meta {


__using( ::std::
	,define_static_array
	,is_enum_v
	,string
	,string_view
)
__using( ::std::meta::
	,enumerators_of
	,identifier_of
)
__using( ::sak::, ensure )


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


} } 


#endif


