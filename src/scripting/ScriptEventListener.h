#pragma once
#ifndef __SCRIPT_EVENT_LISTENER_H
#define __SCRIPT_EVENT_LISTENER_H

//========================================================================
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
// @file ScriptEventListener.h
// @author Mike McShaffry et al.
// @date 13/07/2010
//
// File contains the header for the ScriptEventListener class.
//
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry et al., author(s) of Game
// Coding Complete and games programmer(s).
//
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// I have added/modified the following functionality:
// - I added the class under the GameHalloran namespace.
// -
//
// /////////////////////////////////////////////////////////////////

#include "Actors.h"
#include "LuaStateManager.h"
#include "EventManager.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // @class ScriptEventListener
    // @author Mike McShaffry et al.
    //
    // The ScriptEventListener holds a script callback function that responds
    // to a particular event.
    //
    // /////////////////////////////////////////////////////////////////
    class ScriptEventListener : public IEventListener {
    protected:

        // /////////////////////////////////////////////////////////////////
        // This function is virtual as sub-classes may pass additional
        // parameters.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VCallLuaFunction(LuaPlus::LuaObject & eventData);

        LuaPlus::LuaObject m_HandlerFunction;                       ///<

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param explicitHandlerFunction ?
        //
        // /////////////////////////////////////////////////////////////////
        ScriptEventListener(LuaPlus::LuaObject explicitHandlerFunction);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~ScriptEventListener() {
        }

        // /////////////////////////////////////////////////////////////////
        // Get the name of the event.
        //
        // /////////////////////////////////////////////////////////////////
        virtual char const *VGetName(void) {
            return "Script Listener";
        }

        // /////////////////////////////////////////////////////////////////
        // The event handler function.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VHandleEvent(IEventData const & event);

        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        const LuaPlus::LuaObject & GetHandlerFunction(void) const {
            return m_HandlerFunction;
        }
    };

    // /////////////////////////////////////////////////////////////////
    // @class ScriptActorEventListener
    // @author Mike McShaffry et al.
    //
    // The ScriptActorEventListener holds a script callback function tied
    // to a specific actor, and when called, passes in the actor's script
    // data.
    //
    // /////////////////////////////////////////////////////////////////
    class ScriptActorEventListener : public ScriptEventListener {
    private:

        // /////////////////////////////////////////////////////////////////
        // This will pass the event data object as well as
        // look up our actor's specific script data to pass.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VCallLuaFunction(LuaPlus::LuaObject & eventData);

        const ActorId m_SrcActorID;                             ///< Our source actor.

    public:

        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        ScriptActorEventListener(LuaPlus::LuaObject explicitHandlerFunction, const ActorId actorID);

        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~ScriptActorEventListener() {
        }

        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        virtual char const *VGetName(void) {
            return "Script Actor Listener";
        }

        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        ActorId GetActorID(void) const {
            return m_SrcActorID;
        }
    };

}

#endif  //__SCRIPT_EVENT_LISTENER_H
