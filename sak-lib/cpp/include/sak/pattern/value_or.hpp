//	
//	SPDX-FileCopyrightText: 2025 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_1735295400
#define header_guard_1735295400


namespace sak {
namespace pattern {


struct __value_or
{
	template< typename t_container >
	requires( requires( const t_container& container, const typename t_container::key_type& key ) { container.find( key ); } )
	constexpr auto operator ( ) ( const t_container& container, const typename t_container::key_type& key, const typename t_container::mapped_type& default_value ) const noexcept -> const typename t_container::mapped_type&
	{
		const auto iterator = container.find( key );
		return	iterator not_eq container.end( ) ? iterator->second : default_value;
	}

	template< typename t_container >
	requires( not requires( const t_container& container, const typename t_container::key_type& key ) { container.find( key ); }
		and requires( const t_container& container, const typename t_container::size_type& index ) { container[ index ]; } )
	constexpr auto operator ( ) ( const t_container& container, const typename t_container::size_type& index, const typename t_container::value_type& default_value ) const noexcept -> const typename t_container::value_type&
	{
		return	index < container.size( ) ? container[ index ] : default_value;
	}
};
inline constexpr auto value_or = __value_or{ };


} }


#endif


