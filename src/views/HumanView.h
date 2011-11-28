//========================================================================
// GameCode.h : Defines the entry point for the application.
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
// The source code is managed and maintained through Google Code: http://gamecode3.googlecode.com/svn/trunk/
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

#ifndef __HUMAN_VIEW_H
#define __HUMAN_VIEW_H

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

// /////////////////////////////////////////////////////////////////
// @file HumanView.h
// @author Michael L. McShaffry (edited by PJ O Halloran)
// @date 17/07/2010
//
// File contains the header for the game view layer class that humans
// see and interact with.
// 
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry, author of Game
// Coding Complete and games programmer.
// 
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// I have added/modified the following functionality:
// - I added the class under the GameHalloran namespace.
// - I put the class in its own header and implementation file.
// - Made class members private.
// - I removed the Console class for now as it requires more rendering
//		capability than I can quickly implement right now.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

// Project Headers
#include "IGameView.h"
#include "IScreenElement.h"
#include "WindowManager.h"
#include "CProcess.h"

#include "IInputInterfaces.h"

#include "GameLog.h"
#include "GameOptions.h"
#include "GameException.h"
#include "IGameTimer.h"
#include "OsInputEvents.h"

#include "Vector.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class HumanView
	// @author Michael L. McShaffry (edited by PJ O Halloran).
	//
	// This is a class is the base interface that all game view layer
	// classes must implement.
	//
	// /////////////////////////////////////////////////////////////////
	class HumanView : public IGameView
	{
	private:

		GameViewId m_viewId;										///< The ID of the view.
		boost::optional<ActorId> m_actorId;							///< The ID of the actor attached to the view.
		boost::shared_ptr<CProcessManager> m_processManagerPtr;		///< strictly for things like button animations, etc.
		boost::shared_ptr<IGameTimer> m_timer;						///< View timer.
		F64 m_lastDraw;											///< Last time the game rendered
		bool m_runFullSpeed;										///< set to true if you want to run full speed
		ScreenElementId m_currElemId;								///< The current elment ID.

	protected:

		ScreenElementList m_ScreenElements;							///< List of elements to be displayed for the user to see and interact with.
		IMouseHandler *m_mouseHandler;								///< Interface to the mouse.
		IKeyboardHandler *m_keyboardHandler;						///< Interface to the keyboard.
		boost::shared_ptr<GameOptions> m_optionsPtr;				///< Pointer to the global game options interface.
		boost::shared_ptr<GameLog> m_loggerPtr;						///< Pointer to the global logger.
		boost::shared_ptr<WindowManager> m_screenManPtr;			///< Global Display manager.

		// /////////////////////////////////////////////////////////////////
		// Get the Actor ID.
		//
		// /////////////////////////////////////////////////////////////////
		boost::optional<ActorId> GetActorId() const;

		// /////////////////////////////////////////////////////////////////
		// Get the ProcessManager.
		//
		// /////////////////////////////////////////////////////////////////
		boost::shared_ptr<CProcessManager> GetProcessManager();

		// /////////////////////////////////////////////////////////////////
		// Should we render at full speed?
		//
		// /////////////////////////////////////////////////////////////////
		bool IsRunFullSpeed() const;

		// /////////////////////////////////////////////////////////////////
		// Get the last render timestamp.
		//
		// /////////////////////////////////////////////////////////////////
		F64 GetLastDrawTime() const;

		// /////////////////////////////////////////////////////////////////
		// Get the next screen element ID to assign.
		//
		// /////////////////////////////////////////////////////////////////
		ScreenElementId GetNextElementId()
		{
			return (++m_currElemId);
		};

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param optionsPtr Pointer to the global game options.
		// @param loggerPtr Pointer to the global logger.
		// @param screenManPtr Pointer to the global display manager.
		//
		// @throw GameException& One of the input parameters is NULL.
		// @throw GameException& We fail to allocate memory for any of the
		//							classes members.
		// @throw GameException& Fail to initialize the audio subsystem.
		//
		// /////////////////////////////////////////////////////////////////
		HumanView(boost::shared_ptr<GameOptions> optionsPtr, boost::shared_ptr<GameLog> loggerPtr, boost::shared_ptr<WindowManager> screenManPtr) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~HumanView();

		// /////////////////////////////////////////////////////////////////
		// Called when the view is restored after an ALT-TAB or other event.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnRestore();
		
		// /////////////////////////////////////////////////////////////////
		// Render the game view.
		//
		// @param time The total running time so far.
		// @param elapsedTime The time elapsed since the last render.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnRender(const F64 time, const F32 elapsedTime);
		
		// /////////////////////////////////////////////////////////////////
		// Called when the view lost after a screen resolution change, 
		// ALT-TAB or other similar event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnLostDevice();
		
		// /////////////////////////////////////////////////////////////////
		// Get the type of view.
		//
		// /////////////////////////////////////////////////////////////////
		virtual GameViewType VGetType() const;
		
		// /////////////////////////////////////////////////////////////////
		// Get the ID of the view.
		//
		// /////////////////////////////////////////////////////////////////
		virtual GameViewId VGetId() const;
		
		// /////////////////////////////////////////////////////////////////
		// Attach an Actor to the view.
		//
		// @param vid The ID of the view.
		// @param aid The ID of the actor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnAttach(const GameViewId vid, boost::optional<ActorId> aid);

		// /////////////////////////////////////////////////////////////////
		// The message queue event callback.
		//
		// @param eventObj The message queue.
		// @param elapsedTime The number of seconds since the last event poll.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnEvent(GF_Event &eventObj, const F32 elapsedTime);
		
		// /////////////////////////////////////////////////////////////////
		// Called once per the main loop to update the state of the view.
		//
		// @param elapsedTime The number of seconds since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnUpdate(const F32 elapsedTime);

		// /////////////////////////////////////////////////////////////////
		// Add a screen element to be rendered by the view.
		//
		// The screen elements will be rendered in their Z order, not in the
		// order they were added to the view!
		//
		// @param pScreen Pointer to the screen element to add.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VPushElement(boost::shared_ptr<IScreenElement> pScreen);

		// /////////////////////////////////////////////////////////////////
		// Remove a screen element from the view.
		//
		// @param pScreen Pointer to the screen element to remove.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VPopElement(boost::shared_ptr<IScreenElement> pElement);

		// /////////////////////////////////////////////////////////////////
		// Initialize the audio subsystem.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool InitAudio();

		// /////////////////////////////////////////////////////////////////
		// Camera adjustments.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetCameraOffset(const Vector4 &camOffset) { };
	};

}

#ifdef WIN32
#	pragma warning( pop )
#endif

#endif
