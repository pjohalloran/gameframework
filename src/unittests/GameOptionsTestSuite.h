// /////////////////////////////////////////////////////////////////
// @file GameOptionsTestSuite.h
// @author PJ O Halloran
// @date 30/06/2010
//
// File contains the header for the GameOptions Test Suite.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GAME_OPTIONS_TEST_SUITE_H
#define __GAME_OPTIONS_TEST_SUITE_H

// External Headers
#include <string>

#include <cxxtest/TestSuite.h>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/filesystem.hpp>

// Project Headers
#include "GameOptions.h"

// /////////////////////////////////////////////////////////////////
// @class GameOptionsTestSuite
// @author PJ O Halloran
//
// This class defines a series of unit tests for the GameOptions 
// class.
//
// /////////////////////////////////////////////////////////////////
class GameOptionsTestSuite : public CxxTest::TestSuite
{

private:

	boost::shared_ptr<GameHalloran::GameLog> m_logPtr;					///< Pointer to logging object.
	boost::scoped_ptr<GameHalloran::GameOptions> m_testObjPtr;			///< Pointer to the object under test.
	boost::filesystem::path m_optionsPath;								///< Path to the options xml file.

	// /////////////////////////////////////////////////////////////////
	// Has the required test data been created?
	//
	// /////////////////////////////////////////////////////////////////
	bool IsTestDataReady()
	{
		return (m_logPtr.get() && !m_optionsPath.empty() && m_testObjPtr.get());
	};

public:

	// /////////////////////////////////////////////////////////////////
	// Constructor.
	//
	// /////////////////////////////////////////////////////////////////
	GameOptionsTestSuite()
	{

	};

