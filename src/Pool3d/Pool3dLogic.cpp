// /////////////////////////////////////////////////////////////////
// @file Pool3dLogic.cpp
// @author PJ O Halloran.
// @date 01/11/2010
//
// Pool3D logic class implementation file.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <boost/algorithm/string/case_conv.hpp>

// Project Headers
#include "Pool3dLogic.h"
#include "Pool3dActors.h"

#include "TextResource.h"

using boost::shared_ptr;
using boost::optional;

using std::string;

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// ****************** Pool3dLogicEventListener *********************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ActorParams *Pool3dLogicEventListener::CreateNewActorParams(const LuaPlus::LuaObject &srcData)
	{
		// Check input parameters
		if(!srcData.IsTable())
		{
            GF_LOG_TRACE_ERR("Pool3dLogicEventListener::CreateNewActorParams()", "Lua data is not valid");
			return (NULL);
		}

		//Find out the actor type.
		LuaPlus::LuaObject actorTypeObj = srcData["ActorType"];
		if(!actorTypeObj.IsString())
		{
            GF_LOG_TRACE_ERR("Pool3dLogicEventListener::CreateNewActorParams()", "ActorType field not found");
			return (NULL);
		}

		// OK, we've got a string.  Match it up with the appropriate constructor to build the data.
		std::string actorTypeStr(actorTypeObj.GetString());
		boost::algorithm::to_lower(actorTypeStr);
		ActorParams *actorParamsPtr = NULL;
		if(actorTypeStr.compare("pooltable") == 0)
		{
			actorParamsPtr = GCC_NEW TableActorParams;
		}
		else if(actorTypeStr.compare("poolball") == 0)
		{
			actorParamsPtr = GCC_NEW BallActorParams;
		}
		else if(actorTypeStr.compare("poolcue") == 0)
		{
			actorParamsPtr = GCC_NEW CueActorParams;
		}
		else if(actorTypeStr.compare("unknown") == 0)
		{
			actorParamsPtr = GCC_NEW Pool3dActorParams;
		}
		else
		{
            GF_LOG_TRACE_ERR("Pool3dLogicEventListener::CreateNewActorParams()", std::string("Unknown actor type encountered: ") + std::string(actorTypeStr));
			return (NULL);
		}

		if(actorParamsPtr)
		{
			TErrorMessageList errorMessages;
			if(!actorParamsPtr->VInit(srcData, errorMessages))
			{
				// Clean up memory allocated.
				delete actorParamsPtr;
				actorParamsPtr = NULL;

#if DEBUG
				// Spit out all the error messages.
                GF_LOG_TRACE_ERR("Pool3dLogicEventListener::CreateNewActorParams()", "Actor Parameter Errors: ");
				for (TErrorMessageList::const_iterator iter = errorMessages.begin(), end = errorMessages.end(); iter != end; ++iter)
				{
                    GF_LOG_TRACE_ERR("Pool3dLogicEventListener::CreateNewActorParams()", *iter);
				}
#endif
			}
		}

		return (actorParamsPtr);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Pool3dLogicEventListener::Pool3dLogicEventListener(Pool3dLogic *logicPtr)\
		throw (GameException &) : m_logicPtr(logicPtr)
	{
		if(!m_logicPtr)
		{
			throw GameException(string("Invalid game logic pointer supplied"));
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	char const *Pool3dLogicEventListener::VGetName(void)
	{
		return ("Pool3dLogicEventListener");
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::VHandleEvent(IEventData const &eventObj)
	{
		bool result = true;

		// NB. Note to self, the event will not be propagated anymore if you return TRUE from this method.
		//		Take care you return FALSE for events that might have script listeners...

		// Log receipt of the event and its type.
        GF_LOG_TRACE_TRC(VGetName(), string("Recevied event type = ") + string(eventObj.VGetEventType().getStr()));

		// Check what event has occurred and handle it appropriately.
		if(eventObj.VGetEventType() == EvtData_Request_Pause_Game_Event::sk_EventType)
		{
			const EvtData_Request_Pause_Game_Event &castEvent = static_cast<const EvtData_Request_Pause_Game_Event &>(eventObj);
			result = OnRequestPauseEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Game_State::sk_EventType)
		{
			const EvtData_Game_State &castEvent = static_cast<const EvtData_Game_State &>(eventObj);
			result = OnGameStateChangeEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Move_Actor::sk_EventType)
		{
			const EvtData_Move_Actor &castEvent = static_cast<const EvtData_Move_Actor &>(eventObj);
			result = OnMoveActorEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_New_Actor::sk_EventType)
		{
			const EvtData_New_Actor &castEvent = static_cast<const EvtData_New_Actor &>(eventObj);
			result = OnNewActorEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Request_New_Actor::sk_EventType)
		{
			const EvtData_Request_New_Actor &castEvent = static_cast<const EvtData_Request_New_Actor &>(eventObj);
			// Cast away constness as we need to alter the event data in this callback if the request came from a lua
			//  script.  This is safe as this event is meant only for the logic layer!
			EvtData_Request_New_Actor &nonConstEvent = *(const_cast<EvtData_Request_New_Actor *>(&castEvent));
			result = OnRequestNewActorEvent(nonConstEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Request_Start_Game::sk_EventType)
		{
			const EvtData_Request_Start_Game &castEvent = static_cast<const EvtData_Request_Start_Game &>(eventObj);
			result = OnRequestStartGameEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_UpdateActorParams::sk_EventType)
		{
			const EvtData_UpdateActorParams &castEvent = static_cast<const EvtData_UpdateActorParams &>(eventObj);
			result = OnUpdateActorParametersEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_PhysCollision::sk_EventType)
		{
			const EvtData_PhysCollision &castEvent = static_cast<const EvtData_PhysCollision &>(eventObj);
			result = OnActorCollisionEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_PhysTrigger_Leave::sk_EventType)
		{
			const EvtData_PhysTrigger_Leave &castEvent = static_cast<const EvtData_PhysTrigger_Leave &>(eventObj);
			result = OnTriggerAreaLeaveEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_End_Game::sk_EventType)
		{
			const EvtData_End_Game &castEvent = static_cast<const EvtData_End_Game &>(eventObj);
			result = OnEndGameEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Dialog_Open_Event::sk_EventType)
		{
			const EvtData_Dialog_Open_Event &castEvent = static_cast<const EvtData_Dialog_Open_Event &>(eventObj);
			result = OnOpenDialogEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Dialog_Close_Event::sk_EventType)
		{
			const EvtData_Dialog_Close_Event &castEvent = static_cast<const EvtData_Dialog_Close_Event &>(eventObj);
			result = OnCloseDialogEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Complex_Mesh_Loaded::sk_EventType)
		{
			const EvtData_Complex_Mesh_Loaded &castEvent = static_cast<const EvtData_Complex_Mesh_Loaded &>(eventObj);
			result = OnComplexMeshLoadedEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Camera_Aim_Mode_Request::sk_EventType)
		{
			const EvtData_Camera_Aim_Mode_Request &castEvent = static_cast<const EvtData_Camera_Aim_Mode_Request &>(eventObj);
			result = OnCameraAimModeRequest(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_ResetCueBall::sk_EventType)
		{
			const EvtData_ResetCueBall &castEvent = static_cast<const EvtData_ResetCueBall &>(eventObj);
			result = OnResetCueBall(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Move_Kinematic_Actor::sk_EventType)
		{
			const EvtData_Move_Kinematic_Actor &castEvent = static_cast<const EvtData_Move_Kinematic_Actor &>(eventObj);
			result = OnPlayerMoveActor(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Shot_Start::sk_EventType)
		{
			const EvtData_Shot_Start &castEvent = static_cast<const EvtData_Shot_Start &>(eventObj);
			result = OnPoolShotStart(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Physics_Diagnostics::sk_EventType)
		{
			const EvtData_Physics_Diagnostics &castEvent = static_cast<const EvtData_Physics_Diagnostics &>(eventObj);
			result = OnPhysicsDiagnosticUpdate(castEvent);
		}
		// TODO: Handle other Logic layer events here...
		else
		{
            GF_LOG_DEB(string("Logic: Unknown game event received: ") + eventObj.VGetEventType().getStr());
			result = false;
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnRequestPauseEvent(const EvtData_Request_Pause_Game_Event &eventData)
	{
		m_logicPtr->TogglePause();
		// Consume the event as there is only 1 logic layer per application, and this is a special event to
		//  ask that logic layer to pause the game.
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnGameStateChangeEvent(const EvtData_Game_State &eventData)
	{
		// NB If you wish to let other modules change the game state remember that calling VChangeState sends out a game state change event itself!
		// Let other listeners consume the event.
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnMoveActorEvent(const EvtData_Move_Actor &eventData)
	{
		m_logicPtr->VMoveActor(eventData.GetActorId(), eventData.GetMovement());
		// Let View layer consume this event too.
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnNewActorEvent(const EvtData_New_Actor &eventData)
	{
		// 1) Create logic layers actor.
		ActorParams *params = const_cast<ActorParams *>(eventData.GetActorParams());

		// Handle all actor types here (except for the pool table and cue, we will handle this later when 
		//  the view has loaded in the complex meshes with the EvtData_Complex_Mesh_Loaded event).
		if(params && params->VGetType() != AT_Table && params->VGetType() != AT_Cue)
		{
			boost::shared_ptr<BaseActor> actorPtr = boost::static_pointer_cast<BaseActor>(params->VCreate());
			m_logicPtr->VAddActor(actorPtr, actorPtr->VGetParams().get());

			// Check if the cue ball was added, if so then record its actor ID for the logic layer
			BallActorParams *ballActor = static_cast<BallActorParams *>(params);
			if(ballActor && ballActor->GetBallGroup() == BallActorParams::eCue)
			{
				if(params->VGetId().is_initialized())
				{
					m_logicPtr->m_cueBallActorId = *params->VGetId();
				}
			}
		}

		// Let other listeners receive event.
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnRequestNewActorEvent(EvtData_Request_New_Actor &eventData)
	{
		ActorParams *params = NULL;
		bool gotFromScript = false;

		// Firstly determine did the event come from a script or from code.
		if(eventData.VHasLuaEventData())
		{
			// Actor request came from a lua script.
			gotFromScript = true;
			eventData.VBuildLuaEventData();

			// Build the appropriate ActorParams object.
			params = CreateNewActorParams(eventData.VGetLuaEventData());
		}
		else
		{
			// Actor request has come from code (most likely from VBuildInitialScene() in the logic layer).
			//  Cast away constness as we need to set the ID of the parameters below.
			params = const_cast<ActorParams *>(eventData.GetActorParameters());
		}

		if(!params)
		{
            GF_LOG_TRACE_ERR("Pool3dLogicEventListener::OnRequestNewActorEvent()", "NULL parameters, cannot create actor");
		}
		else
		{
			params->VSetId(m_logicPtr->GetNewActorID());
			IEventDataPtr newActorEventData(GCC_NEW EvtData_New_Actor(*params->VGetId(), params));
			if(!safeQueEvent(newActorEventData))
			{
                GF_LOG_TRACE_ERR("Pool3dLogicEventListener::OnRequestNewActorEvent()", "Failed to send out new actor event");
			}

			// If we got actor from a script, then we needed to allocate some memory creating the parameters, so deallocate it now.
			if(gotFromScript)
			{
				Delete(params);
			}
		}

		// Consume the event as its only meant for the logic layer!
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnRequestStartGameEvent(const EvtData_Request_Start_Game &eventData)
	{
		m_logicPtr->VChangeState(BGS_WaitingForPlayers);
		m_logicPtr->VBuildInitialScene();
		m_logicPtr->m_gameStateObj.m_gameStartTime = g_appPtr->GetCurrTime();
		// Consume event as its meant for the logic layer only!
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnUpdateActorParametersEvent(const EvtData_UpdateActorParams &eventData)
	{
		// Handles updating an actors parameters from LUA scripts!
		boost::shared_ptr<IActor> actorPtr = m_logicPtr->VGetActor(eventData.GetActorId());
		if(!actorPtr)
		{
			std::string idStr;
			try { idStr = boost::lexical_cast<std::string, ActorId>(eventData.GetActorId()); } catch(...) {}
            GF_LOG_TRACE_ERR("Pool3dLogicEventListener::OnUpdateActorParametersEvent()", std::string("No Actor found in the logic layer with the ID supplied: ") + idStr);
		}
		else
		{
			boost::shared_ptr<IActorParams> paramsPtr = actorPtr->VGetParams();
			if(!paramsPtr)
			{
				std::string idStr;
				try { idStr = boost::lexical_cast<std::string, ActorId>(eventData.GetActorId()); } catch(...) {}
                GF_LOG_TRACE_ERR("Pool3dLogicEventListener::OnUpdateActorParametersEvent()", std::string("The Actor ") + idStr + std::string(" has NULL parameters!"));
			}
			else
			{
				// We will reinitialize the parameters from the LUA data supplied
				TErrorMessageList errMsgList;
				if(!paramsPtr->VInit(eventData.VGetLuaEventData(), errMsgList))
				{
                    GF_LOG_TRACE_ERR("Pool3dLogicEventListener::OnUpdateActorParametersEvent()", "Failed to reinitialize the actors parameters (list of causes): ");
					for(TErrorMessageList::iterator i = errMsgList.begin(), end = errMsgList.end(); i != end; ++i)
					{
                        GF_LOG_TRACE_ERR("Pool3dLogicEventListener::OnUpdateActorParametersEvent()", *i);
					}
				}
			}
		}

		// Allow the view to receive the event too.
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnActorCollisionEvent(const EvtData_PhysCollision &eventData)
	{
		//// Update shot state (Inc shot counters, check for penalties).
		//static I32 collCount = 1;
		//if(collCount % 10 == 0)
		//{
		//	std::cout << "Number of collisions: " << collCount << std::endl;
		//}
		//++collCount;


		if(m_logicPtr->m_gameStateObj.m_shotState.IsInProgress())
		{
			const I32 playerGroup = m_logicPtr->m_gameStateObj.m_playersArr[m_logicPtr->m_gameStateObj.m_currentPlayerIndex].m_ballGroup;	// Players ball group.
			boost::shared_ptr<IActor> actorA(m_logicPtr->VGetActor(eventData.GetActorAId()));												// First actor.
			boost::shared_ptr<IActor> actorB(m_logicPtr->VGetActor(eventData.GetActorBId()));												// Second actor.

			if(actorA && actorB)
			{
				I32 typeA(actorA->VGetType()), typeB(actorB->VGetType());
				if(typeA == AT_Ball && typeB == AT_Ball)
				{
					// Increment total number of ball - ball collisions.
					++m_logicPtr->m_gameStateObj.m_shotState.m_bbCollisionsCount;

					// Cast actors to pool ball actors and check which is the cue ball
					boost::shared_ptr<BallActorParams> ballA(boost::static_pointer_cast<BallActorParams>(actorA->VGetParams()));
					boost::shared_ptr<BallActorParams> ballB(boost::static_pointer_cast<BallActorParams>(actorB->VGetParams()));
					boost::shared_ptr<BallActorParams> cueBallActor, otherBallActor;
					if(ballA->GetBallGroup() == BallActorParams::eCue)
					{
						cueBallActor = ballA;
						otherBallActor = ballB;
					}
					else if(ballB->GetBallGroup() == BallActorParams::eCue)
					{
						cueBallActor = ballB;
						otherBallActor = ballA;
					}

					// If one of the balls is a cue ball.
					if(cueBallActor)
					{
						// If no cb - pb collisions yet, check if the cue ball has hit a valid ball first
						if((m_logicPtr->m_gameStateObj.m_shotState.m_cbpbCollisionCount == 0) && ((playerGroup == BallActorParams::eNone) || (otherBallActor->GetBallGroup() == playerGroup)))
						{
							m_logicPtr->m_gameStateObj.m_shotState.m_validFirstHit = true;
						}

						// Increment the number of cb - pb collisions
						++m_logicPtr->m_gameStateObj.m_shotState.m_cbpbCollisionCount;
					}
				}
				else if((typeA == AT_Ball && typeB == AT_Table) || (typeB == AT_Ball && typeA == AT_Table))
				{
					// Increment the number of ball - table collisions
					++m_logicPtr->m_gameStateObj.m_shotState.m_btCollisionCount;
				}
			}
		}
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnTriggerAreaLeaveEvent(const EvtData_PhysTrigger_Leave &eventData)
	{
		// Record ball drop (only when a shot is in progress).
		if(m_logicPtr->m_gameStateObj.m_shotState.IsInProgress() && eventData.GetActorId().is_initialized())
		{
			boost::shared_ptr<IActor> actorPtr = m_logicPtr->VGetActor(*eventData.GetActorId());
			if(actorPtr)
			{
				boost::shared_ptr<BallActorParams> ballActor(boost::static_pointer_cast<BallActorParams>(actorPtr->VGetParams()));

				if(ballActor)
				{
					BallActorParams::BallGroup bg = ballActor->GetBallGroup();

					// If it is the first ball the player has potted (and its one of the main ball types) then record the player group
					if(m_logicPtr->m_gameStateObj.m_playersArr[m_logicPtr->m_gameStateObj.m_currentPlayerIndex].m_score == 0 &&\
						(bg == BallActorParams::eStripes || bg == BallActorParams::ePlain))
					{
						m_logicPtr->m_gameStateObj.m_playersArr[m_logicPtr->m_gameStateObj.m_currentPlayerIndex].m_ballGroup = bg;

						// TODO: Get other player index, set its ball group to the opposite of this one.
						I32 otherIndex = (m_logicPtr->m_gameStateObj.m_currentPlayerIndex == 0 ? 1 : 0);
						if(bg == BallActorParams::eStripes)
						{
							m_logicPtr->m_gameStateObj.m_playersArr[otherIndex].m_ballGroup = BallActorParams::ePlain;
						}
						else
						{
							m_logicPtr->m_gameStateObj.m_playersArr[otherIndex].m_ballGroup = BallActorParams::eStripes;
						}
					}

					// Record ball pot to state.
					m_logicPtr->m_gameStateObj.m_shotState.AddPottedBall(*eventData.GetActorId(),\
																			bg,\
																			m_logicPtr->m_gameStateObj.m_playersArr[m_logicPtr->m_gameStateObj.m_currentPlayerIndex].m_id,\
																			eventData.GetTriggerId(),\
																			static_cast<F32>(g_appPtr->GetCurrTime()));


					// Remove actor from simulation
					m_logicPtr->VRemoveActor(*eventData.GetActorId());
				}
			}
		}
		// TODO: what if a ball drops outside a shot??
		else
		{

		}
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnEndGameEvent(const EvtData_End_Game &eventData)
	{
		// Clear actor data from physics and logic & change to MM state.
		m_logicPtr->ResetGameLogic();
		m_logicPtr->m_gameStateObj.m_shotState.Reset();
		m_logicPtr->m_gameStateObj.m_shotCount = 0;

		m_logicPtr->VChangeState(BGS_MainMenu);

		// Event is only meant for the logic layer so we will consume it here.
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnOpenDialogEvent(const EvtData_Dialog_Open_Event &eventData)
	{
		if(m_logicPtr->GetCurrentState() == BGS_Running)
		{
			m_logicPtr->VChangeState(BGS_DialogPaused);
		}
		// Allow other listeners to receive event.
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnCloseDialogEvent(const EvtData_Dialog_Close_Event &eventData)
	{
		if(m_logicPtr->GetCurrentState() == BGS_DialogPaused)
		{
			// After a dialog is closed we want to return to the state it interrrupted.
			m_logicPtr->VChangeState(m_logicPtr->GetPreviousState());
		}

		// Allow other listeners to receive event.
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnComplexMeshLoadedEvent(const EvtData_Complex_Mesh_Loaded &eventData)
	{
		// Create new Pool Table actor
		ActorParams *params = eventData.GetActorParameters();
		
		boost::shared_ptr<BaseActor> actorPtr = boost::static_pointer_cast<BaseActor>(params->VCreate());
		m_logicPtr->VAddActor(actorPtr, actorPtr->VGetParams().get());

		if(params->VGetType() == AT_Cue)
		{
			// game/scene is loaded now, inform app
			safeQueEvent(IEventDataPtr(GCC_NEW EvtData_Loading_Progress(POOL_GAME_LOADED_ID, 1.0)));
		}
		else if(params->VGetType() == AT_Table)
		{
			TableActorParams *tableActor = static_cast<TableActorParams *>(params);
			if(tableActor && tableActor->VGetId())
			{
				m_logicPtr->m_tableActorId = *tableActor->VGetId();
			}
		}

		// Stop event being propagated as its meant for the logic layer only.
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnCameraAimModeRequest(const EvtData_Camera_Aim_Mode_Request &eventData)
	{
		m_logicPtr->AimModeRequest(eventData.GetCameraMode());
		// Stop event being propagated as its meant for the logic layer only.
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnPoolShotStart(const EvtData_Shot_Start &eventData)
	{
		// Start the shot with 
		if(m_logicPtr->m_gameStateObj.m_shotState.IsInProgress())
		{
			// Log Error - shot start event received during a shot.
            GF_LOG_ERR("Recevied EvtData_Shot_Start event when a shot was already in progress");
			return (false);
		}

		if(eventData.GetPlayerId() != m_logicPtr->m_gameStateObj.m_currentPlayerIndex)
		{
			// Log Error - shot start event received during a shot.
            GF_LOG_ERR("Wrong player took the shot");
			return (false);
		}

		m_logicPtr->m_gameStateObj.m_shotState.m_startTime = g_appPtr->GetCurrTime();
		++m_logicPtr->m_gameStateObj.m_shotCount;

		// Apply force to the cue ball to start the shot.
		if(m_logicPtr->VGetGamePhysics())
		{
			//if(eventData.GetSpinDirection() != Vector3(0.0f))
			//{
			//	m_logicPtr->VGetGamePhysics()->VApplyTorque(eventData.GetSpinDirection(), eventData.GetShotPower(), m_logicPtr->m_cueBallActorId);
			//}
			//else
			//{
				m_logicPtr->VGetGamePhysics()->VApplyForce(eventData.GetShotDirection(), eventData.GetShotPower(), m_logicPtr->m_cueBallActorId);
			//}
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnResetCueBall(const EvtData_ResetCueBall &eventData)
	{
		boost::shared_ptr<IActor> actorPtr = m_logicPtr->VGetActor(m_logicPtr->m_cueBallActorId);
		if(actorPtr)
		{
			boost::shared_ptr<BallActorParams> cueBallActorParams(boost::static_pointer_cast<BallActorParams>(actorPtr->VGetParams()));
			if(!cueBallActorParams || cueBallActorParams->GetBallGroup() != BallActorParams::eCue)
			{
                GF_LOG_TRACE_ERR("Pool3dLogicEventListener::OnResetCueBall()", "Cue ball actor not found");
				return (true);
			}

			// Tell the physics system to warp the actor to the desired position.
			Matrix4 actorMat(g_identityMat);
			actorMat.SetPosition(eventData.GetCueBallPosition());
			if(m_logicPtr->VGetGamePhysics())
			{
				m_logicPtr->VGetGamePhysics()->VKinematicMove(actorMat, m_logicPtr->m_cueBallActorId);
			}
		}

		// Stop event being propagated as its meant for the logic layer only.
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnPlayerMoveActor(const EvtData_Move_Kinematic_Actor &eventData)
	{
		// Move the actor using the physics kinematic move function - Note this won't work well unless
		//  the game actor has been setup with the physics system as a user controlled (kinematic) rigid body.
		if(m_logicPtr->VGetGamePhysics())
		{
			m_logicPtr->VGetGamePhysics()->VKinematicMove(eventData.GetMovement(), eventData.GetActorId());
		}
		// Stop event being propagated as its meant for the logic layer only.
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogicEventListener::OnPhysicsDiagnosticUpdate(const EvtData_Physics_Diagnostics &eventData)
	{
		if(m_logicPtr->VGetGamePhysics())
		{
			m_logicPtr->SetDebugMode(eventData.IsRender());
			if(eventData.GetMode().is_initialized())
			{
				m_logicPtr->SetDiagnosticsMode(*eventData.GetMode());
				m_logicPtr->VGetGamePhysics()->VSetDiagnosticMode(*eventData.GetMode());
			}
		}
		// Stop event being propagated as its meant for the logic layer only.
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	// ************************* Pool3dLogic ***************************
	// /////////////////////////////////////////////////////////////////

	// Declaration of global string constants.
	const std::string Pool3dLogic::PGL_POOL_BALL_COLLISION_SHAPE("PoolBall");
	const std::string Pool3dLogic::PGL_POOL_CUE_COLLISION_SHAPE("PoolCue");
	const std::string Pool3dLogic::PGL_POOL_TABLE_FLOOR_MIDDLE_COLLISION_SHAPE("TableFloorMiddle");
	const std::string Pool3dLogic::PGL_POOL_TABLE_SIDE_COLLISION_SHAPE("TableSide");
	const std::string Pool3dLogic::PGL_POOL_TABLE_FRONT_COLLISION_SHAPE("TableFront");
	const std::string Pool3dLogic::PGL_POOL_TABLE_FRONT_BOUNDARY_COLLISION_SHAPE("FrontBoundary");
	const std::string Pool3dLogic::PGL_POOL_TABLE_SIDE_BOUNDARY_COLLISION_SHAPE("SideBoundary");

#ifdef DEBUG
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dLogic::Pool3dGameState::LogState()
	{
		std::cout << " ***** Game State ***** " << std::endl;
		std::cout << "Game start time: " << m_gameStartTime << std::endl;
		std::cout << "Current Player Index: " << m_currentPlayerIndex << std::endl;
		std::cout << "Number of shots: " << m_shotCount << std::endl << std::endl;
		for(I32 i = 0; i < 2; ++i)
		{
			std::cout << "Player " << i+1 << " Information: " << std::endl;
			std::cout << "\tBall group: " << m_playersArr[i].m_ballGroup << std::endl;
			std::cout << "\tId: " << m_playersArr[i].m_id << std::endl;
			std::cout << "\tScore: " << m_playersArr[i].m_score << std::endl;
			std::cout << "\tShots Remaining: " << m_playersArr[i].m_turnShots << std::endl << std::endl;
		}

		std::cout << "Shot state Information: " << std::endl << std::endl;
		std::cout << "Ball-Ball collision count: " << m_shotState.m_bbCollisionsCount << std::endl;
		std::cout << "CueBall-PlayerBall collision count: " << m_shotState.m_cbpbCollisionCount << std::endl;
		std::cout << "Ball-Table collision count: " << m_shotState.m_btCollisionCount << std::endl;
		std::cout << "Black Ball potted: " << m_shotState.m_blackBallPotted << std::endl;
		std::cout << "Cue Ball potted: " << m_shotState.m_cueBallPotted << std::endl;
		std::cout << "Shot start time: " << m_shotState.m_startTime << std::endl;
		std::cout << "Shot end time: " << m_shotState.m_endTime << std::endl;
		std::cout << "Valid First Hit?: " << m_shotState.m_validFirstHit << std::endl;

		std::cout << "Potted Ball List Info: " << std::endl;
		for(std::vector<BallPottedState>::iterator i = m_shotState.m_pottedBallVec.begin(); i != m_shotState.m_pottedBallVec.end(); ++i)
		{
			std::cout << "\tBall Id: " << (*i).m_ballId << std::endl;
			std::cout << "\tBall Group: " << (*i).m_ballGroup << std::endl;
			std::cout << "\tPlayer Id: " << (*i).m_playerId << std::endl;
			std::cout << "\tPot timestamp: " << (*i).m_potTimestamp << std::endl;
			std::cout << "\tPocket Id: " << (*i).m_pocketId << std::endl;
		}
		std::cout << std::endl;
	}
#endif

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Pool3dLogic::Pool3dLogic(shared_ptr<GameOptions> optionsPtr, shared_ptr<GameLog> loggerPtr, shared_ptr<ModelViewProjStackManager> stackManagerPtr) throw (GameException &)
		: BaseLuaGameLogic(optionsPtr, loggerPtr, stackManagerPtr), m_listener(), m_cueBallActorId(0), m_tableActorId(0)
	{
		m_listener.reset(GCC_NEW Pool3dLogicEventListener(this));

		// Register game events here
		safeAddListener(m_listener, EvtData_Camera_Aim_Mode_Request::sk_EventType);
		safeAddListener(m_listener, EvtData_Complex_Mesh_Loaded::sk_EventType);
		safeAddListener(m_listener, EvtData_Request_Pause_Game_Event::sk_EventType);
		safeAddListener(m_listener, EvtData_Game_State::sk_EventType);
		safeAddListener(m_listener, EvtData_Move_Actor::sk_EventType);
		safeAddListener(m_listener, EvtData_New_Actor::sk_EventType);
		safeAddListener(m_listener, EvtData_Request_New_Actor::sk_EventType);
		safeAddListener(m_listener, EvtData_Request_Start_Game::sk_EventType);
		safeAddListener(m_listener, EvtData_UpdateActorParams::sk_EventType);
		safeAddListener(m_listener, EvtData_PhysCollision::sk_EventType);
		safeAddListener(m_listener, EvtData_PhysTrigger_Leave::sk_EventType);
		safeAddListener(m_listener, EvtData_End_Game::sk_EventType);
		safeAddListener(m_listener, EvtData_Dialog_Open_Event::sk_EventType);
		safeAddListener(m_listener, EvtData_Dialog_Close_Event::sk_EventType);
		safeAddListener(m_listener, EvtData_ResetCueBall::sk_EventType);
		safeAddListener(m_listener, EvtData_Move_Kinematic_Actor::sk_EventType);
		safeAddListener(m_listener, EvtData_Shot_Start::sk_EventType);
		safeAddListener(m_listener, EvtData_Physics_Diagnostics::sk_EventType);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Pool3dLogic::~Pool3dLogic()
	{
		try
		{
			ResetGameLogic();

			// Unregister game events here.
			safeDelListener(m_listener, EvtData_Camera_Aim_Mode_Request::sk_EventType);
			safeDelListener(m_listener, EvtData_Complex_Mesh_Loaded::sk_EventType);
			safeDelListener(m_listener, EvtData_Request_Pause_Game_Event::sk_EventType);
			safeDelListener(m_listener, EvtData_Request_Pause_Game_Event::sk_EventType);
			safeDelListener(m_listener, EvtData_Game_State::sk_EventType);
			safeDelListener(m_listener, EvtData_Move_Actor::sk_EventType);
			safeDelListener(m_listener, EvtData_New_Actor::sk_EventType);
			safeDelListener(m_listener, EvtData_Request_New_Actor::sk_EventType);
			safeDelListener(m_listener, EvtData_Request_Start_Game::sk_EventType);
			safeDelListener(m_listener, EvtData_UpdateActorParams::sk_EventType);
			safeDelListener(m_listener, EvtData_PhysCollision::sk_EventType);
			safeDelListener(m_listener, EvtData_PhysTrigger_Leave::sk_EventType);
			safeDelListener(m_listener, EvtData_End_Game::sk_EventType);
			safeDelListener(m_listener, EvtData_Dialog_Open_Event::sk_EventType);
			safeDelListener(m_listener, EvtData_Dialog_Close_Event::sk_EventType);
			safeDelListener(m_listener, EvtData_ResetCueBall::sk_EventType);
			safeDelListener(m_listener, EvtData_Move_Kinematic_Actor::sk_EventType);
			safeDelListener(m_listener, EvtData_Shot_Start::sk_EventType);
			safeDelListener(m_listener, EvtData_Physics_Diagnostics::sk_EventType);
		}
		catch(...) {}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogic::HasShotEnded()
	{
		bool anyActorMoving = false;

		// Check if any actors are currently moving.
		if(VGetGamePhysics())
		{
			// Check if every game actors linear and angular velocity is 0 yet.
			PhysicsObjectAttributes currActorStatus;
			for(ActorMap::iterator i = m_ActorList.begin(), end = m_ActorList.end(); ((!anyActorMoving) && (i != end)); ++i)
			{
				if(VGetGamePhysics()->VGetActorObjectStatus(i->first, currActorStatus) && currActorStatus.IsMoving())
				{
					// Actor is currently moving
					anyActorMoving = true;
				}
			}
		}

		return (!anyActorMoving);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dLogic::GetNearestPlayerBallPosition(const BallActorParams::BallGroup groupId, const Point3 &cueBallPos, boost::optional<Point3> &posRef)
	{
		// Ensure input/output parameter holds nothing.
		posRef.reset();

		// Check input parameters
		if(groupId == BallActorParams::eNumberBallGroupTypes)
		{
			// No ball found.
			return;
		}

		F32 minDistanceSqr = FLT_MAX;					// Maximum distance found between the cue ball and the player ball(s).

		// Iterate through all game actors and check the positions of all ball actors who
		//  belong to the same group as the one we are searching for.
		for(ActorMap::iterator i = m_ActorList.begin(), end = m_ActorList.end(); i != end; ++i)
		{
			if((i->second)->VGetType() == AT_Ball && (i->second)->VGetParams())
			{
				BallActorParams *currBallParams = static_cast<BallActorParams *>((i->second)->VGetParams().get());
				if(currBallParams && (currBallParams->GetBallGroup() != BallActorParams::eCue && (groupId == BallActorParams::eNone || currBallParams->GetBallGroup() == groupId)))
				{
					Matrix4 actorMat((i->second)->VGetMat());
					Point3 currActorPos;
					actorMat.GetPosition(currActorPos);

					F32 dSqr = cueBallPos.DistanceSqr(currActorPos);
					if(dSqr < minDistanceSqr)
					{
						minDistanceSqr = dSqr;
						posRef = currActorPos;
					}
				}
			}
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dLogic::UpdatePoolGameLogic()
	{
		// If a shot is currently in progress, check if the shot has completed yet.
		if(m_gameStateObj.m_shotState.IsInProgress() && HasShotEnded())
		{
			// A) Mark the end time of the shot - shot complete.
			m_gameStateObj.m_shotState.m_endTime = g_appPtr->GetCurrTime();

			// B) Check if the player potted any balls OR incurred any penalties OR won/lost the game during the previous shot.

			// Count the number of balls potted this shot (valid and invalid balls).
			I32 pBallsCount = m_gameStateObj.m_shotState.GetPottedBallsCount(m_gameStateObj.m_playersArr[m_gameStateObj.m_currentPlayerIndex].m_ballGroup);
			I32 invalidBallsCount = m_gameStateObj.m_shotState.m_pottedBallVec.size() - pBallsCount;
			bool penalty = false;					// Minor shot penalty flag.
			bool gameEndPenalty = false;			// Game ending penalty flag.
			bool gameWon = false;					// Game won flag.

			// Get current players ball group ID from the start of the shot.
			I32 playerGroup = m_gameStateObj.m_playersArr[m_gameStateObj.m_currentPlayerIndex].m_ballGroup;

			// Special case conditions...

			// 1) If not on black and its potted, game LOST...
			if(playerGroup != BallActorParams::eBlack && m_gameStateObj.m_shotState.m_blackBallPotted)
			{
				gameEndPenalty = true;
			}
			// 2) If on black and its potted, game WON
			if(playerGroup == BallActorParams::eBlack && m_gameStateObj.m_shotState.m_blackBallPotted)
			{
				gameWon = true;
//				// TODO: Remove logic test...
//				if(m_gameStateObj.m_playersArr[m_gameStateObj.m_currentPlayerIndex].m_score != 9)
//				{
//					I32 test = 0;
//				}
			}
			// 3) if ball from another group is potted (except black) OR if the player did not hit a ball of their group first with the cue ball, PENALTY...
			if((invalidBallsCount > 0 && !m_gameStateObj.m_shotState.m_blackBallPotted) || !m_gameStateObj.m_shotState.m_validFirstHit)
			{
				penalty = true;
			}

			// Increment this players score
			if(pBallsCount > 0)
			{
				m_gameStateObj.m_playersArr[m_gameStateObj.m_currentPlayerIndex].m_score += pBallsCount;

				// TODO: (Write And) Send out player score update event.

				// If the current player has potted all their balls then, move them onto the black ball group
				if(m_gameStateObj.m_playersArr[m_gameStateObj.m_currentPlayerIndex].m_score >= 7)
				{
					m_gameStateObj.m_playersArr[m_gameStateObj.m_currentPlayerIndex].m_ballGroup = BallActorParams::eBlack;
				}
			}

			// Check if this player potted any of the other players balls during this shot and increment that players score.
			if(invalidBallsCount > 0)
			{
				I32 otherIndex = (m_gameStateObj.m_currentPlayerIndex == 0 ? 1 : 0);
				I32 otherPlayerScore = m_gameStateObj.m_shotState.GetPottedBallsCount(m_gameStateObj.m_playersArr[otherIndex].m_ballGroup);
				m_gameStateObj.m_playersArr[otherIndex].m_score += otherPlayerScore;

				// TODO: (Write And) Send out player score update event.

				// If the current player has potted all their balls then, move them onto the black ball group
				if(m_gameStateObj.m_playersArr[otherIndex].m_score >= 7)
				{
					m_gameStateObj.m_playersArr[otherIndex].m_ballGroup = BallActorParams::eBlack;
				}
			}
			
			// C) Get the resting position of the cue ball in the scene (if it was not potted).
			boost::optional<Point3> cueBallPos;
			if(!gameEndPenalty || !gameWon || !m_gameStateObj.m_shotState.m_cueBallPotted)
			{
				boost::shared_ptr<IActor> cueBallActor = VGetActor(m_cueBallActorId);
				if(cueBallActor)
				{
					Point3 pos;
					cueBallActor->VGetMat().GetPosition(pos);
					cueBallPos = pos;
				}
			}

			// D) Get the position of the nearest player ball and include it in the event.
			boost::optional<Point3> nearestBallPos;
			if((!gameEndPenalty || !gameWon) && cueBallPos.is_initialized())
			{
				GetNearestPlayerBallPosition(BallActorParams::BallGroup(playerGroup), *cueBallPos, nearestBallPos);
			}



			// E) Reset the previous shot state and Select the next player based on how many shots the player has remaining, shot penalties, etc.
			I32 oldPlayerId(m_gameStateObj.m_currentPlayerIndex);
			if(!gameEndPenalty || !gameWon)
			{
				if(penalty)
				{
					// Current player should have no more turns.
					m_gameStateObj.m_playersArr[m_gameStateObj.m_currentPlayerIndex].m_turnShots = 0;
					m_gameStateObj.NextPlayer();

					// Give the next player 2 shots
					m_gameStateObj.m_playersArr[m_gameStateObj.m_currentPlayerIndex].m_turnShots = 2;

					if(m_gameStateObj.m_shotState.m_cueBallPotted)
					{
						//recreate the cue ball actor (the lua state has information on where to place the white ball so we ask the scripts to trigger the cue ball)
						g_appPtr->GetLuaStateManager()->ExecuteString("EventManager:QueueEvent(\"request_new_actor\", p3diBallCueData)");
						LuaPlus::LuaObject cueBallStartFuncObj = g_appPtr->GetLuaStateManager()->GetGlobalState()->GetGlobals().GetByName("p3diGetCueBallStartingPosition");
						LuaPlus::LuaObject cueBallLuaPos;
						if(cueBallStartFuncObj.IsFunction())
						{
							LuaPlus::LuaFunction<LuaPlus::LuaObject> LuaGetCueBallStartPos(cueBallStartFuncObj);
							cueBallLuaPos = LuaGetCueBallStartPos();
							cueBallPos.reset(Point3(0.0));
							GameHalloran::SetPoint3FromLua(cueBallLuaPos, cueBallPos.get());
						}
					}
				}
				else
				{
					// Decrement number of turns if the player failed to pot any of their balls.
					if(pBallsCount == 0 && m_gameStateObj.m_playersArr[m_gameStateObj.m_currentPlayerIndex].m_turnShots != 0)
					{
						--m_gameStateObj.m_playersArr[m_gameStateObj.m_currentPlayerIndex].m_turnShots;
					}

					if(m_gameStateObj.m_playersArr[m_gameStateObj.m_currentPlayerIndex].m_turnShots <= 0)
					{
						m_gameStateObj.m_playersArr[m_gameStateObj.m_currentPlayerIndex].m_turnShots = 0;

						// Move to the next player when the current player has no more turns left and give them 1 shot
						m_gameStateObj.NextPlayer();
						m_gameStateObj.m_playersArr[m_gameStateObj.m_currentPlayerIndex].m_turnShots = 1;
					}
				}
			}

#ifdef _DEBUG
			m_gameStateObj.LogState();
#endif
			m_gameStateObj.m_shotState.Reset();

			// E) Send the shot end event to all game systems...
			safeQueEvent(IEventDataPtr(GCC_NEW EvtData_Shot_End(oldPlayerId, m_gameStateObj.m_currentPlayerIndex, cueBallPos, nearestBallPos, m_gameStateObj.m_playersArr[oldPlayerId].m_score, penalty, gameEndPenalty, gameWon)));
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dLogic::VBuildInitialScene()
	{
		BaseLuaGameLogic::VBuildInitialScene();

		// Execute the Pool3d specific scene startup script.

		std::string initialScriptName(g_appPtr->GetLuaDataDir().string() + std::string("/Pool3dInitialScene.lua"));
		if(!g_appPtr->GetLuaStateManager()->DoFile(initialScriptName.c_str()))
		{
			return;
		}

		//g_appPtr->GetLuaStateManager()->DoResourceFile(std::string("lua") + ZIP_FILE_SEPERATOR + std::string("Pool3dInitialScene.lua"));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogic::VLoadGame(const string &gameNameRef)
	{
		return (BaseLuaGameLogic::VLoadGame(gameNameRef));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dLogic::VSaveGame()
	{
		return (BaseLuaGameLogic::VSaveGame());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dLogic::VOnUpdate(const F64 time, const F32 elapsedTime)
	{
		BaseLuaGameLogic::VOnUpdate(time, elapsedTime);

		// TODO: Update These when im implementing the game logic!
		switch(GetCurrentState())
		{
		case BGS_Initializing:
			{

			}
			break;
		case BGS_LoadingGameEnvironment:
			{

			}
			break;
		case BGS_MainMenu:
			{

			}
			break;

		case BGS_WaitingForPlayers:
			{
				
			}
			break;

		case BGS_SpawnAI:
			{
				
			}
			break;

		case BGS_Running:
			{
				UpdatePoolGameLogic();

				// Update the state of the physics simulation and notify all game systems with game events...
				if(m_pPhysics)
				{
					m_pPhysics->VOnUpdate(elapsedTime);
					m_pPhysics->VSyncVisibleScene();
				}
			}
			break;

		case BGS_Paused:
			{
				
			}
			break;

		case BGS_DialogPaused:
			{

			}
			break;

		default:
			break;
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dLogic::VChangeState(const enum BaseGameState newState)
	{
		BaseLuaGameLogic::VChangeState(newState);

		// TODO: Update These when im implementing the game!
		switch(newState)
		{
		case BGS_Initializing:
			{ // See GameLogic::VOnUpdate()
			}
			break;
		case BGS_LoadingGameEnvironment:
			{ // See GameLogic::VOnUpdate()
			}
			break;
		case BGS_MainMenu:
			{ // Changed from this state to Waiting for players when the request start game event is sent!
			}
			break;

		case BGS_WaitingForPlayers:
			{
				// We have no network players here so we will just move to running mode.
				BaseLuaGameLogic::VChangeState(BGS_Running);
			}
			break;

		case BGS_SpawnAI:
			{  // See GameLogic::VOnUpdate()
				BaseLuaGameLogic::VChangeState(BGS_Running);
			}
			break;

		case BGS_Running:
			{
				// Send out the new game event to notify all listeners that a game has started
				IEventDataPtr newGameEventDataPtr(GCC_NEW EvtData_New_Game());
				if(!safeQueEvent(newGameEventDataPtr))
				{
                    GF_LOG_TRACE_ERR("Pool3dLogic::VChangeState()", std::string("Failed to broadcast the event: ") + EvtData_New_Game::sk_EventType.getStr());
				}
			}
			break;

		case BGS_Paused:
			{
				
			}
			break;

		case BGS_DialogPaused:
			{

			}
			break;

		default:
			break;
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dLogic::ResetGameLogic()
	{
		// Remove all table trigger area bodies from the physics simulation.
		if(VGetGamePhysics())
		{
			ActorMap::iterator i = m_ActorList.find(m_tableActorId);
			if(i != m_ActorList.end())
			{
				boost::shared_ptr<IActor> tableActor = (*i).second;
				if(tableActor)
				{
					TableActorParams *tableParams = static_cast<TableActorParams *>(tableActor->VGetParams().get());
					if(tableParams)
					{
						VGetGamePhysics()->VRemoveTrigger(tableParams->GetBottomLeftPocketTriggerId());
						VGetGamePhysics()->VRemoveTrigger(tableParams->GetBottomRightPocketTriggerId());
						VGetGamePhysics()->VRemoveTrigger(tableParams->GetMiddleLeftPocketTriggerId());
						VGetGamePhysics()->VRemoveTrigger(tableParams->GetMiddleRightPocketTriggerId());
						VGetGamePhysics()->VRemoveTrigger(tableParams->GetTopLeftPocketTriggerId());
						VGetGamePhysics()->VRemoveTrigger(tableParams->GetTopRightPocketTriggerId());
					}
				}
			}
		}

		// Remove all game actors.
		while (!m_ActorList.empty())
		{
			ActorMap::iterator i = m_ActorList.begin();
			ActorId aid = (*i).first;
			VRemoveActor(aid);
		}

		// Reset pool game state.
		m_gameStateObj.m_gameStartTime = 0.0;
		m_gameStateObj.m_currentPlayerIndex = 0;
		m_gameStateObj.m_playersArr[0].Reset();
		m_gameStateObj.m_playersArr[1].Reset();
		m_gameStateObj.m_shotState.Reset();

		// Kill and clear all running tasks.
		m_pProcessManager->ClearAll();

		// Free all allocated Bullet shared collision shapes.
		for(CollisionShapeMap::iterator i = m_collisionShapeLookupTable.begin(), end = m_collisionShapeLookupTable.end(); i != end; ++i)
		{
			if(i->second)
			{
				delete i->second;
				i->second = NULL;
			}
		}
		m_collisionShapeLookupTable.clear();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dLogic::AimModeRequest(const enum ViewMode viewMode)
	{
		// New mode invalid OR aim mode requested but shot is still in progress
		if((viewMode == eNumberViewModes) || (viewMode == eShotAim && m_gameStateObj.m_shotState.IsInProgress()))
		{
			// Disallow camera aim change request.
			return;
		}

		// Allow the view mode change and broadcast an update event.
		m_viewMode = viewMode;
		IEventDataPtr viewModeUpdate(GCC_NEW EvtData_Camera_Aim_Mode_Update(m_viewMode));
		safeQueEvent(viewModeUpdate);
	}

}
