// Frame.h
// Implementation of the GLFrame Class
// Richard S. Wright Jr.
// Code by Richard S. Wright Jr.
/* Copyright (c) 2005-2009, Richard S. Wright Jr.
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
// @file GLFrame.h
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

#ifndef __GF_GL_FRAME
#define __GF_GL_FRAME

// External Headers


// Project Headers
#include <Matrix.h>
#include <Vector.h>


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // @class GLFrame
    // @author Richard S. Wright Jr. and PJ O Halloran.
    //
    // From GL superbible code:
    // "The GLFrame (OrthonormalFrame) class. Possibly the most useful
    // little piece of 3D graphics code for OpenGL immersive environments".
    //
    // A class that represents a frame of reference in 3D space for a
    // 3D object.  The 3D object could be anything from a camera to a
    // game actor.
    //
    // The main change made from the original source is that I wrote my
    // own 3D math library so I changed this class to use that instead of
    // the math library that came with it.  You could say the interface
    // is mostly the same.
    //
    // The implementation is actually much more straightforward and readable
    // in my opinion than the original implementation by Richard S. Wright Jr.
    // My Matrix and Vector classes are much more readable than the C
    // functions used before.
    //
    // /////////////////////////////////////////////////////////////////
    class GLFrame {
    private:

        Point3 m_position;              ///< Where am I?
        Vector3 m_forward;              ///< Where am I going?
        Vector3 m_up;                   ///< Which way is up?
        // NB: The right direction may be got from Up Cross Product Forward
        //  so theres no need to store it.

    public:

        // /////////////////////////////////////////////////////////////////
        // Default position and orientation. At the origin, looking
        // down the positive Z axis (right handed coordinate system).
        //
        // /////////////////////////////////////////////////////////////////
        explicit inline GLFrame(void) : m_position(g_originPt), m_forward(g_forward), m_up(g_up) { };

        // /////////////////////////////////////////////////////////////////
        // Define a frame of reference with your own custom coordinate system.
        //
        // NB
        // The default constructor is what should be used in most cases and we
        // will assume a RIGHT hand coordinate system everywhere so be careful
        // if you use this version.  For example, the rotation, scale, projection
        // matrices are all assuming a right hand system.
        //
        // @param pos The position of the frame of reference in world space.
        // @param up The vector pointing in the up direction.
        // @param forward The vector pointing in the forward direction.
        //
        // /////////////////////////////////////////////////////////////////
        explicit inline GLFrame(const Point3 &pos, const Vector3 &up, const Vector3 &forward)\
    :
        m_position(pos), m_forward(forward), m_up(up) { };

        // /////////////////////////////////////////////////////////////////
        // Non virtual destructor (so don't derive).
        //
        // /////////////////////////////////////////////////////////////////
        ~GLFrame() { };

        // /////////////////////////////////////////////////////////////////
        // Get and Set Location.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetPosition(const Point3 &vPoint) {
            m_position = vPoint;
        };
        inline Point3 GetPosition() const {
            return (m_position);
        };
        inline F32 GetPositionX(void) const {
            return (m_position.GetX());
        };
        inline F32 GetPositionY(void) const {
            return (m_position.GetY());
        };
        inline F32 GetPositionZ(void) const {
            return (m_position.GetZ());
        };

        // /////////////////////////////////////////////////////////////////
        // Get and Set Forward Direction.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetForwardVector(const Vector3 &vDirection) {
            m_forward = vDirection;
        };
        inline Vector3 GetForwardVector() const {
            return (m_forward);
        };

        // /////////////////////////////////////////////////////////////////
        // Get and Set Up Direction.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetUpVector(const Vector3 &vDirection) {
            m_up = vDirection;
        };
        inline Vector3 GetUpVector() const {
            return (m_up);
        };

        // /////////////////////////////////////////////////////////////////
        // Get Right Direction.  Right direction is not stored so it cannot
        // be explicitly set.  If you change the up or forward vectors then
        // this will automatically be changed.
        //
        // @param rightVecOut The Vector that will hold the right direction
        //                      after this call has completed.
        //
        // @return Vector3& A reference to the right vector.
        //
        // /////////////////////////////////////////////////////////////////
        inline Vector3 &GetRightVector(Vector3 &rightVecOut) const {
            m_forward.Cross(m_up, rightVecOut);
            return (rightVecOut);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the X, Y and Z axes.
        //
        // /////////////////////////////////////////////////////////////////
        inline Vector3 GetZAxis() const {
            return (m_forward);
        };
        inline Vector3 GetYAxis() const {
            return (m_up);
        };
        inline Vector3 GetXAxis() const {
            Vector3 xVec;
            return (GetRightVector(xVec));
        };

        // /////////////////////////////////////////////////////////////////
        // Translate along orthonormal axis in world space.
        //
        // @param x The number of unit along the x axis to move.
        // @param y The number of unit along the y axis to move.
        // @param z The number of unit along the z axis to move.
        //
        // /////////////////////////////////////////////////////////////////
        inline void TranslateWorld(const F32 x, const F32 y, const F32 z) {
            m_position += Vector3(x, y, z);
        };

        // /////////////////////////////////////////////////////////////////
        // Translate along orthonormal axis in local space.
        //
        // /////////////////////////////////////////////////////////////////
        inline void TranslateLocal(const F32 x, const F32 y, const F32 z) {
            MoveForward(z);
            MoveUp(y);
            MoveRight(x);
        };

        // /////////////////////////////////////////////////////////////////
        // Move Forward (by default along the Z axis).
        //
        // /////////////////////////////////////////////////////////////////
        inline void MoveForward(const F32 delta) {
            Vector3 mvForwardVec(m_forward);
            mvForwardVec *= delta;
            m_position += mvForwardVec;
        };

        // /////////////////////////////////////////////////////////////////
        // Move up (by default along the Y axis).
        //
        // /////////////////////////////////////////////////////////////////
        inline void MoveUp(const F32 delta) {
            Vector3 mvUpVec(m_up);
            mvUpVec *= delta;
            m_position += mvUpVec;
        };

        // /////////////////////////////////////////////////////////////////
        // Move right (by default, along the X axis).
        //
        // /////////////////////////////////////////////////////////////////
        inline void MoveRight(const F32 delta) {
            Vector3 rightVec;
            GetRightVector(rightVec);
            rightVec *= delta;
            m_position += rightVec;
        };

        // /////////////////////////////////////////////////////////////////
        // Create a Matrix representing the frame of reference.
        //
        // @param matrixOut The matrix to store the result in.
        // @param rotationOnly Should we encode the orientation of the frame
        //                      of reference only.  Set as true if the
        //                      frame of references translation has already
        //                      been applied or if you would not like the
        //                      translation to be applied.
        //
        // /////////////////////////////////////////////////////////////////
        void GetMatrix(Matrix4 &matrixOut, const bool rotationOnly = false);

        // /////////////////////////////////////////////////////////////////
        // Create a matrix representing the frame of reference if the GLFrame
        // is acting as the frame of reference for a camera.
        //
        // Basically, the frame of refernce for a camera transform is flipped
        // in the forward direction and the right direction (up is still up
        // in terms of both the camera and objects in the world).
        //
        // @param matrixOut The matrix to store the result in.
        // @param rotationOnly Should we encode the orientation of the frame
        //                      of reference only.  Set as true if the
        //                      frame of references translation has already
        //                      been applied or if you would not like the
        //                      translation to be applied.
        //
        // /////////////////////////////////////////////////////////////////
        void GetCameraMatrix(Matrix4 &matrixOut, const bool rotationOnly = false);

        // /////////////////////////////////////////////////////////////////
        // Rotate around local Y axis.
        //
        // @param fAngle The angle in degrees to rotate around the Y axis.
        //
        // /////////////////////////////////////////////////////////////////
        void RotateLocalY(const F32 angle);

        // /////////////////////////////////////////////////////////////////
        // Rotate around local Z axis.
        //
        // @param fAngle The angle in degrees to rotate around the Z axis.
        //
        // /////////////////////////////////////////////////////////////////
        void RotateLocalZ(const F32 angle);

        // /////////////////////////////////////////////////////////////////
        // Rotate around local X axis.
        //
        // @param fAngle The angle in degrees to rotate around the X axis.
        //
        // /////////////////////////////////////////////////////////////////
        void RotateLocalX(const F32 angle);

        // /////////////////////////////////////////////////////////////////
        // Reset axes to make sure they are orthonormal. This should be called
        // on occasion if the matrix is long-lived and frequently transformed.
        //
        // /////////////////////////////////////////////////////////////////
        void Normalize(void);

        // /////////////////////////////////////////////////////////////////
        // Rotate around a world axis.
        //
        // @param angle The angle in degrees to rotate the FoR by.
        // @param x The x component of the world axis.
        // @param y The y component of the world axis.
        // @param z The z component of the world axis.
        //
        // /////////////////////////////////////////////////////////////////
        void RotateWorld(const F32 angle, const F32 x, const F32 y, const F32 z);

        // /////////////////////////////////////////////////////////////////
        // Rotate around a local axis.
        //
        // @param angle The angle in degrees to rotate the FoR by.
        // @param x The x component of the local axis.
        // @param y The y component of the local axis.
        // @param z The z component of the local axis.
        //
        // /////////////////////////////////////////////////////////////////
        void RotateLocal(const F32 angle, const F32 x, const F32 y, const F32 z);

        // /////////////////////////////////////////////////////////////////
        // Convert Coordinate Systems.
        //
        // This is pretty much, do the transformation represented by the
        // rotation and position on the point.
        // Is it better to stick to the convention that the destination
        // always comes first, or use the conventions that "sounds" like
        // the function.
        //
        // @param vLocal The vector in local space.
        // @param vWorld The world space output vector.
        // @param rotationOnly Should we include the translation component
        //                      of the FoR.
        //
        // /////////////////////////////////////////////////////////////////
        void LocalToWorld(const Vector3 &vLocal, Vector3 &vWorld, const bool rotationOnly = false);

        // /////////////////////////////////////////////////////////////////
        // Change world coordinates into "local" coordinates
        //
        // /////////////////////////////////////////////////////////////////
        void WorldToLocal(const Vector3 &vWorld, Vector3 &vLocal);

        // /////////////////////////////////////////////////////////////////
        // Transform a point in 3D space by the frame of reference.
        //
        // @param srcPt The input point.
        // @param dstPt The output point transformed by the FoR.
        //
        // /////////////////////////////////////////////////////////////////
        void TransformPoint(const Point3 &srcPt, Point3 &dstPt);

        // /////////////////////////////////////////////////////////////////
        // Rotate a vector by the frame of reference.
        //
        // @param srcVec The input vector.
        // @param dstVec The output vector rotated by the FoR.
        //
        // /////////////////////////////////////////////////////////////////
        void RotateVector(const Vector3 &srcVec, Vector3 &dstVec);
    };

}

#endif
