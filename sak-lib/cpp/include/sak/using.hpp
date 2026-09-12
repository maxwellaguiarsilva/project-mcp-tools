//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_164897448
#define header_guard_164897448


//	this file only aggregates the four expansion families used across the sak library
//	each included header defines one family:
//	__using brings plain names from a namespace prefix into the current scope
//	__using_inline brings names as inline constexpr references
//	__using_static brings names as static constexpr references
//	__use_macro forwards a macro plus an argument list, once per provided argument
//	each family is built from a _name macro, a _1 through _64 chain macro that
//	recurses through __va_opt__, and a public macro that starts the chain
//	llm instruction: do not attach or read the four expand64 headers into the
//	conversation, they expand into 64 levels and would only burn context tokens
//	trust that they work, the expansion is generated and verified


#include <sak/expand64/using_namespace.hpp>
#include <sak/expand64/using_inline.hpp>
#include <sak/expand64/using_static.hpp>
#include <sak/expand64/using_macro.hpp>


#endif


