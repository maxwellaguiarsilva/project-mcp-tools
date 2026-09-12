//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_406251836
#define header_guard_406251836


#include <sak/sak.hpp>
#include <concepts>


namespace sak {
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


} } 


#endif


