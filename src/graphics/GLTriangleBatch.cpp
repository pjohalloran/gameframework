/*
 *  GLTriangleBatch.cpp
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
// @file GLTriangleBatch.cpp
// @author Richard S. Wright Jr & PJ O Halloran
// @date 10/09/2010
//
// A container for batches of triangles.
// Original source from the OpenGL Superbible 5th edition.
//
// /////////////////////////////////////////////////////////////////

#include <cstring>

#include "GLTriangleBatch.h"
#include "GLShaderManager.h"
#include "GameBase.h"
#include "CommonMath.h"
#include "Vector.h"

#ifdef DEBUG
#include <iostream>
#endif

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GLTriangleBatch::Reset(const bool resetGlBuffers)
    {
        Clear();

        // Delete buffer objects
        if(resetGlBuffers) {
            glDeleteBuffers(4, m_bufferObjects);
            memset(m_bufferObjects, 0, sizeof(GLuint) * 4);
#ifndef OPENGL_ES
            glDeleteVertexArrays(1, &m_vertexArrayBufferObject);
            m_vertexArrayBufferObject = 0;
#endif
        }

        m_nMaxIndexes = 0;
        m_nNumIndexes = 0;
        m_nNumVerts = 0;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GLTriangleBatch::Init()
    {
        memset(m_bufferObjects, 0, sizeof(GLuint) * 4);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    GLTriangleBatch::GLTriangleBatch(void) : m_pIndexes(NULL), m_pVerts(NULL), m_pNorms(NULL), m_pTexCoords(NULL), m_nMaxIndexes(0), \
        m_nNumIndexes(0), m_nNumVerts(0), m_vertexArrayBufferObject(0), m_batchComplete(false), m_epsilon(0.00001f)
    {
        Init();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    GLTriangleBatch::GLTriangleBatch(const GLfloat epsilon) : m_pIndexes(NULL), m_pVerts(NULL), m_pNorms(NULL), m_pTexCoords(NULL), m_nMaxIndexes(0), \
        m_nNumIndexes(0), m_nNumVerts(0), m_vertexArrayBufferObject(0), m_batchComplete(false), m_epsilon(epsilon)
    {
        Init();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    GLTriangleBatch::GLTriangleBatch(const GLfloat epsilon, const GLuint nMaxVerts) : m_pIndexes(NULL), m_pVerts(NULL), m_pNorms(NULL), m_pTexCoords(NULL), \
        m_nMaxIndexes(0), m_nNumIndexes(0), m_nNumVerts(0), m_vertexArrayBufferObject(0), m_batchComplete(false), m_epsilon(epsilon)
    {
        Init();
        BeginMesh(nMaxVerts);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    GLTriangleBatch::~GLTriangleBatch(void)
    {
        try {
            Reset();
        } catch(...) {}
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GLTriangleBatch::BeginMesh(const GLuint nMaxVerts)
    {
        Reset();
        m_nMaxIndexes = nMaxVerts;
        m_batchComplete = false;

        // Allocate new blocks. In reality, the other arrays will be
        //  much shorter than the index array.
        m_pIndexes = GCC_NEW GLushort[m_nMaxIndexes];
        memset(m_pIndexes, 0, sizeof(GLushort) * m_nMaxIndexes);

        m_pVerts = GCC_NEW VertexArr[m_nMaxIndexes];
        memset(m_pVerts, 0, sizeof(GLfloat) * 3 * m_nMaxIndexes);

        m_pNorms = GCC_NEW NormalArr[m_nMaxIndexes];
        memset(m_pNorms, 0, sizeof(GLfloat) * 3 * m_nMaxIndexes);

        m_pTexCoords = GCC_NEW TextureArr[m_nMaxIndexes];
        memset(m_pTexCoords, 0, sizeof(GLfloat) * 2 * m_nMaxIndexes);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GLTriangleBatch::AddTriangle(VertexArr verts[3], NormalArr vNorms[3], TextureArr vTexCoords[3], const bool normNormal)
    {
        if(m_batchComplete) {
            // End() has already been called!
            return;
        }

        const float e(0.00001f); // How small a difference to equate

        if(normNormal) {
            // First thing we do is make sure the normals are unit length!
            // It's almost always a good idea to work with pre-normalized normals
            for(U32 i = 0, max = 3; i < max; ++i) {
                Vector3 normVec(vNorms[i][0], vNorms[i][1], vNorms[i][2]);
                normVec.Normalize();
                vNorms[i][0] = normVec.GetX();
                vNorms[i][1] = normVec.GetY();
                vNorms[i][2] = normVec.GetZ();
            }
        }

        // Search for match - triangle consists of three verts
        for(GLuint iVertex = 0; iVertex < 3; ++iVertex) {
            GLuint iMatch = 0;
            for(iMatch = 0; iMatch < m_nNumVerts; ++iMatch) {
                // If the vertex positions are the same
                if(m3dCloseEnough(m_pVerts[iMatch][0], verts[iVertex][0], e) &&
                        m3dCloseEnough(m_pVerts[iMatch][1], verts[iVertex][1], e) &&
                        m3dCloseEnough(m_pVerts[iMatch][2], verts[iVertex][2], e) &&
                        // AND the Normal is the same...
                        m3dCloseEnough(m_pNorms[iMatch][0], vNorms[iVertex][0], e) &&
                        m3dCloseEnough(m_pNorms[iMatch][1], vNorms[iVertex][1], e) &&
                        m3dCloseEnough(m_pNorms[iMatch][2], vNorms[iVertex][2], e) &&
                        // And Texture is the same...
                        m3dCloseEnough(m_pTexCoords[iMatch][0], vTexCoords[iVertex][0], e) &&
                        m3dCloseEnough(m_pTexCoords[iMatch][1], vTexCoords[iVertex][1], e)) {
                    // Then add the index only
                    m_pIndexes[m_nNumIndexes] = iMatch;
                    ++m_nNumIndexes;
                    break;
                }
            }

            // No match for this vertex, add to end of list
            if(iMatch == m_nNumVerts && m_nNumVerts < m_nMaxIndexes && m_nNumIndexes < m_nMaxIndexes) {
                memcpy(m_pVerts[m_nNumVerts], verts[iVertex], sizeof(VertexArr));
                memcpy(m_pNorms[m_nNumVerts], vNorms[iVertex], sizeof(NormalArr));
                memcpy(m_pTexCoords[m_nNumVerts], vTexCoords[iVertex], sizeof(TextureArr));
                m_pIndexes[m_nNumIndexes] = m_nNumVerts;
                ++m_nNumIndexes;
                ++m_nNumVerts;
            }
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GLTriangleBatch::End(const bool clearCpuData)
    {
        if(m_batchComplete) {
            // End() has already been called!
            return;
        }

#ifndef OPENGL_ES
        // Create the master vertex array object
        glGenVertexArrays(1, &m_vertexArrayBufferObject);
        glBindVertexArray(m_vertexArrayBufferObject);
#endif

        // Create the buffer objects
        glGenBuffers(4, m_bufferObjects);

        // Copy data to video memory
        // Vertex data
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[VERTEX_DATA]);
        glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_nNumVerts * 3, m_pVerts, GL_STATIC_DRAW);
        glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Normal data
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[NORMAL_DATA]);
        glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_nNumVerts * 3, m_pNorms, GL_STATIC_DRAW);
        glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Texture coordinates
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[TEXTURE_DATA]);
        glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_nNumVerts * 2, m_pTexCoords, GL_STATIC_DRAW);
        glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Indexes
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferObjects[INDEX_DATA]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*m_nNumIndexes, m_pIndexes, GL_STATIC_DRAW);

        // Done
#ifndef OPENGL_ES
        glBindVertexArray(0);
#endif

        // Unbind to anybody
#ifndef OPENGL_ES
        glBindVertexArray(0);
#endif

        m_batchComplete = true;

        if(clearCpuData) {
            Clear();
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GLTriangleBatch::AddMesh(const TriangleMesh &mesh, const bool normNormal, const bool clearCpuData)
    {
        // TODO: Implement.
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GLTriangleBatch::VDraw(void)
    {
        if(!m_batchComplete) {
            // End() has not been called!
            return;
        }

        // N.B. If OPENGLES is being used ensure to enable the client arrays
        // before calling the glDrawxxxx() routine.
#ifndef OPENGL_ES
        glBindVertexArray(m_vertexArrayBufferObject);
#else
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[VERTEX_DATA]);
        glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
        glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Normal data
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[NORMAL_DATA]);
        glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
        glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Texture coordinates
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[TEXTURE_DATA]);
        glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);
        glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Indexes
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferObjects[INDEX_DATA]);
#endif

        glDrawElements(GL_TRIANGLES, m_nNumIndexes, GL_UNSIGNED_SHORT, 0);

#ifndef OPENGL_ES
        // Unbind to anybody
        glBindVertexArray(0);
#else
        glDisableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
        glDisableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
        glDisableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);
#endif
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GLTriangleBatch::Clear()
    {
        delete [] m_pIndexes;
        m_pIndexes = NULL;
        delete [] m_pVerts;
        m_pVerts = NULL;
        delete [] m_pNorms;
        m_pNorms = NULL;
        delete [] m_pTexCoords;
        m_pTexCoords = NULL;
    }

#ifdef DEBUG
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GLTriangleBatch::PrintDebugInfo() const
    {
        if(m_nMaxIndexes == 0) {
            std::cout << "No mesh" << std::endl;
            return;
        }

        for(I32 i = 0; i < I32(m_nNumVerts); ++i) {
            std::cout << "Vertex " << i << ": " << m_pVerts[i][0] << ", " << m_pVerts[i][1] << ", " << m_pVerts[i][2] << std::endl;
            std::cout << "Normal " << i << ": " << m_pNorms[i][0] << ", " << m_pNorms[i][1] << ", " << m_pNorms[i][2] << std::endl;
            std::cout << "TexCoords " << i << ": " << m_pTexCoords[i][0] << ", " << m_pTexCoords[i][1] << std::endl;
        }

        for(I32 i = 0; i < I32(m_nNumIndexes); ++i) {
            std::cout << "Indice " << i << ": " << m_pIndexes[i] << std::endl;
        }
    }
#endif

}
