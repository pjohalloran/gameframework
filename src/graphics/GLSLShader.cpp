// /////////////////////////////////////////////////////////////////
// @file GLSLShader.cpp
// @author PJ O Halloran
// @date 20/09/2010
//
// An object for loading a vertex and fragment shader and making it
// the current shader to use.
//
// /////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

#include <boost/algorithm/string.hpp>

#include "GLSLShader.h"
#include "GameBase.h"
#include "GameMain.h"
#include "TextResource.h"
#include "HashedString.h"

using std::string;

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLSLShader::FreeProgram()
	{
		if(IsBuilt())
		{
			GF_CLEAR_GL_ERROR();
            
			glDeleteProgram(m_id);
            GF_CHECK_GL_ERROR_TRC("GLSLShader::FreeProgram(): ");
			
            m_id = 0;
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GLSLShader::ValidateProgram() const
	{
		GLint valid;
		
        GF_CLEAR_GL_ERROR();
        
		glValidateProgram(m_id);
		glGetProgramiv(m_id, GL_VALIDATE_STATUS, &valid);
		if(valid == 0)
		{
			return (false);
		}

		return (GF_CHECK_GL_ERROR_TRC("GLSLShader::ValidateProgram(): "));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	GLSLShader::~GLSLShader()
	{
		try
		{
			FreeProgram();
		}
		catch(...)
		{
		}
	}
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    ShaderUniformSPtr GLSLShader::GetUniform(const std::string &name)
    {
        HashedString id(name.c_str());
        for (UniformArray::iterator i = m_uniforms.begin(), end = m_uniforms.end(); i != end; ++i)
        {
            if((*i)->GetId() == id.getHashValue())
            {
                return (*i);
            }
        }
        
        return (ShaderUniformSPtr());
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GLSLShader::UpdateUniformsToGPU()
    {
        for(DirtyList::iterator i = m_dirtyList.begin(), end = m_dirtyList.end(); i != end; ++i)
        {
            (*i)->VClean();
        }
        m_dirtyList.clear();
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GLSLShader::VNotifyDirty(ICleanable *ptr)
    {
        if(ptr)
            m_dirtyList.push_back(ptr);
    }

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GLSLShader::LoadSrcFromFile(const boost::filesystem::path &filenameRef, const GLuint shaderId) const
	{
		bool result = true;				// Result of method.
		GLint fileSize = 0;				// Length of the shader source file.
		FILE *fp;						// File pointer.
		GLubyte *shaderSrc = NULL;		// Pointer to the buffer allocated for the shader.

		// Open the shader file
		fp = fopen(filenameRef.string().c_str(), "r");
		if(fp != NULL)
		{
			// See how long the file is
			while (fgetc(fp) != EOF)
			{
				++fileSize;
			}
			
			// Allocate a block of memory to send in the shader (+1 for an extra NULL terminated character at end.)
			shaderSrc = GCC_NEW GLubyte[fileSize+1];
			if(!shaderSrc)
			{
				result = false;
			}
			
			if(result)
			{
				// Go back to beginning of file
				rewind(fp);
				fread(shaderSrc, 1, fileSize, fp);
				shaderSrc[fileSize] = '\0';
			}
		}
		else
		{
			result = false;
		}
		
		// Load the string into the shader program.
		if(result)
		{
			GLchar *fsStringPtr[1];
			fsStringPtr[0] = reinterpret_cast<GLchar *>(shaderSrc);

			GF_CLEAR_GL_ERROR();

			glShaderSource(shaderId, 1, (const GLchar **)fsStringPtr, NULL);

			result = GF_CHECK_GL_ERROR_TRC("GLSLShader::LoadSrcFromFile(): ");
		}
	    
		// Close file, free memory and return result.
		if(fp)
		{
			fclose(fp);
			fp = NULL;
		}
		if(shaderSrc)
		{
			delete [] shaderSrc;
			shaderSrc = NULL;
		}
		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GLSLShader::LoadSrc(const char *src, const GLuint shaderId) const
	{
		if(!src)
		{
			return (false);
		}

		GF_CLEAR_GL_ERROR();

		// Load the string into the shader program.
		GLchar *fsStringPtr[1];
		char *nonConstSrc = const_cast<char *>(src);
		fsStringPtr[0] = reinterpret_cast<GLchar *>(nonConstSrc);
		glShaderSource(shaderId, 1, (const GLchar **)fsStringPtr, NULL);

		return (GF_CHECK_GL_ERROR_TRC("GLSLShader::LoadSrc(): "));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GLSLShader::ParseSrcForUniforms(const char *src)
	{
		// Ensure input is valid and GLSL program is built and activated.
		if(!src || !IsBuilt())
		{
			return (false);
		}
		if(!IsActivated())
		{
			Activate();
		}

		std::istringstream inputStream(src);

		// NB. Max supported shader line size is set to 1024 for to find uniforms automatically.
		const U32 SIZE = 1024;
		char buffer[SIZE];
		memset(buffer, 0, sizeof(char) * SIZE);
		std::vector<std::string> linesVec;
		linesVec.reserve(10000);

		while(!inputStream.eof())
		{
			inputStream.getline(buffer, SIZE);
			linesVec.push_back(std::string(buffer));
		}

		// Trim excess vector capacity
		std::vector<std::string>(linesVec.begin(), linesVec.end()).swap(linesVec);
		return (LocateUniforms(linesVec));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GLSLShader::ParseFileForUniforms(const boost::filesystem::path &filenameRef)
	{
		// Ensure input is valid and GLSL program is built and activated.
		if(!boost::filesystem::exists(filenameRef) || !IsBuilt())
		{
			return (false);
		}
		if(!IsActivated())
		{
			Activate();
		}

		std::ifstream inputStream(filenameRef.string().c_str());
		if(!inputStream.is_open())
		{
			return (false);
		}

		// NB. Max supported shader line size is set to 1024 for to find uniforms automatically.
		const U32 SIZE = 1024;
		char buffer[SIZE];
		memset(buffer, 0, sizeof(char) * SIZE);
		std::vector<std::string> linesVec;
		linesVec.reserve(256);

		while(!inputStream.eof())
		{
			inputStream.getline(buffer, SIZE);
			linesVec.push_back(std::string(buffer));
		}

		// Trim excess vector capacity
		std::vector<std::string>(linesVec.begin(), linesVec.end()).swap(linesVec);
		return (LocateUniforms(linesVec));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GLSLShader::LocateUniforms(const std::vector<std::string> &linesVec)
	{
		const std::string SEMICOLON_STR(";");
		const std::string WHITESPACE_STR(" ");
		const std::string ARRAY_LEFT_BRACKET_STR("[");
		const std::string UNIFORM_KEYWORD_STR("uniform");
		std::vector<std::string> statements, uniformSections, arraySections;
		std::string currVariableName;

		GF_CLEAR_GL_ERROR();

		// Iterate through each line in the source file looking for uniform declarations.
		for(std::vector<std::string>::const_iterator i = linesVec.begin(), end = linesVec.end(); i != end; ++i)
		{
			// A line might have many GLSL statements...
			boost::algorithm::split(statements, *i, boost::algorithm::is_any_of(SEMICOLON_STR));

			// Parse through all statements in the current line.
			for(std::vector<std::string>::iterator stateIter = statements.begin(), stateEnd = statements.end(); stateIter != stateEnd; ++stateIter)
			{
				// Only consider lines beginning with the uniform keyword.
				if(boost::algorithm::starts_with(*stateIter, UNIFORM_KEYWORD_STR))
				{
					// We found a uniform declaration!
					boost::algorithm::split(uniformSections, *stateIter, boost::algorithm::is_any_of(WHITESPACE_STR));
					if(uniformSections.size() == 3)
					{
						// Handle variable names that are arrays
						boost::algorithm::split(arraySections, uniformSections[2], boost::algorithm::is_any_of(ARRAY_LEFT_BRACKET_STR));

						if(arraySections.size() == 2)
						{
							currVariableName = arraySections[0];
							arraySections.clear();
						}
						else
						{
							currVariableName = uniformSections[2];
						}

						// Get the location of the uniform in the compiled shader source according to the uniform name.
                        GLint loc = glGetUniformLocation(m_id, static_cast<const GLchar *>(currVariableName.c_str()));
						m_uniformMap[currVariableName] = loc;
#if DEBUG
						GLenum errCode = glGetError();
						if(errCode != GL_NO_ERROR)
						{
                            GF_LOG_ERR(std::string("Error finding the uniform location for \"") + currVariableName + std::string("\":") + GameHalloran::GetOpenGLError(errCode));
							return (false);
						}
#endif
                        m_uniforms.push_back(boost::shared_ptr<ShaderUniform>(new ShaderUniform(loc, currVariableName, this)));
					}
					else
					{
						// compile error - should never happen if OpenGL says the program has already been compiled.
						return (false);
					}

					uniformSections.clear();
				}
			}

			statements.clear();
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GLSLShader::BuildProgramFromFiles(const boost::filesystem::path &vsFilename, const boost::filesystem::path &gsFilename, const boost::filesystem::path &fsFilename, const VSAttributeNameList &vsAttList, std::string &messageRef, const bool includeGeometryShader)
	{
		bool result = true;
		
		// Clear error message on entry.
		if(!messageRef.empty())
		{
			messageRef.clear();
		}

		// Check shader source code files exist on the filesystem.
		if(!boost::filesystem::exists(vsFilename))
		{
			messageRef.assign(string("The vertex shader file does not exist at the path specified."));
			result = false;
		}
		if(result && !boost::filesystem::exists(fsFilename))
		{
			messageRef.assign(string("The fragment shader file does not exist at the path specified."));
			result = false;
		}
		if(result && includeGeometryShader && !boost::filesystem::exists(gsFilename))
		{
			messageRef.assign(string("The gemetry shader file does not exist at the path specified."));
			result = false;
		}

		if(result)
		{
			ShaderProgram vsProg(GL_VERTEX_SHADER);			// Vertex shader program.
			ShaderProgram gsProg(GL_GEOMETRY_SHADER);		// Gemoetry shader.
			ShaderProgram fsProg(GL_FRAGMENT_SHADER);		// Fragment shader program.
			
			// A) Load the shader programs from a file.
			if(!LoadSrcFromFile(vsFilename, vsProg.GetShaderId()))
			{
				messageRef.assign(string("Failed to load the vertex shader source."));
				result = false;
			}
			if(result && includeGeometryShader && !LoadSrcFromFile(gsFilename, gsProg.GetShaderId()))
			{
				messageRef.assign(string("Failed to load the geometry shader source."));
				result = false;
			}
			if(result && !LoadSrcFromFile(fsFilename, fsProg.GetShaderId()))
			{
				messageRef.assign(string("Failed to load the fragment shader source."));
				result = false;
			}

			// B) Compile shader programs and check for errors.
			if(result)
			{
                GF_CLEAR_GL_ERROR();
                
				glCompileShader(vsProg.GetShaderId());
                GF_CHECK_GL_ERROR();
				if(includeGeometryShader)
				{
					glCompileShader(gsProg.GetShaderId());
                    GF_CHECK_GL_ERROR();
				}
				glCompileShader(fsProg.GetShaderId());
                GF_CHECK_GL_ERROR();
				
				// Check for errors in all shaders
				GLint compileStatus;

				// 1) Vertex shader.
				glGetShaderiv(vsProg.GetShaderId(), GL_COMPILE_STATUS, &compileStatus);
				if(compileStatus == GL_FALSE)
				{
					const I32 bufSize = 1024;
					char buf[bufSize];
					glGetShaderInfoLog(vsProg.GetShaderId(), bufSize, NULL, buf);
					messageRef.assign(string("The vertex shader failed to compile, error: ") + string(buf));
					result = false;
				}
                GF_CHECK_GL_ERROR();

				// 2) Geometry shader.
				if(result && includeGeometryShader)
				{
					glGetShaderiv(gsProg.GetShaderId(), GL_COMPILE_STATUS, &compileStatus);
					if(compileStatus == GL_FALSE)
					{
						const I32 bufSize = 1024;
						char buf[bufSize];
						glGetShaderInfoLog(gsProg.GetShaderId(), bufSize, NULL, buf);
						messageRef.assign(string("The geometry shader failed to compile, error: ") + string(buf));
						result = false;
					}
                    GF_CHECK_GL_ERROR();
				}

				// 3) Fragment shader.
				if(result)
				{
					glGetShaderiv(fsProg.GetShaderId(), GL_COMPILE_STATUS, &compileStatus);
					if(compileStatus == GL_FALSE)
					{
						const I32 bufSize = 1024;
						char buf[bufSize];
						glGetShaderInfoLog(fsProg.GetShaderId(), bufSize, NULL, buf);
						messageRef.assign(string("The fragment shader failed to compile, error: ") + string(buf));
						result = false;
					}
                    GF_CHECK_GL_ERROR();
				}
			}

			// C) Create GLSL program and link shader programs. Bind attribute names
			//		for the vertex shader. Check for linking errors.
			if(result)
			{
				// Create the final program object, and attach the shaders
				GLuint tmpGlslProgId = glCreateProgram();
				glAttachShader(tmpGlslProgId, vsProg.GetShaderId());
                GF_CHECK_GL_ERROR();
				if(includeGeometryShader)
				{
					glAttachShader(tmpGlslProgId, gsProg.GetShaderId());
                    GF_CHECK_GL_ERROR();
				}
				glAttachShader(tmpGlslProgId, fsProg.GetShaderId());
                GF_CHECK_GL_ERROR();


				// Bind the attribute names to their specific locations.
				I32 index = 0;
				for(VSAttributeNameList::const_iterator curr = vsAttList.begin(), end = vsAttList.end(); curr != end; ++curr, ++index)
				{
					// If we are not supposed to ignore this slot.
					if((*curr).compare(g_ignoreShaderSlot.c_str()) != 0)
					{
						// Bind the attribute to the current slot index.
						glBindAttribLocation(tmpGlslProgId, index, const_cast<char *>((*curr).c_str()));
                        GF_CHECK_GL_ERROR();
					}
				}

				// Attempt to link and check result.
				GLint linkStatus;
				glLinkProgram(tmpGlslProgId);
                
                // Clear out GL error on release builds first...
                GLenum errCode;
                do
                {
                    errCode = glGetError();
                } while(errCode != GL_NO_ERROR);
                
				glGetProgramiv(tmpGlslProgId, GL_LINK_STATUS, &linkStatus);

                // NB: Leave following in all build modes to cleanup shader resources on any errors!
                errCode = glGetError();
				if(linkStatus == GL_FALSE || errCode != GL_NO_ERROR)
				{
					const I32 bufSize = 1024;
					char buf[bufSize];
					glGetProgramInfoLog(tmpGlslProgId, bufSize, NULL, buf);
					messageRef.assign(string("Failed to link GLSL program, error: ") + string(buf));
					glDeleteProgram(tmpGlslProgId);
					result = false;
				}

				// Set the new GLSL program ID and search for shader uniform locations before exit.
				if(result)
				{
					FreeProgram();
					m_id = tmpGlslProgId;

					// Search for the shader uniforms now that the GLSL program has been compiled and linked.
					if(!ParseFileForUniforms(vsFilename))
					{
						result = false;
					}
					if(result && includeGeometryShader && !ParseFileForUniforms(gsFilename))
					{
						result = false;
					}
					if(result && !ParseFileForUniforms(fsFilename))
					{
						result = false;
					}
				}
			}
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GLSLShader::BuildProgramFromSrc(const char *vsSrc, const char *gsSrc, const char *fsSrc, const VSAttributeNameList &vsAttList, std::string &messageRef, const bool includeGeometryShader)
	{
		bool result = true;
		
		// Clear error message on entry.
		if(!messageRef.empty())
		{
			messageRef.clear();
		}

		// Check shader source code is given.
		if(!vsSrc)
		{
			messageRef.assign(string("The vertex shader source is NULL."));
			result = false;
		}
		if(result && !fsSrc)
		{
			messageRef.assign(string("The fragment shader source is NULL."));
			result = false;
		}
		if(result && includeGeometryShader && !gsSrc)
		{
			messageRef.assign(string("The geometry shader source is NULL."));
			result = false;
		}

		if(result)
		{
			ShaderProgram vsProg(GL_VERTEX_SHADER);			// Vertex shader program.
			ShaderProgram gsProg(GL_GEOMETRY_SHADER);		// Gemoetry shader.
			ShaderProgram fsProg(GL_FRAGMENT_SHADER);		// Fragment shader program.
			
			// A) Load the shader programs from a file.
			if(!LoadSrc(vsSrc, vsProg.GetShaderId()))
			{
				messageRef.assign(string("Failed to load the vertex shader source."));
				result = false;
			}
			if(result && includeGeometryShader && !LoadSrc(gsSrc, gsProg.GetShaderId()))
			{
				messageRef.assign(string("Failed to load the geometry shader source."));
				result = false;
			}
			if(result && !LoadSrc(fsSrc, fsProg.GetShaderId()))
			{
				messageRef.assign(string("Failed to load the fragment shader source."));
				result = false;
			}

			// B) Compile shader programs and check for errors.
			if(result)
			{
                GF_CLEAR_GL_ERROR();
                
				glCompileShader(vsProg.GetShaderId());
                GF_CHECK_GL_ERROR();
				if(includeGeometryShader)
				{
					glCompileShader(gsProg.GetShaderId());
                    GF_CHECK_GL_ERROR();
				}
				glCompileShader(fsProg.GetShaderId());
                GF_CHECK_GL_ERROR();
				
				// Check for errors in all shaders
				GLint compileStatus;

				// 1) Vertex shader.
				glGetShaderiv(vsProg.GetShaderId(), GL_COMPILE_STATUS, &compileStatus);
				if(compileStatus == GL_FALSE)
				{
					const I32 bufSize = 1024;
					char buf[bufSize];
					glGetShaderInfoLog(vsProg.GetShaderId(), bufSize, NULL, buf);
					messageRef.assign(string("The vertex shader failed to compile, error: ") + string(buf));
					result = false;
				}
                GF_CHECK_GL_ERROR();

				// 2) Geometry shader.
				if(result && includeGeometryShader)
				{
					glGetShaderiv(gsProg.GetShaderId(), GL_COMPILE_STATUS, &compileStatus);
					if(compileStatus == GL_FALSE)
					{
						const I32 bufSize = 1024;
						char buf[bufSize];
						glGetShaderInfoLog(gsProg.GetShaderId(), bufSize, NULL, buf);
						messageRef.assign(string("The geometry shader failed to compile, error: ") + string(buf));
						result = false;
					}
                    GF_CHECK_GL_ERROR();
				}

				// 3) Fragment shader.
				if(result)
				{
					glGetShaderiv(fsProg.GetShaderId(), GL_COMPILE_STATUS, &compileStatus);
					if(compileStatus == GL_FALSE)
					{
						const I32 bufSize = 1024;
						char buf[bufSize];
						glGetShaderInfoLog(fsProg.GetShaderId(), bufSize, NULL, buf);
						messageRef.assign(string("The fragment shader failed to compile, error: ") + string(buf));
						result = false;
					}
                    GF_CHECK_GL_ERROR();
				}
			}

			// C) Create GLSL program and link shader programs. Bind attribute names
			//		for the vertex shader. Check for linking errors.
			if(result)
			{
				// Create the final program object, and attach the shaders
				GLuint tmpGlslProgId = glCreateProgram();
                GF_CHECK_GL_ERROR();
                
				glAttachShader(tmpGlslProgId, vsProg.GetShaderId());
                GF_CHECK_GL_ERROR();
				if(includeGeometryShader)
				{
					glAttachShader(tmpGlslProgId, gsProg.GetShaderId());
                    GF_CHECK_GL_ERROR();
				}
				glAttachShader(tmpGlslProgId, fsProg.GetShaderId());
                GF_CHECK_GL_ERROR();

				// Bind the attribute names to their specific locations.
				I32 index = 0;
				for(VSAttributeNameList::const_iterator curr = vsAttList.begin(), end = vsAttList.end(); curr != end; ++curr, ++index)
				{
					// If we are not supposed to ignore this slot.
					if((*curr).compare(g_ignoreShaderSlot.c_str()) != 0)
					{
						// Bind the attribute to the current slot index.
						glBindAttribLocation(tmpGlslProgId, index, const_cast<char *>((*curr).c_str()));
                        GF_CHECK_GL_ERROR();
					}
				}

				// Attempt to link and check result.
				GLint linkStatus;
				glLinkProgram(tmpGlslProgId);
                
                // Clear out GL error on release builds first...
                GLenum errCode;
                do
                {
                    errCode = glGetError();
                } while(errCode != GL_NO_ERROR);
                
				glGetProgramiv(tmpGlslProgId, GL_LINK_STATUS, &linkStatus);

                // NB - Leave check in on all builds to clean up GPU resources on any error..
				errCode = glGetError();
				if(linkStatus == GL_FALSE || errCode != GL_NO_ERROR)
				{
					const I32 bufSize = 1024;
					char buf[bufSize];
					glGetProgramInfoLog(tmpGlslProgId, bufSize, NULL, buf);
					messageRef.assign(string("Failed to link GLSL program, error: ") + string(buf));
					glDeleteProgram(tmpGlslProgId);
					result = false;
				}

				// Set the new GLSL program ID and search for uniform locations before exit.
				if(result)
				{
					FreeProgram();
					m_id = tmpGlslProgId;

					// Search for the shader uniforms now that the GLSL program has been compiled and linked.
					if(!ParseSrcForUniforms(vsSrc))
					{
						result = false;
					}
					if(result && includeGeometryShader && !ParseSrcForUniforms(gsSrc))
					{
						result = false;
					}
					if(result && !ParseSrcForUniforms(fsSrc))
					{
						result = false;
					}
				}
			}
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GLSLShader::Activate()
	{
		if(!IsBuilt())
		{
			return (false);
		}
		if(IsActivated())
		{
            UpdateUniformsToGPU();
			return (true);
		}

		GF_CLEAR_GL_ERROR();

		glUseProgram(m_id);
        UpdateUniformsToGPU();

		return (GF_CHECK_GL_ERROR_TRC("GLSLShader::Activate(): "));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GLSLShader::IsActivated() const
	{
		GLint currProgId;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currProgId);
		return (m_id == static_cast<GLuint>(currProgId));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GLSLShader::IsBuilt() const
	{
		return (glIsProgram(m_id) == GL_TRUE);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	GLint GLSLShader::GetUniformLocation(const std::string &uniformName)
	{
		UniformLocationMap::iterator i = m_uniformMap.find(uniformName);
		if(i == m_uniformMap.end())
		{
			// No record of uniform in the cache!
			return (-1);
		}
		return ((*i).second);
	}
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool GLSLShader::SetUniform(const std::string &name, const GLint value, const bool forceCopyToGpu)
    {
        boost::shared_ptr<ShaderUniform> uniform = GetUniform(name);
        if(!uniform)
        {
            return (false);
        }
        
        uniform->SetValue(value, forceCopyToGpu);
        return (true);
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool GLSLShader::SetUniform(const std::string &name, const GLfloat value, const bool forceCopyToGpu)
    {
        boost::shared_ptr<ShaderUniform> uniform = GetUniform(name);
        if(!uniform)
        {
            return (false);
        }
        
        uniform->SetValue(value, forceCopyToGpu);
        return (true);
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool GLSLShader::SetUniform(const std::string &name, const Vector3 &value, const bool forceCopyToGpu)
    {
        boost::shared_ptr<ShaderUniform> uniform = GetUniform(name);
        if(!uniform)
        {
            return (false);
        }
        
        uniform->SetValue((GLfloat*)value.GetComponentsConst(), 3, forceCopyToGpu);
        return (true);
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool GLSLShader::SetUniform(const std::string &name, const Vector4 &value, const bool forceCopyToGpu)
    {
        boost::shared_ptr<ShaderUniform> uniform = GetUniform(name);
        if(!uniform)
        {
            return (false);
        }
        
        uniform->SetValue((GLfloat*)value.GetComponentsConst(), 4, forceCopyToGpu);
        return (true);
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool GLSLShader::SetUniform(const std::string &name, const Point3 &value, const bool forceCopyToGpu)
    {
        boost::shared_ptr<ShaderUniform> uniform = GetUniform(name);
        if(!uniform)
        {
            return (false);
        }
        
        uniform->SetValue((GLfloat*)value.GetComponentsConst(), 3, forceCopyToGpu);
        return (true);
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool GLSLShader::SetUniform(const std::string &name, const Matrix4 &value, const bool forceCopyToGpu)
    {
        boost::shared_ptr<ShaderUniform> uniform = GetUniform(name);
        if(!uniform)
        {
            return (false);
        }
        
        uniform->SetValue((GLfloat*)value.GetComponentsConst(), 16, forceCopyToGpu);
        return (true);
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool GLSLShader::SetUniform(const std::string &name, const Matrix3x3 value, const bool forceCopyToGpu)
    {
        boost::shared_ptr<ShaderUniform> uniform = GetUniform(name);
        if(!uniform)
        {
            return (false);
        }
        
        uniform->SetValue((GLfloat*)value, 9, forceCopyToGpu);
        return (true);
    }

	// /////////////////////////////////////////////////////////////////
	// ******************** MISC Helper functions **********************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<GLSLShader> BuildShaderFromResourceCache(const std::string &shaderName, const VSAttributeNameList &vsNameList)
	{
		boost::shared_ptr<GLSLShader> null;
		boost::shared_ptr<GLSLShader> shaderObj;

		// Check input parameters.
		if(shaderName.empty())
		{
            GF_LOG_TRACE_ERR("BuildShaderFromResourceCache()", "The shader name is empty");
			return (null);
		}
		if(vsNameList.empty())
		{
            GF_LOG_TRACE_ERR("BuildShaderFromResourceCache()", "The VS attribute vector is empty");
			return (null);
		}

		const U32 SIZE = 3;							// Number of shader types.
		const U32 optionalI = 1;						// Index of the optional (geometry) shader.
		std::string shaderExt[SIZE];							// Array of shader extensions.
		boost::shared_ptr<TextResHandle> shaderTextArr[SIZE];	// Array of shader text data.
		bool error = false;										// Error flag.

		// Initialize the arrays
		shaderExt[0] = std::string(".vp");
		shaderExt[1] = std::string(".gp");
		shaderExt[2] = std::string(".fp");
		for(U32 index = 0; index < SIZE; ++index)
		{
			shaderTextArr[index] = boost::shared_ptr<TextResHandle>();
		}

		// Deal with every shader name/ID in the vector.
		// Load in vertex and fragment shader and also the geometry shader if its exists...
		error = false;
		for(U32 index = 0; ((!error) && (index < SIZE)); ++index)
		{
			std::string filename(shaderName + shaderExt[index]);

			// Try to retrieve shader text from resource cache.
			TextResource tr(filename);
			shaderTextArr[index] = boost::static_pointer_cast<TextResHandle>(g_appPtr->GetResourceCache()->GetHandle(&tr));

			// Check if we got it and initialize it.
			if(!shaderTextArr[index] || !shaderTextArr[index]->VInitialize())
			{
				if(index != optionalI)
				{
                    GF_LOG_TRACE_ERR("BuildShaderFromResourceCache()", std::string("Failed to retrieve ") + filename + std::string(" from the resource cache so we cannot build this shader"));
					error = true;
				}
				else
				{
                    GF_LOG_TRACE_DEB("BuildShaderFromResourceCache()", std::string("No geometry shader (") + filename + std::string(") in the resource cache for this shader"));
				}
			}
		}

		// Build the shader if we got the required shader source files from the resource cache.
		if(!error)
		{
			shaderObj.reset(GCC_NEW GLSLShader());
			if(!shaderObj)
			{
                GF_LOG_TRACE_ERR("BuildShaderFromResourceCache()", "Failed to allocate memory for the new GLSLShader object");
			}
			else
			{
				std::string errorMsg;					// Error messages from GLSL compiler.
				bool buildResult = true;				// Result of GLSL program build.

				// Case 1: The program has a geometry shader.
				if(shaderTextArr[optionalI])
				{
					buildResult = shaderObj->Build(shaderTextArr[0]->GetTextBuffer(), shaderTextArr[1]->GetTextBuffer(), shaderTextArr[2]->GetTextBuffer(), vsNameList, errorMsg);
				}
				// Case 2: The program has just a vertex and fragment shader.
				else
				{
					buildResult = shaderObj->Build(shaderTextArr[0]->GetTextBuffer(), shaderTextArr[2]->GetTextBuffer(), vsNameList, errorMsg);
				}

				if(!buildResult)
				{
                    GF_LOG_TRACE_ERR("BuildShaderFromResourceCache()", std::string("Failed to build the ") + shaderName + std::string(" shader: ") + errorMsg);
					error = true;
				}
			}
		}

		return (error ? null : shaderObj);
	}

}
