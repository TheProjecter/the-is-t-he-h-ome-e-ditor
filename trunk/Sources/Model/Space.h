/*
 *  Space.h
 *  the
 *
 *  Created by Atz(c) on 04.01.11.
 *  Copyright 2011 The(c)ompany. All rights reserved.
 *
 */
#ifndef __THE_SPACE_H__

#include <map>
#include <iostream>

#ifndef SPACE_T
#define SPACE_T( RC ) \
	template< class T, int D >RC Space_t< T,D >

namespace The
{
	using namespace std;

	/**
	 * @class Space
	 * @brief The coordinate workspace. A top-level object for scene with data.
	 * @note Interface to create,delete,[de]select and render brushes:
	 */
	template< class T = Floating_t, int D = 4 >class Space_t
		{
			String m_name;
			const PoolFace* m_topPointPool; // a pool with top level point selected.

			typedef std::map< int, PoolFace* >Pools_map_t;
			Pools_map_t m_pools;

			typedef Matrix_t< T,D >Matrix;
			typedef Vector_t< T,D >Vertex;
			Matrix m_modelview;

		public:
			~Space_t(); Space_t( const char* ); Space_t( const Space_t& );
			Space_t copy_selected() const;
			Space_t& paste_from( const Space_t& ); // typically pass temp space copied from selection of over space
			Space_t& operator =( const Space_t& );
			Space_t& clear();
			Space_t& create();
			// cut == copy_selected + clear_selected.
			void createBrush( int sides );
			bool selectBrushes( const Rect& window_rect, bool a_pointing_selection = false );
			bool selectPoints( const Rect& window_rect, bool a_pointing_selection = false );
			bool isSelectedPointsContains( const Rect& ); // none-const: will store index of point was clicked (if any).
			void moveSelectedPointsBy( const Pixel& vector );
			void deleteSelectedBrushes();
			void unselectBrushes();
			void unselectPoints();
			void save() const;
			void load( const char* map_name );
			void render() const;
			void printInfo() const;
			size_t allocatedBytes() const;
			const String& name() const;

		protected:
			PoolFace* operator[]( int );
			PoolFace* operator[]( int ) const;
			void create_pool( Pools_map_t&, size_t dim, size_t num_objects );
		};

#pragma mark -

	SPACE_T( inline const String& )::name() const
		{
			return m_name;
		}

	SPACE_T( inline )::~Space_t()
		{
			clear();
		}

	template< class T, int D >inline Space_t< T,D >& Space_t< T,D >::clear()
		{
			DEF_FOREACH_CONST( Pools_map_t, m_pools, iterator ) { delete iterator->second; }
			m_pools.clear(); m_topPointPool = 0; create(); return *this;
		}

	template< class T, int D >inline void Space_t< T,D >::create_pool( Pools_map_t& lhs, size_t dim, size_t num_objects )
		{
			switch (dim)
			{
                    // std::map< int, PoolFace* >&lhs
                    // template< class Brush, template< class T >class Allocator = Allocator_t >class Pool_t == PoolFace*
				case 3: lhs[ 3 ]= new Pool_t< Brush_t< Vertex,3 > >( num_objects ); break;
				case 4: lhs[ 4 ]= new Pool_t< Brush_t< Vertex,4 > >( num_objects ); break;
				case 6: lhs[ 6 ]= new Pool_t< Brush_t< Vertex,6 > >( num_objects ); break;
				case 8: lhs[ 8 ]= new Pool_t< Brush_t< Vertex,8 > >( num_objects ); break;
			}
		}

	template< class T, int D >inline Space_t< T,D >& Space_t< T,D >::create()
		{
			create_pool( m_pools, 3, 1 );
			create_pool( m_pools, 4, 1 );
			create_pool( m_pools, 6, 1 );
			create_pool( m_pools, 8, 1 );
			return *this;
		}

	SPACE_T( inline )::Space_t( const char* name )
		:	m_name( name )
		,	m_topPointPool (0)
		{
			cout << hex << &m_pools << ": new space<" << D << ">(\"" << m_name << "\")" << endl;
			create();
		}

	SPACE_T( inline )::Space_t( const Space_t< T,D >& rhs )
		:	m_modelview (rhs.m_modelview)
		,	m_topPointPool (0) // XXX: will reset mouse-pressing flag during copy-paste operation.
		{
			// cout << hex << &m_pools << ": space( " << D << " )::space(...)" << endl;
			paste_from( rhs );
		}

	SPACE_T( inline PoolFace* )::operator[]( int poolIndex )
		{
			return m_pools[ poolIndex ];
		}

	SPACE_T( inline PoolFace* )::operator[]( int poolIndex ) const
		{
			return const_cast< Space_t< T,D >* >( this )->m_pools[ poolIndex ];
		}

	template< class T, int D >inline Space_t< T,D >& Space_t< T,D >::operator=( const Space_t& rhs )
		{
			// cout << hex << &m_pools << ": space = ..." << endl;
			clear(); paste_from( rhs ); return* this;
		}

