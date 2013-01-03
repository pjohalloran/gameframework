#pragma once
#ifndef __GL_BATCH__
#define __GL_BATCH__

/*
GLBatch.h
 
Copyright (c) 2009, Richard S. Wright Jr.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list 
of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this list 
of conditions and the following disclaimer in the documentation and/or other 
materials provided with the distribution.

Neither the name of Richard S. Wright Jr. nor the names of other contributors may be used 
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
// @file GLBatch.h
// @author Richard S. Wright Jr & PJ O Halloran
// @date 09/09/2010
//
// A container for batches of geometry (points, lines, line strips,
// line loops, triangles, triangle strips and triangle fans).
// Original source from the OpenGL Superbible 5th edition.
//
// /////////////////////////////////////////////////////////////////

#include "GamePlatform.h"

#include "IGLBatchBase.h"
#include "Matrix.h"
#include "Vector.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class GLBatch
	// @author Richard S. Wright Jr. & PJ O Halloran
	//
	// A container for batches of geometry (points, lines, line strips,
	// line loops, triangles, triangle strips and triangle fans).
	//
	// Changes from original (not very much):
	// - Added comments to class members and methods.
	//
	// - Made class members private.
	//
	// /////////////////////////////////////////////////////////////////
	class GLBatch : public IGLBatchBase
    {
    private:
		GLenum m_primitiveType;				///< What type of primitive is the batch constructing/drawing.
		GLuint m_uiVertexArray;				///< Vertex array buffer ID.
		GLuint m_uiNormalArray;				///< Normal array buffer ID.
		GLuint m_uiColorArray;				///< Color array buffer ID.
		GLuint *m_uiTextureCoordArray;		///< Texture coordinate array buffer IDs.
		GLuint m_vertexArrayObject;			///< Vertex array buffer object ID.
        GLuint m_nVertsBuilding;			///< Building up vertices counter (immediate mode emulator)
        GLuint m_nNumVerts;					///< Number of verticies in this batch
        GLuint m_nNumTextureUnits;			///< Number of texture coordinate sets
        bool m_bBatchDone;					///< Batch has been built
		VertexArr *m_pVerts;				///< Array of vertices arrays.
		NormalArr *m_pNormals;				///< Array of normals arrays.
		ColorArr *m_pColors;				///< Array of colors arrays.
		TextureArr **m_pTexCoords;			///< Array of pointers to texture coordinates.

    public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
        explicit GLBatch(void);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
        virtual ~GLBatch(void);
        
		// /////////////////////////////////////////////////////////////////
		// Start populating the array
		//
		// @param primitive The type of primitive being constructed.
		// @param nVerts The number of expected vertices. 
		// @param nTextureUnits The number of expected texture units.
		//
		// /////////////////////////////////////////////////////////////////
        void Begin(const GLenum primitive, const GLuint nVerts, const GLuint nTextureUnits = 0);
        
		// /////////////////////////////////////////////////////////////////
		// Tell the batch you are done
		//
		// /////////////////////////////////////////////////////////////////
		void End(void);
     
		// /////////////////////////////////////////////////////////////////
		// Block Copy in vertex data
		//
		// @param vVerts Array of vertex data.
		//
		// /////////////////////////////////////////////////////////////////
		void CopyVertexData3f(VertexArr *vVerts);

		// /////////////////////////////////////////////////////////////////
		// Block Copy in normal data.
		//
		// @param vNorms Array of normal data.
		//
		// /////////////////////////////////////////////////////////////////
		void CopyNormalDataf(NormalArr *vNorms);

		// /////////////////////////////////////////////////////////////////
		// Block Copy in color data.
		//
		// @param vColors Array of color data.
		//
		// /////////////////////////////////////////////////////////////////
		void CopyColorData4f(ColorArr *vColors);

		// /////////////////////////////////////////////////////////////////
		// Block Copy in texture data.  
		// Please note that a 3D Vector is used but textures typically have
		// 2 values so the x and y fields in the vector are used and the z 
		// field is unused.
		//
		// @param vTextCoords
		// @param uiTextureLayer
		//
		// /////////////////////////////////////////////////////////////////
		void CopyTexCoordData2f(TextureArr *vTexCoords, const GLuint uiTextureLayer);

		inline void CopyVertexData3f(GLfloat *vVerts) { CopyVertexData3f(reinterpret_cast<VertexArr *>(vVerts)); }

		inline void CopyNormalDataf(GLfloat *vNorms) { CopyNormalDataf(reinterpret_cast<NormalArr *>(vNorms)); }

		inline void CopyColorData4f(GLfloat *vColors) { CopyColorData4f(reinterpret_cast<ColorArr *>(vColors)); }

		inline void CopyTexCoordData2f(GLfloat *vTex, const GLuint uiTextureLayer) { CopyTexCoordData2f(reinterpret_cast<TextureArr *>(vTex), uiTextureLayer); }

		// /////////////////////////////////////////////////////////////////
		// Send the batch of geometry to the pixel shader.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VDraw(void);
 
		// /////////////////////////////////////////////////////////////////
		// Reset the batch.
		//
		// /////////////////////////////////////////////////////////////////
        void Reset(void);
        
		// /////////////////////////////////////////////////////////////////
		// Immediate mode emulation
		// Slowest way to build an array on purpose... Use the above if you can instead
		// /////////////////////////////////////////////////////////////////

		// /////////////////////////////////////////////////////////////////
		// Add a vertex.
		//
		// /////////////////////////////////////////////////////////////////
        void Vertex3f(const GLfloat x, const GLfloat y, const GLfloat z);

        void Vertex3fv(VertexArr vVertex);
        
		// /////////////////////////////////////////////////////////////////
		// Add a normal.  Unlike the old OpenGL immediate mode, you will get
		// junk unless you specify a normal per vertex!
		//
		// /////////////////////////////////////////////////////////////////
        void Normal3f(const GLfloat x, const GLfloat y, const GLfloat z);

        void Normal3fv(NormalArr vNormal);
        
		// /////////////////////////////////////////////////////////////////
		// Add a color.
		//
		// /////////////////////////////////////////////////////////////////
        void Color4f(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a);

        void Color4fv(ColorArr vColor);
        
		// /////////////////////////////////////////////////////////////////
		// Add a texture coordinate.  Unlike the old OpenGL immediate mode,
		// you will get junk unless you specify a texture coordinate per vertex!
		//
		// /////////////////////////////////////////////////////////////////
        void MultiTexCoord2f(const GLuint texture, const GLclampf s, const GLclampf t);

        void MultiTexCoord2fv(const GLuint texture, TextureArr vTexCoord);
	
	};

}

#endif // __GL_BATCH__
