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

#include <time.h>

#include "CommonMath.h"
#include "CRandom.h"
#include "GameBase.h"

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