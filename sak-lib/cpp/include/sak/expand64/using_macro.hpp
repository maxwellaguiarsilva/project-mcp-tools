//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_870320738
#define header_guard_870320738


//	__expand64__
#define __use_macro_name( a_macro, a_args )	a_macro a_args


#define __use_macro_1( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_2( a_macro, __VA_ARGS__ ) )
#define __use_macro_2( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_3( a_macro, __VA_ARGS__ ) )
#define __use_macro_3( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_4( a_macro, __VA_ARGS__ ) )
#define __use_macro_4( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_5( a_macro, __VA_ARGS__ ) )
#define __use_macro_5( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_6( a_macro, __VA_ARGS__ ) )
#define __use_macro_6( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_7( a_macro, __VA_ARGS__ ) )
#define __use_macro_7( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_8( a_macro, __VA_ARGS__ ) )
#define __use_macro_8( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_9( a_macro, __VA_ARGS__ ) )
#define __use_macro_9( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_10( a_macro, __VA_ARGS__ ) )
#define __use_macro_10( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_11( a_macro, __VA_ARGS__ ) )
#define __use_macro_11( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_12( a_macro, __VA_ARGS__ ) )
#define __use_macro_12( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_13( a_macro, __VA_ARGS__ ) )
#define __use_macro_13( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_14( a_macro, __VA_ARGS__ ) )
#define __use_macro_14( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_15( a_macro, __VA_ARGS__ ) )
#define __use_macro_15( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_16( a_macro, __VA_ARGS__ ) )
#define __use_macro_16( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_17( a_macro, __VA_ARGS__ ) )
#define __use_macro_17( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_18( a_macro, __VA_ARGS__ ) )
#define __use_macro_18( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_19( a_macro, __VA_ARGS__ ) )
#define __use_macro_19( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_20( a_macro, __VA_ARGS__ ) )
#define __use_macro_20( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_21( a_macro, __VA_ARGS__ ) )
#define __use_macro_21( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_22( a_macro, __VA_ARGS__ ) )
#define __use_macro_22( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_23( a_macro, __VA_ARGS__ ) )
#define __use_macro_23( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_24( a_macro, __VA_ARGS__ ) )
#define __use_macro_24( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_25( a_macro, __VA_ARGS__ ) )
#define __use_macro_25( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_26( a_macro, __VA_ARGS__ ) )
#define __use_macro_26( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_27( a_macro, __VA_ARGS__ ) )
#define __use_macro_27( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_28( a_macro, __VA_ARGS__ ) )
#define __use_macro_28( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_29( a_macro, __VA_ARGS__ ) )
#define __use_macro_29( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_30( a_macro, __VA_ARGS__ ) )
#define __use_macro_30( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_31( a_macro, __VA_ARGS__ ) )
#define __use_macro_31( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_32( a_macro, __VA_ARGS__ ) )
#define __use_macro_32( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_33( a_macro, __VA_ARGS__ ) )
#define __use_macro_33( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_34( a_macro, __VA_ARGS__ ) )
#define __use_macro_34( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_35( a_macro, __VA_ARGS__ ) )
#define __use_macro_35( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_36( a_macro, __VA_ARGS__ ) )
#define __use_macro_36( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_37( a_macro, __VA_ARGS__ ) )
#define __use_macro_37( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_38( a_macro, __VA_ARGS__ ) )
#define __use_macro_38( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_39( a_macro, __VA_ARGS__ ) )
#define __use_macro_39( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_40( a_macro, __VA_ARGS__ ) )
#define __use_macro_40( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_41( a_macro, __VA_ARGS__ ) )
#define __use_macro_41( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_42( a_macro, __VA_ARGS__ ) )
#define __use_macro_42( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_43( a_macro, __VA_ARGS__ ) )
#define __use_macro_43( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_44( a_macro, __VA_ARGS__ ) )
#define __use_macro_44( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_45( a_macro, __VA_ARGS__ ) )
#define __use_macro_45( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_46( a_macro, __VA_ARGS__ ) )
#define __use_macro_46( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_47( a_macro, __VA_ARGS__ ) )
#define __use_macro_47( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_48( a_macro, __VA_ARGS__ ) )
#define __use_macro_48( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_49( a_macro, __VA_ARGS__ ) )
#define __use_macro_49( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_50( a_macro, __VA_ARGS__ ) )
#define __use_macro_50( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_51( a_macro, __VA_ARGS__ ) )
#define __use_macro_51( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_52( a_macro, __VA_ARGS__ ) )
#define __use_macro_52( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_53( a_macro, __VA_ARGS__ ) )
#define __use_macro_53( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_54( a_macro, __VA_ARGS__ ) )
#define __use_macro_54( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_55( a_macro, __VA_ARGS__ ) )
#define __use_macro_55( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_56( a_macro, __VA_ARGS__ ) )
#define __use_macro_56( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_57( a_macro, __VA_ARGS__ ) )
#define __use_macro_57( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_58( a_macro, __VA_ARGS__ ) )
#define __use_macro_58( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_59( a_macro, __VA_ARGS__ ) )
#define __use_macro_59( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_60( a_macro, __VA_ARGS__ ) )
#define __use_macro_60( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_61( a_macro, __VA_ARGS__ ) )
#define __use_macro_61( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_62( a_macro, __VA_ARGS__ ) )
#define __use_macro_62( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_63( a_macro, __VA_ARGS__ ) )
#define __use_macro_63( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )	__VA_OPT__( __use_macro_64( a_macro, __VA_ARGS__ ) )
#define __use_macro_64( a_macro, a_args, ... )	__use_macro_name( a_macro, a_args )


#define __use_macro( a_macro, ... )	__VA_OPT__( __use_macro_1( a_macro, __VA_ARGS__ ) )


#endif


