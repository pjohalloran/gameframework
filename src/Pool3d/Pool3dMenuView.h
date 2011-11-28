// /////////////////////////////////////////////////////////////////
// @file Pool3dMenuView.h
// @author PJ O Halloran.
// @date 04/11/2010
//
// File contains the header for the Pool3d UI Menu screens view 
// layer class.
//
// /////////////////////////////////////////////////////////////////

#ifndef __POOL_3D_MENU_VIEW_H
#define __POOL_3D_MENU_VIEW_H

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

// External Headers
#include <string>
#include <map>
#include <list>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include <FTGL/ftgl.h>

#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

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
#include "ModelViewProjStackManager.h"
#include "MyOpenGLUI.h"
#include "HashedString.h"
#include "Events.h"
#include "GLSLShader.h"

namespace GameHalloran
{

	class Pool3dMenuView;

	// /////////////////////////////////////////////////////////////////
	// @class Pool3dMenuViewEventListener
	// @author PJ O Halloran
	//
	// This class holds the functionality for listening for and responding
	// to events for the Pool3D UI view layer.
	//
	// /////////////////////////////////////////////////////////////////
	class Pool3dMenuViewEventListener : public IEventListener
	{
		private:

			Pool3dMenuView *m_viewPtr;							///< Pointer to the UI view layer.

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
			explicit Pool3dMenuViewEventListener(Pool3dMenuView *viewPtr) throw (GameException &);
			
			// /////////////////////////////////////////////////////////////////
			// Destructor.
			//
			// /////////////////////////////////////////////////////////////////
			virtual	~Pool3dMenuViewEventListener() { };

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
			// Event handler for the EvtData_Button_Action event.
			//
			// @param eventObj The object containing the event data.
			//
			// @return bool 'true' to indicate that this listener consumed the
			//				event, (and it should NOT continue to be propgated).
			// /////////////////////////////////////////////////////////////////
			bool OnButtonActionEvent(const EvtData_Button_Action &eventObj);

			// /////////////////////////////////////////////////////////////////
			// Event handler for the EvtData_List_Button_Action event.
			//
			// @param eventObj The object containing the event data.
			//
			// @return bool 'true' to indicate that this listener consumed the
			//				event, (and it should NOT continue to be propgated).
			// /////////////////////////////////////////////////////////////////
			bool OnListButtonActionEvent(const EvtData_List_Button_Action &eventObj);

			// /////////////////////////////////////////////////////////////////
			// Event handler for the EvtData_Slider_Action event.
			//
			// @param eventObj The object containing the event data.
			//
			// @return bool 'true' to indicate that this listener consumed the
			//				event, (and it should NOT continue to be propgated).
			// /////////////////////////////////////////////////////////////////
			bool OnSliderActionEvent(const EvtData_Slider_Action &eventObj);

			// /////////////////////////////////////////////////////////////////
			// Event handler for the EvtData_Debug_String event.
			//
			// @param eventObj The object containing the event data.
			//
			// @return bool 'true' to indicate that this listener consumed the
			//				event, (and it should NOT continue to be propgated).
			// /////////////////////////////////////////////////////////////////
			bool OnDebugStringEvent(const EvtData_Debug_String &eventObj);

			// /////////////////////////////////////////////////////////////////
			// Event handler for the EvtData_Game_State event.
			//
			// @param eventObj The object containing the event data.
			//
			// @return bool 'true' to indicate that this listener consumed the
			//				event, (and it should NOT continue to be propgated).
			// /////////////////////////////////////////////////////////////////
			bool OnGameStateChangeEvent(const EvtData_Game_State &eventObj);

			// /////////////////////////////////////////////////////////////////
			// Event handler for the EvtData_New_Game event.
			//
			// @param eventObj The object containing the event data.
			//
			// @return bool 'true' to indicate that this listener consumed the
			//				event, (and it should NOT continue to be propgated).
			// /////////////////////////////////////////////////////////////////
			bool OnNewGameEvent(const EvtData_New_Game &eventObj);

			// /////////////////////////////////////////////////////////////////
			// Event handler for the EvtData_Dialog_Open_Request_Event event.
			//
			// @param eventObj The object containing the event data.
			//
			// @return bool 'true' to indicate that this listener consumed the
			//				event, (and it should NOT continue to be propgated).
			// /////////////////////////////////////////////////////////////////
			bool OnOpenDialogRequestEvent(const EvtData_Dialog_Open_Request_Event &eventData);

			// /////////////////////////////////////////////////////////////////
			// Event handler for the EvtData_Dialog_Open_Event event.
			//
			// @param eventObj The object containing the event data.
			//
			// @return bool 'true' to indicate that this listener consumed the
			//				event, (and it should NOT continue to be propgated).
			// /////////////////////////////////////////////////////////////////
			bool OnOpenDialogEvent(const EvtData_Dialog_Open_Event &eventData);

