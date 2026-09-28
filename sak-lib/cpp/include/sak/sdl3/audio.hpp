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


__using( ::std::, vector, uint16_t, uint32_t )
__using( ::sak::, byte )


class audio final
{
public:
	static auto format_for( uint16_t bits ) -> SDL_AudioFormat
	{
		if( bits == 8 )
			return	SDL_AUDIO_U8;
		return	SDL_AUDIO_S16;
	}

	static auto spec_for( uint16_t channels, uint32_t rate, uint16_t bits ) -> SDL_AudioSpec
	{
		SDL_AudioSpec result{ };
		result.format = format_for( bits );
		result.channels = static_cast< int >( channels );
		result.freq = static_cast< int >( rate );
		return	result;
	}

	audio( ) noexcept
		:m_stream( nullptr )
	{
	}

	explicit audio( const SDL_AudioSpec& spec ) noexcept
		:m_stream( SDL_OpenAudioDeviceStream( SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr ) )
	{
	}

	~audio( ) noexcept
	{
		if( m_stream not_eq nullptr )
			SDL_DestroyAudioStream( m_stream );
	}

	delete_copy_ctc( audio )

	audio( audio&& other ) noexcept
		:m_stream( other.m_stream )
	{
		other.m_stream = nullptr;
	}

	auto operator=( audio&& other ) noexcept -> audio&
	{
		if( this not_eq &other )
		{
			if( m_stream not_eq nullptr )
				SDL_DestroyAudioStream( m_stream );
			m_stream = other.m_stream;
			other.m_stream = nullptr;
		}
		return	*this;
	}

	auto valid( ) const noexcept -> bool { return m_stream not_eq nullptr; }

	auto resume( ) noexcept -> void
	{
		if( m_stream not_eq nullptr )
			SDL_ResumeAudioStreamDevice( m_stream );
	}

	auto pause( ) noexcept -> void
	{
		if( m_stream not_eq nullptr )
			SDL_PauseAudioStreamDevice( m_stream );
	}

	auto paused( ) const noexcept -> bool
	{
		if( m_stream == nullptr )
			return	true;
		return	SDL_AudioStreamDevicePaused( m_stream );
	}

	auto pitch( float ratio ) noexcept -> void
	{
		if( m_stream not_eq nullptr and ratio > 0.0f )
			SDL_SetAudioStreamFrequencyRatio( m_stream, ratio );
	}

	auto gain( float gain ) noexcept -> void
	{
		if( m_stream not_eq nullptr )
			SDL_SetAudioStreamGain( m_stream, gain );
	}

	auto push( const vector< byte >& samples ) noexcept -> void
	{
		if( m_stream not_eq nullptr and not samples.empty( ) )
			SDL_PutAudioStreamData( m_stream, samples.data( ), static_cast< int >( samples.size( ) ) );
	}

	auto queued( ) const noexcept -> int
	{
		if( m_stream == nullptr )
			return	0;
		return	SDL_GetAudioStreamQueued( m_stream );
	}

private:
	SDL_AudioStream*	m_stream{ nullptr };
};


} }


#endif


