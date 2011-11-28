//========================================================================
// EventManager.h : implements a multi-listener multi-sender event system
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
// @file EventManager.h
// @author Mike McShaffry
// @date 13/07/2010
//
// File contains the header for the EventManager class and the event
// data and listener interfaces and base classes.
//
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry, author of Game
// Coding Complete and games programmer.
// 
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// I have added/modified the following functionality:
// - I added the classes under the GameHalloran namespace.
// - I modifed the IEventManager constructor to throw a GameException
//		if it fails to initialize.
//
// /////////////////////////////////////////////////////////////////

#ifndef __EVENT_MANAGER_H
#define __EVENT_MANAGER_H

// External Headers
#include <boost/shared_ptr.hpp>
#include <sstream>

// Project Headers
#include "GameBase.h"
#include "LuaStateManager.h"
#include "HashedString.h"
#include "GameException.h"
//#include "CriticalSection.h"

// Disable exception throw specification warning VS complains about.
//  It doesn't like "throw (somexception)", it likes either "throw()"
//   for function throws nothing or "" for function may throw something.
#pragma warning( disable:4290 )

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// EventType Description
	//
	// Data necessary to register and event classification
	//
	// This is used as auxillary data during event handler
	// registration, but also provides the translation from internal
	// numeric event type designation to a human readible identifier
	// useful for debugging and logging.
	//
	// Note: During event handler registration, it is wrong code to
	// register an event type that has different text associated with
	// the numeric identifier.
	//
	// The event manager generates a 32-biy numeric identity for this
	// event type upon registration and that is what is used by the
	// system from that point out.
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	// Use this event type to register a listener that receives ( but
	// never eats ) all events that pass through the system. It is
	// special in that if the type ident str that is passed in
	// matches this, then the ident value is set to zero.
	// /////////////////////////////////////////////////////////////////
	char const * const kpWildcardEventType = "*";

	// /////////////////////////////////////////////////////////////////
	// Event types are hashed string IDs.  We keep a mapping of
	// both event name string -> ID and vice-versa.
	// /////////////////////////////////////////////////////////////////
	typedef HashedString EventType;

	// /////////////////////////////////////////////////////////////////
	// @class ILuaable
	// @author Mike McShaffry
	//
	// Base interface for LUA capable event data classes.
	//
	// /////////////////////////////////////////////////////////////////
	class ILuaable
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Get the event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const = 0;

		// /////////////////////////////////////////////////////////////////
		// Serializes the event data into the LuaObject.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void) = 0;

		// /////////////////////////////////////////////////////////////////
		//Called when testing whether or not the event has been serialized for script
		//(this allows us to only serialize ONCE per event trigger).
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VHasLuaEventData(void) const = 0;
	};

	// Shared Pointer to an EventData object.
	class IEventData;
	typedef boost::shared_ptr<IEventData> IEventDataPtr;

	// /////////////////////////////////////////////////////////////////
	// @class IEventData
	// @author Mike McShaffry
	//
	// Base interface for event data.
	//
	// /////////////////////////////////////////////////////////////////
	// IEventData Description
	//
	// Base type for event object hierarchy, may be used itself for
	// simplest event notifications ... aka:those that do not carry
	// additional payload data. If any event needs to propogate with
	// payload data it must be defined separately.
	//
	// The userData parameter is used to propogate access from
	// event-firing code to event-listening code. By using a smart
	// pointer object here you can safely create-and-forget the
	// object at event trigger time, and if there is nothing to
	// associate with the event then simply provde the parameter with
	// EventData(NULL) and away you go ... all nice and safe and
	// zero-memory bleed.
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	class IEventData : public ILuaable
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const = 0;

		// /////////////////////////////////////////////////////////////////
		// Get the event timestamp.
		//
		// /////////////////////////////////////////////////////////////////
		virtual F32 VGetTimeStamp() const = 0;

		// /////////////////////////////////////////////////////////////////
		// Serialize the event data into a c++ stream (human readable version).
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const = 0;

		// /////////////////////////////////////////////////////////////////
		// GCC3 note: added for the Multicore chapter
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const = 0;
	};


	// /////////////////////////////////////////////////////////////////
	// @class BaseEventData
	// @author Mike McShaffry
	//
	// Base event data class.  All event data types MUST inherit from
	// this.
	//
	// /////////////////////////////////////////////////////////////////
	class BaseEventData : public IEventData
	{
	protected:
		const F32 m_TimeStamp;			///< The time th event occcurred.
		bool m_bHasLuaEventData;			///< We will build that *only if necessary* (i.e., there is a script-side listener).

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		explicit BaseEventData( const F32 timeStamp = 0.0f ) 
			: m_TimeStamp(timeStamp), m_bHasLuaEventData(false)	//By default, the event does not have any Lua event data.
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~BaseEventData() { }

		// /////////////////////////////////////////////////////////////////
		// Called when sending the event data over to the script-side listener.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const = 0;

		// /////////////////////////////////////////////////////////////////
		// Serializes the event data into the LuaObject.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void) = 0;

		// /////////////////////////////////////////////////////////////////
		// Called when testing whether or not the event has been serialized
		// for script (this allows us to only serialize ONCE per event trigger).
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VHasLuaEventData( void ) const
		{
			return m_bHasLuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Returns the type of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const = 0;

		// /////////////////////////////////////////////////////////////////
		// Get the time stamp when the event occurred.
		//
		// /////////////////////////////////////////////////////////////////
		F32 VGetTimeStamp( void ) const
		{
			return m_TimeStamp;
		}

		// /////////////////////////////////////////////////////////////////
		// Serializing for network out.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
		};
	};

	// /////////////////////////////////////////////////////////////////
	// @class EmptyEventData
	// @author Mike McShaffry
	//
	// Helper for events that require no data (still need to derive for
	// the EventType, tho).
	//
	// /////////////////////////////////////////////////////////////////
	class EmptyEventData : public BaseEventData
	{
	private:
		LuaPlus::LuaObject m_LuaEventData;						///< 

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EmptyEventData( const F32 timeStamp = 0.0f )
			: BaseEventData( timeStamp )
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EmptyEventData()	{ }

		// /////////////////////////////////////////////////////////////////
		// Called when sending the event data over to the script-side listener.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Serializes the event data into the LuaObject.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void);
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData
	// @author Mike McShaffry
	//
	// A template event data class.
	//
	// Note:  This event type will not serialize for Lua listeners.
	//
	// /////////////////////////////////////////////////////////////////
	template < typename T >
	struct EvtData : public BaseEventData
	{
	private:
		// Disable default & copy constructors.
		EvtData();
		EvtData(const EvtData &);

		T m_Value;									///< The event data value.
		LuaPlus::LuaObject m_LuaEventData;			///< The LUA object.

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData<T>(T n)
		{
			m_Value = n;
		}

		// /////////////////////////////////////////////////////////////////
		// Get the value of the data.
		//
		// /////////////////////////////////////////////////////////////////
		const T GetValue() { return m_Value; }

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			//Get the global state.
            // TODO: Commented out to compile with xcode - fix!
            //LuaPlus::LuaState * pState = g_pApp->m_pLuaStateManager->GetGlobalState().Get();
			//m_LuaEventData.AssignNewTable( pState );

			m_bHasLuaEventData = true;
		}
	};

	// Define the EventData INT template.
	typedef EvtData<I32> EvtData_Int;

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_ScriptEvtData
	// @author Mike McShaffry
	//
	// This type of event data is created by script-defined events.
	//
	// The LUA scripts pass in data to the program.
	//
	// /////////////////////////////////////////////////////////////////
	struct EvtData_ScriptEvtData : public BaseEventData
	{
	private:
		const EventType	m_EventType;				///< Type of this event.
		LuaPlus::LuaObject m_LuaEventData;					///< The LUA event data object.

	public:

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return m_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param eventType The event type.
		// @param srcData The LUA object.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_ScriptEvtData(const EventType &eventType, const LuaPlus::LuaObject &srcData)
			: m_EventType(eventType), m_LuaEventData(srcData)
		{
			m_bHasLuaEventData = true;	//Our Lua event data got passed into us (in the LuaObject)!
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of this object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr (GCC_NEW EvtData_ScriptEvtData ( m_EventType, m_LuaEventData ) ); 
		}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data. (This will not do anything since the LUA
		// data was passed here from a script).
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );
			return;	//Already "built" when the event got created.
		}
	};

	// /////////////////////////////////////////////////////////////////
	// @class IEventListener
	// @author Mike McShaffry
	//
	// Base type for all event handler objects, note that this is a
	// C++ interface in the classic sense in that it defines the call
	// sequence but does not provide any functionality nor aggregate
	// any data.
	//
	// Thus, this is suitable for both single-inheritance use and MAY
	// be used safely in a mutiple-inheritance construct, neither is
	// imposed or assumed.
	//
	// /////////////////////////////////////////////////////////////////
	class IEventListener
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		explicit IEventListener()
		{}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~IEventListener()
		{}

		// /////////////////////////////////////////////////////////////////
		// Returns ascii-text name for this listener, used mostly for
		// debugging
		//
		// /////////////////////////////////////////////////////////////////
		virtual char const * VGetName(void) = 0;

