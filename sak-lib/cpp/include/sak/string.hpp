//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_501228163
#define header_guard_501228163


#include <sak/sak.hpp>
#include <sak/math/math.hpp>
#include <sak/ranges/transform.hpp>
#include <ranges>
#include <algorithm>


namespace sak {


using	::std::string;
using	::sak::ranges::eager_transform;
using	::sak::math::between;


constexpr char delta_case	=	( 'a' - 'A' );


struct __to_lower_case
{
	constexpr auto operator ( ) ( char code ) const noexcept -> char
	{
		return	( between( code, 'A', 'Z' ) ? static_cast< char >( code + delta_case ) : code );
	}

	auto operator ( ) ( string text ) const -> string
	{
		eager_transform( text, text.begin( ), *this );
		return	text;
	}
};
inline constexpr auto to_lower_case = __to_lower_case{ };


struct __to_upper_case
{
	constexpr auto operator ( ) ( char code ) const noexcept -> char
	{
		return	( between( code, 'a', 'z' ) ? static_cast< char >( code - delta_case ) : code );
	}

	auto operator ( ) ( string text ) const -> string
	{
		eager_transform( text, text.begin( ), *this );
		return	text;
	}
};
inline constexpr auto to_upper_case = __to_upper_case{ };


}


#endif


