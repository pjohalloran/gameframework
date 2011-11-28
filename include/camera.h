//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: camera.h
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Defines a camera's position and orientation.
//         
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __cameraH__
#define __cameraH__

//#include <d3dx9.h>
#include "Vector.h"
#include "Matrix.h"

namespace GameHalloran
{

	class Camera
	{
	public:
		enum CameraType { LANDOBJECT, AIRCRAFT };

		Camera();
		Camera(CameraType cameraType);
		~Camera();

		void strafe(F32 units); // left/right
		void fly(F32 units);    // up/down
		void walk(F32 units);   // forward/backward
		
		void pitch(F32 angle); // rotate on right vector
		void yaw(F32 angle);   // rotate on up vector
		void roll(F32 angle);  // rotate on look vector

		void getViewMatrix(Matrix4 &viewMatrix);
		void getViewMatrixNoTranslation(Matrix4 &viewMatrix);
		void setCameraType(CameraType cameraType); 
		void getPosition(Point3 &pos); 
		void setPosition(const Point3 &pos); 

		void getRight(Vector3 &right);
		void getUp(Vector3 &up);
		void getLook(Vector3 &look);
	private:
		CameraType  _cameraType;
		Vector3 _right;
		Vector3 _up;
		Vector3 _look;
		Point3 _pos;
	};

}

#endif // __cameraH__
