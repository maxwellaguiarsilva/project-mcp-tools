//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_1785682413
#define header_guard_1785682413


#include <sak/concepts.hpp>
#include <sak/sak.hpp>
#include <sak/math/math.hpp>
#include <charconv>
#include <concepts>
#include <string>
#include <system_error>


namespace sak {
namespace pattern {


__using( ::std::
	,errc
	,from_chars
	,string
)
__using( ::sak::math::
	,is_number
)


struct __to_number
{
	template< is_number t_number >
	constexpr auto operator ( ) ( const string& value, const t_number default_value ) const noexcept -> t_number
	{
		t_number result{ default_value };
		const auto [ pointer, error ] = from_chars( value.data( ), value.data( ) + value.size( ), result );
		return	pointer == value.data( ) + value.size( ) and error == errc{ } ? result : default_value;
	}
};
inline constexpr auto to_number = __to_number{ };


} }


#endif


