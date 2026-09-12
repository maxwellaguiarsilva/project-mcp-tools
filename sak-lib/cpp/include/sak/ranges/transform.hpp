//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_908765432
#define header_guard_908765432


#include <ranges>


namespace sak::ranges {


//	eager_transform: eager algorithm ( std::ranges::transform ), writes to an output range
//	lazy_transform: lazy view adaptor ( std::views::transform ), produces a view
inline constexpr auto eager_transform	=	::std::ranges::transform;
inline constexpr auto lazy_transform	=	::std::views::transform;


}


#endif


