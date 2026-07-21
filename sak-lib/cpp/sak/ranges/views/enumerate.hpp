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
//	File:   include/sak/ranges/views/enumerate.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-02-21 16:00:00
//	


#pragma once
#ifndef header_guard_1597534682
#define header_guard_1597534682


#include <sak/sak.hpp>
#include <sak/using.hpp>
#include <ranges>
#include <utility>


namespace sak::ranges::views {


//    ------------------------------
__using( ::std::, ptrdiff_t )
__using( ::std::views::
	,iota
	,zip
)
__using( ::std::ranges::
	,viewable_range
)
//    ------------------------------


struct __enumerate
{
	template< viewable_range t_range >
	constexpr auto operator ( ) ( t_range&& range, const ptrdiff_t start_index = 0 ) const
	{
		return	zip( iota( start_index ), ::std::forward< t_range >( range ) );
	}
};


inline constexpr auto enumerate = __enumerate{ };


template< viewable_range t_range >
constexpr auto operator bitor( t_range&& range, const __enumerate& )
{
	return	enumerate( ::std::forward< t_range >( range ) );
}


}


#endif


