//========================================================================
// GameLogic.h -  Logic layer interfaces and class definitions.
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
// @file GameLogic.h
// @author Michael L. McShaffry (edited by PJ O Halloran)
// @date 09/07/2010
//
// File contains the header for the interfaces and classes for the
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

#ifndef __GAME_LOGIC_H
#define __GAME_LOGIC_H

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

// External Headers
#include <string>
#include <map>

#include <boost/shared_ptr.hpp>

// Project Headers
#include "IGameLogic.h"
#include "Actors.h"
#include "CProcess.h"
#include "GameOptions.h"
#include "GameLog.h"
#include "GameException.h"
#include "IGamePhysics.h"
#include "IGameView.h"
#include "CRandom.h"
#include "Matrix.h"
#include "ModelViewProjStackManager.h"

namespace GameHalloran
{

	// ActorMap - a std::map with pointers to Actors with a key ActorId.
	typedef std::map<ActorId, boost::shared_ptr<IActor> > ActorMap;

	// /////////////////////////////////////////////////////////////////
	// @class BaseGameLogic
	// @author Michael L. McShaffry (edited by PJ O Halloran).
	//
	// This is a class is the base game logic layer class.
	//
	// /////////////////////////////////////////////////////////////////
	class BaseGameLogic : public IGameLogic
	{
	private:

		ActorId m_LastActorId;									///< The ID of the last actor to be added.
		BaseGameState m_State;									///< The current state of the game.
		I32 m_ExpectedPlayers;									///< How many local human players.
		I32 m_ExpectedRemotePlayers;							///< Expected remote human players.
		I32 m_ExpectedAI;										///< How many AI players.
		GameViewList m_gameViews;								///< Views that are attached to our game.
		//boost::shared_ptr<PathingGraph> m_pPathingGraph;		///< The pathing graph.
		//EventListenerPtr m_pAiEventListener;					///< AI event listener
		bool m_bProxy;											///< Set if this is a proxy for a remote clients game logic.
		I32 m_remotePlayerId;									///< If we are a remote player - what is out socket number on the server
		// Refactor Chapter 19/20 work.
		bool m_RenderDiagnostics;								///< Are we rendering diagnostics?
		I32 m_physicsDiagnosticMode;							///< Diagnostic mode of the physics.
		BaseGameState m_prevState;								///< The previous game state.

		// /////////////////////////////////////////////////////////////////
		// Create the required physics layer object depending on a value
		// in the options file.
		//
		// @param stackManagerPtr Pointer to the MVP stack manager.
		//
		// /////////////////////////////////////////////////////////////////
		bool CreatePhysicsModule(boost::shared_ptr<ModelViewProjStackManager> stackManagerPtr);

		// /////////////////////////////////////////////////////////////////
		// Get the diagnostic options from the options file if we are using
		// the Bullet physics engine.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetBulletDiagnosticOptions();

	protected:

		boost::shared_ptr<CProcessManager> m_pProcessManager;	///< Process Manager in the logic layer.
		boost::shared_ptr<IGamePhysics> m_pPhysics;				///< Pointer to the physics object.
		ActorMap m_ActorList;									///< Container of game actors.
		CRandom m_random;										///< Random number generator.
		boost::shared_ptr<GameLog> m_loggerPtr;					///< Pointer to the global logging object.
		boost::shared_ptr<GameOptions> m_optionsPtr;			///< Pointer to the global options.

