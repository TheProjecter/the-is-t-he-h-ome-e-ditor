/*
 *  Pool.h
 *  the
 *
 *  Created by Atz(c) on 02.01.11.
 *  Copyright 2011 The(c)ompany. All rights reserved.
 *
 */

#ifndef __THE_POOL_H__

#include <iostream>
#include "accessory.h"
#include "dbase4.h"

#ifndef POOL_T
#define POOL_T(_RC_) \
	template< class Brush, template< class T, class Index_t >class A >_RC_ Pool_t< Brush,A >

#ifndef DEF_VERTEXES_ARRAY_AND_T
#define DEF_VERTEXES_ARRAY_AND_T( vertexes ) \
	typedef typename Brush::V_t vertex_t; \
	vertex_t* vertexes =( vertex_t*)( Brush*)&m_brushes[ 0 ];

#ifndef DEF_CONST_VERTEXES_ARRAY_AND_T
#define DEF_CONST_VERTEXES_ARRAY_AND_T( vertexes ) \
	typedef typename Brush::V_t vertex_t; \
	const vertex_t* vertexes =( const vertex_t*)( const Brush*)&m_brushes[ 0 ];

#ifndef FOREACH_APEX
#define FOREACH_APEX( index ) \
	for( int index = 0; index < vertexCount(); index ++)

// k is not a real index of vertex in brushes array. it only position of index inside indexes arrays.
#ifndef FORONLY_VISIBLE_APEXES
#define FORONLY_VISIBLE_APEXES( k ) \
	for( int k = 0; k < m_visibleIndexes.count(); k ++)

#ifndef FORONLY_SELECTED_APEXES
#define FORONLY_SELECTED_APEXES( k ) \
	for( int k = 0; k < m_selectedIndexes.count(); k ++)

#ifndef FOREACH_BRUSH
#define FOREACH_BRUSH( index ) \
	for( int index = 0; index < m_brushes.count(); index ++)

#ifndef FORONLY_SELECTED_BRUSHES
#define FORONLY_SELECTED_BRUSHES( index ) \
	FOREACH_BRUSH( index ) if (! m_centerIndexes.find( index ))

#ifndef FORONLY_SELECTED_BRUSHES_INDEXES
#define FORONLY_SELECTED_BRUSHES_INDEXES( k ) \
	for( int k = 0; k < m_selectedBrushes.count(); k ++)

#ifndef FOREACH_BRUSH_VERTEX
#define FOREACH_BRUSH_VERTEX( apexIndexInBrush ) \
	for( int apexIndexInBrush = 0; apexIndexInBrush < Brush::vertexNumber(); apexIndexInBrush ++)

#ifndef FOREACH_RECORD
#define FOREACH_RECORD( index ) \
	for( int index = 0; index < db_vertexes.records(); index ++)

namespace The
{
	typedef Index_t Index; // FIXME: use default template parameter to size_t
	typedef Center_t Center;

	using namespace std;

#	pragma mark -

