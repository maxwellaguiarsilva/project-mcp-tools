//	
//	SPDX-FileCopyrightText: 2025 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_065083550
#define header_guard_065083550


#define use_default_copy_ctc( class_name ) \
	class_name( const class_name& ) = default; \
	class_name& operator=( const class_name& ) = default;
#define use_default_move_ctc( class_name ) \
	class_name( class_name&& ) = default; \
	class_name& operator=( class_name&& ) = default;
#define use_default_copy_move_ctc( class_name ) \
	use_default_copy_ctc( class_name ); \
	use_default_move_ctc( class_name );

#define use_non_default_copy_ctc( class_name ) \
	class_name( const class_name& ); \
	class_name& operator=( const class_name& );
#define use_non_default_move_ctc( class_name ) \
	class_name( class_name&& ); \
	class_name& operator=( class_name&& );
#define use_non_default_copy_move_ctc( class_name ) \
	use_non_default_copy_ctc( class_name ); \
	use_non_default_move_ctc( class_name );

#define delete_copy_ctc( class_name ) \
	class_name( const class_name& ) = delete; \
	class_name& operator=( const class_name& ) = delete;
#define delete_move_ctc( class_name ) \
	class_name( class_name&& ) = delete; \
	class_name& operator=( class_name&& ) = delete;
#define delete_copy_move_ctc( class_name ) \
	delete_copy_ctc( class_name ); \
	delete_move_ctc( class_name );

#define use_default_dtc( class_name ) \
	virtual ~class_name( ) = default;
#define use_non_default_dtc( class_name ) \
	virtual ~class_name( );


#endif


