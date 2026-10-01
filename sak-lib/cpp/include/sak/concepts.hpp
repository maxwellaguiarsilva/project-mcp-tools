//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_343639034
#define header_guard_343639034


#include <tuple>
#include <sak/geometry/concepts.hpp>
#include <sak/pattern/enum.hpp>


namespace sak {


__using( ::std::
	,remove_cvref_t
	,same_as
	,tuple_size
	,is_class_v
	,is_union_v
	,is_enum_v
)

//	callable object: anything the compiler accepts calling with operator() (zero or more arguments)
template< typename t_callable, typename... t_args >
concept is_callable = requires( t_callable&& callable, t_args&&... args ) {
	static_cast< t_callable&& >( callable )( static_cast< t_args&& >( args )... );
};

template< typename t_tuple >
concept is_tuple = requires { typename tuple_size< remove_cvref_t< t_tuple > >::type; };

template< typename t_type >
concept is_class = is_class_v< t_type >;

template< typename t_type >
concept is_union = is_union_v< t_type >;

template< typename t_type >
concept is_enum = is_enum_v< t_type >;

//	enum type whose enumerators are statically castable to a given target type
template< typename t_enum, typename t_target >
concept is_castable_enum = is_enum< t_enum > and requires( const t_enum value ) { static_cast< t_target >( value ); };

//	variadic identity: every type is the same after removing references and cv-qualifiers
template< typename t_first, typename... t_rest >
concept is_same_decayed = ( same_as< remove_cvref_t< t_first >, remove_cvref_t< t_rest > > and ... );


}


#endif


