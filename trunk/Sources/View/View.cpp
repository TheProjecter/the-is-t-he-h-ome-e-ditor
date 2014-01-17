////////////////////////////////////////////////////////////////////////////////
///  the.cpp
///  the
///
///  Created by Atz on 17.11.10.
///  Copyright 2010 The(c)ompany. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////

#	include <iostream>
#	include "the.h"

////////////////////////////////////////////////////////////////////////////////

namespace The
{
	using namespace std; 

	#	define DEF_ROTATE_CAMERA_X
	//#	define DEF_ROTATE_CAMERA_Y
	//#	define DEF_ROTATE_CAMERA_Z
	//#	define DEF_ROTATE_OBJECT_X
	#	define DEF_ROTATE_OBJECT_Y
	//#	define DEF_ROTATE_OBJECT_Z

	/**
	 * @brief At some of edotor begining time this test was
	 * cyclic modify the camera and axis coordinates angles...
	 */
	void View::onTimerTest()
		{
			static long count = 0;

			GLfloat obj_angle = ::sin( PI*count++/180.0 );
			GLfloat cam_angle = ::sin( PI*count++/180.0 )/3.0;

			m_currentCamera = Matrix();
			m_currentCamera.translate( 0, 0, 102 );
			m_currentCamera.scale( 1, 1,-1 ); // invert Z-axis
			m_currentCamera.x_rotate( -PI/36.0 ); // some lower...

			m_currentObject = Matrix();
			m_currentObject.y_rotate( -PI/2.0 );

	#	ifdef DEF_ROTATE_CAMERA_X
			m_currentCamera.translate( 0, 0.5, 0 );
			m_currentCamera.x_rotate( cam_angle );
	#	endif

	#	ifdef DEF_ROTATE_CAMERA_Y
			m_currentCamera.translate( -0.4, 0, 0 );
			m_currentCamera.y_rotate( cam_angle );
	#	endif

	#	ifdef DEF_ROTATE_CAMERA_Z
			m_currentCamera.z_rotate( cam_angle );
	#	endif

	#	ifdef DEF_ROTATE_OBJECT_X
			m_currentObject.x_rotate( obj_angle );
	#	endif

	#	ifdef DEF_ROTATE_OBJECT_Y
			m_currentObject.y_rotate( obj_angle );
	#	endif

	#	ifdef DEF_ROTATE_OBJECT_Z
			m_currentObject.z_rotate( obj_angle );
	#	endif
		}

	View::View( void* ) // workaround:
		:	m_currentPerspectiveScale( m_currentPerspectiveScale )
		,	m_currentOrthoScale( m_currentOrthoScale )
		,	m_currentNearDistance( m_currentNearDistance )
		,	m_currentFarDistance( m_currentFarDistance )
		,	m_currentFov( m_currentFov )
		,	m_currentRulerStep( m_currentRulerStep )
		,	m_currentCamera( m_currentCamera )
		,	m_currentObject( m_currentObject )
		,	m_currentViewportRect( m_currentViewportRect )
		,	m_currentProjection( m_currentProjection )
		,	m_previousProjection( m_previousProjection )
		,	m_somePointsWasSelectedAtMouseDown( m_somePointsWasSelectedAtMouseDown )
		,	m_someObjectsWasSelectedAtMouseDown( m_someObjectsWasSelectedAtMouseDown )
		{// init all variables by self to use copy constructor form for it instead of default form,
		 //	cout << 2010 << '.'<< 1227 << '.' << 77+160+19+23+108+71+359+42+73+49+272+141+437+87+135+127+254 << endl;
		}// breaking the values!. Or redefine placement new for each of member in initialization list.

