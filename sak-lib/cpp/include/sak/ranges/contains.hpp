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

	//	true if the associative container holds the key ( prefers member contains, falls back to find )
	template< typename t_container >
	requires( input_range< t_container >
		and requires( const t_container& container, const typename t_container::key_type& key ) { container.find( key ); }
		and not ::std::equality_comparable_with< range_reference_t< t_container >, typename t_container::key_type > )
	constexpr auto operator ( ) ( const t_container& container, const typename t_container::key_type& key ) const noexcept -> bool
	{
		if constexpr( requires { container.contains( key ); } )
			return	container.contains( key );
		else
			return	container.find( key ) not_eq container.end( );
	}
};

inline constexpr auto contains = __contains{ };


} } 


#endif


