//	
//	SPDX-FileCopyrightText: 2025 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_360999933
#define header_guard_360999933


#include <sak/sak.hpp>


namespace sak {
namespace pattern {

__using( ::std::
	,apply
)
__using( ::sak::
	,is_tuple
)


template< typename t_invocable >
struct __tupled_caller
{
	t_invocable m_function;
	template< is_tuple t_tuple >
	constexpr auto operator ( )( const t_tuple& args ) const { return apply( m_function, args ); }
};
struct __tupled
{
	template< typename t_invocable >
	constexpr auto operator( )( const t_invocable& invocable ) const {
		return	__tupled_caller< t_invocable >{ invocable };
	}
};
inline constexpr auto tupled = __tupled{ };


} }


#endif


