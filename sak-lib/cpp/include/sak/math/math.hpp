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


#include <cmath>
#include <algorithm>
#include <sak/math/error.hpp>
#include <sak/ranges/transform.hpp>


//	what i expect to be in this file
//	-	niebloids struct with operator() as constexpr
//	-	inline constexpr variables that define aliases for consuming these objects
//	what should not be in this file
//	-	constexpr of functions
//	-	lambdas
//	-	repeated code instead macros


namespace sak {
namespace math {


__using( ::sak::ranges::
	,lazy_transform
)

__using( ::std::
	,bind_back
)

__using( ::std::ranges::
	,range_adaptor_closure
	,input_range
	,viewable_range
	,fold_left
)


//	--------------------------------------------------
//	macros generating niebloids: scalar overload plus range overload (element-wise transform)
//	each niebloid is exposed as sak_<name> plus a descriptive alias
#define __935812590_inline_constexpr_std( a_name ) \
	inline constexpr auto a_name = ::std::a_name{ };


//	unary niebloid closures: scalar overload plus range overload (element-wise transform)
//	range bitor niebloid routes natively via range_adaptor_closure
//	a_constraint narrows the scalar element type ( is_value keeps ranges on the range overload )
#define __935812590_unary_custom( a_name, a_alias, a_expr, a_constraint ) \
struct __sak_##a_name : range_adaptor_closure< __sak_##a_name > \
{ \
	template< a_constraint t_value > \
	constexpr auto operator ( ) ( t_value a_value ) const noexcept { return a_expr; } \
	template< input_range t_range > \
	constexpr auto operator ( ) ( t_range&& range ) const \
	{ \
		return	lazy_transform( ::std::forward< t_range >( range ), *this ); \
	} \
}; \
inline constexpr auto sak_##a_name = __sak_##a_name{ }; \
inline constexpr auto a_alias = __sak_##a_name{ };


#define __935812590_unary_std( a_name, a_alias ) \
	__935812590_unary_custom( a_name, a_alias, ::std::a_name( a_value ), is_arithmetic )


//	n-ary niebloid closures: chunk pattern (adaptor object plus nested closure)
//	range bitor name( bound... ) routes natively via the nested range_adaptor_closure
//	the a_scalar_overload chunk carries the scalar call: expression first, parameters after
//	each parameter is a constrained placeholder ( is_value keeps ranges on the range overload )
//	a_arity is the scalar parameter count; the range overload binds a_arity - 1 trailing values
#define __935812590_scalar_overload( a_expression, ... ) \
	constexpr auto operator ( ) ( __VA_ARGS__ ) const noexcept { return a_expression; }


#define __935812590_nary_custom( a_name, a_alias, a_arity, a_scalar_overload ) \
struct __sak_##a_name \
{ \
	struct __sak_bind_back_factory \
	{ \
		template< typename... t_bound > \
		constexpr auto operator ( ) ( t_bound&&... bound ) const \
		{ \
			return	bind_back( __sak_##a_name{ }, ::std::forward< t_bound >( bound )... ); \
		} \
	}; \
	template< typename... t_bound > \
	struct closure : range_adaptor_closure< closure< t_bound... > > \
	{ \
		::std::tuple< t_bound... > m_bound; \
		constexpr explicit closure( t_bound... bound ) : m_bound( bound... ) { } \
		template< viewable_range t_range > \
		constexpr auto operator ( ) ( t_range&& range ) const \
		{ \
			return	lazy_transform( ::std::forward< t_range >( range ), ::std::apply( __sak_bind_back_factory{ }, m_bound ) ); \
		} \
	}; \
	__935812590_scalar_overload a_scalar_overload; \
	template< viewable_range t_range, typename... t_bound > \
		requires ( sizeof...( t_bound ) == a_arity - 1 ) \
	constexpr auto operator ( ) ( t_range&& range, t_bound... bound ) const \
	{ \
		return	lazy_transform( ::std::forward< t_range >( range ), bind_back( __sak_##a_name{ }, bound... ) ); \
	} \
	template< typename... t_bound > \
		requires ( sizeof...( t_bound ) == a_arity - 1 ) \
	constexpr auto operator ( ) ( t_bound... bound ) const \
	{ \
		return	closure< t_bound... >{ bound... }; \
	} \
}; \
inline constexpr auto sak_##a_name = __sak_##a_name{ }; \
inline constexpr auto a_alias = __sak_##a_name{ };


#define __935812590_binary_std( a_name, a_alias ) \
	__935812590_nary_custom( a_name, a_alias, 2, \
		( ( ::std::a_name( first, second ) ), is_arithmetic auto first, is_arithmetic auto second ) )


//	binary_range_scalar: binary overload plus range overload routed to ::std::ranges
#define __935812590_binary_range_scalar( a_name, a_alias, a_expr ) \
struct __sak_##a_name \
{ \
	constexpr auto operator ( ) ( auto first, auto second ) const noexcept { return a_expr; } \
	constexpr auto operator ( ) ( const auto& range ) const noexcept { return ::std::ranges::a_name( range ); } \
}; \
inline constexpr auto sak_##a_name = __sak_##a_name{ }; \
inline constexpr auto a_alias = __sak_##a_name{ };


//	--------------------------------------------------
//	basic arithmetic and comparison operators re-exported from std
__use_macro( __935812590_inline_constexpr_std
	,(	plus			)
	,(	minus			)
	,(	multiplies		)
	,(	divides			)
	,(	modulus			)
	,(	equal_to		)
	,(	less_equal		)
	,(	greater_equal	)
)


//	--------------------------------------------------
//	binary functions: two scalar operands, or a range transformed element-wise
__935812590_binary_std(	atan2	,arctangent2	)
__935812590_nary_custom( is_multiple, is_multiple, 2, ( ( modulus( first, second ) == 0 ), is_integral auto first, is_integral auto second ) )
__935812590_nary_custom( pow, power, 2, ( ( ::std::pow( first, second ) ), is_arithmetic auto first, is_arithmetic auto second ) )


//	--------------------------------------------------
//	unary functions: one scalar operand, or a range transformed element-wise
//	unary predicates depend on is_multiple, so the binary block sits above
__use_macro( __935812590_unary_custom
	,(	abs			,absolute		,a_value < 0 ? -a_value : a_value	,is_value			)
	,(	negate		,negate			,-a_value							,is_value			)
	,(	square		,square			,a_value * a_value					,is_value			)
	,(	is_even		,is_even		,is_multiple( a_value, 2 )			,is_integral		)
	,(	is_odd		,is_odd			,not is_even( a_value )				,is_integral		)
	,(	sign		,sign			,( a_value > 0 ) - ( a_value < 0 )	,is_arithmetic		)
)


__use_macro( __935812590_unary_std
	,(	acos	,arccosine			)
	,(	asin	,arcsine			)
	,(	atan	,arctangent			)
	,(	ceil	,ceiling			)
	,(	cos		,cosine				)
	,(	exp		,exponential		)
	,(	floor	,round_down			)
	,(	log		,natural_logarithm	)
	,(	log2	,logarithm_base_two	)
	,(	round	,round_to_nearest	)
	,(	sin		,sine				)
	,(	sqrt	,square_root		)
	,(	tan		,tangent			)
	,(	tanh	,hyperbolic_tangent	)
	,(	trunc	,truncate			)
)


//	--------------------------------------------------
//	range reductions: fold a range into a single scalar
//	min and max also sit here: their range overload reduces via ::std::ranges
struct __sak_sum
{
	constexpr auto operator ( ) ( const auto& list ) const noexcept { return fold_left( list, 0, plus ); }
};
inline constexpr auto sum = __sak_sum{ };


struct __sak_dot
{
	constexpr auto operator ( ) ( const auto& value ) const noexcept { return sum( value * value ); }
	constexpr auto operator ( ) ( const auto& first, const auto& second ) const noexcept { return sum( first * second ); }
};
inline constexpr auto dot = __sak_dot{ };


__935812590_binary_range_scalar( min, min, first < second ? first : second )
__935812590_binary_range_scalar( max, max, first > second ? first : second )


//	--------------------------------------------------
//	ternary functions: three operands, or a range transformed element-wise
//	closures capture the extra operands and let a range pipe through them
__935812590_nary_custom( between, between, 3, ( ( value >= first and value <= second ), is_value auto value, auto first, auto second ) )
__935812590_nary_custom( clamp, clamp, 3, ( ( min( max( value, first ), second ) ), is_value auto value, auto first, auto second ) )


#undef __935812590_scalar_overload
#undef __935812590_nary_custom
#undef __935812590_binary_std
#undef __935812590_binary_range_scalar
#undef __935812590_inline_constexpr_std
#undef __935812590_unary_custom
#undef __935812590_unary_std


} }


#endif


