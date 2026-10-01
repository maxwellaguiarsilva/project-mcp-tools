//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_230023971
#define header_guard_230023971


#include <concepts>
#include <cstddef>
#include <functional>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <sak/using.hpp>


namespace sak {
namespace ranges {


__using( ::std::
	,assignable_from
	,convertible_to
	,copy_constructible
	,indirectly_readable
	,invocable
	,invoke_result_t
	,iter_reference_t
	,remove_cvref_t
)
__using( ::std::ranges::
	,input_range
	,view
)

//	element-wise operators for containers (non-view ranges): eager result
template< typename t_range >
concept is_view	=	view< remove_cvref_t< t_range > >;

template< typename t_container >
concept is_resizable	=	requires( t_container& container ) { container.resize( size_t{ 0 } ); };

template< typename t_container >
concept is_string_like	=	requires { typename remove_cvref_t< t_container >::traits_type; };

//	container: an input range that is neither a view nor string-like
template< typename t_container >
concept is_container =
		input_range< t_container >
	and	not is_view< t_container >
	and	not is_string_like< t_container >;

//	at least one of the passed ranges is a view
template< typename... t_ranges >
concept any_is_view = ( is_view< t_ranges > or ... );

template< class t_function, class t_type, class t_iterator >
concept is_indirectly_binary_left_foldable	=
		copy_constructible< t_function >
	and	indirectly_readable< t_iterator >
	and	invocable< t_function&, t_type, iter_reference_t< t_iterator > >
	and	assignable_from< t_type&, invoke_result_t< t_function&, t_type, iter_reference_t< t_iterator > > >
	and	convertible_to< invoke_result_t< t_function&, t_type, iter_reference_t< t_iterator > >, t_type >;


} }


#endif


