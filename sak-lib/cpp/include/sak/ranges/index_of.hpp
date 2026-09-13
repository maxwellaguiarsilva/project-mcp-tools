//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_754211519
#define header_guard_754211519


#include <optional>
#include <ranges>
#include <cstddef>
#include <sak/using.hpp>


namespace sak {
namespace ranges {


//	--------------------------------------------------
__using( ::std::
	,optional
	,size_t
)
__using( ::std::ranges::
	,begin
	,distance
	,end
	,find
	,range_adaptor_closure
	,viewable_range
)
//	--------------------------------------------------


//	position of the first element equal to the value, empty when absent
//	scalar form: index_of( range, value )
//	pipe form: range bitor index_of( value )
struct __index_of
{
	template< typename t_value >
	struct closure : range_adaptor_closure< closure< t_value > >
	{
		t_value	m_value;
		constexpr explicit closure( t_value value ) : m_value( ::std::move( value ) ) { }

		template< viewable_range t_range >
		constexpr auto operator ( ) ( t_range&& range ) const
		{
			return	__index_of{ }( ::std::forward< t_range >( range ), m_value );
		}
	};

	template< viewable_range t_range, typename t_value >
	constexpr auto operator ( ) ( t_range&& range, const t_value& value ) const
	{
		const auto found = find( range, value );
		if( found == end( range ) )
			return	optional< size_t >{ };
		return	optional< size_t >{ distance( begin( range ), found ) };
	}

	template< typename t_value >
	constexpr auto operator ( ) ( t_value value ) const
	{
		return	closure< t_value >{ ::std::move( value ) };
	}
};


inline constexpr auto index_of = __index_of{ };


} }


#endif


