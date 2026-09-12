//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_935739736
#define header_guard_935739736


#include <sak/opengl/opengl.hpp>
#include <string>


namespace sak {
namespace opengl {


using	::std::string;


//	fetches a complete info log by doubling the buffer until the whole log fits
//	usage: fetch_log( glgetshaderinfolog, id ) or fetch_log( glgetprograminfolog, id )
struct __fetch_log
{
	template< class t_get_log >
	requires( is_callable< t_get_log, GLuint, GLsizei, GLsizei*, GLchar* > )
	constexpr auto operator ( ) ( const t_get_log get_log, const GLuint id, const int initial_size = 512 ) const -> const string
	{
		GLsizei length = 0;
		int size = initial_size;
		string log;

		do
		{
			log.resize( size );
			get_log( id, static_cast< GLsizei >( size ), &length, log.data( ) );
			if( length < size - 1 )
			{
				log.resize( length );
				return	log;
			}
			size *= 2;
		}
		while( size <= 1 << 16 );

		return	log;
	}
};
inline constexpr auto fetch_log = __fetch_log{ };


} } 


#endif


