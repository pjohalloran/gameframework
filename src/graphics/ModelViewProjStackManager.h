#pragma once
#ifndef __GLT_GEOMETRY_PIPELINE
#define __GLT_GEOMETRY_PIPELINE

/*
Copyright (c) 2009, Richard S. Wright Jr.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list 
of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this list 
of conditions and the following disclaimer in the documentation and/or other 
materials provided with the distribution.

Neither the name of Richard S. Wright Jr. nor the names of other contributors may be used 
to endorse or promote products derived from this software without specific prior 
written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// /////////////////////////////////////////////////////////////////
// @file ModelViewProjStackManager.h
// @author Richard S. Wright Jr. and PJ O Halloran.
// @date 15/09/2010
//
// File contains the header for the ModelViewProjStackManager class
// for managing the modelview and projection matrix stacks.
//
// Changes from original:
// - Clearly commented.
//
// - Edited to use my 3D math lib.
//
// - Renamed class to have a more meaningfull name (for me) from 
//		GLGeometryTrnsform to ModelViewProjStackManager.
//
// - This class used to contain raw pointers to external objects.
// This was dangerous as a crash will occur if the pointers refer to
// objects that have gone out of scope.  So to improve this I used
// boost shared pointers to gaurantee that we are alwas referring to
// object that exist.
//
// - To get the model view projection matrix ou used to have to call
// a method that would pass back a constant reference.  This is dangerous
// so I removed storing the model view matrix and changed the interface
// slightly so the caller passes in a matrix we fill out here...
//
// /////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>

#include "Matrix.h"
#include "GLMatrixStack.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class ModelViewProjStackManager
	// @author PJ O Halloran & Richard S. Wright Jr.
	//
	// The manager class for the modelview and projection matrix stacks.
	//
	// /////////////////////////////////////////////////////////////////
	class ModelViewProjStackManager
	{
	private:

		//F32 m_normalMatrix[9];									///< The normal matrix, (used in lighting calculations, derived from the current mv matrix).
		boost::shared_ptr<GLMatrixStack> m_modelViewStack;			///< ModelView matrix stack.
		boost::shared_ptr<GLMatrixStack> m_projectionStack;			///< Projection matrix stack.

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		ModelViewProjStackManager(boost::shared_ptr<GLMatrixStack> mvStack,
									boost::shared_ptr<GLMatrixStack> projStack)
									: m_modelViewStack(mvStack)
									, m_projectionStack(projStack)
		{
		};

		// /////////////////////////////////////////////////////////////////
		// Destructor (non virtual so you shouldnt derive from this class).
		//
		// /////////////////////////////////////////////////////////////////
		~ModelViewProjStackManager() {};

		// /////////////////////////////////////////////////////////////////
		// Set the model view matrix stack.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetModelViewMatrixStack(boost::shared_ptr<GLMatrixStack> mModelView) { m_modelViewStack = mModelView; };

		// /////////////////////////////////////////////////////////////////
		// Set the projection matrix stack.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetProjectionMatrixStack(boost::shared_ptr<GLMatrixStack> mProjection) { m_projectionStack = mProjection; };

		// /////////////////////////////////////////////////////////////////
		// Set both the matrix stacks.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetMatrixStacks(boost::shared_ptr<GLMatrixStack> mModelView, boost::shared_ptr<GLMatrixStack> mProjection)
		{
			SetModelViewMatrixStack(mModelView);
			SetProjectionMatrixStack(mProjection);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the current model view projection matrix.
		//
		// @param mvProjMatrix The matrix you supply to hold the current
		//						mv-proj matrix.
		//
		// @return Matrix4& A reference to the mv-proj matrix.  It refers to
		//					the same object ou pass in.  This is useful for
		//					writing commands like:
		//					GetModelViewProjectionMatrix(mat).Inverse() or
		//					whatever.  Just its useful for working striaght
		//					away on the matrix in the same line of code.
		//
		// /////////////////////////////////////////////////////////////////
		Matrix4 &GetModelViewProjectionMatrix(Matrix4 &mvProjMatrix)
		{
			// Get the top most mv and proj matrices (check for NULL stack pointers too).
			Matrix4 mvMat, projMat;
			if(m_modelViewStack)
			{
				m_modelViewStack->GetMatrix(mvMat);
			}
			if(m_projectionStack)
			{
				m_projectionStack->GetMatrix(projMat);
			}

			// Concatenate them together.
			mvProjMatrix = projMat * mvMat;
			return (mvProjMatrix);
		};

		// /////////////////////////////////////////////////////////////////
		// Get a pointer to the current modelview matrix stack.
		//
		// /////////////////////////////////////////////////////////////////
		inline boost::shared_ptr<GLMatrixStack> GetModelViewMatrixStack(void) { return (m_modelViewStack); };

		// /////////////////////////////////////////////////////////////////
		// Get a pointer to the current projection matrix stack.
		//
		// /////////////////////////////////////////////////////////////////
		inline boost::shared_ptr<GLMatrixStack> GetProjectionMatrixStack(void) { return (m_projectionStack); };

		// /////////////////////////////////////////////////////////////////
		// Gets the normal matrix from the current modelview matrix.
		// This is used mainly in the GLSL shaders for lighting calculations.
		//
		// @param rotMatrix An array representing a 3x3 matrix stored in
		//					column major format.
		// @param normalize Should the columns of the rotation matrix be
		//					normalized?
		//
		// /////////////////////////////////////////////////////////////////
		void GetNormalMatrix(Matrix3x3 rotMatrix, const bool normalize = false)
		{
			Matrix4 mvMatrix;
			m_modelViewStack->GetMatrix(mvMatrix);
			ExtractRotationMatrix(mvMatrix, rotMatrix);
			if(normalize)
			{
				Vector3 colOne(rotMatrix[0], rotMatrix[1], rotMatrix[2]);
				Vector3 colTwo(rotMatrix[3], rotMatrix[4], rotMatrix[5]);
				Vector3 colThree(rotMatrix[6], rotMatrix[7], rotMatrix[8]);

				colOne.Normalize();
				colTwo.Normalize();
				colThree.Normalize();

				rotMatrix[0] = colOne.GetX();
				rotMatrix[1] = colOne.GetY();
				rotMatrix[2] = colOne.GetZ();
				rotMatrix[3] = colTwo.GetX();
				rotMatrix[4] = colTwo.GetY();
				rotMatrix[5] = colTwo.GetZ();
				rotMatrix[6] = colThree.GetX();
				rotMatrix[7] = colThree.GetY();
				rotMatrix[8] = colThree.GetZ();
			}
		}
	};

}

#endif
