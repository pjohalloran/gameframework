// /////////////////////////////////////////////////////////////////
// @file Pool3dMenuView.cpp
// @author PJ O Halloran.
// @date 01/11/2010
//
// File contains the header for the Pool3d game view layer class.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <string>
#include <iostream>

#include <cstring>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/case_conv.hpp>

// Project Headers
#include "Pool3dMenuView.h"

#include "GameBase.h"
#include "GameMain.h"
#include "CameraSceneNode.h"

#include "TextResource.h"
#include "ZipFile.h"

using boost::optional;
using boost::shared_ptr;
using boost::filesystem::path;

using std::string;

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// ***************** Pool3dMenuViewEventListener *******************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Pool3dMenuViewEventListener::Pool3dMenuViewEventListener(Pool3dMenuView *viewPtr)\
		throw (GameException &) : m_viewPtr(viewPtr)
	{
		if(!m_viewPtr)
		{
			throw GameException(string("Invalid game UI view pointer supplied."));
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	char const *Pool3dMenuViewEventListener::VGetName(void)
	{
		return ("Pool3dMenuViewEventListener");
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuViewEventListener::VHandleEvent(IEventData const &eventObj)
	{
		bool result = true;

		// NB. Note to self, the event will not be propagated anymore if you return TRUE from this method.
		//		Take care you return FALSE for events that might have script listeners...

		// Log receipt of the event and its type.
        GF_LOG_TRACE_DEB(VGetName(), string("Recevied event type = ") + string(eventObj.VGetEventType().getStr()));

		// Check what event has occurred and handle it appropriately.
		if(eventObj.VGetEventType() == EvtData_Button_Action::sk_EventType)
		{
			const EvtData_Button_Action &castEvent = static_cast<const EvtData_Button_Action &>(eventObj);
			result = OnButtonActionEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_List_Button_Action::sk_EventType)
		{
			const EvtData_List_Button_Action &castEvent = static_cast<const EvtData_List_Button_Action &>(eventObj);
			result = OnListButtonActionEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Slider_Action::sk_EventType)
		{
			const EvtData_Slider_Action &castEvent = static_cast<const EvtData_Slider_Action &>(eventObj);
			result = OnSliderActionEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Debug_String::sk_EventType)
		{
			const EvtData_Debug_String &castEvent = static_cast<const EvtData_Debug_String &>(eventObj);
			result = OnDebugStringEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Game_State::sk_EventType)
		{
			const EvtData_Game_State &castEvent = static_cast<const EvtData_Game_State &>(eventObj);
			result = OnGameStateChangeEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_New_Game::sk_EventType)
		{
			const EvtData_New_Game &castEvent = static_cast<const EvtData_New_Game &>(eventObj);
			result = OnNewGameEvent(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Dialog_Open_Request_Event::sk_EventType)
		{
			const EvtData_Dialog_Open_Request_Event &castEvent = static_cast<const EvtData_Dialog_Open_Request_Event &>(eventObj);
			result = OnOpenDialogRequestEvent(castEvent);
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
		else if(eventObj.VGetEventType() == EvtData_Video_Resolution_Change::sk_EventType)
		{
			const EvtData_Video_Resolution_Change &castEvent = static_cast<const EvtData_Video_Resolution_Change &>(eventObj);
			result = OnVideoResolutionChangeEvent(castEvent);
		}
		// TODO: Handle other UI view events here...
		else
		{
            GF_LOG_DEB(string("Unknown game event received: ") + eventObj.VGetEventType().getStr());
			result = false;
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuViewEventListener::OnButtonActionEvent(const EvtData_Button_Action &eventObj)
	{
		m_viewPtr->OnButtonActionEvent(eventObj.GetButtonId(), eventObj.GetButtonEventId(), eventObj.IsChecked());
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuViewEventListener::OnListButtonActionEvent(const EvtData_List_Button_Action &eventObj)
	{
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuViewEventListener::OnSliderActionEvent(const EvtData_Slider_Action &eventObj)
	{
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuViewEventListener::OnDebugStringEvent(const EvtData_Debug_String &eventObj)
	{
		m_viewPtr->OnDebugStringEvent(eventObj.GetType(), eventObj.GetDebugMessage());
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuViewEventListener::OnGameStateChangeEvent(const EvtData_Game_State &eventObj)
	{
		m_viewPtr->SetState(eventObj.GetState());

		if(eventObj.GetState() == BGS_MainMenu)
		{
			// We need to display the Main menu screen now!
			const char *mainMenuScreenId = g_appPtr->GetLuaStateManager()->GetGlobalState()->GetGlobal("mmScreenType").GetString();
			m_viewPtr->SetCurrentScreenType(mainMenuScreenId);
		}

		// Allow other listeners to consume
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuViewEventListener::OnNewGameEvent(const EvtData_New_Game &eventObj)
	{
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuViewEventListener::OnOpenDialogRequestEvent(const EvtData_Dialog_Open_Request_Event &eventData)
	{
		m_viewPtr->OpenDialog(eventData.GetDialogType(), eventData.GetText(), eventData.GetTimeout(), eventData.GetScriptId());
		// Allow other listeners to consume.
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuViewEventListener::OnOpenDialogEvent(const EvtData_Dialog_Open_Event &eventData)
	{
		// Allow other listeners to consume.
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuViewEventListener::OnCloseDialogEvent(const EvtData_Dialog_Close_Event &eventData)
	{
		// Allow other listeners to consume.
		m_viewPtr->CloseDialog(eventData.GetId(), eventData.GetDialogResult(), eventData.GetScriptId());
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuViewEventListener::OnVideoResolutionChangeEvent(const EvtData_Video_Resolution_Change &eventData)
	{
		// Update the state of each widget according to the new resolution relative to the old resolution.
		for(Pool3dMenuView::ScreenMap::iterator i = m_viewPtr->m_screenMap.begin(), end = m_viewPtr->m_screenMap.end(); i != end; ++i)
		{
			((*i).second)->VOnResolutionChange(eventData.GetOldWidth(), eventData.GetOldHeight(), eventData.GetNewWidth(), eventData.GetNewHeight());
		}
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	// ********************** Pool3dMenuView ***************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMenuView::LoadUiScreens()
	{
		//TextResource mmLuaScriptRes(std::string("lua") + ZIP_FILE_SEPERATOR + std::string("mainmenu.lua"));
		//boost::shared_ptr<TextResHandle> mmLuaHandle = boost::static_pointer_cast<TextResHandle>(g_appPtr->GetResourceCache()->GetHandle(&mmLuaScriptRes));
		//if(!mmLuaHandle || !mmLuaHandle->VInitialize())
		//{
		//	SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("Pool3dMenuView::LoadUiScreens()"), std::string("Failed to get mainmenu lua from cache"));
		//	return;
		//}
		//if(!g_appPtr->GetLuaStateManager()->ExecuteString(mmLuaHandle->GetTextBuffer()))
		//{
		//	SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("Pool3dMenuView::LoadUiScreens()"), std::string("Failed to execute mainmenu lua script"));
		//	return;
		//}

		std::string uiScreensScript = g_appPtr->GetLuaDataDir().string() + std::string("/UiScreenList.lua");
		if(!g_appPtr->GetLuaStateManager()->DoFile(uiScreensScript.c_str()))
		{
			return;
		}

		// Retrieve the Create Dialog LUA function object from the list of globals.
		m_createDialogFunctionLuaObj = g_appPtr->GetLuaStateManager()->GetGlobalState()->GetGlobal("dlgCreateDialog");
		if (m_createDialogFunctionLuaObj.IsNil())
		{
            GF_LOG_TRACE_ERR("Pool3dMenuView::LoadUiScreens()", "Failed to find the Create Dialog LUA function object");
		}
		else
		{
			// Make sure it actually *IS* a function.
			if(!m_createDialogFunctionLuaObj.IsFunction())
			{
                GF_LOG_TRACE_ERR("Pool3dMenuView::LoadUiScreens()", "The Create Dialog LUA function object is not a valid LUA function");
			}
		}

		m_destroyDialogFunctionLuaObj = g_appPtr->GetLuaStateManager()->GetGlobalState()->GetGlobal("dlgDestroyDialog");
		if (m_destroyDialogFunctionLuaObj.IsNil())
		{
            GF_LOG_TRACE_ERR("Pool3dMenuView::LoadUiScreens()", "Failed to find the Destroy Dialog LUA function object");
		}
		else
		{
			// Make sure it actually *IS* a function.
			if(!m_destroyDialogFunctionLuaObj.IsFunction())
			{
                GF_LOG_TRACE_ERR("Pool3dMenuView::LoadUiScreens()", "The Destroy Dialog LUA function object is not a valid LUA function");
			}
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<AbstractWidget> Pool3dMenuView::ConstructWidget(LuaPlus::LuaObject &widgetDataTable)
	{
		// Determine the type of widget being requested and check if we can create it.
		if(!widgetDataTable.IsTable())
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Can't add widget as widget data is not a table"), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::ConstructWidget()", "Can't add widget as widget data is not a table");
			return (boost::shared_ptr<AbstractWidget>());
		}
		LuaPlus::LuaObject widgetTypeTable = widgetDataTable.GetByName("Type");
		if(!widgetTypeTable.IsString())
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Can't add widget as widget does not have a concrete widget \"Type\" field defined"), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::ConstructWidget()", "Can't add widget as widget does not have a concrete widget \"Type\" field defined");
			return (boost::shared_ptr<AbstractWidget>());
		}

		// Check the type of widget the scripter is adding and create the relevant widget.
		//  Ugly, but nessecary to make adding widgets to screens via lua scripts possible.
		std::string widgetTypeStr = widgetTypeTable.GetString();
		boost::algorithm::to_lower(widgetTypeStr);

		boost::shared_ptr<AbstractWidget> widgetPtr;			// Widget object to create.

		try
		{
			// TODO: Add new widget types to here...
			if(widgetTypeStr.compare("container") == 0)
			{
				widgetPtr.reset(GCC_NEW ContainerWidget(widgetDataTable, m_stackManager, m_flatShader, m_texShader, HumanView::GetNextElementId()));
			}
			else if(widgetTypeStr.compare("button") == 0)
			{
				widgetPtr.reset(GCC_NEW ButtonControl(widgetDataTable, m_stackManager, m_flatShader, m_texShader, m_fontPtr, HumanView::GetNextElementId()));
			}
			else if(widgetTypeStr.compare("textbox") == 0)
			{
				widgetPtr.reset(GCC_NEW TextBoxControl(widgetDataTable, m_stackManager, m_flatShader, m_texShader, m_fontPtr, HumanView::GetNextElementId()));
			}
			else if(widgetTypeStr.compare("checkbutton") == 0)
			{
				widgetPtr.reset(GCC_NEW CheckButtonControl(widgetDataTable, m_stackManager, m_flatShader, m_texShader, m_fontPtr, HumanView::GetNextElementId()));
			}
			else if(widgetTypeStr.compare("listbutton") == 0)
			{
				widgetPtr.reset(GCC_NEW ListButtonControl(widgetDataTable, m_stackManager, m_flatShader, m_texShader, m_fontPtr, HumanView::GetNextElementId()));
			}
			else if(widgetTypeStr.compare("slider") == 0)
			{
				widgetPtr.reset(GCC_NEW SliderControl(widgetDataTable, m_stackManager, m_flatShader, m_texShader, m_fontPtr, HumanView::GetNextElementId()));
			}
			// default case
			else
			{
				ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Tried to add unknown widget type: ") + widgetTypeStr, 0.0f);
                GF_LOG_TRACE_ERR("Pool3dMenuView::ConstructWidget()", std::string("Tried to add unknown widget type: ") + widgetTypeStr);
				return (boost::shared_ptr<AbstractWidget>());
			}

			if(!widgetPtr)
			{
				ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Failed to create widget.  Out of memory"), 0.0f);
                GF_LOG_TRACE_ERR("Pool3dMenuView::ConstructWidget()", "Failed to create widget.  Out of memory");
				return (boost::shared_ptr<AbstractWidget>());
			}
		}
		catch(GameException &ge)
		{
			// Some error occurred creating container with the data we gave the constructor.
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Failed to create the widget: ") + std::string(ge.what()), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::ConstructWidget()", std::string("Failed to create the widget: ") + std::string(ge.what()));
			return (boost::shared_ptr<AbstractWidget>());	
		}

		return (widgetPtr);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuView::RegisterScreenType(const char *screenType)
	{
		if(!screenType || strlen(screenType) == 0)
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Trying to register NULL or empty value for screen type"), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::RegisterScreenType()", "Trying to register NULL or empty value for screen type");
			return (false);
		}

		ScreenType newType(screenType);

		// This will ensure two equal strings are not registered for screen type IDs.
		//  Also its possible that the hashing algorithm in use could generate equal hashes for different strings.
		//   We take care of this possibility here too.
		bool found = false;
		for(RegScreenTypeList::iterator i = m_registeredScreens.begin(), end = m_registeredScreens.end(); ((!found) && (i != end)); ++i)
		{
			if(*i == newType)
			{
				found = true;
				if((*i).getStr().compare(screenType) != 0)
				{
					// Hash collision, notify scripter with error dialog.
					ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Error (Hash collision): ") + std::string(screenType) + std::string(" and ") + (*i).getStr() + std::string(" generate the same hash value"), 0.0f);
                    GF_LOG_TRACE_ERR("Pool3dMenuView::RegisterScreenType()", std::string("Error (Hash collision): ") + std::string(screenType) + std::string(" and ") + (*i).getStr() + std::string(" generate the same hash value"));
				}
				else
				{
					ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Screen type ") + std::string(screenType) + std::string(" already registered.  Choose a different screen type string"), 0.0f);
                    GF_LOG_TRACE_ERR("Pool3dMenuView::RegisterScreenType()", std::string("Screen type ") + std::string(screenType) + std::string(" already registered.  Choose a different screen type string"));
				}
			}
		}
		if(!found)
		{
			m_registeredScreens.push_back(newType);
		}

		return (!found);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuView::IsRegistered(const char* screenType)
	{
		if(!screenType || strlen(screenType) == 0)
		{
			return (false);
		}

		ScreenType newType(screenType);				// Hashed screentype.
		bool found = false;							// Did we find the screen type in the list of registered screen types?

		for(RegScreenTypeList::iterator i = m_registeredScreens.begin(), end = m_registeredScreens.end(); ((!found) && (i != end)); ++i)
		{
			if(*i == newType)
			{
				found = true;
			}
		}

		return (found);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuView::AddScreen(const char *screenType, LuaPlus::LuaObject containerDataTable)
	{
		if(!IsRegistered(screenType))
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Can't add screen as no screen type registered"), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::AddScreen()", "Can't add screen as no screen type registered");
			return (false);
		}

		ScreenType hashScreenType(screenType);					// Screen type hash key.
		boost::shared_ptr<ContainerWidget> containerPtr;		// Container/Screen to hold the screen we are creating.
		try
		{
			containerPtr.reset(GCC_NEW ContainerWidget(containerDataTable, m_stackManager, m_flatShader, m_texShader, HumanView::GetNextElementId()));
			if(!containerPtr)
			{
				ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Failed to create screen.  Out of memory"), 0.0f);
                GF_LOG_TRACE_ERR("Pool3dMenuView::AddScreen()", "Failed to create screen.  Out of memory");
				return (false);	
			}

			// Warning this will replace the previous screen if any existed.
			m_screenMap[hashScreenType.getHashValue()] = containerPtr;
			if(m_currentScreenType == hashScreenType.getHashValue())
			{
                GF_LOG_TRACE_INF("Pool3dMenuView::AddScreen()", std::string("Warning: Replacing previous screen with the screen type ") + std::string(screenType));
				m_currScreenPtr = containerPtr;
			}
		}
		catch(GameException &ge)
		{
			// Some error occurred creating container with the data we gave the constructor.
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Failed to add screen: ") + std::string(ge.what()), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::AddScreen()", std::string("Failed to add screen: ") + std::string(ge.what()));
			return (false);	
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	U32 Pool3dMenuView::AddWidgetToScreen(const char *screenType, LuaPlus::LuaObject widgetDataTable)
	{
		if(!IsRegistered(screenType))
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Can't add widget as no screen type registered"), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::AddWidgetToScreen()", "Can't add widget as no screen type registered");
			return (0);
		}

		ScreenType hashScreenType(screenType);					// Screen type hash key.

		// Find the container/screen that we must add the widget to.
		ScreenMap::iterator i = m_screenMap.find(hashScreenType.getHashValue());
		if(i == m_screenMap.end())
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Can't add widget a there is no screen added as of yet for the screen type ") + hashScreenType.getStr(), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::AddWidgetToScreen()", std::string("Can't add widget a there is no screen added as of yet for the screen type ") + hashScreenType.getStr());
			return (0);
		}
		boost::shared_ptr<ContainerWidget> containerPtr = i->second;

		// Create the widget.
		boost::shared_ptr<AbstractWidget> widgetPtr(ConstructWidget(widgetDataTable));
		if(!widgetPtr)
		{
			return (0);
		}

		// Add the widget to the container.
		if(!containerPtr->VAddControl(widgetPtr))
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Failed to add the widget to the control"), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::AddWidgetToScreen()", "Failed to add the widget to the control");
			return (0);
		}

		return (static_cast<U32>(widgetPtr->VGetId()));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	LuaPlus::LuaObject Pool3dMenuView::GetControlData(const char *screenType, U32 widgetId)
	{
		if(!IsRegistered(screenType))
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Can't get widget data as no screen type registered"), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::GetControlData()", "Can't get widget data as no screen type registered");
			return (LuaPlus::LuaObject());
		}

		ScreenType hashScreenType(screenType);					// Screen type hash key.

		// Find the container/screen that we must add the widget to.
		ScreenMap::iterator i = m_screenMap.find(hashScreenType.getHashValue());
		if(i == m_screenMap.end())
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Can't get widget data as there is no screen added as of yet for the screen type ") + hashScreenType.getStr(), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::GetControlData()", std::string("Can't get widget data as there is no screen added as of yet for the screen type ") + hashScreenType.getStr());
			return (LuaPlus::LuaObject());
		}
		boost::shared_ptr<ContainerWidget> containerPtr = i->second;

		LuaPlus::LuaObject widgetData;
		if(!containerPtr->VGetChildLuaData(widgetId, widgetData))
		{
			string idStr;
			try { idStr = boost::lexical_cast<std::string, U32>(widgetId); } catch(...) {}
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("There is no widget with the screen ID ") + idStr + std::string(" attached to the screen ") + hashScreenType.getStr(), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::GetControlData()", std::string("There is no widget with the screen ID ") + idStr + std::string(" attached to the screen ") + hashScreenType.getStr());
			return (LuaPlus::LuaObject());
		}

		return (widgetData);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuView::SetControlData(const char *screenType, U32 widgetId, LuaPlus::LuaObject controlData)
	{
		if(!IsRegistered(screenType))
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Can't set widget data as no screen type registered"), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::SetControlData()", "Can't set widget data as no screen type registered");
			return (false);
		}

		ScreenType hashScreenType(screenType);					// Screen type hash key.

		// Find the container/screen that we must add the widget to.
		ScreenMap::iterator i = m_screenMap.find(hashScreenType.getHashValue());
		if(i == m_screenMap.end())
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Can't set widget data as there is no screen added as of yet for the screen type ") + hashScreenType.getStr(), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::SetControlData()", std::string("Can't set widget data as there is no screen added as of yet for the screen type ") + hashScreenType.getStr());
			return (false);
		}
		boost::shared_ptr<ContainerWidget> containerPtr = i->second;

		if(!containerPtr->VSetChildLuaData(widgetId, controlData))
		{
			string idStr;
			try { idStr = boost::lexical_cast<std::string, U32>(widgetId); } catch(...) {}
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("There is no widget with the screen ID ") + idStr + std::string(" attached to the screen ") + hashScreenType.getStr(), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::SetControlData()", std::string("There is no widget with the screen ID ") + idStr + std::string(" attached to the screen ") + hashScreenType.getStr());
			return (false);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuView::SetScreenWidgetVisibility(const char *screenType, U32 widgetId, bool visible)
	{
		if(!IsRegistered(screenType))
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Can't get widget data as no screen type registered"), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::SetScreenWidgetVisibility()", "Can't get widget data as no screen type registered");
			return (false);
		}

		ScreenType hashScreenType(screenType);					// Screen type hash key.

		// Find the container/screen that we must add the widget to.
		ScreenMap::iterator i = m_screenMap.find(hashScreenType.getHashValue());
		if(i == m_screenMap.end())
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Can't set widget visibility as there is no screen added as of yet for the screen type ") + hashScreenType.getStr(), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::SetScreenWidgetVisibility()", std::string("Can't set widget visibility as there is no screen added as of yet for the screen type ") + hashScreenType.getStr());
			return (false);
		}
		boost::shared_ptr<ContainerWidget> containerPtr = i->second;

		if(!containerPtr->SetChildVisibility(widgetId, visible))
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("No widget with that ID is managed by the screen ") + hashScreenType.getStr(), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::SetScreenWidgetVisibility()", std::string("No widget with that ID is managed by the screen ") + hashScreenType.getStr());
			return (false);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuView::ShowPreviousScreen()
	{
		// No previous screen.
		if(m_prevScreenType == 0)
		{
			return (false);
		}

		ScreenMap::iterator i = m_screenMap.find(m_prevScreenType);
		if(i != m_screenMap.end())
		{
			m_currentScreenType = m_prevScreenType;
			m_prevScreenType = 0;
			m_currScreenPtr = i->second;
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	U32 Pool3dMenuView::CreateContainer(LuaPlus::LuaObject containerDataTable)
	{
		boost::shared_ptr<ContainerWidget> subContainerPtr;		// Sub container.

		try
		{
			subContainerPtr.reset(GCC_NEW ContainerWidget(containerDataTable, m_stackManager, m_flatShader, m_texShader, HumanView::GetNextElementId()));
			if(!subContainerPtr)
			{
				ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Failed to create sub container:  Out of memory"), 0.0f);
                GF_LOG_TRACE_ERR("Pool3dMenuView::CreateContainer()", "Failed to create sub container:  Out of memory");
				return (0);	
			}

			m_subContainerList.push_back(subContainerPtr);
		}
		catch(GameException &ge)
		{
			// Some error occurred creating container with the data we gave the constructor.
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Failed to create sub container: ") + std::string(ge.what()), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::CreateContainer()", std::string("Failed to create sub container: ") + std::string(ge.what()));
			return (0);	
		}

		return (static_cast<U32>(subContainerPtr->VGetId()));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	U32 Pool3dMenuView::CreateWidget(LuaPlus::LuaObject widgetDataTable)
	{
		// Create the widget.
		boost::shared_ptr<AbstractWidget> widgetPtr(ConstructWidget(widgetDataTable));
		if(!widgetPtr)
		{
			return (0);
		}

		m_subWidgetList.push_back(widgetPtr);

		return (static_cast<U32>(widgetPtr->VGetId()));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuView::AddContainerToScreen(const char *screenType, U32 subContainerId)
	{
		if(!IsRegistered(screenType))
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Can't add screen as no screen type registered"), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::AddContainerToScreen()", "Can't add screen as no screen type registered");
			return (false);
		}
		if(subContainerId == 0)
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("0 is not a valid screen ID for a UI widget"), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::AddContainerToScreen()", "0 is not a valid screen ID for a UI widget");
			return (false);
		}

		boost::shared_ptr<ContainerWidget> containerPtr;		// Container to hold the sub container we are adding to a screen.
		bool found = false;										// Did we find the sub container in the list of created sub containers?

		for(WidgetList::iterator i = m_subContainerList.begin(); ((!found) && (i != m_subContainerList.end())); )
		{
			if((*i)->VGetId() == subContainerId)
			{
				found = true;

				ScreenType hashScreenType(screenType);					// Screen type hash key.

				ScreenMap::iterator rootIter = m_screenMap.find(hashScreenType.getHashValue());
				if(rootIter == m_screenMap.end())
				{
					ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("The screen ") + hashScreenType.getStr() + std::string(" does not have an root container added yet!"), 0.0f);
                    GF_LOG_TRACE_ERR("Pool3dMenuView::AddContainerToScreen()", std::string("The screen ") + hashScreenType.getStr() + std::string(" does not have an root container added yet!"));
					return (false);
				}

				if(!(rootIter->second)->VAddControl(*i))
				{
					std::string idStr;
					try { idStr = boost::lexical_cast<std::string, U32>(subContainerId); } catch(...) {}
					ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Failed to add the sub container (id: ") + idStr + std::string(") to the screen ") + hashScreenType.getStr(), 0.0f);
                    GF_LOG_TRACE_ERR("Pool3dMenuView::AddContainerToScreen()", std::string("Failed to add the sub container (id: ") + idStr + std::string(") to the screen ") + hashScreenType.getStr());
					return (false);
				}
				m_subContainerList.erase(i++);
			}
			else
			{
				++i;
			}
		}

		if(!found)
		{
			std::string idStr;
			try { idStr = boost::lexical_cast<std::string, U32>(subContainerId); } catch(...) {}
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("No record of any sub container created with the ID: ") + idStr, 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::AddContainerToScreen()", std::string("No record of any sub container created with the ID: ") + idStr);
			return (false);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuView::AddWidgetToContainer(U32 subContainerId, U32 widgetId)
	{
		if((subContainerId == 0) || (widgetId == 0))
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("0 is not a valid screen ID for a UI widget"), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::AddContainerToScreen()", "0 is not a valid screen ID for a UI widget");
			return (false);
		}

		boost::shared_ptr<ContainerWidget> containerPtr;		// Container to hold the sub container we are adding a widget to.
		boost::shared_ptr<AbstractWidget> widgetPtr;			// Widget to add to the sub container.
		bool foundC = false, foundW = false;					// Did we find the sub container/widget in the respective lists?

		for(WidgetList::iterator i = m_subContainerList.begin(); ((!foundC) && (i != m_subContainerList.end())); )
		{
			if((*i)->VGetId() == subContainerId)
			{
				foundC = true;
				containerPtr = boost::static_pointer_cast<ContainerWidget>(*i);
			}
			else
			{
				++i;
			}
		}
		for(WidgetList::iterator i = m_subWidgetList.begin(); ((!foundW) && (i != m_subWidgetList.end())); )
		{
			if((*i)->VGetId() == widgetId)
			{
				foundW = true;
				widgetPtr = boost::static_pointer_cast<AbstractWidget>(*i);
				m_subWidgetList.erase(i++);
			}
			else
			{
				++i;
			}
		}

		if(foundC && foundW)
		{
			if(!containerPtr->VAddControl(widgetPtr))
			{
				ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Failed to add the widget to the control"), 0.0f);
                GF_LOG_TRACE_ERR("Pool3dMenuView::AddContainerToScreen()", "Failed to add the widget to the control");
				return (false);
			}
		}
		else
		{
			std::string idCStr, idWStr, finalStr;
			try
			{
				idCStr = boost::lexical_cast<std::string, U32>(subContainerId);
				idWStr = boost::lexical_cast<std::string, U32>(widgetId);
			}
			catch(...) {}

			if(!foundC && !foundW)
			{
				finalStr = idCStr + std::string(" ") + idWStr;
			}
			else if(!foundC)
			{
				finalStr = idCStr;
			}
			else if(!foundW)
			{
				finalStr = idWStr;
			}

			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("No record of any widget(s) created with the ID(s): ") + finalStr, 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::AddContainerToScreen()", std::string("No record of any widget(s) created with the ID(s): ") + finalStr);
			return (false);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Pool3dMenuView::Pool3dMenuView(shared_ptr<GameOptions> optionsPtr, shared_ptr<GameLog> loggerPtr, shared_ptr<WindowManager> screenManPtr, shared_ptr<ModelViewProjStackManager> matStackManager) throw (GameException &)
		: HumanView(optionsPtr, loggerPtr, screenManPtr), m_stackManager(matStackManager), m_modelViewStackPtr(), m_projStackPtr(), m_screenMap(),\
			m_currScreenPtr(), m_specialDialogScreenPtr(), m_dialogStartTime(0.0), m_dialogTimeout(0.0f), m_dialogLifetime(0.0f), m_dialogScriptId(0),\
				m_currentScreenType(0), m_prevScreenType(0), m_fontPtr(), m_listenerPtr(), m_metaTable(), m_flatShader(), m_texShader(), m_subContainerList(),\
					m_subWidgetList(), m_state(BGS_Initializing), m_createDialogFunctionLuaObj(), m_destroyDialogFunctionLuaObj()
	{
		if(!m_stackManager)
		{
			throw GameException(string("Matrix manager passed to Pool3dMenuView is NULL."));
		}
		if(!m_stackManager->GetProjectionMatrixStack())
		{
            GF_LOG_TRACE_ERR("Pool3dMenuView()", "The projection matrix stack does not already exist so will create it now!");
			m_projStackPtr.reset(GCC_NEW GLMatrixStack());
			m_stackManager->SetProjectionMatrixStack(m_projStackPtr);
		}
		if(!m_stackManager->GetModelViewMatrixStack())
		{
            GF_LOG_TRACE_ERR("Pool3dMenuView()", "The modelview matrix stack does not already exist so will create it now!");
			m_modelViewStackPtr.reset(GCC_NEW GLMatrixStack());
			m_stackManager->SetModelViewMatrixStack(m_modelViewStackPtr);
		}

		m_modelViewStackPtr = m_stackManager->GetModelViewMatrixStack();
		m_projStackPtr = m_stackManager->GetProjectionMatrixStack();

		m_listenerPtr.reset(GCC_NEW Pool3dMenuViewEventListener(this));
		safeAddListener(m_listenerPtr, EvtData_Button_Action::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_Debug_String::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_List_Button_Action::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_Slider_Action::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_Game_State::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_New_Game::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_Dialog_Open_Request_Event::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_Dialog_Open_Event::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_Dialog_Close_Event::sk_EventType);
		safeAddListener(m_listenerPtr, EvtData_Video_Resolution_Change::sk_EventType);

		// Create the FTGL font for the UI widgets.
		std::string fontFile(g_appPtr->GetDatatDir().string() + std::string("/freesansbold.ttf"));
		m_fontPtr.reset(GCC_NEW FTGLBitmapFont(fontFile.c_str()));
		if(m_fontPtr->Error())
		{
			throw GameException(std::string("Failed to load the font: ") + fontFile);
		}
		m_fontPtr->FaceSize(30);

		VSAttributeNameList flatVs, texVs;
		flatVs.push_back(std::string("vertexPos"));
		texVs.push_back(string("vertexPos"));
		texVs.push_back(g_ignoreShaderSlot);
		texVs.push_back(g_ignoreShaderSlot);
		texVs.push_back(string("texCoords"));
		m_flatShader = GameHalloran::BuildShaderFromResourceCache(std::string("shaders") + ZipFile::ZIP_PATH_SEPERATOR + std::string("flat"), flatVs);
		m_texShader = GameHalloran::BuildShaderFromResourceCache(std::string("shaders") + ZipFile::ZIP_PATH_SEPERATOR + std::string("GuiTextureColor"), texVs);
		if(!m_flatShader || !m_texShader)
		{
			throw GameException(std::string("Failed to build UI shaders for View UI manager"));
		}

		// Setup access to certain functions for the LUA scripts...
		m_metaTable = g_appPtr->GetLuaStateManager()->GetGlobalState()->GetGlobals().CreateTable("Pool3dMenuView");
		m_metaTable.SetObject("__index", m_metaTable);

		// Here we register two functions to make them accessible to script.
		m_metaTable.RegisterObjectDirect("RegisterScreenType", (Pool3dMenuView *)0, &Pool3dMenuView::RegisterScreenType);
		m_metaTable.RegisterObjectDirect("IsRegistered", (Pool3dMenuView *)0, &Pool3dMenuView::IsRegistered);
		m_metaTable.RegisterObjectDirect("AddScreen", (Pool3dMenuView *)0, &Pool3dMenuView::AddScreen);
		m_metaTable.RegisterObjectDirect("AddWidgetToScreen", (Pool3dMenuView *)0, &Pool3dMenuView::AddWidgetToScreen);
		m_metaTable.RegisterObjectDirect("GetControlData", (Pool3dMenuView *)0, &Pool3dMenuView::GetControlData);
		m_metaTable.RegisterObjectDirect("SetControlData", (Pool3dMenuView *)0, &Pool3dMenuView::SetControlData);
		m_metaTable.RegisterObjectDirect("SetCurrentScreenType", (Pool3dMenuView *)0, &Pool3dMenuView::SetCurrentScreenType);
		m_metaTable.RegisterObjectDirect("SetInactive", (Pool3dMenuView *)0, &Pool3dMenuView::SetInactive);
		m_metaTable.RegisterObjectDirect("CreateContainer", (Pool3dMenuView *)0, &Pool3dMenuView::CreateContainer);
		m_metaTable.RegisterObjectDirect("CreateWidget", (Pool3dMenuView *)0, &Pool3dMenuView::CreateWidget);
		m_metaTable.RegisterObjectDirect("AddContainerToScreen", (Pool3dMenuView *)0, &Pool3dMenuView::AddContainerToScreen);
		m_metaTable.RegisterObjectDirect("AddWidgetToContainer", (Pool3dMenuView *)0, &Pool3dMenuView::AddWidgetToContainer);
		m_metaTable.RegisterObjectDirect("SetScreenWidgetVisibility", (Pool3dMenuView *)0, &Pool3dMenuView::SetScreenWidgetVisibility);
		m_metaTable.RegisterObjectDirect("ShowPreviousScreen", (Pool3dMenuView *)0, &Pool3dMenuView::ShowPreviousScreen);
		
		LuaPlus::LuaObject menuViewStateManObj = g_appPtr->GetLuaStateManager()->GetGlobalState()->BoxPointer(this);
		menuViewStateManObj.SetMetaTable(m_metaTable);

		// And here we expose the metatable as a named entity.
		g_appPtr->GetLuaStateManager()->GetGlobalState()->GetGlobals().SetObject("Pool3dMenuView", menuViewStateManObj);

		LoadUiScreens();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Pool3dMenuView::~Pool3dMenuView()
	{
		try
		{
			safeDelListener(m_listenerPtr, EvtData_Button_Action::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Debug_String::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_List_Button_Action::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Slider_Action::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Game_State::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_New_Game::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Dialog_Open_Request_Event::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Dialog_Open_Event::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Dialog_Close_Event::sk_EventType);
			safeDelListener(m_listenerPtr, EvtData_Video_Resolution_Change::sk_EventType);
		}
		catch(...)
		{
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuView::VOnRestore()
	{
		bool result = HumanView::VOnRestore();

		if(result && m_currScreenPtr)
		{
			result = m_currScreenPtr->VOnRestore();
		}

		if(result && m_specialDialogScreenPtr)
		{
			result = m_specialDialogScreenPtr->VOnRestore();
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMenuView::VOnRender(const F64 time, const F32 elapsedTime)
	{
		// Check if there is a screen to render or a dialog to render
		if(m_specialDialogScreenPtr || m_currScreenPtr)
		{
            GF_CLEAR_GL_ERROR();
            
			// Note: We clear the GL buffers from the GameView only as the menu view is rendered on top of it!

			// Disable depth testing (and enable alpha blending) for the UI elements as they are all located on the 0.0 xy plane so have no particular depth.
			//  We prevent the z fighting issue this way.
			//  This is also more efficient as it groups all screen objects together which need this GL state rather than turning it on/off multiple times
			//  during the render operation!
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);

			// Save the perspective matrix and pop it off the stack for now as we require the orthographic matrix.
			Matrix4 topProjStackMat;
			m_projStackPtr->GetMatrix(topProjStackMat);
			m_projStackPtr->PopMatrix();
			{
				// Save MV identity matrix.
				GLMatrixStackRaii mvSaveStack(m_modelViewStackPtr);

				// Render the current UI screen.
				if(m_currScreenPtr)
				{
					m_currScreenPtr->VOnRender(time, elapsedTime);
					GF_CHECK_GL_ERROR_TRC("Pool3dMenuView::VOnRender(): ");
				}

				// Render the dialog last so it will be layered on top of any existing screen.
				//  Not very efficient to overwrite the color buffer like this but a UI is a part of the game where efficiency is not a priority.
				//   and dialogs are or should be a rare occurance (lets optimize what really needs it)!
				if(m_specialDialogScreenPtr)
				{
					m_specialDialogScreenPtr->VOnRender(time, elapsedTime);
					GF_CHECK_GL_ERROR_TRC("Pool3dMenuView::VOnRender(): ");
				}
			}
			// Restore the Perspective matrix to the top of the projection stack.
			m_projStackPtr->PushMatrix(topProjStackMat);

			// Reverse OpenGL state changes
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
            
            GF_CHECK_GL_ERROR_TRC("Pool3dMenuView::VOnRender(): ");
		}

		// Render all the screen elements belong to the view.
		HumanView::VOnRender(time, elapsedTime);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMenuView::VOnLostDevice()
	{
		HumanView::VOnLostDevice();
		if(m_currScreenPtr)
		{
			m_currScreenPtr->VOnLostDevice();
		}
		if(m_specialDialogScreenPtr)
		{
			m_specialDialogScreenPtr->VOnLostDevice();
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuView::VOnEvent(GF_Event &eventObj, const F32 elapsedTime)
	{
		// Call base class version first.
		bool result = true;
		
		// Is there a dialog currently open
		if(m_specialDialogScreenPtr)
		{
			// Handle events only for the dialog until it is closed.
			result = m_specialDialogScreenPtr->VOnEvent(eventObj, elapsedTime);
			return (result);
		}

		// Else we will handle input as normal
		result = HumanView::VOnEvent(eventObj, elapsedTime);

		if(m_currScreenPtr)
		{
			result = m_currScreenPtr->VOnEvent(eventObj, elapsedTime);
		}

		if(result)
		{
			switch(eventObj.id)
			{
			case GF_JOYSTICK_MOVE_EVENT:
				{
				}
				break;
			case GF_MOUSE_BUTTON_EVENT:
			case GF_MOUSE_WHEEL_EVENT:
			case GF_MOUSE_MOVE_EVENT:
			break;
			case GF_KEY_EVENT:
				{
					if(eventObj.key.keyState == GLFW_PRESS)
					{
						switch(eventObj.key.keyId)
						{
							//case GLFW_KEY_UP:
							//	{
							//		g_appPtr->GetLuaStateManager()->ExecuteString("opmResetGameOptions();");
							//	}
							//	break;
						default:
							{
							}
							break;
						}
					}
					else
					{
					}
				}
				break;
			case GF_KEY_CHARACTER_EVENT:
			case GF_JOYSTICK_BUTTON_EVENT:
				break;
			default:
				break;
			}
		}

		return (result);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMenuView::VOnUpdate(const F32 elapsedTime)
	{
		HumanView::VOnUpdate(elapsedTime);
		if(m_currScreenPtr)
		{
			m_currScreenPtr->VOnUpdate(elapsedTime);
		}

		if(m_specialDialogScreenPtr)
		{
			// Must we clear the dialog after a time?
			if(m_dialogTimeout > 0.0f)
			{
				// Check if its time to close it automatically yet
				m_dialogLifetime += elapsedTime;
				if(m_dialogLifetime >= m_dialogTimeout)
				{
					// Send out the close dialog event to tell all listeners (script and in code)
					IEventDataPtr closeDialogEventPtr(GCC_NEW EvtData_Dialog_Close_Event(m_specialDialogScreenPtr->VGetId(), EvtData_Dialog_Close_Event::eNone, m_dialogScriptId));
					safeQueEvent(closeDialogEventPtr);
				}
			}

			m_specialDialogScreenPtr->VOnUpdate(elapsedTime);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuView::SetCurrentScreenType(const char *screenType)
	{
		// Handle screen hiding
		if(!screenType || strlen(screenType) == 0)
		{
			m_currentScreenType = 0;
			m_currScreenPtr.reset();
			return (true);
		}

		if(!IsRegistered(screenType))
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("Screen type not registered"), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::SetCurrentScreenType()", "Screen type not registered");
			return (false);
		}
		ScreenType hashedScreenType(screenType);
		ScreenMap::iterator i = m_screenMap.find(hashedScreenType.getHashValue());
		if(i == m_screenMap.end())
		{
			ReportUserMessage(EvtData_Dialog_Open_Request_Event::eError, std::string("No screen added for screen type: ") + hashedScreenType.getStr(), 0.0f);
            GF_LOG_TRACE_ERR("Pool3dMenuView::SetCurrentScreenType()", std::string("No screen added for screen type: ") + hashedScreenType.getStr());
			return (false);
		}

		// Set current screen.
		m_prevScreenType = m_currentScreenType;
		m_currentScreenType = hashedScreenType.getHashValue();
		m_currScreenPtr = i->second;
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMenuView::OnButtonActionEvent(const ScreenElementId buttonId, const U32 eventTypeId, const bool checked)
	{
		std::string idStr = boost::lexical_cast<std::string, ScreenElementId>(buttonId);
		std::string evidStr = boost::lexical_cast<std::string, U32>(eventTypeId);
		std::string checkedStr = boost::lexical_cast<std::string, I32>((checked ? 1 : 0));
        GF_LOG_INF(std::string("OnButtonActionEvent: ") + idStr + std::string(" ") + evidStr + std::string(" ") + checkedStr);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMenuView::OnDebugStringEvent(const EvtData_Debug_String::eDebugStringType type, const std::string &message)
	{
        GF_LOG_INF(std::string("Lua debug message: ") + message);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuView::OpenDialog(const EvtData_Dialog_Open_Request_Event::DialogType type, const std::string &textRef, const F32 timeout, const U32 scriptId)
	{
		// Firstly we only allow one dialog at a time, we log the attempt to popup multiple dialogs at present.
		if(m_specialDialogScreenPtr)
		{
            GF_LOG_TRACE_INF("Pool3dMenuView::OpenDialog()", std::string("The application tried to popup a dialog when one was already open, Text: ") + textRef);
			return (true);
		}

		// Verify parameters are okay.
		if(textRef.empty() || type == EvtData_Dialog_Open_Request_Event::eUnknown)
		{
            GF_LOG_TRACE_ERR("Pool3dMenuView::OpenDialog()", "Invalid Arguments.  Cannot display dialog");
			return (false);
		}

		// Create the dialog using the global LUA function
		LuaPlus::LuaFunction<bool> CreateDialogLuaFunction(m_createDialogFunctionLuaObj);
		bool result = true;

		// Execute the LUA function that creates the dialog and sets up the event handling for it.
		switch(type)
		{
		case EvtData_Dialog_Open_Request_Event::eConfirm:
			{
				result = CreateDialogLuaFunction("confirm", textRef.c_str(), scriptId);
			}
			break;

		case EvtData_Dialog_Open_Request_Event::eError:
			{
				result = CreateDialogLuaFunction("error", textRef.c_str(), scriptId);
			}
			break;

		case EvtData_Dialog_Open_Request_Event::eInfo:
			{
				result = CreateDialogLuaFunction("information", textRef.c_str(), scriptId);
			}
			break;

		default:
			{
				// Can't get in here as dialogs
			}
			break;
		}

		if(!result)
		{
            GF_LOG_TRACE_ERR("Pool3dMenuView::OpenDialog()", "Failed to create and setup the dialog");
			return (false);
		}


		if(!m_subContainerList.empty())
		{
			m_specialDialogScreenPtr = boost::static_pointer_cast<ContainerWidget>(m_subContainerList.back());
			m_subContainerList.pop_back();
		}
		else
		{
            GF_LOG_TRACE_ERR("Pool3dMenuView::OpenDialog()", "The queue of containers is empty.  No dialog created!");
			return (false);
		}

		if(!m_specialDialogScreenPtr)
		{
            GF_LOG_TRACE_ERR("Pool3dMenuView::OpenDialog()", "Failed to create dialog");
			return (false);
		}

		// Set dialog start time and end time
		m_dialogLifetime = 0.0f;
		m_dialogStartTime = g_appPtr->GetCurrTime();
		m_dialogTimeout = timeout;

		m_dialogScriptId = scriptId;

		// Send out the open dialog event
		IEventDataPtr openDialogEventPtr(GCC_NEW EvtData_Dialog_Open_Event(m_specialDialogScreenPtr->VGetId(), m_dialogScriptId));
		if(!safeQueEvent(openDialogEventPtr))
		{
            GF_LOG_TRACE_ERR("Pool3dMenuView::OpenDialog()", "Failed to broadcast the open dialog event");
			return (false);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMenuView::CloseDialog(const U32 id, const EvtData_Dialog_Close_Event::DialogResult result, const U32 scriptId)
	{
		std::string idStr;					// ID string.
		std::string resultStr;				// result of dialog string.

		try { idStr = boost::lexical_cast<std::string, U32>(id); } catch(...) {}
		switch(result)
		{
		case EvtData_Dialog_Close_Event::eOk:
		case EvtData_Dialog_Close_Event::eYes:
			{
				resultStr.assign("ok");
			}
			break;

		case EvtData_Dialog_Close_Event::eCancel:
		case EvtData_Dialog_Close_Event::eNo:
			{
				resultStr.assign("cancel");
			}
			break;

		case EvtData_Dialog_Close_Event::eNone:
			{
				resultStr.assign("none");
			}
			break;
		}
        
        GF_LOG_TRACE_INF("Pool3dMenuView::CloseDialog()", std::string("Dialog ") + idStr + std::string(" has been closed with a result ") + resultStr);

		m_specialDialogScreenPtr.reset();
		m_dialogLifetime = 0.0f;
		m_dialogStartTime = 0.0f;
		m_dialogTimeout = 0.0f;
		m_dialogScriptId = 0;

		// Destroy the dialog using the global LUA function (cleans up resources allocated by the Create Dialog LUA function)
		LuaPlus::LuaFunction<bool> DestroyDialogLuaFunction(m_destroyDialogFunctionLuaObj);
		if(!DestroyDialogLuaFunction())
		{
            GF_LOG_TRACE_ERR("Pool3dMenuView::CloseDialog()", "Failed to call the Destroy Dialog LUA function");
			return (false);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ReportUserMessage(const EvtData_Dialog_Open_Request_Event::DialogType type, const std::string &messageRef, const F32 timeout)
	{
		IEventDataPtr requestDialogEvent(GCC_NEW EvtData_Dialog_Open_Request_Event(type, messageRef, timeout, 0));
		return (safeQueEvent(requestDialogEvent));
	}

}
