//	
//	Copyright (C) 2025 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
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
//	File:   include/sak/math/math.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2025-12-23 19:05:44
//	


#pragma once
#ifndef header_guard_935812590
#define header_guard_935812590


#include <concepts>
#include <ranges>
#include <exception>
#include <functional>
#include <cmath>
#include <algorithm>


namespace sak {
namespace math {


inline constexpr auto plus			= 	::std::plus{ };
inline constexpr auto minus			= 	::std::minus{ };
inline constexpr auto multiplies	= 	::std::multiplies{ };
inline constexpr auto divides		= 	::std::divides{ };
inline constexpr auto modulus		= 	::std::modulus{ };
inline constexpr auto equal_to		= 	::std::equal_to{ };
inline constexpr auto less_equal	= 	::std::less_equal{ };
inline constexpr auto greater_equal	= 	::std::greater_equal{ };


enum class error
{
	 division_by_zero
	,arithmetic_overflow
	,sqrt_of_negative
};


struct exception : ::std::exception
{
	error m_error;
	constexpr explicit exception( error error_code ) : m_error( error_code ) { }
	
	const char* what( ) const noexcept override
	{
		switch( m_error )
		{
			case error::division_by_zero: return "math: division by zero";
			case error::arithmetic_overflow: return "math: arithmetic overflow";
			case error::sqrt_of_negative: return "math: square root of negative number";
		}
		return	"math: unknown error";
	}
};


using	::std::is_arithmetic_v;
using	::std::bind_front;
using	::std::bind_back;
template< typename t_arithmetic >
concept is_arithmetic = is_arithmetic_v< t_arithmetic >;


//	--------------------------------------------------
//	unary niebloid closures: scalar overload plus range overload (element-wise transform)
//	range bitor niebloid routes natively via range_adaptor_closure
#define __935812590_unary_niebloid( a_name, a_expr ) \
struct __##a_name : ::std::ranges::range_adaptor_closure< __##a_name > \
{ \
	constexpr auto operator ( ) ( auto a_value ) const noexcept { return a_expr; } \
	template< ::std::ranges::input_range t_range > \
	constexpr auto operator ( ) ( t_range&& range ) const \
	{ \
		return	::std::views::transform( ::std::forward< t_range >( range ), *this ); \
	} \
}; \
inline constexpr auto a_name = __##a_name{ };


//	binary/ternary niebloid closures: chunk pattern (adaptor object plus nested closure)
//	range bitor name( bound... ) routes natively via the nested range_adaptor_closure
#define __935812590_binary( a_name, a_expr ) \
struct __##a_name \
{ \
	template< typename t_bound > \
	struct closure : ::std::ranges::range_adaptor_closure< closure< t_bound > > \
	{ \
		t_bound m_bound; \
		constexpr explicit closure( t_bound bound ) : m_bound( bound ) { } \
		template< ::std::ranges::viewable_range t_range > \
		constexpr auto operator ( ) ( t_range&& range ) const \
		{ \
			return	::std::views::transform( ::std::forward< t_range >( range ), \
				[ this ] ( auto&& a_value ) { return __##a_name{ }( ::std::forward< decltype( a_value ) >( a_value ), m_bound ); } ); \
		} \
	}; \
	template< typename t_left, typename t_right > \
	constexpr auto operator ( ) ( t_left a_left, t_right a_right ) const noexcept { return a_expr; } \
	template< ::std::ranges::viewable_range t_range, typename t_right > \
	constexpr auto operator ( ) ( t_range&& range, t_right a_right ) const \
	{ \
		return	::std::views::transform( ::std::forward< t_range >( range ), \
			[ a_right ] ( auto&& a_value ) { return __##a_name{ }( ::std::forward< decltype( a_value ) >( a_value ), a_right ); } ); \
	} \
	template< typename t_bound > \
	constexpr auto operator ( ) ( t_bound a_bound ) const { return closure< t_bound >{ a_bound }; } \
}; \
inline constexpr auto a_name = __##a_name{ };


//	negate: unary closure promoted from std::negate so it pipes natively
__935812590_unary_niebloid( negate, -a_value )


struct __between
{
	template< typename t_start, typename t_end >
	struct closure : ::std::ranges::range_adaptor_closure< closure< t_start, t_end > >
	{
		t_start m_start;
		t_end m_end;
		constexpr closure( t_start start, t_end end ) : m_start( start ), m_end( end ) { }
		template< ::std::ranges::viewable_range t_range >
		constexpr auto operator ( ) ( t_range&& range ) const
		{
			return	::std::views::transform( ::std::forward< t_range >( range ),
				[ this ] ( auto&& a_value ) { return __between{ }( ::std::forward< decltype( a_value ) >( a_value ), m_start, m_end ); } );
		}
	};
	template< typename t_value, typename t_start, typename t_end >
	constexpr auto operator ( ) ( t_value value, t_start start, t_end end ) const noexcept -> bool
	{
		return	value >= start and value <= end;
	}
	template< ::std::ranges::viewable_range t_range, typename t_start, typename t_end >
	constexpr auto operator ( ) ( t_range&& range, t_start start, t_end end ) const
	{
		return	::std::views::transform( ::std::forward< t_range >( range ),
			[ start, end ] ( auto&& a_value ) { return __between{ }( ::std::forward< decltype( a_value ) >( a_value ), start, end ); } );
	}
	template< typename t_start, typename t_end >
	constexpr auto operator ( ) ( t_start start, t_end end ) const { return closure< t_start, t_end >{ start, end }; }
};
inline constexpr auto between = __between{ };


using	::std::ranges::fold_left;
struct __sum
{
	constexpr auto operator ( ) ( const auto& list ) const noexcept { return fold_left( list, 0, plus ); }
};
inline constexpr auto sum = __sum{ };


struct __dot
{
	constexpr auto operator ( ) ( const auto& value ) const noexcept { return sum( value * value ); }
	constexpr auto operator ( ) ( const auto& left, const auto& right ) const noexcept { return sum( left * right ); }
};
inline constexpr auto dot = __dot{ };


__935812590_unary_niebloid( square, a_value * a_value )
__935812590_unary_niebloid( square_root, ::std::sqrt( a_value ) )


struct __min
{
	constexpr auto operator ( ) ( auto left, auto right ) const noexcept { return left < right ? left : right; }
	constexpr auto operator ( ) ( auto&& range ) const noexcept { return ::std::ranges::min( range ); }
};
inline constexpr auto min = __min{ };


struct __max
{
	constexpr auto operator ( ) ( auto left, auto right ) const noexcept { return left > right ? left : right; }
	constexpr auto operator ( ) ( auto&& range ) const noexcept { return ::std::ranges::max( range ); }
};
inline constexpr auto max = __max{ };


struct __clamp
{
	template< typename t_low, typename t_high >
	struct closure : ::std::ranges::range_adaptor_closure< closure< t_low, t_high > >
	{
		t_low m_low;
		t_high m_high;
		constexpr closure( t_low low, t_high high ) : m_low( low ), m_high( high ) { }
		template< ::std::ranges::viewable_range t_range >
		constexpr auto operator ( ) ( t_range&& range ) const
		{
			return	::std::views::transform( ::std::forward< t_range >( range ),
				[ this ] ( auto&& a_value ) { return __clamp{ }( ::std::forward< decltype( a_value ) >( a_value ), m_low, m_high ); } );
		}
	};
	template< typename t_value, typename t_low, typename t_high >
	constexpr auto operator ( ) ( t_value value, t_low low, t_high high ) const noexcept
	{
		return	min( max( value, low ), high );
	}
	template< ::std::ranges::viewable_range t_range, typename t_low, typename t_high >
	constexpr auto operator ( ) ( t_range&& range, t_low low, t_high high ) const
	{
		return	::std::views::transform( ::std::forward< t_range >( range ),
			[ low, high ] ( auto&& a_value ) { return __clamp{ }( ::std::forward< decltype( a_value ) >( a_value ), low, high ); } );
	}
	template< typename t_low, typename t_high >
	constexpr auto operator ( ) ( t_low low, t_high high ) const { return closure< t_low, t_high >{ low, high }; }
};
inline constexpr auto clamp = __clamp{ };


__935812590_binary( is_multiple, modulus( a_left, a_right ) == 0 )


__935812590_unary_niebloid( is_even, is_multiple( a_value, 2 ) )
__935812590_unary_niebloid( is_odd,  not is_even( a_value ) )
__935812590_unary_niebloid( abs,     a_value < 0 ? -a_value : a_value )
__935812590_unary_niebloid( sign,    ( a_value > 0 ) - ( a_value < 0 ) )


#define __935812590_unary( a_name ) \
struct __##a_name : ::std::ranges::range_adaptor_closure< __##a_name > \
{ \
	constexpr auto operator ( ) ( auto a_value ) const noexcept { return ::std::a_name( a_value ); } \
	template< ::std::ranges::input_range t_range > \
	constexpr auto operator ( ) ( t_range&& range ) const \
	{ \
		return	::std::views::transform( ::std::forward< t_range >( range ), *this ); \
	} \
}; \
inline constexpr auto a_name = __##a_name{ };


__935812590_unary( acos )
__935812590_unary( asin )
__935812590_unary( atan )
__935812590_unary( ceil )
__935812590_unary( cos )
__935812590_unary( exp )
__935812590_unary( floor )
__935812590_unary( log )
__935812590_unary( log2 )
__935812590_unary( round )
__935812590_unary( sin )
__935812590_unary( tan )
__935812590_unary( tanh )
__935812590_unary( trunc )
__935812590_binary( atan2, ::std::atan2( a_left, a_right ) )
__935812590_binary( pow, ::std::pow( a_left, a_right ) )


#undef __935812590_binary
#undef __935812590_unary
#undef __935812590_unary_niebloid


} }


#endif


