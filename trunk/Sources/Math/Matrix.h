/*
 *  Matrix.h
 *  the
 *
 *  Created by Alexander Zvyagin on 23.12.10.
 *  Copyright 2010 The(c)ompany. All rights reserved.
 *
 */

#ifndef __THE_MATRIX_H__

#include <strings.h>

typedef bool MAKE_ZERO_MATRIX;


template< class T, int D = 4 >class Matrix_t
	{
		Vector_t< T,D >C0; // column0 (in order of OpenGL load matrix as one long array of T.)
		Vector_t< T,D >C1; // column1
		Vector_t< T,D >C2; // column2
		Vector_t< T,D >C3; // column3

	public:
		Matrix_t( T a00, T a01, T a02, T a03,	// column0
				  T a04, T a05, T a06, T a07,	// column1
				  T a08, T a09, T a10, T a11,	// column2
				  T a12, T a13, T a14, T a15 );	// column3
		Matrix_t();
		Matrix_t( const Matrix_t& );
		Matrix_t( const Matrix_t&, const Matrix_t& );

		Matrix_t& identify();
		Matrix_t& transpose();
		Matrix_t& round();
		Matrix_t& translate( T dx, T dy, T dz );
		Matrix_t& translate( const Vector_t< T,D >& );
		Matrix_t& scale( T sx, T sy, T sz );
		Matrix_t& scale( const Vector_t< T,D >& );
		Matrix_t& x_rotate( T theta );
		Matrix_t& y_rotate( T theta );
		Matrix_t& z_rotate( T theta );
		Matrix_t& operator *= ( const Matrix_t& );
		Matrix_t& getCurrentMatrix( unsigned long type = GL_MODELVIEW_MATRIX, bool round = false );
		Matrix_t& operator = ( const Matrix_t& );

		bool operator == ( const Matrix_t& ) const;
		bool operator != ( const Matrix_t& ) const;

		Matrix_t Rounded() const;
		Matrix_t Transposed() const;
		Matrix_t Translated( const Vector_t< T,D >& ) const;
		Matrix_t Translated( T dx, T dy, T dz ) const;
		Matrix_t Scaled( T sx, T sy, T sz ) const;
		Matrix_t Scaled( const Vector_t< T,D >& ) const;
		Matrix_t X_Rotated( T theta ) const;
		Matrix_t Y_Rotated( T theta ) const;
		Matrix_t Z_Rotated( T theta ) const;

		Vector_t< T,D >& operator [] ( int );
		Vector_t< T,D >const& operator [] ( int )const;
		Vector_t< T,D >translationVector() const;
//		Vector_t< T,3 >translationPoint() const;

		void setTranslationVector( const Vector_t< T,D >& );
//		void setTranslationPoint( const Vector_t< T,3 >& );
		void setCurrentMode( unsigned long mode = GL_MODELVIEW, bool round = false ) const;

		static Matrix_t Scaling( const Vector_t< T,D >& );
		static Matrix_t Scaling( T dx, T dy, T dz );
		static Matrix_t Translation( const Vector_t< T,D >& );
		static Matrix_t Translation( T dx, T dy, T dz );
		static Matrix_t currentMatrix( unsigned long type = GL_MODELVIEW_MATRIX, bool round = false );

	private:
		Matrix_t( MAKE_ZERO_MATRIX );
		friend Matrix_t< T,D >glo_math::matrix_mul_matrix< T,D >( const Matrix_t< T,D >&, const Matrix_t< T,D >& );
	};

#	pragma mark -
#	pragma mark Construct

// IMPORTANT: Must use vectors C0-C3 initialization list to prevent set all vectors to zero in matrix constructors.

template< class T, int D >inline Matrix_t< T,D >
	::	Matrix_t()
	:	C0( 1, 0, 0, 0 )
	,	C1( 0, 1, 0, 0 )
	,	C2( 0, 0, 1, 0 )
	,	C3( 0, 0, 0, 1 )
	{
	}
