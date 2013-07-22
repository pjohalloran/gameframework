#pragma once
#ifndef __GF_BOUNDING_CUBE_H
#define __GF_BOUNDING_CUBE_H

// /////////////////////////////////////////////////////////////////
// @file BoundingCube.h
// @author PJ O Halloran
// @date 29/09/2010
//
// File contains the header for the BoundingCube class.
//
// /////////////////////////////////////////////////////////////////

#include "Vector.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // @class BoundingCube
    // @author PJ O Halloran
    //
    // A bounding 3D box to define a particular cube shaped area in a
    // scene.  Useful for efficient collision detection, scene picking,
    // etc.  Not so accurate as not all 3D objects are exactly in the
    // shape of a cube.
    //
    // /////////////////////////////////////////////////////////////////
    class BoundingCube {
    private:

        Point3 m_min;                       ///< The cubes minimum point.
        Point3 m_max;                       ///< The cubes maximum point.

    protected:


    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // /////////////////////////////////////////////////////////////////
        inline explicit BoundingCube() : m_min(), m_max() { };

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param min The minimum point of the cube.
        // @param max The maximum point of a cube.
        //
        // /////////////////////////////////////////////////////////////////
        inline explicit BoundingCube(const Point3 &min, const Point3 &max) : m_min(min), m_max(max) { };

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        ~BoundingCube() { };

        // /////////////////////////////////////////////////////////////////
        // Get the minimum position of the cube.
        //
        // /////////////////////////////////////////////////////////////////
        inline Point3 GetMin() const {
            return (m_min);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the minimum position of the cube.
        //
        // @param min The new minimum position of the cube.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetMin(const Point3 &min) {
            m_min = min;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the maximum position of the cube.
        //
        // /////////////////////////////////////////////////////////////////
        inline Point3 GetMax() const {
            return (m_max);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the maximum position of the cube.
        //
        // @param min The new maximum position of the cube.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetMax(const Point3 &max) {
            m_max = max;
        };

        // /////////////////////////////////////////////////////////////////
        // Check if a 3D point is inside the bounding cube.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool IsPointInside(const Point3 &pt) const {
            const F32 x = pt.GetX();
            const F32 y = pt.GetY();
            const F32 z = pt.GetZ();
            return (x >= m_min.GetX() && \
                    y >= m_min.GetY() && \
                    z >= m_min.GetZ() && \
                    x <= m_max.GetX() && \
                    y <= m_max.GetY() && \
                    z <= m_max.GetZ());
        };

        // /////////////////////////////////////////////////////////////////
        // Get Width.
        //
        // /////////////////////////////////////////////////////////////////
        inline F32 GetWidth() const {
            return (m_max.GetX() - m_min.GetX());
        };

        // /////////////////////////////////////////////////////////////////
        // Get Height.
        //
        // /////////////////////////////////////////////////////////////////
        inline F32 GetHeight() const {
            return (m_max.GetY() - m_min.GetY());
        };

        // /////////////////////////////////////////////////////////////////
        // Get Depth.
        //
        // /////////////////////////////////////////////////////////////////
        inline F32 GetDepth() const {
            return (m_max.GetZ() - m_min.GetZ());
        };

        // /////////////////////////////////////////////////////////////////
        // Test if the bounding cube has had a valid min and max point set.
        // It is valid if the minimum point is less than or equal to the
        // maximum point.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool IsValid() const {
            return ((m_min.GetX() <= m_max.GetX()) && (m_min.GetY() <= m_max.GetY()) && (m_min.GetZ() <= m_max.GetZ()));
        };

        // /////////////////////////////////////////////////////////////////
        // Calculate the area of front (and back) face of the bounding cube.
        //
        // /////////////////////////////////////////////////////////////////
        inline F32 FaceArea() const {
            return (GetWidth() * GetHeight());
        };

        // /////////////////////////////////////////////////////////////////
        // Calculate the area of the bounding cube.
        //
        // /////////////////////////////////////////////////////////////////
        inline F32 Area() const {
            return (FaceArea() * GetDepth());
        };

    };

}

#endif
