//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_495581557
#define header_guard_495581557


#include <sak/sak.hpp>
#include <sak/ensure.hpp>
#include <sak/opengl/glad/loader.hpp>
#include <sak/sdl3/opengl/attributes.hpp>
#include <sak/sdl3/window.hpp>
#include <SDL3/SDL.h>


namespace sak {
namespace sdl3 {
namespace opengl {


__using( ::sak::, ensure )
__using( ::sak::opengl::, loader_for, detected_loader )


class context
{
public:
	using	loader_type	=	decltype( &SDL_GL_GetProcAddress );

	template< typename t_loader >
		requires loader_for< t_loader, loader_type >
	explicit context( const window& application_window, t_loader load, const attributes& = attributes{ } )
		: m_id( SDL_GL_CreateContext( application_window.id( ) ) )
	{
		ensure( m_id not_eq nullptr, "failed to create opengl context" );
		ensure( load( function_pointer( ) ), "failed to load opengl functions" );
	}

	template< typename t_loader = detected_loader<> >
		requires ( t_loader::available )
	explicit context( const window& application_window, const attributes& gl_attributes = attributes{ } )
		: context( application_window, t_loader{ }, gl_attributes )
	{ }

	~context( ) noexcept { SDL_GL_DestroyContext( m_id ); }

	delete_copy_move_ctc( context )

	auto id( ) const noexcept -> SDL_GLContext { return m_id; }
	auto function_pointer( ) const noexcept -> loader_type { return &SDL_GL_GetProcAddress; }

private:
	SDL_GLContext	m_id{ nullptr };
};


} } } 


#endif


