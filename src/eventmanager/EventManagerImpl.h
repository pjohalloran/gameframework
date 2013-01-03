#pragma once
#ifndef __EVENT_MANAGER_IMPL_H
#define __EVENT_MANAGER_IMPL_H

//========================================================================
// EventMangerImpl.h : implementation side of the event system
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
// @file EventManagerImpl.h
// @author Mike McShaffry
// @date 13/07/2010
//
// File contains the header for the EventManager class.
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
// - I implemented the VTicks() method with my SDL GameTimer class.
// - Made note of unimplemented method VQueueEventFromScript().
// - I commented out EventSnoopers' implementation for now as it
//		uses WIN32 specific stuff.
//
// /////////////////////////////////////////////////////////////////

#include <vector>
#include <list>
#include <map>
#include <set>

#include "EventManager.h"
#include "ScriptEventListener.h"

namespace GameHalloran
{
	typedef std::vector<EventListenerPtr> EventListenerList;
	typedef std::vector<EventType> EventTypeList;

	// /////////////////////////////////////////////////////////////////
	// @class EventManager
	// @author Mike McShaffry
	//
	// One implementation of the listener registry
	//
	// /////////////////////////////////////////////////////////////////
	class EventManager : public IEventManager
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param pName 
		// @param setAsGlobal
		//
		// @throw GameException If we fail to initialize the EventManager.
		//							(LUA state manager pointer is NULL)
		//
		// /////////////////////////////////////////////////////////////////
		explicit EventManager(char const * const pName, bool setAsGlobal) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EventManager();
		
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
		virtual bool VAddListener (EventListenerPtr const &inListener, EventType const &inType);

		// /////////////////////////////////////////////////////////////////
		// Remove a listener/type pairing from the internal tables
		//
		// @param inHandler The listener we are deleting.
		// @param inType The tpe of listener.
		//
		// @return bool false if the pairing was not found.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VDelListener (EventListenerPtr const &inListener, EventType const &inType);

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
		virtual bool VTrigger (IEventData const &inEvent) const;

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
		virtual bool VQueueEvent (IEventDataPtr const &inEvent);

		// /////////////////////////////////////////////////////////////////
		// Thread safe version of VQueueEvent().
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VThreadSafeQueueEvent (IEventDataPtr const &inEvent);

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
		virtual bool VAbortEvent (EventType const &inType, bool allOfType);

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
		virtual bool VTick (U64 maxMillis);

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
		virtual bool VValidateType(EventType const & inType) const;

		// /////////////////////////////////////////////////////////////////
		// Get the list of listeners associated with a specific event
		// type.
		//
		// @param eventType The type of event listeners we are looking for.
		//
		// @return EventListenerList List of event listener objects.
		//
		// /////////////////////////////////////////////////////////////////
		EventListenerList GetListenerList (EventType const &eventType) const;

		// /////////////////////////////////////////////////////////////////
		// Get the list of known event types.
		//
		// @return EventTypeList List of event type objects.
		//
		// /////////////////////////////////////////////////////////////////
		EventTypeList GetTypeList (void) const;

		// /////////////////////////////////////////////////////////////////
		// Registers an event type for the particular usage desired.
		// ...for an event defined in script:
		//
		// @param eventType The type of event (defined in a script).
		//
		// /////////////////////////////////////////////////////////////////
		void RegisterScriptEvent(const EventType & eventType);

		// /////////////////////////////////////////////////////////////////
		// Registers an event type for the particular usage desired.
		// ...for an event defined by code, *NOT* callable by script.
		//
		// @param eventType The type of event (defined in code).
		//
		// /////////////////////////////////////////////////////////////////
		void RegisterCodeOnlyEvent(const EventType & eventType);

		// /////////////////////////////////////////////////////////////////
		// Registers an event type for the particular usage desired.
		// ...for an event defined by code, but callable by script.  
		//
		// REQUIRES the event type to have a constructor taking a LuaObject.
		//
		// @param eventType The type of event (defined in code).
		//
		// /////////////////////////////////////////////////////////////////
		template< class T> void RegisterEvent(const EventType & eventType);

	private:
		
		// /////////////////////////////////////////////////////////////////
		// @class IRegisteredEvent
		// @author Mike McShaffry
		//
		// This class holds meta data for each event type, and allows 
		// (or disallows!) creation of code-defined events from script.
		//
		// /////////////////////////////////////////////////////////////////
		class IRegisteredEvent
		{
		public:

