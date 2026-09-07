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
//	File:   include/sak/sdl3/display.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-09-07 11:40:01
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


