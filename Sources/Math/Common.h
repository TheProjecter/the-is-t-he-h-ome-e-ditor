/*
 *  Size.h
 *  the
 *
 *  Created by Alexander Zvyagin on 23.12.10.
 *  Copyright 2010 The(c)ompany. All rights reserved.
 *
 */

#ifndef __THE_GLOBALS_H__

#pragma mark Forward Declarations

template< class T >union Size_t;
template< class T >struct Rect_t;
template< class T >struct Pixel_t;

template< class T, int D >class Matrix_t;
template< class T, int D >class Vector_t;

template< class T >std::ostream& operator << ( std::ostream&, const Size_t< T >& );
template< class T >std::ostream& operator << ( std::ostream&, const Rect_t< T >& );

template< class T, int D >std::ostream& operator << ( std::ostream&, const Vector_t< T,D >& );
template< class T, int D >std::ostream& operator << ( std::ostream&, const Matrix_t< T,D >& );

#pragma mark -
#pragma mark namespace convert

namespace convert
	{
		inline int mode_from_matrixmode( int matrixmode )
			{
				switch( matrixmode )
					{
						case GL_MODELVIEW_MATRIX: return GL_MODELVIEW;
						case GL_PROJECTION_MATRIX: return GL_PROJECTION;
						case GL_TEXTURE_MATRIX: return GL_TEXTURE;
						default:return 0;
					}
			}
		inline int matrixmode_from_mode( int mode )
			{
				switch( mode )
					{
						case GL_MODELVIEW: return GL_MODELVIEW_MATRIX;
						case GL_PROJECTION: return GL_PROJECTION_MATRIX;
						case GL_TEXTURE: return GL_TEXTURE_MATRIX;
						case GL_COLOR: std::cout << "convert::matrixmode_from_mode -> no GL_COLOR matrix type" << std::endl;
						default: return 0;
					}
			}
	}

#pragma mark -
#pragma mark namespace glo_math

namespace glo_math
{
	template< class T, int D >static inline Vector_t< T,D >& round_vector( Vector_t< T,D >& V ) { DEF_LOOP_FOR( D,i ) { if (DEF_FLOATING_POINT_DENSITY >::abs( V[i] )) V[i] = 0; } return V; }
	template< class T, int D >static inline bool vector_cmp_vector( const Vector_t< T,D >& A, const Vector_t< T,D >& B ) { DEF_LOOP_FOR( D,i ) if (A[i] != B[i]) return false; return true; }

#	pragma mark -
#	pragma mark vector_op_skalar (will return a 1st argument)

	template< class T, int D >static inline Vector_t< T,D >& vector_add_skalar( Vector_t< T,D >& A, const T& B ) { DEF_LOOP_FOR( D,i ) A[i]+= B; return A; }
	template< class T, int D >static inline Vector_t< T,D >& vector_sub_skalar( Vector_t< T,D >& A, const T& B ) { DEF_LOOP_FOR( D,i ) A[i]-= B; return A; }
	template< class T, int D >static inline Vector_t< T,D >& vector_mul_skalar( Vector_t< T,D >& A, const T& B ) { DEF_LOOP_FOR( D,i ) A[i]*= B; return A; }
	template< class T, int D >static inline Vector_t< T,D >& vector_div_skalar( Vector_t< T,D >& A, const T& B ) { DEF_LOOP_FOR( D,i ) A[i]/= B; return A; }

#	pragma mark -
#	pragma mark vector_op_vector (will return a 1st argument)

	template< class T, int D >static inline Vector_t< T,D >& vector_add_vector( Vector_t< T,D >& A, const Vector_t< T,D >& B ) { DEF_LOOP_FOR( D,i ) A[i]+= B[i]; return A; }
	template< class T, int D >static inline Vector_t< T,D >& vector_sub_vector( Vector_t< T,D >& A, const Vector_t< T,D >& B ) { DEF_LOOP_FOR( D,i ) A[i]-= B[i]; return A; }
	template< class T, int D >static inline Vector_t< T,D >& vector_mul_vector( Vector_t< T,D >& A, const Vector_t< T,D >& B ) { DEF_LOOP_FOR( D,i ) A[i]*= B[i]; return A; }
	template< class T, int D >static inline Vector_t< T,D >& vector_div_vector( Vector_t< T,D >& A, const Vector_t< T,D >& B ) { DEF_LOOP_FOR( D,i ) A[i]/= B[i]; return A; }

#	pragma mark -
#	pragma mark const_vector_op_skalar (will return a 1st argument)

	template< class T, int D >static inline Vector_t< T,D >const_vector_add_skalar( const Vector_t< T,D >& A, const T& B ) { Vector_t< T,D >C = A; DEF_LOOP_FOR( D,i ) C[i]+= B; return C; }
	template< class T, int D >static inline Vector_t< T,D >const_vector_sub_skalar( const Vector_t< T,D >& A, const T& B ) { Vector_t< T,D >C = A; DEF_LOOP_FOR( D,i ) C[i]-= B; return C; }
	template< class T, int D >static inline Vector_t< T,D >const_vector_mul_skalar( const Vector_t< T,D >& A, const T& B ) { Vector_t< T,D >C = A; DEF_LOOP_FOR( D,i ) C[i]*= B; return C; }
	template< class T, int D >static inline Vector_t< T,D >const_vector_div_skalar( const Vector_t< T,D >& A, const T& B ) { Vector_t< T,D >C = A; DEF_LOOP_FOR( D,i ) C[i]/= B; return C; }

#	pragma mark -
#	pragma mark const_vector_op_vector (will return a 1st argument)

