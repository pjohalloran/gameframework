// /////////////////////////////////////////////////////////////////
// @file GameLog.h
// @author PJ O Halloran
// @date 23/11/2009
//
// File contains the header for the GameLog class and some helper
// functions for logging during a game.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GAME_LOG_H
#define __GAME_LOG_H

#ifdef WIN32
#	pragma warning( push )
// Disable exception throw specification warning VS complains about.
//  It doesn't like "throw (somexception)", it likes either "throw()"
//   for function throws nothing or "" for function may throw something.
#	pragma warning( disable:4290 )
#	pragma warning( push )
// Disable depreciated function strcpy() warning as we dont want to use strcpy_s to keep our code 
//  platform independant.
#	pragma warning( disable:4996 )
#endif

// External Headers
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

// Project Headers
#include "GameException.h"


namespace GameHalloran
{
	// /////////////////////////////////////////////////////////////////
	// @class GameLog
	// @author PJ O Halloran
	//
	// This class handles logging and log traces of game/program execution
	// out to a file for debugging.
	// 
	// The user supplies a default logging level to the object upon creation.
	// 
	// All messages at this level or exceeding it in order of importance will
	// be written to the log files while all other messages will be discarded.
	// 
	// All messages of level FAT will be preceeded and terminated with a string
	// ####. e.g. #### Fatal Error Occurred! ####
	//
	// All messages of level ERR will be preceeded and terminated with a string
	// ****. e.g. **** Error Occurred! ****
	//
	// All other messages will appear as normal.
	//
	// Optionally, the log messages may be preceeded by a timestamp if you enable
	// this upon creation ("DD:YYYY - HH:MM:SS:MSMS - Log message" ).
	//
	// /////////////////////////////////////////////////////////////////
	class GameLog
	{
		public:
		
			// /////////////////////////////////////////////////////////////////
			// @enum LogLevel
			//
			// The various log levels the logging object or a log message may 
			// have.
			//
			// /////////////////////////////////////////////////////////////////
			enum LogLevel
			{
				FAT = 1,
				ERR,
				INF,
				DEB,
				TRC
			};
			
		private:
		
			std::ofstream m_logger;							///< Logging standard C++ object.
			LogLevel m_logLevel;							///< The current log level of the logging object.
			bool m_timestamp;								///< Should we timestamp the messages?
			
			// /////////////////////////////////////////////////////////////////
			// Create and return a timestamp string.
			// 
			// /////////////////////////////////////////////////////////////////
			std::string GetTimeStamp() const;
		
		public:
			
			// /////////////////////////////////////////////////////////////////
			// Custom constructor.
			//
			// Creates a logging object.
			//
			// If the file identified by the filename supplied already exists, it
			// will be overwritten by a new copy of the file.
			//
			// @param filenameRef The name of the file to log messages to.
			// @param initialLevel The initial log level of the logging object.
			// @param timestamp Should we prefix all messages with a timestamp?
			//
			// @throw GameException If any errors occur opening the file.
			//
			// /////////////////////////////////////////////////////////////////
			GameLog(const boost::filesystem::path &filenameRef, const LogLevel initialLevel, const bool timestamp) throw (GameException &);
			
			// /////////////////////////////////////////////////////////////////
			// Destructor.
			//
			// /////////////////////////////////////////////////////////////////
			virtual	~GameLog();
			
			// /////////////////////////////////////////////////////////////////
			// Plain log function.  Write string supplied to the file if the
			// current log level of the logging object is at or lower to the
			// level supplied.
			//
			// @param level The level to log the message as.
			// @param messageRef The log message.
			//
			// /////////////////////////////////////////////////////////////////
			void Log(const LogLevel level, const std::string &messageRef);
			
			// /////////////////////////////////////////////////////////////////
			// Log a message to the file.  The log message will be prefixed
			// in the file by the prefix string supplied.
			//
			// e.g. "init(): Entering."
			//
			// @param level The log level of the message.
			// @param prefixRef The log prefix.
			// @param messageRef The log message.
			//
			// /////////////////////////////////////////////////////////////////
			void LogAndPrefix(const LogLevel level, const std::string &prefixRef, const std::string &messageRef);
			
			// /////////////////////////////////////////////////////////////////
			// Log a trace message to the file.  The log message will be prefixed
			// in the file by the prefix string supplied.  Log messages of this type
			// will always have a log level of TRC.  Its useful for a stack trace.
			//
			// e.g. "init(): Entering."
			//
			// @param prefixRef The log prefix.
			// @param messageRef The log message.
			//
			// /////////////////////////////////////////////////////////////////
			void LogTrace(const std::string &prefixRef, const std::string &messageRef);

			// /////////////////////////////////////////////////////////////////
			// Get the current log level.
			//
			// /////////////////////////////////////////////////////////////////
			LogLevel GetLogLevel() const;
			
			// /////////////////////////////////////////////////////////////////
			// Set the current log level.
			//
			// @param newLevel The new log level.
			//
			// /////////////////////////////////////////////////////////////////
			void SetLogLevel(const LogLevel newLevel);
			
	};

	// /////////////////////////////////////////////////////////////////
	// Provides a pointer safe mechanism to log messages if the class
	// is unsure if the log pointer is valid or not.
	//
	// Calls GameLog::Log() if the log pointer is valid.
	//
	// @param logPtr The pointer to the GameLog object.
	// @param level The log level of the message.
	// @param messageRef The string to log.
	//
	// /////////////////////////////////////////////////////////////////
	inline void SafeGameLog(boost::shared_ptr<GameLog> logPtr, GameLog::LogLevel level, const std::string &messageRef)
	{
		if(!logPtr.get())
		{
			return;
		}

		logPtr->Log(level, messageRef);
	};

	// /////////////////////////////////////////////////////////////////
	// Provides a pointer safe mechanism to log messages if the class
	// is unsure if the log pointer is valid or not.
	//
	// Calls GameLog::LogTrace() if the log pointer is valid.
	//
	// @param logPtr The pointer to the GameLog object.
	// @param prefixRef The string to attach as a prefix.
	// @param messageRef The string to log.
	//
	// /////////////////////////////////////////////////////////////////
	inline void SafeGameLogTrace(boost::shared_ptr<GameLog> logPtr, const std::string prefixRef, const std::string &messageRef)
	{
		if(!logPtr.get())
		{
			return;
		}

		logPtr->LogTrace(prefixRef, messageRef);
	};

	// /////////////////////////////////////////////////////////////////
	// Provides a pointer safe mechanism to log messages if the class
	// is unsure if the log pointer is valid or not.
	//
	// Calls GameLog::LogAndPrefix() if the log pointer is valid.
	//
	// @param logPtr The pointer to the GameLog object.
	// @param level The log level of the message.
	// @param prefixRef The string to attach as a prefix.
	// @param messageRef The string to log.
	//
	// /////////////////////////////////////////////////////////////////
	inline void SafeGameLogAndPrefix(boost::shared_ptr<GameLog> logPtr, GameLog::LogLevel level, const std::string prefixRef, const std::string &messageRef)
	{
		if(!logPtr.get())
		{
			return;
		}

		logPtr->LogAndPrefix(level, prefixRef, messageRef);
	};
}

#ifdef WIN32
#	pragma warning( pop )
#	pragma warning( pop )
#endif

#endif // END _H
