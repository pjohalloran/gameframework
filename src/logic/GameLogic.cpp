//========================================================================
// GameLogic.cpp -  Logic layer interfaces and class definitions.
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
// @file GameLogic.cpp
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
// Please see the associated file for details on the changes i have
// made.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string.hpp>

#include <bullet/btBulletDynamicsCommon.h>

// Project Headers
#include "GameLogic.h"
#include "GameBase.h"
#include "Events.h"
#include "TextResource.h"

// For the global app pointer.
#include "GameMain.h"

#include "NullPhysics.h"
#include "BulletPhysics.h"
#include "CustomPhysics.h"
#include "HavokPhysics.h"

using boost::optional;
using boost::shared_ptr;
using boost::lexical_cast;
using boost::bad_lexical_cast;

using std::string;

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// ****************** BaseGameLogic ************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool BaseGameLogic::CreatePhysicsModule(shared_ptr<ModelViewProjStackManager> stackManagerPtr)
	{
		string physicsNameStr;						// Name of the physics system from the options file.
		const string NULL_SYSTEM("null");			// Name of the disabled physics system.
		const string CUSTOM_SYSTEM("custom");		// Name of the custom physics system.
		const string BULLET_SYSTEM("bullet");		// Name of the bullet physics system.
		const string HAVOK_SYSTEM("havok");			// Name of the havok physics system.

		if(!RetrieveAndConvertOption<string>(m_optionsPtr, string("PhysicsSystem"), GameOptions::PROGRAMMER, physicsNameStr))
		{
            GF_LOG_TRACE_ERR("BaseGameLogic::CreatePhysicsModule()", "Failed to find \"PhysicsSystem\" in the options file so we will use the default system (NULL)");
//			SafeGameLogAndPrefix(m_loggerPtr,\
									GameLog::ERR,\
									string("BaseGameLogic::CreatePhysicsModule()"),\
									string("Failed to find \"PhysicsSystem\" in the options file so we will use the default system (NULL)"));
			physicsNameStr.assign(NULL_SYSTEM);
		}
		boost::algorithm::to_lower(physicsNameStr);

		if(physicsNameStr.compare(NULL_SYSTEM.c_str()) == 0)
		{
			m_pPhysics.reset(GCC_NEW NullPhysics());
		}
		else if(physicsNameStr.compare(BULLET_SYSTEM.c_str()) == 0)
		{
			if(!stackManagerPtr)
			{
                GF_LOG_TRACE_ERR("BaseGameLogic::CreatePhysicsModule()", "Failed to initialize Bullet as it needs a valid MVP stack manager");
//				SafeGameLogAndPrefix(m_loggerPtr,\
										GameLog::ERR,\
										string("BaseGameLogic::CreatePhysicsModule()"),\
										string("Failed to initialize Bullet as it needs a valid MVP stack manager"));
				return (false);
			}
			m_pPhysics.reset(GCC_NEW BulletPhysics(stackManagerPtr));
			m_physicsDiagnosticMode = GetBulletDiagnosticOptions();
		}
		else if(physicsNameStr.compare(CUSTOM_SYSTEM.c_str()) == 0)
		{
			m_pPhysics.reset(GCC_NEW CustomPhysics());
		}
		else if(physicsNameStr.compare(HAVOK_SYSTEM.c_str()) == 0)
		{
			m_pPhysics.reset(GCC_NEW HavokPhysics());
		}
		else
		{
            GF_LOG_TRACE_ERR("BaseGameLogic::CreatePhysicsModule()", string("The physics system name is not known: ") + physicsNameStr);
//			SafeGameLogAndPrefix(m_loggerPtr,\
									GameLog::ERR,\
									string("BaseGameLogic::CreatePhysicsModule()"),\
									string("The physics system name is not known (") + physicsNameStr + string(")"));
			return (false);
		}

		if(!m_pPhysics || !m_pPhysics->VInitialize())
		{
            GF_LOG_TRACE_ERR("BaseGameLogic::CreatePhysicsModule()", "Failed to initialize the physics system");
//			SafeGameLogAndPrefix(m_loggerPtr,\
									GameLog::ERR,\
									string("BaseGameLogic::CreatePhysicsModule()"),\
									string("Failed to initialize the physics system"));
			return (false);
		}

		// Set the initial diagnostic mode (developer has to turn on the diagnostic mode from the menu/keyboard).
		m_pPhysics->VSetDiagnosticMode(0);

        GF_LOG_TRACE_INF("BaseGameLogic::CreatePhysicsModule()", string("Initialized the ") + physicsNameStr + string(" physics library"));
