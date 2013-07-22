#pragma once
#ifndef __GF_MATRIX_H
#define __GF_MATRIX_H

// /////////////////////////////////////////////////////////////////
// @file Matrix.h
// @author PJ O Halloran
// @date 25/08/2010
//
// File contains the header for the Matrix4 class.
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

#include <string>
#include <ostream>
#include <sstream>

#include "GameTypes.h"
#include "CommonMath.h"
#include "Vector.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // @class Matrix4
    // @author PJ O Halloran
    //
    // This class represents a 4x4 Matrix stored in column major order.
    //
    // e.g. The matrix, M:
    //  |m00    m10     m20     m30|
    //  |m01    m11     m21     m31|
    //  |m02    m12     m22     m32|
    //  |m03    m13     m23     m33|
    //
    // is stored in a continuous area of memory as follows:
    //  index   element
    //  0       m00
    //  1       m01
    //  2       m02
    //  3       m03
    //  4       m10
    //  5       m11
    //  6       m12
    //  7       m13
    //  8       m20
    //  9       m21
    //  10      m22
    //  11      m23
    //  12      m30
    //  13      m31
    //  14      m32
    //  15      m33
    //
    // To get the matrix in row major order for libraries other than OpenGL
    // then you must get the transpose of the matrix.
    //
    // /////////////////////////////////////////////////////////////////
    class Matrix4 {
    public:

        // /////////////////////////////////////////////////////////////////
        // @enum INDEX
        //
        // The index where a particular element may be found in the 1D array.
        //
        // Each entry has the naming convention Mcr where c is the index of
        // the column and r is the index of the row (e.g. M00 refers to the
        // element in the first column and first row).
        //
        // The NUMBER_ELEMENTS entry is the size of the array. Any index
        // greater than NUMBER_ELEMENTS-1 will cause a runtime memory index
        // access error.
        //
        // /////////////////////////////////////////////////////////////////
        enum INDEX {
            M00 = 0,
            M01,
            M02,
            M03,
            M10,
            M11,
            M12,
            M13,
            M20,
            M21,
            M22,
            M23,
            M30,
            M31,
            M32,
            M33,
            NUMBER_ELEMENTS
        };

        // /////////////////////////////////////////////////////////////////
        // @enum COLUMN_INDEX
        //
        // The index where a particular column may be found.
        //
        // /////////////////////////////////////////////////////////////////
        enum COLUMN_INDEX {
            C0 = 0,
            C1,
            C2,
            C3,
            NUMBER_COLS
        };

        // /////////////////////////////////////////////////////////////////
        // @enum ROW_INDEX
        //
        // The index where a particular row may be found.
        //
        // /////////////////////////////////////////////////////////////////
        enum ROW_INDEX {
            R0 = 0,
            R1,
            R2,
            R3,
            NUMBER_ROWS
        };

    private:

        F32 m_mat[NUMBER_ELEMENTS];                                 ///< Array of matrix elements (column major format).

        // /////////////////////////////////////////////////////////////////
        // Get the four 4D column vectors of the matrix.
        //
        // @param colOne First column of the matrix.
        // @param colTwo Second column of the matrix.
        // @param colThree Third column of the matrix.
        // @param colFour Fourth column of the matrix.
        //
        // /////////////////////////////////////////////////////////////////
        inline void GetColumnVectors(Vector4 &colOne, Vector4 &colTwo, Vector4 &colThree, Vector4 &colFour) const {
            colOne.SetX(m_mat[M00]);
            colOne.SetY(m_mat[M01]);
            colOne.SetZ(m_mat[M02]);
            colOne.SetW(m_mat[M03]);
            colTwo.SetX(m_mat[M10]);
            colTwo.SetY(m_mat[M11]);
            colTwo.SetZ(m_mat[M12]);
            colTwo.SetW(m_mat[M13]);
            colThree.SetX(m_mat[M20]);
            colThree.SetY(m_mat[M21]);
            colThree.SetZ(m_mat[M22]);
            colThree.SetW(m_mat[M23]);
            colFour.SetX(m_mat[M30]);
            colFour.SetY(m_mat[M31]);
            colFour.SetZ(m_mat[M32]);
            colFour.SetW(m_mat[M33]);
        }

        // /////////////////////////////////////////////////////////////////
        // Get the four 4D row vectors of the matrix.
        //
        // @param rowOne First row of the matrix.
        // @param rowTwo Second row of the matrix.
        // @param rowThree Third row of the matrix.
        // @param rowFour Fourth row of the matrix.
        //
        // /////////////////////////////////////////////////////////////////
        inline void GetRowVectors(Vector4 &rowOne, Vector4 &rowTwo, Vector4 &rowThree, Vector4 &rowFour) const {
            rowOne.SetX(m_mat[M00]);
            rowOne.SetY(m_mat[M10]);
            rowOne.SetZ(m_mat[M20]);
            rowOne.SetW(m_mat[M30]);
            rowTwo.SetX(m_mat[M01]);
            rowTwo.SetY(m_mat[M11]);
            rowTwo.SetZ(m_mat[M21]);
            rowTwo.SetW(m_mat[M31]);
            rowThree.SetX(m_mat[M02]);
            rowThree.SetY(m_mat[M12]);
            rowThree.SetZ(m_mat[M22]);
            rowThree.SetW(m_mat[M32]);
            rowFour.SetX(m_mat[M03]);
            rowFour.SetY(m_mat[M13]);
            rowFour.SetZ(m_mat[M23]);
            rowFour.SetW(m_mat[M33]);
        };

        // /////////////////////////////////////////////////////////////////
        // Get a component of the determinant (Determinant helper function).
        //
        // N.B.
        // This method was copied from the m3dInvertMatrix44 method from the
        // OpenGL Superbible, 5th edition as I had some trouble understanding
        // the linear algebra involved in calculating a Determinant for a 4x4
        // matrix.
        //
        // /////////////////////////////////////////////////////////////////
        F32 Det3x3(const I32 i, const I32 j) const;

    public:

        // /////////////////////////////////////////////////////////////////
        // Default constructor.
        //
        // /////////////////////////////////////////////////////////////////
        explicit Matrix4();

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param scaler The value to set all elements of the matrix to.
        //
        // /////////////////////////////////////////////////////////////////
        explicit Matrix4(const F32 scaler);

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param matrixArr Array of elements to assign to this matrix.
        //
        // /////////////////////////////////////////////////////////////////
        explicit Matrix4(F32 matrixArr[NUMBER_ELEMENTS]);

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // Sets all elements to custom values.
        //
        // @param mab The element to set where a is the column index and
        //                  b is the row index...
        //
        // /////////////////////////////////////////////////////////////////
        explicit Matrix4(const F32 m00, const F32 m10, const F32 m20, const F32 m30, \
                         const F32 m01, const F32 m11, const F32 m21, const F32 m31, \
                         const F32 m02, const F32 m12, const F32 m22, const F32 m32, \
                         const F32 m03, const F32 m13, const F32 m23, const F32 m33);

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // Sets the elements according to the x, y and z direction vectors
        // and the translation/location point.
        //
        // @param xDir Orientation of the x axis with respect to the eye
        //              coordinate system.
        // @param yDir Orientation of the y axis with respect to the eye
        //              coordinate system.
        // @param zDir Orientation of the z axis with respect to the eye
        //              coordinate system.
        // @param location The position with respect to the eye
        //              coordinate system.
        //
        // /////////////////////////////////////////////////////////////////
        explicit Matrix4(const Vector3 &xDir, const Vector3 &yDir, const Vector3 &zDir, const Point3 &location);

        // /////////////////////////////////////////////////////////////////
        // Copy constructor.
        //
        // /////////////////////////////////////////////////////////////////
        Matrix4(const Matrix4 &matRef)/* { Set(matRef.m_mat); }*/;

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        ~Matrix4();

        // /////////////////////////////////////////////////////////////////
        // Load the identity matrix.  This matrix is analogous to the number
        // 1 for matrices.
        //
        // /////////////////////////////////////////////////////////////////
        void LoadIdentity();

        // /////////////////////////////////////////////////////////////////
        // Fill the matrix with zeros.  This matrix is anologous to the number
        // 0 for matrices.
        //
        // /////////////////////////////////////////////////////////////////
        void ZeroMatrix();

        // /////////////////////////////////////////////////////////////////
        // Edit the matrix to be its transpose.
        //
        // /////////////////////////////////////////////////////////////////
        void Transpose();

        // /////////////////////////////////////////////////////////////////
        // Return the transpose of this matrix.
        //
        // /////////////////////////////////////////////////////////////////
        void Transposed(Matrix4 &outMatrix) const;

        // /////////////////////////////////////////////////////////////////
        // Return the determinant of this matrix.
        //
        // This function uses Laplaces' expansion to calculate the determinant
        // of a 4x4 matrix by getting the determinants of a series of 3x3 and
        // then 2x2 matrices inside the 4x4 matrix (see Fundamentals of Computer
        // Graphics by Peter Shirley et al, Third Edition, Chapter 5
        // Linear Algebra, pg 98-100).
        //
        // N.B.
        // This method was copied from the m3dInvertMatrix44 method from the
        // OpenGL Superbible, 5th edition as I had some trouble understanding
        // the linear algebra involved.
        //
        // /////////////////////////////////////////////////////////////////
        F32 Determinant() const;

        // /////////////////////////////////////////////////////////////////
        // Edit this matrix to be its own inverse.
        //
        // This method uses Laplaces expansion to calculate the cofactors
        // of a series of 3x3 matrices as well as the determinant.
        //
        // N.B.
        // This method was copied from the m3dInvertMatrix44 method from the
        // OpenGL Superbible, 5th edition as I had some trouble understanding
        // the linear algebra involved.
        //
        // @return bool Some matrices do not have an inverse. A simple way of
        //              detemining this is that the matrix has an inverse if
        //              its determinant is not equal to 0.  As a result we will
        //              return true if the matrix has an inverse and false if
        //              not.
        //
        // /////////////////////////////////////////////////////////////////
        bool Inverse();

        // /////////////////////////////////////////////////////////////////
        // Return the inverse of this matrix.
        //
        // This method uses Laplaces expansion to calculate the cofactors
        // of a series of 3x3 matrices as well as the determinant.
        //
        // N.B.
        // This method was copied from the m3dInvertMatrix44 method from the
        // OpenGL Superbible, 5th edition as I had some trouble understanding
        // the linear algebra involved.
        //
        // @return bool Some matrices do not have an inverse. A simple way of
        //              detemining this is that the matrix has an inverse if
        //              its determinant is not equal to 0.  As a result we will
        //              return true if the matrix has an inverse and false if
        //              not.
        //
        // /////////////////////////////////////////////////////////////////
        bool Inversed(Matrix4 &outMatrix) const;

        // /////////////////////////////////////////////////////////////////
        // Edit this matrix to be its own inverse.
        //
        // This method uses Cramers rule to get the inverse.
        //
        // N.B.
        // This method was lifted from a 1999 paper released by Intel
        // titled "Streaming SIMD Extensions - Inverse of a 4*4 matrix".
        //
        // @return bool Some matrices do not have an inverse. A simple way of
        //              detemining this is that the matrix has an inverse if
        //              its determinant is not equal to 0.  As a result we will
        //              return true if the matrix has an inverse and false if
        //              not.
        //
        // /////////////////////////////////////////////////////////////////
        bool InverseCramer();

        // /////////////////////////////////////////////////////////////////
        // Return the inverse of this matrix.
        //
        // This method uses Cramers rule to get the inverse.
        //
        // N.B.
        // This method was lifted from a 1999 paper released by Intel
        // titled "Streaming SIMD Extensions - Inverse of a 4*4 matrix".
        //
        // @return bool Some matrices do not have an inverse. A simple way of
        //              detemining this is that the matrix has an inverse if
        //              its determinant is not equal to 0.  As a result we will
        //              return true if the matrix has an inverse and false if
        //              not.
        //
        // /////////////////////////////////////////////////////////////////
        bool InversedCramer(Matrix4 &outMatrix) const;

        // /////////////////////////////////////////////////////////////////
        // Multiply this matrix by another matrix and return a new copy.
        //
        // /////////////////////////////////////////////////////////////////
        Matrix4 operator*(const Matrix4 &rhs) const;

        // /////////////////////////////////////////////////////////////////
        // Multiply this matrix by another matrix and store the result in this
        // matrix.
        //
        // /////////////////////////////////////////////////////////////////
        Matrix4 &operator*=(const Matrix4 &rhs);

        // /////////////////////////////////////////////////////////////////
        // Multiply this matrix by a vector.
        //
        // @return Vector4 The translated 4D/homogenous vector.
        //
        // /////////////////////////////////////////////////////////////////
        Vector4 operator*(const Vector4 &rhs) const;

        // /////////////////////////////////////////////////////////////////
        // Assign a matrix to this matrix.
        //
        // /////////////////////////////////////////////////////////////////
        Matrix4 &operator=(const Matrix4 &rhs);

        // /////////////////////////////////////////////////////////////////
        // Get the element value at the matrix index.
        //
        // Index element access safe method.
        //
        // @param ci The column index.
        // @param ri The row index.
        // @param valueRef The value of the element if the call was successfull.
        //
        // @return bool True if the indices were inside the range and false if
        //                  not.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool GetElement(const U32 ci, const U32 ri, F32 &valueRef) const {
            const U32 index = ci * NUMBER_ROWS + ri;
            if(index >= NUMBER_ELEMENTS) {
                // Index access violation. No exceptions allowed for performance!
                return (false);
            }
            valueRef = m_mat[index];
            return (true);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the element value at the matrix index.
        //
        // Index element access safe method.
        //
        // @param ci The column index.
        // @param ri The row index.
        // @param scaler The new element value.
        //
        // @return bool True if the indices were inside the range and false if
        //                  not.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool SetElement(const U32 ci, const U32 ri, const F32 scaler) {
            const U32 index = ci * NUMBER_ROWS + ri;
            if(index >= NUMBER_ELEMENTS) {
                // Index access violation. No exceptions allowed for performance!
                return (false);
            }
            m_mat[index] = scaler;
            return (true);
        };

        // /////////////////////////////////////////////////////////////////
        // Array access method for getting and setting element values.
        //
        // This method is the same as accessing a C array. You have to take
        // care that the index passed in is not out of bounds.  The results
        // are undefined in this case.
        //
        // @param index The matrix index (NB use matrix index subscripts from
        //                  the INDEX enum for safer access!!).
        //
        // @return F32& A reference to the matrix element that you may
        //                      read from or write to.
        //
        // /////////////////////////////////////////////////////////////////
        inline F32 &operator [](const I32 index) {
            return (m_mat[index]);
        };

        // /////////////////////////////////////////////////////////////////
        // Print out the matrix into a string.
        //
        // /////////////////////////////////////////////////////////////////
        inline std::string ToString() const {
            std::stringstream conv;
            conv << "|\t" << m_mat[M00] << "\t" << m_mat[M10] << "\t" << m_mat[M20] << "\t" << m_mat[M30] << "\t|\n";
            conv << "|\t" << m_mat[M01] << "\t" << m_mat[M11] << "\t" << m_mat[M21] << "\t" << m_mat[M31] << "\t|\n";
            conv << "|\t" << m_mat[M02] << "\t" << m_mat[M12] << "\t" << m_mat[M22] << "\t" << m_mat[M32] << "\t|\n";
            conv << "|\t" << m_mat[M03] << "\t" << m_mat[M13] << "\t" << m_mat[M23] << "\t" << m_mat[M33] << "\t|";
            return (conv.str());
        };

        // /////////////////////////////////////////////////////////////////
        // Get a read only pointer to the array of components of the object.
        //
        // This method returns an internal pointer in this class which is bad
        // C++ programming.  I made a decision to do this to make it easier
        // to pass data to the GPU.  Otherwise i would have to perform copies
        // of the data to an array. This would involve a copy operation and lots
        // of new[] or new calls to allocate memory.  Memory fragmentation will
        // occur very quickly if we do this for every single vertex and vector.
        //
        // /////////////////////////////////////////////////////////////////
        inline const F32 * const GetComponentsConst() const {
            return (m_mat);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the position encoded in the matrix.
        //
        // @param pos A 4 component vector holding the homogenous coordinates.
        //
        // /////////////////////////////////////////////////////////////////
        inline void GetPosition(Vector4 &pos) const {
            pos.SetX(m_mat[M30]);
            pos.SetY(m_mat[M31]);
            pos.SetZ(m_mat[M32]);
            pos.SetW(m_mat[M33]);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the matrix position column.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetPosition(const Vector4 &pos) {
            m_mat[M30] = pos.GetX();
            m_mat[M31] = pos.GetY();
            m_mat[M32] = pos.GetZ();
            m_mat[M33] = pos.GetW();
        };

        // /////////////////////////////////////////////////////////////////
        // Get the position encoded in the matrix.
        //
        // @param pos A 3 component point to store the real coordinates.
        //
        // /////////////////////////////////////////////////////////////////
        inline void GetPosition(Point3 &pos) const {
            Vector4 pos4;
            GetPosition(pos4);
            if(!FloatCmp(pos4.GetW(), 0.0f)) {
                pos.SetX(pos4.GetX() / pos4.GetW());
                pos.SetY(pos4.GetY() / pos4.GetW());
                pos.SetZ(pos4.GetZ() / pos4.GetW());
            } else {
                pos.SetX(pos4.GetX());
                pos.SetY(pos4.GetY());
                pos.SetZ(pos4.GetZ());
            }
        };

        // /////////////////////////////////////////////////////////////////
        // Set the matrix position column.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetPosition(const Point3 &pos) {
            m_mat[M30] = pos.GetX();
            m_mat[M31] = pos.GetY();
            m_mat[M32] = pos.GetZ();
            m_mat[M33] = 1.0f;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the X direction vector encoded in the matrix.
        //
        // @param xDir The 4 component vector to store the x direction.
        //
        // /////////////////////////////////////////////////////////////////
        inline void GetXDir(Vector4 &xDir) const {
            xDir.SetX(m_mat[M00]);
            xDir.SetY(m_mat[M01]);
            xDir.SetZ(m_mat[M02]);
            xDir.SetW(m_mat[M03]);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the matrix X direction column.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetXDir(const Vector4 &xDir) {
            m_mat[M00] = xDir.GetX();
            m_mat[M01] = xDir.GetY();
            m_mat[M02] = xDir.GetZ();
            m_mat[M03] = xDir.GetW();
        };

        // /////////////////////////////////////////////////////////////////
        // Get the Y direction vector encoded in the matrix.
        //
        // @param yDir The 4 component vector to store the y direction.
        //
        // /////////////////////////////////////////////////////////////////
        inline void GetYDir(Vector4 &yDir) const {
            yDir.SetX(m_mat[M10]);
            yDir.SetY(m_mat[M11]);
            yDir.SetZ(m_mat[M12]);
            yDir.SetW(m_mat[M13]);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the matrix Y direction column.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetYDir(const Vector4 &yDir) {
            m_mat[M10] = yDir.GetX();
            m_mat[M11] = yDir.GetY();
            m_mat[M12] = yDir.GetZ();
            m_mat[M13] = yDir.GetW();
        };

        // /////////////////////////////////////////////////////////////////
        // Get the Z direction vector encoded in the matrix.
        //
        // @param zDir The 4 component vector to store the z direction.
        //
        // /////////////////////////////////////////////////////////////////
        inline void GetZDir(Vector4 &zDir) const {
            zDir.SetX(m_mat[M20]);
            zDir.SetY(m_mat[M21]);
            zDir.SetZ(m_mat[M22]);
            zDir.SetW(m_mat[M23]);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the matrix Z direction column.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetZDir(const Vector4 &zDir) {
            m_mat[M20] = zDir.GetX();
            m_mat[M21] = zDir.GetY();
            m_mat[M22] = zDir.GetZ();
            m_mat[M23] = zDir.GetW();
        };

        // /////////////////////////////////////////////////////////////////
        // Set the matrix to have the values passed in.
        // It is your responsibility to ensure the array of elements is the
        // correct size (16) and that it has its elements stored in column
        // major format.
        //
        // @param elementsArr An array of 16 elements in column major format.
        //
        // /////////////////////////////////////////////////////////////////
        inline void Set(const F32 *elementsArr) {
            memcpy(m_mat, elementsArr, sizeof(F32) * NUMBER_ELEMENTS);
        };

        // /////////////////////////////////////////////////////////////////
        // Check if two matrices are equivalent.  They are equivalent if all
        // their corresponding elements are equal.
        //
        // @param rhs The other Matrix.
        //
        // @return bool True if they are equal and false otherwise.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool operator==(const Matrix4 &rhs) const {
            // Optimization for when you compare a matrix to itself.
            if(this == &rhs) {
                return (true);
            }

            for(U32 i = 0; i < NUMBER_ELEMENTS; ++i) {
                if(!FloatCmp(m_mat[i], rhs.m_mat[i])) {
                    return (false);
                }
            }
            return (true);
        };

        // /////////////////////////////////////////////////////////////////
        // Check if two matrices are NOT equivalent.  They are not equivalent
        // if any of their corresponding elements are not equal.
        //
        // @param rhs The other Matrix.
        //
        // @return bool True if they are not equal and false otherwise.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool operator !=(const Matrix4 &rhs) const {
            return (!(*this == rhs));
        };

    };

    // Commonly used matrices (representing real numbers 1 and 0).
    extern const Matrix4 g_identityMat;
    extern const Matrix4 g_zeroMat;

    // /////////////////////////////////////////////////////////////////
    // Build a rotation MOVE matrix.
    //
    // This may be used to rotate about an arbitrary axis.
    //
    // It may also be used to rotate about the x, y or z axes by
    // specifying the input vector properly.
    // e.g. To rotate 90 degrees about the x axis, call:
    //  - BuildRotationMatrix4(mat, 90, 1.0, 0.0, 0.0).
    //
    // Please note that this method assumes a RIGHT hand coordinate
    // system.
    //
    // @param outMatrix The matrix to store the result in.
    // @param angle The angle in degrees to rotate by.
    // @param x The x axis to rotate around.
    // @param y The y axis to rotate around.
    // @param z The z axis to rotate around.
    //
    // @return Matrix4& The output matrix.
    //
    // /////////////////////////////////////////////////////////////////
    Matrix4 &BuildRotationArbitraryMatrix4(Matrix4 &outMatrix, const F32 angle, const F32 x, const F32 y, const F32 z);

    // /////////////////////////////////////////////////////////////////
    // Build a rotation MOVE matrix about the X axis.
    //
    // Runs faster than calling the arbituary version with an X unit
    // vector as only the absolutely required calculations to determine
    // the rotation matrix are performed.  The XYZ rotation matrices are
    // well known.
    //
    // Please note that this method assumes a RIGHT hand coordinate
    // system.
    //
    // @param outMatrix The matrix to store the result in.
    // @param angle The angle in degrees to rotate by.
    //
    // @return Matrix4& The output matrix.
    //
    // /////////////////////////////////////////////////////////////////
    Matrix4 &BuildRotationXMatrix4(Matrix4 &outMatrix, const F32 angle);

    // /////////////////////////////////////////////////////////////////
    // Build a rotation MOVE matrix about the Y axis.
    //
    // Runs faster than calling the arbituary version with an X unit
    // vector as only the absolutely required calculations to determine
    // the rotation matrix are performed.  The XYZ rotation matrices are
    // well known.
    //
    // Please note that this method assumes a RIGHT hand coordinate
    // system.
    //
    // @param outMatrix The matrix to store the result in.
    // @param angle The angle in degrees to rotate by.
    //
    // @return Matrix4& The output matrix.
    //
    // /////////////////////////////////////////////////////////////////
    Matrix4 &BuildRotationYMatrix4(Matrix4 &outMatrix, const F32 angle);

    // /////////////////////////////////////////////////////////////////
    // Build a rotation MOVE matrix about the Z axis.
    //
    // Runs faster than calling the arbituary version with an X unit
    // vector as only the absolutely required calculations to determine
    // the rotation matrix are performed.  The XYZ rotation matrices are
    // well known.
    //
    // Please note that this method assumes a RIGHT hand coordinate
    // system.
    //
    // @param outMatrix The matrix to store the result in.
    // @param angle The angle in degrees to rotate by.
    //
    // @return Matrix4& The output matrix.
    //
    // /////////////////////////////////////////////////////////////////
    Matrix4 &BuildRotationZMatrix4(Matrix4 &outMatrix, const F32 angle);

    // /////////////////////////////////////////////////////////////////
    // Build a translation MOVE matrix.
    //
    // @param outMatrix The matrix to store the result in.
    // @param x The new position on the x axis.
    // @param y The new position on the y axis.
    // @param z The new position on the z axis.
    //
    // @return Matrix4& The output matrix.
    //
    // /////////////////////////////////////////////////////////////////
    Matrix4 &BuildTranslationMatrix4(Matrix4 &outMatrix, const F32 x, const F32 y, const F32 z);

    // /////////////////////////////////////////////////////////////////
    // Build a scale MOVE matrix.
    //
    // The scale is performed with respect to the origin.
    //
    // @param outMatrix The matrix to store the result in.
    // @param x The value to strech/shrink along the x axis.
    // @param y The value to strech/shrink along the y axis.
    // @param z The value to strech/shrink along the z axis.
    //
    // @return Matrix4& The output matrix.
    //
    // /////////////////////////////////////////////////////////////////
    Matrix4 &BuildScaleMatrix4(Matrix4 &outMatrix, const F32 x, const F32 y, const F32 z);

    // /////////////////////////////////////////////////////////////////
    // Build a scale MOVE matrix.
    //
    // The scale is performed with respect to some arbitrary point.
    //
    // @param outMatrix The matrix to store the result in.
    // @param x The value to strech/shrink along the x axis.
    // @param y The value to strech/shrink along the y axis.
    // @param z The value to strech/shrink along the z axis.
    // @param pt The point in 3D space with which the scale operation
    //              should be relative to.
    //
    // @return Matrix4& The output matrix.
    //
    // /////////////////////////////////////////////////////////////////
    Matrix4 &BuildScaleArbitraryMatrix4(Matrix4 &outMatrix, const F32 x, const F32 y, const F32 z, const Point3 &pt);

    // /////////////////////////////////////////////////////////////////
    // Build a look at VIEW matrix.
    //
    // This look at VIEW matrix assumes a right-hand coordinate
    // system.  That is the positive X axis to the right, the positive Y
    // axis going up and the positive Z axis coming towards the screen
    // (i.e. We are looking down the negative Z axis).
    //
    // @param outMatrix The matrix to store the result in.
    // @param eyePos The position in world space of the lens/eye.
    // @param atPos The position in world space for the eye/lens to focus upon.
    // @param upVec The direction of UP in the world (e.g. sometimes its the y axis or unit vector on the Y axis).
    //
    // @return Matrix4& The output matrix.
    //
    // /////////////////////////////////////////////////////////////////
    Matrix4 &BuildLookAt(Matrix4 &outMatrix, const Point3 &eyePos, const Point3 &atPos, const Vector3 &upVec);

    // /////////////////////////////////////////////////////////////////
    // Build a perspective PROJECTION matrix.
    //
    // This perspective projection matrix assumes a right-hand coordinate
    // system.  That is the positive X axis to the right, the positive Y
    // axis going up and the positive Z axis coming towards the screen
    // (i.e. We are looking down the negative Z axis).
    //
    // @param outMatrix The matrix to store the result in.
    // @param fov The angle or field of view.
    // @param aspect The screen aspect ratio.
    // @param zNear The location of the near clipping plane.
    // @param zFar The location of the far clipping plane.
    //
    // @return Matrix4& The output matrix.
    //
    // /////////////////////////////////////////////////////////////////
    Matrix4 &BuildPerspectiveFov(Matrix4 &outMatrix, const F32 fov, const F32 aspect, const F32 zNear, const F32 zFar);

    // /////////////////////////////////////////////////////////////////
    // Build an orthographic PROJECTION matrix.
    //
    // This orthographic projection matrix assumes a right-hand coordinate
    // system.  That is the positive X axis to the right, the positive Y
    // axis going up and the positive Z axis coming towards the screen
    // (i.e. We are looking down the negative Z axis).
    //
    // @param outMatrix The matrix to store the result in.
    // @param xMin The minimum X plane.
    // @param xMax The maximum X plane.
    // @param yMin Minimum Y plane.
    // @param yMax Maximum Y plane.
    // @param zMin Minimum Z plane.
    // @param zMax Maximum Z plane.
    //
    // @return Matrix4& The output matrix.
    //
    // /////////////////////////////////////////////////////////////////
    Matrix4 &BuildOrthographic(Matrix4 &outMatrix, const F32 xMin, const F32 xMax, const F32 yMin, const F32 yMax, const F32 zMin, const F32 zMax);

    // /////////////////////////////////////////////////////////////////
    // Extract the rotation matrix from a Matrix4.
    //
    // /////////////////////////////////////////////////////////////////
    void ExtractRotationMatrix(const Matrix4 &matrix, Matrix3x3 rotMatArr);

}

#endif
