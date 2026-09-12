//	
//	SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
//	SPDX-License-Identifier: GPL-3.0-or-later
//	


#pragma once
#ifndef header_guard_107605823
#define header_guard_107605823


#include <sak/geometry/point.hpp>


namespace sak {


__using( ::std::
	,forward_iterator_tag
	,ptrdiff_t
	,default_sentinel
	,default_sentinel_t
)
__using( ::std::ranges::
	,view_interface
)
__using( ::sak::math::
	,absolute
	,sign
	,max
	,bind_back
	,greater_equal
)
__using( ::sak::ranges::
	,to
	,lazy_transform
)
__using( ::sak::
	,is_point
)


template< is_point t_point >
class line_view : public view_interface< line_view< t_point > >
{
public:
	using	value_type	=	t_point;

	constexpr line_view( t_point start, t_point end )
		:m_start( start )
		,m_difference( end - start )
		,m_walker_step( m_difference | absolute | to )
		,m_step( m_difference | sign | to )
		,m_total( max( m_walker_step ) )
	{ }
	
	constexpr line_view( ) = default;

	struct iterator
	{
		using	iterator_category	=	forward_iterator_tag;
		using	value_type			=	t_point;
		using	difference_type		=	ptrdiff_t;

		constexpr iterator( ) = default;
		explicit constexpr iterator( const line_view& parent )
			:m_parent( &parent )
			,m_current( parent.m_start )
			,m_walker( parent.m_walker_step )
			,m_index( 0 )
		{ }

		constexpr auto operator * ( ) const noexcept -> value_type { return m_current; }
		
		constexpr auto operator ++ ( ) noexcept -> iterator&
		{
			const auto& parent = *m_parent;
			const t_point direction = lazy_transform( m_walker, bind_back( greater_equal, parent.m_total ) ) | to;

			m_current += parent.m_step * direction;
			m_walker += parent.m_walker_step - direction * parent.m_total;
			++m_index;
			return	*this;
		}

		constexpr auto operator ++ ( int ) noexcept -> iterator
		{
			auto copy = *this;
			++*this;
			return	copy;
		}

		constexpr auto operator == ( default_sentinel_t ) const noexcept -> bool
		{
			return	m_index >= m_parent->m_total;
		}

		const line_view* m_parent{ nullptr };
		t_point m_current;
		t_point m_walker;
		typename t_point::value_type m_index{ 0 };
	};

	constexpr auto begin( ) const noexcept { return iterator( *this ); }
	constexpr auto end( ) const noexcept { return default_sentinel; }

private:
	t_point m_start;
	t_point m_difference;
	t_point m_walker_step;
	t_point m_step;
	typename t_point::value_type m_total;
};


template< is_point t_point >
struct __line_to_closure : ::std::ranges::range_adaptor_closure< __line_to_closure< t_point > >
{
	t_point m_end;
	constexpr explicit __line_to_closure( t_point end ) : m_end( end ) { }
	constexpr auto operator ( ) ( t_point start ) const { return line_view< t_point >( start, m_end ); }
};


struct __line_to
{
	template< is_point t_point >
	constexpr auto operator ( ) ( t_point end ) const
	{
		return	__line_to_closure< t_point >{ end };
	}
};
inline constexpr auto line_to = __line_to{ };


}


#endif


