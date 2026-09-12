//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
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


