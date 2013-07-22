#pragma once
#ifndef _GF_STACK_ALLOCATER_H
#define _GF_STACK_ALLOCATER_H

// ////////////////////////////////////////////////////////////
// @file StackAllocater.h
// @author PJ O Halloran
// @date 06/03/2011
//
// Header for the StackAllocater class.
//
// ////////////////////////////////////////////////////////////

#include "GameAllocater.h"
#include "GameBase.h"

namespace GameHalloran {

    // ////////////////////////////////////////////////////////////
    // @class StackAllocater
    // @author PJ O Halloran
    //
    // A stack managing a block of memory for fast memory allocation.
    //
    // Based on Section 5.2 Memory Management from Game Engine
    // Architecture 1st Edition by Jason Gregory.
    //
    // ////////////////////////////////////////////////////////////
    class StackAllocater : public BaseGameAllocater {
    public:

        // Current location of the stack data type.
        typedef U64 Marker;

    private:

        U64 m_size;                     ///< Total number of bytes allocated for the stack.
        Marker m_topMarker;             ///< Current top of the stack from the beginning of the stack.
        Marker m_endMarker;             ///< Current top of the stack from the end of the stack.

#ifdef DEBUG
        U32 m_numBlocks;                ///< Number of blocks allocated on the stack to date.

        // In the debug build we will ensure all currently free blocks have the same value.
        //  Also blocks will start and end with magic numbers to help detect buffer under/over runs.
        //   Each block will therefore need an extra 2 bytes in debug mode.
        static const U32 FREE_BLOCK_VALUE = 0xAABBCCDD;
        static const U32 START_BLOCK_VALUE = 0xFFEEAABB;
        static const U32 END_BLOCK_VALUE = 0xBBAAEEFF;
#define SA_FREE_BLOCK_VALUE 0xAABBCCDD
#define SA_START_BLOCK_VALUE 0xFFEEAABB
#define SA_END_BLOCK_VALUE 0xBBAAEEFF


        // ////////////////////////////////////////////////////////////
        //
        // ////////////////////////////////////////////////////////////
        bool FourByteCompare(unsigned char *bytesA, unsigned char *bytesB) const {
            return (bytesA && bytesB && (bytesA[0] == bytesB[0]) && (bytesA[1] == bytesB[1]) && (bytesA[2] == bytesB[2]) && (bytesA[3] == bytesB[3]));
        };
#endif

        unsigned char *m_data;                  ///< Block of data.

    public:

        // ////////////////////////////////////////////////////////////
        // Creates a memory block.
        //
        // Ensure to check if the block was created with IsValid() after.
        //
        // @param stackSize The requested size of the stack.
        //
        // ////////////////////////////////////////////////////////////
        explicit StackAllocater(const U64 stackSize);

        // ////////////////////////////////////////////////////////////
        // Destructor.
        //
        // ////////////////////////////////////////////////////////////
        ~StackAllocater();

        // ////////////////////////////////////////////////////////////
        // Check if the stack successfully created?  If not all other
        // class methods will fail!
        //
        // ////////////////////////////////////////////////////////////
        inline bool IsValid() const {
            return (m_data != NULL);
        };

        // ////////////////////////////////////////////////////////////
        // Get the size of the stack in bytes.
        //
        // ////////////////////////////////////////////////////////////
        inline U64 GetSize() const {
            if(!IsValid()) {
                return (0);
            }
            return (m_size);
        }

        // ////////////////////////////////////////////////////////////
        // Allocate a block of memory.
        //
        // @param size Size of memory block to allocate.
        //
        // @return void* NULL on error.
        //
        // ////////////////////////////////////////////////////////////
        virtual void *VAlloc(const U64 size);

        // ////////////////////////////////////////////////////////////
        // TODO
        // (currently not used)
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
        // Deallocate a block of memory (not used).
        //
        // ////////////////////////////////////////////////////////////
        virtual void VDealloc(void *ptr) { };

        // ////////////////////////////////////////////////////////////
        // Run defragmentation on the allocaters memory block (not used).
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

        // ////////////////////////////////////////////////////////////
        // Get the current marker or top of the stack.  Won't be valid
        // unless the stack is valid!
        //
        // ////////////////////////////////////////////////////////////
        inline Marker GetMarker() const {
            return (m_topMarker);
        };

        // ////////////////////////////////////////////////////////////
        // Allocate a block of memory from the end of the stack.
        //
        // @param blockSize Size of the block to allocate in bytes.
        //
        // @return A pointer to the beginning of a block of data of
        //          the size requested or NULL on error.
        //
        // ////////////////////////////////////////////////////////////
        void *AllocEnd(const U64 blockSize);

        // ////////////////////////////////////////////////////////////
        // Get the current marker or top of the stack from the end of
        // the stack.  Won't be valid unless the stack is valid!
        //
        // ////////////////////////////////////////////////////////////
        inline Marker GetEndMarker() const {
            return (m_endMarker);
        };

        // ////////////////////////////////////////////////////////////
        // Free all memory allocated back to the marker supplied.
        //
        // @param marker The top of the stack you wish to free to.
        //
        // ////////////////////////////////////////////////////////////
        void FreeToMarker(const Marker marker);

        // ////////////////////////////////////////////////////////////
        // Free all blocks allocated.
        //
        // ////////////////////////////////////////////////////////////
        inline void Clear() {
            FreeToMarker(0);
            FreeToMarker(m_size);
        };

        // ////////////////////////////////////////////////////////////
        // Get the number of bytes remaining of free memory.
        //
        // ////////////////////////////////////////////////////////////
        inline U64 GetAvailableMemory() const {
            return (m_size - m_topMarker);
        };

#ifdef DEBUG

        // ////////////////////////////////////////////////////////////
        // Verifies that a block of memory has not had any buffer
        // under or over runs.
        //
        // ////////////////////////////////////////////////////////////
        bool VerifyBlock(void *block, const U64 blockSize) const;

        // ////////////////////////////////////////////////////////////
        // Verifies the entire block of data the stack is managing.
        //
        // ////////////////////////////////////////////////////////////
        bool VerifyStack() const;

        // ////////////////////////////////////////////////////////////
        // Get the number of blocks allocated with the stack.
        //
        // ////////////////////////////////////////////////////////////
        inline U32 GetNumberBlocksAllocated() const {
            return (m_numBlocks);
        };

#endif

    };
}

#endif