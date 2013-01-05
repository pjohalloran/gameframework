#pragma once
#ifndef __VECTOR3_TEST_SUITE_H
#define __VECTOR3_TEST_SUITE_H

// /////////////////////////////////////////////////////////////////
// @file Vector3TestSuite.h
// @author PJ O Halloran
// @date 27/08/2010
//
// File contains the header for the Vector3 Test Suite.
//
// /////////////////////////////////////////////////////////////////

#include <cmath>
#include <string>

#include <cxxtest/TestSuite.h>
#include <boost/scoped_ptr.hpp>

#include "CommonMath.h"
#include "Vector.h"
#include "CRandom.h"

using GameHalloran::VecFloat;
using GameHalloran::Vector3;
using GameHalloran::Vector4;
using GameHalloran::Point3;

// /////////////////////////////////////////////////////////////////
// @class Vector3TestSuite
// @author PJ O Halloran
//
// This class defines a series of unit tests for the Vector3 
// class.
//
// /////////////////////////////////////////////////////////////////
class Vector3TestSuite : public CxxTest::TestSuite
{

private:
	
	bool IsTestDataReady()
	{
		return (true);
	};

public:

	// /////////////////////////////////////////////////////////////////
	// Constructor.
	//
	// /////////////////////////////////////////////////////////////////
	Vector3TestSuite()
	{

	};

