//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_553966683
#define header_guard_553966683


#include <string>
#include <stdexcept>
#include <cstdlib>


namespace sak {


using	::std::string;
using	::std::runtime_error;


inline const int exit_success = EXIT_SUCCESS;
inline const int exit_failure = EXIT_FAILURE;


struct __ensure
{
	constexpr auto operator ( ) ( bool expression, const string& message ) const { if( not expression ) throw runtime_error( message ); }
};
inline constexpr auto ensure = __ensure{ };


} 


#endif


