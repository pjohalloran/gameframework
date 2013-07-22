#pragma once
#ifndef __GF_FRUSTRUM_H
#define __GF_FRUSTRUM_H

// /////////////////////////////////////////////////////////////////
// @file Frustrum.h
// @author PJ O Halloran
// @date 26/08/2010
//
// File contains the header for the Frustrum class.
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
// very strong unit testing to verify it is correct on the other hand.
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
// /////////////////////////////////////////////////////////////////

#include "CommonMath.h"
#include "Vector.h"
#include "Matrix.h"
#include "BoundingSphere.h"
#include "Plane.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // @class Frustrum
    // @author Mike McShaffry (edited by PJ O Halloran).
    //
    // This class represents a projection or viewing Frustrum.
    //
    // Please note:
    // This class were lifted from the Frustrum class described in chapter
    // 14 3D Scenes from Games Coding Complete, Third Edition by Mike
    // McShaffry et al.
    //
    // I have changed it to use my math3d library and to use OpenGL for
    // rendering the frustrum in debug mode and also have changed
    // the functionality and interface slightly.
    //
    // /////////////////////////////////////////////////////////////////
    class Frustrum {

    public:

        // /////////////////////////////////////////////////////////////////
        // @enum Side
        //
        // The ID/index for a plane around the Frustrum.
        //
        // /////////////////////////////////////////////////////////////////
        enum Side {
            Near,
            Far,
            Top,
            Right,
            Bottom,
            Left,
            NumPlanes
        };

    private:

        Plane m_Planes[NumPlanes];      // planes of the frustum in camera space
        Point3 m_NearClip[4];           // verts of the near clip plane in camera space
        Point3 m_FarClip[4];            // verts of the far clip plane in camera space
        F32 m_Fov;                      // field of view in radians
        F32 m_Aspect;                   // aspect ratio - width divided by height
        F32 m_Near;                     // near clipping distance
        F32 m_Far;                      // far clipping distance

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // /////////////////////////////////////////////////////////////////
        explicit Frustrum();

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        ~Frustrum() { };

        // /////////////////////////////////////////////////////////////////
        // Check if a 3D point is inside the Frustrum.
        //
        // The 3D point should be transformed to eye/camera space first.
        //
        // @param pt The point to check.
        //
        // @return bool True if the point is inside the frustrum and false
        //              otherwise.
        //
        // /////////////////////////////////////////////////////////////////
        bool Inside(const Point3 &pt) const;

        // /////////////////////////////////////////////////////////////////
        // Check if a BoundingSphere is inside the Frustrum.
        //
        // The position of the sphere should be transformed to eye/camera
        // space first.
        //
        // @param pt The centre point of the sphere.
        //
        // @return bool True if the BoundingSphere is inside the frustrum and
        //              false otherwise.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool Inside(const BoundingSphere &bs) const {
            return (Inside(bs.GetCentre(), bs.GetRadius()));
        };

        // /////////////////////////////////////////////////////////////////
        // Check if a sphere is inside the Frustrum.
        //
        // The location of the sphere should be transformed to eye/camera
        // space first.
        //
        // @param pt The centre point of the sphere.
        // @param radius The radius of the sphere.
        //
        // @return bool True if the sphere is inside the frustrum and false
        //              otherwise.
        //
        // /////////////////////////////////////////////////////////////////
        bool Inside(const Point3 &pt, const F32 radius) const;

        // /////////////////////////////////////////////////////////////////
        // Get a plane belonging to the Frustrum.
        //
        // /////////////////////////////////////////////////////////////////
        inline const Plane &Get(const Side side) {
            return m_Planes[side];
        };

        // /////////////////////////////////////////////////////////////////
        // Set a new field of view for the frustrum and reinitialize it.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetFOV(const F32 fov) {
            Init(fov, m_Aspect, m_Near, m_Far);
        };

        // /////////////////////////////////////////////////////////////////
        // Set a new aspect ratios for the frustrum and reinitialize it.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetAspect(const F32 aspect) {
            Init(m_Fov, aspect, m_Near, m_Far);
        }

        // /////////////////////////////////////////////////////////////////
        // Set a new near clipping distance for the frustrum and reinitialize it.
        //
        // /////////////////////////////////////////////////////////////////
        void SetNear(const F32 nearClip) {
            Init(m_Fov, m_Aspect, nearClip, m_Far);
        }

        // /////////////////////////////////////////////////////////////////
        // Set a new far clipping distance for the frustrum and reinitialize it.
        //
        // /////////////////////////////////////////////////////////////////
        void SetFar(const F32 farClip) {
            Init(m_Fov, m_Aspect, m_Near, farClip);
        }

        // /////////////////////////////////////////////////////////////////
        // Initialize the frustrum.
        //
        // @param fov The field of view (in degrees).
        // @param aspect The aspect ratio of the display.
        // @param near The near clipping distance.
        // @param far The far clipping distance.
        //
        // /////////////////////////////////////////////////////////////////
        void Init(const F32 fov, const F32 aspect, const F32 near, const F32 far);

        // /////////////////////////////////////////////////////////////////
        // Render the sides of the frustrum.
        //
        // Please note that this method assumes a shader has been activated
        // and set up and that the GL state is set appropriately to render.
        //
        // /////////////////////////////////////////////////////////////////
        void Render();
    };

}

#endif