	template< class T, int D >inline Space_t< T,D >& Space_t< T,D >::paste_from( const Space_t& rhs )
		{
			// cout << hex << &m_pools << ": space( " << D << " )::paste " << endl;
			DEF_FOREACH_CONST( Pools_map_t, rhs.m_pools, iterator )
				{
					if (!m_pools[ iterator->first ]) { create_pool( m_pools, iterator->first, rhs.m_pools.count( iterator->first )); }
					m_pools[ iterator->first ]->paste_from( iterator->second );
				}
			//m_modelview = rhs.m_modelview;
			m_topPointPool = 0;
			return* this;
		}

	template< class T, int D >inline Space_t< T,D >Space_t< T,D >::copy_selected() const
		{
			Space_t< T,D >temp("internal");
			DEF_FOREACH_CONST( Pools_map_t, m_pools, iterator )
				{
					iterator->second->copy_selected_to( temp.m_pools[ iterator->first ] );
				}
			return temp;
		}
#	pragma mark -
#	pragma mark >>> RENDER <<<

	SPACE_T( inline void )::render() const
		{
			DEF_FOREACH_CONST( Pools_map_t, m_pools, iterator )
				{
					iterator->second->render();
				}
		}

	SPACE_T( inline void )::printInfo() const
		{
			// cout << "space( " << D << " )::print" << endl;
			DEF_FOREACH_CONST( Pools_map_t, m_pools, iterator )
				{
					iterator->second->print_info();
				}
		}

	SPACE_T( inline size_t )::allocatedBytes() const
		{
			size_t rc = 0;
			DEF_FOREACH_CONST( Pools_map_t, m_pools, iterator )
				{
					rc += iterator->second->allocated_bytes();
				}
			return rc;
		}

	SPACE_T( inline void )::unselectBrushes()
		{
			DEF_FOREACH( Pools_map_t, m_pools, iterator )
				{
					iterator->second->unselectBrushes();
				}
		}

	SPACE_T( inline void )::unselectPoints()
		{
			DEF_FOREACH( Pools_map_t, m_pools, iterator )
				{
					iterator->second->unselectApexes();
				}
		}

	SPACE_T( inline void )::createBrush( int sides )
		{
			PoolFace* ptr = m_pools[ sides ];
			if (! ptr)
			{
				cout << "Pool<" << sides << "> not found."<< endl;
				throw Common::Error("Warning: no pool ptr.");
			}
			ptr->createBrush();
			cout << dec << allocatedBytes() << " bytes" << endl;
		}

	SPACE_T( inline bool )::selectBrushes( const Rect& wr, bool a_pointing_selection )
		{
			bool selected = false;
			DEF_FOREACH( Pools_map_t, m_pools, iterator )
				{
					if (iterator->second->selectBrushes( wr, a_pointing_selection ))
						{
							selected = true;
						}
				}
			return selected;
		}

	SPACE_T( inline void )::deleteSelectedBrushes()
		{
			DEF_FOREACH_CONST( Pools_map_t, m_pools, iterator )
				{
					iterator->second->deleteSelectedBrushes();
				}
		}

// FIXME: проблема в том, что выделенные призмы уже не могут рисоваться в выбранной модели их размещения в памяти: у них разноцветные ребра.
// XXX: научиться создавать цветовые массивы на сложные примитивы - типа GL_LINES _И_ GL_LINE_LOOP.

	SPACE_T( inline bool )::selectPoints( const Rect& wr, bool a_pointing_selection )
		{
			//cout << "P+ " << wr; 

			bool selected = false;
			DEF_FOREACH( Pools_map_t, m_pools, iterator )
				{
					if (iterator->second->selectApexes( wr, a_pointing_selection ))
						{
							selected = true;
						}
				}
			return selected;
		}

	SPACE_T( inline bool )::isSelectedPointsContains( const Rect& wndRect )
		{
			m_topPointPool = 0;
			Floating_t near = 1000000;
			DEF_FOREACH_CONST( Pools_map_t, m_pools, iterator )
				{
					Floating_t depth = near; // compire depth between pools
					if (iterator->second->isSelectedApexesContains( wndRect, &depth ) && depth < near)
						{
							// store pool with z-nearest point under the pointed area
							m_topPointPool = iterator->second;
							near = depth;
						}
				}
			return (m_topPointPool);
		}

	SPACE_T( inline void )::moveSelectedPointsBy( const Pixel& wndVector )
		{
			if (m_topPointPool)
				{
					// Press L and see: "Dimension: 3 / Vertexes: 6 / Indexes: 8"
					Vector distance = m_topPointPool->distanceToMoveBy( wndVector );
					if (Vector() != distance)
						{
							DEF_FOREACH( Pools_map_t, m_pools, iterator )
								{
									iterator->second->moveSelectedApexesBy( distance );
								}
						}
				}
		}
#	pragma mark -

	SPACE_T( void )::save() const
		{
			DEF_FOREACH_CONST( Pools_map_t, m_pools, iterator )
				{
					iterator->second->save();
				}
			// TODO: save this->m_modelview;
		}

	SPACE_T( void )::load( const char* map_name )
		{
			DEF_FOREACH( Pools_map_t, m_pools, iterator )
				{
					iterator->second->load( map_name );
				}
			// TODO: load this->m_modelview;
		}
}

#endif
#undef SPACE_T
#define __THE_SPACE_H__
#endif
