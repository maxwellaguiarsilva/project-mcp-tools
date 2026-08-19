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


#include <sak/ranges/operators.hpp>
#include <sak/ranges/views/rotated.hpp>


namespace sak {
namespace math {


__using( ::sak::ranges::
	,is_view
	,operator*
	,operator+
	,operator-
	,operator/
)
__using( ::sak::ranges::views::
	,rotated
)


//	euclidean norm of a vector: sqrt( dot( vector, vector ) )
//	reduction to a scalar, so it is a plain function object (like sum), not pipeable
struct __length
{
	constexpr auto operator ( ) ( const auto& vector ) const noexcept { return square_root( sum( vector * vector ) ); }
};
inline constexpr auto length = __length{ };


//	unit vector in the same direction, lazy when the input is a view
//	direct-application closure: the pipe applies to the whole vector, no element-wise transform
struct __normalize : ::std::ranges::range_adaptor_closure< __normalize >
{
	template< ::std::ranges::viewable_range t_vector >
	constexpr auto operator ( ) ( t_vector&& vector ) const noexcept
	{
		return	::std::forward< t_vector >( vector ) / length( vector );
	}
};
inline constexpr auto normalize = __normalize{ };


//	cross product via cyclic permutation, only valid for 3-element vectors
//	direct-application binary closure: v | cross( right ) applies to the whole vector
struct __cross
{
	template< typename t_right >
	struct closure : ::std::ranges::range_adaptor_closure< closure< t_right > >
	{
		t_right m_right;
		constexpr explicit closure( t_right right ) : m_right( right ) { }
		template< ::std::ranges::viewable_range t_left >
		constexpr auto operator ( ) ( t_left&& left ) const noexcept
		{
			return	__cross{ }( ::std::forward< t_left >( left ), m_right );
		}
	};

	template< is_crossable t_left, is_crossable t_right >
	constexpr auto operator ( ) ( const t_left& left, const t_right& right ) const noexcept
	{
		return	( left | rotated( 1 ) ) * ( right | rotated( 2 ) ) - ( left | rotated( 2 ) ) * ( right | rotated( 1 ) );
	}

	template< typename t_right >
	constexpr auto operator ( ) ( t_right right ) const { return closure< t_right >{ right }; }
};
inline constexpr auto cross = __cross{ };


//	rodrigues rotation of a vector around an axis by an angle, lazy result
//	direct-application ternary closure: v | rotate( axis, angle ) applies to the whole vector
//	uses the inline sum instead of dot so heterogeneous point/view operands work
struct __rotate
{
	template< typename t_axis >
	struct closure : ::std::ranges::range_adaptor_closure< closure< t_axis > >
	{
		t_axis m_axis;
		float m_angle;
		constexpr closure( t_axis axis, float angle ) : m_axis( axis ), m_angle( angle ) { }
		template< ::std::ranges::viewable_range t_vector >
		constexpr auto operator ( ) ( t_vector&& vector ) const noexcept
		{
			return	__rotate{ }( ::std::forward< t_vector >( vector ), m_axis, m_angle );
		}
	};

	constexpr auto operator ( ) ( const auto& vector, const auto& axis, float angle ) const noexcept
	{
		const float cos_angle = sak_cos( angle );
		const float sin_angle = sak_sin( angle );
		return	vector * cos_angle + cross( axis, vector ) * sin_angle + axis * sum( axis * vector ) * ( 1.0f - cos_angle );
	}

	template< typename t_axis >
	constexpr auto operator ( ) ( t_axis axis, float angle ) const { return closure< t_axis >{ axis, angle }; }
};
inline constexpr auto rotate = __rotate{ };


} }
 

#endif


