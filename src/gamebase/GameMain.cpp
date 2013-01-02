// /////////////////////////////////////////////////////////////////
// @file GameMain.cpp
// @author PJ O Halloran
// @date 21/11/2009
//
// File contains the implementation of the GameMain class.
//
// /////////////////////////////////////////////////////////////////

// External headers
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>

// Project headers
#include "GameBase.h"
#include "GameMain.h"
#include "GlfwGameTimer.h"
#include "SystemCheck.h"
#include "Events.h"
#include "PhysicsEvents.h"

// Namespace Declarations
using std::string;
using std::cerr;
using std::endl;
using std::ostringstream;

using boost::filesystem::path;
using boost::shared_ptr;
using boost::scoped_ptr;
using boost::optional;

// Initialize the global game application pointer.
GameHalloran::GameMain *g_appPtr = NULL;

namespace GameHalloran
{
	
	const F64 GameMain::MAX_FRAMES_PER_SECOND = 60.0;
	//const F64 GameMain::MAX_FRAMES_PER_SECOND = 120.0;
	const F64 GameMain::FRAME_TIME_MS = 1.0 / MAX_FRAMES_PER_SECOND;

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool GameMain::SetGlobalDirectories()
	{
		string gameRoot, data, lua, appData, saveGame;
		if(!GameHalloran::RetrieveAndConvertOption<string>(m_optionsPtr, string("GameRoot"), GameOptions::PROGRAMMER, gameRoot))
		{
            GF_LOG_FAT("Failed to get \"GameRoot\" from options file");
			return (false);
		}
		if(!GameHalloran::RetrieveAndConvertOption<string>(m_optionsPtr, string("GlobalDataDir"), GameOptions::PROGRAMMER, data))
		{
            GF_LOG_FAT("Failed to get \"GlobalDataDir\" from options file");
			return (false);
		}
		if(!GameHalloran::RetrieveAndConvertOption<string>(m_optionsPtr, string("LuaGeneralRoot"), GameOptions::PROGRAMMER, lua))
		{
            GF_LOG_FAT("Failed to get \"LuaGeneralRoot\" from options file");
			return (false);
		}
		if(!GameHalloran::RetrieveAndConvertOption<string>(m_optionsPtr, string("GameName"), GameOptions::PROGRAMMER, appData))
		{
            GF_LOG_FAT("Failed to get \"GameName\" from options file");
			return (false);
		}
#ifndef GF_DEMO_BUILD
		if(!GameHalloran::RetrieveAndConvertOption<string>(m_optionsPtr, string("GameSaveDir"), GameOptions::PROGRAMMER, saveGame))
		{
            GF_LOG_FAT("Failed to get \"GameSaveDir\" from options file");
			return (false);
		}
#endif

		// Create directory names from parameters in the options file.
		string tmp;
		m_gameRootDir.assign(gameRoot.begin(), gameRoot.end());
		tmp.assign(gameRoot + data);
		m_dataDir.assign(tmp.begin(), tmp.end());
		tmp.assign(gameRoot + data + appData);
		m_appDataDir.assign(tmp.begin(), tmp.end());
		tmp.assign(gameRoot + data + lua);
		m_luaCommonDir.assign(tmp.begin(), tmp.end());
#ifndef GF_DEMO_BUILD
		m_saveGameDir.assign(saveGame.begin(), saveGame.end());
#endif

		// Check that they are all valid and existing directories on the filesystem.
		if(!boost::filesystem::is_directory(m_gameRootDir))
		{
            GF_LOG_FAT(m_gameRootDir.string() + string(" is not a valid directory"));
			return (false);
		}
		if(!boost::filesystem::is_directory(m_dataDir))
		{
            GF_LOG_FAT(m_dataDir.string() + string(" is not a valid directory"));
			return (false);
		}
		if(!boost::filesystem::is_directory(m_appDataDir))
		{
            GF_LOG_FAT(m_appDataDir.string() + string(" is not a valid directory"));
			return (false);
		}
		if(!boost::filesystem::is_directory(m_luaCommonDir))
		{
            GF_LOG_FAT(m_luaCommonDir.string() + string(" is not a valid directory"));
			return (false);
		}
#ifndef GF_DEMO_BUILD
		if(!boost::filesystem::is_directory(m_saveGameDir))
		{
            GF_LOG_FAT(m_saveGameDir.string() + string(" is not a valid directory"));
			return (false);
		}
#endif

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameMain::CheckSystemResources()
	{
		SystemCheck systemCheckObj;				// System Check object.
		const I64 MEGABYTE = 1024 * 1024;		// Number of bytes in a MB.
		I64 freeHdSpace = 0;					// Free HD space.
		I64 minSysRam = 0;						// Minimum system RAM.
		I64 minVirtualMemory = 0;				// Minimum V memory.
//		I64 minCpuSpeed = 0;					// Minimum CPU speed.
//		I64 minFreeVideoRam = 0;				// Minimum Video RAM.
//		I32 numJoysticks = 0;					// Number of connected joysticks.

		// Get required HD space from options and check if there is sufficient space.
		if(RetrieveAndConvertOption<I64>(m_optionsPtr, string("MinHdSpace"), GameOptions::PROGRAMMER, freeHdSpace))
		{
			freeHdSpace *= MEGABYTE;
			if(!systemCheckObj.CheckHardDiskSpace(freeHdSpace))
			{
                GF_LOG_ERR("There is not enough free space on the hard disk to run the application");
			}
		}

		// TODO: Implement an accurate measure of CPU speed...
		//// Get required CPU Speed and check if the CPU is fast enough.
		//if(RetrieveAndConvertOption<I64>(m_optionsPtr, string("MinCpuSpeed"), GameOptions::PROGRAMMER, minCpuSpeed))
		//{
		//	if(!systemCheckObj.CheckCpuSpeed(minCpuSpeed))
		//	{
		//		SafeGameLog(m_loggerPtr, GameLog::ERR, string("The CPU is not fast enough to run the application."));
		//	}
		//}

		// Get required system RAM and check if there is sufficient RAM.
		if(RetrieveAndConvertOption<I64>(m_optionsPtr, string("MinSysRam"), GameOptions::PROGRAMMER, minSysRam))
		{
			minSysRam *= MEGABYTE;
			if(!systemCheckObj.CheckPhysicalMemory(minSysRam))
			{
                GF_LOG_ERR("There is not enough system RAM available to run the appplication");
			}
		}

		// Get required virtual memory and check if there is sufficient Virtual memory.
		if(RetrieveAndConvertOption<I64>(m_optionsPtr, string("MinVirtualMemory"), GameOptions::PROGRAMMER, minVirtualMemory))
		{
			minVirtualMemory *= MEGABYTE;
			if(!systemCheckObj.CheckVirtualMemory(minVirtualMemory))
			{
                GF_LOG_ERR("There is not enough virtual memory available to run the application");
			}
		}

		// TODO: Implement a Video RAM check using OpenGL or DirectX...
		//// Get required Video RAM and check if there is enough of it.
		//if(RetrieveAndConvertOption<I64>(m_optionsPtr, string("MinVideoRam"), GameOptions::PROGRAMMER, minFreeVideoRam))
		//{
		//	minFreeVideoRam *= MEGABYTE;
		//	if(!systemCheckObj.CheckVideoMemory(minFreeVideoRam))
		//	{
        //      GF_LOG_ERR("There is not enough video memory to run the application");
		//	}
		//}
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool GameMain::SetUpResourceCache()
	{
		bool result = true;								// Result of method.
		string resFilename;								// Name of the resource file.
		U32 resCacheSize = 0;					// Size of the resource cache.

		// Retrieve the game options for the resource cache.
		if(result && !RetrieveAndConvertOption<U32>(m_optionsPtr, string("ResCacheSize"), GameOptions::PROGRAMMER, resCacheSize))
		{
            GF_LOG_ERR("Failed to get the ResCacheSize option so using a default value instead (5)");
			resCacheSize = 5;
		}
		if(result && !RetrieveAndConvertOption<string>(m_optionsPtr, string("ResFile"), GameOptions::PROGRAMMER, resFilename))
		{
            GF_LOG_ERR("Failed to get the ResFile option");
			result = false;
		}

		// Open the resource file (the memory allocated here will be reclaimed in ResCache::~ResCache()).
		IResourceFile *resContainerPtr;
		if(result)
		{
			path resPath(m_appDataDir);
			resPath.append(resFilename.begin(), resFilename.end());
            GF_LOG_INF(string("Loading resource file: ") + resPath.string());
			resContainerPtr = GCC_NEW ResourceZipFile(resPath);
			if(!resContainerPtr)
			{
                GF_LOG_ERR("Failed to allocate memory for the Resource Container");
				result = false;
			}
		}

		// Initialize the ResCache.
		if(result)
		{
			m_resourceCachePtr.reset(GCC_NEW ResCache(resCacheSize, resContainerPtr, m_loggerPtr));
			if(!m_resourceCachePtr)
			{
                GF_LOG_ERR("Failed to allocate memory for the Resource Cache");
				delete resContainerPtr;
				resContainerPtr = NULL;
				result = false;
			}
		}
		if(result && !m_resourceCachePtr->Init())
		{
            GF_LOG_ERR("Failed to initialize the Resource Cache");
			result = false;
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool GameMain::SetUpScriptManager()
	{
		bool result = true;				// Result of method.

		// Create the LUA state manager.
		m_luaStateManagerPtr.reset(GCC_NEW LuaStateManager());
		if(!m_luaStateManagerPtr)
		{
            GF_LOG_ERR("Failed to allocate memory for the LuaStateManager");
			result = false;
		}

		if(result)
		{
			// Setup access to certain functions for the LUA scripts to some vital global application functions.
			m_metaTable = m_luaStateManagerPtr->GetGlobalState()->GetGlobals().CreateTable("GameMain");
			m_metaTable.SetObject("__index", m_metaTable);

			// Here we register two functions to make them accessible to script (we allow scripts to shut down the application).
			m_metaTable.RegisterObjectDirect("IsRunning", (GameMain *)0, &GameMain::IsRunning);
			m_metaTable.RegisterObjectDirect("SetRunning", (GameMain *)0, &GameMain::SetRunning);
			
			LuaPlus::LuaObject gmStateManObj = g_appPtr->GetLuaStateManager()->GetGlobalState()->BoxPointer(this);
			gmStateManObj.SetMetaTable(m_metaTable);

			// And here we expose the metatable as a named entity.
			m_luaStateManagerPtr->GetGlobalState()->GetGlobals().SetObject("GameMain", gmStateManObj);
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool GameMain::SetUpEventManager()
	{
		bool result = true;				// Result of method.

		// Create the EventManager.
		m_eventManagerPtr.reset(GCC_NEW EventManager("GameEventManager", true));
		if(!m_eventManagerPtr)
		{
            GF_LOG_ERR("Failed to allocate memory for the EventManager");
			result = false;
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GameMain::RegisterBaseGameEvents()
	{
		if(!m_eventManagerPtr)
		{
            GF_LOG_FAT("Cannot log core game events until the event manager has been created");
			return (false);
		}

		// Physics events
		m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_PhysTrigger_Enter::sk_EventType);
		m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_PhysTrigger_Leave::sk_EventType);
		m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_PhysCollision::sk_EventType);
		m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_PhysSeparation::sk_EventType);
		m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_Physics_Diagnostics::sk_EventType);

		// Actor events
		m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_New_Actor::sk_EventType);
		m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_Destroy_Actor::sk_EventType);
		m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_Move_Actor::sk_EventType);
		m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_Move_Kinematic_Actor::sk_EventType);
		m_eventManagerPtr->RegisterEvent<EvtData_Request_New_Actor>(EvtData_Request_New_Actor::sk_EventType);
		m_eventManagerPtr->RegisterEvent<EvtData_UpdateActorParams>(EvtData_UpdateActorParams::sk_EventType);

