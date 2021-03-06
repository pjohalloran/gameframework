#pragma once
#ifndef __GAME_MEMORY_H
#define __GAME_MEMORY_H

// ////////////////////////////////////////////////////////////
// @file GameMemory.h
// @author PJ O Halloran
// @date 13/10/2011
//
// Memory allocation routines.
//
// ////////////////////////////////////////////////////////////

#include "GameTypes.h"

#include "MemoryManager.h"

namespace GameHalloran {

    // Global memory manager instance.
    MemoryManager g_memManObj;

    // ////////////////////////////////////////////////////////////
    // Area to allocate memory from.
    //
    // - eNone:             Auto fail.
    // - eStack:            From custom StackAllocater.
    // - eDoubleBuffer:     From custom DoubleBuffered Stack Allocater.
    // - ePool:             From custom memory pool.
    // - eHeap:             From custom memory heap.
    // - eSystem:           Direct from systems malloc/free.
    //
    // ////////////////////////////////////////////////////////////
    enum AllocId {
        eNone = 0,
        eStack,
        eDoubleBuffer,
        ePool,
        eHeap,
        eSystem
    };

    // ////////////////////////////////////////////////////////////
    // Setup the games memory manager.
    //
    // ////////////////////////////////////////////////////////////
    void GameMemoryInit();

    // ////////////////////////////////////////////////////////////
    // Shutdown the games memory manager.
    //
    // ////////////////////////////////////////////////////////////
    void GameMemoryShutdown();

    // ////////////////////////////////////////////////////////////
    // Games Allocation routine.
    //
    // @param size The size of the memory to allocate.
    //
    // @return void* Pointer to the allocated block of memory.
    //
    // ////////////////////////////////////////////////////////////
    void *GameDefaultAlloc(const U64 size);

    // ////////////////////////////////////////////////////////////
    // Games deallocation routine.
    //
    // @param ptr
    //
    // ////////////////////////////////////////////////////////////
    void GameDefaultDealloc(void *ptr);

    // ////////////////////////////////////////////////////////////
    // Games Allocation routine.
    //
    // @param size The size of the memory to allocate.
    // @param area Allocater to get memory from.
    //
    // @return void* Pointer to the allocated block of memory.
    //
    // ////////////////////////////////////////////////////////////
    void *GameAlloc(const U64 size, const enum AllocId area = eSystem);

    // ////////////////////////////////////////////////////////////
    // Deallocate a memory block.
    //
    // @param ptr Pointer to a memory block to deallocate.
    // @param area Allocater to free memory from.
    //
    // ////////////////////////////////////////////////////////////
    void GameDealloc(void *ptr, const enum AllocId area);

}

#endif
