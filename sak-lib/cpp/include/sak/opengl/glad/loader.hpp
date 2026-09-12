//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_422588152
#define header_guard_422588152


#include <sak/sak.hpp>
#include <sak/opengl/loader.hpp>


#if defined( GLAD_GL_H_ )


namespace sak {
namespace opengl {


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


} } 


#endif


#endif


