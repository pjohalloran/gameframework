// /////////////////////////////////////////////////////////////////
// @file RayCast.cpp
// @author PJ O Halloran
// @date 22/11/2010
//
// File contains the implementation for the RayCast class.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <cmath>


// Project Headers
#include "RayCast.h"

#include "GameBase.h"


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool RayCast::TriangleRayIntersectHelper(const Point3 &ptA, const Point3 &ptB, const Point3 &ptC, F32 &t) const
	{
		Vector3 edge1(ptB - ptA);
		Vector3 edge2(ptC - ptA);

		Vector3 perpendicularVec;
		m_direction.Cross(edge2, perpendicularVec);

		// Get determinant, if its 0, then no intersection can have occurred.
		F32 determinant = edge1.Dot(perpendicularVec);
		if(FloatCmp(determinant, 0.0f))
		{
			return (false);
		}

		// Get distance from V0 to ray origin.
		Vector3 distanceVec = m_origin - ptA;

		// Calculate u
		F32 u = distanceVec.Dot(perpendicularVec);
		if((u < 0.0f) || (u > determinant))
		{
			return (false);
		}

		Vector3 tmp;
		distanceVec.Cross(edge1, tmp);

		// Calculate v
		F32 v = m_direction.Dot(tmp);
		if((v < 0.0f) || (u + v > determinant))
		{
			return (false);
		}

		// Calculate t now
		t = edge2.Dot(tmp);
		determinant = 1.0f / determinant;
		t *= determinant;
		u *= determinant;
		v *= determinant;

		if((t >= 0.0f) && (t <= 1.0f))
		{
			// Intersection occurred
			return (true);
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool RayCast::SphereRayIntersectHelper(const BoundingSphere &sphere, F32 &t0, F32 &t1) const
	{
		// At^2 + Bt + C = 0.
		// A = m_direction.Dot(m_direction) (If m_direction is normalized, A == 1).
		// B = 2 * (m_direction.Dot(P0 - c)) (P0 is ray origin, c is centre of sphere).
		// C = (P0 -c).Dot(P0 - c) - r^2.
		Vector3 originToCentreVec = Vector3(m_origin) - Vector3(sphere.GetCentre());
		const F32 A = 1;
		const F32 B = 2.0f * m_direction.Dot(originToCentreVec);
		const F32 C = originToCentreVec.Dot(originToCentreVec) - (sphere.GetRadius() * sphere.GetRadius());

		// find the discriminant
		F32 discriminant = (B * B) - (4.0f * C);

		// test for imaginary number
		if(discriminant < 0.0f)
		{
			// No intersections, Ray missed the sphere!
			return (false);
		}

		discriminant = sqrtf(discriminant);

		// Calculate t0 and t1.
		t0 = (-B + discriminant) / 2.0f;
		t1 = (-B - discriminant) / 2.0f;

		// if a solution is >= 0, then we intersected the sphere either once or twice.
		if((t0 >= 0.0f) || (t1 >= 0.0f))
		{
			return (true);
		}
		
		// Ray is in front of the sphere, No intersections!
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	RayCast::RayCast(const Point3 &origin, const Vector3 &direction) : m_origin(origin), m_direction(direction)
	{
		// Ensure the direction vector is a unit vector.
		m_direction.Normalize();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	RayCast::RayCast(const I32 screenX, const I32 screenY, const I32 screenWidth, const I32 screenHeight, const Matrix4 &projMatrix) : m_origin(), m_direction()
	{
		F32 m00, m11;
		projMatrix.GetElement(Matrix4::C0, Matrix4::R0, m00);
		projMatrix.GetElement(Matrix4::C1, Matrix4::R1, m11);
		// Projection point x before the projection matrix was applied.
		F32 px = ((((2.0f * screenX) / screenWidth) - 1.0f) / m00);
		// Projection point y before the projection matrix was applied.
		F32 py = ((((-2.0f * screenY) / screenHeight) + 1.0f) / m11);

		// Origin is already 0,0,0, direction is:
		m_direction.Set(px, py, 1.0f);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	RayCast &RayCast::Transform(const Matrix4 &transformMatrix)
	{
		RayCast tmp(*this);
		Transformed(transformMatrix, tmp);
		*this = tmp;
		return (*this);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void RayCast::Transformed(const Matrix4 &transformMatrix, RayCast &outRay) const
	{
		Vector4 originPtVec4(outRay.m_origin);				// Positional 4 component vector of the Rays origin point.
		Vector4 directionVec4(outRay.m_direction);			// 4 component Vector of the rays directional vector.

		// Transform the Rays origin and direction.
		originPtVec4 = transformMatrix * originPtVec4;
		directionVec4 = transformMatrix * directionVec4;

		// Set the output Rays member origin and direction (ensure the direction is normalized).
		outRay.m_origin = Vector3(originPtVec4);
		outRay.m_direction = directionVec4;
		outRay.m_direction.Normalize();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void RayCast::GetPointOnRay(const F32 t, Point3 &pt) const
	{
		pt = Vector3(m_origin) + (t * m_direction);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool RayCast::DoesRayPlaneIntersect(const Plane &plane) const
	{
		Vector3 planeNormal;
		plane.GetUnitNormal(planeNormal);
		F32 denominator = planeNormal.Dot(m_direction);
		if(FloatCmp(denominator, 0.0f))
		{
			// Ray and plane are parallel so they can't intersect.
			return (false);
		}

		// Else they are not parallel so they must intersect.
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool RayCast::GetRayPlaneIntersectionPoint(const Plane &plane, Point3 &intersectionPt) const
	{
		Vector3 planeNormal;
		plane.GetUnitNormal(planeNormal);
		F32 denominator = planeNormal.Dot(m_direction);
		if(FloatCmp(denominator, 0.0f))
		{
			// Ray and plane are parallel so they can't intersect.
			return (false);
		}

		F32 numerator = plane.GetD() - planeNormal.Dot(Vector3(m_origin));
		F32 t = numerator / denominator;
		GetPointOnRay(t, intersectionPt);
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool RayCast::GetRayTriangleIntersectionPoint(const Point3 &ptA, const Point3 &ptB, const Point3 &ptC, Point3 &intersectionPt) const
	{
		F32 t;
		bool intersectOccurred = TriangleRayIntersectHelper(ptA, ptB, ptC, t);
		GetPointOnRay(t, intersectionPt);
		return (intersectOccurred);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool RayCast::GetRaySphereIntersectionPoints(const BoundingSphere &sphere, Point3List &intersectionPtList) const
	{
		intersectionPtList.clear();

		F32 t0, t1;
		bool intersectOccurred = SphereRayIntersectHelper(sphere, t0, t1);
		if(t0 >= 0.0f)
		{
			boost::shared_ptr<Point3> ptA(GCC_NEW Point3);
			GetPointOnRay(t0, *(ptA.get()));
			intersectionPtList.push_back(ptA);
		}
		if(t1 >= 0.0f)
		{
			boost::shared_ptr<Point3> ptB(GCC_NEW Point3);
			GetPointOnRay(t1, *(ptB.get()));
			intersectionPtList.push_back(ptB);
		}
		return (intersectOccurred);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool RayCast::DoesRayCubeIntersect(const BoundingCube &cube) const
	{
		// TODO: Implement if required.
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool RayCast::GetRayCubeIntersectionPoints(const BoundingCube &cube, Point3List &intersectionPtList) const
	{
		// TODO: Implement if required.
		return (false);
	}

}
