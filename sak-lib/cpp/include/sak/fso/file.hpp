//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_529699876
#define header_guard_529699876


#include <sak/sak.hpp>
#include <chrono>
#include <filesystem>
#include <system_error>
#include <sys/stat.h>


namespace sak {
namespace fso {


__using( ::std::, error_code, string )
__using( ::std::chrono::, clock_cast, nanoseconds, system_clock )
__using( ::std::filesystem::, last_write_time )


class file
{
public:
	using	path_type		=	::std::filesystem::path;
	using	time_point		=	system_clock::time_point;

	explicit file( path_type source_path )
		:m_path( ::std::move( source_path ) )
	{
		refresh_components( );
		refresh( );
	}
	use_default_dtc( file );
	use_default_copy_move_ctc( file );

	auto	path( ) const noexcept -> const path_type& { return m_path; }
	auto	base( ) const noexcept -> const path_type& { return m_base; }
	auto	folder( ) const noexcept -> const string& { return m_folder; }
	auto	name( ) const noexcept -> const string& { return m_name; }
	auto	extension( ) const noexcept -> const string& { return m_extension; }
	auto	exists( ) const noexcept -> bool { return m_exists; }
	auto	modified_at( ) const noexcept -> const time_point& { return m_modified_at; }
	auto	created_at( ) const noexcept -> const time_point& { return m_created_at; }

	auto	path( path_type source_path ) -> void
	{
		m_path = ::std::move( source_path );
		refresh_components( );
		refresh( );
	}

	virtual auto refresh( ) -> void
	{
		error_code error;
		m_exists = ::std::filesystem::exists( m_path, error );
		m_modified_at = time_point{ };
		m_created_at = time_point{ };
		if( not m_exists )
			return;
		const auto file_timestamp = last_write_time( m_path, error );
		if( not error )
			m_modified_at = clock_cast< system_clock >( file_timestamp );
		struct ::stat file_status{ };
		if( ::stat( m_path.c_str( ), &file_status ) == 0 )
			m_created_at = system_clock::from_time_t( file_status.st_ctim.tv_sec ) + nanoseconds( file_status.st_ctim.tv_nsec );
	}

private:
	auto refresh_components( ) -> void
	{
		m_base		=	m_path.parent_path( );
		m_folder	=	m_base.filename( ).string( );
		m_name		=	m_path.stem( ).string( );
		m_extension	=	m_path.extension( ).string( );
		if( not m_extension.empty( ) and m_extension.front( ) == '.' )
			m_extension.erase( 0, 1 );
	}

	path_type		m_path;
	path_type		m_base;
	string			m_folder;
	string			m_name;
	string			m_extension;
	bool			m_exists = false;
	time_point		m_modified_at{ };
	time_point		m_created_at{ };
};


} } 


#endif