	struct DB_VertexStruct_Base : public DB::Struct
		{
			//Character sDevName;
			DB::Numeric X;
			DB::Numeric Y;
			DB::Numeric Z;
			DB::Numeric I;
			DB_VertexStruct_Base( int n, DB::PBASE4_t ptr, int d10 );
		};
	inline DB_VertexStruct_Base::DB_VertexStruct_Base( int n, DB::PBASE4_t ptr, int d10 )
		:	DB::Struct( n, ptr )
		,	X (ptr, "X", d10*2, d10-1) // double has 14 base 10 digits fraction precision, float has 7 only, D10 is returns 15.
		,	Y (ptr, "Y", d10*2, d10-1) // setting precision to (std::numeric_limits< double >::digits10 - 1) will round values.
		,	Z (ptr, "Z", d10*2, d10-1)
		,	I (ptr, "I", d10*2, d10-1)
		//,	sStrVar( ptr, "sStrVar", 20 ) // example of string fields.
		{
		}
	template< class T >struct DB_VertexStruct : public DB_VertexStruct_Base
		{
			DB_VertexStruct( int n, DB::PBASE4_t ptr ): DB_VertexStruct_Base( n, ptr, std::numeric_limits< T >::digits10 ) {}
		};
	template <> struct DB_VertexStruct< float > : public DB_VertexStruct_Base
		{
			DB_VertexStruct( int n, DB::PBASE4_t ptr ): DB_VertexStruct_Base( n, ptr, std::numeric_limits< float >::digits10 ) {}
		};
	template <> struct DB_VertexStruct< double > : public DB_VertexStruct_Base
		{
			DB_VertexStruct( int n, DB::PBASE4_t ptr ): DB_VertexStruct_Base( n, ptr, std::numeric_limits< double >::digits10 ) {}
		};
//	template< class T >inline DB_VertexStruct< T >::DB_VertexStruct( int n, DB::PBASE4_t ptr ): DB_VertexStruct_Base( n, ptr, std::numeric_limits< T >::digits10 ) {}
//	template<>inline DB_VertexStruct< float >::DB_VertexStruct( int n, DB::PBASE4_t ptr ): DB_VertexStruct_Base( n, ptr, std::numeric_limits< float >::digits10 ) {}
//	template<>inline DB_VertexStruct< double >::DB_VertexStruct( int n, DB::PBASE4_t ptr ): DB_VertexStruct_Base( n, ptr, std::numeric_limits< double >::digits10 ) {}

	struct PoolFace
		{
			virtual const char* name() const = 0;
			virtual bool compatible( const PoolFace* ) const = 0;
			virtual void createBrush() = 0;
			virtual void save() const = 0;
			virtual void load( const char* map_name ) = 0;
			virtual void render() const = 0;
			virtual void print_info() const = 0;
			virtual size_t allocated_bytes() const = 0;

			virtual bool selectBrushes( const Rect&, bool only_top ) = 0;
			virtual void unselectBrushes() = 0;
			virtual void deleteSelectedBrushes() = 0;

			virtual bool selectApexes( const Rect& wndRect, bool only_top ) = 0;
			virtual bool isSelectedApexesContains( const Rect& wndApexArea, Floating_t* depth ) = 0;
			virtual void unselectApexes() = 0;

			virtual void printAllApexes() const = 0;
			virtual bool printSelectedApexes() const = 0;
			virtual void moveSelectedApexesBy( const Vector& moveBy ) = 0;
			virtual Vector distanceToMoveBy( const Pixel& wndVector ) const = 0;

			virtual void copy_selected_to( PoolFace* ) const = 0;
			virtual void paste_from( const PoolFace* ) = 0;
			virtual void clear() = 0;
			virtual void updateCenters() = 0;
			virtual void updateCenter( Index_t brush_index ) = 0;
			virtual void selectCenter( Index_t brush_index ) = 0;
			virtual bool selectApex( Index_t index, bool rotate = false ) = 0;
			virtual bool rotateApexSelection( Index_t index ) = 0;
			virtual int selectingApexes( const Rect& wndRect, bool only_top, Index_t index ) = 0;
			virtual int brush_dimension() const = 0;
			// cut == copy_selected + clear_selected.
		};
	
#	pragma mark -

	template< class Brush, template< class T, class Index_t = size_t>class Allocator = Allocator_t >

	class Pool_t: public PoolFace
		{
			const char* m_poolname;
			typedef Allocator< Brush >BrushAllocator_t;
			typedef Allocator< Index >IndexesAllocator_t;
			typedef Allocator< Vertex >VertexAllocator_t;
			typedef Allocator< Center >CenterAllocator_t;
			IndexesAllocator_t m_selectedBrushes; // WARNING: valid is only moment to delete! (need validate at any time)
			IndexesAllocator_t m_selectedIndexes;
			IndexesAllocator_t m_visibleIndexes;
			IndexesAllocator_t m_centerIndexes;
			CenterAllocator_t m_centers;
			BrushAllocator_t m_brushes;
			const Vertex* m_topSelectedVertex;
			typedef DB::DBaseIV< DB_VertexStruct< float > >DB_Vertexes_t;

#	pragma mark public