	// /////////////////////////////////////////////////////////////////
	// Destructor.
	//
	// /////////////////////////////////////////////////////////////////
	virtual ~GameOptionsTestSuite()
	{
		try
		{
		}
		catch(...)
		{
		}
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	virtual void setUp()
	{
		// Set up a GameOptions object with a valid options file and logger
		try
		{
			boost::filesystem::path logFilename(std::string("GameOptionsTestSuite.log"));
			m_logPtr.reset(new GameHalloran::GameLog(logFilename.file_string(), GameHalloran::GameLog::DEB, true));

			m_optionsPath = boost::filesystem::path(std::string("testdata/valid.xml"));
			m_testObjPtr.reset(new GameHalloran::GameOptions(m_logPtr, m_optionsPath));
		}
		catch(GameHalloran::GameException &ge)
		{
			std::string msg;
			msg.append(std::string("Exception thrown during GameOptionsTestSuite::setUp(): ") + std::string(ge.what()));
			TS_FAIL(msg.c_str());
		}
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	virtual void tearDown()
	{
		m_testObjPtr.reset();
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testDefaultConstructor(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
			return;
		}

		// Data required to run tests.
		boost::scoped_ptr<GameHalloran::GameOptions> goPtr;		// Pointer to default object.
		std::string name("dummy");								// Sample option name.
		std::string value("65");								// Sample option value.
		boost::filesystem::path newPath(std::string("t.xml"));	// Test path to save to.

		// Test default constructor does not throw an exception.
		TS_ASSERT_THROWS_NOTHING(GameHalloran::GameOptions go);

		// Create default object pointer.
		goPtr.reset(new GameHalloran::GameOptions());

		// Test no options file loaded.
		TS_ASSERT(!goPtr->IsOptionsFileLoaded());

		// Test Get(), Append(), Edit(), Remove() and RemoveOptions() do not throw any exceptions for a default object.
		TS_ASSERT_THROWS_NOTHING(goPtr->Get(name, value));
		TS_ASSERT_THROWS_NOTHING(goPtr->Get(name, value, GameHalloran::GameOptions::PROGRAMMER));

		TS_ASSERT_THROWS_NOTHING(goPtr->Append(name, value));
		TS_ASSERT_THROWS_NOTHING(goPtr->Append(name, value, GameHalloran::GameOptions::PROGRAMMER));

		TS_ASSERT_THROWS_NOTHING(goPtr->Edit(name, value));
		TS_ASSERT_THROWS_NOTHING(goPtr->Edit(name, value, GameHalloran::GameOptions::PROGRAMMER));

		TS_ASSERT_THROWS_NOTHING(goPtr->Remove(name));
		TS_ASSERT_THROWS_NOTHING(goPtr->Remove(name, GameHalloran::GameOptions::PROGRAMMER));

		TS_ASSERT_THROWS_NOTHING(goPtr->Remove(GameHalloran::GameOptions::PROGRAMMER));

		// Test Commit and Save throw exceptions when used after the default constructor.
		TS_ASSERT_THROWS(goPtr->Commit(), GameHalloran::GameException);
		TS_ASSERT_THROWS(goPtr->Commit(true), GameHalloran::GameException);
		TS_ASSERT_THROWS(goPtr->Save(newPath), GameHalloran::GameException);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testParseFile(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
			return;
		}

		// Data required to run tests.
		boost::scoped_ptr<GameHalloran::GameOptions> goPtr;		// Pointer to default object.
		boost::filesystem::path newPath;						// Test path to save to.

		// Create default object pointer.
		goPtr.reset(new GameHalloran::GameOptions());

		// Test no options file loaded.
		TS_ASSERT(!goPtr->IsOptionsFileLoaded());

		// Test ParseFile() with invalid files.

		// A) Valid Directory.
		newPath = boost::filesystem::path(std::string("C:/temp"));
		TS_ASSERT_THROWS(goPtr->ParseFile(newPath), GameHalloran::GameException);
		TS_ASSERT(!goPtr->IsOptionsFileLoaded());

		// B) Invalid directory.
		newPath = boost::filesystem::path(std::string("C:/dirdoesnotexist/doublysure"));
		TS_ASSERT_THROWS(goPtr->ParseFile(newPath), GameHalloran::GameException);
		TS_ASSERT(!goPtr->IsOptionsFileLoaded());

		// C) File that is not an xml.
		newPath = boost::filesystem::path(std::string("testdata/test.txt"));
		TS_ASSERT_THROWS(goPtr->ParseFile(newPath), GameHalloran::GameException);
		TS_ASSERT(!goPtr->IsOptionsFileLoaded());

		// D) Xml file that is not a valid options file.
		newPath = boost::filesystem::path(std::string("testdata/GraphicConfig.xml"));
		TS_ASSERT_THROWS(goPtr->ParseFile(newPath), GameHalloran::GameException);
		TS_ASSERT(!goPtr->IsOptionsFileLoaded());

		// E) File that does not exist.
		newPath = boost::filesystem::path(std::string("testdata/filethatdoesnotexist.txt"));
		TS_ASSERT_THROWS(goPtr->ParseFile(newPath), GameHalloran::GameException);
		TS_ASSERT(!goPtr->IsOptionsFileLoaded());

		// Test ParseFile() with a valid option file.
		newPath = boost::filesystem::path(std::string("testdata/valid.xml"));
		TS_ASSERT_THROWS_NOTHING(goPtr->ParseFile(newPath));
		TS_ASSERT(goPtr->IsOptionsFileLoaded());
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testParseConstructor(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
			return;
		}

		// Data required to run tests.
		boost::shared_ptr<GameHalloran::GameLog> logNullPtr;	// Pointer to a NULL shared GameLog.
		boost::scoped_ptr<GameHalloran::GameOptions> goPtr;		// Pointer to an object.
		boost::filesystem::path newPath;						// Test path to save to.

		// Test options file loaded for global object created with the Parse constructor.
		TS_ASSERT(m_testObjPtr->IsOptionsFileLoaded());

		// Test parse constructor with...
		// A) no log but valid options file
		newPath = boost::filesystem::path(std::string("testdata/valid.xml"));
		TS_ASSERT_THROWS_NOTHING(GameHalloran::GameOptions go(logNullPtr, newPath));

		// B) a valid log and a valid options file
		newPath = boost::filesystem::path(std::string("testdata/valid.xml"));
		TS_ASSERT_THROWS_NOTHING(GameHalloran::GameOptions go(m_logPtr, newPath));

		// C) an invalid options file.
		newPath = boost::filesystem::path(std::string("testdata/GraphicConfig.xml"));
		TS_ASSERT_THROWS(GameHalloran::GameOptions go(m_logPtr, newPath), GameHalloran::GameException);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testIsOptionsFileLoaded(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
			return;
		}

		//TS_TRACE("Already tested for comprehensively in the previous tests.");
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testGet(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
			return;
		}

		// Data required to run tests.
		std::string expectedValue;			// The expected value to find in the file.
		std::string name;					// The current name.
		std::string value;					// The current value retrieved.


		// Test options file loaded for global object created with the Parse constructor.
		TS_ASSERT(m_testObjPtr->IsOptionsFileLoaded());

		// Test Get() with...

		// A) valid name
		name.assign(std::string("ScreenWidth"));
		value.clear();
		expectedValue.assign(std::string("640"));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_EQUALS(value, expectedValue);

		// B) invalid name (with value empty)
		name.assign(std::string("NOTEXISTINGOPTION"));
		value.clear();
		expectedValue.assign(std::string("ERRORVALUE"));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_DIFFERS(value, expectedValue);

		// C) invalid name (with value filled with data beforehand).
		name.assign(std::string("NOTEXISTINGOPTION"));
		value.assign(std::string("randomcrap"));
		expectedValue.assign(std::string("ERRORVALUE"));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_DIFFERS(value, expectedValue);
		TS_ASSERT_EQUALS(value.size(), (size_t) 0);

		// D) blank name
		name.clear();
		value.clear();
		expectedValue.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_EQUALS(value.size(), (size_t) 0);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testAppend(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
			return;
		}

		// Data required to run tests.
		std::string name;					// The current name.
		std::string value;					// The current value.
		std::string retrievedValue;			// The value retrieved.

		// Test options file loaded for global object created with the Parse constructor.
		TS_ASSERT(m_testObjPtr->IsOptionsFileLoaded());

		// Test Append() with...

		// A) A new name/value
		name.assign(std::string("NewOption"));
		value.assign(std::string("NewValue"));
		retrievedValue.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Append(name, value, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, retrievedValue, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT_EQUALS(value, retrievedValue);

		// B) An exisiting name with its current value.
		name.assign(std::string("ScreenWidth"));
		value.assign(std::string("640"));
		retrievedValue.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Append(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, retrievedValue, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_EQUALS(value, retrievedValue);

		// C) An exisiting name with a different value.
		{
			std::string origValue("640");
			name.assign(std::string("ScreenWidth"));
			value.assign(std::string("1000"));
			retrievedValue.clear();
			TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Append(name, value, GameHalloran::GameOptions::PLAYER));
			TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, retrievedValue, GameHalloran::GameOptions::PLAYER));
			TS_ASSERT_EQUALS(value, retrievedValue);
			TS_ASSERT_DIFFERS(value, origValue);
		}

		// D) A valid name but blank value
		name.assign(std::string("PS3"));
		value.assign(std::string(""));
		retrievedValue.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Append(name, value, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, retrievedValue, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT(value.empty());
		TS_ASSERT(retrievedValue.empty());

		// E) A blank name but valid value
		name.assign(std::string(""));
		value.assign(std::string("new value"));
		retrievedValue.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Append(name, value, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, retrievedValue, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT_DIFFERS(value, retrievedValue);
		TS_ASSERT(retrievedValue.empty());

		// F) A blank name and value.
		name.clear();
		value.clear();
		retrievedValue.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Append(name, value, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, retrievedValue, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT(value.empty());
		TS_ASSERT(retrievedValue.empty());
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testEdit(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
			return;
		}

		// Data required to run tests.
		std::string name;					// The current name.
		std::string value;					// The current value.
		std::string retrievedValue;			// The value retrieved.

		// Test options file loaded for global object created with the Parse constructor.
		TS_ASSERT(m_testObjPtr->IsOptionsFileLoaded());

		// Test Edit() with...

		// A) A new name/value
		name.assign(std::string("NewOption"));
		value.assign(std::string("NewValue"));
		retrievedValue.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Edit(name, value, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, retrievedValue, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT(retrievedValue.empty());

		// B) An exisiting name with its current value.
		name.assign(std::string("ScreenWidth"));
		value.assign(std::string("640"));
		retrievedValue.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Edit(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, retrievedValue, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_EQUALS(value, retrievedValue);

		// C) An exisiting name with a different value.
		name.assign(std::string("ScreenWidth"));
		value.assign(std::string("1000"));
		retrievedValue.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Edit(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, retrievedValue, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_EQUALS(value, retrievedValue);

		// D) A valid name but blank value
		name.assign(std::string("PS3"));
		value.assign(std::string(""));
		retrievedValue.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Edit(name, value, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, retrievedValue, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT(value.empty());
		TS_ASSERT(retrievedValue.empty());

		// E) A blank name but valid value
		name.assign(std::string(""));
		value.assign(std::string("new value"));
		retrievedValue.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Edit(name, value, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, retrievedValue, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT_DIFFERS(value, retrievedValue);
		TS_ASSERT(retrievedValue.empty());

		// F) A blank name and value.
		name.clear();
		value.clear();
		retrievedValue.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Edit(name, value, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, retrievedValue, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT(value.empty());
		TS_ASSERT(retrievedValue.empty());
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testRemove(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
			return;
		}

		// Data required to run tests.
		std::string name;					// The current name.
		std::string value;					// The current value retrieved.


		// Test options file loaded for global object created with the Parse constructor.
		TS_ASSERT(m_testObjPtr->IsOptionsFileLoaded());

		// Test Remove() with...

		// A) valid name with PLAYER
		name.assign(std::string("ScreenWidth"));
		value.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT(!value.empty());
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Remove(name, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT(value.empty());

		// B) valid name with PROGRAMMER
		name.assign(std::string("FullScreen"));
		value.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT(!value.empty());
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Remove(name, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT(value.empty());

		// C) invalid name
		name.assign(std::string("INVALIDNAME"));
		value.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value));
		TS_ASSERT(value.empty());
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Remove(name));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value));
		TS_ASSERT(value.empty());

		// D) blank name
		name.clear();
		value.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value));
		TS_ASSERT(value.empty());
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Remove(name));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value));
		TS_ASSERT(value.empty());
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testRemoveOptionType(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
			return;
		}

		// Data required to run tests.
		std::string name;					// The current name.
		std::string value;					// The current value retrieved.
		std::string expectedValue;			// The expected value.

		// Test options file loaded for global object created with the Parse constructor.
		TS_ASSERT(m_testObjPtr->IsOptionsFileLoaded());

		// Test Remove(OptionType)

		// 1) Test values are all inside first.
		// ScreenBpp
		expectedValue.assign(std::string("32"));
		name.assign(std::string("ScreenBpp"));
		value.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_EQUALS(value, expectedValue);

		// ScreenHeight
		expectedValue.assign(std::string("480"));
		name.assign(std::string("ScreenHeight"));
		value.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_EQUALS(value, expectedValue);

		// ScreenWidth
		expectedValue.assign(std::string("640"));
		name.assign(std::string("ScreenWidth"));
		value.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_EQUALS(value, expectedValue);

		// 2) Run Remove(OptionType)
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Remove(GameHalloran::GameOptions::PLAYER));

		// 3) Test all previous values are gone.
		// ScreenBpp
		expectedValue.assign(std::string("32"));
		name.assign(std::string("ScreenBpp"));
		value.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT(value.empty());

		// ScreenHeight
		expectedValue.assign(std::string("480"));
		name.assign(std::string("ScreenHeight"));
		value.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT(value.empty());

		// ScreenWidth
		expectedValue.assign(std::string("640"));
		name.assign(std::string("ScreenWidth"));
		value.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT(value.empty());
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testSave(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
			return;
		}

		// Data required to run tests.
		std::string name;					// The current name.
		std::string value;					// The current value retrieved.
		std::string expectedValue;			// The expected value.
		boost::filesystem::path newPath(std::string("C:/temp/newoptions.xml"));

		// Test options file loaded for global object created with the Parse constructor.
		TS_ASSERT(m_testObjPtr->IsOptionsFileLoaded());

		// Add, edit and remove a value, save the file to a new location and test it was done
		name.assign(std::string("NEW_OPTION"));
		value.assign(std::string("33"));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Append(name, value, GameHalloran::GameOptions::PLAYER));

		name.assign(std::string("FullScreen"));
		value.assign(std::string("TRUE"));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Edit(name, value, GameHalloran::GameOptions::PROGRAMMER));

