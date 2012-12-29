//========================================================================
// GameCode3 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 3rd Edition" by Mike McShaffry, published by
// Charles River Media. ISBN-10: 1-58450-680-6   ISBN-13: 978-1-58450-680-5
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the author a favor and buy a new copy of the book - there are 
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: 
//    http://www.amazon.com/gp/product/1584506806?ie=UTF8&tag=gamecodecompl-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=1584506806
//
// There's a companion web site at http://www.mcshaffry.com/GameCode/
// 
// The source code is managed and maintained through Google Code:
// http://gamecode3.googlecode.com/svn/trunk/
//
// (c) Copyright 2009 Michael L. McShaffry
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License v2
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================

// /////////////////////////////////////////////////////////////////
// @file LuaStateManager.cpp
// @author Mike McShaffry et al.
// @date 13/07/2010
//
// File contains the implementation of the LuaStateManager class.
//
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry et al., author(s) of Game
// Coding Complete and games programmer(s).
// 
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// Please see the associated header file for details on any changes
// I have made.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <assert.h>

#include <algorithm>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

// Project Headers
#include "LuaStateManager.h"

#include "Events.h"
#include "TextResource.h"
#include "GameOptions.h"
#include "GameLog.h"
#include "GameMain.h"

using LuaPlus::LuaStateOwner;
using LuaPlus::LuaObject;

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// ******************** LuaStateManager ****************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	LuaStateManager::LuaStateManager(void) : m_GlobalState(true)
	{
		//Create our metatable...
		m_MetaTable = m_GlobalState->GetGlobals().CreateTable("LuaStateManagerMetaTable");
		m_MetaTable.SetObject("__index", m_MetaTable);

		// Here we register two functions to make them accessible to script.
		m_MetaTable.RegisterObjectDirect("DoFile", (LuaStateManager *)0, &LuaStateManager::DoFile);
		m_MetaTable.RegisterObjectDirect("DoResourceFile", (LuaStateManager *)0, &LuaStateManager::DoResourceFile);
		m_MetaTable.RegisterObjectDirect("PrintDebugMessage", (LuaStateManager *)0, &LuaStateManager::PrintDebugMessage);
		
		LuaPlus::LuaObject luaStateManObj = m_GlobalState->BoxPointer(this);
		luaStateManObj.SetMetaTable(m_MetaTable);

		// And here we expose the metatable as a named entity.
		m_GlobalState->GetGlobals().SetObject("LuaStateManager", luaStateManObj);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	LuaStateManager::~LuaStateManager()
	{
		try { Shutdown(); } catch(...) {}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool LuaStateManager::Init(char const * const pInitFileName)
	{
		// Create our global actor table.
		// This table will hold context for all actors created in the game world.
		LuaPlus::LuaObject globals = m_GlobalState->GetGlobals();
		LuaPlus::LuaObject actorTable = globals.CreateTable("ActorList");

		// Execute the init file and setup some useful global variables for the lua scripts to know.
		if(DoFile(pInitFileName))
		{
			std::string luaCommand;						// The LUA command to execute.
			std::string currOpVal;						// The current option value retrieved.
            boost::shared_ptr<GameOptions> opPtr = g_appPtr->GetGameOptions();
            
			// Write out the location of the game root directory.
			boost::filesystem::path gameRoot(g_appPtr->GetGameRootDir());
			//if(GameHalloran::RetrieveAndConvertOption<std::string>(opPtr, std::string("GameRoot"), GameHalloran::GameOptions::PROGRAMMER, currOpVal))
			//{
			luaCommand = std::string("INIT_GAME_ROOT_PATH = \"") + gameRoot.generic_string() + std::string("\";");
			if(!ExecuteString(luaCommand.c_str()))
				return (false);
			currOpVal.clear();
			luaCommand.clear();
			//}

			// Write out what type of build is running.
#ifdef DEBUG
			luaCommand = std::string("INIT_RUNNING_DEBUG_BUILD = true;");
#else
			luaCommand = std::string("INIT_RUNNING_DEBUG_BUILD = false;");
#endif
			if(!ExecuteString(luaCommand.c_str()))
				return (false);
			luaCommand.clear();

			// Write out the various player options so they are available to the lua UI setup scripts.
			luaCommand = std::string("INIT_PLAYER_OPTIONS = {};");
			if(!ExecuteString(luaCommand.c_str()))
				return (false);
			luaCommand.clear();

			// Sound options.
			if(GameHalloran::RetrieveAndConvertOption<std::string>(opPtr, std::string("MasterVolume"), GameHalloran::GameOptions::PLAYER, currOpVal))
			{
				luaCommand = std::string("INIT_PLAYER_OPTIONS.MasterVolume = ") + currOpVal + std::string(";");
				if(!ExecuteString(luaCommand.c_str()))
					return (false);
				currOpVal.clear();
				luaCommand.clear();
			}
			bool flag;
			if(GameHalloran::RetrieveAndConvertOption<bool>(opPtr, std::string("Music"), GameHalloran::GameOptions::PLAYER, flag))
			{
				luaCommand = std::string("INIT_PLAYER_OPTIONS.Music = ") + (flag ? std::string("true") : std::string("false")) + std::string(";");
				if(!ExecuteString(luaCommand.c_str()))
					return (false);
				luaCommand.clear();
			}
			if(GameHalloran::RetrieveAndConvertOption<bool>(opPtr, std::string("SoundFx"), GameHalloran::GameOptions::PLAYER, flag))
			{
				luaCommand = std::string("INIT_PLAYER_OPTIONS.SoundFx = ") + (flag ? std::string("true") : std::string("false")) + std::string(";");
				if(!ExecuteString(luaCommand.c_str()))
					return (false);
				luaCommand.clear();
			}

			// Graphics options.
			if(GameHalloran::RetrieveAndConvertOption<bool>(opPtr, std::string("RenderShadows"), GameHalloran::GameOptions::PLAYER, flag))
			{
				luaCommand = std::string("INIT_PLAYER_OPTIONS.RenderShadows = ") + (flag ? std::string("true") : std::string("false")) + std::string(";");
				if(!ExecuteString(luaCommand.c_str()))
					return (false);
				luaCommand.clear();
			}
			if(GameHalloran::RetrieveAndConvertOption<std::string>(opPtr, std::string("ScreenWidth"), GameHalloran::GameOptions::PLAYER, currOpVal))
			{
				luaCommand = std::string("INIT_PLAYER_OPTIONS.ScreenResolution = \"") + currOpVal + std::string("*");
                boost::shared_ptr<GameOptions> opPtr = g_appPtr->GetGameOptions();
				if(GameHalloran::RetrieveAndConvertOption<std::string>(opPtr, std::string("ScreenHeight"), GameHalloran::GameOptions::PLAYER, currOpVal))
				{
					luaCommand += currOpVal + std::string("\";");
					if(!ExecuteString(luaCommand.c_str()))
						return (false);
				}
				currOpVal.clear();
				luaCommand.clear();
			}
			if(GameHalloran::RetrieveAndConvertOption<std::string>(opPtr, std::string("Multisampling"), GameHalloran::GameOptions::PLAYER, currOpVal))
			{
				currOpVal = std::string("x") + currOpVal;

				luaCommand = std::string("INIT_PLAYER_OPTIONS.Multisampling = \"") + currOpVal + std::string("\";");
				if(!ExecuteString(luaCommand.c_str()))
					return (false);
				currOpVal.clear();
				luaCommand.clear();
			}
			if(GameHalloran::RetrieveAndConvertOption<std::string>(opPtr, std::string("TextureFilteringType"), GameHalloran::GameOptions::PLAYER, currOpVal))
			{
				luaCommand = std::string("INIT_PLAYER_OPTIONS.TextureFilteringType = \"") + currOpVal + std::string("\";");
				if(!ExecuteString(luaCommand.c_str()))
					return (false);
				currOpVal.clear();
				luaCommand.clear();
			}

			return (true);
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void LuaStateManager::Shutdown()
	{
		// Write out the various player options so they are available to the lua UI setup scripts.
		LuaPlus::LuaObject luaPlayerOptions = m_GlobalState->GetGlobal("INIT_PLAYER_OPTIONS");

		boost::shared_ptr<GameOptions> goPtr = g_appPtr->GetGameOptions();
		if(!goPtr)
		{
			// We must have a valid options pointer to proceed!
			return;
		}

		// Sound options.
		std::string optionName("MasterVolume");
		if(luaPlayerOptions[optionName.c_str()].IsNumber())
		{
			if(!SetAndConvertOption<F32>(goPtr, optionName, GameOptions::PLAYER, luaPlayerOptions[optionName.c_str()].GetFloat()))
                GF_LOG_TRACE_ERR("LuaStateManager::Shutdown()", std::string("Failed to save (to the GameOptions file) the lua option: ") + optionName);
		}
		optionName.assign(std::string("Music"));
		if(luaPlayerOptions[optionName.c_str()].IsBoolean())
		{
			if(!SetAndConvertOption<bool>(goPtr, optionName, GameOptions::PLAYER, luaPlayerOptions[optionName.c_str()].GetBoolean()))
                GF_LOG_TRACE_ERR("LuaStateManager::Shutdown()", std::string("Failed to save (to the GameOptions file) the lua option: ") + optionName);
		}
		optionName.assign(std::string("SoundFx"));
		if(luaPlayerOptions[optionName.c_str()].IsBoolean())
		{
			if(!SetAndConvertOption<bool>(goPtr, optionName, GameOptions::PLAYER, luaPlayerOptions[optionName.c_str()].GetBoolean()))
                GF_LOG_TRACE_ERR("LuaStateManager::Shutdown()", std::string("Failed to save (to the GameOptions file) the lua option: ") + optionName);
		}

		// Graphics options.
		optionName.assign(std::string("RenderShadows"));
		if(luaPlayerOptions[optionName.c_str()].IsBoolean())
		{
			if(!SetAndConvertOption<bool>(goPtr, optionName, GameOptions::PLAYER, luaPlayerOptions[optionName.c_str()].GetBoolean()))
                GF_LOG_TRACE_ERR("LuaStateManager::Shutdown()", std::string("Failed to save (to the GameOptions file) the lua option: ") + optionName);
		}
		optionName.assign(std::string("Multisampling"));
		if(luaPlayerOptions[optionName.c_str()].IsString())
		{
			std::string value(luaPlayerOptions[optionName.c_str()].GetString());
			if(boost::algorithm::istarts_with(value, std::string("x")))
				value.assign(value.substr(1));
			if(!SetAndConvertOption<std::string>(goPtr, optionName, GameOptions::PLAYER, value))
                GF_LOG_TRACE_ERR("LuaStateManager::Shutdown()", std::string("Failed to save (to the GameOptions file) the lua option: ") + optionName);
		}
		optionName.assign(std::string("TextureFilteringType"));
		if(luaPlayerOptions[optionName.c_str()].IsString())
		{
			if(!SetAndConvertOption<std::string>(goPtr, optionName, GameOptions::PLAYER, luaPlayerOptions[optionName.c_str()].GetString()))
                GF_LOG_TRACE_ERR("LuaStateManager::Shutdown()", std::string("Failed to save (to the GameOptions file) the lua option: ") + optionName);
		}
		if(luaPlayerOptions["ScreenResolution"].IsString())
		{
			std::string resolutionStr(luaPlayerOptions["ScreenResolution"].GetString());
			std::vector<std::string> tokens;
			boost::algorithm::split(tokens, resolutionStr, boost::algorithm::is_any_of(std::string("*")));
			if(tokens.size() == 2 && boost::algorithm::all(tokens[0], boost::algorithm::is_digit()) && boost::algorithm::all(tokens[1], boost::algorithm::is_digit()))
			{
				optionName.assign(std::string("ScreenWidth"));
				if(!SetAndConvertOption<std::string>(goPtr, optionName, GameOptions::PLAYER, tokens[0]))
				{
                    GF_LOG_TRACE_ERR("LuaStateManager::Shutdown()", std::string("Failed to save (to the GameOptions file) the lua option: ") + optionName);
				}
				else
				{
					optionName.assign(std::string("ScreenHeight"));
					if(!SetAndConvertOption<std::string>(goPtr, optionName, GameOptions::PLAYER, tokens[1]))
                        GF_LOG_TRACE_ERR("LuaStateManager::Shutdown()", std::string("Failed to save (to the GameOptions file) the lua option: ") + optionName);
				}
			}
		}

		goPtr->Commit();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool LuaStateManager::DoFile(char const * const pFileName)
	{
		return (ExecuteFile(m_GlobalState, pFileName));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool LuaStateManager::DoResourceFile(const char * const scriptName)
	{
		return (ExecuteResourceFile(m_GlobalState, scriptName, g_appPtr->GetResourceCache().get()));
	};

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool LuaStateManager::ExecuteResourceFile(LuaPlus::LuaStateOwner &luaState, const char * const scriptName, ResCache *rcManagerPtr)
	{
		if(!rcManagerPtr || !scriptName || strlen(scriptName) == 0)
		{
			return (false);
		}

		std::string rn(scriptName);
		TextResource tr(rn);
		boost::shared_ptr<TextResHandle> scriptHandle = boost::static_pointer_cast<TextResHandle>(rcManagerPtr->GetHandle(&tr));
		if(!scriptHandle || !scriptHandle->VInitialize())
		{
            GF_LOG_TRACE_ERR("LuaStateManager::ExecuteResourceFile()", std::string("Failed to initialize from cache: ") + tr.GetName());
			return (false);
		}

		std::string scriptData(scriptHandle->GetTextBuffer());

		// Split up the file by the newline character.
#ifdef _WINDOWS
		std::string newLine("\n");
#else
		std::string newLine("\t\n");
#endif
		// Make a large vector to prevent repeating reallocations (500 - we dont know how long the file is, this is a guess to help reduce reallocations).
		std::vector<std::string> linesVec;
		linesVec.reserve(500);
		boost::algorithm::split(linesVec, scriptData, boost::algorithm::is_any_of(newLine));

		// Trim excess capacity.
		std::vector<std::string>(linesVec.begin(), linesVec.end()).swap(linesVec);

		RemoveTrailingCr fo;
		std::for_each(linesVec.begin(), linesVec.end(), fo);

		scriptData.clear();
		for(std::vector<std::string>::iterator i = linesVec.begin(), end = linesVec.end(); i != end; ++i)
		{
			scriptData += *i;
		}

		if(luaState->DoString(scriptData.c_str()) != 0)
		{
            GF_LOG_TRACE_ERR("LuaStateManager::ExecuteResourceFile()", std::string("Failed to execute ") + tr.GetName());
			return (false);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool LuaStateManager::ExecuteFile(LuaPlus::LuaStateOwner &luaState, char const * const pFileName)
	{
		const I32 retVal = luaState->DoFile(pFileName);
		const bool bSucceeded = (0 == retVal);
		assert(bSucceeded && "Unable to execute Lua script file!");
#if DEBUG
        if(!bSucceeded)
            GF_LOG_TRACE_ERR("LuaStateManager::ExecuteFile()", "Failed to execute: " + std::string(pFileName));
#endif
		return (bSucceeded);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool LuaStateManager::ExecuteString(char const * const pStringToExecute)
	{
		const I32 retVal = m_GlobalState->DoString(pStringToExecute);
		const bool bSucceeded = (0 == retVal);
		assert(bSucceeded && "Unable to execute Lua script buffer!");
#if DEBUG
        if(!bSucceeded)
            GF_LOG_TRACE_ERR("LuaStateManager::ExecuteString()", "Failed to execute: " + std::string(pStringToExecute));
#endif
		return (bSucceeded);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	LuaPlus::LuaObject LuaStateManager::GetGlobalActorTable(void)
	{
		return m_GlobalState->GetGlobal("ActorList");
	}

#ifdef DEBUG
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void LuaStateManager::IdentifyLuaObjectType(LuaPlus::LuaObject &objToTest)
	{
		assert(!objToTest.IsNil() && "Nil!");
		assert(!objToTest.IsBoolean() && "Boolean!");
		assert(!objToTest.IsCFunction() && "C-Function!");
		assert(!objToTest.IsFunction() && "Function!");
		assert(!objToTest.IsInteger() && "Integer!");
		assert(!objToTest.IsLightUserData() && "Light User Data!");
		assert(!objToTest.IsNone() && "None!");
		assert(!objToTest.IsNumber() && "Number!");
		assert(!objToTest.IsString() && "String!");
		assert(!objToTest.IsTable() && "Table!");
		assert(!objToTest.IsUserData() && "User Data!");
		assert(!objToTest.IsWString() && "Wide String!");
		assert(0 && "UNKNOWN!");
	}
#endif

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void LuaStateManager::PrintDebugMessage(LuaPlus::LuaObject debugObject)
	{
		LuaPlus::LuaObject stringObj;
		const char * pFinalStr = debugObject.ToString();
		
		// Generate an event.
		const EvtData_Debug_String debugEvent((NULL == pFinalStr) ? "INVALID!" : pFinalStr, EvtData_Debug_String::kDST_ScriptMsg);
		safeTriggerEvent(debugEvent);
	}

	// /////////////////////////////////////////////////////////////////
	// ******************** MISC helper functions **********************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SetPoint3FromLua(const LuaPlus::LuaObject &posData, Point3 &position)
	{
		if(!posData.IsTable())
		{
			return (false);
		}

		LuaPlus::LuaObject xData = posData["x"];
		LuaPlus::LuaObject yData = posData["y"];
		LuaPlus::LuaObject zData = posData["z"];

		if(!xData.IsNumber() || !yData.IsNumber() || !zData.IsNumber())
		{
			return (false);
		}

		F32 x = static_cast<F32>(xData.GetNumber());
		F32 y = static_cast<F32>(yData.GetNumber());
		F32 z = static_cast<F32>(zData.GetNumber());
		position.Set(x, y, z);

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SetVector3FromLua(const LuaPlus::LuaObject &dirData, Vector3 &direction)
	{
		if(!dirData.IsTable())
		{
			return (false);
		}

		LuaPlus::LuaObject xData = dirData["x"];
		LuaPlus::LuaObject yData = dirData["y"];
		LuaPlus::LuaObject zData = dirData["z"];

		if(!xData.IsNumber() || !yData.IsNumber() || !zData.IsNumber())
		{
			return (false);
		}

		F32 x = static_cast<F32>(xData.GetNumber());
		F32 y = static_cast<F32>(yData.GetNumber());
		F32 z = static_cast<F32>(zData.GetNumber());
		direction.Set(x, y, z);

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SetVector4FromLua(const LuaPlus::LuaObject &dirData, Vector4 &direction)
	{
		if(!dirData.IsTable())
		{
			return (false);
		}

		LuaPlus::LuaObject xData = dirData["x"];
		LuaPlus::LuaObject yData = dirData["y"];
		LuaPlus::LuaObject zData = dirData["z"];
		LuaPlus::LuaObject wData = dirData["w"];

		if(!xData.IsNumber() || !yData.IsNumber() || !zData.IsNumber() || !wData.IsNumber())
		{
			return (false);
		}

		F32 x = static_cast<F32>(xData.GetNumber());
		F32 y = static_cast<F32>(yData.GetNumber());
		F32 z = static_cast<F32>(zData.GetNumber());
		F32 w = static_cast<F32>(wData.GetNumber());
		direction.Set(x, y, z, w);

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SetColorFromLua(const LuaPlus::LuaObject &colorData, Vector4 &color)
	{
		if(!colorData.IsTable())
		{
			return (false);
		}

		LuaPlus::LuaObject rData = colorData["r"];
		LuaPlus::LuaObject gData = colorData["g"];
		LuaPlus::LuaObject bData = colorData["b"];
		LuaPlus::LuaObject aData = colorData["a"];

		if(!rData.IsNumber() || !gData.IsNumber() || !bData.IsNumber() || !aData.IsNumber())
		{
			return (false);
		}

		F32 r = static_cast<F32>(rData.GetNumber());
		F32 g = static_cast<F32>(gData.GetNumber());
		F32 b = static_cast<F32>(bData.GetNumber());
		F32 a = static_cast<F32>(aData.GetNumber());
		Clamp<F32>(r, 0.0f, 1.0f);
		Clamp<F32>(g, 0.0f, 1.0f);
		Clamp<F32>(b, 0.0f, 1.0f);
		Clamp<F32>(a, 0.0f, 1.0f);
		color.Set(r, g, b, a);

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SetFloatFromLua(const LuaPlus::LuaObject &numberData, F32 &number)
	{
		if(!numberData.IsNumber())
		{
			return (false);
		}

		number = static_cast<F32>(numberData.GetNumber());
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SetIntFromLua(const LuaPlus::LuaObject &numberData, I32 &number)
	{
		if(!numberData.IsInteger())
		{
			return (false);
		}

		number = numberData.GetInteger();
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SetStringFromLua(const LuaPlus::LuaObject &stringData, std::string &str)
	{
		if(!stringData.IsString())
		{
			return (false);
		}

		str.assign(stringData.GetString());
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SetBoolFromLua(const LuaPlus::LuaObject &flagData, bool &flag)
	{
		if(!flagData.IsBoolean())
		{
			return (false);
		}

		flag = flagData.GetBoolean();
		return (true);
	}

}
