//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
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


