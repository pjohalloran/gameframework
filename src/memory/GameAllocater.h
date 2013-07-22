#pragma once
#ifndef __GAME_ALLOCATER_H
#define __GAME_ALLOCATER_H

// ////////////////////////////////////////////////////////////
// @file GameAllocater.h
// @author PJ O Halloran
// @date 13/10/2011
//
// Base interface for all memory allocation managers.
//
// ////////////////////////////////////////////////////////////

#include "GameTypes.h"
#include "GameBase.h"

namespace GameHalloran {

    // ////////////////////////////////////////////////////////////
    // @class IGameAllocater
    // @author PJ O Halloran
    //
    // Base memory allocater interface.
    //
    // ////////////////////////////////////////////////////////////
    class IGameAllocater : public NonCopyable {
    protected:

        // ////////////////////////////////////////////////////////////
        // Destructor.
        //
        // ////////////////////////////////////////////////////////////
        virtual ~IGameAllocater() {};

    public:

        // ////////////////////////////////////////////////////////////
        // Allocate a block of memory.
        //
        // @param size Size of memory block to allocate.
        //
        // @return void* NULL on error.
        //
        // ////////////////////////////////////////////////////////////
        virtual void *VAlloc(const U64 size) = 0;

        // ////////////////////////////////////////////////////////////
        // Allocate an aligned block of memory.
        //
        // @param size Size of memory block to allocate.
        // @param alignment The alignment of the block of memory.
        //
        // @return void* NULL on error.
        //
        // ////////////////////////////////////////////////////////////
        virtual void *VAllocAligned(const U64 size, const U32 alignment) = 0;

        // ////////////////////////////////////////////////////////////
        // Deallocate a block of memory.
        //
        // ////////////////////////////////////////////////////////////
        virtual void VDealloc(void *ptr) = 0;

        // ////////////////////////////////////////////////////////////
        // Run defragmentation on the allocaters memory block.
        //
        // ////////////////////////////////////////////////////////////
        virtual void VDefrag() = 0;

        // ////////////////////////////////////////////////////////////
        // Run a partial defragmentation on the allocaters memory block.
        // Useful when you want to defragment the block but want to spread
        // the workload over a couple of frames.
        //
        // @param seconds The maximum number of seconds to run for.
        //
        // @return F32 Percentage of memory block left to defragment.
        //
        // ////////////////////////////////////////////////////////////
        virtual F32 VDefrag(const F32 seconds) = 0;
    };

    // ////////////////////////////////////////////////////////////
    // @class BaseGameAllocater
    // @author PJ O Halloran
    //
    // Base memory allocater.  The ID stored here is used by the
    // games memory manager to store and retrieve the allocater.
    //
    // ////////////////////////////////////////////////////////////
    class BaseGameAllocater : public IGameAllocater {
    private:

        U32 m_id;                           ///< Allocater ID.

    protected:

        // ////////////////////////////////////////////////////////////
        // Constructor.
        //
        // ////////////////////////////////////////////////////////////
        explicit BaseGameAllocater() : m_id(0) {};

        // ////////////////////////////////////////////////////////////
        // Destructor.
        //
        // ////////////////////////////////////////////////////////////
        virtual ~BaseGameAllocater() {};

    public:

        // ////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param bgaId ID of the allocater.
        //
        // ////////////////////////////////////////////////////////////
        explicit BaseGameAllocater(const U32 bgaId) : m_id(bgaId) {};

        // ////////////////////////////////////////////////////////////
        // Get the Allocater ID/slot.
        //
        // ////////////////////////////////////////////////////////////
        inline U32 GetId() {
            return (m_id);
        };
    };
}

#endif
