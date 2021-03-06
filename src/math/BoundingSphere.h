#pragma once
#ifndef __GF_BOUNDING_SPHERE_H
#define __GF_BOUNDING_SPHERE_H

// /////////////////////////////////////////////////////////////////
// @file BoundingSphere.h
// @author PJ O Halloran
// @date 29/09/2010
//
// File contains the header for the BoundingSphere class.
//
// /////////////////////////////////////////////////////////////////

#include "Vector.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // @class BoundingSphere
    // @author PJ O Halloran
    //
    // A bounding 3D sphere to define a particular sphere shaped area in a
    // scene.  Useful for efficient collision detection, scene picking,
    // etc.  Not so accurate as not all 3D objects are exactly in the
    // shape of a sphere.
    //
    // /////////////////////////////////////////////////////////////////
    class BoundingSphere {
    private:

        Point3 m_centrePt;                      ///< The cubes minimum point.
        F32 m_radius;                           ///< The radius of the sphere.

    protected:


    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // /////////////////////////////////////////////////////////////////
        inline explicit BoundingSphere() : m_centrePt(), m_radius(0.0f) { };

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param min The centre point of the sphere.
        // @param max The radius of the sphere.
        //
        // /////////////////////////////////////////////////////////////////
        inline explicit BoundingSphere(const Point3 &centrePt, const F32 radius) : m_centrePt(centrePt), m_radius(radius) { };

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        ~BoundingSphere() { };

        // /////////////////////////////////////////////////////////////////
        // Get the centre point of the sphere.
        //
        // /////////////////////////////////////////////////////////////////
        inline Point3 GetCentre() const {
            return (m_centrePt);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the centre point of the sphere.
        //
        // @param centrePt The new centre point of the sphere.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetCentre(const Point3 &centrePt) {
            m_centrePt = centrePt;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the radius of the sphere
        //
        // /////////////////////////////////////////////////////////////////
        inline F32 GetRadius() const {
            return (m_radius);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the radius of the sphere.
        //
        // @param radius The new radius of the sphere.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetRadius(const F32 radius) {
            m_radius = radius;
        };

        // /////////////////////////////////////////////////////////////////
        // Check if a 3D point is inside the bounding sphere.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool IsPointInside(const Point3 &pt) const {
            const F32 dis = m_centrePt.Distance(pt);
            return (dis <= m_radius);
        };

        // /////////////////////////////////////////////////////////////////
        // Check if the sphere has valid contents.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool IsValid() const {
            return (m_radius > 0.0f);
        };

    };

}

#endif
