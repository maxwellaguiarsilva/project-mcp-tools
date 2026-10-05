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


__using( ::std::, make_unique, uint16_t, uint32_t, unique_ptr, vector )
__using( ::sak::, byte )


class audio final
{
public:

	static auto spec_for( uint16_t channels, uint32_t rate, uint16_t bits ) -> SDL_AudioSpec
	{
		SDL_AudioSpec result{ };
		result.format	=	( bits == 8 ) ? SDL_AUDIO_U8 :  SDL_AUDIO_S16;
		result.channels	=	channels;
		result.freq		=	rate;
		return	result;
	}

	audio( ) noexcept
		:m_raii( make_unique< raii_destructor >( ) )
	{ }

	explicit audio( const SDL_AudioSpec& spec )
		:m_raii( make_unique< raii_destructor >( ) )
	{
		m_raii->m_stream	=	SDL_OpenAudioDeviceStream( SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr );
		ensure( m_raii->m_stream not_eq nullptr, SDL_GetError( ) );
	}

	auto resume( ) noexcept -> void { SDL_ResumeAudioStreamDevice( m_raii->m_stream ); }
	auto pause( ) noexcept -> void { SDL_PauseAudioStreamDevice( m_raii->m_stream ); }
	auto paused( ) const noexcept -> bool { return SDL_AudioStreamDevicePaused( m_raii->m_stream ); }
	auto pitch( float ratio ) noexcept -> void { SDL_SetAudioStreamFrequencyRatio( m_raii->m_stream, ratio ); }
	auto gain( float gain ) noexcept -> void { SDL_SetAudioStreamGain( m_raii->m_stream, gain ); }
	auto push( const vector< byte >& samples ) noexcept -> void { SDL_PutAudioStreamData( m_raii->m_stream, samples.data( ), samples.size( ) ); }
	auto queued( ) const noexcept -> int { return SDL_GetAudioStreamQueued( m_raii->m_stream ); }

private:
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


