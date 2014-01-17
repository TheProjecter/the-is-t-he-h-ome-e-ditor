////////////////////////////////////////////////////////////////////////////////
///  the_types.h
///  the
///
///  Created by Atz on 17.11.10.
///  Copyright 2010 The(c)ompany. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __THE_TYPES_H__

// please include "the.h" instead of this file.

////////////////////////////////////////////////////////////////////////////////

typedef unsigned long ULONG;
typedef unsigned int UINT;

namespace The
{
	typedef enum
		{
			PROJECTION_PERSPECTIVE = 1,
			PROJECTION_ORPHO = 2
		}
	Projection_t;

	typedef enum
		{
			EVENT_NONE = 0,
			MOUSE_MOVE = 1,
			MOUSE_ENTER = 2,
			MOUSE_LEAVE = 3,
			MOUSE_PRESS = 4,
			MOUSE_RELEASE = 5,
			MOUSE_DRAG = 6,
			MOUSE_DROP = 7,
			MOUSE_ALT_MOVE = 8, // drag-move
			KEY_PRESS = 10,
			KEY_RELEASE = 11
		}
	Events_t;

	typedef enum
		{
			ID_MOUSE_MOVE_UNDEFINED,
			ID_MOUSE_MOVE_BEFORE_PRESS,
			ID_MOUSE_MOVE_AFTER_PRESS_BEFORE_DRAG,
			ID_MOUSE_MOVE_AFTER_DRAG
		}
	EID_Moving_t;

#ifndef GLUT_ACTIVE_WIN
#define GLUT_ACTIVE_WIN 8
#endif

	typedef enum
		{
			ACTIVE_WIN = GLUT_ACTIVE_WIN,

#	ifdef __APPLE__
			ACTIVE_IGNORE = -1,
			ACTIVE_NONE = 0,
			ACTIVE_SHIFT = 1,
			ACTIVE_CTRL = 2,
			ACTIVE_ALT = 4,
#	else
			ACTIVE_ALT = GLUT_ACTIVE_ALT,
			ACTIVE_CTRL = GLUT_ACTIVE_CTRL,
			ACTIVE_SHIFT = GLUT_ACTIVE_SHIFT,
#	endif
			ACTIVE_ANY = ACTIVE_SHIFT + ACTIVE_CTRL + ACTIVE_ALT + ACTIVE_WIN
		}
	Modifiers_t;
}

#define __THE_TYPES_H__
#endif
