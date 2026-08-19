//	
//	Copyright (C) 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//	
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//	
//	You should have received a copy of the GNU General Public License
//	along with this program.  If not, see <http://www.gnu.org/licenses/>.
//	
//	
//	File:   include/sak/concepts.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-08-16 15:15:43
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


}


#endif


