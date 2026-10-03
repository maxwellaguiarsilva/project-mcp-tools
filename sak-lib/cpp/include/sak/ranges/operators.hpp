//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_781083963
#define header_guard_781083963


#include <sak/math/math.hpp>
#include <sak/ranges/to.hpp>
#include <vector>


namespace sak {
namespace ranges {


__using( ::std::, remove_cvref_t, vector )
__using( ::std::ranges::, viewable_range )
__using( ::std::views::, all, repeat, zip_transform )
__using( ::sak::, is_same_decayed )
__using( ::sak::math::
	,equal_to		,not_equal_to	,greater	,greater_equal	,less	,less_equal
	,plus			,minus			,multiplies	,divides		,modulus
	,bit_and		,bit_not		,bit_or		,bit_xor
	,decrement		,increment		,identity	,negate
	,logical_and	,logical_not	,logical_or
	,is_arithmetic	,is_integral	,is_value
	,shift_left		,shift_right
)


//	emit a complete zip overload: template header, requires, signature and body
#define __781083963_overload( a_requires, a_return, a_tail, a_template, a_operator, a_params, a_zip ) \
template< __unparenthesize a_template > \
a_requires \
constexpr auto operator a_operator ( __unparenthesize a_params ) a_return \
{ return zip_transform( __unparenthesize a_zip ) a_tail; }


//	element-wise operators, containers materialize and views stay lazy, one shared body
//	scalar concept, materialization and forwarding are the only axes per overload
#define __781083963_binary( a_operator, a_operation, a_scalar, a_return ) \
__781083963_overload( requires( is_same_decayed< t_left, t_right > ), -> a_return, | to \
	,( is_container t_left, is_container t_right ) \
	,a_operator	,( t_left&& left, t_right&& right ) \
	,( a_operation, all( left ), all( right ) ) \
) \
__781083963_overload( ,-> a_return, | to \
	,( is_container t_left, a_scalar t_scalar ) \
	,a_operator	,( t_left&& left, t_scalar right ) \
	,( a_operation, all( left ), repeat( right ) ) \
) \
__781083963_overload( ,-> a_return, | to \
	,( is_container t_left, a_scalar t_scalar ) \
	,a_operator	,( t_scalar left, t_left&& right ) \
	,( a_operation, repeat( left ), all( right ) ) \
) \
__781083963_overload( requires( any_is_view< t_left, t_right > ),, \
	,( viewable_range t_left, viewable_range t_right ) \
	,a_operator	,( t_left&& left, t_right&& right ) \
	,( a_operation, all( ::std::forward< t_left >( left ) ), all( ::std::forward< t_right >( right ) ) ) \
) \
__781083963_overload( ,, \
	,( is_view t_left, a_scalar t_scalar ) \
	,a_operator	,( t_left&& left, t_scalar right ) \
	,( a_operation, all( ::std::forward< t_left >( left ) ), repeat( right ) ) \
) \
__781083963_overload( ,, \
	,( is_view t_left, a_scalar t_scalar ) \
	,a_operator	,( t_scalar left, t_left&& right ) \
	,( a_operation, repeat( left ), all( ::std::forward< t_left >( right ) ) ) \
)


#define __781083963_compound( a_operator, a_operation, a_scalar, ... ) \
template< is_container t_left, is_container t_right > \
requires( is_same_decayed< t_left, t_right > ) \
constexpr auto operator a_operator##= ( t_left& left, const t_right& right ) noexcept -> t_left& \
{ return eager_transform( left, right, left.begin( ), a_operation ), left; } \
template< is_container t_left, a_scalar t_scalar > \
constexpr auto operator a_operator##= ( t_left& left, t_scalar right ) noexcept -> t_left& \
{ return eager_transform( left, repeat( right ), left.begin( ), a_operation ), left; }


__use_macro_list(
	(
		 (	+	,plus		,is_arithmetic	,remove_cvref_t< t_left >	)
		,(	-	,minus		,is_arithmetic	,remove_cvref_t< t_left >	)
		,(	*	,multiplies	,is_arithmetic	,remove_cvref_t< t_left >	)
		,(	/	,divides	,is_arithmetic	,remove_cvref_t< t_left >	)
		,(	%	,modulus	,is_arithmetic	,remove_cvref_t< t_left >	)
	)
	,__781083963_binary
	,__781083963_compound
)


//	bitwise and shift scalars are narrowed to integral, container pairs stay as-is
__use_macro_list(
	(
		 (	&	,bit_and	,is_integral	,remove_cvref_t< t_left >	)
		,(	|	,bit_or		,is_integral	,remove_cvref_t< t_left >	)
		,(	^	,bit_xor	,is_integral	,remove_cvref_t< t_left >	)
		,(	<<	,shift_left	,is_integral	,remove_cvref_t< t_left >	)
		,(	>>	,shift_right	,is_integral	,remove_cvref_t< t_left >	)
	)
	,__781083963_binary
	,__781083963_compound
)


//	comparison and logical results are bool per element, so they materialize into vector of bool,
//	reusing the same container would deduce the wrong value type
__use_macro_list(
	(	//	rule-exception: local style consistency
		 (	!=	,not_equal_to	,is_value	,vector< bool >	)
		,(	==	,equal_to		,is_value	,vector< bool >	)
		,(	< 	,less			,is_value	,vector< bool >	)
		,(	<=	,less_equal		,is_value	,vector< bool >	)
		,(	> 	,greater		,is_value	,vector< bool >	)
		,(	>=	,greater_equal	,is_value	,vector< bool >	)
	)
	,__781083963_binary
)


//	element-wise logical combination, no short-circuit: every element pair is always evaluated
__use_macro_list(
	(
		 (	and	,logical_and	,is_value	,vector< bool >	)
		,(	or	,logical_or		,is_value	,vector< bool >	)
	)
	,__781083963_binary
)


#undef __781083963_overload
#undef __781083963_binary
#undef __781083963_compound


//	unary containers stay eager, views stay lazy without materialization,
//	logical negation yields bool per element, so it materializes into vector of bool
#define __781083963_unary_eager( a_operator, a_operation, a_result ) \
template< is_container t_left > \
constexpr auto operator a_operator ( const t_left& left ) -> a_result { return lazy_transform( left, a_operation ) | to; }


#define __781083963_unary_lazy( a_operator, a_operation ) \
template< is_view t_left > \
constexpr auto operator a_operator ( t_left&& left ) { return lazy_transform( all( ::std::forward< t_left >( left ) ), a_operation ); }


//	prefix and postfix act on each element, not on iterator position,
//	containers mutate in place, views stay lazy without mutation,
//	postfix views are omitted since there is nothing to snapshot before a lazy step
#define __781083963_step( a_operator, a_operation ) \
template< is_container t_container > \
constexpr auto operator a_operator ( t_container& container ) noexcept -> t_container& \
{ return eager_transform( container, container.begin( ), a_operation ), container; } \
template< is_container t_container > \
constexpr auto operator a_operator ( t_container& container, int ) -> remove_cvref_t< t_container > \
{ auto old = container; return eager_transform( container, container.begin( ), a_operation ), old; } \
template< is_view t_view > \
constexpr auto operator a_operator ( t_view&& view ) \
{ return lazy_transform( all( ::std::forward< t_view >( view ) ), a_operation ); }


__use_macro( __781083963_unary_eager
	,(	-	,negate		,remove_cvref_t< t_left >	)
	,(	+	,identity	,remove_cvref_t< t_left >	)
	,(	~	,bit_not	,remove_cvref_t< t_left >	)
	,(	not	,logical_not	,vector< bool >		)
)


__use_macro( __781083963_unary_lazy
	,(	-	,negate		)
	,(	+	,identity	)
	,(	~	,bit_not	)
	,(	not	,logical_not	)
)


__use_macro( __781083963_step
	,(	++	,increment	)
	,(	--	,decrement	)
)


#undef __781083963_unary_eager
#undef __781083963_unary_lazy
#undef __781083963_step


} }


#endif


