//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_398000282
#define header_guard_398000282


//	__expand64__
#define __using_static_name( p, a )	static constexpr auto& a = p a;


#define __using_static_1( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_2( p, __VA_ARGS__ ) )
#define __using_static_2( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_3( p, __VA_ARGS__ ) )
#define __using_static_3( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_4( p, __VA_ARGS__ ) )
#define __using_static_4( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_5( p, __VA_ARGS__ ) )
#define __using_static_5( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_6( p, __VA_ARGS__ ) )
#define __using_static_6( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_7( p, __VA_ARGS__ ) )
#define __using_static_7( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_8( p, __VA_ARGS__ ) )
#define __using_static_8( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_9( p, __VA_ARGS__ ) )
#define __using_static_9( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_10( p, __VA_ARGS__ ) )
#define __using_static_10( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_11( p, __VA_ARGS__ ) )
#define __using_static_11( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_12( p, __VA_ARGS__ ) )
#define __using_static_12( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_13( p, __VA_ARGS__ ) )
#define __using_static_13( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_14( p, __VA_ARGS__ ) )
#define __using_static_14( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_15( p, __VA_ARGS__ ) )
#define __using_static_15( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_16( p, __VA_ARGS__ ) )
#define __using_static_16( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_17( p, __VA_ARGS__ ) )
#define __using_static_17( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_18( p, __VA_ARGS__ ) )
#define __using_static_18( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_19( p, __VA_ARGS__ ) )
#define __using_static_19( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_20( p, __VA_ARGS__ ) )
#define __using_static_20( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_21( p, __VA_ARGS__ ) )
#define __using_static_21( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_22( p, __VA_ARGS__ ) )
#define __using_static_22( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_23( p, __VA_ARGS__ ) )
#define __using_static_23( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_24( p, __VA_ARGS__ ) )
#define __using_static_24( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_25( p, __VA_ARGS__ ) )
#define __using_static_25( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_26( p, __VA_ARGS__ ) )
#define __using_static_26( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_27( p, __VA_ARGS__ ) )
#define __using_static_27( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_28( p, __VA_ARGS__ ) )
#define __using_static_28( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_29( p, __VA_ARGS__ ) )
#define __using_static_29( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_30( p, __VA_ARGS__ ) )
#define __using_static_30( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_31( p, __VA_ARGS__ ) )
#define __using_static_31( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_32( p, __VA_ARGS__ ) )
#define __using_static_32( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_33( p, __VA_ARGS__ ) )
#define __using_static_33( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_34( p, __VA_ARGS__ ) )
#define __using_static_34( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_35( p, __VA_ARGS__ ) )
#define __using_static_35( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_36( p, __VA_ARGS__ ) )
#define __using_static_36( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_37( p, __VA_ARGS__ ) )
#define __using_static_37( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_38( p, __VA_ARGS__ ) )
#define __using_static_38( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_39( p, __VA_ARGS__ ) )
#define __using_static_39( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_40( p, __VA_ARGS__ ) )
#define __using_static_40( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_41( p, __VA_ARGS__ ) )
#define __using_static_41( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_42( p, __VA_ARGS__ ) )
#define __using_static_42( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_43( p, __VA_ARGS__ ) )
#define __using_static_43( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_44( p, __VA_ARGS__ ) )
#define __using_static_44( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_45( p, __VA_ARGS__ ) )
#define __using_static_45( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_46( p, __VA_ARGS__ ) )
#define __using_static_46( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_47( p, __VA_ARGS__ ) )
#define __using_static_47( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_48( p, __VA_ARGS__ ) )
#define __using_static_48( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_49( p, __VA_ARGS__ ) )
#define __using_static_49( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_50( p, __VA_ARGS__ ) )
#define __using_static_50( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_51( p, __VA_ARGS__ ) )
#define __using_static_51( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_52( p, __VA_ARGS__ ) )
#define __using_static_52( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_53( p, __VA_ARGS__ ) )
#define __using_static_53( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_54( p, __VA_ARGS__ ) )
#define __using_static_54( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_55( p, __VA_ARGS__ ) )
#define __using_static_55( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_56( p, __VA_ARGS__ ) )
#define __using_static_56( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_57( p, __VA_ARGS__ ) )
#define __using_static_57( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_58( p, __VA_ARGS__ ) )
#define __using_static_58( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_59( p, __VA_ARGS__ ) )
#define __using_static_59( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_60( p, __VA_ARGS__ ) )
#define __using_static_60( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_61( p, __VA_ARGS__ ) )
#define __using_static_61( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_62( p, __VA_ARGS__ ) )
#define __using_static_62( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_63( p, __VA_ARGS__ ) )
#define __using_static_63( p, a, ... )	__using_static_name( p, a )	__VA_OPT__( __using_static_64( p, __VA_ARGS__ ) )
#define __using_static_64( p, a, ... )	__using_static_name( p, a )


#define __using_static( prefix, ... )	__VA_OPT__( __using_static_1( prefix, __VA_ARGS__ ) )


#endif


