//	
//	SPDX-FileCopyrightText: 2025 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_148476052
#define header_guard_148476052


#include <sak/sak.hpp>
#include <vector>
#include <expected>
#include <exception>
#include <atomic>
#include <mutex>


namespace sak {
namespace pattern {


//	-----------------------------
__using( ::std::
	,vector
	,erase_if
	,make_shared
	,shared_ptr
	,weak_ptr
	,expected
	,unexpected
	,exception_ptr
	,current_exception
	,atomic
	,memory_order_acquire
	,memory_order_release
	,mutex
	,lock_guard
	,invoke
)
//	-----------------------------


template< typename t_listener >
class dispatcher final
{
public:
	
	dispatcher( ) = default;
	delete_copy_move_ctc( dispatcher );
	
	struct failed_info
	{
		weak_ptr< t_listener >	listener;
		exception_ptr			exception;
	};
	using	error	=	vector< failed_info >;
	using	result	=	expected< void, error >;
	using	list	=	vector< weak_ptr< t_listener > >;
	
	void operator +=( const shared_ptr< t_listener >& instance )
	{
		if( not instance )
			return;
		auto lock = lock_guard( m_mutex );
		m_list.emplace_back( instance );
	}
	
	template< typename t_method, typename... t_call_args >
	result operator ( ) (
		 t_method member_function_pointer
		,t_call_args&&... arguments
	)
	{
		unsigned clear_count;
		list listeners_list;
		{
			auto lock		=	lock_guard( m_mutex );
			listeners_list	=	m_list;
			clear_count		=	m_clear_count.load( memory_order_acquire );
		};
		
		error   failed_list;
		bool should_clear = false;
		for( const auto& current_listener : listeners_list )
			if( auto locked = current_listener.lock( ) )
				try {
					invoke( member_function_pointer, locked.get( ), arguments... );
				} catch( ... ) { failed_list.emplace_back( current_listener, current_exception( ) ); }
			else
				should_clear = true;
		
		if( should_clear )
			clear( clear_count );
		
		if( failed_list.empty( ) )
			return	{ };
		return	unexpected( failed_list );
	}
	
private:
	list	m_list;
	mutex	m_mutex;
	atomic< unsigned >	m_clear_count	=	0;
	
	void clear( unsigned clear_count )
	{
		auto lock = lock_guard( m_mutex );
		if( clear_count not_eq m_clear_count.load( memory_order_acquire ) )
			return;
		erase_if( m_list, [ ]( const auto& ptr ) { return ptr.expired( ); } );
		m_clear_count.fetch_add( 1, memory_order_release );
	}
};


} };


#endif


