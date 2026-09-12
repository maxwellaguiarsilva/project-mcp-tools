//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
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
			return	::std::views::transform(
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


