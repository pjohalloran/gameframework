#pragma once
#ifndef __C_RANDOM_H
#define __C_RANDOM_H

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

#include <boost/scoped_array.hpp>

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
		boost::scoped_array<U64> m_mt;			///< the array for the state vector (size=CMATH_N)
		I32 m_mti;								///< m_mti==N+1 means m_mt[N] is not initialized */

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
