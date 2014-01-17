/*
 *  Vector.h
 *  the
 *
 *  Created by Alexander Zvyagin on 23.12.10.
 *  Copyright 2010 The(c)ompany. All rights reserved.
 *
 */

#ifndef __THE_VECTOR_H__

#include <new>
#include <strings.h>

namespace trait
	{
		template< class T >int density();
		template <> inline int density< double >() { return GL_DOUBLE; }
		template <> inline int density< float >() { return GL_FLOAT; }
		template <> inline int density< int >() { return GL_INT; }
		template <> inline int density< short >() { return GL_SHORT; }
	}

template< class T, int D >class Vector_base
	{
		protected: T array[ D ];
		public:	Vector_base();
	};

template< class T, int D >inline Vector_base< T,D >
	::	Vector_base()
	{
		// IMPORTANT: Do NOT zero array!
	}

#pragma mark -

template< class T >class Vector_base< T,2 >
	{
		protected: union
			{
				T array[ 2 ];
				public: struct { T X; T Y; };
			};
		public:	Vector_base(); Vector_base( T x, T y, T, T );
	};

template< class T >inline Vector_base< T,2 >
	::	Vector_base()
	{
		// IMPORTANT: Do NOT zero array!
	}

template< class T >inline Vector_base< T,2 >
	::	Vector_base( T x, T y, T, T )
	:	X( x )
	,	Y( y )
	{
	}

#pragma mark -

template< class T >struct Vector_base< T,3 >
	{
		Vector_base();
		Vector_base( T x, T y, T z, T );
		Vector_base( const Vector_base< T,4 >& );
		protected: union
			{
				T array[ 3 ];
				public: struct { T X; T Y; T Z; };
			};
	};

template< class T >inline Vector_base< T,3 >
	::	Vector_base()
	{
		// IMPORTANT: Do NOT zero array!
	}

template< class T >inline Vector_base< T,3 >
	::	Vector_base( T x, T y, T z, T )
	:	X( x )
	,	Y( y )
	,	Z( z )
	{
	}

template< class T >inline Vector_base< T,3 >
	::Vector_base( const Vector_base< T,4 >& rhs )
	:	X( rhs.X )
	,	Y( rhs.Y )
	,	Z( rhs.Z )
	{
	}

#pragma mark -

template< class T >struct Vector_base< T,4 >
	{
		Vector_base();
		Vector_base( T x, T y, T z, T w );
		protected: union
			{
				T array[ 4 ];
				public: struct { T X; T Y; T Z; T W; };
			};
	};

template< class T >inline Vector_base< T,4 >
	::	Vector_base()
	{
		// IMPORTANT: Do NOT zero array (else can't copy matrix to self during states change)!
	}

template< class T >inline Vector_base< T,4 >
	::	Vector_base( T x, T y, T z, T w )
	:	X( x )
	,	Y( y )
	,	Z( z )
	,	W( w ) // typically set to zero by default.
	{
	}

#pragma mark -
#pragma mark class Vector_t { . . . }

template< class T, int D >struct Vector_t: public Vector_base< T,D >
	{
		Vector_t();
		Vector_t( const Vector_t& rhs );
		Vector_t( T x, T y, T z, T w );
		Vector_t( T x, T y, T z );
		Vector_t( T x, T y );
		const
		T& operator [] ( int index ) const;
		T& operator [] ( int index );
		bool operator == ( const Vector_t& rhs ) const;
		bool operator != ( const Vector_t& rhs ) const;
		Vector_t< T,D >operator + ( const T& rhs ) const;
		Vector_t< T,D >operator - ( const T& rhs ) const;
		Vector_t< T,D >operator * ( const T& rhs ) const;
		Vector_t< T,D >operator / ( const T& rhs ) const;
		Vector_t< T,D >operator - ();
		Vector_t< T,D >& operator  = ( const Vector_t& rhs );
		Vector_t< T,D >& operator += ( const Vector_t& rhs );
		Vector_t< T,D >& operator -= ( const Vector_t& rhs );
		Vector_t< T,D >& operator *= ( const Vector_t& rhs );
		Vector_t< T,D >& operator /= ( const Vector_t& rhs );
		Vector_t< T,D >& operator *= ( const Matrix_t< T,D >& rhs );
		Vector_t< T,D >& operator += ( const T& rhs );
		Vector_t< T,D >& operator -= ( const T& rhs );
		Vector_t< T,D >& operator *= ( const T& rhs );
		Vector_t< T,D >& operator /= ( const T& rhs );
		Vector_t< T,D >& skip4thCoordinate();
		Vector_t< T,D >& round();
		Vector_t< T,D >Rounded() const;
		inline void* operator new( size_t, void* ptr ) { return ptr; }
		static int type();
		static int dim();
	};

