//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_148476052
#define header_guard_148476052


#include <sak/sak.hpp>
#include <sak/meta/override.hpp>
#include <sak/ranges/index_of.hpp>
#include <array>
#include <atomic>
#include <bitset>
#include <exception>
#include <expected>
#include <functional>
#include <mutex>


namespace sak {
namespace pattern {


//	-----------------------------
__using( ::std::
	,array
	,atomic
	,bitset
	,current_exception
	,erase_if
	,exception_ptr
	,expected
	,function
	,invoke
	,is_base_of_v
	,lock_guard
	,memory_order_acquire
	,memory_order_release
	,mutex
	,shared_ptr
	,size_t
	,unexpected
	,vector
	,weak_ptr
)
__using( ::std::meta::, info )
__using( ::sak::meta::, overridden_methods, virtual_methods )
__using( ::sak::ranges::, index_of )
//	-----------------------------


//	number of dispatchable virtual methods declared by the listener interface
template< typename t_listener >
consteval auto listener_method_count( ) { return virtual_methods< t_listener >( ).size( ); }


//	subscription mask over the listener interface slots
template< typename t_listener >
using	subscription	=	bitset< listener_method_count< t_listener >( ) >;


//	subscription computed from the methods the derived listener actually overrides
template< typename t_listener, typename t_derived >
consteval auto subscription_of( )
{
	subscription< t_listener >	result;
	for( const auto& method : overridden_methods< t_listener, t_derived >( ) )
		result.set( *index_of( virtual_methods< t_listener >( ), method ) );
	return	result;
}


//	client contract: registration only, exposed to whoever wants to listen
template< typename t_listener >
class listener_registry
{
public:
	virtual ~listener_registry( ) = default;

	template< typename t_derived >
		requires is_base_of_v< t_listener, t_derived >
	auto operator +=( const shared_ptr< t_derived >& instance ) -> void
	{
		if( instance )
			add( instance, subscription_of< t_listener, t_derived >( ) );
	}

private:
	virtual auto add( const shared_ptr< t_listener >& instance, const subscription< t_listener >& subscribed ) -> void = 0;
};


//	server contract: registration plus typed dispatch, used by the host
template< typename t_listener >
class dispatcher final : public listener_registry< t_listener >
{
public:
	struct failed_info
	{
		weak_ptr< t_listener >	listener;
		exception_ptr			exception;
	};
	using	error	=	vector< failed_info >;
	using	result	=	expected< void, error >;

	dispatcher( ) = default;
	delete_copy_move_ctc( dispatcher );

	template< info t_method, typename... t_args >
	auto operator ( ) ( t_args&&... arguments ) -> result
	{
		constexpr auto index = *index_of( virtual_methods< t_listener >( ), t_method );
		return	run( index, [ & ]( t_listener* listener )
			{ invoke( &[: t_method :], listener, arguments... ); } );
	}

private:
	using	bucket	=	vector< weak_ptr< t_listener > >;

	auto run( const size_t index, const function< void( t_listener* ) >& invoke_method ) -> result
	{
		unsigned	clear_count;
		bucket		listeners_list;
		{
			auto lock		=	lock_guard( m_mutex );
			listeners_list	=	m_buckets[ index ];
			clear_count		=	m_clear_count.load( memory_order_acquire );
		}

		error	failed_list;
		bool	should_clear	=	false;
		for( const auto& current_listener : listeners_list )
			if( auto locked = current_listener.lock( ) )
				try {
					invoke_method( locked.get( ) );
				} catch( ... ) { failed_list.emplace_back( current_listener, current_exception( ) ); }
			else
				should_clear = true;

		if( should_clear )
			clear( clear_count );

		if( failed_list.empty( ) )
			return	{ };
		return	unexpected( failed_list );
	}

	auto add( const shared_ptr< t_listener >& instance, const subscription< t_listener >& subscribed ) -> void override
	{
		auto lock = lock_guard( m_mutex );
		for( auto index = 0uz; index < subscribed.size( ); ++index )
			if( subscribed.test( index ) )
				m_buckets[ index ].emplace_back( instance );
	}

	auto clear( const unsigned clear_count ) -> void
	{
		auto lock = lock_guard( m_mutex );
		if( clear_count not_eq m_clear_count.load( memory_order_acquire ) )
			return;
		for( auto& current_bucket : m_buckets )
			erase_if( current_bucket, [ ]( const auto& ptr ) { return ptr.expired( ); } );
		m_clear_count.fetch_add( 1, memory_order_release );
	}

	array< bucket, listener_method_count< t_listener >( ) >	m_buckets;
	mutex				m_mutex;
	atomic< unsigned >	m_clear_count	=	0;
};


} };


#endif


