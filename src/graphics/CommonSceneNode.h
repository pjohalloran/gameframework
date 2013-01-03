#pragma once
#ifndef __GF_COMMON_SCENE_NODE_H
#define __GF_COMMON_SCENE_NODE_H

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
// @file CommonSceneNode.h
// @author Mike McShaffry & PJ O Halloran
// @date 05/10/2010
//
// Common SceneGraph enums and classes.
//
// /////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include <list>

#include "Matrix.h"
#include "ISceneNode.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @enum AlphaType
	//
	// List of blending types that can be set on a node of the SceneGraph.
	//	
	// /////////////////////////////////////////////////////////////////
	enum AlphaType
	{
		AlphaOpaque,
		AlphaTexture,
		AlphaMaterial,
		AlphaVertex
	};

	/////////////////////////////////////////////////////////////////////////////
	// @enum RenderPass
	//
	// 3D scenes are drawn in passes - this enum defines the render passes
	// supported by the 3D scene graph created by class SceneGraphManager (defined
	// elsewhere).
	//
	/////////////////////////////////////////////////////////////////////////////
	enum RenderPass
	{
		RenderPassFirst,
		RenderPassStatic = RenderPassFirst,
		RenderPassActor,
		RenderPassSky,
		RenderPassLast
	};

	// /////////////////////////////////////////////////////////////////
	// @class AlphaSceneNode
	// @author Mike McShaffry & PJ O Halloran.
	//
	// A list of scene nodes that need to be drawn in the alpha pass.
	//
	// /////////////////////////////////////////////////////////////////
	class AlphaSceneNode
	{
	private:

		boost::shared_ptr<ISceneNode> m_node;			///< Pointer to the node.
		Matrix4 m_concatMat;							///< The transformation matrix state to render the node properly.
		F32 m_z;										///< The depth of the node.

	public:
		
		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param node Pointer to the SceneNode.
		// @param mat 
		// @param z The depth of the node.
		//
		// /////////////////////////////////////////////////////////////////
		AlphaSceneNode(boost::shared_ptr<ISceneNode> node, const Matrix4 &mat, const F32 z)\
			: m_node(node), m_concatMat(mat), m_z(z) { };

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		~AlphaSceneNode() { };

		// /////////////////////////////////////////////////////////////////
		// Used to sort alpha nodes by depth.
		//
		// /////////////////////////////////////////////////////////////////
		const bool operator <(const AlphaSceneNode &other) const { return (m_z < other.m_z); };

		// /////////////////////////////////////////////////////////////////
		// Get the matrix state for rendering the node properly.
		//
		// /////////////////////////////////////////////////////////////////
		const Matrix4 &GetMatrix() { return (m_concatMat); };

		// /////////////////////////////////////////////////////////////////
		// Get the node to render.
		//
		// /////////////////////////////////////////////////////////////////
		boost::shared_ptr<ISceneNode> GetNode() { return (m_node); };
	};

	typedef std::list<boost::shared_ptr<AlphaSceneNode> > AlphaSceneNodeList;
}

#endif
