////////////////////////////////////////////////////////////////////////////////
///  CocoaView.h
///  the
///
///  Created by Atz on 17.11.10.
///  Copyright 2010 The(c)ompany. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////

// the.h is included from .m

#import <Cocoa/Cocoa.h>

namespace The
	{
		class View;
	}

@interface CocoaView: NSOpenGLView
	{
		BOOL m_isInteractive;
		NSTimer*	m_timer1;
		The::View*	m_view;
	}

-( IBAction )setOrpho:( id )sender;
-( IBAction )setPerspective:( id )sender;

-( void )resize;

@end
