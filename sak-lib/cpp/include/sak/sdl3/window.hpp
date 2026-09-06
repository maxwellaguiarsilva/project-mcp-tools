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
#include <sak/pattern/bitmask.hpp>
#include <string>
#include <SDL3/SDL.h>


namespace sak {
namespace sdl3 {


__using( ::std::, string )
__using( ::sak::, ensure )
__using( ::sak::pattern::, bitmask )


class window
{
public:
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

	window( const string& title, const int width, const int height, const window_flags flags = window_flags{ } )
		: m_id( SDL_CreateWindow( title.c_str( ), width, height, flags ) )
	{
		ensure( m_id not_eq nullptr, "failed to create sdl window" );
	}

	~window( ) noexcept { SDL_DestroyWindow( m_id ); }

	delete_copy_move_ctc( window )

	auto id( ) const noexcept -> SDL_Window* { return m_id; }
	auto swap( ) const noexcept -> void { SDL_GL_SwapWindow( m_id ); }
	auto title( const string& title ) -> void { SDL_SetWindowTitle( m_id, title.c_str( ) ); }

private:
	SDL_Window*	m_id{ nullptr };
};


} } 


#endif


