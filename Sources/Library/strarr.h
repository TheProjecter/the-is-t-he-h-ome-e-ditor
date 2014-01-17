////////////////////////////////////////////////////////////////////////////////
///  strarr.h
///  The
///
///  Created by Alexander Zvyagin on 06.09.10.
///  Copyright 2010 The(c)ompany. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////

#	ifndef __MY_STL_H__

	namespace arr
		{
			template< class T >inline long sz( const T* s ) { const T* p = s; while( *p++ ); return( p - s ); }
			template< class T >inline T* cp( T* a, const T* b ) { T* r = a; while( *a++ = *b++ ); return r; }
			template< class T >inline bool eq( const T* a, const T* b ) { while( *a  ) { if( *a++ != *b++ ) return false; } return true; }
		}

#	pragma mark -

	namespace str
		{
			bool cmp( const char *a, const char *b );
			char*cpy( char *a, const char *b );
			long len( const char *s );

#	pragma mark -

			class obj
				{
					long  len;
					char* str;
				public:
					operator char*();
					operator const char*() const;
					const
					char& operator[]( int )
					const;
					char& operator[]( int );
					const
					obj& operator+=( const char* );
					obj& operator=( const char* );
					obj& operator=( const obj& );
					obj( const char*, long bytes );
					obj( const char* );
					obj( const obj& );
					obj( long bytes );
					obj();
				   ~obj();
					long size() const;
					const char* value() const;
				};
		} // str::

#	pragma mark -

	inline bool operator==( const str::obj& a, const str::obj& b ) { return !str::cmp( &a[ 0 ], &b[ 0 ] ); }	       
	inline bool operator==( const str::obj& rhs, const char* ptr ) { return !str::cmp( &rhs[ 0 ], ptr ); }	       
	inline bool operator==( const char* ptr, const str::obj& rhs ) { return !str::cmp( &rhs[ 0 ], ptr ); }	       
	inline bool operator!=( const str::obj& a, const str::obj& b ) { return  str::cmp( &a[ 0 ], &b[ 0 ] ); }	       
	inline bool operator!=( const str::obj& rhs, const char* ptr ) { return  str::cmp( &rhs[ 0 ], ptr ); }	       
	inline bool operator!=( const char* ptr, const str::obj& rhs ) { return  str::cmp( &rhs[ 0 ], ptr ); }	       
	inline const str::obj operator+( const str::obj& a, const str::obj& b ) { return( str::obj( a ) += b ); }	       
//	inline ostream& operator<<( ostream& stream, const str::obj& rhs ) { stream << &rhs[ 0 ]; return stream; }

#	pragma mark -

	inline long str::len( const char *s ) { return arr::sz( s ); }
	inline char *str::cpy( char *a, const char *b ) { return arr::cp( a, b ); }
	inline bool str::cmp( const char *a, const char *b ) { return !arr::eq( a, b ); }

#	pragma mark -

	inline str::obj::obj(): len( 0 ), str( 0 ) {}
	inline str::obj::obj( long bytes ): str( new char[ bytes ] ), len( bytes + 1 ) {}
	inline str::obj::obj( const char *ptr ): str( str::cpy( new char[ len + 1 ], ptr )), len( str::len( ptr )) {}
	inline str::obj::obj( const char *ptr, long bytes ): str( str::cpy( new char[ len + 1 ], ptr )), len( bytes ) {}
	inline str::obj::obj( const str::obj& rhs ): str( str::cpy( new char[ len + 1 ], rhs.str )), len( str::len( rhs.str )) {}
	inline str::obj::~obj() { if (str) delete[] str; }

#	pragma mark -

	inline str::obj& str::obj::operator=( const char *ptr ) { if (str) delete[] str; str = str::cpy( new char[ (len = str::len( ptr )) + 1 ], ptr ); return *this; }
	inline str::obj& str::obj::operator=( const str::obj& rhs ) { if (str) delete[] str; str = str::cpy( new char[ (len = str::len( rhs.str )) + 1 ], rhs.str ); return *this; }
	inline char const& str::obj::operator[]( int index ) const { return str[ index ]; }
	inline char& str::obj::operator[]( int index ) { return str[ index ]; }
	inline str::obj::operator const char*() const { return str; }
	inline str::obj::operator char*() { return str; }
	inline const str::obj& str::obj::operator+=( const char* ptr )
		{
			long  oldlen = len;
			char* oldstr = str;
			str = str::cpy( str::cpy( new char[ len += str::len( ptr ) + 1 ], str ) + oldlen, ptr ) - oldlen; // потеряли len и str
			delete[] oldstr;
			return *this;
		}

#	pragma mark -

	inline const char* str::obj::value() const { return str; }
	inline long str::obj::size() const { return len; }

#	pragma mark -

	typedef str::obj String;

#	define __MY_STL_H__
#	endif

