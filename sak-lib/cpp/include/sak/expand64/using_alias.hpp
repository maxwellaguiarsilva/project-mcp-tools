//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_101670672
#define header_guard_101670672


//	__expand64__
#define __using_alias_name( p, a )	using	a = p a;


#define __using_alias_1( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_2( p, __VA_ARGS__ ) )
#define __using_alias_2( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_3( p, __VA_ARGS__ ) )
#define __using_alias_3( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_4( p, __VA_ARGS__ ) )
#define __using_alias_4( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_5( p, __VA_ARGS__ ) )
#define __using_alias_5( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_6( p, __VA_ARGS__ ) )
#define __using_alias_6( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_7( p, __VA_ARGS__ ) )
#define __using_alias_7( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_8( p, __VA_ARGS__ ) )
#define __using_alias_8( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_9( p, __VA_ARGS__ ) )
#define __using_alias_9( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_10( p, __VA_ARGS__ ) )
#define __using_alias_10( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_11( p, __VA_ARGS__ ) )
#define __using_alias_11( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_12( p, __VA_ARGS__ ) )
#define __using_alias_12( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_13( p, __VA_ARGS__ ) )
#define __using_alias_13( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_14( p, __VA_ARGS__ ) )
#define __using_alias_14( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_15( p, __VA_ARGS__ ) )
#define __using_alias_15( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_16( p, __VA_ARGS__ ) )
#define __using_alias_16( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_17( p, __VA_ARGS__ ) )
#define __using_alias_17( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_18( p, __VA_ARGS__ ) )
#define __using_alias_18( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_19( p, __VA_ARGS__ ) )
#define __using_alias_19( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_20( p, __VA_ARGS__ ) )
#define __using_alias_20( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_21( p, __VA_ARGS__ ) )
#define __using_alias_21( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_22( p, __VA_ARGS__ ) )
#define __using_alias_22( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_23( p, __VA_ARGS__ ) )
#define __using_alias_23( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_24( p, __VA_ARGS__ ) )
#define __using_alias_24( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_25( p, __VA_ARGS__ ) )
#define __using_alias_25( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_26( p, __VA_ARGS__ ) )
#define __using_alias_26( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_27( p, __VA_ARGS__ ) )
#define __using_alias_27( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_28( p, __VA_ARGS__ ) )
#define __using_alias_28( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_29( p, __VA_ARGS__ ) )
#define __using_alias_29( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_30( p, __VA_ARGS__ ) )
#define __using_alias_30( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_31( p, __VA_ARGS__ ) )
#define __using_alias_31( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_32( p, __VA_ARGS__ ) )
#define __using_alias_32( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_33( p, __VA_ARGS__ ) )
#define __using_alias_33( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_34( p, __VA_ARGS__ ) )
#define __using_alias_34( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_35( p, __VA_ARGS__ ) )
#define __using_alias_35( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_36( p, __VA_ARGS__ ) )
#define __using_alias_36( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_37( p, __VA_ARGS__ ) )
#define __using_alias_37( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_38( p, __VA_ARGS__ ) )
#define __using_alias_38( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_39( p, __VA_ARGS__ ) )
#define __using_alias_39( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_40( p, __VA_ARGS__ ) )
#define __using_alias_40( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_41( p, __VA_ARGS__ ) )
#define __using_alias_41( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_42( p, __VA_ARGS__ ) )
#define __using_alias_42( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_43( p, __VA_ARGS__ ) )
#define __using_alias_43( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_44( p, __VA_ARGS__ ) )
#define __using_alias_44( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_45( p, __VA_ARGS__ ) )
#define __using_alias_45( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_46( p, __VA_ARGS__ ) )
#define __using_alias_46( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_47( p, __VA_ARGS__ ) )
#define __using_alias_47( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_48( p, __VA_ARGS__ ) )
#define __using_alias_48( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_49( p, __VA_ARGS__ ) )
#define __using_alias_49( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_50( p, __VA_ARGS__ ) )
#define __using_alias_50( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_51( p, __VA_ARGS__ ) )
#define __using_alias_51( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_52( p, __VA_ARGS__ ) )
#define __using_alias_52( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_53( p, __VA_ARGS__ ) )
#define __using_alias_53( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_54( p, __VA_ARGS__ ) )
#define __using_alias_54( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_55( p, __VA_ARGS__ ) )
#define __using_alias_55( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_56( p, __VA_ARGS__ ) )
#define __using_alias_56( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_57( p, __VA_ARGS__ ) )
#define __using_alias_57( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_58( p, __VA_ARGS__ ) )
#define __using_alias_58( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_59( p, __VA_ARGS__ ) )
#define __using_alias_59( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_60( p, __VA_ARGS__ ) )
#define __using_alias_60( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_61( p, __VA_ARGS__ ) )
#define __using_alias_61( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_62( p, __VA_ARGS__ ) )
#define __using_alias_62( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_63( p, __VA_ARGS__ ) )
#define __using_alias_63( p, a, ... )	__using_alias_name( p, a )	__VA_OPT__( __using_alias_64( p, __VA_ARGS__ ) )
#define __using_alias_64( p, a, ... )	__using_alias_name( p, a )


#define __using_alias( prefix, ... )	__VA_OPT__( __using_alias_1( prefix, __VA_ARGS__ ) )


#endif


