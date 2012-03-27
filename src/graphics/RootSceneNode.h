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
// @file RootSceneNode.h
// @author Mike McShaffry & PJ O Halloran
// @date 04/10/2010
//
// Header for the SG root node.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GF_ROOT_SCENE_NODE_H
#define __GF_ROOT_SCENE_NODE_H

// External Headers
#include <boost/shared_ptr.hpp>


// Project Headers
#include "SceneNode.h"
#include "Matrix.h"
#include "IActors.h"


namespace GameHalloran
{

	// Forward declaration of the SceneGraphManager object.
	class SceneGraphManager;

	// /////////////////////////////////////////////////////////////////
	// @class RootSceneNode
	// @author Mike McShaffry & PJ O Halloran.
	//
	// This is the root node of the scene graph.
	//
	// Its a special node whos purpose is to provide control over the
	// general rendering order in the 3D scene.  For example, static
	// geometry drawn first, then game actors, finally the skybox.
	//
	// /////////////////////////////////////////////////////////////////
	class RootSceneNode : public SceneNode
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
        //
        // @param SG manager pointer.
		//
		// /////////////////////////////////////////////////////////////////
		explicit RootSceneNode(SceneGraphManager *sgPtr);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~RootSceneNode() { };

		// /////////////////////////////////////////////////////////////////
		// Add a child to the root node.  The node is grouped according to
		// its render pass property.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VAddChild(boost::shared_ptr<ISceneNode> childNodePtr);

		// /////////////////////////////////////////////////////////////////
		// Removes a child from the scene associated with the actor ID.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VRemoveChild(const ActorId id);

		// /////////////////////////////////////////////////////////////////
		// Renders all child nodes in order of their render pass attributes.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VRenderChildren();

		// /////////////////////////////////////////////////////////////////
		// Root node is always visible as it doesnt represent any actual
		// 3D object in the scene.  It manages the renderpass order of the
		// nodes only.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VIsVisible() const { return (true); };
	};

}

#endif