	template< class T, int D >static inline Vector_t< T,D >const_vector_add_vector( const Vector_t< T,D >& A, const Vector_t< T,D >& B ) { Vector_t< T,D >C = A; DEF_LOOP_FOR( D,i ) C[i]+= B[i]; return C; }
	template< class T, int D >static inline Vector_t< T,D >const_vector_sub_vector( const Vector_t< T,D >& A, const Vector_t< T,D >& B ) { Vector_t< T,D >C = A; DEF_LOOP_FOR( D,i ) C[i]-= B[i]; return C; }
	template< class T, int D >static inline Vector_t< T,D >const_vector_mul_vector( const Vector_t< T,D >& A, const Vector_t< T,D >& B ) { Vector_t< T,D >C = A; DEF_LOOP_FOR( D,i ) C[i]*= B[i]; return C; }
	template< class T, int D >static inline Vector_t< T,D >const_vector_div_vector( const Vector_t< T,D >& A, const Vector_t< T,D >& B ) { Vector_t< T,D >C = A; DEF_LOOP_FOR( D,i ) C[i]/= B[i]; return C; }

#	pragma mark -
#	pragma mark matrix-and-vector

	static const bool make_zero_matrix = true;

	//	See these functions are mostly called.

	template< class T, int D >static inline Matrix_t< T,D >matrix_mul_matrix( const Matrix_t< T,D >& A, const Matrix_t< T,D >& B ) { Matrix_t< T,D >M( make_zero_matrix ); DEF_TRIPLE_LOOP_FOR( D,I,J,K ) M[I][J]+= A[I][K]* B[K][J]; return M; }
	template< class T, int D >static inline Vector_t< T,D >vector_mul_matrix( const Vector_t< T,D >& A, const Matrix_t< T,D >& B ) { Vector_t< T,D >V; DEF_DOUBLE_LOOP_FOR( D,I,J ) V[I]+= A[J]* B[J][I]; return V/*.skip4thCoordinate()*/; }
	template< class T        >static inline Vector_t< T,3 >vector_mul_matrix( const Vector_t< T,3 >& a, const Matrix_t< T,4 >& B )
		{
			Vector_t< T,4 >A( a.X, a.Y, a.Z, 1.0 ); Vector_t< T,4 >V; DEF_DOUBLE_LOOP_FOR( 4,I,J ) V[I]+= A[J]* B[J][I];
			Vector_t< T,3 >v( V.X, V.Y, V.Z ); return v; // a different vector sizes to use memcpy.
		}
	
	// IMPORTANT: See OpenGL will convert matrix 1st by columns, next by rows! (matrix vectors is vertical!!! translation vector is mostly right!)
	template< class T, int D >static inline T* matrix_raw_array( const Matrix_t< T,D >& rhs ) { return( T*)&rhs; }
}


template< class >void transpose_opengl_matrix( GLenum type = GL_MODELVIEW );



template <> inline void transpose_opengl_matrix< GLfloat >( GLenum type )
	{
		GLfloat M[ 16 ], T[ 16 ]; ::glGetFloatv( type, M ); DEF_LOOP_FOR( 4,i ) DEF_LOOP_FOR( 4,j )
			T [4 * i + j] = (i < 3) && (j < 3) ? M [i + 4 * j] : M [4 * i + j]; ::glLoadMatrixf( T );
	}

template <> inline void transpose_opengl_matrix< GLdouble >( GLenum type )
	{
		GLdouble M[ 16 ], T[ 16 ]; ::glGetDoublev( type, M ); DEF_LOOP_FOR( 4,i ) DEF_LOOP_FOR( 4,j )
			T [4 * i + j] = (i < 3) && (j < 3) ? M [i + 4 * j] : M [4 * i + j]; ::glLoadMatrixd( T );
	}

#pragma mark -
#pragma mark Friends: + - for Pixel

template< class T >inline Pixel_t< T >operator + ( const Pixel_t< T >& A, const Pixel_t< T >& B );
template< class T >inline Pixel_t< T >operator - ( const Pixel_t< T >& A, const Pixel_t< T >& B );

#pragma mark -
#pragma mark iostreams

template< class T >inline std::ostream& operator << ( std::ostream& os, const Size_t< T >& rhs )
	{
		return os << rhs.W << "_x_" << rhs.H << std::endl;
	}

template< class T >inline std::ostream& operator << ( std::ostream& os, const Rect_t< T >& rhs )
	{
	//	return os << "Rect:" << '{' << rhs.pos().X << ',' << rhs.pos().Y << '}' << '_' << '[' << rhs.width() << ',' << rhs.height() << ']' << std::endl;
		return os << "Rect:" << '{' << rhs.pos().X << ',' << rhs.pos().Y << '}' << '-' << '[' << rhs.pos().X + rhs.width() << ',' << rhs.pos().Y + rhs.height() << ']' << std::endl;
	}

template< class T, int D >inline std::ostream& operator << ( std::ostream& os, const Vector_t< T,D >& rhs )
	{
		DEF_LOOP_FOR( D,I ) os << '\t' << rhs[ I ]; return os << std::endl;
	}

template< class T, int D >inline std::ostream& operator << ( std::ostream& os, const Matrix_t< T,D >& rhs )
	{
		//	int count = 0;
		DEF_LOOP_FOR( D,i ) {
			DEF_LOOP_FOR( D,j ) os << '\t'/* << count++ << ':' */<< rhs[ j ][ i ];
			os << std::endl;
		}	os << std::endl; return os;
	}

#define __THE_GLOBALS_H__
#endif
