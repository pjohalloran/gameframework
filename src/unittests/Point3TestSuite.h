#pragma once
#ifndef __POINT3_TEST_SUITE_H
#define __POINT3_TEST_SUITE_H

// /////////////////////////////////////////////////////////////////
// @file Point3TestSuite.h
// @author PJ O Halloran
// @date 27/08/2010
//
// File contains the header for the Point3 Test Suite.
//
// /////////////////////////////////////////////////////////////////

#include <cmath>
#include <string>

#include <cxxtest/TestSuite.h>
#include <boost/scoped_ptr.hpp>

#include "CommonMath.h"
#include "Vector.h"

using GameHalloran::Point3;
using GameHalloran::g_originPt;
using GameHalloran::VecFloat;
using GameHalloran::Vector3;

// /////////////////////////////////////////////////////////////////
// @class Point3TestSuite
// @author PJ O Halloran
//
// This class defines a series of unit tests for the Point3
// class.
//
// /////////////////////////////////////////////////////////////////
class Point3TestSuite : public CxxTest::TestSuite {
private:

    Point3 m_testObj;

    bool IsTestDataReady() {
        return (true);
    };

public:

    // /////////////////////////////////////////////////////////////////
    // Constructor.
    //
    // /////////////////////////////////////////////////////////////////
    Point3TestSuite() {
    };

