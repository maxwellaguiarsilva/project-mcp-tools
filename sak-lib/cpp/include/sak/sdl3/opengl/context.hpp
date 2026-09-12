//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_495581557
#define header_guard_495581557


#include <sak/sak.hpp>
#include <sak/ensure.hpp>
#include <sak/sdl3/window.hpp>
#include <SDL3/SDL.h>


namespace sak {
namespace sdl3 {
namespace opengl {


__using( ::sak::, ensure )


class context
{
public:
	explicit context( const window& application_window )
		: m_id( SDL_GL_CreateContext( application_window.id( ) ) )
	{
		ensure( m_id not_eq nullptr, "failed to create opengl context" );
	}

	~context( ) noexcept { SDL_GL_DestroyContext( m_id ); }

	using	loader_type	=	decltype( &SDL_GL_GetProcAddress );

	delete_copy_move_ctc( context )

	auto id( ) const noexcept -> SDL_GLContext { return m_id; }
	auto function_pointer( ) const noexcept -> loader_type { return &SDL_GL_GetProcAddress; }

private:
	SDL_GLContext	m_id{ nullptr };
};


} } } 


#endif


