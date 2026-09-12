//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_364786088
#define header_guard_364786088


#include <format>
#include <string>
#include <string_view>
#include <sak/sak.hpp>


namespace sak {


__using( ::std::
	,format_args
	,make_format_args
	,string
	,string_view
	,vformat
)


struct __format
{
	template< typename... t_arguments >
	auto operator ( ) ( const string_view format_pattern, const t_arguments&... format_arguments ) const -> string
	{
		return	vformat( format_pattern, make_format_args( format_arguments... ) );
	}
};
inline constexpr auto format = __format{ };


} 


#endif


