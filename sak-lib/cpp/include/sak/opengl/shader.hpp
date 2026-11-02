//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_015356114
#define header_guard_015356114


#include <map>
#include <meta>
#include <sak/fso/text_file.hpp>
#include <sak/opengl/fetch_log.hpp>
#include <sak/ranges/contains.hpp>
#include <sak/ranges/to.hpp>
#include <sak/ranges/views/regex_matches.hpp>


namespace sak {
namespace opengl {


__using( ::std::
	,convertible_to
	,define_static_array
	,is_trivially_copyable_v
	,make_unique
	,map
	,pair
	,ptrdiff_t
	,regex
	,runtime_error
	,size_t
	,smatch
	,string
	,string_view
	,unique_ptr
	,vector
)
__using( ::std::meta::
	,access_context
	,enumerators_of
	,identifier_of
	,info
	,nonstatic_data_members_of
	,offset_of
	,size_of
	,type_of
)
__using( ::std::ranges::, input_range, range_reference_t )
__using( ::std::regex_constants::, ECMAScript, multiline )
__using( ::std::views::, join_with, keys, transform )
__using( ::sak::, ensure )
__using( ::sak::fso::, text_file )
__using( ::sak::ranges::, contains, to )
__using( ::sak::ranges::views::, regex_matches )


class shader;


//	a type whose bytes are a contiguous payload with no padding, matching the flat float arrays read on the gpu
consteval auto has_no_padding( const info type ) -> bool
{
	ptrdiff_t offset = 0;
	for( const auto member : nonstatic_data_members_of( type, access_context::current( ) ) )
	{
		if( offset_of( member ).bytes not_eq offset )
			return	false;
		offset += size_of( type_of( member ) );
	}
	return	offset == static_cast< ptrdiff_t >( size_of( type ) );
}

template< typename t_type >
concept is_flat = has_no_padding( ^^t_type ) and is_trivially_copyable_v< t_type >;


template< typename t_range >
concept is_shader_range = input_range< t_range > and convertible_to<
	 range_reference_t< t_range >
	,const shader&
>;


class shader
{
public:
	enum class type : GLenum
	{
		 vertex				=	GL_VERTEX_SHADER
		,fragment			=	GL_FRAGMENT_SHADER
		,geometry			=	GL_GEOMETRY_SHADER
		,tess_control		=	GL_TESS_CONTROL_SHADER
		,tess_evaluation	=	GL_TESS_EVALUATION_SHADER
		,compute			=	GL_COMPUTE_SHADER
	};
	using	enum	type;
	using	table	=	map< type, shader >;

	//	combined glsl files are split on marker lines and compiled once per name
	class loader
	{
	public:
		explicit loader( const string& base_path = "data/shader" ) :m_base_path( base_path ) { }

		auto operator[ ]( const string& name ) -> const table&
		{
			if( not m_cache.contains( name ) )
				m_cache.emplace( name, load( name ) );
			return	m_cache.at( name );
		}

	private:
		auto load( const string& name ) const -> table
		{
			text_file file( m_base_path + "/" + name + ".glsl" );
			ensure( file.exists( ), "unable to read shader file: " + name );

			map< string, type > kind_by_name;
			template for( constexpr auto enumerator : define_static_array( enumerators_of( ^^type ) ) )
				kind_by_name.emplace( string( identifier_of( enumerator ) ), [: enumerator :] );

			const string alternation = kind_by_name | keys | join_with( string_view{ "|" } ) | to;
			const string expression = "^//\\t(" + alternation + ")-shader[\\r\\n]([\\s\\S]*?)^//\\t\\1-shader";
			const vector< pair< string, string > > sections = file.content( )
				|	regex_matches( regex( expression, ECMAScript | multiline ) )
				|	transform( [ ]( const smatch& match ) { return pair{ match[ 1 ].str( ), match[ 2 ].str( ) }; } )
				|	to;

			table shaders;
			for( const auto& [ section_name, body ] : sections )
			{
				const type kind = kind_by_name.at( section_name );
				ensure( not contains( shaders, kind ), "duplicate shader section: " + section_name + " in " + name );
				shaders.try_emplace( kind, body, kind );
			}

			ensure( not shaders.empty( ), "no shader section found in shader file: " + name );
			return	shaders;
		}

