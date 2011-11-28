//========================================================================
// interfaces.h -  Various top level interfaces.
//
//
// Part of the GameCode3 Application
//
// GameCode3 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 3rd Edition" by Mike McShaffry, published by
// Charles River Media. ISBN-10: 1-58450-680-6   ISBN-13: 978-1-58450-680-5
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the author a favor and buy a new copy of the book - there are 
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: 
//    http://www.amazon.com/gp/product/1584506806?ie=UTF8&tag=gamecodecompl-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=1584506806
//
// There's a companion web site at http://www.mcshaffry.com/GameCode/
// 
// The source code is managed and maI32ained through Google Code:
// http://gamecode3.googlecode.com/svn/trunk/
//
// (c) Copyright 2009 Michael L. McShaffry
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License v2
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================

#ifndef __I_INPUT_INTERFACES_H
#define __I_INPUT_INTERFACES_H

// /////////////////////////////////////////////////////////////////
// @file IInputInterfaces.h
// @author Michael L. McShaffry (edited by PJ O Halloran)
// @date 17/07/2010
//
// File contains the header for the user input interfaces.
// 
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry, author of Game
// Coding Complete and games programmer.
// 
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// I have added/modified the following functionality:
// - I added the interfaces under the GameHalloran namespace.
// - Implemented a joystick and gamepad interface myself.
//
// /////////////////////////////////////////////////////////////////

// External Headers


// Project Headers
#include "Point.h"


namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class IKeyboardHandler
	// @author Mike McShaffry
	//
	// Interface for keyboard events.
	//
	// /////////////////////////////////////////////////////////////////
	class IKeyboardHandler
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~IKeyboardHandler() { };

		// /////////////////////////////////////////////////////////////////
		// Called upon a key down/press event.
		//
		// @param id The ID of the key pressed.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnKeyDown(const I32 id) = 0;

		// /////////////////////////////////////////////////////////////////
		// Called upon a key up/release event.
		//
		// @param id The ID of the key released.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnKeyUp(const I32 id) = 0;
	};

	// /////////////////////////////////////////////////////////////////
	// @class IMouseHandler
	// @author Mike McShaffry
	//
	// Interface for mouse events.
	//
	// /////////////////////////////////////////////////////////////////
	class IMouseHandler
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~IMouseHandler(){ };

		// /////////////////////////////////////////////////////////////////
		// Called upon a mouse move event.
		//
		// @param currPos The current position of the mouse cursor.
		// @param relativePos The relative position of the mouse cursor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnMouseMove(const Point &currPos, const Point &relativePos) = 0;

#ifdef USE_NEW_GLFW
		// /////////////////////////////////////////////////////////////////
		// Called upon a mouse wheel move event.
		//
		// @param x The new X position of the mouse wheel.
		// @param y The new Y position of the mouse wheel.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnMouseWheelMove(const I32 x, const I32 y) = 0;        
#else
		// /////////////////////////////////////////////////////////////////
		// Called upon a mouse wheel move event.
		//
		// @param position The new position of the mouse wheel.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnMouseWheelMove(const I32 position) = 0;
#endif

		// /////////////////////////////////////////////////////////////////
		// Called upon a mouse button press.
		//
		// @param The ID of the mouse button that was pressed.
		// @param currPos The position of the mouse cursor when the button
		//					was pressed.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnMouseButtonDown(const I32 buttonId, const Point &currPos) = 0;

		// /////////////////////////////////////////////////////////////////
		// Called upon a mouse left button release.
		//
		// @param The ID of the mouse button that was released.
		// @param currPos The position of the mouse cursor when the button
		//					was released.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnMouseButtonUp(const I32 buttonId, const Point &currPos) = 0;
	};

	// /////////////////////////////////////////////////////////////////
	// @class IJoystickHandler
	// @author PJ O Halloran
	//
	// Interface for joystick events.
	//
	// TODO: A joystick can have many types of sliders/switches etc. For
	// example the joystick used for Microsoft Flight Control. Implement
	// a full interface here IF my game needs to support these kinds of
	// functions!
	//
	// /////////////////////////////////////////////////////////////////
	class IJoystickHandler
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~IJoystickHandler(){ };

		// /////////////////////////////////////////////////////////////////
		// Called when a joystick button is pressed.
		//
		// Not all joysticks have analog/pressure sensitive buttons so we
		// supply a default parameter here to indicate easily that a button
		// is fully pressed for most controllers.
		//
		// @param buttonId The ID of the button.
		// @param pressure A value between 0.0f and 1.0f for how hard a button
		//					was pressed. 0.0f is the minimum pressure and 1.0f
		//					is the maximum.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnJoyButtonDown(const I32 buttonId, const F32 pressure = 1.0f) = 0;

		// /////////////////////////////////////////////////////////////////
		// Called when a joystick button is released.
		//
		// @param buttonId The ID of the button.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnJoyButtonUp(const I32 buttonId) = 0;

		// /////////////////////////////////////////////////////////////////
		// Called when the joystick axis is moved.
		//
		// @param x The new x position of the joystick.
		// @param y The new y position of the joystick.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnJoystickMove(const F32 x, const F32 y) = 0;
	};

	// /////////////////////////////////////////////////////////////////
	// @enum DPadDirection
	//
	// Enumeration of the possible directions events for a gamepads
	// directional pad.
	//
	// /////////////////////////////////////////////////////////////////
	enum DPadDirection
	{
		LEFT = 0,
		RIGHT,
		UP,
		DOWN,
		UP_LEFT,
		DOWN_LEFT,
		UP_RIGHT,
		DOWN_RIGHT,
		NUMBER_DIRECTIONS
	};

	// /////////////////////////////////////////////////////////////////
	// @class IGamepadHandler
	// @author PJ O Halloran.
	//
	// Interface for gamepad events.
	//
	// /////////////////////////////////////////////////////////////////
	class IGamepadHandler
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~IGamepadHandler(){ };

		// /////////////////////////////////////////////////////////////////
		// Called when a trigger button is pressed.
		//
		// @param triggerId The ID of the trigger.
		// @param pressure A value between 0.0f and 1.0f for how hard a trigger
		//					was pressed. 0.0f is the minimum pressure and 1.0f
		//					is the maximum.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnGpTrigger(const I32 triggerId, const F32 pressure) = 0;

		// /////////////////////////////////////////////////////////////////
		// Called when a button is pressed.
		//
		// Not all gamepads have analog/pressure sensitive buttons so we
		// supply a default parameter here to indicate easily that a button
		// is fully pressed for most controllers.
		//
		// @param buttonId The ID of the button.
		// @param pressure A value between 0.0f and 1.0f for how hard a button
		//					was pressed. 0.0f is the minimum pressure and 1.0f
		//					is the maximum.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnGpButtonDown(const I32 buttonId, const F32 pressure = 1.0f) = 0;

		// /////////////////////////////////////////////////////////////////
		// Called when a button is released.
		//
		// @param buttonId The ID of the button.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnGpButtonUp(const I32 buttonId) = 0;

		// /////////////////////////////////////////////////////////////////
		// Called when a dpad button is pressed.
		//
		// @param direction The direction indicated on the dpad.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnGpDirectionalPad(const DPadDirection direction) = 0;

		// /////////////////////////////////////////////////////////////////
		// Called when an analog axis is moved.
		//
		// @param axisId The ID of the axis.
		// @param position The new position of the axis (value in the range
		//					of -1.0f to 1.0f).
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnGpThumbstick(const I32 axisId, const F32 position) = 0;
	};

}

#endif
