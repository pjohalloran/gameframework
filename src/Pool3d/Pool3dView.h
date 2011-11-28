// /////////////////////////////////////////////////////////////////
// @file Pool3dView.h
// @author PJ O Halloran.
// @date 01/11/2010
//
// File contains the header for the Pool3d game view layer class.
//
// /////////////////////////////////////////////////////////////////

#ifndef __POOL_3D_VIEW_H
#define __POOL_3D_VIEW_H

// External Headers
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

// Project Headers
#include "GameBase.h"
#include "HumanView.h"
#include "WindowManager.h"
#include "GameLog.h"
#include "GameOptions.h"
#include "GameException.h"
#include "IGameTimer.h"
#include "OsInputEvents.h"
#include "GLMatrixStack.h"
#include "Frustrum.h"
#include "ModelViewProjStackManager.h"
#include "SceneGraphManager.h"
#include "CameraSceneNode.h"
#include "EventManager.h"
#include "GLTriangleBatch.h"
#include "EnvironmentSceneNode.h"
#include "MyOpenGLUI.h"

#include "OpenAlAudio.h"
#include "OpenAlAudioBuffer.h"
#include "OpenAlAudioSource.h"

#include "Pool3dMovementController.h"
#include "Pool3dEvents.h"
#include "Pool3dActors.h"

// TEST
#include <FTGL/ftgl.h>

namespace GameHalloran
{

	class Pool3dView;

	// /////////////////////////////////////////////////////////////////
	// @class Pool3dViewEventListener
	// @author PJ O Halloran
	//
	// This class holds the functionality for listening for and responding
	// to events for the Pool3D Game view layer.
	//
	// /////////////////////////////////////////////////////////////////
	class Pool3dViewEventListener : public IEventListener
	{
		private:

			Pool3dView *m_viewPtr;							///< Pointer to the UI view layer.

		protected:

		public:
			
			// /////////////////////////////////////////////////////////////////
			// Custom constructor.
			//
			// @param viewPtr Pointer to the games view layer.
			//
			// @throw GameException If the pointer to the game view is invalid.
			//
			// /////////////////////////////////////////////////////////////////
			explicit Pool3dViewEventListener(Pool3dView *viewPtr) throw (GameException &);
			
			// /////////////////////////////////////////////////////////////////
			// Destructor.
			//
			// /////////////////////////////////////////////////////////////////
			virtual	~Pool3dViewEventListener() { };

			// /////////////////////////////////////////////////////////////////
			// Returns ascii-text name for this listener, used mostly for
			// debugging
			//
			// /////////////////////////////////////////////////////////////////
			virtual char const *VGetName(void);

			// /////////////////////////////////////////////////////////////////
			// Return 'false' to indicate that this listener did NOT
			// consume the event, ( and it should continue to be
			// propogated )
			// 					
			// return 'true' to indicate that this listener consumed the
			// event, ( and it should NOT continue to be propgated )
			//
			// /////////////////////////////////////////////////////////////////
			virtual bool VHandleEvent(IEventData const &eventObj);

			// /////////////////////////////////////////////////////////////////
			// Callback triggered when a EvtData_Game_State event
			// is broadcast.
			//
			// @param eventData The event data object.
			//
			// @return bool True if the event is to be stopped now or false if the
			//					event should be propagated further.
			//
			// /////////////////////////////////////////////////////////////////
			bool OnGameStateChangeEvent(const EvtData_Game_State &eventData);

			// /////////////////////////////////////////////////////////////////
			// Callback triggered when a EvtData_Move_Actor event
			// is broadcast.
			//
			// @param eventData The event data object.
			//
			// @return bool True if the event is to be stopped now or false if the
			//					event should be propagated further.
			//
			// /////////////////////////////////////////////////////////////////
			bool OnMoveActorEvent(const EvtData_Move_Actor &eventData);

			// /////////////////////////////////////////////////////////////////
			// Callback triggered when a EvtData_New_Actor event
			// is broadcast.
			//
			// @param eventData The event data object.
			//
			// @return bool True if the event is to be stopped now or false if the
			//					event should be propagated further.
			//
			// /////////////////////////////////////////////////////////////////
			bool OnNewActorEvent(const EvtData_New_Actor &eventData);

