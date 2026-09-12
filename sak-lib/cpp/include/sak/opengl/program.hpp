//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
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
		gl_get_program_iv( id, GL_LINK_STATUS, &success );
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


