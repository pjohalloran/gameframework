// /////////////////////////////////////////////////////////////////
// @file Pool3dGame.h
// @author PJ O Halloran
// @date 01/11/2010
//
// File contains the header for the Pool3D game application class.
//
// /////////////////////////////////////////////////////////////////

#ifndef __POOL_3D_GAME_H
#define __POOL_3D_GAME_H

// External Headers
#include <stdexcept>

#include <boost/shared_ptr.hpp>

// Project Headers
#include "GameBase.h"
#include "GameException.h"
#include "GameLog.h"
#include "GameMain.h"

#include "Pool3dView.h"
#include "Frustrum.h"

namespace GameHalloran
{

	class Pool3dGame;

	// /////////////////////////////////////////////////////////////////
	// @class Pool3dGameEventListener
	// @author PJ O Halloran
	//
	// Event handler for the Pool3D application class.
	//
	// /////////////////////////////////////////////////////////////////
	class Pool3dGameEventListener : public IEventListener
	{
		private:

			Pool3dGame *m_poolAppPtr;							///< Pointer to the games main application class.

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
			explicit Pool3dGameEventListener(Pool3dGame *poolAppPtr) throw (GameException &) : m_poolAppPtr(poolAppPtr)
			{
				if(m_poolAppPtr == NULL) throw GameException(std::string("m_poolAppPtr is NULL"));
			};
			
			// /////////////////////////////////////////////////////////////////
			// Destructor.
			//
			// /////////////////////////////////////////////////////////////////
			virtual	~Pool3dGameEventListener() { };

			// /////////////////////////////////////////////////////////////////
			// Returns ascii-text name for this listener, used mostly for
			// debugging
			//
			// /////////////////////////////////////////////////////////////////
			virtual char const *VGetName(void) { return ("Pool3dGameEventListener"); };

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
			// Callback triggered when a EvtData_Graphics_Config_Change event
			// is broadcast.
			//
			// @param eventData The event data object.
			//
			// @return bool True if the event is to be stopped now or false if the
			//					event should be propagated further.
			//
			// /////////////////////////////////////////////////////////////////
			bool OnGraphicsConfigChange(const EvtData_Graphics_Config_Change &eventData);

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
	// @class Pool3dGame
	// @author PJ O Halloran
	//
	// The core application class of the Pool3D app.
	//
	// /////////////////////////////////////////////////////////////////
	class Pool3dGame : public GameMain
	{
		private:

			boost::shared_ptr<ModelViewProjStackManager> m_mvProjStackManager;				///< Matrix stack manager.
			Frustrum m_viewFrustrum;														///< The viewing frustrum.
			bool m_isAnisotropicExtPresent;													///< Global flag telling us if the hardware supports the GL_EXT_texture_filter_anisotropic
			GameViewId m_gameId;															///< ID of the game view layer.
			GameViewId m_uiId;																///< ID of the UI view layer.
			boost::shared_ptr<Pool3dGameEventListener> m_listener;							///< Application event listener.
            bool m_eventsRegistered;

			// /////////////////////////////////////////////////////////////////
			// Initliaize the OpenGL drawning library and any extensions required
			// by the application.
			//
			// Please note that the OpenGL context itself has been initialized
			// by the WindowManager when this is called.  All thats left to do 
			// is to check for extensions and perform any general OpenGL setup
			// stuff such as setting the clear color, etc.
			//
			// /////////////////////////////////////////////////////////////////
			virtual bool VInitOpenGL();

			// /////////////////////////////////////////////////////////////////
			// Setup the game logic and game views subsystem.  This should be 
			// implemeted by all derived classes.
			// 
			// @return boost::shared_ptr<BaseGameLogic> The logic layer pointer
			//											on success or NULL on 
			//											failure.
			//
			// /////////////////////////////////////////////////////////////////
			virtual boost::shared_ptr<BaseGameLogic> VCreateLogicAndViews();

			// /////////////////////////////////////////////////////////////////
			// Register the game specific events for the EventManager module.
			//
			// /////////////////////////////////////////////////////////////////
			void RegisterGameSpecificEvents();

		protected:


		public:
			
			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// @param loggerPtr Pointer to the GameLog logging object.
			// @param optionsPtr Pointer to the options for the game.
			//
			// @throw GameException If the pointers to the logger or the screen
			//						manager are invalid.
			//
			// /////////////////////////////////////////////////////////////////
			Pool3dGame(boost::shared_ptr<GameLog> loggerPtr, boost::shared_ptr<GameOptions> optionsPtr) throw (GameException &);
			
			// /////////////////////////////////////////////////////////////////
			// Destructor.
			//
			// /////////////////////////////////////////////////////////////////
			virtual	~Pool3dGame();

			// /////////////////////////////////////////////////////////////////
			// Called when the user resizes the window.
			//
			// This method recalculates the Perspective and Orthographic matrices
			// for the Projection matrix stack.  After it has completed the stack
			// will have the current perspective matrix as the top most element
			// and the orthographic matrix will be the 2nd most element (as it
			// will be used less often).
			//
			// /////////////////////////////////////////////////////////////////
			virtual void VOnResize(const I32 width, const I32 height);

			// /////////////////////////////////////////////////////////////////
			// Check if Anisotropic texture filtering is available for use.
			//
			// /////////////////////////////////////////////////////////////////
			bool IsAnisotropicTexturingAvailable() const { return (m_isAnisotropicExtPresent); };

			// /////////////////////////////////////////////////////////////////
			// Get all the saved game filenames from the save directory.
			//
			// TODO: Implement.
			//
			// /////////////////////////////////////////////////////////////////
			virtual bool VGetLoadFilenames(const boost::filesystem::path &directoryRef, std::vector<boost::filesystem::path> &fileListRef) { return (true); };

	};

}

#endif
