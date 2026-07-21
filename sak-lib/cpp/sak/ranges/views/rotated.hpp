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
//	File:   include/sak/ranges/views/rotated.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-02-22 14:53:08
//	


#pragma once
#ifndef header_guard_244814717
#define header_guard_244814717


#include <ranges>
#include <sak/using.hpp>


namespace sak::ranges::views {


//	--------------------------------------------------
__using( ::std::
	,array
	,size_t
)
__using( ::std::ranges::
	,viewable_range
	,size
)
__using( ::std::views::
	,all
	,drop
	,take
	,join
)
//	--------------------------------------------------


struct __rotated
{
	template< viewable_range t_range >
	constexpr auto operator ( ) ( t_range&& range, const size_t offset ) const
	{
		using	::std::views::all;
		const auto length = size( range );
		//	manual concat using array of views + join
		return	array{ all( ::std::forward< t_range >( range ) ), all( ::std::forward< t_range >( range ) ) }
			|	join
			|	drop( offset % length )
			|	take( length );
	}

	constexpr auto operator ( ) ( const size_t offset ) const
	{
		return	[ offset ] ( viewable_range auto&& range ) {
			return	__rotated{ }( ::std::forward< decltype( range ) >( range ), offset );
		};
	}
};

inline constexpr auto rotated = __rotated{ };

template< viewable_range t_range >
constexpr auto operator bitor ( t_range&& range, const __rotated& )
{
	return	rotated( ::std::forward< t_range >( range ), 1 );
}

template< viewable_range t_range, typename t_closure >
requires requires { { ::std::declval< t_closure >( )( ::std::declval< t_range >( ) ) }; }
constexpr auto operator bitor ( t_range&& range, t_closure&& closure )
{
	return	::std::forward< t_closure >( closure )( ::std::forward< t_range >( range ) );
}

}


#endif