			// /////////////////////////////////////////////////////////////////
			// @enum eRegisteredEventMetaData
			//
			// Meta data about the type and usage of this event.
			//
			// kREMD_ScriptDefined: Event is defined in script.
			//
			// kREMD_CodeEventOnly: Event is defined by code, and is 
			//							*NOT* callable from script.
			// 
			//kREMD_CodeEventScriptCallable: Event is defined by code, but is 
			//									callable from script.
			//
			// /////////////////////////////////////////////////////////////////
			enum eRegisteredEventMetaData
			{
				kREMD_ScriptDefined,
				kREMD_CodeEventOnly,
				kREMD_CodeEventScriptCallable,
			};

		private:

			const eRegisteredEventMetaData m_MetaData;				///< Meta data about the type and usage of this event.

		public:

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// @param metaData Informs us about the type and usage of this event.
			//
			// /////////////////////////////////////////////////////////////////
			IRegisteredEvent(const eRegisteredEventMetaData metaData) : m_MetaData(metaData)
			{
			}

			// /////////////////////////////////////////////////////////////////
			// Destructor.
			//
			// /////////////////////////////////////////////////////////////////
			virtual ~IRegisteredEvent()
			{
			}

			// /////////////////////////////////////////////////////////////////
			// Trigger an event incoming from a script.
			//
			// @param LuaObject Data from the script.
			//
			// /////////////////////////////////////////////////////////////////
			virtual bool VTriggerEventFromScript(LuaPlus::LuaObject &srcData) const = 0;

			// /////////////////////////////////////////////////////////////////
			// Queue up an event incoming from a script.
			//
			// @param LuaObject Data from the script.
			//
			// /////////////////////////////////////////////////////////////////
			virtual bool VQueueEventFromScript(LuaPlus::LuaObject &srcData) const = 0;

			// /////////////////////////////////////////////////////////////////
			// Get the metadata on this event.
			//
			// /////////////////////////////////////////////////////////////////
			eRegisteredEventMetaData GetEventMetaData(void) const
			{
				return (m_MetaData);
			}
		};

		// /////////////////////////////////////////////////////////////////
		// @class ScriptDefinedEvent
		// @author Mike McShaffry
		//
		// Script defined event type.
		//
		// /////////////////////////////////////////////////////////////////
		class ScriptDefinedEvent : public IRegisteredEvent
		{
		private:
			//We need to hold onto the event type for when it gets triggered.
			const EventType m_EventType;

		public:

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// @param eventType The type of the event.
			//
			// /////////////////////////////////////////////////////////////////
			ScriptDefinedEvent(const EventType &eventType)\
				: IRegisteredEvent(IRegisteredEvent::kREMD_ScriptDefined),\
					m_EventType(eventType)
			{
			}

			// /////////////////////////////////////////////////////////////////
			// Trigger an event incoming from a script.
			//
			// @param LuaObject Data from the script.
			//
			// /////////////////////////////////////////////////////////////////
			virtual bool VTriggerEventFromScript(LuaPlus::LuaObject &srcData) const;

			// /////////////////////////////////////////////////////////////////
			// Queue up an event incoming from a script.
			//
			// @param LuaObject Data from the script.
			//
			// /////////////////////////////////////////////////////////////////
			virtual bool VQueueEventFromScript(LuaPlus::LuaObject &srcData) const;
		};

		// /////////////////////////////////////////////////////////////////
		// @class CodeOnlyDefinedEvent
		// @author Mike McShaffry
		//
		// Code defined, but script is NOT allowed to create this event type.
		//
		// /////////////////////////////////////////////////////////////////
		class CodeOnlyDefinedEvent : public IRegisteredEvent
		{
		public:

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// /////////////////////////////////////////////////////////////////
			explicit CodeOnlyDefinedEvent(void)\
				: IRegisteredEvent(IRegisteredEvent::kREMD_CodeEventOnly)
			{
			}

