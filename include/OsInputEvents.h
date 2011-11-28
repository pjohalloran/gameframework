// /////////////////////////////////////////////////////////////////
// @file OsInputEvents.h
// @author PJ O Halloran
// @date 01/09/2010
//
// File contains the header for the OS and input events containers
// to be passed around the application.
//
// GLFW does not have a very nice event mechanism like for example
// SDL.  It provides seperate callbacks for keyboard, window and mouse
// events.  Polling must be done on joysticks.  This gave me a problem
// in that I want one simple as possible structure to pass around OS
// events around the game application similar to how the previous input
// library I used worked (SDL) whereas GLFW is more fragmented.
//
// The purpose of the structures and classes defined here is to 
// help simplify the work the GameMain application class has to do
// in order to pass events efficiently around the system.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GF_OS_INPUT_EVENTS_H
#define __GF_OS_INPUT_EVENTS_H

// External Headers
#include <cmath>

#include <deque>
#include <string>

#include <boost/shared_ptr.hpp>

#include "GameBase.h"

// Project Headers


namespace GameHalloran
{

	// Joystick state changed values.
	#define GF_JOYSTICK_DETECTED			0x00000010
	#define GF_JOYSTICK_LOST				0x00000100

	#define GF_AXIS_MIN_CHANGE_DELTA		0.00010f

	#define GF_NUMBER_KEYS					GLFW_KEY_LAST

	// /////////////////////////////////////////////////////////////////
	// The event IDs.
	//
	// /////////////////////////////////////////////////////////////////
	enum GfEventId
	{
		GF_EMPTY_EVENT = 0,
		GF_MOUSE_BUTTON_EVENT,
		GF_MOUSE_WHEEL_EVENT,
		GF_MOUSE_MOVE_EVENT,
		GF_QUIT_EVENT,
		GF_WINDOW_REFRESH_EVENT,
		GF_WINDOW_RESIZE_EVENT,
		GF_ACTIVE_EVENT,
		GF_KEY_EVENT,
		GF_KEY_CHARACTER_EVENT,
		GF_JOYSTICK_STATE_CHANGE_EVENT,
		GF_JOYSTICK_MOVE_EVENT,
		GF_JOYSTICK_BUTTON_EVENT,
		GF_NUMBER_EVENTS
	};

	// /////////////////////////////////////////////////////////////////
	// @struct BaseEvent
	// @author PJ O Halloran
	//
	// Base event data.
	//
	// /////////////////////////////////////////////////////////////////
	struct BaseEvent
	{
		I32 id;					///< ID of the event.
	};

	// /////////////////////////////////////////////////////////////////
	// @struct MouseButtonEvent
	// @author PJ O Halloran
	//
	// Event produced when a mouse button is pressed.
	//
	// /////////////////////////////////////////////////////////////////
	struct MouseButtonEvent : public BaseEvent
	{
		I32 buttonId;				///< ID of the button that was pressed or released.
		I32 state;					///< The type of event (GLFW_PRESS or GLFW_RELEASE).
		I32 x, y;					///< Location of the mouse cursor in screen coordinates when the button event occured.
	};

	// /////////////////////////////////////////////////////////////////
	// @struct MouseWheelEvent
	// @author PJ O Halloran
	//
	// Event produced when the mouse wheel is moved.
	//
	// /////////////////////////////////////////////////////////////////
	struct MouseWheelEvent : public BaseEvent
	{
#ifdef USE_NEW_GLFW
        I32 x;
        I32 y;
#else
		I32 pos;					///< New position of the mouse wheel.
#endif
	};

	// /////////////////////////////////////////////////////////////////
	// @struct MouseMoveEvent
	// @author PJ O Halloran
	//
	// Event produced when the mouse itself is moved.
	//
	// /////////////////////////////////////////////////////////////////
	struct MouseMoveEvent : public BaseEvent
	{
		I32 x, y;					///< The new screen coordinates of the mouse.
		I32 xrel, yrel;				///< The coordinates of the mouse relative to the last reported mouse move event.
	};

