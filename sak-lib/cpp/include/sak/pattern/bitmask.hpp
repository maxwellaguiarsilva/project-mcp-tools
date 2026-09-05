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
#include <sak/math/math.hpp>
#include <sak/pattern/cast.hpp>
#include <sak/ranges/transform.hpp>
#include <algorithm>
#include <concepts>
#include <functional>
#include <initializer_list>
#include <ranges>
#include <type_traits>


namespace sak {
namespace pattern {


__using( ::std::
	,bind_front
	,initializer_list
	,is_enum_v
	,same_as
	,underlying_type_t
)
__using( ::sak::math::, bit_and, bit_not, bit_or, bit_xor )
__using( ::std::ranges::
	,all_of
	,any_of
	,fold_left
	,for_each
)
__using( ::sak::ranges::, lazy_transform )


template< typename t_enum >
	requires ( is_enum_v< t_enum > )
class bitmask
{
public:
	using	enum_type		=	t_enum;
	using	underlying_type	=	underlying_type_t< t_enum >;

	constexpr bitmask( ) noexcept = default;

	template< same_as< t_enum >... t_flags >
	constexpr explicit bitmask( const t_flags... flags ) noexcept { use( flags... ); }
	constexpr explicit bitmask( const initializer_list< t_enum > flags ) noexcept { use( flags ); }

	template< same_as< t_enum >... t_flags >
	constexpr auto use( const t_flags... flags ) noexcept -> void { use( initializer_list{ flags... } ); }
	constexpr auto use( const initializer_list< t_enum > flags ) noexcept -> void
	{
		m_value = fold_left( flags | cast< underlying_type >, m_value, bit_or );
	}

	template< same_as< t_enum >... t_flags >
	constexpr auto remove( const t_flags... flags ) noexcept -> void { remove( initializer_list{ flags... } ); }
	constexpr auto remove( const initializer_list< t_enum > flags ) noexcept -> void
	{
		m_value = fold_left( flags | cast< underlying_type > | lazy_transform( bit_not ), m_value, bit_and );
	}

	template< same_as< t_enum >... t_flags >
	constexpr auto toggle( const t_flags... flags ) noexcept -> void { toggle( initializer_list{ flags... } ); }
	constexpr auto toggle( const initializer_list< t_enum > flags ) noexcept -> void
	{
		m_value = fold_left( flags | cast< underlying_type >, m_value, bit_xor );
	}

	template< same_as< t_enum >... t_flags >
	constexpr auto all( const t_flags... flags ) const noexcept -> bool { return all( initializer_list{ flags... } ); }
	constexpr auto all( const initializer_list< t_enum > flags ) const noexcept -> bool
	{
		return	all_of( flags, bind_front( &bitmask::is_set, this ) );
	}

	template< same_as< t_enum >... t_flags >
	constexpr auto any( const t_flags... flags ) const noexcept -> bool { return any( initializer_list{ flags... } ); }
	constexpr auto any( const initializer_list< t_enum > flags ) const noexcept -> bool
	{
		return	any_of( flags, bind_front( &bitmask::is_set, this ) );
	}

	constexpr auto clear( ) noexcept -> void { m_value = 0; }

private:
	constexpr auto is_set( const t_enum flag ) const noexcept -> bool
	{
		return	( m_value & cast< underlying_type >( flag ) ) == cast< underlying_type >( flag );
	}

	underlying_type	m_value{ 0 };
};


} } 


#endif


