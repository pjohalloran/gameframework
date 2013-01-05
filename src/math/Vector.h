#pragma once
#ifndef __GF_VECTOR_H
#define __GF_VECTOR_H

// /////////////////////////////////////////////////////////////////
// @file Vector.h
// @author PJ O Halloran
// @date 22/08/2010
//
// File contains the header for the Vector3, Vector4 and Point3
// classes.
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
// /////////////////////////////////////////////////////////////////

#include <cmath>
#include <cstring>
#include <string>
#include <ostream>
#include <sstream>
#include <list>

#include <boost/shared_ptr.hpp>

#include "CommonMath.h"
#include "CRandom.h"

namespace GameHalloran
{
	// Forward declarations of Vector4, Point3 for Vector3 and Vector4 conversion constructors.
	class Vector4;
	class Point3;

	// /////////////////////////////////////////////////////////////////
	// @class Vector3
	// @author PJ O Halloran
	//
	// This class represents a 3D Vector.
	//
	// Most methods are explicitly inlined unless they are more than 10
	// lines in length.
	//
	// /////////////////////////////////////////////////////////////////
	class Vector3
	{
		private:

			static const U32 NUMBER_COMPONENTS = 3;

			F32 m_vec[NUMBER_COMPONENTS];					///< XYZ component array.

			// /////////////////////////////////////////////////////////////////
			// Get the sum of the vectors components.  This should not be
			// needed outside the class so is private.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 SumComp() const
			{
				return (m_vec[0] + m_vec[1] + m_vec[2]);
			};

		public:

			// /////////////////////////////////////////////////////////////////
			// Default constructor.
			//
			// Declares a zero length vector with an undefined direction.
			//
			// /////////////////////////////////////////////////////////////////
			explicit inline Vector3() { memset(m_vec, 0, sizeof(F32) * NUMBER_COMPONENTS); };

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Declares a 3D vector with the supplied xyz values.
			//
			// /////////////////////////////////////////////////////////////////
			explicit inline Vector3(const F32 x, const F32 y, const F32 z) { m_vec[0] = x; m_vec[1] = y; m_vec[2] = z; };

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Declares a 3D vector with the xyz componenets having the same
			// initial value.
			//
			// /////////////////////////////////////////////////////////////////
			explicit inline Vector3(const F32 scaler) { m_vec[0] = scaler; m_vec[1] = scaler; m_vec[2] = scaler; };

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Declares a 3D vector with a plain C style array.
			//
			// /////////////////////////////////////////////////////////////////
			explicit inline Vector3(const F32 vecArr[NUMBER_COMPONENTS]) { memcpy(m_vec, vecArr, sizeof(F32) * NUMBER_COMPONENTS); };

			// /////////////////////////////////////////////////////////////////
			// Copy Constructor.
			//
			// Declares a 3D vector with the supplied 3D Vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3(const Vector3 &copyVec) { memcpy(m_vec, copyVec.m_vec, sizeof(F32) * NUMBER_COMPONENTS); };

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Converts a 3D point to a positional vector.
			//
			// In math textbooks a positional vector has a tail at the origin
			// of the world coordinates and a head at the position of the point.
			//
			// /////////////////////////////////////////////////////////////////
			explicit Vector3(const Point3 &pt);

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Converts a 4D vector to a 3D vector.
			//
			// Divides the xyz components of the 4D vector by its w component to
			// get the equivalent 3D vector.
			//
			// /////////////////////////////////////////////////////////////////
			explicit Vector3(const Vector4 &vec4);

			// /////////////////////////////////////////////////////////////////
			// Destructor.
			//
			// /////////////////////////////////////////////////////////////////
			~Vector3() { };

			// /////////////////////////////////////////////////////////////////
			// Getter/Setter methods.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 GetX() const { return (m_vec[0]); };
			inline F32 GetY() const { return (m_vec[1]); };
			inline F32 GetZ() const { return (m_vec[2]); };
			inline void SetX(const F32 x){ m_vec[0] = x; };
			inline void SetY(const F32 y){ m_vec[1] = y; };
			inline void SetZ(const F32 z){ m_vec[2] = z; };
			inline void Set(const F32 x, const F32 y, const F32 z) {m_vec[0] = x; m_vec[1] = y; m_vec[2] = z;};

