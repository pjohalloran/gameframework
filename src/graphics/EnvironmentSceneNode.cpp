// /////////////////////////////////////////////////////////////////
// @file EnvironmentSceneNode.cpp
// @author PJ O Halloran
// @date 06/10/2010
//
// Contains the EnvironmentSceneNode classes implementaion.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <boost/optional.hpp>


// Project Headers
#include "EnvironmentSceneNode.h"
#include "SceneGraphManager.h"
#include "GameMain.h"
#include "GLTools.h"

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void EnvironmentSceneNode::Init(const std::vector<std::string> &cubemapTextureNameVec, const std::string &shaderNameRef, const F32 cmRadius) throw (GameException &)
	{
		// Load the textures for the sides of the cubemap.
		boost::optional<TexHandle> tHandle = g_appPtr->GetTextureManagerPtr()->LoadCubeMap(cubemapTextureNameVec, GL_CLAMP_TO_EDGE);
		if(!tHandle.is_initialized())
		{
			throw GameException(std::string("Failed to load the textures for the EnvironmentMap"));
		}

		m_texHandle = *tHandle;

		SceneNode::SetRadius(cmRadius);
		gltMakeCube(m_cubeBatch, cmRadius);

		// Set the shader name (The SGM should already have this shader built and included).
		SceneNode::SetShaderName(shaderNameRef);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	EnvironmentSceneNode::EnvironmentSceneNode(boost::optional<ActorId> actorId, const Matrix4 &toWorld, const std::vector<std::string> &cubemapTextureNameVec, const std::string &shaderNameRef, const F32 cmRadius) throw (GameException &)\
		: SceneNode(actorId, std::string("EnvironmentSceneNode"), RenderPassSky, Material(), toWorld), m_texHandle(0), m_mvpLocation(-1), m_cmLocation(-1)
	{
		Init(cubemapTextureNameVec, shaderNameRef, cmRadius);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	EnvironmentSceneNode::EnvironmentSceneNode(boost::optional<ActorId> actorId, const Matrix4 &toWorld, const Matrix4 &fromWorld, const std::vector<std::string> &cubemapTextureNameVec, const std::string &shaderNameRef, const F32 cmRadius) throw (GameException &)\
		: SceneNode(actorId, std::string("EnvironmentSceneNode"), RenderPassSky, Material(), toWorld, fromWorld), m_texHandle(0), m_mvpLocation(-1), m_cmLocation(-1)
	{
		Init(cubemapTextureNameVec, shaderNameRef, cmRadius);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	EnvironmentSceneNode::~EnvironmentSceneNode()
	{
		// TextureManager takes care of freeing the Textures.
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool EnvironmentSceneNode::VPreRender(SceneGraphManager *scenePtr)
	{
		bool result = false;

		result = SceneNode::VPreRender(scenePtr);
		
		// Set the uniform locations.
		if(result)
		{
			m_mvpLocation = m_shaderPtr->GetUniformLocation("mvpMatrix");
			m_cmLocation = m_shaderPtr->GetUniformLocation("cubeMap");
			if((m_mvpLocation == -1) || (m_cmLocation == -1))
			{
                GF_LOG_TRACE_ERR("EnvironmentSceneNode::VPreRender()", std::string("Failed to set one or all of the uniforms for ") + VGet()->GetShaderName());
				result = false;
			}
		}

		// Activate the cubemap texture on the first texture unit/layer.
		if(result && !g_appPtr->GetTextureManagerPtr()->Bind(m_texHandle, GL_TEXTURE_CUBE_MAP, GL_TEXTURE0))
		{
            GF_LOG_TRACE_ERR("EnvironmentSceneNode::VPreRender()", "Failed to activate the CubeMap texture");
			result = false;
		}

		// Enable seemless smoothing along the edges of the cubemap.
		if(result)
		{
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

			// Get the camera matrix and clear the cameras position (we want to be able to rotate the environment box but not move it!).
			Matrix4 camMatrix(scenePtr->GetCamera()->VGet()->GetToWorld());
			camMatrix[Matrix4::M30] = 0.0f;
			camMatrix[Matrix4::M31] = 0.0f;
			camMatrix[Matrix4::M32] = 0.0f;
			camMatrix[Matrix4::M33] = 1.0f;
			scenePtr->GetStackManager()->GetModelViewMatrixStack()->LoadMatrix(camMatrix);
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool EnvironmentSceneNode::VRender(SceneGraphManager *scenePtr)
	{
		bool result = SceneNode::VRender(scenePtr);

		if(result)
		{
			// Send the uniforms to the shader.
			Matrix4 mvp;
			scenePtr->GetStackManager()->GetModelViewProjectionMatrix(mvp);
			glUniformMatrix4fv(m_mvpLocation, 1, GL_FALSE, mvp.GetComponentsConst());
			glUniform1i(m_cmLocation, 0);

			// Draw the cubemap.
			m_cubeBatch.VDraw();

			// Check if we are in debug mode, if so we should query OpenGL To see if the draw succeeded!
			if(g_appPtr->GetLoggerPtr() && g_appPtr->GetLoggerPtr()->GetLogLevel() >= GameLog::DEB)
			{
				GLenum error = glGetError();
				if(error != GL_NO_ERROR)
				{
                    GF_LOG_TRACE_ERR("EnvironmentSceneNode::VRender()", "The rendering of the cubemap failed");
					result = false;
				}
			}
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool EnvironmentSceneNode::VPostRender(SceneGraphManager *scenePtr)
	{
		// Disable it now.
		glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		if(!scenePtr)
		{
			return (false);
		}

		return (SceneNode::VPostRender(scenePtr));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool EnvironmentSceneNode::VOnRestore(SceneGraphManager *scenePtr)
	{
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool EnvironmentSceneNode::VOnLostDevice(SceneGraphManager *scenePtr)
	{
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool EnvironmentSceneNode::VOnUpdate(SceneGraphManager *scenePtr, const F32 elapsedTime)
	{
		return (true);
	}

}
