//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_631680322
#define header_guard_631680322


#include <sak/sak.hpp>
#include <sak/ensure.hpp>
#include <sak/geometry/geometry.hpp>
#include <sak/pattern/bitmask.hpp>
#include <sak/pattern/dispatcher.hpp>
#include <sak/sdl3/display.hpp>
#include <memory>
#include <string>
#include <SDL3/SDL.h>


namespace sak {
namespace sdl3 {


__using( ::std::, shared_ptr, string )
__using( ::sak::, ensure )
__using( ::sak::pattern::, bitmask, dispatcher )


#define __631680322_window_geometry( a_name, a_sdl, a_type, a_first, a_second )	\
auto a_name( ) const noexcept -> geometry::a_type									\
{																					\
	int window_##a_first = 0, window_##a_second = 0;								\
	SDL_GetWindow##a_sdl( m_id, &window_##a_first, &window_##a_second );			\
	return	{ window_##a_first, window_##a_second };								\
}																					\
auto a_name( const geometry::a_type& a_value ) -> void { SDL_SetWindow##a_sdl( m_id, a_first( a_value ), a_second( a_value ) ); }


class window
{
public:
	using	geometry	=	::sak::g2i;
	__using_static( geometry::, width, height, left, top )

	enum class flag : SDL_WindowFlags
	{
		//	window state that can be both requested and reported
		 fullscreen				=	SDL_WINDOW_FULLSCREEN
		,hidden					=	SDL_WINDOW_HIDDEN
		,minimized				=	SDL_WINDOW_MINIMIZED
		,maximized				=	SDL_WINDOW_MAXIMIZED
		,occluded				=	SDL_WINDOW_OCCLUDED
		,borderless				=	SDL_WINDOW_BORDERLESS
		,resizable				=	SDL_WINDOW_RESIZABLE
		,always_on_top			=	SDL_WINDOW_ALWAYS_ON_TOP
		,transparent			=	SDL_WINDOW_TRANSPARENT
		,fill_document			=	SDL_WINDOW_FILL_DOCUMENT

		//	focus and modality
		,input_focus			=	SDL_WINDOW_INPUT_FOCUS
		,mouse_focus			=	SDL_WINDOW_MOUSE_FOCUS
		,not_focusable			=	SDL_WINDOW_NOT_FOCUSABLE
		,modal					=	SDL_WINDOW_MODAL
		,external				=	SDL_WINDOW_EXTERNAL

		//	pointer and keyboard capture
		,mouse_grabbed			=	SDL_WINDOW_MOUSE_GRABBED
		,keyboard_grabbed		=	SDL_WINDOW_KEYBOARD_GRABBED
		,mouse_capture			=	SDL_WINDOW_MOUSE_CAPTURE
		,mouse_relative_mode	=	SDL_WINDOW_MOUSE_RELATIVE_MODE

		//	system window roles
		,utility				=	SDL_WINDOW_UTILITY
		,tooltip				=	SDL_WINDOW_TOOLTIP
		,popup_menu				=	SDL_WINDOW_POPUP_MENU

		//	graphics api selected at creation
		,opengl					=	SDL_WINDOW_OPENGL
		,vulkan					=	SDL_WINDOW_VULKAN
		,metal					=	SDL_WINDOW_METAL

		//	pixel density
		,high_pixel_density		=	SDL_WINDOW_HIGH_PIXEL_DENSITY
	};

	using	window_flags	=	bitmask< flag >;

	static constexpr float	window_display_ratio	=	0.8f;

	window( const string& title, const window_flags flags = window_flags{ } )
		: window( title, ::sak::sdl3::display{ }.size( ) * window_display_ratio, flags )
	{ }

	window( const string& title, const geometry::size& size, const window_flags flags = window_flags{ } )
		: m_id( create_window( title, size, flags ) )
		, m_display( SDL_GetDisplayForWindow( m_id ) )
	{
		SDL_SetPointerProperty( SDL_GetWindowProperties( m_id ), "sak.sdl3.window", this );
	}

	~window( ) noexcept { SDL_DestroyWindow( m_id ); }

	delete_copy_move_ctc( window )

	class listener
	{
	public:
		virtual ~listener( ) = default;

		virtual void resize( const geometry::size& ) { }
		virtual void pixel_resize( const geometry::size& ) { }
		virtual void move( const geometry::position& ) { }

		virtual void show( ) { }
		virtual void hide( ) { }
		virtual void minimize( ) { }
		virtual void maximize( ) { }
		virtual void restore( ) { }
		virtual void close_requested( ) { }

		virtual void focus_gained( ) { }
		virtual void focus_lost( ) { }
	};

	auto operator +=( const shared_ptr< listener >& subject ) -> void
	{
		m_dispatcher += subject;
	}

	auto dispatch( const SDL_WindowEvent& event ) -> void
	{
		switch( event.type )
		{
			case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
				( void )m_dispatcher( &listener::pixel_resize, geometry::size{ event.data1, event.data2 } );
				break;
			case SDL_EVENT_WINDOW_RESIZED:
				( void )m_dispatcher( &listener::resize, geometry::size{ event.data1, event.data2 } );
				break;
			case SDL_EVENT_WINDOW_MOVED:
				( void )m_dispatcher( &listener::move, geometry::position{ event.data1, event.data2 } );
				break;

			case SDL_EVENT_WINDOW_SHOWN:			( void )m_dispatcher( &listener::show );			break;
			case SDL_EVENT_WINDOW_HIDDEN:			( void )m_dispatcher( &listener::hide );			break;
			case SDL_EVENT_WINDOW_MINIMIZED:		( void )m_dispatcher( &listener::minimize );		break;
			case SDL_EVENT_WINDOW_MAXIMIZED:		( void )m_dispatcher( &listener::maximize );		break;
			case SDL_EVENT_WINDOW_RESTORED:			( void )m_dispatcher( &listener::restore );			break;
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:	( void )m_dispatcher( &listener::close_requested );	break;
			case SDL_EVENT_WINDOW_FOCUS_GAINED:		( void )m_dispatcher( &listener::focus_gained );	break;
			case SDL_EVENT_WINDOW_FOCUS_LOST:		( void )m_dispatcher( &listener::focus_lost );		break;

			default:
				break;
		}
	}

	auto id( ) const noexcept -> SDL_Window* { return m_id; }
	auto display( ) const noexcept -> const ::sak::sdl3::display& { return m_display; }
	auto swap( ) const noexcept -> void { SDL_GL_SwapWindow( m_id ); }
	auto title( ) const -> string { return SDL_GetWindowTitle( m_id ); }
	auto title( const string& title ) -> void { SDL_SetWindowTitle( m_id, title.c_str( ) ); }

	auto pixel_size( ) const noexcept -> geometry::size
	{
		int window_width = 0, window_height = 0;
		SDL_GetWindowSizeInPixels( m_id, &window_width, &window_height );
		return	{ window_width, window_height };
	}

	__use_macro( __631680322_window_geometry
		,(	size			,Size			,size		,width	,height	)
		,(	minimum_size	,MinimumSize	,size		,width	,height	)
		,(	maximum_size	,MaximumSize	,size		,width	,height	)
		,(	position		,Position		,position	,left	,top	)
	)

	auto show( ) -> void { SDL_ShowWindow( m_id ); }
	auto hide( ) -> void { SDL_HideWindow( m_id ); }
	auto raise( ) -> void { SDL_RaiseWindow( m_id ); }
	auto maximize( ) -> void { SDL_MaximizeWindow( m_id ); }
	auto minimize( ) -> void { SDL_MinimizeWindow( m_id ); }
	auto restore( ) -> void { SDL_RestoreWindow( m_id ); }
	auto fullscreen( const bool is_fullscreen ) -> void { SDL_SetWindowFullscreen( m_id, is_fullscreen ); }
	auto sync( ) -> void { SDL_SyncWindow( m_id ); }

private:
	static auto create_window( const string& title, const geometry::size& size, const window_flags flags ) -> SDL_Window*
	{
		auto*	created_window	=	SDL_CreateWindow( title.c_str( ), width( size ), height( size ), flags );
		ensure( created_window not_eq nullptr, "failed to create sdl window" );
		return	created_window;
	}

	//	todo: integrate application::poll routing for "sak.sdl3.window" property lookup
	SDL_Window*				m_id{ nullptr };
	::sak::sdl3::display	m_display;
	dispatcher< listener >	m_dispatcher;
};


#undef __631680322_window_geometry


} } 


#endif


