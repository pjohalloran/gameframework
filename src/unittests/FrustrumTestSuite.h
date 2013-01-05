#pragma once
#ifndef __FRUSTRUM_TEST_SUITE_H
#define __FRUSTRUM_TEST_SUITE_H

// /////////////////////////////////////////////////////////////////
// @file FrustrumTestSuite.h
// @author PJ O Halloran
// @date 27/08/2010
//
// File contains the header for the Frustrum Test Suite.
//
// /////////////////////////////////////////////////////////////////

#include <cxxtest/TestSuite.h>
#include <boost/scoped_ptr.hpp>

#include "CommonMath.h"
#include "Frustrum.h"
#include "Vector.h"

using GameHalloran::Frustrum;
using GameHalloran::Point3;

// /////////////////////////////////////////////////////////////////
// @class FrustrumTestSuite
// @author PJ O Halloran
//
// This class defines a series of unit tests for the Frustrum 
// class.
//
// /////////////////////////////////////////////////////////////////
class FrustrumTestSuite : public CxxTest::TestSuite
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
	FrustrumTestSuite()
	{

	};

	// /////////////////////////////////////////////////////////////////
	// Destructor.
	//
	// /////////////////////////////////////////////////////////////////
	~FrustrumTestSuite()
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

		TS_ASSERT_THROWS_NOTHING(Frustrum defaultObj);
	};

	// /////////////////////////////////////////////////////////////////
	// Test Frustrum::Init().
	//
	// /////////////////////////////////////////////////////////////////
	void testInit(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		Frustrum obj;
		obj.Init(45.0f, 640/480, 1.0f, 100.0f);

		/*
		TS_WARN(obj.Get(Frustrum::Bottom).ToString().c_str());
		TS_WARN(obj.Get(Frustrum::Top).ToString().c_str());
		TS_WARN(obj.Get(Frustrum::Left).ToString().c_str());
		TS_WARN(obj.Get(Frustrum::Right).ToString().c_str());
		TS_WARN(obj.Get(Frustrum::Near).ToString().c_str());
		TS_WARN(obj.Get(Frustrum::Far).ToString().c_str());
		*/

		Point3 in(0.0f, 0.25f, 0.0f);
		Point3 out(100000.0f, 100000.0f, -100000.0f);

		TS_ASSERT(obj.Inside(in));
		TS_ASSERT(!obj.Inside(out));
	};

	// /////////////////////////////////////////////////////////////////
	// Test Frustrum::SetFov().
	//
	// /////////////////////////////////////////////////////////////////
	void testSetFov(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		//TS_FAIL("Test not implemented.");
	};

	// /////////////////////////////////////////////////////////////////
	// Test Frustrum::SetAspect().
	//
	// /////////////////////////////////////////////////////////////////
	void testSetAspect(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		//TS_FAIL("Test not implemented.");
	};

	// /////////////////////////////////////////////////////////////////
	// Test Frustrum::SetNear().
	//
	// /////////////////////////////////////////////////////////////////
	void testSetNear(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		//TS_FAIL("Test not implemented.");
	};

	// /////////////////////////////////////////////////////////////////
	// Test Frustrum::SetFar().
	//
	// /////////////////////////////////////////////////////////////////
	void testSetFar(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		//TS_FAIL("Test not implemented.");
	};

	// /////////////////////////////////////////////////////////////////
	// Test Frustrum::Inside(const Point &).
	//
	// /////////////////////////////////////////////////////////////////
	void testInsidePoint(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		//TS_FAIL("Test not implemented.");
	};

	// /////////////////////////////////////////////////////////////////
	// Test Frustrum::Inside(const Point &, const VecFloat).
	//
	// /////////////////////////////////////////////////////////////////
	void testInsideSphere(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		//TS_FAIL("Test not implemented.");
	};

	// /////////////////////////////////////////////////////////////////
	// Test Frustrum::Get(enum Side).
	//
	// /////////////////////////////////////////////////////////////////
	void testGet(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		//TS_FAIL("Test not implemented.");
	};

	// /////////////////////////////////////////////////////////////////
	// Test Frustrum::Render().
	//
	// /////////////////////////////////////////////////////////////////
	void testRender(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
		}

		// Can't unit test this.  Must run a program and look at the drawing to see if its correct...
	};

};

#endif
