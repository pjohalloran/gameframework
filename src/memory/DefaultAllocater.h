#pragma once
#ifndef __DEFAULT_ALLOCATER_H
#define __DEFAULT_ALLOCATER_H

// ////////////////////////////////////////////////////////////
// @file DefaultAllocater.h
// @author PJ O Halloran
// @date 13/10/2011
//
// Default system allocater which uses the C standard librarys
// malloc and free.
//
// ////////////////////////////////////////////////////////////

#include <cstdlib>

#include "GameTypes.h"
#include "GameBase.h"

namespace GameHalloran {

    // ////////////////////////////////////////////////////////////
    // @class DefaultAllocater
    // @author PJ O Halloran
    //
    // Default game allocater.
    //
    // ////////////////////////////////////////////////////////////
    class DefaultAllocater : public BaseGameAllocater {
    private:

    public:

        // ////////////////////////////////////////////////////////////
        // Constructor.
        //
        // ////////////////////////////////////////////////////////////
        DefaultAllocater() {};

        // ////////////////////////////////////////////////////////////
        // Destructor.
        //
        // ////////////////////////////////////////////////////////////
        virtual ~DefaultAllocater() {};

        // ////////////////////////////////////////////////////////////
        // Allocate a block of memory.
        //
        // @param size Size of memory block to allocate.
        //
        // @return void* NULL on error.
        //
        // ////////////////////////////////////////////////////////////
        virtual void *VAlloc(const U64 size) {
            return (malloc((size_t)size));
        };

        // ////////////////////////////////////////////////////////////
        // TODO (not currently implemented)
        //
        // Allocate an aligned block of memory.
        //
        // @param size Size of memory block to allocate.
        // @param alignment The alignment of the block of memory.
        //
        // @return void* NULL on error.
        //
        // ////////////////////////////////////////////////////////////
        virtual void *VAllocAligned(const U64 size, const U32 alignment) {
            return (NULL);
        };

        // ////////////////////////////////////////////////////////////
        // Deallocate a block of memory.
        //
        // ////////////////////////////////////////////////////////////
        virtual void VDealloc(void *ptr) {
            free(ptr);
        };

        // ////////////////////////////////////////////////////////////
        // (not used)
        // Run defragmentation on the allocaters memory block.
        //
        // ////////////////////////////////////////////////////////////
        virtual void VDefrag() {};

        // ////////////////////////////////////////////////////////////
        // (not used)
        // Run a partial defragmentation on the allocaters memory block.
        // Useful when you want to defragment the block but want to spread
        // the workload over a couple of frames.
        //
        // @param seconds The maximum number of seconds to run for.
        //
        // @return F32 Percentage of memory block left to defragment.
        //
        // ////////////////////////////////////////////////////////////
        virtual F32 VDefrag(const F32 seconds) {
            return (0.0f);
        };
    };
}

#endif
