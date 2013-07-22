// GLShaderManager.h
/*
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
// @file GLShaderManager.h
// @author Richard S. Wright Jr & PJ O Halloran
// @date 10/09/2010
//
// A class for managing OpenGL shaders.
// Original source from the OpenGL Superbible 5th edition.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GLT_SHADER_MANAGER
#define __GLT_SHADER_MANAGER

// External Headers
#include "GamePlatform.h"

//#include <vector>

// Project Headers


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran {

    // Maximum length of shader name
#define MAX_SHADER_NAME_LENGTH  64

    // /////////////////////////////////////////////////////////////////
    // @enum GLT_STOCK_SHADER
    //
    // The types of stock shaders available.
    // - GLT_SHADER_IDENTITY:
    // Draws geometry using default coordinate system.
    // - GLT_SHADER_FLAT:
    // Allows a transformation matrix for geometry transformations.
    // - GLT_SHADER_SHADED:
    // Color values are intorpolated (smooth shading).
    // - GLT_SHADER_DEFAULT_LIGHT:
    // A light that comes from the eye position.
    // - GLT_SHADER_POINT_LIGHT_DIFF:
    // Light position may be specified.
    // - GLT_SHADER_TEXTURE_REPLACE:
    // Applies a texture bound to the texture unit.
    // - GLT_SHADER_TEXTURE_MODULATE:
    // Multiplies a base color by the texture unit.
    // - GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF:
    // Allows a light to be positioned in the scene and applies a texture
    // and a light value.
    // - GLT_SHADER_TEXTURE_RECT_REPLACE:
    // ??
    //
    // /////////////////////////////////////////////////////////////////
    enum GLT_STOCK_SHADER {
        GLT_SHADER_IDENTITY = 0,
        GLT_SHADER_FLAT,
        GLT_SHADER_SHADED,
        GLT_SHADER_DEFAULT_LIGHT,
        GLT_SHADER_POINT_LIGHT_DIFF,
        GLT_SHADER_TEXTURE_REPLACE,
        GLT_SHADER_TEXTURE_MODULATE,
        GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,
        GLT_SHADER_TEXTURE_RECT_REPLACE,
        GLT_SHADER_LAST
    };

    // /////////////////////////////////////////////////////////////////
    // @enum GLT_SHADER_ATTRIBUTE
    //
    // List of attributes for the stock shaders.
    // - GLT_ATTRIBUTE_VERTEX:
    // - GLT_ATTRIBUTE_COLOR:
    // - GLT_ATTRIBUTE_NORMAL:
    // - GLT_ATTRIBUTE_TEXTURE0:
    // - GLT_ATTRIBUTE_TEXTURE1:
    // - GLT_ATTRIBUTE_TEXTURE2:
    // - GLT_ATTRIBUTE_TEXTURE3:
    //
    // /////////////////////////////////////////////////////////////////
    enum GLT_SHADER_ATTRIBUTE {
        GLT_ATTRIBUTE_VERTEX = 0,
        GLT_ATTRIBUTE_COLOR,
        GLT_ATTRIBUTE_NORMAL,
        GLT_ATTRIBUTE_TEXTURE0,
        GLT_ATTRIBUTE_TEXTURE1,
        GLT_ATTRIBUTE_TEXTURE2,
        GLT_ATTRIBUTE_TEXTURE3,
        GLT_ATTRIBUTE_LAST
    };

    // /////////////////////////////////////////////////////////////////
    // @struct SHADERLOOKUPETRY
    //
    // Holds a vertex and fragment shader name and a shader Id.
    //
    // /////////////////////////////////////////////////////////////////
    struct SHADERLOOKUPETRY {
        char szVertexShaderName[MAX_SHADER_NAME_LENGTH];
        char szFragShaderName[MAX_SHADER_NAME_LENGTH];
        GLuint  uiShaderID;
    };

    // /////////////////////////////////////////////////////////////////
    // @class GLShaderManager
    // @author Richard S. Wright Jr & PJ O Halloran
    //
    // A manager for a group of simple shaders.
    //
    // /////////////////////////////////////////////////////////////////
    class GLShaderManager {
    private:

        GLuint uiStockShaders[GLT_SHADER_LAST];                         ///< Array of stock shaders.
        //std::vector <SHADERLOOKUPETRY> shaderTable;                   ///< Shader lookup table.

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // /////////////////////////////////////////////////////////////////
        GLShaderManager(void);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        ~GLShaderManager(void);

        // /////////////////////////////////////////////////////////////////
        // Call before using the stock shaders.
        //
        //
        // /////////////////////////////////////////////////////////////////
        bool InitializeStockShaders(void);

        // /////////////////////////////////////////////////////////////////
        // Find one of the standard stock shaders and return it's shader handle.
        //
        // /////////////////////////////////////////////////////////////////
        GLuint GetStockShader(GLT_STOCK_SHADER nShaderID);

        // /////////////////////////////////////////////////////////////////
        // Use a stock shader, and pass in the parameters needed
        //
        // /////////////////////////////////////////////////////////////////
        GLint UseStockShader(GLT_STOCK_SHADER nShaderID, ...);

        // /////////////////////////////////////////////////////////////////
        // Load a shader pair from file, return NULL or shader handle.
        // Vertex program name (minus file extension)
        // is saved in the lookup table
        //
        // /////////////////////////////////////////////////////////////////
        GLuint LoadShaderPair(const char *szVertexProgFileName, const char *szFragProgFileName);

        // /////////////////////////////////////////////////////////////////
        // Load shaders from source text.
        //
        // /////////////////////////////////////////////////////////////////
        GLuint LoadShaderPairSrc(const char *szName, const char *szVertexSrc, const char *szFragSrc);

        // /////////////////////////////////////////////////////////////////
        // Ditto above, but pop in the attributes.
        //
        // /////////////////////////////////////////////////////////////////
        GLuint LoadShaderPairWithAttributes(const char *szVertexProgFileName, const char *szFragmentProgFileName, ...);

        // /////////////////////////////////////////////////////////////////
        // Load shaders from source text but with attributes/
        //
        // /////////////////////////////////////////////////////////////////
        GLuint LoadShaderPairSrcWithAttributes(const char *szName, const char *szVertexProg, const char *szFragmentProg, ...);

        // /////////////////////////////////////////////////////////////////
        // Lookup a previously loaded shader
        //
        // /////////////////////////////////////////////////////////////////
        GLuint LookupShader(const char *szVertexProg, const char *szFragProg = 0);

    };

}

#endif