template< class T, int D >inline Matrix_t< T,D >
	::	Matrix_t( const Matrix_t& rhs )
	:	C0( rhs.C0 )
	,	C1( rhs.C1 )
	,	C2( rhs.C2 )
	,	C3( rhs.C3 )
	{
	}
template< class T, int D >inline Matrix_t< T,D >
	::	Matrix_t( T a00, T a01, T a02, T a03,
				  T a04, T a05, T a06, T a07,
				  T a08, T a09, T a10, T a11,
				  T a12, T a13, T a14, T a15 )
	:	C0( a00, a01, a02, a03 )
	,	C1( a04, a05, a06, a07 )
	,	C2( a08, a09, a10, a11 )
	,	C3( a12, a13, a14, a15 )
	{
	}
template< class T, int D >inline Matrix_t< T,D >
	::	Matrix_t( MAKE_ZERO_MATRIX ) // private.
	:	C0( 0, 0, 0, 0 )
	,	C1( 0, 0, 0, 0 )
	,	C2( 0, 0, 0, 0 )
	,	C3( 0, 0, 0, 0 )
	{
	}

#pragma >>> object => camera vertex translation.

// re: typically source==object, target==camera spaces.

template< class T, int D >inline Matrix_t< T,D >
	::	Matrix_t( const Matrix_t< T,D >& source, const Matrix_t< T,D >& target )
	{
		//glLoadTransposeMatrixf/d()/glMultTransposeMatrixf/d().
		// convert back from source space direct to the target space (using transposed matrix for back convertion: see tests).
		self = glo_math::matrix_mul_matrix< T,D >( source.Transposed(), target );
	}

#pragma mark -
#pragma mark class functions

template< class T, int D >inline Vector_t< T,D >Matrix_t< T,D >::translationVector() const { return self[ 3 ]; }
template< class T, int D >inline void Matrix_t< T,D >::setTranslationVector( const Vector_t< T,D >& rhs ) { self[ 3 ] = rhs; }

template< class T, int D >inline Matrix_t< T,D >& Matrix_t< T,D >::identify() { DEF_IDENTIFY_ARRAY( self ); return self; }
template< class T, int D >inline Matrix_t< T,D >& Matrix_t< T,D >::transpose() { DEF_LOOP_FOR( D-1,I ) for( int J = I + 1; J < D; J++) { T Aij=self[I][J]; self[I][J]=self[J][I]; self[J][I]=Aij; } return self; }

// return modified self

template< class T, int D >inline Matrix_t< T,D >& Matrix_t< T,D >::x_rotate( T theta ) { DEF_TRIGONOMETRIC( T, theta ) return self *= Matrix_t< T,D >( DEF_X_ROTATION ); }
template< class T, int D >inline Matrix_t< T,D >& Matrix_t< T,D >::y_rotate( T theta ) { DEF_TRIGONOMETRIC( T, theta ) return self *= Matrix_t< T,D >( DEF_Y_ROTATION ); }
template< class T, int D >inline Matrix_t< T,D >& Matrix_t< T,D >::z_rotate( T theta ) { DEF_TRIGONOMETRIC( T, theta ) return self *= Matrix_t< T,D >( DEF_Z_ROTATION ); }

template< class T, int D >inline Matrix_t< T,D >& Matrix_t< T,D >::translate( T dx, T dy, T dz ) { return self *= Matrix_t< T,D >::Translation( dx,dy,dz ); } // the right version.
//template< class T, int D >inline Matrix_t< T,D >& Matrix_t< T,D >::translate( T dx, T dy, T dz ) { array[3] += Vector_t< T,D >( dx,dy,dz ); array[3][3] = 1; return self; }

