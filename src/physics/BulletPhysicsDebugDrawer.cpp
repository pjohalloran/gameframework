// /////////////////////////////////////////////////////////////////
// @file BulletPhysicsDebugDrawer.cpp
// @author PJ O Halloran
// @date 14/10/2010
//
// Implemetation of the bullet system debug renderer.
//
// /////////////////////////////////////////////////////////////////


// External Headers
#include "GameBase.h"

// Project Headers
#include "BulletPhysicsDebugDrawer.h"

#include "GameMain.h"
#include "TextResource.h"
#include "Matrix.h"
#include "GLBatch.h"
#include "GameColors.h"

////////////////////////// TEMPORARY TEMPORARY TEMPORARY - On SnowLeopard this is suppored, but GLEW doens't hook up properly
////////////////////////// Fixed probably in 10.6.3
//#ifdef __APPLE__
//#define glGenVertexArrays glGenVertexArraysAPPLE
//#define glDeleteVertexArrays  glDeleteVertexArraysAPPLE
//#define glBindVertexArray	glBindVertexArrayAPPLE
//#endif

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool BulletPhysicsDebugDrawer::CreateBuffers()
	{
		// Clear GL error on entry.
		GLenum errCode = glGetError();

		// Generate the VBO and VOA objects.
		glGenVertexArrays(1, &m_vaoId);
		if(m_vaoId == 0)
		{
            GF_LOG_TRACE_ERR("BulletPhysicsDebugDrawer::CreateBuffers()", "Failed to generate the VAO");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("BulletPhysicsDebugDrawer::CreateBuffers()"),\
									std::string("Failed to generate the VAO"));
			return (false);
		}
		glGenBuffers(1, &m_vboId);
		if(m_vboId == 0)
		{
            GF_LOG_TRACE_ERR("BulletPhysicsDebugDrawer::CreateBuffers()", "Failed to generate the VBO");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("BulletPhysicsDebugDrawer::CreateBuffers()"),\
									std::string("Failed to generate the VBO"));
			return (false);
		}

		// Bind to the VBO so we can copy data to the area of GPU memory.
		glBindBuffer(GL_ARRAY_BUFFER, m_vboId);

		// Number of bytes required to store positions, tex coords and normals in the VBO.
		I32 posSize = 2 * 4 * sizeof(GLfloat);				// 4 elements per vertex * 2 vertices * sizeof(F32).
		I32 bufferSize = posSize;							// Size of VBO.

		// Allocate memory and a type for the currently bound VBO (we will be updating this frequently so we use GL_DYNAMIC_DRAW).
		glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW);
		errCode = glGetError();

		// Bind to the VAO to save the vertex attributes state to speed up rendering.
		glBindVertexArray(m_vaoId);

		// Positions at index 0 in VBO.
		GLuint index = 0;
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);

		// Unbind buffers.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		errCode = glGetError();
		return (errCode == GL_NO_ERROR);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool BulletPhysicsDebugDrawer::LoadVertexBuffer(const btVector3 &from, const btVector3 &to)
	{
		GLfloat positions[] = {from.x(), from.y(), from.z(), 1.0f,
								to.x(), to.y(), to.z(), 1.0f};

		GLenum err = glGetError();

		// Copy the line vertices into the VBO.
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
		err = glGetError();

		return (err == GL_NO_ERROR);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	BulletPhysicsDebugDrawer::BulletPhysicsDebugDrawer(boost::shared_ptr<ModelViewProjStackManager> mvpStackManagerPtr, const std::string shaderName) throw (GameException &) \
		: m_vaoId(0), m_vboId(0), m_shaderPtr(), m_mvpStackManagerPtr(mvpStackManagerPtr), m_debugMode(0), m_mvpLoc(-1), m_colorLoc(-1)
	{
		if(!m_mvpStackManagerPtr || shaderName.empty())
		{
			throw GameException(std::string("Parameters passed to BulletPhysicsDebugDrawer() are invalid"));
		}

		// Build the GLSL shader.
		VSAttributeNameList vsAttList;
		vsAttList.push_back(std::string("vertexPos"));
		m_shaderPtr = GameHalloran::BuildShaderFromResourceCache(shaderName, vsAttList);
		if(!m_shaderPtr)
		{
			throw GameException(std::string("Failed to initialize the GLSL program"));
		}

		m_mvpLoc = m_shaderPtr->GetUniformLocation("mvpMatrix");
		m_colorLoc = m_shaderPtr->GetUniformLocation("colorVec");

		if(!CreateBuffers())
		{
			throw GameException(std::string("Failed to create the VBO for the debug drawer"));
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	BulletPhysicsDebugDrawer::~BulletPhysicsDebugDrawer()
	{
		try
		{
			if(glIsBuffer(m_vboId) == GL_TRUE)
			{
				glDeleteBuffers(1, &m_vboId);
			}
			if(glIsVertexArray(m_vaoId) == GL_TRUE)
			{
				glDeleteVertexArrays(1, &m_vaoId);
			}
		}
		catch(...)
		{
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BulletPhysicsDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
	{
		// Setup the shader.
		if(!m_shaderPtr->Activate())
		{
            GF_LOG_TRACE_ERR("BulletPhysicsDebugDrawer::drawLine()", "Failed to activate the shader");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("BulletPhysicsDebugDrawer::drawLine()"),\
									std::string("Failed to activate the shader"));
			return;
		}

		Matrix4 mvp;
		m_mvpStackManagerPtr->GetModelViewProjectionMatrix(mvp);
		glUniformMatrix4fv(m_mvpLoc, 1, GL_FALSE, mvp.GetComponentsConst());
		glUniform4fv(m_colorLoc, 1, color.m_floats);

		GLenum err = glGetError();

		glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
		LoadVertexBuffer(from, to);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		err = glGetError();

		glBindVertexArray(m_vaoId);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);

		err = glGetError();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BulletPhysicsDebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color)
	{
		const btVector3 endPoint = PointOnB + normalOnB * distance;	
		drawLine(PointOnB, endPoint, color);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BulletPhysicsDebugDrawer::reportErrorWarning(const char *warningString)
	{
		if(!warningString)
		{
			return;
		}
        GF_LOG_ERR(std::string("Bullet DebugDraw Error: ") + std::string(warningString));
//		SafeGameLog(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("Bullet DebugDraw Error: ") + std::string(warningString));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BulletPhysicsDebugDrawer::draw3dText(const btVector3 &location, const char *textString)
	{
		// TODO: Implement once i figure out how to render text using OpenGL...
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BulletPhysicsDebugDrawer::setDebugMode(I32 debugMode)
	{
		m_debugMode = debugMode;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	I32 BulletPhysicsDebugDrawer::getDebugMode() const
	{
		return (m_debugMode);
	}

}
