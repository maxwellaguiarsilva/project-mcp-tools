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
//	File:   source/sak/opengl/shader.cpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-08-18 22:18:53
//	


#include <sak/opengl/shader.hpp>


namespace sak {
namespace opengl {


using	::std::string;
using	::std::runtime_error;
using	::sak::opengl::fetch_log;


shader::shader( const string& source, const type shader_type )
{
	const GLuint id = gl_create_shader( shader_type == type::vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER );
	const char* const source_cstr = source.c_str( );
	gl_shader_source( id, 1, &source_cstr, nullptr );
	gl_compile_shader( id );

	int success = 0;
	gl_get_shaderiv( id, GL_COMPILE_STATUS, &success );
	if( not success )
	{
		const string info_log = fetch_log( gl_get_shader_info_log, id );
		gl_delete_shader( id );
		throw	runtime_error( info_log );
	}

	m_id	=	id;
}


shader::~shader( ) noexcept { gl_delete_shader( m_id ); }


} } 


