/*
 *  Allocator.h
 *  the
 *
 *  Created by Atz(c) on 04.01.11.
 *  Copyright 2011 The(c)ompany. All rights reserved.
 *
 */

#ifndef __THE_ALLOCATOR_H__

#include <iostream>

#define DEF_AUTO_INCREASE -1

namespace The
{
	using namespace std;

	template< class T, class Index_t = size_t >class Allocator_t // Array_t
		{
			mutable int m_capacity;
			mutable T* m_data;
			Index_t m_count;
			T* reallocate( Index_t index, int max_elements = DEF_AUTO_INCREASE) const;

		public:
			Allocator_t( Index_t capacity = 1 ); ~Allocator_t();
			Allocator_t& operator=( const Allocator_t& );

			Index_t count() const;
			Index_t capacity() const;
			Index_t create( const T& );
			Index_t allocate( const T& );
			Index_t bytes() const; // helper.
			Index_t add( const Allocator_t& );
			Index_t reserve( int serios_number_of_T );

			bool find( const T&, Index_t* rc = 0 ) const;
			bool contains( const T& ) const;

			void remove( const T& ); // FIXME: must also return Index_t
			void clear();
			void print() const;

			T& operator [] ( Index_t );
			const T& operator [] ( Index_t ) const;
		};

	template< class T, class Index_t >inline Allocator_t< T, Index_t >::Allocator_t( Index_t sz )
		:	m_capacity( sz )
		,	m_data(( T*)::malloc( bytes() + 1 ))
		,	m_count( 0 )
		{
		}

	template< class T, class Index_t >inline Allocator_t< T, Index_t >& Allocator_t< T, Index_t >::operator=( const Allocator_t& rhs )
		{
			if (rhs.m_count) cout << "coping data..." << endl; 
			m_count = rhs.m_count; m_capacity = rhs.m_capacity;
			::free( m_data ); m_data =( T*)::malloc( rhs.bytes() + 1 ); ::memcpy( m_data, rhs.m_data, rhs.bytes() );
			return* this;
		}

	template< class T, class Index_t >inline bool Allocator_t< T, Index_t >::find( const T& rhs, Index_t* prc ) const
		{
			for( int k = 0; k < count(); k ++) if (m_data[ k ] == rhs) { if (prc) *prc = k; return true; } return false;
		}

	template< class T, class Index_t >inline Index_t Allocator_t< T, Index_t >::create( const T& rhs )
		{
			T* ptr = 0;
			try { ptr = &this->operator[]( m_count ); }
			catch( T* addr ) { ptr = addr; }
			new( ptr )T( rhs );
			return m_count ++;
		}

	template< class T, class Index_t >inline Index_t Allocator_t< T, Index_t >::allocate( const T& rhs )
		{
			Index_t index; if (find( rhs, &index )) return index; else return create( rhs );
		}

	template< class T, class Index_t >inline void Allocator_t< T, Index_t >::clear()
		{
			m_count = 0;
		}
#	define DEF_TEST_REALLOC

	template< class T, class Index_t >inline void Allocator_t< T, Index_t >::remove( const T& rhs )
		{
#	ifdef DEF_TEST_REALLOC
			Index_t index;
			if (find( rhs, &index ))
				{
					T* dest = &m_data[ index ]; T* sptr = dest + 1; T* eptr = dest + count() - index;
					size_t size =( eptr-sptr )*sizeof( T ); ::memmove( dest, sptr, size ); m_count--;
					cout << "Bytes:\t" << dec << size << endl;
				}
#	else
			Index_t index;
			if (find( rhs, &index ))
				{
					for( int k = 0; k < count(); k ++)
						{
							if (k > index) // compactificating array starting from index of value found
								{
									m_data[ k-1 ] = m_data[ k ];
								}
						}
					m_count --;
				}
			// QC Passed.
#	endif
		}
	
	template< class T, class Index_t >inline Index_t Allocator_t< T, Index_t >::add( const Allocator_t& rhs )
		{
			if (rhs.m_count)
				{
					T* addr = &m_data[ 0 ]; if (m_capacity - m_count < rhs.m_count)
						{
							addr = reallocate( m_count, m_count + rhs.m_count );
						}
					::memcpy( addr, rhs.m_data, rhs.bytes() ); m_count += rhs.m_count;
				}
			return m_count; // next available index.
		}

	template< class T, class Index_t >inline T* Allocator_t< T, Index_t >::reallocate( Index_t index, int max_elements ) const
		{
			Index_t newBytes = (DEF_AUTO_INCREASE == max_elements) ? 2 * bytes() : max_elements*sizeof( T );
			int capacity = newBytes/sizeof( T ); // std::cout << "reallocate ==> " << dec << capacity << " inch." << std::endl;
			T* ptr =( T*)::malloc( newBytes + 1 ); ::memcpy( ptr, m_data, bytes() ); ::free( m_data ); m_data = ptr;
			m_capacity = capacity; return &m_data[ index ];
		}

	template< class T, class Index_t >inline Index_t Allocator_t< T, Index_t >::reserve( int elements )
		{
			if (elements > m_capacity)
				{
					Index_t newBytes = elements*sizeof( T );
					Index_t capacity = newBytes/sizeof( T );
					std::cout << "reserve ==> " << capacity << " inch." << std::endl;
					T* ptr =( T*)::malloc( newBytes + 1 ); ::free( m_data ); m_data = ptr;
					m_capacity = capacity;
				}
			m_count = elements;
			return m_count;
		}

	template< class T, class Index_t >inline void Allocator_t< T, Index_t >::print() const
		{
			std::cout
			<< std::hex << m_data << ':'
			<< " count=" << m_count
			<< " sizeof=" << std::dec << sizeof( T )/sizeof( char )
			<< " capacity=" << m_capacity
			<< " bytes=" << std::dec << m_capacity * sizeof( T )/ sizeof( char )
			<< std::endl;
		}

	template< class T, class Index_t >inline Allocator_t< T, Index_t >::~Allocator_t() { ::free( m_data ); m_capacity = 0; m_count = 0; }
	template< class T, class Index_t >inline bool Allocator_t< T, Index_t >::contains( const T& rhs ) const { Index_t index; return find( rhs, &index ); }
	template< class T, class Index_t >inline Index_t Allocator_t< T, Index_t >::bytes() const { return m_capacity * sizeof( T ); }
	template< class T, class Index_t >inline Index_t Allocator_t< T, Index_t >::count() const { return m_count; }
	template< class T, class Index_t >inline Index_t Allocator_t< T, Index_t >::capacity() const { return m_capacity; }
	template< class T, class Index_t >inline T& Allocator_t< T, Index_t >::operator [] ( Index_t index )
		{
			if (index < m_capacity)
				return m_data[ index ];
			else
				throw reallocate( index );
		}
	template< class T, class Index_t >inline const T& Allocator_t< T, Index_t >::operator [] ( Index_t index ) const { if (index < m_capacity) return m_data[ index ]; else throw reallocate( index ); }
}

#define __THE_ALLOCATOR_H__
#endif
