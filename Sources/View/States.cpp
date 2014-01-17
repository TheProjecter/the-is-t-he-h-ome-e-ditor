////////////////////////////////////////////////////////////////////////////////
///  Finest C++ State THE_Object pattern - Transitions.
///
///  Author Atz(c)
///
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "the.h"

////////////////////////////////////////////////////////////////////////////////

#define DEF_TRANSITION_FROM( PARAM_STATE_NAME, EVENT )\
	template <> The::View* View_t< The::View::PARAM_STATE_NAME >::processMouseEvent( const Mouse& EVENT )

#define DEF_STATE_ACTION( PARAM_STATE_NAME, TYPE, MODIFIERS, VECTOR )\
	template <> void View_t< The::View::PARAM_STATE_NAME >::processMouseMove( EID_Moving_t TYPE, const unsigned MODIFIERS, const Pixel& VECTOR )

#define DEF_NEXT_STATE( PARAM_STATE_NAME )\
	goTo< The::View::PARAM_STATE_NAME >()

#ifdef DEF_LOG_EVENTS
#	define DEF_PRINT_EVENT( EVENT_NAME ) \
		cout << count++ << EVENT_NAME; \
		glo::print_event( event );
// Conditional macro #define for a function macro is not allowed.
#else
#	define DEF_PRINT_EVENT( EVENT_NAME )
#endif

////////////////////////////////////////////////////////////////////////////////

#pragma mark >>> states description <<<

//ID_STATE_DEFAULT
//ID_STATE_ROTATE_OBJECTS
//ID_STATE_SELECT_OBJECTS
//ID_STATE_SELECT_POINTS
//ID_STATE_MOVE_SELECTED
//ID_STATE_MOVE_VIEWPORT

namespace The
{
#	ifdef DEF_LOG_EVENTS
	static long count = 0;
#	endif
	using namespace std;
	// deselect if mouse simple point to object (not a rectangulare selection)
	static const bool deselecting = true;

	// doIt:
	DEF_TRANSITION_FROM( ID_STATE_DEFAULT, event )
		{
			DEF_PRINT_EVENT("ID_STATE_DEFAULT")

			if (Mouse( MOUSE_PRESS )== event)
				{
					if (/*ext_state == rot_prizm*/0)
						{
							return DEF_NEXT_STATE( ID_STATE_ROTATE_OBJECTS );
						}
					else if ( glo::is_click_to_selection() )
						{
							return DEF_NEXT_STATE( ID_STATE_MOVE_SELECTED );
						}
					else
						return DEF_NEXT_STATE( ID_STATE_MOVE_VIEWPORT );
				}
			else if (Mouse( MOUSE_PRESS, ACTIVE_CTRL )== event)
				{
					m_someObjectsWasSelectedAtMouseDown = glo::select_objects( deselecting );
					return DEF_NEXT_STATE( ID_STATE_SELECT_OBJECTS );
				}
			else if (Mouse( MOUSE_PRESS, ACTIVE_SHIFT )== event)
			{
					m_somePointsWasSelectedAtMouseDown = glo::select_points( deselecting );
					return DEF_NEXT_STATE( ID_STATE_SELECT_POINTS );
				}
			return this;
		}

	DEF_TRANSITION_FROM( ID_STATE_ROTATE_OBJECTS, event )
		{
			DEF_PRINT_EVENT("ID_STATE_ROTATE_OBJECTS")

			if (Mouse( MOUSE_RELEASE, ACTIVE_IGNORE )== event)
				{
					return DEF_NEXT_STATE( ID_STATE_DEFAULT );
				}
			return this;
		}

	DEF_TRANSITION_FROM( ID_STATE_SELECT_OBJECTS, event )
		{
			DEF_PRINT_EVENT("ID_STATE_SELECT_OBJECTS")

			bool drop = Mouse( MOUSE_DROP, ACTIVE_IGNORE )== event;
			bool release = Mouse( MOUSE_RELEASE, ACTIVE_IGNORE )== event;

			// the only one exiting from state is coming: from drop OR release events.
			if (drop || release)
				{
					if (! m_someObjectsWasSelectedAtMouseDown)
						{
							if (! glo::select_objects( release ) && !drop)
								{
									glo::unselect_objects();
								}
						}
					else
						m_someObjectsWasSelectedAtMouseDown = false;

					return DEF_NEXT_STATE( ID_STATE_DEFAULT );
				}
			return this;
		}

	DEF_TRANSITION_FROM( ID_STATE_SELECT_POINTS, event )
		{
			DEF_PRINT_EVENT("ID_STATE_SELECT_POINTS")

			bool drop = Mouse( MOUSE_DROP, ACTIVE_IGNORE )== event;
			bool release = Mouse( MOUSE_RELEASE, ACTIVE_IGNORE )== event;

			// the only one exiting from state is coming: from drop OR release events.
			if (release || drop)
				{
					if (! m_somePointsWasSelectedAtMouseDown)
						{
							if (! glo::select_points( release ) && !drop)
								{
									glo::unselect_points();
								}
						}
					else
						m_somePointsWasSelectedAtMouseDown = false;

					return DEF_NEXT_STATE( ID_STATE_DEFAULT );
				}
			return this;
		}

	DEF_TRANSITION_FROM( ID_STATE_MOVE_SELECTED, event )
		{
			DEF_PRINT_EVENT("ID_STATE_MOVE_SELECTED")

			if (Mouse( MOUSE_RELEASE, ACTIVE_IGNORE )== event)
				{
					return DEF_NEXT_STATE( ID_STATE_DEFAULT );
				}
			return this;
		}

	DEF_TRANSITION_FROM( ID_STATE_MOVE_VIEWPORT, event )
		{
			DEF_PRINT_EVENT("ID_STATE_MOVE_VIEWPORT")

			if (Mouse( MOUSE_RELEASE, ACTIVE_IGNORE )== event)
				{
					return DEF_NEXT_STATE( ID_STATE_DEFAULT );
				}
			return this;
		}
	
	// state action is function may be called fron inside the current machine state.

	DEF_STATE_ACTION( ID_STATE_DEFAULT, T,M,V )
		{
		//	cout << state << endl;
		}
	DEF_STATE_ACTION( ID_STATE_ROTATE_OBJECTS, T,M,V )
		{
			cout << state << endl;
		}
	DEF_STATE_ACTION( ID_STATE_MOVE_SELECTED, T,M,V )
		{
			glo::move_selected_points_by( V );
		}
	DEF_STATE_ACTION( ID_STATE_SELECT_OBJECTS, T,M,V )
		{
		//	cout << state << endl;
		}
	DEF_STATE_ACTION( ID_STATE_SELECT_POINTS, T,M,V )
		{
			// By default in OSX, do not interrupt selection if modifier key was releazed while mouse is moved:
			// interrupt: /*if (ACTIVE_SHIFT != M) DEF_NEXT_STATE( ID_STATE_DEFAULT );*/
		}
	DEF_STATE_ACTION( ID_STATE_MOVE_VIEWPORT, T,M,V )
		{
			const float sensitivity = (PROJECTION_ORPHO == m_currentProjection) ? 1.0 : 10.0;
			float scale = (PROJECTION_ORPHO == m_currentProjection) ? m_currentOrthoScale : 1.0;
//			m_currentCamera.translate( ::lround(V.X/sensitivity/scale), ::lround(V.Y/sensitivity/scale), 0 );
			m_currentCamera.translate( V.X/sensitivity/scale, V.Y/sensitivity/scale, 0 );
		}
}
// The::