    // /////////////////////////////////////////////////////////////////
    // Destructor.
    //
    // /////////////////////////////////////////////////////////////////
    ~Point3TestSuite() {
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void setUp() {
        m_testObj = Point3();
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void tearDown() {
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::Point3().
    //
    // /////////////////////////////////////////////////////////////////
    void testDefaultConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        TS_ASSERT_DELTA(m_testObj.GetX(), 0.0f, delta);
        TS_ASSERT_DELTA(m_testObj.GetY(), 0.0f, delta);
        TS_ASSERT_DELTA(m_testObj.GetZ(), 0.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::Point3(const VecFloat, const VecFloat, const VecFloat).
    //
    // /////////////////////////////////////////////////////////////////
    void testXyzConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = 9.9f, z = 42.56f;
        Point3 pt(x, y, z);
        TS_ASSERT_DELTA(pt.GetX(), x, delta);
        TS_ASSERT_DELTA(pt.GetY(), y, delta);
        TS_ASSERT_DELTA(pt.GetZ(), z, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::Point3(const VecFloat).
    //
    // /////////////////////////////////////////////////////////////////
    void testScalerConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat s = 10.0f;
        Point3 pt(s);
        TS_ASSERT_DELTA(pt.GetX(), s, delta);
        TS_ASSERT_DELTA(pt.GetY(), s, delta);
        TS_ASSERT_DELTA(pt.GetZ(), s, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::Point3(const Point3 &).
    //
    // /////////////////////////////////////////////////////////////////
    void testCopyConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = 9.9f, z = 42.56f;
        Point3 pt(x, y, z);
        Point3 nPt(pt);

        TS_ASSERT_DELTA(pt.GetX(), nPt.GetX(), delta);
        TS_ASSERT_DELTA(pt.GetY(), nPt.GetY(), delta);
        TS_ASSERT_DELTA(pt.GetZ(), nPt.GetZ(), delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::Point3(const Vector3 &).
    //
    // /////////////////////////////////////////////////////////////////
    void testVec3ConvConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = 9.9f, z = 42.56f;
        Vector3 vec(x, y, z);
        Point3 aPt(vec), exPt(x, y, z);

        TS_ASSERT_DELTA(aPt.GetX(), vec.GetX(), delta);
        TS_ASSERT_DELTA(aPt.GetY(), vec.GetY(), delta);
        TS_ASSERT_DELTA(aPt.GetZ(), vec.GetZ(), delta);
        TS_ASSERT_DELTA(aPt.GetX(), exPt.GetX(), delta);
        TS_ASSERT_DELTA(aPt.GetY(), exPt.GetY(), delta);
        TS_ASSERT_DELTA(aPt.GetZ(), exPt.GetZ(), delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3 get/set methods.
    //
    // /////////////////////////////////////////////////////////////////
    void testAccessors(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = 9.9f, z = 42.56f;
        Point3 pt(x, y, z);

        TS_ASSERT_DELTA(pt.GetX(), x, delta);
        TS_ASSERT_DELTA(pt.GetY(), y, delta);
        TS_ASSERT_DELTA(pt.GetZ(), z, delta);

        VecFloat nx = 45.7f, ny = 23.99f, nz = 21.4509f;
        pt.SetX(nx);
        pt.SetY(ny);
        pt.SetZ(nz);

        TS_ASSERT_DELTA(pt.GetX(), nx, delta);
        TS_ASSERT_DELTA(pt.GetY(), ny, delta);
        TS_ASSERT_DELTA(pt.GetZ(), nz, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::operator=().
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorAssignment(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Test Point3 assignment.
        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = 9.9f, z = 42.56f;
        Point3 pt(x, y, z);
        TS_ASSERT_DELTA(pt.GetX(), x, delta);
        TS_ASSERT_DELTA(pt.GetY(), y, delta);
        TS_ASSERT_DELTA(pt.GetZ(), z, delta);
        Point3 nPt;
        nPt = pt;
        TS_ASSERT_DELTA(pt.GetX(), nPt.GetX(), delta);
        TS_ASSERT_DELTA(pt.GetY(), nPt.GetY(), delta);
        TS_ASSERT_DELTA(pt.GetZ(), nPt.GetZ(), delta);

        // Test Vector3 assignment.
        Vector3 vec(x, y, z);
        Point3 vec3Test;
        vec3Test = vec;
        TS_ASSERT_DELTA(vec3Test.GetX(), vec.GetX(), delta);
        TS_ASSERT_DELTA(vec3Test.GetY(), vec.GetY(), delta);
        TS_ASSERT_DELTA(vec3Test.GetZ(), vec.GetZ(), delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::operator==().
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorEquivalent(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat x = 10.0f, y = 9.9f, z = 42.56f;
        Point3 pt(x, y, z), ePt(pt), nePt;

        TS_ASSERT(pt == ePt);
        TS_ASSERT(!(pt == nePt));
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::operator-(const Point3 &).
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorSubtractionPoint(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = 9.9f, z = 42.56f;
        Point3 pt1(x, y, z), pt2(x, y, z);

        Vector3 vec = pt1 - pt2;

        TS_ASSERT_DELTA(vec.GetX(), 0.0f, delta);
        TS_ASSERT_DELTA(vec.GetY(), 0.0f, delta);
        TS_ASSERT_DELTA(vec.GetZ(), 0.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::operator+(const Vector3 &).
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorAddition(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = 9.9f, z = 42.56f;
        Point3 pt1(x, y, z);
        Vector3 vec(x, y, z);

        Point3 pt2 = pt1 + vec;

        TS_ASSERT_DELTA(pt2.GetX(), 2 * x, delta);
        TS_ASSERT_DELTA(pt2.GetY(), 2 * y, delta);
        TS_ASSERT_DELTA(pt2.GetZ(), 2 * z, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::operator-(const Vector3 &).
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorSubtractionVector(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = 9.9f, z = 42.56f;
        Point3 pt1(x, y, z);
        Vector3 vec(x, y, z);

        Point3 pt2 = pt1 - vec;

        TS_ASSERT_DELTA(pt2.GetX(), 0.0f, delta);
        TS_ASSERT_DELTA(pt2.GetY(), 0.0f, delta);
        TS_ASSERT_DELTA(pt2.GetZ(), 0.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::operator+=(const Vector &).
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorPlusEquals(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = 9.9f, z = 42.56f;
        Point3 pt1(x, y, z);
        Vector3 vec(x, y, z);

        pt1 += vec;

        TS_ASSERT_DELTA(pt1.GetX(), 2 * x, delta);
        TS_ASSERT_DELTA(pt1.GetY(), 2 * y, delta);
        TS_ASSERT_DELTA(pt1.GetZ(), 2 * z, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::operator-=(const Vector3 &).
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorMinusEquals(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = 9.9f, z = 42.56f;
        Point3 pt1(x, y, z);
        Vector3 vec(x, y, z);

        pt1 -= vec;

        TS_ASSERT_DELTA(pt1.GetX(), 0.0f, delta);
        TS_ASSERT_DELTA(pt1.GetY(), 0.0f, delta);
        TS_ASSERT_DELTA(pt1.GetZ(), 0.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::operator-().
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorNegate(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = 9.9f, z = 42.56f;
        Point3 pt1(x, y, z), ptNeg;

        ptNeg = -pt1;

        TS_ASSERT_DELTA(pt1.GetX(), x, delta);
        TS_ASSERT_DELTA(pt1.GetY(), y, delta);
        TS_ASSERT_DELTA(pt1.GetZ(), z, delta);
        TS_ASSERT_DELTA(ptNeg.GetX(), -x, delta);
        TS_ASSERT_DELTA(ptNeg.GetY(), -y, delta);
        TS_ASSERT_DELTA(ptNeg.GetZ(), -z, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::DistanceSqr().
    //
    // /////////////////////////////////////////////////////////////////
    void testDistanceSqr(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = 9.9f, z = 42.56f, x1 = 20.0f, y1 = 35.87f, z1 = -4.67f;
        VecFloat dx = x1 - x, dy = y1 - y, dz = z1 - z;
        VecFloat distanceSqr = dx * dx + dy * dy + dz * dz;
        Point3 pt1(x, y, z), pt2(x1, y1, z1);

        TS_ASSERT_DELTA(pt1.DistanceSqr(pt2), distanceSqr, delta);
        TS_ASSERT_DELTA(pt1.GetX(), x, delta);
        TS_ASSERT_DELTA(pt1.GetY(), y, delta);
        TS_ASSERT_DELTA(pt1.GetZ(), z, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::Distance().
    //
    // /////////////////////////////////////////////////////////////////
    void testDistance(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat x = 10.0f, y = 9.9f, z = 42.56f, x1 = 20.0f, y1 = 35.87f, z1 = -4.67f;
        VecFloat dx = x1 - x, dy = y1 - y, dz = z1 - z;
        VecFloat distance = VecFloat(sqrt(dx * dx + dy * dy + dz * dz));
        Point3 pt1(x, y, z), pt2(x1, y1, z1);

        TS_ASSERT_DELTA(pt1.Distance(pt2), distance, delta);
        TS_ASSERT_DELTA(pt1.GetX(), x, delta);
        TS_ASSERT_DELTA(pt1.GetY(), y, delta);
        TS_ASSERT_DELTA(pt1.GetZ(), z, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::ToString().
    //
    // /////////////////////////////////////////////////////////////////
    void testToString(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat x = 10.1f, y = 9.9f, z = 42.56f;
        std::string exStr("[10.1, 9.9, 42.56]"), acStr;
        Point3 pt1(x, y, z);

        acStr = pt1.ToString();

        TS_ASSERT_EQUALS(acStr.compare(exStr.c_str()), 0);
        //TS_WARN(acStr.c_str());
        //TS_WARN(exStr.c_str());
    };

    // /////////////////////////////////////////////////////////////////
    // Test Point3::operator<<().
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorStreamOutput(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Not implemented, if ToString() is okay, this should be okay too...
    };

    // /////////////////////////////////////////////////////////////////
    // Test global points have the correct values.
    //
    // /////////////////////////////////////////////////////////////////
    void testGlobalPoints(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Test Origin point is 0.0, 0.0, 0.0.
        const GameHalloran::VecFloat delta = 0.01f;
        TS_ASSERT_DELTA(GameHalloran::g_originPt.GetX(), 0.0f, delta);
        TS_ASSERT_DELTA(GameHalloran::g_originPt.GetY(), 0.0f, delta);
        TS_ASSERT_DELTA(GameHalloran::g_originPt.GetZ(), 0.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test CalculateNormal().
    //
    // /////////////////////////////////////////////////////////////////
    void testCalculateNormalHelper(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        Point3 a(3.0f, 3.0f, 0.0f), b(1.0f, 0.0f, 0.0f), c(5.0f, 0.0f, 0.0f);
        Vector3 exNormal(0.0f, 0.0, -12.0f), acNormalA, acNormalB;

        acNormalA = GameHalloran::CalculateNormal(a, b, c, acNormalB);

        TS_ASSERT_EQUALS(acNormalA, acNormalB);
        TS_ASSERT_EQUALS(exNormal, acNormalA);
    };

    // /////////////////////////////////////////////////////////////////
    // Test CalculateUnitNormal().
    //
    // /////////////////////////////////////////////////////////////////
    void testCalculateUnitNormalHelper(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        Point3 a(3.0f, 3.0f, 0.0f), b(1.0f, 0.0f, 0.0f), c(5.0f, 0.0f, 0.0f);
        Vector3 exNormal(0.0f, 0.0, -1.0f), acNormalA, acNormalB;

        acNormalA = GameHalloran::CalculateUnitNormal(a, b, c, acNormalB);

        TS_ASSERT_EQUALS(acNormalA, acNormalB);
        TS_ASSERT_EQUALS(exNormal, acNormalA);
        TS_ASSERT_DELTA(exNormal.Magnitude(), 1.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testGenerateRandomPoint(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        float max = 100.0f, min = 0.0f;
        Point3 pt, minPt(min), maxPt(max);
        GameHalloran::CRandom rng;
        rng.Randomize();

        // 1) min = 0, max = +100
        // Test that we never generate a point outside the range (we will try to do so a good few times)
        for(int i = 0; i < 1000; ++i) {
            GameHalloran::GenerateRandomPoint3(pt, rng, minPt, maxPt);
            TS_ASSERT(pt.GetX() >= min);
            TS_ASSERT(pt.GetY() >= min);
            TS_ASSERT(pt.GetZ() >= min);
            TS_ASSERT_LESS_THAN_EQUALS(pt.GetX(), max);
            TS_ASSERT_LESS_THAN_EQUALS(pt.GetY(), max);
            TS_ASSERT_LESS_THAN_EQUALS(pt.GetZ(), max);
        }

        // 2) min = -100, max = +100
        min = -100.0f;
        max = 100.0f;
        minPt.SetX(min);
        minPt.SetY(min);
        minPt.SetZ(min);
        maxPt.SetX(max);
        maxPt.SetY(max);
        maxPt.SetZ(max);
        // Test that we never generate a point outside the range (we will try to do so a good few times)
        for(int i = 0; i < 1000; ++i) {
            GameHalloran::GenerateRandomPoint3(pt, rng, minPt, maxPt);
            TS_ASSERT(pt.GetX() >= min);
            TS_ASSERT(pt.GetY() >= min);
            TS_ASSERT(pt.GetZ() >= min);
            TS_ASSERT_LESS_THAN_EQUALS(pt.GetX(), max);
            TS_ASSERT_LESS_THAN_EQUALS(pt.GetY(), max);
            TS_ASSERT_LESS_THAN_EQUALS(pt.GetZ(), max);
        }

        // 2) min = -100, max = -50
        min = -100.0f;
        max = -50.0f;
        minPt.SetX(min);
        minPt.SetY(min);
        minPt.SetZ(min);
        maxPt.SetX(max);
        maxPt.SetY(max);
        maxPt.SetZ(max);
        // Test that we never generate a point outside the range (we will try to do so a good few times)
        for(int i = 0; i < 1000; ++i) {
            GameHalloran::GenerateRandomPoint3(pt, rng, minPt, maxPt);
            TS_ASSERT(pt.GetX() >= min);
            TS_ASSERT(pt.GetY() >= min);
            TS_ASSERT(pt.GetZ() >= min);
            TS_ASSERT_LESS_THAN_EQUALS(pt.GetX(), max);
            TS_ASSERT_LESS_THAN_EQUALS(pt.GetY(), max);
            TS_ASSERT_LESS_THAN_EQUALS(pt.GetZ(), max);
        }

        // 4) Test when min >= max
        min = 100.0f;
        max = 100.0f;
        minPt.SetX(min);
        minPt.SetY(min);
        minPt.SetZ(min);
        maxPt.SetX(max);
        maxPt.SetY(max);
        maxPt.SetZ(max);
        // Test that we never generate a point outside the range (we will try to do so a good few times)
        for(int i = 0; i < 1000; ++i) {
            GameHalloran::GenerateRandomPoint3(pt, rng, minPt, maxPt);
            TS_ASSERT(pt.GetX() >= min);
            TS_ASSERT(pt.GetY() >= min);
            TS_ASSERT(pt.GetZ() >= min);
            TS_ASSERT_LESS_THAN_EQUALS(pt.GetX(), max);
            TS_ASSERT_LESS_THAN_EQUALS(pt.GetY(), max);
            TS_ASSERT_LESS_THAN_EQUALS(pt.GetZ(), max);
        }

    };

};

#endif
