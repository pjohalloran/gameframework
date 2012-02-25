// /////////////////////////////////////////////////////////////////
// @file CommonBatchSceneNode.cpp
// @author PJ O Halloran
// @date 18/11/2010
//
// Contains the CommonBatchSceneNode classes implementation.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <cstdio>
#include <cstdlib>

#include "GameBase.h"

// Project Headers
#include "CommonBatchSceneNode.h"

#include "GameMain.h"
#include "ZipFile.h"
#include "SceneGraphManager.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CommonBatchSceneNode::Init(const std::string &textureName, const std::string &shaderNameRef)
	{
		// If a custom shader name was supplied then we will use it.
		if(!shaderNameRef.empty())
		{
			SceneNode::SetShaderName(shaderNameRef);
		}

		// Load the texture from the texture manager.
		if(!textureName.empty())
		{
			m_texHandle = g_appPtr->GetTextureManagerPtr()->Load2D(textureName, GL_REPEAT);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	CommonBatchSceneNode::CommonBatchSceneNode(boost::optional<ActorId> actorId, const std::string &nodeName, const RenderPass renderPass, const Material &material, const Matrix4 &toWorld, const std::string &textureName, const std::string &shaderNameRef, boost::shared_ptr<IGLBatchBase> batchPtr)\
		: SceneNode(actorId, nodeName, renderPass, material, toWorld), m_batchPtr(batchPtr), m_texHandle()
	{
		Init(textureName, shaderNameRef);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	CommonBatchSceneNode::CommonBatchSceneNode(boost::optional<ActorId> actorId, const std::string &nodeName, const RenderPass renderPass, const Material &material, const Matrix4 &toWorld, const Matrix4 &fromWorld, const std::string &textureName, const std::string &shaderNameRef, boost::shared_ptr<IGLBatchBase> batchPtr)\
		: SceneNode(actorId, nodeName, renderPass, material, toWorld, fromWorld), m_batchPtr(batchPtr), m_texHandle()
	{
		Init(textureName, shaderNameRef);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	CommonBatchSceneNode::~CommonBatchSceneNode()
	{

	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CommonBatchSceneNode::VPreRender(SceneGraphManager *scenePtr)
	{
		// Call base class version first.
		bool result = SceneNode::VPreRender(scenePtr);

		if(result)
		{
			// Setup shader uniforms if a custom shader is being used to render nodes.
			if(!SceneNode::IsUsingCustomShader())
			{
				// This branch is the normal run of events...
				scenePtr->PrepareAdsShader(VGet()->GetMaterial(), m_texHandle);
			}
			else
			{
				// If were using the flat shader...
                std::string tmpStr(std::string("shaders") + ZipFile::ZIP_PATH_SEPERATOR + std::string("flat"));
				if(VGet()->GetShaderName().compare(tmpStr.c_str()) == 0)
				{
					GLint mvpLocation, colorLocation;
					mvpLocation = m_shaderPtr->GetUniformLocation("mvpMatrix");
					colorLocation = m_shaderPtr->GetUniformLocation("colorVec");
					if((mvpLocation == -1) || (colorLocation == -1))
					{
						result = false;
                        GF_LOG_TRACE_ERR("CommonBatchSceneNode::VPreRender()", "Failed to get the uniform locations for the flat shader");
					}
					if(result)
					{
						Matrix4 mvp;
						scenePtr->GetStackManager()->GetModelViewProjectionMatrix(mvp);
						glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, mvp.GetComponentsConst());
						glUniform4fv(colorLocation, 1, VGet()->GetMaterial().GetDiffuse().GetComponentsConst());
					}
				}
			}
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CommonBatchSceneNode::VRender(SceneGraphManager *scenePtr)
	{
		bool result = SceneNode::VRender(scenePtr);

		if(result && m_batchPtr)
		{
			m_batchPtr->VDraw();
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CommonBatchSceneNode::VPostRender(SceneGraphManager *scenePtr)
	{
		return (SceneNode::VPostRender(scenePtr));
	}

}
