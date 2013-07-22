#pragma once
#ifndef __I_SCREEN_ELEMENT_H
#define __I_SCREEN_ELEMENT_H

//========================================================================
// Interfaces.h : Defines interface classes defined throughout the book
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
// The source code is managed and maintained through Google Code:
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

// /////////////////////////////////////////////////////////////////
// @file IScreenElement.h
// @author Michael L. McShaffry (edited by PJ O Halloran)
// @date 09/07/2010
//
// File contains the header for the game view layer interface.
//
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry, author of Game
// Coding Complete and games programmer.
//
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// I have added/modified the following functionality:
// - Added interface under the GameHalloran namespace.
// - I commented out virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) = 0;
//      as its WIN32 specific, replaced with GLFW equivalent.
//
// /////////////////////////////////////////////////////////////////

#include <list>
#include <boost/shared_ptr.hpp>

#include "OsInputEvents.h"

namespace GameHalloran {

    // ID for the screen element.
    typedef U32 ScreenElementId;

    static const ScreenElementId SCREEN_ELEMENT_ID_NOT_ASSIGNED = 0;

    // /////////////////////////////////////////////////////////////////
    // @class IScreenElement
    // @author Mike McShaffry
    //
    // The base interface for objects that appear on screen.  For example
    // GUI elements such as text boxes, buttons, etc.
    //
    // /////////////////////////////////////////////////////////////////
    class IScreenElement {
    public:

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~IScreenElement() { };

        // /////////////////////////////////////////////////////////////////
        // Get the ID of the screen element.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ScreenElementId VGetId() const = 0;

        // /////////////////////////////////////////////////////////////////
        // Set the ID of the screen element.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSetId(const ScreenElementId id) = 0;

        // /////////////////////////////////////////////////////////////////
        // Called when the application is restored after a minimize or other
        // such event.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnRestore() = 0;

        // /////////////////////////////////////////////////////////////////
        // Called when the application display or device is lost after a
        // display resolution or other such system event.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VOnLostDevice() = 0;

        // /////////////////////////////////////////////////////////////////
        // Render the screen element.
        //
        // @param time The total time in seconds so far.
        // @param elapsedTime The time elapsed since the last render call.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnRender(const F64 time, const F32 elapsedTime) = 0;

        // /////////////////////////////////////////////////////////////////
        // Update the screen element.
        //
        // @param elapsedTime The number of seconds since the last update.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VOnUpdate(const F32 elapsedTime) = 0;

        // /////////////////////////////////////////////////////////////////
        // Get the Z order of the element.
        //
        // /////////////////////////////////////////////////////////////////
        virtual I32 VGetZOrder() const = 0;

        // /////////////////////////////////////////////////////////////////
        // Set the Z order.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSetZOrder(const I32 zOrder) = 0;

        // /////////////////////////////////////////////////////////////////
        // Is the element currently visible?
        //
        // @return bool True if it is visible and false if it is not.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VIsVisible() const = 0;

        // /////////////////////////////////////////////////////////////////
        // Set if the element is currently visible.
        //
        // @param visible True if visible, false if not.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSetVisible(const bool visible) = 0;

        // /////////////////////////////////////////////////////////////////
        // Called once per game loop so the element can check if there was
        // any input from the operating system or from the user.
        //
        // @param eventObj Event structure.
        // @param elapsedTime The number of seconds since the last update.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnEvent(GF_Event &eventObj, const F32 elapsedTime) = 0;

        // /////////////////////////////////////////////////////////////////
        // Check if this screen element has a higher Z order than that of
        // the other screen element.
        //
        // @param other The screen element to check against.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool const operator <(const IScreenElement &other) {
            return (VGetZOrder() < other.VGetZOrder());
        };
    };

    typedef std::list<boost::shared_ptr<IScreenElement> > ScreenElementList;

}

#endif
