////////////////////////////////////////////////////////////////////////////////
///  the.cpp
///  the
///
///  Created by Atz on 17.11.10.
///  Copyright 2010 The(c)ompany. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __THE_C_FUNCTIONS_H__

#include <iostream>
#include "the.h"

////////////////////////////////////////////////////////////////////////////////

namespace The
{
	The::Pixel Mouse::m_posCurrent;
	The::Pixel Mouse::m_posPressed;
	The::Pixel Mouse::m_movedBy;
	bool Mouse::m_isDragged = false;
	const int Mouse::manhattan_distance_pixels = DEF_MANHATTAN_DISTANCE_PX;
}

////////////////////////////////////////////////////////////////////////////////

namespace glo
{
	typedef Space_t< The::Floating_t,4 >Space;
	Space temp("temp"),space("main");

	using namespace std;
	using namespace The;

	void isometric( const Rect& viewport, GLdouble fov, GLdouble near, GLdouble far, GLdouble scale )
		{
			::glMatrixMode( GL_PROJECTION );
			::glLoadIdentity();
			const GLdouble aspect = (GLdouble) viewport.size().Width / (GLdouble) viewport.size().Height;
			const GLdouble h = atan(fov/2.0) * viewport.size().Height / scale;
			const GLdouble w = h * aspect;
			::glFrustum( -w, w, -h, h, near, far ); // QC Passed.
		}

	void linear( const Rect& viewport, GLdouble fov, GLdouble near, GLdouble far, GLdouble scale )
		{
			::glMatrixMode( GL_PROJECTION );
			::glLoadIdentity();
			const GLdouble h = viewport.size().Height / scale;
			const GLdouble w = viewport.size().Width / scale;
			::glOrtho( -w/2.0, w/2.0, -h/2.0, h/2.0, -near, far); // QC Passed.

			// FIXME: don't render centers in orhographic projection:
		//	::gluOrtho2D( -w/2.0, w/2.0, -h/2.0, h/2.0);
		//	::gluOrtho2D( DEF_VIEWPORT_RECT( 0.5, viewport.size() ));
	}
	// globals

	const char* current_mode_string();

	Rect step_rounded_rect( const Rect& R, float, unsigned S );
	Rect step_rounded_rect( const Rect& R, float, unsigned S, const Pixel& O );

	int random_between( int from, int to )
		{
			return rand() % (from - to) + from;
		}

////////////////////////////////////////////////////////////////////////////////

#	pragma mark print
#	define DEF_COUT_MSG( MSG ) cout << MSG << endl;

	void print_opengl_version( bool flag )
	{
		GLint maxRectTextureSize;
		GLint myMaxTextureUnits;
		GLint myMaxTextureSize;
		const GLubyte* strVersion;
		const GLubyte* strExt;
		float myGLVersion;
		GLboolean isVAO, isTexLOD, isColorTable, isFence, isShade, isTextureRectangle;

		strVersion = ::glGetString( GL_VERSION );
		sscanf((char* )strVersion, "%f", &myGLVersion);
		strExt = ::glGetString( GL_EXTENSIONS );
		::glGetIntegerv( GL_MAX_TEXTURE_UNITS, &myMaxTextureUnits );
		::glGetIntegerv( GL_MAX_TEXTURE_SIZE, &myMaxTextureSize );

		isVAO =
			::gluCheckExtension( (const GLubyte*)"GL_APPLE_vertex_array_object", strExt );
		isFence =
			::gluCheckExtension((const GLubyte*)"GL_APPLE_fence", strExt );
		isShade =
			::gluCheckExtension( (const GLubyte*)"GL_ARB_shading_language_100", strExt );
		isColorTable =
			::gluCheckExtension( (const GLubyte*)"GL_SGI_color_table", strExt )
			|| ::gluCheckExtension( (const GLubyte*)"GL_ARB_imaging", strExt );
		isTexLOD =
			::gluCheckExtension ((const GLubyte*)"GL_SGIS_texture_lod", strExt)
			|| (myGLVersion >= 1.2);
		isTextureRectangle =
			::gluCheckExtension( (const GLubyte*)"GL_EXT_texture_rectangle", strExt );

		if (isTextureRectangle)
			::glGetIntegerv( GL_MAX_RECTANGLE_TEXTURE_SIZE_EXT, &maxRectTextureSize );
		else
			maxRectTextureSize = 0;

		std::cout << "OpenGL version: " << myGLVersion << " (need: 3.3+)" << std::endl;
		if (flag) std::cout << strVersion << std::endl << strExt << std::endl;
	}

