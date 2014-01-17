////////////////////////////////////////////////////////////////////////////////
///  the.h
///  the
///
///  Created by Atz on 17.11.10.
///  Copyright 2010 The(c)ompany. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __THE_H__

#include "ogl.h"

#	define DEF_MANHATTAN_DISTANCE_PX 5
#	define DEF_DEFAULT_BRUSH_CAPACITY 100
//#	define DEF_CLEAR_COLOR_WHITE

//#	define DEF_USE_PIXEL_INSTEAD_OF_VECTOR
//#	define DEF_USE_GLUT_TO_SET_PROJECTION
//#	define DEF_USE_OPENGL_MATH_OPS
#	define DEF_USE_MEMCPY_AND_MEMSET

//#	define DEF_LOG_COORDINATES
//#	define DEF_LOG_EVENTS
//#	define DEF_LOG_MODELVIEW
#	define DEF_LOG_VERTEX_ARRAYS

#	define DEF_USE_ONLY_FLOATING_TYPES

#include "the_types.h"
#include "the_math.h"
#include "Utility.h"
#include "Modifiers.h"
#include "Keys.h"
#include "Mouse.h"
#include "View.h"
#include "the_model.h"

#ifndef GLUT_ACTIVE_WIN
#define GLUT_ACTIVE_WIN 8
#endif

#define __THE_H__
#endif
