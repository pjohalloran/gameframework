#pragma once
#ifndef __GAME_MAIN_TEST_SUITE_H
#define __GAME_MAIN_TEST_SUITE_H

// /////////////////////////////////////////////////////////////////
// @file GameMainTestSuite.h
// @author PJ O Halloran
// @date 01/07/2010
//
// File contains the header for the GameMain Test Suite.
//
// /////////////////////////////////////////////////////////////////

#include <cxxtest/TestSuite.h>
#include <boost/scoped_ptr.hpp>

#include "GameMain.h"

// /////////////////////////////////////////////////////////////////
// @class GameMainTestSuite
// @author PJ O Halloran
//
// This class defines a series of unit tests for the GameMain
// class.
//
// /////////////////////////////////////////////////////////////////
class GameMainTestSuite : public CxxTest::TestSuite {

private:



    bool IsTestDataReady() {
        return (true);
    };

public:

    // /////////////////////////////////////////////////////////////////
    // Constructor.
    //
    // /////////////////////////////////////////////////////////////////
    GameMainTestSuite() {

    };

    // /////////////////////////////////////////////////////////////////
    // Destructor.
    //
    // /////////////////////////////////////////////////////////////////
    ~GameMainTestSuite() {

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

        //TS_FAIL("Test not implemented.");
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testOptionsConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        //TS_FAIL("Test not implemented.");
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testSetAndIsRunning(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        //TS_FAIL("Test not implemented.");
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testIsAndSetPaused(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        //TS_FAIL("Test not implemented.");
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testGetScreenManager(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        //TS_FAIL("Test not implemented.");
    };

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void testMain(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        //TS_FAIL("Test not implemented.");
    };

};

#endif
