// /////////////////////////////////////////////////////////////////
// @file Plane.h
// @author PJ O Halloran
// @date 20/11/2010
//
// File contains the header for the Plane class.
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
//      (http://wolfgang-engel.info/blogs).
// - OpenGL Superbible, 5th Edition by Wright et al.
// - OpenGL Programming Guide (red), 7th Edition by Shreiner et al.
// - Bullet v2.76 Vector Math Library by Edwin Cousins
//      (http://www.bulletphysics.com).
// - Game Coding Complete, 3rd Edition by Mike McShaffry et al.
//
// For Plane math, I also referred to "Introduction to 3D Game Programming
// with DirectX 9.0, First Edition by Frank D. Luna".
//
// /////////////////////////////////////////////////////////////////

#ifndef __GF_PLANE_H
#define __GF_PLANE_H

// External Headers
#include <string>
#include <ostream>
#include <sstream>


// Project Headers
#include "CommonMath.h"
#include "Vector.h"
#include "Matrix.h"


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran {
    // /////////////////////////////////////////////////////////////////
    // @class Plane
    // @author PJ O Halloran
    //
    // This class represents a 3D Plane.  Useful for visibility culling
    // in a 3D scene, etc.
    //
    // /////////////////////////////////////////////////////////////////
    class Plane {
    private:

        Vector3 m_normal;                   ///< Unit normal of the plane.
        F32 m_d;                        ///< D component of the plane (d = -n (dot) P0).

        // /////////////////////////////////////////////////////////////////
        // Recalculate the D component.
        //
        // @param point A point lying on the plane.
        //
        // /////////////////////////////////////////////////////////////////
        inline void RecalculateD(const Point3 &point) {
            Vector3 negNormal(-m_normal);
            m_d = negNormal.Dot(Vector3(point));
        };

    public:

        // /////////////////////////////////////////////////////////////////
        // Default constructor cannot create a valid plane so we will
        // disallow it.
        //
        // /////////////////////////////////////////////////////////////////
        Plane() {};

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // Creates a plane from the unit normal of the plane plus a point
        // on the plane.
        //
        // @param normal The unit normal vector of the plane.
        // @param point A point on the same plane as the unit normal.
        //
        // /////////////////////////////////////////////////////////////////
        explicit Plane(const Vector3 &normal, const Point3 &point);

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // Givent three points in 3D space, calculates the 3D plane they all
        // lie upon.
        //
        // The points should be given in clockwise order or else the unit normal
        // of the plane will be facing in the opposite direction to what you
        // expect.
        //
        // @param pt0 The first point.
        // @param pt1 The second point.
        // @param pt2 The third point.
        //
        // /////////////////////////////////////////////////////////////////
        explicit Plane(const Point3 &pt0, const Point3 &pt1, const Point3 &pt2);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        ~Plane() { };

        // /////////////////////////////////////////////////////////////////
        // Get the d component of the Plane:
        // d = -n * Pt0 (where n is the normal of the plane a Pt0 is a point
        //                  on the plane.)
        //
        // /////////////////////////////////////////////////////////////////
        inline F32 GetD() const {
            return (m_d);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the unit normal of the plane.
        //
        // /////////////////////////////////////////////////////////////////
        inline void GetUnitNormal(Vector3 &normalVec) const {
            normalVec = m_normal;
        };

        // /////////////////////////////////////////////////////////////////
        // Assign the another plane to this Plane.
        //
        // @param rhs The Vector to assign to this Vector.
        //
        // @return Vector3 A copy of the resulting vector.
        //
        // /////////////////////////////////////////////////////////////////
        inline Plane &operator=(const Plane &rhs) {
            if(this == &rhs) {
                return (*this);
            }
            m_normal = rhs.m_normal;
            m_d = rhs.m_d;
            return (*this);
        };

        // /////////////////////////////////////////////////////////////////
        // Check if two Planes' are equivalent.
        //
        // @param rhs The Plane to check against this Plane.
        //
        // @return bool True if two Planes are equal and false otherwise.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool operator==(const Plane &rhs) const {
            return (m_normal == rhs.m_normal && FloatCmp(m_d, rhs.m_d));
        };

        // /////////////////////////////////////////////////////////////////
        // Check if two Planes' are not equivalent.
        //
        // @param rhs The Plane to check against this Plane.
        //
        // @return bool True if two Planes are not equal and false otherwise.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool operator!=(const Plane &rhs) const {
            return !(*this == rhs);
        };

        // /////////////////////////////////////////////////////////////////
        // Normalize this Plane.  Also recalculates the D component of the
        // Plane.
        //
        // /////////////////////////////////////////////////////////////////
        Plane &Normalize();

        // /////////////////////////////////////////////////////////////////
        // Calculate the distance from a point in 3D space to this plane.
        //
        // The sign of the result indicates what side of the plane the point
        // is on.  + means it is on the same side as the planes normal.
        // - means that it lies on the opposite side as the planes normal.
        // 0 means that it lies on the plane.
        //
        // @param pt The 3D point.
        //
        // @return F32 The distance from the plane to the point.
        //
        // /////////////////////////////////////////////////////////////////
        F32 Distance(const Point3 &pt) const;

        // /////////////////////////////////////////////////////////////////
        // Check if a point lies on or inside the plane.  The same side as
        // the planes normal.
        //
        // @param pt 3D point.
        //
        // @return bool True if the point is on or inside the Plane.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool Inside(const Point3 &pt) const {
            return (Distance(pt) >= 0.0f);
        };

        // /////////////////////////////////////////////////////////////////
        // Check if a sphere lies on or inside the plane.  The same side as
        // the planes normal.
        //
        // @param pt 3D centre point.
        // @param radius The spheres radius.
        //
        // @return bool True if the point is on or inside the Plane.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool Inside(const Point3 &pt, const F32 radius) const {
            return (Distance(pt) - radius >= 0.0f);
        };

        // /////////////////////////////////////////////////////////////////
        // Transform a Plane with a transformation matrix supplied.
        //
        // @param transformMatrix The transformation you wish to apply to the
        //                          plane.
        //
        // @return Plane& A reference to this Plane.
        //
        // /////////////////////////////////////////////////////////////////
        Plane &Transform(const Matrix4 &transformMatrix);

        // /////////////////////////////////////////////////////////////////
        // Transform a Plane with a transformation matrix supplied.  This
        // Plane is not affected.  Instead the transformed Plane is saved to
        // the "outPlane" parameter.
        //
        // @param transformMatrix The transformation you wish to apply to the
        //                          plane.
        // @param outPlane The Plane to store the transformed plane result in.
        //
        // /////////////////////////////////////////////////////////////////
        void Transformed(const Matrix4 &transformMatrix, Plane &outPlane) const;

        // /////////////////////////////////////////////////////////////////
        // Get the nearest point on the plane to a point in 3D space.
        //
        // @param pt The point in 3D space which we wish to determine the
        //              nearest point that lies on the plane to.
        // @param nearestPlanePt Point object which will hold the nearest point
        //                          on the plane to "pt".
        //
        // /////////////////////////////////////////////////////////////////
        void GetNearestPoint(const Point3 &pt, Point3 &nearestPlanePt) const;

        // /////////////////////////////////////////////////////////////////
        // Return a string of the Planes' components printed in General Form.
        //
        // @return std::string
        //
        // /////////////////////////////////////////////////////////////////
        inline std::string ToString() const {
            std::stringstream strStream;
            strStream << "[ A = " << m_normal.GetX() << ", B = " << m_normal.GetY() << ", C = " << m_normal.GetZ() << ", D = " << m_d << " ]";
            return (strStream.str());
        };

    };

}

#endif
