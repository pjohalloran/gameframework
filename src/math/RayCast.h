#pragma once
#ifndef __GF_RAYCAST_H
#define __GF_RAYCAST_H

// /////////////////////////////////////////////////////////////////
// @file RayCast.h
// @author PJ O Halloran
// @date 22/11/2010
//
// File contains the header for the RayCast class.
//
// This is part of a simple 3D math library I wrote in order to try
// to gain a great understanding of 3D math.
//
// I am using OpenGL so the libs are aimed at being a help for this
// but they should in theory be useful for Direct3D or others also.
// The only difference that I know of is that OpenGL uses the column
// major matrix format while Direct3D uses row major matrix format.
//
// As you will see below, I was influenced by other well written math
// libraries freely available but this code was mostly written by myself
// as a learning experience.  Therefore the code is geared towards
// understanding rather than effiency.
//
// For example, one way i try to make the implementation clear is that
// I use seperate x, y and z members in the Vector3 class.  In an optimized
// lib, they would use an array or instruct the compiler to
// tightly pack the members for maximum throuhput to the GPU.
//
// On the other hand for effiency sake, I do not make use of inheritance or
// virtual functions at all.  For example, I could have Vector4
// inherit from Vector3 and provide Vec4 specific implemetations of Cross,
// Dot, etc.  This would save on code duplication but would have the overhead
// of base class constructors, extra calls, virtual function tables etc.
// In this case the functionality I am duplicating is not that large or
// complex and the performace boost will be worth it.  It does require
// very strong unit testing to verify it is correct on the other hand and
// that it stays correct over time.
//
// If I wasn't learning 3D math, then I would not write my own version.
// Instead I would use something like the Bullet VectorMath library or the
// Direct3D math library.
//
// I would like to cite the following math textbooks/web resources I used
// to learn the math required to write this simple 3D math library:
// - Mathematics for Computer Graphics 2nd Edition by John Vince.
// - Fundamentals of Computer Graphics 3rd Edition by Shirley et al.
// - Mathworld (http://www.mathworld.com).
// - Wolfgang Engels blog and oolong engine 2
//		(http://wolfgang-engel.info/blogs).
// - OpenGL Superbible, 5th Edition by Wright et al.
// - OpenGL Programming Guide (red), 7th Edition by Shreiner et al.
// - Bullet v2.76 Vector Math Library by Edwin Cousins
//		(http://www.bulletphysics.com).
// - Game Coding Complete, 3rd Edition by Mike McShaffry et al.
//
// For RayCast math, I also referred to "Introduction to 3D Game Programming
// with DirectX 9.0, First Edition by Frank D. Luna".
//
// /////////////////////////////////////////////////////////////////

#include <string>
#include <ostream>
#include <sstream>

#include "CommonMath.h"
#include "Vector.h"
#include "Matrix.h"
#include "Plane.h"
#include "BoundingSphere.h"
#include "BoundingCube.h"

namespace GameHalloran
{
	// /////////////////////////////////////////////////////////////////
	// @class RayCast
	// @author PJ O Halloran
	//
	// This class defines a RayCast. A ray is a line of infinite length 
	// with an origin somewhere in 3D space and a direction vector.
	//
	// It may be used for 3D object collision detection and for picking
	// objects from screen space using the mouse cursor.
	//
	// /////////////////////////////////////////////////////////////////
	class RayCast
	{
		private:

			Point3 m_origin;					///< Origin of the ray.
			Vector3 m_direction;				///< Direction of the ray.

			// /////////////////////////////////////////////////////////////////
			// Default constructor disallowed.
			//
			// /////////////////////////////////////////////////////////////////
			RayCast();

			// /////////////////////////////////////////////////////////////////
			// Location of common triangle/ray intersection code.
			//
			// @param ptA Triangle vertex A.
			// @param ptB Triangle vertex B.
			// @param ptC Triangle vertex C.
			// @param t If an intersection occurred this F32 will hold the
			//				distance along the ray that it occurred.
			//
			// @return bool True if the ray and triangle intersect and false
			//				otherwise.
			//
			// /////////////////////////////////////////////////////////////////
			bool TriangleRayIntersectHelper(const Point3 &ptA, const Point3 &ptB, const Point3 &ptC, F32 &t) const;

			// /////////////////////////////////////////////////////////////////
			// Location of common sphere/ray intersection code.
			//
			// @param sphere The sphere to test.
			// @param t0 The distance to the first point of intersection.
			// @param t1 The distance to the second point of intersection.
			//
			// @return bool True if the ray and triangle intersect and false
			//				otherwise.
			//
			// /////////////////////////////////////////////////////////////////
			bool SphereRayIntersectHelper(const BoundingSphere &sphere, F32 &t0, F32 &t1) const;


