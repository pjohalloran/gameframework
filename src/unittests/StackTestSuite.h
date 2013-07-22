#pragma once
#ifndef __STACK_TEST_SUITE_H
#define __STACK_TEST_SUITE_H

// /////////////////////////////////////////////////////////////////
// @file StackTestSuite.h
// @author PJ O Halloran
// @date 19/04/2011
//
// File contains the header for the Stack Test Suite.
//
// /////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#include <cxxtest/TestSuite.h>

#include "Stack.h"

using std::cout;
using std::string;
using std::endl;

// /////////////////////////////////////////////////////////////////
// @class StackTestSuite
// @author PJ O Halloran
//
// This class defines a series of unit tests for the Stack
// class.
//
// /////////////////////////////////////////////////////////////////
class StackTestSuite : public CxxTest::TestSuite {
private:

    typedef GameHalloran::Stack<int> StackInt;
    typedef GameHalloran::Stack<std::string> StackString;
    typedef GameHalloran::Stack<int *> StackIntPointer;
    typedef GameHalloran::U32 U32;

    StackInt *m_testPtr;
    StackString *m_testStrPtr;
    StackIntPointer *m_testArrPtr;

    bool IsTestDataReady() {
        return (NULL != m_testPtr && NULL != m_testStrPtr && NULL != m_testArrPtr);
    };

public:

    // /////////////////////////////////////////////////////////////////
    // Constructor.
    //
    // /////////////////////////////////////////////////////////////////
    StackTestSuite() : CxxTest::TestSuite(), m_testPtr(NULL) {

    };

    // /////////////////////////////////////////////////////////////////
    // Destructor.
    //
    // /////////////////////////////////////////////////////////////////
    ~StackTestSuite() {

    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void setUp() {
        m_testPtr = new StackInt;
        m_testStrPtr = new StackString;
        m_testArrPtr = new StackIntPointer;
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

        StackInt defaultObj;
        TS_ASSERT(defaultObj.IsEmpty());
        TS_ASSERT_EQUALS(defaultObj.Size(), 0);

        StackString strObj;
        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 0);

        StackIntPointer ptrObj;
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

        StackString strObj;
        TS_ASSERT(strObj.Push(std::string("one")));
        TS_ASSERT(strObj.Push(std::string("two")));
        TS_ASSERT(strObj.Push(std::string("three")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 3);

        StackString copyObj(strObj);

        TS_ASSERT(!copyObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), copyObj.Size());

        std::string currElemOrig, currElemCopy;

        for(U32 i(0); i < 3; ++i) {
            TS_ASSERT(strObj.Top(currElemOrig));
            TS_ASSERT(strObj.Pop());
            TS_ASSERT(copyObj.Top(currElemCopy));
            TS_ASSERT(copyObj.Pop());
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

        StackString strObj;
        TS_ASSERT(strObj.Push(std::string("one")));
        TS_ASSERT(strObj.Push(std::string("two")));
        TS_ASSERT(strObj.Push(std::string("three")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 3);

        StackString copyObj;
        copyObj = strObj;

        TS_ASSERT(!copyObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), copyObj.Size());

        std::string currElemOrig, currElemCopy;

        for(U32 i(0); i < 3; ++i) {
            TS_ASSERT(strObj.Top(currElemOrig));
            TS_ASSERT(strObj.Pop());
            TS_ASSERT(copyObj.Top(currElemCopy));
            TS_ASSERT(copyObj.Pop());
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

        StackString strObj;

        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 0);

        TS_ASSERT(strObj.Push(std::string("one")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 1);

        TS_ASSERT(strObj.Push(std::string("two")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 2);

        TS_ASSERT(strObj.Push(std::string("three")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 3);

        TS_ASSERT(strObj.Pop());

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 2);

        TS_ASSERT(strObj.Pop());

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 1);

        TS_ASSERT(strObj.Pop());

        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 0);
    };

    // ////////////////////////////////////////////////////////////
    //
    //
    // ////////////////////////////////////////////////////////////
    void testPushAndPop(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        StackString strObj;
        TS_ASSERT(strObj.Push(std::string("one")));
        TS_ASSERT(strObj.Push(std::string("two")));
        TS_ASSERT(strObj.Push(std::string("three")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 3);

        std::string currElem;
        TS_ASSERT(strObj.Top(currElem));
        TS_ASSERT_EQUALS(currElem.compare("three"), 0);
        TS_ASSERT(strObj.Pop());
        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 2);

        TS_ASSERT(strObj.Top(currElem));
        TS_ASSERT_EQUALS(currElem.compare("two"), 0);
        TS_ASSERT(strObj.Pop());
        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 1);

        TS_ASSERT(strObj.Top(currElem));
        TS_ASSERT_EQUALS(currElem.compare("one"), 0);
        TS_ASSERT(strObj.Pop());
        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 0);

        std::string copyElem(currElem);
        TS_ASSERT(!strObj.Top(currElem));
        TS_ASSERT_EQUALS(copyElem.compare(currElem.c_str()), 0);
        TS_ASSERT(!strObj.Pop());
    };

    // ////////////////////////////////////////////////////////////
    //
    //
    // ////////////////////////////////////////////////////////////
    void testTopAndSetTop(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        StackString strObj;

        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 0);

        std::string topElem;

        TS_ASSERT(!strObj.Top(topElem));

        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 0);

        TS_ASSERT(strObj.SetTop(std::string("newtop")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 1);

        TS_ASSERT(strObj.Top(topElem));
        TS_ASSERT_EQUALS(topElem.compare("newtop"), 0);
        TS_ASSERT(strObj.Pop());

        TS_ASSERT(strObj.Push(std::string("one")));
        TS_ASSERT(strObj.Push(std::string("two")));
        TS_ASSERT(strObj.Push(std::string("three")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 3);

        TS_ASSERT(strObj.Top(topElem));
        TS_ASSERT_EQUALS(topElem.compare("three"), 0);
    };

    // ////////////////////////////////////////////////////////////
    //
    //
    // ////////////////////////////////////////////////////////////
    void testClear(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        StackString strObj;

        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 0);

        TS_ASSERT(strObj.Push(std::string("one")));
        TS_ASSERT(strObj.Push(std::string("two")));
        TS_ASSERT(strObj.Push(std::string("three")));

        TS_ASSERT(!strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 3);

        strObj.Clear();

        TS_ASSERT(strObj.IsEmpty());
        TS_ASSERT_EQUALS(strObj.Size(), 0);
    };

};

#endif
