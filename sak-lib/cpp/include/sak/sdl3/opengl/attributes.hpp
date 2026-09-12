//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_924217115
#define header_guard_924217115


#include <sak/sak.hpp>
#include <utility>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>


namespace sak {
namespace sdl3 {
namespace opengl {


__using( ::sak::, byte, ensure, is_castable_enum )
__using( ::std::, pair, string )


#define __924217115_set_value_attribute( attribute_type, value_type, error_message )	\
auto set( const attribute_type attribute, const value_type value ) -> void { raw_set( attribute, value, error_message ); }


class attributes
{
public:
	using	version	=	pair< byte, byte >;

	enum class profile
	{
		 core			=	SDL_GL_CONTEXT_PROFILE_CORE
		,compatibility	=	SDL_GL_CONTEXT_PROFILE_COMPATIBILITY
		,es				=	SDL_GL_CONTEXT_PROFILE_ES
	};

	enum class buffer_size
	{
		 red				=	SDL_GL_RED_SIZE
		,green				=	SDL_GL_GREEN_SIZE
		,blue				=	SDL_GL_BLUE_SIZE
		,alpha				=	SDL_GL_ALPHA_SIZE
		,depth				=	SDL_GL_DEPTH_SIZE
		,stencil			=	SDL_GL_STENCIL_SIZE
		,accumulation_red	=	SDL_GL_ACCUM_RED_SIZE
		,accumulation_green	=	SDL_GL_ACCUM_GREEN_SIZE
		,accumulation_blue	=	SDL_GL_ACCUM_BLUE_SIZE
		,accumulation_alpha	=	SDL_GL_ACCUM_ALPHA_SIZE
		,total				=	SDL_GL_BUFFER_SIZE
	};

	enum class visual
	{
		 float_buffers		=	SDL_GL_FLOATBUFFERS
		,double_buffered	=	SDL_GL_DOUBLEBUFFER
		,stereo				=	SDL_GL_STEREO
		,accelerated		=	SDL_GL_ACCELERATED_VISUAL
		,share_current		=	SDL_GL_SHARE_WITH_CURRENT_CONTEXT
		,srgb				=	SDL_GL_FRAMEBUFFER_SRGB_CAPABLE
		,no_error			=	SDL_GL_CONTEXT_NO_ERROR
	};

	enum class multisample
	{
		 buffers			=	SDL_GL_MULTISAMPLEBUFFERS
		,samples			=	SDL_GL_MULTISAMPLESAMPLES
	};

	enum class context_flag
	{
		 debug				=	SDL_GL_CONTEXT_DEBUG_FLAG
		,forward_compatible	=	SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG
		,robust_access		=	SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG
		,reset_isolation	=	SDL_GL_CONTEXT_RESET_ISOLATION_FLAG
	};

	enum class release_behavior
	{
		 none				=	SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE
		,flush				=	SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH
	};

	enum class reset_notification
	{
		 no_notification	=	SDL_GL_CONTEXT_RESET_NO_NOTIFICATION
		,lose_context		=	SDL_GL_CONTEXT_RESET_LOSE_CONTEXT
	};

	explicit attributes( const profile gl_profile = profile::core, const version gl_version = version{ 4, 6 } ) { set( gl_profile, gl_version ); }

	~attributes( ) noexcept { SDL_GL_ResetAttributes( ); }

	delete_copy_move_ctc( attributes )

	auto set( const profile attribute, const version context_version ) -> void
	{
		raw_set(	SDL_GL_CONTEXT_PROFILE_MASK		,static_cast< int >( attribute )	,"failed to set opengl profile"			);
		raw_set(	SDL_GL_CONTEXT_MAJOR_VERSION	,context_version.first				,"failed to set opengl major version"	);
		raw_set(	SDL_GL_CONTEXT_MINOR_VERSION	,context_version.second				,"failed to set opengl minor version"	);
	}

	__use_macro( __924217115_set_value_attribute
		,(	buffer_size			,int	,"failed to set opengl buffer size attribute"		)
		,(	visual				,bool	,"failed to set opengl visual attribute"			)
		,(	multisample			,int	,"failed to set opengl multisample attribute"		)
		,(	context_flag		,bool	,"failed to set opengl context flag"				)
		,(	release_behavior	,int	,"failed to set opengl context release behavior"	)
		,(	reset_notification	,int	,"failed to set opengl context reset notification"	)
	)

private:
	template< typename attribute >
	auto raw_set( const attribute gl_attribute, const int value, const string& error_message ) -> void
		requires is_castable_enum< attribute, SDL_GLAttr >
	{
		ensure( SDL_GL_SetAttribute( static_cast< SDL_GLAttr >( gl_attribute ), value ), error_message );
	}
};


#undef __924217115_set_value_attribute


} } } 


#endif


