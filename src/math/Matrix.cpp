// /////////////////////////////////////////////////////////////////
// @file Matrix.cpp
// @author PJ O Halloran
// @date 25/08/2010
//
// File contains the implementations for the Matrix4 class.
// Please see the header file for more details.
//
// /////////////////////////////////////////////////////////////////


// External Headers
#include <string.h>

// Project Headers
#include "Matrix.h"


namespace GameHalloran
{

	// Declare commonly used global matrices.
	const Matrix4 g_identityMat(1.0f, 0.0f, 0.0f, 0.0f,\
								0.0f, 1.0f, 0.0f, 0.0f,\
								0.0f, 0.0f, 1.0f, 0.0f,\
								0.0f, 0.0f, 0.0f, 1.0f);
	const Matrix4 g_zeroMat(0.0f);

	// /////////////////////////////////////////////////////////////////
	// ************************* Matrix4 *******************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4::Matrix4()
	{
		memset(m_mat, 0, sizeof(F32) * NUMBER_ELEMENTS);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4::Matrix4(const F32 scaler)
	{
		for(I32 i = 0; i < NUMBER_ELEMENTS; ++i)
		{
			m_mat[i] = scaler;
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4::Matrix4(F32 matrixArr[NUMBER_ELEMENTS])
	{
		Set(matrixArr);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4::Matrix4(const F32 m00, const F32 m10, const F32 m20, const F32 m30,\
						const F32 m01, const F32 m11, const F32 m21, const F32 m31,\
						const F32 m02, const F32 m12, const F32 m22, const F32 m32,\
						const F32 m03, const F32 m13, const F32 m23, const F32 m33)
	{
		m_mat[M00] = m00;	m_mat[M10] = m10;	m_mat[M20] = m20;	m_mat[M30] = m30;
		m_mat[M01] = m01;	m_mat[M11] = m11;	m_mat[M21] = m21;	m_mat[M31] = m31;
		m_mat[M02] = m02;	m_mat[M12] = m12;	m_mat[M22] = m22;	m_mat[M32] = m32;
		m_mat[M03] = m03;	m_mat[M13] = m13;	m_mat[M23] = m23;	m_mat[M33] = m33;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4::Matrix4(const Vector3 &xDir, const Vector3 &yDir, const Vector3 &zDir, const Point3 &location)
	{
		m_mat[M00] = xDir.GetX();	m_mat[M10] = yDir.GetX();	m_mat[M20] = zDir.GetX();	m_mat[M30] = location.GetX();
		m_mat[M01] = xDir.GetY();	m_mat[M11] = yDir.GetY();	m_mat[M21] = zDir.GetY();	m_mat[M31] = location.GetY();
		m_mat[M02] = xDir.GetZ();	m_mat[M12] = yDir.GetZ();	m_mat[M22] = zDir.GetZ();	m_mat[M32] = location.GetZ();
		m_mat[M03] = 0.0f;			m_mat[M13] = 0.0f;			m_mat[M23] = 0.0f;			m_mat[M33] = 1.0f;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4::Matrix4(const Matrix4 &matRef)
	{
		Set(matRef.m_mat);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4::~Matrix4()
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Matrix4::LoadIdentity()
	{
		Set(g_identityMat.m_mat);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Matrix4::ZeroMatrix()
	{
		Set(g_zeroMat.m_mat);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Matrix4::Transpose()
	{
		Matrix4	tmp;
		Transposed(tmp);
		Set(tmp.m_mat);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Matrix4::Transposed(Matrix4 &outMatrix) const
	{
		outMatrix.m_mat[M00] = m_mat[M00];	outMatrix.m_mat[M10] = m_mat[M01];	outMatrix.m_mat[M20] = m_mat[M02];	outMatrix.m_mat[M30] = m_mat[M03];
		outMatrix.m_mat[M01] = m_mat[M10];	outMatrix.m_mat[M11] = m_mat[M11];	outMatrix.m_mat[M21] = m_mat[M12];	outMatrix.m_mat[M31] = m_mat[M13];
		outMatrix.m_mat[M02] = m_mat[M20];	outMatrix.m_mat[M12] = m_mat[M21];	outMatrix.m_mat[M22] = m_mat[M22];	outMatrix.m_mat[M32] = m_mat[M23];
		outMatrix.m_mat[M03] = m_mat[M30];	outMatrix.m_mat[M13] = m_mat[M31];	outMatrix.m_mat[M23] = m_mat[M32];	outMatrix.m_mat[M33] = m_mat[M33];
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	F32 Matrix4::Det3x3(const I32 i, const I32 j) const
	{
		I32 x, y;
		F32 ret, mat[3][3];

		// Create a 3x3 cofactor matrix (not including any elements from 
		//  the i and j row and colums).
		x = 0;
		for (I32 ii = 0; ii < NUMBER_COLS; ++ii)
		{
			if (ii != i)
			{
				y = 0;
				for (I32 jj = 0; jj < NUMBER_ROWS; ++jj)
				{
					if (jj != j)
					{
						mat[x][y] = m_mat[(ii*NUMBER_COLS)+jj];
						++y;
					}
				}
				++x;
			}
		}

		// See Fundamentals of Computer Graphics Third Edition,
		// Chapter 5 Linear Algebra for a full description of the
		// following formula.
		//
		// Briefly, the formula for calculating the determinant of
		// a 3x3 matrix:
		//		| a b c |
		// A =	| d e f |
		//		| g h i |
		// Cofactor matrices about the first row are:
		//		|e f|			|d f|			|d e|
		// aC =	|h i|	bC =	|g i|	cC =	|g h|
		//
		// so the formula to get the determinant of A is (|A|)...
		// |A| = a.|aC| - b.|bC| + c.|cC|
		//
		// We know how to get the determinant of a 2x2 matrix from
		// the formula a.d - c.b where the 2x2 matrix is:
		//		|a b|
		//		|c d|
		// so it is easy to calculate the determinant of a 3x3 matrix.
		// and recursively matrices of higher dimensions may therefore
		// be calculated also using the same technique.  The algorithm
		// in this method only applies to 4x4 matrices as thats all 
		// we need.
		//
		ret =  mat[0][0]*(mat[1][1]*mat[2][2]-mat[2][1]*mat[1][2]);
		ret -= mat[0][1]*(mat[1][0]*mat[2][2]-mat[2][0]*mat[1][2]);
		ret += mat[0][2]*(mat[1][0]*mat[2][1]-mat[2][0]*mat[1][1]);

		return (ret);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	F32 Matrix4::Determinant() const
	{
		F32 det = 0.0f;

		// calculate 4x4 determinant
		for (I32 i = 0; i < NUMBER_COLS; ++i)
		{
			det += (i & 0x1) ? (-m_mat[i] * Det3x3(0, i)) : (m_mat[i] * Det3x3(0, i));
		}
		
		return (det);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Matrix4::Inverse()
	{
		Matrix4 tmp;
		bool hasInverse = Inversed(tmp);
		if(hasInverse)
		{
			*this = tmp;
		}

		return(hasInverse);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Matrix4::Inversed(Matrix4 &outMatrix) const
	{
		bool hasInverse = true;
		F32 det = Determinant(), detij = 0.0f;

		if(!FloatCmp(det, 0.0f))
		{
			F32 oneDet = 1.0f / det;
			// calculate inverse
			for(I32 i = 0; i < NUMBER_COLS; ++i)
			{
				for(I32 j = 0; j < NUMBER_ROWS; ++j)
				{
					detij = Det3x3(j, i);
					outMatrix.m_mat[(i*NUMBER_COLS)+j] = ((i+j) & 0x1) ? (-detij * oneDet) : (detij * oneDet); 
				}
			}
		}
		else
		{
			hasInverse = false;
		}

		return (hasInverse);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Matrix4::InverseCramer()
	{
		Matrix4 tmp;
		bool hasInverse = InversedCramer(tmp);
		if(hasInverse)
		{
			*this = tmp;
		}

		return(hasInverse);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Matrix4::InversedCramer(Matrix4 &outMatrix) const
	{
		F32 tmp[12];
		F32 det = 0.0f;
		Matrix4 t(*this);
		t.Transpose();

		/* calculate pairs for first 8 elements (cofactors) */
		tmp[0] = t[10] * t[15];
		tmp[1] = t[11] * t[14];
		tmp[2] = t[9] * t[15];
		tmp[3] = t[11] * t[13];
		tmp[4] = t[9] * t[14];
		tmp[5] = t[10] * t[13];
		tmp[6] = t[8] * t[15];
		tmp[7] = t[11] * t[12];
		tmp[8] = t[8] * t[14];
		tmp[9] = t[10] * t[12];
		tmp[10] = t[8] * t[13];
		tmp[11] = t[9] * t[12];

		/* calculate first 8 elements (cofactors) */
		outMatrix[0] = tmp[0]*t[5] + tmp[3]*t[6] + tmp[4]*t[7];
		outMatrix[0] -= tmp[1]*t[5] + tmp[2]*t[6] + tmp[5]*t[7];
		outMatrix[1] = tmp[1]*t[4] + tmp[6]*t[6] + tmp[9]*t[7];
		outMatrix[1] -= tmp[0]*t[4] + tmp[7]*t[6] + tmp[8]*t[7];
		outMatrix[2] = tmp[2]*t[4] + tmp[7]*t[5] + tmp[10]*t[7];
		outMatrix[2] -= tmp[3]*t[4] + tmp[6]*t[5] + tmp[11]*t[7];
		outMatrix[3] = tmp[5]*t[4] + tmp[8]*t[5] + tmp[11]*t[6];
		outMatrix[3] -= tmp[4]*t[4] + tmp[9]*t[5] + tmp[10]*t[6];
		outMatrix[4] = tmp[1]*t[1] + tmp[2]*t[2] + tmp[5]*t[3];
		outMatrix[4] -= tmp[0]*t[1] + tmp[3]*t[2] + tmp[4]*t[3];
		outMatrix[5] = tmp[0]*t[0] + tmp[7]*t[2] + tmp[8]*t[3];
		outMatrix[5] -= tmp[1]*t[0] + tmp[6]*t[2] + tmp[9]*t[3];
		outMatrix[6] = tmp[3]*t[0] + tmp[6]*t[1] + tmp[11]*t[3];
		outMatrix[6] -= tmp[2]*t[0] + tmp[7]*t[1] + tmp[10]*t[3];
		outMatrix[7] = tmp[4]*t[0] + tmp[9]*t[1] + tmp[10]*t[2];
		outMatrix[7] -= tmp[5]*t[0] + tmp[8]*t[1] + tmp[11]*t[2];

		/* calculate pairs for second 8 elements (cofactors) */
		tmp[0] = t[2]*t[7];
		tmp[1] = t[3]*t[6];
		tmp[2] = t[1]*t[7];
		tmp[3] = t[3]*t[5];
		tmp[4] = t[1]*t[6];
		tmp[5] = t[2]*t[5];
		tmp[6] = t[0]*t[7];
		tmp[7] = t[3]*t[4];
		tmp[8] = t[0]*t[6];
		tmp[9] = t[2]*t[4];
		tmp[10] = t[0]*t[5];
		tmp[11] = t[1]*t[4];

		/* calculate second 8 elements (cofactors) */
		outMatrix[8] = tmp[0]*t[13] + tmp[3]*t[14] + tmp[4]*t[15];
		outMatrix[8] -= tmp[1]*t[13] + tmp[2]*t[14] + tmp[5]*t[15];
		outMatrix[9] = tmp[1]*t[12] + tmp[6]*t[14] + tmp[9]*t[15];
		outMatrix[9] -= tmp[0]*t[12] + tmp[7]*t[14] + tmp[8]*t[15];
		outMatrix[10] = tmp[2]*t[12] + tmp[7]*t[13] + tmp[10]*t[15];
		outMatrix[10]-= tmp[3]*t[12] + tmp[6]*t[13] + tmp[11]*t[15];
		outMatrix[11] = tmp[5]*t[12] + tmp[8]*t[13] + tmp[11]*t[14];
		outMatrix[11]-= tmp[4]*t[12] + tmp[9]*t[13] + tmp[10]*t[14];
		outMatrix[12] = tmp[2]*t[10] + tmp[5]*t[11] + tmp[1]*t[9];
		outMatrix[12]-= tmp[4]*t[11] + tmp[0]*t[9] + tmp[3]*t[10];
		outMatrix[13] = tmp[8]*t[11] + tmp[0]*t[8] + tmp[7]*t[10];
		outMatrix[13]-= tmp[6]*t[10] + tmp[9]*t[11] + tmp[1]*t[8];
		outMatrix[14] = tmp[6]*t[9] + tmp[11]*t[11] + tmp[3]*t[8];
		outMatrix[14]-= tmp[10]*t[11] + tmp[2]*t[8] + tmp[7]*t[9];
		outMatrix[15] = tmp[10]*t[10] + tmp[4]*t[8] + tmp[9]*t[9];
		outMatrix[15]-= tmp[8]*t[9] + tmp[11]*t[10] + tmp[5]*t[8];

		/* calculate determinant */
		det=t[0]*outMatrix[0]+t[1]*outMatrix[1]+t[2]*outMatrix[2]+t[3]*outMatrix[3];

		/* calculate matrix inverse */
		det = 1/det;
		for (I32 j = 0; j < 16; ++j)
		{
			outMatrix[j] *= det;
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4 Matrix4::operator*(const Matrix4 &rhs) const
	{
		Matrix4 outMatrix;
		outMatrix[M00] = (m_mat[M00]*rhs.m_mat[M00]) + (m_mat[M10]*rhs.m_mat[M01]) + (m_mat[M20]*rhs.m_mat[M02]) + (m_mat[M30]*rhs.m_mat[M03]);
		outMatrix[M10] = (m_mat[M00]*rhs.m_mat[M10]) + (m_mat[M10]*rhs.m_mat[M11]) + (m_mat[M20]*rhs.m_mat[M12]) + (m_mat[M30]*rhs.m_mat[M13]);
		outMatrix[M20] = (m_mat[M00]*rhs.m_mat[M20]) + (m_mat[M10]*rhs.m_mat[M21]) + (m_mat[M20]*rhs.m_mat[M22]) + (m_mat[M30]*rhs.m_mat[M23]);
		outMatrix[M30] = (m_mat[M00]*rhs.m_mat[M30]) + (m_mat[M10]*rhs.m_mat[M31]) + (m_mat[M20]*rhs.m_mat[M32]) + (m_mat[M30]*rhs.m_mat[M33]);

		outMatrix[M01] = (m_mat[M01]*rhs.m_mat[M00]) + (m_mat[M11]*rhs.m_mat[M01]) + (m_mat[M21]*rhs.m_mat[M02]) + (m_mat[M31]*rhs.m_mat[M03]);
		outMatrix[M11] = (m_mat[M01]*rhs.m_mat[M10]) + (m_mat[M11]*rhs.m_mat[M11]) + (m_mat[M21]*rhs.m_mat[M12]) + (m_mat[M31]*rhs.m_mat[M13]);
		outMatrix[M21] = (m_mat[M01]*rhs.m_mat[M20]) + (m_mat[M11]*rhs.m_mat[M21]) + (m_mat[M21]*rhs.m_mat[M22]) + (m_mat[M31]*rhs.m_mat[M23]);
		outMatrix[M31] = (m_mat[M01]*rhs.m_mat[M30]) + (m_mat[M11]*rhs.m_mat[M31]) + (m_mat[M21]*rhs.m_mat[M32]) + (m_mat[M31]*rhs.m_mat[M33]);

		outMatrix[M02] = (m_mat[M02]*rhs.m_mat[M00]) + (m_mat[M12]*rhs.m_mat[M01]) + (m_mat[M22]*rhs.m_mat[M02]) + (m_mat[M32]*rhs.m_mat[M03]);
		outMatrix[M12] = (m_mat[M02]*rhs.m_mat[M10]) + (m_mat[M12]*rhs.m_mat[M11]) + (m_mat[M22]*rhs.m_mat[M12]) + (m_mat[M32]*rhs.m_mat[M13]);
		outMatrix[M22] = (m_mat[M02]*rhs.m_mat[M20]) + (m_mat[M12]*rhs.m_mat[M21]) + (m_mat[M22]*rhs.m_mat[M22]) + (m_mat[M32]*rhs.m_mat[M23]);
		outMatrix[M32] = (m_mat[M02]*rhs.m_mat[M30]) + (m_mat[M12]*rhs.m_mat[M31]) + (m_mat[M22]*rhs.m_mat[M32]) + (m_mat[M32]*rhs.m_mat[M33]);

		outMatrix[M03] = (m_mat[M03]*rhs.m_mat[M00]) + (m_mat[M13]*rhs.m_mat[M01]) + (m_mat[M23]*rhs.m_mat[M02]) + (m_mat[M33]*rhs.m_mat[M03]);
		outMatrix[M13] = (m_mat[M03]*rhs.m_mat[M10]) + (m_mat[M13]*rhs.m_mat[M11]) + (m_mat[M23]*rhs.m_mat[M12]) + (m_mat[M33]*rhs.m_mat[M13]);
		outMatrix[M23] = (m_mat[M03]*rhs.m_mat[M20]) + (m_mat[M13]*rhs.m_mat[M21]) + (m_mat[M23]*rhs.m_mat[M22]) + (m_mat[M33]*rhs.m_mat[M23]);
		outMatrix[M33] = (m_mat[M03]*rhs.m_mat[M30]) + (m_mat[M13]*rhs.m_mat[M31]) + (m_mat[M23]*rhs.m_mat[M32]) + (m_mat[M33]*rhs.m_mat[M33]);

		return (outMatrix);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4 &Matrix4::operator*=(const Matrix4 &rhs)
	{
		*this = *this * rhs;
		return (*this);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Vector4 Matrix4::operator*(const Vector4 &rhs) const
	{
		Vector4 outVec;
		outVec.SetX((m_mat[M00]*rhs.GetX()) + (m_mat[M10]*rhs.GetY()) + (m_mat[M20]*rhs.GetZ()) + (m_mat[M30]*rhs.GetW()));
		outVec.SetY((m_mat[M01]*rhs.GetX()) + (m_mat[M11]*rhs.GetY()) + (m_mat[M21]*rhs.GetZ()) + (m_mat[M31]*rhs.GetW()));
		outVec.SetZ((m_mat[M02]*rhs.GetX()) + (m_mat[M12]*rhs.GetY()) + (m_mat[M22]*rhs.GetZ()) + (m_mat[M32]*rhs.GetW()));
		outVec.SetW((m_mat[M03]*rhs.GetX()) + (m_mat[M13]*rhs.GetY()) + (m_mat[M23]*rhs.GetZ()) + (m_mat[M33]*rhs.GetW()));
		return (outVec);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4 &Matrix4::operator=(const Matrix4 &rhs)
	{
		Set(rhs.m_mat);
		return (*this);
	}

	// /////////////////////////////////////////////////////////////////
	// ***************** Matrix4 Helper Functions **********************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4 &BuildRotationArbitraryMatrix4(Matrix4 &outMatrix, const F32 angle, const F32 x, const F32 y, const F32 z)
	{
		Vector3 rotationVec(x, y, z);
		if(rotationVec.Magnitude() == 0.0f)
		{
			outMatrix.LoadIdentity();
			return (outMatrix);
		}

		const F32 radians = F32(m3dDegToRad(angle));
		const F32 sinAngle = F32(sin(radians));
		const F32 cosAngle = F32(cos(radians));

		// Rotation matrix needs to be normalized.
		rotationVec.Normalize();

		// Build up the components used to assemble the rotation matrix.
		// For explanation of the formula used to generate this matrix,
		// please see Mathematics for Computer Graphics by John Vince,
		// Second Edition, Chapter 7 Transformations, pg 83.
		//
		const F32 xy = rotationVec.GetX() * rotationVec.GetY();
		const F32 yz = rotationVec.GetY() * rotationVec.GetZ();
		const F32 zx = rotationVec.GetZ() * rotationVec.GetX();
		const F32 xs = rotationVec.GetX() * sinAngle;
		const F32 ys = rotationVec.GetY() * sinAngle;
		const F32 zs = rotationVec.GetZ() * sinAngle;
		const F32 one_c = 1.0f - cosAngle;
		const F32 m00 = (one_c * (rotationVec.GetX() * rotationVec.GetX())) + cosAngle;
		const F32 m01 = (one_c * xy) + zs;
		const F32 m02 = (one_c * zx) - ys;
		const F32 m10 = (one_c * xy) - zs;
		const F32 m11 = (one_c * (rotationVec.GetY() * rotationVec.GetY())) + cosAngle;
		const F32 m12 = (one_c * yz) + xs;
		const F32 m20 = (one_c * zx) + ys;
		const F32 m21 = (one_c * yz) - xs;
		const F32 m22 = (one_c * (rotationVec.GetZ() * rotationVec.GetZ())) + cosAngle;

		outMatrix = Matrix4(m00,	m10,	m20,	0.0f,\
							m01,	m11,	m21,	0.0f,\
							m02,	m12,	m22,	0.0f,\
							0.0f,	0.0f,	0.0f,	1.0f);
		return (outMatrix);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4 &BuildRotationXMatrix4(Matrix4 &outMatrix, const F32 angle)
	{
		const F32 radians = F32(m3dDegToRad(angle));
		const F32 sinAngle = F32(sin(radians));
		const F32 cosAngle = F32(cos(radians));

		outMatrix.LoadIdentity();
		outMatrix.SetElement(Matrix4::C1, Matrix4::R1, cosAngle);
		outMatrix.SetElement(Matrix4::C1, Matrix4::R2, sinAngle);
		outMatrix.SetElement(Matrix4::C2, Matrix4::R1, -sinAngle);
		outMatrix.SetElement(Matrix4::C2, Matrix4::R2, cosAngle);

		return (outMatrix);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4 &BuildRotationYMatrix4(Matrix4 &outMatrix, const F32 angle)
	{
		const F32 radians = F32(m3dDegToRad(angle));
		const F32 sinAngle = F32(sin(radians));
		const F32 cosAngle = F32(cos(radians));

		outMatrix.LoadIdentity();
		outMatrix.SetElement(Matrix4::C0, Matrix4::R0, cosAngle);
		outMatrix.SetElement(Matrix4::C2, Matrix4::R0, sinAngle);
		outMatrix.SetElement(Matrix4::C0, Matrix4::R2, -sinAngle);
		outMatrix.SetElement(Matrix4::C2, Matrix4::R2, cosAngle);

		return (outMatrix);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4 &BuildRotationZMatrix4(Matrix4 &outMatrix, const F32 angle)
	{
		const F32 radians = F32(m3dDegToRad(angle));
		const F32 sinAngle = F32(sin(radians));
		const F32 cosAngle = F32(cos(radians));

		outMatrix.LoadIdentity();
		outMatrix.SetElement(Matrix4::C0, Matrix4::R0, cosAngle);
		outMatrix.SetElement(Matrix4::C1, Matrix4::R0, -sinAngle);
		outMatrix.SetElement(Matrix4::C0, Matrix4::R1, sinAngle);
		outMatrix.SetElement(Matrix4::C1, Matrix4::R1, cosAngle);

		return (outMatrix);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4 &BuildTranslationMatrix4(Matrix4 &outMatrix, const F32 x, const F32 y, const F32 z)
	{
		outMatrix.LoadIdentity();
		outMatrix.SetElement(Matrix4::C3, Matrix4::R0, x);
		outMatrix.SetElement(Matrix4::C3, Matrix4::R1, y);
		outMatrix.SetElement(Matrix4::C3, Matrix4::R2, z);

		return (outMatrix);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4 &BuildScaleMatrix4(Matrix4 &outMatrix, const F32 x, const F32 y, const F32 z)
	{
		outMatrix.LoadIdentity();
		outMatrix.SetElement(Matrix4::C0, Matrix4::R0, x);
		outMatrix.SetElement(Matrix4::C1, Matrix4::R1, y);
		outMatrix.SetElement(Matrix4::C2, Matrix4::R2, z);

		return (outMatrix);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4 &BuildScaleArbitraryMatrix4(Matrix4 &outMatrix, const F32 x, const F32 y, const F32 z, const Point3 &pt)
	{
		outMatrix = BuildScaleMatrix4(outMatrix, x, y, z);
		outMatrix.SetElement(Matrix4::C3, Matrix4::R0, pt.GetX() * (1 - x));
		outMatrix.SetElement(Matrix4::C3, Matrix4::R1, pt.GetY() * (1 - y));
		outMatrix.SetElement(Matrix4::C3, Matrix4::R2, pt.GetZ() * (1 - z));

		return (outMatrix);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4 &BuildLookAt(Matrix4 &outMatrix, const Point3 &eyePos, const Point3 &atPos, const Vector3 &upVec)
	{
		// Get a vector pointing in the same direction as the camera.
		Vector3 dirVec = atPos - eyePos;
		dirVec.Normalize();
		
		// Ensure the UP vector is a normalized one.
		Vector3 upVecCopy(upVec);
		upVecCopy.Normalize();

		Vector3 s;
		dirVec.Cross(upVecCopy, s);
		Vector3 u;
		s.Cross(dirVec, u);

		// Row 1
		outMatrix[Matrix4::M00] = s.GetX();
		outMatrix[Matrix4::M01] = s.GetY();
		outMatrix[Matrix4::M02] = s.GetZ();
		outMatrix[Matrix4::M03] = 0.0f;
		// Row 2
		outMatrix[Matrix4::M10] = u.GetX();
		outMatrix[Matrix4::M11] = u.GetY();
		outMatrix[Matrix4::M12] = u.GetZ();
		outMatrix[Matrix4::M13] = 0.0f;
		// Row 3
		outMatrix[Matrix4::M20] = -dirVec.GetX();
		outMatrix[Matrix4::M21] = -dirVec.GetY();
		outMatrix[Matrix4::M22] = -dirVec.GetZ();
		outMatrix[Matrix4::M23] = 0.0f;
		// Row 4
		outMatrix[Matrix4::M30] = eyePos.GetX();
		outMatrix[Matrix4::M31] = eyePos.GetY();
		outMatrix[Matrix4::M32] = eyePos.GetZ();
		outMatrix[Matrix4::M33] = 1.0f;

		return (outMatrix);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4 &BuildPerspectiveFov(Matrix4 &outMatrix, const F32 fov, const F32 aspect, const F32 zNear, const F32 zFar)
	{
		outMatrix.LoadIdentity();

		const F32 yMax = zNear * tanf(F32(m3dDegToRad(fov)) * 0.5f);
		const F32 yMin = -yMax;
		const F32 xMin = yMin * aspect;
		const F32 xMax = -xMin;
		// Perspective matrix settings taken from OpenGL Superbible math3d lib and Fundamentals
		// of Computer Graphics, Third Edition, Chapter 7 Perspective Projection, pg. 155.
		outMatrix.SetElement(Matrix4::C0, Matrix4::R0, (2.0f * zNear) / (xMax - xMin));
		outMatrix.SetElement(Matrix4::C1, Matrix4::R1, (2.0f * zNear) / (yMax - yMin));
		outMatrix.SetElement(Matrix4::C2, Matrix4::R0, (xMax + xMin) / (xMax - xMin));
		outMatrix.SetElement(Matrix4::C2, Matrix4::R1, (yMax + yMin) / (yMax - yMin));
		outMatrix.SetElement(Matrix4::C2, Matrix4::R2, -((zFar + zNear) / (zFar - zNear)));
		outMatrix.SetElement(Matrix4::C2, Matrix4::R3, -1.0f);
		outMatrix.SetElement(Matrix4::C3, Matrix4::R2, -((2.0f * (zFar*zNear))/(zFar - zNear)));
		outMatrix.SetElement(Matrix4::C3, Matrix4::R3, 0.0f);

		//// From oolong engine 2 to verify that the matrix i am using is giving me the correct perspective projection matrix...
		//F32 f = 1.0f / (F32)tan(m3dDegToRad(fov) * 0.5f);
		//F32 n = 1.0f / (zNear - zFar);
		//outMatrix.SetElement(Matrix4::C0, Matrix4::R0, f / aspect);
		//outMatrix.SetElement(Matrix4::C1, Matrix4::R1, f);
		//outMatrix.SetElement(Matrix4::C2, Matrix4::R2, (zFar + zNear) * n);
		//outMatrix.SetElement(Matrix4::C2, Matrix4::R3, -1.0f);
		//outMatrix.SetElement(Matrix4::C3, Matrix4::R2, (2 * zFar * zNear) * n);
		//outMatrix.SetElement(Matrix4::C3, Matrix4::R3, 0.0f);

		return (outMatrix);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Matrix4 &BuildOrthographic(Matrix4 &outMatrix, const F32 xMin, const F32 xMax, const F32 yMin, const F32 yMax, const F32 zMin, const F32 zMax)
	{
		outMatrix.LoadIdentity();

		// Please see Fundamentals of Computer Graphics, 3rd Edition, Chapter 7
		//  Viewing Transformations, pg.144 for details on the orthographic projection
		//   matrix.
		outMatrix.SetElement(Matrix4::C0, Matrix4::R0, 2.0f / (xMax - xMin));
		outMatrix.SetElement(Matrix4::C1, Matrix4::R1, 2.0f / (yMax - yMin));
		outMatrix.SetElement(Matrix4::C2, Matrix4::R2, 2.0f / (zMin - zMax));
		outMatrix.SetElement(Matrix4::C3, Matrix4::R0, -((xMax + xMin) / (xMax - xMin)));
		outMatrix.SetElement(Matrix4::C3, Matrix4::R1, -((yMax + yMin) / (yMax - yMin)));
		outMatrix.SetElement(Matrix4::C3, Matrix4::R2, -((zMin + zMax) / (zMin - zMax)));

		//// From oolong engine 2...
		//outMatrix.SetElement(Matrix4::C0, Matrix4::R0, 2.0f / (xMax - xMin));
		//outMatrix.SetElement(Matrix4::C1, Matrix4::R1, 2.0f / (yMax - yMin));
		//outMatrix.SetElement(Matrix4::C2, Matrix4::R2, 1.0f / (zMin - zMax));
		//outMatrix.SetElement(Matrix4::C2, Matrix4::R3, zMin / (zMin - zMax));

		return (outMatrix);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ExtractRotationMatrix(const Matrix4 &matrix, Matrix3x3 rotMatArr)
	{
		matrix.GetElement(Matrix4::C0, Matrix4::R0, rotMatArr[0]);
		matrix.GetElement(Matrix4::C0, Matrix4::R1, rotMatArr[1]);
		matrix.GetElement(Matrix4::C0, Matrix4::R2, rotMatArr[2]);

		matrix.GetElement(Matrix4::C1, Matrix4::R0, rotMatArr[3]);
		matrix.GetElement(Matrix4::C1, Matrix4::R1, rotMatArr[4]);
		matrix.GetElement(Matrix4::C1, Matrix4::R2, rotMatArr[5]);

		matrix.GetElement(Matrix4::C2, Matrix4::R0, rotMatArr[6]);
		matrix.GetElement(Matrix4::C2, Matrix4::R1, rotMatArr[7]);
		matrix.GetElement(Matrix4::C2, Matrix4::R2, rotMatArr[8]);
	}

}
