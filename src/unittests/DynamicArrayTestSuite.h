#pragma once
#ifndef __DYNAMIC_ARRAY_TEST_SUITE_H
#define __DYNAMIC_ARRAY_TEST_SUITE_H

// /////////////////////////////////////////////////////////////////
// @file DynamicArrayTestSuite.h
// @author PJ O Halloran
// @date 30/03/2011
//
// File contains the header for the DynamicArray Test Suite.
//
// /////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#include <cxxtest/TestSuite.h>

#include "DynamicArray.h"

using std::cout;
using std::string;
using std::endl;

// /////////////////////////////////////////////////////////////////
// @class DynamicArrayTestSuite
// @author PJ O Halloran
//
// This class defines a series of unit tests for the DynamicArray
// class.
//
// /////////////////////////////////////////////////////////////////
class DynamicArrayTestSuite : public CxxTest::TestSuite {
private:

    typedef GameHalloran::DynamicArray<int> DynamicArrayInt;
    typedef GameHalloran::DynamicArray<int>::Iterator<int> IteratorInt;
    typedef GameHalloran::DynamicArray<std::string> DynamicArrayString;
    typedef GameHalloran::DynamicArray<std::string>::Iterator<std::string> IteratorString;
    typedef GameHalloran::DynamicArray<int *> DynamicArrayIntPointer;
    typedef GameHalloran::DynamicArray<int *>::Iterator<int *> IteratorPointer;
    typedef GameHalloran::U32 U32;

    DynamicArrayInt *m_testPtr;
    DynamicArrayString *m_testStrPtr;
    DynamicArrayIntPointer *m_testArrPtr;

    bool IsTestDataReady() {
        return (NULL != m_testPtr && NULL != m_testStrPtr && NULL != m_testArrPtr);
    };

public:

    // /////////////////////////////////////////////////////////////////
    // Constructor.
    //
    // /////////////////////////////////////////////////////////////////
    DynamicArrayTestSuite() : CxxTest::TestSuite(), m_testPtr(NULL) {

    };

    // /////////////////////////////////////////////////////////////////
    // Destructor.
    //
    // /////////////////////////////////////////////////////////////////
    ~DynamicArrayTestSuite() {

    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void setUp() {
        m_testPtr = new DynamicArrayInt;
        m_testStrPtr = new DynamicArrayString;
        m_testArrPtr = new DynamicArrayIntPointer;
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

        // 1) Create array of ints, objects and pointers
        TS_ASSERT_THROWS_NOTHING(DynamicArrayInt intArray);
        TS_ASSERT_THROWS_NOTHING(DynamicArrayString strArray);
        TS_ASSERT_THROWS_NOTHING(DynamicArrayIntPointer ptrArray);

        // 2) Create int array, test initial state is ok.
        DynamicArrayInt intArray;
        TS_ASSERT_EQUALS(intArray.GetSize(), 0);
        TS_ASSERT(intArray.GetCapacity() > 0);

        // 2) Create object array, test initial state is ok.
        TS_ASSERT_THROWS_NOTHING(DynamicArrayString strArray);
        DynamicArrayString strArray;
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);
        TS_ASSERT(strArray.GetCapacity() > 0);

        // 2) Create pointer array, test initial state is ok.
        TS_ASSERT_THROWS_NOTHING(DynamicArrayIntPointer ptrArray);
        DynamicArrayIntPointer ptrArray;
        TS_ASSERT_EQUALS(ptrArray.GetSize(), 0);
        TS_ASSERT(ptrArray.GetCapacity() > 0);
    };

    // ////////////////////////////////////////////////////////////
    //
    //
    // ////////////////////////////////////////////////////////////
    void testConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // 1) Create array of ints, objects and pointers with size constructor.
        TS_ASSERT_THROWS_NOTHING(DynamicArrayInt intArray(100));
        TS_ASSERT_THROWS_NOTHING(DynamicArrayString strArray(100));
        TS_ASSERT_THROWS_NOTHING(DynamicArrayIntPointer ptrArray(100));

        // 2) Test initial state is as expected.
        DynamicArrayInt intArray(100);
        TS_ASSERT_EQUALS(intArray.GetSize(), 0);
        TS_ASSERT_EQUALS(intArray.GetCapacity(), 100);

