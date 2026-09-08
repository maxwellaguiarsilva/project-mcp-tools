//	
//	Copyright (C) 2025 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
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
//	File:   include/sak/geometry/geometry.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2025-12-23 19:05:44
//	


#pragma once
#ifndef header_guard_356592464
#define header_guard_356592464


#include <sak/geometry/line_view.hpp>
#include <ranges>


namespace sak {


__using( ::sak::
	,is_point
)
__using( ::std::ranges::
	,range_adaptor_closure
)

template< is_point t_point = point< int, 2 > >
struct geometry 
{
	
	using	point	=	t_point;
	using	size	=	t_point;
	using	position	=	t_point;
	using	scalar	=	typename t_point::value_type;

	template< typename t_value, int index >
	struct __axis : range_adaptor_closure< __axis< t_value, index > >
	{
		constexpr auto operator ( ) ( const t_value& value ) const noexcept -> scalar { return value[ index ]; }
	};

	static constexpr auto width		=	__axis< size,		0 >{ };
	static constexpr auto height	=	__axis< size,		1 >{ };
	static constexpr auto left		=	__axis< position,	0 >{ };
	static constexpr auto top		=	__axis< position,	1 >{ };

	struct line
	{
		point start, end;
		constexpr auto size( ) const noexcept -> point { return end - start; }
	};

	struct rectangle
	{
		point start, end;
		constexpr auto size( ) const noexcept -> point { return end - start; }
		constexpr auto contains( const point& point ) const noexcept -> bool
		{
			return	start.is_inside( point ) and point.is_inside( end );
		}
		constexpr auto is_inside( const rectangle& other ) const noexcept -> bool
		{
			return	other.start.is_inside( start ) and end.is_inside( other.end );
		}
	};

};


using	g2i	=	geometry< point< int, 2 > >;
using	g3i	=	geometry< point< int, 3 > >;

using	g2f	=	geometry< point< float, 2 > >;
using	g3f	=	geometry< point< float, 3 > >;


}


#endif


