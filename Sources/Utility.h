////////////////////////////////////////////////////////////////////////////////
///  the_functions.h
///  the
///
///  Created by Atz on 17.11.10.
///  Copyright 2010 The(c)ompany. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __THE_UTILITY_H__

// please include "the.h" instead of this file.

#include <list>

////////////////////////////////////////////////////////////////////////////////

namespace The { struct Keys; }
namespace glo
	{
		using namespace std;
		using namespace The;

		void isometric( const Rect& viewport, GLdouble fov, GLdouble near, GLdouble far, GLdouble scale = 1024 );
		void linear( const Rect& viewport, GLdouble fov, GLdouble near, GLdouble far, GLdouble scale = 1 );

		int random_between( int from, int to );
		const char* current_mode_string();

		void print_opengl_version( bool print_extensions = false );
		void print_event( const Keys& );
		inline void print_current_mode() { cout << current_mode_string() << endl; }
		void print_current_matrix( GLenum );

		void render_ruler( const Rect&, unsigned step );
		void render_normales( GLint );
		void render_selection( const Rect& );
		void render_test_rect( GLint );
		void render_test_sphere();

		Rect step_rounded_rect( const Rect& viewport, unsigned step, float orpho_scale );
		Rect step_rounded_rect( const Rect& viewport, unsigned step, float orpho_scale, const Pixel& object_center );

		Point window_to_object( const Pixel& vertex = Pixel() );
		Point object_to_window( const Point& vertex = Point() );
		Floating_t vertex_depth( const Point& vertex ); // unused.

		Vector translate_distance_for_vertex( const Pixel& wndDistance, const Vertex& vertex );
		void move_vertex_by( Vertex& vector, const Pixel& moveBy, Vector** shift = 0 );

		inline Point vertex_projection( const Vertex& rhs ) { return object_to_window( Point( rhs.X, rhs.Y, rhs.Z )); }
#	ifdef DEF_USE_ONLY_FLOATING_TYPES
		inline Pixel pixel_from_point( const Point& rhs ) { return Pixel( rhs.X, rhs.Y ); }
#	else
		inline Pixel pixel_from_point( const Point& rhs ) { return Pixel(::lround( rhs.X ),::lround( rhs.Y )); }
#	endif
		inline Vector vector_from_point( const Point& rhs ) { return Vector( rhs.X, rhs.Y, rhs.Z, 1 ); }
		inline Matrix matrix_between( const Matrix& object, const Matrix& camera ) { return Matrix( object, camera ); }
		inline void use_matrix_for_mode( const Matrix& rhs, GLenum forMode ) { rhs.setCurrentMode( forMode ); }
		inline void transpose_current_matrix( GLenum type ) { The::transpose_opengl_matrix< GLdouble >( type ); }

		bool select_objects( bool a_pointing_selection = false );
		bool select_points( bool a_pointing_selection = false );
		bool is_click_to_selection();
		void move_selected_points_by( const Pixel& );
		void unselect_objects();
		void unselect_points();

		void save_map();
		void load_map( const char* map_name = "unnamed" );

		void copy_to_buffer();
		void paste_from_buffer();

		Vertex current_axis_vector( const Vertex& );

		// test section:
		void create_brush( int sides = 3 );
		void delete_selected_brushes();
		void render_space();
		void multi_draw_arrays( GLenum mode, int* first, GLsizei* count, GLsizei primcount );

		void print();
		void debug();
	}

////////////////////////////////////////////////////////////////////////////////

#define __THE_UTILITY_H__
#endif