	void print_event( const Keys& rhs )
	{
		using namespace std;
		using namespace The;
	
		switch( rhs.event() )
		{
			case MOUSE_MOVE:	DEF_COUT_MSG( "MOUSE_MOVE" );		break;
			case MOUSE_ENTER:	DEF_COUT_MSG( "MOUSE_ENTER" );		break;
			case MOUSE_LEAVE:	DEF_COUT_MSG( "MOUSE_LEAVE" );		break;
			case MOUSE_PRESS:	DEF_COUT_MSG( "MOUSE_PRESS" );		break;
			case MOUSE_RELEASE:	DEF_COUT_MSG( "MOUSE_RELEASE" );	break;
			case MOUSE_DRAG:	DEF_COUT_MSG( "MOUSE_DRAG" );		break;
			case MOUSE_DROP:	DEF_COUT_MSG( "MOUSE_DROP" );		break;
			case MOUSE_ALT_MOVE:DEF_COUT_MSG( "MOUSE_ALT_MOVE" );	break;
			case KEY_PRESS:		DEF_COUT_MSG( "KEY_PRESS" );		break;
			case KEY_RELEASE:	DEF_COUT_MSG( "KEY_RELEASE" );		break;
			default: DEF_COUT_MSG( "TODO: unknown event enum entry." );
		}
#	ifdef DEF_LOG_COORDINATES
		cout << event << ":{" << coordinate.X << ',' << coordinate.Y << "};" << endl;
#	endif
	}

	void print_current_matrix( GLenum rhs )
	{
		print_current_mode();
		const bool round_the_values = false;
		cout << Matrix_t< float, 4 >::currentMatrix( rhs, round_the_values );
	}

	const char* current_mode_string()
	{
		GLint mode = 0; ::glGetIntegerv( GL_MATRIX_MODE, &mode );
		switch( mode )
		{
			case GL_MODELVIEW:	return "\tGL_MODELVIEW";
			case GL_PROJECTION:	return "\tGL_PROJECTION";
			default:			return "\tGL_TEXTURE?";
		}
	}

////////////////////////////////////////////////////////////////////////////////

#	pragma mark -
#	pragma mark convert

	Point object_to_window( const Point& vertex )
		{
			GLdouble* pM = (GLdouble*)&(Matrix().getCurrentMatrix( GL_MODELVIEW_MATRIX ));
			GLdouble* pP = (GLdouble*)&(Matrix().getCurrentMatrix( GL_PROJECTION_MATRIX ));
			Vector_t< GLint,4 >V; GLint* pV = (GLint*)&V; ::glGetIntegerv( GL_VIEWPORT, pV );
			GLdouble x,y,depth; ::gluProject( vertex.X, vertex.Y, vertex.Z, pM,pP,pV, &x, &y, &depth );
			return Point( x,y,depth );
		}

	Floating_t vertex_depth( const Point& vertex )
		{
			return object_to_window( vertex ).Z;
		}

	Point window_to_object( const Pixel& coordinate )
		{
			GLdouble winX,winY,winZ, objX,objY,objZ;
			GLdouble pM [16]; ::glGetDoublev(  GL_MODELVIEW_MATRIX, pM );
			GLdouble pP [16]; ::glGetDoublev( GL_PROJECTION_MATRIX, pP );
			GLint    pV  [4]; ::glGetIntegerv( GL_VIEWPORT, pV );
			winX = coordinate.X; winY = coordinate.Y;
			::glReadPixels( winX,winY, 1,1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ ); // get {X,Y} depth
			::gluUnProject( winX,winY,winZ, pM,pP,pV, &objX,&objY,&objZ );
			return Point( objX, objY, objZ );
		}

	Vector translate_distance_for_vertex( const Pixel& wndDistance, const Vertex& vertex )
		{
			// get difference between viewport zero point and moveBy vector,
			// and move object vertex point to new location in depth from viewport
			GLdouble x,y,z,depth;
			GLdouble* pM = (GLdouble*)&(Matrix().getCurrentMatrix( GL_MODELVIEW_MATRIX ));
			GLdouble* pP = (GLdouble*)&(Matrix().getCurrentMatrix( GL_PROJECTION_MATRIX ));
			Vector_t< GLint,4 >V; GLint* pV = (GLint*)&V; ::glGetIntegerv( GL_VIEWPORT, pV );

			// get Z-depth
			::gluProject( vertex.X, vertex.Y, vertex.Z, pM,pP,pV, &x, &y, &z );
			depth = z;

			// viewport moveBy ==> vertex space
			::gluUnProject( wndDistance.X, wndDistance.Y, depth, pM,pP,pV, &x, &y, &z );
			Vector obj( x,y,z );

			// viewport zero ==> vertex space
			::gluUnProject( 0, 0, depth, pM,pP,pV, &x, &y, &z );
			return obj - Vector( x,y,z );
		}

