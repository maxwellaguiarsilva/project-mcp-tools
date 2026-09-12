//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#include <sak/opengl/shader.hpp>


namespace sak {
namespace opengl {


using	::std::string;
using	::std::runtime_error;
using	::sak::opengl::fetch_log;


static auto to_gl_shader_stage( const shader::type shader_type ) -> GLenum
{
	switch( shader_type )
	{
		case shader::type::vertex: return GL_VERTEX_SHADER;
		case shader::type::fragment: return GL_FRAGMENT_SHADER;
		case shader::type::geometry: return GL_GEOMETRY_SHADER;
		case shader::type::tess_control: return GL_TESS_CONTROL_SHADER;
		case shader::type::tess_evaluation: return GL_TESS_EVALUATION_SHADER;
		case shader::type::compute: return GL_COMPUTE_SHADER;
	}
	return	GL_VERTEX_SHADER;
}


shader::shader( const string& source, const type shader_type )
{
	const GLuint id = gl_create_shader( to_gl_shader_stage( shader_type ) );
	const char* const source_cstr = source.c_str( );
	gl_shader_source( id, 1, &source_cstr, nullptr );
	gl_compile_shader( id );

	int success = 0;
	gl_get_shader_iv( id, GL_COMPILE_STATUS, &success );
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


