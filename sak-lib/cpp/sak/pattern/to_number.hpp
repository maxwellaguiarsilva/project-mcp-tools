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
//	File:   include/sak/pattern/to_number.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-08-02 12:53:33
//	


#pragma once
#ifndef header_guard_1785682413
#define header_guard_1785682413


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
	,same_as
	,string
)
__using( ::sak::math::
	,is_arithmetic
)


template< typename t_number >
concept is_number	=	is_arithmetic< t_number > and not same_as< t_number, bool >;


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
