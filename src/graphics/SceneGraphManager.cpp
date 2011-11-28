//========================================================================
// Part of the GameCode3 Application
//
// GameCode3 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 3rd Edition" by Mike McShaffry, published by
// Charles River Media. ISBN-10: 1-58450-680-6   ISBN-13: 978-1-58450-680-5
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the author a favor and buy a new copy of the book - there are 
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: 
//    http://www.amazon.com/gp/product/1584506806?ie=UTF8&tag=gamecodecompl-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=1584506806
//
// There's a companion web site at:
// http://www.mcshaffry.com/GameCode/
// 
// The source code is managed and maintained through Google Code:
// http://gamecode3.googlecode.com/svn/trunk/
//
// (c) Copyright 2009 Michael L. McShaffry
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License v2
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================

// /////////////////////////////////////////////////////////////////
// @file SceneGraphManager.cpp
// @author Mike McShaffry & PJ O Halloran
// @date 04/10/2010
//
// Implementation of the SceneGraph manager class.
//
// /////////////////////////////////////////////////////////////////

// External Headers


// Project Headers
#include "SceneGraphManager.h"

#include "GameBase.h"
#include "GameMain.h"

#include "RootSceneNode.h"

#include "ZipFile.h"
#include "TextResource.h"


namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SceneGraphManager::SetupGlobalShaderUniformCache()
	{
		if(!m_globalShaderPtr)
		{
			m_adsUniformCache.Reset();
			return;
		}

		m_adsUniformCache.m_applyTexLoc = m_globalShaderPtr->GetUniformLocation("u_applyTexture");
		m_adsUniformCache.m_texture2dMapLoc = m_globalShaderPtr->GetUniformLocation("u_texture2dMap");

		m_adsUniformCache.m_mvpLoc = m_globalShaderPtr->GetUniformLocation("u_mvpMatrix");
		m_adsUniformCache.m_mvLoc = m_globalShaderPtr->GetUniformLocation("u_mvMatrix");
		m_adsUniformCache.m_normalLoc = m_globalShaderPtr->GetUniformLocation("u_normalMatrix");

		m_adsUniformCache.m_numLightsLoc = m_globalShaderPtr->GetUniformLocation("u_numberLights");
		m_adsUniformCache.m_lightTypesLoc = m_globalShaderPtr->GetUniformLocation("u_lightTypesArr");
		m_adsUniformCache.m_lightPosLoc = m_globalShaderPtr->GetUniformLocation("u_lightPositionArr");
		m_adsUniformCache.m_lightAmbLoc = m_globalShaderPtr->GetUniformLocation("u_lightAmbientArr");
		m_adsUniformCache.m_lightDiffLoc = m_globalShaderPtr->GetUniformLocation("u_lightDiffuseArr");
		m_adsUniformCache.m_lightSpecLoc = m_globalShaderPtr->GetUniformLocation("u_lightSpecularArr");
		m_adsUniformCache.m_spotCutoffLoc = m_globalShaderPtr->GetUniformLocation("u_spotlightCutoffArr");
		m_adsUniformCache.m_spotExpLoc = m_globalShaderPtr->GetUniformLocation("u_spotlightExpArr");
		m_adsUniformCache.m_spotDirLoc = m_globalShaderPtr->GetUniformLocation("u_spotlightDirection");
		m_adsUniformCache.m_constantAttLoc = m_globalShaderPtr->GetUniformLocation("u_cAttArr");
		m_adsUniformCache.m_linearAttLoc = m_globalShaderPtr->GetUniformLocation("u_lAttArr");
		m_adsUniformCache.m_quadAttLoc = m_globalShaderPtr->GetUniformLocation("u_qAttArr");
		m_adsUniformCache.m_globalAmbLoc = m_globalShaderPtr->GetUniformLocation("u_globalAmbient");

		m_adsUniformCache.m_matEmmLoc = m_globalShaderPtr->GetUniformLocation("u_materialE");
		m_adsUniformCache.m_matAmbLoc = m_globalShaderPtr->GetUniformLocation("u_materialA");
		m_adsUniformCache.m_matDiffLoc = m_globalShaderPtr->GetUniformLocation("u_materialD");
		m_adsUniformCache.m_matSpecLoc = m_globalShaderPtr->GetUniformLocation("u_materialS");
		m_adsUniformCache.m_matExpLoc = m_globalShaderPtr->GetUniformLocation("u_materialExp");
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SceneGraphManager::RenderAlphaPass()
	{
		// Save the GL and modelview matrix stack state before alpha pass.
		m_stackManagerPtr->GetModelViewMatrixStack()->PushMatrix();
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Ensure the alpha nodes are sorted.
		m_alphaNodeList.sort();

		while (!m_alphaNodeList.empty())
		{
			AlphaSceneNodeList::reverse_iterator i = m_alphaNodeList.rbegin();

			m_stackManagerPtr->GetModelViewMatrixStack()->PushMatrix((*i)->GetMatrix());
			(*i)->GetNode()->VRender(this);
			m_stackManagerPtr->GetModelViewMatrixStack()->PopMatrix();

			// Since they are shared ptrs, this frees the alpha node and reduces the lists length.
			m_alphaNodeList.pop_back();
		}

		// Restore state pre alpha pass.
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		m_stackManagerPtr->GetModelViewMatrixStack()->PopMatrix();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneGraphManager::ScriptAddDynamicLight(LuaPlus::LuaObject lightData)
	{
		bool result = true;

		try
		{
			Light lightObj(lightData);
			result = AddDynamicLight(boost::shared_ptr<Light>(GCC_NEW Light(lightObj)));
		}
		catch(GameException &ge)
		{
			// TODO: Popup dialog to inform scripter of error.
            GF_LOG_TRACE_ERR("SceneGraphManager::ScriptAddDynamicLight()", std::string("Failed to add a light: ") + std::string(ge.what()));
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("SceneGraphManager::ScriptAddDynamicLight()"),\
									std::string("Failed to add a light: ") + std::string(ge.what()));
			result = false;
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneGraphManager::ScriptSetGlobalIllumination(LuaPlus::LuaObject globalIllData)
	{
		bool result = true;

		try
		{
			Light lightObj(globalIllData);
			SetGlobalIllumination(lightObj.GetAmbient());
		}
		catch(GameException &ge)
		{
			// TODO: Popup dialog to inform scripter of error.
            GF_LOG_TRACE_ERR("SceneGraphManager::ScriptSetGlobalIllumination()", std::string("Failed to set global light: ") + std::string(ge.what()));
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("SceneGraphManager::ScriptSetGlobalIllumination()"),\
									std::string("Failed to set global light: ") + std::string(ge.what()));
			result = false;
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	SceneGraphManager::SceneGraphManager(boost::shared_ptr<ModelViewProjStackManager> stackManagerPtr)\
		: m_adsUniformCache(), m_root(), m_camera(), m_stackManagerPtr(stackManagerPtr), m_alphaNodeList(), m_actorMap(), m_shaderMap(),\
			m_ambientLightSrc(), m_dynamicLights(), m_globalShaderPtr(), m_metaTable()
	{
		m_root.reset(GCC_NEW RootSceneNode);

		// Set up the global illumination ambient light in the scene.
		m_ambientLightSrc.SetLightType(Light::eDirectional);
		m_ambientLightSrc.SetAmbient(GameColor(0.8f, 0.8f, 0.8f, 1.0f));
		m_ambientLightSrc.SetDiffuse(g_gcBlack);
		m_ambientLightSrc.SetSpecular(g_gcWhite);

		//Create our metatable...
		m_metaTable = g_appPtr->GetLuaStateManager()->GetGlobalState()->GetGlobals().CreateTable("SceneGraphManager");
		m_metaTable.SetObject("__index", m_metaTable);

		// Provide access to some SGM methods to scripters.
		m_metaTable.RegisterObjectDirect("AddDynamicLight", (SceneGraphManager *)0, &SceneGraphManager::ScriptAddDynamicLight);
		m_metaTable.RegisterObjectDirect("SetGlobalIllumination", (SceneGraphManager *)0, &SceneGraphManager::ScriptSetGlobalIllumination);
		
		LuaPlus::LuaObject sgmManObj = g_appPtr->GetLuaStateManager()->GetGlobalState()->BoxPointer(this);
		sgmManObj.SetMetaTable(m_metaTable);

		// And here we expose the metatable as a named entity.
		g_appPtr->GetLuaStateManager()->GetGlobalState()->GetGlobals().SetObject("SceneGraphManager", sgmManObj);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SceneGraphManager::SetGlobalIllumination(const GameColor &color)
	{
		m_ambientLightSrc.SetAmbient(color);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneGraphManager::AddDynamicLight(boost::shared_ptr<Light> lightPtr)
	{
		if(lightPtr)
		{
			m_dynamicLights.push_back(lightPtr);
			return (true);
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SceneGraphManager::ResetLights()
	{
		m_dynamicLights.clear();
		m_ambientLightSrc.Reset();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneGraphManager::PrepareAdsShader(const Material &objectMaterial, const boost::optional<TexHandle> textureId)
	{
		if(!m_globalShaderPtr)
		{
			// No global shader.
			return (false);
		}

		// Activate the shader.
		m_globalShaderPtr->Activate();

		// Set up texture uniforms.
		GLint applyTextureVal = GL_FALSE;
		if(textureId.is_initialized())
		{
			applyTextureVal = GL_TRUE;
			g_appPtr->GetTextureManagerPtr()->Bind(*textureId, GL_TEXTURE_2D);
		}

		glUniform1i(m_adsUniformCache.m_applyTexLoc, applyTextureVal);
		glUniform1i(m_adsUniformCache.m_texture2dMapLoc, 0);

		Matrix4 mvpMat, mvMat;
		Matrix3x3 normalMat3;
		m_stackManagerPtr->GetModelViewProjectionMatrix(mvpMat);
		m_stackManagerPtr->GetModelViewMatrixStack()->GetMatrix(mvMat);
		m_stackManagerPtr->GetNormalMatrix(normalMat3, true);
		glUniformMatrix4fv(m_adsUniformCache.m_mvpLoc, 1, GL_FALSE, mvpMat.GetComponentsConst());
		glUniformMatrix4fv(m_adsUniformCache.m_mvLoc, 1, GL_FALSE, mvMat.GetComponentsConst());
		glUniformMatrix3fv(m_adsUniformCache.m_normalLoc, 1, GL_FALSE, normalMat3);

		// Set up Light uniforms.
		if(!m_dynamicLights.empty())
		{
			const GLuint numberLights(m_dynamicLights.size());

			GLint lightTypesArr[MAX_LIGHTS][1];
			GLfloat lightPositionArr[MAX_LIGHTS][3];
			GLfloat lightAmbientArr[MAX_LIGHTS][4];
			GLfloat lightDiffuseArr[MAX_LIGHTS][4];
			GLfloat lightSpecularArr[MAX_LIGHTS][4];
			GLfloat lightCutoffArr[MAX_LIGHTS][1];
			GLfloat lightExpArr[MAX_LIGHTS][1];
			GLfloat lightDirectionArr[MAX_LIGHTS][3];
			GLfloat lightCAttArr[MAX_LIGHTS][1];
			GLfloat lightLAttArr[MAX_LIGHTS][1];
			GLfloat lightQAttArr[MAX_LIGHTS][1];
			I32 index = 0;
			Matrix4 viewMat(m_camera->VGet()->GetToWorld());
			for(LightVector::iterator i = m_dynamicLights.begin(), end = m_dynamicLights.end(); ((index < MAX_LIGHTS) && (i != end)); ++i, ++index)
			{
				boost::shared_ptr<Light> currLight = *i;
				if(currLight->IsOn())
				{
					lightTypesArr[index][0] = currLight->GetLightType();
					lightCutoffArr[index][0] = currLight->GetSpotlightCutoff();
					lightExpArr[index][0] = currLight->GetSpotlightExponent();
					lightCAttArr[index][0] = currLight->GetConstantAttenuation();
					lightLAttArr[index][0] = currLight->GetLinearAttenuation();
					lightQAttArr[index][0] = currLight->GetQuadraticAttenuation();

					// Translate light position and direction vectors into View/Camera space.
					Vector4 lightWorldPos(currLight->GetPosition());
					Vector4 lightWorldDir(currLight->GetDirection());
					Vector4 lightViewPos(viewMat * lightWorldPos);
					Vector4 lightViewDir(viewMat * lightWorldDir);
					lightViewDir.Normalize();
					Vector3 lightViewPos3(lightViewPos);
					Vector3 lightViewDir3(lightViewDir);

					memcpy(lightPositionArr[index], lightViewPos3.GetComponentsConst(), m_adsUniformCache.FLOAT_SIZE * 3);
					memcpy(lightDirectionArr[index], lightViewDir3.GetComponentsConst(), m_adsUniformCache.FLOAT_SIZE * 3);
					memcpy(lightAmbientArr[index], currLight->GetAmbient().GetComponentsConst(), m_adsUniformCache.FLOAT_ARR_SIZE);
					memcpy(lightDiffuseArr[index], currLight->GetDiffuse().GetComponentsConst(), m_adsUniformCache.FLOAT_ARR_SIZE);
					memcpy(lightSpecularArr[index], currLight->GetSpecular().GetComponentsConst(), m_adsUniformCache.FLOAT_ARR_SIZE);
				}
			}

			glUniform1i(m_adsUniformCache.m_numLightsLoc, numberLights);
			glUniform1iv(m_adsUniformCache.m_lightTypesLoc, numberLights, (const GLint *)lightTypesArr);
			glUniform3fv(m_adsUniformCache.m_lightPosLoc, numberLights, (const GLfloat *)lightPositionArr);
			glUniform4fv(m_adsUniformCache.m_lightAmbLoc, numberLights, (const GLfloat *)lightAmbientArr);
			glUniform4fv(m_adsUniformCache.m_lightDiffLoc, numberLights, (const GLfloat *)lightDiffuseArr);
			glUniform4fv(m_adsUniformCache.m_lightSpecLoc, numberLights, (const GLfloat *)lightSpecularArr);
			glUniform1fv(m_adsUniformCache.m_spotCutoffLoc, numberLights, (const GLfloat *)lightCutoffArr);
			glUniform1fv(m_adsUniformCache.m_spotExpLoc, numberLights, (const GLfloat *)lightExpArr);
			glUniform3fv(m_adsUniformCache.m_spotDirLoc, numberLights, (const GLfloat *)lightDirectionArr);
			glUniform1fv(m_adsUniformCache.m_constantAttLoc, numberLights, (const GLfloat *)lightCAttArr);
			glUniform1fv(m_adsUniformCache.m_linearAttLoc, numberLights, (const GLfloat *)lightLAttArr);
			glUniform1fv(m_adsUniformCache.m_quadAttLoc, numberLights, (const GLfloat *)lightQAttArr);
			glUniform4fv(m_adsUniformCache.m_globalAmbLoc, 1, m_ambientLightSrc.GetAmbient().GetComponentsConst());
		}

		// Set up Object/Scene Nodes material uniforms.
		glUniform4fv(m_adsUniformCache.m_matEmmLoc, 1, objectMaterial.GetEmissive().GetComponentsConst());
		glUniform4fv(m_adsUniformCache.m_matAmbLoc, 1, objectMaterial.GetAmbient().GetComponentsConst());
		glUniform4fv(m_adsUniformCache.m_lightDiffLoc, 1, objectMaterial.GetDiffuse().GetComponentsConst());
		glUniform4fv(m_adsUniformCache.m_matSpecLoc, 1, objectMaterial.GetSpecular().GetComponentsConst());
		glUniform1f(m_adsUniformCache.m_matExpLoc, objectMaterial.GetSpecularPower());

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneGraphManager::OnRender()
	{
		// The render passes usually go like this 
		// 1. Static objects & terrain
		// 2. Actors (dynamic objects that can move)
		// 3. The Sky
		// 4. Anything with Alpha

		if (m_root && m_camera)
		{
			// The scene root could be anything, but it
			// is usually a SceneNode with the identity
			// matrix

			// Moved to View Render call now...
			//// Apply the view transformation to the global matrix stack (i.e. point the camera in some direction in the scene).
			//m_camera->VSetViewTransform(this);

			if (m_root->VPreRender(this))
			{
				m_root->VRender(this);
				m_root->VRenderChildren(this);
				m_root->VPostRender(this);
			}
		}

		if(!m_alphaNodeList.empty())
		{
			RenderAlphaPass();
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneGraphManager::OnRestore()
	{
		if (!m_root)
		{
			return (true);
		}
	 
		return (m_root->VOnRestore(this));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneGraphManager::OnLostDevice()
	{
		if (!m_root)
		{
			return (true);
		}
	 
		return (m_root->VOnLostDevice(this));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneGraphManager::OnUpdate(const F32 elapsedTime)
	{
		bool result = true;

		if(m_camera)
		{
			result = m_camera->VOnUpdate(this, elapsedTime);
		}
		if(result && m_root)
		{
			result = m_root->VOnUpdate(this, elapsedTime);
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<ISceneNode> SceneGraphManager::FindActor(const ActorId id)
	{
		SceneActorMap::iterator i = m_actorMap.find(id);
		if (i == m_actorMap.end())
		{
			boost::shared_ptr<ISceneNode> null;
			return (null);
		}

		return ((*i).second);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneGraphManager::AddChild(boost::shared_ptr<ISceneNode> nodePtr, boost::optional<ActorId> id)
	{
		if(nodePtr)
		{
			bool added = m_root->VAddChild(nodePtr);
			if(added)
			{
				if (id.is_initialized())
				{
					m_actorMap[*id] = nodePtr;	
				}
			}
			return (added);
		}
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	const boost::shared_ptr<GLSLShader> SceneGraphManager::GetShader(const std::string &shaderKeyRef)
	{
		boost::shared_ptr<GLSLShader> null;			// Empty shader pointer.

		if(shaderKeyRef.empty())
		{
            GF_LOG_TRACE_ERR("SceneGraphManager::GetShader()", "The shader name is empty");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("SceneGraphManager::GetShader()"), std::string("The shader name is empty"));
			return (null);
		}

		std::map<std::string, boost::shared_ptr<GLSLShader> >::iterator i = m_shaderMap.find(shaderKeyRef);
		if (i == m_shaderMap.end())
		{
			return (null);
		}

		return ((*i).second);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneGraphManager::AddShader(boost::shared_ptr<GLSLShader> shaderPtr, const std::string &shaderNameRef)
	{
		// Check input parameters and prevent duplicates or overwrites...
		if(!shaderPtr)
		{
            GF_LOG_TRACE_ERR("SceneGraphManager::AddShader()", "The shader object pointer is NULL");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("SceneGraphManager::AddShader()"), std::string("The shader object pointer is NULL"));
			return (false);
		}
		if(shaderNameRef.empty())
		{
            GF_LOG_TRACE_ERR("SceneGraphManager::AddShader()", "The shader name is empty");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("SceneGraphManager::AddShader()"), std::string("The shader name is empty"));
			return (false);
		}
		if(ContainsShader(shaderNameRef))
		{
            GF_LOG_TRACE_DEB("SceneGraphManager::AddShader()", "The shader name is already loaded");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::DEB, std::string("SceneGraphManager::AddShader()"), std::string("The shader name is already loaded"));
			return (true);
		}

		// Set the global shader for rendering SG nodes to use the ProgrammablePhongAds shader.
        std::string tmpStr(std::string("shaders") + ZipFile::ZIP_PATH_SEPERATOR + std::string("ProgrammablePhongAds"));
		if(shaderNameRef.compare(tmpStr.c_str()) == 0)
		{
			m_globalShaderPtr = shaderPtr;
			SetupGlobalShaderUniformCache();
		}

		// Safe to append the shader now.
		m_shaderMap[shaderNameRef] = shaderPtr;
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool AddShadersToSceneGraphManager(SceneGraphManager &sgm, const std::vector<std::string> &shaderNameVec, const std::vector<VSAttributeNameList> &vsAttNameListVec)
	{
		// Check input parameters.
		if(shaderNameVec.empty())
		{
            GF_LOG_TRACE_ERR("AddShadersToSceneGraphManager()", "The shader name vector is empty");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AddShadersToSceneGraphManager()"), std::string("The shader name vector is empty"));
			return (false);
		}
		if(vsAttNameListVec.empty())
		{
            GF_LOG_TRACE_ERR("AddShadersToSceneGraphManager()", "The VS attribute name list vector is empty");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AddShadersToSceneGraphManager()"), std::string("The VS attribute name list vector is empty"));
			return (false);
		}
		if(vsAttNameListVec.size() != shaderNameVec.size())
		{
            GF_LOG_TRACE_ERR("AddShadersToSceneGraphManager()", "The size of the shader vector and attribute name list vector do not match");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AddShadersToSceneGraphManager()"), std::string("The size of the shader vector and attribute name list vector do not match"));
			return (false);
		}

		// Deal with every shader name/ID in the vector.
		bool error = false;
		std::vector<VSAttributeNameList>::const_iterator currentAttIter = vsAttNameListVec.begin();
		for(std::vector<std::string>::const_iterator i = shaderNameVec.begin(), end = shaderNameVec.end(); i != end; ++i, ++currentAttIter)
		{
			boost::shared_ptr<GLSLShader> shaderObj(BuildShaderFromResourceCache(*i, *currentAttIter));
			if(shaderObj && !sgm.AddShader(shaderObj, *i))
			{
                GF_LOG_TRACE_ERR("AddShadersToSceneGraphManager()", std::string("Failed to add the ") + *i + std::string(" shader to the SceneGraphManager"));
//				SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AddShadersToSceneGraphManager()"), std::string("Failed to add the ") + *i + std::string(" shader to the SceneGraphManager"));
				error = true;						
			}
		}

		return (!error);
	}

}
