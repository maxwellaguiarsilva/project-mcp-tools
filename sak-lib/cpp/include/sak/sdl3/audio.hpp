//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_282103068
#define header_guard_282103068


#include <sak/sak.hpp>
#include <vector>
#include <SDL3/SDL.h>


namespace sak {
namespace sdl3 {


__using( ::std::
	,make_unique
	,uint16_t
	,uint32_t
	,unique_ptr
	,vector
)
__using( ::sak::, byte )


class audio final
{
public:

	audio( ) noexcept
		:m_raii( make_unique< raii_destructor >( ) )
	{ }

	audio( uint16_t channels, uint32_t rate, uint16_t bits )
		:m_raii( make_unique< raii_destructor >( ) )
	{ open( channels, rate, bits ); }

	auto open( uint16_t channels, uint32_t rate, uint16_t bits ) -> void
	{
		close( );
		SDL_AudioSpec spec{ };
		spec.format		=	( bits == 8 ) ? SDL_AUDIO_U8 : SDL_AUDIO_S16;
		spec.channels	=	channels;
		spec.freq		=	rate;
		stream( SDL_OpenAudioDeviceStream( SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr ) );
		ensure( stream( ) not_eq nullptr, SDL_GetError( ) );
	}

	auto close( ) noexcept -> void { m_raii = make_unique< raii_destructor >( ); }

	auto valid( ) const noexcept -> bool { return stream( ) not_eq nullptr; }

	auto resume( ) noexcept -> void { SDL_ResumeAudioStreamDevice( stream( ) ); }
	auto pause( ) noexcept -> void { SDL_PauseAudioStreamDevice( stream( ) ); }
	auto paused( ) const noexcept -> bool { return SDL_AudioStreamDevicePaused( stream( ) ); }
	auto pitch( float ratio ) noexcept -> void { SDL_SetAudioStreamFrequencyRatio( stream( ), ratio ); }
	auto gain( float gain ) noexcept -> void { SDL_SetAudioStreamGain( stream( ), gain ); }
	auto push( const vector< byte >& samples ) noexcept -> void { SDL_PutAudioStreamData( stream( ), samples.data( ), samples.size( ) ); }
	auto queued( ) const noexcept -> int { return SDL_GetAudioStreamQueued( stream( ) ); }

private:
	auto stream( ) const noexcept -> SDL_AudioStream* { return m_raii->m_stream; }
	auto stream( SDL_AudioStream* value ) noexcept -> void { m_raii->m_stream = value; }

	struct raii_destructor
	{
		~raii_destructor( ) noexcept
		{ if( m_stream not_eq nullptr ) SDL_DestroyAudioStream( m_stream ); }

		SDL_AudioStream*	m_stream{ nullptr };
	};

	unique_ptr< raii_destructor > m_raii;
};


} }


#endif


