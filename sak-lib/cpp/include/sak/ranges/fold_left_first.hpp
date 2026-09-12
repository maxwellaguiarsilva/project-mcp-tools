//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_197609099
#define header_guard_197609099


#include <concepts>
#include <functional>
#include <iterator>
#include <optional>
#include <ranges>
#include <utility>
#include <sak/using.hpp>
#include <sak/ranges/concepts.hpp>


namespace sak {
namespace ranges {


namespace {


__using( ::std::
	,assignable_from
	,constructible_from
	,convertible_to
	,copy_constructible
	,in_place
	,indirectly_readable
	,input_iterator
	,invoke
	,invoke_result_t
	,invocable
	,iter_reference_t
	,iter_value_t
	,move
	,optional
	,ref
	,sentinel_for
)
namespace ranges = ::std::ranges;


}


struct __fold_left_first
{
	template<
		 input_iterator t_iterator
		,sentinel_for< t_iterator > t_sentinel
		,is_indirectly_binary_left_foldable< iter_value_t< t_iterator >, t_iterator > t_function
	>
	requires constructible_from< iter_value_t< t_iterator >, iter_reference_t< t_iterator > >
	constexpr auto operator ( ) ( t_iterator first, t_sentinel last, t_function function ) const
	{
		using	value_type = iter_value_t< t_iterator >;

		if( first == last )
			return	optional< value_type >( );
		
		optional< value_type > init( in_place, *first );

		for( ++first; first not_eq last; ++first )
			*init = invoke( function, ::std::move( *init ), *first );
		
		return	init;
	}

	template<
		 ranges::input_range t_range
		,is_indirectly_binary_left_foldable< ranges::range_value_t< t_range >, ranges::iterator_t< t_range > > t_function
	>
	requires constructible_from< ranges::range_value_t< t_range >, ranges::range_reference_t< t_range > >
	constexpr auto operator ( ) ( t_range&& range, t_function function ) const { return ( *this )( ranges::begin( range ), ranges::end( range ), ref( function ) ); }
};
inline constexpr auto fold_left_first = __fold_left_first{ };


} }


#endif


