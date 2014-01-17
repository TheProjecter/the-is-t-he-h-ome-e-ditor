////////////////////////////////////////////////////////////////////////////////
///  accessory.h
///  The
///
///  Created by Alexander Zvyagin on 06.09.10.
///  Copyright 2010 The(c)ompany. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////

#	ifndef __ACCESSORY_H__

#	include <aio.h>
#	include <fcntl.h>
#	include <stdio.h>	// ::perror()
#	include <stdlib.h>
#	include <string.h>	// ::strerror()
#	include <errno.h>
#	include <unistd.h>
#	include <sys/uio.h>
#	include <sys/stat.h>
#	include <sys/types.h>

#	include "strarr.h"

#	define DEF_DEBUG_INLINES

#	pragma mark -

	namespace Common
		{
			struct Error
				{
					Error( const String& );
					const char* message() const;
					void verbose() const;
					private: const String m_msg;
				};
		}

#	pragma mark -

	namespace File
		{
			extern int const BUF_SIZE;

			struct Access
				{
					enum Type
					{
						Show = O_RDONLY, // == 0
						Edit = O_CREAT | O_RDWR,
						Zero = O_TRUNC | Edit,
						More = O_APPEND| Edit,
						DataBase = Edit
					};
					Access( Type );

				private:

					typedef const char* Mode;

					const int m_access;
					Mode m_mode;

					static Mode Type2mode( Type );

				public:

					int flags() const;
					Mode mode() const;
				};

#		pragma mark -

			class Error
				{
					const String m_msg;

				public:

					Error( const String& );
					const char* message() const;
					void verbose() const;
				};

#		pragma mark -

			struct Handle
				{
				   ~Handle();
					Handle( const String& name, Access::Type type );

					int lseek( int offset, int whence ) const;
					int write( void *buf, unsigned bytes ) const;
					int read( void *buf, unsigned bufsize ) const;
					int eof() const;
					int parse(); // will return the total amout of file lines
					int filelength() const;
					int truncate();
					bool only_spaces( const char *s, int len );
					virtual
					bool next_line( int linenum, const char *line, int length ); // must return false if no err
					const char* name() const;
					unsigned int crc32() const;

				private:

					unsigned int crc32calc( const char *, unsigned int, unsigned int ) const; // Calculate the 32 bit CRC of a string
					static unsigned int const crc32tab[ 256 ];

					String   m_name;
					Access m_access;
					int    m_handle;
					FILE *   m_file;
				};

			//	TODO:
			//	class DBFiles: public Files;
			//	class LogFiles: public TextFiles;
			//	class ConfigFiles: public TextFiles;
			//	class FtnAddr; // class PathName.
		}
		// namespace File

#	pragma mark -

	inline File::Access::Mode File::Access::

		Type2mode( File::Access::Type type )
			{
				switch( type )
				{
					case Show:	return "r" ;
					case Edit:	return "r+";
					case Zero:	return "w+";
					case More:	return "a+";
					default:	return  "" ;
				}
			}

	inline File::Access::

		Access( File::Access::Type type )
			:	m_access( type )
			,	m_mode( File::Access::Type2mode( type ))
			{
			}

	inline int File::Access::

		flags() const
			{
				return m_access;
			}

	inline File::Access::Mode File::Access::

		mode() const
			{
				return m_mode;
			}

#	ifndef DEF_DEBUG_INLINES

#	pragma mark -

	inline Common::Error::

		Error( const String& msg )
			:	m_msg( msg )
			{
				::printf( msg );
			}

	inline const char* Common::Error::

		message() const
			{
				return m_msg;
			}

	inline void Common::Error::

		verbose() const
			{
				::printf( message() );
			}

#	pragma mark -

	inline File::Error::

		Error( const String& msg )
			:	m_msg( msg )
			{
				::perror( msg );
			}

	inline const char* File::Error::

		message() const
			{
				return ::strerror( errno );
			}

	inline void File::Error::

		verbose() const
			{
				::perror( message() );
			}

#	pragma mark -

	inline File::Handle::

		Handle( const String& str, File::Access::Type type )
			:	m_name( str )
			,	m_access( type )
			,	m_handle(str.size() ? ::open( m_name, m_access.flags(), S_IREAD | S_IWRITE ) : -2)
			,	m_file(m_handle > 0 ? ::fdopen( m_handle, m_access.mode() ) : 0)
			{
				if (-1 == m_handle || !(m_file)) throw Error( m_name );
			}
#	endif

	inline File::Handle::

		~Handle()
			{
				// ::fclose() will also close the m_handle
				if (!m_file || ::fclose( m_file ) < 0)
					{
						if (m_handle <= 0 || ::close( m_handle ) < 0)
							{
								throw Error( m_name );
							}
					}
				m_file = 0;
				m_handle = 0;
			}

	inline int File::Handle::

		lseek( int offset, int whence ) const
			{
				int rc = ::lseek( m_handle, offset, whence );
				return( 1 + rc ? rc: throw Error( m_name ));
			}

	inline int File::Handle::

		write( void *buf, unsigned bytes ) const
			{
				int rc = ::write( m_handle, buf, bytes );
				return( 1 + rc ? rc: throw Error( m_name ));
			}

	inline int File::Handle::

		read( void *buf, unsigned bufsize ) const
			{
				int rc = ::read( m_handle, buf, bufsize );
				return( 1 + rc ? rc: throw Error( m_name ));
			}

	inline int File::Handle::

		truncate()
			{
				int rc = ::ftruncate( m_handle, 0 );
				return( 1 + rc ? rc: throw Error( m_name ));
			}

	inline int File::Handle::

		eof() const
			{
				int rc = ::feof( m_file );
				return( 1 + rc ? rc: throw Error( m_name ));
			}

	// inline bool File::Handle::
	inline int File::Handle::

		filelength() const
			{
				struct stat info;
				info.st_size = -1;
				if (::fstat( m_handle, &info ) == -1)
				{
					throw Error( m_name );
				}
				return info.st_size;
			}

	inline const char* File::Handle::name() const
		{
			return m_name;
		}

#	pragma mark -

	typedef File::Handle Files;
	typedef File::Error Errors;

#	define __ACCESSORY_H__
#	endif
