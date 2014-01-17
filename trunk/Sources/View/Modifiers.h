////////////////////////////////////////////////////////////////////////////////
///  objects.h
///  the
///
///  Created by Atz on 17.11.10.
///  Copyright 2010 The(c)ompany. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////

// please include "the.h" instead of this file.

#ifndef __THE_MODIFIERS_H__

namespace The
	{
		class Modifiers
			{
				int m_flags; public: inline int modifiers() const { return m_flags; }
				inline bool operator ==( const Modifiers& rhs ) const { return m_flags == rhs.m_flags; }
				inline bool operator !=( const Modifiers& rhs ) const { return m_flags != rhs.m_flags; }
				inline Modifiers( int flags = 0 ): m_flags( flags ) {}
			};
	}// The::

#define __THE_MODIFIERS_H__
#endif
