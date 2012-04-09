// /////////////////////////////////////////////////////////////////
// @file Pool3dMovementController.cpp
// @author PJ O Halloran.
// @date 01/11/2010
//
// File contains the header for the Pool3d movement controller class
// implementation.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <cstring>

#include "GameBase.h"


// Project Headers
#include "Pool3dMovementController.h"

#include "Pool3dEvents.h"
#include "Pool3dActors.h"


using boost::shared_ptr;


namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Pool3dMovementController::Pool3dMovementController(boost::shared_ptr<CameraSceneNode> cameraNode, boost::shared_ptr<SceneNode> cueNode)\
		: m_lastMousePos(), m_currMousePos(), m_mouseRecentlyMoved(false), m_currMouseWheel(), m_lastMouseWheel(0), m_lMouseButton(false), m_rMouseButton(false),\
			m_mMouseButton(false), m_lockInput(true), m_cameraNode(cameraNode), m_cueNode(cueNode), m_currMode(eShotAim), m_currRotX(0.0f), m_currRotY(0.0f), m_rotAngle(0.0f),\
				m_moveDistance(0.0f), m_dzHeight(0.0f), m_dzWidth(0.0f), m_timeSinceLastInput(0.0f), m_tableViewTimeout(0.0f), m_focusPos(),\
					m_currZoomDistance(0.0f), m_minRotX(0.0f), m_maxRotX(0.0f), m_minDistance(0.0f), m_maxDistance(0.0f), m_aimMode(eShotSetup), m_cueBallPos(),\
						m_nearPlayerBallPos(), m_trailDistance(0.0f), m_currCueXRot(0.0f), m_currCueYRot(0.0f), m_minCueRot(0.0f), m_maxCueRot(0.0f), m_cueDepth(0.0f),\
							m_ballRadius(0.0f), m_currCueDistance(0.0f), m_currPlayerId(0)
	{
		for(I32 i = 0; i < GF_NUMBER_KEYS; ++i)
		{
			m_keyArr[i] = false;
		}

		// Dead zone width/height is 10% of the screen.
		m_dzHeight = g_appPtr->GetWindowManager()->GetHeight() / 5.0f;
		m_dzWidth = g_appPtr->GetWindowManager()->GetWidth() / 5.0f;

		// Set camera ranges.
		m_minDistance = 0.1f;
		m_maxDistance = 5.0f;
		m_minRotX = -90.0f;
		m_maxRotX = 0.0f;

		// Set the cue min and max rotations
		m_minCueRot = -2.0f;
		m_maxCueRot = 2.0f;

		m_minCueDistance = 0.04f;
		m_maxCueDistance = 0.225f;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Pool3dMovementController::~Pool3dMovementController()
	{

	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMovementController::VOnKeyDown(const I32 id)
	{
		// bounds check
		if(id < 0 || id > GF_NUMBER_KEYS)
		{
			return (false);
		}

		// No input updates while the shot is in progress.
		if(m_lockInput)
		{
			return (true);
		}

		m_keyArr[id] = true;
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMovementController::VOnKeyUp(const I32 id)
	{
		if(id < 0 || id > GF_NUMBER_KEYS)
		{
			return (false);
		}

		// No input updates while the shot is in progress.
		if(m_lockInput)
		{
			return (true);
		}

		m_keyArr[id] = false;
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMovementController::VOnMouseMove(const Point &currPos, const Point &relativePos)
	{
		// No input updates while the shot is in progress.
		if(m_lockInput)
		{
			return (true);
		}

		m_lastMousePos = m_currMousePos;
		m_currMousePos.Set(F32(currPos.GetX()), F32(currPos.GetY()), 0.0f);
		GameHalloran::ConvertWindowCoordinates(m_currMousePos);
		m_mouseRecentlyMoved = true;
		return (true);
	}

#if USE_NEW_GLFW
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool Pool3dMovementController::VOnMouseWheelMove(const I32 x, const I32 y)
	{
		// No input updates while the shot is in progress.
		if(m_lockInput)
		{
			return (true);
		}
        
		m_currMouseWheel = x+y;
		return (true);
	}
#else
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMovementController::VOnMouseWheelMove(const I32 position)
	{
		// No input updates while the shot is in progress.
		if(m_lockInput)
		{
			return (true);
		}
        
		m_currMouseWheel = position;
		return (true);
	}
#endif

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMovementController::VOnMouseButtonDown(const I32 buttonId, const Point &currPos)
	{
		// No input updates while the shot is in progress.
		if(m_lockInput)
		{
			return (true);
		}

		switch(buttonId)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			{
				m_lMouseButton = true;
			}
			break;

		case GLFW_MOUSE_BUTTON_RIGHT:
			{
				m_rMouseButton = true;
			}
			break;

		case GLFW_MOUSE_BUTTON_MIDDLE:
			{
				m_mMouseButton = true;
			}
			break;

		default:
			break;
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMovementController::VOnMouseButtonUp(const I32 buttonId, const Point &currPos)
	{
		// No input updates while the shot is in progress.
		if(m_lockInput)
		{
			return (true);
		}

		switch(buttonId)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			{
				m_lMouseButton = false;
			}
			break;

		case GLFW_MOUSE_BUTTON_RIGHT:
			{
				m_rMouseButton = false;
			}
			break;

		case GLFW_MOUSE_BUTTON_MIDDLE:
			{
				m_mMouseButton = false;
			}
			break;

		default:
			break;
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMovementController::ResetAllState()
	{
		m_lMouseButton = m_rMouseButton = m_mMouseButton = false;
		m_mouseRecentlyMoved = false;

		for(U32 i = 0, size = GF_NUMBER_KEYS; i < size; ++i)
		{
			m_keyArr[i] = false;
		}

		m_lastMouseWheel = 0;
		m_currMouseWheel.reset();

		const F32 sw(g_appPtr->GetWindowManager()->GetWidth());				// Screen Width (this might change so we will query it once per loop).
		const F32 sh(g_appPtr->GetWindowManager()->GetHeight());				// Screen Height (this might change so we will query it once per loop).
		const F32 hsw(sw / 2.0f), hsh(sh / 2.0f);								// Half screen width and height.

		m_lastMousePos.Set(hsw, hsh, 0.0f);
		m_currMousePos.Set(hsw, hsh, 0.0f);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMovementController::ReceivedAnyInput() const
	{
		// Check input states in order of the quickiest checks.
		if(m_lMouseButton || m_rMouseButton || m_mMouseButton)
		{
			return (true);
		}
		if(GetRelativeMouseWheelPos() != 0.0f)
		{
			return (true);
		}
		if(m_mouseRecentlyMoved)
		{
			return (true);
		}

		bool keyPressed = false;
		for(U32 i = 0, size = GF_NUMBER_KEYS; ((!keyPressed) && (i < size)); ++i)
		{
			if(m_keyArr[i])
			{
				keyPressed = true;
			}
		}

		return (keyPressed);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMovementController::IsMouseInsideDeadZone() const
	{
		const F32 sw(g_appPtr->GetWindowManager()->GetWidth());				// Screen Width (this might change so we will query it once per loop).
		const F32 sh(g_appPtr->GetWindowManager()->GetHeight());				// Screen Height (this might change so we will query it once per loop).
		const F32 hsw(sw / 2.0f), hsh(sh / 2.0f);								// Half screen width and height.
		Point3 minDzPt(hsw - m_dzWidth, hsh - m_dzHeight, -0.1f);				// Min DZ point.
		Point3 maxDzPt(hsw + m_dzWidth, hsh + m_dzHeight, 0.1f);				// Max DZ point.

		if((m_currMousePos > minDzPt) && (m_currMousePos < maxDzPt))
		{
			return (true);
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	F32 Pool3dMovementController::GetMousePointerVelocityFactor() const
	{
		const F32 sw(g_appPtr->GetWindowManager()->GetWidth());				// Screen Width (this might change so we will query it once per loop).
		const F32 sh(g_appPtr->GetWindowManager()->GetHeight());				// Screen Height (this might change so we will query it once per loop).
		const F32 hsw(sw / 2.0f), hsh(sh / 2.0f);								// Half screen width and height.
		Point3 centrePt(hsw, hsh, 0.0f);										// Centre point of screen.

		// We just need the ratios so theres no need to calculate the actual Distance.
		F32 distanceCenterToTopLeft = centrePt.DistanceSqr(g_originPt);		// Distance from screen centre to top left window corner.
		F32 distanceMouseToCenter = m_currMousePos.DistanceSqr(centrePt);		// Distance from the current mouse position to the screen centre.

		return (distanceMouseToCenter / distanceCenterToTopLeft);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMovementController::CameraPositionAndPoint(const Point3 &position, const Vector3 &up, const Vector3 &lookAt)
	{
		m_cameraNode->GetGlFrame().SetPosition(position);
		m_cameraNode->GetGlFrame().SetForwardVector(lookAt);
		m_cameraNode->GetGlFrame().SetUpVector(up);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMovementController::OnViewChangeEvent()
	{
		m_currRotY = 0.0f;
		m_currRotX = 0.0f;

		switch(m_currMode)
		{
			case eShotAim:
			{
				ResetAllState();

				m_rotAngle = 90.0f;
				m_currCueXRot = 0.0f;
				m_currCueYRot = 0.0f;
				m_currRotX = 0.0f;
				m_currRotY = 0.0f;

				// Reset aim mode to the beginning.
				m_aimMode = 0;
				OnAimChangeEvent();

				// Reset the cue and camera behind the cue ball facing the nearest player ball
				Reset();

				// Unlock the user input on entering aim mode.
				m_lockInput = false;
			}
			break;

			case eTable:
			{
				m_rotAngle = 180.0f;
				m_currRotX = 0.0f;
				m_currRotY = 0.0f;

				// Set the camera to a known location.
				Point3 loc(0.0f, 0.0f, 3.0f);
				CameraPositionAndPoint(loc, g_up, g_forward);

				// Reset timer.
				m_timeSinceLastInput = 0.0f;

				// Calculate the current distance from the focus point (so we can avoid expensive Distance call later).
				m_currZoomDistance = loc.Distance(m_focusPos);
			}
			break;

			// nothing to do...
			case eShotDisplay:
			case eDebugFree:
			default:
			break;
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMovementController::GetUserXyRotation(const F32 rotationAngle, F32 &xRotationRef, F32 &yRotationRef) const
	{
		const F32 sw(g_appPtr->GetWindowManager()->GetWidth());				// Screen Width (this might change so we will query it once per loop).
		const F32 sh(g_appPtr->GetWindowManager()->GetHeight());				// Screen Height (this might change so we will query it once per loop).
		const F32 hsw(sw / 2.0f), hsh(sh / 2.0f);								// Half screen width and height.

		// Reset input parameters.
		xRotationRef = 0.0f;
		yRotationRef = 0.0f;

		// Ensure the user has the mouse outside the deadzone before we see if rotation is to be applied.
		if(IsMouseInsideDeadZone() && !m_keyArr['A'] && !m_keyArr['D'] && !m_keyArr['W'] && !m_keyArr['S'])
		{
			return (false);
		}

		// Rotate left
		if(m_keyArr['A'] || m_currMousePos.GetX() < hsw - m_dzWidth)
		{
			yRotationRef -= rotationAngle;
		}
		// Rotate right
		if(m_keyArr['D'] || m_currMousePos.GetX() > hsw + m_dzWidth)
		{
			yRotationRef += rotationAngle;
		}
		// Rotate up
		if(m_keyArr['W'] || m_currMousePos.GetY() > hsh + m_dzHeight)
		{
			xRotationRef += rotationAngle;
		}
		// Rotate down
		if(m_keyArr['S'] || m_currMousePos.GetY() < hsh - m_dzHeight)
		{
			xRotationRef -= rotationAngle;
		}

		return (xRotationRef != 0.0f || yRotationRef != 0.0f);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMovementController::RotateCameraAroundPoint(const F32 elapsedTime, const Point3 &focusPoint, const F32 minXRotAngle, const F32 maxXRotAngle, F32 *xRotate, F32 *yRotate)
	{
		F32 rotationAngle(m_rotAngle * elapsedTime);							// Angle to rotate by per second and per mouse position.
		F32 yRotAngle(0.0f);													// Angle to rotate around the world Y axis by.
		F32 xRotAngle(0.0f);													// Angle to rotate around the cameras X axis by.

		// If the mouse is currently outside the dead zone then scale rotation speed by its position.
		if(!IsMouseInsideDeadZone())
		{
			rotationAngle *= GetMousePointerVelocityFactor();
		}

		// Rotate camera around Y and X axis based on player input, if any
		if(GetUserXyRotation(rotationAngle, xRotAngle, yRotAngle))
		{
			// Update the current rotation counters and ensure we dont rotate past certain limits.
			if(yRotAngle != 0.0f)
			{
				m_currRotY += yRotAngle;
				Clamp<F32>(m_currRotY, 0.0f, 360.0f);
				//std::cout << "m_currRotY: " << m_currRotY << std::endl;
				//std::cout << "yRotAngle: " << yRotAngle << std::endl;
				if(yRotate)
				{
					*yRotate = yRotAngle;
				}
			}
			if(xRotAngle != 0.0f)
			{
				m_currRotX += xRotAngle;
				if(Clamp<F32>(m_currRotX, minXRotAngle, maxXRotAngle))
				{
					xRotAngle = 0.0f;
				}

				//std::cout << "m_currRotX: " << m_currRotX << std::endl;
				//std::cout << "xRotAngle: " << xRotAngle << std::endl;
				if(xRotate)
				{
					*xRotate = xRotAngle;
				}
			}
		}

		// After bounds check, check if we still have any rotations to apply to the camera.
		if(yRotAngle != 0.0f || xRotAngle != 0.0f)
		{
			// The old camera position and orientation.
			Point3 oldCamPos(m_cameraNode->GetGlFrame().GetPosition());
			Vector3 oldCamLookVec(m_cameraNode->GetGlFrame().GetForwardVector());
			Vector3 oldCamUpVec(m_cameraNode->GetGlFrame().GetUpVector());
			Vector3 oldCamRightVec;
			m_cameraNode->GetGlFrame().GetRightVector(oldCamRightVec);

			// Build rotation matrix for the camera.
			Matrix4 combRotMatrix(g_identityMat);
			if(yRotAngle != 0.0f)
			{
				BuildRotationYMatrix4(combRotMatrix, yRotAngle);
			}
			if(xRotAngle != 0.0f)
			{
				Matrix4 rotXMatrix;
				GameHalloran::BuildRotationArbitraryMatrix4(rotXMatrix, xRotAngle, oldCamRightVec.GetX(), oldCamRightVec.GetY(), oldCamRightVec.GetZ());
				combRotMatrix *= rotXMatrix;
			}

			// Apply rotation to camera vectors.
			Vector4 newCamUp(combRotMatrix * Vector4(oldCamUpVec));
			Vector4 newCamLook(combRotMatrix * Vector4(oldCamLookVec));

			// Rotate the difference vector between camera and focus position by the same amount as the camera vectors.
			Vector4 newCamPos4 = combRotMatrix * Vector4(oldCamPos - focusPoint);
			// To get new camera position, add back on the focus point vector to vector rotated in the previous step.
			newCamPos4 += Vector4(focusPoint);

			// Convert data to Vector3 and Point3 for Setters.
			Vector3 tmp(newCamPos4);
			Point3 newCamPos(tmp);
			Vector3 newCamUp3(newCamUp);
			Vector3 newCamLook3(newCamLook);

			CameraPositionAndPoint(newCamPos, newCamUp3, newCamLook3);

			return (true);
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMovementController::TableViewModeUpdate(const F32 elapsedTime)
	{
		// Go no further if we dont currently have a valid pointer to the camera scene node.
		if(!m_cameraNode)
		{
			return;
		}

		F32 zoomFactor(m_moveDistance * elapsedTime);							// The distance to zoom in or out by.
		I32 relativeWheel(GetRelativeMouseWheelPos());							// The relative position of the mouse wheel since it was last moved.
		F32 zoomValue = 0.0f;													// The actual value we will increase or decrease the current zoom level by based on player input.

		// If the mouse wheel was recently moved, increase the zoom factor proportinally by how fast it was turned
		if(relativeWheel != 0)
		{
			zoomFactor *= abs(relativeWheel);
		}

		// Zoom out
		if(m_keyArr['X'] || relativeWheel < 0.0f)
		{
			zoomValue += zoomFactor;
		}
		// Zoom in
		if(m_keyArr['Z'] || relativeWheel > 0.0f)
		{
			zoomValue -= zoomFactor;
		}

		bool rotationApplied = RotateCameraAroundPoint(elapsedTime, m_focusPos, m_minRotX, m_maxRotX);

		// Check if we receieved any relavant key presses...
		if(rotationApplied || relativeWheel != 0 || m_keyArr['X'] || m_keyArr['Z'])
		{
			m_timeSinceLastInput = 0.0f;
		}
		else
		{
			// The user has not changed the view since the last update so we exit early.

			// Check if we should revert back to shot aim mode yet...
			m_timeSinceLastInput += elapsedTime;
			if(m_timeSinceLastInput > m_tableViewTimeout)
			{
				BroadcastViewModeChangeRequestEvent(eShotAim); 
			}
			return;
		}

		// Apply the current zoom factor.
		if(zoomValue != 0.0f)
		{
			m_currZoomDistance += zoomValue;
			
			//std::cout << "zoomValue: " << zoomValue << std::endl;
			//std::cout << "m_currZoomDistance: " << m_currZoomDistance << std::endl;

			if(!Clamp<F32>(m_currZoomDistance, m_minDistance, m_maxDistance))
			{
				Vector3 camLookVec(m_cameraNode->GetGlFrame().GetForwardVector());
				Vector3 camPosVec(m_cameraNode->GetGlFrame().GetPosition());

				Vector3 newCamPos(camPosVec + (camLookVec * -zoomValue));

				m_cameraNode->GetGlFrame().SetPosition(Point3(newCamPos));
			}
		}
	}
 
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dMovementController::AdjustCueShotSpin(const F32 elapsedTime, Matrix4 &cueWorldMat)
	{
		//F32 rotationAngle(m_rotAngle * elapsedTime);							// Angle to rotate by per second and per mouse position.
		//F32 yRotAngle(0.0f);													// Angle to rotate around the world Y axis by.
		//F32 xRotAngle(0.0f);													// Angle to rotate around the cameras X axis by.

		//// If the mouse is currently outside the dead zone then scale rotation speed by its position.
		//if(!IsMouseInsideDeadZone())
		//{
		//	rotationAngle *= GetMousePointerVelocityFactor();
		//}

		//if(GetUserXyRotation(rotationAngle, xRotAngle, yRotAngle))
		//{
		//	// TODO: Rotate the cue about its centre of rotation (to a limit).
		//	if(yRotAngle != 0.0f)
		//	{
		//		m_currCueYRot += yRotAngle;
		//		if(Clamp<F32>(m_currCueYRot, m_minCueRot, m_maxCueRot))
		//		{
		//			yRotAngle = 0.0f;
		//		}
		//	}
		//	if(xRotAngle != 0.0f)
		//	{
		//		m_currCueXRot += xRotAngle;
		//		if(Clamp<F32>(m_currCueXRot, m_minCueRot, m_maxCueRot))
		//		{
		//			xRotAngle = 0.0f;
		//		}
		//	}
		//}

		//if(yRotAngle != 0.0f || xRotAngle != 0.0f)
		//{
		//	Vector3 camRightVec;
		//	m_cameraNode->GetGlFrame().GetRightVector(camRightVec);

		//	// Build rotation matrix for the cue.
		//	Matrix4 combRotMatrix(g_identityMat);
		//	if(yRotAngle != 0.0f)
		//	{
		//		BuildRotationYMatrix4(combRotMatrix, yRotAngle);
		//	}
		//	if(xRotAngle != 0.0f)
		//	{
		//		Matrix4 rotXMatrix;
		//		GameHalloran::BuildRotationArbitraryMatrix4(rotXMatrix, xRotAngle, camRightVec.GetX(), camRightVec.GetY(), camRightVec.GetZ());
		//		combRotMatrix *= rotXMatrix;
		//	}

		//	// Apply rotation to Cue world matrix.
		//	cueWorldMat *= combRotMatrix;
		//}

		const F32 maxOffset(m_ballRadius);
		F32 moveOffsetFactor(elapsedTime);

		// If the mouse is currently outside the dead zone then scale rotation speed by its position.
		if(!IsMouseInsideDeadZone())
		{
			moveOffsetFactor *= GetMousePointerVelocityFactor();
		}

		F32 xRotAngle(0.0f), yRotAngle(0.0f);
		if(GetUserXyRotation(moveOffsetFactor, xRotAngle, yRotAngle))
		{
			if(yRotAngle != 0.0f)
			{
				m_currCueYRot += yRotAngle;
				if(Clamp<F32>(m_currCueYRot, -maxOffset, maxOffset))
				{
					yRotAngle = 0.0f;
				}
			}
			if(xRotAngle != 0.0f)
			{
				m_currCueXRot += xRotAngle;
				if(Clamp<F32>(m_currCueXRot, -maxOffset, maxOffset))
				{
					xRotAngle = 0.0f;
				}
			}
		}
		if(yRotAngle != 0.0f || xRotAngle != 0.0f)
		{
			Point3 cuePos;
			cueWorldMat.GetPosition(cuePos);

			Vector4 cueRight, cueUp;
			cueWorldMat.GetXDir(cueRight);
			cueWorldMat.GetYDir(cueUp);
			Vector3 cueRight3(cueRight), cueUp3(cueUp);

			cuePos += cueRight3 * yRotAngle;
			cuePos += cueUp3 * xRotAngle;

			cueWorldMat.SetPosition(cuePos);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMovementController::ShotSetupModeUpdate(const F32 elapsedTime)
	{
		bool sendCueEvent(false);			// Tell the logic layer to move the cue actor?
		bool viewRotated(false);			// Was the camera view rotated?
		bool cueRotated(false);				// Did the player rotate the cue?
		I32 powerChange(0);					// The value of how much the player adjusted the shot power.

		// Current Cue pos and orientation.
		Matrix4 cueWorldMat(m_cueNode->VGet()->GetToWorld());

		F32 camXRot(0.0f), camYRot(0.0f);

		// Decide if we are altering the camera direction or the cue direction.
		if(m_keyArr[GLFW_KEY_LSHIFT] || m_keyArr[GLFW_KEY_RSHIFT] || m_rMouseButton)
		{
			// Rotate the cue about its central axis depending on the user mouse and key input.
			cueRotated = AdjustCueShotSpin(elapsedTime, cueWorldMat);
			sendCueEvent = cueRotated;
		}
		else
		{
			// Rotate camera and cue around the world Y axis keeping the camera focused on the cue ball and "d" units behind it.
			viewRotated = RotateCameraAroundPoint(elapsedTime, m_cueBallPos, -40.0f, 5.0f, &camXRot, &camYRot);
		}

		// If the player rotated the camera, then the shot spin was not changed.
		//  We must move the cue to keep it in line with the cameras new pos and orientation.
		if(viewRotated)
		{
			Vector4 oldCueUp, oldCueForward, oldCueRight;
			Vector4 newCueUp, newCueForward, newCueRight;
			Vector3 newCueRight3;

			// Get the old cue FOR vectors.
			cueWorldMat.GetXDir(oldCueRight);
			cueWorldMat.GetYDir(oldCueUp);
			cueWorldMat.GetZDir(oldCueForward);

			// The new possible cue FOR vectors, matching the cameras new FOR vectors.
			newCueUp = Vector4(m_cameraNode->GetGlFrame().GetUpVector());
			newCueForward = Vector4(-m_cameraNode->GetGlFrame().GetForwardVector());
			m_cameraNode->GetGlFrame().GetRightVector(newCueRight3);
			newCueRight = -Vector4(newCueRight3);

			// Offset the cues orientation slightly about the camera right axis so its facing direction is a little under the camera view.
			Matrix4 rot;
			GameHalloran::BuildRotationArbitraryMatrix4(rot, 5.0f, newCueRight3.GetX(), newCueRight3.GetY(), newCueRight3.GetZ());
			newCueForward = rot * newCueForward;

			// Recalculate the position of the cue based on the facing direction and the location of the cue ball and the length of the cue.
			Vector3 cueBallPosVec(m_cueBallPos);
			Vector3 newCuePosVec = cueBallPosVec - (Vector3(newCueForward) * -((m_cueDepth / 2.0f) + m_currCueDistance));

			//// Restore the prev cue shot spin position, if any.
			//if(m_currCueXRot != 0.0f)
			//{
			//	//Matrix4 xRestore;
			//	//GameHalloran::BuildRotationArbitraryMatrix4(xRestore, m_currCueXRot, newCueRight.GetX(), newCueRight.GetY(), newCueRight.GetZ());
			//	//newCueRight = xRestore * newCueRight;
			//	//newCueForward = xRestore * newCueForward;
			//	//newCueUp = xRestore * newCueUp;

			//	newCuePosVec += Vector3(newCueRight) * m_currCueXRot;
			//}

			//if(m_currCueYRot != 0.0f)
			//{
			//	//Matrix4 yRestore;
			//	//GameHalloran::BuildRotationArbitraryMatrix4(yRestore, m_currCueYRot, newCueUp.GetX(), newCueUp.GetY(), newCueUp.GetZ());
			//	//newCueRight = yRestore * newCueRight;
			//	//newCueForward = yRestore * newCueForward;
			//	//newCueUp = yRestore * newCueUp;

			//	newCuePosVec += Vector3(newCueUp) * camYRot;
			//}

			// Change the cues pos and orientation.
			cueWorldMat.SetXDir(newCueRight);
			cueWorldMat.SetYDir(newCueUp);
			cueWorldMat.SetZDir(newCueForward);
			cueWorldMat.SetPosition(Point3(newCuePosVec));

			sendCueEvent = true;
		}

		// Move the cue backwards or forwards for more or less power using the mouse wheel or the Q,E keys
		if(m_keyArr['Q'])
		{
			powerChange -= 1;
		}
		if(m_keyArr['E'])
		{
			powerChange +=1;
		}
		powerChange += GetRelativeMouseWheelPos();

		// If there was an player power change input then adjust the cue position.
		if(powerChange != 0)
		{
			Point3 cuePos;
			cueWorldMat.GetPosition(cuePos);
			Vector4 cueForward4;
			cueWorldMat.GetZDir(cueForward4);

			// Calc the new cue position.
			cuePos += Vector3(cueForward4) * (F32(powerChange) * elapsedTime);

			// Get the distance to the cue ball and ensure it is within the max limits.
			F32 distanceToCueBall = m_cueBallPos.Distance(cuePos) - (m_cueDepth / 2.0f);
#if defined(DEBUG)
			//std::cout << "distanceToCueBall: " << distanceToCueBall << std::endl;
#endif
			if(distanceToCueBall > m_minCueDistance && distanceToCueBall < m_maxCueDistance)
			{
				m_currCueDistance = distanceToCueBall;
				cueWorldMat.SetPosition(cuePos);
				sendCueEvent = true;
			}
		}

		if(sendCueEvent)
		{
			safeTriggerEvent(EvtData_Move_Kinematic_Actor(*m_cueNode->VGet()->GetActorId(), cueWorldMat));
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMovementController::ShotInProgressUpdate(const F32 elapsedTime)
	{
		if(m_cueAnimFloat < 1.0f && m_cueAnimStart != m_cueAnimEnd)
		{
			// Current Cue pos and orientation.
			Matrix4 cueWorldMat(m_cueNode->VGet()->GetToWorld());

			Point3 oldCuePos;
			GameHalloran::Point3Lerp(m_cueAnimStart, m_cueAnimEnd, m_cueAnimFloat, oldCuePos);

			m_cueAnimFloat += elapsedTime * m_shotPower*750.0f*50.0f;

			Point3 newCuePos;
			GameHalloran::Point3Lerp(m_cueAnimStart, m_cueAnimEnd, m_cueAnimFloat, newCuePos);

			Vector3 cueDiffVec = newCuePos - oldCuePos;

			Vector4 cueFaceDir;
			cueWorldMat.GetZDir(cueFaceDir);
			cueFaceDir = -cueFaceDir;
			Vector3 cueFaceDir3(cueFaceDir);

			Point3 currCuePos;
			cueWorldMat.GetPosition(currCuePos);
			Vector3 currCuePosVec(currCuePos);

			currCuePosVec += cueFaceDir3 * cueDiffVec.Magnitude();
			currCuePos = currCuePosVec;

			if(m_cueAnimFloat >= 1.0f)
			{
				// take shot once animation is complete for maximum realism...
				Vector4 camDir(m_cameraNode->GetGlFrame().GetForwardVector());
				Vector4 spinVec(0.0f);

				if(camDir != cueFaceDir)
				{
					spinVec = cueFaceDir3;
				}

				safeTriggerEvent(EvtData_Shot_Start(m_currPlayerId, cueFaceDir3, Vector3(spinVec), m_shotPower));
				return;
			}

			cueWorldMat.SetPosition(currCuePos);
			safeTriggerEvent(EvtData_Move_Kinematic_Actor(*m_cueNode->VGet()->GetActorId(), cueWorldMat));
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMovementController::OnAimChangeEvent()
	{
		switch(m_aimMode)
		{
			case eShotSetup:
			{
				
			}
			break;

			case eShotInProgress:
			{
				// Lock user input for duration of shot.
				m_lockInput = true;

				ResetAllState();

				// Calc shot power (shot will be taken once the cue animation has finished.).
				Matrix4 cueWorldMat(m_cueNode->VGet()->GetToWorld());
				Vector4 camDir(m_cameraNode->GetGlFrame().GetForwardVector());
				Vector4 cueDir;
				Point3 cuePos;
				cueWorldMat.GetPosition(cuePos);
				cueWorldMat.GetZDir(cueDir);

				F32 distanceToCueBall = m_cueBallPos.Distance(cuePos) - (m_cueDepth / 2.0f);
				m_shotPower = ((distanceToCueBall - m_minCueDistance)/(m_maxCueDistance - m_minCueDistance)) / 750.0f;
				//std::cout << "m_shotPower: " << m_shotPower << std::endl;

				// Calc cue animation start and end positions.
				m_cueAnimFloat = 0.0f;
				m_cueAnimStart = cuePos + (-Vector3(cueDir) * m_cueDepth/2.0f);
				m_cueAnimEnd = m_cueBallPos;
				//std::cout << "Cue travel distance: " << m_cueAnimStart.Distance(m_cueAnimEnd) << std::endl;
			}
			break;

			// nothing to do...
			default:
			break;
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMovementController::ShotViewModeUpdate(const F32 elapsedTime)
	{
		// If we dont have the PoolCue SceneNode pointer then we can't do anything here!
		if(!m_cueNode)
		{
			return;
		}

		// Go forward and back between shot modes.
		if(m_aimMode != eShotInProgress)
		{
			if(m_keyArr['Z'] || m_lMouseButton)
			{
				m_aimMode = eShotInProgress;
				OnAimChangeEvent();
			}
		}

		// Update the scene view and cue position based on player input.
		switch(m_aimMode)
		{
			case eShotSetup:		ShotSetupModeUpdate(elapsedTime);	break;
			case eShotInProgress:	ShotInProgressUpdate(elapsedTime);	break;
			default:													break;
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMovementController::FreeRoamViewModeUpdate(const F32 elapsedTime)
	{
		const F32 sw(g_appPtr->GetWindowManager()->GetWidth());				// Screen Width (this might change so we will query it once per loop).
		const F32 sh(g_appPtr->GetWindowManager()->GetHeight());				// Screen Height (this might change so we will query it once per loop).
		const F32 hsw(sw / 2.0f), hsh(sh / 2.0f);								// Half screen width and height.
		// unused F32 velFactor = GetMousePointerVelocityFactor();						// Velocity factor to apply based on position of mouse.
		F32 movementVelocity(m_moveDistance * elapsedTime);
		F32 rotationVelocity(m_rotAngle * elapsedTime);

		// Alter camera rotation speed depending how far away the mouse cursor is from the centre.
		if(!IsMouseInsideDeadZone())
		{
			F32 extraVelocity(GetMousePointerVelocityFactor());
			rotationVelocity *= extraVelocity;
		}

		// Speed up camera movement based on wheel position.
		I32 relativeWheel(GetRelativeMouseWheelPos());
		if(GetRelativeMouseWheelPos() != 0)
		{
			movementVelocity *= abs(relativeWheel);
		}

		// Rotate left
		if(m_keyArr['A'] || m_currMousePos.GetX() < hsw - m_dzWidth)
		{
			m_cameraNode->GetGlFrame().RotateLocalY(rotationVelocity);
		}
		// Rotate right
		if(m_keyArr['D'] || m_currMousePos.GetX() > hsw + m_dzWidth)
		{
			m_cameraNode->GetGlFrame().RotateLocalY(-rotationVelocity);
		}
		// Move forward
		if(m_keyArr['W'] || relativeWheel > 0)
		{
			m_cameraNode->GetGlFrame().MoveForward(movementVelocity);
		}
		// Move backward
		if(m_keyArr['S'] || relativeWheel < 0)
		{
			m_cameraNode->GetGlFrame().MoveForward(-movementVelocity);
		}
		// Look up
		if(m_keyArr[GLFW_KEY_PAGEUP] || m_currMousePos.GetY() > hsh + m_dzHeight)
		{
			m_cameraNode->GetGlFrame().RotateLocalX(rotationVelocity);
		}
		// Look down
		if(m_keyArr[GLFW_KEY_PAGEDOWN] || m_currMousePos.GetY() < hsh - m_dzHeight)
		{
			m_cameraNode->GetGlFrame().RotateLocalX(-rotationVelocity);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMovementController::ResetMouseMovements()
	{
		m_mouseRecentlyMoved = false;
		if(m_currMouseWheel.is_initialized())
		{
			m_lastMouseWheel = *m_currMouseWheel;
			m_currMouseWheel.reset();
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMovementController::Update(const F32 elapsedTime)
	{
		// don't allow the player to change the pos and orientation of the camera or view while the shot is in progress.
		if(m_lockInput)
		{
			if(m_aimMode == eShotInProgress)
			{
				ShotInProgressUpdate(elapsedTime);
			}
			return;
		}

#ifdef _DEBUG

		//F64 startTime = g_appPtr->GetCurrTime();
#endif

		// See if the player tried to change camera modes first (only if a shot is currently not in progress).
		boost::optional<ViewMode> mode;
		if(m_aimMode != eShotInProgress)
		{
			if(m_keyArr['1'])
			{
				// Send table rotate aim mode event
				mode = eShotAim;
			}
			else if(m_keyArr['2'])
			{
				// Send shot aim mode event
				mode = eTable;
			}
			else if(m_keyArr['3'])
			{
				// Send debug Free camera roam event.
				mode = eDebugFree;
			}
		}

		// Ask the logic layer if its okay to change camera modes...
		if(mode.is_initialized() && mode != m_currMode)
		{
			BroadcastViewModeChangeRequestEvent(*mode); 
		}

		// Display the scene based on the current camera mode.  Also accept user input to alter the camera position and orientation.
		switch(m_currMode)
		{
			case eShotAim:		ShotViewModeUpdate(elapsedTime);		break;
			case eTable:		TableViewModeUpdate(elapsedTime);		break;
			case eDebugFree:	FreeRoamViewModeUpdate(elapsedTime);	break;
			case eShotDisplay:	default:	break;
		}

		ResetMouseMovements();

#ifdef _DEBUG
		//F64 endTime = g_appPtr->GetCurrTime();
		//std::cout << "Pool3dMovementController::Update(): Time Taken (ms): " << (endTime - startTime) * 1000.0 << std::endl;
#endif
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMovementController::Reset()
	{
		Vector3 dirToNearestBall(g_forward);						// Direction from the cue ball to the nearest ball.
		Vector3 cueBallPosVec(m_cueBallPos);						// Cue/White ball positional vector.

		// Find the direction vector that we must face the cue on and partly face the camera on 
		//  (if nearest ball is not known we just use the WORLD forward vector).
		if(m_nearPlayerBallPos.is_initialized())
		{
			dirToNearestBall = *m_nearPlayerBallPos - m_cueBallPos;
			dirToNearestBall.Normalize();
		}

		// Reverse the direction vector since we are thinking in terms of camera orientation.
		//  (The Right and Forward direction vectors are reversed when we consider things from the cameras POV). 
		dirToNearestBall = -dirToNearestBall;

		Vector3 cueForward, cueUp, cueRight, cuePosVec;				// Cue orientation and position vectors.

		m_currCueDistance = 0.1f;

		// Calc cue orientation and position.
		cueForward = dirToNearestBall;
		cueUp = g_up;
		cueForward.Cross(cueUp, cueRight);
		cueRight.Normalize();
		cuePosVec = cueBallPosVec - (cueForward * -((m_cueDepth / 2.0f) + m_currCueDistance));

		// Set cue position and orientation.
		if(m_cueNode)
		{
			Matrix4 cueMat(cueRight, cueUp, cueForward, Point3(cuePosVec));
			safeTriggerEvent(EvtData_Move_Kinematic_Actor(*m_cueNode->VGet()->GetActorId(), cueMat));
		}

		Vector3 camForward, camUp, camRight, camPosVec;				// Camera orientation and position vectors.

		// Calc camera orientation and position.
		camPosVec = cueBallPosVec - (cueForward * -m_trailDistance) + (g_up * 0.1f);
		camRight = cueRight;
		camForward = cueBallPosVec - camPosVec;
		camForward.Normalize();
		camForward.Cross(camRight, camUp);

		// Set camera position and orientation
		CameraPositionAndPoint(Point3(camPosVec), camUp, camForward);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMovementController::ResetGameState()
	{
		m_currPlayerId = 0;
		m_currMode = eShotAim;
		OnViewChangeEvent();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dMovementController::SetCueSceneNode(boost::shared_ptr<SceneNode> cueNode)
	{
		m_cueNode = cueNode;
		if(m_cueNode)
		{
			CueActorParams *cueParams = static_cast<CueActorParams *>(m_cueNode->VGetActorParams());
			if(cueParams)
			{
				m_cueDepth = cueParams->GetDepth();
			}
		}
	};
}
