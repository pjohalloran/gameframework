////========================================================================
//// CMath.h -  useful and fast math functions and approximations
////
////
//// Part of the GameCode3 Application
////
//// GameCode3 is the sample application that encapsulates much of the source code
//// discussed in "Game Coding Complete - 3rd Edition" by Mike McShaffry, published by
//// Charles River Media. ISBN-10: 1-58450-680-6   ISBN-13: 978-1-58450-680-5
////
//// If this source code has found it's way to you, and you think it has helped you
//// in any way, do the author a favor and buy a new copy of the book - there are 
//// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
//// by clicking here: 
////    http://www.amazon.com/gp/product/1584506806?ie=UTF8&tag=gamecodecompl-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=1584506806
////
//// There's a companion web site at http://www.mcshaffry.com/GameCode/
//// 
//// The source code is managed and maintained through Google Code:
//// http://gamecode3.googlecode.com/svn/trunk/
////
//// (c) Copyright 2009 Michael L. McShaffry
////
//// This program is free software; you can redistribute it and/or
//// modify it under the terms of the GNU General Public License v2
//// as published by the Free Software Foundation.
////
//// This program is distributed in the hope that it will be useful,
//// but WITHOUT ANY WARRANTY; without even the implied warranty of
//// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//// GNU General Public License for more details.
////
//// You should have received a copy of the GNU General Public License
//// along with this program; if not, write to the Free Software
//// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
////
////========================================================================
//
//
//// /////////////////////////////////////////////////////////////////
//// @file CMath.h
//// @author Michael L. McShaffry (edited by PJ O Halloran)
//// @date 08/07/2010
////
//// File contains the header for the CMath class and a few other utility
//// functions.
////
//// This class was extracted from the Game Coding Complete 3 math code.
//// Originally written by Mike McShaffry author of Game
//// Coding Complete and games programmer.
////
//// I have cleaned it up a little with better comments etc. to fit
//// in with the rest of my code.
////
//// I have added/modified the following functionality:
//// - The constants this class and CMath depend on were moved to a common
////		header file.
//// - I added an explicit constructor and destructor to CMath.
//// - Using my own Point class instead of the struct.
//// - Using custom min and max functions as std::min and std::max does not
////		seem to compile under VS2008 express.
//// - As of the 08/07/2010, commenting out all code that uses ATL and directx
////		as a) VS2008 express does not have ATL and b) im using OpenGL. Will
////		get this functionality in over time when i sort out these issues.
//// - Put the class and functions in under my own namespace.
//// - Using boost optional instead of Mike McShafferys version.  I think defining
////		a second version is wrong when boost::optional works the same and is fine.
//// - Moved various comments from .cpp file to the header where they belong.
//// - TODO... put more here...
////
//// /////////////////////////////////////////////////////////////////
//
//#ifndef __C_MATH_H
//#define __C_MATH_H
//
//// External Headers
//#include <functional>
//#include <vector>
//
////#define WIN32_LEAN_AND_MEAN
////#include <windows.h>
//
//#include <boost/scoped_array.hpp>
//#include <boost/optional.hpp>
//
//// Project Headers
//#include "CommonMath.h"
//#include "CRandom.h"
//#include "Point.h"
//
//namespace GameHalloran
//{
//
//	// Define the Poly typedef.
//	typedef std::vector<Point> Poly;
//
//	// /////////////////////////////////////////////////////////////////
//	// @class CMath
//	// @author Michael L. McShaffry (edited by PJ O Halloran).
//	//
//	// This is a class containing math functions and approximations useful
//	// for commonly encountered math problems in game development.
//	//
//	// /////////////////////////////////////////////////////////////////
//	class CMath
//	{
//	private:
//
//		static const unsigned short angle_to_sin[90];				///< A lookup table for getting the sin of an angle.
//
//	public:
//
//		static CRandom random;										///< The random number generator.
//
//		// /////////////////////////////////////////////////////////////////
//		// Constructor.
//		// 
//		// /////////////////////////////////////////////////////////////////
//		CMath();
//
//		// /////////////////////////////////////////////////////////////////
//		// Destructor.
//		// 
//		// /////////////////////////////////////////////////////////////////
//		~CMath();
//
//		// /////////////////////////////////////////////////////////////////
//		//
//		// /////////////////////////////////////////////////////////////////
//		static int Cos(short angle, int length);
//
//		// /////////////////////////////////////////////////////////////////
//		//
//		// /////////////////////////////////////////////////////////////////
//		static int Sin(short angle, int length);
//
//		// /////////////////////////////////////////////////////////////////
//		//
//		// /////////////////////////////////////////////////////////////////
//		static unsigned int Sqrt(unsigned int n);
//
//		// /////////////////////////////////////////////////////////////////
//		//
//		// /////////////////////////////////////////////////////////////////
//		static void InterpolateLine(int *x, int *y, int end_x, int end_y, int step_size);
//
//		// /////////////////////////////////////////////////////////////////
//		// CMath::Get_Angle()
//		// 
//		// Given arbitrary x, y, this returns angle in degrees 0..359
//		// (Not 0..360 as old asm version did.)
//		// 
//		// E.g. Get_Angle(3,3) returns 45; Get_Angle(-4,0) returns 180
//		// 
//		// If passed in (0,0), returns 0; could easily be modified to crash.
//		// (And perhaps should.)
//		// 
//		// This has been tested vs the old asm Get_Angle;
//		// it agrees, differing at most by 1 from the old result, and
//		// in all such cases I checked, this one was the more accurate
//		// result (this version rounds rather than truncates).
//		// It was also tested for all x,y from -1000 to 1000 versus atan2 function
//		// and never differed by more than 2 from "true" result (which often
//		// varied a little from my HP calculator...).
//		// 
//		// This C actually runs a little faster than the old asm code
//		// (without even turning on compiler optimizations).
//		// 
//		// It also accepts ints, not just short ints, to plan for possible future use.
//		// (If very large ints are ever used, some additional checking must be added.)
//		// 
//		// Written in standard vanilla C, compiles under Watcom or Borland.
//		// 
//		// Most importantly, it gives correct results on inputs where
//		// the old version crashes (e.g. -1, 376), which prompted Russ to
//		// rewrite this in clear C to begin with.
//		// /////////////////////////////////////////////////////////////////
//		static unsigned short GetAngle(int x, int y);
//
//		// /////////////////////////////////////////////////////////////////
//		// Checks if a point is inside a polygon (assumes a closed polygon).
//		// Useful for testing for example, if the mouse cursor is inside a 
//		// button on screen.
//		// 
//		// /////////////////////////////////////////////////////////////////
//		static bool PointInPoly(Point const &test, const Poly &polygon);
//
//		// /////////////////////////////////////////////////////////////////
//		//
//		// /////////////////////////////////////////////////////////////////
//		static bool PointInPoly(int const x, int const y, int const * const	vertex, int const nvertex);
//
//		//// /////////////////////////////////////////////////////////////////
//		////
//		//// /////////////////////////////////////////////////////////////////
//		//static RECT BoundingBox(const POINT &pt1, const POINT &pt2, const POINT &pt3, const POINT &pt4);
//
//		//// /////////////////////////////////////////////////////////////////
//		////
//		//// /////////////////////////////////////////////////////////////////
//		//static RECT BoundingBox(const POINT *verts, const unsigned int nverts);
//
//		//// /////////////////////////////////////////////////////////////////
//		////
//		//// /////////////////////////////////////////////////////////////////
//		//static float const GetDistanceBetween(POINT const &pt1, POINT const &pt2);
//
//	//	// /////////////////////////////////////////////////////////////////
//	//	// @class BoundingBoxFinder
//	//	// @author Michael L. McShaffry
//	//	// 
//	//	// Used to determine the bounding box for a range of point-like objects.
//	//	// This includes POINTS, CPoints, and VertexUV to name a few.
//	//	// This works on any range which includes all STL containers as well as C style arrays.
//	//	// See BoundingBox(const POINT*, const unsigned int) in cpp for example usage.
//	//	// /////////////////////////////////////////////////////////////////
//	//	template <typename PointType>
//	//		class BoundingBoxFinder : std::unary_function<PointType, void>
//	//	{
//	//	private:
//
//	//		boost::optional<RECT> mBoundingBox;					///< 
//
//	//	public:
//
//	//		// /////////////////////////////////////////////////////////////////
//	//		//
//	//		// /////////////////////////////////////////////////////////////////
//	//		void operator()(PointType const & item)
//	//		{
//	//			if (mBoundingBox)
//	//			{
//	//				RECT initialValue = { item.x, item.y, item.x, item.y };
//	//				mBoundingBox = initialValue;
//	//			}
//	//			else
//	//			{
//	//				mBoundingBox->left = GameMathMin(mBoundingBox->left, item.x);
//	//				mBoundingBox->top = GameMathMin(mBoundingBox->top, item.y);
//	//				mBoundingBox->right = GameMathMax(mBoundingBox->right, item.x);
//	//				mBoundingBox->bottom = GameMathMax(mBoundingBox->bottom, item.y);
//	//			}
//	//		};
//
//	//		// /////////////////////////////////////////////////////////////////
//	//		//
//	//		// /////////////////////////////////////////////////////////////////
//	//		RECT const &BoundingBox()
//	//		{
//	//			return *mBoundingBox;
//	//		};
//	//	};
//	//};
//
//	//// /////////////////////////////////////////////////////////////////
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//struct LineSegment
//	//{
//	//	CPoint m_begin;					///< The start point of the line.
//	//	CPoint m_end;					///< The end point of the line.
//
//	//	// /////////////////////////////////////////////////////////////////
//	//	//
//	//	// /////////////////////////////////////////////////////////////////
//	//	LineSegment(const CPoint &begin, const CPoint &end)
//	//	{
//	//		m_begin=begin;
//	//		m_end=end;
//	//	}
//
//	//	// /////////////////////////////////////////////////////////////////
//	//	//
//	//	// /////////////////////////////////////////////////////////////////
//	//	LineSegment()
//	//	{
//	//		m_begin = m_end = CPoint(0,0);
//	//	}
//	//};
//
//
//	//// /////////////////////////////////////////////////////////////////
//	//// lines_intersect:  AUTHOR: Mukesh Prasad
//	////
//	////   This function computes whether two line segments,
//	////   respectively joining the input points (x1,y1) -- (x2,y2)
//	////   and the input points (x3,y3) -- (x4,y4) intersect.
//	////   If the lines intersect, the output variables x, y are
//	////   set to coordinates of the point of intersection.
//	////
//	////   All values are in integers.  The returned value is rounded
//	////   to the nearest integer point.
//	////
//	////   If non-integral grid points are relevant, the function
//	////   can easily be transformed by substituting floating point
//	////   calculations instead of integer calculations.
//	////
//	////  Entry
//	////        x1, y1,  x2, y2   Coordinates of endpoints of one segment.
//	////        x3, y3,  x4, y4   Coordinates of endpoints of other segment.
//	////
//	////   Exit
//	////        x, y              Coordinates of intersection point.
//	////
//	////   The value returned by the function is one of:
//	////
//	////        DONT_INTERSECT    0
//	////        DO_INTERSECT      1
//	////        COLLINEAR         2
//	////
//	//// Error condititions:
//	////
//	////     Depending upon the possible ranges, and particularly on 16-bit
//	////     computers, care should be taken to protect from overflow.
//	////
//	////     In the following code, 'long' values have been used for this
//	////     purpose, instead of 'int'.
//	////
//	//// Changes from the original code:
//	////     
//	////     Used ATL CPoint classes instead of straight integers (MLM)
//	//// /////////////////////////////////////////////////////////////////
//	//int lines_intersect(CPoint one, CPoint two, CPoint three, CPoint four, CPoint &result);
//
//	//// /////////////////////////////////////////////////////////////////
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//bool Intersect(const CRect &rect, const CPoint &center, double const radius);
//
//	//// /////////////////////////////////////////////////////////////////
//	//// Wraps angle so it's between -PI and PI
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//float WrapPi(float wrapMe);
//
//	//// /////////////////////////////////////////////////////////////////
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//float AngleDiff(float lhs, float rhs);
//
//	//// /////////////////////////////////////////////////////////////////
//	//// This function returns the look-at vector for a given orientation,
//	//// which is assumed to be on the Y axis.  Thus, 
//	//// the Y component of the returned vector will always be 0.
//	//// This function is used by the AI system which doesn't
//	//// care about orientation along any other axis.
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//Vec3 GetVectorFromYRotation(float angleRadians);
//
//	//// /////////////////////////////////////////////////////////////////
//	//// This function returns the target orientation for a given look-at
//	//// vector.
//	//// The orientation will be along the Y axis so the Y component of the
//	//// look-at vector is ignored.  This function is used by the AI system
//	//// which doesn't care about orientation along any other axis.
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//float GetYRotationFromVector(const Vec3 &lookAt);
//
//	// /////////////////////////////////////////////////////////////////
//	// Gets the minimum of any value (so long as the type implements the <
//	// operator).
//	//
//	// This is implemented because:
//	// std::min is not available in windows as it clashes with MFC headers.
//	//
//	// @param v1 The first value.
//	// @param v2 The second value.
//	// 
//	// @return T The minimum value found.
//	//
//	// /////////////////////////////////////////////////////////////////
//	template<class T>
//		T GameMathMin(const T v1, const T v2)
//	{
//		return ((v1 < v2) ? v1 : v2);
//	};
//
//	// /////////////////////////////////////////////////////////////////
//	// Gets the maximum of any value (so long as the type implements the >
//	// operator).
//	// 
//	// This is implemented because:
//	// std::min is not available in windows as it clashes with MFC headers.
//	//
//	// @param v1 The first value.
//	// @param v2 The second value.
//	// 
//	// @return T The maximum value found.
//	//
//	// /////////////////////////////////////////////////////////////////
//	template<class T>
//		T GameMathMax(const T v1, const T v2)
//	{
//		return ((v1 > v2) ? v1 : v2);
//	};
//
//}
//
//#endif
