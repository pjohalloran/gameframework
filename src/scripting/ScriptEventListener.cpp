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
// @file ScriptEventListener.cpp
// @author Mike McShaffry et al.
// @date 13/07/2010
//
// File contains the header for the ScriptEventListener class.
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

// External Headers
#include <assert.h>

#include <boost/shared_ptr.hpp>

// Project Headers
#include "ScriptEventListener.h"

// For the global application pointer.
#include "GameMain.h"

using LuaPlus::LuaObject;
using LuaPlus::LuaFunction;
using LuaPlus::LuaState;

using boost::shared_ptr;

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ScriptEventListener::ScriptEventListener(LuaObject explicitHandlerFunction) : m_HandlerFunction(explicitHandlerFunction)
	{
		assert( explicitHandlerFunction.IsFunction() && "Script listener *MUST* be a valid function!" );
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ScriptEventListener::VHandleEvent(IEventData const &event)
	{
		// If we don't already have Lua event data built, do so now.
		if ( false == event.VHasLuaEventData() )
		{
			// This goes against everything you are taught in C++ class.
			// We're going to make this const IEventData non-const because
			// we need to serialize the event for Lua to understand it.
			// We're doing this for three reasons:
			//  a) So we only build Lua data ONCE for any triggered event,
			//     and ONLY when the event needs to be sent to a Lua function
			//     (we don't want to ALWAYS build Lua data).
			//  b) We're not technically changing any of the "real" event
			//     *data*...
			//  c) If we make other listener's HandleEvent() calls take
			//     a non-const event, they could alter it.
			// Don't make a habit of doing this.
			// Note:  We could potentially avoid this by marking the Lua
			// object with the 'mutable' keyword.

			// Pray the const away.
			IEventData & NCEventData = const_cast< IEventData & >( event );

			// Build it and never mention this again.
			NCEventData.VBuildLuaEventData();
		}

		LuaObject eventDataObj = event.VGetLuaEventData();

		//Call the handler function.
		const bool bResult = VCallLuaFunction( eventDataObj );

		return bResult;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ScriptEventListener::VCallLuaFunction(LuaObject &eventData)
	{
		LuaFunction<bool> function( m_HandlerFunction );
		return function( eventData );
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ScriptActorEventListener::ScriptActorEventListener(LuaObject explicitHandlerFunction, const ActorId actorID)
		: ScriptEventListener(explicitHandlerFunction), m_SrcActorID(actorID)
	{ 
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ScriptActorEventListener::VCallLuaFunction(LuaObject &eventData)
	{
		// Find our actor to pass in the actor script data context.
		if(g_appPtr)
		{
			// This is more sanity checking than anything, to ensure that the actor
			// still exists.
			shared_ptr< IActor > gameActor = g_appPtr->GetLogicPtr()->VGetActor(m_SrcActorID);
			if (!gameActor)
			{
				assert( 0 && "Attempted to call a script listener for an actor that couldn't be found!  Did you delete the actor without removing all listeners?" );
				return (false);
			}

			// Get ahold of the actor's script data.
			// unused LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			LuaObject globalActorTable = g_appPtr->GetLuaStateManager()->GetGlobalActorTable();
			assert( globalActorTable.IsTable() && "Global actor table is NOT a table!" );
			LuaObject actorData = globalActorTable[ m_SrcActorID ];

			// We pass in the event data IN ADDITION TO the actor's script data.
			LuaFunction<bool> function( m_HandlerFunction );
			return (function( eventData, actorData ));
		}
		return (false);
	}

}