		// /////////////////////////////////////////////////////////////////
		// Check if this logic layer is acting as a proxy for a logic layer
		// located in a remote location.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VIsProxy()
		{
			return (m_bProxy);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the number of expected players.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetExpectedPlayers()
		{
			return (m_ExpectedPlayers);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the number of expected remote players.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetExpectedRemotePlayers()
		{
			return (m_ExpectedRemotePlayers);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the number of expected AI players.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetExpectedAiPlayers()
		{
			return (m_ExpectedAI);
		};

		// /////////////////////////////////////////////////////////////////
		// Should we render diagnostics information to the screen?
		//
		// /////////////////////////////////////////////////////////////////
		bool IsRenderingDiagnostics() const
		{
			return (m_RenderDiagnostics);
		};

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param optionObj The global game options pointer.
		// @param loggerPtr The global logging object.
		// @param stackManagerPtr Pointer to the MVP stack manager.
		//
		// @throw GameException& If the options pointer is NULL.
		// @throw GameException& If we fail to allocate memory for the internal
		//							process manager.
		//
		// /////////////////////////////////////////////////////////////////
		explicit BaseGameLogic(boost::shared_ptr<GameOptions> optionsPtr, boost::shared_ptr<GameLog> loggerPtr, boost::shared_ptr<ModelViewProjStackManager> stackManagerPtr) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~BaseGameLogic();

		// /////////////////////////////////////////////////////////////////
		// Get a reference/alias to the list of game views held by the logic
		// layer.
		//
		// Be careful about storing this reference as it only exists for the
		// lifetime of this object and using it outside this scope has
		// undefined results!  Do not store the list reference returned!
		// Just call it directly from this method!  That way you can always
		// be sure the list still exists.
		//
		// @return GameViewList& A refernce/alias to the list of game views.
		//
		// /////////////////////////////////////////////////////////////////
		GameViewList &GetGameViewList();

		// /////////////////////////////////////////////////////////////////
		// Toggle pausing the logic.
		//
		// /////////////////////////////////////////////////////////////////
		void TogglePause();

		// /////////////////////////////////////////////////////////////////
		// Toggle debug mode on/off for the game (debug info rendered to 
		// screen).
		//
		// This breaks the app/logic/view design structure but it just a way
		// of printing out debug information from the logic to screen.
		// Intended to be useful for the programmer.
		//
		// /////////////////////////////////////////////////////////////////
		void ToggleDebugMode();

		// /////////////////////////////////////////////////////////////////
		// Set the debug render mode on or off.
		//
		// /////////////////////////////////////////////////////////////////
		void SetDebugMode(const bool dm)
		{
			m_RenderDiagnostics = dm;
			if(m_pPhysics)
			{
				if(m_RenderDiagnostics)
				{
					m_pPhysics->VSetDiagnosticMode(m_physicsDiagnosticMode);
				}
				else
				{
					m_pPhysics->VSetDiagnosticMode(0);
				}
			}
		};

		// /////////////////////////////////////////////////////////////////
		// Get the physics diagnostic mode.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetDiagnosticsMode() const { return (m_physicsDiagnosticMode); };

		// /////////////////////////////////////////////////////////////////
		// Set the physics diagnostics mode.
		//
		// /////////////////////////////////////////////////////////////////
		void SetDiagnosticsMode(const I32 dm) { m_physicsDiagnosticMode = dm; };

		// /////////////////////////////////////////////////////////////////
		// Get an actor.
		//
		// @param id The ID of the Actor to get.
		//
		// @return boost::shared_ptr<IActor> A pointer to an Actor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual boost::shared_ptr<IActor> VGetActor(const ActorId id);

		// /////////////////////////////////////////////////////////////////
		// Get the next actor ID that we can assign to a new actor.
		//
		// /////////////////////////////////////////////////////////////////
		ActorId GetNewActorID(void);
		
		// /////////////////////////////////////////////////////////////////
		// Get a random actor ID.
		//
		// /////////////////////////////////////////////////////////////////
		ActorId GetRandomActor(boost::optional<ActorId> ignoreMe);
		
		//// /////////////////////////////////////////////////////////////////
		//// Get the AI pathing graph.
		////
		//// /////////////////////////////////////////////////////////////////
		//boost::shared_ptr<PathingGraph> GetPathingGraph(void) { return m_pPathingGraph; }

		// /////////////////////////////////////////////////////////////////
		// Build the initial scene.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildInitialScene();

		// /////////////////////////////////////////////////////////////////
		// Add a game view.
		//
		// @param pView Pointer to a game view to add.
		// @param actorId The ID of the actor attached to the game view.
		//
		// @return The ID of the view we just added.  It will not have a value
		//			in the case the view was not added.
		//
		// /////////////////////////////////////////////////////////////////
		virtual boost::optional<GameViewId> VAddView(boost::shared_ptr<IGameView> pView, boost::optional<ActorId> actorId=boost::optional<ActorId>());

		// /////////////////////////////////////////////////////////////////
		// Remove a game view.
		//
		// @param pView Pointer to the game view to remove.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRemoveView(boost::shared_ptr<IGameView> pView);

		// /////////////////////////////////////////////////////////////////
		// Remove a game view.
		//
		// @param viewId The ID of the game view to remove.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRemoveView(const GameViewId viewId);

		// /////////////////////////////////////////////////////////////////
		// Add an actor.
		//
		// @param actor Pointer to the actor to add.
		// @param p Pointer to the parameters belonging to the Actor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddActor(boost::shared_ptr<IActor> actor, IActorParams *p);

		// /////////////////////////////////////////////////////////////////
		// Remove an actor.
		//
		// @param id The ID of the actor to remove.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRemoveActor(const ActorId id);

		// /////////////////////////////////////////////////////////////////
		// Move an actor.
		//
		// @param id The ID of the actor to move.
		// @param mat The movement matrix.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VMoveActor(const ActorId id, const Matrix4 &mat);

		// /////////////////////////////////////////////////////////////////
		// Load a game state.
		//
		// @param gameNameRef The name of the game.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VLoadGame(const std::string &gameNameRef);

		// /////////////////////////////////////////////////////////////////
		// Save the game state.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VSaveGame();

		// /////////////////////////////////////////////////////////////////
		// Set this logic object to be a proxy logic layer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetProxy();

		// /////////////////////////////////////////////////////////////////
		// Called once per main loop to update the logic state of the game.
		//
		// @param time The total application time so far.
		// @param elapsedTime The time elapsed since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnUpdate(const F64 time, const F32 elapsedTime);

		// /////////////////////////////////////////////////////////////////
		// Changing Game Logic State.
		//
		// @param newState The new game state.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VChangeState(const enum BaseGameState newState);

		// /////////////////////////////////////////////////////////////////
		// Render Diagnostics.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRenderDiagnostics();

		// /////////////////////////////////////////////////////////////////
		// Get a pointer to the physics object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual boost::shared_ptr<IGamePhysics> VGetGamePhysics(void);
		
		// /////////////////////////////////////////////////////////////////
		// Attach a process to the logic layers' process manager.
		//
		// @param pProcess A pointer to the process to attach to the logic
		//					layers' process manager.
		//
		// /////////////////////////////////////////////////////////////////
		void AttachProcess(boost::shared_ptr<CProcess> pProcess);

		// /////////////////////////////////////////////////////////////////
		// Get the current game state.
		//
		// /////////////////////////////////////////////////////////////////
		BaseGameState GetCurrentState()
		{
			return (m_State);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the previous game state.
		//
		// /////////////////////////////////////////////////////////////////
		BaseGameState GetPreviousState()
		{
			return (m_prevState);
		};

	};

	// /////////////////////////////////////////////////////////////////
	// @class BaseLuaGameLogic
	// @author Mike McShaffry, PJ O Halloran
	//
	// This class extends the functionality provided for by the 
	// BaseGameLogic class and opens it up to cooperate with external LUA
	// scripts on addition or removal of actors from the logic.
	//
	// For example, when an actor is added to the logic layer, we call
	// the create script function registered to run when an actor of that
	// type is added.  A similar process occurs when we remove an actor.
	//
	// /////////////////////////////////////////////////////////////////
	class BaseLuaGameLogic : public BaseGameLogic
	{
	private:

	protected:

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param optionObj The global game options pointer.
		// @param loggerPtr The global logging object.
		//
		// @throw GameException& If the options pointer is NULL.
		// @throw GameException& If we fail to allocate memory for the internal
		//							process manager.
		//
		// /////////////////////////////////////////////////////////////////
		explicit BaseLuaGameLogic(boost::shared_ptr<GameOptions> optionsPtr, boost::shared_ptr<GameLog> loggerPtr, boost::shared_ptr<ModelViewProjStackManager> stackManagerPtr) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~BaseLuaGameLogic();

		// /////////////////////////////////////////////////////////////////
		// Add an actor.  Also calls the LUA create function registered for
		// actors of this type.
		//
		// Please note that this class does not broadcast any event to
		// signify the addition of an actor.
		//
		// @param actor Pointer to the actor to add.
		// @param p Pointer to the parameters belonging to the Actor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddActor(boost::shared_ptr<IActor> actor, IActorParams *p);

		// /////////////////////////////////////////////////////////////////
		// Remove an actor.  Also calls the LUA destroy function registered
		// for actors of this type.
		//
		// Please note that this class does not broadcast any event to
		// signify the removal of an actor.
		//
		// @param id The ID of the actor to remove.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRemoveActor(const ActorId id);

	};

}

#ifdef WIN32
#	pragma warning( pop )
#endif

#endif
