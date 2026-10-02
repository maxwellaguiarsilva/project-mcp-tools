//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_830006533
#define header_guard_830006533


#include <ranges>
#include <sak/using.hpp>


namespace sak::ranges::views {


__using( ::std::ranges::
	,viewable_range
	,range_adaptor_closure
)
__using( ::std::views::
	,reverse
)


struct __reversed : range_adaptor_closure< __reversed >
{
	template< viewable_range t_range >
	constexpr auto operator ( ) ( t_range&& range ) const { return reverse( ::std::forward< t_range >( range ) ); }
};


inline constexpr auto reversed = __reversed{ };


}


#endif


