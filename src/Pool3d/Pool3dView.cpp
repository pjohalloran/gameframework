// /////////////////////////////////////////////////////////////////
// @file Pool3dView.cpp
// @author PJ O Halloran.
// @date 01/11/2010
//
// File contains the header for the Pool3d game view layer class.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <string>
#include <iostream>

#include <boost/filesystem.hpp>

// Project Headers
#include "Pool3dView.h"

#include "GameBase.h"
#include "GameMain.h"
#include "CameraSceneNode.h"

#include "TextResource.h"
#include "CAudio.h"
#include "EnvironmentSceneNode.h"
#include "OpenAlAudio.h"
#include "GLTools.h"
#include "ZipFile.h"

// DEBUG
#include <iostream>
#include "Pool3dActors.h"
#include "Pool3dSceneNodes.h"

// TEST
#include "ObjModelFileLoader.h"

using boost::optional;
using boost::shared_ptr;
using boost::filesystem::path;

using std::string;

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// ******************* Pool3dViewEventListener *********************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Pool3dViewEventListener::Pool3dViewEventListener(Pool3dView *viewPtr)\
		throw (GameException &) : m_viewPtr(viewPtr)
	{
		if(!m_viewPtr)
		{
			throw GameException(string("Invalid game view pointer supplied."));
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	char const *Pool3dViewEventListener::VGetName(void)
	{
		return ("Pool3dViewEventListener");
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dViewEventListener::VHandleEvent(IEventData const &eventObj)
	{
		bool result = true;

		// NB. Note to self, the event will not be propagated anymore if you return TRUE from this method.
		//		Take care you return FALSE for events that might have script listeners...

		// Log receipt of the event and its type.
        GF_LOG_TRACE_TRC(VGetName(), std::string("Recevied event type = ") + string(eventObj.VGetEventType().getStr()));

		// Check what event has occurred and handle it appropriately.
		if(eventObj.VGetEventType() == EvtData_Destroy_Actor::sk_EventType)
		{
			const EvtData_Destroy_Actor &castEvent = static_cast<const EvtData_Destroy_Actor &>(eventObj);
			result = OnDestroyActorEvent(castEvent);
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
		else if(eventObj.VGetEventType() == EvtData_New_Game::sk_EventType)
		{
			const EvtData_New_Game &castEvent = static_cast<const EvtData_New_Game &>(eventObj);
			result = OnNewGameEvent(castEvent);
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
		else if(eventObj.VGetEventType() == EvtData_Camera_Aim_Mode_Update::sk_EventType)
		{
			const EvtData_Camera_Aim_Mode_Update &castEvent = static_cast<const EvtData_Camera_Aim_Mode_Update &>(eventObj);
			result = OnCameraViewAimUpdate(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Shot_Start::sk_EventType)
		{
			const EvtData_Shot_Start &castEvent = static_cast<const EvtData_Shot_Start &>(eventObj);
			result = OnShotStart(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Shot_End::sk_EventType)
		{
			const EvtData_Shot_End &castEvent = static_cast<const EvtData_Shot_End &>(eventObj);
			result = OnShotEnd(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Loading_Progress::sk_EventType)
		{
			const EvtData_Loading_Progress &castEvent = static_cast<const EvtData_Loading_Progress &>(eventObj);
			result = OnLoadingProgress(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Sound_Config_Change::sk_EventType)
		{
			const EvtData_Sound_Config_Change &castEvent = static_cast<const EvtData_Sound_Config_Change &>(eventObj);
			result = OnSoundConfigChange(castEvent);
		}
		// TODO: Handle other UI view events here...
		else
		{
            GF_LOG_DEB(string("View: Unknown game event received: ") + eventObj.VGetEventType().getStr());
			result = false;
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dViewEventListener::OnGameStateChangeEvent(const EvtData_Game_State &eventData)
	{
		if(g_audioPtr)
		{
			if(eventData.GetState() == BGS_Paused || eventData.GetState() == BGS_DialogPaused)
			{
				g_audioPtr->VPauseAllSounds();
			}
			else if(eventData.GetState() == BGS_Running && m_viewPtr->m_playSoundFx)
			{
				g_audioPtr->VResumeAllSounds();
			}
		}

		if(eventData.GetState() == BGS_MainMenu)
		{
			m_viewPtr->m_sgm.ResetLights();
		}

		m_viewPtr->SetState(eventData.GetState());
		// Allow other listeners to receive event.
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dViewEventListener::OnMoveActorEvent(const EvtData_Move_Actor &eventData)
	{
		// Update the View layers actor position.
		m_viewPtr->MoveActor(eventData.GetActorId(), eventData.GetMovement());
		// Allow logic layer to receieve event too.
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dViewEventListener::OnNewActorEvent(const EvtData_New_Actor &eventData)
	{
		ActorParams *params = const_cast<ActorParams *>(eventData.GetActorParams());

		boost::shared_ptr<ISceneNode> actorNode;

		// TODO: Temporary fix to stop generating a sphere mesh procedurally every time
		//  we create a ball actor as this is extremely slow!!!
		// TODO: To fix this we should generate a sphere mesh/model externally and load it in
		// once and then have each ball actor share it.
		if(params->VGetType() == AT_Ball)
		{
			BallActorParams *ballParams = static_cast<BallActorParams *>(params);
			boost::shared_ptr<IGLBatchBase> batch = m_viewPtr->GetGlobalBallMesh();

			if(batch && ballParams)
			{
				// The balls material for lighting.
				GameHalloran::Material ballMaterial;
				ballMaterial.Reset(ballParams->VGetColor());

				// Initial matrix of the ball.
				Matrix4 mat;
				Point3 pos(ballParams->VGetPos());
				GameHalloran::BuildTranslationMatrix4(mat, pos.GetX(), pos.GetY(), pos.GetZ());

				// Create the appropriate scene node for the ball.
				actorNode.reset(GCC_NEW BallSceneNode(&m_viewPtr->m_sgm, ballParams->VGetId(),\
														ballMaterial,\
														mat,\
														std::string(ballParams->GetTextureName()),\
														std::string(ballParams->GetShaderName()),\
														*ballParams,
														batch));

				if(ballParams->GetBallGroup() == BallActorParams::eCue)
				{
					// Set the initial position of the cue ball.
					m_viewPtr->m_controller->SetSaCueBallPosition(pos);
				}
			}
		}
		else
		{
			// Create the views version of the game actor.
			actorNode = params->VCreateSceneNode();
		}

		// Tell the controller class about the Cue scene node.
		if(params->VGetType() == AT_Cue)
		{
			m_viewPtr->m_controller->SetCueSceneNode(boost::static_pointer_cast<SceneNode>(actorNode));
			if(params->VGetId())
			{
				m_viewPtr->m_cueId = *params->VGetId();
			}
		}
		else if(params->VGetType() == AT_Table)
		{
			if(params->VGetId())
			{
				m_viewPtr->m_tableId = *params->VGetId();
			}
		}

		ActorId id = 0;
		if(params->VGetId().is_initialized())
		{
			id = *(params->VGetId());
		}

		if(!actorNode)
		{
            GF_LOG_TRACE_ERR("Pool3dViewEventListener::OnNewActorEvent()", "Failed to create scene node from parameters");
		}
		else
		{
			// Add actor to the SGM.
			m_viewPtr->m_sgm.AddChild(actorNode, params->VGetId());
			m_viewPtr->NewActor(params->VGetId());
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dViewEventListener::OnDestroyActorEvent(const EvtData_Destroy_Actor &eventData)
	{
		// Remove the actor from the Views SGM.
		if(!m_viewPtr->m_sgm.RemoveChild(eventData.GetActorId()))
		{
			std::string idStr;
			try { idStr = boost::lexical_cast<std::string, ActorId>(eventData.GetActorId()); } catch(...) {}
            GF_LOG_TRACE_ERR("Pool3dViewEventListener::OnDestroyActorEvent()", std::string("Failed to remove any actor from the view with the ID: ") + idStr);
		}

		// Remove the audio source from the OpenAL sound manager, if any.
		if(g_oalAudioPtr)
		{
			g_oalAudioPtr->RemoveActorSource(eventData.GetActorId());
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dViewEventListener::OnNewGameEvent(const EvtData_New_Game &eventData)
	{
		// do nothing for now...

		// What you could do here:

		// 1) Start music playing.
		// 2) Begin panning camera from starting location to the current shot mode location.
		// 3) Fade out UI.
		// 4) Display/Fade in HUD.

		// Ensure audio system is cleared of all previous

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dViewEventListener::OnUpdateActorParametersEvent(const EvtData_UpdateActorParams &eventData)
	{
		// Handles updating an actors parameters from LUA scripts!
		IActorParams *paramsPtr = NULL;

		boost::shared_ptr<ISceneNode> actorNodePtr = m_viewPtr->m_sgm.FindActor(eventData.GetActorId());
		boost::shared_ptr<SceneNode> actorSceneNodePtr;
		if(actorNodePtr)
		{
			// Downcast to type SceneNode to get access to the VGetActorParams() method.
			actorSceneNodePtr = boost::static_pointer_cast<SceneNode>(actorNodePtr);
			if(actorSceneNodePtr)
			{
				paramsPtr = actorSceneNodePtr->VGetActorParams();
			}
		}

		if(!paramsPtr)
		{
			std::string idStr;
			try { idStr = boost::lexical_cast<std::string, ActorId>(eventData.GetActorId()); } catch(...) {}
            GF_LOG_TRACE_ERR("Pool3dViewEventListener::OnUpdateActorParametersEvent()", std::string("Failed to get the parameters for the actor node with the ID: ") + idStr);
		}
		else
		{
			TErrorMessageList errMsgList;
			if(!paramsPtr->VInit(eventData.VGetLuaEventData(), errMsgList))
			{
                GF_LOG_TRACE_ERR("Pool3dViewEventListener::OnUpdateActorParametersEvent()", "Failed to reinitialize the actors SceneNode parameters (list of causes): ");
				for(TErrorMessageList::iterator i = errMsgList.begin(), end = errMsgList.end(); i != end; ++i)
				{
                    GF_LOG_TRACE_ERR("Pool3dViewEventListener::OnUpdateActorParametersEvent()", *i);
				}
			}

			// Reinitialize the actors node.
			if(!actorSceneNodePtr || !actorSceneNodePtr->VOnRestore())
			{
				std::string idStr;
				try { idStr = boost::lexical_cast<std::string, ActorId>(eventData.GetActorId()); } catch(...) {}
                GF_LOG_TRACE_ERR("Pool3dViewEventListener::OnUpdateActorParametersEvent()", std::string("Failed to reinitialize the SceneNode with the ID: ") + idStr);
			}
		}

		// Allow the view to receive the event too.
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dViewEventListener::OnActorCollisionEvent(const EvtData_PhysCollision &eventData)
	{
		// TODO: 
		// Determine what type of actors collided, and play an appropriate sound effect at a volume
		//  level depending on the force of the collision and from a position depending on the location
		//   relative to the cameras current position!

		// Event data to use to determine all this info!
		//eventData.GetActorAId();
		//eventData.GetActorBId;
		//eventData.GetCollisionPointsList();
		//eventData.GetSumFrictionForce();
		//eventData.GetSumNormalForce();

		// Ensure audio system is up and running.
		if(!m_viewPtr->m_playSoundFx || !g_oalAudioPtr || !g_oalAudioPtr->IsInitialized() || !g_oalAudioPtr->HasSoundCard())
		{
			// No sound lib initialized!
			return (false);
		}

		boost::shared_ptr<ISceneNode> actorA = m_viewPtr->m_sgm.FindActor(eventData.GetActorAId());
		boost::shared_ptr<ISceneNode> actorB = m_viewPtr->m_sgm.FindActor(eventData.GetActorBId());

		F32 volume(eventData.GetSumImpulseForce()*1000.0f);
		Clamp<F32>(volume, 0.0f, 1.0f);

		if(actorA && actorB)
		{
			boost::shared_ptr<SceneNode> actorAsn = boost::static_pointer_cast<SceneNode>(actorA);
			boost::shared_ptr<SceneNode> actorBsn = boost::static_pointer_cast<SceneNode>(actorB);

			if(actorAsn && actorBsn)
			{
				IActorParams *actorAParams = actorAsn->VGetActorParams();
				IActorParams *actorBParams = actorBsn->VGetActorParams();

				if(actorAParams && actorBParams)
				{
					ActorType typeA = actorAParams->VGetType();
					ActorType typeB = actorBParams->VGetType();

					// Ball - Ball collision
					if(typeA == AT_Ball && typeB == AT_Ball)
					{
						//I32 aid = *actorAParams->VGetId();
						//I32 bid = *actorBParams->VGetId();
						//std::cout << "ball-ball (" << aid << "-" << bid << ") impulse: " << eventData.GetSumImpulseForce()*1000.0f << std::endl;

						// Get point of collision.
						Point3 posA(actorAsn->GetPosition()), posB(actorBsn->GetPosition());
						Point3 collisionPos;
						GameHalloran::Point3Lerp(posA, posB, 0.5f, collisionPos);

						boost::shared_ptr<OpenALAudioSource> actorASrc = g_oalAudioPtr->FindActorSource(*actorAParams->VGetId());
						if(actorASrc)
						{
							//if(actorASrc->IsPlaying())
							//{
							//	actorASrc->Stop();
							//	actorASrc->DetachBuffers();
							//}

							if(actorASrc->AttachBuffer(m_viewPtr->m_ballCollisionFxBuf))
							{
								//std::cout << "******* volume ********* " << volume << std::endl;
								actorASrc->Play(volume, false);
							}
						}
					}
//					// Ball - Wall collision
//					else if((typeA == AT_Ball && typeB == AT_Table) || (typeA == AT_Table && typeB == AT_Ball))
//					{
//						boost::shared_ptr<OpenALAudioSource> tableSrc = g_oalAudioPtr->FindActorSource(m_viewPtr->m_tableId);
//						if(tableSrc)
//						{
//							if(tableSrc->AttachBuffer(m_viewPtr->m_wallCollisionFxBuf))
//							{
//								//std::cout << "******* volume ********* " << volume << std::endl;
//								tableSrc->Play(volume, false);
//							}
//						}
//					}
				}
			}
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dViewEventListener::OnTriggerAreaLeaveEvent(const EvtData_PhysTrigger_Leave &eventData)
	{
		if(m_viewPtr->m_playSoundFx)
		{
			// Retrieve the table actor parameters.
			boost::shared_ptr<ISceneNode> tableIsnPtr(m_viewPtr->m_sgm.FindActor(m_viewPtr->m_tableId));
			TableActorParams *tableParamsPtr = NULL;
			if(tableIsnPtr)
			{
				boost::shared_ptr<SceneNode> tableSnPtr = boost::static_pointer_cast<SceneNode>(tableIsnPtr);
				if(tableSnPtr)
				{
					IActorParams *paramsPtr = tableSnPtr->VGetActorParams();
					tableParamsPtr = static_cast<TableActorParams *>(paramsPtr);
				}

				if(tableParamsPtr)
				{
					Point3 srcPos;
					bool validTrigger(true);
					const I32 trigId(eventData.GetTriggerId());
					if(trigId == tableParamsPtr->GetBottomLeftPocketTriggerId())
					{
						srcPos = tableParamsPtr->GetBottomLeftPocketTriggerPos();
					}
					else if(trigId == tableParamsPtr->GetBottomRightPocketTriggerId())
					{
						srcPos = tableParamsPtr->GetBottomRightPocketTriggerPos();
					}
					else if(trigId == tableParamsPtr->GetTopLeftPocketTriggerId())
					{
						srcPos = tableParamsPtr->GetTopLeftPocketTriggerPos();
					}
					else if(trigId == tableParamsPtr->GetTopRightPocketTriggerId())
					{
						srcPos = tableParamsPtr->GetTopRightPocketTriggerPos();
					}
					else if(trigId == tableParamsPtr->GetMiddleLeftPocketTriggerId())
					{
						srcPos = tableParamsPtr->GetMiddleLeftPocketTriggerPos();
					}
					else if(trigId == tableParamsPtr->GetMiddleRightPocketTriggerId())
					{
						srcPos = tableParamsPtr->GetMiddleRightPocketTriggerPos();
					}
					else
					{
						validTrigger = false;
					}

					if(validTrigger)
					{
						// Create a source located at the pocket the ball fell into, point it upwards and add an audio cone to give a nice 3D sound
						// over the pocket only.
						// Start the source playing at some volume.
						// Add it to the OpenAL audio manager as a scene audio source (who will automatically remove it once it has finished playing).
						boost::shared_ptr<OpenALAudioSource> ballDropSource(GCC_NEW OpenALAudioSource(srcPos, Vector3(), g_up));
						ballDropSource->AttachBuffer(m_viewPtr->m_ballDropCollisionFxBuf);
						ballDropSource->SetVolume(0.5f);
						ballDropSource->SetAudibleConeAttributes(0.25f, 10.0f, 20.0f);
						ballDropSource->Play(0.5f, false);
						g_oalAudioPtr->AddSceneSource(ballDropSource);
					}
				}
			}
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dViewEventListener::OnCameraViewAimUpdate(const EvtData_Camera_Aim_Mode_Update &eventData)
	{
		m_viewPtr->m_controller->SetViewMode(eventData.GetCameraMode());
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dViewEventListener::OnShotStart(const EvtData_Shot_Start &eventData)
	{
		if(m_viewPtr->m_playSoundFx)
		{
			if(!g_oalAudioPtr || !g_oalAudioPtr->IsInitialized())
			{
				return (false);
			}

			F32 volume(eventData.GetShotPower() * 1000.0f);
			Clamp<F32>(volume, 0.0f, 1.0f);

			boost::shared_ptr<OpenALAudioSource> cueSrc = g_oalAudioPtr->FindActorSource(m_viewPtr->m_cueId);
			if(cueSrc)
			{
				cueSrc->AttachBuffer(m_viewPtr->m_cueCollisionFxBuf);
				//cueSrc->AttachBuffer(m_viewPtr->m_wallCollisionFxBuf);
				cueSrc->Play(volume, false);
			}
		}

		// TODO: maybe a blue chalk particle effect where the cue ball was struck.

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dViewEventListener::OnShotEnd(const EvtData_Shot_End &eventData)
	{
		// Update view layers HUD and movement controller/camera.
		if(eventData.IsCueBallAlive())
		{
			m_viewPtr->m_controller->SetSaCueBallPosition(*eventData.GetFinalCueBallPosition());
			if(eventData.GetNearestBallPosition().is_initialized())
			{
				m_viewPtr->m_controller->SetSaNearestPlayerBallPosition(*eventData.GetNearestBallPosition());
			}
		}
		m_viewPtr->m_controller->SetSaCurrPlayerId(eventData.GetNextPlayerId());
		safeQueEvent(IEventDataPtr(GCC_NEW EvtData_Camera_Aim_Mode_Request(eShotAim)));
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dViewEventListener::OnLoadingProgress(const EvtData_Loading_Progress &eventData)
	{
		// Game loaded event...
		if(eventData.GetId() == POOL_GAME_LOADED_ID && FloatCmp(eventData.GetProgress(), 1.0f))
		{
			//m_viewPtr->m_controller->Reset();
			m_viewPtr->m_controller->ResetGameState();
		}
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dViewEventListener::OnSoundConfigChange(const EvtData_Sound_Config_Change &eventData)
	{
		if(!g_oalAudioPtr || !g_oalAudioPtr->IsInitialized())
		{
			return (false);
		}

		m_viewPtr->m_playMusic = eventData.PlayMusic();
		m_viewPtr->m_playSoundFx = eventData.PlaySoundFx();
		g_oalAudioPtr->SetMasterGain(eventData.GetVolume());

		if(!m_viewPtr->m_playSoundFx)
		{
			// Stop all currently playing sounds from playing.
			g_oalAudioPtr->VStopAllSounds();
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	// ************************* Pool3dView ***************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dView::CreateSceneGraphShaders()
	{
		// TODO: 1) Make this configurable (Shader names and their attribute locations) to make changing shaders easier.

		// TODO: 2) Also make this configurable to use older versions of GLSL to support older GPUs (using GLSL version 3.3 right now).

		// TODO: 3) Make all shader attribute names and locations the same to make swapping between shaders easier and less error prone.

		// Create and add all required shaders to the SGM.
		std::vector<std::string> shaderNameVec;
		std::vector<VSAttributeNameList> shaderAttVec;
		VSAttributeNameList tmpVec;

		// A) Flat shader.
		shaderNameVec.push_back(std::string("shaders") + ZipFile::ZIP_PATH_SEPERATOR + std::string("flat"));
		tmpVec.push_back(std::string("vertexPos"));
		shaderAttVec.push_back(tmpVec);
		tmpVec.clear();

		// B) Environment shader.
		shaderNameVec.push_back(std::string("shaders") + ZipFile::ZIP_PATH_SEPERATOR + std::string("EnvironmentBox"));
		tmpVec.push_back(std::string("vertexPos"));
		shaderAttVec.push_back(tmpVec);
		tmpVec.clear();

		// C) ADS Phong shader.
		shaderNameVec.push_back(std::string("shaders") + ZipFile::ZIP_PATH_SEPERATOR + std::string("ProgrammablePhongAds"));
		tmpVec.push_back(std::string("va_position"));
		tmpVec.push_back(g_ignoreShaderSlot);
		tmpVec.push_back(std::string("va_normal"));
		tmpVec.push_back(std::string("va_texCoords"));
		shaderAttVec.push_back(tmpVec);
		tmpVec.clear();

//		// D) GUI texture shader.
//		shaderNameVec.push_back(std::string("shaders") + ZipFile::ZIP_PATH_SEPERATOR + std::string("GuiTextureColor"));
//		tmpVec.push_back(string("vertexPos"));
//		tmpVec.push_back(g_ignoreShaderSlot);
//		tmpVec.push_back(g_ignoreShaderSlot);
//		tmpVec.push_back(string("texCoords"));
//		shaderAttVec.push_back(tmpVec);
//		tmpVec.clear();

		return (AddShadersToSceneGraphManager(m_sgm, shaderNameVec, shaderAttVec));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dView::CreatePoolBallMesh()
	{
		std::string initialScriptName(g_appPtr->GetLuaDataDir().string() + std::string("/SphereBallDimensions.lua"));
		if(!g_appPtr->GetLuaStateManager()->DoFile(initialScriptName.c_str()))
		{
            GF_LOG_TRACE_ERR("Pool3dView::CreatePoolBallMesh()", string("Failed to execute script ") + initialScriptName);
			return (false);
		}

		LuaPlus::LuaObject poolBallMeshParams = g_appPtr->GetLuaStateManager()->GetGlobalState()->GetGlobal("SphereBallDimensions");
		if(!poolBallMeshParams.IsTable())
		{
            GF_LOG_TRACE_ERR("Pool3dView::CreatePoolBallMesh()", "Could not get global SphereBallDimensions");
			return (false);
		}
		
		BallActorParams param;
		TErrorMessageList errMsg;
		if(!param.VInit(poolBallMeshParams, errMsg))
		{
            GF_LOG_TRACE_ERR("Pool3dView::CreatePoolBallMesh()", "Failed to create global ball mesh params");
			return (false);
		}

		m_commonPoolBallMesh.reset(GCC_NEW GLTriangleBatch());
		if(!m_commonPoolBallMesh)
		{
            GF_LOG_TRACE_ERR("Pool3dView::CreatePoolBallMesh()", "Failed to create common pool ball mesh");
			return (false);
		}

		gltMakeSphere(*m_commonPoolBallMesh.get(), param.GetRadius(), param.GetSlices(), param.GetStacks());

		m_controller->SetSaBallRadius(param.GetRadius());

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dView::InitHud()
	{
		//Vector4 col(0.0f, 0.0f, 0.0f, 0.0f);
		//Point3 pos(0.0f, 1.0f, 0.0f);
		//// Create container and set relative posistion plus a transparent color and no texture.
		//m_hudContainerPtr.reset(GCC_NEW ContainerWidget(Point3(),\
		//												col,\
		//												m_stackManager,\
		//												1.0f,\
		//												1.0f,\
		//												m_sgm.GetShader(std::string("shaders") + ZIP_FILE_SEPERATOR + std::string("flat")),\
		//												m_sgm.GetShader(std::string("shaders") + ZIP_FILE_SEPERATOR + std::string("GuiTextureColor"))));
		//m_hudContainerPtr->VSetPositionType(pos, false);

		// Create and add the child widgets...

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dView::InitSoundOptions()
	{
		LuaPlus::LuaObject optionsTable = g_appPtr->GetLuaStateManager()->GetGlobalState()->GetGlobal("INIT_PLAYER_OPTIONS");

		if(g_oalAudioPtr && optionsTable["MasterVolume"].IsNumber())
		{
			g_oalAudioPtr->SetMasterGain(optionsTable["MasterVolume"].GetNumber());
		}
		if(optionsTable["Music"].IsBoolean())
		{
			m_playMusic = optionsTable["Music"].GetBoolean();
		}
		if(optionsTable["SoundFx"].IsBoolean())
		{
			m_playSoundFx = optionsTable["SoundFx"].GetBoolean();
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Pool3dView::Pool3dView(shared_ptr<GameOptions> optionsPtr, shared_ptr<GameLog> loggerPtr, shared_ptr<WindowManager> screenManPtr, shared_ptr<ModelViewProjStackManager> matStackManager, Frustrum *viewFrustrumPtr) throw (GameException &)
		: HumanView(optionsPtr, loggerPtr, screenManPtr), m_stackManager(matStackManager), m_modelViewStackPtr(), m_projStackPtr(), m_controller(), m_sgm(matStackManager), m_cameraNode(), m_viewFrustrumPtr(viewFrustrumPtr),\
			m_listenerPtr(), m_state(BGS_Initializing), m_commonPoolBallMesh(), m_ddm(eScene), m_ballCollisionFxBuf(), m_wallCollisionFxBuf(), m_cueCollisionFxBuf(), m_ballDropCollisionFxBuf(), m_tableId(0), m_cueId(0),\
				m_playMusic(true), m_playSoundFx(true), m_skyboxNodePtr(), m_hudContainerPtr()
	{
		if(!m_stackManager)
		{
			throw GameException(string("Matrix manager passed to Pool3dView is NULL"));
		}
		if(!m_viewFrustrumPtr)
		{
			throw GameException(string("View Frustrum pointer is NULL"));
		}
		if(!m_stackManager->GetProjectionMatrixStack())
		{
            GF_LOG_TRACE_ERR("Pool3dView()", "The projection matrix stack does not already exist so will create it now!");
			m_projStackPtr.reset(GCC_NEW GLMatrixStack());
			m_stackManager->SetProjectionMatrixStack(m_projStackPtr);
		}
		if(!m_stackManager->GetModelViewMatrixStack())
		{
            GF_LOG_TRACE_ERR("Pool3dView()", "The modelview matrix stack does not already exist so will create it now!");
			m_modelViewStackPtr.reset(GCC_NEW GLMatrixStack());
			m_stackManager->SetModelViewMatrixStack(m_modelViewStackPtr);
		}

		m_modelViewStackPtr = m_stackManager->GetModelViewMatrixStack();
		m_projStackPtr = m_stackManager->GetProjectionMatrixStack();

		m_listenerPtr.reset(GCC_NEW Pool3dViewEventListener(this));

		// Add events to listen for here...
		safeAddListener(m_listenerPtr, EvtData_Destroy_Actor::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_Game_State::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_Move_Actor::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_New_Actor::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_New_Game::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_UpdateActorParams::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_Debug_String::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_PhysCollision::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_PhysTrigger_Leave::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_Camera_Aim_Mode_Update::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_Loading_Progress::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_Shot_End::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_Shot_Start::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_Sound_Config_Change::sk_EventType);

		// Create the special camera scene node and add it to the SGM.
		m_cameraNode.reset(GCC_NEW CameraSceneNode(&m_sgm, m_viewFrustrumPtr));
		m_sgm.SetCamera(m_cameraNode);

#ifdef _DEBUG
		m_cameraNode->SetDebug(true);
#endif

		m_controller.reset(GCC_NEW Pool3dMovementController(m_cameraNode, boost::shared_ptr<SceneNode>()));

		// Set the base classes controller pointers so our Test movement controller will receive input events.
		m_mouseHandler = m_controller.get();
		m_keyboardHandler = m_controller.get();

		// Set all Pool3d control config values here for how player movement will affect the camera.
		m_controller->SetRotationAngle(90.0f);
		m_controller->SetMovementDistance(5.0f);
		m_controller->SetTvTimeout(5.0f);
		m_controller->SetTvFocusPosition(g_originPt);
		m_controller->SetSaNearestPlayerBallPosition(Point3(0.0f, 0.0f, 0.25f));
		m_controller->SetSaTrailDistance(0.5f);
		//m_controller->ResetGameState();

		if(!CreateSceneGraphShaders())
		{
			throw GameException(string("Failed to build the GLSL shaders for the SGM"));
		}

		// Create and add the sky box.
		std::vector<std::string> cubeMapTextureVec;
		cubeMapTextureVec.push_back(std::string("textures") + ZipFile::ZIP_PATH_SEPERATOR + std::string("clouds.tga"));
		cubeMapTextureVec.push_back(std::string("textures") + ZipFile::ZIP_PATH_SEPERATOR + std::string("clouds.tga"));
		cubeMapTextureVec.push_back(std::string("textures") + ZipFile::ZIP_PATH_SEPERATOR + std::string("clouds.tga"));
		cubeMapTextureVec.push_back(std::string("textures") + ZipFile::ZIP_PATH_SEPERATOR + std::string("clouds.tga"));
		cubeMapTextureVec.push_back(std::string("textures") + ZipFile::ZIP_PATH_SEPERATOR + std::string("clouds.tga"));
		cubeMapTextureVec.push_back(std::string("textures") + ZipFile::ZIP_PATH_SEPERATOR + std::string("clouds.tga"));
		m_skyboxNodePtr.reset(GCC_NEW EnvironmentSceneNode(&m_sgm, boost::optional<ActorId>(), g_identityMat, cubeMapTextureVec, std::string("shaders") + ZipFile::ZIP_PATH_SEPERATOR + std::string("EnvironmentBox"), 25.0f));
		m_sgm.AddChild(m_skyboxNodePtr);

		if(!CreatePoolBallMesh())
		{
			throw GameException(std::string("Failed to build pool ball mesh"));
		}

		// Load all required sound FX buffers - note, these may fail to load so these pointers are not guaranteed to be valid later!
		m_ballCollisionFxBuf = GameHalloran::LoadOpenALBufferFromResourceCache(std::string("sounds") + ZipFile::ZIP_PATH_SEPERATOR + std::string("ball_ball_collision_fx.wav"));
		m_wallCollisionFxBuf = GameHalloran::LoadOpenALBufferFromResourceCache(std::string("sounds") + ZipFile::ZIP_PATH_SEPERATOR + std::string("ball_wall_collision_fx.wav"));
		m_cueCollisionFxBuf = GameHalloran::LoadOpenALBufferFromResourceCache(std::string("sounds") + ZipFile::ZIP_PATH_SEPERATOR + std::string("cue_ball_collision_fx.wav"));
		m_ballDropCollisionFxBuf = GameHalloran::LoadOpenALBufferFromResourceCache(std::string("sounds") + ZipFile::ZIP_PATH_SEPERATOR + std::string("ball_drop_fx.ogg"));

		InitSoundOptions();

		InitHud();

		//std::string fontFile(g_appPtr->GetDatatDir().string() + std::string("/freesansbold.ttf"));
		//m_fontPtr.reset(GCC_NEW FTGLTextureFont(fontFile.c_str()));
		//if(m_fontPtr->Error())
		//{
		//	throw GameException(std::string("Failed to load the font: ") + fontFile);
		//}
		//m_fontPtr->FaceSize(100);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Pool3dView::~Pool3dView()
	{
		try
		{
			// Release any created sounds here...
			if(g_oalAudioPtr)
			{
				g_oalAudioPtr->VReleaseAudioBuffer(m_ballCollisionFxBuf);
				g_oalAudioPtr->VReleaseAudioBuffer(m_wallCollisionFxBuf);
				g_oalAudioPtr->VReleaseAudioBuffer(m_cueCollisionFxBuf);
				g_oalAudioPtr->VReleaseAudioBuffer(m_ballDropCollisionFxBuf);
			}

			m_mouseHandler = NULL;
			m_keyboardHandler = NULL;

			// Remove events listened for here.
			safeDelListener(m_listenerPtr, EvtData_Destroy_Actor::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Game_State::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Move_Actor::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_New_Actor::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Destroy_Actor::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_New_Game::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_UpdateActorParams::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Debug_String::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_PhysCollision::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_PhysTrigger_Leave::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Camera_Aim_Mode_Update::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Loading_Progress::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Shot_End::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Shot_Start::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Sound_Config_Change::sk_EventType);
		}
		catch(...)
		{
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dView::VOnRestore()
	{
		return (HumanView::VOnRestore());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dView::VOnRender(const F64 time, const F32 elapsedTime)
	{
		if(m_state == BGS_Running)
		{
            GF_CLEAR_GL_ERROR();
            
			// Clear the color, depth and stencil buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// Save identity matrix.
			m_modelViewStackPtr->PushMatrix();
			{
				// Place and point the camera in the scene.
				m_cameraNode->VSetViewTransform();

				if(IsDrawModeOn(eScene))
				{
					m_sgm.OnRender();
					GF_CHECK_GL_ERROR_TRC("Pool3dView::VOnRender(): ");
				}
				if(IsDrawModeOn(eCulling))
				{
					m_cameraNode->VRender();
					GF_CHECK_GL_ERROR_TRC("Pool3dView::VOnRender(): ");
				}
				if(IsDrawModeOn(ePhysics))
				{
					g_appPtr->GetLogicPtr()->VRenderDiagnostics();
					GF_CHECK_GL_ERROR_TRC("Pool3dView::VOnRender(): ");
				}

				//// TEST CODE FOR FTGL TEXTURE FONT
				//glEnable(GL_TEXTURE_2D);
				//glDisable(GL_DEPTH_TEST);
				//// Set up lighting.
				//F32 light1_ambient[4]  = { 1.0, 1.0, 1.0, 1.0 };
				//F32 light1_diffuse[4]  = { 1.0, 0.9, 0.9, 1.0 };
				//F32 light1_specular[4] = { 1.0, 0.7, 0.7, 1.0 };
				//F32 light1_position[4] = { 400.0, 400.0, 100.0, 1.0 };
				//glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
				//glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
				//glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
				//glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
				//glEnable(GL_LIGHT1);
				//F32 front_emission[4] = { 0.5, 0.4, 0.3, 0.0 };
				//F32 front_ambient[4]  = { 1.0, 1.0, 1.0, 0.0 };
				//F32 front_diffuse[4]  = { 0.95, 0.95, 0.8, 0.0 };
				//F32 front_specular[4] = { 0.8, 0.8, 0.8, 0.0 };
				//glMaterialfv(GL_FRONT, GL_EMISSION, front_emission);
				//glMaterialfv(GL_FRONT, GL_AMBIENT, front_ambient);
				//glMaterialfv(GL_FRONT, GL_DIFFUSE, front_diffuse);
				//glMaterialfv(GL_FRONT, GL_SPECULAR, front_specular);
				//glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
				//glColor4fv(front_diffuse);
				//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
				//glColorMaterial(GL_FRONT, GL_DIFFUSE);
				//glEnable(GL_COLOR_MATERIAL);
				//glEnable(GL_LIGHTING);
				//	glPushMatrix();
				//	//glLoadMatrixf(m_sgm.GetCamera()->VGet()->GetToWorld().GetComponentsConst());
				//	gluLookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
				//	glPushMatrix();
				//	//glTranslatef(0.0f, 0.0f, 0.0f);
				//	// Render font here...
				//	//std::string test("Hello World");
				//	m_fontPtr->Render("Hello World");
				//	glPopMatrix();
				//	glPopMatrix();
				//glEnable(GL_DEPTH_TEST);
			}
			// Restore identity matrix.
			m_modelViewStackPtr->PopMatrix();

			// Render all the screen elements belong to the view.
			HumanView::VOnRender(time, elapsedTime);
		}
		else if(m_state == BGS_Paused)
		{
			// Do no rendering while game is paused.
		}
		else
		{
			// do nothing.
		}
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dView::VOnLostDevice()
	{
		HumanView::VOnLostDevice();
		m_sgm.OnLostDevice();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dView::VOnEvent(GF_Event &eventObj, const F32 elapsedTime)
	{
		static I32 lightIndex = 0;
		static Vector4 offset(g_forward4 * -10.0f);
		bool result = true;

		// Only receieve user input when the game is running.
		if(m_state == BGS_Running)
		{
			// Call base class version first.
			result = HumanView::VOnEvent(eventObj, elapsedTime);

			if(result)
			{
				switch(eventObj.id)
				{
				case GF_JOYSTICK_MOVE_EVENT:
					{
						if(abs(eventObj.joyMove.pos) > 0.1f)
						{
							boost::shared_ptr<Light> l = m_sgm.GetLight(lightIndex);

							if(lightIndex == 0)
							{
								Point3 pos(l->GetPosition());
								if(eventObj.joyMove.axisId == 0)
								{
									pos.SetX(pos.GetX() + eventObj.joyMove.pos * elapsedTime);
								}
								else if(eventObj.joyMove.axisId == 1)
								{
									pos.SetY(pos.GetY() + eventObj.joyMove.pos * elapsedTime);
								}
								else if(eventObj.joyMove.axisId == 2)
								{
									pos.SetZ(pos.GetZ() + eventObj.joyMove.pos * elapsedTime);
								}
								l->SetPosition(pos);
							}
							else if(lightIndex == 1)
							{
								Vector3 dir(l->GetDirection());
								if(eventObj.joyMove.axisId == 0)
								{
									dir.SetX(dir.GetX() + eventObj.joyMove.pos * elapsedTime);
								}
								else if(eventObj.joyMove.axisId == 1)
								{
									dir.SetY(dir.GetY() + eventObj.joyMove.pos * elapsedTime);
								}
								else if(eventObj.joyMove.axisId == 2)
								{
									dir.SetZ(dir.GetZ() + eventObj.joyMove.pos * elapsedTime);
								}
								l->SetDirection(dir);
							}
						}
					}
					break;
				case GF_JOYSTICK_BUTTON_EVENT:
					{
						if(eventObj.joyButton.buttonState == GLFW_RELEASE)
						{
							if(eventObj.joyButton.buttonId == 0)
							{
								if(lightIndex == 0)
								{
									lightIndex = 1;
								}
								else if(lightIndex == 1)
								{
									lightIndex = 0;
								}
							}
							else if(eventObj.joyButton.buttonId == 1)
							{
								boost::shared_ptr<Light> l = m_sgm.GetLight(lightIndex);
								l->ToggleOn();
							}

						}
					}
					break;
				case GF_MOUSE_BUTTON_EVENT:
					{
						if(eventObj.mouseButton.state == GLFW_PRESS)
						{
							//// Print Viewport and GLFW window info.
							//GLint viewport[4];
							//glGetIntegerv(GL_VIEWPORT, viewport);
							//std::cout << "GL_VIEWPORT: " << viewport[0] << ", " << viewport[1] << ", " << viewport[2] << ", " << viewport[3] << std::endl;
							//I32 glfwW, glfwH, glfwX, glfwY;
							//glfwW = g_appPtr->GetWindowManager()->GetWidth();
							//glfwH = g_appPtr->GetWindowManager()->GetHeight();
							//glfwX = -1;
							//glfwY = -1;
							//std::cout << "GLFW WINDOW: " << glfwX << ", " << glfwY << ", " << glfwW << ", " << glfwH << std::endl;

							//// Print Mouse click info.
							//GLdouble realX = static_cast<GLF64>(eventObj.mouseButton.x);
							//GLdouble realY = static_cast<GLdouble>(viewport[3]) - static_cast<GLdouble>(eventObj.mouseButton.y);
							//std::cout << "Mouse Click at: " << realX << ", " << realY << std::endl;

							//// Near Ray Pt and Far Ray Pt.
							//Point3 nearPt, farPt;
							//RayCast *gluRayPtr = NULL, *pickRayPtr = NULL;

							//boost::shared_ptr<ISceneNode> node = m_sgm.FindActor(1);
							//if(node)
							//{
							//	Matrix4 proj, model, view, mv;

							//	m_projStackPtr->GetMatrix(proj);
							//	m_cameraNode->GetGlFrame().GetCameraMatrix(view);
							//	model = node->VGet()->GetToWorld();
							//	mv = view * model;

							//	GLdouble objX, objY, objZ;
							//	GLdouble projDouble[16], mvDouble[16];
							//	for(I32 i = 0; i < 16; ++i)
							//	{
							//		projDouble[i] = static_cast<GLdouble>(proj[i]);
							//		mvDouble[i] = static_cast<GLdouble>(mv[i]);
							//	}

							//	if(gluUnProject(realX, realY, 0.0, mvDouble, projDouble, viewport, &objX, &objY, &objZ) == GL_FALSE)
							//	{
							//		std::cout << "Failed to inverse the screen coordinates (NEAR)." << std::endl;
							//	}
							//	else
							//	{
							//		std::cout << "3D Object Position (Near Plane) at: " << objX << ", " << objY << ", " << objZ << std::endl;
							//		nearPt.Set(static_cast<F32>(objX), static_cast<F32>(objY), static_cast<F32>(objZ));
							//		std::cout << "NearPt: " << nearPt.ToString() << std::endl;
							//		if(gluUnProject(realX, realY, 1.0, mvDouble, projDouble, viewport, &objX, &objY, &objZ) == GL_FALSE)
							//		{
							//			std::cout << "Failed to inverse the screen coordinates (FAR)." << std::endl;
							//		}
							//		else
							//		{
							//			std::cout << "3D Object Position (Far Plane) at: " << objX << ", " << objY << ", " << objZ << std::endl;
							//			farPt.Set(static_cast<F32>(objX), static_cast<F32>(objY), static_cast<F32>(objZ));
							//			std::cout << "FarPt: " << farPt.ToString() << std::endl;

							//			Vector3 rayVector = nearPt - farPt;
							//			//Vector3 rayVector = farPt - nearPt;
							//			rayVector.Normalize();

							//			gluRayPtr = GCC_NEW RayCast(nearPt, rayVector);
							//			std::cout << "GLU Ray (World): " << gluRayPtr->ToString() << std::endl;

							//			if(node->VPick(&m_sgm, *gluRayPtr))
							//			{
							//				std::cout << "GLU Ray = Picked" << std::endl;
							//				BoundingSphere bs;
							//				node->VGet()->GetBoundingSphere(bs);
							//				Point3List ptList;
							//				gluRayPtr->GetRaySphereIntersectionPoints(bs, ptList);
							//				for(Point3List::iterator i = ptList.begin(); i != ptList.end(); ++i)
							//				{
							//					std::cout << "GLU Ray: Intersection point: " << (*i)->ToString() << std::endl;
							//				}
							//			}
							//			else
							//			{
							//				std::cout << "GLU Ray = Not" << std::endl;
							//			}
							//		}
							//	}
							//}

							//// ******** RAYCAST ********
							//{
							//	boost::shared_ptr<ISceneNode> node1 = m_sgm.FindActor(1);
							//	if(node1)
							//	{
							//		Matrix4 proj, view, model, mv, invMv;
							//		m_projStackPtr->GetMatrix(proj);
							//		m_cameraNode->GetGlFrame().GetCameraMatrix(view);
							//		model = node1->VGet()->GetToWorld();
							//		mv = view * model;
							//		mv.Inversed(invMv);

							//		pickRayPtr = GCC_NEW RayCast(realX, realY, viewport[2], viewport[3], proj);
							//		std::cout << "Pick Ray (View): " << pickRayPtr->ToString() << std::endl;

							//		pickRayPtr->Transform(invMv);
							//		std::cout << "Pick Ray (World): " << pickRayPtr->ToString() << std::endl;

							//		if(node1->VPick(&m_sgm, *pickRayPtr))
							//		{
							//			std::cout << "Pick Ray = Picked" << std::endl;
							//			BoundingSphere bs;
							//			node1->VGet()->GetBoundingSphere(bs);
							//			Point3List ptList;
							//			pickRayPtr->GetRaySphereIntersectionPoints(bs, ptList);
							//			for(Point3List::iterator i = ptList.begin(); i != ptList.end(); ++i)
							//			{
							//				std::cout << "Pick Ray: Intersection point: " << (*i)->ToString() << std::endl;
							//			}
							//		}
							//		else
							//		{
							//			std::cout << "Pick Ray = Not" << std::endl;
							//		}
							//	}
							//}
							//Delete(gluRayPtr);
							//Delete(pickRayPtr);
						}
					}
					break;
				case GF_MOUSE_WHEEL_EVENT:
				case GF_MOUSE_MOVE_EVENT:
				break;
				case GF_KEY_EVENT:
					{
						if(eventObj.key.keyState == GLFW_PRESS)
						{
							switch(eventObj.key.keyId)
							{
							case GLFW_KEY_UP:
								{
									//safeQueEvent(IEventDataPtr(GCC_NEW EvtData_ResetCueBall(Point3(0.0f, 0.0f, 0.44f))));
									
									//std::string outputfile(g_appPtr->GetGameRootDir().string() + std::string("/log/luasnapshot.lua"));
									//g_appPtr->GetLuaStateManager()->GetGlobalState()->DumpGlobals(outputfile.c_str(), LuaPlus::LuaState::DUMP_ALPHABETICAL|LuaPlus::LuaState::DUMP_WRITEALL, 10);

									m_controller->Reset();
								}
								break;
							case GLFW_KEY_DOWN:
								{
									Point3 pt(0.0f, 0.0f, 0.44f);
									safeQueEvent(IEventDataPtr(GCC_NEW EvtData_ResetCueBall(pt)));
									m_controller->SetSaCueBallPosition(pt);
								}
								break;
							case 'P':
								{
									// Request the logic layer to pause the game!
 									IEventDataPtr requestPauseEvent(GCC_NEW EvtData_Request_Pause_Game_Event());
									safeQueEvent(requestPauseEvent);
								}
								break;
							case 'H':
								{
									//g_appPtr->GetLuaStateManager()->ExecuteString("displayDialog(\"There was some holy jesus of an error in the scripts\", \"error\", 5.0, 1);");
								}
								break;

							case 'B':
								{
									static I32 counter = 0;
									static Vector3 l(-g_right), r(g_right), f(g_forward), b(-g_forward);
									Vector3 currDir;
									switch(counter)
									{
									case 0: currDir = l; break;
									case 1: currDir = r; break;
									case 2: currDir = f; break;
									case 3: currDir = b; break;
									}
									counter++;
									if(counter > 4) counter = 0;
									g_appPtr->GetLogicPtr()->VGetGamePhysics()->VApplyForce(currDir, 0.001f, 10);
								}
								break;

							case GLFW_KEY_F1:
								{
									// Switch between debug modes...
									if(IsDrawModeOn(eScene) && !IsDrawModeOn(ePhysics))
									{
										TurnOnDrawMode(ePhysics);
										safeTriggerEvent(EvtData_Physics_Diagnostics(true));
									}
									else if(IsDrawModeOn(eScene) && IsDrawModeOn(ePhysics))
									{
										TurnOffDrawMode(eScene);
									}
									else if(IsDrawModeOn(ePhysics) && !IsDrawModeOn(eScene))
									{
										TurnOnDrawMode(eScene);
										TurnOffDrawMode(ePhysics);
										safeTriggerEvent(EvtData_Physics_Diagnostics(false));
									}
								}
								break;

							case GLFW_KEY_F2:
							{
								// Take a screenshot.
								static I32 picIndex = 0;

								++picIndex;

								std::string fn("screenshot"), cv, ext(".tga");
								try { cv = boost::lexical_cast<std::string>(picIndex); } catch(...) {}
								std::string filename(fn + cv + ext);
								GameHalloran::gltGrabScreenTGA(filename.c_str());
								
							}
							break;
                                    
                            case GLFW_KEY_F3:
                            {
                                SceneGraphManager::FogEffectAttributes att(m_sgm.GetFogEffectAttributes());
                                if(att.m_type == SceneGraphManager::FogEffectAttributes::eTYPE_COUNT-1)
                                {
                                    printf("Turning FOG off\n");
                                    m_sgm.EnableFogEffect(SceneGraphManager::FogEffectAttributes::eOff);
                                }
                                else
                                {
                                    printf("Turning FOG %i on\n", att.m_type+1);
                                    m_sgm.EnableFogEffect(att.m_type+1);
                                }
                                break;
                            }
                                    
                            case GLFW_KEY_F8:
                            {
                                if(m_sgm.IsFogOn())
                                {
                                    SceneGraphManager::FogEffectAttributes att(m_sgm.GetFogEffectAttributes());
                                    switch(att.m_type)
                                    {
                                        case SceneGraphManager::FogEffectAttributes::eLinear:
                                        {
                                            att.m_minDistance += 0.1f;
                                            att.m_maxDistance -= 0.1f;
                                            break;
                                        }
                                        case SceneGraphManager::FogEffectAttributes::eExponential:
                                        case SceneGraphManager::FogEffectAttributes::eExponentialByTwo:
                                        {
                                            att.m_density += 0.1f;
                                            break;
                                        }
                                    }
                                    m_sgm.SetFogEffectAttributes(att);
                                }
                                break;
                            }
                                    
                            case GLFW_KEY_F9:
                            {
                                if(m_sgm.IsFogOn())
                                {
                                    SceneGraphManager::FogEffectAttributes att(m_sgm.GetFogEffectAttributes());
                                    switch(att.m_type)
                                    {
                                        case SceneGraphManager::FogEffectAttributes::eLinear:
                                        {
                                            att.m_minDistance -= 0.1f;
                                            att.m_maxDistance += 0.1f;
                                            break;
                                        }
                                        case SceneGraphManager::FogEffectAttributes::eExponential:
                                        case SceneGraphManager::FogEffectAttributes::eExponentialByTwo:
                                        {
                                            att.m_density -= 0.1f;
                                            break;
                                        }
                                    }
                                    m_sgm.SetFogEffectAttributes(att);
                                }
                                break;
                            }

							default:
								break;
							}
						}
						else
						{
						}
					}
					break;
				case GF_KEY_CHARACTER_EVENT:
					break;
				default:
					break;
				}
			}
		}

		return (result);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dView::VOnUpdate(const F32 elapsedTime)
	{
		if(m_state == BGS_Running)
		{
			// Update the scene based on the user input.
			m_controller->Update(elapsedTime);

			// Update the position of the OpenAL listener to be the same position and orientation in world space as the camera.
			if(g_oalAudioPtr)
			{
				// We get the cameras frame of reference matrix here (not the actual matrix which was used for the VIEW transform
				//  as that matrix has its forward and right vectors flipped to move all the elements in the scene appropriately).
				Matrix4 camModel;
				m_cameraNode->GetGlFrame().GetMatrix(camModel);
				Point3 pos;
				Vector4 forward4, up4;
				camModel.GetPosition(pos);
				camModel.GetYDir(up4);
				camModel.GetZDir(forward4);

				Vector3 forward(forward4), up(up4);
				g_oalAudioPtr->Update(pos, forward, up, elapsedTime);

				//static I32 prI32Count = 1;
				//if(printCount % 10 == 0)
				//{
				//	F32 newGain;
				//	Point3 newPos;
				//	Vector3 newVel, newForward, newUp;
				//	g_oalAudioPtr->GetMasterGain(newGain);
				//	g_oalAudioPtr->GetPosition(newPos);
				//	g_oalAudioPtr->GetVelocity(newVel);
				//	g_oalAudioPtr->GetOrientation(newForward, newUp);
				//	std::cout << std::endl << "newPos: " << newPos.ToString() << std::endl;
				//	std::cout << "newGain: " << newGain << std::endl;
				//	std::cout << "newVel: " << newVel.ToString() << std::endl;
				//	std::cout << "newForward: " << newForward.ToString() << std::endl;
				//	std::cout << "newUp: " << newUp.ToString() << std::endl << std::endl;
				//}
				//++printCount;
			}

			HumanView::VOnUpdate(elapsedTime);
			m_sgm.OnUpdate(elapsedTime);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dView::MoveActor(const ActorId actorId, const Matrix4 &movementMat)
	{
		boost::shared_ptr<ISceneNode> actorNode = m_sgm.FindActor(actorId);
		if(actorNode)
		{
			// TODO: This call will calculate the inverse of the movement matrix every time.
			//  This is expensive and might not be needed for every movement so theres room for optimisation here.
			actorNode->VSetTransform(movementMat);

			// If we are using OpenAL...
			if(g_oalAudioPtr)
			{
				// Check if the actor has an audio source registered with the OpenAL system.
				boost::shared_ptr<OpenALAudioSource> nodeAudioSource = g_oalAudioPtr->FindActorSource(actorId);
				if(nodeAudioSource)
				{
					// Get new position.
					Point3 newPos;
					movementMat.GetPosition(newPos);

					// Get new Z/facing direction.
					Vector4 newDir;
					movementMat.GetZDir(newDir);
					Vector3 newDir3(newDir);

					// Update the source (including its velocity).
					nodeAudioSource->Update(newPos, newDir3, static_cast<F32>(g_appPtr->GetCurrTime()));
				}
			}
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dView::NewActor(boost::optional<ActorId> id)
	{
		if(id.is_initialized())
		{
			if(g_oalAudioPtr)
			{
				boost::shared_ptr<ISceneNode> actorNode = m_sgm.FindActor(*id);
				if(actorNode)
				{
					// Add an OpenAL audio source for each game actor.
					Matrix4 actorMat(actorNode->VGet()->GetToWorld());
					Point3 pos;
					Vector4 dir4;
					actorMat.GetPosition(pos);
					actorMat.GetZDir(dir4);
					Vector3 dir(dir4);
					boost::shared_ptr<OpenALAudioSource> actorSource(GCC_NEW OpenALAudioSource(pos, Vector3(), dir));
					g_oalAudioPtr->AddActorSource(actorSource, *id);
				}
			}
		}
	}

}
