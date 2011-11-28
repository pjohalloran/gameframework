// /////////////////////////////////////////////////////////////////
// @file Vector.cpp
// @author PJ O Halloran
// @date 22/08/2010
//
// File contains the implementations for the Vector3 and Vector4
// classes.  Please see the header file for more details.
//
// /////////////////////////////////////////////////////////////////


// External Headers


// Project Headers
#include "Vector.h"


namespace GameHalloran
{

	// The unit vectors along the x, y and z axes.
	// Declared here as global constants as they will be used often.
	const Vector3 g_v3XunitVec(1.0f, 0.0f, 0.0f);
	const Vector3 g_v3YunitVec(0.0f, 1.0f, 0.0f);
	const Vector3 g_v3ZunitVec(0.0f, 0.0f, 1.0f);

	// Define our global vectors describing the coordinate system in use.
	//  NB. Note the forward vector is in the negative Z axis so we are using
	//   a RIGHT HAND coordinate system.
	const Vector3 g_up(0.0f, 1.0f, 0.0f);
	const Vector3 g_right(1.0f, 0.0f, 0.0f);
	const Vector3 g_forward(0.0f, 0.0f, -1.0f);
	const Vector4 g_up4(g_up);
	const Vector4 g_right4(g_right);
	const Vector4 g_forward4(g_forward);

	// /////////////////////////////////////////////////////////////////
	// ************************* Vector3 *******************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Vector3::Vector3(const Point3 &pt)
	{
		m_vec[0] = pt.GetX();
		m_vec[1] = pt.GetY();
		m_vec[2] = pt.GetZ();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Vector3::Vector3(const Vector4 &vec4)
	{
		if(vec4.GetW() > 0.0f)
		{
			m_vec[0] = vec4.GetX() / vec4.GetW();
			m_vec[1] = vec4.GetY() / vec4.GetW();
			m_vec[2] = vec4.GetZ() / vec4.GetW();
		}
		else
		{
			m_vec[0] = vec4.GetX();
			m_vec[1] = vec4.GetY();
			m_vec[2] = vec4.GetZ();
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Vector3 &Vector3::operator=(const Vector4 &rhs)
	{
		if(!FloatCmp(rhs.GetW(), 0.0f))
		{
			m_vec[0] = rhs.GetX() / rhs.GetW();
			m_vec[1] = rhs.GetY() / rhs.GetW();
			m_vec[2] = rhs.GetZ() / rhs.GetW();
		}
		else
		{
			m_vec[0] = rhs.GetX();
			m_vec[1] = rhs.GetY();
			m_vec[2] = rhs.GetZ();
		}
		return (*this);
	};

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Vector3 &Vector3::operator=(const Point3 &rhs)
	{
		m_vec[0] = rhs.GetX();
		m_vec[1] = rhs.GetY();
		m_vec[2] = rhs.GetZ();
		return (*this);
	};

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Vector3 &Vector3Lerp(const Vector3 &a, const Vector3 &b, const F32 interpolateVal, Vector3 &outVec)
	{
		F32 copyIVal(interpolateVal);
		Clamp<F32>(copyIVal, 0.0f, 1.0f);
		outVec.Set(InterpolateFloat(copyIVal, a.GetX(), b.GetX()),\
						InterpolateFloat(copyIVal, a.GetY(), b.GetY()),\
						InterpolateFloat(copyIVal, a.GetZ(), b.GetZ()));
		return (outVec);
	}

	// /////////////////////////////////////////////////////////////////
	// ************************* Vector4 *******************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Vector4::Vector4(const Point3 &pt)
	{
		m_vec[0] = pt.GetX();
		m_vec[1] = pt.GetY();
		m_vec[2] = pt.GetZ();
		m_vec[3] = 1.0f;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Vector4 &Vector4::operator=(const Point3 &rhs)
	{
		m_vec[0] = rhs.GetX();
		m_vec[1] = rhs.GetY();
		m_vec[2] = rhs.GetZ();
		m_vec[3] = 1.0f;
		return (*this);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Vector4 &Vector4Lerp(const Vector4 &a, const Vector4 &b, const F32 interpolateVal, Vector4 &outVec)
	{
		F32 copyIVal(interpolateVal);
		Clamp<F32>(copyIVal, 0.0f, 1.0f);
		outVec.Set(InterpolateFloat(copyIVal, a.GetX(), b.GetX()),\
						InterpolateFloat(copyIVal, a.GetY(), b.GetY()),\
						InterpolateFloat(copyIVal, a.GetZ(), b.GetZ()),\
						InterpolateFloat(copyIVal, a.GetW(), b.GetW()));
		return (outVec);
	}

	// /////////////////////////////////////////////////////////////////
	// ************************* Point3 *******************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Point3 &Point3Lerp(const Point3 &a, const Point3 &b, const F32 interpolateVal, Point3 &outPt)
	{
		F32 copyIVal(interpolateVal);
		Clamp<F32>(copyIVal, 0.0f, 1.0f);
		outPt.Set(InterpolateFloat(copyIVal, a.GetX(), b.GetX()),\
						InterpolateFloat(copyIVal, a.GetY(), b.GetY()),\
						InterpolateFloat(copyIVal, a.GetZ(), b.GetZ()));
		return (outPt);
	}

	// Global constants for Point3.
	const Point3 g_originPt(0.0f, 0.0f, 0.0f);

	// /////////////////////////////////////////////////////////////////
	// ****************** Point3 Helper funtions ***********************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Vector3 &CalculateNormal(const Point3 &ptOne, const Point3 &ptTwo, const Point3 &ptThree, Vector3 &normalOut)
	{
		Vector3 u, v;

		// See Mathematics for Computer Graphics, 2nd Edition by John Vince,
		// Chapter 6, Vectors, pg. 47 for details on how to calculate the
		// normal for a surface.
		u = ptThree - ptTwo;
		v = ptThree - ptOne;
		u.Cross(v, normalOut);
		return (normalOut);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Vector3 &GenerateRandomVector3(Vector3 &vec, CRandom &rng, const Vector3 &minVec, const Vector3 &maxVec)
	{
		vec.SetX(rng.Random(minVec.GetX(), maxVec.GetX()));
		vec.SetY(rng.Random(minVec.GetY(), maxVec.GetY()));
		vec.SetZ(rng.Random(minVec.GetZ(), maxVec.GetZ()));
		return (vec);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Vector4 &GenerateRandomVector4(Vector4 &vec, CRandom &rng, const Vector4 &minVec, const Vector4 &maxVec)
	{
		vec.SetX(rng.Random(minVec.GetX(), maxVec.GetX()));
		vec.SetY(rng.Random(minVec.GetY(), maxVec.GetY()));
		vec.SetZ(rng.Random(minVec.GetZ(), maxVec.GetZ()));
		vec.SetW(rng.Random(minVec.GetW(), maxVec.GetW()));
		return (vec);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Point3 &GenerateRandomPoint3(Point3 &pt, CRandom &rng, const Point3 &minPt, const Point3 &maxPt)
	{
		pt.SetX(rng.Random(minPt.GetX(), maxPt.GetX()));
		pt.SetY(rng.Random(minPt.GetY(), maxPt.GetY()));
		pt.SetZ(rng.Random(minPt.GetZ(), maxPt.GetZ()));
		return (pt);
	}

}
