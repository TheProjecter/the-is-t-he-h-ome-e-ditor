/*
 *  Vector.h
 *  the
 *
 *  Created by Alexander Zvyagin on 23.12.10.
 *  Copyright 2010 The(c)ompany. All rights reserved.
 *
 */

#pragma mark -
#pragma mark pixel
#pragma mark -

template< class T >struct Pixel_t
{
	struct{ T X; T Y; };
	inline Pixel_t(): X( 0 ), Y( 0 ) {}
	inline Pixel_t( T x, T y ): X( x ), Y( y ) {}
};