		public:

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Creates a ray using the specified origin and direction.
			//
			// This ray exists in WORLD space coordinates and does not need any
			// further transformations.
			//
			// This constructor gives us a ray thats useful for detecting collisions
			// between scene objects.
			//
			// @param origin The origin point of the Ray.
			// @param direction The direction vector of the ray.
			//
			// /////////////////////////////////////////////////////////////////
			explicit RayCast(const Point3 &origin, const Vector3 &direction);

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Given a position on screen and the viewport width & height plus
			// the current projection matrix, compute/create the Ray beginning at
			// the origin of view space and passing through the screen coordinate.
			//
			// This ray exists in VIEW space coordinates.  To test if a ray
			// intersects with any game objects you will need to have the ray and
			// objects in the same coordinate system. For example, one method would
			// be to transform the ray into world space and test if it intersects
			// with any objects.  Get the MODELVIEW matrix, calculate its inverse
			// and transform the Ray by that matrix to accomplish this!
			//
			// This constructor gives us a ray useful for "picking" scene objects
			// with a mouse cursor.
			//
			// @param screenX X position on the viewport window.
			// @param screenY Y position on the viewport window.
			// @param screenWidth Width of screen/viewport.
			// @param screenHeight Height of screen/viewport.
			// @param projMatrix The projection matrix.
			//
			// /////////////////////////////////////////////////////////////////
			explicit RayCast(const I32 screenX, const I32 screenY, const I32 screenWidth, const I32 screenHeight, const Matrix4 &projMatrix);

			// /////////////////////////////////////////////////////////////////
			// Destructor.
			//
			// /////////////////////////////////////////////////////////////////
			~RayCast() { };

			// /////////////////////////////////////////////////////////////////
			// Get the direction of the ray.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 GetDirection() const { return (m_direction); };

			// /////////////////////////////////////////////////////////////////
			// Set the direction of the ray.
			//
			// /////////////////////////////////////////////////////////////////
			inline void SetDirection(const Vector3 &direction) { m_direction = direction; };
			
			// /////////////////////////////////////////////////////////////////
			// Get the origin point of the ray.
			//
			// /////////////////////////////////////////////////////////////////
			inline Point3 GetOrigin() const { return (m_origin); };

			// /////////////////////////////////////////////////////////////////
			// Set the origin point of the ray.
			//
			// /////////////////////////////////////////////////////////////////
			inline void SetOrigin(const Point3 &origin) { m_origin = origin; }; 

			// /////////////////////////////////////////////////////////////////
			// Check if two Rays' are equivalent.
			//
			// @param rhs The Ray to check against this Ray.
			//
			// @return bool True if two Ray are equal and false otherwise.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool operator==(const RayCast &rhs) const { return ((m_origin == rhs.m_origin) && (m_direction == rhs.m_direction)); };

			// /////////////////////////////////////////////////////////////////
			// Check if two Rays' are not equivalent.
			//
			// @param rhs The Ray to check against this Ray.
			//
			// @return bool True if two Rays are not equal and false otherwise.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool operator!=(const RayCast &rhs) const { return !(*this == rhs); };

			// /////////////////////////////////////////////////////////////////
			// Transform a ray with a transformation matrix supplied.
			//
			// @param transformMatrix The transformation you wish to apply to the
			//							ray.
			//
			// @return RayCast& A reference to this RayCast object.
			//
			// /////////////////////////////////////////////////////////////////
			RayCast &Transform(const Matrix4 &transformMatrix);

			// /////////////////////////////////////////////////////////////////
			// Transform a ray with a transformation matrix supplied.  This 
			// RayCast object is not affected.  Instead the transformed ray 
			// is saved to the "outRay" parameter.
			//
			// @param transformMatrix The transformation you wish to apply to the
			//							ra.
			// @param outRay The RayCast object to store the transformed plane 
			//					result in.
			//
			// /////////////////////////////////////////////////////////////////
			void Transformed(const Matrix4 &transformMatrix, RayCast &outRay) const;

			// /////////////////////////////////////////////////////////////////
			// Find a point on the ray.
			//
			// A ray is defined mathematically as:
			// p(t) = P0 + tu	(P0 is the ray origin, u is the ray direction).
			// with t being the distance along the ray (0 <= t <= infinity).
			//
			// This function simply finds a point on the ray for different values 
			// of t.
			//
			// @param t The distance along the ray.
			// @param pt The point found for the value of t supplied.
			//
			// /////////////////////////////////////////////////////////////////
			void GetPointOnRay(const F32 t, Point3 &pt) const;

