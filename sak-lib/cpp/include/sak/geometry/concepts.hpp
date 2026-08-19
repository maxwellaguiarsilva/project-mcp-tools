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
//	File:   include/sak/geometry/concepts.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-08-16 15:55:34
//	


#pragma once
#ifndef header_guard_013585305
#define header_guard_013585305


#include <sak/math/concepts.hpp>


namespace sak {

//	forward declaration so the trait can detect point specializations
template< ::sak::math::is_arithmetic t_scalar, size_t num_dimensions >
requires	( num_dimensions > 1 )
class point;

__using( ::std::
	,false_type
	,remove_cvref_t
	,true_type
)

template< typename t_point >
struct __is_point : false_type { };
template< ::sak::math::is_arithmetic t_scalar, size_t num_dimensions >
struct __is_point< point< t_scalar, num_dimensions > > : true_type { };
template< typename t_point >
concept is_point = __is_point< remove_cvref_t< t_point > >::value;


}


#endif


