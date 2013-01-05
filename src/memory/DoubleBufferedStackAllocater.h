#pragma once
#ifndef _GF_DOUBLE_BUFFERED_STACK_ALLOCATER_H
#define _GF_DOUBLE_BUFFERED_STACK_ALLOCATER_H

// ////////////////////////////////////////////////////////////
// @file DoubleBufferedStackAllocater.h
// @author PJ O Halloran
// @date 06/03/2011
//
// Header and impl for the DoubleBufferedStackAllocater class.
//
// ////////////////////////////////////////////////////////////

#include "GameAllocater.h"
#include "StackAlocater.h"

namespace GameHalloran
{

	// ////////////////////////////////////////////////////////////
	// @class DoubleBufferedStackAllocater
	// @author PJ O Halloran
	//
	// A double buffered stack allocater to make dynamic memory
	// available for 2 frames in order to pass data onto the next
	// frame update/render cycle in a game loop.
	//
	// ////////////////////////////////////////////////////////////
	class DoubleBufferedStackAllocater : public BaseGameAllocater
	{
	private:
		U64 m_currStack;
		StackAllocater m_stacks[2];

	public:

		// ////////////////////////////////////////////////////////////
		// Swap to the other buffer. Called at the end of a game loop
		// cycle.
		//
		// ////////////////////////////////////////////////////////////
		inline void SwapBuffers() { m_currStack = !(U64)m_currStack; };

		// ////////////////////////////////////////////////////////////
		// Clear the current buffer.
		//
		// ////////////////////////////////////////////////////////////
		inline void ClearCurrentBuffer() { m_stacks[m_currStack].VClear(); };

		// ////////////////////////////////////////////////////////////
		// Clea all buffers.
		//
		// ////////////////////////////////////////////////////////////
		inline void ClearAll() { m_stacks[0].VClear(); m_stacks[1].VClear(); };

		// ////////////////////////////////////////////////////////////
		// Allocate a block of memory from the current buffer.
		//
		// @param blockSize The intended size of the block.
		//
		// @return void* Pointer to the beginning of the memory block.
		//
		// ////////////////////////////////////////////////////////////
		inline virtual void *VAlloc(const U64 blockSize) { return (m_stacks[m_currStack].VAlloc(blockSize)); };

		// ////////////////////////////////////////////////////////////
		// Get the marker of the top of the current stack.
		//
		// ////////////////////////////////////////////////////////////
		U64 GetCurrentBufferMarker() const { return (m_stacks[m_currStack].GetMarker()); };

		// ////////////////////////////////////////////////////////////
		// Free to the position in the stack defined by the marker.
		//
		// ////////////////////////////////////////////////////////////
		void FreeToCurrentBufferMarker(const U64 marker) { m_stacks[m_currStack].FreeToMarker(marker); };
        
        // ////////////////////////////////////////////////////////////
        // (not used)
        // Allocate an aligned block of memory.
        //
        // @param size Size of memory block to allocate.
        // @param alignment The alignment of the block of memory.
        //
        // @return void* NULL on error.
        //
        // ////////////////////////////////////////////////////////////
        virtual void *VAllocAligned(const U64 size, const U32 alignment) { return (NULL); };
        
        // ////////////////////////////////////////////////////////////
        // (not used)
        // Deallocate a block of memory.
        //
        // ////////////////////////////////////////////////////////////
        virtual void VDealloc(void *ptr) { return (NULL); };
        
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
        virtual F32 VDefrag(const F32 seconds) { return (0.0f); };
	};

}

#endif