#ifdef _WINDOWS
		// /////////////////////////////////////////////////////////////////
		// Return 'false' to indicate that this listener did NOT
		// consume the event, ( and it should continue to be
		// propogated )
		// 					
		// return 'true' to indicate that this listener consumed the
		// event, ( and it should NOT continue to be propgated )
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VHandleEvent( IEventData const & eventObj ) = 0
		{		
			// Note: while VHandleEvent() MUST be implemented in all
			// derivative classes, (as this function is pure-virtual
			// and thus the hook for IEventListener being an
			// interface definition) a base implementation is
			// provided here to make it easier to wire up do-nothing
			// stubs that can easily be wired to log the
			// unhandled-event (once logging is available)

			// VHandleEvent() functioning should be kept as brief as
			// possible as multiple events will need to be evaluated
			// per-frame in many cases.
			return true;
		};
#elif defined (TARGET_OS_MAC)
		virtual bool VHandleEvent( IEventData const & eventObj )
		{		
			// Note: while VHandleEvent() MUST be implemented in all
			// derivative classes, (as this function is pure-virtual
			// and thus the hook for IEventListener being an
			// interface definition) a base implementation is
			// provided here to make it easier to wire up do-nothing
			// stubs that can easily be wired to log the
			// unhandled-event (once logging is available)
            
			// VHandleEvent() functioning should be kept as brief as
			// possible as multiple events will need to be evaluated
			// per-frame in many cases.
			return true;
		};