			// /////////////////////////////////////////////////////////////////
			// Callback triggered when a EvtData_Destroy_Actor event
			// is broadcast.
			//
			// @param eventData The event data object.
			//
			// @return bool True if the event is to be stopped now or false if the
			//					event should be propagated further.
			//
			// /////////////////////////////////////////////////////////////////
			bool OnDestroyActorEvent(const EvtData_Destroy_Actor &eventData);

			// /////////////////////////////////////////////////////////////////
			// Callback triggered when a EvtData_New_Game event
			// is broadcast.
			//
			// @param eventData The event data object.
			//
			// @return bool True if the event is to be stopped now or false if the
			//					event should be propagated further.
			//
			// /////////////////////////////////////////////////////////////////
			bool OnNewGameEvent(const EvtData_New_Game &eventData);

			// /////////////////////////////////////////////////////////////////
			// Callback triggered when a EvtData_UpdateActorParams event
			// is broadcast.
			//
			// @param eventData The event data object.
			//
			// @return bool True if the event is to be stopped now or false if the
			//					event should be propagated further.
			//
			// /////////////////////////////////////////////////////////////////
			bool OnUpdateActorParametersEvent(const EvtData_UpdateActorParams &eventData);

			// /////////////////////////////////////////////////////////////////
			// Callback triggered when a EvtData_PhysCollision event
			// is broadcast.
			//
			// @param eventData The event data object.
			//
			// @return bool True if the event is to be stopped now or false if the
			//					event should be propagated further.
			//
			// /////////////////////////////////////////////////////////////////
			bool OnActorCollisionEvent(const EvtData_PhysCollision &eventData);

			// /////////////////////////////////////////////////////////////////
			// Callback triggered when a EvtData_PhysTrigger_Leave event
			// is broadcast.
			//
			// @param eventData The event data object.
			//
			// @return bool True if the event is to be stopped now or false if the
			//					event should be propagated further.
			//
			// /////////////////////////////////////////////////////////////////
			bool OnTriggerAreaLeaveEvent(const EvtData_PhysTrigger_Leave &eventData);

			// /////////////////////////////////////////////////////////////////
			// Callback triggered when a EvtData_Camera_Aim_Mode_Update event
			// is broadcast.
			//
			// @param eventData The event data object.
			//
			// @return bool True if the event is to be stopped now or false if the
			//					event should be propagated further.
			//
			// /////////////////////////////////////////////////////////////////
			bool OnCameraViewAimUpdate(const EvtData_Camera_Aim_Mode_Update &eventData);

			// /////////////////////////////////////////////////////////////////
			// Callback triggered when a EvtData_Shot_Start event
			// is broadcast.
			//
			// @param eventData The event data object.
			//
			// @return bool True if the event is to be stopped now or false if the
			//					event should be propagated further.
			//
			// /////////////////////////////////////////////////////////////////
			bool OnShotStart(const EvtData_Shot_Start &eventData);

			// /////////////////////////////////////////////////////////////////
			// Callback triggered when a EvtData_Shot_End event
			// is broadcast.
			//
			// @param eventData The event data object.
			//
			// @return bool True if the event is to be stopped now or false if the
			//					event should be propagated further.
			//
			// /////////////////////////////////////////////////////////////////
			bool OnShotEnd(const EvtData_Shot_End &eventData);

			// /////////////////////////////////////////////////////////////////
			// Callback triggered when a EvtData_Loading_Progress event
			// is broadcast.
			//
			// @param eventData The event data object.
			//
			// @return bool True if the event is to be stopped now or false if the
			//					event should be propagated further.
			//
			// /////////////////////////////////////////////////////////////////
			bool OnLoadingProgress(const EvtData_Loading_Progress &eventData);

			// /////////////////////////////////////////////////////////////////
			// Callback triggered when a EvtData_Sound_Config_Change event
			// is broadcast.
			//
			// @param eventData The event data object.
			//
			// @return bool True if the event is to be stopped now or false if the
			//					event should be propagated further.
			//
			// /////////////////////////////////////////////////////////////////
			bool OnSoundConfigChange(const EvtData_Sound_Config_Change &eventData);

	};

