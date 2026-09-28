//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_356592464
#define header_guard_356592464


#include <sak/geometry/line_view.hpp>
#include <sak/ranges/count_to.hpp>


namespace sak {


__using( ::sak::, is_point )
__using( ::sak::math::, cross, dot, min, max )
__using( ::sak::ranges::, count_to, lazy_transform, to )
__using( ::sak::ranges::views::, rotated )
__using( ::std::, array )
__using( ::std::ranges::, range_adaptor_closure )
__using( ::std::views::, zip )

template< is_point t_point = point< int, 2 > >
struct geometry 
{
	
	using	point		=	t_point;
	using	size		=	t_point;
	using	position	=	t_point;
	using	color		=	::sak::point< float, 4 >;
	using	scalar		=	typename t_point::value_type;

	template< typename t_value, int index >
	struct __axis : range_adaptor_closure< __axis< t_value, index > >
	{
		constexpr auto operator ( ) ( const t_value& value ) const noexcept -> typename t_value::value_type { return value[ index ]; }
	};

	static constexpr auto left		=	__axis<	position	,0	>{ };
	static constexpr auto top		=	__axis<	position	,1	>{ };
	static constexpr auto width		=	__axis<	size		,0	>{ };
	static constexpr auto height	=	__axis<	size		,1	>{ };
	static constexpr auto depth		=	__axis<	size		,2	>{ };
	static constexpr auto red		=	__axis<	color		,0	>{ };
	static constexpr auto green		=	__axis<	color		,1	>{ };
	static constexpr auto blue		=	__axis<	color		,2	>{ };
	static constexpr auto alpha		=	__axis<	color		,3	>{ };

	struct line
	{
		position start, end;
		constexpr auto size( ) const noexcept -> size { return end - start; }
	};

	struct rectangle
	{
		position start, end;
		constexpr auto size( ) const noexcept -> size { return end - start; }
		constexpr auto contains( const point& point ) const noexcept -> bool
		{
			return	start.is_inside( point ) and point.is_inside( end );
		}
		constexpr auto is_inside( const rectangle& other ) const noexcept -> bool
		{
			return	other.start.is_inside( start ) and end.is_inside( other.end );
		}
	};

	struct triangle
	{
		position first, second, third;

		constexpr auto box( ) const noexcept -> rectangle
		{
			position low = first;
			position high = first;
			for( const auto axis : count_to( first.size( ) ) )
			{
				low[ axis ] = min( low[ axis ], min( second[ axis ], third[ axis ] ) );
				high[ axis ] = max( high[ axis ], max( second[ axis ], third[ axis ] ) );
			}
			return	rectangle{ low, high };
		}

		constexpr auto contains( const point& spot ) const noexcept -> bool
		{
			const position normal = cross( second - first, third - first ) | to;
			const array vertices{ first, second, third };
			const auto dots = zip( vertices, vertices | rotated )
				| lazy_transform( [ & ]( const auto& edge )
				{
					const auto& [ current, next ] = edge;
					return	dot( cross( spot - next, current - next ), normal );
				} );
			const scalar zero{ 0 };
			return	min( dots ) >= zero or max( dots ) <= zero;
		}
	};

};


using	g2i	=	geometry< point< int, 2 > >;
using	g3i	=	geometry< point< int, 3 > >;
using	g2f	=	geometry< point< float, 2 > >;
using	g3f	=	geometry< point< float, 3 > >;


}


#endif