#else
#error "Platform not supported yet."
#endif
	};

	// Some typedefs to be used in the EventManager class.
	typedef boost::shared_ptr<IEventListener> EventListenerPtr;
	// TODO: Bring this in when we get ch. multiprogramming done.
	//typedef concurrent_queue<IEventDataPtr> ThreadSafeEventQueue;

	// /////////////////////////////////////////////////////////////////
	// @class IEventManager
	// @author Mike McShaffry
	//
	// This is the object which maintains the list of registered
	// events and their listeners
	//
	// This is a many-to-many relationship, as both one listener can
	// be configured to process multiple event types and of course
	// multiple listeners can be registered to each event type.
	//
	// The interface to this contruct uses smart pointer wrapped
	// objects, the purpose being to ensure that no object that the
	// registry is referring to is destoyed before it is removed from
	// the registry AND to allow for the registry to be the only
	// place where this list is kept ... the application code does
	// not need to maintain a second list.
	//
	// Simply tearing down the registry (e.g.: destroying it) will
	// automatically clean up all pointed-to objects ( so long as
	// there are no other oustanding references, of course ).
	//
	// /////////////////////////////////////////////////////////////////
	class IEventManager
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// @enum eConstants
		//
		// /////////////////////////////////////////////////////////////////
		enum eConstants
		{
			kINFINITE = 0xffffffff
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param pName 
		// @param setAsGlobal
		//
		// @throw GameException& If we fail to initialize the IEventManager.
		//
		// /////////////////////////////////////////////////////////////////
		explicit IEventManager(char const * const pName, bool setAsGlobal) throw (GameException &);
		
		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~IEventManager();

		// /////////////////////////////////////////////////////////////////
		// Register a handler for a specific event type, implicitly
		// the event type will be added to the known event types if
		// not already known.
		//
		// The function will return false on failure for any
		// reason. The only really anticipated failure reason is if
		// the input event type is bad ( e.g.: known-ident number
		// with different signature text, or signature text is empty)
		//
		// @param inHandler Pointer to the listener to add.
		// @param inType The type of event we are adding.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VAddListener ( EventListenerPtr const & inHandler,
								   EventType const & inType ) = 0;

		// /////////////////////////////////////////////////////////////////
		// Remove a listener/type pairing from the internal tables
		//
		// @param inHandler The listener we are deleting.
		// @param inType The type of listener.
		//
		// @return bool false if the pairing was not found.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VDelListener ( EventListenerPtr const & inHandler,
								   EventType const & inType ) = 0;

		// /////////////////////////////////////////////////////////////////
		// Fire off event - synchronous - do it NOW kind of thing -
		// analogous to Win32 SendMessage() API.
		//
		// @param inEvent The event data or the actual event that has occurrred.
		//
		// @return bool true if the event was consumed, false if not. Note
		// that it is acceptable for all event listeners to act on an
		// event and not consume it, this return signature exists to
		// allow complete propogation of that shred of information
		// from the internals of this system to outside uesrs.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VTrigger ( IEventData const & inEvent ) const = 0;

		// /////////////////////////////////////////////////////////////////
		// Fire off event - asynchronous - do it WHEN the event
		// system tick() method is called, normally at a judicious
		// time during game-loop processing.
		//
		// @param The event data or the actual event that has occurrred.
		//
		// @return bool true if the message was added to the processing
		// queue, false otherwise.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VQueueEvent ( IEventDataPtr const & inEvent ) = 0;

		// /////////////////////////////////////////////////////////////////
		// Thread safe version of VQueueEvent().
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VThreadSafeQueueEvent ( IEventDataPtr const & inEvent ) = 0;

		// /////////////////////////////////////////////////////////////////
		// Find the next-available instance of the named event type
		// and remove it from the processing queue.
		//
		// This may be done up to the point that it is actively being
		// processed ...  e.g.: is safe to happen during event
		// processing itself.
		//
		// if 'allOfType' is input true, then all events of that type
		// are cleared from the input queue.
		//
		// @param inType The type of event to abort.
		// @param allOfType Abort all these events?
		//
		// @return bool true if the event was found and removed, false
		// otherwise
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VAbortEvent ( EventType const & inType,
								  bool allOfType = false ) = 0;

		// /////////////////////////////////////////////////////////////////
		// Allow for processing of any queued messages, optionally
		// specify a processing time limit so that the event
		// processing does not take too long. Note the danger of
		// using this artificial limiter is that all messages may not
		// in fact get processed.
		//
		// @param maxMillis Process events on the queue for a specified
		//						amount of time.
		//
		// @return bool true if all messages ready for processing were
		// completed, false otherwise (e.g. timeout )
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VTick ( U64 maxMillis = kINFINITE ) = 0;

		// --- information lookup functions ---
		
		// /////////////////////////////////////////////////////////////////
		// Validate an event type, this does NOT add it to the
		// internal registry, only verifies that it is legal (
		// e.g. either the ident number is not yet assigned, or it is
		// assigned to matching signature text, and the signature
		// text is not empty ).
		//
		// @param inType The type of event to validate.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VValidateType( EventType const & inType ) const = 0;

	private:

		// /////////////////////////////////////////////////////////////////
		// internal use only accessor for the static methods in the
		// helper to use to get the active global instance.
		//
		// /////////////////////////////////////////////////////////////////
		static IEventManager * Get();

		// /////////////////////////////////////////////////////////////////
		// These methods are declared friends in order to get access to the
		// Get() method. Since there is no other private entity declared
		// in this class this does not break encapsulation, but does allow
		// us to do this without requiring macros or other older-style
		// mechanims.
		//
		// /////////////////////////////////////////////////////////////////
		friend bool safeAddListener( EventListenerPtr const & inHandler,
									 EventType const & inType );
		
		friend bool safeDelListener( EventListenerPtr const & inHandler,
									 EventType const & inType );

		friend bool safeTriggerEvent( IEventData const & inEvent );

		friend bool safeQueEvent( IEventDataPtr const & inEvent );

		friend bool threadSafeQueEvent( IEventDataPtr const & inEvent );

		friend bool safeAbortEvent( EventType const & inType,
									bool allOfType = false );

		friend bool safeTickEventManager( U64 maxMillis =
									IEventManager::kINFINITE );

		friend bool safeValidateEventType( EventType const & inType );

	};

	// /////////////////////////////////////////////////////////////////
	// Lastly, these methods are used for easy-access methods to carry
	// out basic operations without needing to pass around a pointer-to
	// ( or some other construct ) for sharing a global event manager.
	//
	// /////////////////////////////////////////////////////////////////
	bool safeAddListener( EventListenerPtr const & inHandler,
						  EventType const & inType );
		
	bool safeDelListener( EventListenerPtr const & inHandler,
						  EventType const & inType );

	bool safeTriggerEvent( IEventData const & inEvent );

	bool safeQueEvent( IEventDataPtr const & inEvent );
	bool threadSafeQueEvent( IEventDataPtr const & inEvent );

	bool safeAbortEvent( EventType const & inType,
						 bool allOfType /* = false */ );

	bool safeTickEventManager( U64 maxMillis
						/* = IEventManager::kINFINITE */ );

	bool safeValidateEventType( EventType const & inType );

}

#endif
