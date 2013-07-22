// ////////////////////////////////////////////////////////////
// @file GameMemory.cpp
// @author PJ O Halloran
// @date 13/10/2011
//
// Memory allocation routines impl.
//
// ////////////////////////////////////////////////////////////

#if defined(_WINDOWS) || defined(WIN32)
#include <malloc.h>
#else
#include <cstdlib>
#endif

#include "GameMemory.h"

namespace GameHalloran {

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void GameMemoryInit()
    {
        // Register all required memory allocaters here.

        // Allow game devs to pass in a function pointer here to set
        // up any custom game specific allocaters here also.
    }

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void GameMemoryShutdown()
    {
        g_memManObj.Clear();
    }

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void *GameDefaultAlloc(const U64 size)
    {
        return (malloc((size_t)size));
    }

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void GameDefaultDealloc(void *ptr)
    {
        free(ptr);
    }

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void *GameAlloc(const U64 size, const enum AllocId area)
    {
        void *block(NULL);

        if(area < DEFAULT_MAX_MEMORY_ALLOCATERS) {
            block = g_memManObj.GetAllocater(area)->VAlloc(size);
        } else {
            block = GameDefaultAlloc(size);
        }

        return (block);
    }

    // ////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////
    void GameDealloc(void *ptr, const enum AllocId area)
    {
        if(area < DEFAULT_MAX_MEMORY_ALLOCATERS) {
            g_memManObj.GetAllocater(area)->VDealloc(ptr);
        } else {
            GameDefaultDealloc(ptr);
        }
    }
}
