////////////////////////////////////////////////////////////////////////////////
///  objects.h
///  the
///
///  Created by Atz on 17.11.10.
///  Copyright 2010 The(c)ompany. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////

// please include "the.h" instead of this file.

#ifndef __THE_KEYS_H__

namespace The
	{
		class Keys: public Modifiers
			{
				int m_event; // press/release etc.
				int m_code; public:
				inline bool operator ==( const Keys& rhs ) const
					{
						const bool ignoreMods = this->modifiers() == ACTIVE_IGNORE || rhs.modifiers() == ACTIVE_IGNORE;
					//	const bool any = (this->modifiers() == ACTIVE_ANY) || (rhs.modifiers() == ACTIVE_ANY);
						const bool mods = (ignoreMods /*|| any*/ ? true : this->modifiers() == rhs.modifiers());
					//	std::cout << this->modifiers() << '=' << rhs.modifiers() << '>' << mods << '\t'
					//		<< (m_code == rhs.m_code) << '\t' << (m_event == rhs.m_event) << std::endl;
						return mods && (m_code == rhs.m_code) && (m_event == rhs.m_event);
					}
				inline bool operator !=( const Keys& rhs ) const { return !operator ==( rhs ); }
				inline int event() const { return m_event; }
				inline int key() const { return m_code; }
				inline Keys( const Events_t e, int k, int m = 0 ): Modifiers( m ), m_event( e ), m_code( k ) {}
				protected: inline Keys( int m, const Events_t e ): Modifiers( m ), m_event( e ), m_code( -1 ) {}
		};
}// The::

#define __THE_KEYS_H__
#endif
