////////////////////////////////////////////////////////////////////////////////
///  objects.h
///  the
///
///  Created by Atz on 17.11.10.
///  Copyright 2010 The(c)ompany. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __THE_VIEW_H__

#include <new>


// please include "the.h" instead of this file.


namespace The
{
	struct View // The really is a class "View"; in Obc-C this may have an outlet variables below:
		{
			View();

#pragma mark >>> condition states the class is may be <<<

			typedef enum // View is parametrized by states:
				{
					ID_STATE_DEFAULT = 0,
					ID_STATE_ROTATE_OBJECTS,
					ID_STATE_SELECT_OBJECTS,
					ID_STATE_SELECT_POINTS,
					ID_STATE_MOVE_SELECTED,
					ID_STATE_MOVE_VIEWPORT
				};

			// handle events from window
			void handle_mouse_enter(	const unsigned modifiers, const Pixel& coordinate );
			void handle_mouse_leave(	const unsigned modifiers, const Pixel& coordinate );
			void handle_mouse_press(	const unsigned modifiers, const Pixel& coordinate );
			void handle_mouse_release(	const unsigned modifiers, const Pixel& coordinate );
			void handle_mouse_drag(		const unsigned modifiers, const Pixel& coordinate );
			void handle_mouse_drop(		const unsigned modifiers, const Pixel& coordinate );
			void handle_mouse_move(		const unsigned modifiers, const Pixel& coordinate, bool flag );
			void handle_key_press(		const unsigned key, const unsigned modifiers );
			void handle_key_release(	const unsigned key, const unsigned modifiers );
			void handle_viewport_rect(	const Rect& rect );

			// projection types
			void setOrpho(	bool permanent = true );
			void setPerspective( bool permanent = true );

			// drawing the something
			virtual void renderScene( int );
			void onTimerTest();

		protected:
			void revertProjection();
			void useCurrentProjection();
			void useCurrentModelview();

		private: // utility calls

			void setProjection( Projection_t, bool permanent = true );
			void setCurrentCamera( Matrix& );
			void setCurrentObject( Matrix& );

			void renderSelectedObjects(); // private
			void renderUnselectedObjects(); // private
			void renderSelectionRect();

			void processKeyEvent( const Keys& event ); // goto next state by internal selectors.

			// redefined in states
			virtual View* processMouseEvent( const Mouse& event ) = 0; // goto next state by internal selectors.
			virtual void processMouseMove( EID_Moving_t move_t, const unsigned modifiers, const Pixel& movedBy ) = 0;
			
		protected: // data accessed by technical derived state-classes

			unsigned		m_currentPerspectiveScale; // syntetic
			unsigned		m_currentOrthoScale; // syntetic
			unsigned		m_currentNearDistance; // IBOutlets?
			unsigned		m_currentFarDistance;
			unsigned		m_currentRulerStep;
			GLdouble		m_currentFov;
			Matrix			m_currentObject;
			Matrix			m_currentCamera;
			Rect			m_currentViewportRect;
			Projection_t	m_currentProjection;
			Projection_t	m_previousProjection;
			bool			m_somePointsWasSelectedAtMouseDown;
			bool			m_someObjectsWasSelectedAtMouseDown;

			// make memory placement of state is to permanent
			inline void* operator new( size_t, void* ptr ) { return ptr; }
			View( void* ); // prevent variables reinit during states.
		};

	////////////////////////////////////////////////////////////////////////////////

	// States-definer.
	template< int S = View::ID_STATE_DEFAULT >struct View_t: public View
		{
			inline View_t(): View() {}
			inline View_t( void* ): View( NULL ) {}
			protected: typedef enum { state = S }; // need to select current state in render_Scene( int )
			virtual void renderScene( int ) { View::renderScene( state ); }
			virtual View* processMouseEvent( const Mouse& event );
			virtual void processMouseMove( EID_Moving_t move_t, const unsigned modifiers, const Pixel& movedBy );
			template< int T >inline View* goTo() { return/* overloaded_*/new( this )View_t< T >( NULL ); }
		};

	////////////////////////////////////////////////////////////////////////////////

	inline void View::handle_key_press( const unsigned key, const unsigned modifiers )
		{
			processKeyEvent( Keys( KEY_PRESS, key, modifiers ));
		}
	inline void View::handle_key_release( const unsigned key, const unsigned modifiers )
		{
			processKeyEvent( Keys( KEY_RELEASE, key, modifiers ));
		}
	inline void View::handle_viewport_rect( const Rect& rect )
		{
			m_currentViewportRect = rect; ::glViewport( 0, 0, rect.size().W, rect.size().H );
		}
	inline void View::setCurrentCamera( Matrix& rhs )
		{
			m_currentCamera = rhs;
		}
	inline void View::setCurrentObject( Matrix& rhs )
		{
			m_currentObject = rhs;
		}
	inline void View::revertProjection()
		{
			setProjection( m_previousProjection );
		}
	inline void View::useCurrentProjection()
		{
			setProjection( m_currentProjection, false );
		}
	inline void View::setProjection( Projection_t type, bool flag )
		{
			switch( type )
				{
					case PROJECTION_PERSPECTIVE:setPerspective( flag ); return;
					case PROJECTION_ORPHO:		setOrpho( flag ); return;
				}
		}
}//The::

////////////////////////////////////////////////////////////////////////////////

#define __THE_VIEW_H__
#endif
