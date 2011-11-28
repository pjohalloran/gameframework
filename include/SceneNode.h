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
// @file SceneNode.h
// @author Mike McShaffry & PJ O Halloran
// @date 04/10/2010
//
// Contains the SceneNode classes definition.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GF_SCENE_NODE_H
#define __GF_SCENE_NODE_H

// External Headers
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include <string>


// Project Headers
#include "ISceneNode.h"
#include "Matrix.h"
#include "Vector.h"
#include "IActors.h"
#include "Actors.h"
#include "SceneNodeProperties.h"
#include "GLSLShader.h"


namespace GameHalloran
{

	// Forward delaration of the SceneGraphManager class.
	class SceneGraphManager;

	// /////////////////////////////////////////////////////////////////
	// @class SceneNode
	// @author Mike McShaffry & PJ O Halloran
	//
	// Implements ISceneNode. Forms the base class for any node that can
	// exist in the 3D scene graph managed by class SceneGraphManager.
	//
	// /////////////////////////////////////////////////////////////////
	class SceneNode : public ISceneNode
	{
	private:

		ISceneNode *m_parentPtr;						///< Pointer to the nodes parent.
		SceneNodeProperties m_props;					///< Nodes properties.
		bool m_useCustomShader;							///< Are we using a custom shader to render this node (if not the SGMs' default GLSL program will be used)?

	protected:

		SceneNodeList m_children;						///< The child nodes list.
		boost::shared_ptr<GLSLShader> m_shaderPtr;		///< Custom GLSL shader program we are using to render the node (if this is NULL then the global SGM shader will be used).

		// /////////////////////////////////////////////////////////////////
		// Are we using a custom shader to render this node?
		//
		// /////////////////////////////////////////////////////////////////
		inline bool IsUsingCustomShader() const { return (m_useCustomShader); };

		// /////////////////////////////////////////////////////////////////
		// Render a scene node.  The SceneNode rendered may either be this
		// node itself or one of its children Scene Nodes.
		//
		// @param scenePtr Pointer to the SGM.
		// @param snPtr Pointer to the SceneNode to render.
		//
		// /////////////////////////////////////////////////////////////////
		void RenderSceneNode(SceneGraphManager *scenePtr, ISceneNode *snPtr);

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param actorId The optional Actor ID.
		// @param name Name of the node.
		// @param renderPass What renderpass the node belongs to.
		// @param material The objects material.
		// @param toWorld Position and orientation of node relative to its
		//					parent.
		//
		// /////////////////////////////////////////////////////////////////
		explicit SceneNode(boost::optional<ActorId> actorId, const std::string &name, const RenderPass renderPass, const Material &material, const Matrix4 &toWorld);

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param actorId The optional Actor ID.
		// @param name Name of the node.
		// @param renderPass What renderpass the node belongs to.
		// @param material The objects material.
		// @param toWorld Position and orientation of node relative to its
		//					parent.
		// @param fromWorld Inverse of toWorld matrix.
		//
		// /////////////////////////////////////////////////////////////////
		explicit SceneNode(boost::optional<ActorId> actorId, const std::string &name, const RenderPass renderPass, const Material &material, const Matrix4 &toWorld, const Matrix4 &fromWorld);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~SceneNode();

		// /////////////////////////////////////////////////////////////////
		// Get the nodes properties.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const SceneNodeProperties * const VGet() const { return (&m_props); };

		// /////////////////////////////////////////////////////////////////
		// Set the render state before rendering.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VPreRender(SceneGraphManager *scenePtr);

		// /////////////////////////////////////////////////////////////////
		// Render the node.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VRender(SceneGraphManager *scenePtr);

		// /////////////////////////////////////////////////////////////////
		// Reset the render state after rendering.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VPostRender(SceneGraphManager *scenePtr);

		// /////////////////////////////////////////////////////////////////
		// Set the nodes transformation matrices.
		//
		// This method will calculate the fromWorld inverse matrix of toWorld
		// automatically.
		//
		// @param toWorld The nodes orientation and position relative to
		//						its parent node (or to the world if there is
		//						no parent).
		// 
		// /////////////////////////////////////////////////////////////////
		virtual void VSetTransform(const Matrix4 &toWorld);

