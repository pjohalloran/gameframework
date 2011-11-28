// /////////////////////////////////////////////////////////////////
// @file GLSLShader.h
// @author PJ O Halloran
// @date 20/09/2010
//
// An object for loading a vertex and fragment shader and making it
// the current shader to use.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GF_GLSL_SHADER_H
#define __GF_GLSL_SHADER_H

// External Headers
#include "GameBase.h"

#include <map>
#include <string>

#include <boost/filesystem.hpp>

// Project Headers


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// A list of attribute names.  The index of the name in the container is the attribute index also.
	typedef std::vector<std::string> VSAttributeNameList;
	static const std::string g_ignoreShaderSlot("--ignore--");

	// /////////////////////////////////////////////////////////////////
	// @class GLSLShader
	// @author PJ O Halloran.
	//
	// A simple class for loading and building shaders into a GL shader
	// object and activating it.
	//
	// I saw no point in writing a C++ wrapper here over OpenGLs shader
	// methods so I give the user read only access to the program ID so 
	// they may set uniforms and other such operations directly with the
	// OpenGL API.
	//
	// This class exists to keep shader compiling and linking in one place
	// and to guarantee "glDeleteProgram" is called on the program object
	// when instances of this object go out of scope or are freed.
	//
	// /////////////////////////////////////////////////////////////////
	class GLSLShader
	{
	private:

		GLuint m_id;							///< The GL shader program ID.

		typedef std::map<std::string, GLint> UniformLocationMap;
		UniformLocationMap m_uniformMap;		///< Map of uniform variable names to their locations in the shader program.

		// /////////////////////////////////////////////////////////////////
		// Uses OpenGL to validate the program.
		//
		// @return bool True if the program contents are valid.
		//
		// /////////////////////////////////////////////////////////////////
		bool ValidateProgram() const;

		// /////////////////////////////////////////////////////////////////
		// Load the shader source from a file.
		//
		// @param filenameRef The name of the file on the filesystem.
		// @param The ID of the shader object.
		//
		// @bool True on sucess.
		//
		// /////////////////////////////////////////////////////////////////
		bool LoadSrcFromFile(const boost::filesystem::path &filenameRef, const GLuint shaderId) const;

		// /////////////////////////////////////////////////////////////////
		// Load the shader source from a char array.
		//
		// @param src The shader source code.
		// @param The ID of the shader object.
		//
		// @bool True on sucess.
		//
		// /////////////////////////////////////////////////////////////////
		bool LoadSrc(const char *src, const GLuint shaderId) const;

		// /////////////////////////////////////////////////////////////////
		// Parse through the shader source for the uniform variable names
		// so we may get their locations.
		//
		// The shader must have been succesfully compiled and activated
		// for this call to work successfully.
		//
		// @param src The shader source code.
		//
		// @return bool True|False on success or failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool ParseSrcForUniforms(const char *src);

		// /////////////////////////////////////////////////////////////////
		// Parse through the shader source for the uniform variable names
		// so we may get their locations.
		//
		// The shader must have been succesfully compiled and activated
		// for this call to work successfully.
		//
		// @param src The shader source code.
		//
		// @return bool True|False on success or failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool ParseFileForUniforms(const boost::filesystem::path &filenameRef);

		// /////////////////////////////////////////////////////////////////
		// Locate the program uniforms from the vector of program source lines.
		//
		// @param linesVec Container of source code lines.
		//
		// @return bool True|False on success or failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool LocateUniforms(const std::vector<std::string> &linesVec);

		// /////////////////////////////////////////////////////////////////
		// Build the GLSL program.
		//
		// @param vsFilename The vertex shader file.
		// @param gsFilename The geometry shader file.
		// @param fsFilename The fragment shader file.
		// @param vsAttList The list of attribute names to pass to the vertex
		//						shader.
		// @param messageRef A string containing details on any errors that
		//						might occur.
		// @param includeGeometryShader Bool, should we build and include the
		//								gemoetry shader.
		//
		// @return bool True on success.
		// @return bool False if either the vertex or fragement shader files
		//					do not exist.
		// @return bool False if the geometry shader is to be included but the
		//					the gemoetr file does not exist.
		// @return bool False if we fail to build or link the shaders to create
		//				the GLSL program.
		//
		// /////////////////////////////////////////////////////////////////
		bool BuildProgramFromFiles(const boost::filesystem::path &vsFilename, const boost::filesystem::path &gsFilename, const boost::filesystem::path &fsFilename, const VSAttributeNameList &vsAttList, std::string &messageRef, const bool includeGeometryShader = false);

		// /////////////////////////////////////////////////////////////////
		// Build the GLSL program from src char arrays.
		//
		// @param vsSrc The vertex shader src.
		// @param gsSrc The geometry shader src.
		// @param fsSrc The fragment shader src.
		// @param vsAttList The list of attribute names to pass to the vertex
		//						shader.
		// @param messageRef A string containing details on any errors that
		//						might occur.
		// @param includeGeometryShader Bool, should we build and include the
		//								gemoetry shader.
		//
		// @return bool True on success.
		// @return bool False if either the vertex or fragement shader src
		//					is NULL.
		// @return bool False if the geometry shader is to be included but the
		//					the gemoetry src is NULL.
		// @return bool False if we fail to build or link the shaders to create
		//				the GLSL program.
		//
		// /////////////////////////////////////////////////////////////////
		bool BuildProgramFromSrc(const char *vsSrc, const char *gsSrc, const char *fsSrc, const VSAttributeNameList &vsAttList, std::string &messageRef, const bool includeGeometryShader = false);

		// /////////////////////////////////////////////////////////////////
		// @class ShaderProgram
		// @author PJ O Halloran
		//
		// Handles creating and freeing a shader program and provides access
		// the the GL handle for the OpenGL shader functions.
		//
		// /////////////////////////////////////////////////////////////////
		class ShaderProgram
		{
		private:

			GLuint m_sid;					///< The shader program handle.
			GLenum m_type;					///< The type of shader.

		public:

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// @param type The shader type (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER
			//				or GL_GEOMETRY_SHADER).
			//
			// /////////////////////////////////////////////////////////////////
			ShaderProgram(GLenum type) : m_sid(0), m_type(type) { 
                GF_CLEAR_GL_ERROR(); 
                m_sid = glCreateShader(m_type); 
                GF_CHECK_GL_ERROR(); };

			// /////////////////////////////////////////////////////////////////
			// Destructor.
			//
			// /////////////////////////////////////////////////////////////////
			~ShaderProgram() { try { GF_CLEAR_GL_ERROR(); glDeleteShader(m_sid); GF_CHECK_GL_ERROR_TRC("~ShaderProgram(): "); } catch(...) {} };

			// /////////////////////////////////////////////////////////////////
			// Get the shader handle.
			//
			// /////////////////////////////////////////////////////////////////
			GLuint GetShaderId() { return (m_sid); };
		};

	public:

		// /////////////////////////////////////////////////////////////////
		// Default constructor.
		//
		// /////////////////////////////////////////////////////////////////
		GLSLShader() : m_id(0) { };

		// /////////////////////////////////////////////////////////////////
		// Destructor.  
		// 
		// Guarantees that the shader program will be freed.
		//
		// /////////////////////////////////////////////////////////////////
		~GLSLShader();

		// /////////////////////////////////////////////////////////////////
		// Builds and compiles the vertex and fragment shaders.
		// The vertex shaders input names and indices and set with the list
		// of vertex attribute names also supplied.
		//
		// If a program was already held by this object, it will be replaced
		// on successfull run of this method.
		//
		// @param std::string Vertex shader filename.
		// @param std::string Fragment shader filename.
		// @param VSAttributeNameList List of vertex shader attribute names.
		// @param std::string String to store any error messages inside if
		//						we fail to build the shader program.
		//
		// @return bool True if the shader was built successfully with
		//				the vertex and fragement shaders and false otherwise.
		//				If this method fails check the GL error state and the
		//				input string parameter for more information. You will
		//				need to rewrite the shaders manually to correct them
		//				in this case.
		//
		// /////////////////////////////////////////////////////////////////
		inline bool Build(const boost::filesystem::path &vsFilename, const boost::filesystem::path &fsFilename, const VSAttributeNameList &vsAttList, std::string &messageRef)
		{
			return (BuildProgramFromFiles(vsFilename, boost::filesystem::path(), fsFilename, vsAttList, messageRef, false));
		};

		// /////////////////////////////////////////////////////////////////
		// Builds and compiles the vertex, geometry and fragment shaders.
		// The vertex shaders input names and indices and set with the list
		// of vertex attribute names also supplied.
		//
		// If a program was already held by this object, it will be replaced
		// on successfull run of this method.
		//
		// @param std::string Vertex shader filename.
		// @param std::string Geometry shader filename.
		// @param std::string Fragment shader filename.
		// @param VSAttributeNameList List of vertex shader attribute names.
		// @param std::string String to store any error messages inside if
		//						we fail to build the shader program.
		//
		// @return bool True if the shader was built successfully with
		//				the vertex and fragement shaders and false otherwise.
		//				If this method fails check the GL error state and the
		//				input string parameter for more information. You will
		//				need to rewrite the shaders manually to correct them
		//				in this case.
		//
		// /////////////////////////////////////////////////////////////////
		inline bool Build(const boost::filesystem::path &vsFilename, const boost::filesystem::path &gsFilename, const boost::filesystem::path &fsFilename, const VSAttributeNameList &vsAttList, std::string &messageRef)
		{
			return (BuildProgramFromFiles(vsFilename, gsFilename, fsFilename, vsAttList, messageRef, true));
		};

		// /////////////////////////////////////////////////////////////////
		// Builds and compiles the vertex and fragment shaders from source.
		// The vertex shaders input names and indices and set with the list
		// of vertex attribute names also supplied.
		//
		// If a program was already held by this object, it will be replaced
		// on successfull run of this method.
		//
		// @param std::string Vertex shader source.
		// @param std::string Fragment shader source.
		// @param VSAttributeNameList List of vertex shader attribute names.
		// @param std::string String to store any error messages inside if
		//						we fail to build the shader program.
		//
		// @return bool True if the shader was built successfully with
		//				the vertex and fragement shaders and false otherwise.
		//				If this method fails check the GL error state and the
		//				input string parameter for more information. You will
		//				need to rewrite the shaders manually to correct them
		//				in this case.
		//
		// /////////////////////////////////////////////////////////////////
		inline bool Build(const char *vsSrc, const char *fsSrc, const VSAttributeNameList &vsAttList, std::string &messageRef)
		{
			return (BuildProgramFromSrc(vsSrc, NULL, fsSrc, vsAttList, messageRef));
		};

		// /////////////////////////////////////////////////////////////////
		// Builds and compiles the vertex, geometry and fragment shaders
		// from source.
		// The vertex shaders input names and indices and set with the list
		// of vertex attribute names also supplied.
		//
		// If a program was already held by this object, it will be replaced
		// on successfull run of this method.
		//
		// @param std::string Vertex shader source.
		// @param std::string Geometry shader source.
		// @param std::string Fragment shader source.
		// @param VSAttributeNameList List of vertex shader attribute names.
		// @param std::string String to store any error messages inside if
		//						we fail to build the shader program.
		//
		// @return bool True if the shader was built successfully with
		//				the vertex and fragement shaders and false otherwise.
		//				If this method fails check the GL error state and the
		//				input string parameter for more information. You will
		//				need to rewrite the shaders manually to correct them
		//				in this case.
		//
		// /////////////////////////////////////////////////////////////////
		inline bool Build(const char *vsSrc, const char *gsSrc, const char *fsSrc, const VSAttributeNameList &vsAttList, std::string &messageRef)
		{
			return (BuildProgramFromSrc(vsSrc, NULL, fsSrc, vsAttList, messageRef));
		};

		// /////////////////////////////////////////////////////////////////
		// Make this shader program the current GL rendering shader program.
		//
		// This method will not do anything if this program was not yet built
		// successfully.
		//
		// Please note that you should run this before you try to set uniforms
		// for the shaders. Alternatively grab the program ID and call
		// "glUseProgram" yourself.  Either is safe.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool Activate() const;

		// /////////////////////////////////////////////////////////////////
		// Check if this shader program is the current program in use by GL.
		//
		// /////////////////////////////////////////////////////////////////
		bool IsActivated() const;

		// /////////////////////////////////////////////////////////////////
		// Check if this program has been built successfully yet.
		//
		// /////////////////////////////////////////////////////////////////
		bool IsBuilt() const;

		// /////////////////////////////////////////////////////////////////
		// Get the program Id.
		//
		// This will not hold a valid value until you Build the program with
		// a VS/FS or a VS/GS/FS.
		//
		// /////////////////////////////////////////////////////////////////
		inline GLuint GetProgramId() const { return (m_id); };

		// /////////////////////////////////////////////////////////////////
		// Get the location of a program uniform from the uniforms cached
		// earlier on program build.
		//
		// @param uniformName The name of the uniform to search for.
		//
		// @return GLint -1 if not found or val >= 0 on success.
		//
		// /////////////////////////////////////////////////////////////////
		GLint GetUniformLocation(const std::string &uniformName);

		// /////////////////////////////////////////////////////////////////
		// Frees the GLSL program.
		//
		// Sometimes you might want to ensure that the GLSL program is removed
		// from the GPU.
		//
		// /////////////////////////////////////////////////////////////////
		void FreeProgram();

	};

	// /////////////////////////////////////////////////////////////////
	// Builds a shader after retrieving its source from the global
	// ResourceCache manager.
	//
	// @param shaderName The ID/path of the shaders to retrieve from the
	//						resource cache manager (excluding the final
	//						shader extension, e.g. "shaders\\flat" or
	//						"flat").
	// @param vsNameList List of vertex attributes order information.
	//
	// @return boost::shared_ptr<GLSLShader> A pointer to a GLSLShader
	//											object or NULL on failure.
	//
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<GLSLShader> BuildShaderFromResourceCache(const std::string &shaderName, const VSAttributeNameList &vsNameList);

}

#endif