        DynamicArrayString strArray(100);
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 100);

        DynamicArrayIntPointer ptrArray(100);
        TS_ASSERT_EQUALS(ptrArray.GetSize(), 0);
        TS_ASSERT_EQUALS(ptrArray.GetCapacity(), 100);
    };

    // ////////////////////////////////////////////////////////////
    //
    //
    // ////////////////////////////////////////////////////////////
    void testValueConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // 1) Create array of ints, objects and pointers with size constructor.
        TS_ASSERT_THROWS_NOTHING(DynamicArrayInt intArray(100, 1));
        TS_ASSERT_THROWS_NOTHING(DynamicArrayString strArray(100, std::string("defaultval")));
        TS_ASSERT_THROWS_NOTHING(DynamicArrayIntPointer ptrArray(100, NULL));

        // 2) Test initial state is as expected.
        DynamicArrayInt intArray(100, 1);
        TS_ASSERT_EQUALS(intArray.GetSize(), 100);
        TS_ASSERT_EQUALS(intArray.GetCapacity(), 100 * 2);
        for(U32 i(0); i < intArray.GetSize(); ++i) {
            TS_ASSERT_EQUALS(intArray[i], 1);
        }

        DynamicArrayString strArray(100, std::string("defaultval"));
        TS_ASSERT_EQUALS(strArray.GetSize(), 100);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 100 * 2);
        for(U32 i(0); i < strArray.GetSize(); ++i) {
            TS_ASSERT_EQUALS(strArray[i].compare("defaultval"), 0);
        }

        DynamicArrayIntPointer ptrArray(100, NULL);
        TS_ASSERT_EQUALS(ptrArray.GetSize(), 100);
        TS_ASSERT_EQUALS(ptrArray.GetCapacity(), 100 * 2);
        for(U32 i(0); i < ptrArray.GetSize(); ++i) {
            TS_ASSERT(!ptrArray[i]);
        }
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testCopyConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // 1) Create test int, string and pointer array data.
        DynamicArrayInt objInt(100);
        int intArr[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
        const int iSize = 10;
        for(int i = 0; i < iSize; ++i) {
            objInt.PushBack(intArr[i]);
        }
        DynamicArrayString objStr(100);
        std::string strArr[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
        const int sSize = 10;
        for(int i = 0; i < sSize; ++i) {
            objStr.PushBack(strArr[i]);
        }
        DynamicArrayIntPointer objPtr(100);
        int *ptrArr[] = {&intArr[0], &intArr[1], &intArr[2], &intArr[3], &intArr[4], &intArr[5], &intArr[6], &intArr[7], &intArr[8], &intArr[9]};
        const int pSize = 10;
        for(int i = 0; i < pSize; ++i) {
            objPtr.PushBack(ptrArr[i]);
        }

        // 2) Int DyArr...

        // 2a) Test int copy constructor.
        TS_ASSERT_THROWS_NOTHING(DynamicArrayInt intArray(objInt));
        DynamicArrayInt intArray(objInt);
        TS_ASSERT_EQUALS(intArray.GetSize(), iSize);
        TS_ASSERT_EQUALS(intArray.GetSize(), objInt.GetSize());
        TS_ASSERT_EQUALS(intArray.GetCapacity(), objInt.GetCapacity());

        // 2b) Test all elements are as expected.
        for(U32 i = 0; i < intArray.GetSize(); ++i) {
            TS_ASSERT_EQUALS(intArray[i], intArr[i]);
        }

        // 2c) Test we can use the iterators to browse array contents.
        U32 index = 0;
        for(IteratorInt i = intArray.Begin(), end = intArray.End(); i != end; ++i, ++index) {
            TS_ASSERT_EQUALS(*i, intArr[index]);
        }
        IteratorInt testI = intArray.Begin();
        TS_ASSERT_EQUALS(*testI, intArr[0]);
        TS_ASSERT_EQUALS(*(testI + 1), intArr[1]);
        TS_ASSERT_EQUALS(*(testI + 5), intArr[5]);
        TS_ASSERT_EQUALS(*(testI + 9), intArr[9]);

        // 3) object DyArr...

        // 3a) Test object copy constructor.
        TS_ASSERT_THROWS_NOTHING(DynamicArrayString strArray(objStr));
        DynamicArrayString strArray(objStr);
        TS_ASSERT_EQUALS(strArray.GetSize(), sSize);
        TS_ASSERT_EQUALS(strArray.GetSize(), objStr.GetSize());
        TS_ASSERT_EQUALS(strArray.GetCapacity(), objStr.GetCapacity());

        // 3b) Test all elements are as expected.
        for(U32 i = 0; i < strArray.GetSize(); ++i) {
            TS_ASSERT_EQUALS(strArray[i].compare(strArr[i]), 0);
        }

        // 3c) Test we can use the iterators to browse array contents.
        index = 0;
        for(IteratorString i = strArray.Begin(), end = strArray.End(); i != end; ++i, ++index) {
            TS_ASSERT_EQUALS((*i).compare(strArr[index]), 0);
        }
        IteratorString testS = strArray.Begin();
        TS_ASSERT_EQUALS((*testS).compare(strArr[0]), 0);
        TS_ASSERT_EQUALS((*(testS + 1)).compare(strArr[1]), 0);
        TS_ASSERT_EQUALS((*(testS + 5)).compare(strArr[5]), 0);
        TS_ASSERT_EQUALS((*(testS + 9)).compare(strArr[9]), 0);

        // 4) Pointer DyArr...

        // 4a) Test copy constructor.
        TS_ASSERT_THROWS_NOTHING(DynamicArrayIntPointer ptrArray(objPtr));
        DynamicArrayIntPointer ptrArray(objPtr);
        TS_ASSERT_EQUALS(ptrArray.GetSize(), pSize);
        TS_ASSERT_EQUALS(ptrArray.GetSize(), objPtr.GetSize());
        TS_ASSERT_EQUALS(ptrArray.GetCapacity(), objPtr.GetCapacity());

        // 4b) Test all elements are as expected.
        for(U32 i = 0; i < ptrArray.GetSize(); ++i) {
            TS_ASSERT_EQUALS(ptrArray[i], ptrArr[i]);
        }

        // 4c) Test we can use the iterators to browse array contents.
        index = 0;
        for(IteratorPointer i = ptrArray.Begin(), end = ptrArray.End(); i != end; ++i, ++index) {
            TS_ASSERT_EQUALS(*i, ptrArr[index]);
        }
        IteratorPointer testP = ptrArray.Begin();
        TS_ASSERT_EQUALS(*testP, ptrArr[0]);
        TS_ASSERT_EQUALS(*(testP + 1), ptrArr[1]);
        TS_ASSERT_EQUALS(*(testP + 5), ptrArr[5]);
        TS_ASSERT_EQUALS(*(testP + 9), ptrArr[9]);
    };

    // ////////////////////////////////////////////////////////////
    // Test the iterator range constructor.
    //
    // ////////////////////////////////////////////////////////////
    void testIteratorRangeConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        DynamicArrayInt obj(100);
        int arr[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
        const int s = 10;
        for(int i = 0; i < s; ++i) {
            obj.PushBack(arr[i]);
        }

        TS_ASSERT_THROWS_NOTHING(DynamicArrayInt intArray(obj.Begin(), obj.End()));
        DynamicArrayInt intArray(obj.Begin(), obj.End());
        TS_ASSERT_EQUALS(intArray.GetSize(), s);
        TS_ASSERT_EQUALS(intArray.GetCapacity(), s * 2);
        for(int i = 0; i < s; ++i) {
            TS_ASSERT_EQUALS(intArray[i], arr[i]);
        }
    };

    // ////////////////////////////////////////////////////////////
    // Test the C array copy constructor.
    //
    // ////////////////////////////////////////////////////////////
    void testArrayConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        int arr[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
        const int s = 10;

        TS_ASSERT_THROWS_NOTHING(DynamicArrayInt intArray(arr, s));
        DynamicArrayInt intArray(arr, s);
        TS_ASSERT_EQUALS(intArray.GetSize(), s);
        TS_ASSERT_EQUALS(intArray.GetCapacity(), s * 2);
        for(int i = 0; i < s; ++i) {
            TS_ASSERT_EQUALS(intArray[i], arr[i]);
        }
    };

    // ////////////////////////////////////////////////////////////
    // Test the = operator.
    //
    // ////////////////////////////////////////////////////////////
    void testOperatorAssignment(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        DynamicArrayInt obj(100);

        TS_ASSERT(obj.IsEmpty());

        int arr[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
        const int s = 10;
        for(int i = 0; i < s; ++i) {
            obj.PushBack(arr[i]);
        }

        TS_ASSERT_EQUALS(obj.GetCapacity(), 100);
        TS_ASSERT_EQUALS(obj.GetSize(), s);
        TS_ASSERT(!obj.IsEmpty());

        for(int i = 0; i < s; ++i) {
            TS_ASSERT_EQUALS(obj[i], arr[i]);
        }

        DynamicArrayInt objCopy;

        TS_ASSERT(objCopy.IsEmpty());
        TS_ASSERT(objCopy.GetCapacity() > 0);
        TS_ASSERT_EQUALS(objCopy.GetSize(), 0);

        objCopy = obj;

        TS_ASSERT(!objCopy.IsEmpty());
        TS_ASSERT_EQUALS(objCopy.GetCapacity(), obj.GetCapacity());
        TS_ASSERT_EQUALS(objCopy.GetSize(), obj.GetSize());
        TS_ASSERT_EQUALS(objCopy.GetCapacity(), 100);
        TS_ASSERT_EQUALS(objCopy.GetSize(), s);

        for(int i = 0; i < s; ++i) {
            TS_ASSERT_EQUALS(obj[i], arr[i]);
            TS_ASSERT_EQUALS(obj[i], objCopy[i]);
        }
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testBegin(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        DynamicArrayString strArray(50);

        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 50);
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);

        strArray.PushBack(std::string("element 1"));
        strArray.PushBack(std::string("element 2"));
        strArray.PushBack(std::string("element 3 "));
        strArray.PushBack(std::string("element 4   "));
        strArray.PushBack(std::string("element 5"));

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 50);
        TS_ASSERT_EQUALS(strArray.GetSize(), 5);

        IteratorString iter = strArray.Begin();

        // Test its a valid pointer.
        TS_ASSERT(!(!iter));
        TS_ASSERT_EQUALS(iter->compare("element 1"), 0);
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testEnd(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        DynamicArrayString strArray(50);

        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 50);
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);

        strArray.PushBack(std::string("element 1"));
        strArray.PushBack(std::string("element 2"));
        strArray.PushBack(std::string("element 3 "));
        strArray.PushBack(std::string("element 4   "));
        strArray.PushBack(std::string("element 5"));

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 50);
        TS_ASSERT_EQUALS(strArray.GetSize(), 5);

        IteratorString iter = strArray.End();

        // Test its not a valid pointer.
        TS_ASSERT(!iter);
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testGetAndSetCapacity(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        DynamicArrayString strArray;
        U32 initialCapacity(strArray.GetCapacity());
        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetCapacity(), initialCapacity);
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);

        for(U32 i = 0; i < initialCapacity + 1; ++i) {
            strArray.PushBack(std::string("test"));
        }

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetCapacity(), initialCapacity * 2);
        TS_ASSERT_EQUALS(strArray.GetSize(), initialCapacity + 1);

        strArray.SetCapacity(5);

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetCapacity(), initialCapacity * 2);
        TS_ASSERT_EQUALS(strArray.GetSize(), 5);

        strArray.SetCapacity(initialCapacity * 2 * 2);

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetCapacity(), initialCapacity * 2 * 2);
        TS_ASSERT_EQUALS(strArray.GetSize(), 5);
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testGetAndSetSize(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        DynamicArrayString strArray;
        U32 initialCapacity(strArray.GetCapacity());

        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetCapacity(), initialCapacity);
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);

        strArray.SetSize(initialCapacity - 1);

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetCapacity(), initialCapacity);
        TS_ASSERT_EQUALS(strArray.GetSize(), initialCapacity - 1);

        strArray.SetSize(initialCapacity * 2);

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetCapacity(), initialCapacity * 4);
        TS_ASSERT_EQUALS(strArray.GetSize(), initialCapacity * 2);
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testPushBack(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        DynamicArrayString strArray;

        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);

        U32 initialCapacity(strArray.GetCapacity());

        for(U32 i = 0; i < 50; ++i) {
            strArray.PushBack(std::string("test"));
        }

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 50);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), initialCapacity * 2 * 2 * 2);
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testPopBack(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        DynamicArrayString strArray;

        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);

        U32 iC(strArray.GetCapacity());

        for(U32 i = 0; i < 50; ++i) {
            strArray.PushBack(std::string("test"));
        }

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 50);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), iC * 2 * 2 * 2);

        for(U32 i = 0; i < 10; ++i) {
            strArray.PopBack();
        }

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 40);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), iC * 2 * 2 * 2);

        for(U32 i = 0; i < 10; ++i) {
            strArray.PopBack();
        }

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 30);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), iC * 2 * 2 * 2);

        for(U32 i = 0; i < 10; ++i) {
            strArray.PopBack();
        }

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 20);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), iC * 2 * 2 * 2);

        for(U32 i = 0; i < 10; ++i) {
            strArray.PopBack();
        }

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 10);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), iC * 2 * 2 * 2);

        for(U32 i = 0; i < 10; ++i) {
            strArray.PopBack();
        }

        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), iC * 2 * 2 * 2);

        for(U32 i = 0; i < 10; ++i) {
            strArray.PopBack();
        }

        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), iC * 2 * 2 * 2);
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testBackAndFront(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        DynamicArrayString strArray;

        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);

        U32 iC(strArray.GetCapacity());

        std::string fStr("FrontString");
        strArray.PushBack(fStr);

        for(U32 i = 0; i < 50; ++i) {
            strArray.PushBack(std::string("test"));
        }

        std::string lStr("LastString");
        strArray.PushBack(lStr);

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 50 + 2);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), iC * 2 * 2 * 2);

        //TS_WARN(strArray.Front().c_str());
        //TS_WARN(strArray.Back().c_str());

        TS_ASSERT_EQUALS(strArray.Front().compare(fStr.c_str()), 0);
        TS_ASSERT_EQUALS(strArray.Back().compare(lStr.c_str()), 0);

        for(U32 i = 0; i < 10; ++i) {
            strArray.PushBack(std::string("test"));
        }

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 60 + 2);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), iC * 2 * 2 * 2);

        lStr = std::string("NewLastString");
        strArray.PushBack(lStr);

        TS_ASSERT_EQUALS(strArray.Front().compare(fStr.c_str()), 0);
        TS_ASSERT_EQUALS(strArray.Back().compare(lStr.c_str()), 0);
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testAt(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        DynamicArrayString strArray(50);

        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 50);

        const std::string elem("new element");
        for(U32 i(0); i < 10; ++i) {
            strArray.PushBack(elem);
        }

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 10);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 50);

        // In bounds access
        std::string currVal;
        for(U32 i(0); i < 10; ++i) {
            TS_ASSERT(strArray.At(i, currVal));
            TS_ASSERT_EQUALS(currVal.compare(elem.c_str()), 0);
        }

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 10);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 50);

        // Out of bounds access
        TS_ASSERT(!strArray.At(10, currVal));
        TS_ASSERT(!strArray.At(20, currVal));
        TS_ASSERT(!strArray.At(30, currVal));
    };

    // ////////////////////////////////////////////////////////////
    // Test the operator[].
    //
    // ////////////////////////////////////////////////////////////
    void testOperatorArrayAccess(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        DynamicArrayString strArray(50);

        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 50);

        const std::string elem("new element");
        for(U32 i(0); i < 10; ++i) {
            strArray.PushBack(elem);
        }

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 10);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 50);

        // 1) Read access using int indices.
        for(U32 i(0); i < 10; ++i) {
            TS_ASSERT_EQUALS(strArray[i].compare(elem.c_str()), 0);
        }

        // 2) Read access using iterators.
        for(IteratorString i = strArray.Begin(), end = strArray.End(); i != end; ++i) {
            TS_ASSERT_EQUALS((*i).compare(elem.c_str()), 0);
        }

        // 3) Write access using int indices.
        const std::string testWriteVal("TESTTEST");
        strArray[4] = testWriteVal;

        // Check write was successfull using int indices
        TS_ASSERT_EQUALS(strArray[0].compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[1].compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[2].compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[3].compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[4].compare(testWriteVal.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[5].compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[6].compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[7].compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[8].compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[9].compare(elem.c_str()), 0);

        // Check write was successfull using iterator access.
        IteratorString begin(strArray.Begin());
        TS_ASSERT_EQUALS((*begin).compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS((*(begin + 1)).compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS((*(begin + 2)).compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS((*(begin + 3)).compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS((*(begin + 4)).compare(testWriteVal.c_str()), 0);
        TS_ASSERT_EQUALS((*(begin + 5)).compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS((*(begin + 6)).compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS((*(begin + 7)).compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS((*(begin + 8)).compare(elem.c_str()), 0);
        TS_ASSERT_EQUALS((*(begin + 9)).compare(elem.c_str()), 0);
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testClear(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        DynamicArrayString strArray(1000);

        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        const std::string val("value");
        for(U32 i(0); i < 10; ++i) {
            strArray.PushBack(val);
        }

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 10);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        // Test we can access all inserted elements.
        std::string currVal;
        for(U32 i(0); i < strArray.GetSize(); ++i) {
            TS_ASSERT(strArray.At(i, currVal));
        }

        // Clear the array.
        TS_ASSERT_THROWS_NOTHING(strArray.Clear());

        // Test that now no elements are in the array and accesses fail.
        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);
        for(U32 i(0); i < 10; ++i) {
            TS_ASSERT(!strArray.At(i, currVal));
        }
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testInsert(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        DynamicArrayString strArray(1000), initialStateArr(1000);

        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        const std::string val("value"), insertedValue("INSERT_ELEMENT"), testVal("TestVal");

        // Fill array with default values.
        for(U32 i(0); i < 10; ++i) {
            strArray.PushBack(val);
        }
        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 10);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        // Save initial state of the array so we can reset it between tests...
        initialStateArr = strArray;

        // 1) Insert value at the beginning.
        // Put marker at start to test if its moved properly as in the Insert() documentation.
        strArray[0] = testVal;
        TS_ASSERT(strArray.Insert(insertedValue, strArray.Begin()));

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 11);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        // Test value was inserted to the correct location as well as the
        //  previous element being moved to the end while the other elements
        //   are all identical to before.
        TS_ASSERT_EQUALS(strArray[0].compare(insertedValue.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[1].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[2].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[3].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[4].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[5].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[6].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[7].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[8].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[9].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[10].compare(testVal.c_str()), 0);

        // Clear array and fill it with test data again for the next test.
        strArray = initialStateArr;


        // 2) Insert value at the end.
        TS_ASSERT(strArray.Insert(insertedValue, strArray.End()));

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 11);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        // Test value was inserted to the correct location as well as the
        //  previous element being moved to the end while the other elements
        //   are all identical to before.
        TS_ASSERT_EQUALS(strArray[0].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[1].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[2].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[3].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[4].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[5].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[6].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[7].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[8].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[9].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[10].compare(insertedValue.c_str()), 0);

        // Clear array and fill it with test data again for the next test.
        strArray = initialStateArr;

        // 3) Insert value somewhere in the middle.
        // Put marker at location to test if its moved properly as in the Insert() documentation.
        strArray[4] = testVal;
        TS_ASSERT(strArray.Insert(insertedValue, strArray.Begin() + 4));

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 11);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        // Test value was inserted to the correct location as well as the
        //  previous element being moved to the end while the other elements
        //   are all identical to before.
        TS_ASSERT_EQUALS(strArray[0].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[1].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[2].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[3].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[4].compare(insertedValue.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[5].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[6].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[7].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[8].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[9].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[10].compare(testVal.c_str()), 0);

        // Clear array and fill it with test data again for the next test.
        strArray = initialStateArr;
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testInsertAndMove(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        DynamicArrayString strArray(1000), initialStateArr(1000);

        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        const std::string val("value"), insertedValue("INSERT_ELEMENT"), testVal("TestVal");

        // Fill array with default values.
        for(U32 i(0); i < 10; ++i) {
            strArray.PushBack(val);
        }
        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 10);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        // Save initial state of the array so we can reset it between tests...
        initialStateArr = strArray;

        //for(U32 i(0); i < strArray.GetSize(); ++i)
        //{
        //  cout << strArray[i] << endl;
        //}

        // 1) Insert value at the beginning.
        // Put marker at start to test if its moved properly as in the InsertAndMove() documentation.
        strArray[0] = testVal;
        TS_ASSERT(strArray.InsertAndMove(insertedValue, strArray.Begin()));

        //for(U32 i(0); i < strArray.GetSize(); ++i)
        //{
        //  cout << strArray[i] << endl;
        //}

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 11);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        // Test value was inserted to the correct location and all other
        //  elements moved up by 1.
        TS_ASSERT_EQUALS(strArray[0].compare(insertedValue.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[1].compare(testVal.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[2].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[3].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[4].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[5].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[6].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[7].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[8].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[9].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[10].compare(val.c_str()), 0);

        // Clear array and fill it with test data again for the next test.
        strArray = initialStateArr;


        // 2) Insert value at the end.
        TS_ASSERT(strArray.InsertAndMove(insertedValue, strArray.End()));

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 11);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        // Test value was inserted to the correct location.
        TS_ASSERT_EQUALS(strArray[0].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[1].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[2].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[3].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[4].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[5].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[6].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[7].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[8].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[9].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[10].compare(insertedValue.c_str()), 0);

        // Clear array and fill it with test data again for the next test.
        strArray = initialStateArr;

        // 3) Insert value somewhere in the middle.
        // Put marker at location to test if its moved properly as in the Insert() documentation.
        strArray[4] = testVal;
        TS_ASSERT(strArray.InsertAndMove(insertedValue, strArray.Begin() + 4));

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 11);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        // Test value was inserted to the correct location as well as the
        //  other elements being moved up by 1.
        TS_ASSERT_EQUALS(strArray[0].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[1].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[2].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[3].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[4].compare(insertedValue.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[5].compare(testVal.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[6].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[7].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[8].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[9].compare(val.c_str()), 0);
        TS_ASSERT_EQUALS(strArray[10].compare(val.c_str()), 0);

        // Clear array and fill it with test data again for the next test.
        strArray = initialStateArr;
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testRemove(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        DynamicArrayString strArray(1000), initialStateArr(1000);

        TS_ASSERT(strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 0);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        std::string strArr[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
        const U32 s = 10;
        const std::string val("value");

        // Fill array with default values.
        for(U32 i(0); i < s; ++i) {
            strArray.PushBack(strArr[i]);
        }
        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), s);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        // Save initial state of the array so we can reset it between tests...
        initialStateArr = strArray;

        // 1) Test removing element at the beginning.
        TS_ASSERT(strArray.Remove(strArray.Begin()));

        //for(U32 i(0); i < strArray.GetSize(); ++i)
        //{
        //  cout << strArray[i] << endl;
        //}

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 9);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        TS_ASSERT_EQUALS(strArray[0].compare(strArr[1].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[1].compare(strArr[2].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[2].compare(strArr[3].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[3].compare(strArr[4].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[4].compare(strArr[5].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[5].compare(strArr[6].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[6].compare(strArr[7].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[7].compare(strArr[8].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[8].compare(strArr[9].c_str()), 0);

        // Reset array
        strArray = initialStateArr;

        // 2) Test removal of element somewhere in the middle of the array.
        TS_ASSERT(strArray.Remove(strArray.Begin() + 6));

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 9);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        //for(U32 i(0); i < strArray.GetSize(); ++i)
        //{
        //  cout << strArray[i] << endl;
        //}

        TS_ASSERT_EQUALS(strArray[0].compare(strArr[0].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[1].compare(strArr[1].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[2].compare(strArr[2].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[3].compare(strArr[3].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[4].compare(strArr[4].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[5].compare(strArr[5].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[6].compare(strArr[7].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[7].compare(strArr[8].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[8].compare(strArr[9].c_str()), 0);

        // Reset array
        strArray = initialStateArr;

        // 3) Test removal from the end of the array.
        TS_ASSERT(strArray.Remove(strArray.End()));

        //for(U32 i(0); i < strArray.GetSize(); ++i)
        //{
        //  cout << strArray[i] << endl;
        //}

        TS_ASSERT(!strArray.IsEmpty());
        TS_ASSERT_EQUALS(strArray.GetSize(), 9);
        TS_ASSERT_EQUALS(strArray.GetCapacity(), 1000);

        TS_ASSERT_EQUALS(strArray[0].compare(strArr[0].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[1].compare(strArr[1].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[2].compare(strArr[2].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[3].compare(strArr[3].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[4].compare(strArr[4].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[5].compare(strArr[5].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[6].compare(strArr[6].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[7].compare(strArr[7].c_str()), 0);
        TS_ASSERT_EQUALS(strArray[8].compare(strArr[8].c_str()), 0);

        // Reset array
        strArray = initialStateArr;
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testCopyRange(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Create an array with test data.
        DynamicArrayString testArray;
        std::string one("one"), two("two"), three("three"), four("four");
        testArray.PushBack(one);
        testArray.PushBack(two);
        testArray.PushBack(three);
        testArray.PushBack(four);

        TS_ASSERT(!testArray.IsEmpty());
        TS_ASSERT_EQUALS(testArray.GetSize(), 4);

        // 1) Copy all test data into empty array.
        {
            DynamicArrayString emptyArray;

            TS_ASSERT(emptyArray.IsEmpty());
            TS_ASSERT_EQUALS(emptyArray.GetSize(), 0);

            emptyArray.CopyRange(testArray.Begin(), testArray.End(), emptyArray.Begin());

            TS_ASSERT(!emptyArray.IsEmpty());
            TS_ASSERT_EQUALS(emptyArray.GetSize(), 4);

            U32 in(0);
            for(IteratorString i(emptyArray.Begin()), end(emptyArray.End()); i != end; ++i, ++in) {
                TS_ASSERT_EQUALS((*i).compare(testArray[in].c_str()), 0);
            }
        }

        // 2) Copy middle of test data into empty array.
        {
            DynamicArrayString emptyArray;

            TS_ASSERT(emptyArray.IsEmpty());
            TS_ASSERT_EQUALS(emptyArray.GetSize(), 0);

            emptyArray.CopyRange(testArray.Begin() + 1, testArray.Begin() + 2, emptyArray.Begin());

            TS_ASSERT(!emptyArray.IsEmpty());
            TS_ASSERT_EQUALS(emptyArray.GetSize(), 2);

            cout << (*emptyArray.Begin()) << endl;
            cout << (*(emptyArray.Begin() + 1)) << endl;

            TS_ASSERT_EQUALS((*emptyArray.Begin()).compare(testArray[1].c_str()), 0);
            TS_ASSERT_EQUALS((*(emptyArray.Begin() + 1)).compare(testArray[2].c_str()), 0);
        }

        // 3) Copy first half into empty array.
        {
            DynamicArrayString emptyArray;

            TS_ASSERT(emptyArray.IsEmpty());
            TS_ASSERT_EQUALS(emptyArray.GetSize(), 0);

            emptyArray.CopyRange(testArray.Begin(), testArray.Begin() + 1, emptyArray.Begin());

            TS_ASSERT(!emptyArray.IsEmpty());
            TS_ASSERT_EQUALS(emptyArray.GetSize(), 2);

            TS_ASSERT_EQUALS((*emptyArray.Begin()).compare(testArray[0].c_str()), 0);
            TS_ASSERT_EQUALS((*(emptyArray.Begin() + 1)).compare(testArray[1].c_str()), 0);
        }

        // 4) Copy last half into empty array.
        {
            DynamicArrayString emptyArray;

            TS_ASSERT(emptyArray.IsEmpty());
            TS_ASSERT_EQUALS(emptyArray.GetSize(), 0);

            emptyArray.CopyRange(testArray.Begin() + 2, testArray.Begin() + 3, emptyArray.Begin());

            TS_ASSERT(!emptyArray.IsEmpty());
            TS_ASSERT_EQUALS(emptyArray.GetSize(), 2);

            TS_ASSERT_EQUALS((*emptyArray.Begin()).compare(testArray[2].c_str()), 0);
            TS_ASSERT_EQUALS((*(emptyArray.Begin() + 1)).compare(testArray[3].c_str()), 0);
        }

        // Create testDest array with some data.

        // 5) Copy all test data into array at beginning.

        // 6) Copy all test data into array at end.

        // 7) Copy all test data into array somewhere in the middle.

        // Self copy tests.

        // Try to copy entire test data array into itself at the end
        {
            testArray.CopyRange(testArray.Begin(), testArray.End(), testArray.End());

            TS_ASSERT(!testArray.IsEmpty());
            TS_ASSERT_EQUALS(testArray.GetSize(), 8);
        }
    };

};

#endif
