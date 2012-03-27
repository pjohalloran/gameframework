#ifndef __MEMORY_MANAGER_H
#define __MEMORY_MANAGER_H

// ////////////////////////////////////////////////////////////
// @file MemoryManager.h
// @author PJ O Halloran
// @date 13/10/2011
//
// Games memory manager.
//
// ////////////////////////////////////////////////////////////


#include "GameTypes.h"
#include "GameBase.h"
#include "GameAllocater.h"


// ////////////////////////////////////////////////////////////
//
// ////////////////////////////////////////////////////////////
namespace GameHalloran
{
    
    static const U32 DEFAULT_MAX_MEMORY_ALLOCATERS = 20;
    
    // ////////////////////////////////////////////////////////////
    // @class MemoryManager
    // @author PJ O Halloran
    //
    // Games memory manager where allocaters can be registered
    // and memory can be managed from.
    //
    // ////////////////////////////////////////////////////////////
    class MemoryManager : NonCopyable
    {
    private:
        
        BaseGameAllocater *m_allocaters[DEFAULT_MAX_MEMORY_ALLOCATERS];            ///< Array of registered game memory allocaters.
        
    public:
        
        // ////////////////////////////////////////////////////////////
        // Constructor.
        //
        // ////////////////////////////////////////////////////////////
        MemoryManager()
        {
            for(U32 i(0); i < DEFAULT_MAX_MEMORY_ALLOCATERS; ++i)
            {
                m_allocaters[i] = NULL;
            }
        };
        
        // ////////////////////////////////////////////////////////////
        // Destructor.
        //
        // ////////////////////////////////////////////////////////////
        ~MemoryManager() { try { Clear(); } catch(...) { } };
        
        // ////////////////////////////////////////////////////////////
        // Register a allocater with the gam system.
        //
        // @param allocaterPtr Allocater to register.
        //
        // @return bool True|False on success|failure.
        //
        // ////////////////////////////////////////////////////////////
        inline bool RegisterAllocater(BaseGameAllocater *allocaterPtr)
        {
            if(NULL != allocaterPtr || allocaterPtr->GetId() > DEFAULT_MAX_MEMORY_ALLOCATERS)
            {
                return (false);
            }
            
            UnregisterAllocater(allocaterPtr->GetId());
            m_allocaters[allocaterPtr->GetId()] = allocaterPtr;
            return (true);
        };
        
        // ////////////////////////////////////////////////////////////
        // Unregister an allocater.
        //
        // @param aid The ID of the allocater.
        //
        // @return True|False on success|failure.
        //
        // ////////////////////////////////////////////////////////////
        inline bool UnregisterAllocater(const U32 aid)
        {
            if(aid > DEFAULT_MAX_MEMORY_ALLOCATERS)
            {
                return (false);
            }
            
            m_allocaters[aid] = NULL;
            return (true);
        };
        
        // ////////////////////////////////////////////////////////////
        // Check if a allocater is registered in a slot.
        //
        // @param aid Allocater ID.
        //
        // @return bool True|False if the allocater exists or not.
        //
        // ////////////////////////////////////////////////////////////
        inline bool IsAllocaterRegistered(const U32 aid)
        {
            return (aid < DEFAULT_MAX_MEMORY_ALLOCATERS && m_allocaters[aid]);
        };
        
        // ////////////////////////////////////////////////////////////
        // Clear all registered allocaters.
        //
        // ////////////////////////////////////////////////////////////
        inline void Clear()
        {
            for(U32 i(0); i < DEFAULT_MAX_MEMORY_ALLOCATERS; ++i)
            {
                UnregisterAllocater(i);
            }
        };
        
        // ////////////////////////////////////////////////////////////
        // Get a specific allocater.
        //
        // ////////////////////////////////////////////////////////////
        inline BaseGameAllocater *GetAllocater(const U32 aid)
        {
            if(aid > DEFAULT_MAX_MEMORY_ALLOCATERS)
            {
                return (NULL);
            }
            
            return (m_allocaters[aid]);
        };
    };
    
}


#endif
