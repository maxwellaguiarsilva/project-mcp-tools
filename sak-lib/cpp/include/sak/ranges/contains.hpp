//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
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


