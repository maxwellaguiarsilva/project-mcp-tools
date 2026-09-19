//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_015356114
#define header_guard_015356114


#include <sak/opengl/fetch_log.hpp>


namespace sak {
namespace opengl {


__using( ::std::, runtime_error, string )
__using( ::sak::opengl::, fetch_log )


class shader
{
public:
	enum class type : GLenum
	{
		 vertex			=	GL_VERTEX_SHADER
		,fragment			=	GL_FRAGMENT_SHADER
		,geometry			=	GL_GEOMETRY_SHADER
		,tess_control		=	GL_TESS_CONTROL_SHADER
		,tess_evaluation	=	GL_TESS_EVALUATION_SHADER
		,compute			=	GL_COMPUTE_SHADER
	};
	using	enum	type;

	shader( const string& source, const type shader_type = vertex )
	{
		const GLuint id = gl_create_shader( static_cast< GLenum >( shader_type ) );
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

	~shader( ) noexcept { gl_delete_shader( m_id ); }

	delete_copy_move_ctc( shader )

	auto id( ) const noexcept -> GLuint { return m_id; }

private:
	GLuint	m_id{ 0 };
};


} } 


#endif


