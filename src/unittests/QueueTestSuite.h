#pragma once
#ifndef __QUEUE_TEST_SUITE_H
#define __QUEUE_TEST_SUITE_H

// /////////////////////////////////////////////////////////////////
// @file QueueTestSuite.h
// @author PJ O Halloran
// @date 19/04/2011
//
// File contains the header for the Queue Test Suite.
//
// /////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#include <cxxtest/TestSuite.h>

#include "Queue.h"

using std::cout;
using std::string;
using std::endl;

// /////////////////////////////////////////////////////////////////
// @class QueueTestSuite
// @author PJ O Halloran
//
// This class defines a series of unit tests for the Queue
// class.
//
// /////////////////////////////////////////////////////////////////
class QueueTestSuite : public CxxTest::TestSuite {
private:

    typedef GameHalloran::Queue<int> QueueInt;
    typedef GameHalloran::Queue<std::string> QueueString;
    typedef GameHalloran::Queue<int *> QueueIntPointer;
    typedef GameHalloran::U32 U32;

    QueueInt *m_testPtr;
    QueueString *m_testStrPtr;
    QueueIntPointer *m_testArrPtr;

    bool IsTestDataReady() {
        return (NULL != m_testPtr && NULL != m_testStrPtr && NULL != m_testArrPtr);
    };

public:

    // /////////////////////////////////////////////////////////////////
    // Constructor.
    //
    // /////////////////////////////////////////////////////////////////
    QueueTestSuite() : CxxTest::TestSuite(), m_testPtr(NULL) {

    };

    // /////////////////////////////////////////////////////////////////
    // Destructor.
    //
    // /////////////////////////////////////////////////////////////////
    ~QueueTestSuite() {

    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void setUp() {
        m_testPtr = new QueueInt;
        m_testStrPtr = new QueueString;
        m_testArrPtr = new QueueIntPointer;
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void tearDown() {
        delete m_testPtr;
        m_testPtr = NULL;

        delete m_testStrPtr;
        m_testStrPtr = NULL;

        delete m_testArrPtr;
        m_testArrPtr = NULL;
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testDefaultConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        QueueInt defaultObj;
        TS_ASSERT(defaultObj.IsEmpty());
        TS_ASSERT_EQUALS(defaultObj.Size(), 0);

        QueueString strObj;
        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 0);

        QueueIntPointer ptrObj;
        TS_ASSERT(ptrObj.IsEmpty());
        TS_ASSERT_EQUALS(ptrObj.Size(), 0);
    };

    // ////////////////////////////////////////////////////////////
    //
    //
    // ////////////////////////////////////////////////////////////
    void testCopyConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        QueueString strObj;
        TS_ASSERT(strObj.PushFront(std::string("one")));
        TS_ASSERT(strObj.PushFront(std::string("two")));
        TS_ASSERT(strObj.PushFront(std::string("three")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 3);

        QueueString copyObj(strObj);

        TS_ASSERT(!copyObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), copyObj.Size());

        std::string currElemOrig, currElemCopy;

        for(U32 i(0); i < 3; ++i) {
            TS_ASSERT(strObj.PopBack(currElemOrig));
            TS_ASSERT(copyObj.PopBack(currElemCopy));
            TS_ASSERT_EQUALS(currElemOrig.compare(currElemCopy.c_str()), 0);
            TS_ASSERT_EQUALS(strObj.Size(), copyObj.Size());
        }
        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT(copyObj.IsEmpty());
    };

    // ////////////////////////////////////////////////////////////
    //
    //
    // ////////////////////////////////////////////////////////////
    void testOperatorAssignment(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        QueueString strObj;
        TS_ASSERT(strObj.PushFront(std::string("one")));
        TS_ASSERT(strObj.PushFront(std::string("two")));
        TS_ASSERT(strObj.PushFront(std::string("three")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 3);

        QueueString copyObj;
        copyObj = strObj;

        TS_ASSERT(!copyObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), copyObj.Size());

        std::string currElemOrig, currElemCopy;

        for(U32 i(0); i < 3; ++i) {
            TS_ASSERT(strObj.PopBack(currElemOrig));
            TS_ASSERT(copyObj.PopBack(currElemCopy));
            TS_ASSERT_EQUALS(currElemOrig.compare(currElemCopy.c_str()), 0);
            TS_ASSERT_EQUALS(strObj.Size(), copyObj.Size());
        }
        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT(copyObj.IsEmpty());
    };

    // ////////////////////////////////////////////////////////////
    //
    //
    // ////////////////////////////////////////////////////////////
    void testSizeAndIsEmpty(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        QueueString strObj;

        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 0);

        TS_ASSERT(strObj.PushFront(std::string("one")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 1);

        TS_ASSERT(strObj.PushFront(std::string("two")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 2);

        TS_ASSERT(strObj.PushFront(std::string("three")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 3);

        std::string tmp;
        TS_ASSERT(strObj.PopBack(tmp));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 2);

        TS_ASSERT(strObj.PopBack(tmp));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 1);

        TS_ASSERT(strObj.PopBack(tmp));

        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 0);
    };

    // ////////////////////////////////////////////////////////////
    //
    //
    // ////////////////////////////////////////////////////////////
    void testPushFrontAndPopBack(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        QueueString strObj;
        TS_ASSERT(strObj.PushFront(std::string("one")));
        TS_ASSERT(strObj.PushFront(std::string("two")));
        TS_ASSERT(strObj.PushFront(std::string("three")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 3);

        std::string currElem;
        TS_ASSERT(strObj.PopBack(currElem));
        TS_ASSERT_EQUALS(currElem.compare("one"), 0);
        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 2);

        TS_ASSERT(strObj.PopBack(currElem));
        TS_ASSERT_EQUALS(currElem.compare("two"), 0);
        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 1);

        TS_ASSERT(strObj.PopBack(currElem));
        TS_ASSERT_EQUALS(currElem.compare("three"), 0);
        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 0);

        std::string copyElem(currElem);
        TS_ASSERT(!strObj.PopBack(currElem));
        TS_ASSERT_EQUALS(copyElem.compare(currElem.c_str()), 0);
    };

    // ////////////////////////////////////////////////////////////
    //
    //
    // ////////////////////////////////////////////////////////////
    void testClear(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        QueueString strObj;

        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 0);

        TS_ASSERT(strObj.PushFront(std::string("one")));
        TS_ASSERT(strObj.PushFront(std::string("two")));
        TS_ASSERT(strObj.PushFront(std::string("three")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 3);

        strObj.Clear();

        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 0);
    };

};

#endif
