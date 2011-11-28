// /////////////////////////////////////////////////////////////////
// @file SystemCheck.cpp
// @author PJ O Halloran
// @date 25/06/2010
//
// File contains the implementation of the SystemCheck class.
//
// /////////////////////////////////////////////////////////////////

// External headers
#include <cstring>
#include <iostream>

#include <boost/shared_ptr.hpp>

// Project headers
#include "SystemCheck.h"
#include "GameBase.h"

// Namespace Declarations


namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// *********************** SystemCheck ****************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool SystemCheck::CheckHardDiskSpace(const I64 minFreeSpace)
	{
        bool result = false;						// Result of method.
#ifdef _WINDOWS
		I32 const drive = _getdrive();			// HD drive ID/number.
		struct _diskfree_t diskfree;			// Disk check struct.

		memset((void *)&diskfree, 0, sizeof(diskfree));

		_getdiskfree(drive, &diskfree);

		unsigned __int64 const neededClusters = 
			minFreeSpace /(diskfree.sectors_per_cluster*diskfree.bytes_per_sector);

		if(diskfree.avail_clusters < neededClusters)
		{
			result = false;
		}
#endif

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool SystemCheck::CheckCpuSpeed(const I64 minSpeed)
	{
		// Cannot compile the GetCpuSpeed.cpp file.  Google to see what
		// this system is missing...
		// unused bool result = true;

#ifdef _WINDOWS
		std::cerr << "No CPU check implemented yet for the Windows platform!" << std::endl;
		// unused result = false;
#else
		std::cerr << "No CPU check implemented yet for this platform!" << std::endl;
		// unused result = false;
#endif

		// TODO: Implement.
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool SystemCheck::CheckPhysicalMemory(const I64 minFreeSRam)
	{
		bool result = true;

#ifdef _WINDOWS
		MEMORYSTATUSEX status;

		GlobalMemoryStatusEx(&status);

		if(status.ullTotalPhys < minFreeSRam)
		{
			result = false;
		}
#else
		// TODO: Implement for other platforms...
		std::cerr << "No RAM check implemented yet for this platform!" << std::endl;
		result = false;
#endif

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool SystemCheck::CheckVideoMemory(const I64 minFreeVRam)
	{
		// TODO: Implement with OpenGL.
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool SystemCheck::CheckForJoysticks(GfJoyCont &joystickList, I32 &numJoysticksDetected)
	{
		// NB This code uses GLFW to check for joysticks which is a 
		//  cross platform window library for OpenGL applications.

		// Ensure the list is cleared before we proceed.
		if(!joystickList.empty())
		{
			joystickList.clear();
		}
		numJoysticksDetected = 0;

		// Iterate through all josticks that GLFW supports
		for(I32 i = 0; i <= GLFW_JOYSTICK_LAST; ++i)
		{
			JoystickInfoPtr currJoystickPtr(GCC_NEW JoystickInfo(i));
			if(currJoystickPtr->IsDetected())
			{
				++numJoysticksDetected;
			}
			joystickList.push_back(currJoystickPtr);
		}

		return (numJoysticksDetected > 0);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SystemCheck::CheckVirtualMemory(const I64 minFreeVirtualMemory)
	{
		bool result = true;
#ifdef _WINDOWS
		MEMORYSTATUSEX status;

		GlobalMemoryStatusEx(&status);

		if(status.ullTotalVirtual < minFreeVirtualMemory)
		{
			result = false;
		}
#else
		std::cout << "No Virtual memory available on this platform!" << std::endl;
#endif

		return (result);
	}

}
