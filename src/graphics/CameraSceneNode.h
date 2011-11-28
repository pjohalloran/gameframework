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
// @file CameraSceneNode.h
// @author Mike McShaffry & PJ O Halloran
// @date 05/10/2010
//
// Special Camera SceneNode header file.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GF_CAMERA_SCENE_NODE_H
#define __GF_CAMERA_SCENE_NODE_H

// External Headers
#include <boost/shared_ptr.hpp>

#include <string>


// Project Headers
#include "Matrix.h"
#include "IActors.h"
#include "Frustrum.h"
#include "GLFrame.h"
#include "SceneNode.h"
#include "ZipFile.h"

// DEBUG
//#include "camera.h"

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	//// Forward declaraction.
	//class SceneGraphManager;

	// /////////////////////////////////////////////////////////////////
	// @class CameraNode
	// @author Mike McShaffry & PJ O Halloran.
	//
	// A camera node controls the view transform and holds the view
	// frustum definition.
	//
	// /////////////////////////////////////////////////////////////////
	class CameraSceneNode : public SceneNode
	{
	private:

		GLFrame m_frame;							///< The cameras FoR class.
		bool m_updateCameraMatrix;					///< Did some outside user access the FOR class to move the camera??
		Frustrum *m_frustrumPtr;					///< Viewing frustrum.
		bool m_active;								///< Is the camera active or not?
		bool m_debugCamera;							///< Is the camera in debug mode?
		boost::shared_ptr<SceneNode> m_target;		///< The target node of the camera.
		Vector4 m_camOffsetVector;					///< Direction of camera relative to target.
		//Camera m_camera;

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param frustrum The viewing frustrum.
		//
		// /////////////////////////////////////////////////////////////////
		inline explicit CameraSceneNode(Frustrum *frustrumPtr) : SceneNode(boost::optional<ActorId>(), std::string("Camera"), RenderPassFirst, Material(), GameHalloran::g_identityMat),\
			m_frame(), m_updateCameraMatrix(false), m_frustrumPtr(frustrumPtr), m_active(true), m_debugCamera(false), m_target(), m_camOffsetVector(0.0f, 0.0f, -10.0f, 0.0f)
		{
			Matrix4 camMat;
			m_frame.GetCameraMatrix(camMat);
			VSetTransform(camMat);
			SetShaderName(std::string("shaders") + ZipFile::ZIP_PATH_SEPERATOR + std::string("flat"));
		};

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~CameraSceneNode() { };

		// /////////////////////////////////////////////////////////////////
		// Updates the node once per main loop.
		//
		// @param scenePtr SceneGraph manager pointer.
		// @param elapsedTime The number of seconds since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnUpdate(SceneGraphManager *scenePtr, const F32 elapsedTime);

		// /////////////////////////////////////////////////////////////////
		// Renders node.
		//
		// @param scenePtr SceneGraph manager pointer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VRender(SceneGraphManager *scenePtr);

		// /////////////////////////////////////////////////////////////////
		// Called when application is restored.
		//
		// @param scenePtr SceneGraph manager pointer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnRestore(SceneGraphManager *scenePtr);

		// /////////////////////////////////////////////////////////////////
		// Special camera node is always visible.
		//
		// /////////////////////////////////////////////////////////////////
		inline virtual bool VIsVisible(SceneGraphManager *scenePtr) const { return (m_active); };

		// /////////////////////////////////////////////////////////////////
		// Set the ModelView matris stacks top most element to be this cameras
		// matrix.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VSetViewTransform(SceneGraphManager *scenePtr);

		// /////////////////////////////////////////////////////////////////
		// Get the viewing frustrum.
		//
		// /////////////////////////////////////////////////////////////////
		Frustrum *GetFrustum() { return (m_frustrumPtr); }

		// /////////////////////////////////////////////////////////////////
		// Set the target node of the camera.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetTarget(boost::shared_ptr<SceneNode> target){ m_target = target; };

		// /////////////////////////////////////////////////////////////////
		// Clear the target.
		//
		// /////////////////////////////////////////////////////////////////
		inline void ClearTarget() { m_target.reset(); };

		// /////////////////////////////////////////////////////////////////
		// Get the cameras target node.
		//
		// /////////////////////////////////////////////////////////////////
		inline boost::shared_ptr<SceneNode> GetTarget() { return (m_target); };

		// /////////////////////////////////////////////////////////////////
		// Set the cameras offset vector.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetCameraOffset(const Vector4 &cameraOffset) { m_camOffsetVector = cameraOffset; };

		// /////////////////////////////////////////////////////////////////
		// Get the camera frame of reference.  Do not store a reference to the
		// alias returned as if the camera node goes out of scope then it does
		// not exist anymore. Also any changes to the cameras FoR need to be
		// communicated to the SceneNodes matrices also.
		//
		// Instead use this method directly to access the GLFrame interface to
		// move the camera around, if required and these housekeeping tasks
		// will be done automatically for you.
		//
		// e.g.
		// camSceneNode->GetGlFrame().MoveUp(50.0f);
		//
		// /////////////////////////////////////////////////////////////////
		GLFrame &GetGlFrame();

		// /////////////////////////////////////////////////////////////////
		// Overridden and disabled for camera node.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VAddChild(boost::shared_ptr<ISceneNode> childNodePtr) { return (true); };

		// /////////////////////////////////////////////////////////////////
		// Overridden and disabled for camera node.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VRemoveChild(const ActorId id) { return (true); };

		// /////////////////////////////////////////////////////////////////
		// Overridden and disabled for camera node.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VRenderChildren(SceneGraphManager *scenePtr) { return (true); };

		// /////////////////////////////////////////////////////////////////
		// Turn on or off rendering the cameras' Frustrum to aid debugging
		// scene culling problems.
		//
		// /////////////////////////////////////////////////////////////////
		void SetDebug(const bool debug) { m_debugCamera = debug; };
	};

}

#endif
