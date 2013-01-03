// /////////////////////////////////////////////////////////////////
// @file WindowManager.cpp
// @author PJ O Halloran
// @date 06/12/2009
//
// File contains the implementation of the WindowManager class.
//
// /////////////////////////////////////////////////////////////////

#include <string>

#include <boost/lexical_cast.hpp>

#include "WindowManager.h"
#include "GameMain.h"
#include "Events.h"

// Namespace Declarations
using std::string;

using boost::shared_ptr;
using boost::lexical_cast;
using boost::bad_lexical_cast;

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool WindowManager::CheckGlfwParameters()
	{
		I32 result = 0;
		bool error = false;

        GF_LOG_DEB("Checking GLFW intialization parameters");

#ifdef USE_NEW_GLFW
        result = glfwGetWindowParam(m_glfwPtr, GLFW_RED_BITS);
#else
        result = glfwGetWindowParam(GLFW_RED_BITS);
#endif
		if(m_params.GetCBufRedBits() != 0 && result != m_params.GetCBufRedBits())
		{
            GF_LOG_ERR("GLFW_RED_BITS got does not match that requested");
			error = true;
		}

#ifdef USE_NEW_GLFW
        result = glfwGetWindowParam(m_glfwPtr, GLFW_GREEN_BITS);
#else
        result = glfwGetWindowParam(GLFW_GREEN_BITS);
#endif
		if(m_params.GetCBufGreenBits() != 0 && result != m_params.GetCBufGreenBits())
		{
            GF_LOG_ERR("GLFW_GREEN_BITS got does not match that requested");
			error = true;
		}

#ifdef USE_NEW_GLFW
        result = glfwGetWindowParam(m_glfwPtr, GLFW_BLUE_BITS);
#else
        result = glfwGetWindowParam(GLFW_BLUE_BITS);
#endif
		if(m_params.GetCBufBlueBits() != 0 && result != m_params.GetCBufBlueBits())
		{
            GF_LOG_ERR("GLFW_BLUE_BITS got does not match that requested");
			error = true;
		}

#ifdef USE_NEW_GLFW
        result = glfwGetWindowParam(m_glfwPtr, GLFW_ALPHA_BITS);
#else
        result = glfwGetWindowParam(GLFW_ALPHA_BITS);
#endif
		if(m_params.GetAlphaBufferSize() != 0 && result != m_params.GetAlphaBufferSize())
		{
            GF_LOG_ERR("GLFW_ALPHA_BITS got does not match that requested");
			error = true;
		}

#ifdef USE_NEW_GLFW
        result = glfwGetWindowParam(m_glfwPtr, GLFW_DEPTH_BITS);
#else
        result = glfwGetWindowParam(GLFW_DEPTH_BITS);
#endif
		if(m_params.GetDepthBufferSize() != 0 && result != m_params.GetDepthBufferSize())
		{
            GF_LOG_ERR("GLFW_DEPTH_BITS got does not match that requested");
			error = true;
		}
        
#ifdef USE_NEW_GLFW
        result = glfwGetWindowParam(m_glfwPtr, GLFW_STENCIL_BITS);
#else
        result = glfwGetWindowParam(GLFW_STENCIL_BITS);
#endif
		if(m_params.GetStencilBufferSize() != 0 && result != m_params.GetStencilBufferSize())
		{
            GF_LOG_ERR("GLFW_STENCIL_BITS got does not match that requested");
			error = true;
		}

		I32 stereo = m_params.IsStereoRendering() ? 1 : 0;
#ifdef USE_NEW_GLFW
        result = glfwGetWindowParam(m_glfwPtr, GLFW_STEREO);
#else
        result = glfwGetWindowParam(GLFW_STEREO);
#endif
		if(result != stereo)
		{
            GF_LOG_ERR("GLFW_STEREO got does not match that requested");
			error = true;
		}

#ifdef USE_NEW_GLFW
        result = glfwGetWindowParam(m_glfwPtr, GLFW_FSAA_SAMPLES);
#else
        result = glfwGetWindowParam(GLFW_FSAA_SAMPLES);
#endif
		if(m_params.GetFsaaSamplesNumber() != 0 && result != m_params.GetFsaaSamplesNumber())
		{
            GF_LOG_ERR("GLFW_FSAA_SAMPLES got does not match that requested");
			error = true;
		}

#ifdef USE_NEW_GLFW
		I32 glMajGot = glfwGetWindowParam(m_glfwPtr, GLFW_OPENGL_VERSION_MAJOR);
		I32 glMinGot = glfwGetWindowParam(m_glfwPtr, GLFW_OPENGL_VERSION_MINOR);
#else
		I32 glMajGot = glfwGetWindowParam(GLFW_OPENGL_VERSION_MAJOR);
		I32 glMinGot = glfwGetWindowParam(GLFW_OPENGL_VERSION_MINOR);
#endif
		I32 glMajReq = m_params.GetGlMajorVersion();
		I32 glMinReq = m_params.GetGlMinorVersion();

		if(!CheckGlVersionIsMinimumSupported(glMajGot, glMinGot))
		{
			// TODO: We need to pop up an error message if this occurs on screen to make this clear.
           GF_LOG_FAT("The OpenGL library on the system is not the minimum we require to run these demos (version 2.1).  Please update your graphics hardware and/or your vendor drivers");
			error = true;
		}
		if(!CheckGlVersionMatches(glMajGot, glMinGot, glMajReq, glMinReq))
		{
			// TODO: We have to alter how we render the application if this occurs, some graphics effects may not work.
			// Not a serious error but log it anyway.
            GF_LOG_ERR("The OpenGL version got does not match the version requested");
		}

#ifdef USE_NEW_GLFW
		result = glfwGetWindowParam(m_glfwPtr, GLFW_OPENGL_PROFILE);
#else
		result = glfwGetWindowParam(GLFW_OPENGL_PROFILE);
#endif
		if(m_params.GetGlProfile() != 0 && result != m_params.GetGlProfile())
		{
            GF_LOG_ERR("GLFW_OPENGL_PROFILE got does not match that requested");
			// TODO: Figure out why GLFW is reporting that we are not getting the compatibility profile when requested.
			//  To proceed i am marking this temporarily as not a fatal startup error...
			//error = true;
		}

		if(!error)
		{
            GF_LOG_INF("GLFW window and OpenGL context was setup as requested");
		}

		return (!error);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void WindowManager::LogWindowProperties()
	{
#if DEBUG
        GF_LOG_DEB("Logging GLFW properties");

		string majorStr, minorStr, revStr;
		I32 major, minor, rev;
		glfwGetVersion(&major, &minor, &rev);
		try
		{
			majorStr = lexical_cast<string, I32>(major);
			minorStr = lexical_cast<string, I32>(minor);
			revStr = lexical_cast<string, I32>(rev);
            GF_LOG_DEB(string("Using GLFW version: ") + majorStr + string(" ") + minorStr + string(" ") + revStr);
		}
		catch(bad_lexical_cast &)
		{
		}
#ifndef USE_NEW_GLFW
		glfwGetGLVersion(&major, &minor, &rev);
		try
		{
			majorStr = lexical_cast<string, I32>(major);
			minorStr = lexical_cast<string, I32>(minor);
			revStr = lexical_cast<string, I32>(rev);
            GF_LOG_DEB(string("Using OpenGL version: ") + majorStr + string(" ") + minorStr + string(" ") + revStr);
		}
		catch(bad_lexical_cast &)
		{
		}
#endif

		GLFWvidmode mode;
		glfwGetDesktopMode(&mode);
		string rStr, gStr, bStr, wStr, hStr;
		try
		{
#ifdef USE_NEW_GLFW
			rStr = lexical_cast<string, I32>(mode.redBits);
			gStr = lexical_cast<string, I32>(mode.greenBits);
			bStr = lexical_cast<string, I32>(mode.blueBits);
			wStr = lexical_cast<string, I32>(mode.width);
			hStr = lexical_cast<string, I32>(mode.height);
#else
			rStr = lexical_cast<string, I32>(mode.RedBits);
			gStr = lexical_cast<string, I32>(mode.GreenBits);
			bStr = lexical_cast<string, I32>(mode.BlueBits);
			wStr = lexical_cast<string, I32>(mode.Width);
			hStr = lexical_cast<string, I32>(mode.Height);
#endif
            GF_LOG_DEB(string("Current desktop video mode: ") + wStr + string("X") + hStr + string(" (") + rStr + string(",") + gStr + string(",") + bStr + string(")"));
#ifdef USE_NEW_GLFW
			rStr = lexical_cast<string, I32>(glfwGetWindowParam(m_glfwPtr, GLFW_RED_BITS));
			gStr = lexical_cast<string, I32>(glfwGetWindowParam(m_glfwPtr, GLFW_GREEN_BITS));
			bStr = lexical_cast<string, I32>(glfwGetWindowParam(m_glfwPtr, GLFW_BLUE_BITS));
#else
			rStr = lexical_cast<string, I32>(glfwGetWindowParam(GLFW_RED_BITS));
			gStr = lexical_cast<string, I32>(glfwGetWindowParam(GLFW_GREEN_BITS));
			bStr = lexical_cast<string, I32>(glfwGetWindowParam(GLFW_BLUE_BITS));
#endif
			I32 w, h;
#ifdef USE_NEW_GLFW
            glfwGetWindowSize(m_glfwPtr, &w, &h);
#else
            glfwGetWindowSize(&w, &h);
#endif
			wStr = lexical_cast<string, I32>(w);
			hStr = lexical_cast<string, I32>(h);
            GF_LOG_DEB(string("Current Window Video Mode: ") + wStr + string("X") + hStr + string(" (") + rStr + string(",") + gStr + string(",") + bStr + string(")"));
			const I32 maxModes = 256;
			GLFWvidmode vidModes[maxModes];
            I32 total = glfwGetVideoModes(vidModes, maxModes);
			string indexStr;
            GF_LOG_DEB("List of available video modes:");
			for(I32 i = 0; i < total; ++i)
			{
				indexStr = lexical_cast<string, I32>(i+1);
#ifdef USE_NEW_GLFW
				rStr = lexical_cast<string, I32>(vidModes[i].redBits);
				gStr = lexical_cast<string, I32>(vidModes[i].greenBits);
				bStr = lexical_cast<string, I32>(vidModes[i].blueBits);
				wStr = lexical_cast<string, I32>(vidModes[i].width);
				hStr = lexical_cast<string, I32>(vidModes[i].height);
#else
				rStr = lexical_cast<string, I32>(vidModes[i].RedBits);
				gStr = lexical_cast<string, I32>(vidModes[i].GreenBits);
				bStr = lexical_cast<string, I32>(vidModes[i].BlueBits);
				wStr = lexical_cast<string, I32>(vidModes[i].Width);
				hStr = lexical_cast<string, I32>(vidModes[i].Height);
#endif
                GF_LOG_DEB(indexStr + string(": ") + wStr + string("X") + hStr + string(" (") + rStr + string(",") + gStr + string(",") + bStr + string(")"));
			}
		}
		catch(bad_lexical_cast &)
		{
		}

		string numStr;
		try
		{
#ifndef USE_NEW_GLFW
			numStr = lexical_cast<string, I32>(glfwGetNumberOfProcessors());
            GF_LOG_DEB(string("Number of processors available: ") + numStr);
#endif
#ifdef USE_NEW_GLFW
            numStr = lexical_cast<string, I32>(glfwGetWindowParam(m_glfwPtr, GLFW_ACCELERATED));
#else
            numStr = lexical_cast<string, I32>(glfwGetWindowParam(GLFW_ACCELERATED));
#endif
            GF_LOG_DEB(string("Window Hardware Acceleration (1=on, 0=off): ") + numStr);
		}
		catch(bad_lexical_cast &)
		{
            GF_LOG_TRACE_ERR("WindowManager::LogWindowProperties()", "Lexical cast error");
		}
#endif
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	WindowManager::WindowManager(const WindowParameters &params,
								boost::shared_ptr<GameLog> &loggerPtr) throw (GameException &)
								: m_params(params)
								, m_loggerPtr(loggerPtr)
								, m_metaTable()
#ifdef USE_NEW_GLFW
								, m_glfwPtr(NULL)
#endif
	{
		glfwInit();
		
		glfwOpenWindowHint(GLFW_STEREO, m_params.IsStereoRendering() ? 1 : 0);
		glfwOpenWindowHint(GLFW_FSAA_SAMPLES, m_params.GetFsaaSamplesNumber());

		if(m_params.GetGlProfile() != 0 && (m_params.GetGlMajorVersion() < 3 || (m_params.GetGlMajorVersion() == 3 && m_params.GetGlMinorVersion() < 2)))
		{
			glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
			glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
		}
		else
		{
			glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, m_params.GetGlMajorVersion());
			glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, m_params.GetGlMinorVersion());
		}

		glfwOpenWindowHint(GLFW_OPENGL_PROFILE, m_params.GetGlProfile());
        //glfwOpenWindowHint(GLFW_OPENGL_PROFILE, 0);
		glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, m_params.IsGlDebugContext() ? 1 : 0);

