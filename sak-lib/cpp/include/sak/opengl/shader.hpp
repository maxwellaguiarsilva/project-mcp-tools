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


using	::std::string;
using	::std::runtime_error;
using	::sak::opengl::fetch_log;


class shader
{
public:
	enum class type
	{
		 vertex
		,fragment
		,geometry
		,tess_control
		,tess_evaluation
		,compute
	};

	shader( const string& source, const type shader_type = type::vertex );
	~shader( ) noexcept;

	delete_copy_move_ctc( shader )

	auto id( ) const noexcept -> GLuint { return m_id; }

private:
	GLuint	m_id{ 0 };
};


} } 


#endif


