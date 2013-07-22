#pragma once
#ifndef __POINT_TEST_SUITE_H
#define __POINT_TEST_SUITE_H

// /////////////////////////////////////////////////////////////////
// @file PointTestSuite.h
// @author PJ O Halloran
// @date 01/07/2010
//
// File contains the header for the Point Test Suite.
//
// /////////////////////////////////////////////////////////////////

#include <cxxtest/TestSuite.h>
#include <boost/scoped_ptr.hpp>

#include "Point.h"

// /////////////////////////////////////////////////////////////////
// @class PointTestSuite
// @author PJ O Halloran
//
// This class defines a series of unit tests for the Point
// class.
//
// /////////////////////////////////////////////////////////////////
class PointTestSuite : public CxxTest::TestSuite {
private:

    boost::scoped_ptr<GameHalloran::Point> m_testObjPtr;            ///< Pointer to the object under test.

    bool IsTestDataReady() {
        return (m_testObjPtr.get() != NULL);
    };

public:

    // /////////////////////////////////////////////////////////////////
    // Constructor.
    //
    // /////////////////////////////////////////////////////////////////
    PointTestSuite() {

    };

    // /////////////////////////////////////////////////////////////////
    // Destructor.
    //
    // /////////////////////////////////////////////////////////////////
    ~PointTestSuite() {

    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void setUp() {
        m_testObjPtr.reset(new GameHalloran::Point());
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void tearDown() {
        m_testObjPtr.reset();
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testDefaultConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

        // Test initial location is (0, 0).
        TS_ASSERT_EQUALS(m_testObjPtr->GetX(), 0);
        TS_ASSERT_EQUALS(m_testObjPtr->GetY(), 0);

        // Test 2 default objects are equivalent.
        GameHalloran::Point defaultObj;
        TS_ASSERT_EQUALS(defaultObj, *m_testObjPtr);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testIntConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

        // Data required for the test.
        const int x = 10;
        const int y = 20;
        GameHalloran::Point intObj(x, y);

        // Test initial location is (x, y).
        TS_ASSERT_EQUALS(intObj.GetX(), x);
        TS_ASSERT_EQUALS(intObj.GetY(), y);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testCopyConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

        // Data required for the test.
        const int x = 10;
        const int y = 20;
        m_testObjPtr->SetX(x);
        m_testObjPtr->SetY(y);

        // Copy object
        GameHalloran::Point copyObj(*m_testObjPtr);

        TS_ASSERT_EQUALS(copyObj.GetX(), x);
        TS_ASSERT_EQUALS(copyObj.GetY(), y);
        TS_ASSERT_EQUALS(copyObj, *m_testObjPtr);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testGetAndSetX(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

        // Data required for test.
        int x = -20;

        TS_ASSERT_EQUALS(m_testObjPtr->GetX(), 0);
        m_testObjPtr->SetX(x);
        TS_ASSERT_EQUALS(m_testObjPtr->GetX(), x);

        x = 400;
        m_testObjPtr->SetX(x);
        TS_ASSERT_EQUALS(m_testObjPtr->GetX(), x);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testGetAndSetY(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

        // Data required for test.
        int y = -70;

        TS_ASSERT_EQUALS(m_testObjPtr->GetY(), 0);
        m_testObjPtr->SetY(y);
        TS_ASSERT_EQUALS(m_testObjPtr->GetY(), y);

        y = 800;
        m_testObjPtr->SetY(y);
        TS_ASSERT_EQUALS(m_testObjPtr->GetY(), y);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorEquals(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

        // Data required for test.
        int x = 50;
        int y = -70;
        GameHalloran::Point otherObj(x, y);

        // Test the objects are not equivalent beforehand.
        TS_ASSERT_DIFFERS(otherObj, *m_testObjPtr);

        // Set the objects using the assignment operator.
        *m_testObjPtr = otherObj;

        // Test the objects are now equivalent.
        TS_ASSERT_EQUALS(otherObj, *m_testObjPtr);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorEquivalent(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

        // Data required for test.
        int x = 50;
        int y = -70;
        GameHalloran::Point otherObj(x, y);

        // Test the objects are not equivalent beforehand.
        TS_ASSERT_DIFFERS(otherObj, *m_testObjPtr);

        // Set the objects the same.
        m_testObjPtr->SetX(otherObj.GetX());
        m_testObjPtr->SetY(otherObj.GetY());

        // Test the objects are now equivalent.
        TS_ASSERT_EQUALS(otherObj, *m_testObjPtr);
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorLessThan(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

        // Set test object to an X and Y.
        m_testObjPtr->SetX(10);
        m_testObjPtr->SetY(10);

        // Data required for test.
        GameHalloran::Point lesser(9, 9);
        GameHalloran::Point equal(*m_testObjPtr);
        GameHalloran::Point greater(11, 11);

        // Test less than operator.
        TS_ASSERT(lesser < *m_testObjPtr);
        TS_ASSERT(!(equal < *m_testObjPtr));
        TS_ASSERT(!(greater < *m_testObjPtr));
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testDistance(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
            return;
        }

        // Data required for test.
        GameHalloran::Point pxpy(10, 10);           // Point in the positive x axis and positive y axis.
        GameHalloran::Point pxny(10, -10);          // Point in the positive x axis and negative y axis.
        GameHalloran::Point nxny(-10, -10);         // Point in the negative x axis and negative y axis.
        GameHalloran::Point nxpy(-10, 10);          // Point in the negative x axis and positive y axis.
        float dis = 14.14f;                         // Distance to the origin (approx) (worked out by hand using distance formula).
        float delta = 0.01f;                        // Floating point error to ignore in comparisions.

        // Test distance
        TS_ASSERT_DELTA(m_testObjPtr->Distance(pxpy), dis, delta);
        TS_ASSERT_DELTA(m_testObjPtr->Distance(pxny), dis, delta);
        TS_ASSERT_DELTA(m_testObjPtr->Distance(nxny), dis, delta);
        TS_ASSERT_DELTA(m_testObjPtr->Distance(nxpy), dis, delta);
    };

};

#endif
