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
//	File:   include/sak/ranges/count_to.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-08-02 00:14:23
//	


#pragma once
#ifndef header_guard_734082913
#define header_guard_734082913


#include <concepts>
#include <ranges>
#include <sak/using.hpp>


namespace sak::ranges {


//	--------------------------------------------------
__using( ::std::views::, iota )
//	--------------------------------------------------


//	integer range [ 0, bound ): casts the zero to the bound type so the
//	iota endpoints share the same integer type
struct __count_to
{
	template< ::std::integral t_integer >
	constexpr auto operator ( ) ( const t_integer bound ) const
	{
		return	iota( static_cast< t_integer >( 0 ), bound );
	}
};

inline constexpr auto count_to = __count_to{ };


}


#endif

