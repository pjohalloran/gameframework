//========================================================================
// EventManagerImpl.cpp : implementation side of the event system
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
// @file EventManagerImpl.cpp
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
// Please see the associated header for details on an changes I have
// made.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <string>

#include <boost/shared_ptr.hpp>

//// For WIN32 version of VTicks() using GetTickCount()
//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>

// Project Headers
#include "EventManagerImpl.h"

// For the global game/application pointer.
#include "GameMain.h"

#include "IGameTimer.h"
#include "GlfwGameTimer.h"


using boost::shared_ptr;

using std::string;

using LuaPlus::LuaObject;

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// ************************ EventManager ***************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	EventManager::EventManager(char const * const pName, bool setAsGlobal) throw (GameException &)
		: IEventManager(pName, setAsGlobal), m_typeList(), m_registry(), m_activeQueue(0), /*m_RealtimeEventQueue(),*/\
			m_MetaTable(), m_ScriptEventListenerMap(), m_ScriptActorEventListenerMap(), m_ScriptDefinedEventTypeSet()
	{
		if(!g_appPtr)
		{
			throw GameException(string("Cannot setup the EventManager as the global application pointer is NULL."));
		}

		//Open up access to script.
		{
			shared_ptr<LuaStateManager> luaStateManager = g_appPtr->GetLuaStateManager();

			// Note that this is slightly different than in the book, since the
			// Lua chapter was AFTER the event manager chapter.
			//Create our metatable...
			m_MetaTable = luaStateManager->GetGlobalState()->GetGlobals().CreateTable("EventManager");
			m_MetaTable.SetObject("__index", m_MetaTable);
			
			m_MetaTable.RegisterObjectDirect( "TriggerEvent", (EventManager *)0, &EventManager::TriggerEventFromScript );
			m_MetaTable.RegisterObjectDirect( "QueueEvent", (EventManager *)0, &EventManager::QueueEventFromScript );
			m_MetaTable.RegisterObjectDirect( "RegisterEventType", (EventManager *)0, &EventManager::RegisterScriptEventType );
			m_MetaTable.RegisterObjectDirect( "AddScriptListener", (EventManager *)0, &EventManager::AddScriptListener );
			m_MetaTable.RegisterObjectDirect( "RemoveScriptListener", (EventManager *)0, &EventManager::RemoveScriptListener );
			m_MetaTable.RegisterObjectDirect( "AddScriptActorListener", (EventManager *)0, &EventManager::AddScriptActorListener );
			m_MetaTable.RegisterObjectDirect( "RemoveScriptActorListener", (EventManager *)0, &EventManager::RemoveScriptActorListener );
			
			LuaObject luaStateManObj = luaStateManager->GetGlobalState()->BoxPointer(this);
			luaStateManObj.SetMetaTable(m_MetaTable);
			luaStateManager->GetGlobalState()->GetGlobals().SetObject("EventManager", luaStateManObj);
		}
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	EventManager::~EventManager()
	{
		try
		{
			m_activeQueue = 0;
		}
		catch(...)
		{
		}
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool EventManager::VAddListener(EventListenerPtr const &inListener, EventType const & inType)
	{
		if ( ! VValidateType( inType ) )
			return false;

		// check / update type list
		
		EventTypeSet::iterator evIt = m_typeList.find( inType );

		// find listener map entry, create one if no table already
		// exists for this entry ...

		EventListenerMap::iterator elmIt =
			m_registry.find( inType.getHashValue() );

		if ( elmIt == m_registry.end() )
		{
			EventListenerMapIRes elmIRes = m_registry.insert(
				EventListenerMapEnt( inType.getHashValue(),
									  EventListenerTable() ) );

			// whoops, could not insert into map!?!?
			if ( elmIRes.second == false )
				return false;

			// should not be possible, how did we insert and create
			// an empty table!?!?!
			if ( elmIRes.first == m_registry.end() )
				return false;

			// store it so we can update the mapped list next ...
			elmIt = elmIRes.first;
		}

		// update the mapped list of listeners, walk the existing
		// list (if any entries) to prevent duplicate addition of
		// listeners. This is a bit more costly at registration time
		// but will prevent the hard-to-notice duplicate event
		// propogation sequences that would happen if double-entries
		// were allowed.

		// note: use reference to make following code more simple
		EventListenerTable & evlTable = (*elmIt).second;
		
		for ( EventListenerTable::iterator it = evlTable.begin(),
				  itEnd = evlTable.end(); it != itEnd ; it++ )
		{
			bool bListenerMatch = ( *it == inListener );

			if ( bListenerMatch )
				return false;
		}

		// okay, event type validated, event listener validated,
		// event listener not already in map, add it

		evlTable.push_back( inListener );

		return true;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool EventManager::VDelListener(EventListenerPtr const &inListener, EventType const &inType)
	{
		if ( ! VValidateType( inType ) )
			return false;

		bool rc = false;

		// brute force method, iterate through all existing mapping
		// entries looking for the matching listener and remove it.

		for ( EventListenerMap::iterator it = m_registry.begin(),
				  itEnd = m_registry.end(); it != itEnd; it++ )
		{
			EventListenerTable & table    = it->second;

			for ( EventListenerTable::iterator it2 = table.begin(),
					  it2End = table.end(); it2 != it2End; it2++ )
			{
				if ( *it2 == inListener )
				{
					// found match, remove from table,
					table.erase( it2 );

					// update return code
					rc = true;

					// and early-quit the inner loop as addListener()
					// code ensures that each listener can only
					// appear in one event's processing list once.
					break;
				}
			}
		}
		
		return rc;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool EventManager::VTrigger(IEventData const &inEvent) const
	{
		if ( ! VValidateType( inEvent.VGetEventType() ) )
			return false;

		EventListenerMap::const_iterator itWC = m_registry.find( 0 );

		if ( itWC != m_registry.end() )
		{
			EventListenerTable const & table = itWC->second;
		
			for ( EventListenerTable::const_iterator it2 = table.begin(),
					  it2End = table.end(); it2 != it2End; it2++ )
			{
				(*it2)->VHandleEvent( inEvent );
			}
		}
		
		EventListenerMap::const_iterator it =
			m_registry.find( inEvent.VGetEventType().getHashValue() );

		if ( it == m_registry.end() )
			return false;

		EventListenerTable const & table = it->second;

		bool processed = false;
		
		for ( EventListenerTable::const_iterator it2 = table.begin(),
				  it2End = table.end(); it2 != it2End; it2++ )
		{
			EventListenerPtr listener = *it2;
			if ( listener->VHandleEvent( inEvent ) )
			{
				// only set to true, if processing eats the messages
				processed = true;
			}
		}
		
		return processed;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool EventManager::VQueueEvent ( IEventDataPtr const & inEvent )
	{
		assert ( m_activeQueue >= 0 );
		assert ( m_activeQueue < kNumQueues );

		if ( ! VValidateType( inEvent->VGetEventType() ) )
			return false;

		EventListenerMap::const_iterator it =
			m_registry.find( inEvent->VGetEventType().getHashValue() );

		if ( it == m_registry.end() )
		{		
			// if global listener is not active, then abort queue add
			EventListenerMap::const_iterator itWC = m_registry.find( 0 );

			if ( itWC == m_registry.end() )
			{
				// no listeners for this event, skipit
				return false;
			}
		}
		
		m_queues[m_activeQueue].push_back( inEvent );
		
		return true;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool EventManager::VThreadSafeQueueEvent(IEventDataPtr const & inEvent)
	{
		// TODO: Implement this when we get the multiprogramming chapter done.
		//m_RealtimeEventQueue.push(inEvent);
		//assert(0 && "Not implemented - the editor won't link with boost::thread");
		return true;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool EventManager::VAbortEvent(EventType const &inType, bool allOfType)
	{
		assert ( m_activeQueue >= 0 );
		assert ( m_activeQueue < kNumQueues );

		if ( ! VValidateType( inType ) )
			return false;

		EventListenerMap::iterator it = m_registry.find( inType.getHashValue() );

		if ( it == m_registry.end() )
			return false; // no listeners for this event, skipit

		bool rc = false;

		// See a good discussion on this code here:
		// http://www.mcshaffry.com/GameCode/thread.php?threadid=517&sid=

		EventQueue &evtQueue = m_queues[m_activeQueue];

		for ( EventQueue::iterator it = evtQueue.begin(),
				  itEnd = evtQueue.end(); it != itEnd; it++ )
		{
			if ( (*it)->VGetEventType() == inType )
			{
				it = evtQueue.erase(it);
				rc = true;
				if ( !allOfType )
					break;
			}
			else
			{
				++it;
			}
		}

		return rc;
	}

	// /////////////////////////////////////////////////////////////////
	// GameTimer version.
	//
	// /////////////////////////////////////////////////////////////////
	bool EventManager::VTick (U64 maxMillis)
	{
		//// Reset listeners removed flag upon entry.
		//m_listenersRemoved = false;

		// Create and start a timer on entry.
		shared_ptr<IGameTimer> timer(GCC_NEW GlfwGameTimer());
		timer->VStart();

		EventListenerMap::const_iterator itWC = m_registry.find( 0 );

		// TODO: Implement this when i get multiprogramming chapter done.
		//// This section added to handle events from other threads
		//// Check out Chapter 18
		//// --------------------------------------------------------
		//IEventDataPtr rte;
		//
		//while (m_RealtimeEventQueue.try_pop(rte))
		//{
		//	VQueueEvent(rte);
		//
		//	curMs = GetTickCount();
		//	if ( maxMillis != IEventManager::kINFINITE )
		//	{
		//
		//		if ( curMs >= maxMs )
		//		{
		//			assert(0 && "A realtime process is spamming the event manager!");
		//		}
		//	}
		//}
		//// --------------------------------------------------------

		// swap active queues, make sure new queue is empty after the
		// swap ...
		
		I32 queueToProcess = m_activeQueue;

		m_activeQueue = ( m_activeQueue + 1 ) % kNumQueues;

		m_queues[m_activeQueue].clear();

		// now process as many events as we can ( possibly time
		// limited ) ... always do AT LEAST one event, if ANY are
		// available ...

		while ( m_queues[queueToProcess].size() > 0 )
		{
			IEventDataPtr event = m_queues[queueToProcess].front();

			m_queues[queueToProcess].pop_front();
			
			EventType const & eventType = event->VGetEventType();

			EventListenerMap::const_iterator itListeners =
				m_registry.find( eventType.getHashValue() );

			// Handle of events of type wildcard
			if ( itWC != m_registry.end() )
			{
				EventListenerTable const & table = itWC->second;
		
				for(EventListenerTable::const_iterator it2 = table.begin(); it2 != table.end(); ++it2 )
				{
					(*it2)->VHandleEvent( *event );
				}
			}

			// no listerners currently for this event type, skipit
			if ( itListeners == m_registry.end() )
				continue;

			EventListenerTable const & table = itListeners->second;

			// Handle all events of the current type in the queue
			for(EventListenerTable::const_iterator it = table.begin(); it != table.end(); ++it)
			{
				if ( (*it)->VHandleEvent( *event ) )
				{
					break;
				}
			}

			if ( maxMillis != IEventManager::kINFINITE )
			{

				if ( static_cast<U64>(timer->VGetTime()) >= maxMillis )
				{
					// time ran about, abort processing loop
					break;
				}
			}
		}
		
		// if any events left to process, push them onto the active
		// queue.
		//
		// Note: to preserver sequencing, go bottom-up on the
		// raminder, inserting them at the head of the active
		// queue...

		bool queueFlushed = ( m_queues[queueToProcess].size() == 0 );

		if ( !queueFlushed )
		{
			while ( m_queues[queueToProcess].size() > 0 )
			{
				IEventDataPtr event = m_queues[queueToProcess].back();

				m_queues[queueToProcess].pop_back();

				m_queues[m_activeQueue].push_front( event );
			}
		}
		
		// all done, this pass
		
		return queueFlushed;
	}

	// --- information lookup functions ---

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool EventManager::VValidateType(EventType const &inType) const
	{
		if ( 0 == inType.getStr().length() )
			return false;

		if ( ( inType.getHashValue() == 0 ) &&
			(strcmp(inType.getStr().c_str(),kpWildcardEventType) != 0) )
			return false;

		EventTypeSet::const_iterator evIt =
			m_typeList.find( inType );

		if ( evIt == m_typeList.end() )
		{
			assert( 0 && "Failed validation of an event type; it was probably not registered with the EventManager!" );
			return false;
		}
		
		return true;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	EventListenerList EventManager::GetListenerList(EventType const &eventType) const
	{
		// invalid event type, so sad
		if ( ! VValidateType( eventType ) )
			return EventListenerList();

		EventListenerMap::const_iterator itListeners =
			m_registry.find( eventType.getHashValue() );

		// no listerners currently for this event type, so sad
		if ( itListeners == m_registry.end() )
			return EventListenerList();

		EventListenerTable const & table = itListeners->second;

		// there was, but is not now, any listerners currently for
		// this event type, so sad
		if ( table.size() == 0 )
			return EventListenerList();

		EventListenerList result;

		result.reserve( table.size() );
		
		for ( EventListenerTable::const_iterator it = table.begin(),
				  end = table.end(); it != end ; it++ )
		{
			result.push_back( *it );
		}

		return result;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	EventTypeList EventManager::GetTypeList (void) const
	{
		// no entries, so sad
		if ( m_typeList.size() == 0 )
			return EventTypeList();

		EventTypeList result;

		result.reserve( m_typeList.size() );

		for ( EventTypeSet::const_iterator it = m_typeList.begin(),
				  itEnd = m_typeList.end(); it != itEnd; it++ )
		{
			result.push_back( it->first );
		}
		
		return result;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool EventManager::AddScriptListener(char const * const pEventName, LuaObject callbackFunction)
	{
		//Ensure this event type exists.
		const EventType testEventType( pEventName );
		const EventTypeSet::const_iterator typeIter = m_typeList.find( testEventType );
		if ( m_typeList.end() == typeIter )
		{
			assert( 0 && "Attempted to listen to an event type that wasn't registered!" );
			return false;
		}

		const U64 eventID = testEventType.getHashValue();

		//OK, valid event type.  Make sure this isn't a duplicate.
		ScriptEventListenerMap::const_iterator mapIter = m_ScriptEventListenerMap.find( eventID );
		while ( m_ScriptEventListenerMap.end() != mapIter )
		{
			//Iterate through and ensure no duplicates.
			const ScriptEventListenerPtr evtListener = mapIter->second;
			const LuaObject & evtObj = evtListener->GetHandlerFunction();
			if ( evtObj == callbackFunction )
			{
				assert( 0 && "Attempted to listen to the same event handler twice!" );
				return false;
			}
			++mapIter;
		}

		//Now let's rez up a new script listener.
		ScriptEventListenerPtr listener( GCC_NEW ScriptEventListener( callbackFunction ) );

		m_ScriptEventListenerMap.insert( std::make_pair( eventID, listener ) );

		const bool bSuccess = VAddListener( listener, testEventType );
		return bSuccess;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool EventManager::RemoveScriptListener(const char *const pEventName, LuaObject callbackFunction)
	{
		//Ensure this event type exists.
		const EventType testEventType( pEventName );
		const EventTypeSet::const_iterator typeIter = m_typeList.find( testEventType );
		if ( m_typeList.end() == typeIter )
		{
			assert( 0 && "Attempted to remove a listener for an event type that doesn't exist!" );
			return false;
		}

		const U64 eventID = testEventType.getHashValue();

		//OK, now ensure it exists in the multimap.
		bool bFound = false;
		ScriptEventListenerMap::iterator mapIter = m_ScriptEventListenerMap.find( eventID );
		while ( m_ScriptEventListenerMap.end() != mapIter )
		{
			const ScriptEventListenerPtr evtListener = mapIter->second;
			const LuaObject & evtObj = evtListener->GetHandlerFunction();
			if ( evtObj == callbackFunction )
			{
				bFound = true;
				break;
			}
			++mapIter;
		}

		if ( false == bFound )
		{
			assert( 0 && "Attempted to remove a script listener for an event it was never listening for!" );
			return false;
		}

		//Remove this listener from the map.
		const ScriptEventListenerPtr listener = mapIter->second;
		m_ScriptEventListenerMap.erase( mapIter );

		//Now remove from the "main" listener set.
		const bool bSuccess = VDelListener( listener, testEventType );
		return bSuccess;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool EventManager::AddScriptActorListener(char const * const pEventName, LuaObject callbackFunction, const I32 actorID)
	{
		//Ensure this event type exists.
		const EventType testEventType( pEventName );
		const EventTypeSet::const_iterator typeIter = m_typeList.find( testEventType );
		if ( m_typeList.end() == typeIter )
		{
			assert( 0 && "Attempted to listen to an event type that wasn't registered!" );
			return false;
		}

		const U64 eventID = testEventType.getHashValue();

		//OK, valid event type.  Make sure this isn't a duplicate.
		ScriptActorEventListenerMap::const_iterator mapIter = m_ScriptActorEventListenerMap.find( eventID );
		while ( m_ScriptActorEventListenerMap.end() != mapIter )
		{
			//Iterate through and ensure no duplicates.
			const ScriptActorEventListenerPtr evtListener = mapIter->second;
			const LuaObject & evtObj = evtListener->GetHandlerFunction();
			if ( ( evtObj == callbackFunction ) && ( actorID == evtListener->GetActorID() ) )
			{
				assert( 0 && "Attempted to listen to the same event handler twice for a specific actor!" );
				return false;
			}
			++mapIter;
		}

		//Now let's rez up a new script listener.
		ScriptActorEventListenerPtr listener( GCC_NEW ScriptActorEventListener( callbackFunction, actorID ) );

		m_ScriptActorEventListenerMap.insert( std::make_pair( eventID, listener ) );

		const bool bSuccess = VAddListener( listener, testEventType );
		return bSuccess;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool EventManager::RemoveScriptActorListener(const char *const pEventName, LuaObject callbackFunction, const I32 actorID)
	{
		//Ensure this event type exists.
		const EventType testEventType( pEventName );
		const EventTypeSet::const_iterator typeIter = m_typeList.find( testEventType );
		if ( m_typeList.end() == typeIter )
		{
			assert( 0 && "Attempted to remove a listener for an event type that doesn't exist!" );
			return false;
		}

		const U64 eventID = testEventType.getHashValue();

		//OK, now ensure it exists in the multimap.
		bool bFound = false;
		ScriptActorEventListenerMap::iterator mapIter = m_ScriptActorEventListenerMap.find( eventID );
		while ( m_ScriptActorEventListenerMap.end() != mapIter )
		{
			const ScriptActorEventListenerPtr evtListener = mapIter->second;
			const LuaObject & evtObj = evtListener->GetHandlerFunction();
			if ( ( evtObj == callbackFunction ) && ( actorID == evtListener->GetActorID() ) )
			{
				bFound = true;
				break;
			}
			++mapIter;
		}

		if ( false == bFound )
		{
			assert( 0 && "Attempted to remove a script listener for an actor event it was never listening for!" );
			return false;
		}

		//Remove this listener from the map.
		const ScriptActorEventListenerPtr listener = mapIter->second;
		m_ScriptActorEventListenerMap.erase( mapIter );

		//Now remove from the "main" listener set.
		const bool bSuccess = VDelListener( listener, testEventType );
		return bSuccess;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool EventManager::TriggerEventFromScript(char const * const pEventName, LuaObject luaEventData)
	{
		const EventType eventType( pEventName );

		//Look this event type up.
		const EventTypeSet::const_iterator iter = m_typeList.find( eventType );
		if ( iter == m_typeList.end() )
		{
			assert( 0 && "Attempted to trigger an event type that doesn't exist!" );
			return false;
		}

		//This level of indirection lets us create code-side events or script-side events.
		IRegisteredEventPtr regEvent = iter->second;
		const bool bResult = regEvent->VTriggerEventFromScript( luaEventData );

		return bResult;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool EventManager::QueueEventFromScript(char const * const pEventName, LuaPlus::LuaObject luaEventData)
	{
		const EventType eventType( pEventName );

		//Look this event type up.
		const EventTypeSet::const_iterator iter = m_typeList.find( eventType );
		if ( iter == m_typeList.end() )
		{
			assert( 0 && "Attempted to queue an event type that doesn't exist!" );
			return false;
		}

		//This level of indirection lets us create code-side events or script-side events.
		IRegisteredEventPtr regEvent = iter->second;
		const bool bResult = regEvent->VQueueEventFromScript( luaEventData );

		return bResult;
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void EventManager::RegisterScriptEventType(char const * const pEventName)
	{
		//Create a new script-defined event object.
		const EventType eventType( pEventName );
		RegisterScriptEvent( eventType );
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void EventManager::RegisterScriptEvent( const EventType & eventType )
	{
		IRegisteredEventPtr metaData( GCC_NEW EventManager::ScriptDefinedEvent( eventType ) );
		AddRegisteredEventType( eventType, metaData );
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void EventManager::RegisterCodeOnlyEvent( const EventType & eventType )
	{
		IRegisteredEventPtr metaData( GCC_NEW CodeOnlyDefinedEvent() );
		AddRegisteredEventType( eventType, metaData );
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool EventManager::ScriptDefinedEvent::VTriggerEventFromScript( LuaObject & srcData ) const
	{
		const EvtData_ScriptEvtData scriptEvent( m_EventType, srcData );
		return safeTriggerEvent( scriptEvent );
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool EventManager::ScriptDefinedEvent::VQueueEventFromScript( LuaObject & srcData ) const
	{
		IEventDataPtr queueEventPtr(GCC_NEW EvtData_ScriptEvtData( m_EventType, srcData ));
		return safeQueEvent( queueEventPtr );
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void EventManager::AddRegisteredEventType( const EventType & eventType, IRegisteredEventPtr metaData )
	{
		const EventTypeSet::const_iterator iter = m_typeList.find( eventType );
		if ( iter != m_typeList.end() )
		{
			assert( 0 && "Attempted to register an event type that has already been registered!" );
		}
		else
		{
			// We're good...
			m_typeList.insert( std::make_pair( eventType, metaData ) );
		}
	}

}