		public:
			Pool_t( int capacity );
			virtual ~Pool_t();
			const char* name() const;
			bool compatible( const PoolFace* ) const;
			void createBrush();

			void render() const;
			bool selectBrushes( const Rect&, bool only_top );
			void unselectBrushes();
			void deleteSelectedBrushes();

			bool selectApexes( const Rect& wndRect, bool only_top );
			void unselectApexes();
			 // will remember top(near) selected vertex:
			bool isSelectedApexesContains( const Rect& wndApexArea, Floating_t* depth );

			void printAllApexes() const;
			bool printSelectedApexes() const;
			void moveSelectedApexesBy( const Vector& moveBy );
			Vector distanceToMoveBy( const Pixel& wndVector ) const;
			void print_info() const;
			void save() const;
			void load( const char* map_name );
			size_t allocated_bytes() const;

			// copy-paste:
			Pool_t& operator =( const Pool_t& );
			void paste_from( const PoolFace* );
			void copy_selected_to( PoolFace* ) const;
			void clear();
			
#	pragma mark protected

		protected:
			int brush_dimension() const;
			int vertexCount() const;
			static const char* newName();
			void updateCenters();
			void updateCenter( Index_t brush_index );
			void selectCenter( Index_t brush_index );
			bool selectApex( Index_t vertex_index, bool rotate = false );
			bool rotateApexSelection( Index_t vertex_index );
			int selectingApexes( const Rect& wndRect, bool only_top, Index_t index );

		private:
			static const int O = 0; // starting index and zero stride
			static const int S = 2; // complex stride factor
			static const int P = 5; // apex point size (px)
			static const int L = 1; // line width (px)
		};

#	pragma mark -
#	pragma mark operators

	template< class Brush, template< class T, class Index_t >class A >inline Pool_t< Brush,A >& Pool_t< Brush,A >::operator =( const Pool_t& rhs )
		{
			cout << "pool( " << brush_dimension() << " )::operator =" << endl;
			m_selectedBrushes = rhs.m_selectedBrushes;
			m_selectedIndexes = rhs.m_selectedIndexes;
			m_visibleIndexes = rhs.m_visibleIndexes;
			m_centerIndexes = rhs.m_centerIndexes;
			m_centers = rhs.m_centers;
			m_brushes = rhs.m_brushes;
			m_topSelectedVertex = 0;
			return* this;
		}

	POOL_T( inline )::Pool_t( int capacity )
		:	m_poolname( newName() )
		,	m_brushes( capacity )
		,	m_centers( capacity )
		,	m_centerIndexes( capacity )
		,	m_visibleIndexes( Brush::vertexNumber() * capacity )
		,	m_selectedIndexes( Brush::vertexNumber() * capacity )
		,	m_selectedBrushes( capacity )
		,	m_topSelectedVertex( 0 )
		{
		}

	POOL_T()::~Pool_t()
		{
			delete [] m_poolname; m_poolname = 0;
		}

	POOL_T( const char* )::newName() // static
		{
			char* rc = new char[ 255 ];
			::sprintf(rc, "POOL%d.dbf", Brush::vertexNumber() / 2);
			return rc;
		}

	POOL_T( const char* )::name() const
		{
			return m_poolname;
		}

	POOL_T( inline int )::brush_dimension() const
		{
			return Brush::vertexNumber() / 2;
		}
	
	POOL_T( inline bool )::compatible( const PoolFace* ptr ) const
		{
			return ptr && (brush_dimension() == ptr->brush_dimension());
		}
	
	POOL_T( inline int )::vertexCount() const
		{
			return Brush::vertexNumber() * m_brushes.count();
		}

