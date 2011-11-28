//========================================================================
// EventManager.cpp : implements a multi-listener multi-sender event system
//
// Part of the GameCode3 Application
//
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
// The source code is managed and maintained through Google Code: http://gamecode3.googlecode.com/svn/trunk/
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
// @file EventManager.cpp
// @author Mike McShaffry
// @date 13/07/2010
//
// File contains the header for the EventManager class and all its
// other related classes.
//
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry, author of Game
// Coding Complete and games programmer.
// 
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// Please see the related header file for details on any changes I
// have made.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <cstring>
#include <assert.h>

// Project Headers
#include "EventManager.h"

// For global app pointer.
#include "GameMain.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void EmptyEventData::VBuildLuaEventData(void)
	{
		assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

		//Get the global state.
		LuaPlus::LuaState *pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();

		//We just set a nil object.
		m_LuaEventData.AssignNil( pState );

		//Now we've "got data".
		m_bHasLuaEventData = true;
	}

	// 
	static IEventManager * g_pEventMgr = NULL;

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	IEventManager * IEventManager::Get()
	{
		return g_pEventMgr;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	IEventManager::IEventManager(char const * const pName, bool setAsGlobal) throw (GameException &)
	{
		if ( setAsGlobal )
			g_pEventMgr = this;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	IEventManager::~IEventManager()
	{
		if ( g_pEventMgr == this )
			g_pEventMgr = NULL;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool safeAddListener( EventListenerPtr const & inHandler, EventType const & inType )
	{
		assert(IEventManager::Get() && "No event manager!");
		return IEventManager::Get()->VAddListener( inHandler, inType );
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool safeDelListener( EventListenerPtr const & inHandler, EventType const & inType )
	{
		assert(IEventManager::Get() && "No event manager!");
		return IEventManager::Get()->VDelListener( inHandler, inType );
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool safeTriggerEvent( IEventData const & inEvent )
	{
		assert(IEventManager::Get() && "No event manager!");
		return IEventManager::Get()->VTrigger( inEvent );
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool safeQueEvent( IEventDataPtr const & inEvent )
	{
		assert(IEventManager::Get() && "No event manager!");
		return IEventManager::Get()->VQueueEvent( inEvent );
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool threadSafeQueEvent ( IEventDataPtr const & inEvent )
	{
		assert(IEventManager::Get() && "No event manager!");
		return IEventManager::Get()->VThreadSafeQueueEvent( inEvent );
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool safeAbortEvent( EventType const & inType,
						 bool allOfType /*= false*/ )
	{
		assert(IEventManager::Get() && "No event manager!");
		return IEventManager::Get()->VAbortEvent( inType, allOfType );
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool safeTickEventManager( U64 maxMillis /*= kINFINITE*/ )
	{
		assert(IEventManager::Get() && "No event manager!");
		return IEventManager::Get()->VTick( maxMillis );
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool safeValidateEventType( EventType const & inType )
	{
		assert(IEventManager::Get() && "No event manager!");
		return IEventManager::Get()->VValidateType( inType );
	}

}