		// /////////////////////////////////////////////////////////////////
		// Set the nodes transformation matrices.
		//
		// @param toWorld The nodes orientation and position relative to
		//						its parent node (or to the world if there is
		//						no parent).
		// @param fromWorld Inverse of toWorld.
		// 
		// /////////////////////////////////////////////////////////////////
		virtual void VSetTransform(const Matrix4 &toWorld, const Matrix4 &fromWorld);

		// /////////////////////////////////////////////////////////////////
		// Adds a child to this node.
		//
		// @param childNodePtr Pointer to child node to add.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VAddChild(boost::shared_ptr<ISceneNode> childNodePtr);

		// /////////////////////////////////////////////////////////////////
		// Searches for and removes a child from this node.
		//
		// @param id ID of the game actor the node represents.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VRemoveChild(const ActorId id);

		// /////////////////////////////////////////////////////////////////
		// Render all child nodes.
		//
		// @param scenePtr SceneGraph manager pointer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VRenderChildren(SceneGraphManager *scenePtr);

		// /////////////////////////////////////////////////////////////////
		// Called when application is restored.
		//
		// @param scenePtr SceneGraph manager pointer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnRestore(SceneGraphManager *scenePtr);

		// /////////////////////////////////////////////////////////////////
		// Called when application loses focus.
		//
		// @param scenePtr SceneGraph manager pointer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnLostDevice(SceneGraphManager *scenePtr);

		// /////////////////////////////////////////////////////////////////
		// Is the node currently visible.
		//
		// @param scenePtr SceneGraph manager pointer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VIsVisible(SceneGraphManager *scenePtr) const;

		// /////////////////////////////////////////////////////////////////
		// Check if the ray intersects with any of the children of this
		// node.
		//
		// @param scenePtr SceneGraph manager pointer.
		// @param rayCastPtr Pointer to the RayCast class.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VPick(SceneGraphManager *scenePtr, const RayCast &ray);

		// /////////////////////////////////////////////////////////////////
		// Updates the node once per main loop.
		//
		// @param scenePtr SceneGraph manager pointer.
		// @param elapsedTime The number of seconds since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnUpdate(SceneGraphManager *scenePtr, const F32 elapsedTime);

		// /////////////////////////////////////////////////////////////////
		// Set the alpha value of the node.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void SetAlpha(const F32 alpha) { m_props.SetAlpha(alpha); };

		// /////////////////////////////////////////////////////////////////
		// Get the position of the node.  The position will be in the world
		// coordinate system.
		//
		// /////////////////////////////////////////////////////////////////
		Point3 GetPosition() const;

		// /////////////////////////////////////////////////////////////////
		// Set the position of the node.  The position you set should be in
		// the world coordinate system.
		//
		// /////////////////////////////////////////////////////////////////
		void SetPosition(const Point3 &pos);

		// /////////////////////////////////////////////////////////////////
		// Most scene nodes will not have actor params, so they return 
		// a NULL pointer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IActorParams *VGetActorParams(void) { return (NULL); };

		// /////////////////////////////////////////////////////////////////
		// Set a nodes actor parameters.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetActorParams(IActorParams *actorParamsPtr) { };

		// /////////////////////////////////////////////////////////////////
		// Get the nodes parent.
		//
		// /////////////////////////////////////////////////////////////////
		inline virtual const ISceneNode * const VGetParentPtr() const { return (m_parentPtr); };

		// /////////////////////////////////////////////////////////////////
		// Sets the nodes parent.
		//
		// /////////////////////////////////////////////////////////////////
		inline virtual void VSetParentPtr(ISceneNode *parentPtr) { m_parentPtr = parentPtr; };

		// /////////////////////////////////////////////////////////////////
		// Check if the node has a parent node.
		//
		// /////////////////////////////////////////////////////////////////
		inline bool HasParent() const { return (m_parentPtr != NULL); };

		// /////////////////////////////////////////////////////////////////
		// Set the name of the shader to use to render the node.
		//
		// /////////////////////////////////////////////////////////////////
		void SetShaderName(const std::string &shaderNameRef);

		// /////////////////////////////////////////////////////////////////
		// Set the radius of the node.
		//
		// /////////////////////////////////////////////////////////////////
		void SetRadius(const F32 radius) { m_props.SetRadius(radius); };

		// /////////////////////////////////////////////////////////////////
		// Set the material of the node.
		//
		// /////////////////////////////////////////////////////////////////
		void SetMaterial(const Material &material) { m_props.SetMaterial(material); };

	};

}

#endif
