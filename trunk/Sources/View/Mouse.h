////////////////////////////////////////////////////////////////////////////////
///  objects.h
///  the
///
///  Created by Atz on 17.11.10.
///  Copyright 2010 The(c)ompany. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __THE_MOUSE_H__

// please include "the.h" instead of this file.

namespace The
	{
		class Mouse: public Keys // inherite m_code for mouse buttons from class Keys
			{
				static const int manhattan_distance_pixels;
				static Pixel m_posCurrent;
				static Pixel m_posPressed;
				static Pixel m_movedBy;
				static bool m_isDragged;
			public:
				inline Mouse( const Events_t e, int m = ACTIVE_NONE ): Keys( m, e ) {}
				inline static Rect selectionRect() { return Rect( m_posPressed, m_posCurrent ); }
				inline static Rect pressedRect();
				inline static Pixel currentPos() { return m_posCurrent; }
				inline static Pixel pressedPos() { return m_posPressed; }
				inline static Pixel movedBy() { return m_movedBy; }
				inline static bool dragged() { return m_isDragged; }
				inline static void updatePos( const Pixel& coordinate ) { m_posCurrent = coordinate; }
				inline static Pixel dragDistance() { return Pixel( m_posCurrent.X - m_posPressed.X, m_posCurrent.Y - m_posPressed.Y ); }
				inline static bool isManhattanDistance() { Pixel distance = dragDistance();
					return (::abs(::round( distance.X )) >= manhattan_distance_pixels)
						|| (::abs(::round( distance.Y )) >= manhattan_distance_pixels);
				}
				inline static void setDragged( bool flag ) { m_isDragged = flag; }
				inline static void setPressedPos( const Pixel& rhs ) { m_posPressed = rhs; }
				inline static void setCurrentPos( const Pixel& rhs ) {
					m_movedBy = Pixel( rhs.X - m_posCurrent.X, rhs.Y - m_posCurrent.Y );
					m_posCurrent = rhs;
				}
				inline bool operator ==( const Mouse& rhs ) const { return Keys::operator ==( rhs ); }
				inline bool operator !=( const Mouse& rhs ) const { return Keys::operator !=( rhs ); }
			//	bool operator ==( const Mouse& rhs ) const;
			//	bool operator !=( const Mouse& rhs ) const;
			};

		inline Rect Mouse::pressedRect()
			{
				const Floating_t& x1 = m_posPressed.X - manhattan_distance_pixels;
				const Floating_t& x2 = m_posPressed.X + manhattan_distance_pixels;
				const Floating_t& y1 = m_posPressed.Y - manhattan_distance_pixels;
				const Floating_t& y2 = m_posPressed.Y + manhattan_distance_pixels;
				return Rect( Pixel( x1,y1 ), Pixel( x2,y2 ));
			}
}// The::

////////////////////////////////////////////////////////////////////////////////

#define __THE_MOUSE_H__
#endif
