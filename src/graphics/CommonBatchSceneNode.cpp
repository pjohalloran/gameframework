// /////////////////////////////////////////////////////////////////
// @file CommonBatchSceneNode.cpp
// @author PJ O Halloran
// @date 18/11/2010
//
// Contains the CommonBatchSceneNode classes implementation.
//
// /////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cstdlib>

#include "GameBase.h"
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
            
            // If were using the flat shader...
            std::string tmpStr(std::string("shaders") + ZipFile::ZIP_PATH_SEPERATOR + std::string("flat"));
            if(VGet()->GetShaderName().compare(tmpStr.c_str()) == 0)
            {
                m_mvpUniform = m_shaderPtr->GetUniform("mvpMatrix");
                m_colorUniform = m_shaderPtr->GetUniform("colorVec");
                if((m_mvpUniform == NULL) || (m_colorUniform == NULL))
                {
                    GF_LOG_TRACE_ERR("CommonBatchSceneNode::Init()", "Failed to get the uniform locations for the flat shader");
                }
            }
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
	CommonBatchSceneNode::CommonBatchSceneNode(SceneGraphManager *sgPtr,
												boost::optional<ActorId> actorId,
												const std::string &nodeName,
												const RenderPass renderPass,
												const Material &material,
												const Matrix4 &toWorld,
												const std::string &textureName,
												const std::string &shaderNameRef,
												boost::shared_ptr<IGLBatchBase> batchPtr)\
												: SceneNode(sgPtr, actorId, nodeName, renderPass, material, toWorld)
												, m_batchPtr(batchPtr)
												, m_texHandle()
												, m_mvpUniform()
												, m_colorUniform()
	{
		Init(textureName, shaderNameRef);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	CommonBatchSceneNode::CommonBatchSceneNode(SceneGraphManager *sgPtr,
												boost::optional<ActorId> actorId,
												const std::string &nodeName,
												const RenderPass renderPass,
												const Material &material,
												const Matrix4 &toWorld,
												const Matrix4 &fromWorld,
												const std::string &textureName,
												const std::string &shaderNameRef,
												boost::shared_ptr<IGLBatchBase> &batchPtr)\
												: SceneNode(sgPtr, actorId, nodeName, renderPass, material, toWorld, fromWorld)
												, m_batchPtr(batchPtr)
												, m_texHandle()
												, m_mvpUniform()
												, m_colorUniform()
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
	bool CommonBatchSceneNode::VPreRender()
	{
		// Call base class version first.
		bool result = SceneNode::VPreRender();

		if(result)
		{
			// Setup shader uniforms if a custom shader is being used to render nodes.
			if(!SceneNode::IsUsingCustomShader())
			{
				// This branch is the normal run of events...
				m_sgmPtr->PrepareAdsShader(VGet()->GetMaterial(), m_texHandle);
			}
			else
			{
				// Flat shader support only right now.
                Matrix4 mvp;
                m_sgmPtr->GetStackManager()->GetModelViewProjectionMatrix(mvp);
                m_mvpUniform->SetValue((GLfloat * const)mvp.GetComponentsConst(), 16);
                m_colorUniform->SetValue((GLfloat * const)VGet()->GetMaterial().GetDiffuse().GetComponentsConst(), 4);
                
                m_shaderPtr->Activate();
			}
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CommonBatchSceneNode::VRender()
	{
		bool result = SceneNode::VRender();

		if(result && m_batchPtr)
			m_batchPtr->VDraw();

		return (result);
	}

}
