//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
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


