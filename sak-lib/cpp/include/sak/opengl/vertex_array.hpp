//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_117096077
#define header_guard_117096077


#include <sak/opengl/opengl.hpp>


namespace sak {
namespace opengl {


__using( ::std::, make_unique, unique_ptr )


class vertex_array
{
public:
	vertex_array( )
		:m_raii( make_unique< raii_destructor >( ) )
	{
		GLuint vertex_array_id = 0;
		gl_create_vertex_arrays( 1, &vertex_array_id );
		id( vertex_array_id );
	}

	auto bind( ) const noexcept -> void { gl_bind_vertex_array( id( ) ); }

private:
	auto id( ) const noexcept -> GLuint { return m_raii->m_id; }
	auto id( const GLuint value ) noexcept -> void { m_raii->m_id = value; }

	struct raii_destructor
	{
		~raii_destructor( ) noexcept { gl_delete_vertex_arrays( 1, &m_id ); }

		GLuint	m_id{ 0 };
	};

	unique_ptr< raii_destructor > m_raii;
};


} } 


#endif


