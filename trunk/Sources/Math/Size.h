/*
 *  Size.h
 *  the
 *
 *  Created by Alexander Zvyagin on 23.12.10.
 *  Copyright 2010 The(c)ompany. All rights reserved.
 *
 */

#pragma mark -
#pragma mark size
#pragma mark -

template< class T >union Size_t
{
	struct { T W; T H; }; // is under union
	struct { T Width; T Height; }; // is under union
	inline Size_t(): W( 0 ), H( 0 ) {}
	inline Size_t( T w, T h ): W( w ), H( h ) {}
	inline Size_t( const Size_t& rhs ): W( rhs.W ), H( rhs.H ) {}
	inline Size_t& operator = ( const Size_t& rhs ) { W = rhs.W; H = rhs.H; return self; }
	friend std::ostream& operator << < T >( std::ostream&, const Size_t< T >& );
};
