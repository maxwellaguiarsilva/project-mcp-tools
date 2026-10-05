//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_495581557
#define header_guard_495581557


#include <sak/sdl3/opengl/loader.hpp>
#include <sak/sdl3/opengl/attributes.hpp>
#include <sak/sdl3/window.hpp>


namespace sak {
namespace sdl3 {
namespace opengl {


__using( ::sak::, ensure )
__using( ::std::, make_shared, make_unique, shared_ptr, unique_ptr )


class context
{
public:
	using	loader_type	=	decltype( &SDL_GL_GetProcAddress );

	template< typename t_loader >
		requires loader_for< t_loader, loader_type >
	explicit context( window& application_window, t_loader load, const attributes& = attributes{ } )
		:m_raii( make_unique< raii_destructor >( ) )
	{
		m_raii->m_id	=	SDL_GL_CreateContext( application_window.id( ) );
		ensure( m_raii->m_id not_eq nullptr, "failed to create opengl context" );
		ensure( load( &SDL_GL_GetProcAddress ), "failed to load opengl functions" );

		m_viewport_listener = make_shared< viewport_listener >( );
		application_window.listeners( ) += m_viewport_listener;
	}

	template< typename t_loader = detected_loader< > >
		requires ( t_loader::available )
	explicit context( window& application_window, const attributes& gl_attributes = attributes{ } )
		: context( application_window, t_loader{ }, gl_attributes )
	{ }

	auto id( ) const noexcept -> SDL_GLContext { return m_raii->m_id; }
	auto function_pointer( ) const noexcept -> loader_type { return &SDL_GL_GetProcAddress; }

private:
	class viewport_listener final : public window::listener
	{
	public:
		using	geometry	=	::sak::g2i;
		__using_static( geometry::, width, height )

		void pixel_resize( const geometry::size& new_size ) override
		{
			gl_viewport( 0, 0, width( new_size ), height( new_size ) );
		}
	};

	struct raii_destructor
	{
		~raii_destructor( ) noexcept { SDL_GL_DestroyContext( m_id ); }

		SDL_GLContext	m_id{ nullptr };
	};

	unique_ptr< raii_destructor >			m_raii;
	shared_ptr< viewport_listener >	m_viewport_listener;
};


} } } 


#endif