	// /////////////////////////////////////////////////////////////////
	// @struct QuitEvent
	// @author PJ O Halloran
	//
	// Event produced when the application is quit.
	//
	// /////////////////////////////////////////////////////////////////
	struct QuitEvent : public BaseEvent
	{
	};

	// /////////////////////////////////////////////////////////////////
	// @struct WindowRefreshEvent
	// @author PJ O Halloran
	//
	// Event produced when the window should be refreshed.
	//
	// /////////////////////////////////////////////////////////////////
	struct WindowRefreshEvent : public BaseEvent
	{
	};

	// /////////////////////////////////////////////////////////////////
	// @struct WindowResizeEvent
	// @author PJ O Halloran
	//
	// Event produced when a window resize event occurs.
	//
	// /////////////////////////////////////////////////////////////////
	struct WindowResizeEvent : public BaseEvent
	{
		I32 w, h;					///< The new width and height of the window.
	};

	// /////////////////////////////////////////////////////////////////
	// @struct ActiveEvent
	// @author PJ O Halloran
	//
	// Event produced when the window loses or gains focus.
	//
	// /////////////////////////////////////////////////////////////////
	struct ActiveEvent : public BaseEvent
	{
		I32 focus;					///< Has a value of 0 if the window is not in focus and 1 if it is in focus.
	};

	// /////////////////////////////////////////////////////////////////
	// @struct KeyEvent
	// @author PJ O Halloran
	//
	// Event produced when a key is pressed or released.
	//
	// /////////////////////////////////////////////////////////////////
	struct KeyEvent : public BaseEvent
	{
		I32 keyId;					///< Key identifier.
		I32 keyState;				///< State of the key (GLFW_PRESS or GLFW_RELEASE).
	};

	// /////////////////////////////////////////////////////////////////
	// @struct KeyCharacterEvent
	// @author PJ O Halloran
	//
	// Event produced when a character key is pressed or released.
	//
	// /////////////////////////////////////////////////////////////////
	struct KeyCharacterEvent : public BaseEvent
	{
		I32 characterId;			///< Key character.
		I32 characterState;			///< State of the character (GLFW_PRESS or GLFW_RELEASE).
	};

	// /////////////////////////////////////////////////////////////////
	// @struct JoystickStateChangeEvent
	// @author PJ O Halloran
	//
	// Event produced when a joystick is detected or when it is lost.
	//
	// If the joystick was lost, then the numAxes & numButtons fields
	// will not be relevant.
	//
	// /////////////////////////////////////////////////////////////////
	struct JoystickStateChangeEvent : public BaseEvent
	{
		I32 joyId;					///< ID of the joystick.
		I32 state;					///< State of the joystick (GF_JOYSTICK_DETECTED or GF_JOYSTICK_LOST).
		I32 numAxes;				///< When a joystick is detected, this tells us the number of axes on the pad.
		I32 numButtons;				///< When a joystick is detected, this tells us the number of buttons on the pad.
	};

	// /////////////////////////////////////////////////////////////////
	// @struct JoystickStateChangeEvent
	// @author PJ O Halloran
	//
	// Event produced when a joysticks dpad/thumb pad is moved.
	//
	// /////////////////////////////////////////////////////////////////
	struct JoystickMoveEvent : public BaseEvent
	{
		I32 joyId;					///< ID of the joystick.
		I32 axisId;					///< ID of the axis.
		F32 pos;					///< New axis position.
	};

	// /////////////////////////////////////////////////////////////////
	// @struct JoystickButtonEvent
	// @author PJ O Halloran
	//
	// Event produced when a joysticks button is pressed/released.
	//
	// /////////////////////////////////////////////////////////////////
	struct JoystickButtonEvent : public BaseEvent
	{
		I32 joyId;					///< ID of the joystick.
		I32 buttonId;				///< ID of the button.
		I32 buttonState;			///< Joystick buttons state (GLFW_PRESS or GLFW_RELEASE).
	};