			// /////////////////////////////////////////////////////////////////
			// Determine if the ray intersects with a 3D plane.
			//
			// @param plane The 3D plane.
			//
			// @return bool If the ray intersects a 3D plane and false if not.
			//
			// /////////////////////////////////////////////////////////////////
			bool DoesRayPlaneIntersect(const Plane &plane) const;

			// /////////////////////////////////////////////////////////////////
			// Determine if the ray intersects with a 3D plane.
			// Also retrieves the intersection point.
			//
			// @param plane The 3D plane.
			// @param intersectionPt The point of intersection of the ray and
			//							plane IF an intersection exists.
			//
			// @return bool If the ray intersects a 3D plane and false if not.
			//
			// /////////////////////////////////////////////////////////////////
			bool GetRayPlaneIntersectionPoint(const Plane &plane, Point3 &intersectionPt) const;

			// /////////////////////////////////////////////////////////////////
			// Determine if the ray intersects with a 3D triangle.  The three
			// vertices of the triangle should be specified in clockwise order
			// or else the result will be incorrect.
			//
			// @param ptA Triangle vertex A.
			// @param ptB Triangle vertex B.
			// @param ptC Triangle vertex C.
			//
			// @return bool If the ray intersects a triangle and false if not.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool DoesRayTriangleIntersect(const Point3 &ptA, const Point3 &ptB, const Point3 &ptC) const
			{
				F32 t;
				return (TriangleRayIntersectHelper(ptA, ptB, ptC, t));
			};

			// /////////////////////////////////////////////////////////////////
			// Determine if the ray intersects with a 3D plane.  The three
			// vertices of the triangle should be specified in clockwise order
			// or else the result will be incorrect.  Also retrieves the 
			// intersection point.
			//
			// @param ptA Triangle vertex A.
			// @param ptB Triangle vertex B.
			// @param ptC Triangle vertex C.
			// @param intersectionPt Holds the point of intersection of the ray
			//							and triangle IF an intersection exists.
			//
			// @return bool If the ray intersects a 3D plane and false if not.
			//
			// /////////////////////////////////////////////////////////////////
			bool GetRayTriangleIntersectionPoint(const Point3 &ptA, const Point3 &ptB, const Point3 &ptC, Point3 &intersectionPt) const;

			// /////////////////////////////////////////////////////////////////
			// Determine if the ray intersects with a sphere.
			//
			// @param sphere The sphere.
			//
			// @return bool If the ray intersects a sphere and false if not.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool DoesRaySphereIntersect(const BoundingSphere &sphere) const
			{
				F32 t0, t1;
				return (SphereRayIntersectHelper(sphere, t0, t1));
			}

			// /////////////////////////////////////////////////////////////////
			// Determine if the ray intersects with a sphere. Also retrieves the
			// intersections points, if an intersection occurred.  There may be 
			// 0, 1 or 2 intersection points depending on the position of the ray
			// and the sphere.
			//
			// @param sphere The sphere.
			// @param intersectionPtList List of intersection points.  Will be
			//								empty if no intersections occurred.
			//
			// @return bool If the ray intersects a sphere and false if not.
			//
			// /////////////////////////////////////////////////////////////////
			bool GetRaySphereIntersectionPoints(const BoundingSphere &sphere, Point3List &intersectionPtList) const;

			// /////////////////////////////////////////////////////////////////
			// Determine if the ray intersects with a cube.
			//
			// @param cube The cube.
			//
			// @return bool If the ray intersects a cube and false if not.
			//
			// /////////////////////////////////////////////////////////////////
			bool DoesRayCubeIntersect(const BoundingCube &cube) const;

			// /////////////////////////////////////////////////////////////////
			// Determine if the ray intersects with a cube. Also retrieves the
			// intersections points, if an intersection occurred.  There may be 
			// 0, 1 or 2 intersection points depending on the position of the ray
			// and the cube.
			//
			// @param cube The cube.
			// @param intersectionPtList List of intersection points.  Will be
			//								empty if no intersections occurred.
			//
			// @return bool If the ray intersects a cube and false if not.
			//
			// /////////////////////////////////////////////////////////////////
			bool GetRayCubeIntersectionPoints(const BoundingCube &cube, Point3List &intersectionPtList) const;

			// /////////////////////////////////////////////////////////////////
			// Return a string of the Rays' components.
			//
			// @return std::string
			//
			// /////////////////////////////////////////////////////////////////
			inline std::string ToString() const
			{
				return (std::string("Ray = Origin: ") + m_origin.ToString() + std::string(" & Direction: ") + m_direction.ToString());
			};

	};

}

#endif