			// /////////////////////////////////////////////////////////////////
			// Assign the components of a Vector3 to this vector.
			//
			// @param rhs The Vector to assign to this Vector.
			//
			// @return Vector3 A copy of the resulting vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 &operator=(const Vector3 &rhs)
			{
				if(this == &rhs)
				{
					return (*this);
				}

				m_vec[0] = rhs.m_vec[0];
				m_vec[1] = rhs.m_vec[1];
				m_vec[2] = rhs.m_vec[2];
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Assign the components of a Vector4 to this vector.
			//
			// @param rhs The 4D Vector to assign to this Vector.
			//
			// @return Vector3 A copy of the resulting vector.
			//
			// /////////////////////////////////////////////////////////////////
			Vector3 &operator=(const Vector4 &rhs);

			// /////////////////////////////////////////////////////////////////
			// Assign the components of a Point3 to this vector.
			//
			// @param rhs The 3D Point to assign to this Vector.
			//
			// @return Vector3 A copy of the resulting vector.
			//
			// /////////////////////////////////////////////////////////////////
			Vector3 &operator=(const Point3 &rhs);

			// /////////////////////////////////////////////////////////////////
			// Check if two vectors are equivalent.  They are equivalent in general
			// if their magnitude and direction are equal.  In simple terms we can
			// say they are equivalent if their components are all equivalent.
			//
			// @param rhs The Vector to check against this Vector.
			//
			// @return bool True if two vectors are equal and false otherwise.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool operator==(const Vector3 &rhs) const
			{
				if(this == &rhs) return (true);
				return (FloatCmp(m_vec[0], rhs.m_vec[0]) && FloatCmp(m_vec[1], rhs.m_vec[1]) && FloatCmp(m_vec[2], rhs.m_vec[2]));
			};

			// /////////////////////////////////////////////////////////////////
			// Check if two vectors are not equivalent.
			//
			// @param rhs The Vector to check against this Vector.
			//
			// @return bool True if two vectors are not equal and false otherwise.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool operator!=(const Vector3 &rhs) const
			{
				return (!(*this == rhs));
			};

			// /////////////////////////////////////////////////////////////////
			// Add a vector to this vector and return the resulting Vector.
			//
			// @param rhs The Vector to add to this Vector.
			//
			// @return Vector3 A copy of the resulting vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 operator+(const Vector3 &rhs) const
			{
				return (Vector3(m_vec[0] + rhs.m_vec[0], m_vec[1] + rhs.m_vec[1], m_vec[2] + rhs.m_vec[2]));
			};

			// /////////////////////////////////////////////////////////////////
			// Subtract a vector from this vector and return the resulting Vector.
			//
			// @param rhs The Vector to subtract from this Vector.
			//
			// @return Vector3 A copy of the resulting vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 operator-(const Vector3 &rhs) const
			{
				return (Vector3(m_vec[0] - rhs.m_vec[0], m_vec[1] - rhs.m_vec[1], m_vec[2] - rhs.m_vec[2]));
			};

			// /////////////////////////////////////////////////////////////////
			// Add a vector to this vector.
			//
			// @param rhs The Vector to add to this Vector.
			//
			// @return Vector3& The new version of this Vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 &operator+=(const Vector3 &rhs)
			{
				m_vec[0] += rhs.m_vec[0];
				m_vec[1] += rhs.m_vec[1];
				m_vec[2] += rhs.m_vec[2];
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Subtract a vector from this vector.
			//
			// @param rhs The Vector to subtract from this Vector.
			//
			// @return Vector3& The new version of this Vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 &operator-=(const Vector3 &rhs)
			{
				m_vec[0] -= rhs.m_vec[0];
				m_vec[1] -= rhs.m_vec[1];
				m_vec[2] -= rhs.m_vec[2];
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Negate a vector.
			//
			// @return Vector3& The new version of this Vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 operator-() const
			{
				return (Vector3(-m_vec[0], -m_vec[1], -m_vec[2]));
			};

			// /////////////////////////////////////////////////////////////////
			// Multiply a vector by a scaler.
			//
			// @param scaler The scaler value.
			//
			// @return Vector3 A copy of the resulting Vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 operator*(const F32 scaler) const
			{
				Vector3 copy(*this);
				copy.m_vec[0] *= scaler;
				copy.m_vec[1] *= scaler;
				copy.m_vec[2] *= scaler;
				return (copy);
			};

			// /////////////////////////////////////////////////////////////////
			// Divide a vector by a scaler.
			//
			// @param scaler The scaler value.
			//
			// @return Vector3 A copy of the resulting Vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 operator/(const F32 scaler) const
			{
				Vector3 copy(*this);
				if(!FloatCmp(scaler, 0.0f))
				{
					copy.m_vec[0] /= scaler;
					copy.m_vec[1] /= scaler;
					copy.m_vec[2] /= scaler;
				}
				return (copy);
			};

			// /////////////////////////////////////////////////////////////////
			// Multiply this vector by a scaler.
			//
			// @param scaler The scaler value.
			//
			// @return Vector3& This vector with the scaler applied.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 &operator*=(const F32 scaler)
			{
				m_vec[0] *= scaler;
				m_vec[1] *= scaler;
				m_vec[2] *= scaler;
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Divide this vector by a scaler.
			//
			// @param scaler The scaler value.
			//
			// @return Vector3& This vector with the scaler applied.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 &operator/=(const F32 scaler)
			{
				if(!FloatCmp(scaler, 0.0f))
				{
					m_vec[0] /= scaler;
					m_vec[1] /= scaler;
					m_vec[2] /= scaler;
				}
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Check if this Vector is less than another vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool operator<(const Vector3 &rhs) const
			{
				return (MagnitudeSqr() < rhs.MagnitudeSqr());
			};

			// /////////////////////////////////////////////////////////////////
			// Get the magnitude squared of this vector otherwise known as the
			// length of the vector.
			//
			// This version does not compute the square root and has no casts so
			// is faster if you want to compare two vectors lengths.  If you need
			// the actual length of the vector for some operation then you should
			// use the Magnitude() method instead.
			//
			// @return F32 The magnitude or length of this vector squared.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 MagnitudeSqr() const
			{
				return (m_vec[0]*m_vec[0] + m_vec[1]*m_vec[1] + m_vec[2]*m_vec[2]);
			};

			// /////////////////////////////////////////////////////////////////
			// Get the magnitude of this vector otherwise known as the
			// length of the vector.
			//
			// @return F32 The magnitude or length of this vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 Magnitude() const
			{
				return (static_cast<F32>(sqrt(static_cast<F32>(MagnitudeSqr()))));
			};

			// /////////////////////////////////////////////////////////////////
			// Normalize this vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 &Normalize()
			{
				const F32 length = Magnitude();
				if(!FloatCmp(length, 0.0f))
				{
					m_vec[0] /= length;
					m_vec[1] /= length;
					m_vec[2] /= length;
				}
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Normalize this vector and store the result in the vector supplied
			// so as to NOT to modify this vector.
			//
			// @param outVecRef The vector to store the result of normalizing this
			//						vector in.
			//
			// /////////////////////////////////////////////////////////////////
			inline void Normalized(Vector3 &outVecRef) const
			{
				const F32 length = Magnitude();
				if(!FloatCmp(length, 0.0f))
				{
					outVecRef.m_vec[0] = m_vec[0] / length;
					outVecRef.m_vec[1] = m_vec[1] / length;
					outVecRef.m_vec[2] = m_vec[2] / length;
				}
			};

			// /////////////////////////////////////////////////////////////////
			// Calculate the dot product of this vector and another vector.
			// Otherwise known as the scaler product.
			//
			// Useful for back face culling or lighting calculations as it may be
			// used to calculate the angle between two vector.
			//
			// e.g. If a plane/primitives surface normal vectors' angle to this
			// vector is > 90, then it is said to be a back facing plane/primitive.
			//
			// @param rhsVecRef The other vector to use to get the scaler or dot
			//						product of, along with this vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 Dot(const Vector3 &rhsVecRef) const
			{
				return (m_vec[0]*rhsVecRef.m_vec[0] + m_vec[1]*rhsVecRef.m_vec[1] + m_vec[2]*rhsVecRef.m_vec[2]);
			};

			// /////////////////////////////////////////////////////////////////
			// Calculate the cross product of two vectors.
			// The resultant vector is orthogonal to this and the other vector.
			//
			// Useful for getting the surface normal vector of two vectors.
			//
			// @param rhsVecRef The other vector to use to get the scaler or dot
			//						product of, along with this vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline void Cross(const Vector3 &rhsVecRef, Vector3 &outVecRef) const
			{
				outVecRef.m_vec[0] = m_vec[1]*rhsVecRef.m_vec[2] -m_vec[2]*rhsVecRef.m_vec[1];
				outVecRef.m_vec[1] = -(m_vec[0]*rhsVecRef.m_vec[2] - m_vec[2]*rhsVecRef.m_vec[0]);
				outVecRef.m_vec[2] = m_vec[0]*rhsVecRef.m_vec[1] - m_vec[1]*rhsVecRef.m_vec[0];
			};

			// /////////////////////////////////////////////////////////////////
			// Return a string of the Vector3s' components.
			//
			// @return std::string
			//
			// /////////////////////////////////////////////////////////////////
			inline std::string ToString() const
			{
				std::stringstream strStream;
				strStream << "[" << m_vec[0] << ", " << m_vec[1] << ", " << m_vec[2] << "]";
				return (strStream.str());
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
			inline const F32 * const GetComponentsConst() const
			{
				return (m_vec);
			};
	};

	// /////////////////////////////////////////////////////////////////
	// Multiply a vector by a scaler with the scaler on the lhs of the
	// expression.
	//
	// @param lhs The scaler value.
	// @param rhs The Vector.
	//
	// @return Vector3 A copy of the resulting Vector.
	//
	// /////////////////////////////////////////////////////////////////
	inline Vector3 operator*(const F32 lhs, const Vector3 &rhs)
	{
		Vector3 copy(rhs);
		copy = rhs * lhs;
		return (copy);
	};

	// /////////////////////////////////////////////////////////////////
	// Get the angle between two vectors using the dot product.
	//
	// This is a helper function for Vector3. As such it is located
	// externally to strengthen encapsulation.
	//
	// @param u The first vector.
	// @param v The second vector.
	//
	// @F32 The angle between the two vectors (in radians).
	//
	// /////////////////////////////////////////////////////////////////
	inline F32 AngleBetweenVector3(const Vector3 &u, const Vector3 &v)
	{
		F64 dotP = static_cast<F64>(u.Dot(v));
		return (static_cast<F32>(m3dRadToDeg(acos(dotP))));
	};

	// /////////////////////////////////////////////////////////////////
	// Check if a vector is a normal vector (of length 1).
	//
	// This is a helper function for Vector3. As such it is located
	// externally to strengthen encapsulation.
	//
	// @param vecRef The vector to perform the check upon.
	//
	// @return bool True if the vector is of unit length and false if not.
	//
	// /////////////////////////////////////////////////////////////////
	inline bool IsNormalizedVector3(const Vector3 &vecRef)
	{
		return (FloatCmp(vecRef.Magnitude(), 1.0f));
	};

	// /////////////////////////////////////////////////////////////////
	// Performs linear interpolation on two 3 component vectors.
	//
	// @param a The start vector.
	// @param b The end vector.
	// @param interpolateVal Interpolation value (should be between 0
	//							and 1, if not it is clamped).
	// @param outVec Output vector.
	//
	// @return Vector3& Output vector.
	//
	// /////////////////////////////////////////////////////////////////
	Vector3 &Vector3Lerp(const Vector3 &a, const Vector3 &b, const F32 interpolateVal, Vector3 &outVec);

	// /////////////////////////////////////////////////////////////////
	// @class Vector4
	// @author PJ O Halloran
	//
	// This class represents a 4D Vector.
	//
	// This is useful for storing a 3D vector in homogenous coordinates
	// so that we may use the vector in 3D transformations.
	//
	// Most methods are explicitly inlined unless they are more than 10
	// lines in length.  These methods deemed too long are in the cpp file.
	// This is to keep the object files size down.
	//
	// /////////////////////////////////////////////////////////////////
	class Vector4
	{
		private:

			static const U32 NUMBER_COMPONENTS = 4;

			F32 m_vec[NUMBER_COMPONENTS];					///< XYZW component array.

			// /////////////////////////////////////////////////////////////////
			// Get the sum of the vectors components.  This should not be
			// needed outside the class so is private.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 SumComp() const
			{
				return (m_vec[0] + m_vec[1] + m_vec[2] + m_vec[3]);
			};

		public:

			// /////////////////////////////////////////////////////////////////
			// Default constructor.
			//
			// Declares a zero length vector with an undefined direction.
			//
			// /////////////////////////////////////////////////////////////////
			explicit inline Vector4() { memset(m_vec, 0, sizeof(F32) * NUMBER_COMPONENTS); };

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Declares a 4D vector with the supplied xyzw values.
			//
			// /////////////////////////////////////////////////////////////////
			explicit inline Vector4(const F32 x, const F32 y, const F32 z, const F32 w)\
				{ m_vec[0] = x; m_vec[1] = y; m_vec[2] = z; m_vec[3] = w; };

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Declares a 4D vector with the xyzw componenets having the same
			// initial value.
			//
			// /////////////////////////////////////////////////////////////////
			explicit inline Vector4(const F32 scaler) { m_vec[0] = scaler; m_vec[1] = scaler; m_vec[2] = scaler; m_vec[3] = scaler; };

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Declares a 4D vector with a plain C style array.
			//
			// /////////////////////////////////////////////////////////////////
			explicit inline Vector4(const F32 vecArr[NUMBER_COMPONENTS]) { memcpy(m_vec, vecArr, sizeof(F32) * NUMBER_COMPONENTS); };

			// /////////////////////////////////////////////////////////////////
			// Copy Constructor.
			//
			// Declares a 4D vector with the supplied 4D Vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector4(const Vector4 &copyVec) { memcpy(m_vec, copyVec.m_vec, sizeof(F32) * NUMBER_COMPONENTS); };

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Converts a 3D point to a positional Vector4 with a w component (with
			// a value of 1).
			//
			// The Vector4 has a w component of 1 to make scaling unnessecary as
			// homogenous coordinates with a w value greater than 1 must have their
			// xyz coordinates divided by the w component to get the original 3D
			// point (See Mathematics for Computer Graphics b John Vince, 2nd
			// Edition, Chapter 7 Transformations, pg. 57).
			//
			// In math textbooks a positional vector has a tail at the origin
			// of the world coordinates and a head at the position of the point.
			//
			// /////////////////////////////////////////////////////////////////
			explicit Vector4(const Point3 &pt);

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Converts a 3D vector to a 4D vector with a w component(with a
			// value of 0).
			//
			// A vector is defined by two points [x, y, z, 1] and [x', y', z', 1].
			// The reason that the w component of the 4D homogenous vector is 0 is
			// that the w component collapses to 0 when you subtract two points
			// (See Mathematics for Computer Graphics by John Vince, 2nd Edition,
			// Chapter 7 Transformations, pg. 99).
			//
			// /////////////////////////////////////////////////////////////////
			explicit inline Vector4(const Vector3 &vec3)
				{ m_vec[0] = vec3.GetX(); m_vec[1] = vec3.GetY(); m_vec[2] = vec3.GetZ(); m_vec[3] = 0.0f; };

			// /////////////////////////////////////////////////////////////////
			// Destructor.
			//
			// /////////////////////////////////////////////////////////////////
			~Vector4() { };

			// /////////////////////////////////////////////////////////////////
			// Getter/Setter methods.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 GetX() const { return (m_vec[0]); };
			inline F32 GetY() const { return (m_vec[1]); };
			inline F32 GetZ() const { return (m_vec[2]); };
			inline F32 GetW() const { return (m_vec[3]); };
			inline void SetX(const F32 x){ m_vec[0] = x; };
			inline void SetY(const F32 y){ m_vec[1] = y; };
			inline void SetZ(const F32 z){ m_vec[2] = z; };
			inline void SetW(const F32 w){ m_vec[3] = w; };
			inline void Set(const F32 x, const F32 y, const F32 z, const F32 w) {m_vec[0] = x; m_vec[1] = y; m_vec[2] = z; m_vec[3] = w;};

			// /////////////////////////////////////////////////////////////////
			// Assign the components of a Vector4 to this vector.
			//
			// @param rhs The Vector to assign to this Vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector4 &operator=(const Vector4 &rhs)
			{
				if(this == &rhs)
				{
					return (*this);
				}

				m_vec[0] = rhs.m_vec[0];
				m_vec[1] = rhs.m_vec[1];
				m_vec[2] = rhs.m_vec[2];
				m_vec[3] = rhs.m_vec[3];
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Assign the components of a Vector3 to this vector.
			//
			// @param rhs The Vector to assign to this Vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector4 &operator=(const Vector3 &rhs)
			{
				m_vec[0] = rhs.GetX();
				m_vec[1] = rhs.GetY();
				m_vec[2] = rhs.GetZ();
				m_vec[3] = 0.0f;
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Assign the components of a Point3 to this vector.
			//
			// @param rhs The Vector to assign to this Vector.
			//
			// /////////////////////////////////////////////////////////////////
			Vector4 &operator=(const Point3 &rhs);

			// /////////////////////////////////////////////////////////////////
			// Check if two vectors are equivalent.  They are equivalent in general
			// if their magnitude and direction are equal.  In simple terms we can
			// say they are equivalent if their components are all equivalent.
			//
			// @param rhs The Vector to check against this Vector.
			//
			// @return bool True if two vectors are equal and false otherwise.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool operator==(const Vector4 &rhs) const
			{
				if(this == &rhs) return (true);
				return (FloatCmp(m_vec[0], rhs.m_vec[0]) && FloatCmp(m_vec[1], rhs.m_vec[1]) && FloatCmp(m_vec[2], rhs.m_vec[2])\
					&& FloatCmp(m_vec[3], rhs.m_vec[3]));
			};

			// /////////////////////////////////////////////////////////////////
			// Check if two vectors are not equivalent.
			//
			// @param rhs The Vector to check against this Vector.
			//
			// @return bool True if two vectors are not equal and false otherwise.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool operator!=(const Vector4 &rhs) const
			{
				return (!(*this == rhs));
			};

			// /////////////////////////////////////////////////////////////////
			// Add a vector to this vector and return the resulting Vector.
			//
			// @param rhs The Vector to add to this Vector.
			//
			// @return Vector4 A copy of the resulting vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector4 operator+(const Vector4 &rhs) const
			{
				return (Vector4(m_vec[0] + rhs.m_vec[0], m_vec[1] + rhs.m_vec[1], m_vec[2] + rhs.m_vec[2], m_vec[3] + rhs.m_vec[3]));
			};

			// /////////////////////////////////////////////////////////////////
			// Subtract a vector from this vector and return the resulting Vector.
			//
			// @param rhs The Vector to subtract from this Vector.
			//
			// @return Vector4 A copy of the resulting vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector4 operator-(const Vector4 &rhs) const
			{
				return (Vector4(m_vec[0] - rhs.m_vec[0], m_vec[1] - rhs.m_vec[1], m_vec[2] - rhs.m_vec[2], m_vec[3] - rhs.m_vec[3]));
			};

			// /////////////////////////////////////////////////////////////////
			// Add a vector to this vector.
			//
			// @param rhs The Vector to add to this Vector.
			//
			// @return Vector4& The new version of this Vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector4 &operator+=(const Vector4 &rhs)
			{
				m_vec[0] += rhs.m_vec[0];
				m_vec[1] += rhs.m_vec[1];
				m_vec[2] += rhs.m_vec[2];
				m_vec[3] += rhs.m_vec[3];
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Subtract a vector from this vector.
			//
			// @param rhs The Vector to subtract from this Vector.
			//
			// @return Vector4& The new version of this Vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector4 &operator-=(const Vector4 &rhs)
			{
				m_vec[0] -= rhs.m_vec[0];
				m_vec[1] -= rhs.m_vec[1];
				m_vec[2] -= rhs.m_vec[2];
				m_vec[3] -= rhs.m_vec[3];
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Negate a vector.
			//
			// @return Vector3& The new version of this Vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector4 operator-() const
			{
				return (Vector4(-m_vec[0], -m_vec[1], -m_vec[2], -m_vec[3]));
			};

			// /////////////////////////////////////////////////////////////////
			// Multiply a vector by a scaler.
			//
			// @param scaler The scaler value.
			//
			// @return Vector3 A copy of the resulting Vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector4 operator*(const F32 scaler) const
			{
				Vector4 copy(*this);
				copy.m_vec[0] *= scaler;
				copy.m_vec[1] *= scaler;
				copy.m_vec[2] *= scaler;
				copy.m_vec[3] *= scaler;
				return (copy);
			};

			// /////////////////////////////////////////////////////////////////
			// Divide a vector by a scaler.
			//
			// @param scaler The scaler value.
			//
			// @return Vector4 A copy of the resulting Vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector4 operator/(const F32 scaler) const
			{
				Vector4 copy(*this);
				if(!FloatCmp(scaler, 0.0f))
				{
					copy.m_vec[0] /= scaler;
					copy.m_vec[1] /= scaler;
					copy.m_vec[2] /= scaler;
					copy.m_vec[3] /= scaler;
				}
				return (copy);
			};

			// /////////////////////////////////////////////////////////////////
			// Multiply this vector by a scaler.
			//
			// @param scaler The scaler value.
			//
			// @return Vector3& This vector with the scaler applied.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector4 &operator*=(const F32 scaler)
			{
				m_vec[0] *= scaler;
				m_vec[1] *= scaler;
				m_vec[2] *= scaler;
				m_vec[3] *= scaler;
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Divide this vector by a scaler.
			//
			// @param scaler The scaler value.
			//
			// @return Vector3& This vector with the scaler applied.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector4 &operator/=(const F32 scaler)
			{
				if(!FloatCmp(scaler, 0.0f))
				{
					m_vec[0] /= scaler;
					m_vec[1] /= scaler;
					m_vec[2] /= scaler;
					m_vec[3] /= scaler;
				}
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Check if this Vector is less than another vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool operator<(const Vector4 &rhs) const
			{
				return (MagnitudeSqr() < rhs.MagnitudeSqr());
			};

			// /////////////////////////////////////////////////////////////////
			// Get the magnitude squared of this vector otherwise known as the
			// length of the vector.
			//
			// This version does not compute the square root and has no casts so
			// is faster if you want to compare two vectors lengths.  If you need
			// the actual length of the vector for some operation then you should
			// use the Magnitude() method instead.
			//
			// @return F32 The magnitude or length of this vector squared.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 MagnitudeSqr() const
			{
				return (m_vec[0]*m_vec[0] + m_vec[1]*m_vec[1] + m_vec[2]*m_vec[2] + m_vec[3]*m_vec[3]);
			};

			// /////////////////////////////////////////////////////////////////
			// Get the magnitude of this vector otherwise known as the
			// length of the vector.
			//
			// @return F32 The magnitude or length of this vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 Magnitude() const
			{
				return (static_cast<F32>(sqrt(static_cast<F32>(MagnitudeSqr()))));
			};

			// /////////////////////////////////////////////////////////////////
			// Normalize this vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector4 &Normalize()
			{
				const F32 length = Magnitude();
				if(!FloatCmp(length, 0.0f))
				{
					m_vec[0] /= length;
					m_vec[1] /= length;
					m_vec[2] /= length;
					m_vec[3] /= length;
				}
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Normalize this vector and store the result in the vector supplied
			// so as to NOT to modify this vector.
			//
			// @param outVecRef The vector to store the result of normalizing this
			//						vector in.
			//
			// /////////////////////////////////////////////////////////////////
			inline void Normalized(Vector4 &outVecRef) const
			{
				const F32 length = Magnitude();
				if(!FloatCmp(length, 0.0f))
				{
					outVecRef.m_vec[0] = m_vec[0] / length;
					outVecRef.m_vec[1] = m_vec[1] / length;
					outVecRef.m_vec[2] = m_vec[2] / length;
					outVecRef.m_vec[3] = m_vec[3] / length;
				}
			};

			// /////////////////////////////////////////////////////////////////
			// Calculate the dot product of this vector and another vector.
			// Otherwise known as the scaler product.
			//
			// Useful for back face culling or lighting calculations as it may be
			// used to calculate the angle between two vector.
			//
			// e.g. If a plane/primitives surface normal vectors' angle to this
			// vector is > 90, then it is said to be a back facing plane/primitive.
			//
			// @param rhsVecRef The other vector to use to get the scaler or dot
			//						product of, along with this vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 Dot(const Vector4 &rhsVecRef) const
			{
				return (m_vec[0]*rhsVecRef.m_vec[0] + m_vec[1]*rhsVecRef.m_vec[1] + m_vec[2]*rhsVecRef.m_vec[2] + m_vec[3]*rhsVecRef.m_vec[3]);
			};

			// /////////////////////////////////////////////////////////////////
			// Return a string of the Vector4s' components.
			//
			// @return std::string
			//
			// /////////////////////////////////////////////////////////////////
			inline std::string ToString() const
			{
				std::stringstream strStream;
				strStream << "[" << m_vec[0] << ", " << m_vec[1] << ", " << m_vec[2] << ", " << m_vec[3] << "]";
				return (strStream.str());
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
			inline const F32 * const GetComponentsConst() const
			{
				return (m_vec);
			};
	};

	// /////////////////////////////////////////////////////////////////
	// Multiply a vector by a scaler with the scaler on the lhs of the
	// expression.
	//
	// @param lhs The scaler.
	// @param rhs The Vector.
	//
	// @return Vector3 A copy of the resulting Vector.
	//
	// /////////////////////////////////////////////////////////////////
	inline Vector4 operator*(const F32 lhs, const Vector4 &rhs)
	{
		Vector4 copy(rhs);
		copy = rhs * lhs;
		return (copy);
	};

	// /////////////////////////////////////////////////////////////////
	// Performs linear interpolation on two 4 component vectors.
	//
	// @param a The start vector.
	// @param b The end vector.
	// @param interpolateVal Interpolation value (should be between 0
	//							and 1, if not it is clamped).
	// @param outVec Output vector.
	//
	// @return Vector4& Output vector.
	//
	// /////////////////////////////////////////////////////////////////
	Vector4 &Vector4Lerp(const Vector4 &a, const Vector4 &b, const F32 interpolateVal, Vector4 &outVec);

	// /////////////////////////////////////////////////////////////////
	// @class Point3
	// @author PJ O Halloran
	//
	// This class represents a 3D Point.
	//
	// Most Math APIs just represent a point as a 3D Vector however a 3D
	// vector class typically has operations that do not make sense for
	// points.  For example it does not make mathematical sense to add two
	// points.  As a result, I will use a seperate Point class for 3D points
	// and will provide conversion constructors to change from one type
	// to the other as required.  I got this idea from the Bullet VectorMath
	// library, not from a math textbook.
	//
	// Most methods are explicitly inlined unless they are more than 10
	// lines in length.  These methods deemed too long are in the cpp file.
	// This is to keep the object files sizes down.
	//
	// /////////////////////////////////////////////////////////////////
	class Point3
	{
		private:

			static const U32 NUMBER_COMPONENTS = 3;

			F32 m_vec[NUMBER_COMPONENTS];					///< XYZ component array.

		public:

			// /////////////////////////////////////////////////////////////////
			// Default constructor.
			//
			// Declares a Point at the origin.
			//
			// /////////////////////////////////////////////////////////////////
			explicit inline Point3() { memset(m_vec, 0, sizeof(F32) * NUMBER_COMPONENTS); };

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Declares a 3D point with the supplied xyz values.
			//
			// /////////////////////////////////////////////////////////////////
			explicit inline Point3(const F32 x, const F32 y, const F32 z) { m_vec[0] = x; m_vec[1] = y; m_vec[2] = z; };

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Declares a 3D point with the xyz componenets having the same
			// initial value.
			//
			// /////////////////////////////////////////////////////////////////
			explicit inline Point3(const F32 scaler) { m_vec[0] = scaler; m_vec[1] = scaler; m_vec[2] = scaler; };

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Declares a 3D point with a plain C style array.
			//
			// /////////////////////////////////////////////////////////////////
			explicit inline Point3(const F32 ptArr[NUMBER_COMPONENTS]) { memcpy(m_vec, ptArr, sizeof(F32) * NUMBER_COMPONENTS); };

			// /////////////////////////////////////////////////////////////////
			// Copy Constructor.
			//
			// Declares a 3D point with the supplied 3D Point.
			//
			// /////////////////////////////////////////////////////////////////
			inline Point3(const Point3 &copyPt) { memcpy(m_vec, copyPt.m_vec, sizeof(F32) * NUMBER_COMPONENTS); };

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// Converts a 3D positional vector to a point.
			//
			// /////////////////////////////////////////////////////////////////
			explicit inline Point3(const Vector3 &vec) { m_vec[0] = vec.GetX(); m_vec[1] = vec.GetY(); m_vec[2] = vec.GetZ(); };

			// /////////////////////////////////////////////////////////////////
			// Destructor.
			//
			// /////////////////////////////////////////////////////////////////
			~Point3() { };

			// /////////////////////////////////////////////////////////////////
			// Getter/Setter methods.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 GetX() const { return (m_vec[0]); };
			inline F32 GetY() const { return (m_vec[1]); };
			inline F32 GetZ() const { return (m_vec[2]); };
			inline void SetX(const F32 x){ m_vec[0] = x; };
			inline void SetY(const F32 y){ m_vec[1] = y; };
			inline void SetZ(const F32 z){ m_vec[2] = z; };
			inline void Set(const F32 x, const F32 y, const F32 z) {m_vec[0] = x; m_vec[1] = y; m_vec[2] = z;};

			// /////////////////////////////////////////////////////////////////
			// Assign a Point3 to this Point.
			//
			// @param rhs The Point to assign to this Point.
			//
			// /////////////////////////////////////////////////////////////////
			inline Point3 &operator=(const Point3 &rhs)
			{
				if(this == &rhs)
				{
					return (*this);
				}

				m_vec[0] = rhs.m_vec[0];
				m_vec[1] = rhs.m_vec[1];
				m_vec[2] = rhs.m_vec[2];
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Assign a Vector3 to this Point.
			//
			// @param rhs The Vector to assign to this Point.
			//
			// /////////////////////////////////////////////////////////////////
			inline Point3 &operator=(const Vector3 &rhs)
			{
				m_vec[0] = rhs.GetX();
				m_vec[1] = rhs.GetY();
				m_vec[2] = rhs.GetZ();
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Check if two points are equal.
			//
			// @param rhs The Point to check against this Point.
			//
			// @return bool True if the points are equal.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool operator==(const Point3 &rhs) const
			{
				if(this == &rhs)
				{
					return (true);
				}

				return (FloatCmp(m_vec[0], rhs.m_vec[0]) && FloatCmp(m_vec[1], rhs.m_vec[1]) && FloatCmp(m_vec[2], rhs.m_vec[2]));
			};

			// /////////////////////////////////////////////////////////////////
			// Check if two points are not equal.
			//
			// @param rhs The Point to check against this Point.
			//
			// @return bool True if the points are equal.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool operator!=(const Point3 &rhs) const
			{
				return (!(*this == rhs));
			};

			// /////////////////////////////////////////////////////////////////
			// Subtract another point from this point.  The result is the vector
			// between them.
			//
			// @param rhs The point to subtract from this point.
			//
			// @return Vector3 A copy of the resulting vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 operator-(const Point3 &rhs) const
			{
				return (Vector3(m_vec[0] - rhs.m_vec[0], m_vec[1] - rhs.m_vec[1], m_vec[2] - rhs.m_vec[2]));
			};

			// /////////////////////////////////////////////////////////////////
			// Add a vector to this Point.
			// The result is a point at the location ||rhs|| and the direction of
			// the vector rhs away from the original 3D point.
			//
			// @param rhs The Vector to add to this Vector.
			//
			// @return Vector3 A copy of the resulting vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Point3 operator+(const Vector3 &rhs) const
			{
				return (Point3(m_vec[0] + rhs.GetX(), m_vec[1] + rhs.GetY(), m_vec[2] + rhs.GetZ()));
			};

			// /////////////////////////////////////////////////////////////////
			// Subtract a vector to this Point.
			// The result is a point at the location ||rhs|| and the MINUS/opposite
			// direction of the vector away from the original 3D point.
			//
			// @param rhs The Vector to add to this Vector.
			//
			// @return Vector3 A copy of the resulting vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Point3 operator-(const Vector3 &rhs) const
			{
				return (Point3(m_vec[0] - rhs.GetX(), m_vec[1] - rhs.GetY(), m_vec[2] - rhs.GetZ()));
			};

			// /////////////////////////////////////////////////////////////////
			// Add a vector to this Point.
			// The result is a point at the location ||rhs|| and the direction of
			// the vector away from the original 3D point.
			//
			// @param rhs The Vector to add to this Vector.
			//
			// @return Vector3 A copy of the resulting vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Point3 &operator+=(const Vector3 &rhs)
			{
				m_vec[0] += rhs.GetX();
				m_vec[1] += rhs.GetY();
				m_vec[2] += rhs.GetZ();
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Subtract a vector to this Point.
			// The result is a point at the location ||rhs|| and the MINUS
			// direction of the vector away from the original 3D point.
			//
			// @param rhs The Vector to add to this Vector.
			//
			// @return Vector3 A copy of the resulting vector.
			//
			// /////////////////////////////////////////////////////////////////
			inline Point3 &operator-=(const Vector3 &rhs)
			{
				m_vec[0] -= rhs.GetX();
				m_vec[1] -= rhs.GetY();
				m_vec[2] -= rhs.GetZ();
				return (*this);
			};

			// /////////////////////////////////////////////////////////////////
			// Negate a Point.
			// 
			// /////////////////////////////////////////////////////////////////
			inline Point3 operator-() const
			{
				return (Point3(-m_vec[0], -m_vec[1], -m_vec[2]));
			};

			// /////////////////////////////////////////////////////////////////
			// Check if this Point is less than another point.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool operator<(const Point3 &rhs) const
			{
				return ((m_vec[0] < rhs.m_vec[0]) && (m_vec[1] < rhs.m_vec[1]) && (m_vec[2] < rhs.m_vec[2]));
			};

			// /////////////////////////////////////////////////////////////////
			// Check if this Point is less than or equal to another point.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool operator<=(const Point3 &rhs) const
			{
				return ((m_vec[0] <= rhs.m_vec[0]) && (m_vec[1] <= rhs.m_vec[1]) && (m_vec[2] <= rhs.m_vec[2]));
			};

			// /////////////////////////////////////////////////////////////////
			// Check if this Point is greater than another point.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool operator>(const Point3 &rhs) const
			{
				return ((m_vec[0] > rhs.m_vec[0]) && (m_vec[1] > rhs.m_vec[1]) && (m_vec[2] > rhs.m_vec[2]));
			};

			// /////////////////////////////////////////////////////////////////
			// Check if this Point is greater than or equal to another point.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool operator>=(const Point3 &rhs) const
			{
				return ((m_vec[0] >= rhs.m_vec[0]) && (m_vec[1] >= rhs.m_vec[1]) && (m_vec[2] >= rhs.m_vec[2]));
			};

			// /////////////////////////////////////////////////////////////////
			// Get the distance squared between two 3D points.
			//
			// Use this method if you want to compare two distance values as it
			// is fast (no casting or calculating the square root).  If you need
			// the actual distance, then please use Distance() instead.
			//
			// @param pt The other point to get the distance to.
			//
			// @return F32 The distance squared between two 3D points.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 DistanceSqr(const Point3 &pt) const
			{
				F32 dX = pt.m_vec[0] - m_vec[0];
				F32 dY = pt.m_vec[1] - m_vec[1];
				F32 dZ = pt.m_vec[2] - m_vec[2];
				return (dX*dX + dY*dY + dZ*dZ);
			};

			// /////////////////////////////////////////////////////////////////
			// Get the distance between two 3D points.
			//
			// @param pt The other point to get the distance to.
			//
			// @return F32 The distance between two 3D points.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 Distance(const Point3 &pt) const
			{
				return (static_cast<F32>(sqrt(static_cast<F32>(DistanceSqr(pt)))));
			};

			// /////////////////////////////////////////////////////////////////
			// Return a string of the Point3s' components.
			//
			// @return std::string
			//
			// /////////////////////////////////////////////////////////////////
			inline std::string ToString() const
			{
				std::stringstream strStream;
				strStream << "[" << m_vec[0] << ", " << m_vec[1] << ", " << m_vec[2] << "]";
				return (strStream.str());
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
			inline const F32 * const GetComponentsConst() const
			{
				return (m_vec);
			};
	};

	// /////////////////////////////////////////////////////////////////
	// Performs linear interpolation on two 3 component points.
	//
	// @param a The start point.
	// @param b The end point.
	// @param interpolateVal Interpolation value (should be between 0
	//							and 1, if not it is clamped).
	// @param outPt Output pt.
	//
	// @return Point3& Output pt.
	//
	// /////////////////////////////////////////////////////////////////
	Point3 &Point3Lerp(const Point3 &a, const Point3 &b, const F32 interpolateVal, Point3 &outPt);

	// Often used 3D points defined globally and externally.
	extern const Point3 g_originPt;
	// Define our global vectors describing the coordinate system in use.
	extern const Vector3 g_up;
	extern const Vector3 g_right;
	extern const Vector3 g_forward;
	extern const Vector4 g_up4;
	extern const Vector4 g_right4;
	extern const Vector4 g_forward4;
	// The unit vectors along the x, y and z axes.
	// Declared here as global constants as they will be used often.
	extern const Vector3 g_v3XunitVec;
	extern const Vector3 g_v3YunitVec;
	extern const Vector3 g_v3ZunitVec;

	// /////////////////////////////////////////////////////////////////
	// Given three points in 3D space, calulate the normal vector.
	//
	// The points must be supplied in counterclockwise order in order to
	// calculate the normal correctly.
	//
	// @param ptOne The first point.
	// @param ptTwo The second point.
	// @param ptThree The third point.
	// @param normalOut Object that contains details on the normal vector.
	//
	// /////////////////////////////////////////////////////////////////
	Vector3 &CalculateNormal(const Point3 &ptOne, const Point3 &ptTwo, const Point3 &ptThree, Vector3 &normalOut);

	// /////////////////////////////////////////////////////////////////
	// Given three points in 3D space, calulate the UNIT normal vector.
	//
	// The points must be supplied in counterclockwise order in order to
	// calculate the normal correctly.
	//
	// @param ptOne The first point.
	// @param ptTwo The second point.
	// @param ptThree The third point.
	// @param normalOut Object that contains details on the normal vector.
	//
	// /////////////////////////////////////////////////////////////////
	inline Vector3 &CalculateUnitNormal(const Point3 &ptOne, const Point3 &ptTwo, const Point3 &ptThree, Vector3 &normalOut)
	{
		normalOut = CalculateNormal(ptOne, ptTwo, ptThree, normalOut);
		normalOut.Normalize();
		return (normalOut);
	};

	// /////////////////////////////////////////////////////////////////
	// Generate a Vector3 with random xyz components between a defined
	// range.
	//
	// @param vec The Vector to hold the random componenets.
	// @param rng The random number generator.
	// @param minVec The minimum vector.
	// @param maxVec The maximum vector.
	//
	// @return Vector3& A reference to the vector with the random 
	//					components. It refers to the same object passed
	//					in.
	//
	// /////////////////////////////////////////////////////////////////
	Vector3 &GenerateRandomVector3(Vector3 &vec, CRandom &rng, const Vector3 &minVec, const Vector3 &maxVec);

	// /////////////////////////////////////////////////////////////////
	// Generate a Vector4 with random xyzw components.
	//
	// @param vec The Vector to hold the random componenets.
	// @param rng The random number generator.
	// @param minVec The minimum vector.
	// @param maxVec The maximum vector.
	//
	// @return Vector4& A reference to the vector with the random 
	//					components. It refers to the same object passed
	//					in.
	//
	// /////////////////////////////////////////////////////////////////
	Vector4 &GenerateRandomVector4(Vector4 &vec, CRandom &rng, const Vector4 &minVec, const Vector4 &maxVec);

	// /////////////////////////////////////////////////////////////////
	// Generate a Point3 with random xyz components.
	//
	// @param pt The Point3 object to hold the random componenets.
	// @param rng The random number generator.
	// @param minPt The minimum point.
	// @param maxPt The maximum point.
	//
	// @return Point3& A reference to the Point3 object with the random 
	//					components. It refers to the same object passed
	//					in.
	//
	// /////////////////////////////////////////////////////////////////
	Point3 &GenerateRandomPoint3(Point3 &pt, CRandom &rng, const Point3 &minPt, const Point3 &maxPt);

	// Typedefs of various lists
	typedef std::list<boost::shared_ptr<Vector3> > Vector3List;
	typedef std::list<boost::shared_ptr<Vector4> > Vector4List;
	typedef std::list<boost::shared_ptr<Point3> > Point3List;

	typedef std::list<Vector3> Vector3ValList;
	typedef std::list<Vector4> Vector4ValList;
	typedef std::list<Point3> Point3ValList;

}

#endif
