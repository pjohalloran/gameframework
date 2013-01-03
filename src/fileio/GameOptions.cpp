// /////////////////////////////////////////////////////////////////
// @file GameOptions.cpp
// @author PJ O Halloran
// @date 25/06/2010
//
// File contains the implementation of the GameOptions class.
//
// /////////////////////////////////////////////////////////////////

#include <vector>

#include <boost/algorithm/string.hpp>

#include "GameOptions.h"
#include "GameBase.h"
#include "GameMain.h"
#include "GameException.h"
#include "GameLog.h"

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

namespace GameHalloran
{

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
		
		if(!exists(filePathRef))
		{
            GF_LOG_ERR(string("The path ") + filePathRef.string() + string(" does not exist"));
			result = false;
		}

		if(result && !filePathRef.has_filename())
		{
            GF_LOG_INF(string("The path ") + filePathRef.string() + string(" is not a file"));
			result = false;
		}

		if(result)
		{
			const string fileExt = filePathRef.extension().string();
			if(!iends_with(fileExt, XML_EXT))
			{
                GF_LOG_INF(string("The file ") + filePathRef.string() + string(" does not have an xml extension"));
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

		TiXmlHandle docHandle(&doc);

		if(m_logPtr && (m_logPtr->GetLogLevel() >= GameLog::TRC))
		{
			doc.Print();
		}

		TiXmlElement *rootElemPtr = docHandle.FirstChild(ROOT_NODE.c_str()).ToElement();
		if(!rootElemPtr)
		{
			throw (GameException(string("The file is not a valid game options file (No \"Options\" root node)")));
		}

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

		const char *idValueStr = optionsTypeElementPtr->Attribute(OPTION_TYPE_ID.c_str());
		if(IsValidGameOptionId(idValueStr))
		{
			for(TiXmlElement *currChildPtr = optionsTypeElementPtr->FirstChildElement(); currChildPtr; currChildPtr = currChildPtr->NextSiblingElement())
			{
				name.clear();
				key.clear();
				value.clear();
				
				if(currChildPtr && !currChildPtr->ToComment())
				{
					name.assign(currChildPtr->Value());
					if(currChildPtr->GetText())
					{
						value.assign(currChildPtr->GetText());
					}

					if(!CreateKey(idValueStr, name, key))
					{
                        GF_LOG_ERR(string("Failed to create the key for the ") + name + string(" option so we will not add it"));
					}
					else
					{
						m_optionsMap[key] = value;
					}
				}
			}
		}
		else
		{
            GF_LOG_TRACE_ERR("GameOptions::ParseOptions()", string("The current ") + OPTION_TYPE_NODE + string(" did not have a valid id: ") + string(idValueStr)); 
		}
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool GameOptions::IsValidGameOptionId(const char *optionIdStr) const
	{
		bool result = true;					// Result of the method.

		if(!optionIdStr)
		{
            GF_LOG_TRACE_DEB("GameOptions::IsValidGameOptionId()", "The option ID string retrieved from the options file is NULL");
			result = false;
		}

		if(result)
		{
			try
			{
				lexical_cast<I32>(optionIdStr);
			}
			catch(bad_lexical_cast &)
			{
                GF_LOG_TRACE_DEB("GameOptions::IsValidGameOptionId()", string("The option ID string retrieved from the options file is not a valid number: ") + std::string(optionIdStr));
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

		if(!keyRef.empty())
		{
			keyRef.clear();
		}

		if(optionIdStr.empty())
		{
			result = false;
            GF_LOG_TRACE_DEB("GameOptions::CreateKey()", "The option ID was an empty string");
		}
		if(result && optionNameRef.empty())
		{
			result = false;
            GF_LOG_TRACE_DEB("GameOptions::CreateKey()", "The option name was an empty string");
		}

		if(result)
		{
			keyRef.assign(optionIdStr + DOT_SEPERATOR + optionNameRef);
            GF_LOG_TRACE_TRC("GameOptions::CreateKey()", string("Created the key: ") + keyRef);
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

		try
		{
			idStr = lexical_cast<string, I32>(optionType);
		}
		catch(bad_lexical_cast &)
		{
            GF_LOG_TRACE_ERR("GameOptions::AssembleKey()", "Failed to convert the OptionType ID supplied to a string");
			result = false;
		}

		if(result && !CreateKey(idStr, optionNameRef, keyRef))
		{
            GF_LOG_TRACE_ERR("GameOptions::AssembleKey()", string("Failed to generate the key from the option type id (") + idStr + string(" and option name supplied: ") + optionNameRef);
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
            GF_LOG_TRACE_ERR("GameOptions::ExtractIdAndOptionsName()", "The key is empty");
			result = false;
		}

		if(!optionsName.empty())
		{
			optionsName.clear();
		}

		vector<string> tokens;
		if(result)
		{
			split(tokens, keyRef, is_any_of(GameOptions::DOT_SEPERATOR));

			if(tokens.size() != 2)
			{
                GF_LOG_TRACE_ERR("GameOptions::ExtractIdAndOptionsName()", string("The key ") + keyRef + string(" contained an invalid number of tokens"));
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
                GF_LOG_TRACE_ERR("GameOptions::ExtractIdAndOptionsName()", string("Failed to convert the ID ") + tokens[0] + string(" to a number"));
				result = false;
			}
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	GameOptions::GameOptions() : m_isModified(false)
									, m_isValidFileOpen(false)
									, m_optionsFilePath()
									, m_optionsMap()
									, m_logPtr()
	{
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	GameOptions::GameOptions(shared_ptr<GameLog> &logPtr,
								const path &filePathRef) throw (GameException &)
								: m_isModified(false)
								, m_isValidFileOpen(false)
								, m_optionsFilePath(filePathRef)
								, m_optionsMap()
								, m_logPtr(logPtr)
	{
		ParseFile(m_optionsFilePath);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	GameOptions::~GameOptions()
	{
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameOptions::ParseFile(const path &filePathRef) throw (GameException &)
	{
		if(!m_isValidFileOpen)
		{
			m_optionsFilePath = filePathRef;
			m_optionsMap.clear();
			m_isModified = false;
			m_isValidFileOpen = false;
		}

		if(!IsValidXmlFilePath(m_optionsFilePath))
		{
			throw GameException(string("The filepath ") + m_optionsFilePath.string() \
				+ string(" is not a valid filepath."));
		}
		
		ParseXmlFileHelper();
		m_isValidFileOpen = true;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameOptions::Get(const string &optionNameRef, string &valueRef, const OptionType optionType)
	{
		string key;							// The key generated from the id and option name.

		if(optionNameRef.empty())
		{
			return;
		}

		if(AssembleKey(optionType, optionNameRef, key))
		{
			valueRef.assign(m_optionsMap[key]);
            GF_LOG_TRC(string("Got value ") + valueRef + string(" from the option ") + key);
		}
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameOptions::Append(const string &optionNameRef, const string &valueRef, const OptionType optionType)
	{
		string key;							// The key generated from the id and option name.

		if(optionNameRef.empty())
		{
			return;
		}

		if(AssembleKey(optionType, optionNameRef, key))
		{
			m_optionsMap[key] = valueRef;
            GF_LOG_DEB(string("Set value ") + valueRef + string(" for the option ") + key);

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

		if(optionNameRef.empty())
		{
			return;
		}

		if(AssembleKey(optionType, optionNameRef, key))
		{
			std::map<std::string, std::string>::iterator i = m_optionsMap.find(key);
			if(i != m_optionsMap.end())
			{
				m_optionsMap[key] = valueRef;
                GF_LOG_DEB(string("Set value ") + valueRef + string(" for the option ") + key);

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

		if(optionNameRef.empty())
		{
			return;
		}

		if(AssembleKey(optionType, optionNameRef, key))
		{
			m_optionsMap.erase(key);
            GF_LOG_DEB(string("Removed (if it exists) option ") + key);

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

		try
		{
			typeStr = lexical_cast<string, I32>((I32) optionType);
		}
		catch(bad_lexical_cast &)
		{
            GF_LOG_TRACE_ERR("GameOptions::Remove(OptionType)", "Failed remove all options (failed to convert the optionType to a string)");
			return;
		}

		if(!error)
		{
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

		if(!exists(fileDir))
		{
            GF_LOG_TRACE_INF("GameOptions::Save()", string("The directory ") + fileDir.string() + string(" does not exist so will attempt to create it"));
			if(!create_directory(fileDir))
			{
				throw GameException(string("Failed to create the directory ") + fileDir.string());
			}
		}

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

		if(m_isModified || forceCommit)
		{
			TiXmlDocument doc;												// XML document object for building XML file.
			string key;														// The current key.
			string value;													// The current value.
			string name;													// The current option name.
			OptionType id = PLAYER;											// The current 
			vector<TiXmlElement *> optionTypeVec;							// Vector of OptionType's

			doc.LinkEndChild(GCC_NEW TiXmlDeclaration("1.0", "utf-8", ""));

			doc.LinkEndChild(GCC_NEW TiXmlComment("If you are manually editing the file, remember that the OptionsType id must be unique, must start at 0 and be incremented according to how the OptionsType are layed out in the file!"));

			TiXmlElement *rootPtr = GCC_NEW TiXmlElement(GameOptions::ROOT_NODE.c_str());
			doc.LinkEndChild(rootPtr);

			for(map<string, string>::iterator curr = m_optionsMap.begin(); curr != m_optionsMap.end(); ++curr)
			{
				name.clear();
				id = PLAYER;
				key.assign(curr->first);
				value.assign(curr->second);

				if(!ExtractIdAndOptionsName(key, name, id))
				{
                    GF_LOG_TRACE_ERR("GameOptions::Commit()", string("Failed to extract id and name from the key ") + key);
				}
				else
				{
					if((id + 1) > ((I32)optionTypeVec.size()))
					{
						TiXmlElement *elemPtr = GCC_NEW TiXmlElement(GameOptions::OPTION_TYPE_NODE.c_str());
						elemPtr->SetAttribute(GameOptions::OPTION_TYPE_ID.c_str(), (I32) id);

						rootPtr->LinkEndChild(elemPtr);
						optionTypeVec.push_back(elemPtr);
					}

					TiXmlElement *nameValueElemPtr = GCC_NEW TiXmlElement(name.c_str());
					nameValueElemPtr->LinkEndChild(GCC_NEW TiXmlText(value.c_str()));
					optionTypeVec[id]->LinkEndChild(nameValueElemPtr);
				}
			}

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
	void GameOptions::SetLoggingPtr(shared_ptr<GameLog> &loggerPtr)
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
