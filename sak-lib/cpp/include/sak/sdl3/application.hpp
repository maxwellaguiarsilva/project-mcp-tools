//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_498437518
#define header_guard_498437518


#include <sak/sak.hpp>
#include <concepts>
#include <functional>
#include <initializer_list>
#include <type_traits>
#include <sak/ensure.hpp>
#include <sak/math/math.hpp>
#include <sak/pattern/bitmask.hpp>
#include <sak/pattern/dispatcher.hpp>
#include <sak/sdl3/window.hpp>
#include <SDL3/SDL.h>


namespace sak {
namespace sdl3 {


__using( ::sak::pattern::, bitmask, dispatcher )
__using( ::sak::math::, between )
__using( ::std::, function, initializer_list, same_as, shared_ptr )


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

	class listener
	{
	public:
		virtual ~listener( ) = default;

		virtual void quit( ) { }
	};

	auto operator +=( const shared_ptr< listener >& subject ) -> void { m_dispatcher += subject; }

	auto poll( ) -> bool
	{
		SDL_Event event;
		while( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_EVENT_QUIT )
			{
				( void )m_dispatcher( &listener::quit );
				m_is_running = false;
				continue;
			}

			if( between( event.type, SDL_EVENT_WINDOW_FIRST, SDL_EVENT_WINDOW_LAST ) )
				if( auto* raw_window = SDL_GetWindowFromEvent( &event ) )
					if( auto* raw_instance = static_cast< window* >( SDL_GetPointerProperty( SDL_GetWindowProperties( raw_window ), "sak.sdl3.window", nullptr ) ) )
					{
						//	reference alias keeps the dispatch body free of pointer noise
						auto& window_instance	=	*raw_instance;
						window_instance.dispatch( event.window );
					}
		}

		return	m_is_running;
	}

	auto run( const function< void( ) >& action ) -> void
	{
		while( poll( ) )
			action( );
	}

	auto quit( ) noexcept -> void { m_is_running = false; }

private:
	init_flags				m_flags;
	dispatcher< listener >	m_dispatcher;
	bool					m_is_running{ true };
};


} } 


#endif


