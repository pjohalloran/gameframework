// /////////////////////////////////////////////////////////////////////////////
// @file pool3d.cpp
// @author PJ O Halloran
// @date 01/11/2010
//
// Pool3D game main entry point.
//
// /////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#ifdef _WINDOWS
#   include <crtdbg.h>
#endif

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "GameBase.h"
#include "GameLog.h"
#include "GameException.h"
#include "GameOptions.h"
#include "Pool3dGame.h"

// /////////////////////////////////////////////////////////////////////////////
// Check if the actual game root directory matches that of the game root stored
// in the options file and updates it if nessecary.  This might have changed if
// the player moved the game directory somewhere else for example.
//
// /////////////////////////////////////////////////////////////////////////////
bool CheckGameRootDir(boost::shared_ptr<GameHalloran::GameOptions> &optionsPtr, boost::filesystem::path &rootPath)
{
    // Validate parameters
    if(!optionsPtr || !boost::filesystem::is_directory(rootPath)) {
        std::cerr << "CheckGameRootDir(): Invalid parameters." << std::endl;
        return (false);
    }

    // Ensure options file is loaded into memory.
    if(!optionsPtr->IsOptionsFileLoaded()) {
        try {
            boost::filesystem::path opPath(rootPath.string() + std::string("/data/Pool3D/options.xml"));
            optionsPtr->ParseFile(opPath);
        } catch(GameHalloran::GameException &ge) {
            std::cerr << "CheckGameRootDir(): " << ge.what() << std::endl;
            return (false);
        }
    }

    // Get current root directory as stored in options file.
    std::string opRootDir;
    if(GameHalloran::RetrieveAndConvertOption<std::string>(optionsPtr, std::string("GameRoot"), GameHalloran::GameOptions::PROGRAMMER, opRootDir)) {
        boost::filesystem::path opRootPath(opRootDir);
        if(!boost::filesystem::is_directory(opRootPath) || opRootPath.string().compare(rootPath.string().c_str()) != 0) {
            // Save the root directory path to the options file.
            std::cout << "Setting the root directory to " << rootPath.string() << std::endl;
            optionsPtr->Edit(std::string("GameRoot"), rootPath.string(), GameHalloran::GameOptions::PROGRAMMER);

            // We handle a game root change here by saving it to the options file (release build only).
#ifndef _DEBUG
            optionsPtr->Commit();
#endif
        }
    }

    return (true);
}

// /////////////////////////////////////////////////////////////////////////////
// Main application entry point.
//
// @param args Number of command line arguments.
// @param argv Command line arguments.
//
// /////////////////////////////////////////////////////////////////////////////
int main(int args, char *argv[])
{
    using GameHalloran::GameOptions;
    using GameHalloran::GameLog;
    using GameHalloran::GameException;
    using GameHalloran::Pool3dGame;
    using boost::shared_ptr;
    using boost::filesystem::path;

    shared_ptr<GameOptions> optionsPtr;         // Game options manager.
    shared_ptr<GameLog> logPtr;                 // Logger.
    shared_ptr<Pool3dGame> gamePtr;             // Game application class (starts up the logic and view layers and manages the game subsystems).
    int value = 0;                              // Main function return value.

#if defined(DEBUG)
#   if defined(_WINDOWS)
    // Perform a memory leak check on exit when we are running on Windows
    int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    // set this flag to keep memory blocks around
    tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;                // this flag will cause intermittent pauses in the game!
    //// perform memory check for each alloc/dealloc
    //tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;                // remember this is VERY VERY SLOW!
    //_CRTDBG_LEAK_CHECK_DF is used at program initialization to force a
    //   leak check just before program exit. This is important because
    //   some classes may dynamically allocate memory in globally constructed
    //   objects.
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);
#   else
    std::cout << "You will need to use Valgrind to manually check for memory leaks on this platform!" << std::endl;
#   endif
#endif

    try {
#if defined(DEBUG)
        // To handle the current $GAMEROOT path for debugging comment in or out the following path set.

#if defined(WIN32)
        // 1) Include this for debugging in Visual Studio (exe run from inside $GAMEROOT/src/Pool3d/)
        path rootDirPath = boost::filesystem::current_path().parent_path().parent_path();
#elif defined(TARGET_OS_MAC)
        path rootDirPath = boost::filesystem::current_path().parent_path();
        // or 2) Include this for running debug builds from the command line or from a profiler (Sleepy/Gdebugger).
        //path rootDirPath = boost::filesystem::current_path().parent_path();
#endif
#else
        path rootDirPath = boost::filesystem::current_path().parent_path();
#endif
        // Ensure path ends with trailing dir seperator.
        if(!boost::algorithm::ends_with(rootDirPath.string(), std::string("/")) || !boost::algorithm::ends_with(rootDirPath.string(), std::string("\\"))) {
            rootDirPath = boost::filesystem::path(rootDirPath.string() + std::string("/"));
        }

        path endOp(std::string("data/Pool3D/options.xml"));
        path endLog(std::string("log/Pool3D.log"));
        path optionsFilePath(rootDirPath.string() + endOp.string());
        path logFilePath(rootDirPath.string() + endLog.string());

        // Ensure the log directory exists!
        if(!boost::filesystem::exists(rootDirPath.string() + std::string("log/"))) {
            boost::filesystem::create_directory(boost::filesystem::path(rootDirPath.string() + std::string("log/")));
        }

        std::cout << "optionsFilePath: " << optionsFilePath.string() << std::endl;
        std::cout << "logFilePath: " << logFilePath.string() << std::endl;

        logPtr = shared_ptr<GameLog>(GCC_NEW GameLog(logFilePath, GameLog::DEB, true));
        optionsPtr = shared_ptr<GameOptions>(GCC_NEW GameOptions(logPtr, optionsFilePath));

        bool error = false;

        // Get the current game root.  If its different from the actual game root log this and change it and commit the change to the options file.
        if(!CheckGameRootDir(optionsPtr, rootDirPath)) {
            error = true;
            value = 1;
        }

        if(!error) {
            gamePtr = shared_ptr<Pool3dGame>(GCC_NEW Pool3dGame(logPtr, optionsPtr));

            // Initialize the App class (and the various game sub system managers).
            if(!gamePtr->Initialize()) {
                error = true;
                std::cerr << "Failed to initialize the app class, check the log file..." << std::endl;
                value = 1;
            }

            if(!error) {
                // Enter main game loop.
                gamePtr->Main();
            }
        }
    } catch(GameException &ge) {
        GF_LOG_FAT(ge.what());
        std::cerr << "Exception was caught in main(): " << ge.what() << std::endl;
        value = 1;
    }

    // Explicitly free global game objects in reverse order to how they were allocated.
    gamePtr.reset();
    optionsPtr.reset();
    logPtr.reset();

    return (value);
}
