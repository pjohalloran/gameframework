//========================================================================
// CMath.h -  useful and fast math functions and approximations
//
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
// The source code is managed and maintained through Google Code: http://gamecode3.googlecode.com/svn/trunk/
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
// @file CRandom.h
// @author Michael L. McShaffry (edited by PJ O Halloran)
// @date 08/07/2010
//
// File contains the header for the CRandom class.  
// This class was extracted from the Game Coding Complete 3 math code.
// Originally written by Mike McShaffry et al., author(s) of Game
// Coding Complete and games programmer(s).
//
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// I have added/modified the following functionality:
// - The constants this class and CMath depend on were moved to a common
//		header file.
// - TODO... put more here...
//
// /////////////////////////////////////////////////////////////////

#ifndef __C_RANDOM_H
#define __C_RANDOM_H

// External Headers
#include <boost/scoped_array.hpp>

// Project Headers
#include "GameTypes.h"
#include "CommonMath.h"

namespace GameHalloran
{
	// /////////////////////////////////////////////////////////////////
	// @class CRandom
	// @author Takuji Nishimura, Makoto Matsumoto and Michael L. McShaffry
	//				(edited by PJ O Halloran).
	//
	// This is a Mersenne Twister pseudorandom number generator
	// originally developed by Takuji Nishimura and Makoto Matsumoto.
	//
	// /////////////////////////////////////////////////////////////////
	class CRandom
	{
	private:
		
		U32 m_rSeed;							///< The current random number seed.
		U32 m_rSeedSp;							///< 
		boost::scoped_array<U64> m_mt;		///< the array for the state vector (size=CMATH_N)
		I32 m_mti;										///< m_mti==N+1 means m_mt[N] is not initialized */

	public:


		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		CRandom(void);

		// /////////////////////////////////////////////////////////////////
		// Destrucutor.
		//
		// /////////////////////////////////////////////////////////////////
		~CRandom();

		// /////////////////////////////////////////////////////////////////
		// Generate a random number between 0 and n-1.
		//
		// @param n The maximum value (-1) of the random number to generate.
		//
		// /////////////////////////////////////////////////////////////////
		U32 Random(U32 n);

		// /////////////////////////////////////////////////////////////////
		// Return a random float between 0.0f and 1.0f.
		//
		// /////////////////////////////////////////////////////////////////
		F32 Random();

		// /////////////////////////////////////////////////////////////////
		// Return a random float between min and max.
		//
		// /////////////////////////////////////////////////////////////////
		F32 Random(const F32 min, const F32 max);

		// /////////////////////////////////////////////////////////////////
		// Set the random seed number used to generate all the random
		// numbers from this point onwards.
		//
		// @param newSeed The new random number seed to be used.
		//
		// /////////////////////////////////////////////////////////////////
		void SetRandomSeed(U32 newSeed);

		// /////////////////////////////////////////////////////////////////
		// Get the value of the current seed being used to generate the 
		// random numbers.
		//
		// /////////////////////////////////////////////////////////////////
		U32 GetRandomSeed(void);

		// /////////////////////////////////////////////////////////////////
		// Set the seed to some random number.
		//
		// /////////////////////////////////////////////////////////////////
		void Randomize(void);
	};
}

#endif
