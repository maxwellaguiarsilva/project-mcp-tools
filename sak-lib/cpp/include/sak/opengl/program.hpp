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
//	File:   include/sak/opengl/program.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-08-18 23:17:23
//	


#pragma once
#ifndef header_guard_327579479
#define header_guard_327579479


#include <concepts>
#include <ranges>
#include <stdexcept>
#include <string>
#include <sak/opengl/fetch_log.hpp>
#include <sak/opengl/shader.hpp>


namespace sak {
namespace opengl {


__using( ::sak::opengl::, fetch_log, shader )
__using( ::std::, convertible_to, runtime_error, string )
__using( ::std::ranges::, input_range, range_reference_t )


template< input_range t_range >
requires convertible_to< range_reference_t< t_range >, const shader& >
class program
{
public:
	explicit program( const t_range& shaders )
	{
		const GLuint id = gl_create_program( );
		for( const shader& current_shader : shaders )
			gl_attach_shader( id, current_shader.id( ) );

		gl_link_program( id );

		int success = 0;
		gl_get_programiv( id, GL_LINK_STATUS, &success );
		if( not success )
		{
			const string info_log = fetch_log( gl_get_program_info_log, id );
			gl_delete_program( id );
			throw	runtime_error( info_log );
		}

		m_id	=	id;
	}

	~program( ) noexcept { gl_delete_program( m_id ); }

	delete_copy_move_ctc( program )

	auto use( ) const noexcept -> void { gl_use_program( m_id ); }
	auto id( ) const noexcept -> GLuint { return m_id; }

private:
	GLuint	m_id{ 0 };
};


} } 


#endif