#pragma mark -
#pragma mark vector members

template< class T, int D >inline Vector_t< T,D >
	::	Vector_t( T x, T y ) // w must be always manualy set to 1 while create vertex.
	:	Vector_base< T,D >( x,y,1,1 )
	{
		// do nothing...
	}
template< class T, int D >inline Vector_t< T,D >
	::	Vector_t( T x, T y, T z ) // w must be always manualy set to 1 while create vertex.
	:	Vector_base< T,D >( x,y,z,1 )
	{
		// do nothing...
	}
template< class T, int D >inline Vector_t< T,D >
	::	Vector_t( T x, T y, T z, T w ) // w must be always manualy set to 1 while create vertex.
	:	Vector_base< T,D >( x,y,z,w )
	{
		// do nothing...
	}
template< class T, int D >inline Vector_t< T,D >
	::	Vector_t( const Vector_t< T,D >& rhs )
	{
#	ifndef DEF_USE_MEMCPY_AND_MEMSET
		DEF_LOOP_FOR( D,i ) self[ i ] = rhs[ i ];
#	else
		::memcpy( this, &rhs, sizeof( Vector_t< T,D > ) );
		//::memcpy( &Vector_base< T,D >::array[ 0 ], &rhs.Vector_base< T,D >::array[ 0 ], sizeof( T ) );
#	endif
	}
template< class T, int D >inline Vector_t< T,D >
	::	Vector_t()
	{
#	ifndef DEF_USE_MEMCPY_AND_MEMSET
		DEF_LOOP_FOR( D,i ) Vector_base< T,D >::array[ i ] = 0.0;
#	else
		::memset( this, 0.0, sizeof( Vector_t< T,D > ) );
		//::memset( &Vector_base< T,D >::array[ 0 ], 0.0, sizeof( T ) );
#	endif
	}

template< class T, int D >inline Vector_t< T,D >& Vector_t< T,D >::operator = ( const Vector_t< T,D >& rhs )
	{
#	ifndef DEF_USE_MEMCPY_AND_MEMSET
		DEF_LOOP_FOR( D,i ) self[i] = rhs[i]; return self;
#	else
		::memcpy( this, &rhs, sizeof( Vector_t< T,D >) ); return self;
#endif
	}

template< class T, int D >inline bool Vector_t< T,D >::operator == ( const Vector_t& rhs ) const { return glo_math::vector_cmp_vector< T,D >( self, rhs ); }
template< class T, int D >inline bool Vector_t< T,D >::operator != ( const Vector_t& rhs ) const { return!glo_math::vector_cmp_vector< T,D >( self, rhs ); }

template< class T, int D >inline T& Vector_t< T,D >::operator [] ( int index ) { return Vector_base< T,D >::array[ index ]; }
template< class T, int D >inline const T& Vector_t< T,D >::operator [] ( int index ) const { return Vector_base< T,D >::array[ index ]; }

template< class T, int D >inline Vector_t< T,D >& Vector_t< T,D >::operator *= ( const Matrix_t< T,D >& rhs ) { self = glo_math::vector_mul_matrix< T,D >( self, rhs ); return self; }
template< class T, int D >inline Vector_t< T,D >Vector_t< T,D >::operator -() { Vector_t< T,D >opposite; DEF_LOOP_FOR( D,i ) opposite[i] = -self[i]; return opposite; }

