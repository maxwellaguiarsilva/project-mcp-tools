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


__using( ::std::
	,remove_cvref_t
	,vector
)
__using( ::std::ranges::
	,viewable_range
)
__using( ::std::views::
	,all
	,repeat
	,zip_transform
)
__using( ::sak::, is_same_decayed )
__using( ::sak::math::
	,bit_and
	,bit_not
	,bit_or
	,bit_xor
	,decrement
	,divides
	,equal_to
	,greater
	,greater_equal
	,identity
	,increment
	,is_arithmetic
	,is_integral
	,is_value
	,less
	,less_equal
	,logical_and
	,logical_not
	,logical_or
	,minus
	,modulus
	,multiplies
	,negate
	,not_equal_to
	,plus
	,shift_left
	,shift_right
)


//	element-wise operators for containers (non-view ranges): eager result
#define __781083963_eager( a_operator, a_operation ) \
template< is_container t_left, is_container t_right > \
requires( is_same_decayed< t_left, t_right > ) \
constexpr auto operator a_operator ( t_left&& left, t_right&& right ) -> remove_cvref_t< t_left > \
{ return zip_transform( a_operation, all( left ), all( right ) ) | to; } \
template< is_container t_left, is_arithmetic t_scalar > \
constexpr auto operator a_operator ( t_left&& left, t_scalar right ) -> remove_cvref_t< t_left > \
{ return zip_transform( a_operation, all( left ), repeat( right ) ) | to; } \
template< is_arithmetic t_scalar, is_container t_right > \
constexpr auto operator a_operator ( t_scalar left, t_right&& right ) -> remove_cvref_t< t_right > \
{ return zip_transform( a_operation, repeat( left ), all( right ) ) | to; }


#define __781083963_compound( a_operator, a_operation ) \
template< is_container t_left, is_container t_right > \
requires( is_same_decayed< t_left, t_right > ) \
constexpr auto operator a_operator##= ( t_left& left, const t_right& right ) noexcept -> t_left& \
{ return eager_transform( left, right, left.begin( ), a_operation ), left; } \
template< is_container t_left, is_arithmetic t_scalar > \
constexpr auto operator a_operator##= ( t_left& left, t_scalar right ) noexcept -> t_left& \
{ return eager_transform( left, repeat( right ), left.begin( ), a_operation ), left; }


//	element-wise operators for views (at least one operand is a view): lazy result
#define __781083963_lazy( a_operator, a_operation ) \
template< viewable_range t_left, viewable_range t_right > \
requires( any_is_view< t_left, t_right > ) \
constexpr auto operator a_operator ( t_left&& left, t_right&& right ) \
{ return zip_transform( a_operation, all( ::std::forward< t_left >( left ) ), all( ::std::forward< t_right >( right ) ) ); } \
template< is_view t_left, is_arithmetic t_scalar > \
constexpr auto operator a_operator ( t_left&& left, t_scalar right ) \
{ return zip_transform( a_operation, all( ::std::forward< t_left >( left ) ), repeat( right ) ); } \
template< is_arithmetic t_scalar, is_view t_right > \
constexpr auto operator a_operator ( t_scalar left, t_right&& right ) \
{ return zip_transform( a_operation, repeat( left ), all( ::std::forward< t_right >( right ) ) ); }


//	bitwise and shift scalars are narrowed to integral, container pairs stay as-is
#define __781083963_eager_integral( a_operator, a_operation ) \
template< is_container t_left, is_container t_right > \
requires( is_same_decayed< t_left, t_right > ) \
constexpr auto operator a_operator ( t_left&& left, t_right&& right ) -> remove_cvref_t< t_left > \
{ return zip_transform( a_operation, all( left ), all( right ) ) | to; } \
template< is_container t_left, is_integral t_scalar > \
constexpr auto operator a_operator ( t_left&& left, t_scalar right ) -> remove_cvref_t< t_left > \
{ return zip_transform( a_operation, all( left ), repeat( right ) ) | to; } \
template< is_integral t_scalar, is_container t_right > \
constexpr auto operator a_operator ( t_scalar left, t_right&& right ) -> remove_cvref_t< t_right > \
{ return zip_transform( a_operation, repeat( left ), all( right ) ) | to; }