#if USE_NEW_GLFW
		if((m_glfwPtr = glfwOpenWindow(m_params.GetWidth(),\
                                      m_params.GetHeight(),\
                                      m_params.IsFullscreen() ? GLFW_FULLSCREEN : GLFW_WINDOWED,
                                      m_params.GetTitle().c_str(),\
                                      NULL)) == NULL)
#else
		if(glfwOpenWindow(m_params.GetWidth(),\
							m_params.GetHeight(),\
							m_params.GetCBufRedBits(),\
							m_params.GetCBufGreenBits(),\
							m_params.GetCBufBlueBits(),\
							m_params.GetAlphaBufferSize(),\
							m_params.GetDepthBufferSize(),\
							m_params.GetStencilBufferSize(),\
							m_params.IsFullscreen() ? GLFW_FULLSCREEN : GLFW_WINDOW) != GL_TRUE)
#endif
		{
			glfwTerminate();
			throw GameException(string("Failed to open the application window.  Please check your OpenGL/Window settings."));
		}

		if(!CheckGlfwParameters())
		{
#ifdef USE_NEW_GLFW
            glfwCloseWindow(m_glfwPtr);
#endif
			glfwTerminate();
			throw GameException(string("Serious error occurred initializing the window.  Please check the log file."));
		}

		if(!m_params.IsFullscreen())
		{
#ifdef USE_NEW_GLFW
			glfwSetWindowTitle(m_glfwPtr, m_params.GetTitle().c_str());
			glfwSetWindowPos(m_glfwPtr, m_params.GetXPos(), m_params.GetYPos());
#else
			glfwSetWindowTitle(m_params.GetTitle().c_str());
			glfwSetWindowPos(m_params.GetXPos(), m_params.GetYPos());
#endif
		}
		glfwSwapInterval(1);
