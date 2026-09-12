//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_805823872
#define header_guard_805823872


#include <sak/sak.hpp>
#include <sak/geometry/geometry.hpp>
#include <SDL3/SDL.h>


namespace sak {
namespace sdl3 {


class display
{
public:
	using	geometry	=	::sak::g2i;

	display( ) noexcept : display( SDL_GetPrimaryDisplay( ) ) { }

	explicit display( const SDL_DisplayID display_id ) noexcept
		: m_id( display_id )
	{ }

	delete_copy_move_ctc( display )

	auto id( ) const noexcept -> SDL_DisplayID { return m_id; }

	auto size( ) const noexcept -> geometry::size
	{
		SDL_Rect bounds{ };
		SDL_GetDisplayBounds( m_id, &bounds );
		return	{ bounds.w, bounds.h };
	}

private:
	SDL_DisplayID	m_id{ 0 };
};


} } 


#endif