#define __781083963_compound_integral( a_operator, a_operation ) \
template< is_container t_left, is_container t_right > \
requires( is_same_decayed< t_left, t_right > ) \
constexpr auto operator a_operator##= ( t_left& left, const t_right& right ) noexcept -> t_left& \
{ return eager_transform( left, right, left.begin( ), a_operation ), left; } \
template< is_container t_left, is_integral t_scalar > \
constexpr auto operator a_operator##= ( t_left& left, t_scalar right ) noexcept -> t_left& \
{ return eager_transform( left, repeat( right ), left.begin( ), a_operation ), left; }


#define __781083963_lazy_integral( a_operator, a_operation ) \
template< viewable_range t_left, viewable_range t_right > \
requires( any_is_view< t_left, t_right > ) \
constexpr auto operator a_operator ( t_left&& left, t_right&& right ) \
{ return zip_transform( a_operation, all( ::std::forward< t_left >( left ) ), all( ::std::forward< t_right >( right ) ) ); } \
template< is_view t_left, is_integral t_scalar > \
constexpr auto operator a_operator ( t_left&& left, t_scalar right ) \
{ return zip_transform( a_operation, all( ::std::forward< t_left >( left ) ), repeat( right ) ); } \
template< is_integral t_scalar, is_view t_right > \
constexpr auto operator a_operator ( t_scalar left, t_right&& right ) \
{ return zip_transform( a_operation, repeat( left ), all( ::std::forward< t_right >( right ) ) ); }


//	comparison and logical results are bool per element, so they materialize into vector of bool,
//	reusing the same container would deduce the wrong value type
#define __781083963_eager_bool( a_operator, a_operation ) \
template< is_container t_left, is_container t_right > \
requires( is_same_decayed< t_left, t_right > ) \
constexpr auto operator a_operator ( t_left&& left, t_right&& right ) -> vector< bool > \
{ return zip_transform( a_operation, all( left ), all( right ) ) | to; } \
template< is_container t_left, is_value t_scalar > \
constexpr auto operator a_operator ( t_left&& left, t_scalar right ) -> vector< bool > \
{ return zip_transform( a_operation, all( left ), repeat( right ) ) | to; } \
template< is_value t_scalar, is_container t_right > \
constexpr auto operator a_operator ( t_scalar left, t_right&& right ) -> vector< bool > \
{ return zip_transform( a_operation, repeat( left ), all( right ) ) | to; }


#define __781083963_lazy_bool( a_operator, a_operation ) \
template< viewable_range t_left, viewable_range t_right > \
requires( any_is_view< t_left, t_right > ) \
constexpr auto operator a_operator ( t_left&& left, t_right&& right ) \
{ return zip_transform( a_operation, all( ::std::forward< t_left >( left ) ), all( ::std::forward< t_right >( right ) ) ); } \
template< is_view t_left, is_value t_scalar > \
constexpr auto operator a_operator ( t_left&& left, t_scalar right ) \
{ return zip_transform( a_operation, all( ::std::forward< t_left >( left ) ), repeat( right ) ); } \
template< is_value t_scalar, is_view t_right > \
constexpr auto operator a_operator ( t_scalar left, t_right&& right ) \
{ return zip_transform( a_operation, repeat( left ), all( ::std::forward< t_right >( right ) ) ); }


#define __781083963_arithmetic( a_macro ) __use_macro( a_macro \
	,(	+	,plus		) \
	,(	-	,minus		) \
	,(	*	,multiplies	) \
	,(	/	,divides	) \
	,(	%	,modulus	) \
)


#define __781083963_integral( a_macro ) __use_macro( a_macro \
	,(	&	,bit_and	) \
	,(	|	,bit_or		) \
	,(	^	,bit_xor	) \
	,(	<<	,shift_left	) \
	,(	>>	,shift_right	) \
)


#define __781083963_comparison( a_macro ) __use_macro( a_macro \
	,(	==	,equal_to	) \
	,(	not_eq	,not_equal_to	) \
	,(	<	,less		) \
	,(	<=	,less_equal	) \
	,(	>	,greater	) \
	,(	>=	,greater_equal	) \
)


