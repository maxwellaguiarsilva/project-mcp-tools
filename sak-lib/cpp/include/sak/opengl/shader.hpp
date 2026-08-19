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
//	File:   include/sak/opengl/shader.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-08-18 22:18:53
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