	// /////////////////////////////////////////////////////////////////
	// @class Pool3dView
	// @author PJ O Halloran.
	//
	// Pool3D game view class.
	//
	// /////////////////////////////////////////////////////////////////
	class Pool3dView : public HumanView
	{
		friend class Pool3dViewEventListener;

	public:

		// /////////////////////////////////////////////////////////////////
		// @enum DrawMode
		//
		// List of different draw modes.
		//
		// eNone:			Draw nothing.
		// eScene:			Draw the scene normally.
		// eCulling:		Render the camera frustrum to debug scene culling.
		// ePhysics:		Render physics data to debug physics.
		// eHud:			Render the HUD over the game?
		// eMaxModes:		Maximum draw mode.
		//
		// /////////////////////////////////////////////////////////////////
		enum DrawMode
		{
			eNone = 0,
			eScene = 1,
			eCulling = 2,
			ePhysics = 4,
			eHud = 8,
			eMaxModes
		};

	private:

		boost::shared_ptr<ModelViewProjStackManager> m_stackManager;	///< Application wide matrix stack manager.
		boost::shared_ptr<GLMatrixStack> m_modelViewStackPtr;			///< Pointer to the model view stack.
		boost::shared_ptr<GLMatrixStack> m_projStackPtr;				///< Pointer to the projection stack.
		boost::shared_ptr<Pool3dMovementController> m_controller;		///< The movement/input controller interface for the scene.
		SceneGraphManager m_sgm;										///< The 3D scene graph manager.
		boost::shared_ptr<CameraSceneNode> m_cameraNode;				///< Camera node class.
		Frustrum *m_viewFrustrumPtr;									///< Viewing frustrum pointer.
		boost::shared_ptr<IEventListener> m_listenerPtr;				///< Pointer to the Game views event listener.
		BaseGameState m_state;											///< Current game state (in sync with logic layer).
		boost::shared_ptr<GLTriangleBatch> m_commonPoolBallMesh;		///< The commonly used 3D pool ball mesh for all balls.
		int m_ddm;														///< Debug draw mode.
		
		boost::shared_ptr<OpenALAudioBuffer> m_ballCollisionFxBuf;		///< Sound FX buffer for ball-ball collisions.
		boost::shared_ptr<OpenALAudioBuffer> m_wallCollisionFxBuf;		///< Sound FX buffer for ball-wall collisions.
		boost::shared_ptr<OpenALAudioBuffer> m_cueCollisionFxBuf;		///< Sound FX buffer for cue-ball collisions.
		boost::shared_ptr<OpenALAudioBuffer> m_ballDropCollisionFxBuf;	///< Sound FX buffer for ball pot/drop collisions.
		
		ActorId m_tableId;												///< Pool Table actor ID.
		ActorId m_cueId;												///< Pool Cue actor ID.

		bool m_playMusic;												///< Should music be played?
		bool m_playSoundFx;												///< Should sound fx be played?

		boost::shared_ptr<EnvironmentSceneNode> m_skyboxNodePtr;		///< Pointer to the skybox scene node.

		boost::shared_ptr<ContainerWidget> m_hudContainerPtr;			///< Pointer to the HUD game status overlay widget container.


		//// TEST
		//boost::shared_ptr<FTFont> m_fontPtr;									///< Pointer to the FTGL font.

		// /////////////////////////////////////////////////////////////////
		// Constructor helper function.  Create shaders for the scene graph
		// manager.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool CreateSceneGraphShaders();

		// /////////////////////////////////////////////////////////////////
		// Runs the lua build initial scene script to create the procedularly
		// generated pool ball mesh.
		//
		// /////////////////////////////////////////////////////////////////
		bool CreatePoolBallMesh();

		// /////////////////////////////////////////////////////////////////
		// Create the game screen game status overlay.
		//
		// /////////////////////////////////////////////////////////////////
		bool InitHud();

		// /////////////////////////////////////////////////////////////////
		// Retrieves startup values of the sound options and sets the state
		// of the View accordingly.
		//
		// /////////////////////////////////////////////////////////////////
		void InitSoundOptions();