	Vertex current_x( const Vertex& V )
		{
			return Vertex( V.X, V.Y, V.Z );
		}

	Vertex current_y( const Vertex& V )
		{
			return Vertex( V.X, V.Y, V.Z );
		}

	Vertex current_z( const Vertex& V )
		{
			return Vertex( V.X, V.Y, V.Z );
		}

	Vertex current_axis_vector( const Vertex& rhs )
		{
			return current_z( rhs );
		}
	
////////////////////////////////////////////////////////////////////////////////

#	pragma mark -
#	pragma mark render

	/**
	 * @note doit with glLoadIdentity modelview after rendering scene
	 */
	void render_selection( const Rect& rect )
	{
		::glEnable( GL_LINE_SMOOTH );
		::glLineWidth( 1 );
		::glColor3d( 0,1,1 );
		::glBegin( GL_LINES );

		::glVertex3d( rect.left,  rect.top, 0 );
		::glVertex3d( rect.right, rect.top, 0 );

		::glVertex3d( rect.right, rect.top, 0 );
		::glVertex3d( rect.right, rect.bottom, 0 );

		::glVertex3d( rect.right, rect.bottom, 0 );
		::glVertex3d( rect.left,  rect.bottom, 0 );

		::glVertex3d( rect.left,  rect.bottom, 0 );
		::glVertex3d( rect.left,  rect.top, 0 );

		::glEnd();
	}

	void render_normales( GLint L )
	{
		::glEnable( GL_LINE_SMOOTH );
		::glLineWidth( 3 );
		::glColor3d( 0,1,1 );
		::glBegin( GL_LINES );

		// Z:Red (ontop X and Y)
		::glColor3d(  1,0,0 );
		::glVertex3d( 0,0,0 );
		::glVertex3d( 0,0,L );
			
		// X:Blue
		::glColor3d(  0,0,1 );
		::glVertex3d( 0,0,0 );
		::glVertex3d( L,0,0 );

		// Y:Green
		::glColor3d(  0,1,0 );
		::glVertex3d( 0,0,0 );
		::glVertex3d( 0,L,0 );
		::glEnd();

		// 0
		::glPointSize( 6 );
		::glColor3d(  1,1,0 );
		::glBegin( GL_POINTS );
		::glVertex3d( 0,0,0 );
		::glEnd();
	}

	void render_test_rect( GLint L )
	{
		::glDisable( GL_LINE_SMOOTH );
		::glLineWidth( 1 );
		::glColor3d( 1, 1, 1 );
		::glBegin( GL_LINES );

		::glVertex3d( L, L, 0 );
		::glVertex3d( L,-L, 0 );

		::glVertex3d( L,-L, 0 );
		::glVertex3d(-L,-L, 0 );

		::glVertex3d(-L,-L, 0 );
		::glVertex3d(-L, L, 0 );

		::glVertex3d(-L, L, 0 );
		::glVertex3d( L, L, 0 );

		::glEnd();
	}

	void render_test_sphere()
	{
		  GLUquadric* quadric =
		::gluNewQuadric();
		::gluSphere( quadric, 16, 16, 16 );
		::gluDeleteQuadric( quadric );
	}
	
	/**
	 * @note draw at object coordinate space
	 */
	Rect step_rounded_rect( const Rect& R, unsigned S, float C ) // FIXME: depricated.
	{
		const Pixel nil = Pixel();
	//	const Pixel fix = nil - (nil / S) * S; // ???
		const Pixel min = pixel_from_point( window_to_object( R.topLeft() ) );
		const Pixel max = pixel_from_point( window_to_object( R.bottomRight()));
		return Rect( min, max );
	}

	/**
	 * @note draw at window coordinate space
	 */
	Rect step_rounded_rect( const Rect& R, unsigned S, float C, const Pixel& O )
	{
#	ifdef DEF_USE_ONLY_FLOATING_TYPES
		Pixel fix; // остаток от округления до шага ячейки
		fix.X = O.X - ::floor(O.X/S)*S;
		fix.Y = O.Y - ::floor(O.Y/S)*S;
#	else
		Pixel fix = O - (O/S)*S; // FIXME: integer calculation - cause a fixed precision error
#	endif
		Pixel delta = fix - Pixel( R.size().W / 2.0, R.size().H / 2.0);
		const Pixel min = R.topLeft() += delta;
		const Pixel max = R.bottomRight() += delta;
		return Rect( min, max );
	}