template< class T, int D >inline Matrix_t< T,D >& Matrix_t< T,D >::translate( const Vector_t< T,D >& dV ) { return Matrix_t< T,D >::translate( dV.X, dV.Y, dV.Z ); }
template< class T, int D >inline Matrix_t< T,D >& Matrix_t< T,D >::scale( T sx, T sy, T sz ) { return self *= Matrix_t< T,D >::Scaling( sx,sy,sz ); }
template< class T, int D >inline Matrix_t< T,D >& Matrix_t< T,D >::scale( const Vector_t< T,D >& dS ) { return Matrix_t< T,D >::scale( DEF_SCALING( dS.X, dS.Y, dS.Z ) ); }

// return new modified object

template< class T, int D >inline Matrix_t< T,D >Matrix_t< T,D >::X_Rotated( T theta ) const { DEF_TRIGONOMETRIC( T, theta ) return self * Matrix_t< T,D >( DEF_X_ROTATION ); }
template< class T, int D >inline Matrix_t< T,D >Matrix_t< T,D >::Y_Rotated( T theta ) const { DEF_TRIGONOMETRIC( T, theta ) return self * Matrix_t< T,D >( DEF_Y_ROTATION ); }
template< class T, int D >inline Matrix_t< T,D >Matrix_t< T,D >::Z_Rotated( T theta ) const { DEF_TRIGONOMETRIC( T, theta ) return self * Matrix_t< T,D >( DEF_Z_ROTATION ); }
template< class T, int D >inline Matrix_t< T,D >Matrix_t< T,D >::Translated( const Vector_t< T,D >& dV ) const { Matrix_t< T,D >temp = self; return temp.translate( dV ); }
template< class T, int D >inline Matrix_t< T,D >Matrix_t< T,D >::Translated( T dx, T dy, T dz ) const { Matrix_t< T,D >temp = self; return temp.translate( dx,dy,dz ); }
template< class T, int D >inline Matrix_t< T,D >Matrix_t< T,D >::Scaled( T sx, T sy, T sz ) const { return self * Matrix_t< T,D >::Scaling( sx,sy,sz ); }
template< class T, int D >inline Matrix_t< T,D >Matrix_t< T,D >::Scaled( const Vector_t< T,D >& dS ) const { return Matrix_t< T,D >::scale( DEF_SCALING( dS.X, dS.Y, dS.Z ) ); }
template< class T, int D >inline Matrix_t< T,D >Matrix_t< T,D >::Transposed() const { Matrix_t< T,D >temp = self; return temp.transpose(); }

#pragma mark -
#pragma mark static functions

template< class T, int D >inline Matrix_t< T,D >Matrix_t< T,D >::Translation( const Vector_t< T,D >& dV ) { return Matrix_t< T,D >::Translation( dV.X, dV.Y, dV.Z ); }
template< class T, int D >inline Matrix_t< T,D >Matrix_t< T,D >::Translation( T dx, T dy, T dz ) { return Matrix_t< T,D >( DEF_TRANSLATION( dx,dy,dz ) ); }
template< class T, int D >inline Matrix_t< T,D >Matrix_t< T,D >::Scaling( const Vector_t< T,D >& sV ) { return Matrix_t< T,D >::Scaling( sV.X, sV.Y, sV.Z ); }
template< class T, int D >inline Matrix_t< T,D >Matrix_t< T,D >::Scaling( T sx, T sy, T sz ) { return Matrix_t< T,D >( DEF_SCALING( sx,sy,sz ) ); }


#pragma mark -
#pragma mark class operators

template< class T, int D >inline bool Matrix_t< T,D >::operator == ( const Matrix_t& rhs ) const { DEF_DOUBLE_LOOP_FOR( D,i,j ) if (self[i][j]!=rhs[i][j]) return false; return true; }
template< class T, int D >inline bool Matrix_t< T,D >::operator != ( const Matrix_t& rhs ) const { DEF_DOUBLE_LOOP_FOR( D,i,j ) if (self[i][j]!=rhs[i][j]) return true; return false; }

