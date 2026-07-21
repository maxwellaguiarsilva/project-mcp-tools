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
//	File:   include/sak/ranges/chunk.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-01-05 20:32:14
//	


#pragma once
#ifndef header_guard_555387710
#define header_guard_555387710


#include <ranges>
#include <algorithm>
#include <concepts>
#include <sak/using.hpp>


namespace sak::ranges {


//	--------------------------------------------------
__using( ::std::
	,min
	,size_t
)
__using( ::std::ranges::
	,viewable_range
	,subrange
	,size
	,begin
	,range_adaptor_closure
)
__using( ::std::views::
	,all
	,iota
	,transform
)
//	--------------------------------------------------


struct __chunk
{
	struct closure : public range_adaptor_closure< closure >
	{
		constexpr explicit closure( const size_t offset ) : m_offset( offset ) { }

		template< viewable_range t_range >
		constexpr auto operator ( ) ( t_range&& range ) const
		{
			auto view = all( ::std::forward< t_range >( range ) );
			const auto length = size( view );
			return	transform(
				 iota( size_t( 0 ), ( length + m_offset - 1 ) / m_offset )
				,[ view, offset = m_offset, length ]( const size_t index ) { return subrange(
					 begin( view ) + ( index * offset )
					,begin( view ) + min( ( index + 1 ) * offset, length )
				); }
			);
		}

		private:
			size_t m_offset;
	};

	template< viewable_range t_range >
	constexpr auto operator ( ) ( t_range&& range, const size_t offset ) const
	{
		return	closure( offset )( ::std::forward< t_range >( range ) );
	}

	constexpr auto operator ( ) ( const size_t offset ) const
	{
		return	closure( offset );
	}
};

inline constexpr auto chunk = __chunk{ };


}


#endif


