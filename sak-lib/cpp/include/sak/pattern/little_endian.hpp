//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_1046606406
#define header_guard_1046606406


#include <sak/sak.hpp>
#include <bit>


namespace sak {
namespace pattern {


__using( ::sak::, byte, ensure )
__using( ::std::
	,bit_cast
	,size_t
	,string
	,uint16_t
	,uint32_t
	,vector
)


struct __read_little_u16
{
	constexpr auto operator ( ) ( const vector< byte >& data, size_t offset, const char* context ) const -> uint16_t
	{
		ensure( offset + 2 <= data.size( ), string( context ) + " read past end" );
		return	static_cast< uint16_t >( data[ offset ] | ( data[ offset + 1 ] << 8 ) );
	}
};
inline constexpr auto read_little_u16 = __read_little_u16{ };


struct __read_little_u32
{
	constexpr auto operator ( ) ( const vector< byte >& data, size_t offset, const char* context ) const -> uint32_t
	{
		ensure( offset + 4 <= data.size( ), string( context ) + " read past end" );
		return	static_cast< uint32_t >( data[ offset ] ) | ( static_cast< uint32_t >( data[ offset + 1 ] ) << 8 ) | ( static_cast< uint32_t >( data[ offset + 2 ] ) << 16 ) | ( static_cast< uint32_t >( data[ offset + 3 ] ) << 24 );
	}
};
inline constexpr auto read_little_u32 = __read_little_u32{ };


struct __read_little_float
{
	constexpr auto operator ( ) ( const vector< byte >& data, size_t offset, const char* context ) const -> float
	{
		return	bit_cast< float >( read_little_u32( data, offset, context ) );
	}
};
inline constexpr auto read_little_float = __read_little_float{ };


} }


#endif


