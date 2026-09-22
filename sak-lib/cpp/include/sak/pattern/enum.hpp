//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_849431501
#define header_guard_849431501


#include <sak/sak.hpp>


namespace sak {
namespace pattern {


template< typename t_array, typename t_enum >
	requires ::std::is_enum_v< t_enum >
constexpr auto operator|( t_array&& array, const t_enum index ) noexcept -> decltype( auto )
{
	return	array[ static_cast< ::std::underlying_type_t< t_enum > >( index ) ];
}


} } 


#endif


