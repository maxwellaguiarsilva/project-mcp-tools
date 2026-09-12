//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_341997975
#define header_guard_341997975


//	__expand64__
#define __using_inline_name( p, a )	inline constexpr auto& a = p a;


#define __using_inline_1( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_2( p, __VA_ARGS__ ) )
#define __using_inline_2( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_3( p, __VA_ARGS__ ) )
#define __using_inline_3( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_4( p, __VA_ARGS__ ) )
#define __using_inline_4( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_5( p, __VA_ARGS__ ) )
#define __using_inline_5( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_6( p, __VA_ARGS__ ) )
#define __using_inline_6( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_7( p, __VA_ARGS__ ) )
#define __using_inline_7( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_8( p, __VA_ARGS__ ) )
#define __using_inline_8( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_9( p, __VA_ARGS__ ) )
#define __using_inline_9( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_10( p, __VA_ARGS__ ) )
#define __using_inline_10( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_11( p, __VA_ARGS__ ) )
#define __using_inline_11( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_12( p, __VA_ARGS__ ) )
#define __using_inline_12( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_13( p, __VA_ARGS__ ) )
#define __using_inline_13( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_14( p, __VA_ARGS__ ) )
#define __using_inline_14( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_15( p, __VA_ARGS__ ) )
#define __using_inline_15( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_16( p, __VA_ARGS__ ) )
#define __using_inline_16( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_17( p, __VA_ARGS__ ) )
#define __using_inline_17( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_18( p, __VA_ARGS__ ) )
#define __using_inline_18( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_19( p, __VA_ARGS__ ) )
#define __using_inline_19( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_20( p, __VA_ARGS__ ) )
#define __using_inline_20( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_21( p, __VA_ARGS__ ) )
#define __using_inline_21( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_22( p, __VA_ARGS__ ) )
#define __using_inline_22( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_23( p, __VA_ARGS__ ) )
#define __using_inline_23( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_24( p, __VA_ARGS__ ) )
#define __using_inline_24( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_25( p, __VA_ARGS__ ) )
#define __using_inline_25( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_26( p, __VA_ARGS__ ) )
#define __using_inline_26( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_27( p, __VA_ARGS__ ) )
#define __using_inline_27( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_28( p, __VA_ARGS__ ) )
#define __using_inline_28( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_29( p, __VA_ARGS__ ) )
#define __using_inline_29( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_30( p, __VA_ARGS__ ) )
#define __using_inline_30( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_31( p, __VA_ARGS__ ) )
#define __using_inline_31( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_32( p, __VA_ARGS__ ) )
#define __using_inline_32( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_33( p, __VA_ARGS__ ) )
#define __using_inline_33( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_34( p, __VA_ARGS__ ) )
#define __using_inline_34( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_35( p, __VA_ARGS__ ) )
#define __using_inline_35( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_36( p, __VA_ARGS__ ) )
#define __using_inline_36( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_37( p, __VA_ARGS__ ) )
#define __using_inline_37( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_38( p, __VA_ARGS__ ) )
#define __using_inline_38( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_39( p, __VA_ARGS__ ) )
#define __using_inline_39( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_40( p, __VA_ARGS__ ) )
#define __using_inline_40( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_41( p, __VA_ARGS__ ) )
#define __using_inline_41( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_42( p, __VA_ARGS__ ) )
#define __using_inline_42( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_43( p, __VA_ARGS__ ) )
#define __using_inline_43( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_44( p, __VA_ARGS__ ) )
#define __using_inline_44( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_45( p, __VA_ARGS__ ) )
#define __using_inline_45( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_46( p, __VA_ARGS__ ) )
#define __using_inline_46( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_47( p, __VA_ARGS__ ) )
#define __using_inline_47( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_48( p, __VA_ARGS__ ) )
#define __using_inline_48( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_49( p, __VA_ARGS__ ) )
#define __using_inline_49( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_50( p, __VA_ARGS__ ) )
#define __using_inline_50( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_51( p, __VA_ARGS__ ) )
#define __using_inline_51( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_52( p, __VA_ARGS__ ) )
#define __using_inline_52( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_53( p, __VA_ARGS__ ) )
#define __using_inline_53( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_54( p, __VA_ARGS__ ) )
#define __using_inline_54( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_55( p, __VA_ARGS__ ) )
#define __using_inline_55( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_56( p, __VA_ARGS__ ) )
#define __using_inline_56( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_57( p, __VA_ARGS__ ) )
#define __using_inline_57( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_58( p, __VA_ARGS__ ) )
#define __using_inline_58( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_59( p, __VA_ARGS__ ) )
#define __using_inline_59( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_60( p, __VA_ARGS__ ) )
#define __using_inline_60( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_61( p, __VA_ARGS__ ) )
#define __using_inline_61( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_62( p, __VA_ARGS__ ) )
#define __using_inline_62( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_63( p, __VA_ARGS__ ) )
#define __using_inline_63( p, a, ... )	__using_inline_name( p, a )	__VA_OPT__( __using_inline_64( p, __VA_ARGS__ ) )
#define __using_inline_64( p, a, ... )	__using_inline_name( p, a )


#define __using_inline( prefix, ... )	__VA_OPT__( __using_inline_1( prefix, __VA_ARGS__ ) )


#endif


