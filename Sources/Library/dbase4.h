////////////////////////////////////////////////////////////////////////////////
///  dbf.h
///  DBaseIV
///
///  Created by Alexander Zvyagin on 06.09.10.
///  Copyright 2010 The(c)ompany. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////

#	ifndef __DBASE4_H__

#	include <iostream>
#	include <time.h>
#	include	<sys/types.h>
#	include	<stdio.h>
#	include	<stdlib.h>
#	include <math.h>

	namespace DB
		{
			using namespace std;
			const char CR = 0x0D;
			const char LF = 0x1A;
			const int blank_space = 0x20;
			template< class struct_type >class DBaseIV;
			class Struct; // DB::Struct
			typedef DBaseIV< Struct >* PBASE4_t;
			inline char* num2str( char* buf, double num, int dec )
				{
					long dec_multilier = ::pow( 10, dec );
					double rc = ::round(num * dec_multilier);
					::sprintf( buf, "%g", rc/dec_multilier );
				//	::printf("%g ==> %s (%zu/%lu)\n", num, buf, ::strlen(buf), dec);
					return buf;
				}
#	pragma mark -

			class Fields // DB::Fields
				{
					char name[ 11 ];
					char type; // C, N, ...
					void *ptr;
					unsigned char len;
					unsigned char dec;
					char reseirved[ 14 ];
				//	---------------------
					unsigned char field_pos;
					protected: PBASE4_t pBase; // 'DBaseIV::DBaseIV()'â 'pBase = this'
					public: const
					char* getName() const;
					int getLen() const;
					int getDec() const;
					PBASE4_t get_pBase() const;
					int getFieldPos() const;
					void setFieldPos( unsigned char pos );
					Fields( PBASE4_t ptr, char t, const char *n, unsigned char l, unsigned char d );
					void Init( PBASE4_t ptr, char t, const char *n, unsigned char l, unsigned char d );
					protected: void Reset( PBASE4_t ptr, char t, const char *n, unsigned char l, unsigned char d );
				};
#	pragma mark -

			class Struct
				{
					PBASE4_t pBase; int num_descriptors; DB::Fields *Iarr;
					public: const
					Fields& operator[]( int i ) const;
					Fields& operator[]( int i );
					Struct( int n_descr, PBASE4_t p );
					void setFieldLens() const;
				};
#	pragma mark -

			class Date
				{
					char year; char mounth; char day;
					protected: void get_date();
					public: Date();
				};
#	pragma mark -

			class Header // DB::Header, length 32 bytes
				{
					char n_ver; // 1
					Date last_dat; // 3
					unsigned long num_recs; // 4
					unsigned short head_len; // 2
					unsigned short rec_len; // 2
					char reseirved[ 20 ];
					public: Header( int num_descr, unsigned short reclen );
					unsigned short getRecLength() const;
					unsigned short getFullHeadLen() const;
					unsigned long getNumRecords() const;
					void setNumRecords(	unsigned long n_recs );
					void ReInit( int num_descr, unsigned short reclen );
				};
#	pragma mark -

			template< class struct_type/* Character or Numeric */>class DBaseIV : public Files
				{
					friend class Struct;
					Header* pHeader;
					struct_type db_struct; // DBDescriptor derived.
					public:class Record;
					friend class Record;
					public: int get_num_descriptors() const;
					DBaseIV( const char* filename );
				   ~DBaseIV();
					void on_get_value( char *pbuf, int len, int field_shift );
					void on_set_value( const char *pbuf, int len, int field_shift );
					void Truncate();
					void Create();
					void Open();
					void append_blank();
					void set_rec_pos( int n_rec );
					void test();
					public: const // starting from 2012+:
					Fields& operator[]( int pos ) const;
					Fields& operator[]( int pos );
					int records() const;
					DBaseIV* iterator();
				};
#	pragma mark -
#	pragma mark class DBaseIV::Record

			template< class struct_type >class DBaseIV< struct_type >::Record
				{
					DBaseIV< struct_type >* pBase; int n_rec;
					public: struct_type* operator->();
					Record( DBaseIV< struct_type >* );
					Record operator++();
					Record operator--();
					Record operator++( int );
					Record operator--( int );
					bool operator==( const Record& ) const;
					bool operator!=( const Record& ) const;
				};
#	pragma mark -

			class Numeric: public Fields
				{
					public: Numeric( PBASE4_t ptr, const char *name, unsigned char len, unsigned char dec );
					operator double(); operator const double() const;
					Numeric& operator=( double num );
				};
#	pragma mark -
			
			class Character: public Fields
				{
					public: Character( PBASE4_t ptr, const char *name, unsigned char len/*, unsigned char dec = 0*/ );
					operator String(); operator const String() const;
					Character& operator=( const char* );
				};
#	pragma mark -

			inline Struct::Struct( int n_descr, PBASE4_t p )
				:	pBase( p )
				,	num_descriptors( n_descr )
				//, Iarr(( Fields * )( this + 1 ))
				{
					Iarr = ( Fields * )( this + 1 );
				//	std::cout << "3: " << num_descriptors << std::endl;
				//	setFieldLens();
				}

			inline void Struct::setFieldLens() const
				{
				//	std::cout << "2: " << num_descriptors << std::endl;
					int shift = 0;
					for (int i = 0; i < num_descriptors; i ++)
						{
					//		std::cout << "shift: " << shift << std::endl;
							pBase->db_struct[ i ].setFieldPos( shift );
							shift += pBase->db_struct[ i ].getLen();
						}
				}

			inline Fields& Struct::operator[]( int i ) { return Iarr[i]; }
			inline const Fields& Struct::operator[]( int i ) const { return Iarr[i]; }

#	pragma mark -

			inline Fields::Fields( PBASE4_t ptr, char t, const char *n, unsigned char l, unsigned char d )
				:	pBase( ptr )
				,	type( t )
				,	len( l )
				,	dec( d )
				,	ptr( 0 )
				,	field_pos( 0 )
				{
					Reset( ptr, t, n, l, d );
				}

			inline void Fields::Reset( PBASE4_t ptr, char t, const char *n, unsigned char l, unsigned char d )
				{
					for (int i = 0; i < 11; i ++) { name[ i ] = 0; }
					for (int i = 0; i < 14; i ++) { reseirved[ i ] = 0; }
					int i=0; const char *p = n; while( *p && i<11 ) name[ i++ ] = *p++;
				}

			inline void Fields::Init( PBASE4_t ptr, char t, const char *n, unsigned char l, unsigned char d )
				{
					pBase = ptr;
					type = t;
					len = l;
					dec = d;
					ptr = 0;
					field_pos = 0;
					Reset( ptr, t, n, l, d );
				}

			inline PBASE4_t Fields::get_pBase() const { return pBase; }
			inline const char *Fields::getName() const { return &name[0]; }
			inline int Fields::getLen() const { return len; }
			inline int Fields::getDec() const { return dec; }
			inline void Fields::setFieldPos( unsigned char pos ) { field_pos = pos; }
			inline int Fields::getFieldPos() const { return field_pos; }

#	pragma mark -

			inline void Date::get_date()
				{
					tzset();
					time_t sec;
					time( &sec );
					tm const *t = localtime( &sec );
					year =   (unsigned char)(t->tm_year % 100);
					mounth = (unsigned char) t->tm_mon + 1;
					day =    (unsigned char) t->tm_mday;
				}

			inline Date::Date() { get_date(); }

#	pragma mark -
		/*
			Header::Header()
				:	n_ver (3)
				,	num_recs (0)
				,	head_len (33)
				,	rec_len (1)
				{	for (int i = 0; i < 20; i ++) reseirved[i] = 0;	 }
		*/
			inline Header::Header( int num_descr, unsigned short reclen )
				:	n_ver (3)
				,	num_recs (0)
				,	head_len (32 * (num_descr + 1) + 1)
				,	rec_len (reclen + 1)
				{	for (int i = 0; i < 20; i ++) reseirved[i] = 0;  }

			inline void Header::ReInit( int num_descr, unsigned short reclen )
				{
					n_ver = 3;
					num_recs = 0;
					head_len = 32 * (num_descr + 1) + 1;
					rec_len = reclen + 1;
					for (int i = 0; i < 20; i ++) reseirved[i] = 0;
				}

			inline unsigned short Header::getFullHeadLen() const { return head_len; }
			inline unsigned short Header::getRecLength() const { return rec_len; }
			inline unsigned long Header::getNumRecords() const { return num_recs; }
			inline void Header::setNumRecords( unsigned long n_recs ) { num_recs = n_recs; }

#	pragma mark -
#	pragma mark DBaseIV::

			template< class struct_type >inline DBaseIV< struct_type >::DBaseIV( const char *filename )
					:	Files( filename, File::Access::DataBase )
					,	db_struct( get_num_descriptors(), (PBASE4_t)this )
					,	pHeader( 0 )
					{
						db_struct.setFieldLens();
						int reclen = 0;
						for (int i = 0; i < get_num_descriptors(); i ++) { reclen += db_struct[i].getLen(); }
						pHeader = new Header( get_num_descriptors(), reclen );
						if (! filelength() ) { Create(); } else Open();
						// std::cout << "1: " << get_num_descriptors() << std::endl;
					}

			template< class struct_type >inline DBaseIV< struct_type >::~DBaseIV()
					{
						// close(); // doIt in ~Files().
						if (pHeader) delete pHeader;
					}

			template< class struct_type >inline void DBaseIV< struct_type >::Truncate()
					{
						Files::lseek( 0, SEEK_SET );
						db_struct.setFieldLens();
						int reclen = 0;
						for (int i = 0; i < get_num_descriptors(); i ++) { reclen += db_struct[i].getLen(); }
						pHeader->ReInit( get_num_descriptors(), reclen );
						Create();
					}

			template< class struct_type >inline void DBaseIV< struct_type >::Create()
					{
					//	open( O_BINARY | O_CREAT | O_RDWR/*| O_APPEND*/, S_IREAD | S_IWRITE );
						write (pHeader, sizeof (DB::Header) );
						for (int i = 0; i < get_num_descriptors(); i ++) { write( &db_struct[i], sizeof(char)*32 ); }
						char end_stat;
						end_stat = CR; write( &end_stat, 1 );
						end_stat = LF; write( &end_stat, 1 );
					}

			template< class struct_type >inline void DBaseIV< struct_type >::Open()
					{
					//	open( O_BINARY | O_RDWR );
						read( pHeader, sizeof( DB::Header ));
					}

			template< class struct_type >inline void DBaseIV< struct_type >::append_blank()
					{
					//	lseek( sizeof( DB::Header ) + pHeader->getRecLength() * pHeader->getNumRecords(), SEEK_SET );
						lseek( -1, SEEK_END ); unsigned char *buf = new unsigned char[ pHeader->getRecLength() ];
						for (int i = 0; i < pHeader->getRecLength(); i ++) { buf[i] = 0; }
						write( buf, pHeader->getRecLength()); delete[] buf;
						pHeader->setNumRecords( pHeader->getNumRecords() + 1 );
						lseek (0, SEEK_SET); write (pHeader, sizeof (DB::Header) );
						lseek (0, SEEK_END);
						char end_stat = LF; write( &end_stat, 1 );
					}

			template< class struct_type >inline void DBaseIV< struct_type >::set_rec_pos( int n_rec )
					{
						int rec_pos = pHeader->getFullHeadLen() + 1 + n_rec * pHeader->getRecLength();
						lseek (rec_pos, SEEK_SET);
						// std::cout << std::endl << "0x" << hex << rec_pos << dec;
					}

			template< class struct_type >inline void DBaseIV< struct_type >::on_get_value( char *pbuf, int len, int field_shift )
					{
						// std::cout << field_shift << std::endl;
						lseek (field_shift, SEEK_CUR);
						read (pbuf, len);
					}

			template< class struct_type >inline void DBaseIV< struct_type >::on_set_value( const char *pbuf, int len, int field_shift )
					{
						lseek (field_shift, SEEK_CUR);
						write ((void*)pbuf, len);
					}

			template< class struct_type >inline void DBaseIV< struct_type >::test()
					{
						for (int i = 0; i < get_num_descriptors(); i ++)
							{
								std::cout << std::endl << db_struct[ i ].get_pBase();
								std::cout << std::endl << db_struct[ i ].getName();
								std::cout << std::endl << db_struct[ i ].getLen();
								std::cout << std::endl << db_struct[ i ].getDec();
							}
						std::cout << std::endl<< hex << this;
					}

			template< class struct_type >inline int DBaseIV< struct_type >::get_num_descriptors() const { return (sizeof( struct_type ) - sizeof( Struct )) / sizeof( DB::Fields ); }

#	pragma mark Starting from 2012+

			template< class struct_type >inline Fields& DBaseIV< struct_type >::operator[]( int pos ) { return db_struct[ pos ]; }
			template< class struct_type >inline const Fields& DBaseIV< struct_type >::operator[]( int pos ) const { return db_struct[ pos ]; }
			template< class struct_type >inline int DBaseIV< struct_type >::records() const { return pHeader->getNumRecords(); }
			template< class struct_type >inline class DBaseIV< struct_type >* DBaseIV< struct_type >::iterator() { return this; };

#	pragma mark -

			template< class struct_type >inline DBaseIV< struct_type >::Record::Record( DBaseIV* ptr ): pBase (ptr), n_rec (0) {}
			template< class struct_type >inline struct_type* DBaseIV< struct_type >::Record::operator->() { pBase->set_rec_pos( n_rec ); return &pBase->db_struct; }
			template< class struct_type >inline typename DBaseIV< struct_type >::Record	DBaseIV< struct_type >::Record::operator++() { if (++n_rec >= pBase->pHeader->getNumRecords()) --n_rec; return *this; }
			template< class struct_type >inline typename DBaseIV< struct_type >::Record	DBaseIV< struct_type >::Record::operator--() { if (--n_rec < 0) ++n_rec; return *this; }
			template< class struct_type >inline typename DBaseIV< struct_type >::Record	DBaseIV< struct_type >::Record::operator++( int ) { Record tmp = *this; if (++n_rec >= pBase->pHeader->getNumRecords()) --n_rec; return tmp; }
			template< class struct_type >inline typename DBaseIV< struct_type >::Record	DBaseIV< struct_type >::Record::operator--( int ) { Record tmp = *this; if (--n_rec < 0) ++n_rec; return tmp; }
			template< class struct_type >inline bool DBaseIV< struct_type >::Record::operator==( Record const &r ) const { return( n_rec == r.n_rec ); }
			template< class struct_type >inline bool DBaseIV< struct_type >::Record::operator!=( Record const &r ) const { return( n_rec != r.n_rec ); }

#	pragma mark -

			inline Numeric::Numeric( PBASE4_t ptr, const char *name, unsigned char len, unsigned char dec )
				: Fields( ptr, 'N', name, len, dec ) {}

			inline Numeric& Numeric::operator=( double num )
				{
					int len = getLen();
					int dec = getDec();
					char* buf = new char[ len + 1 ];
					for( int i=0 ; i<len ; i++ ) { buf[ i ] = blank_space; }
					buf[ len ] = 0;
					num2str(buf, num, dec);
					pBase->on_set_value( buf, len, getFieldPos() );
					delete[] buf;
					return *this;
				}

			inline Numeric::operator double()
				{
					int len = getLen();
					char *pbuf = new char[ len + 1 ];

					pbuf[ len ] = 0;
					pBase->on_get_value( &pbuf[ 0 ], len, getFieldPos() );

					char *p = &pbuf[ 0 ];
					int i = 0;

					while((*p == blank_space )&&( i<len ))
						{
							++p;
							++i;
						};

					double rc = atof( p );

					delete[] pbuf;
					return rc;
				}

			inline Numeric::operator const double() const
				{
					int len = getLen();
					char *pbuf = new char[ len + 1 ];

					pbuf[ len ] = 0;
					pBase->on_get_value( &pbuf[ 0 ], len, getFieldPos() );

					char *p = &pbuf[ 0 ];
					int i = 0;

					while((*p == blank_space )&&( i<len ))
						{
							++p;
							++i;
						};

					double rc = atof( p );

					delete[] pbuf;
					return rc;
				}
#	pragma mark -

			inline Character::Character( PBASE4_t ptr, const char *name, unsigned char len/*, unsigned char dec = 0*/ )
				:
					Fields( ptr, 'C', name, len, 0 )
				{
				}

			inline Character& Character::operator=( const char* s )
				{
					int len = getLen();
					char *pbuf = new char[ len ];
					int i = 0;
					// for( int i=0 ; i<len ; i++ )
					while( i++ < len ) {
						pbuf[ i ] = blank_space;
					}
					i=0;
					const char *p = s;

					while( *p && i<len )
						pbuf[ i++ ] = *p++;

					pBase->on_set_value( &pbuf[ 0 ], len, getFieldPos() );

					delete[] pbuf;
					return *this;
				}

			inline Character::operator String()
				{
					int len = getLen();
					char *pbuf = new char[ len + 1 ];
					pbuf[ len ] = 0;
					pBase->on_get_value( &pbuf[ 0 ], len, getFieldPos() );
					String rc = &pbuf[ 0 ];
					delete[] pbuf;
					return( rc );
				}

			inline Character::operator const String() const
				{
					int len = getLen();
					char *pbuf = new char[ len + 1 ];
					pbuf[ len ] = 0;
					pBase->on_get_value( &pbuf[ 0 ], len, getFieldPos() );
					String rc = &pbuf[ 0 ];
					delete[] pbuf;
					return( rc );
				}
		}// DB::

#	define __DBASE4_H__
#	endif
