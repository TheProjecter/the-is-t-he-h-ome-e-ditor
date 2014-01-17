/*
 *  Rect.h
 *  the
 *
 *  Created by Alexander Zvyagin on 23.12.10.
 *  Copyright 2010 The(c)ompany. All rights reserved.
 *
 */

#pragma mark class Rect
#pragma mark -

template< class T >struct Rect_t // pos( left, top )
{
	T top; T left; T bottom; T right;
	Rect_t();
	Rect_t( T x, T y, T w, T h );
#ifdef DEF_USE_PIXEL_INSTEAD_OF_VECTOR
	Rect_t( const Pixel_t< T >& topLeft, const Pixel_t< T >& bottomRight );
	Rect_t( const Pixel_t< T >& pos, const Size_t< T >& size );
	bool contains( const Pixel_t< T >& rhs, short exceed = 0 ) const;
	Pixel_t< T >pos() const;
#else
	Rect_t( const Vector_t< T,2 >& topLeft, const Vector_t< T,2 >& bottomRight );
	Rect_t( const Vector_t< T,2 >& pos, const Size_t< T >& size );
	bool contains( const Vector_t< T,2 >& rhs, short exceed = 0 ) const;
	Vector_t< T,2 >pos() const;
#endif
	bool contains( const Vector_t< T,3 >& rhs, short exceed = 0 ) const;
//	bool contains( const Point& rhs, short exceed = 0 ) const;
	Size_t< T >size() const;
	T width() const;
	T height() const;
	bool isNull() const;
	void sort();
	Rect_t sorted() const;
	Rect_t& resize( T w, T h );
	Rect_t& resize( const Size_t< T >& sz );
#ifdef DEF_USE_PIXEL_INSTEAD_OF_VECTOR
	Rect_t& moveBy( const Pixel_t< T >& rhs );
	Rect_t& moveTo( const Pixel_t< T >& rhs );
#else
	Rect_t& moveBy( const Vector_t< T,2 >& rhs );
	Rect_t& moveTo( const Vector_t< T,2 >& rhs );
#endif
	Rect_t& moveBy( T x, T y );
	Rect_t& moveTo( T x, T y );
#ifdef DEF_USE_PIXEL_INSTEAD_OF_VECTOR
	Pixel_t< T >topLeft() const;
	Pixel_t< T >topRight() const;
	Pixel_t< T >bottomLeft() const;
	Pixel_t< T >bottomRight() const;
#else
	Vector_t< T,2 >topLeft() const;
	Vector_t< T,2 >topRight() const;
	Vector_t< T,2 >bottomLeft() const;
	Vector_t< T,2 >bottomRight() const;
#endif
	friend std::ostream& operator << < T >( std::ostream&, const Rect_t< T >& );
};

#pragma mark -

	template< class T >inline Rect_t< T >
		::	Rect_t()
		:	top( 0 )
		,	left( 0 )
		,	bottom( 0 )
		,	right( 0 )
		{
		}

	template< class T >inline Rect_t< T >
		::	Rect_t( T x, T y, T w, T h )
		:	left( x )
		,	top( y )
		,	right( x + w )
		,	bottom( y + h )
		{
		}

#ifdef DEF_USE_PIXEL_INSTEAD_OF_VECTOR

	template< class T >inline Rect_t< T >
		::	Rect_t( const Pixel_t< T >& topLeft, const Pixel_t< T >& bottomRight )
		:	top( topLeft.Y )
		,	left( topLeft.X )
		,	bottom( bottomRight.Y )
		,	right( bottomRight.X )
		{
		}

	template< class T >inline Rect_t< T >
		::	Rect_t( const Pixel_t< T >& pos, const Size_t< T >& size )
		:	top( pos.Y )
		,	left( pos.X )
		,	bottom( size.Y - pos.Y )
		,	right( size.X - pos.X )
		{
		}
#else
	template< class T >inline Rect_t< T >
		::	Rect_t( const Vector_t< T,2 >& topLeft, const Vector_t< T,2 >& bottomRight )
		:	top( topLeft.Y )
		,	left( topLeft.X )
		,	bottom( bottomRight.Y )
		,	right( bottomRight.X )
		{
		}

	template< class T >inline Rect_t< T >
		::	Rect_t( const Vector_t< T,2 >& pos, const Size_t< T >& size )
		:	top( pos.Y )
		,	left( pos.X )
		,	bottom( size.H - pos.Y )
		,	right( size.W - pos.X )
		{
		}
#endif

	template< class T >inline void Rect_t< T >
		::	sort()
		{
			// TODO: sort
		}

	template< class T >inline Rect_t< T >Rect_t< T >
		::	sorted() const
		{
			Rect_t< T >R;
			const bool cond1 = (left > right); R.left = cond1 ? right : left; R.right = cond1 ? left : right;
			const bool cond2 = (top > bottom); R.top = cond2 ? bottom : top; R.bottom = cond2 ? top : bottom;
			return R;
		}