		string m_base_path;
		map< string, table > m_cache;
	};

	//	raii shader storage buffer owning a heap array of packed payloads; binding is a separate explicit step
	template< typename t_payload >
	class storage_buffer
	{
	public:
		static_assert( is_flat< t_payload >, "the ssbo payload must be a tightly packed flat structure" );

		//	element count and float stride prefixed to every ssbo payload so the shader iterates generically
		struct ssbo_header { GLuint m_count; GLuint m_stride; };

		storage_buffer( const size_t count )
			:m_raii( make_unique< raii_destructor >( ) )
			,m_data( make_unique< t_payload[ ] >( count ) )
			,m_count( count )
			,m_payload_size( count * sizeof( t_payload ) )
			,m_byte_size( sizeof( ssbo_header ) + m_payload_size )
		{
			GLuint buffer_id = 0;
			gl_create_buffers( 1, &buffer_id );
			id( buffer_id );
			gl_named_buffer_storage( id( ), m_byte_size, nullptr, GL_DYNAMIC_STORAGE_BIT );
			const ssbo_header header{ static_cast< GLuint >( count ), sizeof( t_payload ) / sizeof( float ) };
			gl_named_buffer_sub_data( id( ), 0, sizeof( header ), &header );
		}

		auto bind( const GLuint index ) const noexcept -> void { gl_bind_buffer_base( GL_SHADER_STORAGE_BUFFER, index, id( ) ); }
		auto flush( ) const noexcept -> void { gl_named_buffer_sub_data( id( ), sizeof( ssbo_header ), m_payload_size, m_data.get( ) ); }

		auto operator[ ]( const size_t index ) noexcept -> t_payload& { return m_data[ index ]; }
		auto data( ) const noexcept -> t_payload* { return m_data.get( ); }
		auto count( ) const noexcept -> size_t { return m_count; }

	private:
		auto id( ) const noexcept -> GLuint { return m_raii->m_id; }
		auto id( const GLuint value ) noexcept -> void { m_raii->m_id = value; }

		struct raii_destructor
		{
			~raii_destructor( ) noexcept { gl_delete_buffers( 1, &m_id ); }
			GLuint	m_id{ 0 };
		};

		unique_ptr< raii_destructor > m_raii;
		unique_ptr< t_payload[ ] > m_data;
		size_t m_count;
		const size_t m_payload_size;
		const size_t m_byte_size;
	};


	shader( const string& source, const type shader_type = vertex )
		:m_raii( make_unique< raii_destructor >( ) )
	{
		id( gl_create_shader( static_cast< GLenum >( shader_type ) ) );

		const char* const source_cstr	=	source.c_str( );
		gl_shader_source( id( ), 1, &source_cstr, nullptr );
		gl_compile_shader( id( ) );

		int success = 0;
		gl_get_shader_iv( id( ), GL_COMPILE_STATUS, &success );
		if( not success )
		{
			const string info_log = fetch_log( gl_get_shader_info_log, id( ) );
			throw	runtime_error( info_log );
		}
	}

private:
	template< is_shader_range t_range >
	friend class program;

	auto id( ) const noexcept -> GLuint { return m_raii->m_id; }
	auto id( const GLuint value ) noexcept -> void { m_raii->m_id = value; }

	struct raii_destructor
	{
		~raii_destructor( ) noexcept { gl_delete_shader( m_id ); }

		GLuint	m_id{ 0 };
	};

	unique_ptr< raii_destructor > m_raii;
};


} } 


#endif


