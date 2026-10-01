//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_781083963
#define header_guard_781083963


#include <sak/math/math.hpp>
#include <sak/ranges/to.hpp>


namespace sak {
namespace ranges {


__using( ::std::
	,remove_cvref_t
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
	,divides
	,is_arithmetic
	,minus
	,modulus
	,multiplies
	,negate
	,plus
)


//	element-wise operators for containers (non-view ranges): eager result
#define __sak_operators_eager( a_operator, a_operation ) \
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


#define __sak_operators_compound( a_operator, a_operation ) \
template< is_container t_left, is_container t_right > \
requires( is_same_decayed< t_left, t_right > ) \
constexpr auto operator a_operator##= ( t_left& left, const t_right& right ) noexcept -> t_left& \
{ return eager_transform( left, right, left.begin( ), a_operation ), left; } \
template< is_container t_left, is_arithmetic t_scalar > \
constexpr auto operator a_operator##= ( t_left& left, t_scalar right ) noexcept -> t_left& \
{ return eager_transform( left, repeat( right ), left.begin( ), a_operation ), left; }


//	element-wise operators for views (at least one operand is a view): lazy result
#define __sak_operators_lazy( a_operator, a_operation ) \
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


__sak_operators_eager( + ,plus		)
__sak_operators_eager( - ,minus		)
__sak_operators_eager( * ,multiplies	)
__sak_operators_eager( / ,divides	)
__sak_operators_eager( % ,modulus	)

__sak_operators_compound( + ,plus		)
__sak_operators_compound( - ,minus		)
__sak_operators_compound( * ,multiplies	)
__sak_operators_compound( / ,divides	)
__sak_operators_compound( % ,modulus	)

__sak_operators_lazy( + ,plus		)
__sak_operators_lazy( - ,minus		)
__sak_operators_lazy( * ,multiplies	)
__sak_operators_lazy( / ,divides	)
__sak_operators_lazy( % ,modulus	)


#undef __sak_operators_eager
#undef __sak_operators_compound
#undef __sak_operators_lazy


//	unary negation for containers
template< is_container t_left >
constexpr auto operator - ( const t_left& left ) -> remove_cvref_t< t_left >
{
	return	lazy_transform( left, negate ) | to;
}


} }
 

#endif


