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
//	File:   include/sak/math/concepts.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-08-12 13:02:42
//	


#pragma once
#ifndef header_guard_718150240
#define header_guard_718150240


#include <sak/ranges/concepts.hpp>


namespace sak {
namespace math {


//	concepts narrowing the scalar overloads of the niebloids below
__using( ::std::
	,is_arithmetic_v
	,is_integral_v
	,same_as
)
__using( ::std::ranges::
	,size
)
template< typename t_arithmetic >
concept is_arithmetic = is_arithmetic_v< t_arithmetic >;

template< typename t_integral >
concept is_integral = is_integral_v< t_integral >;

//	single value, not a range: routes ranges to the element-wise overload instead of the scalar one
template< typename t_value >
concept is_value	=	not ::std::ranges::input_range< t_value >;

template< typename t_number >
concept is_number	=	is_arithmetic< t_number > and not same_as< t_number, bool >;

//	fixed-size tuples are verified at compile time, lazy views are trusted
template< typename t_vector >
concept is_triplet	=	( not ::sak::ranges::is_view< t_vector > ) and requires( const t_vector& vector ) { requires size( vector ) == 3; };

template< typename t_vector >
concept is_crossable	=	is_triplet< t_vector > or ::sak::ranges::is_view< t_vector >;


} }


#endif


