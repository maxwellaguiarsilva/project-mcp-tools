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
//	File:   include/sak/ranges/contains.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-08-15 16:34:00
//	


#pragma once
#ifndef header_guard_492294529
#define header_guard_492294529


#include <algorithm>
#include <concepts>
#include <functional>
#include <initializer_list>
#include <ranges>
#include <utility>
#include <sak/using.hpp>


namespace sak {
namespace ranges {


//	--------------------------------------------------
__using( ::std::ranges::
	,any_of
	,input_range
	,range_reference_t
)
__using( ::std::
	,bind_front
)

inline constexpr auto std_contains = ::std::ranges::contains;
//	--------------------------------------------------


//	true if the range contains the value ( single-value overload, mirrors ::std::ranges::contains )
struct __contains
{
	template< input_range t_range, class t_value >
	requires( ::std::equality_comparable_with< range_reference_t< t_range >, t_value > )
	constexpr auto operator ( ) ( t_range&& range, const t_value& value ) const -> bool
	{
		return	std_contains( ::std::forward< t_range >( range ), value );
	}

	//	true if the range contains any of the braced-list values: contains( range, { "foo", "bar" } )
	template< input_range t_range, class t_value >
	requires( ::std::equality_comparable_with< range_reference_t< t_range >, t_value > )
	constexpr auto operator ( ) ( t_range&& range, ::std::initializer_list< t_value > values ) const -> bool
	{
		return	any_of( ::std::forward< t_range >( range ), bind_front( std_contains, values ) );
	}
};

inline constexpr auto contains = __contains{ };


} } 


#endif


