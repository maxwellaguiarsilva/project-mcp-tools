//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
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


