#pragma once
#ifndef __LINKED_LIST_TEST_SUITE_H
#define __LINKED_LIST_TEST_SUITE_H

// /////////////////////////////////////////////////////////////////
// @file LinkedListTestSuite.h
// @author PJ O Halloran
// @date 30/03/2011
//
// File contains the header for the LinkedList Test Suite.
//
// /////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#include <cxxtest/TestSuite.h>

#include "LinkedList.h"

using std::cout;
using std::string;
using std::endl;

// /////////////////////////////////////////////////////////////////
// @class LinkedListTestSuite
// @author PJ O Halloran
//
// This class defines a series of unit tests for the LinkedList
// class.
//
// /////////////////////////////////////////////////////////////////
class LinkedListTestSuite : public CxxTest::TestSuite {
private:

    typedef GameHalloran::LinkedList<int> LinkedListInt;
    typedef GameHalloran::LinkedList<int>::Iterator<int> IteratorInt;
    typedef GameHalloran::LinkedList<std::string> LinkedListString;
    typedef GameHalloran::LinkedList<std::string>::Iterator<std::string> IteratorString;
    typedef GameHalloran::LinkedList<int *> LinkedListIntPointer;
    typedef GameHalloran::LinkedList<int *>::Iterator<int *> IteratorPointer;
    typedef GameHalloran::U32 U32;

    LinkedListInt *m_testPtr;
    LinkedListString *m_testStrPtr;
    LinkedListIntPointer *m_testArrPtr;

    bool IsTestDataReady() {
        return (NULL != m_testPtr && NULL != m_testStrPtr && NULL != m_testArrPtr);
    };

public:

    // /////////////////////////////////////////////////////////////////
    // Constructor.
    //
    // /////////////////////////////////////////////////////////////////
    LinkedListTestSuite() : CxxTest::TestSuite(), m_testPtr(NULL) {

    };