	View::View()
		:	m_currentPerspectiveScale( 1024 )
		,	m_currentOrthoScale( 1 )
		,	m_currentRulerStep( 64 )
		,	m_currentFov( 90 )
		,	m_currentNearDistance( 1 ) // zNear and zFar must be a positive values
		,	m_currentFarDistance( 999 )
		,	m_currentViewportRect( -1,-1,-1,-1 )
		,	m_currentProjection( PROJECTION_PERSPECTIVE )
		,	m_previousProjection( m_currentProjection )
		{
			::srand( time( NULL ));

	#	ifdef DEF_CLEAR_COLOR_WHITE
			::glClearColor( 0.5,0.5,0.5,1 );
	#	else
			::glClearColor( 0,0,0,1 );
	#	endif
			::glClearDepth( 1.0 );
			::glEnable( GL_DEPTH_TEST );
			::glDepthRange( 0,100 );
			::glEnable( GL_BLEND );
			::glEnable( GL_ALPHA_TEST );
			::glEnable( GL_POINT_SMOOTH );
			::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

			m_currentCamera.translate( 0, 0, 50 );
			m_currentCamera.scale( 1, 1,-1 ); // invert Z-axis
			m_currentObject = Matrix();
		//	m_currentObject.x_rotate(-PI/2.0 );
		}

	////////////////////////////////////////////////////////////////////////////////


	void View::setPerspective( bool permanent )
		{
			glo::isometric( m_currentViewportRect, m_currentFov, m_currentNearDistance, m_currentFarDistance, m_currentPerspectiveScale );
			m_currentProjection = PROJECTION_PERSPECTIVE;
			if (permanent) // made do not return to over
				m_previousProjection = PROJECTION_PERSPECTIVE;
		}

	void View::setOrpho( bool permanent )
		{
			glo::linear( m_currentViewportRect, m_currentFov, m_currentNearDistance, m_currentFarDistance, m_currentOrthoScale );
			m_currentProjection = PROJECTION_ORPHO;
			if (permanent) // made do not return to over
				m_previousProjection = PROJECTION_ORPHO;
		}

	void View::useCurrentModelview()
		{
			using namespace glo;

			::glMatrixMode( GL_MODELVIEW );

			Matrix camera = m_currentCamera;

			if (PROJECTION_ORPHO == m_currentProjection)
				{
					// render the scene in ortho projection at Z==0.
					static const int translation_vector = 3;
					camera[ translation_vector ].Z = 0;
				}

			use_matrix_for_mode( matrix_between( m_currentObject, camera ), GL_MODELVIEW );

	#	ifdef DEF_LOG_MODELVIEW

			static int count = 0;
		
			if (100 == count++)
				{
					::glMatrixMode( GL_MODELVIEW );		print_current_matrix( GL_MODELVIEW_MATRIX );
				//	::glMatrixMode( GL_PROJECTION );	print_current_matrix( GL_PROJECTION_MATRIX );
					::glMatrixMode( GL_MODELVIEW );		count = 0;
				}
	#	endif
		}
	
	////////////////////////////////////////////////////////////////////////////////

#	pragma mark -
#	pragma mark render

#	define DEF_RENDER_SELECTION( STATE )\
		ID_STATE_SELECT_OBJECTS == STATE || ID_STATE_SELECT_POINTS == STATE

//#	define DEF_RENDER_RULER_IN_OBJECT_SPACE // broken.

	void View::renderScene( int state )
		{
	#	ifdef DEF_CLEAR_COLOR_WHITE
			::glClearColor( 0.5,0.5,0.5,1 );
	#	else
			::glClearColor( 0,0,0,1 );
	#	endif
			::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			useCurrentProjection(); // will temporary setOrpho(false)/setPerspective(false) [and see for the View::revertProjection pair below]
			useCurrentModelview(); // current matrix is to be MODELVIEW.

			if (PROJECTION_ORPHO==m_currentProjection)
				{
					::glMatrixMode( GL_MODELVIEW );
					::glPushMatrix();
						{
							// ruler
							const Floating_t scale = m_currentOrthoScale;
							const Integer_t step = m_currentRulerStep;
							const Rect& V = m_currentViewportRect;
							const Rect& actualViewport = Rect( V.pos().X, V.pos().Y, V.width()/scale, V.height()/scale );
							const Pixel& center = glo::pixel_from_point( glo::object_to_window())/scale;
							const Rect rect = glo::step_rounded_rect( actualViewport, step, scale, center );
							::glLoadIdentity(); glo::render_ruler( rect, step ); // 2D background
						}
					::glPopMatrix(); // modelview matrix will also be restored later by View::useCurrentModelview().
				}

#	pragma mark >>> RENDER <<<

			::glEnable( GL_POINT_SMOOTH );
			::glEnable( GL_LINE_SMOOTH );

			glo::render_normales( 32 );
			glo::render_test_rect( 16 );
			glo::render_space(); // <- все объекты

			if (Mouse::isManhattanDistance() && DEF_RENDER_SELECTION( state ))
				{
					::glMatrixMode( GL_PROJECTION );
					::glLoadIdentity(); ::gluOrtho2D( DEF_VIEWPORT_RECT( 0.5, m_currentViewportRect.size() ));
						{
							::glMatrixMode( GL_MODELVIEW );
							::glPushMatrix();
								{
									::glLoadIdentity(); renderSelectionRect(); // 2D foreground (QC Passed)
								}
							::glPopMatrix();
						}
					revertProjection();
				}

		::glFinish();
		::glutSwapBuffers();
	}

