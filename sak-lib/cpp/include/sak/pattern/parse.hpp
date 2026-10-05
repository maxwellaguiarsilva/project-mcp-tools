//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_138312140
#define header_guard_138312140


#include <sak/ranges/to.hpp>
#include <sak/ranges/transform.hpp>
#include <charconv>
#include <string_view>
#include <system_error>


namespace sak {
namespace pattern {


__using( ::std::
	,convertible_to
	,errc
	,from_chars
	,same_as
	,string_view
)
__using( ::std::ranges::
	,contiguous_range
	,input_range
	,range_adaptor_closure
	,range_reference_t
	,range_value_t
	,sized_range
	,viewable_range
)
__using( ::sak::math::, is_number )
__using( ::sak::ranges::, lazy_transform, to )


//	the whole source is one contiguous, sized text buffer a string_view can view
template< typename t_range >
concept __text_range = contiguous_range< t_range > and sized_range< t_range > and same_as< range_value_t< t_range >, char >;

//	each element of the source is viewable as text
template< typename t_range >
concept __text_list = input_range< t_range > and convertible_to< range_reference_t< t_range >, string_view >;


//	single conversion primitive shared by every overload
template< is_number t_number >
constexpr auto __parse_scalar( const string_view text, const t_number default_value ) noexcept -> t_number
{
	t_number result{ default_value };
	const auto [ pointer, error ] = from_chars( text.data( ), text.data( ) + text.size( ), result );
	return	pointer == text.data( ) + text.size( ) and error == errc{ } ? result : default_value;
}


//	absent fallback yields the zero value, present fallback is converted to the target
struct __parse_default_absent
{
	template< is_number t_number >
	constexpr auto operator ( ) ( ) const noexcept -> t_number { return t_number{ }; }
};

template< is_number t_number >
struct __parse_default_present
{
	t_number m_value;

	template< is_number t_target >
	constexpr auto operator ( ) ( ) const noexcept -> t_target { return m_value; }
};


//	element parser bound to one fallback value
template< is_number t_number >
struct __parse_element
{
	t_number m_default;

	constexpr auto operator ( ) ( const string_view text ) const noexcept -> t_number
	{ return __parse_scalar( text, m_default ); }
};


//	deferred conversion: scalar when the target is a number, container when the target is a range of numbers
template< viewable_range t_range, typename t_default >
struct __parse_proxy
{
	t_range m_range;
	t_default m_default;

	template< is_number t_number >
		requires( __text_range< t_range > )
	constexpr operator t_number ( ) &&
	{ return __parse_scalar( string_view{ m_range }, m_default.template operator ( )< t_number >( ) ); }

	template< input_range t_target >
		requires( is_number< range_value_t< t_target > > and __text_list< t_range > )
	constexpr operator t_target ( ) &&
	{
		using	t_number	=	range_value_t< t_target >;
		return	lazy_transform( ::std::forward< t_range >( m_range ), __parse_element< t_number >{ m_default.template operator ( )< t_number >( ) } ) | to;
	}
};


//	a bound fallback turns any text into a closure carrying that fallback
template< is_number t_number >
struct __parse_with_default : range_adaptor_closure< __parse_with_default< t_number > >
{
	t_number m_default;

	template< viewable_range t_range >
	constexpr auto operator ( ) ( t_range&& range ) const noexcept -> __parse_proxy< t_range, __parse_default_present< t_number > >
	{ return { ::std::forward< t_range >( range ), m_default }; }
};


struct __parse : range_adaptor_closure< __parse >
{
	template< is_number t_number >
	constexpr auto operator ( ) ( const string_view text, const t_number default_value ) const noexcept -> t_number
	{ return __parse_scalar( text, default_value ); }

	template< is_number t_number >
	constexpr auto operator ( ) ( const t_number default_value ) const noexcept -> __parse_with_default< t_number >
	{ return { { }, default_value }; }

	template< viewable_range t_range >
	constexpr auto operator ( ) ( t_range&& range ) const noexcept -> __parse_proxy< t_range, __parse_default_absent >
	{ return { ::std::forward< t_range >( range ) }; }
};
inline constexpr auto parse = __parse{ };


} } 


#endif


