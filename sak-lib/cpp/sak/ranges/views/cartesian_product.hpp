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
//	File:   include/sak/ranges/views/cartesian_product.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-01-05 21:09:03
//	


#pragma once
#ifndef header_guard_147258369
#define header_guard_147258369


#include <sak/sak.hpp>
#include <sak/using.hpp>
#include <sak/ranges/transform.hpp>
#include <ranges>
#include <utility>


namespace sak::ranges::views {


//	-----------------------------
__using( ::std::
	,make_pair
)
__using( ::std::ranges::
	,viewable_range
	,range_adaptor_closure
)
__using( ::std::views::
	,all
	,join
)
__using( ::sak::ranges::
	,lazy_transform
)
//	-----------------------------


struct __cartesian_product
{
	template< viewable_range t_second_range >
	struct closure : range_adaptor_closure< closure< t_second_range > >
	{
		t_second_range m_second_range;
		constexpr explicit closure( t_second_range second_range ) : m_second_range( second_range ) { }
		template< viewable_range t_first_range >
		constexpr auto operator ( ) ( t_first_range&& first_range ) const
		{
			return	__cartesian_product{ }( ::std::forward< t_first_range >( first_range ), m_second_range );
		}
	};

	template< viewable_range t_first_range, viewable_range t_second_range >
	constexpr auto operator ( ) ( t_first_range&& first_range, t_second_range&& second_range ) const
	{
		auto second_view = all( ::std::forward< t_second_range >( second_range ) );
		return	all( ::std::forward< t_first_range >( first_range ) )
			bitor	lazy_transform( [ second_view ] ( auto const& first_element ) {
				return	second_view bitor lazy_transform( [ first_element ] ( auto const& second_element ) {
					return	make_pair( first_element, second_element );
				} );
			} )
			bitor	join;
	}

	template< viewable_range t_second_range >
	constexpr auto operator ( ) ( t_second_range&& second_range ) const
	{
		return	closure< t_second_range >{ ::std::forward< t_second_range >( second_range ) };
	}
};


inline constexpr auto cartesian_product = __cartesian_product{ };


}


#endif