			// /////////////////////////////////////////////////////////////////
			// Event handler for the EvtData_Dialog_Close_Event event.
			//
			// @param eventObj The object containing the event data.
			//
			// @return bool 'true' to indicate that this listener consumed the
			//				event, (and it should NOT continue to be propgated).
			// /////////////////////////////////////////////////////////////////
			bool OnCloseDialogEvent(const EvtData_Dialog_Close_Event &eventData);

			// /////////////////////////////////////////////////////////////////
			// Event handler for the EvtData_Video_Resolution_Change event.
			//
			// @param eventObj The object containing the event data.
			//
			// @return bool 'true' to indicate that this listener consumed the
			//				event, (and it should NOT continue to be propgated).
			// /////////////////////////////////////////////////////////////////
			bool OnVideoResolutionChangeEvent(const EvtData_Video_Resolution_Change &eventData);

	};

	// /////////////////////////////////////////////////////////////////
	// @class Pool3dMenuView
	// @author PJ O Halloran.
	//
	// Pool3D game view class.
	//
	// /////////////////////////////////////////////////////////////////
	class Pool3dMenuView : public HumanView
	{
		friend class Pool3dMenuViewEventListener;

	private:

		// Typedef for unique screen types registered with the MenuView so we may dynamically add screen types at runtime (from C++ or from lua scripts).
		typedef HashedString ScreenType;

		// List of registered screen type hash strings.
		typedef std::list<ScreenType> RegScreenTypeList;

		// Mapping of containers (i.e. UI screens) to hashed screen types.
		typedef std::map<U64, boost::shared_ptr<ContainerWidget> > ScreenMap;

		boost::shared_ptr<ModelViewProjStackManager> m_stackManager;			///< Application wide matrix stack manager.
		boost::shared_ptr<GLMatrixStack> m_modelViewStackPtr;					///< Pointer to the model view stack.
		boost::shared_ptr<GLMatrixStack> m_projStackPtr;						///< Pointer to the projection stack.
		RegScreenTypeList m_registeredScreens;									///< List of registered screens.
		ScreenMap m_screenMap;													///< Mapping of screen containers to container types/handles.
		U64 m_currentScreenType;										///< Type of the current screen in use (ident of the screen type).
		U64 m_prevScreenType;											///< Tpe of the previous screen in use (ident of the screen type).
		boost::shared_ptr<ContainerWidget> m_currScreenPtr;						///< Pointer to the current screen UI container.
		boost::shared_ptr<ContainerWidget> m_specialDialogScreenPtr;			///< Dialog screen for temporary and rare pop up dialog screens.
		F64 m_dialogStartTime;												///< The time the dialog was created.
		F32 m_dialogTimeout;													///< The max time the dialog is to be displayed for (<= 0.0 means until user closes it).
		F32 m_dialogLifetime;													///< How long in seconds the dialog has been open for.
		U32 m_dialogScriptId;											///< Script side ID of the dialog.
		boost::shared_ptr<FTFont> m_fontPtr;									///< Pointer to the FTGL font.
		boost::shared_ptr<IEventListener> m_listenerPtr;						///< Pointer to the Views event system listener object.
		LuaPlus::LuaObject m_metaTable;											///< Provides external access to member functions for LUA scripts.
		boost::shared_ptr<GLSLShader> m_flatShader;								///< Shader for producing flat shaded triangles for the views UI widgets.
		boost::shared_ptr<GLSLShader> m_texShader;								///< Shader for producing texture mapped shaded triangles for the UI widgets.
		WidgetList m_subContainerList;											///< List of created sub containers to be added to screens.
		WidgetList m_subWidgetList;												///< List of created widgets to be added to sub containers.
		BaseGameState m_state;													///< Current state of the game
		LuaPlus::LuaObject m_createDialogFunctionLuaObj;						///< The global create dialog LUA function object.
		LuaPlus::LuaObject m_destroyDialogFunctionLuaObj;						///< The global destroy dialog LUA function object.

		// /////////////////////////////////////////////////////////////////
		// Load all the minimumly required UI screens via external lua
		// scripts.
		// - Error screen/dialog.
		// - Confirmation screen/dialog.
		//
		// /////////////////////////////////////////////////////////////////
		void LoadUiScreens();

		// /////////////////////////////////////////////////////////////////
		// Construct a widget from the widget lua data supplied.
		//
		// @param widgetDataTable Lua data describing the widget.
		//
		// @return boost::shared_ptr<AbstractWidget> NULL on error or a widget
		//												on success.
		//
		// /////////////////////////////////////////////////////////////////
		boost::shared_ptr<AbstractWidget> ConstructWidget(LuaPlus::LuaObject &widgetDataTable);

