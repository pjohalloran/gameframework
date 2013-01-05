#pragma once
#ifndef __I_GAME_VIEW_H
#define __I_GAME_VIEW_H

//========================================================================
// IGameView.h -  Game view layer class interface.
//
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
// @file IGameView.h
// @author Michael L. McShaffry (edited by PJ O Halloran)
// @date 09/07/2010
//
// File contains the header for the game view layer interface.
// 
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry, author of Game
// Coding Complete and games programmer.
// 
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// I have added/modified the following functionality:
// - I commented out virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) = 0;
//		as its WIN32 specific and replaced it with an SDL equivalent.
//
// /////////////////////////////////////////////////////////////////

#include <list>

#include <boost/optional.hpp>

#include "Actors.h"
#include "OsInputEvents.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @enum GameViewType
	//
	// Enum of game view types.
	//
	// /////////////////////////////////////////////////////////////////
	enum GameViewType
	{
		GameView_Human,
		GameView_Remote,
		GameView_AI,
		GameView_Recorder,
		GameView_Other
	};

	typedef U32 GameViewId;

	// ID for a dummy view.
	static const GameViewId VIEW_ID_NO_VIEW_ATTACHED = 0;

	// /////////////////////////////////////////////////////////////////
	// @class IGameView
	// @author Michael L. McShaffry (edited by PJ O Halloran).
	//
	// This is a class is the base interface that all game view layer
	// classes must implement.
	//
	// /////////////////////////////////////////////////////////////////
	class IGameView 
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~IGameView() { };
		
		// /////////////////////////////////////////////////////////////////
		// Called when the view is restored after an ALT-TAB or other event.
		//
		// @return bool 
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnRestore() = 0;
		
		// /////////////////////////////////////////////////////////////////
		// Render the game view.
		//
		// @param time The total running time so far.
		// @param elapsedTime The time elapsed since the last render.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnRender(const F64 time, const F32 elapsedTime) = 0;
		
		// /////////////////////////////////////////////////////////////////
		// Called when the view lost after a screen resolution, ALT-TAB or
		// other similar event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnLostDevice() = 0;
		
		// /////////////////////////////////////////////////////////////////
		// Get the type of view.
		//
		// /////////////////////////////////////////////////////////////////
		virtual GameViewType VGetType() const = 0;
		
		// /////////////////////////////////////////////////////////////////
		// Get the ID of the view.
		//
		// /////////////////////////////////////////////////////////////////
		virtual GameViewId VGetId() const = 0;
		
		// /////////////////////////////////////////////////////////////////
		// Attach an Actor to the view.
		//
		// @param vid The ID of the view.
		// @param aid The ID of the actor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnAttach(const GameViewId vid, boost::optional<ActorId> aid) = 0;

		// /////////////////////////////////////////////////////////////////
		// The message queue event callback.
		//
		// @param eventObj The event structure.
		// @param elapsedTime The number of seconds since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnEvent(GF_Event &eventObj, const F32 elapsedTime) = 0;
		
		// /////////////////////////////////////////////////////////////////
		// Called once per the main loop to update the state of the view.
		//
		// @param elapsedTime The number of seconds since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnUpdate(const F32 elapsedTime) = 0;
	};

	typedef std::list<boost::shared_ptr<IGameView> > GameViewList;

}

#endif
