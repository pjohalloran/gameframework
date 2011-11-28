// IGLBatchBase.h
/* Copyright 2009, Richard S. Wright Jr. All Rights Reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list 
of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this list 
of conditions and the following disclaimer in the documentation and/or other 
materials provided with the distribution.

Neither the name of Richard S. Wright Jr. nor the names of other contributors may be used 
to endorse or promote products derived from this software without specific prior 
written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// /////////////////////////////////////////////////////////////////
// @file IGLBatchBase.h
// @author Richard S. Wright Jr & PJ O Halloran
// @date 09/09/2010
//
// Base interface for batches of geometry.  Original source from the
// OpenGL Superbible 5th edition.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GL_BATCH_BASE__
#define __GL_BATCH_BASE__

// External Headers

// Project Headers


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// Type definitions for batches of geometry.
	typedef GLfloat VertexArr[3];
	typedef GLfloat NormalArr[3];
	typedef GLfloat ColorArr[4];
	typedef GLfloat TextureArr[2];
	typedef GLfloat Matrix4Arr[16];

	// /////////////////////////////////////////////////////////////////
	// @class IGLBatchBase
	// @author Richard S. Wright Jr. & PJ O Halloran.
	//
	// This base class is a pure virtual class with one single virtual 
	// function, Draw(). The GLBegin class and GLTriangleBatch classes
	// are derived from this. Having a virtual Draw() function allows
	// these classes to be collected by container classes that can
	// then iterate over them and call their draw methods.
	//
	// Changes made from original:
	// - Changed name of class from GLBatchBase to IGLBatchBase.
	//
	// - Added virtual destructor to ensure all derived classes
	// destructors are virtual.
	//
	// - Changed name of method Draw to VDraw to point out easily that
	// it is a virtual call.
	//
	// /////////////////////////////////////////////////////////////////
	class IGLBatchBase
	{
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