		// /////////////////////////////////////////////////////////////////
		/* ***************** Script callable functions ****************** */
		//
		// NB LuaPlus expects parameters in a certain way.  For example it
		// accepts only basic C++ data types plus a few objects it knows of
		// itself (i.e. LuaObject).  It does not accept const parameters,
		// pointers or aliases.  That is why the script callable function 
		// definitions here are not const safe and pass by value!
		// 
		// /////////////////////////////////////////////////////////////////

		// /////////////////////////////////////////////////////////////////
		// Register a new screen type with the UI view manager.
		//
		// @param The screen type string ID.
		//
		// @return bool True if the screen was registered and false otherwise.
		//
		// /////////////////////////////////////////////////////////////////
		bool RegisterScreenType(const char *screenType);

		// /////////////////////////////////////////////////////////////////
		// Check if a screen type string is registered with the UI view
		// manager.
		//
		// /////////////////////////////////////////////////////////////////
		bool IsRegistered(const char* screenType);

		// /////////////////////////////////////////////////////////////////
		// Add a container widget as the root widget for a screen to the 
		// UI view manager.
		//
		// N.B. The screen type must be registered firstly.
		//
		// @param screenType The unique screen ID.
		// @param containerDataTable A LUA table used to create the container.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool AddScreen(const char *screenType, LuaPlus::LuaObject containerDataTable);

		// /////////////////////////////////////////////////////////////////
		// Add a widget to the root container for a screen.
		//
		// This is useful as it gives you the ability to add a widget or
		// control directly to the screen.
		//
		// N.B. The screen type must be registered and added firstly.
		//
		// N.B. Adding a container widget using this call is not useful as
		// there is no way to add child widgets to it.  Instead create the
		// container first and add widgets to it using the CreateContainer
		// API described below.
		//
		// @param screenType The unique screen ID.
		// @param widgetDataTable A LUA table used to create the widget.
		//
		// @return U32 The ID of the widget on success or 0 on
		//						failure.
		//
		// /////////////////////////////////////////////////////////////////
		U32 AddWidgetToScreen(const char *screenType, LuaPlus::LuaObject widgetDataTable);

		// /////////////////////////////////////////////////////////////////
		// Retrieve a controls data.
		//
		// @param screenType The unique screen ID to which the widget is
		//						attached to.
		// @param widgetId The controls ID.
		//
		// @param LuaPlus::LuaObject A table containig the widgets data.
		//
		// /////////////////////////////////////////////////////////////////
		LuaPlus::LuaObject GetControlData(const char *screenType, U32 widgetId);

		// /////////////////////////////////////////////////////////////////
		// Sets a controls data.
		//
		// @param screenType The unique screen ID to which the widget is
		//						attached to.
		// @param widgetId The controls ID.
		// @param controlData The new data of the control.
		//
		// @param bool True/False on success/failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool SetControlData(const char *screenType, U32 widgetId, LuaPlus::LuaObject controlData);

		// /////////////////////////////////////////////////////////////////
		// Show or hide any widget for a screen.
		//
		// @param screenType The screen ID.
		// @param widgetId The ID of the widget to show or hide.
		// @param visible Show or Hide the widget?
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool SetScreenWidgetVisibility(const char *screenType, U32 widgetId, bool visible);

		// /////////////////////////////////////////////////////////////////
		// Show the previously displayed screen.
		//
		// @return bool True if there was a previous screen and false if not.
		//
		// /////////////////////////////////////////////////////////////////
		bool ShowPreviousScreen();

		// /////////////////////////////////////////////////////////////////
		/* ***************** Create Sub Container API ******************* */
		/* ****************** Also callable by script ******************* */
		// /////////////////////////////////////////////////////////////////

		// /////////////////////////////////////////////////////////////////
		// Create a sub container widget for a screen.
		//
		// @param containerDataTable Lua data describing the container.
		//
		// @return U32 0 on error or the contaners screen ID on success.
		//
		// /////////////////////////////////////////////////////////////////
		U32 CreateContainer(LuaPlus::LuaObject containerDataTable);

		// /////////////////////////////////////////////////////////////////
		// Creates a widget.
		//
		// @param widgetDataTable The widget lua data.
		//
		// @return U32 0 on error or the widgets screen ID on success.
		//
		// /////////////////////////////////////////////////////////////////
		U32 CreateWidget(LuaPlus::LuaObject widgetDataTable);

		// /////////////////////////////////////////////////////////////////
		// Adds a container as a child widget to an already created and
		// registered screen.
		//
		// NB.  You must all all widgets to the sub container with
		//		AddWidgetToContainer() before adding the sub container to the
		//		screen.
		//
		// NB.	The screen must already have been registered and created with
		//		the UI view manager for this call to succeed.
		//
		// @param screenType The screen ID to add the sub container to.
		// @param subContainerId The screen ID of the sub container to add.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool AddContainerToScreen(const char *screenType, U32 subContainerId);