template< class T, int D >inline Vector_t< T,D >Vector_t< T,D >::operator + ( const T& rhs ) const { return glo_math::const_vector_add_skalar< T,D >( self, rhs ); }
template< class T, int D >inline Vector_t< T,D >Vector_t< T,D >::operator - ( const T& rhs ) const { return glo_math::const_vector_sub_skalar< T,D >( self, rhs ); }
template< class T, int D >inline Vector_t< T,D >Vector_t< T,D >::operator * ( const T& rhs ) const { return glo_math::const_vector_mul_skalar< T,D >( self, rhs ); }
template< class T, int D >inline Vector_t< T,D >Vector_t< T,D >::operator / ( const T& rhs ) const { return glo_math::const_vector_div_skalar< T,D >( self, rhs ); }

template< class T, int D >inline Vector_t< T,D >& Vector_t< T,D >::operator += ( const Vector_t< T,D >& rhs ) { glo_math::vector_add_vector< T,D >( self, rhs ); return self; }
template< class T, int D >inline Vector_t< T,D >& Vector_t< T,D >::operator -= ( const Vector_t< T,D >& rhs ) { glo_math::vector_sub_vector< T,D >( self, rhs ); return self; }
template< class T, int D >inline Vector_t< T,D >& Vector_t< T,D >::operator *= ( const Vector_t< T,D >& rhs ) { glo_math::vector_mul_vector< T,D >( self, rhs ); return self; }
template< class T, int D >inline Vector_t< T,D >& Vector_t< T,D >::operator /= ( const Vector_t< T,D >& rhs ) { glo_math::vector_div_vector< T,D >( self, rhs ); return self; }

template< class T, int D >inline Vector_t< T,D >& Vector_t< T,D >::operator += ( const T& rhs ) { return glo_math::vector_add_skalar< T,D >( self, rhs ); }
template< class T, int D >inline Vector_t< T,D >& Vector_t< T,D >::operator -= ( const T& rhs ) { return glo_math::vector_sub_skalar< T,D >( self, rhs ); }
template< class T, int D >inline Vector_t< T,D >& Vector_t< T,D >::operator *= ( const T& rhs ) { return glo_math::vector_mul_skalar< T,D >( self, rhs ); }
template< class T, int D >inline Vector_t< T,D >& Vector_t< T,D >::operator /= ( const T& rhs ) { return glo_math::vector_div_skalar< T,D >( self, rhs ); }

template< class T, int D >inline Vector_t< T,D >operator + ( const Vector_t< T,D >& A, const Vector_t< T,D >& B ) { return glo_math::const_vector_add_vector< T,D >( A,B ); }
template< class T, int D >inline Vector_t< T,D >operator - ( const Vector_t< T,D >& A, const Vector_t< T,D >& B ) { return glo_math::const_vector_sub_vector< T,D >( A,B ); }
template< class T, int D >inline Vector_t< T,D >operator / ( const Vector_t< T,D >& A, const Vector_t< T,D >& B ) { return glo_math::const_vector_div_vector< T,D >( A,B ); }

template< class T, int D >inline Vector_t< T,D >operator * ( const Vector_t< T,D >& V, const Matrix_t< T,D >& M ) { return glo_math::vector_mul_matrix< T,D >( V,M ); }
template< class T >inline Vector_t< T,3 >operator * ( const Vector_t< T,3 >& V, const Matrix_t< T,4 >& M ) { return glo_math::vector_mul_matrix( V,M ); }

template< class T, int D >inline Vector_t< T,D >& Vector_t< T,D >::round() { return glo_math::round_vector( self ); }
template< class T, int D >inline Vector_t< T,D >Vector_t< T,D >::Rounded() const { Vector_t rc = self; return rc.round(); }

// 4th part to 1: will only applied to 4D vectors.
template< class T, int D >inline Vector_t< T,D >& Vector_t< T,D >::skip4thCoordinate() { if (4==D) Vector_base< T,D >::array[ 3 ] = 1.0; return self; }
template< class T, int D >inline int Vector_t< T,D >::type() { return trait::density< T >(); }
template< class T, int D >inline int Vector_t< T,D >::dim() { return D; }

#define __THE_VECTOR_H__
#endif
