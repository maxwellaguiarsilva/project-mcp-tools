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
//	File:   include/sak/ranges/transform.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-08-03 22:00:00
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


