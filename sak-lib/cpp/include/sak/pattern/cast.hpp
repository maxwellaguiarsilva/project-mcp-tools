//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
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