//		SafeGameLogAndPrefix(m_loggerPtr, GameLog::INF, string("BaseGameLogic::CreatePhysicsModule()"), string("Initialized the ") + physicsNameStr + string(" physics library"));
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	I32 BaseGameLogic::GetBulletDiagnosticOptions()
	{
		std::string diagnosticOptions;				// Diagnostic option from options file.
		
		if(!RetrieveAndConvertOption<string>(m_optionsPtr, string("BulletPhysicsDebugMode"), GameOptions::PROGRAMMER, diagnosticOptions))
		{
			// No diagnostics.
			return (0);
		}

		boost::algorithm::to_lower(diagnosticOptions);

		I32 diagnosticMode = 0;						// Diagnostic mode (0 = off).
		std::vector<std::string> tokensVec;			// Vector to store tokens inside.

		// Split up the tokens.
		boost::algorithm::split(tokensVec, diagnosticOptions, boost::algorithm::is_any_of(std::string(",")));

		if(tokensVec.empty())
		{
			// No diagnostics.
			return (0);
		}

		const std::string WF("wireframe");
		const std::string AABB("aabb");
		const std::string FT("featurestext");
		const std::string CP("contactpoints");
		const std::string ND("nodeactivation");
		const std::string NHT("nohelptext");
		const std::string DT("drawtext");
		const std::string PT("profiletimings");
		const std::string ESC("enablesatcmp");
		const std::string DLCP("disablelcp");
		const std::string ECCD("enableccd");
		const std::string DC("drawconstraints");
		const std::string DCL("drawconstraintslimits");
		const std::string FWF("fastwireframe");

		// Turn all requested debug options.
		for(std::vector<std::string>::iterator i = tokensVec.begin(), end = tokensVec.end(); i != end; ++i)
		{
			if((*i).compare(WF.c_str()) == 0)
			{
				diagnosticMode = diagnosticMode | btIDebugDraw::DBG_DrawWireframe;
			}
			else if((*i).compare(AABB.c_str()) == 0)
			{
				diagnosticMode = diagnosticMode | btIDebugDraw::DBG_DrawAabb;
			}
			else if((*i).compare(FT.c_str()) == 0)
			{
				diagnosticMode = diagnosticMode | btIDebugDraw::DBG_DrawFeaturesText;
			}
			else if((*i).compare(CP.c_str()) == 0)
			{
				diagnosticMode = diagnosticMode | btIDebugDraw::DBG_DrawContactPoints;
			}
			else if((*i).compare(ND.c_str()) == 0)
			{
				diagnosticMode = diagnosticMode | btIDebugDraw::DBG_NoDeactivation;
			}
			else if((*i).compare(NHT.c_str()) == 0)
			{
				diagnosticMode = diagnosticMode | btIDebugDraw::DBG_NoHelpText;
			}
			else if((*i).compare(DT.c_str()) == 0)
			{
				diagnosticMode = diagnosticMode | btIDebugDraw::DBG_DrawText;
			}
			else if((*i).compare(PT.c_str()) == 0)
			{
				diagnosticMode = diagnosticMode | btIDebugDraw::DBG_ProfileTimings;
			}
			else if((*i).compare(ESC.c_str()) == 0)
			{
				diagnosticMode = diagnosticMode | btIDebugDraw::DBG_EnableSatComparison;
			}
			else if((*i).compare(DLCP.c_str()) == 0)
			{
				diagnosticMode = diagnosticMode | btIDebugDraw::DBG_DisableBulletLCP;
			}
			else if((*i).compare(ECCD.c_str()) == 0)
			{
				diagnosticMode = diagnosticMode | btIDebugDraw::DBG_EnableCCD;
			}
			else if((*i).compare(DC.c_str()) == 0)
			{
				diagnosticMode = diagnosticMode | btIDebugDraw::DBG_DrawConstraints;
			}
			else if((*i).compare(DCL.c_str()) == 0)
			{
				diagnosticMode = diagnosticMode | btIDebugDraw::DBG_DrawConstraintLimits;
			}
			else if((*i).compare(FWF.c_str()) == 0)
			{
				diagnosticMode = diagnosticMode | btIDebugDraw::DBG_FastWireframe;
			}
			else
			{
				// unknown option...
                GF_LOG_TRACE_ERR("BaseGameLogic::GetBulletDiagnosticOptions()", string("Unknown option: ") + *i);
//				SafeGameLogAndPrefix(m_loggerPtr,\
										GameLog::ERR,\
										string("BaseGameLogic::GetBulletDiagnosticOptions()"),\
										string("Unknown option: ") + *i);
			}
		}

		return (diagnosticMode);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	BaseGameLogic::BaseGameLogic(shared_ptr<GameOptions> optionsPtr, shared_ptr<GameLog> loggerPtr, shared_ptr<ModelViewProjStackManager> stackManagerPtr)\
		throw (GameException &) : m_pProcessManager(), m_random(), m_ActorList(), m_LastActorId(0), m_State(BGS_Initializing),\
			m_ExpectedPlayers(0), m_ExpectedRemotePlayers(0), m_ExpectedAI(0), m_gameViews()\
				/*, m_pPathingGraph(), m_pAiEventListener()*/, m_bProxy(false), m_remotePlayerId(0),\
					m_RenderDiagnostics(false),	m_physicsDiagnosticMode(0), m_prevState(BGS_Initializing), m_pPhysics(), m_loggerPtr(loggerPtr), m_optionsPtr(optionsPtr)
	{
		if(!optionsPtr)
		{
			throw GameException(string("A Valid options object was not supplied to the logic layer constructor"));
		}

		m_pProcessManager.reset(GCC_NEW CProcessManager);
		if(!m_pProcessManager)
		{
			throw GameException(string("Failed to allocate memory for the logic layers process manager"));
		}

		m_random.Randomize();

		if(!CreatePhysicsModule(stackManagerPtr))
		{
			throw GameException(string("Failed to setup the physics module"));
		}

		// Create AI module
		//m_pPathingGraph.reset(CreatePathingGraph());
		//m_pAiEventListener = EventListenerPtr (GCC_NEW AiEventListener ( ));
		// Register AI event for the logic layers AI listener.
		//safeAddListener(m_pAiEventListener, EvtData_AiSteer::sk_EventType);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	BaseGameLogic::~BaseGameLogic()
	{
		try
		{
			// MrMike: 12-Apr-2009 
			//   Added this to explicity remove views from the game logic list.
			while (!m_gameViews.empty())
			{
				m_gameViews.pop_front();
			}

			// Print out an error to the log file if the programmer forgets to clear the Actor list in the derived classes.
			if(!m_ActorList.empty())
			{
                GF_LOG_TRACE_ERR("BaseGameLogic::~BaseGameLogic()", "You should destroy the actor list in the inherited class!");
//				SafeGameLogAndPrefix(m_loggerPtr, GameLog::ERR, string("BaseGameLogic::~BaseGameLogic()"), string("You should destroy the actor list in the inherited class!"));
			}
		}
		catch(...)
		{
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	GameViewList &BaseGameLogic::GetGameViewList()
	{
		return (m_gameViews);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseGameLogic::ToggleDebugMode()
	{
		m_RenderDiagnostics = !m_RenderDiagnostics;
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
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ActorId BaseGameLogic::GetNewActorID(void)
	{
		return (++m_LastActorId);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ActorId BaseGameLogic::GetRandomActor(optional<ActorId> ignoreMe)
	{
		// choose a random element
		U32 count = (U32)m_ActorList.size();
		U32 choice = m_random.Random(count);
		
		// since this is a map, we have to walk the tree to find the item in question
		ActorMap::iterator it = m_ActorList.begin();
		for (U32 i = 0; i < choice; i++)
		{
			++it;
		}
		
		// If we're ignoring an actor, choose the next one.  incidentally, this makes the next actor the most likely 
		// choice to be chosen, although the player won't notice in a game of a dozen or so actors.
		if (ignoreMe && it->first == *ignoreMe)
		{
			++it;
			if (it == m_ActorList.end())
			{
				it = m_ActorList.begin();
			}
		}
		
		// found someone
		return (it->first);
	}

	//// /////////////////////////////////////////////////////////////////
	////
	//// /////////////////////////////////////////////////////////////////
	//shared_ptr<PathingGraph> BaseGameLogic::GetPathingGraph(void)
	//{
	//	return m_pPathingGraph;
	//}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool BaseGameLogic::VLoadGame(const string &gameNameRef)
	{
		// TODO: Implement.
        GF_LOG_ERR("BaseGameLogic::VLoadGame() is not implemented yet!");
//		SafeGameLog(m_loggerPtr, GameLog::ERR, string("BaseGameLogic::VLoadGame() is not implemented yet!"));
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool BaseGameLogic::VSaveGame()
	{
		// TODO: Implement.
        GF_LOG_ERR("BaseGameLogic::VSaveGame() is not implemented yet!");
//		SafeGameLog(m_loggerPtr, GameLog::ERR, string("BaseGameLogic::VSaveGame() is not implemented yet!"));
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseGameLogic::VSetProxy()
	{
        GF_LOG_INF("The logic layer is acting as a proxy for a remote logic layer");
		SafeGameLog(m_loggerPtr, GameLog::INF, string("The logic layer is acting as a proxy for a remote logic layer."));
		m_bProxy = true;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	shared_ptr<IGamePhysics> BaseGameLogic::VGetGamePhysics(void)
	{
		return (m_pPhysics);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseGameLogic::AttachProcess(shared_ptr<CProcess> pProcess)
	{
		if (m_pProcessManager)
		{
			m_pProcessManager->Attach(pProcess);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseGameLogic::VAddActor(shared_ptr<IActor> actor, IActorParams *p)
	{
		if(!actor)
		{
            GF_LOG_ERR("No valid actor pointer supplied so we cannot add the actor to the logic layer");
//			SafeGameLog(m_loggerPtr, GameLog::ERR, string("No valid actor pointer supplied so we cannot add the actor to the logic layer!"));
			return;
		}
		if(!p)
		{
            GF_LOG_ERR("No parameters were supplied so we cannot add the actor to the logic layer");
//			SafeGameLog(m_loggerPtr, GameLog::ERR, string("No parameters were supplied so we cannot add the actor to the logic layer!"));
			return;
		}

		optional<ActorId> destActorID = p->VGetId();
		if(destActorID)
		{
			m_ActorList[*destActorID] = actor;
			actor->VSetID(*destActorID);
		}
		else
		{
            GF_LOG_ERR("Attempted to add an actor with no actor ID");
			SafeGameLog(m_loggerPtr, GameLog::ERR, string("Attempted to add an actor with no actor ID!"));
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseGameLogic::VRemoveActor(const ActorId aid)
	{
		if (m_bProxy)
			return;

		if(VGetActor(aid))
		{
			if(m_pPhysics)
			{
				m_pPhysics->VRemoveActor(aid);
			}

			m_ActorList.erase(aid);
			safeQueEvent(IEventDataPtr(GCC_NEW EvtData_Destroy_Actor(aid)));
		}
#if DEBUG
		else
		{
			try
			{
				string idStr = lexical_cast<string, ActorId>(aid);
                GF_LOG_ERR(string("Attempted to remove an actor (id=") + idStr + string(") that does not exist!"));
//				SafeGameLog(m_loggerPtr, GameLog::ERR, string("Attempted to remove an actor (id=") + idStr + string(") that does not exist!"));
			}
			catch(bad_lexical_cast &)
			{
                GF_LOG_ERR("Attempted to remove an actor that does not exist");
//				SafeGameLog(m_loggerPtr, GameLog::ERR, string("Attempted to remove an actor that does not exist!"));
			}
		}
#endif
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseGameLogic::VMoveActor(const ActorId id, const Matrix4 &mat)
	{
		shared_ptr<IActor> pActor = VGetActor(id);

		if (pActor)
		{
			pActor->VSetMat(mat);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	shared_ptr<IActor> BaseGameLogic::VGetActor(const ActorId id)
	{
		ActorMap::iterator i = m_ActorList.find(id);
		if (i==m_ActorList.end())
		{
			shared_ptr<IActor> null;
			return (null);
		}

		return ((*i).second);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseGameLogic::VOnUpdate(const F64 time, const F32 elapsedTime)
	{
		switch(m_State)
		{
			case BGS_Initializing:
			{
				// If we get to here we're ready to attach players
				VChangeState(BGS_MainMenu);
			}
			break;

			case BGS_LoadingGameEnvironment:
			{
				if (g_appPtr->VLoadGame())
				{	
					VChangeState(BGS_SpawnAI);
				}
				else
				{
                    GF_LOG_TRACE_FAT("BaseGameLogic::VOnUpdate()", "The game failed to load");
//					SafeGameLogAndPrefix(m_loggerPtr, GameLog::FAT, string("BaseGameLogic::VOnUpdate()"), string("The game failed to load."));
					g_appPtr->SetRunning(false);
				}
			}
			break;

			case BGS_MainMenu:
			case BGS_WaitingForPlayers:
				// do nothing here but derived versions might do something.
			break;

			case BGS_SpawnAI:
			if (this->m_ExpectedAI == 0)
			{
				// the base game logic doesn't spawn AI - your child class will do that.
				// in the case no AI are coming, the base game logic will go ahead and move to 
				// the next state.
				VChangeState(BGS_Running);
			}
			break;

			case BGS_Running:
			{
				m_pProcessManager->UpdateProcesses(elapsedTime);
			}
			break;

			case BGS_Paused:
			case BGS_DialogPaused:
				// do nothing here but derived versions might do something.
			break;

			default:
			{
                GF_LOG_ERR("Unrecognized game state");
			}
			break;
		}

		// Update all the views.
		for(GameViewList::iterator i=m_gameViews.begin(); i != m_gameViews.end(); ++i)
		{
			(*i)->VOnUpdate(elapsedTime);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseGameLogic::VChangeState(const enum BaseGameState newState)
	{
		// TODO: Implement networking here!!!
		// PJ - I have removed networking for now, might add it in later.

		if (newState == BGS_WaitingForPlayers)
		{
			//RetrieveAndConvertOption<I32>(m_optionsPtr, string("ExpectedPlayers"), GameOptions::PROGRAMMER, m_ExpectedPlayers);
			//RetrieveAndConvertOption<I32>(m_optionsPtr, string("ExpectedRemotePlayers"), GameOptions::PROGRAMMER, m_ExpectedRemotePlayers);
			//RetrieveAndConvertOption<I32>(m_optionsPtr, string("ExpectedAiPlayers"), GameOptions::PROGRAMMER, m_ExpectedAI);
			m_ExpectedPlayers = 2;
			m_ExpectedRemotePlayers = 0;
			m_ExpectedAI = 0;
		
			//if (!g_pApp->m_pOptions->m_gameHost.empty())
			//{
			//	VSetProxy();
			//	m_ExpectedAI = 0;			// the server will create these
			//	m_ExpectedRemotePlayers = 0;	// the server will create these
			//	ClientSocketManager *pClient = GCC_NEW ClientSocketManager(g_pApp->m_pOptions->m_gameHost, g_pApp->m_pOptions->m_listenPort);
			//	if (!pClient->Connect())
			//	{
			//		// Throw up a main menu
			//		VChangeState(BGS_MainMenu);
			//		return;
			//	}
			//	g_pApp->m_pBaseSocketManager = pClient;
			//}
			//else if (m_ExpectedRemotePlayers > 0)
			//{
			//	BaseSocketManager *pServer = GCC_NEW BaseSocketManager();
			//	if (!pServer->Init())
			//	{
			//		// Throw up a main menu
			//		VChangeState(BGS_MainMenu);	
			//		return;
			//	}
			//	pServer->AddSocket(GCC_NEW GameServerListenSocket(g_pApp->m_pOptions->m_listenPort));
			//	g_pApp->m_pBaseSocketManager = pServer;
			//}
		}

		// Set the new state and the previous state.
		m_prevState = m_State;
		m_State = newState;

		// Broadcast the game state change to all interested listeners.
		if (!m_bProxy)
		{
			safeQueEvent(IEventDataPtr(GCC_NEW EvtData_Game_State(m_State)));
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseGameLogic::VBuildInitialScene()
	{
		// Execute our startup script file that will handle creating and positing the game actors (via the logic and view layers with game events).
		TextResource luaStartupScriptRes(string("lua") + ZipFile::ZIP_PATH_SEPERATOR + std::string("startup.lua"));
		shared_ptr<TextResHandle> scriptHandle = boost::static_pointer_cast<TextResHandle>(g_appPtr->GetResourceCache()->GetHandle(&luaStartupScriptRes));
		if(!scriptHandle || !scriptHandle->VInitialize())
		{
			// failed to retrieve script.
            GF_LOG_TRACE_ERR("BaseGameLogic::VBuildInitialScene()", string("Failed to retrieve ") + luaStartupScriptRes.GetName());
//			SafeGameLogAndPrefix(m_loggerPtr, GameLog::ERR, string("BaseGameLogic::VBuildInitialScene()"), string("Failed to retrieve ") + luaStartupScriptRes.GetName());
			return;
		}

		if(!g_appPtr->GetLuaStateManager()->ExecuteString(scriptHandle->GetTextBuffer()))
		{
            GF_LOG_TRACE_ERR("BaseGameLogic::VBuildInitialScene()", string("Unable to execute ") + luaStartupScriptRes.GetName());
//			SafeGameLogAndPrefix(m_loggerPtr, GameLog::ERR, string("BaseGameLogic::VBuildInitialScene()"), string("Unable to execute ") + luaStartupScriptRes.GetName());
			return;
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseGameLogic::VRenderDiagnostics() 
	{
		// Chapter 19/20 refactor work
		//   - move physics pointer into BaseGameLogic
		//   - but make the inherited logic choose the implementation
		if (m_RenderDiagnostics && m_pPhysics)
		{
			m_pPhysics->VRenderDiagnostics();
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseGameLogic::TogglePause()
	{
		if(m_State == BGS_Running)
		{
			VChangeState(BGS_Paused);
			// Broadcast event to all interested script side listeners.
			IEventDataPtr pauseEventPtr(GCC_NEW EvtData_Pause_Game_Event(true));
			safeQueEvent(pauseEventPtr);
		}
		else if(m_State == BGS_Paused)
		{
			VChangeState(BGS_Running);
			// Broadcast event to all interested script side listeners.
			IEventDataPtr unpauseEventPtr(GCC_NEW EvtData_Pause_Game_Event(false));
			safeQueEvent(unpauseEventPtr);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	optional<GameViewId> BaseGameLogic::VAddView(shared_ptr<IGameView> pView, optional<ActorId> actorId)
	{
		optional<GameViewId> id;

		if(!pView)
		{
            GF_LOG_ERR("Tried to add a NULL GameView pointer to the logic layer");
//			SafeGameLog(m_loggerPtr, GameLog::ERR, string("Tried to add a NULL GameView pointer to the logic layer!"));
			return (id);
		}

		// This makes sure that all views have a non-zero view id.
		id = static_cast<GameViewId>(m_gameViews.size());
		m_gameViews.push_back(pView);
		pView->VOnAttach(*id, actorId);
		pView->VOnRestore();

		return (id);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseGameLogic::VRemoveView(shared_ptr<IGameView> pView)
	{
		if(pView)
		{
			m_gameViews.remove(pView);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseGameLogic::VRemoveView(const GameViewId viewId)
	{
		shared_ptr<IGameView> pView;
		for(GameViewList::iterator curr = m_gameViews.begin(); ((!pView) && (curr != m_gameViews.end())); ++curr)
		{
			if((*curr)->VGetId() == viewId)
			{
				pView = *curr;
			}
		}

		if(pView)
		{
			VRemoveView(pView);
		}
	}

	// /////////////////////////////////////////////////////////////////
	// ****************** BaseLuaGameLogic ************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	BaseLuaGameLogic::BaseLuaGameLogic(shared_ptr<GameOptions> optionsPtr, shared_ptr<GameLog> loggerPtr, shared_ptr<ModelViewProjStackManager> stackManagerPtr)\
		throw (GameException &) : BaseGameLogic(optionsPtr, loggerPtr, stackManagerPtr)
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	BaseLuaGameLogic::~BaseLuaGameLogic()
	{
		try { } catch(...) { }
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseLuaGameLogic::VAddActor(shared_ptr<IActor> actor, IActorParams *p)
	{
		BaseGameLogic::VAddActor(actor, p);

		if(!p || !actor)
		{
			// No need to log error as its done already in BaseGameLogic::VAddActor().
			return;
		}

		shared_ptr<LuaStateManager> luaStateManPtr = g_appPtr->GetLuaStateManager();

		// Ensure script knows about this actor, too.
		LuaPlus::LuaState *pState = luaStateManPtr->GetGlobalState().Get();
		LuaPlus::LuaObject globalActorTable = luaStateManPtr->GetGlobalActorTable();
		if(!pState)
		{
            GF_LOG_TRACE_ERR("BaseLuaGameLogic::VAddActor()", "Failed to get the global LUA state object");
//			SafeGameLogAndPrefix(m_loggerPtr, GameLog::ERR, string("BaseLuaGameLogic::VAddActor()"), string("Failed to get the global LUA state object."));
			return;
		}
		if(!globalActorTable.IsTable())
		{
            GF_LOG_TRACE_ERR("BaseLuaGameLogic::VAddActor()", "Script side actor table does not exist");
//			SafeGameLogAndPrefix(m_loggerPtr, GameLog::ERR, string("BaseLuaGameLogic::VAddActor()"), string("Global actor table is NOT a table! This means that the actor was added but we failed to register the OnCreateFunc and OnDestroyFunc and failed to call the OnCreateFunc() in the LUA scripts!"));
			return;
		}

		LuaPlus::LuaObject addedActorData = globalActorTable.CreateTable(*p->VGetId());	//The actor ID is the key.
		addedActorData.SetInteger("ActorID", *p->VGetId());

		if(0 != p->VGetCreateFuncName())
		{
			addedActorData.SetString("OnCreateFunc", p->VGetCreateFuncName());
		}

		if(0 != p->VGetDestroyFuncName()) 
		{
			addedActorData.SetString("OnDestroyFunc", p->VGetDestroyFuncName());
		}

		//If this actor has any script-specific functions to call, do so now.
		if (0 != strlen(p->VGetCreateFuncName()))
		{
			// First attempt to FIND the function specified.
			LuaPlus::LuaObject foundObj = luaStateManPtr->GetGlobalState()->GetGlobal(p->VGetCreateFuncName());
			if(foundObj.IsNil())
			{
                GF_LOG_TRACE_ERR("BaseLuaGameLogic::VAddActor()", "OnCreateFunc missing...");
//				SafeGameLogAndPrefix(m_loggerPtr, GameLog::ERR, string("BaseLuaGameLogic::VAddActor()"), string("Unable to find specified OnCreateFunc function (") + string(p->VGetCreateFuncName()) + string(")."));
			}
			else
			{
				// Make sure it actually *IS* a function.
				if(!foundObj.IsFunction())
				{
                    GF_LOG_TRACE_ERR("BaseLuaGameLogic::VAddActor()", "OnCreateFunc is not a LUA function - possibly some other script type");
//					SafeGameLogAndPrefix(m_loggerPtr, GameLog::ERR, string("BaseLuaGameLogic::VAddActor()"), string("Specified OnCreateFunc is not a valid function (") + string(p->VGetCreateFuncName()) + string(")."));
				}
				else
				{
					// Attempt to call the function.
					LuaPlus::LuaFunction<void> onCreateFunc(foundObj);
					onCreateFunc(*p->VGetId(), addedActorData);
				}
			}
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseLuaGameLogic::VRemoveActor(const ActorId id)
	{
		shared_ptr<IActor> actor = VGetActor(id);
		if(!actor)
		{
			try
			{
				string actorIdStr = lexical_cast<string, ActorId>(id);
                GF_LOG_TRACE_ERR("BaseLuaGameLogic::VRemoveActor()", string("Actor (") + actorIdStr + string(") does not exist"));
//				SafeGameLogAndPrefix(m_loggerPtr, GameLog::ERR, string("BaseLuaGameLogic::VRemoveActor()"), string("Actor (") + actorIdStr + string(") does not exist."));
			}
			catch(bad_lexical_cast &)
			{
                GF_LOG_TRACE_ERR("BaseLuaGameLogic::VRemoveActor()", "Actor with that ID does not exist");
//				SafeGameLogAndPrefix(m_loggerPtr, GameLog::ERR, string("BaseLuaGameLogic::VRemoveActor()"), string("Actor with that ID does not exist."));
			}
			return;
		}

		// Get the global LUA state manager.
		shared_ptr<LuaStateManager> luaStateManPtr = g_appPtr->GetLuaStateManager();

		// Call any script-related destructor.
		LuaPlus::LuaState *pState = luaStateManPtr->GetGlobalState().Get();
		LuaPlus::LuaObject globalActorTable = luaStateManPtr->GetGlobalActorTable();
		if(!pState)
		{
            GF_LOG_TRACE_ERR("BaseLuaGameLogic::VRemoveActor()", "Failed to get the global LUA state objec");
			SafeGameLogAndPrefix(m_loggerPtr, GameLog::ERR, string("BaseLuaGameLogic::VRemoveActor()"), string("Failed to get the global LUA state object."));
			return;
		}
		if(!globalActorTable.IsTable())
		{
            GF_LOG_TRACE_ERR("BaseLuaGameLogic::VRemoveActor()", "Global script side actor table does not exist");
			SafeGameLogAndPrefix(m_loggerPtr, GameLog::ERR, string("BaseLuaGameLogic::VRemoveActor()"), string("Global actor table is NOT a table!"));
			return;
		}

		LuaPlus::LuaObject actorData = globalActorTable[id];

		shared_ptr<IActorParams> actorParams = actor->VGetParams();
		if(0 != strlen(actorParams->VGetDestroyFuncName()))
		{
			// First attempt to FIND the function specified.
			LuaPlus::LuaObject foundObj = luaStateManPtr->GetGlobalState()->GetGlobal(actorParams->VGetDestroyFuncName());
			if (foundObj.IsNil())
			{
                GF_LOG_TRACE_ERR("BaseLuaGameLogic::VRemoveActor()", "Unable to find specified OnDestroyFunc");
//				SafeGameLogAndPrefix(m_loggerPtr, GameLog::ERR, string("BaseLuaGameLogic::VRemoveActor()"), string("Unable to find specified OnDestroyFunc function (") + string(actorParams->VGetDestroyFuncName()) + string(")."));
			}
			else
			{
				//Make sure it actually *IS* a function.
				if(!foundObj.IsFunction())
				{
                    GF_LOG_TRACE_ERR("BaseLuaGameLogic::VRemoveActor()", "OnDestroyFunc not a valid script function");
//					SafeGameLogAndPrefix(m_loggerPtr, GameLog::ERR, string("BaseLuaGameLogic::VRemoveActor()"), string("The specified OnDestroyFunc function (") + string(actorParams->VGetDestroyFuncName()) + string(") is not a valid function."));
				}
				else
				{
					//Attempt to call the function.
					LuaPlus::LuaFunction<void> onDestroyFunc(foundObj);
					onDestroyFunc(id, actorData);	//Pass in the actor ID and this actor's user-owned data table.
				}
			}
		}

		// Remove the actor from script's grubby little hands.
		actorData.AssignNil(pState);

		// Remove the actor from the logics container.
		BaseGameLogic::VRemoveActor(id);
	}

}
