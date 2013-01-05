#pragma once
#ifndef __LUA_STATE_MANAGER_H
#define __LUA_STATE_MANAGER_H

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
// @file LuaStateManager.h
// @author Mike McShaffry et al.
// @date 13/07/2010
//
// File contains the header for the LuaStateManager class.
//
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry et al., author(s) of Game
// Coding Complete and games programmer(s).
// 
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// I have added/modified the following functionality:
// - I added the class under the GameHalloran namespace.
// - 
//
// /////////////////////////////////////////////////////////////////

#include <string>

#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

#include "Vector.h"
#include "ResCache2.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class LuaStateManager
	// @author Mike McShaffry et al.
	//
	// Provides an interface to pass data to and from the program and
	// external LUA scripts (via the LuaPlus library).
	//
	// /////////////////////////////////////////////////////////////////
	class LuaStateManager
	{
	private:

		LuaPlus::LuaStateOwner m_GlobalState;					///< Our global LuaState.
		LuaPlus::LuaObject m_MetaTable;							///< Our portal to the outside world.

		// /////////////////////////////////////////////////////////////////
		// Debug print string function (callable from script).
		//
		// /////////////////////////////////////////////////////////////////
		void PrintDebugMessage(LuaPlus::LuaObject debugObject);

		// /////////////////////////////////////////////////////////////////
		// Called on destruction, performs the reverse of init(), namely
		// saving the lua player options to the global xml options file so that
		// they persist.
		//
		// /////////////////////////////////////////////////////////////////
		void Shutdown();

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		LuaStateManager(void);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		~LuaStateManager();

		// /////////////////////////////////////////////////////////////////
		// I hate two-stage initialization, but due to dependencies, 
		// we have to have an Init called after the Event Manager gets built.
		// This function runs the init script.
		//
		// @param pInitFileName The name of the initialize Lua game script.
		//
		// /////////////////////////////////////////////////////////////////
		bool Init(char const * const pInitFileName);

		// /////////////////////////////////////////////////////////////////
		// Returns the main state used by the entire game.  
		// Other implementations may use multiple LuaStates, but for our purposes
		// a single state will do just fine.
		//
		// /////////////////////////////////////////////////////////////////
		LuaPlus::LuaStateOwner &GetGlobalState(void)
		{
			return (m_GlobalState);
		};

		// /////////////////////////////////////////////////////////////////
		// Executes a Lua script file.
		//
		// @param pFileName The filename of the LUA script.
		//
		// @return bool True on success or false on error.
		//
		// /////////////////////////////////////////////////////////////////
		bool DoFile(char const * const pFileName);

		// /////////////////////////////////////////////////////////////////
		// Executes a Lua script file from the default resource cache file.
		//
		// @param scriptName The filename of the LUA script.
		//
		// @return bool True on success or false on error.
		//
		// /////////////////////////////////////////////////////////////////
		bool DoResourceFile(const char * const scriptName);

		// /////////////////////////////////////////////////////////////////
		// Executes a lua script file located in a resource cache file.
		//
		// @param luaState The LuaState/context/VM we wish to run the script
		//					upon.
		// @param scriptName The name of the script in the resource file.
		// @param rcManagerPtr Pointer to the manager of the resource cache file.
		//
		// @return bool True on success or false on error.
		//
		// /////////////////////////////////////////////////////////////////
		static bool ExecuteResourceFile(LuaPlus::LuaStateOwner &luaState, const char * const scriptName, ResCache *rcManagerPtr);

		// /////////////////////////////////////////////////////////////////
		// Execute a LUA script file.
		//
		// @param luaState The LuaState/context/VM we wish to run the script
		//					upon.
		// @param pFileName The filename of the LUA script to run.
		//
		// /////////////////////////////////////////////////////////////////
		static bool ExecuteFile(LuaPlus::LuaStateOwner &luaState, char const * const pFileName);

		// /////////////////////////////////////////////////////////////////
		// Executes an arbitrary Lua command.
		//
		// @param pStringToExecute The LUA statement to execute.
		//
		// /////////////////////////////////////////////////////////////////
		bool ExecuteString(char const * const pStringToExecute);

#ifdef DEBUG
		// /////////////////////////////////////////////////////////////////
		// Debug function for determining an object's type.
		//
		// @param objToTest The LUA object we will test.
		//
		// /////////////////////////////////////////////////////////////////
		static void IdentifyLuaObjectType(LuaPlus::LuaObject &objToTest);
#endif

		// /////////////////////////////////////////////////////////////////
		// The table where all actor context and data is stored
		// for script accessability.
		//
		// @return LuaObject A copy of the LUA object.
		//
		// /////////////////////////////////////////////////////////////////
		LuaPlus::LuaObject GetGlobalActorTable(void);
	};

	// /////////////////////////////////////////////////////////////////
	// Set a Point3 object from a LuaPlus object.
	//
	// The object must have the format: "TableName = { x = , y =, z = }"
	//
	// @param posData Lua data.
	// @param position Object to set.
	//
	// @return bool True on success or false on failure.
	//
	// /////////////////////////////////////////////////////////////////
	bool SetPoint3FromLua(const LuaPlus::LuaObject &posData, Point3 &position);

	// /////////////////////////////////////////////////////////////////
	// Set a Vector3 object from a LuaPlus object.
	//
	// The object must have the format: "TableName = { x = , y =, z = }"
	//
	// @param dirData Lua data.
	// @param direction Object to set.
	//
	// @return bool True on success or false on failure.
	//
	// /////////////////////////////////////////////////////////////////
	bool SetVector3FromLua(const LuaPlus::LuaObject &dirData, Vector3 &direction);

	// /////////////////////////////////////////////////////////////////
	// Set a Vector4 object from a LuaPlus object.
	//
	// The object must have the format: "TableName = { x = , y =, z =, w= }"
	//
	// @param dirData Lua data.
	// @param direction Object to set.
	//
	// @return bool True on success or false on failure.
	//
	// /////////////////////////////////////////////////////////////////
	bool SetVector4FromLua(const LuaPlus::LuaObject &dirData, Vector4 &direction);

	// /////////////////////////////////////////////////////////////////
	// Set a GameColor object from a LuaPlus object.
	//
	// The object must have the format: "TableName = { r = , g =, b =, a = }"
	//
	// @param colorData Lua data.
	// @param color Object to set.
	//
	// @return bool True on success or false on failure.
	//
	// /////////////////////////////////////////////////////////////////
	bool SetColorFromLua(const LuaPlus::LuaObject &colorData, Vector4 &color);

	// /////////////////////////////////////////////////////////////////
	// Set a F32 from a LuaPlus object.
	//
	// The object must have the format: "Name = number"
	//
	// @param numberData Lua data.
	// @param number Object to set.
	//
	// @return bool True on success or false on failure.
	//
	// /////////////////////////////////////////////////////////////////
	bool SetFloatFromLua(const LuaPlus::LuaObject &numberData, F32 &number);

	// /////////////////////////////////////////////////////////////////
	// Set an int from a LuaPlus object.
	//
	// The object must have the format: "Name = number"
	//
	// @param numberData Lua data.
	// @param number Object to set.
	//
	// @return bool True on success or false on failure.
	//
	// /////////////////////////////////////////////////////////////////
	bool SetIntFromLua(const LuaPlus::LuaObject &numberData, I32 &number);

	// /////////////////////////////////////////////////////////////////
	// Set a string from a LuaPlus object.
	//
	// The object must have the format: "Name = string"
	//
	// @param stringData Lua data.
	// @param str Object to set.
	//
	// @return bool True on success or false on failure.
	//
	// /////////////////////////////////////////////////////////////////
	bool SetStringFromLua(const LuaPlus::LuaObject &stringData, std::string &str);

	// /////////////////////////////////////////////////////////////////
	// Set a bool from a LuaPlus object.
	//
	// The object must have the format: "Name = true|false"
	//
	// @param flagData Lua data.
	// @param flag Object to set.
	//
	// @return bool True on success or false on failure.
	//
	// /////////////////////////////////////////////////////////////////
	bool SetBoolFromLua(const LuaPlus::LuaObject &flagData, bool &flag);

}

#endif	// __LUA_STATE_MANAGER_H
