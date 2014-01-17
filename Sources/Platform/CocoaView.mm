////////////////////////////////////////////////////////////////////////////////
///  CocoaView.m
///  the
///
///  Created by Atz on 17.11.10.
///  Copyright 2010 The(c)ompany. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////

#import "the.h"
/*
// the.h expecting model:
#include "ogl.h"
#include "the_math.h"
#include "the_types.h"
#include "Utility.h"
#include "Modifiers.h"
#include "Keys.h"
#include "Mouse.h"
#include "View.h"
*/
#import "CocoaView.h"


@implementation CocoaView


+( NSOpenGLPixelFormat* )basicPixelFormat // reimp.
	{
		NSOpenGLPixelFormatAttribute attributes [] = // an array:
			{
				NSOpenGLPFAWindow,
				NSOpenGLPFADoubleBuffer,			// double buffered
				NSOpenGLPFADepthSize,
				(NSOpenGLPixelFormatAttribute) 16,	// 16 bit depth buffer
				(NSOpenGLPixelFormatAttribute) nil
			};
		return [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
	}

-( void )prepareOpenGL // reimp.
	{
		glo::print_opengl_version();
		// XXX: make ::glo_init();
		m_view = ::new The::View_t <>;
		[self resize];
	}

-( void )rotate
	{
		// m_view->onTimerTest();
		[super setNeedsDisplay:YES];
	}

-( void )setTimer
	{
		m_timer1 = [[NSTimer scheduledTimerWithTimeInterval: 1.0 / 40
													 target: self
												   selector: @selector( rotate )
												   userInfo: nil
													repeats: YES] retain];
	}

-( void )updateTrackingAreas
	{
#	if (MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_5)
		// FIXME: no mouse move events
		[super addTrackingRect:[super bounds] owner:self userData:nil assumeInside:NO];
#	else
		NSArray* trackingArray = nil;
		if (trackingArray = [super trackingAreas])
			{
				for( NSTrackingArea* area in trackingArray )
					{
						[super removeTrackingArea:area];
					}
			}
		if (!m_isInteractive)
			{
				return;
			}
		[super updateTrackingAreas];

		NSUInteger trackingOptions
			=	NSTrackingMouseEnteredAndExited
			|	NSTrackingActiveInActiveApp
			|	NSTrackingInVisibleRect
			|	NSTrackingMouseMoved;

		NSRect theRect = [super frame];
		NSRect newRect = NSMakeRect( 0, 0, theRect.size.width, theRect.size.height );

		NSTrackingArea *area = [[NSTrackingArea alloc]
								initWithRect: newRect
									 options: trackingOptions
									   owner: self
									userInfo: nil];

		[super addTrackingArea:area];
		[area release];
#	endif
	}

-( void )awakeFromNib
	{
		[self updateTrackingAreas]; m_isInteractive = YES;
		[self setTimer];
	}


-( void )dealloc
	{
		[m_timer1 release]; m_timer1 = nil;
		[super dealloc];
	}

-( BOOL )acceptsFirstResponder
	{
		return YES;
	}

-( BOOL )becomeFirstResponder
	{
		return YES;
	}

-( BOOL )resignFirstResponder
	{
		return YES;
	}

// -( NSView* )hitTest:( NSPoint )thePoint { NSPoint point = [self convertPoint:thePoint fromView:[self superview]]; }
/*
 NSAlphaShiftKeyMask // CAPS LOCK
 NSShiftKeyMask // SHIFT
 NSControlKeyMask // CTRL
 NSAlternateKeyMask // ALT
 NSCommandKeyMask // WIN
 NSNumericPadKeyMask // set with arrows:
 NSFunctionKeyMask // F1-F12
 
 [NSEvent modifierFlags]
 
 NSUpArrowFunctionKey
 NSDownArrowFunctionKey
 NSLeftArrowFunctionKey
 NSRightArrowFunctionKey
*/

+( unsigned )glutModifiersFromNSEvent:( unsigned int )sysflags
	{
		unsigned int glutflags = 0;
		// FIXME: NSCommandKeyMask is unworking (tested on 10.6.8)
		if (sysflags & NSAlternateKeyMask) glutflags |= GLUT_ACTIVE_ALT;
		if (sysflags & NSControlKeyMask) glutflags |= GLUT_ACTIVE_CTRL;
		if (sysflags & NSShiftKeyMask) glutflags |= GLUT_ACTIVE_SHIFT;
		if (sysflags & NSCommandKeyMask) glutflags |= GLUT_ACTIVE_WIN;
		// std::cout << sysflags << " ==> " << glutflags << std::endl;
		return glutflags;
	}

-( The::Pixel )coordinateFromWindow:( NSPoint )thePoint
	{
		NSPoint point = [self convertPoint:thePoint fromView:nil/* from window */];
		return glo::pixel_from_point( The::Point( point.x, point.y, 0 ));
	}

#	define DEF_GLUT_MODIFIERS( event )\
		[[self class] glutModifiersFromNSEvent:[event modifierFlags]]

#	define DEF_CONVERT_POINT( event )\
		[self coordinateFromWindow:[event locationInWindow]]

#	define DEF_KEY_CODE( event )\
		[event keyCode]

#	define DEF_INVALIDATE\
		[super setNeedsDisplay:YES];


////////////////////////////////////////////////////////////////////////////////


-( void )mouseEntered:( NSEvent* )theEvent
	{
		m_view->handle_mouse_enter( DEF_GLUT_MODIFIERS( theEvent ), DEF_CONVERT_POINT( theEvent )); DEF_INVALIDATE
	}

-( void )mouseExited:( NSEvent* )theEvent
	{
		m_view->handle_mouse_leave( DEF_GLUT_MODIFIERS( theEvent ), DEF_CONVERT_POINT( theEvent )); DEF_INVALIDATE
	}

-( void )mouseDown:( NSEvent* )theEvent
	{
		m_view->handle_mouse_press( DEF_GLUT_MODIFIERS( theEvent ), DEF_CONVERT_POINT( theEvent )); /*[NSCursor hide];*/ DEF_INVALIDATE
	}

-( void )mouseUp:( NSEvent* )theEvent
	{
		m_view->handle_mouse_release( DEF_GLUT_MODIFIERS( theEvent ), DEF_CONVERT_POINT( theEvent )); /*[NSCursor unhide];*/ DEF_INVALIDATE
	}

-( void )mouseMoved:( NSEvent* )theEvent
	{
		m_view->handle_mouse_move( DEF_GLUT_MODIFIERS( theEvent ), DEF_CONVERT_POINT( theEvent ), 0 ); DEF_INVALIDATE
	}

-( void )mouseDragged:( NSEvent* )theEvent
	{
		m_view->handle_mouse_move( DEF_GLUT_MODIFIERS( theEvent ), DEF_CONVERT_POINT( theEvent ), 1 ); DEF_INVALIDATE
	}

////////////////////////////////////////////////////////////////////////////////

-( void )keyDown:( NSEvent* )theEvent
	{
		m_view->handle_key_press( DEF_KEY_CODE( theEvent ), DEF_GLUT_MODIFIERS( theEvent )); DEF_INVALIDATE
	}

-( void )keyUp:( NSEvent* )theEvent
	{
		m_view->handle_key_release( DEF_KEY_CODE( theEvent ), DEF_GLUT_MODIFIERS( theEvent )); DEF_INVALIDATE
	}

////////////////////////////////////////////////////////////////////////////////

+( The::Rect )theRectFromNSRect:( NSRect )rect
	{
		const float X = ::round( rect.origin.x );
		const float Y = ::round( rect.origin.y );
		const float W = ::round( rect.size.width );
		const float H = ::round( rect.size.height );
		return The::Rect( X,Y,W,H );
	}

-( void )resize
	{
		m_view->handle_viewport_rect( [CocoaView theRectFromNSRect: [self bounds] ]);
	}

-( void )setFrame:( NSRect )rect
	{
		[super setFrame:rect];
		[self resize];
	}

-( void )viewDidEndLiveResize
	{
		[super viewDidEndLiveResize];
		[self resize];
	}

-( IBAction )setOrpho:( id )sender
	{
		NSLog(@"-( IBAction )setOrpho:( id )sender");
		m_view->setOrpho(); DEF_INVALIDATE
	}

-( IBAction )setPerspective:( id )sender
	{
		NSLog(@"-( IBAction )setPerspective:( id )sender");
		m_view->setPerspective(); DEF_INVALIDATE
	}

-( void )drawRect:( NSRect )rect
	{
		static const int any_value = 100; m_view->renderScene( any_value );
	}

@end
