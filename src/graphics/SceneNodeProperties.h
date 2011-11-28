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
// @file SceneNodeProperties.h
// @author Mike McShaffry & PJ O Halloran
// @date 04/10/2010
//
// A simple get/set class that holds attributes for each node in the
// SG.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GF_SCENE_NODE_PROPERTIES_H
#define __GF_SCENE_NODE_PROPERTIES_H

// External Headers
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include <string>


// Project Headers
#include "Matrix.h"
#include "IActors.h"
#include "BoundingSphere.h"
#include "BoundingCube.h"
#include "GameColors.h"
#include "CommonSceneNode.h"


namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class SceneNodeProperties
	// @author Mike McShaffry & PJ O Halloran.
	//
	// This class used in the SceneNode class, and gains easy
	// read only access to scene node properties such as its ActorId or 
	// render pass, with a single ISceneNode::VGet() call.
	//
	// /////////////////////////////////////////////////////////////////
	class SceneNodeProperties
	{
	private:

		boost::optional<ActorId> m_actorId;						///< The ID of the associated game actor.
		std::string m_name;										///< The name of the node (for debugging).
		Matrix4 m_toWorld;										///< Matrix that holds the position and orientation of the node relative to its parent.
		Matrix4 m_fromWorld;									///< Inverse matrix of the above.
		F32 m_radius;											///< The radius of the sphere enclosing the node and all its children.
		RenderPass m_renderPass;								///< What render pass the node belongs to.
		AlphaType m_alphaType;									///< The alpha/blend type of the node.
		F32 m_alpha;											///< Alpha value of the node.
		std::string m_shaderName;								///< Name of the shader to use to render the node.
		Material m_material;									///< The material type of the node.

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		inline explicit SceneNodeProperties() : m_actorId(), m_name(), m_toWorld(), m_fromWorld(),\
			m_radius(0.0f), m_renderPass(RenderPassFirst), m_alphaType(AlphaOpaque), m_alpha(g_OPAQUE), m_shaderName(), m_material() { };

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		// 
		// @param id Optional Actor ID.
		// @param nameRef Name of node.
		// @param toWorld Position and orientation of node relative to parent.
		// @param fromWorld Inverse of toWorld.
		// @param radius The radius of the sphere enclosing the node and all
		//					its children.
		// @param rp RenderPass of node.
		// @param at AlphaType of node.
		// @param shaderName Name of the shader program to render the node.
		// @param material The nodes material.
		//
		// /////////////////////////////////////////////////////////////////
		inline explicit SceneNodeProperties(const boost::optional<ActorId> id, const std::string &nameRef, const Matrix4 &toWorld, const Matrix4 &fromWorld, const F32 radius, const RenderPass rp, const AlphaType at, const F32 alpha, const std::string shaderName, const Material material)\
			: m_actorId(id), m_name(nameRef), m_toWorld(toWorld), m_fromWorld(fromWorld), m_radius(radius), m_renderPass(rp), m_alphaType(at), m_alpha(alpha), m_shaderName(shaderName), m_material(material) { };

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		inline ~SceneNodeProperties() { };

		// /////////////////////////////////////////////////////////////////
		// Get the Actor ID.
		//
		// /////////////////////////////////////////////////////////////////
		inline boost::optional<ActorId> GetActorId() const { return (m_actorId); };

		// /////////////////////////////////////////////////////////////////
		// Set the Actor ID.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetActorId(boost::optional<ActorId> id) { m_actorId = id; };

		// /////////////////////////////////////////////////////////////////
		// Get the orientation and position of the node relative to its
		// parent.
		//
		// /////////////////////////////////////////////////////////////////
		inline Matrix4 GetToWorld() const { return (m_toWorld); };

		// /////////////////////////////////////////////////////////////////
		// Set the to world matrix.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetToWorld(const Matrix4 &toWorld) { m_toWorld = toWorld; };

		// /////////////////////////////////////////////////////////////////
		// Inverse of ToWorld().
		//
		// /////////////////////////////////////////////////////////////////
		inline Matrix4 GetFromWorld() const { return (m_fromWorld); };

		// /////////////////////////////////////////////////////////////////
		// Set the from world matrix.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetFromWorld(const Matrix4 &fromWorld) { m_fromWorld = fromWorld; };

		// /////////////////////////////////////////////////////////////////
		// Get the name of the node.
		//
		// /////////////////////////////////////////////////////////////////
		inline std::string GetName() const { return (m_name); };

		// /////////////////////////////////////////////////////////////////
		// Set the name of the node.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetName(const std::string &nameRef) { m_name = nameRef; };
		
		// /////////////////////////////////////////////////////////////////
		// Is the node going to be blended somehow?
		//
		// /////////////////////////////////////////////////////////////////
		inline bool HasAlpha() const { return (m_alpha != g_OPAQUE); };

		// /////////////////////////////////////////////////////////////////
		// Get the nodes alpha value.
		//
		// /////////////////////////////////////////////////////////////////
		inline F32 GetAlpha() const { return (m_alpha); };

		// /////////////////////////////////////////////////////////////////
		// Set the nodes alpha value.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetAlpha(const F32 a) { m_alpha = a; };

		// /////////////////////////////////////////////////////////////////
		// Get the alpha type of the node.
		//
		// /////////////////////////////////////////////////////////////////
		inline AlphaType GetAlphaType() const { return (m_alphaType); };

		// /////////////////////////////////////////////////////////////////
		// Set the alpha type of the node.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetAlphaType(const AlphaType alpha) { m_alphaType = alpha; };

		// /////////////////////////////////////////////////////////////////
		// Get the nodes render pass.
		//
		// /////////////////////////////////////////////////////////////////
		inline RenderPass GetRenderPass() const { return (m_renderPass); };

		// /////////////////////////////////////////////////////////////////
		// Set the nodes render pass.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetRenderPass(const RenderPass pass) { m_renderPass = pass; };

		// /////////////////////////////////////////////////////////////////
		// Get the radius of the sphere enclosing the node and all its
		// children.
		//
		// /////////////////////////////////////////////////////////////////
		inline F32 GetRadius() const { return (m_radius); };

		// /////////////////////////////////////////////////////////////////
		// Set the radius of the sphere enclosing the node and all its
		// children.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetRadius(const F32 radius) { m_radius = radius; };

		// /////////////////////////////////////////////////////////////////
		// Get the nodes bounding sphere.
		//
		// /////////////////////////////////////////////////////////////////
		inline BoundingSphere &GetBoundingSphere(BoundingSphere &bs) const
		{
			Point3 pos;						// Current position of the node in world space.
			m_toWorld.GetPosition(pos);

			bs.SetRadius(m_radius);
			bs.SetCentre(pos);
			return (bs);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the nodes bounding box.
		//
		// /////////////////////////////////////////////////////////////////
		BoundingCube &GetBoundingBox(BoundingCube &bb) const;

		// /////////////////////////////////////////////////////////////////
		// Get the name of the shader used to render the node.
		//
		// /////////////////////////////////////////////////////////////////
		inline std::string GetShaderName() const { return (m_shaderName); };

		// /////////////////////////////////////////////////////////////////
		// Set the name of the shader used to render the node.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetShaderName(const std::string &shaderName) { m_shaderName = shaderName; };

		// /////////////////////////////////////////////////////////////////
		// Get the nodes material.
		//
		// /////////////////////////////////////////////////////////////////
		inline Material GetMaterial() const { return (m_material); };

		// /////////////////////////////////////////////////////////////////
		// Set the nodes material.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetMaterial(const Material &material) { m_material = material; };
	};

}

#endif