	// /////////////////////////////////////////////////////////////////
	// @union GF_Event
	// @author PJ O Halloran
	//
	// The main gameframework event union to pass about the system.
	//
	// /////////////////////////////////////////////////////////////////
	union GF_Event
	{
		I32 id;											///< ID of the type of event that occurred and that is now inside the union.
		MouseButtonEvent mouseButton;
		MouseWheelEvent mouseWheel;
		MouseMoveEvent mouseMove;
		QuitEvent quit;
		WindowRefreshEvent refresh;
		WindowResizeEvent resize;
		ActiveEvent active;
		KeyEvent key;
		KeyCharacterEvent keyChar;
		JoystickStateChangeEvent joyStateChange;
		JoystickMoveEvent joyMove;
		JoystickButtonEvent joyButton;
	};

	// Our event queue type definition.
	typedef std::deque<GF_Event> GfEventQueue;

	// /////////////////////////////////////////////////////////////////
	// @class GfEventFactory
	// @author PJ O Halloran
	//
	// Common class for filling out OS input and window event structures.
	//
	// /////////////////////////////////////////////////////////////////
	class GfEventFactory
	{
	private:

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		GfEventFactory() { };

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		~GfEventFactory() { };

		// /////////////////////////////////////////////////////////////////
		// Fill out a QuitEvent structure.
		// 
		// /////////////////////////////////////////////////////////////////
		void CreateCloseWindowEvent(QuitEvent &eventObj) const;

		// /////////////////////////////////////////////////////////////////
		// Fill out a WindowResizeEvent structure.
		// 
		// /////////////////////////////////////////////////////////////////
		void CreateResizeWindowEvent(WindowResizeEvent &eventObj, const I32 width, const I32 height) const;

		// /////////////////////////////////////////////////////////////////
		// Fill out a WindowRefreshEvent structure.
		// 
		// /////////////////////////////////////////////////////////////////
		void CreateRefreshWindowEvent(WindowRefreshEvent &eventObj) const;

		// /////////////////////////////////////////////////////////////////
		// Fill out a KeyEvent structure.
		// 
		// /////////////////////////////////////////////////////////////////
		void CreateKeyEvent(KeyEvent &eventObj, const I32 keyId, const I32 action) const;

		// /////////////////////////////////////////////////////////////////
		// Fill out a KeyCharacterEvent structure.
		// 
		// /////////////////////////////////////////////////////////////////
		void CreateKeyCharacterEvent(KeyCharacterEvent &eventObj, const I32 characterId, const I32 action) const;

		// /////////////////////////////////////////////////////////////////
		// Fill out a MouseButtonEvent structure.
		// 
		// /////////////////////////////////////////////////////////////////
		void CreateMouseButtonEvent(MouseButtonEvent &eventObj, const I32 buttonId, const I32 action) const;

		// /////////////////////////////////////////////////////////////////
		// Fill out a MouseMoveEvent structure.
		// 
		// /////////////////////////////////////////////////////////////////
		void CreateMouseMoveEvent(MouseMoveEvent &eventObj, const I32 x, const I32 y, const I32 prevX, const I32 prevY) const;

#ifdef USE_NEW_GLFW
		// /////////////////////////////////////////////////////////////////
		// Fill out a MouseWheelEvent structure.
		// 
		// /////////////////////////////////////////////////////////////////
		void CreateMouseWheelEvent(MouseWheelEvent &eventObj, const I32 x, const I32 y) const;
#else
		// /////////////////////////////////////////////////////////////////
		// Fill out a MouseWheelEvent structure.
		// 
		// /////////////////////////////////////////////////////////////////
		void CreateMouseWheelEvent(MouseWheelEvent &eventObj, const I32 position) const;
#endif

		// /////////////////////////////////////////////////////////////////
		// Fill out a ActiveWindowEvent structure.
		// 
		// /////////////////////////////////////////////////////////////////
		void CreateActiveWindowEvent(ActiveEvent &eventObj, const I32 active) const;

