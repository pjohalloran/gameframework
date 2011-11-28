//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: camera.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Defines a camera's position and orientation.
//         
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "camera.h"

namespace GameHalloran
{

	Camera::Camera()
	{
		_cameraType = AIRCRAFT;

		_pos   = Point3(g_originPt);
		_right = Vector3(g_right);
		_up    = Vector3(g_up);
		_look  = Vector3(g_forward);
	}

	Camera::Camera(CameraType cameraType)
	{
		_cameraType = cameraType;

		_pos   = Point3(g_originPt);
		_right = Vector3(g_right);
		_up    = Vector3(g_up);
		_look  = Vector3(g_forward);
	}

	Camera::~Camera()
	{
	}

	void Camera::getPosition(Point3 &pos)
	{
		pos = _pos;
	}

	void Camera::setPosition(const Point3 &pos)
	{
		_pos = pos;
	}

	void Camera::getRight(Vector3 &right)
	{
		right = _right;
	}

	void Camera::getUp(Vector3 &up)
	{
		up = _up;
	}

	void Camera::getLook(Vector3 &look)
	{
		look = _look;
	}

	void Camera::walk(F32 units)
	{
		// move only on xz plane for land object
		if( _cameraType == LANDOBJECT )
			_pos += Vector3(_look.GetX(), 0.0f, _look.GetZ()) * units;

		if( _cameraType == AIRCRAFT )
			_pos += _look * units;
	}

	void Camera::strafe(F32 units)
	{
		// move only on xz plane for land object
		if( _cameraType == LANDOBJECT )
			_pos += Vector3(_right.GetX(), 0.0f, _right.GetZ()) * units;

		if( _cameraType == AIRCRAFT )
			_pos += _right * units;
	}

	void Camera::fly(F32 units)
	{
		// move only on y-axis for land object
		if( _cameraType == LANDOBJECT )
			_pos.SetY(_pos.GetY() + units);

		if( _cameraType == AIRCRAFT )
			_pos += _up * units;
	}

	void Camera::pitch(F32 angle)
	{
		Matrix4 T;
		GameHalloran::BuildRotationArbitraryMatrix4(T, angle, _right.GetX(), _right.GetY(), _right.GetZ());

		// rotate _up and _look around _right vector
		_up = T * Vector4(_up);
		_look = T * Vector4(_look);
	}

	void Camera::yaw(F32 angle)
	{
		Matrix4 T;

		// rotate around world y (0, 1, 0) always for land object
		if( _cameraType == LANDOBJECT )
			GameHalloran::BuildRotationYMatrix4(T, angle);

		// rotate around own up vector for aircraft
		if( _cameraType == AIRCRAFT )
			GameHalloran::BuildRotationArbitraryMatrix4(T, angle, _up.GetX(), _up.GetY(), _up.GetZ());

		// rotate _right and _look around _up or y-axis
		_right = T * Vector4(_right);
		_look = T * Vector4(_look);
	}

	void Camera::roll(F32 angle)
	{
		// only roll for aircraft type
		if( _cameraType == AIRCRAFT )
		{
			Matrix4 T;
			GameHalloran::BuildRotationArbitraryMatrix4(T, angle, _look.GetX(), _look.GetY(), _look.GetZ());

			// rotate _up and _right around _look vector
			_right = T * Vector4(_right);
			_up = T * Vector4(_up);
		}
	}

	void Camera::getViewMatrix(Matrix4 &viewMatrix)
	{
		// Keep camera's axes orthogonal to eachother
		_look.Normalize();

		_up.Cross(_look, _right);
		_up.Normalize();

		_right.Cross(_up, _look);
		_right.Normalize();

		// Build the view matrix:
		F32 x = -_right.Dot(Vector3(_pos));
		F32 y = -_up.Dot(Vector3(_pos));
		F32 z = -_look.Dot(Vector3(_pos));

		viewMatrix[Matrix4::M00] = _right.GetX();
		viewMatrix[Matrix4::M01] = _up.GetX();
		viewMatrix[Matrix4::M02] = _look.GetX();
		viewMatrix[Matrix4::M03] = 0.0f;

		viewMatrix[Matrix4::M10] = _right.GetY();
		viewMatrix[Matrix4::M11] = _up.GetY();
		viewMatrix[Matrix4::M12] = _look.GetY();
		viewMatrix[Matrix4::M13] = 0.0f;

		viewMatrix[Matrix4::M20] = _right.GetZ();
		viewMatrix[Matrix4::M21] = _up.GetZ();
		viewMatrix[Matrix4::M22] = _look.GetZ();
		viewMatrix[Matrix4::M23] = 0.0f;

		viewMatrix[Matrix4::M30] = x;
		viewMatrix[Matrix4::M31] = y;
		viewMatrix[Matrix4::M32] = z;
		viewMatrix[Matrix4::M33] = 1.0f;

		//(*V)(0,0) = _right.x; (*V)(0, 1) = _up.x; (*V)(0, 2) = _look.x; (*V)(0, 3) = 0.0f;
		//(*V)(1,0) = _right.y; (*V)(1, 1) = _up.y; (*V)(1, 2) = _look.y; (*V)(1, 3) = 0.0f;
		//(*V)(2,0) = _right.z; (*V)(2, 1) = _up.z; (*V)(2, 2) = _look.z; (*V)(2, 3) = 0.0f;
		//(*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
	}

	void Camera::getViewMatrixNoTranslation(Matrix4 &viewMatrix)
	{
		// Keep camera's axes orthogonal to eachother
		_look.Normalize();

		_up.Cross(_look, _right);
		_up.Normalize();

		_right.Cross(_up, _look);
		_right.Normalize();

		// Build the view matrix:
		viewMatrix[Matrix4::M00] = _right.GetX();
		viewMatrix[Matrix4::M01] = _up.GetX();
		viewMatrix[Matrix4::M02] = _look.GetX();
		viewMatrix[Matrix4::M03] = 0.0f;

		viewMatrix[Matrix4::M10] = _right.GetY();
		viewMatrix[Matrix4::M11] = _up.GetY();
		viewMatrix[Matrix4::M12] = _look.GetY();
		viewMatrix[Matrix4::M13] = 0.0f;

		viewMatrix[Matrix4::M20] = _right.GetZ();
		viewMatrix[Matrix4::M21] = _up.GetZ();
		viewMatrix[Matrix4::M22] = _look.GetZ();
		viewMatrix[Matrix4::M23] = 0.0f;

		viewMatrix[Matrix4::M30] = 0.0f;
		viewMatrix[Matrix4::M31] = 0.0f;
		viewMatrix[Matrix4::M32] = 0.0f;
		viewMatrix[Matrix4::M33] = 1.0f;
	}

	void Camera::setCameraType(CameraType cameraType)
	{
		_cameraType = cameraType;
	}

}
