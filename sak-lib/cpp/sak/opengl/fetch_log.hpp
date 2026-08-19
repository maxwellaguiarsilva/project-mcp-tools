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
//	File:   include/sak/opengl/fetch_log.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-08-16 22:23:58
//	


#pragma once
#ifndef header_guard_935739736
#define header_guard_935739736


#include <sak/sak.hpp>
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


