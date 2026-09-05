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
//	File:   include/sak/pattern/bitmask.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-09-05 11:59:08
//	


#pragma once
#ifndef header_guard_119678595
#define header_guard_119678595


#include <sak/sak.hpp>
#include <algorithm>
#include <concepts>
#include <initializer_list>
#include <type_traits>


namespace sak {
namespace pattern {


__using( ::std::
	,initializer_list
	,is_enum_v
	,same_as
	,underlying_type_t
)
__using( ::std::ranges::
	,all_of
	,any_of
	,for_each
)


template< typename t_enum >
	requires ( is_enum_v< t_enum > )
class bitmask
{
public:
	using	enum_type		=	t_enum;
	using	underlying_type	=	underlying_type_t< t_enum >;

	constexpr bitmask( ) noexcept = default;

	template< same_as< t_enum >... t_flags >
	constexpr explicit bitmask( const t_enum first_flag, const t_flags... other_flags ) noexcept
	{
		use( first_flag, other_flags... );
	}

	constexpr explicit bitmask( const initializer_list< t_enum > flags ) noexcept
	{
		use( flags );
	}

	template< same_as< t_enum >... t_flags >
	constexpr auto use( const t_enum first_flag, const t_flags... other_flags ) noexcept -> void
	{
		m_value |= to_underlying( first_flag );
		( ( m_value |= to_underlying( other_flags ) ), ... );
	}

	constexpr auto use( const initializer_list< t_enum > flags ) noexcept -> void
	{
		for_each( flags, [ this ]( const auto flag ) { m_value |= to_underlying( flag ); } );
	}

	template< same_as< t_enum >... t_flags >
	constexpr auto remove( const t_enum first_flag, const t_flags... other_flags ) noexcept -> void
	{
		m_value &= static_cast< underlying_type >( ~to_underlying( first_flag ) );
		( ( m_value &= static_cast< underlying_type >( ~to_underlying( other_flags ) ) ), ... );
	}

	constexpr auto remove( const initializer_list< t_enum > flags ) noexcept -> void
	{
		for_each( flags, [ this ]( const auto flag ) { m_value &= static_cast< underlying_type >( ~to_underlying( flag ) ); } );
	}

	template< same_as< t_enum >... t_flags >
	constexpr auto toggle( const t_enum first_flag, const t_flags... other_flags ) noexcept -> void
	{
		m_value ^= to_underlying( first_flag );
		( ( m_value ^= to_underlying( other_flags ) ), ... );
	}

	constexpr auto toggle( const initializer_list< t_enum > flags ) noexcept -> void
	{
		for_each( flags, [ this ]( const auto flag ) { m_value ^= to_underlying( flag ); } );
	}

	template< same_as< t_enum >... t_flags >
	constexpr auto all( const t_enum first_flag, const t_flags... other_flags ) const noexcept -> bool
	{
		return	is_set( first_flag ) and ( is_set( other_flags ) and ... );
	}

	constexpr auto all( const initializer_list< t_enum > flags ) const noexcept -> bool
	{
		return	all_of( flags, [ this ]( const auto flag ) { return is_set( flag ); } );
	}

	template< same_as< t_enum >... t_flags >
	constexpr auto any( const t_enum first_flag, const t_flags... other_flags ) const noexcept -> bool
	{
		return	is_set( first_flag ) or ( is_set( other_flags ) or ... );
	}

	constexpr auto any( const initializer_list< t_enum > flags ) const noexcept -> bool
	{
		return	any_of( flags, [ this ]( const auto flag ) { return is_set( flag ); } );
	}

	constexpr auto clear( ) noexcept -> void
	{
		m_value = 0;
	}

private:
	static constexpr auto to_underlying( const t_enum flag ) noexcept -> underlying_type
	{
		return	static_cast< underlying_type >( flag );
	}

	constexpr auto is_set( const t_enum flag ) const noexcept -> bool
	{
		return	( m_value & to_underlying( flag ) ) == to_underlying( flag );
	}

	underlying_type	m_value{ 0 };
};


} } 


#endif


