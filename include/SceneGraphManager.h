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
// @file SceneGraphManager.h
// @author Mike McShaffry & PJ O Halloran
// @date 04/10/2010
//
// The SceneGraph manager class.
//
// Also contains the AlphaSceneNode scene node class for specially
// blended scene nodes.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GF_SCENE_GRAPH_MANAGER_H
#define __GF_SCENE_GRAPH_MANAGER_H

// External Headers
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include <list>
#include <map>
#include <string>

#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

// Project Headers
#include "Matrix.h"
#include "IActors.h"
#include "ISceneNode.h"
#include "SceneNode.h"
#include "CameraSceneNode.h"
#include "ModelViewProjStackManager.h"
#include "GLSLShader.h"
#include "GameColors.h"
#include "LuaStateManager.h"
#include "TextureManager.h"

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class SceneGraphManager
	// @author PJ O Halloran
	//
	// A heirarchical container of scene nodes, which are classes that
	// implemente the ISceneNode interface.
	//
	// This manager holds a general GLSL program for rendering SG nodes.
	// It is an ADS shader using phong shading with a dynamic number
	// of lights.  
	//
	// With regards to dynamic lights, this depends on the GPU available:
	// The SGM interface allows you to add lights via C++
	// or LUA which are then passed to the shader as uniforms.  But 
	// keep in mind that the maximum number of uniforms and therefore
	// lights is dependant on the graphics hardware.  You may add however 
	// many lights you like but the shader program will only process the first
	// N lights where N is the maximum number of vertex uniforms it can
	// handle - the number of other uniforms to be passed.  The shaders
	// default maximum is 8 lights.
	//
	// /////////////////////////////////////////////////////////////////
	class SceneGraphManager
	{
	private:

		static const I32 MAX_LIGHTS = 8;

		// /////////////////////////////////////////////////////////////////
		// @struct AdsUniformLocCache
		// @author PJ O Halloran
		//
		// A cache of the uniform locations for the often used ADS phong
		// shader so we don't have to perform expensive glGet operations
		// many times per frame for every element in the scene.
		//
		// Used only in PrepareAdsShader().
		//
		// /////////////////////////////////////////////////////////////////
		struct AdsUniformLocCache
		{
			// Useful constants for PrepareAdsShader().
			static const size_t FLOAT_SIZE = sizeof(GLfloat);
			static const size_t FLOAT_ARR_SIZE = sizeof(GLfloat)*4;
			static const size_t INT_SIZE = sizeof(GLint);

			// Texture uniforms.
			GLint m_applyTexLoc;			///< Location for the uniform "u_applyTexture".
			GLint m_texture2dMapLoc;		///< Location for the uniform "u_texture2dMap".

			// Transformation uniforms.
			GLint m_mvpLoc;					///< Location for the uniform "u_mvpMatrix".
			GLint m_mvLoc;					///< Location for the uniform "u_mvMatrix".
			GLint m_normalLoc;				///< Location for the uniform "u_normalMatrix".

			// Lighting uniforms.
			GLint m_numLightsLoc;			///< Location for the uniform "u_numberLights".
			GLint m_lightTypesLoc;			///< Location for the uniform "u_lightTypesArr".
			GLint m_lightPosLoc;			///< Location for the uniform "u_lightPositionArr".
			GLint m_lightAmbLoc;			///< Location for the uniform "u_lightAmbientArr".
			GLint m_lightDiffLoc;			///< Location for the uniform "u_lightDiffuseArr".
			GLint m_lightSpecLoc;			///< Location for the uniform "u_lightSpecularArr".
			GLint m_spotCutoffLoc;			///< Location for the uniform "u_spotlightCutoffArr".
			GLint m_spotExpLoc;				///< Location for the uniform "u_spotlightExpArr".
			GLint m_spotDirLoc;				///< Location for the uniform "u_spotlightDirection".
			GLint m_constantAttLoc;			///< Location for the uniform "u_cAttArr".
			GLint m_linearAttLoc;			///< Location for the uniform "u_lAttArr".
			GLint m_quadAttLoc;				///< Location for the uniform "u_qAttArr".
			GLint m_globalAmbLoc;			///< Location for the uniform "u_globalAmbient".

			// Scene object material uniforms.
			GLint m_matEmmLoc;				///< Location for the uniform "u_materialE".
			GLint m_matAmbLoc;				///< Location for the uniform "u_materialA".
			GLint m_matDiffLoc;				///< Location for the uniform "u_materialD".
			GLint m_matSpecLoc;				///< Location for the uniform "u_materialS".
			GLint m_matExpLoc;				///< Location for the uniform "u_materialExp".

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// /////////////////////////////////////////////////////////////////
			AdsUniformLocCache()
			{
				Reset();
			};

			// /////////////////////////////////////////////////////////////////
			// Reset the cache.
			//
			// /////////////////////////////////////////////////////////////////
			void Reset()
			{
				m_applyTexLoc = -1;
				m_texture2dMapLoc = -1;
				m_mvpLoc = -1;
				m_mvLoc = -1;
				m_normalLoc = -1;
				m_numLightsLoc = -1;
				m_lightTypesLoc = -1;
				m_lightPosLoc = -1;
				m_lightAmbLoc = -1;
				m_lightDiffLoc = -1;
				m_lightSpecLoc = -1;
				m_spotCutoffLoc = -1;
				m_spotExpLoc = -1;
				m_spotDirLoc = -1;
				m_constantAttLoc = -1;
				m_linearAttLoc = -1;
				m_quadAttLoc = -1;
				m_globalAmbLoc = -1;
				m_matEmmLoc = -1;
				m_matAmbLoc = -1;
				m_matDiffLoc = -1;
				m_matSpecLoc = -1;
				m_matExpLoc = -1;
			};
		};

		AdsUniformLocCache m_adsUniformCache;									///< Cache of uniforms for the global ADS shader.
		boost::shared_ptr<SceneNode> m_root;									///< The root node of the SG.
		boost::shared_ptr<CameraSceneNode> m_camera;							///< The node acting as the camera.
		boost::shared_ptr<ModelViewProjStackManager> m_stackManagerPtr;			///< Pointer to the modelview/proj stack manager.
		AlphaSceneNodeList m_alphaNodeList;										///< List of nodes to render during the alpha rendering pass.
		SceneActorMap m_actorMap;												///< STL map that makes finding nodes associated with a game actor easier.
		std::map<std::string, boost::shared_ptr<GLSLShader> > m_shaderMap;		///< Container of GLSLShader programs used to render nodes in the scene graph.
		Light m_ambientLightSrc;												///< The global ambient light in the scene.
		LightVector m_dynamicLights;											///< List of dynamic lights attached to the scene.
		boost::shared_ptr<GLSLShader> m_globalShaderPtr;						///< The SGMs' main GLSL shader program (ADS model with phong or goraud shading) (nodes may still use their own shaders if they wish to).
		LuaPlus::LuaObject m_metaTable;											///< LuaPlus metatable for opening up access to external scripts to some of the SGM functionality.

		// /////////////////////////////////////////////////////////////////
		// Find all the uniforms for the global ADS phong shader and cache
		// them for later use.
		//
		// /////////////////////////////////////////////////////////////////
		void SetupGlobalShaderUniformCache();

		// /////////////////////////////////////////////////////////////////
		// Render all the blended scene nodes in reverse Z sorted order.
		//
		// /////////////////////////////////////////////////////////////////
		void RenderAlphaPass();

		// /////////////////////////////////////////////////////////////////
		// ***************** Begin Script Callable SGM API *****************
		// /////////////////////////////////////////////////////////////////

		// /////////////////////////////////////////////////////////////////
		// Add a dynamic light to the scene.
		//
		// @param lightData Table describing the light.
		//
		// @return bool True on success and false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool ScriptAddDynamicLight(LuaPlus::LuaObject lightData);

		// /////////////////////////////////////////////////////////////////
		// Set the scenes global illumination.
		//
		// @param globalIllData Table describing the global light.
		//
		// @return bool True on success and false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool ScriptSetGlobalIllumination(LuaPlus::LuaObject globalIllData);

		// /////////////////////////////////////////////////////////////////
		// ****************** End Script Callable SGM API ******************
		// /////////////////////////////////////////////////////////////////

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param stackManagerPtr The mv and proj mnatrix stack manager.
		//
		// /////////////////////////////////////////////////////////////////
		explicit SceneGraphManager(boost::shared_ptr<ModelViewProjStackManager> stackManagerPtr);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		~SceneGraphManager() { };

		// /////////////////////////////////////////////////////////////////
		// Set the global illumination in the scene. This is a fast
		// approximation for light bouncing around the scene (e.g. sunlight).
		// The ambient light color value applied to each pixel in the scene.
		//
		// Turn off global light by setting the color as black:
		//	(0.0, 0.0, 0.0, 1.0)
		//
		// @param color The new ambient light color in the scene.
		//
		// /////////////////////////////////////////////////////////////////
		void SetGlobalIllumination(const GameColor &color);

		// /////////////////////////////////////////////////////////////////
		// Add a dynamic light to the scene.  Note: The more lights you add 
		// the slower the scene will render.
		//
		// Also see notes in class definition comments about hardware number
		// of lights limits.
		//
		// @param lightPtr Pointer to the dynamic light to add.
		//
		// @return bool True if the light was added and false if not.
		//
		// /////////////////////////////////////////////////////////////////
		bool AddDynamicLight(boost::shared_ptr<Light> lightPtr);

		// /////////////////////////////////////////////////////////////////
		// Get a Light.
		//
		// @param index The index of the light to retrieve.
		//
		// @return boost::shared_ptr<Light> NULL if there is no light at the
		//									index or the Light onject.
		//
		// /////////////////////////////////////////////////////////////////
		boost::shared_ptr<Light> GetLight(const U32 index)
		{
			try
			{
				return (m_dynamicLights.at(index));
			}
			catch(...)
			{
				return (boost::shared_ptr<Light>());
			}
		};

		// /////////////////////////////////////////////////////////////////
		// Removes all lights from the SGM.
		//
		// /////////////////////////////////////////////////////////////////
		void ResetLights();

		// /////////////////////////////////////////////////////////////////
		// Get the ADS default GLSL shader.
		//
		// /////////////////////////////////////////////////////////////////
		boost::shared_ptr<GLSLShader> GetAdsShader() { return (m_globalShaderPtr); };

		// /////////////////////////////////////////////////////////////////
		// Set all the required program uniforms for the ADS shader.
		//
		// @param objectMaterial The scene nodes material.
		// @param textureId The scene nodes texture Id [optional].
		//
		// @return bool True if the shader has been prepared and false
		//				otherwise.
		//
		// /////////////////////////////////////////////////////////////////
		bool PrepareAdsShader(const Material &objectMaterial, const boost::optional<TexHandle> textureId = boost::optional<TexHandle>());

		// /////////////////////////////////////////////////////////////////
		// Call to render all the nodes in the SG.
		//
		// /////////////////////////////////////////////////////////////////
		bool OnRender();

		// /////////////////////////////////////////////////////////////////
		// Called when the application is restored.
		//
		// /////////////////////////////////////////////////////////////////
		bool OnRestore();

		// /////////////////////////////////////////////////////////////////
		// Called when the application loses the screen.
		//
		// /////////////////////////////////////////////////////////////////
		bool OnLostDevice();

		// /////////////////////////////////////////////////////////////////
		// Updates the SG.
		//
		// @param elapsedTime The number of seconds since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		bool OnUpdate(const F32 elapsedTime);

		// /////////////////////////////////////////////////////////////////
		// Find a node associated with an actor in the SG.
		//
		// /////////////////////////////////////////////////////////////////
		boost::shared_ptr<ISceneNode> FindActor(const ActorId id);

		// /////////////////////////////////////////////////////////////////
		// Add a node to the SG.  Optionally you may associate a game actor
		// with the node.
		//
		// @param id The optional Actor ID.
		// @param nodePtr Pointer to the node to add.
		//
		// /////////////////////////////////////////////////////////////////
		bool AddChild(boost::shared_ptr<ISceneNode> nodePtr, boost::optional<ActorId> id = boost::optional<ActorId>());

		// /////////////////////////////////////////////////////////////////
		// Remove a node from the SG associated with the ID supplied.
		//
		// @param id The ID of the game actor.
		//
		// /////////////////////////////////////////////////////////////////
		inline bool RemoveChild(const ActorId id)
		{
			m_actorMap.erase(id);
			return (m_root->VRemoveChild(id));
		};

		// /////////////////////////////////////////////////////////////////
		// Set the special Camera scene node.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetCamera(boost::shared_ptr<CameraSceneNode> camera) { if(camera) { m_camera = camera; } };

		// /////////////////////////////////////////////////////////////////
		// Get the special Camera node.
		//
		// /////////////////////////////////////////////////////////////////
		inline const boost::shared_ptr<CameraSceneNode> GetCamera() const { return (m_camera); };

		// /////////////////////////////////////////////////////////////////
		// Get the ModelView/Projection matrix stack manager.
		//
		// /////////////////////////////////////////////////////////////////
		inline boost::shared_ptr<ModelViewProjStackManager> GetStackManager() { return (m_stackManagerPtr); };

		// /////////////////////////////////////////////////////////////////
		// Add an alpha scene node to the special list to render after all
		// non blended nodes.
		//
		// Will not be added if the shared pointer supplied does not hold a
		// valid pointer.
		//
		// @param asn Pointer to the node to render specially.
		//
		// /////////////////////////////////////////////////////////////////
		inline void AddAlphaSceneNode(boost::shared_ptr<AlphaSceneNode> asn) { if(asn) { m_alphaNodeList.push_back(asn); } };

		// /////////////////////////////////////////////////////////////////
		// Check if the ray intersects with any of the nodes being managed
		// by the SGM.
		//
		// @param ray The raycast.
		//
		// @return bool True if the ray intersected with any of the SGMs nodes
		//					and false otherwise.
		//
		// /////////////////////////////////////////////////////////////////
		inline bool Pick(const RayCast &ray) { return (m_root->VPick(this, ray)); };

		// /////////////////////////////////////////////////////////////////
		// Gets a shader program.
		//
		// @param shaderKeyRef The name of the shader.
		//
		// @return const boost::shared_ptr<GLSLShader> A read only pointer to
		//			the shader program to be used for rendering or NULL if no
		//			such shader program is known by the SceneGraphManager.
		//
		// /////////////////////////////////////////////////////////////////
		const boost::shared_ptr<GLSLShader> GetShader(const std::string &shaderKeyRef);

		// /////////////////////////////////////////////////////////////////
		// Checks if a shader program is loaded.
		//
		// @param shaderKeyRef The name of the shader.
		//
		// @return bool True if the shader exists and false otherwise.
		//
		// /////////////////////////////////////////////////////////////////
		inline bool ContainsShader(const std::string &shaderKeyRef)
		{
			return (GetShader(shaderKeyRef).get() != NULL);
		};

		// /////////////////////////////////////////////////////////////////
		// Adds a pre built shader to the SceneGraphManager.
		//
		// @param shaderPtr Pointer to the shader program.
		// @param shaderNameRef The name of the shader, hereafter used to
		//			reference the shader.
		//
		// @return bool True on success.
		// @return bool False if the shaderPtr is NULL or the shader name
		//				string is empty.
		// @return bool False if a shader already exists in the list with the
		//				same name as that of the shader to add.
		//
		// /////////////////////////////////////////////////////////////////
		bool AddShader(boost::shared_ptr<GLSLShader> shaderPtr, const std::string &shaderNameRef);

	};

	// /////////////////////////////////////////////////////////////////
	// Utility function that loads a group of shaders from the resource
	// cache and adds them to the SceneGraphManager object also
	// supplied.
	//
	// @param sgm The SceneGraphManager that should manage the loaded
	//				shaders.
	// @param shaderNameVec A vector of strings containing the names of
	//						the shaders in the main resource cache file.
	// @param vsAttNameListVec A vector of attribute name lists for each
	//							of the shaders we are to build.
	//
	// @return bool True on success and false on failure. Please see the
	//				main application log file for details if the function
	//				fails.
	//
	// /////////////////////////////////////////////////////////////////
	bool AddShadersToSceneGraphManager(SceneGraphManager &sgm, const std::vector<std::string> &shaderNameVec, const std::vector<VSAttributeNameList> &vsAttNameListVec);

}

#endif