	/**
	 * @note modelview was been set to identity before calling this function.
	 */
	void render_ruler( const Rect& rect, unsigned step )
	{
		::glColor3d( 0,0,1 );
		::glLineWidth( 1 );
		::glDisable( GL_LINE_SMOOTH );
		::glBegin( GL_LINES );
			{
				Floating_t w2 = rect.width()/2;
				Floating_t h2 = rect.height()/2;

				for( Floating_t x = rect.left; x < w2; x += step )
					{
						::glVertex3d( x,-h2, 0 );
						::glVertex3d( x, h2, 0 );
					}
				for( Floating_t x = rect.left; x > -w2; x -= step )
					{
						::glVertex3d( x,-h2, 0 );
						::glVertex3d( x, h2, 0 );
					}
				for( Floating_t y = rect.top; y < h2; y += step )
					{
						::glVertex3d(-w2, y, 0 );
						::glVertex3d( w2, y, 0 );
					}
				for( Floating_t y = rect.top; y > -h2; y -= step )
					{
						::glVertex3d(-w2, y, 0 );
						::glVertex3d( w2, y, 0 );
					}
			}
		::glEnd();

		::glColor3d( 1,1,1 );
		::glPointSize( 5 );
		::glBegin( GL_POINTS );
			{
				::glVertex3i( 0,0,0 ); // center
			}
		::glEnd();
	}

////////////////////////////////////////////////////////////////////////////////

#	pragma mark -
#	pragma mark TECT

	bool check_brush_size() // looking around... see "tests" ?))
		{
			// true if brush created successfully. TODO: check vertexes arrays also.
			// тип значения не имеет из-за относительности подсчета кол-ва точек в призмах
			return sizeof( The::Brush_t< The::Floating_t,3 > )/sizeof( The::Floating_t ) == 6
				&& sizeof( The::Brush_t< The::Floating_t,4 > )/sizeof( The::Floating_t ) == 8;
		}

	void create_brush( int sides )
		{
			// TODO: need to always create brush in a camera system, and translate to objects system in a current orientation.
			// HOWTO: create temporary camera system space copy and paste from it to current one.
			temp.clear();
			try {
					temp.createBrush( sides );
				}
			catch( Common::Error& exception )
				{
					exception.verbose(); cout << "Brush created using current workspace." << endl;
					space.createBrush( sides ); // QC Passed way.
					return;
				}
			space.paste_from( temp );
		}

	void delete_selected_brushes()
		{
			space.deleteSelectedBrushes();
		}

	void render_space()
		{
			space.render();
		}

	bool select_objects( bool a_pointing_selection/* mouse clicked directly on brush center w/o drag */)
		{
			const Rect& area = Mouse::isManhattanDistance()
				? Mouse::selectionRect().sorted() : Mouse::pressedRect();
			return space.selectBrushes( area, a_pointing_selection );
		}

	bool select_points( bool a_pointing_selection/* mouse clicked directly on brush apex w/o drag */)
		{
			const Rect& area = Mouse::isManhattanDistance()
				? Mouse::selectionRect().sorted() : Mouse::pressedRect();
			return space.selectPoints( area, a_pointing_selection );
		}

	void unselect_points()
		{
			space.unselectPoints();
		}

	void unselect_objects()
		{
			space.unselectBrushes();
		}

	bool is_click_to_selection()
		{
			return space.isSelectedPointsContains( Mouse::pressedRect() );
		}

	void move_selected_points_by( const Pixel& vector )
		{
			space.moveSelectedPointsBy( vector );
		}

	// example
	void multi_draw_arrays( GLenum mode, int* first, GLsizei* count, GLsizei primcount )
		{
			for( int i = 0; i < primcount; i++ )
				{
					if (count[i] > 0)
						::glDrawArrays( mode, first[i], count[i] );
				}
		}

	void save_map()
		{
			space.save();
		}

	void load_map( const char* map_name )
		{
			space.load( map_name );
		}

	void copy_to_buffer()
		{
			temp = space.copy_selected();
		}

	void paste_from_buffer()
		{
			space.paste_from( temp );
		}
	//	(Re: *no current space at now)

	void print()
		{
			space.printInfo();
		}

	void debug()
		{
			cout << "<breakpoint>" << endl;
		}
}
// glo::

#define __THE_C_FUNCTIONS_H__
#endif