		// /////////////////////////////////////////////////////////////////
		// Fill out a JoystickStatusChangeEvent structure.
		// 
		// /////////////////////////////////////////////////////////////////
		void CreateJoystickStatusChangeEvent(JoystickStateChangeEvent &eventObj, const I32 joyId, const I32 status, const I32 numAxes, const I32 numButtons) const;

		// /////////////////////////////////////////////////////////////////
		// Fill out a JoystickMoveEvent structure.
		// 
		// /////////////////////////////////////////////////////////////////
		void CreateJoystickMoveEvent(JoystickMoveEvent &eventObj, const I32 joyId, const I32 axisId, const F32 position) const;

		// /////////////////////////////////////////////////////////////////
		// Fill out a JoystickButtonEvent structure.
		// 
		// /////////////////////////////////////////////////////////////////
		void CreateJoystickButtonEvent(JoystickButtonEvent &eventObj, const I32 joyId, const I32 buttonId, const I32 buttonState) const;

	};

	// /////////////////////////////////////////////////////////////////
	// @class JoystickInfo
	// @author PJ O Halloran
	// 
	// Details on a joystick and an I32erface for polling the joystick
	// for input events.
	//
	// /////////////////////////////////////////////////////////////////
	class JoystickInfo
	{
	private:

		int m_id;						///< Joystick ID.
		I32 m_numAxes;					///< Number of axes.
		I32 m_numButtons;				///< Number of buttons.
		I32 m_playerId;					///< The ID of the player using the joystick (-1 if no player using it).
		F32 *m_axisPositionsArr;		///< Array of axis movement states.
		unsigned char *m_buttonsArr;	///< Array of button states.
		bool m_wasPluggedIn;			///< Record of the joystick detected status on the previous call to PollJoystick.

		// /////////////////////////////////////////////////////////////////
		// Initialize the axis and button internal arrays with the size
		// m_numAxes and m_numButtons (if they are greater than 0).
		//
		// /////////////////////////////////////////////////////////////////
		void CreateStateArrays();

		// /////////////////////////////////////////////////////////////////
		// Free memory allocated for state arrays.
		//
		// /////////////////////////////////////////////////////////////////
		void DestroyStateArrays();

		// /////////////////////////////////////////////////////////////////
		// Copy the contents of the arrays passed in into the internal state
		// arrays.  Any previous arrays and the state they contained will be
		// lost on calling this method.
		//
		// /////////////////////////////////////////////////////////////////
		void CopyStateArrays(const F32 * const axisArr, const I32 axisArrSize, const unsigned char * const buttonsArr, const I32 buttonArrSize);

		// /////////////////////////////////////////////////////////////////
		// Check if the difference in the axis position is large enough in
		// order to deem that an axis movement event should be broadcast.
		//
		// /////////////////////////////////////////////////////////////////
		inline bool DidAxisChange(const F32 lastPos, const F32 currPos) const
		{
			return (std::abs(lastPos - currPos) > GF_AXIS_MIN_CHANGE_DELTA);
		}

	public:

		// Value for the internal player ID member when there is no player using the joystick.
		static const I32 NO_PLAYER_ATTACHED = -1;

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		JoystickInfo(const I32 id) : m_id(id), m_numAxes(0), m_numButtons(0), m_playerId(NO_PLAYER_ATTACHED), m_axisPositionsArr(NULL),\
			m_buttonsArr(NULL), m_wasPluggedIn(false) { };

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// Use this constructor if you use the GLFW lib yourself to query
		// the entire joysticks status.
		//
		// /////////////////////////////////////////////////////////////////
		JoystickInfo(const I32 id, const I32 numAxes, const I32 numButtons, const I32 pId = NO_PLAYER_ATTACHED)\
			: m_id(id), m_numAxes(numAxes), m_numButtons(numButtons), m_playerId(pId), m_axisPositionsArr(NULL),\
				m_buttonsArr(NULL), m_wasPluggedIn(false) { };

