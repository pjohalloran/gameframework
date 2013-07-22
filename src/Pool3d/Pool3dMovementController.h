// /////////////////////////////////////////////////////////////////
// @file Pool3dMovementController.h
// @author PJ O Halloran.
// @date 01/11/2010
//
// File contains the header for the Pool3d movement controller class.
//
// /////////////////////////////////////////////////////////////////

#ifndef __POOL_3D_MOVEMENT_CONTROLLER_H
#define __POOL_3D_MOVEMENT_CONTROLLER_H

// External Headers
#include <boost/shared_ptr.hpp>

// Project Headers
#include "IInputInterfaces.h"
#include "OsInputEvents.h"
#include "CameraSceneNode.h"

#include "Pool3dEvents.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // @class Pool3dMovementController
    // @author PJ O Halloran
    //
    // Class that implements a controller for the Pool3D application
    // using the keyboard and the mouse for camera and shot control.
    //
    // This is part of the *View* layer of the application and is closely
    // related to the Pool3dView class (which creates and destroys this).
    //
    // /////////////////////////////////////////////////////////////////
    class Pool3dMovementController : public IKeyboardHandler, public IMouseHandler {
    private:

        // States of the keyboard and mouse.
        bool m_keyArr[GF_NUMBER_KEYS];                          ///< Array of key states.
        Point3 m_lastMousePos;                                  ///< Last recorded position of the mouse.
        Point3 m_currMousePos;                                  ///< The current position of the mouse.
        bool m_mouseRecentlyMoved;                              ///< Was the mouse moved before the last update?
        boost::optional<I32> m_currMouseWheel;                  ///< Current wheel position.
        I32 m_lastMouseWheel;                                   ///< Last recorded mouse wheel position.
        bool m_lMouseButton;                                    ///< Press state of the left mouse button.
        bool m_rMouseButton;                                    ///< Press state of the right mouse button.
        bool m_mMouseButton;                                    ///< Press state of the middle mouse button.
        bool m_lockInput;                                       ///< Flag indicating if the user input is currently being ignored (useful for pause game or events where you don't want user affecting scene elements).

        boost::shared_ptr<CameraSceneNode> m_cameraNode;        ///< The camera scene node.
        boost::shared_ptr<SceneNode> m_cueNode;                 ///< The Cue scene node.

        // General parameters for all camera modes.
        ViewMode m_currMode;                                    ///< Current camera aiming mode (This is a copy of the Logic layers central value).
        F32 m_currRotY;                                     ///< Current angle of rotation about the Y axis of the camera.
        F32 m_currRotX;                                     ///< Current angle of rotation about the X axis of the camera.
        F32 m_rotAngle;                                     ///< Angle to rotate camera by per second (in degrees).
        F32 m_moveDistance;                                 ///< Distance to move camera by per second (in meters).
        F32 m_dzHeight;                                     ///< Height of the mouse dead zone box in the center of the screen.
        F32 m_dzWidth;                                      ///< Width of the mouse dead zone box in the center of the screen.

        // Table View mode parameters.
        F32 m_timeSinceLastInput;                               ///< Number of seconds elapsed since the last time the player changed the view.
        F32 m_tableViewTimeout;                             ///< Number of seconds allowed before the game returns to Shot aim mode.
        Point3 m_focusPos;                                      ///< Position to focus the camera on currently and to rotate about.
        F32 m_currZoomDistance;                             ///< Current distance from the camera to the focus point.
        F32 m_minRotX;                                      ///< Minimum angle of rotation allowed on X axis.
        F32 m_maxRotX;                                      ///< Maximum angle of rotation allowed on X axis.
        F32 m_minDistance;                                  ///< Minimum distance to the focus point from the camera.
        F32 m_maxDistance;                                  ///< Maximum distance from the focus point to the camera.

        // Shot mode parameters.
        I32 m_aimMode;                                          ///< Current shot aim mode when the camera and game is in SHOT view mode.
        Point3 m_cueBallPos;                                    ///< Position of the cue ball.
        boost::optional<Point3> m_nearPlayerBallPos;            ///< Position of the nearest valid ball to point towards at the beginning of a shot.
        F32 m_trailDistance;                                    ///< How far behind the cue ball the camera will fix itself.
        F32 m_currCueXRot;                                  ///< The current X rotation of the cue.
        F32 m_currCueYRot;                                  ///< The current Y rotation of the cue.
        F32 m_minCueRot;                                        ///< Minimum Cue rotation angle on any axis.
        F32 m_maxCueRot;                                        ///< Maximum cue rotation angle on any axis.
        F32 m_cueDepth;                                     ///< Depth/Length of the cue mesh.
        F32 m_ballRadius;                                       ///< Radius of the pool balls.
        F32 m_minCueDistance;                                   ///< Minimum distance from the cue tip to the ball.
        F32 m_maxCueDistance;                                   ///< Maximum distance from the cue tip to the ball.
        F32 m_currCueDistance;                              ///< The current distance from the cue tip to the cue ball.
        I32 m_currPlayerId;                                     ///< ID of the player to take the next shot.

        F32 m_cueAnimFloat;
        Point3 m_cueAnimStart;
        Point3 m_cueAnimEnd;
        F32 m_shotPower;

        // /////////////////////////////////////////////////////////////////
        // Get the relative mouse wheel position.
        //
        // /////////////////////////////////////////////////////////////////
        inline F32 GetRelativeMouseWheelPos() const {
            return (m_lastMouseWheel == 0 || !m_currMouseWheel.is_initialized() ? 0.0f : F32(*m_currMouseWheel - m_lastMouseWheel));
        };

        // /////////////////////////////////////////////////////////////////
        // Get the relative mouse position.
        //
        // /////////////////////////////////////////////////////////////////
        inline Point3 GetRelativeMousePos() const {
            if(m_lastMousePos != m_currMousePos) {
                return (Point3(Vector3(m_currMousePos) - Vector3(m_lastMousePos)));
            }

            return (g_originPt);
        };

        // /////////////////////////////////////////////////////////////////
        // Send out the EvtData_Camera_Aim_Mode_Request event to the logic
        // layer.
        //
        // /////////////////////////////////////////////////////////////////
        inline void BroadcastViewModeChangeRequestEvent(const enum ViewMode mode) const {
            /*safeQueEvent(IEventDataPtr(GCC_NEW EvtData_Camera_Aim_Mode_Request(mode)));*/ safeTriggerEvent(EvtData_Camera_Aim_Mode_Request(mode));
        };

        // /////////////////////////////////////////////////////////////////
        // Check if there was any player input since the last update.
        // any key pressed, mouse moved, etc.
        //
        // /////////////////////////////////////////////////////////////////
        bool ReceivedAnyInput() const;

        // /////////////////////////////////////////////////////////////////
        // Reset all input state on entering eShotInProgress.
        //
        // /////////////////////////////////////////////////////////////////
        void ResetAllState();

        // /////////////////////////////////////////////////////////////////
        // Called at the end of the update method to reset mouse movements.
        //
        // /////////////////////////////////////////////////////////////////
        void ResetMouseMovements();

        // /////////////////////////////////////////////////////////////////
        // Get the x and y rotation to apply to the camera or cue based on
        // the player key or mouse input since the last controller update.
        //
        // @param rotationAngle Angle to rotate by this frame.
        // @param xRotationRef The amount of rotation around the X axis.
        // @param yRotationRef The amount of rotation around the Y axis.
        //
        // @return bool True if there was any rotation and false if not.
        //
        // /////////////////////////////////////////////////////////////////
        bool GetUserXyRotation(const F32 rotationAngle, F32 &xRotationRef, F32 &yRotationRef) const;

        // /////////////////////////////////////////////////////////////////
        // Handle player input when we are in table view mode.
        //
        // /////////////////////////////////////////////////////////////////
        void TableViewModeUpdate(const F32 elapsedTime);

        // /////////////////////////////////////////////////////////////////
        // Handle player input when we are in shot view mode.
        //
        // /////////////////////////////////////////////////////////////////
        void ShotViewModeUpdate(const F32 elapsedTime);

        // /////////////////////////////////////////////////////////////////
        // Handle player input to direct the shot when we are in shooting mode.
        //
        // /////////////////////////////////////////////////////////////////
        void ShotSetupModeUpdate(const F32 elapsedTime);

        // /////////////////////////////////////////////////////////////////
        // Animates the cue going back and striking the cue ball.
        //
        // /////////////////////////////////////////////////////////////////
        void ShotInProgressUpdate(const F32 elapsedTime);

        // /////////////////////////////////////////////////////////////////
        // Handle player input when we are in free roam view mode.
        //
        // /////////////////////////////////////////////////////////////////
        void FreeRoamViewModeUpdate(const F32 elapsedTime);

        // /////////////////////////////////////////////////////////////////
        // Check if the mouse cursor is outside the dead zone of the screen.
        // The dead zone is an area in the centre of the screen where the mouse
        // cursor may be placed so that the scene orientation is not changed.
        //
        // /////////////////////////////////////////////////////////////////
        bool IsMouseInsideDeadZone() const;

        // /////////////////////////////////////////////////////////////////
        // Rotate the camera around a point in world space.  Camera will
        // rotate around a Y axis parallel to the world Y axis and around
        // the Cameras X axis within the constraints of the min and max
        // X angles supplied.
        //
        // @param elapsedTime Time elapsed since the last update.
        // @param focusPoint The point to focus upon.
        // @param minXRotAngle Minimum Camera X (right) axis rotation angle.
        // @param maxXRotAngle Maximum Camera X (right) axis rotation angle.
        //
        // @return bool True if any rotation was performed and false if not.
        //
        // /////////////////////////////////////////////////////////////////
        bool RotateCameraAroundPoint(const F32 elapsedTime, const Point3 &focusPoint, const F32 minXRotAngle = 0.0f, const F32 maxXRotAngle = 0.0f, F32 *xRotate = NULL, F32 *yRotate = NULL);

        // /////////////////////////////////////////////////////////////////
        // Adjust the shot spin amount.
        //
        // @param elapsedTime The time elapsed since the last update.
        // @param cueWorldMat The pos and orientation of the cue in world
        //                      space.
        //
        // @return bool True if shot spin was adjusted and false if not.
        //
        // /////////////////////////////////////////////////////////////////
        bool AdjustCueShotSpin(const F32 elapsedTime, Matrix4 &cueWorldMat);

        // /////////////////////////////////////////////////////////////////
        // Position and orient the camera in the scene.
        //
        // @param focusPoint The cameras position.
        // @param up The cameras up vector.
        // @param lookAt The cameras look at or forward vector.
        //
        // /////////////////////////////////////////////////////////////////
        void CameraPositionAndPoint(const Point3 &position, const Vector3 &up, const Vector3 &lookAt);

        // /////////////////////////////////////////////////////////////////
        // Resets camera and class state when the camera view mode is changed
        // externally.
        //
        // /////////////////////////////////////////////////////////////////
        void OnViewChangeEvent();

        // /////////////////////////////////////////////////////////////////
        // Resets some aim state when the aim mode is changed.
        //
        // /////////////////////////////////////////////////////////////////
        void OnAimChangeEvent();

        // /////////////////////////////////////////////////////////////////
        // Gets the velocity factor (0 <= x <= 1) to apply to the camera
        // movement or rotation depending on how far the mouse pointer is
        // from the screen centre.  The further away from the center the
        // greater the velocity.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetMousePointerVelocityFactor() const;

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param cameraNode Pointer to the camera scene node we are in
        //                      control of.
        // @param cueNode Pointer to the pool cue scene node.
        //
        // /////////////////////////////////////////////////////////////////
        Pool3dMovementController(boost::shared_ptr<CameraSceneNode> cameraNode, boost::shared_ptr<SceneNode> cueNode);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~Pool3dMovementController();

        // /////////////////////////////////////////////////////////////////
        // Called upon a key down/press event.
        //
        // @param id The ID of the key pressed.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnKeyDown(const I32 id);

        // /////////////////////////////////////////////////////////////////
        // Called upon a key up/release event.
        //
        // @param id The ID of the key released.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnKeyUp(const I32 id);

        // /////////////////////////////////////////////////////////////////
        // Called upon a mouse move event.
        //
        // @param currPos The current position of the mouse cursor.
        // @param relativePos The relative position of the mouse cursor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnMouseMove(const Point &currPos, const Point &relativePos);

#if USE_NEW_GLFW
        // /////////////////////////////////////////////////////////////////
        // Called upon a mouse wheel move event.
        //
        // @param x The new X position of the mouse wheel.
        // @param y The new Y position of the mouse wheel.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnMouseWheelMove(const I32 x, const I32 y);
#else
        // /////////////////////////////////////////////////////////////////
        // Called upon a mouse wheel move event.
        //
        // @param position The new position of the mouse wheel.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnMouseWheelMove(const I32 position);
#endif

        // /////////////////////////////////////////////////////////////////
        // Called upon a mouse button press.
        //
        // @param The ID of the mouse button that was pressed.
        // @param currPos The position of the mouse cursor when the button
        //                  was pressed.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnMouseButtonDown(const I32 buttonId, const Point &currPos);

        // /////////////////////////////////////////////////////////////////
        // Called upon a mouse left button release.
        //
        // @param The ID of the mouse button that was released.
        // @param currPos The position of the mouse cursor when the button
        //                  was released.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnMouseButtonUp(const I32 buttonId, const Point &currPos);

        // /////////////////////////////////////////////////////////////////
        // Update the position of the player controllable game objects
        // via player input.  Should be called once per game loop.
        //
        // @param elapsedTime The number of seconds since the last update.
        //
        // /////////////////////////////////////////////////////////////////
        void Update(const F32 elapsedTime);

        // /////////////////////////////////////////////////////////////////
        // Called to reset the pos and orientation of the camera behind the
        // cue balls current position.  Also places the cue game actor in line
        // with and facing the cue ball (facing in the same direction as the
        // camera).
        //
        // /////////////////////////////////////////////////////////////////
        void Reset();

        // /////////////////////////////////////////////////////////////////
        // Called to reset the game state held here when a new game starts.
        //
        // /////////////////////////////////////////////////////////////////
        void ResetGameState();

        // /////////////////////////////////////////////////////////////////
        // Get the current camera aiming mode.
        //
        // /////////////////////////////////////////////////////////////////
        ViewMode GetViewMode() const {
            return (m_currMode);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the current camera aiming mode.  This should only be set when
        // the logic layer changes its value.
        //
        // /////////////////////////////////////////////////////////////////
        void SetViewMode(const enum ViewMode mode) {
            m_currMode = mode;
            OnViewChangeEvent();
        };

        // /////////////////////////////////////////////////////////////////
        // Get the cue scene node pointer.
        //
        // /////////////////////////////////////////////////////////////////
        boost::shared_ptr<SceneNode> GetCueSceneNode() const {
            return (m_cueNode);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the cue scene node pointer.
        //
        // /////////////////////////////////////////////////////////////////
        void SetCueSceneNode(boost::shared_ptr<SceneNode> cueNode);

        // /////////////////////////////////////////////////////////////////
        // Get the maximum angle of table rotation per second.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetRotationAngle() const {
            return (m_rotAngle);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the maximum angle of table rotation per second.
        //
        // /////////////////////////////////////////////////////////////////
        void SetRotationAngle(const F32 rotAngle) {
            if(rotAngle >= 0.0f && rotAngle <= 360.0f) {
                m_rotAngle = rotAngle;
            }
        };

        // /////////////////////////////////////////////////////////////////
        // Get the maximum distance of camera movement per second (in meters).
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetMovementDistance() const {
            return (m_moveDistance);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the maximum distance of camera movement per second (in meters).
        //
        // /////////////////////////////////////////////////////////////////
        void SetMovementDistance(const F32 md) {
            m_moveDistance = md;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the timeout in seconds before we return from eTable mode to
        // eShotAim.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetTvTimeout() const {
            return (m_tableViewTimeout);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the timeout in seconds before we return from eTable mode to
        // eShotAim.
        //
        // /////////////////////////////////////////////////////////////////
        void SetTvTimeout(const F32 timeout) {
            if(timeout > 0.0f) {
                m_tableViewTimeout = timeout;
            }
        };

        // /////////////////////////////////////////////////////////////////
        // Get the focus position of the camera while in eTable mode.
        //
        // /////////////////////////////////////////////////////////////////
        Point3 GetTvFocusPosition() const {
            return (m_focusPos);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the focus position of the camera while in eTable mode.
        //
        // /////////////////////////////////////////////////////////////////
        void SetTvFocusPosition(const Point3 &position) {
            m_focusPos = position;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the current position of the cue ball.
        //
        // /////////////////////////////////////////////////////////////////
        Point3 GetSaCueBallPosition() const {
            return (m_cueBallPos);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the current position of the cue ball.
        //
        // /////////////////////////////////////////////////////////////////
        void SetSaCueBallPosition(const Point3 &position) {
            m_cueBallPos = position;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the current position of the nearest player ball.
        //
        // /////////////////////////////////////////////////////////////////
        boost::optional<Point3> GetSaNearestPlayerBallPosition() const {
            return (m_nearPlayerBallPos);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the current position of the nearest player ball.
        //
        // /////////////////////////////////////////////////////////////////
        void SetSaNearestPlayerBallPosition(const boost::optional<Point3> &position) {
            m_nearPlayerBallPos = position;
        };

        // /////////////////////////////////////////////////////////////////
        // Reset the nearest player ball position to hold no value.
        //
        // /////////////////////////////////////////////////////////////////
        void ResetSaNearestPlayerBallPosition() {
            m_nearPlayerBallPos.reset();
        };

        // /////////////////////////////////////////////////////////////////
        // Get the trail distance the camera will stay behind the cue ball
        // in eShotAim mode.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetSaTrailDistance() const {
            return (m_trailDistance);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the trail distance the camera will stay behind the cue ball
        // in eShotAim mode.
        //
        // /////////////////////////////////////////////////////////////////
        void SetSaTrailDistance(const F32 trailDistance) {
            if(trailDistance > 0.0f) {
                m_trailDistance = trailDistance;
            }
        };

        // /////////////////////////////////////////////////////////////////
        // Get the ball radius.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetSaBallRadius() const {
            return (m_ballRadius);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the ball radius.
        //
        // /////////////////////////////////////////////////////////////////
        void SetSaBallRadius(const F32 br) {
            m_ballRadius = br;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the current player index.
        //
        // /////////////////////////////////////////////////////////////////
        I32 GetSaCurrPlayerId() const {
            return (m_currPlayerId);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the current player index.
        //
        // /////////////////////////////////////////////////////////////////
        void SetSaCurrPlayerId(const I32 pid) {
            m_currPlayerId = pid;
        };

        // /////////////////////////////////////////////////////////////////
        // Check if the input is currently locked.
        //
        // /////////////////////////////////////////////////////////////////
        bool IsInputLocked() const {
            return (m_lockInput);
        };

        // /////////////////////////////////////////////////////////////////
        // Lock or unlock user input.
        //
        // /////////////////////////////////////////////////////////////////
        void SetInputLock(const bool flag) {
            m_lockInput = flag;
        };

    };

}

#endif
