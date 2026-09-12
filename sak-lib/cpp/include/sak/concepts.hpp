//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_343639034
#define header_guard_343639034


#include <sak/math/concepts.hpp>
#include <tuple>


namespace sak {


__using( ::std::
	,remove_cvref_t
	,tuple_size
)

//	callable object: anything the compiler accepts calling with operator() (zero or more arguments)
template< typename t_callable, typename... t_args >
concept is_callable = requires( t_callable&& callable, t_args&&... args ) {
	static_cast< t_callable&& >( callable )( static_cast< t_args&& >( args )... );
};

template< typename t_tuple >
concept is_tuple = requires { typename tuple_size< remove_cvref_t< t_tuple > >::type; };

//	enum type whose enumerators are statically castable to a given target type
template< typename t_enum, typename t_target >
concept is_castable_enum = ::std::is_enum_v< t_enum > and requires( const t_enum value ) { static_cast< t_target >( value ); };


}


#endif


