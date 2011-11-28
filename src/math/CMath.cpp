////========================================================================
//// CMath.cpp -  useful and fast math functions and approximations
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
//// File contains the implemetation for the CMath class and a few other 
//// utility functions.
////
//// This class was extracted from the Game Coding Complete 3 math code.
//// Originally written by Mike McShaffry author of Game
//// Coding Complete and games programmer.
////
//// Please see the CMath header for details on the few changes I have
//// made.
////
//// /////////////////////////////////////////////////////////////////
//
//// External headers
//#include <algorithm>
//#include <vector>
//#include <cmath>
//
//// Project headers
//#include "CommonMath.h"
//#include "CRandom.h"
//#include "CMath.h"
//
//// Namespace Declarations
//
//
//namespace GameHalloran
//{
//
//	// Declare static class members here.
//	const unsigned short CMath::angle_to_sin[90] = 
//	{
//		   0, 1144, 2287, 3430, 4572, 5712, 6850, 7987, 9121,10252,
//	   11380,12505,13626,14742,15855,16962,18064,19161,20252,21336,
//	   22415,23486,24550,25607,26656,27697,28729,29753,30767,31772,
//	   32768,33754,34729,35693,36647,37590,38521,39441,40348,41243,
//	   42126,42995,43852,44695,45525,46341,47143,47930,48703,49461,
//	   50203,50931,51643,52339,53020,53684,54332,54963,55578,56175,
//	   56756,57319,57865,58393,58903,59396,59870,60326,60764,61183,
//	   61584,61966,62328,62672,62997,63303,63589,63856,64104,64332,
//	   64540,64729,64898,65048,65177,65287,65376,65446,65496,65526,
//	};
//	CRandom			CMath::random;
//
//	// /////////////////////////////////////////////////////////////////
//	//
//	// /////////////////////////////////////////////////////////////////
//	CMath::CMath()
//	{
//	}
//
//	// /////////////////////////////////////////////////////////////////
//	//
//	// /////////////////////////////////////////////////////////////////
//	CMath::~CMath()
//	{
//	}
//
//	// /////////////////////////////////////////////////////////////////
//	//
//	// /////////////////////////////////////////////////////////////////
//	int CMath::Sin(short angle, int length)
//	{
//	   int result;						// The sin of the angle found.
//
//	   // normalize to 0..359
//	   if (length < 0) {
//		  length *= -1;
//		  angle += 180;
//	   }
//	   if (360 <= angle) {
//		  do {
//			 angle -= 360;
//		  } while (360 <= angle);
//		  //angle = angle % 360; // correct, but slower on average
//	   } else if (angle < 0) {
//		  do {
//			 angle += 360;
//		  } while (angle < 0);
//		  //angle = (u16)((angle + 33120) % 360); // correct, but slower on average
//	   }
//
//	   if (angle < 180) {
//		  if (angle < 90) {
//			 result = (int)(((unsigned int)length * angle_to_sin[angle] + 32768UL) / 65536UL);
//		  } else if (angle == 90) {
//			 result = length;
//		  } else {
//			 result = (int)(((unsigned int)length * angle_to_sin[180 - angle] + 32768UL) / 65536UL);
//		  }
//	   } else {
//		  if (angle < 270) {
//			 result = -(int)(((unsigned int)length * angle_to_sin[angle - 180] + 32768UL) / 65536UL);
//		  } else if (angle == 270) {
//			 result = -length;
//		  } else {
//			 result = -(int)(((unsigned int)length * angle_to_sin[360 - angle] + 32768UL) / 65536UL);
//		  }
//	   }
//
//	   return (result);
//	}
//
//	// /////////////////////////////////////////////////////////////////
//	//
//	// /////////////////////////////////////////////////////////////////
//	int CMath::Cos(short angle, int length)
//	{
//	   return Sin(angle + 90, length);
//	}
//
//	// /////////////////////////////////////////////////////////////////
//	//
//	// /////////////////////////////////////////////////////////////////
//	unsigned int CMath::Sqrt( unsigned int n )
//	{
//	   unsigned int lo;
//	   unsigned int hi;
//
//	   if (65535U * 65535U <= n)
//		  return (65535);
//
//	   if( n <= 65535 )
//	   {
//		  lo = 0;
//		  hi = 256;
//	   }
//	   else
//	   {
//		  lo = 256;
//		  hi = 65535;
//	   }
//
//	   do
//	   {
//		  const unsigned int mid = (lo + hi) / 2;
//
//		  if( mid * mid <= n )
//			 lo = mid;
//		  else
//			 hi = mid;
//	   }
//	   while( lo + 1 < hi );
//
//	   return (lo);
//	}
//
//	// /////////////////////////////////////////////////////////////////
//	//
//	// /////////////////////////////////////////////////////////////////
//	void CMath::InterpolateLine(int *x, int *y, int end_x, int end_y, int step_size)
//	{
//	   short delta_x, delta_y, angle;
//
//	   if ( step_size <= 0 || ( *x == end_x && *y == end_y ) ) return;
//
//	   delta_x = short(end_x - *x);
//	   delta_y = short(end_y - *y);
//
//	   if ( abs(delta_x) > 255 || abs(delta_y) > 255 )
//		  angle    = GetAngle( delta_x/2, delta_y/2 );
//	   else
//		  angle    = GetAngle( delta_x, delta_y );
//
//	   if ( *x < end_x )
//	   {
//		  *x += Cos( angle, step_size );
//		  if ( *x > end_x ) *x = end_x;
//	   }
//	   else
//	   { 
//		  *x += Cos( angle, step_size );
//		  if ( *x < end_x ) *x = end_x;
//	   }
//
//	   if ( *y < end_y )
//	   {
//		  *y += Sin( angle, step_size );
//		  if ( *y > end_y ) *y = end_y;
//	   }
//	   else
//	   { 
//		  *y += Sin( angle, step_size );
//		  if ( *y < end_y ) *y = end_y;
//	   }
//	   return;
//	}
//
//	// /////////////////////////////////////////////////////////////////
//	//
//	// /////////////////////////////////////////////////////////////////
//	unsigned short CMath::GetAngle(int x, int y)
//	{
//	   short result = -1;
//
//	   if (x == 0) {
//		  if (y < 0) {
//			 result = 270;
//		  } else if (0 < y) {
//			 result = 90;
//		  } else {
//			 result = 0;
//		  }
//	   } else if (y == 0) {
//		  if (x < 0) {
//			 result = 180;
//		  } else {
//			 result = 0;
//		  }
//	   } else {
//		  int correction;
//
//		  if (x < 0) {
//			 if (y < 0) {
//				x = -x;
//				y = -y;
//				correction = 180;
//			 } else {
//				const int old_x = x;
//				x = y;
//				y = -old_x;
//				correction = 90;
//			 }
//		  } else {
//			 if (y < 0) {
//				const int old_x = x;
//				x = -y;
//				y = old_x;
//				correction = 270;
//			 } else {
//				correction = 0;
//			 }
//		  }
//
//		  //assert(0 < x);
//		  //assert(0 < y);
//		  if (x == y) {
//			 result = 45;
//		  } else {
//			 /*
//				For y < x, this table takes quotient y * 128 / x
//				(which will be 0..127)
//				and tells corresponding angle 0..45
//			 */
//			 static const unsigned char quotient_to_angle[128] = {
//				 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7,
//				 7, 8, 8, 8, 9, 9,10,10,11,11,11,12,12,13,13,14,
//				14,14,15,15,16,16,16,17,17,18,18,18,19,19,20,20,
//				20,21,21,22,22,22,23,23,24,24,24,25,25,25,26,26,
//				26,27,27,27,28,28,29,29,29,30,30,30,31,31,31,32,
//				32,32,32,33,33,33,34,34,34,35,35,35,36,36,36,36,
//				37,37,37,38,38,38,38,39,39,39,39,40,40,40,40,41,
//				41,41,41,42,42,42,42,43,43,43,43,44,44,44,44,45,
//			 };
//
//			 if (x < y) {
//				const unsigned int quotient = ((unsigned int)x * 128) / (unsigned int)y;
//				result = 90 - quotient_to_angle[quotient];
//			 } else {
//				const unsigned int quotient = ((unsigned int)y * 128) / (unsigned int)x;
//				result = quotient_to_angle[quotient];
//			 }
//		  }
//		  result = short(result + correction);
//		  if (result == 360) {
//			 result = 0;
//		  }
//	   }
//	   return result;
//	}
//
//	// /////////////////////////////////////////////////////////////////
//	//
//	// /////////////////////////////////////////////////////////////////
//	bool CMath::PointInPoly(int const xt, int const	yt,	int const * const vertex, int const nvertex)
//	{
//		int xnew,ynew;     
//		int xold,yold;     
//		int x1,y1;
//		int x2,y2;     
//		int i;     
//		bool inside=false;     
//		
//		if (nvertex < 3) 
//		{
//			  return(0);     
//		}     
//		xold=vertex[(nvertex-1)*2];
//		yold=vertex[(nvertex-1)*2+1];     
//		for (i=0 ; i < nvertex ; i++) 
//		{
//			xnew = vertex[i*2];
//			ynew = vertex[i*2+1];
//			if (xnew > xold) 
//			{
//				x1=xold;               
//				x2=xnew;               
//				y1=yold;
//				y2=ynew;          
//			}          
//			else 
//			{               
//				x1=xnew;
//				x2=xold;               
//				y1=ynew;               
//				y2=yold;          
//			}
//			if ((xnew < xt) == (xt <= xold)         /* edge "open" at left end */
//			   && ((long)yt-(long)y1)*(long)(x2-x1)
//				< ((long)y2-(long)y1)*(long)(xt-x1)) 
//			{               
//				inside=!inside;
//			}          
//			xold=xnew;          
//			yold=ynew;     
//		}     
//		return(inside);
//	}
//
//	//// /////////////////////////////////////////////////////////////////
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//RECT CMath::BoundingBox(const POINT &pt1, const POINT &pt2, const POINT &pt3, const POINT &pt4)
//	//{
//	//	RECT bounding;
//	//	bounding.top = GameMathMin<LONG>( pt1.y, GameMathMin<LONG>(pt2.y, GameMathMin<LONG>( pt3.y, pt4.y ) ) );
//	//	bounding.bottom = GameMathMax<LONG>( pt1.y, GameMathMax<LONG>( pt2.y, GameMathMax<LONG>( pt3.y, pt4.y ) ) );
//	//	bounding.left = GameMathMin<LONG>( pt1.x, GameMathMin<LONG>( pt2.x, GameMathMin<LONG>( pt3.x, pt4.x ) ) );
//	//	bounding.right = GameMathMax<LONG>( pt1.x, GameMathMax<LONG>( pt2.x, GameMathMax<LONG>( pt3.x, pt4.x ) ) );
//	//	return bounding;
//	//}
//
//	//// /////////////////////////////////////////////////////////////////
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//RECT CMath::BoundingBox(const POINT *verts, const unsigned int nverts)
//	//{
//	//	assert (nverts > 0);
//	//	return (std::for_each(verts, verts + nverts, BoundingBoxFinder<POINT>()).BoundingBox());
//	//}
//
//	//// /////////////////////////////////////////////////////////////////
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//float const CMath::GetDistanceBetween(POINT const &pt1, POINT const &pt2)
//	//{
//	//	float const xDiff = fabsf(float(pt2.x) - float(pt1.x));
//	//	float const yDiff = fabsf(float(pt2.y) - float(pt1.y));
//
//	//	return (sqrtf(xDiff * xDiff + yDiff * yDiff));
//	//}
//
//	//// /////////////////////////////////////////////////////////////////
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//bool CMath::PointInPoly( Point const &test, const Poly & polygon)
//	//{
//	//	Point newPoint, oldPoint;
//	//	Point left, right;
//
//	//	bool inside=false;     
//
//	//	size_t points = polygon.size();
//
//	//	// The polygon must at least be a triangle
//	//	if (points < 3) 
//	//		  return false;     
//
//	//	oldPoint = polygon[points-1];
//
//	//	for (unsigned int i=0 ; i < points; i++) 
//	//	{
//	//		newPoint = polygon[i];
//	//		if (newPoint.GetX() > oldPoint.GetX()) 
//	//		{
//	//			left = oldPoint;               
//	//			right = newPoint;
//	//		}          
//	//		else 
//	//		{               
//	//			left = newPoint;               
//	//			right = oldPoint;
//	//		}
//
//	//		// A point exactly on the left side of the polygon
//	//		// will not intersect - as if it were "open"
//	//		if ((newPoint.GetX() < test.GetX()) == (test.GetX() <= oldPoint.GetX())         
//	//		   && (test.GetY()-left.GetY()) * (right.GetX()-left.GetX())
//	//			< (right.GetY()-left.GetY()) * (test.GetX()-left.GetX()) ) 
//	//		{               
//	//			inside=!inside;
//	//		}
//
//	//		oldPoint = newPoint;
//	//	}     
//	//	return(inside);
//	//}
//
//	//// /////////////////////////////////////////////////////////////////
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//int lines_intersect(CPoint one, CPoint two,	CPoint three, CPoint four, CPoint &result)
//	//{
//	//	long a1, a2, b1, b2, c1, c2; /* Coefficients of line eqns. */
//	//	long r1, r2, r3, r4;         /* 'Sign' values */
//	//	long denom, offset, num;     /* Intermediate values */
//
//	//	/* Compute a1, b1, c1, where line joining points 1 and 2
//	//	 * is "a1 x  +  b1 y  +  c1  =  0".
//	//	 */
//	//	a1 = two.y - one.y; //y2 - y1;
//	//	b1 = one.x - two.x; //x1 - x2;
//	//	c1 = two.x * one.y - one.x * two.y; //x2 * y1 - x1 * y2;
//
//	//	/* Compute r3 and r4.
//	//	 */
//	//	r3 = a1 * three.x + b1 * three.y + c1;  //a1 * x3 + b1 * y3 + c1;
//	//	r4 = a1 * four.x + b1 * four.y + c1;			//a1 * x4 + b1 * y4 + c1;
//
//	//	/* Check signs of r3 and r4.  If both point 3 and point 4 lie on
//	//	 * same side of line 1, the line segments do not intersect.
//	//	 */
//	//	if ( r3 != 0 &&
//	//		 r4 != 0 &&
//	//		 SAME_SIGNS( r3, r4 ))
//	//		return ( DONT_INTERSECT );
//
//	//	/* Compute a2, b2, c2 */
//	//	a2 = four.y - three.y; //y4 - y3;
//	//	b2 = three.x - four.x; //x3 - x4;
//	//	c2 = four.x * three.y - three.x * four.y; //x4 * y3 - x3 * y4;
//
//	//	/* Compute r1 and r2 */
//	//	r1 = a2 * one.x + b2 * one.y + c2; //a2 * x1 + b2 * y1 + c2;
//	//	r2 = a2 * two.x + b2 * two.y + c2; //a2 * x2 + b2 * y2 + c2;
//
//	//	/* Check signs of r1 and r2.  If both point 1 and point 2 lie
//	//	 * on same side of second line segment, the line segments do
//	//	 * not intersect.
//	//	 */
//	//	if ( r1 != 0 &&
//	//		 r2 != 0 &&
//	//		 SAME_SIGNS( r1, r2 ))
//	//		return (DONT_INTERSECT);
//
//	//	/* Line segments intersect: compute intersection point. 
//	//	 */
//	//	denom = a1 * b2 - a2 * b1;
//	//	if ( denom == 0 )
//	//		return (COLLINEAR);
//	//	offset = denom < 0 ? - denom / 2 : denom / 2;
//
//	//	/* The denom/2 is to get rounding instead of truncating.  It
//	//	 * is added or subtracted to the numerator, depending upon the
//	//	 * sign of the numerator.
//	//	 */
//	//	num = b1 * c2 - b2 * c1;
//	//	result.x = ( num < 0 ? num - offset : num + offset ) / denom;
//
//	//	num = a2 * c1 - a1 * c2;
//	//	result.y = ( num < 0 ? num - offset : num + offset ) / denom;
//
//	//	return (DO_INTERSECT);
//	//}
//
//	//// /////////////////////////////////////////////////////////////////
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//bool Intersect(const CRect &rect, const CPoint &center, double const radius)
//	//{
//	//	CRect r = rect;
//	//	double radiusiusSquared = radius * radius;
//
//	//	/* Translate coordinates, placing C at the origin. */
//	//	r.left -= center.x;  r.bottom -= center.y;
//	//	r.right -= center.x;  r.top -= center.y;
//
//	//	if (r.right < 0) 			/* R to left of circle center */
//	//	{
// //  			if (r.bottom < 0) 		/* R in lower left corner */
// //    			return ((r.right * r.right + r.bottom * r.bottom) < radiusiusSquared);
// //  			else if (r.top > 0) 	/* R in upper left corner */
// //  				return ((r.right * r.right + r.top * r.top) < radiusiusSquared);
// //  			else 					/* R due West of circle */
// //  				return(abs(r.right) < radius);
//	//	}
//	//	else if (r.left > 0)  	/* R to right of circle center */
//	//	{
// //			if (r.bottom < 0) 	/* R in lower right corner */
// //    					return ((r.left * r.left) < radiusiusSquared);
// //  			else if (r.top > 0)  	/* R in upper right corner */
// //    				return ((r.left * r.left + r.top + r.top) < radiusiusSquared);
// //  			else 				/* R due East of circle */
// //    				return (r.left < radius);
//	//	}
//	//	else				/* R on circle vertical centerline */
//	//	{
// //  			if (r.bottom < 0) 	/* R due South of circle */
// //    				return (abs(r.bottom) < radius);
// //  			else if (r.top > 0)  	/* R due North of circle */
// //    				return (r.top < radius);
// //  			else 				/* R contains circle centerpoint */
// //    				return(TRUE);
//	//	}
//	//}
//
//	// /////////////////////////////////////////////////////////////////
//	//
//	// /////////////////////////////////////////////////////////////////
//	float Interpolate(float normalizedValue, float begin, float end)
//	{
//		// first check input values
//		assert(normalizedValue>=0.0f);
//		assert(normalizedValue<=1.0f);
//		assert(end>begin);
//
//		return ( normalizedValue * (end - begin) ) + begin;
//	}
//
//	//// /////////////////////////////////////////////////////////////////
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//void MapYDeadZone(Vec3 &input, float deadZone)
//	//{
//	//	if (deadZone>=1.0f)
//	//		return;
//
//	//	// The dead zone is assumed to be zero close to the origin
//	//	// so we have to interpolate to find the right dead zone for
//	//	// our current value of X.
//	//	float actualDeadZone = Interpolate(fabs(input.x), 0.0f, deadZone);
//
//	//	if (fabs(input.y) < actualDeadZone)
//	//	{	
//	//		input.y = 0.0f;
//	//		return;
//	//	}
//
//	//	// Y is outside of the dead zone, but we still need to 
//	//	// interpolate it so we don't see any popping.
//
//	//	// Map Y values [actualDeadZone, 1.0f] to [0.0f, 1.0f]
//	//	float normalizedY = (input.y - actualDeadZone) / (1.0f - actualDeadZone);
//	//	input.y = normalizedY;
//	//}
//
//	//// /////////////////////////////////////////////////////////////////
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//float WrapPi( float wrapMe )
//	//{
//	//	float const twoPi = 2*D3DX_PI;
//	//	if ( fabs(wrapMe) > D3DX_PI ) 
//	//	{
//	//		wrapMe += D3DX_PI;
//	//		wrapMe -= floor( wrapMe * (1.0f / twoPi) ) * twoPi;
//	//		wrapMe -= D3DX_PI;
//	//	}
//
//	//	return wrapMe;
//	//}
//
//	//// /////////////////////////////////////////////////////////////////
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//float AngleDiff( float lhs, float rhs )
//	//{
//	//	lhs = WrapPi( lhs );
//	//	rhs = WrapPi( rhs );
//
//	//	return WrapPi( lhs - rhs );
//	//}
//
//	//// /////////////////////////////////////////////////////////////////
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//Vec3 GetVectorFromYRotation(float angleRadians)
//	//{
//	//	Vec3 lookAt;
//	//	WrapPi(angleRadians);
//	//	lookAt.x = cos(angleRadians);
//	//	lookAt.y = 0;
//	//	lookAt.z = sin(angleRadians);
//	//	lookAt.Normalize();  // just in case
//	//	return lookAt;
//	//}
//
//	//// /////////////////////////////////////////////////////////////////
//	////
//	//// /////////////////////////////////////////////////////////////////
//	//float GetYRotationFromVector(const Vec3& lookAt)
//	//{
//	//	Vec3 zUnit(0,0,1);  // 0 orientation means staring down the positive Z axis
//	//	float angle = (atan2(lookAt.z,lookAt.x*-1) - atan2(zUnit.z,zUnit.x));
//	//	return WrapPi(angle);
//	//}
//
//
//}
