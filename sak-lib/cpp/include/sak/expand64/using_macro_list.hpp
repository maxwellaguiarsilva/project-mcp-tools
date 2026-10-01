//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_026497278
#define header_guard_026497278


#include <sak/expand64/using_macro.hpp>


#define __use_macro_list_unpack( ... )	__VA_ARGS__


//	__expand64__
#define __use_macro_list_name( a_args, a_macro )	__use_macro( a_macro, __use_macro_list_unpack a_args )


#define __use_macro_list_1( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_2( a_args, __VA_ARGS__ ) )
#define __use_macro_list_2( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_3( a_args, __VA_ARGS__ ) )
#define __use_macro_list_3( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_4( a_args, __VA_ARGS__ ) )
#define __use_macro_list_4( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_5( a_args, __VA_ARGS__ ) )
#define __use_macro_list_5( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_6( a_args, __VA_ARGS__ ) )
#define __use_macro_list_6( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_7( a_args, __VA_ARGS__ ) )
#define __use_macro_list_7( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_8( a_args, __VA_ARGS__ ) )
#define __use_macro_list_8( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_9( a_args, __VA_ARGS__ ) )
#define __use_macro_list_9( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_10( a_args, __VA_ARGS__ ) )
#define __use_macro_list_10( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_11( a_args, __VA_ARGS__ ) )
#define __use_macro_list_11( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_12( a_args, __VA_ARGS__ ) )
#define __use_macro_list_12( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_13( a_args, __VA_ARGS__ ) )
#define __use_macro_list_13( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_14( a_args, __VA_ARGS__ ) )
#define __use_macro_list_14( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_15( a_args, __VA_ARGS__ ) )
#define __use_macro_list_15( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_16( a_args, __VA_ARGS__ ) )
#define __use_macro_list_16( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_17( a_args, __VA_ARGS__ ) )
#define __use_macro_list_17( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_18( a_args, __VA_ARGS__ ) )
#define __use_macro_list_18( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_19( a_args, __VA_ARGS__ ) )
#define __use_macro_list_19( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_20( a_args, __VA_ARGS__ ) )
#define __use_macro_list_20( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_21( a_args, __VA_ARGS__ ) )
#define __use_macro_list_21( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_22( a_args, __VA_ARGS__ ) )
#define __use_macro_list_22( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_23( a_args, __VA_ARGS__ ) )
#define __use_macro_list_23( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_24( a_args, __VA_ARGS__ ) )
#define __use_macro_list_24( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_25( a_args, __VA_ARGS__ ) )
#define __use_macro_list_25( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_26( a_args, __VA_ARGS__ ) )
#define __use_macro_list_26( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_27( a_args, __VA_ARGS__ ) )
#define __use_macro_list_27( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_28( a_args, __VA_ARGS__ ) )
#define __use_macro_list_28( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_29( a_args, __VA_ARGS__ ) )
#define __use_macro_list_29( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_30( a_args, __VA_ARGS__ ) )
#define __use_macro_list_30( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_31( a_args, __VA_ARGS__ ) )
#define __use_macro_list_31( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_32( a_args, __VA_ARGS__ ) )
#define __use_macro_list_32( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_33( a_args, __VA_ARGS__ ) )
#define __use_macro_list_33( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_34( a_args, __VA_ARGS__ ) )
#define __use_macro_list_34( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_35( a_args, __VA_ARGS__ ) )
#define __use_macro_list_35( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_36( a_args, __VA_ARGS__ ) )
#define __use_macro_list_36( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_37( a_args, __VA_ARGS__ ) )
#define __use_macro_list_37( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_38( a_args, __VA_ARGS__ ) )
#define __use_macro_list_38( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_39( a_args, __VA_ARGS__ ) )
#define __use_macro_list_39( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_40( a_args, __VA_ARGS__ ) )
#define __use_macro_list_40( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_41( a_args, __VA_ARGS__ ) )
#define __use_macro_list_41( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_42( a_args, __VA_ARGS__ ) )
#define __use_macro_list_42( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_43( a_args, __VA_ARGS__ ) )
#define __use_macro_list_43( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_44( a_args, __VA_ARGS__ ) )
#define __use_macro_list_44( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_45( a_args, __VA_ARGS__ ) )
#define __use_macro_list_45( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_46( a_args, __VA_ARGS__ ) )
#define __use_macro_list_46( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_47( a_args, __VA_ARGS__ ) )
#define __use_macro_list_47( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_48( a_args, __VA_ARGS__ ) )
#define __use_macro_list_48( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_49( a_args, __VA_ARGS__ ) )
#define __use_macro_list_49( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_50( a_args, __VA_ARGS__ ) )
#define __use_macro_list_50( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_51( a_args, __VA_ARGS__ ) )
#define __use_macro_list_51( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_52( a_args, __VA_ARGS__ ) )
#define __use_macro_list_52( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_53( a_args, __VA_ARGS__ ) )
#define __use_macro_list_53( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_54( a_args, __VA_ARGS__ ) )
#define __use_macro_list_54( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_55( a_args, __VA_ARGS__ ) )
#define __use_macro_list_55( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_56( a_args, __VA_ARGS__ ) )
#define __use_macro_list_56( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_57( a_args, __VA_ARGS__ ) )
#define __use_macro_list_57( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_58( a_args, __VA_ARGS__ ) )
#define __use_macro_list_58( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_59( a_args, __VA_ARGS__ ) )
#define __use_macro_list_59( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_60( a_args, __VA_ARGS__ ) )
#define __use_macro_list_60( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_61( a_args, __VA_ARGS__ ) )
#define __use_macro_list_61( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_62( a_args, __VA_ARGS__ ) )
#define __use_macro_list_62( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_63( a_args, __VA_ARGS__ ) )
#define __use_macro_list_63( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )	__VA_OPT__( __use_macro_list_64( a_args, __VA_ARGS__ ) )
#define __use_macro_list_64( a_args, a_macro, ... )	__use_macro_list_name( a_args, a_macro )


#define __use_macro_list( a_args, ... )	__VA_OPT__( __use_macro_list_1( a_args, __VA_ARGS__ ) )


#endif


