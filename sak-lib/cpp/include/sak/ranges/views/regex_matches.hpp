//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_025953311
#define header_guard_025953311


#include <sak/sak.hpp>
#include <regex>


namespace sak::ranges::views {


//	--------------------------------------------------
__using( ::std::
	,regex
	,regex_iterator
)
__using( ::std::ranges::
	,range_adaptor_closure
	,subrange
	,viewable_range
	,iterator_t
	,begin
	,end
)
//	--------------------------------------------------


struct __regex_matches : range_adaptor_closure< __regex_matches >
{
	struct closure : range_adaptor_closure< closure >
	{
		const regex& m_pattern;
		explicit closure( const regex& pattern ) : m_pattern( pattern ) { }
		template< viewable_range t_range >
		auto operator ( ) ( t_range&& range ) const
		{ return __regex_matches{ }( ::std::forward< t_range >( range ), m_pattern ); }
	};

	template< viewable_range t_range >
	auto operator ( ) ( t_range&& range, const regex& pattern ) const
	{
		using iterator = iterator_t< t_range >;
		return	subrange(
			 regex_iterator< iterator >( begin( range ), end( range ), pattern )
			,regex_iterator< iterator >( )
		);
	}

	auto operator ( ) ( const regex& pattern ) const { return closure{ pattern }; }
};

inline constexpr auto regex_matches = __regex_matches{ };


}


#endif


