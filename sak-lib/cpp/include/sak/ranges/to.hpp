//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_642017385
#define header_guard_642017385


#include <array>
#include <algorithm>
#include <sak/sak.hpp>


namespace sak::ranges {


//	--------------------------------------------------
__using( ::std::
	,array
	,constructible_from
	,convertible_to
	,from_range_t
	,size_t
)
__using( ::std::ranges::
	,copy
	,input_range
	,range_adaptor_closure
	,range_reference_t
	,range_value_t
	,view
	,viewable_range
)
//	--------------------------------------------------


//	true when std::ranges::to can materialize a range into the target type
//	directly (constructible from the range, directly or via from_range_t, or insertable)
//	or by recursively materializing each element, mirroring std::ranges::to
template< typename t_target, typename t_range >
struct __toable;

//	in the recursive path the target is a range whose elements are not convertible
//	to its value type, so every element must itself be materializable
template< typename t_target, typename t_range, bool = input_range< t_target > >
struct __toable_element { static constexpr bool value = false; };

template< typename t_target, typename t_range >
struct __toable_element< t_target, t_range, true >
{
	static constexpr bool value =
		not convertible_to< range_reference_t< t_range >, range_value_t< t_target > >
		and	__toable< range_value_t< t_target >, range_reference_t< t_range > >::value;
};

template< typename t_target, typename t_range >
struct __toable
{
	static constexpr bool value = not view< t_target > and	(
			constructible_from< t_target, from_range_t, t_range >
		or	constructible_from< t_target, t_range >
		or	requires( t_target& target, range_reference_t< t_range > value )
			{ target.insert( target.end( ), value ); }
		or	__toable_element< t_target, t_range >::value
	);
};

template< typename t_target, typename t_range >
concept is_toable = __toable< t_target, t_range >::value;


//	universal materializer: range | to -> proxy that converts to any target
//	the general path delegates to std::ranges::to< t_target >
//	a cv-qualified destination deduces a const target, delegate to the unqualified materializer
//	std::array has no from_range_t constructor, so it gets a direct copy specialization
template< typename t_target > struct __to_impl;
template< typename t_target > struct __to_impl< const t_target > : __to_impl< t_target > { };
template< typename t_target >
struct __to_impl
{
	template< viewable_range t_range >
		requires( ( is_class< t_target > or is_union< t_target > ) and is_toable< t_target, t_range > )
	static constexpr auto apply( t_range&& range )
	{ return ::std::ranges::to< t_target >( ::std::forward< t_range >( range ) ); }
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


//	materializable targets: the generic path handles only class and union types
//	scalar or other non-class targets must provide an explicit __to_impl specialization
template< typename t_target, typename t_range >
concept materializes = requires( t_range&& range )
{ __to_impl< t_target >::apply( ::std::forward< t_range >( range ) ); };


//	proxy holds the range and converts on assignment to a strong type
//	auto deduction is intentionally rejected so the target type is always explicit
template< viewable_range t_range >
struct __to_proxy
{
	t_range m_range;

	template< typename t_target >
		requires materializes< t_target, t_range >
	constexpr operator t_target( ) &&
	{ return __to_impl< t_target >::apply( ::std::forward< t_range >( m_range ) ); }
};


struct __to_closure : range_adaptor_closure< __to_closure >
{
	template< viewable_range t_range >
	constexpr auto operator ( ) ( t_range&& range ) const noexcept
	{ return __to_proxy< t_range >{ ::std::forward< t_range >( range ) }; }
};
inline constexpr auto to = __to_closure{ };


}


#endif


