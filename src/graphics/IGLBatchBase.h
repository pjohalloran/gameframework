// /////////////////////////////////////////////////////////////////
// @file IGLBatchBase.h
// @author PJ O Halloran
// @date 09/09/2010
//
// Base interface for batches of geometry.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GL_BATCH_BASE__
#define __GL_BATCH_BASE__

#include "GamePlatform.h"

namespace GameHalloran {

    // Type definitions for batches of geometry.
    typedef GLfloat VertexArr[3];
    typedef GLfloat NormalArr[3];
    typedef GLfloat ColorArr[4];
    typedef GLfloat TextureArr[2];
    typedef GLfloat Matrix4Arr[16];

    // /////////////////////////////////////////////////////////////////
    // @class IGLBatchBase
    // @author PJ O Halloran.
    //
    // This base class is a pure virtual class with one single virtual
    // function, Draw(). The GLBegin class and GLTriangleBatch classes
    // are derived from this. Having a virtual Draw() function allows
    // these classes to be collected by container classes that can
    // then iterate over them and call their draw methods.
    //
    // /////////////////////////////////////////////////////////////////
    class IGLBatchBase {
    public:

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~IGLBatchBase() { };

        // /////////////////////////////////////////////////////////////////
        // Method that should send the batch of geometry to the vertex
        // shader.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VDraw(void) = 0;

    };

}

#endif