	void View::renderSelectionRect()
		{
			// get selection in screen space
			const Rect& windowRect = Mouse::selectionRect();

			// translate to object coordinate space
			const Point& point1 = glo::window_to_object( windowRect.topLeft() );
			const Point& point2 = glo::window_to_object( windowRect.bottomRight() );

			// convert points to pixels to make integer rect
			const Pixel& pixel1 = glo::pixel_from_point( point1 );
			const Pixel& pixel2 = glo::pixel_from_point( point2 );

			// render using global routine
			glo::render_selection( Rect( pixel1, pixel2 ));
		}
	
	////////////////////////////////////////////////////////////////////////////////

#	pragma mark -
#	pragma mark events


	void View::handle_mouse_enter( const unsigned modifiers, const Pixel& coordinate )
		{
			Mouse::setCurrentPos( coordinate );
			processMouseEvent( Mouse( MOUSE_ENTER, modifiers ));
		}

	void View::handle_mouse_leave( const unsigned modifiers, const Pixel& coordinate )
		{
			Mouse::setCurrentPos( coordinate );
			processMouseEvent( Mouse( MOUSE_LEAVE, modifiers ));
		}

	void View::handle_mouse_press( const unsigned modifiers, const Pixel& coordinate )
		{
			Mouse::setCurrentPos( coordinate );
			Mouse::setPressedPos( coordinate );
			processMouseEvent( Mouse( MOUSE_PRESS, modifiers ));
		}

	void View::handle_mouse_release( const unsigned modifiers, const Pixel& coordinate )
		{
			Mouse::setCurrentPos( coordinate );
			if ( Mouse::dragged() )
				{
					handle_mouse_drop( modifiers, coordinate );
					Mouse::setDragged( false ); // drag operation completed.
				}
			processMouseEvent( Mouse( MOUSE_RELEASE, modifiers ));
		}

	void View::handle_mouse_drag( const unsigned modifiers, const Pixel& coordinate )
		{
			Mouse::setCurrentPos( coordinate );
			processMouseEvent( Mouse( MOUSE_DRAG, modifiers ));
		}

	void View::handle_mouse_drop( const unsigned modifiers, const Pixel& coordinate )
		{
			Mouse::setCurrentPos( coordinate );
			processMouseEvent( Mouse( MOUSE_DROP, modifiers ));
		}

	void View::handle_mouse_move( const unsigned modifiers, const Pixel& coordinate, bool flag )
		{
			EID_Moving_t type = ID_MOUSE_MOVE_UNDEFINED;

			Mouse::setCurrentPos( coordinate );

			if (flag)
			{
				// start drag operation (and make custom drag flag)
				if (! Mouse::dragged() )
				{
					if (Mouse::isManhattanDistance())
					{
						Mouse::setDragged( true );
						handle_mouse_drag( modifiers, coordinate );
					}
					type = ID_MOUSE_MOVE_AFTER_PRESS_BEFORE_DRAG;
				}
				else
					type = ID_MOUSE_MOVE_AFTER_DRAG;
			}
			else
				type = ID_MOUSE_MOVE_BEFORE_PRESS;


			processMouseMove( type, modifiers, Mouse::movedBy() );
		}

#pragma mark -

