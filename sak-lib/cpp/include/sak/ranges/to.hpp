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
//	File:   include/sak/ranges/to.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-08-03 10:00:00
//	


#pragma once
#ifndef header_guard_642017385
#define header_guard_642017385


#include <ranges>
#include <array>
#include <algorithm>
#include <sak/using.hpp>


namespace sak::ranges {


//	--------------------------------------------------
__using( ::std::
	,array
	,size_t
)
__using( ::std::ranges::
	,viewable_range
	,copy
	,range_adaptor_closure
)
//	--------------------------------------------------


//	universal materializer: range bitor to -> proxy that converts to any target
//	the general path delegates to std::ranges::to< t_target >
//	std::array has no from_range_t constructor, so it gets a direct copy specialization
template< typename t_target > struct __to_impl;

template< typename t_target >
struct __to_impl
{
	template< viewable_range t_range >
	static constexpr auto apply( t_range&& range )
	{
		return	::std::ranges::to< t_target >( ::std::forward< t_range >( range ) );
	}
};

template< typename t_value, size_t t_size >
struct __to_impl< array< t_value, t_size > >
{
	template< viewable_range t_range >
	static constexpr auto apply( t_range&& range )
	{
		array< t_value, t_size > result;
		copy( ::std::forward< t_range >( range ), result.begin( ) );
		return	result;
	}
};


//	proxy holds the range and converts on assignment to a strong type
//	auto deduction is intentionally rejected so the target type is always explicit
template< viewable_range t_range >
struct __to_proxy
{
	t_range m_range;

	template< typename t_target >
	constexpr operator t_target( ) &&
	{
		return	__to_impl< t_target >::apply( ::std::forward< t_range >( m_range ) );
	}
};


struct __to_closure : range_adaptor_closure< __to_closure >
{
	template< viewable_range t_range >
	constexpr auto operator ( ) ( t_range&& range ) const
	{
		return	__to_proxy< t_range >{ ::std::forward< t_range >( range ) };
	}
};


inline constexpr auto to = __to_closure{ };


}


#endif


