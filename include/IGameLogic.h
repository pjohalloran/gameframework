//========================================================================
// IGameLogic.h -  Logic layer interface.
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
// @file IGameLogic.h
// @author Michael L. McShaffry (edited by PJ O Halloran)
// @date 22/07/2010
//
// File contains the header for the interface for the
// logic layer of the game.
// 
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry, author of Game
// Coding Complete and games programmer.
// 
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// I have added/modified the following functionality:
// - I added a VSaveGame() method to IGameLogic as recomended in 
//		GCC3.
//
// /////////////////////////////////////////////////////////////////

#ifndef __I_GAME_LOGIC_H
#define __I_GAME_LOGIC_H

// External Headers
#include <string>
#include <map>

#include <boost/shared_ptr.hpp>

// Project Headers
#include "IActors.h"
#include "IGamePhysics.h"
#include "Matrix.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @enum BaseGameState
	//
	// Base game state.
	//  - BGS_Initializing:
	//		Initializing the game and checking system resources.
	//  - BGS_LoadingGameEnvironment:
	//		Loading the game.
	//  - BGS_MainMenu:
	//		In the main menu state.
	//  - BGS_WaitingForPlayers:
	//		Waiting for players to join.
	//  - BGS_SpawnAI:
	//		Waiting for the AI to spawn.
	//  - BGS_Running:
	//		Game running.
	//  - BGS_Paused:
	//		The game is currently paused (due to the user pausing).
	//  - BGS_DialogPaused:
	//		The game is currently paused (due to a popup dialog).
	//
	// /////////////////////////////////////////////////////////////////
	enum BaseGameState
	{
		BGS_Initializing,
		BGS_LoadingGameEnvironment,
		BGS_MainMenu,
		BGS_WaitingForPlayers,
		BGS_SpawnAI,
		BGS_Running,
		BGS_Paused,
		BGS_DialogPaused
	};

	// /////////////////////////////////////////////////////////////////
	// @class IGameLogic
	// @author Michael L. McShaffry (edited by PJ O Halloran).
	//
	// This is a class is the base interface that all logic layer classes
	// must implement.
	//
	// /////////////////////////////////////////////////////////////////
	class IGameLogic
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~IGameLogic() { };

		// /////////////////////////////////////////////////////////////////
		// Get the game actor with the id supplied.
		//
		// @param id The ID of the actor to lookup.
		//
		// /////////////////////////////////////////////////////////////////
		virtual boost::shared_ptr<IActor> VGetActor(const ActorId id) = 0;

		// /////////////////////////////////////////////////////////////////
		// Add an actor.
		//
		// @param actor Pointer to the actor.
		// @param p The Actor parameters.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddActor(boost::shared_ptr<IActor> actor, IActorParams *p) = 0;

		// /////////////////////////////////////////////////////////////////
		// Remove an actor.
		//
		// @param id The ID of the actor to remove.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRemoveActor(const ActorId id) = 0;

		// /////////////////////////////////////////////////////////////////
		// Move the actor in the scene (Updates the position of the actor
		// in the world).
		//
		// @param id The ID of the actor to move.
		// @param mat The movement matrix.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VMoveActor(const ActorId id, const Matrix4 &mat) = 0;

		// /////////////////////////////////////////////////////////////////
		// Load a games state.
		//
		// @param gameName The name of the game.
		//
		// @return bool True on success, false on error.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VLoadGame(const std::string &gameNameRef) = 0;

		// /////////////////////////////////////////////////////////////////
		// Save a games state.
		//
		// Added by PJ O Halloran 09/07/2010...
		//
		// @return bool True on success, false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VSaveGame() = 0;

		// /////////////////////////////////////////////////////////////////
		// Set this Logic layer as a proxy for a remote client.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetProxy() = 0;	

		// /////////////////////////////////////////////////////////////////
		// Update the stte of the game.  Called once per game loop so the logic
		// layer may update its state.
		//
		// @param time The total time so far.
		// @param elapsedTime The time elapsed since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnUpdate(const F64 time, const F32 elapsedTime) = 0;

		// /////////////////////////////////////////////////////////////////
		// Change the state of the game.
		//
		// @param newState The new state of the game.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VChangeState(const enum BaseGameState newState) = 0;

		// /////////////////////////////////////////////////////////////////
		// Get a pointer to the physics interface.
		//
		// @return boost::shared_ptr<IGamePhysics> A pointer to the physics
		//											interface.
		//
		// /////////////////////////////////////////////////////////////////
		virtual boost::shared_ptr<IGamePhysics> VGetGamePhysics(void) = 0;

		// /////////////////////////////////////////////////////////////////
		// Build up the initial scene.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildInitialScene() = 0;
	};
}

#endif