	POOL_T( inline void )::updateCenters()
		{
			FOREACH_BRUSH( brush_index )
				{
					updateCenter( brush_index );
				}
		}
	POOL_T( inline void )::updateCenter( Index_t brush_index )
		{
#	ifdef DEF_DEBUG_CENTERS
			if (brush_index < m_centers.count()) {
#	endif
				m_centers[ brush_index ] = m_brushes[ brush_index ].center();
#	ifdef DEF_DEBUG_CENTERS
			} else try { Vertex center = m_centers[ brush_index ]; }
			catch(...) { cout << "pool( " << D << " )::unresolved: " << brush_index
				<< '/' << m_centers.count() << '/' << m_centers.capacity() << endl; }
#	endif
		}

	POOL_T( size_t )::allocated_bytes() const
		{
			return m_selectedIndexes.bytes() + m_visibleIndexes.bytes() + m_centerIndexes.bytes() + m_centers.bytes() + m_brushes.bytes();
		}

#	pragma mark -
#	pragma mark brushes

	POOL_T( inline void )::createBrush()
		{
			const Brush& rhs = Brush( 10,10 ); // FIXME: Create Brush_base and pass parameter to PoolFace::createBrush() = 0
			m_centerIndexes.allocate( m_centers.create( m_brushes[ m_brushes.create( rhs ) ].center() ) );
			cout << "brushes/centers: " << m_brushes.count() << " / " << m_centers.count() << endl;
		}

	POOL_T( inline void )::selectCenter( Index_t brush_index )
		{
			m_centerIndexes.remove( brush_index ); // hide center,
			FOREACH_BRUSH_VERTEX( local_index ) // and show all apexes.
				{
					Index vertex_index = brush_index * Brush::vertexNumber() + local_index;
					m_visibleIndexes.allocate( vertex_index );
				}
		}

	POOL_T( inline bool )::selectBrushes( const Rect& wndRect, bool only_top )
		{
			bool changes = false;
			Floating_t z = 1;
			int top_index = -1;
			FOREACH_BRUSH( brush_index )
				{
					// recalculate after mouse released: this important due to invisible
					// center doesn't move following to moved brush with selected apexes
					updateCenter( brush_index );

					const Point& center_projection = glo::vertex_projection( m_centers[ brush_index ] );
					bool isClickToCenter = wndRect.contains( center_projection );
					bool isCenterVisible = m_centerIndexes.contains( brush_index );

					if (isCenterVisible && isClickToCenter)
						{
							if (! only_top)
								{
									selectCenter( brush_index );
									changes = true;
								}
							else if (center_projection.Z < z) // search for top _one_
								{
									z = center_projection.Z;
									top_index = brush_index;
								}
						}
				}
			if (-1 != top_index && only_top)
				{
					//cout << "near is: " << z << endl;
					selectCenter( top_index );
					changes = true;
				}
			return changes;
		}

	POOL_T( inline void )::unselectBrushes()
		{
			FOREACH_BRUSH( brush_index )
				{
					updateCenter( brush_index );
					if (! m_centerIndexes.contains( brush_index ))
						{
							m_centerIndexes.allocate( brush_index );
							FOREACH_BRUSH_VERTEX( vertex_index )
								{
									Index index = Brush::vertexNumber() * brush_index + vertex_index;
									m_visibleIndexes.remove( index );
									m_selectedIndexes.remove( index );
								}
						}
				}
		}

#	pragma mark -
#	pragma mark copy/paste