	// /////////////////////////////////////////////////////////////////
	// Destructor.
	//
	// /////////////////////////////////////////////////////////////////
	~Vector3TestSuite()
	{

	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void setUp()
	{

	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void tearDown()
	{

	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testDefaultConstructor(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		Vector3 defaultObj;

		TS_ASSERT_DELTA(defaultObj.GetX(), 0.0f, delta);
		TS_ASSERT_DELTA(defaultObj.GetY(), 0.0f, delta);
		TS_ASSERT_DELTA(defaultObj.GetZ(), 0.0f, delta);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testXyzConstructor(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		const VecFloat x = 99.45f, y = -102.54f, z = 0.2345f;
		Vector3 obj(x, y, z);

		TS_ASSERT_DELTA(obj.GetX(), x, delta);
		TS_ASSERT_DELTA(obj.GetY(), y, delta);
		TS_ASSERT_DELTA(obj.GetZ(), z, delta);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testScalerConstructor(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		const VecFloat s = 99.45f;
		Vector3 obj(s);

		TS_ASSERT_DELTA(obj.GetX(), s, delta);
		TS_ASSERT_DELTA(obj.GetY(), s, delta);
		TS_ASSERT_DELTA(obj.GetZ(), s, delta);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testCopyConstructor(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		const VecFloat x = 99.45f, y = -102.54f, z = 0.2345f;
		Vector3 obj(x, y, z);
		TS_ASSERT_DELTA(obj.GetX(), x, delta);
		TS_ASSERT_DELTA(obj.GetY(), y, delta);
		TS_ASSERT_DELTA(obj.GetZ(), z, delta);

		Vector3 cp(obj);
		TS_ASSERT_DELTA(cp.GetX(), x, delta);
		TS_ASSERT_DELTA(cp.GetY(), y, delta);
		TS_ASSERT_DELTA(cp.GetZ(), z, delta);
		TS_ASSERT_DELTA(cp.GetX(), obj.GetX(), delta);
		TS_ASSERT_DELTA(cp.GetY(), obj.GetY(), delta);
		TS_ASSERT_DELTA(cp.GetZ(), obj.GetZ(), delta);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testPoint3ConvConstructor(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		const VecFloat x = 99.45f, y = -102.54f, z = 0.2345f;
		Point3 pt(x, y, z);

		Vector3 vec(pt);
		TS_ASSERT_DELTA(vec.GetX(), pt.GetX(), delta);
		TS_ASSERT_DELTA(vec.GetY(), pt.GetY(), delta);
		TS_ASSERT_DELTA(vec.GetZ(), pt.GetZ(), delta);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testVector4ConvConstructor(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		VecFloat x = 99.45f, y = -102.54f, z = 0.2345f, w = 1.0f;
		Vector4 vec4a(x, y, z, w);

		Vector3 vec3a(vec4a);
		TS_ASSERT_DELTA(vec3a.GetX(), vec4a.GetX(), delta);
		TS_ASSERT_DELTA(vec3a.GetY(), vec4a.GetY(), delta);
		TS_ASSERT_DELTA(vec3a.GetZ(), vec4a.GetZ(), delta);

		w = 3.0f;
		Vector4 vec4b(x, y, z, w);
		Vector3 vec3b(vec4b);
		TS_ASSERT_DELTA(vec3b.GetX(), vec4b.GetX() / vec4b.GetW(), delta);
		TS_ASSERT_DELTA(vec3b.GetY(), vec4b.GetY() / vec4b.GetW(), delta);
		TS_ASSERT_DELTA(vec3b.GetZ(), vec4b.GetZ() / vec4b.GetW(), delta);

		// Test the divide by zero error does not occur (when w is 0).
		x = 99.45f, y = -102.54f, z = 0.2345f, w = 0.0f;
		Vector4 zeroTest(x, y, z, w);
		Vector3 zeroVec3(zeroTest);
		TS_ASSERT_DELTA(zeroVec3.GetX(), x, delta);
		TS_ASSERT_DELTA(zeroVec3.GetY(), y, delta);
		TS_ASSERT_DELTA(zeroVec3.GetZ(), z, delta);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testAccessors(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		VecFloat x = 99.45f, y = -102.54f, z = 0.2345f;
		Vector3 obj(x, y, z);

		TS_ASSERT_DELTA(obj.GetX(), x, delta);
		TS_ASSERT_DELTA(obj.GetY(), y, delta);
		TS_ASSERT_DELTA(obj.GetZ(), z, delta);

		x = 34.54f;	y = 12.98f;	z = 546.87f;
		obj.SetX(x);	obj.SetY(y);	obj.SetZ(z);
		TS_ASSERT_DELTA(obj.GetX(), x, delta);
		TS_ASSERT_DELTA(obj.GetY(), y, delta);
		TS_ASSERT_DELTA(obj.GetZ(), z, delta);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testOperatorAssignment(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;

		// Test Vector3 assignment
		VecFloat x = 99.45f, y = -102.54f, z = 0.2345f;
		Vector3 obj(x, y, z);
		Vector3 eq = obj;
		TS_ASSERT_DELTA(obj.GetX(), eq.GetX(), delta);
		TS_ASSERT_DELTA(obj.GetY(), eq.GetY(), delta);
		TS_ASSERT_DELTA(obj.GetZ(), eq.GetZ(), delta);

		// Test Vector4 assignment.
		VecFloat w = 1.0f;
		Vector3 vec4TestVec;
		vec4TestVec = Vector4(x, y, z, w);
		TS_ASSERT_DELTA(vec4TestVec.GetX(), x, delta);
		TS_ASSERT_DELTA(vec4TestVec.GetY(), y, delta);
		TS_ASSERT_DELTA(vec4TestVec.GetZ(), z, delta);

		// Test Vector4 assignment when w is 0.
		w = 0.0f;
		Vector3 vec4ZeroTestVec;
		vec4ZeroTestVec = Vector4(x, y, z, w);
		TS_ASSERT_DELTA(vec4TestVec.GetX(), x, delta);
		TS_ASSERT_DELTA(vec4TestVec.GetY(), y, delta);
		TS_ASSERT_DELTA(vec4TestVec.GetZ(), z, delta);

		// Test Point3 assignment.
		Vector3 pt3TestVec;
		pt3TestVec = Point3(x, y, z);
		TS_ASSERT_DELTA(pt3TestVec.GetX(), x, delta);
		TS_ASSERT_DELTA(pt3TestVec.GetY(), y, delta);
		TS_ASSERT_DELTA(pt3TestVec.GetZ(), z, delta);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testOperatorEquivalent(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		VecFloat x = 99.45f, y = -102.54f, z = 0.2345f;
		Vector3 obj(x, y, z), eq(obj), ne;

		TS_ASSERT(obj == eq);
		TS_ASSERT(!(obj == ne));
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testOperatorPlusVector(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		VecFloat x = 10.0f, y = -10.5f, z = 54.2f, x1 = 89.05f, y1 = 34.54f, z1 = 23.08f;
		Vector3 obj(x, y, z), objb(x1, y1, z1), exVec(x+x1, y+y1, z+z1), acVec;

		acVec = obj + objb;
		TS_ASSERT(acVec == exVec);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testOperatorMinusVector(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		VecFloat x = 10.0f, y = -10.5f, z = 54.2f, x1 = 89.05f, y1 = 34.54f, z1 = 23.08f;
		Vector3 obj(x, y, z), objb(x1, y1, z1), exVec(x-x1, y-y1, z-z1), acVec;

		acVec = obj - objb;
		TS_ASSERT(acVec == exVec);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testOperatorPlusEqualsVector(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		VecFloat x = 10.0f, y = -10.5f, z = 54.2f, x1 = 89.05f, y1 = 34.54f, z1 = 23.08f;
		Vector3 obj(x, y, z), objb(x1, y1, z1), exVec(x+x1, y+y1, z+z1);

		obj += objb;
		TS_ASSERT(obj == exVec);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testOperatorMinusEqualsVector(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		VecFloat x = 10.0f, y = -10.5f, z = 54.2f, x1 = 89.05f, y1 = 34.54f, z1 = 23.08f;
		Vector3 obj(x, y, z), objb(x1, y1, z1), exVec(x-x1, y-y1, z-z1);

		obj -= objb;
		TS_ASSERT(obj == exVec);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testOperatorNegate(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		VecFloat x = 10.0f, y = -10.5f, z = 54.2f;
		Vector3 obj(x, y, z), orig(obj), exVec(-x, -y, -z), acVec;

		acVec = -obj;

		TS_ASSERT(acVec == exVec);
		TS_ASSERT(obj == orig);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testOperatorMultiplyScaler(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		VecFloat x = 10.0f, y = -10.5f, z = 54.2f, s = 3.5f;
		Vector3 obj(x, y, z), orig(obj), exVec(x*s, y*s, z*s), acVec;

		acVec = obj * s;

		TS_ASSERT(acVec == exVec);
		TS_ASSERT(obj == orig);

		// Now we will test the non member operator* version with the
		// scaler on the lhs and the vector on the right hand side of
		// the equation (as multiplication should be commutative).
		x = 77.7f, y = -100.345f, z = 11.2f, s = 5.0f;
		Vector3 objb(x, y, z), origb(objb), exVecb(x*s, y*s, z*s), acVecb;

		// NOTE the scaler on the lhs!!
		acVecb = s * objb;

		TS_ASSERT(acVecb == exVecb);
		TS_ASSERT(objb == origb);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testOperatorDivideScaler(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		// Test divide by non zero number.
		const VecFloat delta = 0.01f;
		VecFloat x = 10.0f, y = -10.5f, z = 54.2f, s = 3.5f;
		Vector3 obj(x, y, z), orig(obj), exVec(x/s, y/s, z/s), acVec;
		acVec = obj / s;
		TS_ASSERT(acVec == exVec);
		TS_ASSERT(obj == orig);

		// Test divide by zero number.
		s = 0.0f;
		Vector3 zeroTestVec(x, y, z), origZ(zeroTestVec), exVecZ(x, y, z), acVecZ;
		acVecZ = zeroTestVec / s;
		TS_ASSERT(acVecZ == exVecZ);
		TS_ASSERT(zeroTestVec == origZ);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testOperatorMultiplyEqualsScaler(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		VecFloat x = 10.0f, y = -10.5f, z = 54.2f, s = 3.5f;
		Vector3 obj(x, y, z), exVec(x*s, y*s, z*s);

		obj *= s;

		TS_ASSERT(obj == exVec);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testOperatorDivideEqualsScaler(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		// Test divide by non zero number.
		const VecFloat delta = 0.01f;
		VecFloat x = 10.0f, y = -10.5f, z = 54.2f, s = 3.5f;
		Vector3 obj(x, y, z), exVec(x/s, y/s, z/s);
		obj /= s;
		TS_ASSERT(obj == exVec);

		// Test divide by zero number.
		s = 0.0f;
		Vector3 zeroTestVec(x, y, z), exVecZ(x, y, z);
		zeroTestVec /= s;
		TS_ASSERT(zeroTestVec == exVecZ);
	};

	//// /////////////////////////////////////////////////////////////////
	//// 
	//// /////////////////////////////////////////////////////////////////
	//void testOperatorMultiplyVector(void)
	//{
	//	if(!IsTestDataReady())
	//	{
	//		TS_FAIL("Test data not created.");
	//	}
	//	VecFloat x = 10.0f, y = -10.0f, z = 2.0f, x1 = 1.0f, y1 = 2.0f, z1 = 3.0f;
	//	Vector3 obja(x, y, z), origa(obja), objb(x1, y1, z1), origb(objb), exVec(60.0f, -60.0f, 12.0f), acVec;
	//	acVec = obja * objb;
	//	TS_ASSERT(acVec == exVec);
	//	TS_ASSERT(obja == origa);
	//	TS_ASSERT(objb == origb);
	//};
	//// /////////////////////////////////////////////////////////////////
	//// 
	//// /////////////////////////////////////////////////////////////////
	//void testOperatorMultiplyEqualsVector(void)
	//{
	//	if(!IsTestDataReady())
	//	{
	//		TS_FAIL("Test data not created.");
	//	}
	//	const VecFloat delta = 0.01f;
	//	VecFloat x = 10.0f, y = -10.0f, z = 2.0f, x1 = 1.0f, y1 = 2.0f, z1 = 3.0f;
	//	Vector3 obja(x, y, z), objb(x1, y1, z1), origb(objb), exVec(60.0f, -60.0f, 12.0f);
	//	obja *= objb;
	//	TS_ASSERT(obja == exVec);
	//	TS_ASSERT(objb == origb);
	//};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testMagnitudeSqr(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		VecFloat x = 10.0f, y = -10.0f, z = 2.0f;
		VecFloat exMag = x*x + y*y + z*z, acMag;
		Vector3 obj(x, y, z);

		acMag = obj.MagnitudeSqr();

		TS_ASSERT(acMag == exMag);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testMagnitude(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		VecFloat x = 10.0f, y = -10.0f, z = 2.0f;
		VecFloat exMag = VecFloat(sqrt(x*x + y*y + z*z)), acMag;
		Vector3 obj(x, y, z);

		acMag = obj.Magnitude();

		TS_ASSERT(acMag == exMag);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testNormalize(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		// Normalize a non zero vector.
		const VecFloat delta = 0.01f;
		VecFloat x = 10.0f, y = -10.0f, z = 2.0f;
		Vector3 obj(x, y, z), orig(obj);
		VecFloat mag = obj.Magnitude();
		Vector3 exVec(x/mag, y/mag, z/mag);
		obj.Normalize();
		TS_ASSERT(obj == exVec);
		TS_ASSERT(!(obj == orig));

		// Normalize a zero vector.
		Vector3 zeroVector, copy(zeroVector);
		TS_ASSERT_THROWS_NOTHING(zeroVector.Normalize());
		TS_ASSERT(zeroVector == copy);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testNormalized(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		VecFloat x = 10.0f, y = -10.0f, z = 2.0f;
		Vector3 obj(x, y, z), orig(obj);
		VecFloat mag = obj.Magnitude();
		Vector3 exVec(x/mag, y/mag, z/mag), acVec;

		obj.Normalized(acVec);

		TS_ASSERT(acVec == exVec);
		TS_ASSERT(obj == orig);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testDot(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		VecFloat x = 10.0f, y = -10.0f, z = 2.0f, x1 = 1.0f, y1 = 2.0f, z1 = 3.0f;
		Vector3 veca(x, y, z), origa(veca), vecb(x1, y1, z1), origb(vecb);
		VecFloat exDot = x*x1 + y*y1 + z*z1, acDot;

		acDot = veca.Dot(vecb);

		TS_ASSERT(veca == origa);
		TS_ASSERT(vecb == origb);
		TS_ASSERT(acDot == exDot);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testCross(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		Vector3 unity(0.0f, 1.0f, 0.0f), unitx(1.0f, 0.0f, 0.0f), exUnitz(0.0f, 0.0f, -1.0f), acUnitz;
		unity.Cross(unitx, acUnitz);
		TS_ASSERT(acUnitz == exUnitz);

		// Reverse the order of the vectors, the resulting vector should be of the same length but pointing
		// in the opposite direction.
		exUnitz = Vector3(0.0f, 0.0f, 1.0f);
		unitx.Cross(unity, acUnitz);
		TS_ASSERT(acUnitz == exUnitz);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testToString(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		Vector3 vec(1.5f, 2.8f, 398.56f);
		std::string exStr("[1.5, 2.8, 398.56]"), acStr;

		acStr = vec.ToString();
		TS_ASSERT_EQUALS(acStr.compare(exStr.c_str()), 0);
		//TS_WARN(acStr.c_str());
		//TS_WARN(exStr.c_str());
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testOperatorStreamOut(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		// Not implemented, if ToString() is okay, this should be okay too...
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testAngleBetweenVector(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;
		Vector3 x(1.0f, 0.0f, 0.0f), y(0.0f, 1.0f, 0.0f);
		VecFloat exAngle = 90.0f, acAngle;

		// Get the angle between the unit x and y vectors aligned with their respective axes.
		//  It should be 90 degrees.
		acAngle = GameHalloran::AngleBetweenVector3(x, y);
		TS_ASSERT_DELTA(acAngle, exAngle, delta);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testIsNormalized(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		Vector3 normVec(1.0f, 0.0f, 0.0f), vec(99.67f, 45.34f, -900.32f);

		TS_ASSERT(GameHalloran::IsNormalizedVector3(normVec));
		TS_ASSERT(!GameHalloran::IsNormalizedVector3(vec));
		vec.Normalize();
		TS_ASSERT(GameHalloran::IsNormalizedVector3(vec));
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testGlobalVector3s(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		const VecFloat delta = 0.01f;

		// These unit tests assume a Global RIGHT hand system is in use...
		// Test up dir
		TS_ASSERT_DELTA(GameHalloran::g_up.GetX(), 0.0f, delta);
		TS_ASSERT_DELTA(GameHalloran::g_up.GetY(), 1.0f, delta);
		TS_ASSERT_DELTA(GameHalloran::g_up.GetZ(), 0.0f, delta);
		// Test right dir
		TS_ASSERT_DELTA(GameHalloran::g_right.GetX(), 1.0f, delta);
		TS_ASSERT_DELTA(GameHalloran::g_right.GetY(), 0.0f, delta);
		TS_ASSERT_DELTA(GameHalloran::g_right.GetZ(), 0.0f, delta);
		// Test forward direction.
		TS_ASSERT_DELTA(GameHalloran::g_forward.GetX(), 0.0f, delta);
		TS_ASSERT_DELTA(GameHalloran::g_forward.GetY(), 0.0f, delta);
		TS_ASSERT_DELTA(GameHalloran::g_forward.GetZ(), -1.0f, delta);

		// Test unit x vector
		TS_ASSERT_DELTA(GameHalloran::g_v3XunitVec.GetX(), 1.0f, delta);
		TS_ASSERT_DELTA(GameHalloran::g_v3XunitVec.GetY(), 0.0f, delta);
		TS_ASSERT_DELTA(GameHalloran::g_v3XunitVec.GetZ(), 0.0f, delta);


		// Test unit y vector
		TS_ASSERT_DELTA(GameHalloran::g_v3YunitVec.GetX(), 0.0f, delta);
		TS_ASSERT_DELTA(GameHalloran::g_v3YunitVec.GetY(), 1.0f, delta);
		TS_ASSERT_DELTA(GameHalloran::g_v3YunitVec.GetZ(), 0.0f, delta);

		// Test unit z vector.
		TS_ASSERT_DELTA(GameHalloran::g_v3ZunitVec.GetX(), 0.0f, delta);
		TS_ASSERT_DELTA(GameHalloran::g_v3ZunitVec.GetY(), 0.0f, delta);
		TS_ASSERT_DELTA(GameHalloran::g_v3ZunitVec.GetZ(), 1.0f, delta);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testGenerateRandomVector(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}
		
		float max = 100.0f, min = 0.0f;
		Vector3 vec, minV(min), maxV(max);
		GameHalloran::CRandom rng;
		rng.Randomize();
		
		// 1) min = 0, max = +100
		// Test that we never generate a vector outside the range (we will try to do so a good few times)
		for(int i = 0; i < 1000; ++i)
		{
			GameHalloran::GenerateRandomVector3(vec, rng, minV, maxV);
			TS_ASSERT(vec.GetX() >= min);
			TS_ASSERT(vec.GetY() >= min);
			TS_ASSERT(vec.GetZ() >= min);
			TS_ASSERT_LESS_THAN_EQUALS(vec.GetX(), max);
			TS_ASSERT_LESS_THAN_EQUALS(vec.GetY(), max);
			TS_ASSERT_LESS_THAN_EQUALS(vec.GetZ(), max);
		}

		// 2) min = -100, max = +100
		min = -100.0f;
		max = 100.0f;
		minV.SetX(min); minV.SetY(min); minV.SetZ(min);
		maxV.SetX(max); maxV.SetY(max); maxV.SetZ(max);
		// Test that we never generate a vector outside the range (we will try to do so a good few times)
		for(int i = 0; i < 1000; ++i)
		{
			GameHalloran::GenerateRandomVector3(vec, rng, minV, maxV);
			TS_ASSERT(vec.GetX() >= min);
			TS_ASSERT(vec.GetY() >= min);
			TS_ASSERT(vec.GetZ() >= min);
			TS_ASSERT_LESS_THAN_EQUALS(vec.GetX(), max);
			TS_ASSERT_LESS_THAN_EQUALS(vec.GetY(), max);
			TS_ASSERT_LESS_THAN_EQUALS(vec.GetZ(), max);
		}

		// 2) min = -100, max = -50
		min = -100.0f;
		max = -50.0f;
		minV.SetX(min); minV.SetY(min); minV.SetZ(min);
		maxV.SetX(max); maxV.SetY(max); maxV.SetZ(max);
		// Test that we never generate a vector outside the range (we will try to do so a good few times)
		for(int i = 0; i < 1000; ++i)
		{
			GameHalloran::GenerateRandomVector3(vec, rng, minV, maxV);
			TS_ASSERT(vec.GetX() >= min);
			TS_ASSERT(vec.GetY() >= min);
			TS_ASSERT(vec.GetZ() >= min);
			TS_ASSERT_LESS_THAN_EQUALS(vec.GetX(), max);
			TS_ASSERT_LESS_THAN_EQUALS(vec.GetY(), max);
			TS_ASSERT_LESS_THAN_EQUALS(vec.GetZ(), max);
		}

		// 4) Test when min >= max
		min = 100.0f;
		max = 100.0f;
		minV.SetX(min); minV.SetY(min); minV.SetZ(min);
		maxV.SetX(max); maxV.SetY(max); maxV.SetZ(max);
		// Test that we never generate a vector outside the range (we will try to do so a good few times)
		for(int i = 0; i < 1000; ++i)
		{
			GameHalloran::GenerateRandomVector3(vec, rng, minV, maxV);
			TS_ASSERT(vec.GetX() >= min);
			TS_ASSERT(vec.GetY() >= min);
			TS_ASSERT(vec.GetZ() >= min);
			TS_ASSERT_LESS_THAN_EQUALS(vec.GetX(), max);
			TS_ASSERT_LESS_THAN_EQUALS(vec.GetY(), max);
			TS_ASSERT_LESS_THAN_EQUALS(vec.GetZ(), max);
		}

	};

};

#endif
