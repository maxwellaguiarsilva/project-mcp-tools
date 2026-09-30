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
	,forward
	,remove_cvref_t
	,same_as
	,size_t
)
__using( ::std::ranges::
	,input_range
	,view
	,viewable_range
)
__using( ::std::views::
	,all
	,repeat
	,zip_transform
)
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
template< input_range t_left, input_range t_right > \
requires( not is_view< t_left > and not is_view< t_right > and not is_string_like< t_left > and not is_string_like< t_right > and same_as< remove_cvref_t< t_left >, remove_cvref_t< t_right > > ) \
constexpr auto operator a_operator ( t_left&& left, t_right&& right ) -> remove_cvref_t< t_left > \
{ \
	return	zip_transform( a_operation, all( left ), all( right ) ) | to; \
} \
template< input_range t_left, is_arithmetic t_scalar > \
requires( not is_view< t_left > and not is_string_like< t_left > ) \
constexpr auto operator a_operator ( t_left&& left, t_scalar right ) -> remove_cvref_t< t_left > \
{ \
	return	zip_transform( a_operation, all( left ), repeat( right ) ) | to; \
} \
template< is_arithmetic t_scalar, input_range t_right > \
requires( not is_view< t_right > and not is_string_like< t_right > ) \
constexpr auto operator a_operator ( t_scalar left, t_right&& right ) -> remove_cvref_t< t_right > \
{ \
	return	zip_transform( a_operation, repeat( left ), all( right ) ) | to; \
}


#define __sak_operators_compound( a_operator, a_operation ) \
template< input_range t_left, input_range t_right > \
requires( not is_view< t_left > and not is_view< t_right > and not is_string_like< t_left > and not is_string_like< t_right > and same_as< remove_cvref_t< t_left >, remove_cvref_t< t_right > > ) \
constexpr auto operator a_operator##= ( t_left& left, const t_right& right ) noexcept -> t_left& \
{ \
	eager_transform( left, right, left.begin( ), a_operation ); \
	return	left; \
} \
template< input_range t_left, is_arithmetic t_scalar > \
requires( not is_view< t_left > and not is_string_like< t_left > ) \
constexpr auto operator a_operator##= ( t_left& left, t_scalar right ) noexcept -> t_left& \
{ \
	eager_transform( left, repeat( right ), left.begin( ), a_operation ); \
	return	left; \
}


//	element-wise operators for views (at least one operand is a view): lazy result
#define __sak_operators_lazy( a_operator, a_operation ) \
template< viewable_range t_left, viewable_range t_right > \
requires( is_view< t_left > or is_view< t_right > ) \
constexpr auto operator a_operator ( t_left&& left, t_right&& right ) \
{ \
	return	zip_transform( a_operation, all( ::std::forward< t_left >( left ) ), all( ::std::forward< t_right >( right ) ) ); \
} \
template< viewable_range t_left, is_arithmetic t_scalar > \
requires( is_view< t_left > ) \
constexpr auto operator a_operator ( t_left&& left, t_scalar right ) \
{ \
	return	zip_transform( a_operation, all( ::std::forward< t_left >( left ) ), repeat( right ) ); \
} \
template< is_arithmetic t_scalar, viewable_range t_right > \
requires( is_view< t_right > ) \
constexpr auto operator a_operator ( t_scalar left, t_right&& right ) \
{ \
	return	zip_transform( a_operation, repeat( left ), all( ::std::forward< t_right >( right ) ) ); \
}


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
template< input_range t_left >
requires( not is_view< t_left > and not is_string_like< t_left > )
constexpr auto operator - ( const t_left& left ) -> remove_cvref_t< t_left >
{
	return	lazy_transform( left, negate ) | to;
}


} }
 

#endif


