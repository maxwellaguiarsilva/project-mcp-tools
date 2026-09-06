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
//	File:   include/sak/sdl3/application.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-09-05 11:59:08
//	


#pragma once
#ifndef header_guard_498437518
#define header_guard_498437518


#include <sak/sak.hpp>
#include <concepts>
#include <initializer_list>
#include <type_traits>
#include <sak/ensure.hpp>
#include <sak/pattern/bitmask.hpp>
#include <SDL3/SDL.h>


namespace sak {
namespace sdl3 {


__using( ::sak::pattern::, bitmask )
__using( ::std::, initializer_list, same_as )


class application
{
public:
	enum class flag : SDL_InitFlags
	{
		 audio		=	SDL_INIT_AUDIO
		,video		=	SDL_INIT_VIDEO
		,joystick	=	SDL_INIT_JOYSTICK
		,haptic		=	SDL_INIT_HAPTIC
		,gamepad	=	SDL_INIT_GAMEPAD
		,events		=	SDL_INIT_EVENTS
		,sensor		=	SDL_INIT_SENSOR
		,camera		=	SDL_INIT_CAMERA
	};

	using	init_flags	=	bitmask< flag >;

	explicit application( const init_flags flags = init_flags{ flag::video } )
		: m_flags( flags )
	{ ensure( SDL_Init( m_flags ), SDL_GetError( ) ); }

	explicit application( const initializer_list< flag > flags )
		: application( init_flags{ flags } )
	{ }

	template< same_as< flag >... t_flags >
		requires ( sizeof...( t_flags ) > 0 )
	explicit application( const t_flags... flags )
		: application( init_flags{ flags... } )
	{ }

	~application( ) noexcept { SDL_Quit( ); }

	delete_copy_move_ctc( application )

	auto flags( ) const noexcept -> const init_flags& { return m_flags; }

private:
	init_flags	m_flags;
};


} } 


#endif


