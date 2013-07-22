// /////////////////////////////////////////////////////////////////
// @file OsInputEvents.cpp
// @author PJ O Halloran
// @date 01/09/2010
//
// File contains the implemetation file for the OS and input events
// classes.
//
// /////////////////////////////////////////////////////////////////

#include <cstring>
#include <sstream>

#include "OsInputEvents.h"
#include "GameBase.h"
#include "GameMain.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // ********************** GfEventFactory ***************************
    // /////////////////////////////////////////////////////////////////

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GfEventFactory::CreateCloseWindowEvent(QuitEvent &eventObj) const
    {
        eventObj.id = GF_QUIT_EVENT;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GfEventFactory::CreateResizeWindowEvent(WindowResizeEvent &eventObj, const I32 width, const I32 height) const
    {
        eventObj.id = GF_WINDOW_RESIZE_EVENT;
        eventObj.w = width;
        eventObj.h = height;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GfEventFactory::CreateRefreshWindowEvent(WindowRefreshEvent &eventObj) const
    {
        eventObj.id = GF_WINDOW_REFRESH_EVENT;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GfEventFactory::CreateKeyEvent(KeyEvent &eventObj, const I32 keyId, const I32 action) const
    {
        eventObj.id = GF_KEY_EVENT;
        eventObj.keyId = keyId;
        eventObj.keyState = action;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GfEventFactory::CreateKeyCharacterEvent(KeyCharacterEvent &eventObj, const I32 characterId, const I32 action) const
    {
        eventObj.id = GF_KEY_CHARACTER_EVENT;
        eventObj.characterId = characterId;
        eventObj.characterState = action;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GfEventFactory::CreateMouseButtonEvent(MouseButtonEvent &eventObj, const I32 buttonId, const I32 action) const
    {
        eventObj.id = GF_MOUSE_BUTTON_EVENT;
        eventObj.buttonId = buttonId;
        eventObj.state = action;
        // Taking the mouse pos here will not give the best accuracy
        // of where the mouse pointer actually was when the button event occurred
        // but this is a limitation of GLFW that i have to work around...
#ifdef USE_NEW_GLFW
        glfwGetMousePos(g_appPtr->GetWindowManager()->GetGlfwWindowPtr(), &eventObj.x, &eventObj.y);
#else
        glfwGetMousePos(&eventObj.x, &eventObj.y);
#endif
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GfEventFactory::CreateMouseMoveEvent(MouseMoveEvent &eventObj, const I32 x, const I32 y, const I32 prevX, const I32 prevY) const
    {
        eventObj.id = GF_MOUSE_MOVE_EVENT;
        eventObj.x = x;
        eventObj.y = y;
        eventObj.xrel = ((prevX == 0) ? 0 : x - prevX);
        eventObj.yrel = ((prevY == 0) ? 0 : y - prevY);
    }

#ifdef USE_NEW_GLFW
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GfEventFactory::CreateMouseWheelEvent(MouseWheelEvent &eventObj, const I32 x, const I32 y) const
    {
        eventObj.id = GF_MOUSE_WHEEL_EVENT;
        eventObj.x = x;
        eventObj.y = y;
    }
#else
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GfEventFactory::CreateMouseWheelEvent(MouseWheelEvent &eventObj, const I32 position) const
    {
        eventObj.id = GF_MOUSE_WHEEL_EVENT;
        eventObj.pos = position;
    }
#endif

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GfEventFactory::CreateActiveWindowEvent(ActiveEvent &eventObj, const I32 active) const
    {
        eventObj.id = GF_ACTIVE_EVENT;
        eventObj.focus = active;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GfEventFactory::CreateJoystickStatusChangeEvent(JoystickStateChangeEvent &eventObj, const I32 joyId, const I32 status, const I32 numAxes, const I32 numButtons) const
    {
        eventObj.id = GF_JOYSTICK_STATE_CHANGE_EVENT;
        eventObj.joyId = joyId;
        eventObj.numAxes = numAxes;
        eventObj.numButtons = numButtons;
        eventObj.state = status;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GfEventFactory::CreateJoystickMoveEvent(JoystickMoveEvent &eventObj, const I32 joyId, const I32 axisId, const F32 position) const
    {
        eventObj.id = GF_JOYSTICK_MOVE_EVENT;
        eventObj.axisId = axisId;
        eventObj.joyId = joyId;
        eventObj.pos = position;

        // Ensure the range of positions is between -1 and 1.
        if(eventObj.pos < -1.0f) {
            eventObj.pos = -1.0f;
        } else if(eventObj.pos > 1.0f) {
            eventObj.pos = 1.0f;
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GfEventFactory::CreateJoystickButtonEvent(JoystickButtonEvent &eventObj, const I32 joyId, const I32 buttonId, const I32 buttonState) const
    {
        eventObj.id = GF_JOYSTICK_BUTTON_EVENT;
        eventObj.buttonId = buttonId;
        eventObj.joyId = joyId;
        eventObj.buttonState = buttonState;
    }

    // /////////////////////////////////////////////////////////////////
    // ********************** JoystickInfo *****************************
    // /////////////////////////////////////////////////////////////////

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void JoystickInfo::CreateStateArrays()
    {
        if(m_numAxes > 0) {
            m_axisPositionsArr = GCC_NEW F32[m_numAxes];
            memset(m_axisPositionsArr, 0, m_numAxes * sizeof(F32));
        }
        if(m_numButtons > 0) {
            m_buttonsArr = GCC_NEW unsigned char[m_numButtons];
            memset(m_buttonsArr, 0, m_numButtons * sizeof(unsigned char));
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void JoystickInfo::DestroyStateArrays()
    {
        DeleteArray(m_buttonsArr);
        DeleteArray(m_axisPositionsArr);
        m_buttonsArr = NULL;
        m_axisPositionsArr = NULL;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void JoystickInfo::CopyStateArrays(const F32 * const axisArr, const I32 axisArrSize, const unsigned char * const buttonsArr, const I32 buttonArrSize)
    {
        bool resize = false;

        if((m_numAxes != axisArrSize) || (m_numButtons != buttonArrSize)) {
            resize = true;
            m_numAxes = axisArrSize;
            m_numButtons = buttonArrSize;
        }

        if(resize) {
            CreateStateArrays();
        }

        if(axisArr && m_numAxes > 0) {
            memcpy(m_axisPositionsArr, axisArr, m_numAxes);
        }
        if(buttonsArr && m_numButtons > 0) {
            memcpy(m_buttonsArr, buttonsArr, m_numButtons);
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    JoystickInfo::JoystickInfo(const JoystickInfo &obj) : m_id(obj.m_id), m_playerId(obj.m_playerId), \
        m_axisPositionsArr(NULL), m_buttonsArr(NULL), m_wasPluggedIn(false)
    {
        // NB m_numAxes and m_numButtons are not set in member initialization list as they will be set in CopyStateArrays
        //  They need to be different to the size of  the other object to allocate the memory for them.
        if(m_wasPluggedIn) {
            CopyStateArrays(obj.m_axisPositionsArr, obj.m_numAxes, obj.m_buttonsArr, obj.m_numButtons);
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    JoystickInfo::~JoystickInfo()
    {
        try {
            DestroyStateArrays();
        } catch(...) {
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void JoystickInfo::operator=(const JoystickInfo &rhs)
    {
        // NB m_numAxes and m_numButtons are not set as they will be set in CopyStateArrays
        //  They need to be different to the size of  the other object to allocate the memory for them.
        m_id = rhs.m_id;
        m_playerId = rhs.m_playerId;
        m_wasPluggedIn = rhs.m_wasPluggedIn;

        if(m_wasPluggedIn) {
            CopyStateArrays(rhs.m_axisPositionsArr, rhs.m_numAxes, rhs.m_buttonsArr, rhs.m_numButtons);
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    std::string JoystickInfo::ToString(const bool verbose) const
    {
        std::stringstream strStream;
        strStream << "[ID: " << m_id << ", Number Axes: " << m_numAxes << ", Number Buttons: " << m_numButtons << ", playerId: ";
        if(IsPlayerAttached()) {
            strStream << m_playerId;
        } else {
            strStream << "No player attached";
        }
        strStream << ", Detected State: " << (m_wasPluggedIn ? "ON" : "OFF");

        if(verbose && m_wasPluggedIn) {
            strStream << ", Axis State: (";
            for(I32 i = 0; i < m_numAxes; ++i) {
                strStream << m_axisPositionsArr[i] << " ";
            }

            strStream << "), Button State: (";
            for(I32 i = 0; i < m_numButtons; ++i) {
                strStream << m_buttonsArr[i] << " ";
            }
            strStream << ")";
        }

        strStream << "]";
        return (strStream.str());
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool JoystickInfo::IsDetected() const
    {
        return (glfwGetJoystickParam(m_id, GLFW_PRESENT) == GL_TRUE);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void JoystickInfo::PollJoystickState(GfEventQueue &eventQueue, GfEventFactory *eventFactoryPtr, const bool pollAxis)
    {
        // Exit if we did not receive the event factory (GameMain has an instance of it and should pass
        // that here to prevent creating/destroing an object continuously every time we call this method).
        if(!eventFactoryPtr) {
            return;
        }

        // 1) Check if joystick is connected/disconnected currently...
        // If this state is different, then send joystick state change event
        // and update the internal class members...
        bool stateChange = false, detected = IsDetected();

        if(detected != m_wasPluggedIn) {
            // State change!
            stateChange = true;
            GF_Event eventObj;
            eventObj.id = GF_JOYSTICK_STATE_CHANGE_EVENT;
            eventFactoryPtr->CreateJoystickStatusChangeEvent(eventObj.joyStateChange, \
                    m_id, \
                    (detected ? 1 : 0), \
                    (detected ? m_numAxes : 0), \
                    (detected ? m_numButtons : 0));
            eventQueue.push_back(eventObj);

            if(detected) {
                m_wasPluggedIn = true;
                m_numAxes = glfwGetJoystickParam(m_id, GLFW_AXES);
                m_numButtons = glfwGetJoystickParam(m_id, GLFW_BUTTONS);
                CreateStateArrays();
            } else {
                m_wasPluggedIn = false;
                m_numAxes = 0;
                m_numButtons = 0;
                DestroyStateArrays();
            }
        }

        // 2) If joystick is plugged in then
        if(m_wasPluggedIn) {
            F32 *tmpAxisArr = NULL;
            unsigned char *tmpButtonsArr = NULL;

            if(m_numAxes > 0) {
                tmpAxisArr = GCC_NEW F32[m_numAxes];
                memset(tmpAxisArr, 0, sizeof(F32) * m_numAxes);
                glfwGetJoystickPos(m_id, tmpAxisArr, m_numAxes);
            }
            if(m_numButtons > 0) {
                tmpButtonsArr = GCC_NEW unsigned char[m_numButtons];
                memset(tmpButtonsArr, 0, sizeof(unsigned char) * m_numButtons);
                glfwGetJoystickButtons(m_id, tmpButtonsArr, m_numButtons);
            }

            // If the joystick was just plugged in then we will not send any events, we will just record
            //  the initial state of the joystick.
            if(!stateChange) {
                if(tmpAxisArr) {
                    // Check all axes for movement changes.
                    for(I32 axisI = 0; axisI < m_numAxes; ++axisI) {
                        // Rule out very tiny movements as sometimes axes may be off center or not calibrated 100%.
                        if(fabs(tmpAxisArr[axisI]) > 0.05f) {
                            if(pollAxis || ((!pollAxis) && DidAxisChange(m_axisPositionsArr[axisI], tmpAxisArr[axisI]))) {
                                // Joystick movement event!
                                GF_Event eventObj;
                                eventObj.id = GF_JOYSTICK_MOVE_EVENT;
                                eventFactoryPtr->CreateJoystickMoveEvent(eventObj.joyMove, m_id, axisI, tmpAxisArr[axisI]);
                                eventQueue.push_back(eventObj);
                            }
                        }
                    }
                }
                if(tmpButtonsArr) {
                    // Check all buttons for state changes
                    for(I32 buttonI = 0; buttonI < m_numButtons; ++buttonI) {
                        if(m_buttonsArr[buttonI] != tmpButtonsArr[buttonI]) {
                            // Joystick button event!
                            GF_Event eventObj;
                            eventObj.id = GF_JOYSTICK_BUTTON_EVENT;
                            eventFactoryPtr->CreateJoystickButtonEvent(eventObj.joyButton, m_id, buttonI, tmpButtonsArr[buttonI]);
                            eventQueue.push_back(eventObj);
                        }
                    }
                }
            }

            // Copy the new state values into the internal arrays before we leave the function for next time.
            if(tmpAxisArr) {
                memcpy(m_axisPositionsArr, tmpAxisArr, m_numAxes * sizeof(F32));
            }
            if(tmpButtonsArr) {
                memcpy(m_buttonsArr, tmpButtonsArr, m_numButtons * sizeof(unsigned char));
            }

            DeleteArray(tmpButtonsArr);
            DeleteArray(tmpAxisArr);
        }
    }

}
