//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_016647174
#define header_guard_016647174


#include <sak/sak.hpp>
#include <sak/ensure.hpp>
#include <sak/sdl3/window.hpp>
#include <SDL3/SDL.h>


namespace sak {
namespace sdl3 {


__using( ::sak::, ensure )


class display
{
public:
	display( ) noexcept
		: m_id( SDL_GetPrimaryDisplay( ) )
	{ }

	explicit display( const window& application_window )
		: m_id( SDL_GetDisplayForWindow( application_window.id( ) ) )
	{ ensure( m_id not_eq 0, "failed to get sdl display for window" ); }

	delete_copy_move_ctc( display )

	auto id( ) const noexcept -> SDL_DisplayID { return m_id; }

private:
	SDL_DisplayID	m_id{ 0 };
};


} } 


#endif


