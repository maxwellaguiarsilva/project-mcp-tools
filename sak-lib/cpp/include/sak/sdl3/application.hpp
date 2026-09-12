//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
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


