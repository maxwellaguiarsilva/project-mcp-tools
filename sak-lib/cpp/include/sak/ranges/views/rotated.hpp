//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
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


