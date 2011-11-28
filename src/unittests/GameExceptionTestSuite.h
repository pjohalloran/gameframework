// /////////////////////////////////////////////////////////////////
// @file GameExceptionTestSuite.h
// @author PJ O Halloran
// @date 01/07/2010
//
// File contains the header for the GameException Test Suite.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GAME_EXCEPTION_TEST_SUITE_H
#define __GAME_EXCEPTION_TEST_SUITE_H

// External Headers
#include <cxxtest/TestSuite.h>

#include <string>

// Project Headers
#include "GameException.h"

// /////////////////////////////////////////////////////////////////
// @class GameExceptionTestSuite
// @author PJ O Halloran
//
// This class defines a series of unit tests for the GameException 
// class.
//
// /////////////////////////////////////////////////////////////////
class GameExceptionTestSuite : public CxxTest::TestSuite
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
	GameExceptionTestSuite()
	{

	};

	// /////////////////////////////////////////////////////////////////
	// Destructor.
	//
	// /////////////////////////////////////////////////////////////////
	~GameExceptionTestSuite()
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

		TS_ASSERT_THROWS_NOTHING(GameHalloran::GameException());

		GameHalloran::GameException defaultObj;
		const char *msg;
		TS_ASSERT_THROWS_NOTHING(msg = defaultObj.what());
		TS_ASSERT_EQUALS(strcmp(msg, "Unknown exception"), 0);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testStringConstructor(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		std::string msgStr("some exception");
		TS_ASSERT_THROWS_NOTHING(GameHalloran::GameException(msgStr));

		GameHalloran::GameException obj(msgStr);
		const char *msg;
		TS_ASSERT_THROWS_NOTHING(msg = obj.what());
		TS_ASSERT_EQUALS(strcmp(msg, msgStr.c_str()), 0);
	};

};

#endif
