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
//	File:   include/sak/math/vector.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-08-01 23:10:00
//	


#pragma once
#ifndef header_guard_457193204
#define header_guard_457193204


#include <sak/sak.hpp>
#include <sak/ranges/operators.hpp>
#include <sak/ranges/views/rotated.hpp>


namespace sak {
namespace math {


__using( ::sak::ranges::
	,is_view
	,operator*
	,operator/
	,operator-
)
__using( ::sak::ranges::views::
	,rotated
)


template< typename t_vector >
concept is_triplet	=	( not is_view< t_vector > ) and requires( const t_vector& vector ) { requires ::std::ranges::size( vector ) == 3; };

//	fixed-size tuples are verified at compile time, lazy views are trusted
template< typename t_vector >
concept is_crossable	=	is_triplet< t_vector > or is_view< t_vector >;


//	euclidean norm of a vector: sqrt( dot( vector, vector ) )
constexpr auto length( const auto& vector ) noexcept { return square_root( sum( vector * vector ) ); }

//	unit vector in the same direction, lazy when the input is a view
constexpr auto normalize( const auto& vector ) noexcept { return vector / length( vector ); }

//	cross product via cyclic permutation, only valid for 3-element vectors
template< is_crossable t_left, is_crossable t_right >
constexpr auto cross( const t_left& left, const t_right& right ) noexcept
{
	return	( left | rotated( 1 ) ) * ( right | rotated( 2 ) ) - ( left | rotated( 2 ) ) * ( right | rotated( 1 ) );
}


} }
 

#endif