#define __781083963_logical( a_macro ) __use_macro( a_macro \
	,(	and	,logical_and	) \
	,(	or	,logical_or	) \
)


__781083963_arithmetic( __781083963_eager )
__781083963_arithmetic( __781083963_compound )
__781083963_arithmetic( __781083963_lazy )


//	pipe stays safe here: eager needs is_container and lazy needs a view on at least one side,
//	a closure on the right is not a range, so container bitor closure never matches these overloads
__781083963_integral( __781083963_eager_integral )
__781083963_integral( __781083963_compound_integral )
__781083963_integral( __781083963_lazy_integral )


__781083963_comparison( __781083963_eager_bool )
__781083963_comparison( __781083963_lazy_bool )


//	element-wise logical combination, no short-circuit: every element pair is always evaluated
__781083963_logical( __781083963_eager_bool )
__781083963_logical( __781083963_lazy_bool )


#undef __781083963_eager
#undef __781083963_compound
#undef __781083963_lazy
#undef __781083963_eager_integral
#undef __781083963_compound_integral
#undef __781083963_lazy_integral
#undef __781083963_eager_bool
#undef __781083963_lazy_bool
#undef __781083963_arithmetic
#undef __781083963_integral
#undef __781083963_comparison
#undef __781083963_logical


//	unary negation for containers
template< is_container t_left >
constexpr auto operator - ( const t_left& left ) -> remove_cvref_t< t_left > { return lazy_transform( left, negate ) | to; }

//	unary plus and complement for containers, eager result
template< is_container t_left >
constexpr auto operator + ( const t_left& left ) -> remove_cvref_t< t_left > { return lazy_transform( left, identity ) | to; }
template< is_container t_left >
constexpr auto operator ~ ( const t_left& left ) -> remove_cvref_t< t_left > { return lazy_transform( left, bit_not ) | to; }

//	logical negation yields bool per element, so it materializes into vector of bool, no short-circuit involved
template< is_container t_left >
constexpr auto operator not ( const t_left& left ) -> vector< bool > { return lazy_transform( left, logical_not ) | to; }

//	unary views stay lazy, no materialization needed
template< is_view t_left >
constexpr auto operator - ( t_left&& left ) { return lazy_transform( all( ::std::forward< t_left >( left ) ), negate ); }
template< is_view t_left >
constexpr auto operator + ( t_left&& left ) { return lazy_transform( all( ::std::forward< t_left >( left ) ), identity ); }
template< is_view t_left >
constexpr auto operator ~ ( t_left&& left ) { return lazy_transform( all( ::std::forward< t_left >( left ) ), bit_not ); }
template< is_view t_left >
constexpr auto operator not ( t_left&& left ) { return lazy_transform( all( ::std::forward< t_left >( left ) ), logical_not ); }


//	prefix and postfix act on each element, not on iterator position,
//	containers mutate in place, views stay lazy without mutation,
//	postfix views are omitted since there is nothing to snapshot before a lazy step
template< is_container t_type >
constexpr auto operator ++ ( t_type& container ) noexcept -> t_type&
{ return eager_transform( container, container.begin( ), increment ), container; }
template< is_container t_type >
constexpr auto operator ++ ( t_type& container, int ) -> remove_cvref_t< t_type >
{
	auto old = container;
	eager_transform( container, container.begin( ), increment );
	return	old;
}
template< is_container t_type >
constexpr auto operator -- ( t_type& container ) noexcept -> t_type&
{ return eager_transform( container, container.begin( ), decrement ), container; }
template< is_container t_type >
constexpr auto operator -- ( t_type& container, int ) -> remove_cvref_t< t_type >
{
	auto old = container;
	eager_transform( container, container.begin( ), decrement );
	return	old;
}
template< is_view t_type >
constexpr auto operator ++ ( t_type&& view )
{ return lazy_transform( all( ::std::forward< t_type >( view ) ), increment ); }
template< is_view t_type >
constexpr auto operator -- ( t_type&& view )
{ return lazy_transform( all( ::std::forward< t_type >( view ) ), decrement ); }


} }


#endif


