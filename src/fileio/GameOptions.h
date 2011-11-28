// /////////////////////////////////////////////////////////////////
// @file GameOptions.h
// @author PJ O Halloran
// @date 25/06/2010
//
// File contains the header for the GameOptions class and the
// IGameOptionsFactory interface.  It also contains the definition
// of the RetrieveAndConvertOption() helper function for the 
// GameOptions class.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GAME_OPTIONS_H
#define __GAME_OPTIONS_H

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

// External Headers
#include <map>
#include <string>
#include <exception>

#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <tinyxml.h>

// Project Headers
#include "GameException.h"
#include "GameLog.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class GameOptions
	// @author PJ O Halloran
	//
	// This class provides an interface to the game options file, usually
	// stored in $GAMEROOT/data/.  This class provides methods to load and
	// parse the game options file (an xml file) as well as to append new 
	// options or to edit existing game options.
	//
	// /////////////////////////////////////////////////////////////////
	class GameOptions
	{

	public:

		enum OptionType
		{
			PLAYER,
			PROGRAMMER
		};

	private:

		bool m_isModified;										///< Indicates if any game options have been added or edited.
		bool m_isValidFileOpen;									///< Has a valid options file been opened and loaded.
		boost::filesystem::path m_optionsFilePath;				///< The filepath of the options file we will be working upon.
		std::map<std::string, std::string> m_optionsMap;		///< Container of options.
		boost::shared_ptr<GameLog> m_logPtr;					///< Smart pointer to the GameLog logging object.

		static const std::string XML_EXT;						///< Declaration of static class constants.
		static const std::string ROOT_NODE;						///< Declaration of static class constants.
		static const std::string OPTION_TYPE_NODE;				///< Declaration of static class constants.
		static const std::string OPTION_TYPE_ID;				///< Declaration of static class constants.
		static const std::string DOT_SEPERATOR;					///< Declaration of static class constants.

		// /////////////////////////////////////////////////////////////////
		// Checks if the path supplied is a valid, existing file with an xml
		// extension.
		//
		// @param filePathRef The filepath of the options file to check.
		//
		// @return bool True if the path is a valid options file, otherwise
		//				we return false.
		//
		// /////////////////////////////////////////////////////////////////
		bool IsValidXmlFilePath(const boost::filesystem::path &filePathRef) const;

		// /////////////////////////////////////////////////////////////////
		// Parses the xml file and loads it into the map container.
		//
		// @throw GameException& If the xml file was not a valid options xml
		//							file.
		// @throw GameException& If some error occurs parsing the xml file.
		// /////////////////////////////////////////////////////////////////
		void ParseXmlFileHelper() throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Parses the xml file and loads it into the map container.
		//
		// @param optionsTypeElementPtr A pointer to the OptionsType XML
		//								element.
		//
		// @throw GameException& If the element is NULL.
		// @throw GameException& If some error occurs parsing the xml
		//							element.
		// /////////////////////////////////////////////////////////////////
		void ParseOptions(TiXmlElement *optionsTypeElementPtr) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Check if the string is a valid game option ID.
		//
		// @param optionIdStr The ID of the option.
		//
		// @return bool True if the string is a valid option, false otherwise.
		//
		// /////////////////////////////////////////////////////////////////
		bool IsValidGameOptionId(const char *optionIdStr) const;

		// /////////////////////////////////////////////////////////////////
		// Creates the key from the ID and the option name.
		//
		// @param optionIdStr The ID of the option.
		// @param optionNameRef The name of the option.
		// @param keyRef The parameter to store the generated key inside.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool CreateKey(const std::string &optionIdStr, const std::string &optionNameRef, std::string &keyRef) const;

		// /////////////////////////////////////////////////////////////////
		// Assembles and creates a key from the option type and option name.
		//
		// @param optionIdStr The ID of the option.
		// @param optionNameRef The name of the option.
		// @param keyRef The parameter to store the generated key inside.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool AssembleKey(const OptionType optionType, const std::string &optionNameRef, std::string &keyRef) const;

		// /////////////////////////////////////////////////////////////////
		// Take a key and extract the ID and option name from it.
		//
		// A key is in the format "ID.NAME" so its a simple string split
		// operation.
		//
		// @param keyRef The internal key.
		// @param optionsName The name of the option retrieved from the key.
		// @param type The option id extracted from the key.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool ExtractIdAndOptionsName(const std::string &keyRef, std::string &optionsName, OptionType &id) const;

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		GameOptions();

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// Initializes object and parses the file supplied and loads the game
		// options into memory.
		//
		// @param logPtr Shared pointer to the GameLog object.
		// @param filePathRef The filepath of the options file.
		//
		// @throw GameException& If the file was not a valid game options file.
		// @throw GameException& If we failed to open the file.
		//
		// /////////////////////////////////////////////////////////////////
		GameOptions(boost::shared_ptr<GameLog> logPtr, const boost::filesystem::path &filePathRef) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// Automatically discards all uncommited changes made to the options
		// file on destruction.
		//
		// /////////////////////////////////////////////////////////////////
		~GameOptions();

		// /////////////////////////////////////////////////////////////////
		// Parses the options file and loads the game options into memory.
		// 
		// If there is already a file loaded into memory then it will be 
		// discarded.  You must call Commit() manually before the call to
		// ParseFile() if you wish to save your changes to the old options
		// file.  In this case, if the ParseFile() fails then the old options
		// file must be reloaded.  Consider the old options gone if you ever
		// call ParseFile()!
		// 
		// @param filePathRef The filepath of the options file.
		// 
		// @throw GameException& If the options file was not a valid, existing 
		//							xml file.
		// @throw GameException& If the file was not a valid game options file.
		// @throw GameException& If we failed to open the file.
		// 
		// /////////////////////////////////////////////////////////////////
		void ParseFile(const boost::filesystem::path &filePathRef) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Get the options value identified by the option name.
		//
		// This call will have no effect unless an options file has been
		// loaded with the GameOptions(GameLog*, path) constructor or 
		// ParseFile() has been successfully called.  
		//
		// @param optionNameRef The ID of the option.
		// @param valueRef The string that will hold the value of the option
		//					on exit.
		// @param optionTpe The type of option.
		//
		// /////////////////////////////////////////////////////////////////
		void Get(const std::string &optionNameRef, std::string &valueRef, const OptionType optionType = PLAYER);

		// /////////////////////////////////////////////////////////////////
		// Append a new option and value.  Please note you must call Commit()
		// to save the changes to the file. 
		//
		// This call will have no effect unless an options file has been
		// loaded with the GameOptions(GameLog*, path) constructor or 
		// ParseFile() has been successfully called. 
		//
		// @param optionNameRef The ID of the option.
		// @param valueRef The value of the option.
		// @param The type of option.
		//
		// /////////////////////////////////////////////////////////////////
		void Append(const std::string &optionNameRef, const std::string &valueRef, const OptionType optionType = PLAYER);

		// /////////////////////////////////////////////////////////////////
		// Edits an existing option.  Please note you must call Commit()
		// to save the changes to the file. 
		//
		// This call will have no effect unless an options file has been
		// loaded with the GameOptions(GameLog*, path) constructor or 
		// ParseFile() has been successfully called. 
		//
		// @param optionNameRef The ID of the option to edit.
		// @param valueRef The new value of the option.
		// @param The type of option.
		//
		// /////////////////////////////////////////////////////////////////
		void Edit(const std::string &optionNameRef, const std::string &valueRef, const OptionType optionType = PLAYER);

		// /////////////////////////////////////////////////////////////////
		// Removes an option.  Please note you must call Commit()
		// to save the changes to the file.
		//
		// This call will have no effect unless an options file has been
		// loaded with the GameOptions(GameLog*, path) constructor or 
		// ParseFile() has been successfully called. 
		//
		// @param optionNameRef The ID of the option to remove.
		// @param The type of option.
		//
		// /////////////////////////////////////////////////////////////////
		void Remove(const std::string &optionNameRef, const OptionType optionType = PLAYER);

		// /////////////////////////////////////////////////////////////////
		// Removes an OptionType element from memory and all associated
		// options. Please note you must call Commit() to save the changes
		// to the file.
		//
		// This call will have no effect unless an options file has been
		// loaded with the GameOptions(GameLog*, path) constructor or 
		// ParseFile() has been successfully called. 
		//
		// @param The type of option.
		//
		// /////////////////////////////////////////////////////////////////
		void Remove(const OptionType optionType);

		// /////////////////////////////////////////////////////////////////
		// Saves the current options loaded into memory into a new xml file
		// located at the filename specified.
		//
		// This should be only done when required (such as on game exit) as disk
		// access is slow!
		//
		// @throw GameException& If no options file has been loaded into
		//							memory yet.
		// @throw GameException& If we found that the new directory did not
		//							exist and we failed to create it.
		//
		// /////////////////////////////////////////////////////////////////
		void Save(const boost::filesystem::path &newFilePathRef) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Saves any unsaved changes to the options file used to originally
		// load the options.
		//
		// This should be only done when required (such as on game exit) as disk
		// access is slow!
		//
		// @param forceCommit Should we save the file to disk even if there
		//						have been no recorded changes.
		//
		// @throw GameException& If no options file has been loaded into
		//							memory yet.
		//
		// /////////////////////////////////////////////////////////////////
		void Commit(const bool forceCommit = false) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Is there any options file loaded into memory yet.
		//
		// @return bool True if the file is loaded and false otherwise.
		//
		// /////////////////////////////////////////////////////////////////
		bool IsOptionsFileLoaded() const;

		// /////////////////////////////////////////////////////////////////
		// Sets the game logging pointer.
		//
		// /////////////////////////////////////////////////////////////////
		void SetLoggingPtr(boost::shared_ptr<GameLog> loggerPtr);

	};

	// /////////////////////////////////////////////////////////////////
	// @class IGameOptionsFactory
	// @author PJ O Halloran
	//
	// Base class interface that all games will extend to create and read
	// in their own custom options files.
	//
	// /////////////////////////////////////////////////////////////////
	class IGameOptionsFactory
	{
	private:

	protected:

		boost::shared_ptr<GameOptions> m_gameOptionsPtr;				///< Shared pointer to the GameOptions object generated.

		// /////////////////////////////////////////////////////////////////
		// Adds the default options to the options file.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddDefaultOptions() = 0;

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// Calls the ReadOptions() and/or AddDefaultOptions() to be implemented
		// by derived classes.
		//
		// Creates a GameOptions object from the options file provided.
		// If there is no valid options file at the path specified, then
		// it will attempt to create a new one with default options.
		//
		// @param optionsFilePathRef The path of the options file.
		//
		// /////////////////////////////////////////////////////////////////
		IGameOptionsFactory(const boost::filesystem::path &optionsFilePathRef);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~IGameOptionsFactory();

		// /////////////////////////////////////////////////////////////////
		// Creates a GameOptions object from the options file provided.
		// If there is no valid options file at the path specified, then
		// it will attempt to create a new one with default options.
		//
		// @param optionsFilePathRef The path of the options file.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VReadOptions(const boost::filesystem::path &optionsFilePathRef) = 0;

		// /////////////////////////////////////////////////////////////////
		// Get the shared pointer to the GameOptions object.
		//
		// /////////////////////////////////////////////////////////////////
		boost::shared_ptr<GameOptions> GetGameOptionsSPtr();

	};

	// /////////////////////////////////////////////////////////////////
	// A template helper function for getting values from a GameOptions
	// object and converting them into a specific type.
	// 
	// This method uses the boost::lexical_cast<> function and therefore
	// will only successfully convert basic C++ types (e.g. int, string,
	// F32, etc.).  It also might successfully work on types that 
	// implement their own >> operators however I have not tested that!
	//
	// @param optionsPtr Pointer to the GameOptions class.
	// @param optionName The name of the option to look up.
	// @param type The type of option (@see GameOptions::OptionType).
	// @param outputVal The object to store the converted result inside.
	//
	// @return bool True on successfull conversion and false otherwise.
	//
	// /////////////////////////////////////////////////////////////////
	template<typename T>
		bool RetrieveAndConvertOption(boost::shared_ptr<GameOptions> &optionsPtr, const std::string &optionName, const GameOptions::OptionType type, T &outputVal)
	{
		std::string valueStr;			// Value got from options file.
		bool result(false);				// Result of method.

		if(optionsPtr)
		{
			// Get the option value and convert it to the type requested.
			optionsPtr->Get(optionName, valueStr, type);
			try
			{
				outputVal = boost::lexical_cast<T, std::string>(valueStr);
				result = true;
			}
			catch(boost::bad_lexical_cast &)
			{
				result = false;
			}
		}

		return (result);
	};

	// /////////////////////////////////////////////////////////////////
	// A template helper function for converting objects to std::string
	// and setting the GameOptions values from them.
	// 
	// This method uses the boost::lexical_cast<> function and therefore
	// will only successfully convert basic C++ types (e.g. int, string,
	// F32, etc.).  It also might successfully work on types that 
	// implement their own >> operators however I have not tested that!
	//
	// @param optionsPtr Pointer to the GameOptions class.
	// @param optionName The name of the option to look up.
	// @param type The type of option (@see GameOptions::OptionType).
	// @param inputVal The input object to convert and set.
	//
	// @return bool True on successfull conversion and false otherwise.
	//
	// /////////////////////////////////////////////////////////////////
	template<typename T>
		bool SetAndConvertOption(boost::shared_ptr<GameOptions> &optionsPtr, const std::string &optionName, const GameOptions::OptionType type, const T &inputVal)
	{
		bool result(false);					// Result of method.

		if(optionsPtr)
		{
			try
			{
				std::string valueStr(boost::lexical_cast<std::string, T>(inputVal));
				optionsPtr->Edit(optionName, valueStr, type);
				result = true;
			}
			catch(boost::bad_lexical_cast &)
			{
				result = false;
			}
		}

		return (result);
	}

}

#ifdef WIN32
#	pragma warning( pop )
#endif

#endif
