//========================================================================
// HashedString.cpp : Defines some useful string hash functions
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
// Please see the associated header file for details on the changes
// I have made.
//
// /////////////////////////////////////////////////////////////////

#include "HashedString.h"

namespace GameHalloran
{
	void *HashedString::hash_name(char const *pIdentStr)
	{
		// Relatively simple hash of arbitrary text string into a
		// 32-bit identifier Output value is
		// input-valid-deterministic, but no guarantees are made
		// about the uniqueness of the output per-input
		//
		// Input value is treated as lower-case to cut down on false
		// separations cause by human mistypes. Sure, it could be
		// construed as a programming error to mix up your cases, and
		// it cuts down on permutations, but in Real World Usage
		// making this text case-sensitive will likely just lead to
		// Pain and Suffering.
		//
		// This code lossely based upon the adler32 checksum by Mark
		// Adler and published as part of the zlib compression
		// library sources.

		// largest prime smaller than 65536
		U64 BASE = 65521L;

		// NMAX is the largest n such that 255n(n+1)/2 +
		// (n+1)(BASE-1) <= 2^32-1
		U64 NMAX = 5552;

		#define DO1(buf,i)  {s1 += tolower(buf[i]); s2 += s1;}
		#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
		#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
		#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
		#define DO16(buf)   DO8(buf,0); DO8(buf,8);

		if (pIdentStr == NULL)
			return NULL;

		// Missing reference to kpWildcardEventType string here, compare with pg276 of GCC3.

		U64 s1 = 0;
		U64 s2 = 0;

		for ( size_t len = strlen( pIdentStr ); len > 0 ; )
		{
			U64 k = len < NMAX ? len : NMAX;

			len -= k;

			while (k >= 16)
			{
				DO16(pIdentStr);
				pIdentStr += 16;
				k -= 16;
			}
			
			if (k != 0) do
			{
				s1 += tolower( *pIdentStr++ );
				s2 += s1;
			} while (--k);
			
			s1 %= BASE;
			s2 %= BASE;
		}

		return reinterpret_cast<void *>( (s2 << 16) | s1 );
		#undef DO1
		#undef DO2
		#undef DO4
		#undef DO8
		#undef DO16
	}

}
