//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_352612026
#define header_guard_352612026


#include <sak/geometry/concepts.hpp>
#include <sak/math/vector.hpp>
#include <sak/pattern/tupled.hpp>
#include <sak/ranges/to.hpp>
#include <utility>


namespace sak {


//	--------------------------------------------------
using	::sak::pattern::tupled;
__using( ::sak::, is_callable )
//	--------------------------------------------------
__using( ::std::
	,array
	,convertible_to
	,from_range
	,from_range_t
	,size_t
	,views::take
)
__using( ::std::ranges::
	,copy
	,input_range
	,viewable_range
)
__using( ::sak::math::
	,multiplies
	,equal_to
	,less_equal
	,greater_equal
	,is_arithmetic
	,fold_left
	,length
	,operator*
	,operator+
	,operator-
	,operator/
	,operator%
	,operator*=
	,operator+=
	,operator-=
	,operator/=
	,operator%=
)
//	--------------------------------------------------


template< is_arithmetic t_scalar, size_t num_dimensions >
requires	( num_dimensions > 1 )
class point;


template< is_arithmetic t_scalar = int, size_t num_dimensions = 2 >
requires	( num_dimensions > 1 )
class point final : private array< t_scalar, num_dimensions >
{
public:
	using	super_type	= 	array< t_scalar, num_dimensions >; 

	__using( typename super_type::
		,value_type
		,size_type
		,difference_type
		,pointer
		,const_pointer
		,reference
		,const_reference
		,iterator
		,const_iterator
	)

	__using( super_type::
		,begin
		,end
		,size
		,operator [ ]
		,data
		,fill
	)

	constexpr point( ) : super_type{ } { }

	template< typename... t_args >
		requires( sizeof...( t_args ) == num_dimensions
			and ( convertible_to< t_args, t_scalar > and ... )
		)
	constexpr point( t_args... args )
		: super_type{ static_cast< t_scalar >( args )... }
	{ }

	template< input_range t_range, typename t_first, typename... t_remaining >
		requires( sizeof...( t_remaining ) + 1 < num_dimensions
			and convertible_to< t_first, t_scalar >
			and ( convertible_to< t_remaining, t_scalar > and ... )
		)
	constexpr point( t_range&& range, t_first first, t_remaining... remaining )
		: super_type{ }
	{
		using	::std::ranges::copy;
		using	::std::views::take;
		constexpr auto remaining_count = sizeof...( t_remaining ) + 1;
		constexpr auto prefix_size = num_dimensions - remaining_count;
		copy( ::std::forward< t_range >( range ) | take( prefix_size ), super_type::begin( ) );
		size_t index = prefix_size;
		( *this )[ index ] = static_cast< t_scalar >( first );
		++index;
		( ( ( *this )[ index ] = static_cast< t_scalar >( remaining ), ++index ), ... );
	}

	template< is_arithmetic t_other_scalar, size_t other_dimensions >
		requires( other_dimensions not_eq num_dimensions )
	constexpr explicit point( const point< t_other_scalar, other_dimensions >& other )
		: super_type{ }
	{
		using	::std::ranges::copy;
		copy( other | take( num_dimensions ), super_type::begin( ) );
	}

	template< input_range t_range >
	constexpr point( from_range_t, t_range&& range )
		: super_type{ }
	{
		copy( ::std::forward< t_range >( range ), super_type::begin( ) );
	}

	template< is_callable< t_scalar, t_scalar > t_operation >
	constexpr auto is_all( const point& other, const t_operation& operation ) const noexcept -> bool
	{
		using	::std::views::zip;
		using	::std::ranges::all_of;
		return	all_of( zip( *this, other ), tupled( operation ) );
	}
	constexpr auto operator == ( const point& other ) const noexcept -> bool { return is_all( other, equal_to ); }
	constexpr auto is_inside( const point& other ) const noexcept -> bool { return is_all( other, less_equal ); }
	constexpr auto length( ) const noexcept -> t_scalar { return ::sak::math::length( *this ); }
	constexpr auto product( ) const noexcept -> t_scalar { return fold_left( *this, 1, multiplies ); }

};


}


namespace sak::ranges {


template< typename t_scalar, size_t num_dimensions >
struct __to_impl< point< t_scalar, num_dimensions > >
{
	template< viewable_range t_range >
	static constexpr auto apply( t_range&& range )
	{
		return	point< t_scalar, num_dimensions >( from_range, ::std::forward< t_range >( range ) );
	}
};


}


#endif


