////////////////////////////////////////////////////////////////////////////////
///  Brush.h
///  the
///
///  Created by Atz(c) on 02.01.11.
///  Copyright 2011 The(c)ompany. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __THE_BRUSH_H__

#include <new>
#include <iostream>

#ifndef BRUSH_T
#define BRUSH_T( RC ) \
	template< class V, int N >RC Brush_t< V,N >

namespace The
{
	using namespace std;

	struct Center_t: public Vertex
		{
			Center_t();
			Center_t( const Vertex& );
			bool operator==( const Center_t& ) const;
			bool operator!=( const Center_t& ) const;
		};

	inline Center_t::Center_t(): Vertex::Vector_t< double,4 >() {}
	inline Center_t::Center_t( const Vertex& rhs ): Vertex::Vector_t< double,4 >( rhs ) {}
	inline bool Center_t::operator==( const Center_t& rhs ) const { return &rhs == this && Vertex::operator==( rhs ); }
	inline bool Center_t::operator!=( const Center_t& rhs ) const { return &rhs != this || Vertex::operator!=( rhs ); }

	template< class V, int N >class Brush_t
		{
			V m_array[ N*2 ]; // arrys of vectors with {X,Y,Z} physically allocated using placement new in requested place
			void random_init();
			void inconvex_init( Floating_t H = glo::random_between( -16, 16 ), Floating_t R = glo::random_between( -16, 16 ));
			void init( Floating_t height = 10, Floating_t radius = 10 );
			bool compire( const Brush_t& ) const;
		public:
			Brush_t();
			Brush_t( Floating_t height, Floating_t radius );
			void rotate( Floating_t angle );
			Center_t center() const;
			static int vertexNumber();
			void* operator new( size_t, void* ptr );
			bool operator==( const Brush_t& ) const;
			bool operator!=( const Brush_t& ) const;
			Brush_t& operator=( const Brush_t& );
			typedef V V_t;
		};

	BRUSH_T( inline )::Brush_t() { random_init(); }
	BRUSH_T( inline )::Brush_t( Floating_t height, Floating_t radius ) { init( height,radius ); }
	BRUSH_T( inline int )::vertexNumber() { return 2*N; }
	BRUSH_T( inline bool)::operator==( const Brush_t& rhs ) const { return compire( rhs ); }
	BRUSH_T( inline bool)::operator!=( const Brush_t& rhs ) const { return !compire( rhs ); }
	BRUSH_T( inline void*)::operator new( size_t, void* ptr ) { return ptr; }
	BRUSH_T( inline bool)::compire( const Brush_t& rhs ) const { for( size_t k = 0; k < 2*N; k ++) if (rhs.m_array[ k ] != m_array[ k ]) return false; return true; }
	BRUSH_T( inline Center_t )::center() const { Center_t c; for( int n = 0; n < N * 2; n ++) c += m_array[ n ]; return ((c /= N) /= 2.0).round(); }
	template< class V, int N >inline Brush_t< V,N >& Brush_t< V,N >::operator=( const Brush_t< V,N >& rhs ) { for( int n = 0; n < 2*N; n ++) m_array[ n ] = rhs.m_array[ n ]; return *this; }

#	pragma mark -

	BRUSH_T( inline void )::random_init()
		{
			for( int index = 0; index < Brush_t::vertexNumber()/2; index++ )
				{
					const long x = glo::random_between( -16,16 );
					const long y = glo::random_between( -16,16 );
					const long z = glo::random_between( 15,17 );
					switch( V::dim() )
						{
							case 3: m_array[ 2*index]= V (x,y,z );  m_array[ 2*index+1 ]= V (x,y,-z );  break;
							case 4: m_array[ 2*index]= V (x,y,z,1); m_array[ 2*index+1 ]= V (x,y,-z,1); break;
						}
				}
		}

	BRUSH_T( inline void )::inconvex_init( Floating_t H, Floating_t R )
		{
			static const Floating_t a_pi = 3.14159267;
			static const Floating_t zero = 0.001;
			const Floating_t D = 2 *a_pi / N;
			m_array[ 0 ] = V( R,0,0 );
			m_array[ N ] = V( R,0,H );
			for( int n = 1; n < N; n ++) // N is brush dimension.
				{
					Floating_t A = R * ::cos(n * D); if (::fabs( A ) < zero) { A = 0; }
					Floating_t B = R * ::sin(n * D); if (::fabs( B ) < zero) { B = 0; }
					m_array[n + 0] = V ( A,B,0 );
					m_array[n + N] = V ( A,B,H );
				}
		}

	BRUSH_T( inline void )::init( Floating_t H, Floating_t R )
		{
			static const Floating_t a_pi = 3.14159267;
			static const Floating_t zero = 0.001;
			const Floating_t D = 2 *a_pi / N;
			for( int n = 0; n < N; n ++) // 0,1,2
				{
					Floating_t A = R * ::cos(n * D); if (::fabs( A ) < zero) { A = 0; }
					Floating_t B = R * ::sin(n * D); if (::fabs( B ) < zero) { B = 0; }
					m_array[2*n + 0] = V ( A,B,0 ); // 0,2,4
					m_array[2*n + 1] = V ( A,B,H ); // 1,3,5
				}
		}

	BRUSH_T( inline void )::rotate( Floating_t angle )
		{
			for( int n = 0; n < N * 2; n ++)
				{
					// TODO: rotate array vertexes in camera coordinates
					cout << m_array[ n ];
				}
		}
}

#endif
#undef BRUSH_T

#define __THE_BRUSH_H__
#endif