	POOL_T( inline void )::paste_from( const PoolFace* ptr )
		{
			if (!ptr || !compatible( ptr ))
				return; // check brush dimension compatibility with over pool

			const Pool_t< Brush,A >* pool = static_cast< const Pool_t< Brush,A >* >( ptr );
			if (pool->m_brushes.count())
				{
					unselectBrushes(); m_topSelectedVertex = 0; cout << "pool( " << brush_dimension() << " )::paste" << endl;
					int old_count = m_brushes.count(); m_brushes.add( pool->m_brushes ); m_centers.add( pool->m_centers );
					for( int brush_index = old_count; brush_index < m_brushes.count(); brush_index ++) FOREACH_BRUSH_VERTEX( vertex_index )
						m_selectedIndexes.allocate( Brush::vertexNumber() * brush_index + vertex_index );
				}
		}

	POOL_T( inline void )::copy_selected_to( PoolFace* ptr ) const
		{
			if (ptr && compatible (ptr) && (m_centerIndexes.count() != m_centers.count()))
				{
					cout << "pool( " << brush_dimension() << " )::copy" << endl;
					Pool_t< Brush,A >* pool = static_cast< Pool_t< Brush,A >* >( ptr );
					pool->m_topSelectedVertex = 0;
					FORONLY_SELECTED_BRUSHES( brush_index )
						{
							pool->m_brushes.create( m_brushes[ brush_index ] );
							pool->m_centers.create( m_centers[ brush_index ] );
						}
				}
		}

	POOL_T( inline void )::deleteSelectedBrushes()
		{
			m_selectedBrushes.clear(); unselectApexes(); FORONLY_SELECTED_BRUSHES( brush_index ) // m_centerIndexes && m_brushes
				{
					cout << "pool< " << brush_dimension() << " >::will_delete_selected( " << brush_index << " )" << endl;
					m_selectedBrushes.allocate( brush_index );
				}
			m_centerIndexes.clear(); unselectBrushes(); // will update m_centerIndexes

			Index value = 0; FORONLY_SELECTED_BRUSHES_INDEXES( k ) // safe
				{
					// m_brushes compactificated. Need to reduce index on each step
					Index brush_index = m_selectedBrushes[ k ] - value++;
					m_brushes.remove( m_brushes[ brush_index ] );
					m_centers.remove( m_centers[ brush_index ] );
				}
			m_centerIndexes.clear(); unselectBrushes(); // update against after modification
		}
	
	POOL_T( inline void )::clear()
		{
			cout << "pool( " << brush_dimension() << " )::clear" << endl;
			m_selectedBrushes.clear(); m_selectedIndexes.clear();
			m_visibleIndexes.clear(); m_centerIndexes.clear();
			m_centers.clear(); m_brushes.clear();
			m_topSelectedVertex = 0;
		}

#	pragma mark -
#	pragma mark apexes

	POOL_T( inline void )::unselectApexes()
		{
			FORONLY_SELECTED_APEXES( k )
				{
					m_visibleIndexes.allocate( m_selectedIndexes[ k ] );
				}
			m_selectedIndexes.clear();
		}

	POOL_T( inline bool )::selectApex( Index_t index, bool rotate )
		{
			bool selected = m_selectedIndexes.contains( index );
			bool visible = m_visibleIndexes.contains( index );
			bool changes = false;
			if (visible == selected)
				{
					cout << "exceeding work to enumarate invisible brush vertexes is detected ("
						<< index << ')' << endl;
				}
			else if (visible) // selecting
				{
					m_selectedIndexes.allocate( index );
					m_visibleIndexes.remove( index );
					changes = true;
				}
			else if (rotate && selected) // deselecting if only rotate selection requared.
				{
					m_selectedIndexes.remove( index );
					m_visibleIndexes.allocate( index );
					changes = true;
				}
			return changes;
		}

	POOL_T( inline bool )::rotateApexSelection( Index_t index )
		{
			return selectApex( index, true );
		}

