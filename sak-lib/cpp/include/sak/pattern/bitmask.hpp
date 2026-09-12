//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
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


#define __119678595_mutator( a_name, a_pipeline, a_op ) \
	template< same_as< t_enum >... t_flags > \
	constexpr auto a_name( const t_flags... flags ) noexcept -> void { a_name( initializer_list{ flags... } ); } \
	constexpr auto a_name( const initializer_list< t_enum > flags ) noexcept -> void \
	{ \
		m_value = fold_left( flags | a_pipeline, m_value, a_op ); \
	}

#define __119678595_predicate( a_name, a_algorithm ) \
	template< same_as< t_enum >... t_flags > \
	constexpr auto a_name( const t_flags... flags ) const noexcept -> bool { return a_name( initializer_list{ flags... } ); } \
	constexpr auto a_name( const initializer_list< t_enum > flags ) const noexcept -> bool \
	{ \
		return	a_algorithm( flags, bind_front( &bitmask::is_set, this ) ); \
	}


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

	__119678595_mutator( use, cast< underlying_type >, bit_or )
	__119678595_mutator( remove, cast< underlying_type > | lazy_transform( bit_not ), bit_and )
	__119678595_mutator( toggle, cast< underlying_type >, bit_xor )

	__119678595_predicate( all, all_of )
	__119678595_predicate( any, any_of )

	constexpr auto clear( ) noexcept -> void { m_value = 0; }

	constexpr auto value( ) const noexcept -> underlying_type { return m_value; }
	constexpr operator underlying_type( ) const noexcept { return m_value; }

private:
	constexpr auto is_set( const t_enum flag ) const noexcept -> bool
	{
		return	( m_value & cast< underlying_type >( flag ) ) == cast< underlying_type >( flag );
	}

	underlying_type	m_value{ 0 };
};


#undef __119678595_mutator
#undef __119678595_predicate


} } 


#endif


