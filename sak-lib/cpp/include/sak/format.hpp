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
//	File:   include/sak/format.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-09-03 18:08:50
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