	POOL_T( inline int )::selectingApexes( const Rect& wndRect, bool only_top, Index_t index ) // unused
		{
			Floating_t z = 1;
			int top_index = -1;
			DEF_VERTEXES_ARRAY_AND_T( vertexes );
			const Point& apex_projection = glo::vertex_projection( vertexes[ index ] );
			if (wndRect.contains( apex_projection ))
				{
					if (only_top) // clicking directly to apex
						{
							if (apex_projection.Z < z) // search for top _one_ only
								{
									z = apex_projection.Z;
									top_index = index;
								}
						}
					else if (selectApex( index )) // will select all contained
						{
							top_index = index; // is found
						}
				}
			return top_index;
		}
/*
	POOL_T( inline bool )::selectApexes( const Rect& wndRect, bool only_top ) // experimental (how to exclude unselected brush vertexes from search).
		{
			int top_index = -1; bool found = false;
			FORONLY_VISIBLE_APEXES( k )
				{
					int top = selectingApexes( wndRect, only_top, m_visibleIndexes[ k ] );
					if (top != top_index) { k --; } // fix array.
					top_index = top;
				}
			FORONLY_SELECTED_APEXES( k )
				{
					int top = selectingApexes( wndRect, only_top, m_selectedIndexes[ k ] );
					if (top != top_index) { k --; } // fix array.
					top_index = top;
				}
			found = -1 != top_index;
			if (found && only_top)
				{
					if (!rotateApexSelection( top_index ))
						{
							glo::debug();
						}
					return true;
				}
			return found;
		}
*/
	POOL_T( inline bool )::selectApexes( const Rect& wndRect, bool only_top ) // QC Passed.
		{
			bool changes = false;
			Floating_t distance = 1;
			int top_index = -1;
			DEF_VERTEXES_ARRAY_AND_T( vertexes );
			// FOREACH_APEX( vertex_index ) // exceeding work to comparision of unselected brushes vertexes from pool* (see below).
			FORONLY_SELECTED_BRUSHES( brush_index ) FOREACH_BRUSH_VERTEX( local_index ) // and show all apexes.
				{
					Index vertex_index = brush_index * Brush::vertexNumber() + local_index;
					const Point& apex_projection = glo::vertex_projection( vertexes[ vertex_index ] );
					if (wndRect.contains( apex_projection ))
						{
							if (only_top) // click directly to apex
								{
									Floating_t depth = apex_projection.Z;
									if (depth < distance) // search for top _one_
										{
											distance = depth;
											top_index = vertex_index;
										}
								}
							else if (selectApex( vertex_index ))
								{
									changes = true;
								}
						}
				}
			bool found = -1 != top_index;
			if (found && only_top)
				{
					if (!rotateApexSelection( top_index ))
						{
							glo::debug(); // *TODO: set breakpoint to debug invisible AND unselected brush vertexes.
						}
					changes = true;
				}
			return changes;
		}

	POOL_T( inline bool )::isSelectedApexesContains( const Rect& wndApexArea, Floating_t* param ) // QC Passed.
		{
			if (m_selectedIndexes.count() && param)
				{
					Floating_t distance = 1;
					const typename Brush::V_t* topVertex = 0;
					DEF_VERTEXES_ARRAY_AND_T( vertexes );
					FORONLY_SELECTED_APEXES( k )
						{
							Index index = m_selectedIndexes[ k ];
							const Point& apex_projection = glo::vertex_projection( vertexes[ index ] );
							if (wndApexArea.contains( apex_projection ))
								{
									Floating_t depth = apex_projection.Z;
									if (distance > depth)
										{
											distance = depth;
											topVertex = &vertexes[ index ];
										}
								}
						}

					if (topVertex)
						{
							*param = distance; m_topSelectedVertex = topVertex;
							return true;
						}
				}
			m_topSelectedVertex = 0; return false;
		}

#	pragma mark -
#	pragma mark move/print

	POOL_T( inline Vector )::distanceToMoveBy( const Pixel& wndVector ) const
		{
			Vector rc;
			if (m_topSelectedVertex)
				{
					rc = glo::translate_distance_for_vertex( wndVector, *m_topSelectedVertex );
				}
			return rc;
		}
	
