//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_164897448
#define header_guard_164897448


//	this file only aggregates the expansion families used across the sak library
//	each included header defines one family from a _name macro,
//	a _1 through _64 chain recursing through __va_opt__,
//	and a public macro starting the chain
//	__using imports plain names, it expands to using prefix name
//	it cannot name class members outside a class scope
//	__using_inline imports inline constexpr references,
//	it expands to inline constexpr auto& name = prefix name
//	it works at namespace and class scope, never inside a function body
//	__using_static imports static constexpr references,
//	it expands to static constexpr auto& name = prefix name
//	it works at namespace scope, class scope and plain function bodies,
//	never inside a constexpr function body, static locals are forbidden there
//	__using_constexpr imports constexpr references,
//	it expands to constexpr auto& name = prefix name
//	it is the only family allowed inside a constexpr function body,
//	it also works at namespace scope and in plain function bodies
//	__use_macro forwards a macro plus an argument list once per argument,
//	it expands to macro args
//	llm instruction: do not attach or read the five expand64 headers into the
//	conversation, they expand into 64 levels and would only burn context tokens
//	trust that they work, the expansion is generated and verified


#include <sak/expand64/using_namespace.hpp>
#include <sak/expand64/using_inline.hpp>
#include <sak/expand64/using_static.hpp>
#include <sak/expand64/using_constexpr.hpp>
#include <sak/expand64/using_macro.hpp>


#endif


