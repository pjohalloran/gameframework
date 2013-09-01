#pragma once
#ifndef __GAME_MAIN_H
#define __GAME_MAIN_H

// /////////////////////////////////////////////////////////////////
// @file GameMain.h
// @author PJ O Halloran
// @date 21/11/2009
//
// File contains the header for the GameMain class.
//
// /////////////////////////////////////////////////////////////////

#include <stdexcept>
#include <vector>
#include <deque>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

#include "GameBase.h"
#include "GameException.h"
#include "WindowManager.h"
#include "LuaStateManager.h"
#include "EventManagerImpl.h"
#include "GameLogic.h"
#include "Timer.h"
#include "Point.h"
#include "OsInputEvents.h"

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran {

    class ResCache;
    class TextureManager;
    class TextureAtlasManager;
    class GameOptions;
    class GameLog;
    class FontBufferCache;

    // /////////////////////////////////////////////////////////////////
    // @class GameMain
    // @author PJ O Halloran
    //
    // This class handles initalization, management and cleanup of the
    // platform independant application.
    //
    // /////////////////////////////////////////////////////////////////
    class GameMain {
    private:

        static const F64 MAX_FRAMES_PER_SECOND;                     ///< Max fps drawn by the game.
        static const F64 FRAME_TIME_MS;                             ///< Time allowed to draw one frame.

        // Main loop management data.
        F64 m_lastRenderTime;                                       ///< Timestamp of the last render call.
        F64 m_lastUpdateTime;                                       ///< Timestamp of the last update call.
        F64 m_lastEventTime;                                        ///< Timestamp of the last OS event call.
        boost::shared_ptr<Timer> m_frameRateTimer;                  ///< Timer to regulate the frame rate.
        U32 m_frameCount;                                           ///< Number of frames rendered so far in the current second.
        U32 m_framesInPastSecond;                                   ///< Number of frames rendered in the past second.
        boost::shared_ptr<Timer> m_appTimer;                        ///< Timer used to record the application start time, etc.
        F64 m_startTime;                                            ///< Start time of the game loop.
        bool m_isRunning;                                           ///< Flag indicating if the game is currently running.

        // Global sub system managers.
        boost::shared_ptr<ResCache> m_resourceCachePtr;             ///< Resource Cache manager.
        boost::shared_ptr<LuaStateManager> m_luaStateManagerPtr;    ///< LUA state manager.
        boost::shared_ptr<EventManager> m_eventManagerPtr;          ///< Event framework.
        boost::shared_ptr<BaseGameLogic> m_logicPtr;                ///< Pointer to the logic layer.
        boost::shared_ptr<TextureAtlasManager> m_atlasPtr;          ///< TextureAtlas manager.

        // GLFW/OS event data.
        GfEventFactory m_eventFactoryObj;                           ///< Global OS input/window event factory object.
        GfEventQueue m_eventQueue;                                  ///< The OS window/input event queue.
        I32 m_prevX;                                                ///< Previous X screen position of the mouse.
        I32 m_prevY;                                                ///< Previous Y screen position of the mouse.
        bool m_prevActiveState;                                     ///< Previous active state of the window.
        GfJoyCont m_joystickList;                                   ///< List of joysticks currently detected by the OS.
        LuaPlus::LuaObject m_metaTable;                             ///< Provides external access to member functions for LUA scripts.

        // /////////////////////////////////////////////////////////////////
        // Set all the global directories of interest throughout the
        // application.
        //
        // /////////////////////////////////////////////////////////////////
        bool SetGlobalDirectories();

        // /////////////////////////////////////////////////////////////////
        // Check the system resources.
        //
        // /////////////////////////////////////////////////////////////////
        void CheckSystemResources();

        // /////////////////////////////////////////////////////////////////
        // Setup the resource caching object for the game.
        //
        // @return bool True on success and false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool SetUpResourceCache();

        // /////////////////////////////////////////////////////////////////
        // Setup the LUA scripting manager.
        //
        // @return bool True on success and false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool SetUpScriptManager();

        // /////////////////////////////////////////////////////////////////
        // Setup the game event framework.
        //
        // @return bool True on success and false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool SetUpEventManager();

        // /////////////////////////////////////////////////////////////////
        // Register the core game events.
        //
        // @return bool True on success and false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool RegisterBaseGameEvents();

        // /////////////////////////////////////////////////////////////////
        // Setup the window manager.
        //
        // @return bool True on success and false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool SetUpWindowManager();

        // /////////////////////////////////////////////////////////////////
        // Get the minimum window/OpenGL context system parameters defined
        // for this application in the user configuration file.
        //
        // /////////////////////////////////////////////////////////////////
        WindowParameters GetMinimumWindowParameters();

        // /////////////////////////////////////////////////////////////////
        // Helper function for SetUpWindowManager(). Creates a window with
        // the user defined options.  If this is not successfull then it will
        // attempt to create a context matching the video mode of the desktop.
        //
        // Finally in the case that this also fails it will attempt to fall
        // back to a context with our minimum defined specifications.
        // (OpenGL 2.1, 16 bit color buffer, 640*480 resolution fullscreen
        // application with no alpha, depth, color, stencil or multisampling
        // buffers).
        //
        // @param userParams The user defined window parameters.
        // @param useDesktopSettings Should we retrieve the current desktop
        //                              settings?
        //
        // @return bool True on creation of a window and false otherwise.
        //
        // /////////////////////////////////////////////////////////////////
        bool CreateOpenGLWindow(struct WindowParameters &userParams, const bool useDesktopSettings = true);

        // /////////////////////////////////////////////////////////////////
        // Setup the game logic and game views subsystem.  This should be
        // implemeted by all derived classes.
        //
        // @return boost::shared_ptr<BaseGameLogic> The logic layer pointer
        //                                          on success or NULL on
        //                                          failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual boost::shared_ptr<BaseGameLogic> VCreateLogicAndViews() = 0;

        // /////////////////////////////////////////////////////////////////
        // Initliaize the OpenGL drawning library and any extensions required
        // by the application.
        //
        // Please note that the OpenGL context itself has been initialized
        // by the WindowManager.  All thats left to do is to check for
        // extensions and perform any general OpenGL setup stuff such as
        // setting the clear color, etc.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VInitOpenGL() = 0;

    protected:

        // Global sub system managers (available to subclasses).
        boost::shared_ptr<GameLog> m_loggerPtr;                                 ///< Pointer to the logging object.
        boost::shared_ptr<WindowManager> m_windowManagerPtr;                    ///< WindowManager.
        boost::shared_ptr<GameOptions> m_optionsPtr;                            ///< Pointer to the GameOptions for the game.
        boost::shared_ptr<TextureManager> m_texManagerPtr;                      ///< OpenGL texture manager.
        boost::shared_ptr<FontBufferCache> m_fontBufferCachePtr;                ///< The Font VB cache.

        // Directories of interest globally to the application.
        boost::filesystem::path m_gameRootDir;                                  ///< Location of game root.
        boost::filesystem::path m_dataDir;                                      ///< Location of global data directory.
        boost::filesystem::path m_appDataDir;                                   ///< Location of application data directory.
        boost::filesystem::path m_luaCommonDir;                                 ///< Location of common LUA scripts diirectory.
        boost::filesystem::path m_saveGameDir;                                  ///< Save game directory.

        // /////////////////////////////////////////////////////////////////
        // Poll for input events from the players. The default behaviour may
        // be overridden by derived classes if you wish.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VPollEvents();

        // /////////////////////////////////////////////////////////////////
        // Update the state of all game objects from input such as the player,
        // AI, physics calculations or the network.  The default behaviour may
        // be overridden by derived classes if you wish.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VUpdate();

        // /////////////////////////////////////////////////////////////////
        // The method that will render the scene. The default behaviour may
        // be overridden by derived classes if you wish.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VRender();

        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool SetupTextureAtlasManager(const std::string &resourceId);

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param loggerPtr Pointer to the GameLog logging object.
        // @param optionsPtr Pointer to the options for the game.
        //
        // @throw GameException If the pointers to the logger or the screen
        //                      manager are invalid.
        //
        // /////////////////////////////////////////////////////////////////
        GameMain(boost::shared_ptr<GameLog> &loggerPtr, boost::shared_ptr<GameOptions> &optionsPtr) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~GameMain();

        // /////////////////////////////////////////////////////////////////
        // Initializes the game and all its subsystems.  Please note the order
        // of subsystem setup is important as some subsystems depend on others
        // to be running.  For example, many subsystems provide access to
        // external lua scripts, therefore they require the Script manager
        // to have been initialized beforehand.
        //
        // **** Please note this method cannot be called from the constructor
        // as it calls some pure virtual functions which are implemented
        // in derivied classes.  These derived objects do not exist yet during
        // the execution of the GameMain constructor. ****
        //
        // Check system resources.
        // Setup:
        // - Resource cache.
        // - Script manager.
        // - Event manager.
        // - Register the core game events.
        // - Load startup script.
        // - Window manager.
        // - Game views.
        // - OpenGL/DirectX Context.
        //
        // This setup code is not in the constructor since the initialization
        // performed here is so complex and vital to the application, its
        // better to keep it out of the constructor as it gives us more flexibility
        // when dealing with errors.  In a constructor, the only thing you can do
        // really is throw an exception, or log it.
        //
        // @return bool True on success and false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool Initialize();

        // /////////////////////////////////////////////////////////////////
        // Check if the game is still running.
        //
        // /////////////////////////////////////////////////////////////////
        bool IsRunning();

        // /////////////////////////////////////////////////////////////////
        // Set if the game should still be running.
        //
        // /////////////////////////////////////////////////////////////////
        void SetRunning(const bool isRunning);

        // /////////////////////////////////////////////////////////////////
        // Get the number of frames generated so far in the current second.
        //
        // /////////////////////////////////////////////////////////////////
        U32 GetFrameCount() const;

        // /////////////////////////////////////////////////////////////////
        // Get the number of frames drawn to the screen in the past second.
        //
        // /////////////////////////////////////////////////////////////////
        U32 GetFramesInPastSecond() const;

        // /////////////////////////////////////////////////////////////////
        // Get the time we started the games main or animation loop.
        //
        // @return F64 The start time of the game loop.
        //
        // /////////////////////////////////////////////////////////////////
        F64 GetAnimationLoopStartTime() const;

        // /////////////////////////////////////////////////////////////////
        // Get the current time since we started the game loop.
        //
        // @return F64 The number of seconds since the main or animation
        //                  loop started.
        //
        // /////////////////////////////////////////////////////////////////
        F64 GetCurrTime();

        // /////////////////////////////////////////////////////////////////
        // The main loop of the game.  Renders the scene and polls continuously
        // for input events.  This method will only return when the running
        // flag is unset so be sure to poll for and unset the running flag in
        // all derived classes.  This method also regulates the frame rate of
        // the game and calculates the fps.
        //
        // /////////////////////////////////////////////////////////////////
        void Main();

        // /////////////////////////////////////////////////////////////////
        // Get a pointer to the WindowManager.
        //
        // /////////////////////////////////////////////////////////////////
        boost::shared_ptr<WindowManager> GetWindowManager() {
            return (m_windowManagerPtr);
        };

        // /////////////////////////////////////////////////////////////////
        // Get a pointer to the global resource cache.
        //
        // /////////////////////////////////////////////////////////////////
        boost::shared_ptr<ResCache> GetResourceCache() {
            return (m_resourceCachePtr);
        };

        // /////////////////////////////////////////////////////////////////
        // Get a pointer to the global LUA scripting state manager.
        //
        // /////////////////////////////////////////////////////////////////
        boost::shared_ptr<LuaStateManager> GetLuaStateManager() {
            return (m_luaStateManagerPtr);
        };

        // /////////////////////////////////////////////////////////////////
        // Get a pointer to the global event manager.
        //
        // /////////////////////////////////////////////////////////////////
        boost::shared_ptr<EventManager> GetEventManager() {
            return (m_eventManagerPtr);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the pointer to the logic layer.
        //
        // /////////////////////////////////////////////////////////////////
        boost::shared_ptr<BaseGameLogic> GetLogicPtr() {
            return (m_logicPtr);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the game options.
        //
        // /////////////////////////////////////////////////////////////////
        boost::shared_ptr<GameOptions> GetGameOptions() {
            return (m_optionsPtr);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the logging object.
        //
        // /////////////////////////////////////////////////////////////////
        boost::shared_ptr<GameLog> GetLoggerPtr() {
            return (m_loggerPtr);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the Texture Manager object.
        //
        // /////////////////////////////////////////////////////////////////
        boost::shared_ptr<TextureManager> GetTextureManagerPtr() {
            return (m_texManagerPtr);
        };

        boost::shared_ptr<FontBufferCache> GetFontBufferCachePtr() {
            return (m_fontBufferCachePtr);
        }

        // /////////////////////////////////////////////////////////////////
        // Get the TextureAtlas Manager object.
        //
        // /////////////////////////////////////////////////////////////////
        boost::shared_ptr<TextureAtlasManager> GetAtlasManagerPtr() {
            return (m_atlasPtr);
        };

        // /////////////////////////////////////////////////////////////////
        // Parses a directory and returns a list of all the game files that
        // may be used to load a previously saved game state.
        //
        // This is to be implemented by derived classes so they may search
        // for game specific saved files.
        //
        // @param directoryRef The directory to parse to search for saved
        //                      games.
        // @param fileListRef A list which will be filled up with save game
        //                      files.
        //
        // @return bool True on success if some files were found and false
        //              on error (for example if the directory path was not
        //              a valid existing directory).
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VGetLoadFilenames(const boost::filesystem::path &directoryRef, std::vector<boost::filesystem::path> &fileListRef) = 0;

        // /////////////////////////////////////////////////////////////////
        // Load a game.
        //
        // If the filename is not supplied, then VLoadGame should start a new
        // game by default.
        //
        // @param filenameRef The path of the saved game file.
        //
        // @return bool True on success, false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VLoadGame(const boost::filesystem::path &filenameRef = boost::filesystem::path()) {
            // To be implemented fully by subclasses.
            return (true);
        };

        // /////////////////////////////////////////////////////////////////
        // Save a game.
        //
        // @param saveFilenameRef The path to save the game file to.
        //
        // @return bool True on success and false on failure (for example
        //              if we did not have permission to save to the directory).
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VSaveGame(const boost::filesystem::path &saveFilenameRef) {
            // To be implemented fully by subclasses.
            return (true);
        };

        // /////////////////////////////////////////////////////////////////
        // Called when the application loses or gains window focus.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VOnActiveEvent(const bool focus);

        // /////////////////////////////////////////////////////////////////
        // Called when the user resizes the window.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VOnResize(const I32 width, const I32 height);

        // /////////////////////////////////////////////////////////////////
        // Called when the window needs to be refreshed.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VOnRefresh();

        // /////////////////////////////////////////////////////////////////
        // Sleep for x milliseconds.
        //
        // /////////////////////////////////////////////////////////////////
        void Sleep(const F64 milliseconds);

        // /////////////////////////////////////////////////////////////////
        // Called when the application detects a joystick is connected or
        // plugged out.
        //
        // @param joyId The ID of the joystick.
        // @param state Was the joystick detected or lost?
        // @param numAxes The number of axes.
        // @param numButtons The number of buttons.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VOnJoystickStateChange(const I32 joyId, const I32 state, const I32 numAxes, const I32 numButtons);

        // /////////////////////////////////////////////////////////////////
        // Poll all joysticks for state changes and input events.
        //
        // /////////////////////////////////////////////////////////////////
        void PollJoysticks();

        // /////////////////////////////////////////////////////////////////
        // Get the game root directory.
        //
        // /////////////////////////////////////////////////////////////////
        boost::filesystem::path GetGameRootDir() const {
            return (m_gameRootDir);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the global data directory.
        //
        // /////////////////////////////////////////////////////////////////
        boost::filesystem::path GetDatatDir() const {
            return (m_dataDir);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the application specific directory.
        //
        // /////////////////////////////////////////////////////////////////
        boost::filesystem::path GetAppDataDir() const {
            return (m_appDataDir);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the global LUA scripts directory.
        //
        // /////////////////////////////////////////////////////////////////
        boost::filesystem::path GetLuaDataDir() const {
            return (m_luaCommonDir);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the save game directory.
        //
        // /////////////////////////////////////////////////////////////////
        boost::filesystem::path GetSaveGameDir() const {
            return (m_saveGameDir);
        };

        // /////////////////////////////////////////////////////////////////
        // ****************** GLFW/OS input event handlers *****************
        //
        // These methods are an intermediate layer between GLFW and the
        // application.  They take input events from GLFW and convert them
        // into game OS events.  Finally these events are added to the
        // applications global OS event queue.
        //
        // /////////////////////////////////////////////////////////////////

        // /////////////////////////////////////////////////////////////////
        // Handler for the close window event when the user requests
        // to quit the application by for example, clicking on the close icon.
        //
        // /////////////////////////////////////////////////////////////////
        void OnGlfwCloseWindowEvent();

        // /////////////////////////////////////////////////////////////////
        // Handler for the window resize event type.
        //
        // @param width The new width of the window.
        // @param height The new height of the window.
        //
        // /////////////////////////////////////////////////////////////////
        void OnGlfwResizeWindowEvent(const I32 width, const I32 height);

        // /////////////////////////////////////////////////////////////////
        // Handler for the window refresh event type.
        //
        // /////////////////////////////////////////////////////////////////
        void OnGlfwRefreshWindowEvent();

        // /////////////////////////////////////////////////////////////////
        // Handler for a key press/release event.
        //
        // @param keyId The ID of the key.
        // @param action The action (GLFW_PRESS or GLFW_RELEASE).
        //
        // /////////////////////////////////////////////////////////////////
        void OnGlfwKeyEvent(const I32 keyId, const I32 action);

        // /////////////////////////////////////////////////////////////////
        // Handler for a key character press/release event.
        //
        // @param characterId The ID of the character.
        // @param action The action (GLFW_PRESS or GLFW_RELEASE).
        //
        // /////////////////////////////////////////////////////////////////
        void OnGlfwKeyCharacterEvent(const I32 characterId, const I32 action);

        // /////////////////////////////////////////////////////////////////
        // Handler for a mouse button press/release event.
        //
        // @param buttonId The ID of the button.
        // @param action The action (GLFW_PRESS or GLFW_RELEASE).
        //
        // /////////////////////////////////////////////////////////////////
        void OnGlfwMouseButtonEvent(const I32 buttonId, const I32 action);

        // /////////////////////////////////////////////////////////////////
        // Handler for a mouse move event.
        //
        // @param x The screen x position of the mouse.
        // @param y The screen y position of the mouse.
        //
        // /////////////////////////////////////////////////////////////////
        void OnGlfwMouseMoveEvent(const I32 x, const I32 y);

#ifdef USE_NEW_GLFW
        // /////////////////////////////////////////////////////////////////
        // Handler for a mouse wheel event.
        //
        // @param x X scroll offset.
        // @param y Y scroll offset.
        //
        // /////////////////////////////////////////////////////////////////
        void OnGlfwMouseWheelEvent(const I32 x, const I32 y);
#else
        // /////////////////////////////////////////////////////////////////
        // Handler for a mouse wheel event.
        //
        // @param position The new position of the mouse wheel.
        //
        // /////////////////////////////////////////////////////////////////
        void OnGlfwMouseWheelEvent(const I32 position);
#endif

        // /////////////////////////////////////////////////////////////////
        // Handler for a window active event.
        //
        // @param active Did the window gain or lose focus.
        //
        // /////////////////////////////////////////////////////////////////
        void OnGlfwActiveWindowEvent(const I32 active);

        // /////////////////////////////////////////////////////////////////
        // Handler for a joystick detection event.
        //
        // @param joyId The ID of the joystick whose state has changed.
        // @param status The status of the joystick (detected/lost).
        // @param numAxes The number of axes in the detected joystick.
        // @param numButtons The number of buttons in the detected joystick.
        //
        // /////////////////////////////////////////////////////////////////
        void OnGlfwJoystickStatusChangeEvent(const I32 joyId, const I32 status, const I32 numAxes, const I32 numButtons);

        // /////////////////////////////////////////////////////////////////
        // Handler for a joystick move event.
        //
        // @param joyId The ID of the joystick which moved.
        // @param axisId The ID of the axis that moved.
        // @param position The current position of the axis.
        //
        // /////////////////////////////////////////////////////////////////
        void OnGlfwJoystickMoveEvent(const I32 joyId, const I32 axisId, const F32 position);

        // /////////////////////////////////////////////////////////////////
        // Handler for a joystick button event.
        //
        // @param joyId The ID of the joystick which had a button event.
        // @param buttonId The ID of the button.
        // @param buttonState The press/release state of the button.
        //
        // /////////////////////////////////////////////////////////////////
        void OnGlfwJoystickButtonEvent(const I32 joyId, const I32 buttonId, const I32 buttonState);

    };

    // /////////////////////////////////////////////////////////////////
    // GLFW input/window callback events.  The GLFW lib will call these
    // methods when it detects these events from the OS.
    //
    // These methods will then use the global application pointer
    // g_appPtr defined below, to trigger the appropriate events in the
    // main application class which will itself send relevant events
    // to the games subsystems.
    //
    // /////////////////////////////////////////////////////////////////
#ifdef USE_NEW_GLFW
    void GLFW_FUNC_PRE OnGlfwKeyCallback(GLFWwindow window, I32 key, I32 action);
    void GLFW_FUNC_PRE OnGlfwKeyCharacterCallback(GLFWwindow window, I32 character);
    void GLFW_FUNC_PRE OnGlfwMouseButtonCallback(GLFWwindow window, I32 button, I32 action);
    void GLFW_FUNC_PRE OnGlfwMouseMoveCallback(GLFWwindow window, I32 x, I32 y);
    void GLFW_FUNC_PRE OnGlfwMouseWheelCallback(GLFWwindow window, I32 posX, I32 posY);
    I32  GLFW_FUNC_PRE OnGlfwWindowCloseCallback(GLFWwindow window);
    void GLFW_FUNC_PRE OnGlfwWindowResizeCallback(GLFWwindow window, I32 width, I32 height);
    void GLFW_FUNC_PRE OnGlfwWindowRefreshCallback(GLFWwindow window);
#else
    void GLFW_FUNC_PRE OnGlfwKeyCallback(I32 key, I32 action);
    void GLFW_FUNC_PRE OnGlfwKeyCharacterCallback(I32 character, I32 action);
    void GLFW_FUNC_PRE OnGlfwMouseButtonCallback(I32 button, I32 action);
    void GLFW_FUNC_PRE OnGlfwMouseMoveCallback(I32 x, I32 y);
    void GLFW_FUNC_PRE OnGlfwMouseWheelCallback(I32 pos);
    I32  GLFW_FUNC_PRE OnGlfwWindowCloseCallback(void);
    void GLFW_FUNC_PRE OnGlfwWindowResizeCallback(I32 width, I32 height);
    void GLFW_FUNC_PRE OnGlfwWindowRefreshCallback(void);
#endif

    // /////////////////////////////////////////////////////////////////
    // Convert from orthographic to GLFW compatible window coordinates.
    //
    // Assumes the use of the ortho matrix:
    // BuildOrthographicMatrix4(mat, 0.0f, screenW, screenH, 0.0f, -1.0f, 1.0f)
    //
    // @param pt The point to convert.
    //
    // /////////////////////////////////////////////////////////////////
    void ConvertWindowCoordinates(Point3 &pt);

}

// The global application pointer.
extern GameHalloran::GameMain *g_appPtr;

//#define FORCE_LOGGING 1

// Defines to compile out logging on release build
#if defined(DEBUG) || defined(FORCE_LOGGING)

#ifndef GF_LOG_ERR
#define GF_LOG_ERR(msg) if(g_appPtr) { SafeGameLog(g_appPtr->GetLoggerPtr(), GameLog::ERR, msg); }
#endif
#ifndef GF_LOG_TRC
#define GF_LOG_TRC(msg) if(g_appPtr) { SafeGameLog(g_appPtr->GetLoggerPtr(), GameLog::TRC, msg); }
#endif
#ifndef GF_LOG_INF
#define GF_LOG_INF(msg) if(g_appPtr) { SafeGameLog(g_appPtr->GetLoggerPtr(), GameLog::INF, msg); }
#endif
#ifndef GF_LOG_FAT
#define GF_LOG_FAT(msg) if(g_appPtr) { SafeGameLog(g_appPtr->GetLoggerPtr(), GameLog::FAT, msg); }
#endif
#ifndef GF_LOG_DEB
#define GF_LOG_DEB(msg) if(g_appPtr) { SafeGameLog(g_appPtr->GetLoggerPtr(), GameLog::DEB, msg); }
#endif

#ifndef GF_LOG_TRACE_ERR
#define GF_LOG_TRACE_ERR(trc,msg) if(g_appPtr) { SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, trc, msg); }
#endif
#ifndef GF_LOG_TRACE_TRC
#define GF_LOG_TRACE_TRC(trc,msg) if(g_appPtr) { SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::TRC, trc, msg); }
#endif
#ifndef GF_LOG_TRACE_INF
#define GF_LOG_TRACE_INF(trc,msg) if(g_appPtr) { SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::INF, trc, msg); }
#endif
#ifndef GF_LOG_TRACE_FAT
#define GF_LOG_TRACE_FAT(trc,msg) if(g_appPtr) { SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::FAT, trc, msg); }
#endif
#ifndef GF_LOG_TRACE_DEB
#define GF_LOG_TRACE_DEB(trc,msg) if(g_appPtr) { SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::DEB, trc, msg); }
#endif

#else

#ifndef GF_LOG_ERR
#define GF_LOG_ERR(msg)
#endif
#ifndef GF_LOG_TRC
#define GF_LOG_TRC(msg)
#endif
#ifndef GF_LOG_INF
#define GF_LOG_INF(msg)
#endif
#ifndef GF_LOG_FAT
#define GF_LOG_FAT(msg)
#endif
#ifndef GF_LOG_DEB
#define GF_LOG_DEB(msg)
#endif

#ifndef GF_LOG_TRACE_ERR
#define GF_LOG_TRACE_ERR(trc,msg)
#endif
#ifndef GF_LOG_TRACE_TRC
#define GF_LOG_TRACE_TRC(trc,msg)
#endif
#ifndef GF_LOG_TRACE_INF
#define GF_LOG_TRACE_INF(trc,msg)
#endif
#ifndef GF_LOG_TRACE_FAT
#define GF_LOG_TRACE_FAT(trc,msg)
#endif
#ifndef GF_LOG_TRACE_DEB
#define GF_LOG_TRACE_DEB(trc,msg)
#endif

#endif // END LOG DEFINES

#endif
