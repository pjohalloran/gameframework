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
// @file CameraSceneNode.cpp
// @author Mike McShaffry & PJ O Halloran
// @date 05/10/2010
//
// Special Camera SceneNode implementation file.
//
// /////////////////////////////////////////////////////////////////

// External Headers


// Project Headers
#include "CameraSceneNode.h"

#include "GameMain.h"

#include "SceneGraphManager.h"

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CameraSceneNode::VOnUpdate(const F32 elapsedTime)
	{
		// If somebod moved the camera using the FoR class then we must update it now before rendering occurs.
		if(m_updateCameraMatrix)
		{
			Matrix4 newCamMat;
			m_frame.GetCameraMatrix(newCamMat);
			VSetTransform(newCamMat);
			m_updateCameraMatrix = false;
		}

		// A camera scene node does not have any children so it is safe not to call the base class for instances of it.
		//return (SceneNode::VOnUpdate(scenePtr, elapsedTime));
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CameraSceneNode::VRender()
	{
		bool preRenderStep = false;

		bool result = SceneNode::VPreRender();
			
		if(result)
		{
			preRenderStep = true;
			result = SceneNode::VRender();
		}

		if(result && m_debugCamera)
		{
            Matrix4 prevMat, mvp;
            m_sgmPtr->GetStackManager()->GetModelViewMatrixStack()->GetMatrix(prevMat);
            m_sgmPtr->GetStackManager()->GetModelViewMatrixStack()->PopMatrix();
            m_sgmPtr->GetStackManager()->GetModelViewProjectionMatrix(mvp);

            // Inefficient but just for debugging
            m_mvpUniform = m_shaderPtr->GetUniform("mvpMatrix");
            m_colorUniform = m_shaderPtr->GetUniform("colorVec");
            m_mvpUniform->SetValue((GLfloat * const)mvp.GetComponentsConst(), 16);
            m_colorUniform->SetValue((GLfloat * const)g_gcRed.GetComponentsConst(), 4);
            
            m_frustrumPtr->Render();

            m_sgmPtr->GetStackManager()->GetModelViewMatrixStack()->PushMatrix(prevMat);
		}

		if(result || preRenderStep)
			SceneNode::VPostRender();

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CameraSceneNode::VOnRestore()
	{
		// Proj matrix used to be set here but this is done in my global game app class now (it could also be done here).
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CameraSceneNode::VSetViewTransform()
	{
		//If there is a target, make sure the camera is
		// rigidly attached right behind the target
		if(m_target)
		{
			// Get targets transformation matrix.
			Matrix4 targetTransformMat(m_target->VGet()->GetToWorld());

			// Transform the offset vector by the targets matrix to get the offset vector in world coordinates.
			Vector4 atWorld = targetTransformMat * m_camOffsetVector;

			// Get the position of the target and apply the offset.
			Point3 pos;
			targetTransformMat.GetPosition(pos);
			pos += Vector3(atWorld);

			// Set our cameras transformation to be equal to the targets with the offset in position applied.
			targetTransformMat[Matrix4::M30] = pos.GetX();
			targetTransformMat[Matrix4::M31] = pos.GetY();
			targetTransformMat[Matrix4::M32] = pos.GetZ();
			targetTransformMat[Matrix4::M33] = 1.0f;
			VSetTransform(targetTransformMat);
		}

		m_sgmPtr->GetStackManager()->GetModelViewMatrixStack()->LoadMatrix(VGet()->GetToWorld());
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	GLFrame &CameraSceneNode::GetGlFrame()
	{
		m_updateCameraMatrix = true;
		return (m_frame);
	}

}
