// GLTools.h
// OpenGL SuperBible
/* Copyright 1998 - 2003 Richard S. Wright Jr.
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

#ifndef __GLTOOLS__LIBRARY
#define __GLTOOLS__LIBRARY

// External Headers
// Bring in OpenGL 
// Windows
#ifdef WIN32
#include <windows.h>		// Must have for Windows platform builds
//#ifndef GLEW_STATIC
//#define GLEW_STATIC
//#endif

#include <gl\glew.h>			// OpenGL Extension "autoloader"
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#endif

// Mac OS X
#ifdef __APPLE__
#include <stdlib.h>

#include <TargetConditionals.h>
#if TARGET_OS_IPHONE | TARGET_IPHONE_SIMULATOR
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define OPENGL_ES
#else
#include <GL/glew.h>
#include <OpenGL/gl.h>		// Apple OpenGL haders (version depends on OS X SDK version)
#endif
#endif

// Linux
#ifdef linux
#define GLEW_STATIC
#include <glew.h>
#endif

////////////////////////// TEMPORARY TEMPORARY TEMPORARY - On SnowLeopard this is suppored, but GLEW doens't hook up properly
////////////////////////// Fixed probably in 10.6.3
//#ifdef __APPLE__
//#define glGenVertexArrays glGenVertexArraysAPPLE
//#define glDeleteVertexArrays  glDeleteVertexArraysAPPLE
//#define glBindVertexArray	glBindVertexArrayAPPLE
//#ifndef OPENGL_ES
//#define glGenerateMipmap    glGenerateMipmapEXT
//#endif
//#endif

#include <stdio.h>
#include <math.h>

// Project Headers
#include "GLBatch.h"
#include "GLTriangleBatch.h"
#include "Matrix.h"
#include "Vector.h"
#include "CommonMath.h"

namespace GameHalloran
{

	// There is a static block allocated for loading shaders to 
	// prevent heap fragmentation
	#define MAX_SHADER_LENGTH   8192

	///////////////////////////////////////////////////////
	// Macros for big/little endian happiness
	// These are intentionally written to be easy to understand what they 
	// are doing... no flames please on the inefficiency of these.
	#ifdef __BIG_ENDIAN__
	///////////////////////////////////////////////////////////
	// This function says, "this pointer is a little endian value"
	// If the value must be changed it is... otherwise, this
	// function is defined away below (on Intel systems for example)
	inline void LITTLE_ENDIAN_WORD(void *pWord)
		{
		unsigned char *pBytes = (unsigned char *)pWord;
		unsigned char temp;
	    
		temp = pBytes[0];
		pBytes[0] = pBytes[1];
		pBytes[1] = temp;
		}

	///////////////////////////////////////////////////////////
	// This function says, "this pointer is a little endian value"
	// If the value must be changed it is... otherwise, this
	// function is defined away below (on Intel systems for example)
	inline void LITTLE_ENDIAN_DWORD(void *pWord)
		{
		unsigned char *pBytes = (unsigned char *)pWord;
		unsigned char temp;
	    
		// Swap outer bytes
		temp = pBytes[3];
		pBytes[3] = pBytes[0];
		pBytes[0] = temp;
	    
		// Swap inner bytes
		temp = pBytes[1];
		pBytes[1] = pBytes[2];
		pBytes[2] = temp;
		}
	#else

	// Define them away on little endian systems
	#define LITTLE_ENDIAN_WORD 
	#define LITTLE_ENDIAN_DWORD 
	#endif

	// Define targa header.  Used here and also in ImageResource.cpp
	#pragma pack(1)
	typedef struct
	{
		GLbyte	identsize;              // Size of ID field that follows header (0)
		GLbyte	colorMapType;           // 0 = None, 1 = paletted
		GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
		unsigned short	colorMapStart;          // First colour map entry
		unsigned short	colorMapLength;         // Number of colors
		unsigned char 	colorMapBits;   // bits per palette entry
		unsigned short	xstart;                 // image x origin
		unsigned short	ystart;                 // image y origin
		unsigned short	width;                  // width in pixels
		unsigned short	height;                 // height in pixels
		GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
		GLbyte	descriptor;             // image descriptor
	} TGAHEADER;
	#pragma pack(8)

	// These structures match the layout of the equivalent Windows specific structs 
	// used by Win32.  Used here and in ImageResource.cpp.
	#pragma pack(1)
	struct RGB { 
	  GLbyte blue;
	  GLbyte green;
	  GLbyte red;
	  GLbyte alpha;
	};

	struct BMPInfoHeader {
	  GLuint	size;
	  GLuint	width;
	  GLuint	height;
	  GLushort  planes;
	  GLushort  bits;
	  GLuint	compression;
	  GLuint	imageSize;
	  GLuint	xScale;
	  GLuint	yScale;
	  GLuint	colors;
	  GLuint	importantColors;
	};

	struct BMPHeader {
	  GLushort	type; 
	  GLuint	size; 
	  GLushort	unused; 
	  GLushort	unused2; 
	  GLuint	offset; 
	}; 

	struct BMPInfo {
	  BMPInfoHeader		header;
	  RGB				colors[1];
	};
	#pragma pack(8)

	///////////////////////////////////////////////////////////////////////////////
	//         THE LIBRARY....
	///////////////////////////////////////////////////////////////////////////////

	// Get the OpenGL version
	void gltGetOpenGLVersion(GLint &nMajor, GLint &nMinor);

	// Check to see if an exension is supported
	int gltIsExtSupported(const char *szExtension);

	// Set working directoyr to /Resources on the Mac
	void gltSetWorkingDirectory(const char *szArgv);

	///////////////////////////////////////////////////////////////////////////////
	GLbyte* gltReadBMPBits(const char *szFileName, int *nWidth, int *nHeight);

	/////////////////////////////////////////////////////////////////////////////////////
	// Load a .TGA file
	GLbyte *gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat, GLbyte *pDat = NULL);

	// Capture the frame buffer and write it as a .tga
	// Does not work on the iPhone
	#ifndef OPENGL_ES
	GLint gltGrabScreenTGA(const char *szFileName);
	#endif


	// Make Objects
	void gltMakeTorus(GLTriangleBatch& torusBatch, GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor);
	void gltMakeSphere(GLTriangleBatch& sphereBatch, GLfloat fRadius, GLint iSlices, GLint iStacks);
	void gltMakeDisk(GLTriangleBatch& diskBatch, GLfloat innerRadius, GLfloat outerRadius, GLint nSlices, GLint nStacks);
	void gltMakeCylinder(GLTriangleBatch& cylinderBatch, GLfloat baseRadius, GLfloat topRadius, GLfloat fLength, GLint numSlices, GLint numStacks);
	void gltMakeCube(GLBatch& cubeBatch, GLfloat fRadius);

	// Shader loading support
	void	gltLoadShaderSrc(const char *szShaderSrc, GLuint shader);
	bool	gltLoadShaderFile(const char *szFile, GLuint shader);

	GLuint	gltLoadShaderPair(const char *szVertexProg, const char *szFragmentProg);
	GLuint   gltLoadShaderPairWithAttributes(const char *szVertexProg, const char *szFragmentProg, ...);

	GLuint gltLoadShaderPairSrc(const char *szVertexSrc, const char *szFragmentSrc);
	GLuint gltLoadShaderPairSrcWithAttributes(const char *szVertexProg, const char *szFragmentProg, ...);

	bool gltCheckErrors(GLuint progName = 0);
	void gltGenerateOrtho2DMat(GLuint width, GLuint height, Matrix4 &orthoMatrix, GLBatch &screenQuad);

	inline void NormalizeNormalArr(NormalArr arr)
	{
		Vector3 normVec(arr[0], arr[1], arr[2]);
		normVec.Normalize();
		arr[0] = normVec.GetX();
		arr[1] = normVec.GetY();
		arr[2] = normVec.GetZ();
	};

}

#endif
