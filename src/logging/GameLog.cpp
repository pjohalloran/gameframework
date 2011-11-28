// /////////////////////////////////////////////////////////////////
// @file GameLog.cpp
// @author PJ O Halloran
// @date 23/11/2009
//
// File contains the implementation of the GameLog class.
//
// /////////////////////////////////////////////////////////////////

// External headers
#include <exception>
#include <stdexcept>
#include <fstream>
#include <string>
#include <cstdio>
#include <ctime>

// Project headers
#include "GameException.h"
#include "GameLog.h"

// Namespace Declarations
using std::ofstream;
using std::endl;
using std::string;
using std::exception;

using boost::filesystem::path;

namespace GameHalloran
{
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	GameLog::GameLog(const path &filenameRef, const LogLevel initialLevel, const bool timestamp) 
		throw (GameException &)	: m_logger(), m_logLevel(initialLevel), m_timestamp(timestamp)
	{
		m_logger.open(filenameRef.string().c_str());
		if(!m_logger.is_open())
		{
			throw (GameException(string("Failed to open the log file.")));
		}
		
		Log(FAT, string("Start"));
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	GameLog::~GameLog()
	{
		try
		{
			Log(FAT, string("End"));
			m_logger.close();
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
	string GameLog::GetTimeStamp() const
	{
		time_t rawtime;				// Raw system time.
		struct tm * timeinfo;		// More detailed time struct.
		string timestamp("");		// timestamp to return.

		// Get the time.
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		timestamp.assign(string(asctime(timeinfo)));
		
		// If last character is an endline, remove it.
		if(timestamp.substr(timestamp.size()-1, timestamp.size()-1).compare("\n") == 0)
		{
			timestamp.erase(timestamp.size()-1);
		}
		
		return (timestamp);
	}
	
	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameLog::Log(const LogLevel level, const string &messageRef)
	{
		if(!m_logger.is_open())
		{
			return;
		}
		
		// Check if message should be logged.
		if(level <= m_logLevel)
		{
			// Create highlight text to make ERR or FAT logs stand out.
			string highlightText("");
			if(level == FAT)
			{
				highlightText.assign(string(" #### "));
			}
			else if(level == ERR)
			{
				highlightText.assign(string(" **** "));
			}
			
			// Log error.
			if(m_timestamp)
			{
				m_logger << GetTimeStamp() << ": " << highlightText << messageRef << "." << highlightText << endl;
			}
			else
			{
				m_logger << highlightText << messageRef << "." << highlightText << endl;
			}
		}
	}
	
	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GameLog::LogAndPrefix(const LogLevel level, const string &prefixRef, const string &messageRef)
	{
		Log(level, prefixRef + string(": ") + messageRef);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameLog::LogTrace(const std::string &prefixRef, const std::string &messageRef)
	{
		LogAndPrefix(TRC, prefixRef, messageRef);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	GameLog::LogLevel GameLog::GetLogLevel() const
	{
		return (m_logLevel);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GameLog::SetLogLevel(const LogLevel newLevel)
	{
		m_logLevel = newLevel;
	}
}