	POOL_T( inline void )::moveSelectedApexesBy( const Vector& moveBy )
		{
			DEF_VERTEXES_ARRAY_AND_T( vertexes );
			FORONLY_SELECTED_APEXES( k ) // loop by only selected
				{
					vertexes[ m_selectedIndexes[ k ] ] += moveBy;
				}
		}

	POOL_T( inline void )::printAllApexes() const
		{
			DEF_CONST_VERTEXES_ARRAY_AND_T( vertexes );
			FOREACH_APEX( index )
				{
					const vertex_t& vertex = vertexes[ index ];
					cout << index << ':' << vertex;
				}
		}

	POOL_T( inline bool )::printSelectedApexes() const
		{
			cout << "Selected:" << endl;
			bool hasSelected = false;
			DEF_CONST_VERTEXES_ARRAY_AND_T( vertexes );
			FORONLY_SELECTED_APEXES( k ) // print existing indexes created by m_selectedIndexes.allocate( index )
				{
					hasSelected = true;
					Index idx = m_selectedIndexes[ k ]; // a real vertex index in brushes array.
					const vertex_t& vertex = vertexes[ idx ]; cout << idx << ":" << vertex;
				}
			return hasSelected;
		}

	POOL_T( void )::print_info() const
		{
			using namespace std; printAllApexes();
			cout<< "Dimension: "  << Brush::vertexNumber() / 2
				<< " / Vertexes: " << dec << vertexCount()
				<< " / Centers: " << m_centerIndexes.count()
				<< " / Red: " << m_visibleIndexes.count()
				<< " / Yellow: " << m_selectedIndexes.count()
				<< " / "; printSelectedApexes();
		}

#	pragma mark -
#	pragma mark save/load

	POOL_T( inline void )::save() const
		{
			if (!vertexCount())
				return;
			cout << "Save " << vertexCount() << " vertexes of " << vertexCount() / Brush::vertexNumber() << " brushes to " << m_poolname << endl;
			cout << "brushes/centers: " << m_brushes.count() << " / " << m_centers.count() << endl;
			try {
			DB_Vertexes_t db_vertexes( m_poolname );
			db_vertexes.Truncate();
			DEF_VERTEXES_ARRAY_AND_T( vertexes );
			int recdiff = vertexCount() - db_vertexes.records();
			if (recdiff > 0)
				{
					for( int idx = 0; idx < recdiff; idx ++)
						db_vertexes.append_blank();
				}
					DB_Vertexes_t::Record iterator = &db_vertexes;
					FOREACH_APEX( index )
						{
							iterator->X = vertexes[ index ].X;
							iterator->Y = vertexes[ index ].Y;
							iterator->Z = vertexes[ index ].Z;
							iterator->I = vertexes[ index ].W;
							iterator ++;
						}
				}
			catch( const Errors& exception )
				{
					exception.verbose();
				}
		}

	POOL_T( inline void )::load( const char* unused_map_name )
		{
			DB_Vertexes_t* ptr;
			try {
					ptr = new DB_Vertexes_t(m_poolname);
					if (! ptr) throw File::Error(m_poolname);
				}
			catch( Errors& exception )
				{
					exception.verbose();
					return;
				}
				DB_Vertexes_t& db_vertexes = *ptr;
				int num_rec = db_vertexes.records(); if (! num_rec) { return; } clear();
				double elements = 0; double fract = ::modf( ((double)num_rec) / (double)Brush::vertexNumber(), &elements );
				if (fract) { cout << "<<<<< Not integer brushes number in " << m_poolname << endl; return; }
				cout << "Load " << num_rec << " vertexes of " << elements << " brushes from " << m_poolname << endl;
				m_brushes.reserve( elements ); m_centers.reserve( elements ); m_centerIndexes.reserve( elements );
				DEF_VERTEXES_ARRAY_AND_T( vertexes ); DB_Vertexes_t::Record iterator = db_vertexes.iterator();
				FOREACH_RECORD( index ) { vertexes[ index ] = Vertex( iterator->X, iterator->Y, iterator->Z, iterator->I ); iterator ++; }
				FOREACH_BRUSH( index ) { m_centers[ index ] = m_brushes[ index ].center(); m_centerIndexes[ index ] = index; }
				cout << "brushes/centers: " << m_brushes.count() << " / " << m_centers.count() << endl;
		}
	
#	pragma mark -
#	pragma mark >>> RENDER <<<

