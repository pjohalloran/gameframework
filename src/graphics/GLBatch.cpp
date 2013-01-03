/*
GLBatch.cpp
 
Copyright (c) 2009, Richard S. Wright Jr.
GLTools Open Source Library
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
*/

// /////////////////////////////////////////////////////////////////
// @file GLBatch.cpp
// @author Richard S. Wright Jr & PJ O Halloran
// @date 09/09/2010
//
// A container for batches of geometry (points, lines, line strips,
// line loops, triangles, triangle strips and triangle fans).
// Original source from the OpenGL Superbible 5th edition.
//
// /////////////////////////////////////////////////////////////////

#include "GameBase.h"
#include "GLBatch.h"
#include "GLShaderManager.h"

#ifdef OPENGL_ES
	#define GL_WRITE_ONLY   GL_WRITE_ONLY_OES
	#define glMapBuffer     glMapBufferOES
	#define glUnmapBuffer   glUnmapBufferOES
#endif

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	GLBatch::GLBatch(void)
				: m_primitiveType(0)
				, m_uiVertexArray(0)
				, m_uiNormalArray(0)
				, m_uiColorArray(0)
				, m_uiTextureCoordArray(NULL)
				, m_vertexArrayObject(0)
				, m_nVertsBuilding(0)
				, m_nNumVerts(0)
				, m_nNumTextureUnits(0)
				, m_bBatchDone(false)
				, m_pVerts(NULL)
				, m_pNormals(NULL)
				, m_pColors(NULL)
				, m_pTexCoords(NULL)
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	GLBatch::~GLBatch(void)
	{
		try
		{
			// Vertex buffer objects
			if(m_uiVertexArray != 0)
			{
				glDeleteBuffers(1, &m_uiVertexArray);
			}
			if(m_uiNormalArray != 0)
			{
				glDeleteBuffers(1, &m_uiNormalArray);
			}
			if(m_uiColorArray != 0)
			{
				glDeleteBuffers(1, &m_uiColorArray);
			}
			
			for(U32 i = 0; i < m_nNumTextureUnits; ++i)
			{
				glDeleteBuffers(1, &m_uiTextureCoordArray[i]);
			}

			#ifndef OPENGL_ES
				glDeleteVertexArrays(1, &m_vertexArrayObject);
			#endif
		        
			delete [] m_uiTextureCoordArray;
			m_uiTextureCoordArray = NULL;
			delete [] m_pTexCoords;
			m_pTexCoords = NULL;
		}
		catch(...)
		{
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::Begin(GLenum primitive, GLuint nVerts, GLuint nTextureUnits)
	{
		m_primitiveType = primitive;
		m_nNumVerts = nVerts;
	    
		// Limit to four texture units
		if(nTextureUnits > 4)
		{
			nTextureUnits = 4;
		}   
		m_nNumTextureUnits = nTextureUnits;
		
		if(m_nNumTextureUnits != 0)
		{
			m_uiTextureCoordArray = GCC_NEW GLuint[m_nNumTextureUnits];

			// An array of pointers to texture coordinate arrays
			m_pTexCoords = GCC_NEW TextureArr*[m_nNumTextureUnits];
			for(U32 i = 0; i < m_nNumTextureUnits; ++i)
			{
				m_uiTextureCoordArray[i] = 0;
				m_pTexCoords[i] = NULL;
			}
		}
			
		// Vertex Array object for this Array
		#ifndef OPENGL_ES
			glGenVertexArrays(1, &m_vertexArrayObject);
			glBindVertexArray(m_vertexArrayObject);
		#endif
    }
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::CopyVertexData3f(VertexArr *vVerts) 
	{
		// First time, create the buffer object, allocate the space
		if(m_uiVertexArray == 0)
		{
			glGenBuffers(1, &m_uiVertexArray);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_nNumVerts, vVerts, GL_DYNAMIC_DRAW);
		}
		else
		{
			// Just bind to existing object
			glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);

			// Copy the data in
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * m_nNumVerts, vVerts);
			m_pVerts = NULL;
		}
    }
        
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::CopyNormalDataf(NormalArr *vNorms) 
	{
		// First time, create the buffer object, allocate the space
		if(m_uiNormalArray == 0)
		{
			glGenBuffers(1, &m_uiNormalArray);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiNormalArray);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_nNumVerts, vNorms, GL_DYNAMIC_DRAW);
		}
		else
		{
			// Just bind to existing object
			glBindBuffer(GL_ARRAY_BUFFER, m_uiNormalArray);
		
			// Copy the data in
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * m_nNumVerts, vNorms);
			m_pNormals = NULL;
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::CopyColorData4f(ColorArr *vColors) 
	{
		// First time, create the buffer object, allocate the space
		if(m_uiColorArray == 0)
		{
			glGenBuffers(1, &m_uiColorArray);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiColorArray);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * m_nNumVerts, vColors, GL_DYNAMIC_DRAW);
		}
		else
		{
			// Just bind to existing object
			glBindBuffer(GL_ARRAY_BUFFER, m_uiColorArray);
		
			// Copy the data in
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 4 * m_nNumVerts, vColors);
			m_pColors = NULL;
		}
    }

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::CopyTexCoordData2f(TextureArr *vTexCoords, const GLuint uiTextureLayer) 
	{
		// First time, create the buffer object, allocate the space
		if(m_uiTextureCoordArray[uiTextureLayer] == 0)
		{
			glGenBuffers(1, &m_uiTextureCoordArray[uiTextureLayer]);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiTextureCoordArray[uiTextureLayer]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * m_nNumVerts, vTexCoords, GL_DYNAMIC_DRAW);
		}
		else
		{	// Just bind to existing object
			glBindBuffer(GL_ARRAY_BUFFER, m_uiTextureCoordArray[uiTextureLayer]);
		
			// Copy the data in
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 2 * m_nNumVerts, vTexCoords);
			m_pTexCoords[uiTextureLayer] = NULL;
		}
    }
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::End(void)
	{
		#ifndef OPENGL_ES
			// Check to see if items have been added one at a time
			if(m_pVerts != NULL)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
				glUnmapBuffer(GL_ARRAY_BUFFER);
				m_pVerts = NULL;
			}
			if(m_pColors != NULL)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_uiColorArray);
				glUnmapBuffer(GL_ARRAY_BUFFER);
				m_pColors = NULL; 
			}
			if(m_pNormals != NULL)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_uiNormalArray);
				glUnmapBuffer(GL_ARRAY_BUFFER);
				m_pNormals = NULL;
			}
			for(U32 i = 0; i < m_nNumTextureUnits; i++)
			{
				if(m_pTexCoords[i] != NULL)
				{
					glBindBuffer(GL_ARRAY_BUFFER, m_uiTextureCoordArray[i]);
					glUnmapBuffer(GL_ARRAY_BUFFER);
					m_pTexCoords[i] = NULL;
				}
			}

			// Set up the vertex array object
			glBindVertexArray(m_vertexArrayObject);
		#endif
		
		if(m_uiVertexArray !=0)
		{
			glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
			glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
			
		if(m_uiColorArray != 0)
		{
			glEnableVertexAttribArray(GLT_ATTRIBUTE_COLOR);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiColorArray);
			glVertexAttribPointer(GLT_ATTRIBUTE_COLOR, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}
			
		if(m_uiNormalArray != 0)
		{
			glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiNormalArray);
			glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
			
		// How many texture units
		for(U32 i = 0; i < m_nNumTextureUnits; i++)
		{
			if(m_uiTextureCoordArray[i] != 0)
			{
				glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0 + i),
				glBindBuffer(GL_ARRAY_BUFFER, m_uiTextureCoordArray[i]);
				glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0 + i, 2, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}
		
		m_bBatchDone = true;
		#ifndef OPENGL_ES
			glBindVertexArray(0);
		#endif
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::Reset(void)
	{
		m_bBatchDone = false;
		m_nVertsBuilding = 0;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::Vertex3f(const GLfloat x, const GLfloat y, const GLfloat z)
	{
		// First see if the vertex array buffer has been created...
		if(m_uiVertexArray == 0)
		{
			// Nope, we need to create it
			glGenBuffers(1, &m_uiVertexArray);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_nNumVerts, NULL, GL_DYNAMIC_DRAW);
		}
			
		// Now see if it's already mapped, if not, map it
		if(m_pVerts == NULL)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
			m_pVerts = reinterpret_cast<VertexArr *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		}
			
		// Ignore if we go past the end, keeps things from blowing up
		if(m_nVertsBuilding >= m_nNumVerts)
		{
			return;
		}
		
		// Copy it in...
		m_pVerts[m_nVertsBuilding][0] = x;
		m_pVerts[m_nVertsBuilding][1] = y;
		m_pVerts[m_nVertsBuilding][2] = z;
		m_nVertsBuilding++;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::Vertex3fv(VertexArr vVertex)
	{
		// First see if the vertex array buffer has been created...
		if(m_uiVertexArray == 0)
		{
			// Nope, we need to create it
			glGenBuffers(1, &m_uiVertexArray);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_nNumVerts, NULL, GL_DYNAMIC_DRAW);
		}
		
		// Now see if it's already mapped, if not, map it
		if(m_pVerts == NULL)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
			m_pVerts = reinterpret_cast<VertexArr *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		}
		
		// Ignore if we go past the end, keeps things from blowing up
		if(m_nVertsBuilding >= m_nNumVerts)
		{
			return;
		}
		
		// Copy it in...
		memcpy(m_pVerts[m_nVertsBuilding], vVertex, sizeof(VertexArr));
		m_nVertsBuilding++;
	}
        
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::Normal3f(const GLfloat x, const GLfloat y, const GLfloat z)
	{
		// First see if the vertex array buffer has been created...
		if(m_uiNormalArray == 0)
		{
			// Nope, we need to create it
			glGenBuffers(1, &m_uiNormalArray);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiNormalArray);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_nNumVerts, NULL, GL_DYNAMIC_DRAW);
		}
		
		// Now see if it's already mapped, if not, map it
		if(m_pNormals == NULL)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_uiNormalArray);
			m_pNormals = reinterpret_cast<NormalArr *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		}
		
		// Ignore if we go past the end, keeps things from blowing up
		if(m_nVertsBuilding >= m_nNumVerts)
		{
			return;
		}
		
		// Copy it in...
		m_pNormals[m_nVertsBuilding][0] = x;
		m_pNormals[m_nVertsBuilding][1] = y;
		m_pNormals[m_nVertsBuilding][2] = z;
	}
        
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::Normal3fv(NormalArr vNormal)
	{
		// First see if the vertex array buffer has been created...
		if(m_uiNormalArray == 0)
		{
			// Nope, we need to create it
			glGenBuffers(1, &m_uiNormalArray);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiNormalArray);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_nNumVerts, NULL, GL_DYNAMIC_DRAW);
		}
		
		// Now see if it's already mapped, if not, map it
		if(m_pNormals == NULL)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_uiNormalArray);
			m_pNormals = reinterpret_cast<NormalArr *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		}
		
		// Ignore if we go past the end, keeps things from blowing up
		if(m_nVertsBuilding >= m_nNumVerts)
		{
			return;
		}
		
		// Copy it in...
		memcpy(m_pNormals[m_nVertsBuilding], vNormal, sizeof(NormalArr));
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::Color4f(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a)
	{
		// First see if the vertex array buffer has been created...
		if(m_uiColorArray == 0)
		{
			// Nope, we need to create it
			glGenBuffers(1, &m_uiColorArray);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiColorArray);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * m_nNumVerts, NULL, GL_DYNAMIC_DRAW);
		}
		
		// Now see if it's already mapped, if not, map it
		if(m_pColors == NULL)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_uiColorArray);
			m_pColors = reinterpret_cast<ColorArr *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		}
		
		// Ignore if we go past the end, keeps things from blowing up
		if(m_nVertsBuilding >= m_nNumVerts)
		{
			return;
		}
		
		// Copy it in...
		m_pColors[m_nVertsBuilding][0] = r;
		m_pColors[m_nVertsBuilding][1] = g;
		m_pColors[m_nVertsBuilding][2] = b;
		m_pColors[m_nVertsBuilding][3] = a;
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::Color4fv(ColorArr vColor)
	{
		// First see if the vertex array buffer has been created...
		if(m_uiColorArray == 0)
		{
			// Nope, we need to create it
			glGenBuffers(1, &m_uiColorArray);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiColorArray);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * m_nNumVerts, NULL, GL_DYNAMIC_DRAW);
		}
		
		// Now see if it's already mapped, if not, map it
		if(m_pColors == NULL)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_uiColorArray);
			m_pColors = reinterpret_cast<ColorArr *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		}
		
		// Ignore if we go past the end, keeps things from blowing up
		if(m_nVertsBuilding >= m_nNumVerts)
		{
			return;
		}
		
		// Copy it in...
		memcpy(m_pColors[m_nVertsBuilding], vColor, sizeof(ColorArr));
	}
        
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::MultiTexCoord2f(const GLuint texture, const GLclampf s, const GLclampf t)
	{
		// First see if the vertex array buffer has been created...
		if(m_uiTextureCoordArray[texture] == 0)
		{
			// Nope, we need to create it
			glGenBuffers(1, &m_uiTextureCoordArray[texture]);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiTextureCoordArray[texture]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * m_nNumVerts, NULL, GL_DYNAMIC_DRAW);
		}
		
		// Now see if it's already mapped, if not, map it
		if(m_pTexCoords[texture] == NULL)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_uiTextureCoordArray[texture]);
			m_pTexCoords[texture] = reinterpret_cast<TextureArr *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		}
		
		// Ignore if we go past the end, keeps things from blowing up
		if(m_nVertsBuilding >= m_nNumVerts)
		{
			return;
		}
		
		// Copy it in...
		m_pTexCoords[texture][m_nVertsBuilding][0] = s;
		m_pTexCoords[texture][m_nVertsBuilding][1] = t;
	}
   
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::MultiTexCoord2fv(const GLuint texture, TextureArr vTexCoord)
	{	
		// First see if the vertex array buffer has been created...
		if(m_uiTextureCoordArray[texture] == 0)
		{
			// Nope, we need to create it
			glGenBuffers(1, &m_uiTextureCoordArray[texture]);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiTextureCoordArray[texture]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * m_nNumVerts, NULL, GL_DYNAMIC_DRAW);
		}
		
		// Now see if it's already mapped, if not, map it
		if(m_pTexCoords[texture] == NULL)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_uiTextureCoordArray[texture]);
			m_pTexCoords[texture] = reinterpret_cast<TextureArr *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		}
		
		// Ignore if we go past the end, keeps things from blowing up
		if(m_nVertsBuilding >= m_nNumVerts)
		{
			return;
		}
		
		// Copy it in...
		memcpy(m_pTexCoords[texture], vTexCoord, sizeof(TextureArr));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLBatch::VDraw(void)
	{
		if(!m_bBatchDone)
		{
			return;
		}
	    
		#ifndef OPENGL_ES
			// Set up the vertex array object
			glBindVertexArray(m_vertexArrayObject);
		#else
			if(m_uiVertexArray !=0)
			{
				glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
				glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
				glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}
		    
			if(m_uiColorArray != 0)
			{
				glEnableVertexAttribArray(GLT_ATTRIBUTE_COLOR);
				glBindBuffer(GL_ARRAY_BUFFER, m_uiColorArray);
				glVertexAttribPointer(GLT_ATTRIBUTE_COLOR, 4, GL_FLOAT, GL_FALSE, 0, 0);
			}
		    
			if(m_uiNormalArray != 0)
			{
				glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
				glBindBuffer(GL_ARRAY_BUFFER, m_uiNormalArray);
				glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}
		    
			// How many texture units
			for(U32 i = 0; i < m_nNumTextureUnits; i++)
			{
				if(m_uiTextureCoordArray[i] != 0)
				{
					glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0 + i),
					glBindBuffer(GL_ARRAY_BUFFER, m_uiTextureCoordArray[i]);
					glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0 + i, 2, GL_FLOAT, GL_FALSE, 0, 0);
				}
			}
		#endif

		glDrawArrays(m_primitiveType, 0, m_nNumVerts);
		
		#ifndef OPENGL_ES
			glBindVertexArray(0);
		#else
			glDisableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
			glDisableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
			glDisableVertexAttribArray(GLT_ATTRIBUTE_COLOR);

			for(U32 i = 0; i < m_nNumTextureUnits; i++)
			{
				if(m_uiTextureCoordArray[i] != 0)
				{
					glDisableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0 + i);
				}
			}
		#endif
	}

}
