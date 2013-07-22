#pragma once
#ifndef __MATRIX4_TEST_SUITE_H
#define __MATRIX4_TEST_SUITE_H

// /////////////////////////////////////////////////////////////////
// @file Matrix4TestSuite.h
// @author PJ O Halloran
// @date 27/08/2010
//
// File contains the header for the Matrix4 Test Suite.
//
// /////////////////////////////////////////////////////////////////

#include <cxxtest/TestSuite.h>
#include <boost/scoped_ptr.hpp>

#include "CommonMath.h"
#include "Matrix.h"

using GameHalloran::VecFloat;
using GameHalloran::Vector3;
using GameHalloran::Vector4;
using GameHalloran::Point3;
using GameHalloran::Matrix4;

// /////////////////////////////////////////////////////////////////
// @class Matrix4TestSuite
// @author PJ O Halloran
//
// This class defines a series of unit tests for the Matrix4
// class.
//
// /////////////////////////////////////////////////////////////////
class Matrix4TestSuite : public CxxTest::TestSuite {
private:

    bool IsTestDataReady() {
        return (true);
    };

public:

    // /////////////////////////////////////////////////////////////////
    // Constructor.
    //
    // /////////////////////////////////////////////////////////////////
    Matrix4TestSuite() {

    };

    // /////////////////////////////////////////////////////////////////
    // Destructor.
    //
    // /////////////////////////////////////////////////////////////////
    ~Matrix4TestSuite() {

    };

    // /////////////////////////////////////////////////////////////////
    //
    //
    // /////////////////////////////////////////////////////////////////
    void setUp() {

    };