#ifdef DEF_USE_PIXEL_INSTEAD_OF_VECTOR

	template< class T >inline bool Rect_t< T >
		::	contains( const Pixel_t< T >& rhs, short exceed ) const
		{
			const bool cond1 = (left <= right); T l = cond1 ? left : right; T r = cond1 ? right : left;
			const bool cond2 = (top <= bottom); T t = cond2 ? top : bottom; T b = cond2 ? bottom : top;
			return (rhs.X >= l - exceed) && (rhs.X <= r + exceed) && (rhs.Y >= t - exceed) && (rhs.Y <= b + exceed);
		}
#else
	template< class T >inline bool Rect_t< T >
		::	contains( const Vector_t< T,2 >& rhs, short exceed ) const
		{
			const bool cond1 = (left <= right); T l = cond1 ? left : right; T r = cond1 ? right : left;
			const bool cond2 = (top <= bottom); T t = cond2 ? top : bottom; T b = cond2 ? bottom : top;
			return (rhs.X >= l - exceed) && (rhs.X <= r + exceed) && (rhs.Y >= t - exceed) && (rhs.Y <= b + exceed);
		}
#endif
	template< class T >inline bool Rect_t< T >
		::	contains( const Vector_t< T,3 >& rhs, short exceed ) const
	//	::	contains( const Point& rhs, short exceed ) const
		{
			const bool cond1 = (left <= right); T l = cond1 ? left : right; T r = cond1 ? right : left;
			const bool cond2 = (top <= bottom); T t = cond2 ? top : bottom; T b = cond2 ? bottom : top;
			return (rhs.X >= l - exceed) && (rhs.X <= r + exceed) && (rhs.Y >= t - exceed) && (rhs.Y <= b + exceed);
		}

	template< class T >inline Size_t< T >Rect_t< T >::size() const { return Size_t< T >( right - left, bottom - top ); }

	template< class T >inline T Rect_t< T >::width() const { return( right - left ); }
	template< class T >inline T Rect_t< T >::height() const { return( bottom - top ); }

	template< class T >inline bool Rect_t< T >::isNull() const { return !top && !bottom && !left && !right; }

	template< class T >inline Rect_t< T >& Rect_t< T >::resize( T w, T h ) { right = left + w; bottom = top + h; return self; }
	template< class T >inline Rect_t< T >& Rect_t< T >::resize( const Size_t< T >& sz ) { right = left + sz.Width; bottom = top + sz.Height; return self; }

#ifdef DEF_USE_PIXEL_INSTEAD_OF_VECTOR

	template< class T >inline Rect_t< T >& Rect_t< T >::moveBy( const Pixel_t< T >& rhs ) { left += rhs.X; top += rhs.Y; right += rhs.X; bottom += rhs.Y; return self; }
	template< class T >inline Rect_t< T >& Rect_t< T >::moveTo( const Pixel_t< T >& rhs ) { right += rhs.X - left; bottom += rhs.Y - top; left = rhs.X; top = rhs.Y; return self; }
#else
	template< class T >inline Rect_t< T >& Rect_t< T >::moveBy( const Vector_t< T,2 >& rhs ) { left += rhs.X; top += rhs.Y; right += rhs.X; bottom += rhs.Y; return self; }
	template< class T >inline Rect_t< T >& Rect_t< T >::moveTo( const Vector_t< T,2 >& rhs ) { right += rhs.X - left; bottom += rhs.Y - top; left = rhs.X; top = rhs.Y; return self; }

#endif
	template< class T >inline Rect_t< T >& Rect_t< T >::moveBy( T x, T y ) { left += x; top += y; right += x; bottom += y; return self; }
	template< class T >inline Rect_t< T >& Rect_t< T >::moveTo( T x, T y ) { right += x - left; bottom += y - top; left = x; top = y; return self; }

#ifdef DEF_USE_PIXEL_INSTEAD_OF_VECTOR

	template< class T >inline Pixel_t< T >Rect_t< T >::pos() const { return Pixel_t< T >( left, top ); }
	template< class T >inline Pixel_t< T >Rect_t< T >::topLeft() const { return Pixel_t< T >( left, top ); }
	template< class T >inline Pixel_t< T >Rect_t< T >::topRight() const { Pixel_t< T >( right, top ); }
	template< class T >inline Pixel_t< T >Rect_t< T >::bottomLeft() const { Pixel_t< T >( left, bottom ); }
	template< class T >inline Pixel_t< T >Rect_t< T >::bottomRight() const { Pixel_t< T >( right, bottom ); }
#else
	template< class T >inline Vector_t< T,2 >Rect_t< T >::pos() const { return Vector_t< T,2 >( left, top ); }
	template< class T >inline Vector_t< T,2 >Rect_t< T >::topLeft() const { return Vector_t< T,2 >( left, top ); }
	template< class T >inline Vector_t< T,2 >Rect_t< T >::topRight() const { return Vector_t< T,2 >( right, top ); }
	template< class T >inline Vector_t< T,2 >Rect_t< T >::bottomLeft() const { return Vector_t< T,2 >( left, bottom ); }
	template< class T >inline Vector_t< T,2 >Rect_t< T >::bottomRight() const { return Vector_t< T,2 >( right, bottom ); }

#endif
