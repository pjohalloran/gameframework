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
// @file RootSceneNode.cpp
// @author Mike McShaffry & PJ O Halloran
// @date 04/10/2010
//
// Implementation for the SG root node.
//
// /////////////////////////////////////////////////////////////////

#include <boost/optional.hpp>
#include <string>

#include "RootSceneNode.h"
#include "ZipFile.h"
#include "GameMain.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	RootSceneNode::RootSceneNode(SceneGraphManager *sgPtr)
								: SceneNode(sgPtr, boost::optional<ActorId>(), std::string(), RenderPassFirst, Material(), GameHalloran::g_identityMat)
	{
		boost::shared_ptr<SceneNode> staticGroup(GCC_NEW SceneNode(sgPtr, boost::optional<ActorId>(), std::string("StaticGroup"), RenderPassStatic, Material(), GameHalloran::g_identityMat));
		m_children.push_back(staticGroup);	// RenderPass_Static = 0

		boost::shared_ptr<SceneNode> actorGroup(GCC_NEW SceneNode(sgPtr, boost::optional<ActorId>(), std::string("ActorGroup"), RenderPassActor, Material(), GameHalloran::g_identityMat));
		m_children.push_back(actorGroup);	// RenderPass_Actor = 1

		boost::shared_ptr<SceneNode> skyGroup(GCC_NEW SceneNode(sgPtr, boost::optional<ActorId>(), std::string("SkyGroup"), RenderPassSky, Material(), GameHalloran::g_identityMat));
		m_children.push_back(skyGroup);	// RenderPass_Sky = 2

		SetShaderName(std::string("shaders") + ZipFile::ZIP_PATH_SEPERATOR + std::string("flat"));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool RootSceneNode::VAddChild(boost::shared_ptr<ISceneNode> childNodePtr)
	{
		if(!childNodePtr)
			return (false);

		// The Root node has children that divide the scene graph into render passes.
		// Scene nodes will get added to these children based on the value of the
		// render pass member variable.
		if (!m_children[childNodePtr->VGet()->GetRenderPass()])
		{
            GF_LOG_TRACE_ERR("RootSceneNode::VAddChild()", "No such render pass");
			return (false);
		}

		return (m_children[childNodePtr->VGet()->GetRenderPass()]->VAddChild(childNodePtr));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool RootSceneNode::VRemoveChild(ActorId id)
	{
		bool anythingRemoved = false;
		for(I32 i = RenderPassFirst, end = RenderPassLast; ((!anythingRemoved) && (i < end)); ++i)
		{
			if(m_children[i]->VRemoveChild(id))
			{
				anythingRemoved = true;
			}
		}
		return (anythingRemoved);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool RootSceneNode::VRenderChildren()
	{
		for (I32 pass = RenderPassFirst, end = RenderPassLast; pass < end; ++pass)
		{
			switch(pass)
			{
				case RenderPassStatic:
				case RenderPassActor:
				case RenderPassSky:
					m_children[pass]->VRenderChildren();
					break;
			}
		}

		return (true);
	}

}
