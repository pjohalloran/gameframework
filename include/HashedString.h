//========================================================================
// HashedString.h : Defines a class to hash strings.
//
// Part of the GameCode3 Application
//
// GameCode3 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 3rd Edition" by Mike McShaffry, published by
// Charles River Media. ISBN-10: 1-58450-680-6   ISBN-13: 978-1-58450-680-5
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the author a favor and buy a new copy of the book - there are 
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: 
//    http://www.amazon.com/gp/product/1584506806?ie=UTF8&tag=gamecodecompl-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=1584506806
//
// There's a companion web site at http://www.mcshaffry.com/GameCode/
// 
// The source code is managed and maintained through Google Code:
// http://gamecode3.googlecode.com/svn/trunk/
//
// (c) Copyright 2009 Michael L. McShaffry
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License v2
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================

// /////////////////////////////////////////////////////////////////
// @file HashedString.h
// @author Mike McShaffry
// @date 13/07/2010
//
// File contains the header for the HashedString class.
//
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry, author of Game
// Coding Complete and games programmer.
// 
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// I have added/modified the following functionality:
// - I added the class under the GameHalloran namespace.
// - I removed all the string utility functions in this class as
//		they are unnessecary for my work.
// - I have made note that there is a difference between the code
//		and the book. On pg 276 the book has the wildcard reference in
//		hash_string():
//		if(strcmp(pIdentStr, kpWildcardEventType) == 0)
//			return 0;
//		The code does not have this.  Must investigate if it has been
//		placed elsewhere...
//
// /////////////////////////////////////////////////////////////////

#ifndef __HASHED_STRING_H
#define __HASHED_STRING_H

// External Headers
#include <string>


// Project Headers
#include "GameTypes.h"


namespace GameHalloran
{

	#pragma warning(push)
	#pragma warning(disable : 4311)

	// /////////////////////////////////////////////////////////////////
	// @class HashedString
	// @author Mike McShaffry
	//
	// A hashed string.  It retains the initial (ANSI) string in addition
	// to the hash value for easy reference.
	//
	// /////////////////////////////////////////////////////////////////
	class HashedString
	{
	private:

		// note: m_ident is stored as a void* not an int, so that in
		// the debugger it will show up as hex-values instead of
		// integer values. This is a bit more representative of what
		// we're doing here and makes it easy to allow external code
		// to assign event types as desired.
		void *m_ident;							///< The hashed string (in hex characters).
		std::string m_identStr;					///< The original string.

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		explicit HashedString(char const * const pIdentString)
			: m_ident(hash_name(pIdentString)), m_identStr(pIdentString)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Get the hash value of the string.
		//
		// /////////////////////////////////////////////////////////////////
		U64 getHashValue(void) const
		{
			return (reinterpret_cast<U64>(m_ident));
		}

		// /////////////////////////////////////////////////////////////////
		// Get the original string.
		//
		// /////////////////////////////////////////////////////////////////
		const std::string &getStr() const
		{
			return (m_identStr);
		}

		// /////////////////////////////////////////////////////////////////
		// Hash the string.
		//
		// @param pIdentStr The string that we wish to create a hash of.
		//
		// /////////////////////////////////////////////////////////////////
		static void * hash_name(char const *pIdentStr);

		// /////////////////////////////////////////////////////////////////
		// Check if this hash is less than the hash on the rhs.
		//
		// /////////////////////////////////////////////////////////////////
		bool operator< (HashedString const &rhs) const
		{
			bool r = (getHashValue() < rhs.getHashValue());
			return (r);
		}

		// /////////////////////////////////////////////////////////////////
		// Check if this hash is equal to the hash on the rhs.
		// Please note that this does not guarantee that the original strings 
		// are different.  The algorithm used means that hash collisions may
		// occur.
		//
		// /////////////////////////////////////////////////////////////////
		bool operator== (HashedString const &rhs) const
		{
			bool r = (getHashValue() == rhs.getHashValue());
			return (r);
		}
	};

	//Remove the warning for warning #4311...
	#pragma warning(pop)

}

#endif
