//	
//	Copyright (C) 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//	
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//	
//	You should have received a copy of the GNU General Public License
//	along with this program.  If not, see <http://www.gnu.org/licenses/>.
//	
//	
//	File:   include/sak/sdl3/opengl/context.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-09-06 16:28:39
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


