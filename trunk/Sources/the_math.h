////////////////////////////////////////////////////////////////////////////////
/// @file the_math.h
/// Artifical Y-editor ))
///
/// @author Atz
/// @date Okt-Nov 2010.
///
////////////////////////////////////////////////////////////////////////////////

// >>> starting private library header <<<

#ifndef __THE_MATH_H__

// please include "the.h" instead of this file.

#include <math.h>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

#define self (*this )

#define DEF_LOOP_FOR( dim, index )\
	for( int index = 0; index < dim; index++ )

#define DEF_DOUBLE_LOOP_FOR( dim, index1, index2 )\
	DEF_LOOP_FOR( dim, index1 )\
	DEF_LOOP_FOR( dim, index2 )

#define DEF_TRIPLE_LOOP_FOR( dim, index1, index2, index3 )\
	DEF_DOUBLE_LOOP_FOR( dim, index1, index2 )\
	DEF_LOOP_FOR( dim, index3 )

#define DEF_IDENTIFY_ARRAY( ARRAY ) \
	DEF_DOUBLE_LOOP_FOR( D,I,J ) ARRAY[ I ][ J ] = I==J ? 1 : 0;

#define DEF_FOREACH( TYPE, INSTANCE, ITERATOR ) \
	for( TYPE::iterator ITERATOR = INSTANCE.begin(); iterator != INSTANCE.end(); iterator++ )

#define DEF_FOREACH_CONST( TYPE, INSTANCE, ITERATOR ) \
	for( TYPE::const_iterator ITERATOR = INSTANCE.begin(); iterator != INSTANCE.end(); iterator++ )

// IMPORTANT:
// See translations, scaling, rotations and identity below
// are defined as OpenGL-styled vectors-column:

#define DEF_TRANSLATION( X,Y,Z ) 1,0,0,0, 0,1,0,0, 0,0,1,0, X,Y,Z,1
#define DEF_SCALING( X,Y,Z ) X,0,0,0, 0,Y,0,0, 0,0,Z,0, 0,0,0,1
#define DEF_IDENTITY 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1

#define DEF_X_ROTATION 1,0,0,0, 0,costheta,sintheta,0, 0,-sintheta,costheta,0, 0,0,0,1
#define DEF_Y_ROTATION costheta,0,-sintheta,0, 0,1,0,0, sintheta,0,costheta,0, 0,0,0,1
#define DEF_Z_ROTATION costheta,sintheta,0,0, -sintheta,costheta,0,0, 0,0,1,0, 0,0,0,1

#define DEF_TRIGONOMETRIC( T, theta )\
	T sintheta = ::sin( theta );\
	T costheta = ::cos( theta );

// round to 1
#define DEF_ROUND_EXPRESSION \
	(value > DEF_ROUND_MAX && value < DEF_ROUND_MAX + 2*DEF_ROUND_MIN) ? 1.0 : ((value > 0.0 && value < DEF_ROUND_MIN) ? 0.0 : value)

#define DEF_ROUND_MIN 0.005
#define DEF_ROUND_MAX 0.995

#define DEF_FLOATING_POINT_DENSITY 0.0001

#define DEF_CAMERA_TRANSLATION_DISTANCE 102

#ifndef PI
#define PI 3.1415926
#endif

// default matrix is |E|
// default vector is {0}

#	define DEF_VIEWPORT_RECT( HALF, SIZE )\
	HALF *-SIZE.W, HALF * SIZE.W, HALF *-SIZE.H, HALF * SIZE.H

////////////////////////////////////////////////////////////////////////////////

namespace The
{
#	define self (*this )

	typedef GLdouble Floating_t;
	typedef GLint Integer_t;
	typedef UINT Index_t;

#	include "Common.h"
#	include "Pixel.h"
#	include "Size.h"
#	include "Vector.h"
#	include "Matrix.h"

#	ifdef DEF_USE_ONLY_FLOATING_TYPES // depricated: use only double precision to avoid ruler bug.
		typedef Size_t< Floating_t >Size;
		typedef Rect_t< Floating_t >Rect;
#		ifdef DEF_USE_PIXEL_INSTEAD_OF_VECTOR
			typedef Pixel_t< Floating_t >Pixel;
#		else
			typedef Vector_t< Floating_t,2 >Pixel;
#		endif
		typedef Vector_t< Floating_t,3 >Point;
#	else
		typedef Size_t< Integer_t >Size;
		typedef Rect_t< Integer_t >Rect;
	
#		ifdef DEF_USE_PIXEL_INSTEAD_OF_VECTOR
			typedef Pixel_t< Integer_t >Pixel;
#		else
			typedef Vector_t< Integer_t,2 >Pixel;
#		endif
		typedef Vector_t< Integer_t,3 >Point;
#	endif

	typedef Matrix_t< Floating_t,4 >Matrix;
	typedef Vector_t< Floating_t,4 >Vector;
	typedef Vector_t< Floating_t,4 >Vertex;
	typedef Vector_t< Vector*,2 >Line; // XXX:4->3 ?..

#	include "Rect.h" // will use Point type in contains() after it defined behind.

#	ifdef self
#	undef self
#	endif
}
// The

////////////////////////////////////////////////////////////////////////////////

#define __THE_MATH_H__
#endif
