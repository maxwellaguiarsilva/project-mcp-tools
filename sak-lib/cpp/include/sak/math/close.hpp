//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_1510654891
#define header_guard_1510654891


#include <sak/math/math.hpp>


namespace sak::math {


//	--------------------------------------------------
//	approximate equality: true when the two values differ by less than epsilon
//	the gap is folded through absolute so the caller needs no ordering assumption
//	--------------------------------------------------
struct __close
{
	template< typename t_value >
	constexpr auto operator ( ) ( const t_value first, const t_value second, const t_value epsilon ) const noexcept -> bool
	{ return absolute( first - second ) < epsilon; }
};

inline constexpr auto close = __close{ };


}


#endif


