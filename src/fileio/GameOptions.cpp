// /////////////////////////////////////////////////////////////////
// @file GameOptions.cpp
// @author PJ O Halloran
// @date 25/06/2010
//
// File contains the implementation of the GameOptions class.
//
// /////////////////////////////////////////////////////////////////

// External headers
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

// Project headers
#include "GameOptions.h"
#include "GameBase.h"
#include "GameMain.h"

// Namespace Declarations
using std::string;
using std::map;
using std::vector;

using boost::filesystem::path;
using boost::filesystem::exists;
using boost::filesystem::create_directory;
using boost::algorithm::iends_with;
using boost::algorithm::split;
using boost::algorithm::is_any_of;
using boost::lexical_cast;
using boost::bad_lexical_cast;
using boost::shared_ptr;


// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// Definition of static class constants.
	const std::string GameOptions::XML_EXT = string(".xml");
	const std::string GameOptions::ROOT_NODE = string("Options");
	const std::string GameOptions::OPTION_TYPE_NODE = string("OptionType");
	const std::string GameOptions::OPTION_TYPE_ID = string("id");
	const std::string GameOptions::DOT_SEPERATOR = string(".");

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GameOptions::IsValidXmlFilePath(const path &filePathRef) const
	{
		bool result = true;						// Result of the method.
		
		// Check if the path exists.
		if(!exists(filePathRef))
		{
            GF_LOG_ERR(string("The path ") + filePathRef.string() + string(" does not exist"));
//			SafeGameLog(m_logPtr, GameLog::ERR, string("The path ") + filePathRef.string() + string(" does not exist"));
			result = false;
		}

		// Check the path has a filename.
		if(result && !filePathRef.has_filename())
		{
            GF_LOG_INF(string("The path ") + filePathRef.string() + string(" is not a file"));
//			SafeGameLog(m_logPtr, GameLog::INF, string("The path ") + filePathRef.string() + string(" is not a file"));
			result = false;
		}

		// Check the filename has an xml extension.
		if(result)
		{
			const string fileExt = filePathRef.extension().string();
			if(!iends_with(fileExt, XML_EXT))
			{
                GF_LOG_INF(string("The file ") + filePathRef.string() + string(" does not have an xml extension"));
//				SafeGameLog(m_logPtr, GameLog::INF, string("The file ") + filePathRef.string() + string(" does not have an xml extension"));
				result = false;
			}
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameOptions::ParseXmlFileHelper() throw (GameException &)
	{
		TiXmlDocument doc(m_optionsFilePath.string().c_str());		// XML document.

		if(!doc.LoadFile())
		{
			throw GameException(string("Failed to load the options file supplied: ") + string(doc.ErrorDesc()));
		}

		// Handle to the document.
		TiXmlHandle docHandle(&doc);

		// Perform some standerd output for debugging if the debug level of the app is above a certain level.
		if(m_logPtr && (m_logPtr->GetLogLevel() >= GameLog::TRC))
		{
			doc.Print();
		}

		// Get the Options root node.
		TiXmlElement *rootElemPtr = docHandle.FirstChild(ROOT_NODE.c_str()).ToElement();
		if(!rootElemPtr)
		{
			throw (GameException(string("The file is not a valid game options file (No \"Options\" root node)")));
		}

		// Parse Options children (OptionType)
		for(TiXmlElement *currElemPtr = rootElemPtr->FirstChildElement(); currElemPtr; currElemPtr = currElemPtr->NextSiblingElement())
		{
			if(currElemPtr && !currElemPtr->ToComment())
			{
				ParseOptions(currElemPtr);
			}
		}
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameOptions::ParseOptions(TiXmlElement *optionsTypeElementPtr) throw (GameException &)
	{
		string name("");					// The name of the current option.
		string key("");						// The key for the current option in the map.
		string value("");					// The value of the current option.

		if(!optionsTypeElementPtr)
		{
			throw GameException(string("The pointer to the ") + OPTION_TYPE_NODE + string(" is NULL"));
		}

		// Get the ID value.
		const char *idValueStr = optionsTypeElementPtr->Attribute(OPTION_TYPE_ID.c_str());
		if(IsValidGameOptionId(idValueStr))
		{
			for(TiXmlElement *currChildPtr = optionsTypeElementPtr->FirstChildElement(); currChildPtr; currChildPtr = currChildPtr->NextSiblingElement())
			{
				name.clear();
				key.clear();
				value.clear();
				
				// If the current element pointer is valid and it is not an xml comment, then parse it further.
				if(currChildPtr && !currChildPtr->ToComment())
				{
					name.assign(currChildPtr->Value());
					if(currChildPtr->GetText())
					{
						value.assign(currChildPtr->GetText());
					}

					if(!CreateKey(idValueStr, name, key))
					{
						// Log this and skip the option.
//                        GF_LOG_ERR(string("Failed to create the key for the ") + name + string(" option so we will not add it"));
						SafeGameLog(m_logPtr, GameLog::ERR, string("Failed to create the key for the ") + name + string(" option so we will not add it"));
					}
					else
					{
//                        GF_LOG_TRC(string("Adding [") + key + string("] = ") + value + string(" to the game options"));
						m_optionsMap[key] = value;
					}
				}
			}
		}
		else
		{
//            GF_LOG_TRACE_ERR("GameOptions::ParseOptions()", string("The current ") + OPTION_TYPE_NODE + string(" did not have a valid id: ") + string(idValueStr));
			SafeGameLogAndPrefix(m_logPtr, GameLog::ERR, string("GameOptions::ParseOptions()"),\
				string("The current ") + OPTION_TYPE_NODE + string(" did not have a valid id (") + string(idValueStr) + string(").")); 
		}
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool GameOptions::IsValidGameOptionId(const char *optionIdStr) const
	{
		bool result = true;					// Result of the method.

		// Ensure the ID is not NULL.
		if(!optionIdStr)
		{
//            GF_LOG_TRACE_DEB("GameOptions::IsValidGameOptionId()", "The option ID string retrieved from the options file is NULL");
			SafeGameLogAndPrefix(m_logPtr, GameLog::DEB, string("GameOptions::IsValidGameOptionId()"), string("The option ID string retrieved from the options file is NULL"));
			result = false;
		}

		// Ensure the ID is a valid number
		if(result)
		{
			try
			{
				lexical_cast<I32>(optionIdStr);
			}
			catch(bad_lexical_cast &)
			{
//                GF_LOG_TRACE_DEB("GameOptions::IsValidGameOptionId()", string("The option ID string retrieved from the options file is not a valid number: ") + std::string(optionIdStr));
				SafeGameLogAndPrefix(m_logPtr, GameLog::DEB, string("GameOptions::IsValidGameOptionId()"),\
					string("The option ID string retrieved from the options file is not a valid number (") + std::string(optionIdStr) + string(")"));
				result = false;
			}
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool GameOptions::CreateKey(const string &optionIdStr, const string &optionNameRef, string &keyRef) const
	{
		bool result = true;				// Result of the method.

		// Clear the output parameter on entry.
		if(!keyRef.empty())
		{
			keyRef.clear();
		}

		// Check input parameters.
		if(optionIdStr.empty())
		{
			result = false;
//            GF_LOG_TRACE_DEB("GameOptions::CreateKey()", "The option ID was an empty string");
			SafeGameLogAndPrefix(m_logPtr, GameLog::DEB, string("GameOptions::CreateKey()"), string("The option ID was an empty string"));
		}
		if(result && optionNameRef.empty())
		{
			result = false;
//            GF_LOG_TRACE_DEB("GameOptions::CreateKey()", "The option name was an empty string");
			SafeGameLogAndPrefix(m_logPtr, GameLog::DEB, string("GameOptions::CreateKey()"), string("The option name was an empty string"));
		}

		// Generate the key.
		if(result)
		{
			// The key = id number + "." + option name (e.g. 0.Width, 1.UpMovement, etc.).
			keyRef.assign(optionIdStr + DOT_SEPERATOR + optionNameRef);
//            GF_LOG_TRACE_TRC("GameOptions::CreateKey()", string("Created the key: ") + keyRef);
			SafeGameLogAndPrefix(m_logPtr, GameLog::TRC, string("GameOptions::CreateKey()"), string("Created the key: ") + keyRef);
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool GameOptions::AssembleKey(const GameOptions::OptionType optionType, const std::string &optionNameRef, std::string &keyRef) const
	{
		bool result = true;					// Result of method.
		string idStr;						// String to store the converted ID inside.

		// Convert the type to a string.
		try
		{
			idStr = lexical_cast<string, I32>(optionType);
		}
		catch(bad_lexical_cast &)
		{
//            GF_LOG_TRACE_ERR("GameOptions::AssembleKey()", "Failed to convert the OptionType ID supplied to a string");
			SafeGameLogAndPrefix(m_logPtr, GameLog::ERR, string("GameOptions::AssembleKey()"), string("Failed to convert the OptionType ID supplied to a string"));
			result = false;
		}

		// Create the key to access the value.
		if(result && !CreateKey(idStr, optionNameRef, keyRef))
		{
//            GF_LOG_TRACE_ERR("GameOptions::AssembleKey()", string("Failed to generate the key from the option type id (") + idStr + string(" and option name supplied: ") + optionNameRef);
			SafeGameLogAndPrefix(m_logPtr, GameLog::ERR, string("GameOptions::AssembleKey"),\
				string("Failed to generate the key from the option type id (") + idStr + string(" and option name supplied (") + optionNameRef + string(")"));
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool GameOptions::ExtractIdAndOptionsName(const string &keyRef, string &optionsName, OptionType &id) const
	{
		bool result = true;				// Result of method.

		if(keyRef.empty())
		{
//            GF_LOG_TRACE_ERR("GameOptions::ExtractIdAndOptionsName()", "The key is empty");
			SafeGameLogAndPrefix(m_logPtr, GameLog::ERR, string("GameOptions::ExtractIdAndOptionsName()"), string("The key is empty"));
			result = false;
		}

		if(!optionsName.empty())
		{
			optionsName.clear();
		}

		vector<string> tokens;			// Vector of strings returned by the split algorithm.
		// Split the key into tokens
		if(result)
		{
			split(tokens, keyRef, is_any_of(GameOptions::DOT_SEPERATOR));

			if(tokens.size() != 2)
			{
//                GF_LOG_TRACE_ERR("GameOptions::ExtractIdAndOptionsName()", string("The key ") + keyRef + string(" contained an invalid number of tokens"));
				SafeGameLogAndPrefix(m_logPtr, GameLog::ERR, string("GameOptions::ExtractIdAndOptionsName()"), string("The key ") + keyRef + string(" contained an invalid number of tokens"));
				result = false;
			}
		}

		if(result)
		{
			try
			{
				I32 temp = lexical_cast<I32, string>(tokens[0]);
				id = (OptionType) temp;
				optionsName.assign(tokens[1]);
			}
			catch(bad_lexical_cast &)
			{
//                GF_LOG_TRACE_ERR("GameOptions::ExtractIdAndOptionsName()", string("Failed to convert the ID ") + tokens[0] + string(" to a number"));
				SafeGameLogAndPrefix(m_logPtr, GameLog::ERR, string("GameOptions::ExtractIdAndOptionsName()"), string("Failed to convert the ID ") + tokens[0] + string(" to a number"));
				result = false;
			}
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	GameOptions::GameOptions() : m_isModified(false),\
									m_isValidFileOpen(false),\
									m_optionsFilePath(),\
									m_optionsMap(),\
									m_logPtr()
	{
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	GameOptions::GameOptions(shared_ptr<GameLog> logPtr, const path &filePathRef) throw (GameException &) :	m_isModified(false),\
																				m_isValidFileOpen(false),\
																				m_optionsFilePath(filePathRef),\
																				m_optionsMap(),\
																				m_logPtr(logPtr)
	{
		ParseFile(m_optionsFilePath);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	GameOptions::~GameOptions()
	{
		try
		{
			
		}
		catch(...)
		{
		}
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameOptions::ParseFile(const path &filePathRef) throw (GameException &)
	{
		// If there is a file already loaded into memory, then discard it.
		if(!m_isValidFileOpen)
		{
            //GF_LOG_INF("Clearing the previous options file from memory");
			SafeGameLog(m_logPtr, GameLog::INF, string("Clearing the previous options file from memory."));
			m_optionsFilePath = filePathRef;
			m_optionsMap.clear();
			m_isModified = false;
			m_isValidFileOpen = false;
		}

		// Check the path supplied is a valid xml file.
		if(!IsValidXmlFilePath(m_optionsFilePath))
		{
			throw GameException(string("The filepath ") + m_optionsFilePath.string() \
				+ string(" is not a valid filepath."));
		}
		
		ParseXmlFileHelper();

		// Set the file valid flag on exit, we will never get here unless the 
		//  file was loaded into memory successfully.
        //GF_LOG_INF(string("Options file ") + m_optionsFilePath.string() + string(" loaded successfully"));
		SafeGameLog(m_logPtr, GameLog::INF, string("Options file ") + m_optionsFilePath.string() + string(" loaded successfully"));
		m_isValidFileOpen = true;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameOptions::Get(const string &optionNameRef, string &valueRef, const OptionType optionType)
	{
		string key;							// The key generated from the id and option name.

		// If the option name is empty then exit.
		if(optionNameRef.empty())
		{
			return;
		}

		if(AssembleKey(optionType, optionNameRef, key))
		{
			// Get the value (please note if no value matching the key is found in the set then a new blank value will be added with the invalid key name!)
			valueRef.assign(m_optionsMap[key]);
            //GF_LOG_TRC(string("Got value ") + valueRef + string(" from the option ") + key);
			SafeGameLog(m_logPtr, GameLog::TRC, string("Got value ") + valueRef + string(" from the option ") + key);
		}
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameOptions::Append(const string &optionNameRef, const string &valueRef, const OptionType optionType)
	{
		string key;							// The key generated from the id and option name.

		// If the option name is empty then exit.
		if(optionNameRef.empty())
		{
			return;
		}

		if(AssembleKey(optionType, optionNameRef, key))
		{
			// Add/Set the value (please note if no value matching the key is found in the set then a new blank value will be added with the invalid key name!)
			m_optionsMap[key] = valueRef;
            //GF_LOG_DEB(string("Set value ") + valueRef + string(" for the option ") + key);
			SafeGameLog(m_logPtr, GameLog::DEB, string("Set value ") + valueRef + string(" for the option ") + key);

			if(!m_isModified)
			{
				m_isModified = true;
			}
		}
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameOptions::Edit(const string &optionNameRef, const string &valueRef, const OptionType optionType)
	{
		string key;							// The key generated from the id and option name.

		// If the option name is empty then exit.
		if(optionNameRef.empty())
		{
			return;
		}

		if(AssembleKey(optionType, optionNameRef, key))
		{
			// Search for the key in the container first.
			std::map<std::string, std::string>::iterator i = m_optionsMap.find(key);
			if(i != m_optionsMap.end())
			{
				// Set the new value (please note if no value matching the key is found in the set then a new blank value will be added with the invalid key name!)
				m_optionsMap[key] = valueRef;
                //GF_LOG_DEB(string("Set value ") + valueRef + string(" for the option ") + key);
				SafeGameLog(m_logPtr, GameLog::DEB, string("Set value ") + valueRef + string(" for the option ") + key);

				if(!m_isModified)
				{
					m_isModified = true;
				}
			}
		}
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameOptions::Remove(const std::string &optionNameRef, const OptionType optionType)
	{
		string key;							// The key generated from the id and option name.

		// If the option name is empty then exit.
		if(optionNameRef.empty())
		{
			return;
		}

		if(AssembleKey(optionType, optionNameRef, key))
		{
			m_optionsMap.erase(key);
            //GF_LOG_DEB(string("Removed (if it exists) option ") + key);
			SafeGameLog(m_logPtr, GameLog::DEB, string("Removed (if it exists) option ") + key);

			if(!m_isModified)
			{
				m_isModified = true;
			}
		}
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameOptions::Remove(const OptionType optionType)
	{
		bool error = false;				// Has an error occurred.
		string key;						// The current key.
		string typeStr;					// OptionType converted to a string.

		// Convert option type to a string.
		try
		{
			typeStr = lexical_cast<string, I32>((I32) optionType);
		}
		catch(bad_lexical_cast &)
		{
            //GF_LOG_TRACE_ERR("GameOptions::Remove(OptionType)", "Failed remove all options (failed to convert the optionType to a string)");
			SafeGameLogAndPrefix(m_logPtr, GameLog::ERR, string("GameOptions::Remove(OptionType)"), string("Failed remove all options (failed to convert the optionType to a string)."));
			return;
		}

		if(!error)
		{
			// Remove all options in map beginning with the OptionType ID.
			bool memberErased = false;
			for(map<string, string>::iterator curr = m_optionsMap.begin(); curr != m_optionsMap.end(); (memberErased ? curr = m_optionsMap.begin() : ++curr))
			{
				memberErased = false;
				key.assign(curr->first);

				if(boost::algorithm::istarts_with(key, typeStr))
				{
					m_optionsMap.erase(key);
					memberErased = true;
				}
			}
		}
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameOptions::Save(const path &newFilePathRef) throw (GameException &)
	{
		path fileDir = newFilePathRef.parent_path();			// Directory where new options file will be placed.

		if(!m_isValidFileOpen)
		{
			throw (GameException(string("There is no options file loaded so cannot perform the save")));
		}

		// Create the directory if it does not exist.
		if(!exists(fileDir))
		{
            //GF_LOG_TRACE_INF("GameOptions::Save()", string("The directory ") + fileDir.string() + string(" does not exist so will attempt to create it"));
			SafeGameLogAndPrefix(m_logPtr, GameLog::INF, string("GameOptions::Save()"), string("The directory ") + fileDir.string() + string(" does not exist so will attempt to create it"));
			if(!create_directory(fileDir))
			{
				throw GameException(string("Failed to create the directory ") + fileDir.string());
			}
		}

		// Set the file path to save the new options file to and commit the changes to it.
		m_optionsFilePath = newFilePathRef;
		Commit(true);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameOptions::Commit(const bool forceCommit) throw (GameException &)
	{
		if(!m_isValidFileOpen)
		{
			throw (GameException(string("There is no options file loaded so cannot perform the commit")));
		}

		// Save data in the map to the xml file if there has been any changes
		//  or if we are being forced to commit.
		if(m_isModified || forceCommit)
		{
			TiXmlDocument doc;												// XML document object for building XML file.
			string key;														// The current key.
			string value;													// The current value.
			string name;													// The current option name.
			OptionType id = PLAYER;	// The current 
			vector<TiXmlElement *> optionTypeVec;							// Vector of OptionType's

			// NB: tinyxml takes in pointers allocated off the heap and frees the memory internally
			//  so theres no need to worry about all the LinkEndChild(GCC_NEW TiXmlElement()) calls here.

			// Create XML declaration node.
			doc.LinkEndChild(GCC_NEW TiXmlDeclaration("1.0", "utf-8", ""));

			// Create rules comment node and add to the doc.
			doc.LinkEndChild(GCC_NEW TiXmlComment("If you are manually editing the file, remember that the OptionsType id must be unique, must start at 0 and be incremented according to how the OptionsType are layed out in the file!"));

			// Create ROOT node and add to the doc.
			TiXmlElement *rootPtr = GCC_NEW TiXmlElement(GameOptions::ROOT_NODE.c_str());
			doc.LinkEndChild(rootPtr);

			// Create various child OptionsType nodes and add them to the root element.
			for(map<string, string>::iterator curr = m_optionsMap.begin(); curr != m_optionsMap.end(); ++curr)
			{
				name.clear();
				id = PLAYER;
				key.assign(curr->first);
				value.assign(curr->second);

				if(!ExtractIdAndOptionsName(key, name, id))
				{
					// skip this option and log error
                    //GF_LOG_TRACE_ERR("GameOptions::Commit()", string("Failed to extract id and name from the key ") + key);
					SafeGameLogAndPrefix(m_logPtr, GameLog::ERR, string("GameOptions::Commit()"), string("Failed to extract id and name from the key ") + key);
				}
				else
				{
					if((id + 1) > ((I32)optionTypeVec.size()))
					{
						// Create new OptionsType element and add the id attribute.
						TiXmlElement *elemPtr = GCC_NEW TiXmlElement(GameOptions::OPTION_TYPE_NODE.c_str());
						elemPtr->SetAttribute(GameOptions::OPTION_TYPE_ID.c_str(), (I32) id);

						// Add element to the root Options node.
						rootPtr->LinkEndChild(elemPtr);

						// Add element to the vector managing the OptionType nodes created.
						optionTypeVec.push_back(elemPtr);
					}

					// Create the name/value element called "name" and add the text element with the text "value" to it
					TiXmlElement *nameValueElemPtr = GCC_NEW TiXmlElement(name.c_str());
					nameValueElemPtr->LinkEndChild(GCC_NEW TiXmlText(value.c_str()));

					// Add the name/value element to the OptionsType node.
					optionTypeVec[id]->LinkEndChild(nameValueElemPtr);
				}
			}

			// Save the xml file to the path.
			doc.SaveFile(m_optionsFilePath.string().c_str());
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GameOptions::IsOptionsFileLoaded() const
	{
		return (m_isValidFileOpen);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameOptions::SetLoggingPtr(shared_ptr<GameLog> loggerPtr)
	{
		m_logPtr = loggerPtr;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	IGameOptionsFactory::IGameOptionsFactory(const path &optionsFilePathRef) : m_gameOptionsPtr()
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	IGameOptionsFactory::~IGameOptionsFactory()
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	shared_ptr<GameOptions> IGameOptionsFactory::GetGameOptionsSPtr()
	{
		return (m_gameOptionsPtr);
	}

}