	/////////////////////////////////////////////////////////////////
	// TODO: animated rotation around needed axis with pointed center.
	/////////////////////////////////////////////////////////////////

	void View::processKeyEvent( const Keys& key )
		{
			static Keys esc( KEY_PRESS, 53 );

			// camera near-far
			static Keys a( KEY_PRESS, 0, ACTIVE_NONE );
			static Keys z( KEY_PRESS, 6, ACTIVE_NONE );

			// camera move
			static Keys left( KEY_PRESS, 123 );
			static Keys right( KEY_PRESS, 124 );
			static Keys down( KEY_PRESS, 125 );
			static Keys up( KEY_PRESS, 126 );

			// camera rotate around objects
			static Keys alt_left( KEY_PRESS, 123, ACTIVE_ALT );
			static Keys alt_right( KEY_PRESS, 124, ACTIVE_ALT );
			static Keys alt_down( KEY_PRESS, 125, ACTIVE_ALT );
			static Keys alt_up( KEY_PRESS, 126, ACTIVE_ALT );

			// camera rotate around self
			static Keys shift_left( KEY_PRESS, 123, ACTIVE_SHIFT );
			static Keys shift_right( KEY_PRESS, 124, ACTIVE_SHIFT );
			static Keys shift_down( KEY_PRESS, 125, ACTIVE_SHIFT );
			static Keys shift_up( KEY_PRESS, 126, ACTIVE_SHIFT );

			// camera scale
			static Keys upscale( KEY_PRESS, 30 ); // ']'
			static Keys downscale( KEY_PRESS, 33 ); // '['

			// ruler step
			static Keys upruler( KEY_PRESS, 42 ); // '\'
			static Keys downruler( KEY_PRESS, 39 ); // '

			// objects create
			static Keys num3( KEY_PRESS, 20 ); // 3-sides
			static Keys num4( KEY_PRESS, 21 ); // 4-sides
			static Keys num6( KEY_PRESS, 22 ); // 6-sides
			static Keys num8( KEY_PRESS, 28 ); // 8-sides

			// objects copy-paste
			static Keys ctrl_c( KEY_PRESS, 8, ACTIVE_CTRL ); // copy
			static Keys ctrl_v( KEY_PRESS, 9, ACTIVE_CTRL ); // paste

			static Keys d( KEY_PRESS, 2 ); // delete
			static Keys l( KEY_PRESS, 37 ); // load
			static Keys s( KEY_PRESS, 1 ); // save

			static Keys o( KEY_PRESS, 31 ); // orpho
			static Keys p( KEY_PRESS, 35 ); // perspective
			static Keys w( KEY_PRESS, 13 ); // switch between orpho-perspective

			static Keys q( KEY_PRESS, 12 ); // quit
			static Keys i( KEY_PRESS, 34 ); // some statistics
			// static Keys c( KEY_PRESS, 8 ); // ?

			if (key == esc)
				{
					cout << "escape" << endl; // test key
				}
			// camera near-far
			else if (key == a)
				{
					m_currentCamera.translate( 0, 0, 1 );
				}
			else if (key == z)
				{
					m_currentCamera.translate( 0, 0,-1 );
				}
			// camera move
			else if (key == left)
				{
					m_currentCamera.translate( 1, 0, 0 );
				}
			else if (key == right)
				{
					m_currentCamera.translate(-1, 0, 0 );
				}
			else if (key == down)
				{
					m_currentCamera.translate( 0, 1, 0 );
				}
			else if (key == up)
				{
					m_currentCamera.translate( 0,-1, 0 );
				}
			// camera rotate around objects
			else if (key == alt_left)
				{
					m_currentCamera.translate( 0,0,DEF_CAMERA_TRANSLATION_DISTANCE );
					m_currentCamera.y_rotate(-PI/2.0 );
					m_currentCamera.translate( 0,0,-DEF_CAMERA_TRANSLATION_DISTANCE );
				}
			else if (key == alt_right)
				{
					m_currentCamera.translate( 0,0,DEF_CAMERA_TRANSLATION_DISTANCE );
					m_currentCamera.y_rotate( PI/2.0 );
					m_currentCamera.translate( 0,0,-DEF_CAMERA_TRANSLATION_DISTANCE );
				}
			else if (key == alt_down)
				{
					m_currentCamera.translate( 0,0,DEF_CAMERA_TRANSLATION_DISTANCE );
					m_currentCamera.x_rotate(-PI/2.0 );
					m_currentCamera.translate( 0,0,-DEF_CAMERA_TRANSLATION_DISTANCE );
				}
			else if (key == alt_up)
				{
					m_currentCamera.translate( 0,0,DEF_CAMERA_TRANSLATION_DISTANCE );
					m_currentCamera.x_rotate( PI/2.0 );
					m_currentCamera.translate( 0,0,-DEF_CAMERA_TRANSLATION_DISTANCE );
				}
			// camera rotate around self
			else if (key == shift_left)
				{
					m_currentCamera.x_rotate( PI/2.0 ); // QC Passed.
				}
			else if (key == shift_right)
				{
					m_currentCamera.x_rotate(-PI/2.0 ); // QC Passed.
				}
			else if (key == shift_down)
				{
					m_currentCamera.y_rotate( PI/2.0 ); // QC Passed.
				}
			else if (key == shift_up)
				{
					m_currentCamera.y_rotate(-PI/2.0 ); // QC Passed.
				}
			// camera scale
			else if (key == upscale)
				{
					if (PROJECTION_PERSPECTIVE == m_currentProjection) m_currentPerspectiveScale *= 2;
					if (PROJECTION_ORPHO == m_currentProjection) m_currentOrthoScale *= 2;
				}
			else if (key == downscale)
				{
					if (PROJECTION_PERSPECTIVE == m_currentProjection)
						{
							m_currentPerspectiveScale /= 2;
							if (m_currentPerspectiveScale < 1)
								m_currentPerspectiveScale = 1;
						}
					if (PROJECTION_ORPHO == m_currentProjection)
						{
							m_currentOrthoScale /= 2;
							if (m_currentOrthoScale < 1)
								m_currentOrthoScale = 1;
						}
				}
			// ruler step
			else if (key == upruler)
				{
					if (PROJECTION_ORPHO == m_currentProjection)
					{
						m_currentRulerStep *= 2;
						cout << m_currentRulerStep << endl;
					}
				}
			else if (key == downruler)
				{
					if (PROJECTION_ORPHO == m_currentProjection)
						{
							m_currentRulerStep /= 2;
							if (m_currentRulerStep < 1)
								{
									m_currentRulerStep = 1;
								}
							cout << m_currentRulerStep << endl;
						}
				}
			// objects create
			else if (key == num3)
				{
					glo::create_brush( 3 );
				}
			else if (key == num4)
				{
					glo::create_brush( 4 );
				}
			else if (key == num6)
				{
					glo::create_brush( 6 );
				}
			else if (key == num8)
				{
					glo::create_brush( 8 );
				}
			// objects copy-paste
			else if (key == ctrl_c)
				{
					glo::copy_to_buffer();
				}
			else if (key == ctrl_v)
				{
					glo::paste_from_buffer();
				}
			else if (key == d)
				{
					glo::delete_selected_brushes();
				}
			// save-load
			else if (key == s)
				{
					glo::save_map();
				}
			else if (key == l)
				{
					glo::load_map();
				}
			// orpho-perspective-cycle
			else if (key == o)
				{
					setOrpho();
				}
			else if (key == p)
				{
					setPerspective();
				}
			else if (key == w)
				{
					if (PROJECTION_PERSPECTIVE == m_currentProjection) setOrpho(); else // important
					if (PROJECTION_ORPHO == m_currentProjection) setPerspective();
				}
			// debug
			else if (key == i)
				{
					glo::print();
				}
			// close window
			else if (key == q)
				{
					::exit( 1 );
				}
		//	else
		//		std::cout << "key: " << key.key() << " mod: " << key.modifiers() << std::endl;
		}
}
