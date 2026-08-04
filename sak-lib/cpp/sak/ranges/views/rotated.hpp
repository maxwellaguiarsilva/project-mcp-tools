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
	,size_t
)
__using( ::std::ranges::
	,viewable_range
	,size
	,range_adaptor_closure
)
__using( ::std::views::
	,concat
	,drop
	,take
)
//	--------------------------------------------------


struct __rotated : range_adaptor_closure< __rotated >
{
	struct closure : range_adaptor_closure< closure >
	{
		size_t m_offset;
		constexpr explicit closure( size_t offset ) : m_offset( offset ) { }
		template< viewable_range t_range >
		constexpr auto operator ( ) ( t_range&& range ) const
		{
			return	__rotated{ }( ::std::forward< t_range >( range ), m_offset );
		}
	};

	template< viewable_range t_range >
	constexpr auto operator ( ) ( t_range&& range ) const
	{
		return	__rotated{ }( ::std::forward< t_range >( range ), 1 );
	}

	template< viewable_range t_range >
	constexpr auto operator ( ) ( t_range&& range, const size_t offset ) const
	{
		const auto length = size( range );
		return	concat( range, range )
			|	drop( offset % length )
			|	take( length );
	}

	constexpr auto operator ( ) ( const size_t offset ) const
	{
		return	closure{ offset };
	}
};

inline constexpr auto rotated = __rotated{ };

}


#endif