	POOL_T( inline void )::render() const
		{
			Index nSides = Brush::vertexNumber() / 2;
			typedef typename Brush::V_t vertex_t;
			vertex_t* vertexes =( vertex_t*)( Brush*)&m_brushes[ 0 ];  // represent brushes array as vertexes array
			if (vertexes)
				{
					::glLineWidth( L );
					::glPointSize( P );
					::glEnableClientState( GL_VERTEX_ARRAY );

					// top side (line loop)
					::glVertexPointer( vertex_t::dim(), vertex_t::type(), S * sizeof( vertex_t ), &vertexes[ 0 ] );
					::glColor3d( /*1,0,1*/1,1,1 ); FOREACH_BRUSH( brush ) ::glDrawArrays( GL_LINE_LOOP, brush*nSides, nSides );

					// bottom side (line loop)
					::glVertexPointer( vertex_t::dim(), vertex_t::type(), S * sizeof( vertex_t ), &vertexes[ 1 ] );
					::glColor3d( /*0,1,1*/1,1,1 ); FOREACH_BRUSH( brush )::glDrawArrays( GL_LINE_LOOP, brush*nSides, nSides );

					// back sides (lines)
					::glVertexPointer( vertex_t::dim(), vertex_t::type(), O, &vertexes[ 0 ] );
					::glColor3d( 1,1,1 ); ::glDrawArrays( GL_LINES, O, vertexCount() );

					// indeces (see it affected by last called glVertexPointer also)
					::glColor3d( 1,0,0 ); ::glDrawElements( GL_POINTS, m_visibleIndexes.count(), GL_UNSIGNED_INT, &m_visibleIndexes[ 0 ] );
					::glColor3d( 1,1,0 ); ::glDrawElements( GL_POINTS, m_selectedIndexes.count(), GL_UNSIGNED_INT, &m_selectedIndexes[ 0 ] );
//#	define	DEF_PERSISTENT_CENTERS
#				ifndef DEF_PERSISTENT_CENTERS
					// center vertexes (points)
					::glVertexPointer( vertex_t::dim(), vertex_t::type(), O, &m_centers[ 0 ] ); // change array
					::glColor3d( 1,0,0 ); ::glDrawElements( GL_POINTS, m_centerIndexes.count(), GL_UNSIGNED_INT, &m_centerIndexes[ 0 ] );
#				else
					// debug: draw centers as arrays of vertexes instead of indexes to it.
					::glVertexPointer( vertex_t::dim(), vertex_t::type(), O, &m_centers[ 0 ] );
					::glColor3d( 0,1,0 ); ::glDrawArrays( GL_POINTS, O, m_centers.count() );
#				endif
					::glDisableClientState( GL_VERTEX_ARRAY );
				}
		}

} // The::

#undef FOREACH_RECORD
#endif

#undef FOREACH_BRUSH_VERTEX
#endif

#undef FORONLY_SELECTED_BRUSHES_INDEXES
#endif

#undef FORONLY_SELECTED_BRUSHES
#endif

#undef FOREACH_BRUSH
#endif

#undef FORONLY_SELECTED_APEXES
#endif

#undef FORONLY_VISIBLE_APEXES
#endif

#undef FOREACH_APEX
#endif

#undef DEF_CONST_VERTEXES_ARRAY_AND_T
#endif

#undef DEF_VERTEXES_ARRAY_AND_T
#endif

#undef POOL_T
#endif

#define __THE_POOL_H__
#endif
