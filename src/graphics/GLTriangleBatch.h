#pragma once
#ifndef __TRIANGLE_BATCH_H
#define __TRIANGLE_BATCH_H

/*
 *  GLTriangleBatch.h
 *  OpenGL SuperBible
 *
Copyright (c) 2007-2009, Richard S. Wright Jr.
All rights reserved.

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

 *  This class allows you to simply add triangles as if this class were a 
 *  container. The AddTriangle() function searches the current list of triangles
 *  and determines if the vertex/normal/texcoord is a duplicate. If so, it addes
 *  an entry to the index array instead of the list of vertices.
 *  When finished, call EndMesh() to free up extra unneeded memory that is reserved
 *  as workspace when you call BeginMesh().
 *
 *  This class can easily be extended to contain other vertex attributes, and to 
 *  save itself and load itself from disk (thus forming the beginnings of a custom
 *  model file format).
 *
 */

// /////////////////////////////////////////////////////////////////
// @file GLTriangleBatch.h
// @author Richard S. Wright Jr & PJ O Halloran
// @date 10/09/2010
//
// A container for batches of triangles.
// Original source from the OpenGL Superbible 5th edition.
//
// /////////////////////////////////////////////////////////////////

#include "GamePlatform.h"
#include "IGLBatchBase.h"
#include "Triangle.h"

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	//
	// /////////////////////////////////////////////////////////////////
	enum TBatchType
	{
		VERTEX_DATA = 0,
		NORMAL_DATA,
		TEXTURE_DATA,
		INDEX_DATA
	};

	// /////////////////////////////////////////////////////////////////
	// @class GLTriangleBatch
	// @author Richard S. Wright Jr & PJ O Halloran
	//
	// Container for batches of triangles.
	//
	// /////////////////////////////////////////////////////////////////
	class GLTriangleBatch : public IGLBatchBase
    {
    private:

        GLushort *m_pIndexes;				///< Array of indices.
        VertexArr *m_pVerts;				///< Array of vertices.
        NormalArr *m_pNorms;				///< Array of normals.
        TextureArr *m_pTexCoords;			///< Array of texture coordinates.
        GLuint m_nMaxIndexes;				///< Maximum workspace
        GLuint m_nNumIndexes;				///< Number of indexes currently used
        GLuint m_nNumVerts;					///< Number of vertices actually used
        GLuint m_bufferObjects[4];			///< GL buffer objects for the batchs' indices, vertices, normals and texture coordinates.
		GLuint m_vertexArrayBufferObject;	///< GL VBO array buffer object ID.
		bool m_batchComplete;				///< Has the batch been completed and sent to the GPU?
		const GLfloat m_epsilon;			///< How small a difference between floats are allowed until they are deemed equal (for generating indices).

		// /////////////////////////////////////////////////////////////////
		// Constructor helper function.
		//
		// /////////////////////////////////////////////////////////////////
		void Init();

		// /////////////////////////////////////////////////////////////////
		// Reset internal data.
		//
		// @param resetGlBuffers Should we reset the OpenGL VBO and VBO array?
		//
		// /////////////////////////////////////////////////////////////////
		void Reset(const bool resetGlBuffers = true);

    public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
        GLTriangleBatch(void);

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param epsilon How small a difference between floats are allowed
		//					until they are deemed equal (for generating indices).
		//
		// /////////////////////////////////////////////////////////////////
        GLTriangleBatch(const GLfloat epsilon);

		// /////////////////////////////////////////////////////////////////
		// Constructor.  Also starts a batch.
		//
		// @param epsilon How small a difference between floats are allowed
		//					until they are deemed equal (for generating indices).
		// @param nMaxVerts Maximum number of vertices expected.
		//
		// /////////////////////////////////////////////////////////////////
        GLTriangleBatch(const GLfloat epsilon, const GLuint nMaxVerts);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
        ~GLTriangleBatch(void);
        
		// /////////////////////////////////////////////////////////////////
		// Start assembling a mesh. You need to specify a maximum amount
		// of indexes that you expect. The EndMesh will clean up any uneeded
		// memory.
		//
		// Call BeginMesh when you intend to add one triangle at a time.  This
		// is done automatically If you are using AddMesh().
		//
		// @param nMaxVerts Maximum number of vertices expected.
		//
		// /////////////////////////////////////////////////////////////////
        void BeginMesh(const GLuint nMaxVerts);

		// /////////////////////////////////////////////////////////////////
		// Add a triangle to the mesh (one at a time). This is extremely slow
		// for large meshes.  Its better to use AddMesh() for such meshes!
		//
		// This searches the current list for identical (well, almost identical 
		// - these are floats) verts. If one is found, it is added to the
		// index array. If not, it is added to both the index array and the
		// vertex array grows by one as well.
		//
		// @param verts Array of vertices.
		// @param vNorms Array of normals.
		// @param vTexCoords Array of texture coordinates.
		// @param normNormal Should we normalize the normals?
		//
		// /////////////////////////////////////////////////////////////////
        void AddTriangle(VertexArr verts[3], NormalArr vNorms[3], TextureArr vTexCoords[3], const bool normNormal = false);

		// /////////////////////////////////////////////////////////////////
		// End the mesh assembling.
		// Compact the data. This is a nice utility, but you should really
		// save the results of the indexing for future use if the model data
		// is static (doesn't change).
		//
		// Call End when you have been adding one triangle at a time.  This
		// is done automatically If you are using AddMesh().
		//
		// @param clearCpuData Should we clear the CPU batch data now or retain
		//						it?
		//
		// /////////////////////////////////////////////////////////////////
        void End(const bool clearCpuData = true);

		// /////////////////////////////////////////////////////////////////
		// Add a triangle mesh to the batch.  There is no need to call
		// BeginMesh() or End() when submitting the batch using this method!
		//
		// @param mesh Triangle mesh to submit as the batch.
		// @param normNormal Should we normalize the normals?
		// @param clearCpuData Should we clear the CPU batch data now or retain
		//						it?
		//
		// /////////////////////////////////////////////////////////////////
		void AddMesh(const TriangleMesh &mesh, const bool normNormal = false, const bool clearCpuData = true);

		// /////////////////////////////////////////////////////////////////
        // Get the number of indices.
		//
		// /////////////////////////////////////////////////////////////////
        inline GLuint GetIndexCount(void) const { return (m_nNumIndexes); };

		// /////////////////////////////////////////////////////////////////
		// Get the number of vertices.
		//
		// /////////////////////////////////////////////////////////////////
        inline GLuint GetVertexCount(void) const { return (m_nNumVerts); };

		// /////////////////////////////////////////////////////////////////
        // Draw the batch.
		//
		// /////////////////////////////////////////////////////////////////
        virtual void VDraw(void);

		// /////////////////////////////////////////////////////////////////
		// Clear batch data held in CPU memory.  When End() is called all data
		// is copied to the GPU into VBOs.  The CPU data may be retained
		// so that other operations may be performed on it.
		//
		// This call allows you to explicitly clear the CPU side batch 
		// data after End() if you decided to keep it around then (why keep it? 
		// usually to add a similar mesh to the physics simulation running
		// concurrently with the rendering!).
		//
		// N.B. This call will do nothing unless the TriangleBatch has been
		// completed with a call to End().
		// 
		// /////////////////////////////////////////////////////////////////
		void Clear();

		// /////////////////////////////////////////////////////////////////
		// Get the index array.
		//
		// Exposed private member to help create mesh for the physics system!
		//
		// N.B. This call will return nothing unless the TriangleBatch has been
		// completed with a call to End() (so we dont mess with batch before
		// the GPU has it).
		//
		// /////////////////////////////////////////////////////////////////
		GLushort *GetIndexArray() const { if(!m_batchComplete) { return (NULL); } return (m_pIndexes); };

		// /////////////////////////////////////////////////////////////////
		// Get the vertex array.
		//
		// Exposed private member to help create mesh for the physics system!
		//
		// N.B. This call will return nothing unless the TriangleBatch has been
		// completed with a call to End() (so we dont mess with batch before
		// the GPU has it).
		//
		// /////////////////////////////////////////////////////////////////
		VertexArr *GetVertexArray() const { if(!m_batchComplete) { return (NULL); } return (m_pVerts); };

		// /////////////////////////////////////////////////////////////////
		// Is the batch complete (i.e. has the batch been submitted to the
		// GPU).
		//
		// /////////////////////////////////////////////////////////////////
		bool IsBatchComplete() const { return (m_batchComplete); };

#ifdef _DEBUG
		// /////////////////////////////////////////////////////////////////
		// Debug routine...
		//
		// /////////////////////////////////////////////////////////////////
		void PrintDebugInfo() const;
#endif

    };

}

#endif
