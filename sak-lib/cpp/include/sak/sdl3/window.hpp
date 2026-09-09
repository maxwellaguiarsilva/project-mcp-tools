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
//	File:   include/sak/sdl3/window.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-09-06 16:25:25
//	


#pragma once
#ifndef header_guard_631680322
#define header_guard_631680322


#include <sak/sak.hpp>
#include <sak/ensure.hpp>
#include <sak/geometry/geometry.hpp>
#include <sak/pattern/bitmask.hpp>
#include <sak/pattern/dispatcher.hpp>
#include <memory>
#include <string>
#include <SDL3/SDL.h>


namespace sak {
namespace sdl3 {


__using( ::std::, shared_ptr, string )
__using( ::sak::, ensure )
__using( ::sak::pattern::, bitmask, dispatcher )
using	geometry	=	::sak::g2i;
__using_constexpr( geometry::, width, height, left, top )


class window
{
public:
	using	geometry	=	::sak::g2i;

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

	window( const string& title, const geometry::size& size, const window_flags flags = window_flags{ } )
		: m_id( SDL_CreateWindow( title.c_str( ), width( size ), height( size ), flags ) )
	{
		ensure( m_id not_eq nullptr, "failed to create sdl window" );
		SDL_SetPointerProperty( SDL_GetWindowProperties( m_id ), "sak.window", this );
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
	auto swap( ) const noexcept -> void { SDL_GL_SwapWindow( m_id ); }
	auto title( ) const -> string { return SDL_GetWindowTitle( m_id ); }
	auto title( const string& title ) -> void { SDL_SetWindowTitle( m_id, title.c_str( ) ); }

	auto size( ) const noexcept -> geometry::size
	{
		int window_width = 0, window_height = 0;
		SDL_GetWindowSize( m_id, &window_width, &window_height );
		return	{ window_width, window_height };
	}

	auto size( const geometry::size& size ) -> void
	{
		SDL_SetWindowSize( m_id, width( size ), height( size ) );
	}

	auto pixel_size( ) const noexcept -> geometry::size
	{
		int window_width = 0, window_height = 0;
		SDL_GetWindowSizeInPixels( m_id, &window_width, &window_height );
		return	{ window_width, window_height };
	}

	auto minimum_size( ) const noexcept -> geometry::size
	{
		int window_width = 0, window_height = 0;
		SDL_GetWindowMinimumSize( m_id, &window_width, &window_height );
		return	{ window_width, window_height };
	}

	auto minimum_size( const geometry::size& size ) -> void
	{
		SDL_SetWindowMinimumSize( m_id, width( size ), height( size ) );
	}

	auto maximum_size( ) const noexcept -> geometry::size
	{
		int window_width = 0, window_height = 0;
		SDL_GetWindowMaximumSize( m_id, &window_width, &window_height );
		return	{ window_width, window_height };
	}

	auto maximum_size( const geometry::size& size ) -> void
	{
		SDL_SetWindowMaximumSize( m_id, width( size ), height( size ) );
	}

	auto position( ) const noexcept -> geometry::position
	{
		int position_x = 0, position_y = 0;
		SDL_GetWindowPosition( m_id, &position_x, &position_y );
		return	{ position_x, position_y };
	}

	auto position( const geometry::position& position ) -> void
	{
		SDL_SetWindowPosition( m_id, left( position ), top( position ) );
	}

	auto show( ) -> void { SDL_ShowWindow( m_id ); }
	auto hide( ) -> void { SDL_HideWindow( m_id ); }
	auto raise( ) -> void { SDL_RaiseWindow( m_id ); }
	auto maximize( ) -> void { SDL_MaximizeWindow( m_id ); }
	auto minimize( ) -> void { SDL_MinimizeWindow( m_id ); }
	auto restore( ) -> void { SDL_RestoreWindow( m_id ); }
	auto fullscreen( const bool is_fullscreen ) -> void { SDL_SetWindowFullscreen( m_id, is_fullscreen ); }
	auto sync( ) -> void { SDL_SyncWindow( m_id ); }

private:
	//	todo: integrate application::poll routing for "sak.window" property lookup
	SDL_Window*				m_id{ nullptr };
	dispatcher< listener >	m_dispatcher;
};


} } 


#endif


