#pragma once
#ifndef __GF_I_SCENE_NODE_H
#define __GF_I_SCENE_NODE_H

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
// @file ISceneNode.h
// @author Mike McShaffry & PJ O Halloran
// @date 04/10/2010
//
// Base SceneGraph interface.
//
// /////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include <vector>
#include <map>

#include "IActors.h"

namespace GameHalloran
{

	class SceneNodeProperties;
	class SceneGraphManager;
	class Matrix4;
	class RayCast;

	// /////////////////////////////////////////////////////////////////
	// @class ISceneNode
	// @author Mike McShaffry & PJ O Halloran.
	//
	// Base scene node interface.
	//
	// /////////////////////////////////////////////////////////////////
	class ISceneNode
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~ISceneNode() { };

		// /////////////////////////////////////////////////////////////////
		// Get a read only pointer to the attributes of the scene node.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const SceneNodeProperties * const VGet() const = 0;

		// /////////////////////////////////////////////////////////////////
		// Set the nodes transformation matrices.
		//
		// @param toWorld The nodes orientation and position relative to
		//						its parent node (or to the world if there is
		//						no parent).
		// 
		// /////////////////////////////////////////////////////////////////
		virtual void VSetTransform(const Matrix4 &toWorld) = 0;

		// /////////////////////////////////////////////////////////////////
		// Set the nodes transformation matrices.
		//
		// @param toWorld The nodes orientation and position relative to
		//						its parent node (or to the world if there is
		//						no parent).
		// @param fromWorld Inverse of toWorld.
		// 
		// /////////////////////////////////////////////////////////////////
		virtual void VSetTransform(const Matrix4 &toWorld, const Matrix4 &fromWorld) = 0;

		// /////////////////////////////////////////////////////////////////
		// Called before node is rendered.
		//
		// @param scenePtr SceneGraph manager pointer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VPreRender() = 0;

		// /////////////////////////////////////////////////////////////////
		// Renders node.
		//
		// @param scenePtr SceneGraph manager pointer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VRender() = 0;

		// /////////////////////////////////////////////////////////////////
		// Called after node is rendered.
		//
		// @param scenePtr SceneGraph manager pointer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VPostRender() = 0;

		// /////////////////////////////////////////////////////////////////
		// Adds a child to this node.
		//
		// @param childNodePtr Pointer to child node to add.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VAddChild(boost::shared_ptr<ISceneNode> childNodePtr) = 0;

		// /////////////////////////////////////////////////////////////////
		// Searches for and removes a child from this node.
		//
		// @param id ID of the game actor the node represents.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VRemoveChild(const ActorId id) = 0;

		// /////////////////////////////////////////////////////////////////
		// Render all child nodes.
		//
		// @param scenePtr SceneGraph manager pointer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VRenderChildren() = 0;

		// /////////////////////////////////////////////////////////////////
		// Called when application is restored.
		//
		// @param scenePtr SceneGraph manager pointer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnRestore() = 0;

		// /////////////////////////////////////////////////////////////////
		// Called when application loses focus.
		//
		// @param scenePtr SceneGraph manager pointer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnLostDevice() = 0;

		// /////////////////////////////////////////////////////////////////
		// Is the node currently visible.
		//
		// @param scenePtr SceneGraph manager pointer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VIsVisible() const = 0;

		// /////////////////////////////////////////////////////////////////
		// Check if the ray intersects with this SceneNode.
		//
		// @param scenePtr SceneGraph manager pointer.
		// @param ray Raycast.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VPick(const RayCast &ray) = 0;

		// /////////////////////////////////////////////////////////////////
		// Updates the node once per main loop.
		//
		// @param scenePtr SceneGraph manager pointer.
		// @param elapsedTime The number of seconds since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnUpdate(const F32 deltaMilliseconds) = 0;

		// /////////////////////////////////////////////////////////////////
		// Get the nodes parent.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const ISceneNode * const VGetParentPtr() const = 0;

		// /////////////////////////////////////////////////////////////////
		// Sets the nodes parent.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetParentPtr(ISceneNode *parentPtr) = 0;
        
        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSetSceneManager(SceneGraphManager *sgmPtr) = 0;
	};

	// /////////////////////////////////////////////////////////////////
	// Every scene node has a list of its children - this is implemented
	// as a vector since it is expected that we won't add/delete nodes 
	// very often, and we'll want fast random access to each child.
	//
	// /////////////////////////////////////////////////////////////////
	typedef std::vector<boost::shared_ptr<ISceneNode> > SceneNodeList;

	// /////////////////////////////////////////////////////////////////
	// An STL map that allows fast lookup of a scene node given an 
	// ActorId.
	//
	// /////////////////////////////////////////////////////////////////
	typedef std::map<ActorId, boost::shared_ptr<ISceneNode> > SceneActorMap;

}

#endif