		// /////////////////////////////////////////////////////////////////
		// Copy constructor.
		//
		// /////////////////////////////////////////////////////////////////
		JoystickInfo(const JoystickInfo &obj);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		~JoystickInfo();

		// /////////////////////////////////////////////////////////////////
		// Set the player ID.
		//
		// /////////////////////////////////////////////////////////////////
		void SetPlayerId(const I32 id) { m_playerId = id; };

		// /////////////////////////////////////////////////////////////////
		// Get the player ID.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetPlayerId() const { return (m_playerId); };

		// /////////////////////////////////////////////////////////////////
		// Is a player currently using the joystick?
		//
		// /////////////////////////////////////////////////////////////////
		bool IsPlayerAttached() const { return (m_playerId != NO_PLAYER_ATTACHED); };

		// /////////////////////////////////////////////////////////////////
		// Is the joystick currently plugged in.
		//
		// /////////////////////////////////////////////////////////////////
		bool IsDetected() const;

		// /////////////////////////////////////////////////////////////////
		// Get the joystick ID (read only member).
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetJoystickId() const { return (m_id); };

		// /////////////////////////////////////////////////////////////////
		// Get the number of buttons (read only member).
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetNumberButtons() const { return (m_numButtons); };

		// /////////////////////////////////////////////////////////////////
		// Get the number of axes (read only member).
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetNumberAxes() const { return (m_numAxes); };

		// /////////////////////////////////////////////////////////////////
		// Less than operator.
		//
		// /////////////////////////////////////////////////////////////////
		bool operator<(const JoystickInfo &rhs) { return (m_id < rhs.m_id); };

		// /////////////////////////////////////////////////////////////////
		// Greater than operator.
		//
		// /////////////////////////////////////////////////////////////////
		bool operator>(const JoystickInfo &rhs) { return (m_id > rhs.m_id); };

		// /////////////////////////////////////////////////////////////////
		// Equivalent operator.
		//
		// /////////////////////////////////////////////////////////////////
		bool operator==(const JoystickInfo &rhs) { return (m_id == rhs.m_id); };

		// /////////////////////////////////////////////////////////////////
		// Assignment operator.
		//
		// /////////////////////////////////////////////////////////////////
		void operator=(const JoystickInfo &rhs);

		// /////////////////////////////////////////////////////////////////
		// Returns a human readable string with the joysticks details for
		// debugging purposes.
		//
		// @param verbose Should we print out button and axis state too?
		//
		// /////////////////////////////////////////////////////////////////
		std::string ToString(const bool verbose = false) const;

		// /////////////////////////////////////////////////////////////////
		// Polls the current joystick for state changes or input events.
		//
		// This method should be called once per loop for every connected
		// joystick that the application is interested in getting input for.
		//
		// Please note that if the eventFactoryPtr is NULL then this method
		// will not be able to do anything.  This is to keep 1 global
		// event factory object on the stack and not have to create and destroy
		// C++ object every time this function is called which would typically be
		// once per main loop iteration.
		//
		// @param eventQueue The queue of events that we will append joystick
		//						state change events to.
		// @param eventFactoryPtr The OS input factory object.
		// @param pollAxis If true then we will poll the current axis position
		//					and send an event every time.  If false we will check
		//					the axis position and send an event only if the axis
		//					moved significantly.
		//
		// /////////////////////////////////////////////////////////////////
		void PollJoystickState(GfEventQueue &eventQueue, GfEventFactory *eventFactoryPtr, const bool pollAxis = true);

	};

	// Container type of joystick list, deque is used as we need random access
	// yet at the same time we will be keeping the list sorted and efficient
	// element inserts are also a plus.  deque is a good middle ground for this
	// over using either vector (random access) or list (fast inserts).
	typedef boost::shared_ptr<JoystickInfo> JoystickInfoPtr;
	typedef std::deque<JoystickInfoPtr > GfJoyCont;

}

#endif
