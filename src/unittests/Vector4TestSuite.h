#pragma once
#ifndef __VECTOR4_TEST_SUITE_H
#define __VECTOR4_TEST_SUITE_H

// /////////////////////////////////////////////////////////////////
// @file Vector4TestSuite.h
// @author PJ O Halloran
// @date 27/08/2010
//
// File contains the header for the Vector4 Test Suite.
//
// /////////////////////////////////////////////////////////////////

#include <cxxtest/TestSuite.h>
#include <boost/scoped_ptr.hpp>

#include "CommonMath.h"
#include "Vector.h"

using GameHalloran::VecFloat;
using GameHalloran::Vector3;
using GameHalloran::Vector4;
using GameHalloran::Point3;

// /////////////////////////////////////////////////////////////////
// @class Vector4TestSuite
// @author PJ O Halloran
//
// This class defines a series of unit tests for the Vector4
// class.
//
// /////////////////////////////////////////////////////////////////
class Vector4TestSuite : public CxxTest::TestSuite {
private:

    bool IsTestDataReady() {
        return (true);
    };

public:

    // /////////////////////////////////////////////////////////////////
    // Constructor.
    //
    // /////////////////////////////////////////////////////////////////
    Vector4TestSuite() {

    };

    // /////////////////////////////////////////////////////////////////
    // Destructor.
    //
    // /////////////////////////////////////////////////////////////////
    ~Vector4TestSuite() {

    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void setUp() {

    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void tearDown() {

    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testDefaultConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        Vector4 defaultObj;

        TS_ASSERT_DELTA(defaultObj.GetX(), 0.0f, delta);
        TS_ASSERT_DELTA(defaultObj.GetY(), 0.0f, delta);
        TS_ASSERT_DELTA(defaultObj.GetZ(), 0.0f, delta);
        TS_ASSERT_DELTA(defaultObj.GetW(), 0.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testXyzConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        const VecFloat x = 99.45f, y = -102.54f, z = 0.2345f, w = 1.67f;
        Vector4 obj(x, y, z, w);

        TS_ASSERT_DELTA(obj.GetX(), x, delta);
        TS_ASSERT_DELTA(obj.GetY(), y, delta);
        TS_ASSERT_DELTA(obj.GetZ(), z, delta);
        TS_ASSERT_DELTA(obj.GetW(), w, delta);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testScalerConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        const VecFloat s = 99.45f;
        Vector4 obj(s);

        TS_ASSERT_DELTA(obj.GetX(), s, delta);
        TS_ASSERT_DELTA(obj.GetY(), s, delta);
        TS_ASSERT_DELTA(obj.GetZ(), s, delta);
        TS_ASSERT_DELTA(obj.GetW(), s, delta);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testCopyConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        const VecFloat x = 99.45f, y = -102.54f, z = 0.2345f, w = 5.0f;
        Vector4 obj(x, y, z, w);
        TS_ASSERT_DELTA(obj.GetX(), x, delta);
        TS_ASSERT_DELTA(obj.GetY(), y, delta);
        TS_ASSERT_DELTA(obj.GetZ(), z, delta);
        TS_ASSERT_DELTA(obj.GetW(), w, delta);

        Vector4 cp(obj);
        TS_ASSERT_DELTA(cp.GetX(), x, delta);
        TS_ASSERT_DELTA(cp.GetY(), y, delta);
        TS_ASSERT_DELTA(cp.GetZ(), z, delta);
        TS_ASSERT_DELTA(cp.GetW(), w, delta);
        TS_ASSERT_DELTA(cp.GetX(), obj.GetX(), delta);
        TS_ASSERT_DELTA(cp.GetY(), obj.GetY(), delta);
        TS_ASSERT_DELTA(cp.GetZ(), obj.GetZ(), delta);
        TS_ASSERT_DELTA(cp.GetW(), obj.GetW(), delta);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testPoint3ConvConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        const VecFloat x = 99.45f, y = -102.54f, z = 0.2345f;
        Point3 pt(x, y, z);

        Vector4 vec(pt);
        TS_ASSERT_DELTA(vec.GetX(), pt.GetX(), delta);
        TS_ASSERT_DELTA(vec.GetY(), pt.GetY(), delta);
        TS_ASSERT_DELTA(vec.GetZ(), pt.GetZ(), delta);
        TS_ASSERT_DELTA(vec.GetW(), 1.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testVector3ConvConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 99.45f, y = -102.54f, z = 0.2345f;
        Vector3 vec3(x, y, z);

        Vector4 vec4(vec3);
        TS_ASSERT_DELTA(vec4.GetX(), vec3.GetX(), delta);
        TS_ASSERT_DELTA(vec4.GetY(), vec3.GetY(), delta);
        TS_ASSERT_DELTA(vec4.GetZ(), vec3.GetZ(), delta);
        TS_ASSERT_DELTA(vec4.GetW(), 0.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testAccessors(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 99.45f, y = -102.54f, z = 0.2345f, w = 0.9f;
        Vector4 obj(x, y, z, w);

        TS_ASSERT_DELTA(obj.GetX(), x, delta);
        TS_ASSERT_DELTA(obj.GetY(), y, delta);
        TS_ASSERT_DELTA(obj.GetZ(), z, delta);
        TS_ASSERT_DELTA(obj.GetW(), w, delta);

        x = 34.54f;
        y = 12.98f;
        z = 546.87f, w = 0.77f;
        obj.SetX(x);
        obj.SetY(y);
        obj.SetZ(z);
        obj.SetW(w);
        TS_ASSERT_DELTA(obj.GetX(), x, delta);
        TS_ASSERT_DELTA(obj.GetY(), y, delta);
        TS_ASSERT_DELTA(obj.GetZ(), z, delta);
        TS_ASSERT_DELTA(obj.GetW(), w, delta);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorAssignment(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Test Vector4 assignment.
        const VecFloat delta = 0.01f;
        VecFloat x = 99.45f, y = -102.54f, z = 0.2345f, w = 3.4f;
        Vector4 obj(x, y, z, w);
        Vector4 eq = obj;
        TS_ASSERT_DELTA(obj.GetX(), eq.GetX(), delta);
        TS_ASSERT_DELTA(obj.GetY(), eq.GetY(), delta);
        TS_ASSERT_DELTA(obj.GetZ(), eq.GetZ(), delta);
        TS_ASSERT_DELTA(obj.GetW(), eq.GetW(), delta);

        // Test Vector3 assignment.
        Vector3 vec3(x, y, z);
        Vector4 vec3Test;
        vec3Test = vec3;
        TS_ASSERT_DELTA(vec3Test.GetX(), vec3.GetX(), delta);
        TS_ASSERT_DELTA(vec3Test.GetY(), vec3.GetY(), delta);
        TS_ASSERT_DELTA(vec3Test.GetZ(), vec3.GetZ(), delta);
        TS_ASSERT_DELTA(vec3Test.GetW(), 0.0f, delta);

        // Test Point3 assignment.
        Point3 pt3(x, y, z);
        Vector4 pt3Test;
        pt3Test = pt3;
        TS_ASSERT_DELTA(pt3Test.GetX(), pt3.GetX(), delta);
        TS_ASSERT_DELTA(pt3Test.GetY(), pt3.GetY(), delta);
        TS_ASSERT_DELTA(pt3Test.GetZ(), pt3.GetZ(), delta);
        TS_ASSERT_DELTA(pt3Test.GetW(), 1.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorEquivalent(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 99.45f, y = -102.54f, z = 0.2345f, w = 8.34f;
        Vector4 obj(x, y, z, w), eq(obj), ne;

        TS_ASSERT(obj == eq);
        TS_ASSERT(!(obj == ne));
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorPlusVector(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat x = 10.0f, y = -10.5f, z = 54.2f, w = 86.4f, x1 = 89.05f, y1 = 34.54f, z1 = 23.08f, w1 = -300.987f;
        Vector4 obj(x, y, z, w), objb(x1, y1, z1, w1), exVec(x + x1, y + y1, z + z1, w + w1), acVec;

        acVec = obj + objb;
        TS_ASSERT(acVec == exVec);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorMinusVector(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat x = 10.0f, y = -10.5f, z = 54.2f, w = 86.4f, x1 = 89.05f, y1 = 34.54f, z1 = 23.08f, w1 = -300.987f;
        Vector4 obj(x, y, z, w), objb(x1, y1, z1, w1), exVec(x - x1, y - y1, z - z1, w - w1), acVec;

        acVec = obj - objb;
        TS_ASSERT(acVec == exVec);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorPlusEqualsVector(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat x = 10.0f, y = -10.5f, z = 54.2f, w = -23.45f, x1 = 89.05f, y1 = 34.54f, z1 = 23.08f, w1 = 784.98f;
        Vector4 obj(x, y, z, w), objb(x1, y1, z1, w1), exVec(x + x1, y + y1, z + z1, w + w1);

        obj += objb;
        TS_ASSERT(obj == exVec);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorMinusEqualsVector(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat x = 10.0f, y = -10.5f, z = 54.2f, w = -23.45f, x1 = 89.05f, y1 = 34.54f, z1 = 23.08f, w1 = 784.98f;
        Vector4 obj(x, y, z, w), objb(x1, y1, z1, w1), exVec(x - x1, y - y1, z - z1, w - w1);

        obj -= objb;
        TS_ASSERT(obj == exVec);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorNegate(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = -10.5f, z = 54.2f, w = -45.67f;
        Vector4 obj(x, y, z, w), orig(obj), exVec(-x, -y, -z, -w), acVec;

        acVec = -obj;

        TS_ASSERT(acVec == exVec);
        TS_ASSERT(obj == orig);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorMultiplyScaler(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = -10.5f, z = 54.2f, w = 98.21f, s = 3.5f;
        Vector4 obj(x, y, z, w), orig(obj), exVec(x * s, y * s, z * s, w * s), acVec;

        acVec = obj * s;

        TS_ASSERT(acVec == exVec);
        TS_ASSERT(obj == orig);

        // Now we will test the non member operator* version with the
        // scaler on the lhs and the vector on the right hand side of
        // the equation (as multiplication should be commutative).
        x = 77.7f, y = -100.345f, z = 11.2f, w = -45.23f, s = 5.0f;
        Vector4 objb(x, y, z, w), origb(objb), exVecb(x * s, y * s, z * s, w * s), acVecb;

        // NOTE the scaler on the lhs!!
        acVecb = s * objb;

        TS_ASSERT(acVecb == exVecb);
        TS_ASSERT(objb == origb);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorDivideScaler(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Test divide by non zero number.
        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = -10.5f, z = 54.2f, w = -9.0f, s = 3.5f;
        Vector4 obj(x, y, z, w), orig(obj), exVec(x / s, y / s, z / s, w / s), acVec;
        acVec = obj / s;
        TS_ASSERT(acVec == exVec);
        TS_ASSERT(obj == orig);

        // Test divide by zero number.
        s = 0.0f;
        Vector4 zeroTest(x, y, z, w), origZ(zeroTest), exVecZ(x, y, z, w), acVecZ;
        acVecZ = zeroTest / s;
        TS_ASSERT(acVecZ == exVecZ);
        TS_ASSERT(zeroTest == origZ);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorMultiplyEqualsScaler(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = -10.5f, z = 54.2f, w = -12.5f, s = 3.5f;
        Vector4 obj(x, y, z, w), exVec(x * s, y * s, z * s, w * s);

        obj *= s;

        TS_ASSERT(obj == exVec);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorDivideEqualsScaler(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Test divide by non zero number.
        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = -10.5f, z = 54.2f, w = -23.4f, s = 3.5f;
        Vector4 obj(x, y, z, w), exVec(x / s, y / s, z / s, w / s);
        obj /= s;
        TS_ASSERT(obj == exVec);

        // Test divide by zero number.
        s = 0.0f;
        Vector4 zeroTest(x, y, z, w), exVecZ(x, y, z, w);
        zeroTest /= s;
        TS_ASSERT(zeroTest == exVecZ);
    };

    //// /////////////////////////////////////////////////////////////////
    ////
    //// /////////////////////////////////////////////////////////////////
    //void testOperatorMultiplyVector(void)
    //{
    //  if(!IsTestDataReady())
    //  {
    //      TS_FAIL("Test data not created.");
    //  }
    //  VecFloat x = 10.0f, y = -10.0f, z = 2.0f, w = -3.0f, x1 = -1.0f, y1 = 2.0f, z1 = 3.0f, w1 = -4.0f;
    //  Vector4 obja(x, y, z, w), origa(obja), objb(x1, y1, z1, w1), origb(objb), exVec(0.0f, 0.0f, 0.0f, 0.0f), acVec;
    //  acVec = obja * objb;
    //  TS_ASSERT(acVec == exVec);
    //  TS_ASSERT(obja == origa);
    //  TS_ASSERT(objb == origb);
    //};
    //// /////////////////////////////////////////////////////////////////
    ////
    //// /////////////////////////////////////////////////////////////////
    //void testOperatorMultiplyEqualsVector(void)
    //{
    //  if(!IsTestDataReady())
    //  {
    //      TS_FAIL("Test data not created.");
    //  }
    //  VecFloat x = 10.0f, y = -10.0f, z = 2.0f, w = -3.0f, x1 = -1.0f, y1 = 2.0f, z1 = 3.0f, w1 = -4.0f;
    //  Vector4 obja(x, y, z, w), origa(obja), objb(x1, y1, z1, w1), origb(objb), exVec(0.0f, 0.0f, 0.0f, 0.0f);
    //  obja *= objb;
    //  TS_ASSERT(obja == exVec);
    //  TS_ASSERT(objb == origb);
    //};

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testMagnitudeSqr(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = -10.0f, z = 2.0f, w = 89.45f;
        VecFloat exMag = x * x + y * y + z * z + w * w, acMag;
        Vector4 obj(x, y, z, w);

        acMag = obj.MagnitudeSqr();

        TS_ASSERT(acMag == exMag);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testMagnitude(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = -10.0f, z = 2.0f, w = -34.56f;
        VecFloat exMag = VecFloat(sqrt(x * x + y * y + z * z + w * w)), acMag;
        Vector4 obj(x, y, z, w);

        acMag = obj.Magnitude();

        TS_ASSERT(acMag == exMag);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testNormalize(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Test normalize non zero vector.
        VecFloat x = 10.0f, y = -10.0f, z = 2.0f, w = 23.65f;
        Vector4 obj(x, y, z, w), orig(obj);
        VecFloat mag = obj.Magnitude();
        Vector4 exVec(x / mag, y / mag, z / mag, w / mag);
        obj.Normalize();
        TS_ASSERT(obj == exVec);
        TS_ASSERT(!(obj == orig));

        // Test normalize zero vector.
        Vector4 zeroVec, copy(zeroVec);
        TS_ASSERT_THROWS_NOTHING(zeroVec.Normalize());
        TS_ASSERT(zeroVec == copy);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testNormalized(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat x = 10.0f, y = -10.0f, z = 2.0f, w = 923.4f;
        Vector4 obj(x, y, z, w), orig(obj);
        VecFloat mag = obj.Magnitude();
        Vector4 exVec(x / mag, y / mag, z / mag, w / mag), acVec;

        obj.Normalized(acVec);

        TS_ASSERT(acVec == exVec);
        TS_ASSERT(obj == orig);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testDot(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = -10.0f, z = 2.0f, w = 12.56f, x1 = 1.0f, y1 = 2.0f, z1 = 3.0f, w1 = 923.67f;
        Vector4 veca(x, y, z, w), origa(veca), vecb(x1, y1, z1, w1), origb(vecb);
        VecFloat exDot = x * x1 + y * y1 + z * z1 + w * w1, acDot;

        acDot = veca.Dot(vecb);

        TS_ASSERT(veca == origa);
        TS_ASSERT(vecb == origb);
        TS_ASSERT(acDot == exDot);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testToString(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        Vector4 vec(1.5f, 2.8f, 398.56f, -126.45f);
        std::string exStr("[1.5, 2.8, 398.56, -126.45]"), acStr;

        acStr = vec.ToString();
        TS_ASSERT_EQUALS(acStr.compare(exStr.c_str()), 0);
        //TS_WARN(acStr.c_str());
        //TS_WARN(exStr.c_str());
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorStreamOut(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Not implemented, if ToString() is okay, this should be okay too...
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testGlobalVector4s(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;

        // Note that this unit test assumes a world coordinate right hand system.
        // Test up dir
        TS_ASSERT_DELTA(GameHalloran::g_up4.GetX(), 0.0f, delta);
        TS_ASSERT_DELTA(GameHalloran::g_up4.GetY(), 1.0f, delta);
        TS_ASSERT_DELTA(GameHalloran::g_up4.GetZ(), 0.0f, delta);
        TS_ASSERT_DELTA(GameHalloran::g_up4.GetW(), 0.0f, delta);
        // Test right dir
        TS_ASSERT_DELTA(GameHalloran::g_right4.GetX(), 1.0f, delta);
        TS_ASSERT_DELTA(GameHalloran::g_right4.GetY(), 0.0f, delta);
        TS_ASSERT_DELTA(GameHalloran::g_right4.GetZ(), 0.0f, delta);
        TS_ASSERT_DELTA(GameHalloran::g_right4.GetW(), 0.0f, delta);
        // Test forward direction.
        TS_ASSERT_DELTA(GameHalloran::g_forward4.GetX(), 0.0f, delta);
        TS_ASSERT_DELTA(GameHalloran::g_forward4.GetY(), 0.0f, delta);
        TS_ASSERT_DELTA(GameHalloran::g_forward4.GetZ(), -1.0f, delta);
        TS_ASSERT_DELTA(GameHalloran::g_forward4.GetW(), 0.0f, delta);
    };

    // TODO: Implement unit tests for Plane class (replacing Plane3D tested below in the comments).

    //// /////////////////////////////////////////////////////////////////
    //// Test CalculatePlane().
    ////
    //// /////////////////////////////////////////////////////////////////
    //void testCalculatePlaneHelper(void)
    //{
    //  if(!IsTestDataReady())
    //  {
    //      TS_FAIL("Test data not created.");
    //  }
    //  //Plane3d &CalculatePlane(const Point3 &ptOne, const Point3 &ptTwo, const Point3 &ptThree, Plane3d &planeOut)
    //  Point3 one(2.0f, 3.0f, -3.0f);
    //  Point3 two(2.0f, 0.0f, -2.0f);
    //  Point3 three(3.0f, 0.0f, -5.0f);
    //  GameHalloran::Plane3d exPlane(-0.943f, -0.104f, -0.314f, 1.257f), acPlane;
    //  acPlane = GameHalloran::CalculatePlane(one, two, three, acPlane);
    //  TS_ASSERT(acPlane == exPlane);
    //};

    //// /////////////////////////////////////////////////////////////////
    //// Test CalculateDistanceToPlane().
    ////
    //// /////////////////////////////////////////////////////////////////
    //void testCalculateDistanceToPlaneHelper(void)
    //{
    //  if(!IsTestDataReady())
    //  {
    //      TS_FAIL("Test data not created.");
    //  }
    //  //VecFloat CalculateDistanceToPlane(const Plane3d &plane, const Point3 &pt)
    //  Point3 one(2.0f, 3.0f, -3.0f);
    //  Point3 two(2.0f, 0.0f, -2.0f);
    //  Point3 three(3.0f, 0.0f, -5.0f);
    //  GameHalloran::Plane3d plane;
    //  VecFloat p1Ex = 4.086f, p1Ac, p2Ex = -1.257f, p2Ac;
    //  plane = GameHalloran::CalculatePlane(one, two, three, plane);
    //
    //  Point3 P1(-2.0f, 0.0f, -3.0f), P2(3.0f, 0.0f, -1.0f);
    //  p1Ac = GameHalloran::CalculateDistanceToPlane(plane, P1);
    //  p2Ac = GameHalloran::CalculateDistanceToPlane(plane, P2);
    //  TS_ASSERT(p1Ac > 0.0f);
    //  TS_ASSERT(p2Ac < 0.0f);
    //  const VecFloat delta = 0.01f;
    //  TS_ASSERT_DELTA(p1Ac, p1Ex, delta);
    //  TS_ASSERT_DELTA(p2Ac, p2Ex, delta);
    //};

    //// /////////////////////////////////////////////////////////////////
    //// Test IsPointInsidePlane().
    ////
    //// /////////////////////////////////////////////////////////////////
    //void testIsPointInsidePlaneHelper(void)
    //{
    //  if(!IsTestDataReady())
    //  {
    //      TS_FAIL("Test data not created.");
    //  }
    //  //bool IsPointInsidePlane(const Plane3d &plane, const Point3 &pt)
    //  Point3 one(2.0f, 3.0f, -3.0f);
    //  Point3 two(2.0f, 0.0f, -2.0f);
    //  Point3 three(3.0f, 0.0f, -5.0f);
    //  GameHalloran::Plane3d plane;
    //  plane = GameHalloran::CalculatePlane(one, two, three, plane);
    //
    //  Point3 P1(-2.0f, 0.0f, -3.0f), P2(3.0f, 0.0f, -1.0f);
    //  TS_ASSERT(GameHalloran::IsPointInsidePlane(plane, P1));
    //  TS_ASSERT(!GameHalloran::IsPointInsidePlane(plane, P2));
    //};

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testGenerateRandomVector(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        float max = 100.0f, min = 0.0f;
        Vector4 vec, minV(min), maxV(max);
        GameHalloran::CRandom rng;
        rng.Randomize();

        // 1) min = 0, max = +100
        // Test that we never generate a vector outside the range (we will try to do so a good few times)
        for(int i = 0; i < 1000; ++i) {
            GameHalloran::GenerateRandomVector4(vec, rng, minV, maxV);
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
        minV.SetX(min);
        minV.SetY(min);
        minV.SetZ(min);
        minV.SetW(min);
        maxV.SetX(max);
        maxV.SetY(max);
        maxV.SetZ(max);
        maxV.SetW(max);
        // Test that we never generate a vector outside the range (we will try to do so a good few times)
        for(int i = 0; i < 1000; ++i) {
            GameHalloran::GenerateRandomVector4(vec, rng, minV, maxV);
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
        minV.SetX(min);
        minV.SetY(min);
        minV.SetZ(min);
        minV.SetW(min);
        maxV.SetX(max);
        maxV.SetY(max);
        maxV.SetZ(max);
        maxV.SetW(max);
        // Test that we never generate a vector outside the range (we will try to do so a good few times)
        for(int i = 0; i < 1000; ++i) {
            GameHalloran::GenerateRandomVector4(vec, rng, minV, maxV);
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
        minV.SetX(min);
        minV.SetY(min);
        minV.SetZ(min);
        minV.SetW(min);
        maxV.SetX(max);
        maxV.SetY(max);
        maxV.SetZ(max);
        maxV.SetW(max);
        // Test that we never generate a vector outside the range (we will try to do so a good few times)
        for(int i = 0; i < 1000; ++i) {
            GameHalloran::GenerateRandomVector4(vec, rng, minV, maxV);
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
