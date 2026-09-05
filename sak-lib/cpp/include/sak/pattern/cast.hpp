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
//	File:   include/sak/pattern/cast.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-09-05 19:19:18
//	


#pragma once
#ifndef header_guard_595213129
#define header_guard_595213129


#include <sak/sak.hpp>
#include <sak/ranges/transform.hpp>
#include <ranges>
#include <utility>


namespace sak {
namespace pattern {


__using( ::sak::math::, is_value )
__using( ::sak::ranges::, lazy_transform )
__using( ::std::ranges::, input_range, range_adaptor_closure )


template< typename t_target >
struct __cast : range_adaptor_closure< __cast< t_target > >
{
	template< is_value t_source >
	constexpr auto operator ( ) ( t_source&& value ) const noexcept -> t_target
	{
		return	static_cast< t_target >( ::std::forward< t_source >( value ) );
	}

	template< input_range t_range >
	constexpr auto operator ( ) ( t_range&& range ) const
	{
		return	lazy_transform( ::std::forward< t_range >( range ), *this );
	}
};


template< typename t_target >
inline constexpr auto cast = __cast< t_target >{ };


} } 


#endif