			// /////////////////////////////////////////////////////////////////
			// Trigger an event incoming from a script.
			//
			// NB This is not possible as events of this form may only be triggered
			// in code. E.g. important system events like game shutdown, etc.
			//
			// @param LuaObject Data from the script.
			//
			// /////////////////////////////////////////////////////////////////
			virtual bool VTriggerEventFromScript(LuaPlus::LuaObject &srcData) const
			{
				assert( 0 && "Attempted to trigger a code-ONLY triggerable event!" );
				return (false);
			}

			// /////////////////////////////////////////////////////////////////
			// Queue up an event incoming from a script.
			//
			// NB This is not possible as events of this form may only be queued
			// in code. E.g. important system events like game shutdown, etc.
			//
			// @param LuaObject Data from the script.
			//
			// /////////////////////////////////////////////////////////////////
			virtual bool VQueueEventFromScript(LuaPlus::LuaObject &srcData) const
			{
				assert( 0 && "Attempted to queue a code-ONLY queueable event!" );
				return (false);
			}
		};

		// /////////////////////////////////////////////////////////////////
		// @class ScriptCallableCodeEvent
		// @author Mike McShaffry
		//
		// Code defined, but also creatable from script.
		//
		// /////////////////////////////////////////////////////////////////
		template < class T >
		class ScriptCallableCodeEvent : public IRegisteredEvent
		{
		public:

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// /////////////////////////////////////////////////////////////////
			explicit ScriptCallableCodeEvent(void)\
				: IRegisteredEvent(IRegisteredEvent::kREMD_CodeEventScriptCallable)
			{
			}

			// /////////////////////////////////////////////////////////////////
			// Trigger an event incoming from a script.
			//
			// @param LuaObject Data from the script.
			//
			// /////////////////////////////////////////////////////////////////
			virtual bool VTriggerEventFromScript(LuaPlus::LuaObject &srcData) const
			{
				const T eventData(srcData);	//Construct directly.
				return safeTriggerEvent(eventData);
			}

			// /////////////////////////////////////////////////////////////////
			// Queue up an event incoming from a script.
			//
			// @param LuaObject Data from the script.
			//
			// /////////////////////////////////////////////////////////////////
			virtual bool VQueueEventFromScript( LuaPlus::LuaObject & srcData ) const
			{
				IEventDataPtr eventPtr(GCC_NEW T(srcData));
				return safeQueEvent(eventPtr);
			}
		};		

		// Pointer to a valid registered event.
		typedef boost::shared_ptr<IRegisteredEvent> IRegisteredEventPtr;

		// /////////////////////////////////////////////////////////////////
		// Verifies that such an event does not already exist, then 
		// registers it.
		//
		// @param eventType The type of event.
		// @param metaData Data about the event.
		//
		// /////////////////////////////////////////////////////////////////
		void AddRegisteredEventType(const EventType &eventType, IRegisteredEventPtr metaData);

		// one global instance
		typedef std::map<EventType, IRegisteredEventPtr>		EventTypeSet;

		// insert result into event type set
		typedef std::pair<EventTypeSet::iterator, bool>		EventTypeSetIRes;

		// one list per event type ( stored in the map )
		typedef std::list<EventListenerPtr>					EventListenerTable;

		// mapping of event ident to listener list
		typedef std::map<U32, EventListenerTable>	EventListenerMap;

		// entry in the event listener map
		typedef std::pair<U32, EventListenerTable>	EventListenerMapEnt;

		// insert result into listener map
		typedef std::pair<EventListenerMap::iterator, bool>	EventListenerMapIRes;

		// queue of pending- or processing-events
		typedef std::list<IEventDataPtr>						EventQueue;

		// /////////////////////////////////////////////////////////////////
		// @enum eConstats
		// Constants for the EventManager
		//
		// kNumQueues:		The number of queues managed by the EM.
		//
		// /////////////////////////////////////////////////////////////////
		enum eConstants
		{
			kNumQueues = 2
		};
		
		EventTypeSet m_typeList;				///< list of registered event types.
		EventListenerMap m_registry;			///< mapping of event types to listeners.
		EventQueue m_queues[kNumQueues];		///< event processing queue, double buffered to prevent infinite cycles.
		I32 m_activeQueue;						///< valid denoting which queue is actively processing, en-queing events goes to the
												///<  opposing queue.
		// TODO: Add this in when we get multiprogramming chapter done.
		//ThreadSafeEventQueue m_RealtimeEventQueue;

		// ALL SCRIPT-RELATED FUNCTIONS
	private:

