//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_1597534682
#define header_guard_1597534682


#include <sak/sak.hpp>
#include <sak/using.hpp>
#include <ranges>
#include <utility>


namespace sak::ranges::views {


//	-----------------------------
__using( ::std::, ptrdiff_t )
__using( ::std::views::
	,iota
	,zip
)
__using( ::std::ranges::
	,viewable_range
	,range_adaptor_closure
)
//	-----------------------------


struct __enumerate : range_adaptor_closure< __enumerate >
{
	template< viewable_range t_range >
	constexpr auto operator ( ) ( t_range&& range, const ptrdiff_t start_index = 0 ) const
	{
		return	zip( iota( start_index ), ::std::forward< t_range >( range ) );
	}
};


inline constexpr auto enumerate = __enumerate{ }; 


}


#endif


