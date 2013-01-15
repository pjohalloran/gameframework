// /////////////////////////////////////////////////////////////////////////////
// @file main.cpp
// @author PJ O Halloran
// @date 13/01/2013
//
// Game main entry point.
//
// /////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#ifdef _WINDOWS
#	include <crtdbg.h>
#endif

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "GameBase.h"
#include "GameLog.h"
#include "GameException.h"
#include "GameOptions.h"
#include "TestApp.h"

bool CheckGameRootDir(boost::shared_ptr<GameHalloran::GameOptions> &optionsPtr, boost::filesystem::path &rootPath) {
	if(!optionsPtr || !boost::filesystem::is_directory(rootPath)) {
		std::cerr << "CheckGameRootDir(): Invalid parameters." << std::endl;
		return (false);
	}

	if(!optionsPtr->IsOptionsFileLoaded()) {
		try {
			boost::filesystem::path opPath(rootPath.string() + std::string("/data/TestApp/options.xml"));
			optionsPtr->ParseFile(opPath);
		}
		catch(GameHalloran::GameException &ge) {
			std::cerr << "CheckGameRootDir(): " << ge.what() << std::endl;
			return (false);
		}
	}

	std::string opRootDir;
	if(GameHalloran::RetrieveAndConvertOption<std::string>(optionsPtr, std::string("GameRoot"), GameHalloran::GameOptions::PROGRAMMER, opRootDir)) {
		boost::filesystem::path opRootPath(opRootDir);
		if(!boost::filesystem::is_directory(opRootPath) || opRootPath.string().compare(rootPath.string().c_str()) != 0) {
			std::cout << "Setting the root directory to " << rootPath.string() << std::endl;
			optionsPtr->Edit(std::string("GameRoot"), rootPath.string(), GameHalloran::GameOptions::PROGRAMMER);

#ifndef _DEBUG
			optionsPtr->Commit();
#endif
		}
	}

	return (true);
}

int main(int args, char *argv[]) {
	using GameHalloran::GameOptions;
	using GameHalloran::GameLog;
	using GameHalloran::GameException;
    using GameHalloran::TestApp;
	using boost::shared_ptr;
	using boost::filesystem::path;

	shared_ptr<GameOptions> optionsPtr;
	shared_ptr<GameLog> logPtr;
	shared_ptr<TestApp> gamePtr;
	int value = 0;

#if defined(DEBUG)
#	if defined(_WINDOWS)
		int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
		tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;				// this flag will cause intermittent pauses in the game!
		//tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;				// remember this is VERY VERY SLOW!
		tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
		_CrtSetDbgFlag(tmpDbgFlag);
#	else
		std::cout << "You will need to use Valgrind to manually check for memory leaks on this platform!" << std::endl;
#	endif
#endif

	try  {
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
		if(!boost::algorithm::ends_with(rootDirPath.string(), std::string("/")) || !boost::algorithm::ends_with(rootDirPath.string(), std::string("\\"))) {
			rootDirPath = boost::filesystem::path(rootDirPath.string() + std::string("/"));
		}

		path endOp(std::string("data/TestApp/options.xml"));
		path endLog(std::string("log/TestApp.log"));
		path optionsFilePath(rootDirPath.string() + endOp.string());
		path logFilePath(rootDirPath.string() + endLog.string());

        if(!boost::filesystem::exists(rootDirPath.string() + std::string("log/"))) {
            boost::filesystem::create_directory(boost::filesystem::path(rootDirPath.string() + std::string("log/")));
        }
        
		std::cout << "optionsFilePath: " << optionsFilePath.string() << std::endl;
		std::cout << "logFilePath: " << logFilePath.string() << std::endl;

		logPtr = shared_ptr<GameLog>(GCC_NEW GameLog(logFilePath, GameLog::DEB, true));
		optionsPtr = shared_ptr<GameOptions>(GCC_NEW GameOptions(logPtr, optionsFilePath));

		bool error = false;

		if(!CheckGameRootDir(optionsPtr, rootDirPath)) {
			error = true;
			value = 1;
		}

		if(!error) {
			gamePtr = shared_ptr<TestApp>(GCC_NEW TestApp(logPtr, optionsPtr));

			if(!gamePtr->Initialize()) {
				error = true;
				std::cerr << "Failed to initialize the app class, check the log file..." << std::endl;
				value = 1;
			}

			if(!error) {
				gamePtr->Main();
			}
		}
	}
	catch(GameException &ge) {
        GF_LOG_FAT(ge.what());
		std::cerr << "Exception was caught in main(): " << ge.what() << std::endl;
		value = 1;
	}

	gamePtr.reset();
	optionsPtr.reset();
	logPtr.reset();

	return (value);
}