		// /////////////////////////////////////////////////////////////////
		// Adds a widget as a child widget to an already created sub 
		// container.
		//
		// NB.	The sub container must already have been created for this
		//		call to succeed with CreateContainer().
		//
		// @param subContainerId The screen ID of the sub container to add
		//							the widget to.
		// @param widgetId The ID of the widget to add.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool AddWidgetToContainer(U32 subContainerId, U32 widgetId);

		// /////////////////////////////////////////////////////////////////
		/* *************** End Script Callable LUA API ****************** */
		// /////////////////////////////////////////////////////////////////

	protected:


	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param optionsPtr Pointer to the global game options.
		// @param loggerPtr Pointer to the global logger.
		// @param screenManPtr Pointer to the global display manager.
		//
		// @throw GameException& One of the input parameters is NULL.
		// @throw GameException& We fail to allocate memory for any of the
		//							classes members.
		// @throw GameException& Fail to initialize the audio subsystem.
		//
		// /////////////////////////////////////////////////////////////////
		Pool3dMenuView(boost::shared_ptr<GameOptions> optionsPtr, boost::shared_ptr<GameLog> loggerPtr, boost::shared_ptr<WindowManager> screenManPtr,\
			boost::shared_ptr<ModelViewProjStackManager> matStackManager) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~Pool3dMenuView();

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
		// Get the type of the current active screen being controlled by this
		// View.
		//
		// /////////////////////////////////////////////////////////////////
		U64 GetCurrentScreenType() const { return (m_currentScreenType); };

		// /////////////////////////////////////////////////////////////////
		// Change the views current screen type.
		//
		// @param screenType The UI screen you wish to be rendered.  A NULL
		//						or empty string will hide the current screen.
		//
		// @return bool True if the screen type was registered with the UI
		//				view manager AND if there is a screen added for the
		//				screen type.  False otherwise.
		//
		// /////////////////////////////////////////////////////////////////
		bool SetCurrentScreenType(const char *screenType);

		// /////////////////////////////////////////////////////////////////
		// Set the views current screen to be NULL to stop rendering
		// the UI screens.
		// 
		// /////////////////////////////////////////////////////////////////
		inline void SetInactive() { SetCurrentScreenType(0); };

		// /////////////////////////////////////////////////////////////////
		// Check if any menu is being displayed right now.
		//
		// /////////////////////////////////////////////////////////////////
		bool IsActive() const { return (m_currentScreenType != 0); };

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
		// TODO: Making this publicly visible is potentially a source of bugs
		//		I want the Pool3dViewEventListener to be the only class with
		//		access to this method.  At the same time i hate the friend C++
		//		mechanism, figure out some other way of allowing this class
		//		access to this method!
		//
		// @param state The new state of the game.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetState(const BaseGameState state)
		{
			m_state = state;
		};

		// /////////////////////////////////////////////////////////////////
		// Callback triggered when a button is pressed.
		//
		// /////////////////////////////////////////////////////////////////
		void OnButtonActionEvent(const ScreenElementId buttonId, const U32 eventTypeId, const bool checked);

		// /////////////////////////////////////////////////////////////////
		// Callback triggered when an external script prints a debug message.
		//
		// /////////////////////////////////////////////////////////////////
		void OnDebugStringEvent(const EvtData_Debug_String::eDebugStringType type, const std::string &message);

		// /////////////////////////////////////////////////////////////////
		// Opens a popup dialog.
		//
		// @param type The dialog type.
		// @param textRef The text to render on the dialog.
		// @param timeout The number of seconds to display the dialog for.
		//					If this value is less than or equal to 0.0 then
		//					the dialog will stay open until the user closes
		//					it.
		// @param scriptId Script side dialog ID.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool OpenDialog(const EvtData_Dialog_Open_Request_Event::DialogType type, const std::string &textRef, const F32 timeout, const U32 scriptId);

		// /////////////////////////////////////////////////////////////////
		// Closes the currently open popup dialog.
		//
		// @param id The screen ID of the dialog.
		// @param result The result of the dialog.
		// @param scriptId Script side dialog ID.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool CloseDialog(const U32 id, const EvtData_Dialog_Close_Event::DialogResult result, const U32 scriptId);

	};

	// /////////////////////////////////////////////////////////////////
	// Reports a message to the user.  This function triggers opening a 
	// popup dialog box to the user.
	//
	// @param type The type of dialog to use.
	// @param The message.
	// @param timeout The length of time the dialog should display for
	//					until automatically disappearing.
	//
	// @return bool True on success and false on failure.
	//
	// /////////////////////////////////////////////////////////////////
	bool ReportUserMessage(const EvtData_Dialog_Open_Request_Event::DialogType type, const std::string &messageRef, const F32 timeout);

}

#ifdef WIN32
#	pragma warning( pop )
#endif

#endif