	protected:


	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param optionsPtr Pointer to the global game options.
		// @param loggerPtr Pointer to the global logger.
		// @param screenManPtr Pointer to the global display manager.
		// @param stackMager Pointer to the global mv/proj matrix stack manager.
		// @param viewFrustrumPtr Pointer to the applications viewing Frustrum.
		//
		// @throw GameException& One of the input parameters is NULL.
		// @throw GameException& We fail to allocate memory for any of the
		//							classes members.
		// @throw GameException& Fail to initialize the audio subsystem.
		// @throw GameException& If we fail to build the required shades for
		//							the game views scene graph manager.
		//
		// /////////////////////////////////////////////////////////////////
		Pool3dView(boost::shared_ptr<GameOptions> optionsPtr, boost::shared_ptr<GameLog> loggerPtr, boost::shared_ptr<WindowManager> screenManPtr, boost::shared_ptr<ModelViewProjStackManager> matStackManager, Frustrum *viewFrustrumPtr) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~Pool3dView();

		// /////////////////////////////////////////////////////////////////
		// Called when the view is restored after an ALT-TAB or other event.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnRestore();
		
		// /////////////////////////////////////////////////////////////////
		// Render the game view.
		//
		// @param time The total running time so far.
		// @param elapsedTime The time elapsed since the last render.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnRender(const F64 time, const F32 elapsedTime);
		
		// /////////////////////////////////////////////////////////////////
		// Called when the view lost after a screen resolution change, 
		// ALT-TAB or other similar event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnLostDevice();

		// /////////////////////////////////////////////////////////////////
		// The message queue event callback.
		//
		// @param eventObj The message queue.
		// @param elapsedTime Number of seconds since the last event poll.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnEvent(GF_Event &eventObj, const F32 elapsedTime);
		
		// /////////////////////////////////////////////////////////////////
		// Called once per the main loop to update the state of the view.
		//
		// @param elapsedTime The number of seconds since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnUpdate(const F32 elapsedTime);

		// /////////////////////////////////////////////////////////////////
		// The current game state.  This is kept in sync with the logic
		// layer.
		//
		// @return BaseGameState The current state of the game as known by the
		//							game view layer.
		//
		// /////////////////////////////////////////////////////////////////
		inline BaseGameState GetState() const
		{
			return (m_state);
		};

		// /////////////////////////////////////////////////////////////////
		// Set the state of the view layer.  This should only be set when the
		// logic layers state changes!
		//
		// @param state The new state of the game.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetState(const BaseGameState state)
		{
			m_state = state;
		};

		// /////////////////////////////////////////////////////////////////
		// Get the current draw mode.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetDrawMode() const { return (m_ddm); };

		// /////////////////////////////////////////////////////////////////
		// Set the current debug mode.
		//
		// /////////////////////////////////////////////////////////////////
		void SetDrawMode(const I32 debugMode) { m_ddm = debugMode; };

		// /////////////////////////////////////////////////////////////////
		// Turn on a specific draw mode.
		//
		// /////////////////////////////////////////////////////////////////
		void TurnOnDrawMode(const enum DrawMode mode) { m_ddm |= mode; };

		// /////////////////////////////////////////////////////////////////
		// Turn off a specific draw mode.
		//
		// /////////////////////////////////////////////////////////////////
		void TurnOffDrawMode(const enum DrawMode mode) { m_ddm = m_ddm & (~mode); };

		// /////////////////////////////////////////////////////////////////
		// Check if a specific draw mode is currently on.
		//
		// /////////////////////////////////////////////////////////////////
		bool IsDrawModeOn(const enum DrawMode mode) const { return ((m_ddm & mode) > 0); };

		// /////////////////////////////////////////////////////////////////
		// Moves an actor to a new orientation and location.
		//
		// @param actorId The ID of the actor that moved.
		// @param movementMat The new orientation and position of the actor.
		//
		// @return N/A
		//
		// /////////////////////////////////////////////////////////////////
		void MoveActor(const ActorId actorId, const Matrix4 &movementMat);

		// /////////////////////////////////////////////////////////////////
		// Get the global created pool ball mesh.
		//
		// @return boost::shared_ptr<IGLBatchBase>
		//
		// /////////////////////////////////////////////////////////////////
		boost::shared_ptr<IGLBatchBase> GetGlobalBallMesh()
		{
			return (boost::static_pointer_cast<IGLBatchBase>(m_commonPoolBallMesh));
		}

		void NewActor(boost::optional<ActorId> id);

	};

}

#endif
