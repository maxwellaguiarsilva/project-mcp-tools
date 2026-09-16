//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_271625505
#define header_guard_271625505


//	__expand64__
#define __using_constexpr_name( p, a )	constexpr auto& a = p a;


#define __using_constexpr_1( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_2( p, __VA_ARGS__ ) )
#define __using_constexpr_2( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_3( p, __VA_ARGS__ ) )
#define __using_constexpr_3( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_4( p, __VA_ARGS__ ) )
#define __using_constexpr_4( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_5( p, __VA_ARGS__ ) )
#define __using_constexpr_5( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_6( p, __VA_ARGS__ ) )
#define __using_constexpr_6( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_7( p, __VA_ARGS__ ) )
#define __using_constexpr_7( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_8( p, __VA_ARGS__ ) )
#define __using_constexpr_8( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_9( p, __VA_ARGS__ ) )
#define __using_constexpr_9( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_10( p, __VA_ARGS__ ) )
#define __using_constexpr_10( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_11( p, __VA_ARGS__ ) )
#define __using_constexpr_11( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_12( p, __VA_ARGS__ ) )
#define __using_constexpr_12( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_13( p, __VA_ARGS__ ) )
#define __using_constexpr_13( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_14( p, __VA_ARGS__ ) )
#define __using_constexpr_14( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_15( p, __VA_ARGS__ ) )
#define __using_constexpr_15( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_16( p, __VA_ARGS__ ) )
#define __using_constexpr_16( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_17( p, __VA_ARGS__ ) )
#define __using_constexpr_17( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_18( p, __VA_ARGS__ ) )
#define __using_constexpr_18( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_19( p, __VA_ARGS__ ) )
#define __using_constexpr_19( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_20( p, __VA_ARGS__ ) )
#define __using_constexpr_20( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_21( p, __VA_ARGS__ ) )
#define __using_constexpr_21( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_22( p, __VA_ARGS__ ) )
#define __using_constexpr_22( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_23( p, __VA_ARGS__ ) )
#define __using_constexpr_23( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_24( p, __VA_ARGS__ ) )
#define __using_constexpr_24( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_25( p, __VA_ARGS__ ) )
#define __using_constexpr_25( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_26( p, __VA_ARGS__ ) )
#define __using_constexpr_26( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_27( p, __VA_ARGS__ ) )
#define __using_constexpr_27( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_28( p, __VA_ARGS__ ) )
#define __using_constexpr_28( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_29( p, __VA_ARGS__ ) )
#define __using_constexpr_29( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_30( p, __VA_ARGS__ ) )
#define __using_constexpr_30( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_31( p, __VA_ARGS__ ) )
#define __using_constexpr_31( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_32( p, __VA_ARGS__ ) )
#define __using_constexpr_32( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_33( p, __VA_ARGS__ ) )
#define __using_constexpr_33( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_34( p, __VA_ARGS__ ) )
#define __using_constexpr_34( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_35( p, __VA_ARGS__ ) )
#define __using_constexpr_35( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_36( p, __VA_ARGS__ ) )
#define __using_constexpr_36( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_37( p, __VA_ARGS__ ) )
#define __using_constexpr_37( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_38( p, __VA_ARGS__ ) )
#define __using_constexpr_38( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_39( p, __VA_ARGS__ ) )
#define __using_constexpr_39( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_40( p, __VA_ARGS__ ) )
#define __using_constexpr_40( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_41( p, __VA_ARGS__ ) )
#define __using_constexpr_41( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_42( p, __VA_ARGS__ ) )
#define __using_constexpr_42( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_43( p, __VA_ARGS__ ) )
#define __using_constexpr_43( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_44( p, __VA_ARGS__ ) )
#define __using_constexpr_44( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_45( p, __VA_ARGS__ ) )
#define __using_constexpr_45( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_46( p, __VA_ARGS__ ) )
#define __using_constexpr_46( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_47( p, __VA_ARGS__ ) )
#define __using_constexpr_47( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_48( p, __VA_ARGS__ ) )
#define __using_constexpr_48( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_49( p, __VA_ARGS__ ) )
#define __using_constexpr_49( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_50( p, __VA_ARGS__ ) )
#define __using_constexpr_50( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_51( p, __VA_ARGS__ ) )
#define __using_constexpr_51( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_52( p, __VA_ARGS__ ) )
#define __using_constexpr_52( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_53( p, __VA_ARGS__ ) )
#define __using_constexpr_53( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_54( p, __VA_ARGS__ ) )
#define __using_constexpr_54( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_55( p, __VA_ARGS__ ) )
#define __using_constexpr_55( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_56( p, __VA_ARGS__ ) )
#define __using_constexpr_56( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_57( p, __VA_ARGS__ ) )
#define __using_constexpr_57( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_58( p, __VA_ARGS__ ) )
#define __using_constexpr_58( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_59( p, __VA_ARGS__ ) )
#define __using_constexpr_59( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_60( p, __VA_ARGS__ ) )
#define __using_constexpr_60( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_61( p, __VA_ARGS__ ) )
#define __using_constexpr_61( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_62( p, __VA_ARGS__ ) )
#define __using_constexpr_62( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_63( p, __VA_ARGS__ ) )
#define __using_constexpr_63( p, a, ... )	__using_constexpr_name( p, a )	__VA_OPT__( __using_constexpr_64( p, __VA_ARGS__ ) )
#define __using_constexpr_64( p, a, ... )	__using_constexpr_name( p, a )


#define __using_constexpr( prefix, ... )	__VA_OPT__( __using_constexpr_1( prefix, __VA_ARGS__ ) )


#endif
