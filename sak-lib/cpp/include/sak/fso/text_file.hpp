//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_555213248
#define header_guard_555213248


#include <sak/fso/file.hpp>
#include <fstream>
#include <ios>
#include <string_view>


namespace sak {
namespace fso {


__using( ::std::
	,ifstream
	,istreambuf_iterator
	,ofstream
	,optional
	,streamsize
	,string
	,string_view
)
__using( ::std::filesystem::, create_directories )
__using( ::sak::, ensure )


class text_file final : public file
{
public:
	using	optional_content	=	optional< string >;

	explicit text_file( path_type source_path )
		:file( ::std::move( source_path ) )
	{
		read( );
	}
	use_default_dtc( text_file );
	use_default_copy_move_ctc( text_file );

	auto content( ) const noexcept -> const optional_content& { return m_content; }

	auto read( ) -> const optional_content&
	{
		if( exists( ) )
		{
			ifstream input_stream( path( ) );
			ensure( input_stream.is_open( ), "text_file: unable to read file: " + path( ).string( ) );
			m_content = string( istreambuf_iterator< char >( input_stream ), istreambuf_iterator< char >{ } );
		}
		return	m_content;
	}

	auto write( const string_view new_content ) -> string
	{
		const auto parent = path( ).parent_path( );
		if( not parent.empty( ) )
			create_directories( parent );
		ofstream output_stream( path( ) );
		ensure( output_stream.is_open( ), "text_file: unable to write file: " + path( ).string( ) );
		output_stream.write( new_content.data( ), static_cast< streamsize >( new_content.size( ) ) );
		output_stream.close( );
		ensure( output_stream.good( ), "text_file: failed to write file: " + path( ).string( ) );
		refresh( );
		return	"created file: " + path( ).string( ) + "\n";
	}

	auto refresh( ) -> void override
	{
		file::refresh( );
		read( );
	}

private:
	optional_content	m_content;
};


} } 


#endif


