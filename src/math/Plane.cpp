// /////////////////////////////////////////////////////////////////
// @file Plane.cpp
// @author PJ O Halloran
// @date 20/11/2010
//
// File contains the implemenation for the Plane class.
//
// /////////////////////////////////////////////////////////////////

// External Headers


// Project Headers
#include "Plane.h"


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Plane::Plane(const Vector3 &normal, const Point3 &point) : m_normal(normal), m_d(0.0f)
	{
		// Ennsure the normal vector is of unit length.
		m_normal.Normalize();
		RecalculateD(point);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Plane::Plane(const Point3 &pt0, const Point3 &pt1, const Point3 &pt2) : m_normal(), m_d(0.0f)
	{
		Vector3 u, v;
		u = pt1 - pt0;
		v = pt2 - pt0;

		u.Cross(v, m_normal);
		m_normal.Normalize();
		RecalculateD(pt0);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Plane &Plane::Normalize()
	{
		F32 magNormal = m_normal.Magnitude();
		m_normal /= magNormal;
		m_d /= magNormal;
		return (*this);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	F32 Plane::Distance(const Point3 &pt) const
	{
		return (m_normal.Dot(Vector3(pt)) + m_d);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Plane &Plane::Transform(const Matrix4 &transformMatrix)
	{
		Plane tmp;
		Transformed(transformMatrix, tmp);
		*this = tmp;
		return (*this);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Plane::Transformed(const Matrix4 &transformMatrix, Plane &outPlane) const
	{
		// We need to calculate the inverse, transpose of the transformation matrix to transform the plane!
		Matrix4 tmpMat;
		transformMatrix.Inversed(tmpMat);
		tmpMat.Transpose();

		// Create a 4 componenet vector from this plane storing the normal in XYZ and the D component in W.
		Vector4 planeVec4(m_normal.GetX(), m_normal.GetY(), m_normal.GetZ(), m_d);

		// Transform the plane.
		Vector4 tPlaneVec4 = transformMatrix * planeVec4;

		// Store the result in the output Plane object.
		outPlane.m_normal.Set(tPlaneVec4.GetX(), tPlaneVec4.GetY(), tPlaneVec4.GetZ());
		outPlane.m_d = tPlaneVec4.GetW();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Plane::GetNearestPoint(const Point3 &pt, Point3 &nearestPlanePt) const
	{
		F32 k = Distance(pt);
		Vector3 ptToPlaneVec = m_normal * -k;
		nearestPlanePt = pt + ptToPlaneVec;
	}


}