    // /////////////////////////////////////////////////////////////////
    //
    //
    // /////////////////////////////////////////////////////////////////
    void tearDown() {

    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::Matrix4()
    //
    // /////////////////////////////////////////////////////////////////
    void testDefaultConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        Matrix4 defaultObj;

        const VecFloat delta = 0.01f;
        VecFloat arr[Matrix4::NUMBER_ELEMENTS];
        for(int i = 0; i < Matrix4::NUMBER_COLS; ++i) {
            for(int j = 0; j < Matrix4::NUMBER_ROWS; ++j) {
                defaultObj.GetElement(0, 0, arr[i * j + j]);
                TS_ASSERT_DELTA(arr[i * j + j], 0.0f, delta);
            }
        }
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::Matrix4(const VecFloat)
    //
    // /////////////////////////////////////////////////////////////////
    void testScalerConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat scaler = 99.45f;
        Matrix4 mat(scaler);
        const VecFloat delta = 0.01f;
        VecFloat arr[Matrix4::NUMBER_ELEMENTS];
        for(int i = 0; i < Matrix4::NUMBER_COLS; ++i) {
            for(int j = 0; j < Matrix4::NUMBER_ROWS; ++j) {
                mat.GetElement(0, 0, arr[i * j + j]);
                TS_ASSERT_DELTA(arr[i * j + j], scaler, delta);
            }
        }
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::Matrix4(const VecFloat (* 16))
    //
    // /////////////////////////////////////////////////////////////////
    void testCompleteConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        VecFloat m00 = 1.0f, m10 = 2.0f, m20 = 3.0f, m30 = 4.0f;
        VecFloat m01 = 1.1f, m11 = 2.2f, m21 = 3.3f, m31 = 4.4f;
        VecFloat m02 = 5.5f, m12 = 6.6f, m22 = 7.7f, m32 = 8.8f;
        VecFloat m03 = 9.9f, m13 = 2.5f, m23 = 6.1f, m33 = 7.3f;

        Matrix4 mat(m00, m10, m20, m30, \
                    m01, m11, m21, m31, \
                    m02, m12, m22, m32, \
                    m03, m13, m23, m33);

        VecFloat g00, g10, g20, g30;
        VecFloat g01, g11, g21, g31;
        VecFloat g02, g12, g22, g32;
        VecFloat g03, g13, g23, g33;
        mat.GetElement(0, 0, g00);
        mat.GetElement(1, 0, g10);
        mat.GetElement(2, 0, g20);
        mat.GetElement(3, 0, g30);
        mat.GetElement(0, 1, g01);
        mat.GetElement(1, 1, g11);
        mat.GetElement(2, 1, g21);
        mat.GetElement(3, 1, g31);
        mat.GetElement(0, 2, g02);
        mat.GetElement(1, 2, g12);
        mat.GetElement(2, 2, g22);
        mat.GetElement(3, 2, g32);
        mat.GetElement(0, 3, g03);
        mat.GetElement(1, 3, g13);
        mat.GetElement(2, 3, g23);
        mat.GetElement(3, 3, g33);

        TS_ASSERT_DELTA(m00, g00, delta);
        TS_ASSERT_DELTA(m10, g10, delta);
        TS_ASSERT_DELTA(m20, g20, delta);
        TS_ASSERT_DELTA(m30, g30, delta);
        TS_ASSERT_DELTA(m01, g01, delta);
        TS_ASSERT_DELTA(m11, g11, delta);
        TS_ASSERT_DELTA(m21, g21, delta);
        TS_ASSERT_DELTA(m31, g31, delta);
        TS_ASSERT_DELTA(m02, g02, delta);
        TS_ASSERT_DELTA(m12, g12, delta);
        TS_ASSERT_DELTA(m22, g22, delta);
        TS_ASSERT_DELTA(m32, g32, delta);
        TS_ASSERT_DELTA(m03, g03, delta);
        TS_ASSERT_DELTA(m13, g13, delta);
        TS_ASSERT_DELTA(m23, g23, delta);
        TS_ASSERT_DELTA(m33, g33, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::Matrix4(const Vector3 &, const Vector3 &, const Vector3 &, const Point3 &)
    //
    // /////////////////////////////////////////////////////////////////
    void testVectorPointConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        Vector3 xOrientation(1.0f, 2.0f, 3.0f), yOrientation(4.0f, 5.0f, 6.0f), zOrientation(7.0f, 8.0f, 9.0f);
        Point3 pos(20.0f, 30.0f, 40.0f);

        Matrix4 mat(xOrientation, yOrientation, zOrientation, pos);

        VecFloat g00, g10, g20, g30;
        VecFloat g01, g11, g21, g31;
        VecFloat g02, g12, g22, g32;
        VecFloat g03, g13, g23, g33;
        mat.GetElement(0, 0, g00);
        mat.GetElement(1, 0, g10);
        mat.GetElement(2, 0, g20);
        mat.GetElement(3, 0, g30);
        mat.GetElement(0, 1, g01);
        mat.GetElement(1, 1, g11);
        mat.GetElement(2, 1, g21);
        mat.GetElement(3, 1, g31);
        mat.GetElement(0, 2, g02);
        mat.GetElement(1, 2, g12);
        mat.GetElement(2, 2, g22);
        mat.GetElement(3, 2, g32);
        mat.GetElement(0, 3, g03);
        mat.GetElement(1, 3, g13);
        mat.GetElement(2, 3, g23);
        mat.GetElement(3, 3, g33);

        TS_ASSERT_DELTA(xOrientation.GetX(), g00, delta);
        TS_ASSERT_DELTA(yOrientation.GetX(), g10, delta);
        TS_ASSERT_DELTA(zOrientation.GetX(), g20, delta);
        TS_ASSERT_DELTA(pos.GetX(), g30, delta);
        TS_ASSERT_DELTA(xOrientation.GetY(), g01, delta);
        TS_ASSERT_DELTA(yOrientation.GetY(), g11, delta);
        TS_ASSERT_DELTA(zOrientation.GetY(), g21, delta);
        TS_ASSERT_DELTA(pos.GetY(), g31, delta);
        TS_ASSERT_DELTA(xOrientation.GetZ(), g02, delta);
        TS_ASSERT_DELTA(yOrientation.GetZ(), g12, delta);
        TS_ASSERT_DELTA(zOrientation.GetZ(), g22, delta);
        TS_ASSERT_DELTA(pos.GetZ(), g32, delta);
        TS_ASSERT_DELTA(0.0f, g03, delta);
        TS_ASSERT_DELTA(0.0f, g13, delta);
        TS_ASSERT_DELTA(0.0f, g23, delta);
        TS_ASSERT_DELTA(1.0f, g33, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::Matrix4(const Matrix4 &)
    //
    // /////////////////////////////////////////////////////////////////
    void testCopyConstructor(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        Matrix4 mat(56.23f);
        Matrix4 copy(mat);

        VecFloat m00, m10, m20, m30;
        VecFloat m01, m11, m21, m31;
        VecFloat m02, m12, m22, m32;
        VecFloat m03, m13, m23, m33;
        mat.GetElement(0, 0, m00);
        mat.GetElement(1, 0, m10);
        mat.GetElement(2, 0, m20);
        mat.GetElement(3, 0, m30);
        mat.GetElement(0, 1, m01);
        mat.GetElement(1, 1, m11);
        mat.GetElement(2, 1, m21);
        mat.GetElement(3, 1, m31);
        mat.GetElement(0, 2, m02);
        mat.GetElement(1, 2, m12);
        mat.GetElement(2, 2, m22);
        mat.GetElement(3, 2, m32);
        mat.GetElement(0, 3, m03);
        mat.GetElement(1, 3, m13);
        mat.GetElement(2, 3, m23);
        mat.GetElement(3, 3, m33);

        VecFloat c00, c10, c20, c30;
        VecFloat c01, c11, c21, c31;
        VecFloat c02, c12, c22, c32;
        VecFloat c03, c13, c23, c33;
        copy.GetElement(0, 0, c00);
        copy.GetElement(1, 0, c10);
        copy.GetElement(2, 0, c20);
        copy.GetElement(3, 0, c30);
        copy.GetElement(0, 1, c01);
        copy.GetElement(1, 1, c11);
        copy.GetElement(2, 1, c21);
        copy.GetElement(3, 1, c31);
        copy.GetElement(0, 2, c02);
        copy.GetElement(1, 2, c12);
        copy.GetElement(2, 2, c22);
        copy.GetElement(3, 2, c32);
        copy.GetElement(0, 3, c03);
        copy.GetElement(1, 3, c13);
        copy.GetElement(2, 3, c23);
        copy.GetElement(3, 3, c33);

        TS_ASSERT_DELTA(m00, c00, delta);
        TS_ASSERT_DELTA(m10, c10, delta);
        TS_ASSERT_DELTA(m20, c20, delta);
        TS_ASSERT_DELTA(m30, c30, delta);
        TS_ASSERT_DELTA(m01, c01, delta);
        TS_ASSERT_DELTA(m11, c11, delta);
        TS_ASSERT_DELTA(m21, c21, delta);
        TS_ASSERT_DELTA(m31, c31, delta);
        TS_ASSERT_DELTA(m02, c02, delta);
        TS_ASSERT_DELTA(m12, c12, delta);
        TS_ASSERT_DELTA(m22, c22, delta);
        TS_ASSERT_DELTA(m32, c32, delta);
        TS_ASSERT_DELTA(m03, c03, delta);
        TS_ASSERT_DELTA(m13, c13, delta);
        TS_ASSERT_DELTA(m23, c23, delta);
        TS_ASSERT_DELTA(m33, c33, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::LoadIdentity()
    //
    // /////////////////////////////////////////////////////////////////
    void testLoadIdentity(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        Matrix4 mat(12.45f);

        mat.LoadIdentity();

        VecFloat g00, g10, g20, g30;
        VecFloat g01, g11, g21, g31;
        VecFloat g02, g12, g22, g32;
        VecFloat g03, g13, g23, g33;
        mat.GetElement(0, 0, g00);
        mat.GetElement(1, 0, g10);
        mat.GetElement(2, 0, g20);
        mat.GetElement(3, 0, g30);
        mat.GetElement(0, 1, g01);
        mat.GetElement(1, 1, g11);
        mat.GetElement(2, 1, g21);
        mat.GetElement(3, 1, g31);
        mat.GetElement(0, 2, g02);
        mat.GetElement(1, 2, g12);
        mat.GetElement(2, 2, g22);
        mat.GetElement(3, 2, g32);
        mat.GetElement(0, 3, g03);
        mat.GetElement(1, 3, g13);
        mat.GetElement(2, 3, g23);
        mat.GetElement(3, 3, g33);

        TS_ASSERT_DELTA(g00, 1.0f, delta);
        TS_ASSERT_DELTA(g10, 0.0f, delta);
        TS_ASSERT_DELTA(g20, 0.0f, delta);
        TS_ASSERT_DELTA(g30, 0.0f, delta);
        TS_ASSERT_DELTA(g01, 0.0f, delta);
        TS_ASSERT_DELTA(g11, 1.0f, delta);
        TS_ASSERT_DELTA(g21, 0.0f, delta);
        TS_ASSERT_DELTA(g31, 0.0f, delta);
        TS_ASSERT_DELTA(g02, 0.0f, delta);
        TS_ASSERT_DELTA(g12, 0.0f, delta);
        TS_ASSERT_DELTA(g22, 1.0f, delta);
        TS_ASSERT_DELTA(g32, 0.0f, delta);
        TS_ASSERT_DELTA(g03, 0.0f, delta);
        TS_ASSERT_DELTA(g13, 0.0f, delta);
        TS_ASSERT_DELTA(g23, 0.0f, delta);
        TS_ASSERT_DELTA(g33, 1.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::ZeroMatrix()
    //
    // /////////////////////////////////////////////////////////////////
    void testZeroMatrix(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        const VecFloat delta = 0.01f;
        Matrix4 mat(12.45f);

        mat.ZeroMatrix();

        VecFloat g00, g10, g20, g30;
        VecFloat g01, g11, g21, g31;
        VecFloat g02, g12, g22, g32;
        VecFloat g03, g13, g23, g33;
        mat.GetElement(0, 0, g00);
        mat.GetElement(1, 0, g10);
        mat.GetElement(2, 0, g20);
        mat.GetElement(3, 0, g30);
        mat.GetElement(0, 1, g01);
        mat.GetElement(1, 1, g11);
        mat.GetElement(2, 1, g21);
        mat.GetElement(3, 1, g31);
        mat.GetElement(0, 2, g02);
        mat.GetElement(1, 2, g12);
        mat.GetElement(2, 2, g22);
        mat.GetElement(3, 2, g32);
        mat.GetElement(0, 3, g03);
        mat.GetElement(1, 3, g13);
        mat.GetElement(2, 3, g23);
        mat.GetElement(3, 3, g33);

        TS_ASSERT_DELTA(g00, 0.0f, delta);
        TS_ASSERT_DELTA(g10, 0.0f, delta);
        TS_ASSERT_DELTA(g20, 0.0f, delta);
        TS_ASSERT_DELTA(g30, 0.0f, delta);
        TS_ASSERT_DELTA(g01, 0.0f, delta);
        TS_ASSERT_DELTA(g11, 0.0f, delta);
        TS_ASSERT_DELTA(g21, 0.0f, delta);
        TS_ASSERT_DELTA(g31, 0.0f, delta);
        TS_ASSERT_DELTA(g02, 0.0f, delta);
        TS_ASSERT_DELTA(g12, 0.0f, delta);
        TS_ASSERT_DELTA(g22, 0.0f, delta);
        TS_ASSERT_DELTA(g32, 0.0f, delta);
        TS_ASSERT_DELTA(g03, 0.0f, delta);
        TS_ASSERT_DELTA(g13, 0.0f, delta);
        TS_ASSERT_DELTA(g23, 0.0f, delta);
        TS_ASSERT_DELTA(g33, 0.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::Transpose()
    //
    // /////////////////////////////////////////////////////////////////
    void testTranspose(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat m00 = 1.0f, m10 = 2.0f, m20 = 7.7f, m30 = 99.12f;
        VecFloat m01 = 0.0f, m11 = 45.1f, m21 = 1.7f, m31 = 4.12f;
        VecFloat m02 = 392.12f, m12 = 5.7f, m22 = 7.1f, m32 = 85.12f;
        VecFloat m03 = 99999.3f, m13 = 27.93f, m23 = 99.7f, m33 = 0.0f;
        Matrix4 mat(m00, m10, m20, m30, \
                    m01, m11, m21, m31, \
                    m02, m12, m22, m32, \
                    m03, m13, m23, m33);

        Matrix4 exTrans(m00, m01, m02, m03, \
                        m10, m11, m12, m13, \
                        m20, m21, m22, m23, \
                        m30, m31, m32, m33);

        mat.Transpose();

        VecFloat g00, g10, g20, g30;
        VecFloat g01, g11, g21, g31;
        VecFloat g02, g12, g22, g32;
        VecFloat g03, g13, g23, g33;
        mat.GetElement(0, 0, g00);
        mat.GetElement(1, 0, g10);
        mat.GetElement(2, 0, g20);
        mat.GetElement(3, 0, g30);
        mat.GetElement(0, 1, g01);
        mat.GetElement(1, 1, g11);
        mat.GetElement(2, 1, g21);
        mat.GetElement(3, 1, g31);
        mat.GetElement(0, 2, g02);
        mat.GetElement(1, 2, g12);
        mat.GetElement(2, 2, g22);
        mat.GetElement(3, 2, g32);
        mat.GetElement(0, 3, g03);
        mat.GetElement(1, 3, g13);
        mat.GetElement(2, 3, g23);
        mat.GetElement(3, 3, g33);
        VecFloat t00, t10, t20, t30;
        VecFloat t01, t11, t21, t31;
        VecFloat t02, t12, t22, t32;
        VecFloat t03, t13, t23, t33;
        exTrans.GetElement(0, 0, t00);
        exTrans.GetElement(1, 0, t10);
        exTrans.GetElement(2, 0, t20);
        exTrans.GetElement(3, 0, t30);
        exTrans.GetElement(0, 1, t01);
        exTrans.GetElement(1, 1, t11);
        exTrans.GetElement(2, 1, t21);
        exTrans.GetElement(3, 1, t31);
        exTrans.GetElement(0, 2, t02);
        exTrans.GetElement(1, 2, t12);
        exTrans.GetElement(2, 2, t22);
        exTrans.GetElement(3, 2, t32);
        exTrans.GetElement(0, 3, t03);
        exTrans.GetElement(1, 3, t13);
        exTrans.GetElement(2, 3, t23);
        exTrans.GetElement(3, 3, t33);

        const VecFloat delta = 0.01f;
        TS_ASSERT_DELTA(g00, t00, delta);
        TS_ASSERT_DELTA(g10, t10, delta);
        TS_ASSERT_DELTA(g20, t20, delta);
        TS_ASSERT_DELTA(g30, t30, delta);
        TS_ASSERT_DELTA(g01, t01, delta);
        TS_ASSERT_DELTA(g11, t11, delta);
        TS_ASSERT_DELTA(g21, t21, delta);
        TS_ASSERT_DELTA(g31, t31, delta);
        TS_ASSERT_DELTA(g02, t02, delta);
        TS_ASSERT_DELTA(g12, t12, delta);
        TS_ASSERT_DELTA(g22, t22, delta);
        TS_ASSERT_DELTA(g32, t32, delta);
        TS_ASSERT_DELTA(g03, t03, delta);
        TS_ASSERT_DELTA(g13, t13, delta);
        TS_ASSERT_DELTA(g23, t23, delta);
        TS_ASSERT_DELTA(g33, t33, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::Transposed()
    //
    // /////////////////////////////////////////////////////////////////
    void testTransposed(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat m00 = 1.0f, m10 = 2.0f, m20 = 7.7f, m30 = 99.12f;
        VecFloat m01 = 0.0f, m11 = 45.1f, m21 = 1.7f, m31 = 4.12f;
        VecFloat m02 = 392.12f, m12 = 5.7f, m22 = 7.1f, m32 = 85.12f;
        VecFloat m03 = 99999.3f, m13 = 27.93f, m23 = 99.7f, m33 = 0.0f;
        Matrix4 mat(m00, m10, m20, m30, \
                    m01, m11, m21, m31, \
                    m02, m12, m22, m32, \
                    m03, m13, m23, m33);

        Matrix4 exTrans(m00, m01, m02, m03, \
                        m10, m11, m12, m13, \
                        m20, m21, m22, m23, \
                        m30, m31, m32, m33);

        Matrix4 acTrans;
        mat.Transposed(acTrans);

        VecFloat g00, g10, g20, g30;
        VecFloat g01, g11, g21, g31;
        VecFloat g02, g12, g22, g32;
        VecFloat g03, g13, g23, g33;
        acTrans.GetElement(0, 0, g00);
        acTrans.GetElement(1, 0, g10);
        acTrans.GetElement(2, 0, g20);
        acTrans.GetElement(3, 0, g30);
        acTrans.GetElement(0, 1, g01);
        acTrans.GetElement(1, 1, g11);
        acTrans.GetElement(2, 1, g21);
        acTrans.GetElement(3, 1, g31);
        acTrans.GetElement(0, 2, g02);
        acTrans.GetElement(1, 2, g12);
        acTrans.GetElement(2, 2, g22);
        acTrans.GetElement(3, 2, g32);
        acTrans.GetElement(0, 3, g03);
        acTrans.GetElement(1, 3, g13);
        acTrans.GetElement(2, 3, g23);
        acTrans.GetElement(3, 3, g33);
        VecFloat t00, t10, t20, t30;
        VecFloat t01, t11, t21, t31;
        VecFloat t02, t12, t22, t32;
        VecFloat t03, t13, t23, t33;
        exTrans.GetElement(0, 0, t00);
        exTrans.GetElement(1, 0, t10);
        exTrans.GetElement(2, 0, t20);
        exTrans.GetElement(3, 0, t30);
        exTrans.GetElement(0, 1, t01);
        exTrans.GetElement(1, 1, t11);
        exTrans.GetElement(2, 1, t21);
        exTrans.GetElement(3, 1, t31);
        exTrans.GetElement(0, 2, t02);
        exTrans.GetElement(1, 2, t12);
        exTrans.GetElement(2, 2, t22);
        exTrans.GetElement(3, 2, t32);
        exTrans.GetElement(0, 3, t03);
        exTrans.GetElement(1, 3, t13);
        exTrans.GetElement(2, 3, t23);
        exTrans.GetElement(3, 3, t33);

        const VecFloat delta = 0.01f;
        TS_ASSERT_DELTA(g00, t00, delta);
        TS_ASSERT_DELTA(g10, t10, delta);
        TS_ASSERT_DELTA(g20, t20, delta);
        TS_ASSERT_DELTA(g30, t30, delta);
        TS_ASSERT_DELTA(g01, t01, delta);
        TS_ASSERT_DELTA(g11, t11, delta);
        TS_ASSERT_DELTA(g21, t21, delta);
        TS_ASSERT_DELTA(g31, t31, delta);
        TS_ASSERT_DELTA(g02, t02, delta);
        TS_ASSERT_DELTA(g12, t12, delta);
        TS_ASSERT_DELTA(g22, t22, delta);
        TS_ASSERT_DELTA(g32, t32, delta);
        TS_ASSERT_DELTA(g03, t03, delta);
        TS_ASSERT_DELTA(g13, t13, delta);
        TS_ASSERT_DELTA(g23, t23, delta);
        TS_ASSERT_DELTA(g33, t33, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::Determinant()
    //
    // /////////////////////////////////////////////////////////////////
    void testDeterminant(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Test getting a determinant when the expected determinant is 0.
        Matrix4 mat(1.0f, 2.0f, 3.0f, 4.0f, \
                    5.0f, 6.0f, 7.0f, 8.0f, \
                    9.0f, 10.0f, 9.0f, 8.0f, \
                    7.0f, 6.0f, 5.0f, 4.0f);
        const VecFloat delta = 0.01f;
        VecFloat acDet, exDet = 0.0f;
        acDet = mat.Determinant();
        TS_ASSERT_DELTA(acDet, exDet, delta);

        // Test getting a determinant when the expected determinant is non 0.
        Matrix4 testMat(1.0f, 0.0f, 0.0f, 0.0f, \
                        0.0f, 0.0f, -1.0f, 0.0f, \
                        0.0f, 1.0f, 0.0f, 0.0f, \
                        0.0f, 0.0f, 0.0f, 1.0f);
        exDet = 1.0f;
        acDet = testMat.Determinant();
        TS_ASSERT_DELTA(acDet, exDet, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::Inversed()
    //
    // /////////////////////////////////////////////////////////////////
    void testInversed(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        //// Get the inverse of a matrix (when the input matrix has no inverse).
        //Matrix4 mat(143.0f, 2.573f, 6895.0f, 4.0f,\
        //          5.0f, 6.0f, 9392.0f, 8.0f,\
        //          9.0f, 592.0f, 9.0f, 8.0f,\
        //          7.0f, 1020.0f, 5.0f, 4.0f);
        //Matrix4 copy(mat);
        //bool hasInverse = mat.Inverse();
        //TS_ASSERT(!hasInverse);
        //if(hasInverse)
        //{
        //  // If you multiply a matrix by its inverse, then you should get the identity matrix as a result
        //  Matrix4 result = copy * mat;
        //  // Check if the result is the identity matrix...
        //  VecFloat g00, g10, g20, g30;
        //  VecFloat g01, g11, g21, g31;
        //  VecFloat g02, g12, g22, g32;
        //  VecFloat g03, g13, g23, g33;
        //  result.GetElement(0, 0, g00);   result.GetElement(1, 0, g10);   result.GetElement(2, 0, g20);   result.GetElement(3, 0, g30);
        //  result.GetElement(0, 1, g01);   result.GetElement(1, 1, g11);   result.GetElement(2, 1, g21);   result.GetElement(3, 1, g31);
        //  result.GetElement(0, 2, g02);   result.GetElement(1, 2, g12);   result.GetElement(2, 2, g22);   result.GetElement(3, 2, g32);
        //  result.GetElement(0, 3, g03);   result.GetElement(1, 3, g13);   result.GetElement(2, 3, g23);   result.GetElement(3, 3, g33);
        //  const VecFloat delta = 0.01f;
        //  TS_WARN(result.ToString().c_str());
        //  TS_ASSERT_DELTA(g00, 1.0f, delta);
        //  TS_ASSERT_DELTA(g10, 0.0f, delta);
        //  TS_ASSERT_DELTA(g20, 0.0f, delta);
        //  TS_ASSERT_DELTA(g30, 0.0f, delta);
        //  TS_ASSERT_DELTA(g01, 0.0f, delta);
        //  TS_ASSERT_DELTA(g11, 1.0f, delta);
        //  TS_ASSERT_DELTA(g21, 0.0f, delta);
        //  TS_ASSERT_DELTA(g31, 0.0f, delta);
        //  TS_ASSERT_DELTA(g02, 0.0f, delta);
        //  TS_ASSERT_DELTA(g12, 0.0f, delta);
        //  TS_ASSERT_DELTA(g22, 1.0f, delta);
        //  TS_ASSERT_DELTA(g32, 0.0f, delta);
        //  TS_ASSERT_DELTA(g03, 0.0f, delta);
        //  TS_ASSERT_DELTA(g13, 0.0f, delta);
        //  TS_ASSERT_DELTA(g23, 0.0f, delta);
        //  TS_ASSERT_DELTA(g33, 1.0f, delta);
        //}

        // Get the inverse of a matrix (when the input matrix does have an inverse).
        Matrix4 testMat(1.0f, 0.0f, 0.0f, 0.0f, \
                        0.0f, 0.0f, -1.0f, 0.0f, \
                        0.0f, 1.0f, 0.0f, 0.0f, \
                        0.0f, 0.0f, 0.0f, 1.0f);
        Matrix4 exInverse(1.0f, 0.0f, 0.0f, 0.0f, \
                          0.0f, 0.0f, 1.0f, 0.0f, \
                          0.0f, -1.0f, 0.0f, 0.0f, \
                          0.0f, 0.0f, 0.0f, 1.0f);
        Matrix4 acInverse;
        bool hasInverse = false;
        TS_ASSERT(hasInverse = testMat.Inversed(acInverse));
        //TS_WARN(testMat.ToString().c_str());
        //TS_WARN(acInverse.ToString().c_str());
        if(hasInverse) {
            const VecFloat delta = 0.01f;
            for(int i = 0; i < Matrix4::NUMBER_ELEMENTS; ++i) {
                //TS_WARN("i");
                TS_ASSERT_DELTA(acInverse[i], exInverse[i], delta);
            }
        }
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::Inverse()
    //
    // /////////////////////////////////////////////////////////////////
    void testInverse(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Get the inverse of a matrix (when the input matrix does have an inverse).
        Matrix4 testMat(1.0f, 0.0f, 0.0f, 0.0f, \
                        0.0f, 0.0f, -1.0f, 0.0f, \
                        0.0f, 1.0f, 0.0f, 0.0f, \
                        0.0f, 0.0f, 0.0f, 1.0f);
        Matrix4 exInverse(1.0f, 0.0f, 0.0f, 0.0f, \
                          0.0f, 0.0f, 1.0f, 0.0f, \
                          0.0f, -1.0f, 0.0f, 0.0f, \
                          0.0f, 0.0f, 0.0f, 1.0f);
        bool hasInverse = false;
        TS_ASSERT(hasInverse = testMat.Inverse());
        //TS_WARN(testMat.ToString().c_str());
        if(hasInverse) {
            const VecFloat delta = 0.01f;
            for(int i = 0; i < Matrix4::NUMBER_ELEMENTS; ++i) {
                //TS_WARN("i");
                TS_ASSERT_DELTA(testMat[i], exInverse[i], delta);
            }
        }

        //Matrix4 mat(143.0f, 2.573f, 6895.0f, 4.0f,\
        //          5.0f, 6.0f, 9392.0f, 8.0f,\
        //          9.0f, 592.0f, 9.0f, 8.0f,\
        //          7.0f, 1020.0f, 5.0f, 4.0f);
        //// Get the inverse of a matrix.
        //Matrix4 inv;
        //bool hasInverse = mat.Inversed(inv);
        //TS_ASSERT(hasInverse);
        //if(hasInverse)
        //{
        //  // If you multiply a matrix by its inverse, then you should get the identity matrix as a result
        //  Matrix4 result = mat * inv;
        //  // Check if the result is the identity matrix...
        //  VecFloat g00, g10, g20, g30;
        //  VecFloat g01, g11, g21, g31;
        //  VecFloat g02, g12, g22, g32;
        //  VecFloat g03, g13, g23, g33;
        //  result.GetElement(0, 0, g00);   result.GetElement(1, 0, g10);   result.GetElement(2, 0, g20);   result.GetElement(3, 0, g30);
        //  result.GetElement(0, 1, g01);   result.GetElement(1, 1, g11);   result.GetElement(2, 1, g21);   result.GetElement(3, 1, g31);
        //  result.GetElement(0, 2, g02);   result.GetElement(1, 2, g12);   result.GetElement(2, 2, g22);   result.GetElement(3, 2, g32);
        //  result.GetElement(0, 3, g03);   result.GetElement(1, 3, g13);   result.GetElement(2, 3, g23);   result.GetElement(3, 3, g33);
        //  const VecFloat delta = 0.01f;
        //  TS_WARN(result.ToString().c_str());
        //  TS_ASSERT_DELTA(g00, 1.0f, delta);
        //  TS_ASSERT_DELTA(g10, 0.0f, delta);
        //  TS_ASSERT_DELTA(g20, 0.0f, delta);
        //  TS_ASSERT_DELTA(g30, 0.0f, delta);
        //  TS_ASSERT_DELTA(g01, 0.0f, delta);
        //  TS_ASSERT_DELTA(g11, 1.0f, delta);
        //  TS_ASSERT_DELTA(g21, 0.0f, delta);
        //  TS_ASSERT_DELTA(g31, 0.0f, delta);
        //  TS_ASSERT_DELTA(g02, 0.0f, delta);
        //  TS_ASSERT_DELTA(g12, 0.0f, delta);
        //  TS_ASSERT_DELTA(g22, 1.0f, delta);
        //  TS_ASSERT_DELTA(g32, 0.0f, delta);
        //  TS_ASSERT_DELTA(g03, 0.0f, delta);
        //  TS_ASSERT_DELTA(g13, 0.0f, delta);
        //  TS_ASSERT_DELTA(g23, 0.0f, delta);
        //  TS_ASSERT_DELTA(g33, 1.0f, delta);
        //}
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::InversedCramer(Matrix4 &)
    //
    // /////////////////////////////////////////////////////////////////
    void testInversedCramer(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Get the inverse of a matrix (when the input matrix does have an inverse).
        Matrix4 testMat(1.0f, 0.0f, 0.0f, 0.0f, \
                        0.0f, 0.0f, -1.0f, 0.0f, \
                        0.0f, 1.0f, 0.0f, 0.0f, \
                        0.0f, 0.0f, 0.0f, 1.0f);
        Matrix4 exInverse(1.0f, 0.0f, 0.0f, 0.0f, \
                          0.0f, 0.0f, 1.0f, 0.0f, \
                          0.0f, -1.0f, 0.0f, 0.0f, \
                          0.0f, 0.0f, 0.0f, 1.0f);
        Matrix4 acInverse;
        bool hasInverse = false;
        TS_ASSERT(hasInverse = testMat.InversedCramer(acInverse));
        //TS_WARN(testMat.ToString().c_str());
        //TS_WARN(acInverse.ToString().c_str());
        if(hasInverse) {
            const VecFloat delta = 0.01f;
            for(int i = 0; i < Matrix4::NUMBER_ELEMENTS; ++i) {
                //TS_WARN("i");
                TS_ASSERT_DELTA(acInverse[i], exInverse[i], delta);
            }
        }
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::InverseCramer()
    //
    // /////////////////////////////////////////////////////////////////
    void testInverseCramer(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Get the inverse of a matrix (when the input matrix does have an inverse).
        Matrix4 testMat(1.0f, 0.0f, 0.0f, 0.0f, \
                        0.0f, 0.0f, -1.0f, 0.0f, \
                        0.0f, 1.0f, 0.0f, 0.0f, \
                        0.0f, 0.0f, 0.0f, 1.0f);
        Matrix4 exInverse(1.0f, 0.0f, 0.0f, 0.0f, \
                          0.0f, 0.0f, 1.0f, 0.0f, \
                          0.0f, -1.0f, 0.0f, 0.0f, \
                          0.0f, 0.0f, 0.0f, 1.0f);
        bool hasInverse = false;
        TS_ASSERT(hasInverse = testMat.InverseCramer());
        //TS_WARN(testMat.ToString().c_str());
        if(hasInverse) {
            const VecFloat delta = 0.01f;
            for(int i = 0; i < Matrix4::NUMBER_ELEMENTS; ++i) {
                //TS_WARN("i");
                TS_ASSERT_DELTA(testMat[i], exInverse[i], delta);
            }
        }
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::operator*(Matrix4)
    //
    // /////////////////////////////////////////////////////////////////
    void testMultiplyMatrix(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        Matrix4 a(1.0f, 2.0f, 3.0f, 4.0f, \
                  5.0f, 6.0f, 7.0f, 8.0f, \
                  9.0f, 10.0f, 11.0f, 12.0f, \
                  13.0f, 14.0f, 15.0f, 16.0f);

        Matrix4 b(2.0f, 2.0f, 2.0f, 2.0f, \
                  3.0f, 3.0f, 3.0f, 3.0f, \
                  4.0f, 4.0f, 4.0f, 4.0f, \
                  5.0f, 5.0f, 5.0f, 5.0f);

        Matrix4 exRes(40.0f, 40.0f, 40.0f, 40.0f, \
                      96.0f, 96.0f, 96.0f, 96.0f, \
                      152.0f, 152.0f, 152.0f, 152.0f, \
                      208.0f, 208.0f, 208.0f, 208.0f);

        Matrix4 acRes = a * b;

        VecFloat a00, a10, a20, a30;
        VecFloat a01, a11, a21, a31;
        VecFloat a02, a12, a22, a32;
        VecFloat a03, a13, a23, a33;
        acRes.GetElement(0, 0, a00);
        acRes.GetElement(1, 0, a10);
        acRes.GetElement(2, 0, a20);
        acRes.GetElement(3, 0, a30);
        acRes.GetElement(0, 1, a01);
        acRes.GetElement(1, 1, a11);
        acRes.GetElement(2, 1, a21);
        acRes.GetElement(3, 1, a31);
        acRes.GetElement(0, 2, a02);
        acRes.GetElement(1, 2, a12);
        acRes.GetElement(2, 2, a22);
        acRes.GetElement(3, 2, a32);
        acRes.GetElement(0, 3, a03);
        acRes.GetElement(1, 3, a13);
        acRes.GetElement(2, 3, a23);
        acRes.GetElement(3, 3, a33);

        VecFloat e00, e10, e20, e30;
        VecFloat e01, e11, e21, e31;
        VecFloat e02, e12, e22, e32;
        VecFloat e03, e13, e23, e33;
        exRes.GetElement(0, 0, e00);
        exRes.GetElement(1, 0, e10);
        exRes.GetElement(2, 0, e20);
        exRes.GetElement(3, 0, e30);
        exRes.GetElement(0, 1, e01);
        exRes.GetElement(1, 1, e11);
        exRes.GetElement(2, 1, e21);
        exRes.GetElement(3, 1, e31);
        exRes.GetElement(0, 2, e02);
        exRes.GetElement(1, 2, e12);
        exRes.GetElement(2, 2, e22);
        exRes.GetElement(3, 2, e32);
        exRes.GetElement(0, 3, e03);
        exRes.GetElement(1, 3, e13);
        exRes.GetElement(2, 3, e23);
        exRes.GetElement(3, 3, e33);

        // Test the elements in the expected matrix match what was actually found after the matrix multiplication.
        const VecFloat delta = 0.01f;
        TS_ASSERT_DELTA(a00, e00, delta);
        TS_ASSERT_DELTA(a10, e10, delta);
        TS_ASSERT_DELTA(a20, e20, delta);
        TS_ASSERT_DELTA(a30, e30, delta);
        TS_ASSERT_DELTA(a01, e01, delta);
        TS_ASSERT_DELTA(a11, e11, delta);
        TS_ASSERT_DELTA(a21, e21, delta);
        TS_ASSERT_DELTA(a31, e31, delta);
        TS_ASSERT_DELTA(a02, e02, delta);
        TS_ASSERT_DELTA(a12, e12, delta);
        TS_ASSERT_DELTA(a22, e22, delta);
        TS_ASSERT_DELTA(a32, e32, delta);
        TS_ASSERT_DELTA(a03, e03, delta);
        TS_ASSERT_DELTA(a13, e13, delta);
        TS_ASSERT_DELTA(a23, e23, delta);
        TS_ASSERT_DELTA(a33, e33, delta);

        // Test Multiplying a matrix by the identity and zero matrices.
        Matrix4 identity;
        identity.LoadIdentity();
        Matrix4 zero;
        zero.ZeroMatrix();

        Matrix4 iTest = a * identity;

        // Test the iTest == a
        a.GetElement(0, 0, a00);
        a.GetElement(1, 0, a10);
        a.GetElement(2, 0, a20);
        a.GetElement(3, 0, a30);
        a.GetElement(0, 1, a01);
        a.GetElement(1, 1, a11);
        a.GetElement(2, 1, a21);
        a.GetElement(3, 1, a31);
        a.GetElement(0, 2, a02);
        a.GetElement(1, 2, a12);
        a.GetElement(2, 2, a22);
        a.GetElement(3, 2, a32);
        a.GetElement(0, 3, a03);
        a.GetElement(1, 3, a13);
        a.GetElement(2, 3, a23);
        a.GetElement(3, 3, a33);
        iTest.GetElement(0, 0, e00);
        iTest.GetElement(1, 0, e10);
        iTest.GetElement(2, 0, e20);
        iTest.GetElement(3, 0, e30);
        iTest.GetElement(0, 1, e01);
        iTest.GetElement(1, 1, e11);
        iTest.GetElement(2, 1, e21);
        iTest.GetElement(3, 1, e31);
        iTest.GetElement(0, 2, e02);
        iTest.GetElement(1, 2, e12);
        iTest.GetElement(2, 2, e22);
        iTest.GetElement(3, 2, e32);
        iTest.GetElement(0, 3, e03);
        iTest.GetElement(1, 3, e13);
        iTest.GetElement(2, 3, e23);
        iTest.GetElement(3, 3, e33);
        TS_ASSERT_DELTA(a00, e00, delta);
        TS_ASSERT_DELTA(a10, e10, delta);
        TS_ASSERT_DELTA(a20, e20, delta);
        TS_ASSERT_DELTA(a30, e30, delta);
        TS_ASSERT_DELTA(a01, e01, delta);
        TS_ASSERT_DELTA(a11, e11, delta);
        TS_ASSERT_DELTA(a21, e21, delta);
        TS_ASSERT_DELTA(a31, e31, delta);
        TS_ASSERT_DELTA(a02, e02, delta);
        TS_ASSERT_DELTA(a12, e12, delta);
        TS_ASSERT_DELTA(a22, e22, delta);
        TS_ASSERT_DELTA(a32, e32, delta);
        TS_ASSERT_DELTA(a03, e03, delta);
        TS_ASSERT_DELTA(a13, e13, delta);
        TS_ASSERT_DELTA(a23, e23, delta);
        TS_ASSERT_DELTA(a33, e33, delta);

        Matrix4 zTest = a * zero;

        // Test that zTest == zero.
        zero.GetElement(0, 0, a00);
        zero.GetElement(1, 0, a10);
        zero.GetElement(2, 0, a20);
        zero.GetElement(3, 0, a30);
        zero.GetElement(0, 1, a01);
        zero.GetElement(1, 1, a11);
        zero.GetElement(2, 1, a21);
        zero.GetElement(3, 1, a31);
        zero.GetElement(0, 2, a02);
        zero.GetElement(1, 2, a12);
        zero.GetElement(2, 2, a22);
        zero.GetElement(3, 2, a32);
        zero.GetElement(0, 3, a03);
        zero.GetElement(1, 3, a13);
        zero.GetElement(2, 3, a23);
        zero.GetElement(3, 3, a33);
        zTest.GetElement(0, 0, e00);
        zTest.GetElement(1, 0, e10);
        zTest.GetElement(2, 0, e20);
        zTest.GetElement(3, 0, e30);
        zTest.GetElement(0, 1, e01);
        zTest.GetElement(1, 1, e11);
        zTest.GetElement(2, 1, e21);
        zTest.GetElement(3, 1, e31);
        zTest.GetElement(0, 2, e02);
        zTest.GetElement(1, 2, e12);
        zTest.GetElement(2, 2, e22);
        zTest.GetElement(3, 2, e32);
        zTest.GetElement(0, 3, e03);
        zTest.GetElement(1, 3, e13);
        zTest.GetElement(2, 3, e23);
        zTest.GetElement(3, 3, e33);
        TS_ASSERT_DELTA(a00, e00, delta);
        TS_ASSERT_DELTA(a10, e10, delta);
        TS_ASSERT_DELTA(a20, e20, delta);
        TS_ASSERT_DELTA(a30, e30, delta);
        TS_ASSERT_DELTA(a01, e01, delta);
        TS_ASSERT_DELTA(a11, e11, delta);
        TS_ASSERT_DELTA(a21, e21, delta);
        TS_ASSERT_DELTA(a31, e31, delta);
        TS_ASSERT_DELTA(a02, e02, delta);
        TS_ASSERT_DELTA(a12, e12, delta);
        TS_ASSERT_DELTA(a22, e22, delta);
        TS_ASSERT_DELTA(a32, e32, delta);
        TS_ASSERT_DELTA(a03, e03, delta);
        TS_ASSERT_DELTA(a13, e13, delta);
        TS_ASSERT_DELTA(a23, e23, delta);
        TS_ASSERT_DELTA(a33, e33, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::operator*=(Matrix4)
    //
    // /////////////////////////////////////////////////////////////////
    void testMultiplyEqualsMatrix(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        Matrix4 a(1.0f, 2.0f, 3.0f, 4.0f, \
                  5.0f, 6.0f, 7.0f, 8.0f, \
                  9.0f, 10.0f, 11.0f, 12.0f, \
                  13.0f, 14.0f, 15.0f, 16.0f);
        Matrix4 copya(a);

        Matrix4 b(2.0f, 2.0f, 2.0f, 2.0f, \
                  3.0f, 3.0f, 3.0f, 3.0f, \
                  4.0f, 4.0f, 4.0f, 4.0f, \
                  5.0f, 5.0f, 5.0f, 5.0f);

        Matrix4 exRes(40.0f, 40.0f, 40.0f, 40.0f, \
                      96.0f, 96.0f, 96.0f, 96.0f, \
                      152.0f, 152.0f, 152.0f, 152.0f, \
                      208.0f, 208.0f, 208.0f, 208.0f);

        a *= b;

        VecFloat a00, a10, a20, a30;
        VecFloat a01, a11, a21, a31;
        VecFloat a02, a12, a22, a32;
        VecFloat a03, a13, a23, a33;
        a.GetElement(0, 0, a00);
        a.GetElement(1, 0, a10);
        a.GetElement(2, 0, a20);
        a.GetElement(3, 0, a30);
        a.GetElement(0, 1, a01);
        a.GetElement(1, 1, a11);
        a.GetElement(2, 1, a21);
        a.GetElement(3, 1, a31);
        a.GetElement(0, 2, a02);
        a.GetElement(1, 2, a12);
        a.GetElement(2, 2, a22);
        a.GetElement(3, 2, a32);
        a.GetElement(0, 3, a03);
        a.GetElement(1, 3, a13);
        a.GetElement(2, 3, a23);
        a.GetElement(3, 3, a33);

        VecFloat e00, e10, e20, e30;
        VecFloat e01, e11, e21, e31;
        VecFloat e02, e12, e22, e32;
        VecFloat e03, e13, e23, e33;
        exRes.GetElement(0, 0, e00);
        exRes.GetElement(1, 0, e10);
        exRes.GetElement(2, 0, e20);
        exRes.GetElement(3, 0, e30);
        exRes.GetElement(0, 1, e01);
        exRes.GetElement(1, 1, e11);
        exRes.GetElement(2, 1, e21);
        exRes.GetElement(3, 1, e31);
        exRes.GetElement(0, 2, e02);
        exRes.GetElement(1, 2, e12);
        exRes.GetElement(2, 2, e22);
        exRes.GetElement(3, 2, e32);
        exRes.GetElement(0, 3, e03);
        exRes.GetElement(1, 3, e13);
        exRes.GetElement(2, 3, e23);
        exRes.GetElement(3, 3, e33);

        // Test the elements in the expected matrix match what was actually found after the matrix multiplication.
        const VecFloat delta = 0.01f;
        TS_ASSERT_DELTA(a00, e00, delta);
        TS_ASSERT_DELTA(a10, e10, delta);
        TS_ASSERT_DELTA(a20, e20, delta);
        TS_ASSERT_DELTA(a30, e30, delta);
        TS_ASSERT_DELTA(a01, e01, delta);
        TS_ASSERT_DELTA(a11, e11, delta);
        TS_ASSERT_DELTA(a21, e21, delta);
        TS_ASSERT_DELTA(a31, e31, delta);
        TS_ASSERT_DELTA(a02, e02, delta);
        TS_ASSERT_DELTA(a12, e12, delta);
        TS_ASSERT_DELTA(a22, e22, delta);
        TS_ASSERT_DELTA(a32, e32, delta);
        TS_ASSERT_DELTA(a03, e03, delta);
        TS_ASSERT_DELTA(a13, e13, delta);
        TS_ASSERT_DELTA(a23, e23, delta);
        TS_ASSERT_DELTA(a33, e33, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::operator*(Vector4)
    //
    // /////////////////////////////////////////////////////////////////
    void testMultiplyVector(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        Matrix4 a(1.0f, 2.0f, 1.0f, 2.0f, \
                  3.0f, 4.0f, 4.0f, 3.0f, \
                  2.0f, 1.0f, 2.0f, 1.0f, \
                  4.0f, 3.0f, 3.0f, 4.0f);
        Vector4 inVec(2.0f, 3.0f, 1.0f, 0.0f);

        Vector4 exVec(9.0f, 22.0f, 9.0f, 20.0f), acVec;

        acVec = a * inVec;

        TS_ASSERT(acVec == exVec);
        TS_ASSERT(!(acVec == inVec));
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::operator=()
    //
    // /////////////////////////////////////////////////////////////////
    void testAssignment(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        Matrix4 a(1.0f, 2.0f, 3.0f, 4.0f, \
                  5.0f, 6.0f, 7.0f, 8.0f, \
                  9.0f, 10.0f, 11.0f, 12.0f, \
                  13.0f, 14.0f, 15.0f, 16.0f);
        Matrix4 copya = a;

        VecFloat a00, a10, a20, a30;
        VecFloat a01, a11, a21, a31;
        VecFloat a02, a12, a22, a32;
        VecFloat a03, a13, a23, a33;
        a.GetElement(0, 0, a00);
        a.GetElement(1, 0, a10);
        a.GetElement(2, 0, a20);
        a.GetElement(3, 0, a30);
        a.GetElement(0, 1, a01);
        a.GetElement(1, 1, a11);
        a.GetElement(2, 1, a21);
        a.GetElement(3, 1, a31);
        a.GetElement(0, 2, a02);
        a.GetElement(1, 2, a12);
        a.GetElement(2, 2, a22);
        a.GetElement(3, 2, a32);
        a.GetElement(0, 3, a03);
        a.GetElement(1, 3, a13);
        a.GetElement(2, 3, a23);
        a.GetElement(3, 3, a33);

        VecFloat e00, e10, e20, e30;
        VecFloat e01, e11, e21, e31;
        VecFloat e02, e12, e22, e32;
        VecFloat e03, e13, e23, e33;
        copya.GetElement(0, 0, e00);
        copya.GetElement(1, 0, e10);
        copya.GetElement(2, 0, e20);
        copya.GetElement(3, 0, e30);
        copya.GetElement(0, 1, e01);
        copya.GetElement(1, 1, e11);
        copya.GetElement(2, 1, e21);
        copya.GetElement(3, 1, e31);
        copya.GetElement(0, 2, e02);
        copya.GetElement(1, 2, e12);
        copya.GetElement(2, 2, e22);
        copya.GetElement(3, 2, e32);
        copya.GetElement(0, 3, e03);
        copya.GetElement(1, 3, e13);
        copya.GetElement(2, 3, e23);
        copya.GetElement(3, 3, e33);

        // Test the elements  in both matrices match.
        const VecFloat delta = 0.01f;
        TS_ASSERT_DELTA(a00, e00, delta);
        TS_ASSERT_DELTA(a10, e10, delta);
        TS_ASSERT_DELTA(a20, e20, delta);
        TS_ASSERT_DELTA(a30, e30, delta);
        TS_ASSERT_DELTA(a01, e01, delta);
        TS_ASSERT_DELTA(a11, e11, delta);
        TS_ASSERT_DELTA(a21, e21, delta);
        TS_ASSERT_DELTA(a31, e31, delta);
        TS_ASSERT_DELTA(a02, e02, delta);
        TS_ASSERT_DELTA(a12, e12, delta);
        TS_ASSERT_DELTA(a22, e22, delta);
        TS_ASSERT_DELTA(a32, e32, delta);
        TS_ASSERT_DELTA(a03, e03, delta);
        TS_ASSERT_DELTA(a13, e13, delta);
        TS_ASSERT_DELTA(a23, e23, delta);
        TS_ASSERT_DELTA(a33, e33, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::GetElement()
    //
    // /////////////////////////////////////////////////////////////////
    void testGetElement(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat s = 67.9f;
        Matrix4 a(s);

        VecFloat elem = 0.0f;

        const VecFloat delta = 0.01f;
        TS_ASSERT(a.GetElement(0, 0, elem));
        TS_ASSERT_DELTA(elem, s, delta);

        TS_ASSERT(!a.GetElement(4, 5, elem));
        TS_ASSERT(!a.GetElement(5, 4, elem));
        TS_ASSERT(!a.GetElement(4, 3, elem));
        TS_ASSERT(!a.GetElement(3, 4, elem));
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::SetElement()
    //
    // /////////////////////////////////////////////////////////////////
    void testSetElement(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat s = 67.9f;
        Matrix4 a(s);

        VecFloat elem = 0.0f;
        const VecFloat delta = 0.01f;
        VecFloat nv = 32.87f;
        TS_ASSERT(a.SetElement(0, 0, nv));
        TS_ASSERT(a.GetElement(0, 0, elem));
        TS_ASSERT_DELTA(elem, nv, delta);

        TS_ASSERT(!a.SetElement(4, 5, 0.0f));
        TS_ASSERT(!a.SetElement(5, 4, 0.0f));
        TS_ASSERT(!a.SetElement(4, 3, 0.0f));
        TS_ASSERT(!a.SetElement(3, 4, 0.0f));
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::ToString()
    //
    // /////////////////////////////////////////////////////////////////
    void testToString(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat s = 67.9f;
        Matrix4 a(s);
        std::string exStr("|\t67.9\t67.9\t67.9\t67.9\t|\n|\t67.9\t67.9\t67.9\t67.9\t|\n|\t67.9\t67.9\t67.9\t67.9\t|\n|\t67.9\t67.9\t67.9\t67.9\t|");
        std::string acStr = a.ToString();

        //TS_WARN(exStr.c_str());
        //TS_WARN(acStr.c_str());
        TS_ASSERT_EQUALS(acStr.compare(exStr.c_str()), 0);
    };

    // /////////////////////////////////////////////////////////////////
    // Test Matrix4::operator<<()
    //
    // /////////////////////////////////////////////////////////////////
    void testOperatorStreamOutput(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        // Not implemented... If ToString() is okay, then this is okay too...
    };

    // /////////////////////////////////////////////////////////////////
    // Test global matrices are of the expected values.
    //
    // /////////////////////////////////////////////////////////////////
    void testGlobalMatrix4(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat a00, a10, a20, a30;
        VecFloat a01, a11, a21, a31;
        VecFloat a02, a12, a22, a32;
        VecFloat a03, a13, a23, a33;
        const VecFloat delta = 0.01f;

        // Test zero matrix has the expected values.
        GameHalloran::g_zeroMat.GetElement(0, 0, a00);
        GameHalloran::g_zeroMat.GetElement(1, 0, a10);
        GameHalloran::g_zeroMat.GetElement(2, 0, a20);
        GameHalloran::g_zeroMat.GetElement(3, 0, a30);
        GameHalloran::g_zeroMat.GetElement(0, 1, a01);
        GameHalloran::g_zeroMat.GetElement(1, 1, a11);
        GameHalloran::g_zeroMat.GetElement(2, 1, a21);
        GameHalloran::g_zeroMat.GetElement(3, 1, a31);
        GameHalloran::g_zeroMat.GetElement(0, 2, a02);
        GameHalloran::g_zeroMat.GetElement(1, 2, a12);
        GameHalloran::g_zeroMat.GetElement(2, 2, a22);
        GameHalloran::g_zeroMat.GetElement(3, 2, a32);
        GameHalloran::g_zeroMat.GetElement(0, 3, a03);
        GameHalloran::g_zeroMat.GetElement(1, 3, a13);
        GameHalloran::g_zeroMat.GetElement(2, 3, a23);
        GameHalloran::g_zeroMat.GetElement(3, 3, a33);
        TS_ASSERT_DELTA(a00, 0.0f, delta);
        TS_ASSERT_DELTA(a10, 0.0f, delta);
        TS_ASSERT_DELTA(a20, 0.0f, delta);
        TS_ASSERT_DELTA(a30, 0.0f, delta);
        TS_ASSERT_DELTA(a01, 0.0f, delta);
        TS_ASSERT_DELTA(a11, 0.0f, delta);
        TS_ASSERT_DELTA(a21, 0.0f, delta);
        TS_ASSERT_DELTA(a31, 0.0f, delta);
        TS_ASSERT_DELTA(a02, 0.0f, delta);
        TS_ASSERT_DELTA(a12, 0.0f, delta);
        TS_ASSERT_DELTA(a22, 0.0f, delta);
        TS_ASSERT_DELTA(a32, 0.0f, delta);
        TS_ASSERT_DELTA(a03, 0.0f, delta);
        TS_ASSERT_DELTA(a13, 0.0f, delta);
        TS_ASSERT_DELTA(a23, 0.0f, delta);
        TS_ASSERT_DELTA(a33, 0.0f, delta);

        // Test identity matrix has the expected values.
        GameHalloran::g_identityMat.GetElement(0, 0, a00);
        GameHalloran::g_identityMat.GetElement(1, 0, a10);
        GameHalloran::g_identityMat.GetElement(2, 0, a20);
        GameHalloran::g_identityMat.GetElement(3, 0, a30);
        GameHalloran::g_identityMat.GetElement(0, 1, a01);
        GameHalloran::g_identityMat.GetElement(1, 1, a11);
        GameHalloran::g_identityMat.GetElement(2, 1, a21);
        GameHalloran::g_identityMat.GetElement(3, 1, a31);
        GameHalloran::g_identityMat.GetElement(0, 2, a02);
        GameHalloran::g_identityMat.GetElement(1, 2, a12);
        GameHalloran::g_identityMat.GetElement(2, 2, a22);
        GameHalloran::g_identityMat.GetElement(3, 2, a32);
        GameHalloran::g_identityMat.GetElement(0, 3, a03);
        GameHalloran::g_identityMat.GetElement(1, 3, a13);
        GameHalloran::g_identityMat.GetElement(2, 3, a23);
        GameHalloran::g_identityMat.GetElement(3, 3, a33);
        TS_ASSERT_DELTA(a00, 1.0f, delta);
        TS_ASSERT_DELTA(a10, 0.0f, delta);
        TS_ASSERT_DELTA(a20, 0.0f, delta);
        TS_ASSERT_DELTA(a30, 0.0f, delta);
        TS_ASSERT_DELTA(a01, 0.0f, delta);
        TS_ASSERT_DELTA(a11, 1.0f, delta);
        TS_ASSERT_DELTA(a21, 0.0f, delta);
        TS_ASSERT_DELTA(a31, 0.0f, delta);
        TS_ASSERT_DELTA(a02, 0.0f, delta);
        TS_ASSERT_DELTA(a12, 0.0f, delta);
        TS_ASSERT_DELTA(a22, 1.0f, delta);
        TS_ASSERT_DELTA(a32, 0.0f, delta);
        TS_ASSERT_DELTA(a03, 0.0f, delta);
        TS_ASSERT_DELTA(a13, 0.0f, delta);
        TS_ASSERT_DELTA(a23, 0.0f, delta);
        TS_ASSERT_DELTA(a33, 1.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test BuildRotationArbitruaryMatrix4().
    //
    // /////////////////////////////////////////////////////////////////
    void testBuildArbitruaryRotation(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        //Matrix4 test;
        //GameHalloran::BuildRotationArbitraryMatrix4(test, 90, 0.0f, 1.0f, 0.0f);
        //TS_WARN(test.ToString().c_str());
        //Point3 pt(1.0f, 10.0f, -2.0f);
        //Vector4 vec4(pt);
        //Vector4 outPt = test * vec4;
        //TS_WARN(outPt.ToString().c_str());

        // Build rotations about the x, y and z axes as these cases are easy to test for,
        // if they match for these specific cases then the function is *probably* working for all cases...

        const VecFloat angle = 90.0f;
        VecFloat a00, a10, a20, a30;
        VecFloat a01, a11, a21, a31;
        VecFloat a02, a12, a22, a32;
        VecFloat a03, a13, a23, a33;
        const VecFloat delta = 0.01f;
        Matrix4 out;

        // 1) Rotation about the X axis.
        {
            GameHalloran::BuildRotationArbitraryMatrix4(out, angle, 1.0f, 0.0f, 0.0f);
            out.GetElement(0, 0, a00);
            out.GetElement(1, 0, a10);
            out.GetElement(2, 0, a20);
            out.GetElement(3, 0, a30);
            out.GetElement(0, 1, a01);
            out.GetElement(1, 1, a11);
            out.GetElement(2, 1, a21);
            out.GetElement(3, 1, a31);
            out.GetElement(0, 2, a02);
            out.GetElement(1, 2, a12);
            out.GetElement(2, 2, a22);
            out.GetElement(3, 2, a32);
            out.GetElement(0, 3, a03);
            out.GetElement(1, 3, a13);
            out.GetElement(2, 3, a23);
            out.GetElement(3, 3, a33);
            TS_ASSERT_DELTA(a00, 1.0f, delta);
            TS_ASSERT_DELTA(a10, 0.0f, delta);
            TS_ASSERT_DELTA(a20, 0.0f, delta);
            TS_ASSERT_DELTA(a30, 0.0f, delta);
            TS_ASSERT_DELTA(a01, 0.0f, delta);
            TS_ASSERT_DELTA(a11, cos(m3dDegToRad(angle)), delta);
            TS_ASSERT_DELTA(a21, -sin(m3dDegToRad(angle)), delta);
            TS_ASSERT_DELTA(a31, 0.0f, delta);
            TS_ASSERT_DELTA(a02, 0.0f, delta);
            TS_ASSERT_DELTA(a12, sin(m3dDegToRad(angle)), delta);
            TS_ASSERT_DELTA(a22, cos(m3dDegToRad(angle)), delta);
            TS_ASSERT_DELTA(a32, 0.0f, delta);
            TS_ASSERT_DELTA(a03, 0.0f, delta);
            TS_ASSERT_DELTA(a13, 0.0f, delta);
            TS_ASSERT_DELTA(a23, 0.0f, delta);
            TS_ASSERT_DELTA(a33, 1.0f, delta);
        }

        out.ZeroMatrix();

        // 2) Rotation about the Y axis.
        {
            GameHalloran::BuildRotationArbitraryMatrix4(out, angle, 0.0f, 1.0f, 0.0f);
            out.GetElement(0, 0, a00);
            out.GetElement(1, 0, a10);
            out.GetElement(2, 0, a20);
            out.GetElement(3, 0, a30);
            out.GetElement(0, 1, a01);
            out.GetElement(1, 1, a11);
            out.GetElement(2, 1, a21);
            out.GetElement(3, 1, a31);
            out.GetElement(0, 2, a02);
            out.GetElement(1, 2, a12);
            out.GetElement(2, 2, a22);
            out.GetElement(3, 2, a32);
            out.GetElement(0, 3, a03);
            out.GetElement(1, 3, a13);
            out.GetElement(2, 3, a23);
            out.GetElement(3, 3, a33);
            TS_ASSERT_DELTA(a00, cos(m3dDegToRad(angle)), delta);
            TS_ASSERT_DELTA(a10, 0.0f, delta);
            TS_ASSERT_DELTA(a20, sin(m3dDegToRad(angle)), delta);
            TS_ASSERT_DELTA(a30, 0.0f, delta);
            TS_ASSERT_DELTA(a01, 0.0f, delta);
            TS_ASSERT_DELTA(a11, 1.0f, delta);
            TS_ASSERT_DELTA(a21, 0.0f, delta);
            TS_ASSERT_DELTA(a31, 0.0f, delta);
            TS_ASSERT_DELTA(a02, -sin(m3dDegToRad(angle)), delta);
            TS_ASSERT_DELTA(a12, 0.0f, delta);
            TS_ASSERT_DELTA(a22, cos(m3dDegToRad(angle)), delta);
            TS_ASSERT_DELTA(a32, 0.0f, delta);
            TS_ASSERT_DELTA(a03, 0.0f, delta);
            TS_ASSERT_DELTA(a13, 0.0f, delta);
            TS_ASSERT_DELTA(a23, 0.0f, delta);
            TS_ASSERT_DELTA(a33, 1.0f, delta);
        }

        out.ZeroMatrix();

        // 3) Rotation about the Z axis.
        {
            GameHalloran::BuildRotationArbitraryMatrix4(out, angle, 0.0f, 0.0f, 1.0f);
            out.GetElement(0, 0, a00);
            out.GetElement(1, 0, a10);
            out.GetElement(2, 0, a20);
            out.GetElement(3, 0, a30);
            out.GetElement(0, 1, a01);
            out.GetElement(1, 1, a11);
            out.GetElement(2, 1, a21);
            out.GetElement(3, 1, a31);
            out.GetElement(0, 2, a02);
            out.GetElement(1, 2, a12);
            out.GetElement(2, 2, a22);
            out.GetElement(3, 2, a32);
            out.GetElement(0, 3, a03);
            out.GetElement(1, 3, a13);
            out.GetElement(2, 3, a23);
            out.GetElement(3, 3, a33);
            TS_ASSERT_DELTA(a00, cos(m3dDegToRad(angle)), delta);
            TS_ASSERT_DELTA(a10, -sin(m3dDegToRad(angle)), delta);
            TS_ASSERT_DELTA(a20, 0.0f, delta);
            TS_ASSERT_DELTA(a30, 0.0f, delta);
            TS_ASSERT_DELTA(a01, sin(m3dDegToRad(angle)), delta);
            TS_ASSERT_DELTA(a11, cos(m3dDegToRad(angle)), delta);
            TS_ASSERT_DELTA(a21, 0.0f, delta);
            TS_ASSERT_DELTA(a31, 0.0f, delta);
            TS_ASSERT_DELTA(a02, 0.0f, delta);
            TS_ASSERT_DELTA(a12, 0.0f, delta);
            TS_ASSERT_DELTA(a22, 1.0f, delta);
            TS_ASSERT_DELTA(a32, 0.0f, delta);
            TS_ASSERT_DELTA(a03, 0.0f, delta);
            TS_ASSERT_DELTA(a13, 0.0f, delta);
            TS_ASSERT_DELTA(a23, 0.0f, delta);
            TS_ASSERT_DELTA(a33, 1.0f, delta);
        }
    };

    // /////////////////////////////////////////////////////////////////
    // Test BuildRotationXMatrix4().
    //
    // /////////////////////////////////////////////////////////////////
    void testBuildXRotation(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        Matrix4 out;
        VecFloat angle = 90.0f;
        GameHalloran::BuildRotationXMatrix4(out, angle);
        //TS_WARN(out.ToString().c_str());
        //Point3 pt(1.0f, 10.0f, -2.0f);
        //Vector4 vec4(pt);
        //Vector4 outPt = out * vec4;
        //TS_WARN(outPt.ToString().c_str());

        VecFloat a00, a10, a20, a30;
        VecFloat a01, a11, a21, a31;
        VecFloat a02, a12, a22, a32;
        VecFloat a03, a13, a23, a33;
        const VecFloat delta = 0.01f;
        out.GetElement(0, 0, a00);
        out.GetElement(1, 0, a10);
        out.GetElement(2, 0, a20);
        out.GetElement(3, 0, a30);
        out.GetElement(0, 1, a01);
        out.GetElement(1, 1, a11);
        out.GetElement(2, 1, a21);
        out.GetElement(3, 1, a31);
        out.GetElement(0, 2, a02);
        out.GetElement(1, 2, a12);
        out.GetElement(2, 2, a22);
        out.GetElement(3, 2, a32);
        out.GetElement(0, 3, a03);
        out.GetElement(1, 3, a13);
        out.GetElement(2, 3, a23);
        out.GetElement(3, 3, a33);
        TS_ASSERT_DELTA(a00, 1.0f, delta);
        TS_ASSERT_DELTA(a10, 0.0f, delta);
        TS_ASSERT_DELTA(a20, 0.0f, delta);
        TS_ASSERT_DELTA(a30, 0.0f, delta);
        TS_ASSERT_DELTA(a01, 0.0f, delta);
        TS_ASSERT_DELTA(a11, cos(m3dDegToRad(angle)), delta);
        TS_ASSERT_DELTA(a21, -sin(m3dDegToRad(angle)), delta);
        TS_ASSERT_DELTA(a31, 0.0f, delta);
        TS_ASSERT_DELTA(a02, 0.0f, delta);
        TS_ASSERT_DELTA(a12, sin(m3dDegToRad(angle)), delta);
        TS_ASSERT_DELTA(a22, cos(m3dDegToRad(angle)), delta);
        TS_ASSERT_DELTA(a32, 0.0f, delta);
        TS_ASSERT_DELTA(a03, 0.0f, delta);
        TS_ASSERT_DELTA(a13, 0.0f, delta);
        TS_ASSERT_DELTA(a23, 0.0f, delta);
        TS_ASSERT_DELTA(a33, 1.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test BuildRotationYMatrix4().
    //
    // /////////////////////////////////////////////////////////////////
    void testBuildYRotation(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat angle = 90.0f;
        Matrix4 out;
        GameHalloran::BuildRotationYMatrix4(out, angle);
        //TS_WARN(out.ToString().c_str());
        //Point3 pt(1.0f, 10.0f, -2.0f);
        //Vector4 vec4(pt);
        //Vector4 outPt = out * vec4;
        //TS_WARN(outPt.ToString().c_str());

        VecFloat a00, a10, a20, a30;
        VecFloat a01, a11, a21, a31;
        VecFloat a02, a12, a22, a32;
        VecFloat a03, a13, a23, a33;
        const VecFloat delta = 0.01f;
        out.GetElement(0, 0, a00);
        out.GetElement(1, 0, a10);
        out.GetElement(2, 0, a20);
        out.GetElement(3, 0, a30);
        out.GetElement(0, 1, a01);
        out.GetElement(1, 1, a11);
        out.GetElement(2, 1, a21);
        out.GetElement(3, 1, a31);
        out.GetElement(0, 2, a02);
        out.GetElement(1, 2, a12);
        out.GetElement(2, 2, a22);
        out.GetElement(3, 2, a32);
        out.GetElement(0, 3, a03);
        out.GetElement(1, 3, a13);
        out.GetElement(2, 3, a23);
        out.GetElement(3, 3, a33);
        TS_ASSERT_DELTA(a00, cos(m3dDegToRad(angle)), delta);
        TS_ASSERT_DELTA(a10, 0.0f, delta);
        TS_ASSERT_DELTA(a20, sin(m3dDegToRad(angle)), delta);
        TS_ASSERT_DELTA(a30, 0.0f, delta);
        TS_ASSERT_DELTA(a01, 0.0f, delta);
        TS_ASSERT_DELTA(a11, 1.0f, delta);
        TS_ASSERT_DELTA(a21, 0.0f, delta);
        TS_ASSERT_DELTA(a31, 0.0f, delta);
        TS_ASSERT_DELTA(a02, -sin(m3dDegToRad(angle)), delta);
        TS_ASSERT_DELTA(a12, 0.0f, delta);
        TS_ASSERT_DELTA(a22, cos(m3dDegToRad(angle)), delta);
        TS_ASSERT_DELTA(a32, 0.0f, delta);
        TS_ASSERT_DELTA(a03, 0.0f, delta);
        TS_ASSERT_DELTA(a13, 0.0f, delta);
        TS_ASSERT_DELTA(a23, 0.0f, delta);
        TS_ASSERT_DELTA(a33, 1.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test BuildRotationZMatrix4().
    //
    // /////////////////////////////////////////////////////////////////
    void testBuildZRotation(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat angle = 90.0f;
        Matrix4 out;
        GameHalloran::BuildRotationZMatrix4(out, angle);
        //TS_WARN(out.ToString().c_str());
        //Point3 pt(1.0f, 10.0f, -2.0f);
        //Vector4 vec4(pt);
        //Vector4 outPt = out * vec4;
        //TS_WARN(outPt.ToString().c_str());

        VecFloat a00, a10, a20, a30;
        VecFloat a01, a11, a21, a31;
        VecFloat a02, a12, a22, a32;
        VecFloat a03, a13, a23, a33;
        const VecFloat delta = 0.01f;
        out.GetElement(0, 0, a00);
        out.GetElement(1, 0, a10);
        out.GetElement(2, 0, a20);
        out.GetElement(3, 0, a30);
        out.GetElement(0, 1, a01);
        out.GetElement(1, 1, a11);
        out.GetElement(2, 1, a21);
        out.GetElement(3, 1, a31);
        out.GetElement(0, 2, a02);
        out.GetElement(1, 2, a12);
        out.GetElement(2, 2, a22);
        out.GetElement(3, 2, a32);
        out.GetElement(0, 3, a03);
        out.GetElement(1, 3, a13);
        out.GetElement(2, 3, a23);
        out.GetElement(3, 3, a33);
        TS_ASSERT_DELTA(a00, cos(m3dDegToRad(angle)), delta);
        TS_ASSERT_DELTA(a10, -sin(m3dDegToRad(angle)), delta);
        TS_ASSERT_DELTA(a20, 0.0f, delta);
        TS_ASSERT_DELTA(a30, 0.0f, delta);
        TS_ASSERT_DELTA(a01, sin(m3dDegToRad(angle)), delta);
        TS_ASSERT_DELTA(a11, cos(m3dDegToRad(angle)), delta);
        TS_ASSERT_DELTA(a21, 0.0f, delta);
        TS_ASSERT_DELTA(a31, 0.0f, delta);
        TS_ASSERT_DELTA(a02, 0.0f, delta);
        TS_ASSERT_DELTA(a12, 0.0f, delta);
        TS_ASSERT_DELTA(a22, 1.0f, delta);
        TS_ASSERT_DELTA(a32, 0.0f, delta);
        TS_ASSERT_DELTA(a03, 0.0f, delta);
        TS_ASSERT_DELTA(a13, 0.0f, delta);
        TS_ASSERT_DELTA(a23, 0.0f, delta);
        TS_ASSERT_DELTA(a33, 1.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test BuildTranslationMatrix4().
    //
    // /////////////////////////////////////////////////////////////////
    void testBuildTranslation(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat x = 2.0f, y = 5.0f, z = -65.9f;
        Matrix4 out;
        GameHalloran::BuildTranslationMatrix4(out, x, y, z);
        //TS_WARN(out.ToString().c_str());

        VecFloat a00, a10, a20, a30;
        VecFloat a01, a11, a21, a31;
        VecFloat a02, a12, a22, a32;
        VecFloat a03, a13, a23, a33;
        const VecFloat delta = 0.01f;
        out.GetElement(0, 0, a00);
        out.GetElement(1, 0, a10);
        out.GetElement(2, 0, a20);
        out.GetElement(3, 0, a30);
        out.GetElement(0, 1, a01);
        out.GetElement(1, 1, a11);
        out.GetElement(2, 1, a21);
        out.GetElement(3, 1, a31);
        out.GetElement(0, 2, a02);
        out.GetElement(1, 2, a12);
        out.GetElement(2, 2, a22);
        out.GetElement(3, 2, a32);
        out.GetElement(0, 3, a03);
        out.GetElement(1, 3, a13);
        out.GetElement(2, 3, a23);
        out.GetElement(3, 3, a33);
        TS_ASSERT_DELTA(a00, 1.0f, delta);
        TS_ASSERT_DELTA(a10, 0.0f, delta);
        TS_ASSERT_DELTA(a20, 0.0f, delta);
        TS_ASSERT_DELTA(a30, x, delta);
        TS_ASSERT_DELTA(a01, 0.0f, delta);
        TS_ASSERT_DELTA(a11, 1.0f, delta);
        TS_ASSERT_DELTA(a21, 0.0f, delta);
        TS_ASSERT_DELTA(a31, y, delta);
        TS_ASSERT_DELTA(a02, 0.0f, delta);
        TS_ASSERT_DELTA(a12, 0.0f, delta);
        TS_ASSERT_DELTA(a22, 1.0f, delta);
        TS_ASSERT_DELTA(a32, z, delta);
        TS_ASSERT_DELTA(a03, 0.0f, delta);
        TS_ASSERT_DELTA(a13, 0.0f, delta);
        TS_ASSERT_DELTA(a23, 0.0f, delta);
        TS_ASSERT_DELTA(a33, 1.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test BuildScaleMatrix4().
    //
    // /////////////////////////////////////////////////////////////////
    void testBuildScale(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat x = 2.0f, y = 5.0f, z = -65.9f;
        Matrix4 out;
        GameHalloran::BuildScaleMatrix4(out, x, y, z);
        //TS_WARN(out.ToString().c_str());

        VecFloat a00, a10, a20, a30;
        VecFloat a01, a11, a21, a31;
        VecFloat a02, a12, a22, a32;
        VecFloat a03, a13, a23, a33;
        const VecFloat delta = 0.01f;
        out.GetElement(0, 0, a00);
        out.GetElement(1, 0, a10);
        out.GetElement(2, 0, a20);
        out.GetElement(3, 0, a30);
        out.GetElement(0, 1, a01);
        out.GetElement(1, 1, a11);
        out.GetElement(2, 1, a21);
        out.GetElement(3, 1, a31);
        out.GetElement(0, 2, a02);
        out.GetElement(1, 2, a12);
        out.GetElement(2, 2, a22);
        out.GetElement(3, 2, a32);
        out.GetElement(0, 3, a03);
        out.GetElement(1, 3, a13);
        out.GetElement(2, 3, a23);
        out.GetElement(3, 3, a33);
        TS_ASSERT_DELTA(a00, x, delta);
        TS_ASSERT_DELTA(a10, 0.0f, delta);
        TS_ASSERT_DELTA(a20, 0.0f, delta);
        TS_ASSERT_DELTA(a30, 0.0f, delta);
        TS_ASSERT_DELTA(a01, 0.0f, delta);
        TS_ASSERT_DELTA(a11, y, delta);
        TS_ASSERT_DELTA(a21, 0.0f, delta);
        TS_ASSERT_DELTA(a31, 0.0f, delta);
        TS_ASSERT_DELTA(a02, 0.0f, delta);
        TS_ASSERT_DELTA(a12, 0.0f, delta);
        TS_ASSERT_DELTA(a22, z, delta);
        TS_ASSERT_DELTA(a32, 0.0f, delta);
        TS_ASSERT_DELTA(a03, 0.0f, delta);
        TS_ASSERT_DELTA(a13, 0.0f, delta);
        TS_ASSERT_DELTA(a23, 0.0f, delta);
        TS_ASSERT_DELTA(a33, 1.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test BuildScaleArbitruaryMatrix4().
    //
    // /////////////////////////////////////////////////////////////////
    void testBuildScaleArbitruary(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        VecFloat x = 1.0f, y = 2.0f, z = 3.0f;
        Matrix4 out;
        Point3 pt(45.0f, 45.0f, 10.0f);
        GameHalloran::BuildScaleArbitraryMatrix4(out, x, y, z, pt);
        //TS_WARN(out.ToString().c_str());

        VecFloat a00, a10, a20, a30;
        VecFloat a01, a11, a21, a31;
        VecFloat a02, a12, a22, a32;
        VecFloat a03, a13, a23, a33;
        const VecFloat delta = 0.01f;
        out.GetElement(0, 0, a00);
        out.GetElement(1, 0, a10);
        out.GetElement(2, 0, a20);
        out.GetElement(3, 0, a30);
        out.GetElement(0, 1, a01);
        out.GetElement(1, 1, a11);
        out.GetElement(2, 1, a21);
        out.GetElement(3, 1, a31);
        out.GetElement(0, 2, a02);
        out.GetElement(1, 2, a12);
        out.GetElement(2, 2, a22);
        out.GetElement(3, 2, a32);
        out.GetElement(0, 3, a03);
        out.GetElement(1, 3, a13);
        out.GetElement(2, 3, a23);
        out.GetElement(3, 3, a33);
        TS_ASSERT_DELTA(a00, x, delta);
        TS_ASSERT_DELTA(a10, 0.0f, delta);
        TS_ASSERT_DELTA(a20, 0.0f, delta);
        TS_ASSERT_DELTA(a30, pt.GetX() * (1 - x), delta);
        TS_ASSERT_DELTA(a01, 0.0f, delta);
        TS_ASSERT_DELTA(a11, y, delta);
        TS_ASSERT_DELTA(a21, 0.0f, delta);
        TS_ASSERT_DELTA(a31, pt.GetY() * (1 - y), delta);
        TS_ASSERT_DELTA(a02, 0.0f, delta);
        TS_ASSERT_DELTA(a12, 0.0f, delta);
        TS_ASSERT_DELTA(a22, z, delta);
        TS_ASSERT_DELTA(a32, pt.GetZ() * (1 - z), delta);
        TS_ASSERT_DELTA(a03, 0.0f, delta);
        TS_ASSERT_DELTA(a13, 0.0f, delta);
        TS_ASSERT_DELTA(a23, 0.0f, delta);
        TS_ASSERT_DELTA(a33, 1.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test BuildLookAt().
    //
    // /////////////////////////////////////////////////////////////////
    void testBuildLookAt(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        Matrix4 out;
        Point3 eye(0.0f, 0.0f, 0.0f), at(2.0f, 5.0f, -10.0f);
        Vector3 up(0.0f, 1.0f, 0.0f);
        GameHalloran::BuildLookAt(out, eye, at, up);
        //TS_WARN(out.ToString().c_str());

        Vector3 upCopy(up);
        upCopy.Normalize();
        Vector3 dir(at - eye);
        dir.Normalize();
        //TS_WARN(dir.ToString().c_str());
        Vector3 s, u;
        dir.Cross(upCopy, s);
        s.Cross(dir, u);

        //TS_WARN(t.ToString().c_str());
        //TS_WARN(s.ToString().c_str());
        //TS_WARN(u.ToString().c_str());

        const VecFloat delta = 0.01f;

        TS_ASSERT_DELTA(out[Matrix4::M00], s.GetX(), delta);
        TS_ASSERT_DELTA(out[Matrix4::M01], s.GetY(), delta);
        TS_ASSERT_DELTA(out[Matrix4::M02], s.GetZ(), delta);
        TS_ASSERT_DELTA(out[Matrix4::M03], 0.0f, delta);

        TS_ASSERT_DELTA(out[Matrix4::M10], u.GetX(), delta);
        TS_ASSERT_DELTA(out[Matrix4::M11], u.GetY(), delta);
        TS_ASSERT_DELTA(out[Matrix4::M12], u.GetZ(), delta);
        TS_ASSERT_DELTA(out[Matrix4::M13], 0.0f, delta);

        TS_ASSERT_DELTA(out[Matrix4::M20], -dir.GetX(), delta);
        TS_ASSERT_DELTA(out[Matrix4::M21], -dir.GetY(), delta);
        TS_ASSERT_DELTA(out[Matrix4::M22], -dir.GetZ(), delta);
        TS_ASSERT_DELTA(out[Matrix4::M23], 0.0f, delta);

        TS_ASSERT_DELTA(out[Matrix4::M30], eye.GetX(), delta);
        TS_ASSERT_DELTA(out[Matrix4::M31], eye.GetY(), delta);
        TS_ASSERT_DELTA(out[Matrix4::M32], eye.GetZ(), delta);
        TS_ASSERT_DELTA(out[Matrix4::M33], 1.0f, delta);
    };

    // /////////////////////////////////////////////////////////////////
    // Test BuildPerspectiveFov().
    //
    // /////////////////////////////////////////////////////////////////
    void testBuildPerspectiveFov(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }


        // The reason this method is not comprehensively unit tested is that I dont 100% understand the method used
        //  to generate the matrix in this case so until i do i will just print it out to the test results command line.

        Matrix4 out;
        GameHalloran::BuildPerspectiveFov(out, 45.0f, 640.0f / 480.0f, 1.0f, 100.0f);
        //TS_WARN(out.ToString().c_str());
    };

    // /////////////////////////////////////////////////////////////////
    // Test BuildOrthographic().
    //
    // /////////////////////////////////////////////////////////////////
    void testBuildOrthographic(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }


        // The reason this method is not comprehensively unit tested is that I dont 100% understand the method used
        //  to generate the matrix in this case so until i do i will just print it out to the test results command line.

        Matrix4 out;
        GameHalloran::BuildOrthographic(out, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        //TS_WARN(out.ToString().c_str());
        Point3 pt(1.0f, 2.0f, -5.0f);
        Vector4 vec4(pt);
        Vector4 result = out * vec4;
        //TS_WARN(result.ToString().c_str());
    };

    // /////////////////////////////////////////////////////////////////
    // Test ExtractRotationMatrix().
    //
    // /////////////////////////////////////////////////////////////////
    void testExtractRotationMatrix(void) {
        if(!IsTestDataReady()) {
            TS_FAIL("Test data not created.");
        }

        GameHalloran::Matrix4 inputMat(1.0f, 2.0f, 3.0f, 4.0f, \
                                       5.0f, 6.0f, 7.0f, 8.0f, \
                                       9.0f, 10.0f, 11.0f, 12.0f, \
                                       13.0f, 14.0f, 15.0f, 16.0f);
        GameHalloran::Matrix3x3 outEx = {1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f};
        GameHalloran::Matrix3x3 outAc;

        GameHalloran::ExtractRotationMatrix(inputMat, outAc);
        const GameHalloran::VecFloat delta = 0.01f;
        for(int i = 0; i < 9; ++i) {
            TS_ASSERT_DELTA(outAc[i], outEx[i], delta);
        }
    };

};

#endif