template< class T, int D >inline Vector_t< T,D >& Matrix_t< T,D >::operator [] ( int index ) { return ((Vector_t< T,D >*)this)[ index ]; }
template< class T, int D >inline Vector_t< T,D >const& Matrix_t< T,D >::operator [] ( int index ) const { return ((Vector_t< T,D >*)this)[ index ]; }

template< class T, int D >inline Matrix_t< T,D >& Matrix_t< T,D >::operator *= ( const Matrix_t& rhs ) { self = glo_math::matrix_mul_matrix< T,D >( self, rhs ); return self; }
template< class T, int D >inline Matrix_t< T,D >& Matrix_t< T,D >::operator = ( const Matrix_t& rhs )
	{
#	ifndef DEF_USE_MEMCPY_AND_MEMSET
		DEF_DOUBLE_LOOP_FOR( D,i,j )  self[ i ][ j ] = rhs[ i ][ j ];  return self;
#	else
		::memcpy( this, &rhs, sizeof( Matrix_t< T,D > ) ); return self;
#	endif
	}

#pragma mark -
#pragma mark global operators

// See mostly called function:

template< class T, int D >inline Matrix_t< T,D >operator *( const Matrix_t< T,D >& A, const Matrix_t< T,D >& B ) { return glo_math::matrix_mul_matrix< T,D >( A,B ); }

#pragma mark -
#pragma mark rounding

template< class T, int D > inline Matrix_t< T,D >& Matrix_t< T,D >::round() { DEF_DOUBLE_LOOP_FOR( D,I,J ) { T value = self[I][J]; self[I][J] = DEF_ROUND_EXPRESSION; } return self; }
template< class T, int D > inline Matrix_t< T,D >Matrix_t< T,D >::Rounded() const { Matrix_t temp = self; DEF_DOUBLE_LOOP_FOR( D,I,J ) { T value = temp[I][J]; temp[I][J] = DEF_ROUND_EXPRESSION; } return temp; }

#pragma mark -
#pragma mark Specialized OpenGL operations with 4x4 matrices only:

template <> inline Matrix_t< float,4 >Matrix_t< float,4 >::currentMatrix( unsigned long type, bool flag ) { Matrix_t temp; ::glGetFloatv( type,( float* )&temp ); return flag ? temp.Rounded() : temp; }
template <> inline Matrix_t< double,4 >Matrix_t< double,4 >::currentMatrix( unsigned long type, bool flag ) { Matrix_t temp; ::glGetDoublev( type,( double* )&temp ); return flag ? temp.Rounded() : temp; }
template <> inline Matrix_t< float,4 >& Matrix_t< float,4 >::getCurrentMatrix( unsigned long type, bool flag ) { ::glGetFloatv( type,( float* )this ); return flag ? self.round() : self; }
template <> inline Matrix_t< double,4 >& Matrix_t< double,4 >::getCurrentMatrix( unsigned long type, bool flag ) { ::glGetDoublev( type,( double*)this ); return flag ? self.round() : self; }
template <> inline void Matrix_t< float,4 >::setCurrentMode( unsigned long temp, bool flag ) const
	{
		GLint mode = 0; ::glGetIntegerv( GL_MATRIX_MODE, &mode ); ::glMatrixMode( temp );
		if (!flag) ::glLoadMatrixf( (float*)this ); else { Matrix_t temp = Rounded(); ::glLoadMatrixf( (float*)&temp ); } ::glMatrixMode( mode );
	}
template <> inline void Matrix_t< double,4 >::setCurrentMode( unsigned long temp, bool flag ) const
	{
		GLint mode = 0; ::glGetIntegerv( GL_MATRIX_MODE, &mode ); ::glMatrixMode( temp );
		if (!flag) ::glLoadMatrixd( (double*)this ); else { Matrix_t temp = Rounded(); ::glLoadMatrixd( (double*)&temp ); } ::glMatrixMode( mode );
	}

#define __THE_MATRIX_H__
#endif
