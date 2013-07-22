// /////////////////////////////////////////////////////////////////
// @file SystemCheck.cpp
// @author PJ O Halloran
// @date 25/06/2010
//
// File contains the implementation of the SystemCheck class.
//
// /////////////////////////////////////////////////////////////////

#include <cstring>
#include <iostream>

#include "SystemCheck.h"
#include "GameBase.h"

#if defined(__linux__) || defined(__unix__) || defined(TARGET_OS_MAC) || defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
#include <sys/statvfs.h>
#endif

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // *********************** SystemCheck ****************************
    // /////////////////////////////////////////////////////////////////

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SystemCheck::CheckHardDiskSpace(const U64 minFreeSpace)
    {
        bool result = false;                        // Result of method.

#ifdef _WINDOWS
        I32 const drive = _getdrive();          // HD drive ID/number.
        struct _diskfree_t diskfree;            // Disk check struct.

        memset((void *)&diskfree, 0, sizeof(diskfree));

        _getdiskfree(drive, &diskfree);

        unsigned __int64 const neededClusters =
            minFreeSpace / (diskfree.sectors_per_cluster * diskfree.bytes_per_sector);

        result = diskfree.avail_clusters >= neededClusters;
#else
        U64 mb = 0;
        struct statvfs sfs;

        memset((void *)&sfs, 0, sizeof(sfs));

        if(statvfs("/", &sfs) != -1) {
            mb = (U64)sfs.f_bsize * sfs.f_bfree;
        }

        result = mb >= minFreeSpace;
#endif

        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SystemCheck::CheckCpuSpeed(const U64 minSpeed)
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
    bool SystemCheck::CheckPhysicalMemory(const U64 minFreeSRam)
    {
        bool result = true;

#ifdef _WINDOWS
        MEMORYSTATUSEX status;

        GlobalMemoryStatusEx(&status);

        if(status.ullTotalPhys < minFreeSRam) {
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
    bool SystemCheck::CheckVideoMemory(const U64 minFreeVRam)
    {
        // TODO: Implement with OpenGL.
        return (false);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SystemCheck::CheckForJoysticks(GfJoyCont &joystickList, U32 &numJoysticksDetected)
    {
        // NB This code uses GLFW to check for joysticks which is a
        //  cross platform window library for OpenGL applications.

        // Ensure the list is cleared before we proceed.
        if(!joystickList.empty()) {
            joystickList.clear();
        }
        numJoysticksDetected = 0;

        // Iterate through all josticks that GLFW supports
        for(I32 i = 0; i <= GLFW_JOYSTICK_LAST; ++i) {
            JoystickInfoPtr currJoystickPtr(GCC_NEW JoystickInfo(i));
            if(currJoystickPtr->IsDetected()) {
                ++numJoysticksDetected;
            }
            joystickList.push_back(currJoystickPtr);
        }

        return (numJoysticksDetected > 0);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SystemCheck::CheckVirtualMemory(const U64 minFreeVirtualMemory)
    {
        bool result = true;
#ifdef _WINDOWS
        MEMORYSTATUSEX status;

        GlobalMemoryStatusEx(&status);

        if(status.ullTotalVirtual < minFreeVirtualMemory) {
            result = false;
        }
#else
        std::cout << "No Virtual memory available on this platform!" << std::endl;
#endif

        return (result);
    }

}
