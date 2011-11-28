// /////////////////////////////////////////////////////////////////
// @file GLFrame.cpp
// @author Richard S. Wright Jr & PJ O Halloran
// @date 08/09/2010
//
// A class that represents a frame of reference in 3D space for a
// 3D object.  The 3D object could be anything from a camera to a
// game actor.
//
// This code was taken from the OpenGL superbible, 5th edition
// while I was learning 3D graphics and I modified it for my own use.
//
// /////////////////////////////////////////////////////////////////


// External Headers


// Project Headers
#include "GLFrame.h"


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFrame::GetMatrix(Matrix4 &matrixOut, const bool rotationOnly)
	{
		// Ensure up and forward are orthonormal.
		Normalize();

		// Calculate the right side (x) vector, drop it right into the matrix
		Vector3 xAxis;
		xAxis = GetXAxis();
		matrixOut.SetElement(Matrix4::C0, Matrix4::R0, xAxis.GetX());
		matrixOut.SetElement(Matrix4::C0, Matrix4::R1, xAxis.GetY());
		matrixOut.SetElement(Matrix4::C0, Matrix4::R2, xAxis.GetZ());
		matrixOut.SetElement(Matrix4::C0, Matrix4::R3, 0.0f);
	   
		// Y Column
		matrixOut.SetElement(Matrix4::C1, Matrix4::R0, m_up.GetX());
		matrixOut.SetElement(Matrix4::C1, Matrix4::R1, m_up.GetY());
		matrixOut.SetElement(Matrix4::C1, Matrix4::R2, m_up.GetZ());
		matrixOut.SetElement(Matrix4::C1, Matrix4::R3, 0.0f);
	                            
		// Z Column
		matrixOut.SetElement(Matrix4::C2, Matrix4::R0, m_forward.GetX());
		matrixOut.SetElement(Matrix4::C2, Matrix4::R1, m_forward.GetY());
		matrixOut.SetElement(Matrix4::C2, Matrix4::R2, m_forward.GetZ());
		matrixOut.SetElement(Matrix4::C2, Matrix4::R3, 0.0f);

		// If translation not already done.
		if(!rotationOnly)
		{
			matrixOut.SetElement(Matrix4::C3, Matrix4::R0, m_position.GetX());
			matrixOut.SetElement(Matrix4::C3, Matrix4::R1, m_position.GetY());
			matrixOut.SetElement(Matrix4::C3, Matrix4::R2, m_position.GetZ()); 
		}
		else
		{
			matrixOut.SetElement(Matrix4::C3, Matrix4::R0, 0.0f);
			matrixOut.SetElement(Matrix4::C3, Matrix4::R1, 0.0f);
			matrixOut.SetElement(Matrix4::C3, Matrix4::R2, 0.0f);
		}
		matrixOut.SetElement(Matrix4::C3, Matrix4::R3, 1.0f);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFrame::GetCameraMatrix(Matrix4 &matrixOut, const bool rotationOnly)
	{
		// Ensure up and forward are orthonormal.
		Normalize();

		Vector3 invForward, invRight;			// Inverse vectors for camera POV.
		
		matrixOut.LoadIdentity();

		// Calculate the forward and right vectors from the camera POV.
		invForward = -m_forward;
		m_up.Cross(invForward, invRight);
		//GetRightVector(invRight);
		//invRight = -invRight;

		// Make rotation matrix
		// X column
		matrixOut.SetElement(Matrix4::C0, Matrix4::R0, invRight.GetX());
		matrixOut.SetElement(Matrix4::C0, Matrix4::R1, invRight.GetY());
		matrixOut.SetElement(Matrix4::C0, Matrix4::R2, invRight.GetZ());
		matrixOut.SetElement(Matrix4::C0, Matrix4::R3, 0.0f);
	   
		// Y Column
		matrixOut.SetElement(Matrix4::C1, Matrix4::R0, m_up.GetX());
		matrixOut.SetElement(Matrix4::C1, Matrix4::R1, m_up.GetY());
		matrixOut.SetElement(Matrix4::C1, Matrix4::R2, m_up.GetZ());
		matrixOut.SetElement(Matrix4::C1, Matrix4::R3, 0.0f);
	                            
		// Z Column
		matrixOut.SetElement(Matrix4::C2, Matrix4::R0, invForward.GetX());
		matrixOut.SetElement(Matrix4::C2, Matrix4::R1, invForward.GetY());
		matrixOut.SetElement(Matrix4::C2, Matrix4::R2, invForward.GetZ());
		matrixOut.SetElement(Matrix4::C2, Matrix4::R3, 0.0f);

		// Transpose matrix before adding translation info to it.
		matrixOut.Transpose();
		
		if(!rotationOnly)
		{
			// Apply translation too
			Matrix4 trans;				// Matrix to store the translation in.
			BuildTranslationMatrix4(trans, -m_position.GetX(), -m_position.GetY(), -m_position.GetZ());  
			matrixOut *= trans;
		}
	}


	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GLFrame::RotateLocalY(const F32 angle)
	{
		// Create a rotation matrix around the Up vector
		Matrix4 rotMat;
		BuildRotationArbitraryMatrix4(rotMat, angle, m_up.GetX(), m_up.GetY(), m_up.GetZ());
		// Calculate the new forward position considering the new rotation (mat multiplication requires a 4D vector).
		m_forward = rotMat * Vector4(m_forward);
	}


	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GLFrame::RotateLocalZ(const F32 angle)
	{
		// Create a rotation matrix around the Forward vector
		Matrix4 rotMat;
		BuildRotationArbitraryMatrix4(rotMat, angle, m_forward.GetX(), m_forward.GetY(), m_forward.GetZ());
		// Calculate the new up position considering the new rotation (mat multiplication requires a 4D vector).
		m_up = rotMat * Vector4(m_up);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFrame::RotateLocalX(const F32 angle)
	{
		// Create a rotation matrix around the Right vector
		Vector3 rightVec;
		GetRightVector(rightVec);
		Matrix4 rotMat;
		BuildRotationArbitraryMatrix4(rotMat, angle, rightVec.GetX(), rightVec.GetY(), rightVec.GetZ());
		// Rotate about the Y and Z axes (mat multiplication requires a 4D vector).
		m_up = rotMat * Vector4(m_up);
		m_forward = rotMat * Vector4(m_forward);
	}


	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFrame::Normalize(void)
	{
		// Calculate cross product of up and forward vectors
		Vector3 cross;
		m_up.Cross(m_forward, cross);

		// Use result to recalculate forward vector
		cross.Cross(m_up, m_forward);	

		// Also check for unit length.
		m_up.Normalize();
		m_forward.Normalize();
	}


	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFrame::RotateWorld(const F32 angle, const F32 x, const F32 y, const F32 z)
	{
		// Create the Rotation matrix
		Matrix4 rotMat;
		BuildRotationArbitraryMatrix4(rotMat, angle, x, y, z);

		// Transform the up and forward vectors.
		m_up = rotMat * Vector4(m_up);
		m_forward = rotMat * Vector4(m_forward);
	}


	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void GLFrame::RotateLocal(const F32 angle, const F32 x, const F32 y, const F32 z)
	{
		Vector3 vWorldVect;
		Vector3 vLocalVect(x, y, z);

		LocalToWorld(vLocalVect, vWorldVect, true);
		RotateWorld(angle, vWorldVect.GetX(), vWorldVect.GetY(), vWorldVect.GetZ());
	}


	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFrame::LocalToWorld(const Vector3 &vLocal, Vector3 &vWorld, const bool rotationOnly)
	{
		// Create the rotation matrix based on the vectors.
		Matrix4 rotMat;
		GetMatrix(rotMat, true);

		// Do the rotation.
		vWorld = rotMat * Vector4(vLocal);

		// Do the translation.
		if(!rotationOnly)
		{
			vWorld += Vector3(m_position);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFrame::WorldToLocal(const Vector3 &vWorld, Vector3 &vLocal)
	{
		// Translate the origin
		Vector3 vNewWorld;
		vNewWorld = vWorld - Vector3(m_position);

		// Create the rotation matrix based on the vectors
		Matrix4 rotMat, invMat;
		GetMatrix(rotMat, true);

		// Do the rotation based on inverted matrix
		rotMat.Inversed(invMat);

		// Convert to local coordinates.
		vLocal = invMat * Vector4(vNewWorld);
	}
    
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFrame::TransformPoint(const Point3 &srcPt, Point3 &dstPt)
	{
		Matrix4 m;
		GetMatrix(m, false);    // Rotate and translate
		
		Vector3 dstVec;
		dstVec = m * Vector4(srcPt);
		dstPt = dstVec;
	}
    
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLFrame::RotateVector(const Vector3 &srcVec, Vector3 &dstVec)
	{
		Matrix4 m;
		GetMatrix(m, true);    // Rotate only

		dstVec = m * Vector4(srcVec);
	}

}
