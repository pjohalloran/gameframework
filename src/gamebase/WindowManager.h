// /////////////////////////////////////////////////////////////////
// @file WindowManager.h
// @author PJ O Halloran
// @date 06/12/2009
//
// File contains the header for the WindowManager class.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GF_WINDOW_MANAGER_H
#define __GF_WINDOW_MANAGER_H

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

// External Headers
#include <stdexcept>

#include <cstdlib>
#include <cstdio>

#include "GameBase.h"

#include <boost/shared_ptr.hpp>

#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

// Project Headers
#include "GameException.h"
#include "GameLog.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @struct WindowParameters
	// @author PJ O Halloran
	//
	// Holds the parameters for a Window (GLFW specific).
	//
	// This struct is passed to the window manager to informit how the
	// window and the OpenGL context should be setup.  Most of these
	// parameters may not be changed after the window has been created
	// as the OpenGL context should not be changed after the initial window
	// setup.  At least GLFW does not allow it.  For my demos this is
	// sufficient behaviour, however for a commercial application I
	// understand that this is not acceptable.
	//
	// Some of these parameters may be changed after window
	// creation.  For example, the window width and height may be changed
	// and the window can be minimized and restored.  Generally speaking
	// all OpenGL parameters may not be changed and all window only parameters
	// (apart from fullscreen) may be changed in line with how GLFW is
	// designed.
	//
	// /////////////////////////////////////////////////////////////////
	struct WindowParameters
	{
		I32 m_w, m_h, m_x, m_y;								///< Dimensions/Position.
		I32 m_cbR, m_cbG, m_cbB;							///< Color buffer rgb.
		I32 m_abSize;										///< Alpha buffer size.
		I32 m_dbSize;										///< Depth buffer size.
		I32 m_sbSize;										///< Stencil buffer size.
		bool m_fullscreen;									///< Should app be fullscreen?
		I32 m_accumbR, m_accumbG, m_accumbB, m_accumbA;		///< Accumalation buffer attributes.
		I32 m_numAuxBuffers;								///< Number of auxillary buffers.
		bool m_stereoRendering;								///< Should stereo rendering be initialized.
		I32 m_numFsaaSample;								///< Number of FSAA samples for multisampling.
		std::string m_title;								///< Window title.
		bool m_autoPollEvents;								///< Should events automatically be polled for on glfwSwapBuffers()?
		bool m_keyRepeat;									///< Should a key held down send key events reapetidly?
		bool m_mouseVisible;								///< Is mouse visible (no on fullscreen automatically)?
		bool m_systemKeysEnabled;							///< Should system keys be enabled/disabled?
		I32 m_glMajorV, m_glMinorV;							///< OpenGL version to use?
		I32 m_glProfile;									///< GL profile to use (for OpenGL 3.2 and up only).
		bool m_glDebugContext;								///< Create debug OpenGL context?

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		WindowParameters() : m_w(0), m_h(0), m_x(0), m_y(0), m_cbR(0), m_cbG(0), m_cbB(0), m_abSize(0), m_dbSize(0),\
			m_sbSize(0), m_fullscreen(false), m_accumbR(0), m_accumbG(0), m_accumbB(0), m_accumbA(0),\
				m_numAuxBuffers(0), m_stereoRendering(false), m_numFsaaSample(0), m_title(), m_autoPollEvents(true),\
					m_keyRepeat(true), m_mouseVisible(true), m_systemKeysEnabled(false), m_glMajorV(0), m_glMinorV(0),\
						m_glProfile(0), m_glDebugContext(false) {};

		// /////////////////////////////////////////////////////////////////
		// Various mathods to get and set general window attributes.
		//
		// /////////////////////////////////////////////////////////////////
		void SetDimensions(const I32 w, const I32 h) { m_w = w; m_h = h; };
		I32 GetWidth() const { return m_w; };
		I32 GetHeight() const { return m_h; };

		void SetPosition(const I32 x, const I32 y) { m_x = x; m_y = y; };
		I32 GetXPos() const { return (m_x); };
		I32 GetYPos() const { return (m_y); };

		void SetTitle(const std::string &titleRef) { m_title = titleRef; };
		std::string GetTitle() const { return (m_title); };

		void SetFullscreen(const bool n) { m_fullscreen = n; };
		bool IsFullscreen() const { return (m_fullscreen); };

		void SetAutoPollEvents(const bool n) { m_autoPollEvents = n; };
		bool IsAutoPollEvents() const { return (m_autoPollEvents); };

		void SetKeyRepeat(const bool n) { m_keyRepeat = n; };
		bool IsKeyRepeat() const { return (m_keyRepeat); };

		void SetMouseVisible(const bool n) { m_mouseVisible = n; };
		bool IsMouseVisible() const { return (m_mouseVisible); };

		void SetSystemKeysEnabled(const bool n) { m_systemKeysEnabled = n; };
		bool IsSystemKeysEnabled() const { return (m_systemKeysEnabled); };


		// /////////////////////////////////////////////////////////////////
		// Various methods to get and set the OpenGL buffer and setup
		// attributes.
		//
		// /////////////////////////////////////////////////////////////////
		void SetColorBuffer(const I32 r, const I32 g, const I32 b) { m_cbR = r; m_cbG = g; m_cbB = b; };
		I32 GetCBufRedBits() const { return (m_cbR); };
		I32 GetCBufGreenBits() const { return (m_cbG); };
		I32 GetCBufBlueBits() const { return (m_cbB); };

		void SetAlphaBufferSize(const I32 a) { m_abSize = a; };
		I32 GetAlphaBufferSize() const { return (m_abSize); };

		void SetDepthBufferSize(const I32 d) { m_dbSize = d; };
		I32 GetDepthBufferSize() const { return (m_dbSize); };

		void SetStencilBufferSize(const I32 s) { m_sbSize = s; };
		I32 GetStencilBufferSize() const { return (m_sbSize); };

		void SetAccumBuffer(const I32 r, const I32 g, const I32 b, const I32 a) { m_accumbR = r; m_accumbG = g; m_accumbB = b; m_accumbA = a; };
		I32 GetAccumBufRedBits() const { return (m_accumbR); };
		I32 GetAccumBufGreenBits() const { return (m_accumbG); };
		I32 GetAccumBufBlueBits() const { return (m_accumbB); };
		I32 GetAccumBufAlphaBits() const { return (m_accumbA); };

		void SetAuxBuffersNumber(const I32 n) { m_numAuxBuffers = n; };
		I32 GetAuxBuffersNumber() const { return (m_numAuxBuffers); };

		void SetFsaaSamplesNumber(const I32 n) { m_numFsaaSample = n; };
		I32 GetFsaaSamplesNumber() const { return (m_numFsaaSample); };

		void SetStereoRendering(const bool n) { m_stereoRendering = n; };
		bool IsStereoRendering() const { return (m_stereoRendering); };

		void SetGlVersion(const I32 majV, const I32 minV) { m_glMajorV = majV; m_glMinorV = minV; };
		I32 GetGlMajorVersion() const { return (m_glMajorV); };
		I32 GetGlMinorVersion() const { return (m_glMinorV); };

		// Note - If you change the GL profile then the minimum GL version must be 3.2!
		void SetGlProfile(const I32 n) { m_glProfile = n; if(m_glMajorV < 3) { m_glMajorV = 3; m_glMinorV = 2; } };
		I32 GetGlProfile() const { return (m_glProfile); };
		void SetGlDebugContext(const bool n) { m_glDebugContext = n; };
		bool IsGlDebugContext() const { return (m_glDebugContext); };
	};

	// /////////////////////////////////////////////////////////////////
	// @class WindowManager
	// @author PJ O Halloran
	//
	// This class handles initalization and cleanup of the
	// GLFW system and the window.  It sets the video mode and checks
	// for input events from the OS.
	//
	// /////////////////////////////////////////////////////////////////
	class WindowManager
	{
		private:
		
			WindowParameters m_params;						///< The desired window attributes (or the attributes the window had when it was created).
			boost::shared_ptr<GameLog> m_loggerPtr;			///< Pointer to the logging object.
			LuaPlus::LuaObject m_metaTable;					///< Provides external access to member functions for LUA scripts.
#if USE_NEW_GLFW
            GLFWwindow m_glfwPtr;                              ///< GLFW3 pointer.
#endif
        
			// /////////////////////////////////////////////////////////////////
			// Check if the window was set up okay with the parameters the user
			// specified.  If there are any problems, then this method will log
			// them in the logfile.
			//
			// @return bool True if the window was set up okay and false if any
			//				serious GLFW configuration issues occurred.
			//
			// /////////////////////////////////////////////////////////////////
			bool CheckGlfwParameters();

			// /////////////////////////////////////////////////////////////////
			// Log various information to the log file for session debugging.
			//
			// /////////////////////////////////////////////////////////////////
			void LogWindowProperties();

		public:
			
			// /////////////////////////////////////////////////////////////////
			// Custom constructor.
			//
			// If the width/height is 0, then the desktop video mode will be used.
			//
			// @param params The desired window attributes.
			// @param loggerPtr Pointer to the GameLog logging object.
			//
			// @throw GameException If we fail to initialize the window.
			// @throw GameException& If the windows parameters are not supplied.
			//
			// /////////////////////////////////////////////////////////////////
			WindowManager(const WindowParameters &params, boost::shared_ptr<GameLog> &loggerPtr) throw (GameException &);

			// /////////////////////////////////////////////////////////////////
			// Destructor.
			//
			// /////////////////////////////////////////////////////////////////
			~WindowManager();
			
			// /////////////////////////////////////////////////////////////////
			// Get the parameters the window was created with.  These may be
			// different to the current window state.
			//
			// /////////////////////////////////////////////////////////////////
			WindowParameters GetWindowParameters() const {	return (m_params); };
			
			// /////////////////////////////////////////////////////////////////
			// Toggle if the window is minimized or restored.
			//
			// This generates an application lost device/restore event so
			// that all textures/sounds may be disposed of or restored.
			//
			// This method should be called in my application rather than calling
			// glfwIconfiy() or glfwRestoreWindow() methods directly as it does
			// some internal housekeeping.
			//
			// /////////////////////////////////////////////////////////////////
			void ToggleIconify();
			
			// /////////////////////////////////////////////////////////////////
			// Check if the window is minimized.
			//
			// /////////////////////////////////////////////////////////////////
			bool IsIconified();

			// /////////////////////////////////////////////////////////////////
			// Check if the windown is currently in focus.
			//
			// /////////////////////////////////////////////////////////////////
			bool IsActive() const;

			// /////////////////////////////////////////////////////////////////
			// Check if the window is resizable.
			//
			// /////////////////////////////////////////////////////////////////
			bool IsResizable() const;

			// /////////////////////////////////////////////////////////////////
			// Check if the window is opened.
			//
			// /////////////////////////////////////////////////////////////////
			bool IsOpened() const;

			// /////////////////////////////////////////////////////////////////
			// Get current height of the application window.  The vertical
			// resolution is returned if the window is fullscreen.
			//
			// /////////////////////////////////////////////////////////////////
			I32 GetHeight();

			// /////////////////////////////////////////////////////////////////
			// Get current width of the application window.  The horizontal
			// resolution is returned if the window is fullscreen.
			//
			// /////////////////////////////////////////////////////////////////
			I32 GetWidth();

			// /////////////////////////////////////////////////////////////////
			// Query the windowing system for info about the actual state of the
			// OpenGL context which was created.
			//
			// See GLFW documentation on glfwGetWindowParam().
			//
			// /////////////////////////////////////////////////////////////////
			I32 GetWindowParam(const I32 paramId) const;

			// /////////////////////////////////////////////////////////////////
			// Get the window parameters from the current desktop mode.
			//
			// /////////////////////////////////////////////////////////////////
			WindowParameters GetDesktopModeParameters() const;

			// /////////////////////////////////////////////////////////////////
			// Set the video mode of the window.  OpenGL context will still be
			// valid after this call.
			//
			// The closest valid resolution will be chosen to the parameters
			// supplied.
			//
			// @param screenWidth The new horizontal reaolution of the window.
			// @param screenHeight The new vertical resolution of the window.
			//
			// /////////////////////////////////////////////////////////////////
			bool SetVideoMode(const I32 screenWidth, const I32 screenHeight);

			// /////////////////////////////////////////////////////////////////
			// Create and execute a lua command that creates a table of all
			// possible video resolutions.  This table is available in the lua
			// instance set running by the lua state manager.
			//
			// The table has the name "wmVideoResolutionTable" and has the format:
			//
			// "wmVideoResolutionTable = 
			// {
			//	"640*480",
			//	"800*600",
			//	"1024*940",
			//	(etc.)
			// }"
			//
			// /////////////////////////////////////////////////////////////////
			void SetLuaVideoResolutionsTable();
        
            // /////////////////////////////////////////////////////////////////
            // Swap the buffers.
            //
            // /////////////////////////////////////////////////////////////////
            void SwapBuffers() { glfwSwapBuffers(); };
        
#ifdef USE_NEW_GLFW
            // /////////////////////////////////////////////////////////////////
            // Get the GLFW window context pointer.
            //
            // /////////////////////////////////////////////////////////////////
            inline GLFWwindow GetGlfwWindowPtr() const { return (m_glfwPtr); };
#endif
	};

	// The minimum OpenGL version my demos will support (I think its fair to assume that most recent PCs support GL 2.1 at least).
	#define GF_GL_MAJOR_VERSION	2
	#define GF_GL_MINOR_VERSION	1

	// /////////////////////////////////////////////////////////////////
	// Check that the OpenGL version we got from the window manager
	// was at least the minimum required for these demos to operate.
	//
	// @param majGot The OpenGL major version number on this PC.
	// @param minGot The OpenGL minor version number on this PC.
	//
	// /////////////////////////////////////////////////////////////////
	bool CheckGlVersionIsMinimumSupported(const I32 majGot, const I32 minGot);

	// /////////////////////////////////////////////////////////////////
	// Check that the OpenGL version we got from the window manager
	// matches the one we requested.
	//
	// @param majGot The OpenGL major version number on this PC.
	// @param minGot The OpenGL minor version number on this PC.
	// @param majReq The OpenGL major version number requested.
	// @param minReq The OpenGL minor version number requested.
	//
	// /////////////////////////////////////////////////////////////////
	bool CheckGlVersionMatches(const I32 majGot, const I32 minGot, const I32 majReq, const I32 minReq);

}

#ifdef WIN32
#	pragma warning( pop )
#endif

#endif

