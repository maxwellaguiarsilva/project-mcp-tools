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
//	File:   include/sak/math/error.hpp
//	Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	
//	Created on 2026-08-12 13:02:42
//	


#pragma once
#ifndef header_guard_324439324
#define header_guard_324439324


#include <sak/sak.hpp>
#include <exception>


namespace sak {
namespace math {


//	infrastructure: error codes and the exception type for the math domain
enum class error
{
	 division_by_zero
	,arithmetic_overflow
	,sqrt_of_negative
};


struct exception : ::std::exception
{
	error m_error;
	constexpr explicit exception( error error_code ) : m_error( error_code ) { }
	
	const char* what( ) const noexcept override
	{
		switch( m_error )
		{
			case error::division_by_zero: return "math: division by zero";
			case error::arithmetic_overflow: return "math: arithmetic overflow";
			case error::sqrt_of_negative: return "math: square root of negative number";
		}
		return	"math: unknown error";
	}
};


} }


#endif