    // /////////////////////////////////////////////////////////////////
    // Destructor.
    //
    // /////////////////////////////////////////////////////////////////
    ~LinkedListTestSuite() {

    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void setUp() {
        m_testPtr = new LinkedListInt;
        m_testStrPtr = new LinkedListString;
        m_testArrPtr = new LinkedListIntPointer;
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

        LinkedListInt intList;
        TS_ASSERT(intList.IsEmpty());
        IteratorInt beginInt(intList.Begin()), endInt(intList.End());
        TS_ASSERT(beginInt == endInt);
        TS_ASSERT_EQUALS(intList.Size(), 0);
        int tmpInt(0);
        TS_ASSERT(!intList.Back(tmpInt));
        TS_ASSERT(!intList.Front(tmpInt));
        TS_ASSERT(!intList.PopBack());
        TS_ASSERT(!intList.PopFront());

        LinkedListString strList;
        TS_ASSERT(strList.IsEmpty());
        IteratorString beginStr(strList.Begin()), endStr(strList.End());
        TS_ASSERT(beginStr == endStr);
        TS_ASSERT_EQUALS(strList.Size(), 0);
        std::string tmpStr;
        TS_ASSERT(!strList.Back(tmpStr));
        TS_ASSERT(!strList.Front(tmpStr));
        TS_ASSERT(!strList.PopBack());
        TS_ASSERT(!strList.PopFront());

        LinkedListIntPointer intPtrList;
        TS_ASSERT(intPtrList.IsEmpty());
        IteratorPointer beginPtrInt(intPtrList.Begin()), endPtrInt(intPtrList.End());
        TS_ASSERT(beginPtrInt == endPtrInt);
        TS_ASSERT_EQUALS(intPtrList.Size(), 0);
        int *tmpIntPtr(0);
        TS_ASSERT(!intPtrList.Back(tmpIntPtr));
        TS_ASSERT(!intPtrList.Front(tmpIntPtr));
        TS_ASSERT(!intPtrList.PopBack());
        TS_ASSERT(!intPtrList.PopFront());
    };

    // ////////////////////////////////////////////////////////////
    //
    //
    // ////////////////////////////////////////////////////////////
    void testCopyConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Create a test linked list with some data.
        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int size = 10;
        LinkedListInt intList;
        for(int i(0); i < size; ++i) {
            intList.PushBack(arr[i]);
        }
        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), size);

        // Create list from copy constructor.
        LinkedListInt copyObj(intList);

        // Test lists are identical.
        TS_ASSERT(!copyObj.IsEmpty());
        TS_ASSERT_EQUALS(copyObj.Size(), size);
        int index(0);
        for(IteratorInt i = copyObj.Begin(), end = copyObj.End(); i != end; ++i, ++index) {
            TS_ASSERT_EQUALS(*i, arr[index]);
        }
    };

    // ////////////////////////////////////////////////////////////
    //
    //
    // ////////////////////////////////////////////////////////////
    void testIteratorConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Create a test linked list with some data.
        LinkedListString strList;
        std::string strArr[] = {"a", "b", "c", "d", "e"};
        const U32 size = 5;
        for(U32 i(0); i < size; ++i) {
            strList.PushBack(strArr[i]);
        }

        // Create list from iter constructor.
        {
            LinkedListString iterList(strList.Begin(), strList.End());

            // Test lists are identical.
            TS_ASSERT_EQUALS(iterList.Size(), strList.Size());
            TS_ASSERT_EQUALS(iterList.IsEmpty(), strList.IsEmpty());
            IteratorString currStrI = strList.Begin();
            IteratorString endStr = strList.End();
            for(IteratorString i = iterList.Begin(), end = iterList.End(); ((i != end) && (currStrI != endStr)); ++i, ++currStrI) {
                TS_ASSERT_EQUALS((*i).compare((*currStrI).c_str()), 0);
            }
        }

        // Create list from subset of data.
        // Test lists are identical.
        {
            LinkedListString iterList(strList.Begin() + 1, strList.Begin() + 3);

            // Test lists are identical.
            TS_ASSERT_EQUALS(iterList.Size(), 2);
            TS_ASSERT_EQUALS(iterList.IsEmpty(), strList.IsEmpty());
            IteratorString currStrI = strList.Begin() + 1;
            IteratorString endStr = strList.Begin() + 3;
            for(IteratorString i = iterList.Begin(), end = iterList.End(); ((i != end) && (currStrI != endStr)); ++i, ++currStrI) {
                TS_ASSERT_EQUALS((*i).compare((*currStrI).c_str()), 0);
            }
        }
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testAssignmentOperator(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Create a test linked list with some data.
        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int size = 10;
        LinkedListInt intList;
        for(int i(0); i < size; ++i) {
            intList.PushBack(arr[i]);
        }
        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), size);

        // Create list from assignment operator.
        LinkedListInt copyObj;
        copyObj = intList;

        // Test lists are identical.
        TS_ASSERT(!copyObj.IsEmpty());
        TS_ASSERT_EQUALS(copyObj.Size(), size);
        int index(0);
        for(IteratorInt i = copyObj.Begin(), end = copyObj.End(); i != end; ++i, ++index) {
            TS_ASSERT_EQUALS(*i, arr[index]);
        }
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testSize(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int size = 10;
        LinkedListInt intList;

        TS_ASSERT_EQUALS(intList.Size(), 0);

        for(int i(0); i < size; ++i) {
            intList.PushBack(arr[i]);
            TS_ASSERT_EQUALS(intList.Size(), i + 1);
        }

        TS_ASSERT_EQUALS(intList.Size(), size);

        TS_ASSERT(intList.PopBack());
        TS_ASSERT_EQUALS(intList.Size(), size - 1);

        TS_ASSERT(intList.PopFront());
        TS_ASSERT_EQUALS(intList.Size(), size - 2);

        intList.Insert(76, intList.Begin() + 6);
        TS_ASSERT_EQUALS(intList.Size(), size - 1);
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testIsEmpty(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int size = 10;
        LinkedListInt intList;

        TS_ASSERT(intList.IsEmpty());

        for(int i(0); i < size; ++i) {
            intList.PushBack(arr[i]);
            TS_ASSERT(!intList.IsEmpty());
        }

        for(int i(0); i < size - 1; ++i) {
            TS_ASSERT(intList.PopBack());
            TS_ASSERT(!intList.IsEmpty());
        }

        TS_ASSERT(intList.PopBack());
        TS_ASSERT(intList.IsEmpty());

        TS_ASSERT(!intList.PopBack());
        TS_ASSERT(intList.IsEmpty());
    };

    // ////////////////////////////////////////////////////////////
    //
    //
    // ////////////////////////////////////////////////////////////
    void testBeginAndEnd(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Create a test linked list with some data.
        LinkedListString strList;

        // Test the end iterator always points at nothing.
        IteratorString end = strList.End();
        TS_ASSERT(!end);

        // For an empty list, begin equals end iter.
        IteratorString begin = strList.Begin();
        TS_ASSERT(!begin);
        TS_ASSERT(begin == end);

        // Add data to list.
        std::string strArr[] = {"a", "b", "c", "d", "e"};
        const U32 size = 5;
        for(U32 i(0); i < size; ++i) {
            strList.PushBack(strArr[i]);
        }

        // Test the end iterator always points at nothing.
        end = strList.End();
        TS_ASSERT(!end);

        begin = strList.Begin();
        TS_ASSERT(!(!begin));
        TS_ASSERT(begin != end);
        TS_ASSERT_EQUALS((*begin).compare(strArr[0].c_str()), 0);

        ++begin;
        TS_ASSERT(!(!begin));
        TS_ASSERT(begin != end);
        TS_ASSERT_EQUALS((*begin).compare(strArr[1].c_str()), 0);

        ++begin;
        TS_ASSERT(!(!begin));
        TS_ASSERT(begin != end);
        TS_ASSERT_EQUALS((*begin).compare(strArr[2].c_str()), 0);

        ++begin;
        TS_ASSERT(!(!begin));
        TS_ASSERT(begin != end);
        TS_ASSERT_EQUALS((*begin).compare(strArr[3].c_str()), 0);

        ++begin;
        TS_ASSERT(!(!begin));
        TS_ASSERT(begin != end);
        TS_ASSERT_EQUALS((*begin).compare(strArr[4].c_str()), 0);

        ++begin;
        TS_ASSERT(!begin);
        TS_ASSERT(begin == end);
    };

    // ////////////////////////////////////////////////////////////
    //
    //
    // ////////////////////////////////////////////////////////////
    void testIterator(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Create a test linked list with some data.
        LinkedListString strList;

        // Test the end iterator always points at nothing.
        IteratorString begin = strList.Begin(), end = strList.End();

        TS_ASSERT(!end);
        TS_ASSERT(!begin);
        TS_ASSERT(begin == end);

        // Add data to list.
        std::string strArr[] = {"a", "b", "c", "d", "e"};
        const U32 size = 5;
        for(U32 i(0); i < size; ++i) {
            strList.PushBack(strArr[i]);
        }

        // Test the end iterator always points at nothing.
        end = strList.End();
        TS_ASSERT(!end);

        // Test Pre inc
        begin = strList.Begin();
        TS_ASSERT(!(!begin));
        TS_ASSERT(begin != end);
        TS_ASSERT_EQUALS((*begin).compare(strArr[0].c_str()), 0);

        ++begin;
        TS_ASSERT(!(!begin));
        TS_ASSERT(begin != end);
        TS_ASSERT_EQUALS((*begin).compare(strArr[1].c_str()), 0);
        ++begin;
        TS_ASSERT(!(!begin));
        TS_ASSERT(begin != end);
        TS_ASSERT_EQUALS((*begin).compare(strArr[2].c_str()), 0);
        ++begin;
        TS_ASSERT(!(!begin));
        TS_ASSERT(begin != end);
        TS_ASSERT_EQUALS((*begin).compare(strArr[3].c_str()), 0);
        ++begin;
        TS_ASSERT(!(!begin));
        TS_ASSERT(begin != end);
        TS_ASSERT_EQUALS((*begin).compare(strArr[4].c_str()), 0);
        ++begin;
        TS_ASSERT(!begin);
        TS_ASSERT(begin == end);

        // Test Post inc.
        begin = strList.Begin();
        TS_ASSERT(!(!begin));
        TS_ASSERT(begin != end);
        TS_ASSERT_EQUALS((*begin).compare(strArr[0].c_str()), 0);

        begin++;
        TS_ASSERT(!(!begin));
        TS_ASSERT(begin != end);
        TS_ASSERT_EQUALS((*begin).compare(strArr[1].c_str()), 0);
        begin++;
        TS_ASSERT(!(!begin));
        TS_ASSERT(begin != end);
        TS_ASSERT_EQUALS((*begin).compare(strArr[2].c_str()), 0);
        begin++;
        TS_ASSERT(!(!begin));
        TS_ASSERT(begin != end);
        TS_ASSERT_EQUALS((*begin).compare(strArr[3].c_str()), 0);
        begin++;
        TS_ASSERT(!(!begin));
        TS_ASSERT(begin != end);
        TS_ASSERT_EQUALS((*begin).compare(strArr[4].c_str()), 0);
        begin++;
        TS_ASSERT(!begin);
        TS_ASSERT(begin == end);

        // Test Pre dec
        IteratorString curr = strList.End();
        --curr;
        TS_ASSERT(!(!curr));
        TS_ASSERT_EQUALS((*curr).compare(strArr[4].c_str()), 0);

        --curr;
        TS_ASSERT(!(!curr));
        TS_ASSERT_EQUALS((*curr).compare(strArr[3].c_str()), 0);
        --curr;
        TS_ASSERT(!(!curr));
        TS_ASSERT_EQUALS((*curr).compare(strArr[2].c_str()), 0);
        --curr;
        TS_ASSERT(!(!curr));
        TS_ASSERT_EQUALS((*curr).compare(strArr[1].c_str()), 0);
        --curr;
        TS_ASSERT(!(!curr));
        TS_ASSERT_EQUALS((*curr).compare(strArr[0].c_str()), 0);
        --curr;
        TS_ASSERT(!(!curr));

        // Test Post dec
        curr = strList.End();
        curr--;
        TS_ASSERT(!(!curr));
        TS_ASSERT_EQUALS((*curr).compare(strArr[4].c_str()), 0);

        curr--;
        TS_ASSERT(!(!curr));
        TS_ASSERT_EQUALS((*curr).compare(strArr[3].c_str()), 0);
        curr--;
        TS_ASSERT(!(!curr));
        TS_ASSERT_EQUALS((*curr).compare(strArr[2].c_str()), 0);
        curr--;
        TS_ASSERT(!(!curr));
        TS_ASSERT_EQUALS((*curr).compare(strArr[1].c_str()), 0);
        curr--;
        TS_ASSERT(!(!curr));
        TS_ASSERT_EQUALS((*curr).compare(strArr[0].c_str()), 0);
        curr--;
        TS_ASSERT(!(!curr));

        // Test offset inc (+)
        curr = strList.Begin();
        IteratorString ni = curr + 0;
        TS_ASSERT(curr == ni);

        ni = curr + 1;
        TS_ASSERT(curr != ni);
        TS_ASSERT_EQUALS((*ni).compare(strArr[1].c_str()), 0);

        ni = curr + strList.Size();
        TS_ASSERT(curr != ni);
        TS_ASSERT(ni == strList.End());

        // Test offset inc (+=)
        curr = strList.Begin();
        curr += 0;
        TS_ASSERT(curr == strList.Begin());

        curr += 1;
        TS_ASSERT_EQUALS((*curr).compare(strArr[1].c_str()), 0);

        curr += strList.Size();
        TS_ASSERT(curr == strList.End());
        TS_ASSERT(!curr);

        // Test offset inc (-)
        curr = strList.End();
        ni = curr - 0;
        TS_ASSERT(curr == ni);

        ni = curr - 1;
        TS_ASSERT(curr != ni);
        TS_ASSERT_EQUALS((*ni).compare(strArr[4].c_str()), 0);

        ni = curr - strList.Size();
        TS_ASSERT(curr != ni);
        TS_ASSERT(ni == strList.Begin());
        TS_ASSERT_EQUALS((*ni).compare(strArr[0].c_str()), 0);

        // Test offset inc (-=)
        curr = strList.End();
        curr -= 0;
        TS_ASSERT(curr == strList.End());

        curr -= 1;
        TS_ASSERT_EQUALS((*curr).compare(strArr[4].c_str()), 0);

        curr -= strList.Size();
        TS_ASSERT(curr == strList.Begin());
        TS_ASSERT_EQUALS((*curr).compare(strArr[0].c_str()), 0);

        // Test < operator.
        curr = strList.Begin();
        IteratorString a = curr + 1;
        IteratorString b = curr + 2;
        IteratorString c = curr + 3;
        IteratorString d = curr + 4;

        TS_ASSERT(!(curr < curr));
        TS_ASSERT(curr < strList.End());
        TS_ASSERT(curr < a);
        TS_ASSERT(curr < b);
        TS_ASSERT(curr < c);
        TS_ASSERT(curr < d);

        TS_ASSERT(!(a < curr));
        TS_ASSERT(!(a < a));
        TS_ASSERT(a < b);
        TS_ASSERT(a < c);
        TS_ASSERT(a < d);

        TS_ASSERT(!(b < curr));
        TS_ASSERT(!(b < a));
        TS_ASSERT(!(b < b));
        TS_ASSERT(b < c);
        TS_ASSERT(b < d);

        TS_ASSERT(!(c < curr));
        TS_ASSERT(!(c < a));
        TS_ASSERT(!(c < b));
        TS_ASSERT(!(c < c));
        TS_ASSERT(c < d);

        TS_ASSERT(!(d < curr));
        TS_ASSERT(!(d < a));
        TS_ASSERT(!(d < b));
        TS_ASSERT(!(d < c));
        TS_ASSERT(!(d < d));
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testPushBackAndPopBack(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        const int size = 10;
        LinkedListInt intList;

        TS_ASSERT(intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), 0);

        // Test popping empty list.
        TS_ASSERT(!intList.PopBack());

        // Test pushing data onto list.
        for(int i(0); i < size; ++i) {
            TS_ASSERT(intList.PushBack(arr[i]));
        }

        // Test list holds data in correct order.
        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), size);
        U32 index(0);
        for(IteratorInt i = intList.Begin(), end = intList.End(); i != end; ++i, ++index) {
            TS_ASSERT_EQUALS(*i, arr[index]);
        }

        // Test poppin all data.
        for(int i(0); i < size; ++i) {
            TS_ASSERT(intList.PopBack());
        }

        // Ensure list is empty.
        TS_ASSERT(intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), 0);

        // Test popping empty list.
        TS_ASSERT(!intList.PopBack());
    };

    // ////////////////////////////////////////////////////////////
    //
    //
    // ////////////////////////////////////////////////////////////
    void testPushFrontAndPopFront(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        const int size = 10;
        LinkedListInt intList;

        TS_ASSERT(intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), 0);

        // Test popping empty list.
        TS_ASSERT(!intList.PopFront());

        // Test pushing data onto list.
        for(int i(0); i < size; ++i) {
            TS_ASSERT(intList.PushFront(arr[i]));
        }

        // Test list holds data in correct order.
        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), size);
        U32 index(size - 1);
        for(IteratorInt i = intList.Begin(), end = intList.End(); i != end; ++i, --index) {
            TS_ASSERT_EQUALS(*i, arr[index]);
        }

        // Test poppin all data.
        for(int i(0); i < size; ++i) {
            TS_ASSERT(intList.PopFront());
        }

        // Ensure list is empty.
        TS_ASSERT(intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), 0);

        // Test popping empty list.
        TS_ASSERT(!intList.PopFront());
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testFrontAndBack(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Create a test linked list with some data.
        LinkedListString strList;
        std::string strArr[] = {"a", "b", "c", "d", "e"};
        const U32 size = 5;
        for(U32 i(0); i < size; ++i) {
            strList.PushBack(strArr[i]);
        }

        std::string front, back;

        TS_ASSERT(strList.Front(front));
        TS_ASSERT(strList.Back(back));
        TS_ASSERT_EQUALS(front.compare(strArr[0].c_str()), 0);
        TS_ASSERT_EQUALS(back.compare(strArr[4].c_str()), 0);

        TS_ASSERT(strList.PopFront());

        TS_ASSERT(strList.Front(front));
        TS_ASSERT(strList.Back(back));
        TS_ASSERT_EQUALS(front.compare(strArr[1].c_str()), 0);
        TS_ASSERT_EQUALS(back.compare(strArr[4].c_str()), 0);

        TS_ASSERT(strList.PopBack());

        TS_ASSERT(strList.Front(front));
        TS_ASSERT(strList.Back(back));
        TS_ASSERT_EQUALS(front.compare(strArr[1].c_str()), 0);
        TS_ASSERT_EQUALS(back.compare(strArr[3].c_str()), 0);
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testClear(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int size = 10;
        LinkedListInt intList;

        TS_ASSERT(intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), 0);

        TS_ASSERT_THROWS_NOTHING(intList.Clear());

        TS_ASSERT(intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), 0);

        for(int i(0); i < size; ++i) {
            intList.PushBack(arr[i]);
        }

        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), size);

        TS_ASSERT_THROWS_NOTHING(intList.Clear());

        TS_ASSERT(intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), 0);
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testInsertAndRemove(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        const U32 size = 10;
        LinkedListInt intList;

        TS_ASSERT(intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), 0);

        // Test insert at back.
        for(U32 i(0); i < size; ++i) {
            TS_ASSERT(intList.Insert(arr[i], intList.End()));
        }

        // Ensure list is as expected.
        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), size);
        U32 index(0);
        for(IteratorInt i = intList.Begin(), end = intList.End(); i != end; ++i, ++index) {
            TS_ASSERT_EQUALS(*i, arr[index]);
        }

        // Test inserting data into middle of the list.
        TS_ASSERT(intList.Insert(99, intList.Begin() + 4));
        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), size + 1);
        IteratorInt currIter = intList.Begin();
        TS_ASSERT_EQUALS(*currIter, arr[0]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[1]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[2]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[3]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, 99);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[4]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[5]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[6]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[7]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[8]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[9]);
        ++currIter;

        // Test removing new data in the middle of the list.
        intList.Remove(intList.Begin() + 4);
        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), size);
        currIter = intList.Begin();
        TS_ASSERT_EQUALS(*currIter, arr[0]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[1]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[2]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[3]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[4]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[5]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[6]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[7]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[8]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[9]);
        ++currIter;

        // Test removing data at the beginning of the list.
        intList.Remove(intList.Begin());
        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), size - 1);
        currIter = intList.Begin();
        TS_ASSERT_EQUALS(*currIter, arr[1]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[2]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[3]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[4]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[5]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[6]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[7]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[8]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[9]);
        ++currIter;

        // Test removing data at the end of the list.
        intList.Remove(intList.End());
        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), size - 2);
        currIter = intList.Begin();
        TS_ASSERT_EQUALS(*currIter, arr[1]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[2]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[3]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[4]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[5]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[6]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[7]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[8]);
        ++currIter;
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testInsertRange(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        const U32 size = 10;
        LinkedListInt intList;

        TS_ASSERT(intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), 0);

        // Test insert at back.
        for(U32 i(0); i < size; ++i) {
            TS_ASSERT(intList.Insert(arr[i], intList.End()));
        }

        // Ensure list is as expected.
        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), size);
        U32 index(0);
        for(IteratorInt i = intList.Begin(), end = intList.End(); i != end; ++i, ++index) {
            TS_ASSERT_EQUALS(*i, arr[index]);
        }

        // Build a new list.
        LinkedListInt newList;
        newList.PushBack(20);
        newList.PushBack(30);
        newList.PushBack(40);
        newList.PushBack(50);

        // Save old list.
        LinkedListInt copyList(intList);

        // Test inserting all new list at the beginning.
        intList.InsertRange(newList.Begin(), newList.End(), intList.Begin());

        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), copyList.Size() + newList.Size());
        IteratorInt currIter = intList.Begin();
        TS_ASSERT_EQUALS(*currIter, 20);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, 30);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, 40);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, 50);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[0]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[1]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[2]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[3]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[4]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[5]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[6]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[7]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[8]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[9]);
        ++currIter;

        // Reset list.
        intList = copyList;

        // Test inserting all new list in the middle.
        intList.InsertRange(newList.Begin(), newList.End(), intList.Begin() + 5);

        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), copyList.Size() + newList.Size());
        currIter = intList.Begin();
        TS_ASSERT_EQUALS(*currIter, arr[0]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[1]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[2]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[3]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[4]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, 20);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, 30);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, 40);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, 50);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[5]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[6]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[7]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[8]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[9]);
        ++currIter;

        // Reset list.
        intList = copyList;

        // Test inserting all new list at the end.
        intList.InsertRange(newList.Begin(), newList.End(), intList.End());

        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), copyList.Size() + newList.Size());
        currIter = intList.Begin();
        TS_ASSERT_EQUALS(*currIter, arr[0]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[1]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[2]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[3]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[4]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[5]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[6]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[7]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[8]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[9]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, 20);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, 30);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, 40);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, 50);
        ++currIter;
    };

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void testRemoveRange(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        const U32 size = 10;
        LinkedListInt intList;

        TS_ASSERT(intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), 0);

        // Test insert at back.
        for(U32 i(0); i < size; ++i) {
            TS_ASSERT(intList.Insert(arr[i], intList.End()));
        }

        // Ensure list is as expected.
        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), size);
        U32 index(0);
        for(IteratorInt i = intList.Begin(), end = intList.End(); i != end; ++i, ++index) {
            TS_ASSERT_EQUALS(*i, arr[index]);
        }

        // Save old list.
        LinkedListInt copyList(intList);

        // Test removing all new list at the beginning.
        TS_ASSERT(intList.RemoveRange(intList.Begin(), intList.End()));

        TS_ASSERT(intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), 0);

        // Reset list.
        intList = copyList;

        // Test removing some elements from beginning
        TS_ASSERT(intList.RemoveRange(intList.Begin(), intList.Begin() + 2));

        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), copyList.Size() - 3);
        IteratorInt currIter = intList.Begin();
        TS_ASSERT_EQUALS(*currIter, arr[3]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[4]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[5]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[6]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[7]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[8]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[9]);
        ++currIter;
        TS_ASSERT(currIter == intList.End());

        // Reset list.
        intList = copyList;

        // Test removing some elements from the middle.
        TS_ASSERT(intList.RemoveRange(intList.Begin() + 2, intList.Begin() + 5));

        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), copyList.Size() - 4);
        currIter = intList.Begin();
        TS_ASSERT_EQUALS(*currIter, arr[0]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[1]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[6]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[7]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[8]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[9]);
        ++currIter;
        TS_ASSERT(currIter == intList.End());

        // Reset list.
        intList = copyList;

        // Test removing some elements from the end.
        TS_ASSERT(intList.RemoveRange(intList.Begin() + 7, intList.End()));

        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), copyList.Size() - 3);
        currIter = intList.Begin();
        TS_ASSERT_EQUALS(*currIter, arr[0]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[1]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[2]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[3]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[4]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[5]);
        ++currIter;
        TS_ASSERT_EQUALS(*currIter, arr[6]);
        ++currIter;
        TS_ASSERT(currIter == intList.End());

        // Reset list.
        intList = copyList;

        // Test trying to remove elements with iterator(s) referring to different lists.
        TS_ASSERT(!intList.RemoveRange(copyList.Begin(), copyList.End()));
        // Ensure list is as expected.
        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), size);
        index = 0;
        for(IteratorInt i = intList.Begin(), end = intList.End(); i != end; ++i, ++index) {
            TS_ASSERT_EQUALS(*i, arr[index]);
        }

        TS_ASSERT(!intList.RemoveRange(intList.Begin(), copyList.End()));
        // Ensure list is as expected.
        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), size);
        index = 0;
        for(IteratorInt i = intList.Begin(), end = intList.End(); i != end; ++i, ++index) {
            TS_ASSERT_EQUALS(*i, arr[index]);
        }

        TS_ASSERT(!intList.RemoveRange(copyList.Begin(), intList.End()));
        // Ensure list is as expected.
        TS_ASSERT(!intList.IsEmpty());
        TS_ASSERT_EQUALS(intList.Size(), size);
        index = 0;
        for(IteratorInt i = intList.Begin(), end = intList.End(); i != end; ++i, ++index) {
            TS_ASSERT_EQUALS(*i, arr[index]);
        }
    };

};

#endif
