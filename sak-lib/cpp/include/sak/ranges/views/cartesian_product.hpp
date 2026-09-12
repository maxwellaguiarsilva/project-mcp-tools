//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
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


