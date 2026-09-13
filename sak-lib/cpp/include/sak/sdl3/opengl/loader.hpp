//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_452039566
#define header_guard_452039566


#include <sak/sak.hpp>
#include <concepts>


namespace sak {
namespace sdl3 {
namespace opengl {


__using( ::std::, convertible_to )


template< typename t_loader, typename t_function_pointer >
concept loader_for = requires( t_loader load, t_function_pointer function_pointer )
{
	{ load( function_pointer ) } -> convertible_to< bool >;
};


template< typename = void >
struct detected_loader
{
	static constexpr bool available = false;
};


#if defined( GLAD_GL_H_ )


template<>
struct detected_loader< void >
{
	static constexpr bool available = true;

	template< typename t_function_pointer >
	auto operator ( ) ( t_function_pointer function_pointer ) const -> bool
	{
		return	gladLoadGL( function_pointer ) not_eq 0;
	}
};


#endif


} } } 


#endif


