#ifndef GF_I_CLEANABLE_H
#define GF_I_CLEANABLE_H

// /////////////////////////////////////////////////////////////////
// @file ICleanable.h
// @author PJ O Halloran
// @date 26/11/2011
//
// ICleanable interface.
//
// /////////////////////////////////////////////////////////////////

namespace GameHalloran
{
    
    // /////////////////////////////////////////////////////////////////
    // @class ICleanable
    // @author PJ O Halloran
    //
    // Cleanable object interface.
    //
    // /////////////////////////////////////////////////////////////////
    class ICleanable
    {
    public:
        
        virtual ~ICleanable() {};
        
        virtual void VClean() = 0;
    };
    
}

#endif
