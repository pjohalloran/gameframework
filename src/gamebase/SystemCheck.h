#pragma once
#ifndef __SYSTEM_CHECK_H
#define __SYSTEM_CHECK_H

// /////////////////////////////////////////////////////////////////
// @file SystemCheck.h
// @author PJ O Halloran
// @date 25/06/2010
//
// File contains the header for the various SystemCheck classes.
//
// /////////////////////////////////////////////////////////////////

#include "GameBase.h"
#include "OsInputEvents.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class SystemCheck
	// @author PJ O Halloran
	//
	// A series of functions that checks system resources.
	//
	// /////////////////////////////////////////////////////////////////
	class SystemCheck
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		SystemCheck() { };

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		~SystemCheck() { };

		// /////////////////////////////////////////////////////////////////
		// Check if there is enough hard disk space available.
		//
		// @param minFreeSpace The number of bytes at a minimum that must be
		//						free for the application to run.
		//
		// @return bool True if there is the required space available on the
		//					HD.
		//
		// /////////////////////////////////////////////////////////////////
		bool CheckHardDiskSpace(const U64 minFreeSpace);

		// /////////////////////////////////////////////////////////////////
		// Check if the CPU is fast enough to run the application.
		//
		// @param minSpeed The minimum CPU speed required to run the app.
		//
		// @return bool True if the CPU is faster than the minimum specified.
		//
		// /////////////////////////////////////////////////////////////////
		bool CheckCpuSpeed(const U64 minSpeed);

		// /////////////////////////////////////////////////////////////////
		// Check if there is enough available system RAM on the system to run
		// the application.
		//
		// @param minFreeSRam The minimum RAM in bytes for the application
		//							to run.
		//
		// @return bool True if the RAM available is greater than the minimum
		//				specified.
		//
		// /////////////////////////////////////////////////////////////////
		bool CheckPhysicalMemory(const U64 minFreeSRam);

		// /////////////////////////////////////////////////////////////////
		// Check if there is enough available video RAM on the system to run
		// the application.
		//
		// @param minFreeVRam The minimum video RAM in bytes for the 
		//						application to run.
		//
		// @return bool True if the video RAM available is greater than the
		//				minimum specified.
		//
		// /////////////////////////////////////////////////////////////////
		bool CheckVideoMemory(const U64 minFreeVRam);

		// /////////////////////////////////////////////////////////////////
		// Check for all joysticks detected by the system.
		//
		// @param joystickList Container to store a list of all available
		//						joysticks found inside.
		// @param numJoysticksDetected The number of joysticks detected.
		//
		// @return bool True if any joysticks were detected.
		//
		// /////////////////////////////////////////////////////////////////
		bool CheckForJoysticks(GfJoyCont &joystickList, U32 &numJoysticksDetected);

		// /////////////////////////////////////////////////////////////////
		// Check if there is enough available virtual memory on the system to
		// run the application.  This method will do nothing and return true
		// on platforms other than Windows as Virtual memory is a windows only
		// issue.
		//
		// @param minFreeSRam The minimum virtual memory in bytes for the
		//						application to run.
		//
		// @return bool True if the virtual memory available is greater 
		//				than the minimum specified.
		//
		// /////////////////////////////////////////////////////////////////
		bool CheckVirtualMemory(const U64 minFreeVirtualMemory);

	};
}

#endif