		// /////////////////////////////////////////////////////////////////
		// Registers a script-based event.
		//
		// @param pEventName The name of the event.
		//
		// /////////////////////////////////////////////////////////////////
		void RegisterScriptEventType(char const * const pEventName);

		// /////////////////////////////////////////////////////////////////
		// Add a script listener.  Note that we pass a Lua *function* as the
		// second parameter, and tie that function to a specific
		// event name. We'll call the Lua function when the event type comes in.
		//
		// @param pEventName The name of the event.
		// @param callbackFunction The LUA function to call.
		//
		// /////////////////////////////////////////////////////////////////
		bool AddScriptListener(char const * const pEventName, LuaPlus::LuaObject callbackFunction);

		// /////////////////////////////////////////////////////////////////
		// Remove a script listener.  Note that we pass a Lua *function* as the
		// second parameter, and tie that function to a specific
		// event name. We'll call the Lua function when the event type comes in.
		//
		// @param pEventName The name of the event.
		// @param callbackFunction The LUA function to call.
		//
		// /////////////////////////////////////////////////////////////////
		bool RemoveScriptListener(char const * const pEventName, LuaPlus::LuaObject callbackFunction);

		// /////////////////////////////////////////////////////////////////
		// Triggers an event from script.  The event data will be serialized for any
		// code listeners.  Script listeners will receive the data table passed in.
		//
		// @param pEventName The name of the event.
		// @param callbackFunction The LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		bool TriggerEventFromScript(char const * const pEventName, LuaPlus::LuaObject luaEventData);

		// /////////////////////////////////////////////////////////////////
		// Queues an event from script.  The event data will be serialized for any
		// code listeners.  Script listeners will receive the data table passed in.
		//
		// @param pEventName The name of the event.
		// @param callbackFunction The LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		bool QueueEventFromScript(char const * const pEventName, LuaPlus::LuaObject luaEventData);

		// /////////////////////////////////////////////////////////////////
		// Actor-specific event listener controls.  These will pass in the actor context
		// upon calling.
		//
		// @param pEventName The name of the event.
		// @param callbackFunction The LUA function to call.
		// @param actorId The ID of the actor.
		//
		// /////////////////////////////////////////////////////////////////
		bool AddScriptActorListener(char const * const pEventName, LuaPlus::LuaObject callbackFunction, const I32 actorID);

		// /////////////////////////////////////////////////////////////////
		// Actor-specific event listener controls.  These will pass in the actor context
		// upon calling.
		//
		// @param pEventName The name of the event.
		// @param callbackFunction The LUA function to call.
		// @param actorId The ID of the actor.
		//
		// /////////////////////////////////////////////////////////////////
		bool RemoveScriptActorListener(char const * const pEventName, LuaPlus::LuaObject callbackFunction, const I32 actorID);

		LuaPlus::LuaObject m_MetaTable;												///< Allows access for script calls.

		// Holds all allocated script listeners.
		// It maps an event ID to a set of listeners.
		typedef boost::shared_ptr<ScriptEventListener> ScriptEventListenerPtr;
		typedef std::multimap<U32, ScriptEventListenerPtr> ScriptEventListenerMap;
		ScriptEventListenerMap m_ScriptEventListenerMap;

		// Holds all allocated script ACTOR listeners.
		typedef boost::shared_ptr<ScriptActorEventListener> ScriptActorEventListenerPtr;
		typedef std::multimap<U32, ScriptActorEventListenerPtr> ScriptActorEventListenerMap;
		ScriptActorEventListenerMap m_ScriptActorEventListenerMap;

		// Holds all registered script event types.
		typedef std::set<EventType> ScriptDefinedEventTypeSet;
		ScriptDefinedEventTypeSet m_ScriptDefinedEventTypeSet;
	};

	// /////////////////////////////////////////////////////////////////
	// PJ - Implementation of the method (I think its located in the header
	// so it may create and use the IRegisteredEvent and classes of that
	// base type).
	//
	// /////////////////////////////////////////////////////////////////
	template<class T> void EventManager::RegisterEvent(const EventType & eventType)
	{
		IRegisteredEventPtr metaData(GCC_NEW ScriptCallableCodeEvent<T>());
		AddRegisteredEventType(eventType, metaData);
	}

}

#endif // __EVENT_MANAGER_IMPL_H