		// TODO: Network events
		//m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_Remote_Client::sk_EventType);
		//m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_Network_Player_Actor_Assignment::sk_EventType);
		
		// TODO: AI events
		//m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_AiSteer::sk_EventType);

		// General game events
		m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_Video_Resolution_Change::sk_EventType);
		m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_Update_Tick::sk_EventType);
		m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_Debug_String::sk_EventType);
		m_eventManagerPtr->RegisterCodeOnlyEvent(EvtData_Game_State::sk_EventType);
		m_eventManagerPtr->RegisterEvent<EvtData_Pause_Game_Event>(EvtData_Pause_Game_Event::sk_EventType);
		m_eventManagerPtr->RegisterEvent<EvtData_Request_Pause_Game_Event>(EvtData_Request_Pause_Game_Event::sk_EventType);
		m_eventManagerPtr->RegisterEvent<EvtData_Request_Start_Game>(EvtData_Request_Start_Game::sk_EventType);
		m_eventManagerPtr->RegisterEvent<EvtData_New_Game>(EvtData_New_Game::sk_EventType);
		m_eventManagerPtr->RegisterEvent<EvtData_End_Game>(EvtData_End_Game::sk_EventType);

		// UI events (some of these events can be triggered from scripts too so are set up appropriately).
		m_eventManagerPtr->RegisterEvent<EvtData_Button_Action>(EvtData_Button_Action::sk_EventType);
		m_eventManagerPtr->RegisterEvent<EvtData_List_Button_Action>(EvtData_List_Button_Action::sk_EventType);
		m_eventManagerPtr->RegisterEvent<EvtData_Slider_Action>(EvtData_Slider_Action::sk_EventType);
		m_eventManagerPtr->RegisterEvent<EvtData_Dialog_Open_Request_Event>(EvtData_Dialog_Open_Request_Event::sk_EventType);
		m_eventManagerPtr->RegisterEvent<EvtData_Dialog_Open_Event>(EvtData_Dialog_Open_Event::sk_EventType);
		m_eventManagerPtr->RegisterEvent<EvtData_Dialog_Close_Event>(EvtData_Dialog_Close_Event::sk_EventType);

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool GameMain::SetUpWindowManager()
	{
		bool result = true;											// Result of method.
		I32 screenWidth = 0;										// The width of the window.
		I32 screenHeight = 0;										// The height of the window.
		string gameName;											// The name of the game/window title.
		I32 fullscreen = 1;											// Should the game run in fullscreen mode?
		I32 multisampling = 0;										// Number of multisamples.
		I32 glMajor = 3, glMinor = 3;								// GL version.
		I32 cbSize = 0, dbSize = 0, sbSize = 0, alphaBits = 0;		// Buffer sizes.
		I32 profile = 0;											// GL compatibility mode.
		bool useDesktopSettings(true);								// Should we use desktop settings?
		bool glDebugContext(false);									// Should we use an OpenGL debug context?

		// Get the various window and GL context configurable options.
		if(!RetrieveAndConvertOption<bool>(m_optionsPtr, string("UseDesktopSettings"), GameOptions::PROGRAMMER, useDesktopSettings))
		{
            GF_LOG_ERR("Failed to get the UseDesktopSettings option so using a default value instead (true)");
			useDesktopSettings = true;
		}
		if(!RetrieveAndConvertOption<I32>(m_optionsPtr, string("ScreenWidth"), GameOptions::PLAYER, screenWidth))
		{
            GF_LOG_ERR("Failed to get the ScreenWidth option so using a default value instead (640)");
			screenWidth = 0;
		}
		if(!RetrieveAndConvertOption<I32>(m_optionsPtr, string("ScreenHeight"), GameOptions::PLAYER, screenHeight))
		{
            GF_LOG_ERR("Failed to get the ScreenHeight option so using a default value instead (480)");
			screenHeight = 0;
		}
		if(!RetrieveAndConvertOption<string>(m_optionsPtr, string("GameName"), GameOptions::PROGRAMMER, gameName))
		{
            GF_LOG_ERR("Failed to get the GameName option so using a default value instead \" -- Unknown Title -- \"");
			gameName.assign(string(" -- Unknown Title -- "));
		}
		if(!RetrieveAndConvertOption<I32>(m_optionsPtr, string("FullScreen"), GameOptions::PROGRAMMER, fullscreen))
		{
            GF_LOG_ERR("Failed to get the FullScreen option so using a default value instead (on)");
			fullscreen = 1;
		}
		if(!RetrieveAndConvertOption<I32>(m_optionsPtr, string("Multisampling"), GameOptions::PLAYER, multisampling))
		{
            GF_LOG_ERR("Failed to get the Multisampling option so using a default value instead (off)");
			multisampling = 0;
		}
		if(!RetrieveAndConvertOption<I32>(m_optionsPtr, string("OpenGLMajor"), GameOptions::PROGRAMMER, glMajor))
		{
            GF_LOG_ERR("Failed to get the OpenGLMajor option so using a default value instead (3)");
			glMajor = 3;
		}
		if(!RetrieveAndConvertOption<I32>(m_optionsPtr, string("OpenGLMinor"), GameOptions::PROGRAMMER, glMinor))
		{
            GF_LOG_ERR("Failed to get the OpenGLMinor option so using a default value instead (3)");
			glMinor = 3;
		}
		string profileStr;
		if(!RetrieveAndConvertOption<string>(m_optionsPtr, string("OpenGLProfile"), GameOptions::PROGRAMMER, profileStr))
		{
            GF_LOG_ERR("Failed to get the OpenGLProfile option so using a default value instead (Unknown)");
			profile = 0;
		}
		if(!RetrieveAndConvertOption<I32>(m_optionsPtr, string("ColorBufferSize"), GameOptions::PROGRAMMER, cbSize))
		{
            GF_LOG_ERR("Failed to get the ColorBufferSize option so using a default value instead (desktop default)");
			cbSize = 0;
		}
		if(!RetrieveAndConvertOption<I32>(m_optionsPtr, string("DepthBufferSize"), GameOptions::PROGRAMMER, dbSize))
		{
            GF_LOG_ERR("Failed to get the DepthBufferSize option so using a default value instead (desktop default)");
			dbSize = 0;
		}
		if(!RetrieveAndConvertOption<I32>(m_optionsPtr, string("StencilBufferSize"), GameOptions::PROGRAMMER, sbSize))
		{
            GF_LOG_ERR("Failed to get the StencilBufferSize option so using a default value instead (off)");
			sbSize = 0;
		}
		if(!RetrieveAndConvertOption<I32>(m_optionsPtr, string("AlphaBits"), GameOptions::PROGRAMMER, alphaBits))
		{
            GF_LOG_ERR("Failed to get the AlphaBits option so using a default value instead (off)");
			alphaBits = 0;
		}
		if(!RetrieveAndConvertOption<bool>(m_optionsPtr, string("OpenGLDebug"), GameOptions::PROGRAMMER, glDebugContext))
		{
            GF_LOG_ERR("Failed to get the OpenGLDebug option so using a default value instead (off)");
			glDebugContext = 0;
		}

		boost::algorithm::to_lower(profileStr);
		if(profileStr.compare("compatibility") == 0)
		{
			profile = GLFW_OPENGL_COMPAT_PROFILE;
		}
		else if(profileStr.compare("core") == 0)
		{
			profile = GLFW_OPENGL_CORE_PROFILE;
		}

		// Create user defined window and OpenGL context parameters.
		WindowParameters params;

		if(!useDesktopSettings)
		{
			// Set the SW, SH, CB size, DB size and fullscreen parameters from the options.
			params.SetDimensions(screenWidth, screenHeight);
			params.SetFullscreen((fullscreen == 0) ? false : true);
			if(cbSize == 24)
			{
				params.SetColorBuffer(8, 8, 8);
			}
			else if(cbSize == 16)
			{
				params.SetColorBuffer(5, 6, 5);
			}
			else
			{
				// invalid options, fall back to default
                GF_LOG_ERR("Invalid color buffer size from options.  Falling back to default");
				cbSize = 0;
				params.SetColorBuffer(0, 0, 0);
			}
			if(dbSize != 16 && dbSize != 24 && dbSize != cbSize)
			{
                GF_LOG_ERR("Invalid depth buffer size from options.  Falling back to same size as color buffer");
				dbSize = cbSize;
			}
			params.SetDepthBufferSize(dbSize);
		}

		// Set the remainder of the parameters from the options.
		params.SetTitle(gameName);
		params.SetGlVersion(glMajor, glMinor);
		params.SetGlProfile(profile);
		params.SetGlDebugContext(glDebugContext);
		params.SetAlphaBufferSize(alphaBits);
		params.SetStencilBufferSize(sbSize);
		params.SetFsaaSamplesNumber(multisampling);
		
		// Create the best possible OpenGL context and window (depending on the hardware available).
		result = CreateOpenGLWindow(params, useDesktopSettings);
		if(result)
		{
			// Set up the GLFW window and input callbacks now.
#ifdef USE_NEW_GLFW
			glfwSetWindowCloseCallback(OnGlfwWindowCloseCallback);
			glfwSetWindowSizeCallback(OnGlfwWindowResizeCallback);
			glfwSetWindowRefreshCallback(OnGlfwWindowRefreshCallback);
			glfwSetKeyCallback(OnGlfwKeyCallback);
			glfwSetCharCallback(OnGlfwKeyCharacterCallback);
			glfwSetMouseButtonCallback(OnGlfwMouseButtonCallback);
			glfwSetMousePosCallback(OnGlfwMouseMoveCallback);
			glfwSetScrollCallback(OnGlfwMouseWheelCallback);
#else
			glfwSetWindowCloseCallback(OnGlfwWindowCloseCallback);
			glfwSetWindowSizeCallback(OnGlfwWindowResizeCallback);
			glfwSetWindowRefreshCallback(OnGlfwWindowRefreshCallback);
			glfwSetKeyCallback(OnGlfwKeyCallback);
			glfwSetCharCallback(OnGlfwKeyCharacterCallback);
			glfwSetMouseButtonCallback(OnGlfwMouseButtonCallback);
			glfwSetMousePosCallback(OnGlfwMouseMoveCallback);
			glfwSetMouseWheelCallback(OnGlfwMouseWheelCallback);
#endif

			// Check if there are any joysticks plugged in now (after GLFW has been initialized...)
			SystemCheck systemCheckObj;
			I32 numJoysticks = 0;
			if(!systemCheckObj.CheckForJoysticks(m_joystickList, numJoysticks))
			{
                GF_LOG_INF("There are no joysticks plugged into the system");
			}
			else
			{
				ostringstream conv;
				conv << numJoysticks;
                GF_LOG_INF(string("There are ") + conv.str() + string(" joysticks plugged in"));
			}
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	WindowParameters GameMain::GetMinimumWindowParameters()
	{
		// TODO: Get these parameters from the xml configuration file.
		WindowParameters minParams;

		string gameName;
		if(!RetrieveAndConvertOption<string>(m_optionsPtr, string("GameName"), GameOptions::PROGRAMMER, gameName))
		{
            GF_LOG_ERR("Failed to get the GameName option so using a default value instead \" -- Unknown Title -- \"");
			gameName.assign(string(" -- Unknown Title -- "));
		}
		minParams.SetTitle(gameName);

		return (minParams);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GameMain::CreateOpenGLWindow(struct WindowParameters &userParams, const bool useDesktopSettings)
	{
		// Get current desktop settings if we are configured to use them.
		if(useDesktopSettings)
		{
			// Create a temporary min spec window to determine the current desktop mode params.
			WindowParameters minParams = GetMinimumWindowParameters();
			WindowParameters desktopParams;

			try
			{
				m_windowManagerPtr.reset(GCC_NEW WindowManager(minParams, m_loggerPtr));
				desktopParams = m_windowManagerPtr->GetDesktopModeParameters();
				m_windowManagerPtr.reset();
			}	
			catch(GameException &)
			{
                GF_LOG_FAT("Failed to retrieve desktop settings");
				return (false);
			}

			userParams.SetDimensions(desktopParams.GetWidth(), desktopParams.GetHeight());
			userParams.SetFullscreen(true);
			userParams.SetColorBuffer(desktopParams.GetCBufRedBits(), desktopParams.GetCBufGreenBits(), desktopParams.GetCBufBlueBits());
			userParams.SetDepthBufferSize(desktopParams.GetDepthBufferSize());
		}

		// 1) Try to setup window with user defined parameters.
		try
		{
			m_windowManagerPtr.reset(GCC_NEW WindowManager(userParams, m_loggerPtr));
		}
		catch(GameException &)
		{
            GF_LOG_ERR("Failed to create window with user defined options");
			return (false);
		}

		return (true);
	}
    
    // /////////////////////////////////////////////////////////////////
    //
    //
    // /////////////////////////////////////////////////////////////////
    bool GameMain::SetupTextureAtlasManager(const std::string &resourceId)
    {
        if(!m_resourceCachePtr)
            return (false);
        
        m_atlasPtr = boost::shared_ptr<TextureAtlasManager>(new TextureAtlasManager());
        
        return (m_atlasPtr && m_atlasPtr->LoadFromResourceCache(resourceId));
    }

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GameMain::Initialize()
	{
		bool result = true;			// Result of setup calls.

		// Check the system.
		CheckSystemResources();

		// Set the global wide directories.
		result = SetGlobalDirectories();

		// Create the game resource cache.
		if(result)
		{
			result = SetUpResourceCache();
		}

		// Initialize the LUA scripting manager.
		if(result)
		{
			result = SetUpScriptManager();
		}

		// Initialize the event framework.
		if(result)
		{
			result = SetUpEventManager();
		}

		// Register the core game events
		if(result)
		{
			result = RegisterBaseGameEvents();
		}

		// Load the startup script 
		if(result)
		{
			string initScript(m_luaCommonDir.string() + string("init.lua"));
			result = m_luaStateManagerPtr->Init(initScript.c_str());
		}

		// Create the window and display.
		if(result)
		{
			result = SetUpWindowManager();
		}

		// Initialize the OpenGL drawing library and the OpenGL extensions required for the application.
		//  Please note that the TextureManager should be initialized in here after all other OpenGL
		//  specifics are set up.
		if(result)
		{
			result = VInitOpenGL();
		}

		// Initialize the game logic and views subsystem (implemented
		//  by all derived classes).
		if(result)
		{
			m_logicPtr = VCreateLogicAndViews();
			if(!m_logicPtr)
			{
                GF_LOG_TRACE_FAT("GameMain::Initialize()", "The base game logic pointer has not been set");
				result = false;
			}
		}

		// If we failed, ensure the memory allocated by VCreateLogicAndViews() is cleaned up.
		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	GameMain::GameMain(shared_ptr<GameLog> &loggerPtr, shared_ptr<GameOptions> &optionsPtr)\
		throw (GameException &) : m_lastRenderTime(0.0), m_lastUpdateTime(0.0), m_lastEventTime(0.0), m_frameRateTimer(), m_frameCount(0),\
			m_framesInPastSecond(0), m_appTimer(), m_startTime(0.0), m_isRunning(true), m_resourceCachePtr(), m_luaStateManagerPtr(),\
				m_eventManagerPtr(), m_logicPtr(), m_atlasPtr(), m_loggerPtr(loggerPtr), m_windowManagerPtr(), m_optionsPtr(optionsPtr),\
					m_eventQueue(), m_prevX(0), m_prevY(0), m_prevActiveState(false), m_joystickList(), m_metaTable(),\
					m_texManagerPtr(), m_gameRootDir(), m_dataDir(), m_appDataDir(), m_luaCommonDir(), m_saveGameDir()
	{
		// Set the global game application pointer here!
		g_appPtr = this;

		m_frameRateTimer = shared_ptr<IGameTimer>(GCC_NEW GlfwGameTimer());
		m_appTimer = shared_ptr<IGameTimer>(GCC_NEW GlfwGameTimer());

		if(!optionsPtr)
		{
			g_appPtr = NULL;
			throw (GameException(string("GameOptions is NULL! We need a set of valid game options before we can setup the game.")));
		}
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	GameMain::~GameMain()
	{
		try
		{
		}
		catch(...) {}
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GameMain::IsRunning()
	{
		return (m_isRunning);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::SetRunning(const bool isRunning)
	{
		m_isRunning = isRunning;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	U32 GameMain::GetFramesInPastSecond() const
	{
		return (m_framesInPastSecond);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	U32 GameMain::GetFrameCount() const
	{
		return (m_frameCount);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::VPollEvents()
	{
		// The total elapsed time so far.
		F64 time = m_appTimer->VGetTime();
		// The time elapsed since the last successfull poll events call.
		F32 elapsedTime = F32(time) - F32(m_lastEventTime);

		// Check for new events from glfw (this will fill up the event queue again).
		glfwPollEvents();

		// Bugfix: GLFW close window bug, Check if the user clicked on the close window button.
		if(!m_isRunning)
		{
			// no need to poll for more events, just shutdown as quickly as possible...
			return;
		}

		// Poll for events we are not able to receive by callback from GLFW.
		// Active window, joystick events, etc.
		if(m_prevActiveState != m_windowManagerPtr->IsActive())
		{
			OnGlfwActiveWindowEvent(m_windowManagerPtr->IsActive() ? 1 : 0);
			m_prevActiveState = m_windowManagerPtr->IsActive();
		}

		// Go through all joysticks and see if there have been any detected/lost.
		//  and also check for input events for any joysticks connected.
		PollJoysticks();

		// Pump each event in the queue through the system.
		for(GfEventQueue::iterator curr = m_eventQueue.begin(); curr != m_eventQueue.end(); ++curr)
		{
			switch(curr->id)
			{
				// Handle imporatant global input/OS window events here.
				case GF_ACTIVE_EVENT:
				{
					VOnActiveEvent((curr->active.focus == 1 ? true : false));
				}
				break;
				case GF_QUIT_EVENT:
				{
					SetRunning(false);
				}
				break;
				case GF_WINDOW_RESIZE_EVENT:
				{
					VOnResize(curr->resize.w, curr->resize.h);
				}
				break;
				case GF_WINDOW_REFRESH_EVENT:
				{
					VOnRefresh();
				}
				break;
				case GF_JOYSTICK_STATE_CHANGE_EVENT:
				{
					VOnJoystickStateChange(curr->joyStateChange.joyId, curr->joyStateChange.state, curr->joyStateChange.numAxes, curr->joyStateChange.numButtons);
				}
				break;

				// Pass input events off around the system.
				case GF_MOUSE_BUTTON_EVENT:
				case GF_MOUSE_WHEEL_EVENT:
				case GF_MOUSE_MOVE_EVENT:
				case GF_KEY_EVENT:
				case GF_KEY_CHARACTER_EVENT:
				case GF_JOYSTICK_MOVE_EVENT:
				case GF_JOYSTICK_BUTTON_EVENT:
				{
#ifdef DEBUG
					if(m_loggerPtr && m_loggerPtr->GetLogLevel() >= GameLog::TRC)
					{
						switch(curr->id)
						{
						case GF_MOUSE_BUTTON_EVENT:
							std::cout << "Mouse button " << curr->mouseButton.buttonId << " was " << (curr->mouseButton.state == GLFW_PRESS ? " pressed " : " released ") << " at position " << curr->mouseButton.x << ", " << curr->mouseButton.y << std::endl;
							break;
						case GF_MOUSE_WHEEL_EVENT:
#if defined(USE_NEW_GLFW)
                            std::cout << "Mouse wheel moved to a new position of " << curr->mouseWheel.x << "," << curr->mouseWheel.y << std::endl;
#else
                            std::cout << "Mouse wheel moved to a new position of " << curr->mouseWheel.pos << std::endl;
#endif
							break;
						case GF_MOUSE_MOVE_EVENT:
							std::cout << "Mouse moved to position " << curr->mouseMove.x << ", " << curr->mouseMove.y << " relative motion: " << curr->mouseMove.xrel << ", " << curr->mouseMove.yrel << std::endl;
							break;
						case GF_KEY_EVENT:
							std::cout << "The key " << curr->key.keyId << " was " << (curr->key.keyState == GLFW_PRESS ? " pressed " : " released ") << std::endl;
							break;
						case GF_KEY_CHARACTER_EVENT:
							std::cout << "The character " << curr->keyChar.characterId << " was " << (curr->keyChar.characterState == GLFW_PRESS ? " pressed " : " released ") << std::endl;
							break;
						case GF_JOYSTICK_MOVE_EVENT:
							std::cout << "The joystick " << curr->joyMove.joyId << " axis' " << curr->joyMove.axisId << " was moved to " << curr->joyMove.pos << std::endl;
							break;
						case GF_JOYSTICK_BUTTON_EVENT:
							std::cout << "The joystick " << curr->joyButton.joyId << " buttons' " << curr->joyButton.buttonId << " was " << (curr->joyButton.buttonState == GLFW_PRESS ? " pressed " : " released ") << std::endl;
							break;
						}
					}
#endif

					// We will treat pressing the escape key as a quit shortcut for our demos.
					if((curr->id == GF_KEY_EVENT) && (curr->key.keyState == GLFW_RELEASE) && (curr->key.keyId == GLFW_KEY_ESC))
					{
						SetRunning(false);
					}

					// Note the reverse order! User input is grabbed first from the view that is on top, 
					// which is the last one in the list.
					//GameViewList viewList = m_logicPtr->GetGameViewList();
					//bool topViewHandled = false;
					//for(GameViewList::reverse_iterator i = viewList.rbegin(); ((!topViewHandled) && (i != viewList.rend())); ++i)
					//{
					//	if((*i)->VOnEvent(*curr))
					//	{
					//		// The events must be passed to the top view only so exit loop now!
					//		topViewHandled = true;
					//	}
					//}
					GameViewList viewList = m_logicPtr->GetGameViewList();
					for(GameViewList::reverse_iterator i = viewList.rbegin(); i != viewList.rend(); ++i)
					{
						(*i)->VOnEvent(*curr, elapsedTime);
					}
				}
				break;
			}
		}

		// Clear event queue as all events have been handled.
		m_eventQueue.clear();

		// Record the last time the events were polled.
		m_lastEventTime = m_appTimer->VGetTime();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::PollJoysticks()
	{
		// Poll for every possible joystick.
		for(GfJoyCont::const_iterator curr = m_joystickList.begin(), end = m_joystickList.end(); curr != end; ++curr)
		{
			(*curr)->PollJoystickState(m_eventQueue, &m_eventFactoryObj);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::VOnRefresh()
	{
        GF_LOG_INF("Window refresh event occurred");
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::VOnJoystickStateChange(const I32 joyId, const I32 state, const I32 numAxes, const I32 numButtons)
	{
        GF_LOG_INF("Joystick state change event occurred");
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::VOnActiveEvent(const bool focus)
	{
        GF_LOG_DEB(focus ? "Gained application focus" : "Lost application focus");
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::VOnResize(const I32 width, const I32 height)
	{
        GF_LOG_INF("Resize window event occurred");
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::VUpdate()
	{
		// The total elapsed time so far.
		F64 time = m_appTimer->VGetTime();
		// The time elapsed since the last successfull update call.
		F32 elapsedTime = F32(time) - F32(m_lastUpdateTime);

		// allow event queue to process for a maximum of 20 ms to deal with game events.
		safeTickEventManager(20);

		// Update the logic layer.
		m_logicPtr->VOnUpdate(time, elapsedTime);

		// Record the end time of the last successfull update.
		m_lastUpdateTime = m_appTimer->VGetTime();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::VRender()
	{
		// The total elapsed time so far in seconds.
		F64 time = m_appTimer->VGetTime();
		// The time elapsed since the last successfull render call (in seconds).
		F32 elapsedTime = F32(time) - F32(m_lastRenderTime);

		// Render all the game views in the order in which they were added.
		GameViewList viewList = m_logicPtr->GetGameViewList();
		for(GameViewList::iterator i = viewList.begin(); i != viewList.end(); ++i)
		{
			(*i)->VOnRender(time, elapsedTime);
		}

		// Record the end time of the last successfull render.
		m_lastRenderTime = m_appTimer->VGetTime();
	}
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GameMain::Sleep(const F64 milliseconds)
    {
#ifdef USE_NEW_GLFW
        // TODO: This is dumb, Replace with a proper sleep function!
        F64 endTime = glfwGetTime() + fabs(milliseconds);
        while(glfwGetTime() < endTime)
        {
            // do nothing
        }
#else
        glfwSleep(milliseconds);
#endif
    }

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::Main()
	{
		if(!m_logicPtr)
		{
            GF_LOG_FAT("Cannot run the main game loop as the logic layer does not exist");
			return;
		}

		// Timer used to calculate the number of frames drawn in the past second.
		shared_ptr<IGameTimer> secondTimer(GCC_NEW GlfwGameTimer());

		// Reset timers and variables required for loop.
		m_framesInPastSecond = 0;
		m_frameCount = 0;
		m_appTimer->VStart();
		m_startTime = m_appTimer->VGetTime();
		secondTimer->VStart();

#ifdef DEBUG
		bool printed = false;
#endif
		
		// The main game loop.
		while(m_isRunning)
		{
			// Start frame update and draw timer
			m_frameRateTimer->VStart();

			// Calculate statistics
			if(secondTimer->VGetTime() >= 1.0)
			{
				m_framesInPastSecond = m_frameCount;
#ifdef DEBUG
				std::cout << "FPS: " << m_framesInPastSecond << std::endl;
#endif
				m_frameCount = 0;
				secondTimer->VStart();
			}
			else
			{
				++m_frameCount;
			}

			// Update the state of the game objects (implemented by all derived classes).
			VUpdate();

			// Render the scene (implemented by all derived classes).
			VRender();

			// Flip the screen back buffer.
			m_windowManagerPtr->SwapBuffers();

			// Regulate fps by sleeping for a short period
			//  if this frame took less than FRAME_TIME_MS to update and render.
			if(m_frameRateTimer->VGetTime() < FRAME_TIME_MS)
			{
				F64 milliseconds = FRAME_TIME_MS - m_frameRateTimer->VGetTime();
				// Sleep the remaining frame time
				Sleep(milliseconds);
			}

			// Poll for input events (implemented by derived classes).
			VPollEvents();

#ifdef DEBUG
			//// Print out fps statistics every 5 seconds to console approx.
			//I32 testVal = I32(m_appTimer->VGetTime()) % 5;
			//if(testVal == 4 && !printed)
			//{
			//	std::cout << "m_framesInPastSecond: " << m_framesInPastSecond << std::endl;
			//	printed = true;
			//}
			//else
			//{
			//	printed = false;
			//}
#endif
		}
        GF_LOG_INF("Leaving the main game loop now");
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	F64 GameMain::GetAnimationLoopStartTime() const
	{
		return (m_startTime);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	F64 GameMain::GetCurrTime()
	{
		if(m_appTimer)
		{
			return (m_appTimer->VGetTime() - m_startTime);
		}

		return (0.0);
	}

	// /////////////////////////////////////////////////////////////////
	// ********************** GLFW callbacks ***************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameMain::OnGlfwCloseWindowEvent()
	{
		SetRunning(false);

		// Bugfix: GLFW shutdown on window close bug, Instead of broadcasting an event, 
		// we will just set the running flag to false.
		//GF_Event eventObj;
		//eventObj.id = GF_QUIT_EVENT;
		//m_eventFactoryObj.CreateCloseWindowEvent(eventObj.quit);
		//m_eventQueue.push_back(eventObj);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::OnGlfwResizeWindowEvent(const I32 width, const I32 height)
	{
		GF_Event eventObj;
		eventObj.id = GF_WINDOW_RESIZE_EVENT;
		m_eventFactoryObj.CreateResizeWindowEvent(eventObj.resize, width, height);
		m_eventQueue.push_back(eventObj);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::OnGlfwRefreshWindowEvent()
	{
		GF_Event eventObj;
		eventObj.id = GF_WINDOW_REFRESH_EVENT;
		m_eventFactoryObj.CreateRefreshWindowEvent(eventObj.refresh);
		m_eventQueue.push_back(eventObj);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::OnGlfwKeyEvent(const I32 keyId, const I32 action)
	{
		GF_Event eventObj;
		eventObj.id = GF_KEY_EVENT;
		m_eventFactoryObj.CreateKeyEvent(eventObj.key, keyId, action);
		m_eventQueue.push_back(eventObj);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::OnGlfwKeyCharacterEvent(const I32 characterId, const I32 action)
	{
		GF_Event eventObj;
		eventObj.id = GF_KEY_CHARACTER_EVENT;
		m_eventFactoryObj.CreateKeyCharacterEvent(eventObj.keyChar, characterId, action);
		m_eventQueue.push_back(eventObj);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::OnGlfwMouseButtonEvent(const I32 buttonId, const I32 action)
	{
		GF_Event eventObj;
		eventObj.id = GF_MOUSE_BUTTON_EVENT;
		m_eventFactoryObj.CreateMouseButtonEvent(eventObj.mouseButton, buttonId, action);
		m_eventQueue.push_back(eventObj);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::OnGlfwMouseMoveEvent(const I32 x, const I32 y)
	{
		GF_Event eventObj;
		eventObj.id = GF_MOUSE_MOVE_EVENT;
		m_eventFactoryObj.CreateMouseMoveEvent(eventObj.mouseMove, x, y, m_prevX, m_prevY);
		m_prevX = x;
		m_prevY = y;
		m_eventQueue.push_back(eventObj);
	}

#ifdef USE_NEW_GLFW
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::OnGlfwMouseWheelEvent(const I32 x, const I32 y)
	{
		GF_Event eventObj;
		eventObj.id = GF_MOUSE_WHEEL_EVENT;
		m_eventFactoryObj.CreateMouseWheelEvent(eventObj.mouseWheel, x, y);
		m_eventQueue.push_back(eventObj);
	}
#else
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::OnGlfwMouseWheelEvent(const I32 position)
	{
		GF_Event eventObj;
		eventObj.id = GF_MOUSE_WHEEL_EVENT;
		m_eventFactoryObj.CreateMouseWheelEvent(eventObj.mouseWheel, position);
		m_eventQueue.push_back(eventObj);
	}
#endif

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::OnGlfwActiveWindowEvent(const I32 active)
	{
		GF_Event eventObj;
		eventObj.id = GF_ACTIVE_EVENT;
		m_eventFactoryObj.CreateActiveWindowEvent(eventObj.active, active);
		m_eventQueue.push_back(eventObj);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::OnGlfwJoystickStatusChangeEvent(const I32 joyId, const I32 status, const I32 numAxes, const I32 numButtons)
	{
		GF_Event eventObj;
		eventObj.id = GF_JOYSTICK_STATE_CHANGE_EVENT;
		m_eventFactoryObj.CreateJoystickStatusChangeEvent(eventObj.joyStateChange, joyId, status, numAxes, numButtons);
		m_eventQueue.push_back(eventObj);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::OnGlfwJoystickMoveEvent(const I32 joyId, const I32 axisId, const F32 position)
	{
		GF_Event eventObj;
		eventObj.id = GF_JOYSTICK_MOVE_EVENT;
		m_eventFactoryObj.CreateJoystickMoveEvent(eventObj.joyMove, joyId, axisId, position);
		m_eventQueue.push_back(eventObj);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameMain::OnGlfwJoystickButtonEvent(const I32 joyId, const I32 buttonId, const I32 buttonState)
	{
		GF_Event eventObj;
		eventObj.id = GF_JOYSTICK_BUTTON_EVENT;
		m_eventFactoryObj.CreateJoystickButtonEvent(eventObj.joyButton, joyId, buttonId, buttonState);
		m_eventQueue.push_back(eventObj);
	}

#ifdef USE_NEW_GLFW
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFW_FUNC_PRE OnGlfwKeyCallback(GLFWwindow window, I32 key, I32 action)
	{
		g_appPtr->OnGlfwKeyEvent(key, action);
	}
    
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFW_FUNC_PRE OnGlfwKeyCharacterCallback(GLFWwindow window, I32 character)
	{
		g_appPtr->OnGlfwKeyCharacterEvent(character, 0);
	}
    
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFW_FUNC_PRE OnGlfwMouseButtonCallback(GLFWwindow window, I32 button, I32 action)
	{
		g_appPtr->OnGlfwMouseButtonEvent(button, action);
	}
    
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFW_FUNC_PRE OnGlfwMouseMoveCallback(GLFWwindow window, I32 x, I32 y)
	{
		g_appPtr->OnGlfwMouseMoveEvent(x, y);
	}
    
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFW_FUNC_PRE OnGlfwMouseWheelCallback(GLFWwindow window, I32 posX, I32 posY)
	{
		g_appPtr->OnGlfwMouseWheelEvent(posX, posY);
	}
    
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	I32 GLFW_FUNC_PRE OnGlfwWindowCloseCallback(GLFWwindow window)
	{
		g_appPtr->OnGlfwCloseWindowEvent();
        
		// TODO: This causes GLFW to close the window so maybe do so only if the user is really closing the app...
		return (GL_TRUE);
	}
    
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFW_FUNC_PRE OnGlfwWindowResizeCallback(GLFWwindow window, I32 width, I32 height)
	{
		g_appPtr->OnGlfwResizeWindowEvent(width, height);
	}
    
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFW_FUNC_PRE OnGlfwWindowRefreshCallback(GLFWwindow window)
	{
		g_appPtr->OnGlfwRefreshWindowEvent();
	}
#else
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFW_FUNC_PRE OnGlfwKeyCallback(I32 key, I32 action)
	{
		g_appPtr->OnGlfwKeyEvent(key, action);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFW_FUNC_PRE OnGlfwKeyCharacterCallback(I32 character, I32 action)
	{
		g_appPtr->OnGlfwKeyCharacterEvent(character, action);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFW_FUNC_PRE OnGlfwMouseButtonCallback(I32 button, I32 action)
	{
		g_appPtr->OnGlfwMouseButtonEvent(button, action);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFW_FUNC_PRE OnGlfwMouseMoveCallback(I32 x, I32 y)
	{
		g_appPtr->OnGlfwMouseMoveEvent(x, y);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFW_FUNC_PRE OnGlfwMouseWheelCallback(I32 pos)
	{
		g_appPtr->OnGlfwMouseWheelEvent(pos);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	I32 GLFW_FUNC_PRE OnGlfwWindowCloseCallback(void)
	{
		g_appPtr->OnGlfwCloseWindowEvent();

		// TODO: This causes GLFW to close the window so maybe do so only if the user is really closing the app...
		return (GL_TRUE);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFW_FUNC_PRE OnGlfwWindowResizeCallback(I32 width, I32 height)
	{
		g_appPtr->OnGlfwResizeWindowEvent(width, height);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFW_FUNC_PRE OnGlfwWindowRefreshCallback(void)
	{
		g_appPtr->OnGlfwRefreshWindowEvent();
	}
#endif

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ConvertWindowCoordinates(Point3 &pt)
	{
		//pt.SetY(abs(pt.GetY() - static_cast<F32>(g_appPtr->GetWindowManager()->GetHeight())));
		pt.SetY(abs(static_cast<F32>(g_appPtr->GetWindowManager()->GetHeight()) - pt.GetY()));
	}

}
