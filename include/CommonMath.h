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
// @file CommonMath.h
// @author Michael L. McShaffry (edited by PJ O Halloran)
// @date 08/07/2010
//
// File contains the header for commonly used defines for the CRandom
// and CMath classes.
// 
// This class was extracted from the Game Coding Complete 3 math code.
// Originally written by Mike McShaffry et al., author(s) of Game
// Coding Complete and games programmer.
//
// I have located common datatypes and defines here also for my 3D math
// library.
//
// /////////////////////////////////////////////////////////////////

#ifndef __COMMON_MATH_H
#define __COMMON_MATH_H

// External Headers
#include <cmath>


// Project Headers
#include "GameTypes.h"


namespace GameHalloran
{

	// Some common defines for GCC3 math code (used only for the random generator i am using as i decided to write and use my own math lib...)

	/* Period parameters */  
	#define CMATH_N 624
	#define CMATH_M 397
	#define CMATH_MATRIX_A 0x9908b0df   /* constant vector a */
	#define CMATH_UPPER_MASK 0x80000000 /* most significant w-r bits */
	#define CMATH_LOWER_MASK 0x7fffffff /* least significant r bits */

	/* Tempering parameters */   
	#define CMATH_TEMPERING_MASK_B 0x9d2c5680
	#define CMATH_TEMPERING_MASK_C 0xefc60000
	#define CMATH_TEMPERING_SHIFT_U(y)  (y >> 11)
	#define CMATH_TEMPERING_SHIFT_S(y)  (y << 7)
	#define CMATH_TEMPERING_SHIFT_T(y)  (y << 15)
	#define CMATH_TEMPERING_SHIFT_L(y)  (y >> 18)

	// Return codes for the line intersect functions in CMath.
	#define DONT_INTERSECT 0
	#define DO_INTERSECT 1
	#define COLLINEAR 2

	/**************************************************************
	 *                                                            *
	 *    NOTE:  The following macro to determine if two numbers  *
	 *    have the same sign, is for 2's complement number        *
	 *    representation.  It will need to be modified for other  *
	 *    number systems.                                         *
	 *                                                            *
	 **************************************************************/
	#define SAME_SIGNS( a, b )	\
			(((I64) ((U64) a ^ (U64) b)) >= 0 )
	// END GCC3 math common defines.

	#define ERROR_DELTA 0.000001
	#define FLOAT_ERROR_DELTA 0.000001f

	// /////////////////////////////////////////////////////////////////
	// float version.
	//
	// Compare two floating point values and see if the difference
	// between them is less than some predefined very small float
	// to account for floating point error.
	//
	// /////////////////////////////////////////////////////////////////
	inline bool FloatCmp(const F32 a, const F32 b, const F32 epsilon = FLOAT_ERROR_DELTA)
	{
		return (std::abs(a - b) <= epsilon);
	};

	// /////////////////////////////////////////////////////////////////
	// F64 version.
	//
	// Compare two floating point values and see if the difference
	// between them is less than some predefined very small float
	// to account for floating point error.
	//
	// /////////////////////////////////////////////////////////////////
	inline bool DoubleCmp(const F64 a, const F64 b, const F64 epsilon = ERROR_DELTA)
	{
		return (std::abs(a - b) <= epsilon);
	};

	// NB. The following constants are taken directly from the OpenGL superbible
	// GLTools library as they will be handy for my 3D math library.

	///////////////////////////////////////////////////////////////////////////////
	// Useful constants
	#define M3D_PI (3.14159265358979323846)
	#define M3D_2PI (2.0 * M3D_PI)
	#define M3D_PI_DIV_180 (0.017453292519943296)
	#define M3D_INV_PI_DIV_180 (57.2957795130823229)

	///////////////////////////////////////////////////////////////////////////////
	// Useful shortcuts and macros
	// Radians are king... but we need a way to swap back and forth for programmers and presentation.
	// Leaving these as Macros instead of inline functions, causes constants
	// to be evaluated at compile time instead of run time, e.g. m3dDegToRad(90.0)
	#define m3dDegToRad(x)	((x)*M3D_PI_DIV_180)
	#define m3dRadToDeg(x)	((x)*M3D_INV_PI_DIV_180)

	// Hour angles
	#define m3dHrToDeg(x)	((x) * (1.0 / 15.0))
	#define m3dHrToRad(x)	m3dDegToRad(m3dHrToDeg(x))

	#define m3dDegToHr(x)	((x) * 15.0))
	#define m3dRadToHr(x)	m3dDegToHr(m3dRadToDeg(x))

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Compare floats and doubles... 
	inline bool m3dCloseEnough(const F32 fCandidate, const F32 fCompare, const F32 fEpsilon)
    {
		return (fabs(fCandidate - fCompare) < fEpsilon);
    };
	inline bool m3dCloseEnough(const F64 dCandidate, const F64 dCompare, const F64 dEpsilon)
    {
		return (fabs(dCandidate - dCompare) < dEpsilon);
	};

	// /////////////////////////////////////////////////////////////////
	// Clamps a basic or object type between and maximum and minimum
	// value.
	//
	// Commonly used in graphics programming to keep a F32 value
	// between 0.0 and 1.0.
	//
	// Objects must have the operator<, operator> and operator=
	// functions implemented to be compatible with this method.
	//
	// @param valRef The value to clamp.
	// @param min Minimum value.
	// @param max Maximum value.
	//
	// @return bool True if the value was clamped and false if not.
	//
	// /////////////////////////////////////////////////////////////////
	template<typename T>
		bool Clamp(T &valRef, const T &min, const T &max)
	{
		bool clamp = false;

		if(valRef < min)
		{
			clamp = true;
			valRef = min;
		}
		else if(valRef > max)
		{
			clamp = true;
			valRef = max;
		}

		return (clamp);
	};

	// /////////////////////////////////////////////////////////////////
	// Performs a linear interpolation.  Gets a value between a min and
	// max value which depends on the interpolated, clamped value
	// supplied.
	//
	// /////////////////////////////////////////////////////////////////
	inline F32 InterpolateFloat(const F32 interpolatedValue, const F32 minValue, const F32 maxValue)
	{
		return (minValue + (interpolatedValue * (maxValue - minValue)));
	};

	// /////////////////////////////////////////////////////////////////
	// Get the maximum of two values.
	//
	// Implemented for the microsoft platform as std::max and std::min
	// are not available as they clash with microsoft headers!
	//
	// @param a First object.
	// @param b Second object
	//
	// @return T The greater object.  In the case both are equivalent
	//				either being returned is correct.
	//
	// /////////////////////////////////////////////////////////////////
	template<typename T>
		T CmMax(const T &a, const T &b)
	{
		if(a > b)
		{
			return (a);
		}

		return (b);
	};

	// /////////////////////////////////////////////////////////////////
	// Get the minimum of two values.
	//
	// Implemented for the microsoft platform as std::max and std::min
	// are not available as they clash with microsoft headers!
	//
	// @param a First object.
	// @param b Second object
	//
	// @return T The greater object.  In the case both are equivalent
	//				either being returned is correct.
	//
	// /////////////////////////////////////////////////////////////////
	template<typename T>
		T CmMin(const T &a, const T &b)
	{
		if(a < b)
		{
			return (a);
		}

		return (b);
	};

}

#endif
