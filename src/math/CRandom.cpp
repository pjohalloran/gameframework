//========================================================================
// CMath_Random.cpp -  a useful random number generator class
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
// @file CRandom.cpp
// @author Michael L. McShaffry (edited by PJ O Halloran)
// @date 08/07/2010
//
// File contains the implementation for the CRandom class.  
// This class was extracted from the Game Coding Complete 3 math code.
// Originally written by Mike McShaffry et al., author(s) of Game
// Coding Complete and games programmer(s).
//
// Please see the header files for the details of the few changes I
// have made.
//
// /////////////////////////////////////////////////////////////////

// External headers
#include <time.h>

// Project headers
#include "CommonMath.h"
#include "CRandom.h"
#include "GameBase.h"

// Namespace Declarations


namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	CRandom::CRandom(void) : m_rSeed(1), m_rSeedSp(0), m_mt(GCC_NEW U64[CMATH_N]), m_mti(CMATH_N+1)
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	CRandom::~CRandom()
	{
	}
		
	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	U32 CRandom::Random(U32 n)
	{
		U64 y;										// 
		static U64 mag01[2]={0x0, CMATH_MATRIX_A};	// 

		if(n == 0)
		{
			return(0);
		}

		/* mag01[x] = x * MATRIX_A  for x=0,1 */

		 /* generate N words at one time */
		if (m_mti >= CMATH_N)
		{
			I32 kk;

			/* if sgenrand() has not been called, */
			if (m_mti == CMATH_N+1)
			{
				/* a default initial seed is used   */
				SetRandomSeed(4357);
			}

			for (kk = 0; kk < CMATH_N - CMATH_M; kk++)
			{
				y = (m_mt[kk]&CMATH_UPPER_MASK)|(m_mt[kk+1]&CMATH_LOWER_MASK);
				m_mt[kk] = m_mt[kk+CMATH_M] ^ (y >> 1) ^ mag01[y & 0x1];
			}
			for (;kk < CMATH_N - 1; kk++)
			{
				y = (m_mt[kk]&CMATH_UPPER_MASK)|(m_mt[kk+1]&CMATH_LOWER_MASK);
				m_mt[kk] = m_mt[kk+(CMATH_M-CMATH_N)] ^ (y >> 1) ^ mag01[y & 0x1];
			}
			y = (m_mt[CMATH_N-1]&CMATH_UPPER_MASK)|(m_mt[0]&CMATH_LOWER_MASK);
			m_mt[CMATH_N-1] = m_mt[CMATH_M-1] ^ (y >> 1) ^ mag01[y & 0x1];

			m_mti = 0;
		}
	  
		y = m_mt[m_mti++];
		y ^= CMATH_TEMPERING_SHIFT_U(y);
		y ^= CMATH_TEMPERING_SHIFT_S(y) & CMATH_TEMPERING_MASK_B;
		y ^= CMATH_TEMPERING_SHIFT_T(y) & CMATH_TEMPERING_MASK_C;
		y ^= CMATH_TEMPERING_SHIFT_L(y);

		// ET - old engine added one to the result.
		// We almost NEVER wanted to use this function
		// like this.  So, removed the +1 to return a 
		// range from 0 to n (not including n).
		return (y%n);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	F32 CRandom::Random()
	{
		I32 r = (F32)Random(0xffffffff);
		//I32 r = static_cast<F32>(Random(0xffffffff));

		F32 divisor = static_cast<F32>(0xffffffff);
		return (r / divisor) + 0.5f;
		//return (fr / divisor) + 0.5f;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	F32 CRandom::Random(const F32 min, const F32 max)
	{
		if(min >= max)
		{
			// bad input...
			return (min);
		}

		// return F32 in [min, max] range.
		//F32 f = Random();
		F32 f = (rand() % 10000) * 0.0001f; 
		return (f * (max - min)) + min;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void CRandom::SetRandomSeed(U32 n)
	{
		/* setting initial seeds to m_mt[N] using       */
		/* the generator Line 25 of Table 1 in          */
		/* [KNUTH 1981, The Art of Computer Programming */
		/*    Vol. 2 (2nd Ed.), pp102]                  */
		m_mt[0]= n & 0xffffffff;
		for (m_mti=1; m_mti < CMATH_N; m_mti++)
		{
			m_mt[m_mti] = (69069 * m_mt[m_mti-1]) & 0xffffffff;
		}

		m_rSeed = n;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	U32 CRandom::GetRandomSeed(void)
	{
		return (m_rSeed);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void CRandom::Randomize(void)
	{
		SetRandomSeed(static_cast<U32>(time(NULL)));
	}
}