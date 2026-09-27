//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_039286706
#define header_guard_039286706

//	sak	=	swiss_army_knife
//	it is designed as a collection of generic, domain-independent utilities covering mathematics, geometry, and design patterns 
//	domain agnostic: it contains no business logic or special-specific hardware dependencies
//	modern paradigms: it leverages c++ features such as `ranges`, `views`, and custom `niebloids` to reduce visual noise and promote **functional composition**, oop and `extrem don't repeat yourself mindset`



#include <cstdint>
#include <cstdio>
#include <memory>
#include <print>
#include <sak/default_ctc_dtc.hpp>
#include <sak/ensure.hpp>
#include <sak/concepts.hpp>


namespace sak {


using	byte	=	uint8_t;


}

#endif