		name.assign(std::string("ScreenWidth"));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Remove(name, GameHalloran::GameOptions::PLAYER));

		// Save the file.
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Save(newPath));

		// Test file exists at new location (and at the old location).
		TS_ASSERT(boost::filesystem::exists(newPath));
		TS_ASSERT(boost::filesystem::exists(m_optionsPath));
		
		// Test the changes made above are in the new file.
		name.assign(std::string("NEW_OPTION"));
		value.clear();
		expectedValue.assign(std::string("33"));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_EQUALS(value, expectedValue);

		name.assign(std::string("FullScreen"));
		value.clear();
		expectedValue.assign(std::string("TRUE"));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT_EQUALS(value, expectedValue);

		name.assign(std::string("ScreenWidth"));
		value.clear();
		expectedValue.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT(value.empty());

		// Try saving the file to a directory that does not exist yet.
		newPath = boost::filesystem::path(std::string("C:/temp/DIRNOTEXIST/newoptions.xml"));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Save(newPath));
		// Test it succeeded.
		TS_ASSERT(boost::filesystem::exists(newPath));

		// Try saving to a partition that does not exist.
		newPath = boost::filesystem::path(std::string("S:/temp/newoptions.xml"));
		TS_ASSERT_THROWS(m_testObjPtr->Save(newPath), boost::filesystem::basic_filesystem_error<boost::filesystem::path>);
	};

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void testCommit(void)
	{
		if(!IsTestDataReady())
		{
			TS_FAIL("Test data not created.");
			return;
		}

		// Data required to run tests.
		std::string name;					// The current name.
		std::string value;					// The current value retrieved.
		std::string expectedValue;			// The expected value.
		boost::filesystem::path newPath(std::string("C:/temp/newoptions.xml"));		

		// Test options file loaded for global object created with the Parse constructor.
		TS_ASSERT(m_testObjPtr->IsOptionsFileLoaded());

		// NB!
		// First, save the file to a new location so that we do not overwrite the test data
		//  when we call Commit later!
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Save(newPath));
		TS_ASSERT(boost::filesystem::exists(newPath));

		// Add, edit and remove a value, save the file to a new location and test it was done
		name.assign(std::string("NEW_OPTION"));
		value.assign(std::string("33"));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Append(name, value, GameHalloran::GameOptions::PLAYER));

		name.assign(std::string("FullScreen"));
		value.assign(std::string("TRUE"));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Edit(name, value, GameHalloran::GameOptions::PROGRAMMER));

		name.assign(std::string("ScreenWidth"));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Remove(name, GameHalloran::GameOptions::PLAYER));

		// Commit the changes to the file.
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Commit());

		// Test file exists at the location.
		TS_ASSERT(boost::filesystem::exists(newPath));
		
		// Test the changes made above are in the new file.
		name.assign(std::string("NEW_OPTION"));
		value.clear();
		expectedValue.assign(std::string("33"));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT_EQUALS(value, expectedValue);

		name.assign(std::string("FullScreen"));
		value.clear();
		expectedValue.assign(std::string("TRUE"));
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PROGRAMMER));
		TS_ASSERT_EQUALS(value, expectedValue);

		name.assign(std::string("ScreenWidth"));
		value.clear();
		expectedValue.clear();
		TS_ASSERT_THROWS_NOTHING(m_testObjPtr->Get(name, value, GameHalloran::GameOptions::PLAYER));
		TS_ASSERT(value.empty());
	};

};

#endif
