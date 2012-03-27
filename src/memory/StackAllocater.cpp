// ////////////////////////////////////////////////////////////
// @file StackAllocater.cpp
// @author PJ O Halloran
// @date 06/03/2011
//
// Impl for the StackAllocater class.
//
// ////////////////////////////////////////////////////////////

#if defined(_WINDOWS) || defined(WIN32)
    #include <malloc.h>
#else
    #include <cstdlib>
#endif

#ifdef _DEBUG
	#include <cstring>
	#include <cstdio>
#endif

#include "StackAllocater.h"


namespace GameHalloran
{

	// ////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////
	StackAllocater::StackAllocater(const U64 stackSize) : m_size(stackSize), m_topMarker(0), m_endMarker(stackSize), m_data(0)
	{
		if(m_size != 0)
		{
			m_data = static_cast<unsigned char *>(malloc(m_size));
#ifdef _DEBUG
			if(!IsValid())
			{
				printf("StackAllocater::StackAllocater(): Failed to allocate a block of memory of %u bytes with malloc.", m_size);
			}

			m_numBlocks = 0;
			memset(m_data, FREE_BLOCK_VALUE, m_size);
#endif
		}
	}

	// ////////////////////////////////////////////////////////////
	// Destructor.
	//
	// ////////////////////////////////////////////////////////////
	StackAllocater::~StackAllocater()
	{
		try
		{
//#ifdef _DEBUG
//			if(!VerifyStack())
//			{
//				printf("StackAllocater::~StackAllocater(): The stack was corrupted during its lifetime!\n");
//			}
//#endif
			if(IsValid())
			{
				free(m_data);
			}
		}
		catch(...) {}
	}

	// ////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////
	void *StackAllocater::VAlloc(const U64 blockSize)
	{
		// Not enough space to allocate the block!
#ifdef _DEBUG
		if(!IsValid() || m_topMarker + blockSize + 8 > m_endMarker)
		{
			printf("StackAllocater::Alloc(): Failed to allocate %u bytes.  Not enough free space!", blockSize);
#else
		if(!IsValid() || m_topMarker + blockSize > m_endMarker)
		{
#endif
			return (NULL);
		}

		unsigned char *topStack = m_data + m_topMarker;

#ifdef _DEBUG
		++m_numBlocks;

		m_topMarker += blockSize + 8;

		unsigned char *endStack = topStack + blockSize + 1;
		topStack = reinterpret_cast<unsigned char *>(SA_START_BLOCK_VALUE);
		topStack += 4;
		endStack = reinterpret_cast<unsigned char *>(SA_END_BLOCK_VALUE);
#else
		m_topMarker += blockSize;
#endif

		return (static_cast<void *>(topStack));
	}

	// ////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////
	void *StackAllocater::AllocEnd(const U64 blockSize)
	{
		// Not enough space to allocate the block!
#ifdef _DEBUG
		if(!IsValid() || m_endMarker - blockSize - 8 < m_topMarker)
		{
			printf("StackAllocater::AllocEnd(): Failed to allocate %u bytes.  Not enough free space!", blockSize);
#else
		if(!IsValid() || m_endMarker - blockSize < m_topMarker)
		{
#endif
			return (NULL);
		}

		unsigned char *topStack = m_data + m_endMarker - blockSize;

#ifdef _DEBUG
		topStack -= 8;

		++m_numBlocks;

		m_endMarker -= blockSize - 8;

		unsigned char *endStack = topStack + blockSize + 1;
		topStack = reinterpret_cast<unsigned char *>(SA_START_BLOCK_VALUE);
		topStack += 4;
		endStack = reinterpret_cast<unsigned char *>(SA_END_BLOCK_VALUE);
#else
		m_endMarker -= blockSize;
#endif

		return (static_cast<void *>(topStack));
	}

	// ////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////
	void StackAllocater::FreeToMarker(const Marker marker)
	{
		if(IsValid())
		{
			if(marker < m_topMarker)
			{
#ifdef _DEBUG
				--m_numBlocks;
				U64 bytesToClear(m_topMarker - marker);
				memset(m_data + marker, FREE_BLOCK_VALUE, bytesToClear);
#endif
				m_topMarker = marker;
			}
			else if(marker > m_endMarker)
			{
#ifdef _DEBUG
				--m_numBlocks;
				U64 bytesToClear(m_endMarker - marker);
				memset(m_data + marker, FREE_BLOCK_VALUE, bytesToClear);
#endif
				m_endMarker = marker;
			}
		}
	}

#ifdef _DEBUG

	// ////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////
	bool StackAllocater::VerifyBlock(void *block, const U64 blockSize) const
	{
		unsigned char *memBlock = static_cast<unsigned char *>(block);
		if(!IsValid()\
			|| (!memBlock)\
			|| (memBlock - 1 != reinterpret_cast<unsigned char *>(SA_START_BLOCK_VALUE))\
			|| (memBlock + blockSize != reinterpret_cast<unsigned char *>(SA_END_BLOCK_VALUE)))
		{
			return (false);
		}

		return (true);
	}

	// ////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////
	bool StackAllocater::VerifyStack() const
	{
		if(!IsValid())
		{
			return (false);
		}

		unsigned char *startValue = reinterpret_cast<unsigned char *>(SA_START_BLOCK_VALUE);
		unsigned char *freeValue = reinterpret_cast<unsigned char *>(SA_FREE_BLOCK_VALUE);

		// Search for all start block markers and verify that we find
		//  exactly the number of blocks we expect and that all bytes
		//   at the top of the stack and below the end of any used memory
		//	  at the end of the stack have been unset!
		U32 blocksFound(0);
		bool error(false);
		for(U32 i = 0; ((!error) && (i < m_size)); ++i)
		{
			if(FourByteCompare(m_data + i, startValue))
			{
				++blocksFound;
			}
			if((i > m_topMarker) && (i < m_endMarker) && !FourByteCompare(m_data + i, freeValue))
			{
				error = true;
				printf("StackAllocater::VerifyStack(): Found an unset byte in the stack in the free memory section at index %i", i);
			}
		}

		if(!error || blocksFound != m_numBlocks)
		{
			return (false);
		}

		return (true);
	}
#endif

}