#ifndef USE_NEW_GLFW
		glfwDisable(GLFW_AUTO_POLL_EVENTS);
#endif

		if(m_params.IsKeyRepeat())
		{
#ifdef USE_NEW_GLFW
            glfwEnable(m_glfwPtr, GLFW_KEY_REPEAT);
#else
            glfwEnable(GLFW_KEY_REPEAT);
#endif
		}
		else
		{
#ifdef USE_NEW_GLFW
            glfwDisable(m_glfwPtr, GLFW_KEY_REPEAT);
#else
            glfwDisable(GLFW_KEY_REPEAT);
#endif
		}
		if(m_params.IsMouseVisible())
		{
#ifdef USE_NEW_GLFW
            glfwEnable(m_glfwPtr, GLFW_MOUSE_CURSOR);
#else
            glfwEnable(GLFW_MOUSE_CURSOR);
#endif
		}
		else
		{
#ifdef USE_NEW_GLFW
            glfwDisable(m_glfwPtr, GLFW_MOUSE_CURSOR);
#else
            glfwDisable(GLFW_MOUSE_CURSOR);
#endif
		}
		
		LogWindowProperties();

		// Lua scripting access.
		m_metaTable = g_appPtr->GetLuaStateManager()->GetGlobalState()->GetGlobals().CreateTable("WindowManager");
		m_metaTable.SetObject("__index", m_metaTable);
		m_metaTable.RegisterObjectDirect("SetVideoMode", (WindowManager *)0, &WindowManager::SetVideoMode);
		m_metaTable.RegisterObjectDirect("GetWidth", (WindowManager *)0, &WindowManager::GetWidth);
		m_metaTable.RegisterObjectDirect("GetHeight", (WindowManager *)0, &WindowManager::GetHeight);
		m_metaTable.RegisterObjectDirect("ToggleIconify", (WindowManager *)0, &WindowManager::ToggleIconify);
		m_metaTable.RegisterObjectDirect("IsIconified", (WindowManager *)0, &WindowManager::IsIconified);
		m_metaTable.RegisterObjectDirect("SetLuaVideoResolutionsTable", (WindowManager *)0, &WindowManager::SetLuaVideoResolutionsTable);
		LuaPlus::LuaObject wmStateManObj = g_appPtr->GetLuaStateManager()->GetGlobalState()->BoxPointer(this);
		wmStateManObj.SetMetaTable(m_metaTable);
		g_appPtr->GetLuaStateManager()->GetGlobalState()->GetGlobals().SetObject("WindowManager", wmStateManObj);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	WindowManager::~WindowManager()
	{
		try
		{
            GF_LOG_TRACE_DEB("WindowManager::~WindowManager()", "Shutting down the GLFW");
#ifdef USE_NEW_GLFW
            glfwCloseWindow(m_glfwPtr);
#endif
			glfwTerminate();
		}
		catch(...)
		{
			// Don't allow the destructor to throw exceptions, this is
			//  simply good C++ practice...
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void WindowManager::ToggleIconify()
	{
		if(IsIconified())
		{
#ifdef USE_NEW_GLFW
            glfwRestoreWindow(m_glfwPtr);
#else
            glfwRestoreWindow();
#endif
			// TODO: Send some event here...
		}
		else
		{
#ifdef USE_NEW_GLFW
            glfwIconifyWindow(m_glfwPtr);
#else
            glfwIconifyWindow();
#endif
			// TODO: Send some event here...
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool WindowManager::IsIconified()
	{
#ifdef USE_NEW_GLFW
        return (glfwGetWindowParam(m_glfwPtr, GLFW_ICONIFIED) == GL_TRUE);
#else
        return (glfwGetWindowParam(GLFW_ICONIFIED) == GL_TRUE);
#endif
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool WindowManager::IsActive() const
	{
#ifdef USE_NEW_GLFW
        return (glfwGetWindowParam(m_glfwPtr, GLFW_ACTIVE) == GL_TRUE);
#else
        return (glfwGetWindowParam(GLFW_ACTIVE) == GL_TRUE);
#endif
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool WindowManager::IsResizable() const
	{
#ifdef USE_NEW_GLFW
        return (glfwGetWindowParam(m_glfwPtr, GLFW_WINDOW_NO_RESIZE) == GL_TRUE);
#else
        return (glfwGetWindowParam(GLFW_WINDOW_NO_RESIZE) == GL_TRUE);
#endif
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool WindowManager::IsOpened() const
	{
#ifdef USE_NEW_GLFW
        return (glfwGetWindowParam(m_glfwPtr, GLFW_ACTIVE) == GL_TRUE && glfwGetWindowParam(m_glfwPtr, GLFW_ICONIFIED) == GL_FALSE);
#else
        return (glfwGetWindowParam(GLFW_OPENED) == GL_TRUE);
#endif
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	I32 WindowManager::GetHeight()
	{
		I32 w, h;
#ifdef USE_NEW_GLFW
        glfwGetWindowSize(m_glfwPtr, &w, &h);
#else
        glfwGetWindowSize(&w, &h);
#endif
		return (h);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	I32 WindowManager::GetWidth()
	{
		I32 w, h;
#ifdef USE_NEW_GLFW
        glfwGetWindowSize(m_glfwPtr, &w, &h);
#else
        glfwGetWindowSize(&w, &h);
#endif
		return (w);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	I32 WindowManager::GetWindowParam(const I32 paramId) const
	{
#ifdef USE_NEW_GLFW
        return (glfwGetWindowParam(m_glfwPtr, paramId));
#else
        return (glfwGetWindowParam(paramId));
#endif
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	WindowParameters WindowManager::GetDesktopModeParameters() const
	{
		WindowParameters desktopModeParams;

		GLFWvidmode mode;
		glfwGetDesktopMode(&mode);

		// We will fill out what we can from here according to the GLFWvidmode struct and
		//  let the caller decide on the rest of the window parameters.
#ifdef USE_NEW_GLFW
		desktopModeParams.SetDimensions(mode.width, mode.height);
		desktopModeParams.SetColorBuffer(mode.redBits, mode.greenBits, mode.blueBits);
		desktopModeParams.SetDepthBufferSize(mode.redBits + mode.greenBits + mode.blueBits);
#else
		desktopModeParams.SetDimensions(mode.Width, mode.Height);
		desktopModeParams.SetColorBuffer(mode.RedBits, mode.GreenBits, mode.BlueBits);
		desktopModeParams.SetDepthBufferSize(mode.RedBits + mode.GreenBits + mode.BlueBits);
#endif

		return (desktopModeParams);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool WindowManager::SetVideoMode(const I32 screenWidth, const I32 screenHeight)
	{
		if(screenWidth != GetWidth() || screenHeight != GetHeight())
		{
			I32 oldW, oldH;
#ifdef USE_NEW_GLFW
            glfwGetWindowSize(m_glfwPtr, &oldW, &oldH);
			glfwSetWindowSize(m_glfwPtr, screenWidth, screenHeight);
#else
            glfwGetWindowSize(&oldW, &oldH);
			glfwSetWindowSize(screenWidth, screenHeight);
#endif

			I32 newW, newH;
#ifdef USE_NEW_GLFW
            glfwGetWindowSize(m_glfwPtr, &newW, &newH);
#else
            glfwGetWindowSize(&newW, &newH);
#endif
			if(newW != oldW || newH != oldH)
			{
				EvtData_Video_Resolution_Change eventData(oldW, oldH, newW, newH);
				safeTriggerEvent(eventData);
			}
		}
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void WindowManager::SetLuaVideoResolutionsTable()
	{
#ifdef USE_NEW_GLFW
        const I32 displayMode = glfwGetWindowParam(m_glfwPtr, GLFW_RED_BITS) + glfwGetWindowParam(m_glfwPtr, GLFW_GREEN_BITS) + glfwGetWindowParam(m_glfwPtr, GLFW_BLUE_BITS);
#else
        const I32 displayMode = glfwGetWindowParam(GLFW_RED_BITS) + glfwGetWindowParam(GLFW_GREEN_BITS) + glfwGetWindowParam(GLFW_BLUE_BITS);
#endif

		string luaCommand("wmVideoResolutionTable = { ");
		const string sep("*"), comma(","), quote("\"");
		string wStr, hStr;

		const I32 maxModes = 256;
		GLFWvidmode vidModes[maxModes];
		I32 total = glfwGetVideoModes(vidModes, maxModes);
		for(I32 i = 0; i < total; ++i)
		{
			// We dont allow the user to change display mode once the app is started, limitation of GLFW
			// so we only show them resolutions in the same display mode.
			// We could get around this by restarting the application if it was a big issue.
#ifdef USE_NEW_GLFW
			if(vidModes[i].redBits + vidModes[i].greenBits + vidModes[i].blueBits == displayMode)
			{
				wStr = lexical_cast<string, I32>(vidModes[i].width);
				hStr = lexical_cast<string, I32>(vidModes[i].height);
#else
            if(vidModes[i].RedBits + vidModes[i].GreenBits + vidModes[i].BlueBits == displayMode)
            {
                wStr = lexical_cast<string, I32>(vidModes[i].Width);
                hStr = lexical_cast<string, I32>(vidModes[i].Height);
#endif
				luaCommand.append(quote + wStr + sep + hStr + quote + comma);
			}
		}
		luaCommand.append(string(" };"));
		g_appPtr->GetLuaStateManager()->ExecuteString(luaCommand.c_str());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CheckGlVersionIsMinimumSupported(const I32 majGot, const I32 minGot)
	{
		if(majGot < GF_GL_MAJOR_VERSION)
		{
			return (false);
		}
		else if(majGot == GF_GL_MAJOR_VERSION)
		{
			if(minGot < GF_GL_MINOR_VERSION)
			{
				return (false);
			}
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CheckGlVersionMatches(const I32 majGot, const I32 minGot, const I32 majReq, const I32 minReq)
	{
		if(majGot != majReq || minGot != minReq)
		{
			return (false);
		}

		return (true);
	}
}
