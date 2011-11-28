//========================================================================
// Events.h : defines common game events
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

#ifndef __GAME_EVENTS_H
#define __GAME_EVENTS_H

// /////////////////////////////////////////////////////////////////
// @file Events.h
// @author Mike McShaffry
// @date 13/07/2010
//
// File contains the header for common game events.
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
// - 
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <sstream>

// Project Headers
#include "GameBase.h"
#include "EventManager.h"
#include "GameMain.h"
#include "Actors.h"
#include "Vector.h"
#include "Matrix.h"

namespace GameHalloran
{
	// Auxillary data decls ...
	//
	// data that is passed per-event in the userData parameter
	// 
	// ( for some, but not all, events )

	//-- new object notification

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_New_Actor
	// @author Mike McShaffry
	//
	// This event is sent out when an actor is
	// *actually* created.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_New_Actor : public BaseEventData
	{
	public:
		static const EventType sk_EventType;				///< Type of event.

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param id The ID of the Actor.
		// @param pCreateParams The parameters belonging to the actor.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_New_Actor(ActorId id, ActorParams *pCreateParams)
		{
			m_id = id;
			m_pActorParams = reinterpret_cast<ActorParams *>(GCC_NEW char[pCreateParams->VGetSize()]);
			memcpy(m_pActorParams, pCreateParams, pCreateParams->VGetSize());
			m_pActorParams->VSetId(id);
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in A string stream object.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_New_Actor(std::istringstream &in)
		{
			in >> m_id;
			m_pActorParams = ActorParams::CreateFromStream( in );
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr( GCC_NEW EvtData_New_Actor( m_id, m_pActorParams ) );
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_New_Actor() 
		{ 
			Delete(m_pActorParams); 
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
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			//Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );

			//Serialize the data necessary.
			m_LuaEventData.SetInteger( "ActorId", m_id );
			
			//Create a new sub-table to hold actor params.
			LuaPlus::LuaObject actorParamsObj = m_LuaEventData.CreateTable( "ActorParams" );
			if ( NULL != m_pActorParams )
			{
				//TODO JWC!   James what the hell is supposed to go here???
			}

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event data out to a string stream.
		//
		// @param out The output string stream.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_id << " ";
			m_pActorParams->VSerialize(out);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the Actor who was just created.
		//
		// /////////////////////////////////////////////////////////////////
		ActorId GetActorId() const
		{
			return (m_id);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the parameters of the Actor who was just created.
		//
		// /////////////////////////////////////////////////////////////////
		const ActorParams *GetActorParams() const
		{
			return (m_pActorParams);
		};

	private:
		ActorId m_id;									///< ID of actor created
		ActorParams *m_pActorParams;					///< Parameters for actor
		LuaPlus::LuaObject	m_LuaEventData;				///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Destroy_Actor
	// @author Mike McShaffry
	//
	// This event is sent out when an actor is
	// *actually* destroyed.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Destroy_Actor : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param id The ID of the actor who was destroyed.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Destroy_Actor(ActorId id) : m_id(id)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The stream containing the data required to construct the
		//				event.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Destroy_Actor(std::istringstream &in)
		{
			in >> m_id;
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr (GCC_NEW EvtData_Destroy_Actor(m_id));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Destroy_Actor() {}

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
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );

			//Serialize the data necessary.
			m_LuaEventData.SetInteger( "ActorId", m_id );
			
			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event to a stream.
		//
		// @param out The stream to serialize the event to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_id;
		}

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the actor who was destroyed.
		//
		// /////////////////////////////////////////////////////////////////
		ActorId GetActorId() const
		{
			return (m_id);
		};

	private:

		ActorId m_id;										///< The ID of the actor who was just destroyed.
		LuaPlus::LuaObject m_LuaEventData;					///< The lua event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Move_Actor
	// @author Mike McShaffry
	//
	// This event is sent out when an actor is
	// moved.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Move_Actor : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param id The ID of the actor who has moved.
		// @param mat The movement the actor has made.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Move_Actor( ActorId id, const Matrix4 &mat) : m_Id(id), m_Mat(mat) { };

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream used to create the event.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Move_Actor(std::istringstream &in)
		{
			in >> m_Id;
			for (I32 i=0; i < Matrix4::NUMBER_ELEMENTS; ++i)
			{
				in >> m_Mat[i];
			}
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
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			//Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );

			//Now assign the data.
			m_LuaEventData.SetInteger("ActorId", m_Id);

			//We don't want a whole 4x4 matrix, so just give us the position.
			Vector4 srcPos;
			m_Mat.GetPosition(srcPos);
			LuaPlus::LuaObject posTable = m_LuaEventData.CreateTable("Pos", 3);
			posTable.SetNumber(1, srcPos.GetX());
			posTable.SetNumber(2, srcPos.GetY());
			posTable.SetNumber(3, srcPos.GetZ());

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event to a stream.
		//
		// @param out The stream to serialize the event to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			Matrix4 copy(m_Mat);
			out << m_Id << " ";
			for (I32 i=0; i < Matrix4::NUMBER_ELEMENTS; ++i)
			{
				out << copy[i] << " ";
			}
		};

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return (IEventDataPtr(GCC_NEW EvtData_Move_Actor(m_Id, m_Mat)));
		};

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the actor.
		//
		// /////////////////////////////////////////////////////////////////
		ActorId GetActorId() const
		{
			return (m_Id);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the movement matrix the actor just made.
		//
		// /////////////////////////////////////////////////////////////////
		Matrix4 GetMovement() const
		{
			return (m_Mat);
		};

	private:
		ActorId m_Id;										///< The ID of the actor.
		Matrix4 m_Mat;										///< The movement the actor made.
		LuaPlus::LuaObject m_LuaEventData;					///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_New_Game
	// @author Mike McShaffry
	//
	// This event is sent out when a new game is started.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_New_Game : public EmptyEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_New_Game()
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_New_Game());
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_New_Game( std::istringstream &in )
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param srcData The LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_New_Game(LuaPlus::LuaObject srcData)
		{
		}
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_End_Game
	// @author PJ O Halloran
	//
	// This event is sent out when a game is quitted or ends.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_End_Game : public EmptyEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_End_Game()
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_End_Game());
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_End_Game( std::istringstream &in )
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param srcData The LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_End_Game(LuaPlus::LuaObject srcData)
		{
		}
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Request_Start_Game
	// @author Mike McShaffry
	//
	// This event is sent out when there is a request to start a new game.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Request_Start_Game : public EmptyEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Request_Start_Game()
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Request_Start_Game( std::istringstream &in )
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param srcData The LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Request_Start_Game( LuaPlus::LuaObject srcData )
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Request_Start_Game());
		}
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Game_State
	// @author Mike McShaffry
	//
	// This event is sent out when there is a game state change.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Game_State : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param gameState The new game state.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Game_State(const BaseGameState gameState) : m_gameState(gameState)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Game_State(std::istringstream &in)
		{
			I32 tempVal;
			in >> tempVal;
			m_gameState = static_cast<BaseGameState>(tempVal);
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Game_State(m_gameState));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Game_State() {}

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
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );

			//Serialize the data necessary.
			char const * const pGSVarName = "GameState";
			switch( m_gameState )
			{
			case BGS_Initializing:
				m_LuaEventData.SetString( pGSVarName, "initializing" );
				break;
			case BGS_LoadingGameEnvironment:
				m_LuaEventData.SetString( pGSVarName, "loadinggame" );
				break;
			case BGS_MainMenu:
				m_LuaEventData.SetString( pGSVarName, "mainmenu" );
				break;
			case BGS_WaitingForPlayers:
				m_LuaEventData.SetString( pGSVarName, "waitingforplayers" );
				break;
			case BGS_SpawnAI:
				m_LuaEventData.SetString( pGSVarName, "spawnai" );
				break;
			case BGS_Running:
				m_LuaEventData.SetString( pGSVarName, "running" );
				break;
			case BGS_Paused:
				m_LuaEventData.SetString( pGSVarName, "paused" );
				break;
			default:
				assert( 0 && "Unknown/unsupported game state encountered!" );
				break;
			}

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			const I32 tempVal = static_cast<I32>(m_gameState);
			out << tempVal;
		}

		// /////////////////////////////////////////////////////////////////
		// Get the new game state.
		//
		// /////////////////////////////////////////////////////////////////
		BaseGameState GetState() const
		{
			return (m_gameState);
		};

	private:
		BaseGameState m_gameState;									///< The new game state.
		LuaPlus::LuaObject m_LuaEventData;							///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Remote_Client
	// @author Mike McShaffry
	//
	// This event is sent out when ???.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Remote_Client : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the new event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param socketId The ID of the socket.
		// @param ipaddress The IP address of the client.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Remote_Client( const I32 socketid, const I32 ipaddress )
			: m_socketId( socketid ), m_ipAddress(ipaddress)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr( GCC_NEW EvtData_Remote_Client( m_socketId, m_ipAddress ) );
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The input stream.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Remote_Client( std::istringstream &in )
		{
			in >> m_socketId;
			in >> m_ipAddress;
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Remote_Client() {}

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
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );

			//Serialize the data necessary.
			m_LuaEventData.SetInteger( "SocketId", m_socketId );
			m_LuaEventData.SetInteger( "IpAddress", m_ipAddress );

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event out to a stream.
		//
		// @param out The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_socketId << " ";
			out << m_ipAddress;
		}

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the socket.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetSocketId() const
		{
			return (m_socketId);
		};

		// /////////////////////////////////////////////////////////////////
		// The IP address of the client.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetIpAddress() const
		{
			return (m_ipAddress);
		};

	private:
		LuaPlus::LuaObject m_LuaEventData;					///< The LUA event data.
		I32 m_socketId;										///< The ID of the socket.
		I32 m_ipAddress;									///< The IP address of the client.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Update_Tick
	// @author Mike McShaffry
	//
	// This event is sent out once per game loop when there is an update
	// event.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Update_Tick : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param deltaMilliseconds The number of milliseconds since the last
		//							update.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Update_Tick( const I32 deltaMilliseconds )
			: m_DeltaMilliseconds( deltaMilliseconds )
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr (GCC_NEW EvtData_Update_Tick ( m_DeltaMilliseconds ) );
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
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			//Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );

			//Serialize the data necessary.
			m_LuaEventData.SetInteger( "DeltaMS", m_DeltaMilliseconds );
			m_LuaEventData.SetNumber( "Seconds", ( (F32)m_DeltaMilliseconds / 1000.0f ) );

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event to a stream.
		//
		// @param out The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out)
		{
			assert( 0 && "You should not be serializing update ticks!" );
		}

		// /////////////////////////////////////////////////////////////////
		// Get the new event type.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetDeltaMilliseconds() const
		{
			return (m_DeltaMilliseconds);
		};

	private:
		LuaPlus::LuaObject m_LuaEventData;						///< The LUA event data.
		I32 m_DeltaMilliseconds;								///< The number of milliseconds since the last update.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Debug_String
	// @author Mike McShaffry
	//
	// This event is sent out when ????
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Debug_String : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// @enum eDebugStringType
		//
		// The type of debug string.
		//
		// kDST_ScriptMsg:		A debug string from a script??
		//
		// /////////////////////////////////////////////////////////////////
		enum eDebugStringType
		{
			kDST_ScriptMsg,
			// ...Add additional types here.
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param debugMessage The debug message.
		// @param type The tpe of debug string.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Debug_String( const std::string & debugMessage, const eDebugStringType type )
			: m_DebugMessage( debugMessage ), m_Type( type )
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr ( GCC_NEW EvtData_Debug_String (m_DebugMessage, m_Type) );
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
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			//Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );

			//Serialize the data necessary.
			m_LuaEventData.SetString( "Text", m_DebugMessage.c_str() );
			switch( m_Type )
			{
			case kDST_ScriptMsg:
				m_LuaEventData.SetString( "Type", "Script" );
				break;
			default:
				assert( 0 && "Debug message of unknown/unsupported type encountered!" );
				break;
			}

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void serialize( std::ostringstream & out )
		{
			assert( 0 && "You should not be serializing debug strings!" );
		}

		// /////////////////////////////////////////////////////////////////
		// Get the Debug Message.
		//
		// /////////////////////////////////////////////////////////////////
		std::string GetDebugMessage() const
		{
			return (m_DebugMessage);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the type.
		//
		// /////////////////////////////////////////////////////////////////
		eDebugStringType GetType() const
		{
			return (m_Type);
		};

	private:
		std::string m_DebugMessage;							///< The debug message.
		eDebugStringType m_Type;							///< The debug string type.
		LuaPlus::LuaObject m_LuaEventData;					///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Network_Player_Actor_Assignment
	// @author Mike McShaffry
	//
	// This event is sent out when ????
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Network_Player_Actor_Assignment : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param actorId The ID of the actor.
		// @param remotePlayerId The ID of the remote player.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Network_Player_Actor_Assignment( const ActorId actorId, const I32 remotePlayerId )
			: m_actorId( actorId ), m_remotePlayerId(remotePlayerId)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			 return IEventDataPtr( GCC_NEW EvtData_Network_Player_Actor_Assignment( m_actorId, m_remotePlayerId ) ) ;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The input stream.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Network_Player_Actor_Assignment( std::istringstream &in )
		{
			in >> m_actorId;
			in >> m_remotePlayerId;
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Network_Player_Actor_Assignment() {}

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
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );

			//Serialize the data necessary.
			m_LuaEventData.SetInteger( "ActorId", m_actorId );
			m_LuaEventData.SetInteger( "RemotePlayerId", m_remotePlayerId );

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param out The output stream.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_actorId << " ";
			out << m_remotePlayerId;
		}

		// /////////////////////////////////////////////////////////////////
		// Get the Actor ID.
		//
		// /////////////////////////////////////////////////////////////////
		ActorId GetActorId() const
		{
			return (m_actorId);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the remote player ID.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetType() const
		{
			return (m_remotePlayerId);
		};

	private:
		ActorId m_actorId;								///< The ID of the actor.
		I32 m_remotePlayerId;							///< The ID of the remote player.
		LuaPlus::LuaObject m_LuaEventData;				///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Decompress_Request
	// @author Mike McShaffry
	//
	// This event is sent out when a file is requested to be decompressed.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Decompress_Request : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param zipFileName The filename of the ZIP file.
		// @param filename The filename.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Decompress_Request(const std::string &zipFileName, const std::string &filename)
			: m_zipFileName(zipFileName), m_fileName(filename)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Get a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr (GCC_NEW EvtData_Decompress_Request(m_zipFileName, m_fileName));
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
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );
			m_bHasLuaEventData = false;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize( std::ostringstream & out )
		{
			assert( 0 && "You should not be serializing decompression requests!" );
		}

		// /////////////////////////////////////////////////////////////////
		// Get the ZIP filename.
		//
		// /////////////////////////////////////////////////////////////////
		std::string GetZipFileName() const
		{
			return (m_zipFileName);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the filename.
		//
		// /////////////////////////////////////////////////////////////////
		std::string GetFileName() const
		{
			return (m_fileName);
		};

	private:
		std::string m_zipFileName;						///< The zip filename.
		std::string m_fileName;							///< The filename??
		LuaPlus::LuaObject m_LuaEventData;				///< LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Decompression_Progress
	// @author Mike McShaffry
	//
	// This event is sent out when a file decompression progress event
	// occurs.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Decompression_Progress : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param progress The current progress of the 
		// @param zipFileName The compressed filename.
		// @param filename The filename??
		// @param buffer ??
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Decompression_Progress(I32 progress, const std::string &zipFileName, const std::string &filename, void *buffer)
			: m_progress(progress), m_zipFileName(zipFileName), m_fileName(filename), m_buffer(buffer)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr (GCC_NEW EvtData_Decompression_Progress(m_progress, m_zipFileName, m_fileName, m_buffer));
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
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );
			m_bHasLuaEventData = false;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out)
		{
			assert( 0 && "You should not be serializing decompression progress events!" );
		}

		// /////////////////////////////////////////////////////////////////
		// Get the ZIP filename.
		//
		// /////////////////////////////////////////////////////////////////
		std::string GetZipFileName() const
		{
			return (m_zipFileName);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the filename.
		//
		// /////////////////////////////////////////////////////////////////
		std::string GetFileName() const
		{
			return (m_fileName);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the buffer.
		//
		// /////////////////////////////////////////////////////////////////
		void *GetBuffer() const
		{
			return (m_buffer);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the progress.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetProgress() const
		{
			return (m_progress);
		};

	private:
		I32 m_progress;									///< Current progress.
		std::string m_zipFileName;						///< The ZIP filename.
		std::string m_fileName;							///< The filename??
		void *m_buffer;									///< The buffer??
		LuaPlus::LuaObject m_LuaEventData;				///< LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Request_New_Actor
	// @author Mike McShaffry
	//
	// This event is sent by a client ASKING to
	// create an actor.  It can be sent from script
	// or via code.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Request_New_Actor : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param srcData The LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Request_New_Actor(LuaPlus::LuaObject srcData)
			: m_ActorDef(srcData), m_pActorParams(NULL)
		{
			m_bHasLuaEventData = true;	//Our Lua event data got passed into us!
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param pCreateParams The actor parameters.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Request_New_Actor(const ActorParams *pCreateParams)
			: m_pActorParams(NULL)
		{
			m_bHasLuaEventData = false;	//We *don't* have Lua event data.
			m_pActorParams = reinterpret_cast<ActorParams *>(GCC_NEW char[pCreateParams->VGetSize()]);
			memcpy(m_pActorParams, pCreateParams, pCreateParams->VGetSize());
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr (GCC_NEW EvtData_Request_New_Actor(m_ActorDef));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Request_New_Actor()
		{
			Delete(m_pActorParams);
		}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert( ( true == m_bHasLuaEventData ) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!" );
			return m_ActorDef;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( true == m_bHasLuaEventData ) && "Already built lua event data!" );
			return;	//Already "built" when the event got created.
		}

		// /////////////////////////////////////////////////////////////////
		// Get the Actor Parameters.
		//
		// /////////////////////////////////////////////////////////////////
		const ActorParams *GetActorParameters() const
		{
			return (m_pActorParams);
		};

	private:
		ActorParams *m_pActorParams;					///< Pointer to the Actor Parameters.
		LuaPlus::LuaObject m_ActorDef;					///< LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Request_New_Actor
	// @author Mike McShaffry
	//
	// This event updates an actor's properties.  It is very general purpose,
	// and only updates those stats requested to be altered.
	// It's kind of gross, but it's useful as a generic method of altering
	// parameters via script.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_UpdateActorParams : public BaseEventData
	{
	private:
		LuaPlus::LuaObject m_LuaEventData;				///< LUA event data.
		ActorId m_ActorID;								///< The ID of the actor whos parameters are being updated.

	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the actor ID.
		//
		// /////////////////////////////////////////////////////////////////
		ActorId GetActorId() const
		{
			return (m_ActorID);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param eventData The LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_UpdateActorParams(LuaPlus::LuaObject eventData)
			: m_LuaEventData(eventData), m_ActorID(0)
		{
			assert( eventData.IsTable() && "UpdateActorParams event requires a valid table!" );
			LuaPlus::LuaObject actorID = eventData[ "ActorID" ];
			assert( actorID.IsInteger() && "UpdateActorParams event requires member 'ActorID', and for it to be an integer!" );
			m_ActorID = actorID.GetInteger();
			m_bHasLuaEventData = true;	//We're just using what was passed into us.
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr (GCC_NEW EvtData_UpdateActorParams(m_LuaEventData) );
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
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( true == m_bHasLuaEventData ) && "Already built lua event data!" );
			return;	//Already "built" when the event got created.
		}
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Pause_Game
	// @author PJ O' Halloran
	//
	// Event fired when the game is paused or unpaused.  This is only used
	// to inform script side listeners of a change in the pause state of
	// the game.
	//
	// The C++ side of the game keeps track of the current state with the
	// EvtData_Game_State event!
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Pause_Game_Event : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param paused The current pause state of the game.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Pause_Game_Event(const bool paused) : m_paused(paused)
		{
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param eventData The LUA event data passed in from the script.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Pause_Game_Event(LuaPlus::LuaObject &eventData)\
			: m_paused(false), m_LuaEventData(eventData)
		{
			if(eventData.IsTable())
			{
				LuaPlus::LuaObject pauseTable = eventData["PauseState"];
				if(pauseTable.IsBoolean())
				{
					m_paused = pauseTable.GetBoolean();
				}
			}
			m_bHasLuaEventData = true;	//We're just using what was passed into us.
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Pause_Game_Event(std::istringstream &in)
		{
			I32 tempVal;
			in >> tempVal;
			if(tempVal == 0)
			{
				m_paused = false;
			}
			else
			{
				m_paused = true;
			}
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Pause_Game_Event(m_paused));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Pause_Game_Event() {}

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

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			m_LuaEventData.SetBoolean("PauseState", m_paused);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			const I32 tempVal = static_cast<I32>(m_paused);
			out << tempVal;
		}

		// /////////////////////////////////////////////////////////////////
		// Get the new game state.
		//
		// /////////////////////////////////////////////////////////////////
		bool IsPaused() const
		{
			return (m_paused);
		};

	private:
		bool m_paused;									///< The new game paused state.
		LuaPlus::LuaObject m_LuaEventData;				///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Button_Action
	// @author PJ O' Halloran
	//
	// Event fired when a UI button is clicked.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Button_Action : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param buttonId The screen ID of the button.
		// @param eventTypeId The ID of the type of button event.
		// @param checked In the case its a checkbox, is the button checked?
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Button_Action(const U32 buttonId, const I32 eventTypeId, const bool checked = true)\
			: m_id(buttonId), m_evtId(eventTypeId), m_checked(checked)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param eventData The LUA event data passed in from the script.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Button_Action(LuaPlus::LuaObject &eventData)\
			: m_id(0), m_evtId(0), m_checked(false), m_LuaEventData(eventData)
		{
			if(eventData.IsTable())
			{
				LuaPlus::LuaObject buttonIdTable = eventData["ButtonId"];
				if(buttonIdTable.IsInteger())
				{
					m_id = buttonIdTable.GetInteger();
				}
				LuaPlus::LuaObject eventIdTable = eventData["EventTypeId"];
				if(eventIdTable.IsInteger())
				{
					m_evtId = eventIdTable.GetInteger();
				}
				LuaPlus::LuaObject checkedTable = eventData["Checked"];
				if(checkedTable.IsBoolean())
				{
					m_checked = checkedTable.GetBoolean();
				}
			}
			m_bHasLuaEventData = true;	//We're just using what was passed into us.
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Button_Action(std::istringstream &in)
		{
			in >> m_id;
			in >> m_evtId;
			I32 checked;
			in >> checked;
			m_checked = (checked == 0 ? false : true);
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Button_Action(m_id, m_evtId, m_checked));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Button_Action() {}

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

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			m_LuaEventData.SetInteger("ButtonId", m_id);
			m_LuaEventData.SetInteger("EventTypeId", m_evtId);
			m_LuaEventData.SetBoolean("Checked", m_checked);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_id << " ";
			out << m_evtId << " ";
			out << (m_checked ? 1 : 0) << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the button that was pressed.
		//
		// /////////////////////////////////////////////////////////////////
		U32 GetButtonId() const
		{
			return (m_id);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the button event.
		//
		// /////////////////////////////////////////////////////////////////
		U32 GetButtonEventId() const
		{
			return (m_evtId);
		};

		// /////////////////////////////////////////////////////////////////
		// Check if the button is currently checked (in the case the button
		// is a checkbox, otherwise this field may be safely ignored).
		//
		// /////////////////////////////////////////////////////////////////
		bool IsChecked() const
		{
			return (m_checked);
		};

	private:
		U32 m_id;								///< The ID of the button.
		I32 m_evtId;									///< ID of the event.
		bool m_checked;									///< Is the button currently checked (Checkbox only).
		LuaPlus::LuaObject m_LuaEventData;				///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_List_Button_Action
	// @author PJ O' Halloran
	//
	// Event fired when a list button is clicked.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_List_Button_Action : public EvtData_Button_Action
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param buttonId The screen ID of the button.
		// @param eventTypeId The ID of the type of button event.
		// @param textRef The text of the current selection.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_List_Button_Action(const U32 buttonId, const I32 eventTypeId, const std::string &textRef)\
			: EvtData_Button_Action(buttonId, eventTypeId), m_text(textRef)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param eventData The LUA event data passed in from the script.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_List_Button_Action(LuaPlus::LuaObject &eventData)\
			: EvtData_Button_Action(eventData), m_LuaEventData(eventData)
		{
			if(eventData.IsTable())
			{
				LuaPlus::LuaObject textTable = eventData["Selection"];
				if(textTable.IsString())
				{
					m_text = textTable.GetString();
				}
			}
			m_bHasLuaEventData = true;	//We're just using what was passed into us.
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_List_Button_Action(std::istringstream &in) : EvtData_Button_Action(in), m_text()
		{
			in >> m_text;
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_List_Button_Action(EvtData_Button_Action::GetButtonId(), EvtData_Button_Action::GetButtonEventId(), m_text));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_List_Button_Action() {}

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

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			m_LuaEventData.SetInteger("ButtonId", EvtData_Button_Action::GetButtonId());
			m_LuaEventData.SetInteger("EventTypeId", EvtData_Button_Action::GetButtonEventId());
			m_LuaEventData.SetBoolean("Checked", EvtData_Button_Action::IsChecked());
			m_LuaEventData.SetString("Selection", m_text.c_str());

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			EvtData_Button_Action::VSerialize(out);
			out << m_text << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the current text selection of the list button.
		//
		// /////////////////////////////////////////////////////////////////
		std::string GetSelectionText() const
		{
			return (m_text);
		};

	private:
		std::string m_text;								///< The text selection of the button.
		LuaPlus::LuaObject m_LuaEventData;				///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Slider_Action
	// @author PJ O' Halloran
	//
	// Event fired when a UI slider control is adjusted.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Slider_Action : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param sliderId The screen ID of the slider.
		// @param eventTypeId The ID of the type of slider event.
		// @param sliderPos The position of the slider.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Slider_Action(const U32 sliderId, const I32 eventTypeId, const F32 sliderPos)\
			: m_id(sliderId), m_evtId(eventTypeId), m_sliderPos(sliderPos)
		{
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param eventData The LUA event data passed in from the script.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Slider_Action(LuaPlus::LuaObject &eventData)\
			: m_id(0), m_evtId(0), m_sliderPos(0.0f), m_LuaEventData(eventData)
		{
			if(eventData.IsTable())
			{
				LuaPlus::LuaObject idTable = eventData["SliderId"];
				if(idTable.IsInteger())
				{
					m_id = idTable.GetInteger();
				}
				LuaPlus::LuaObject evtIdTable = eventData["EventTypeId"];
				if(evtIdTable.IsInteger())
				{
					m_evtId = evtIdTable.GetInteger();
				}
				LuaPlus::LuaObject sliderPosTable = eventData["SliderPosition"];
				if(sliderPosTable.IsNumber())
				{
					m_sliderPos = static_cast<F32>(sliderPosTable.GetNumber());
				}
			}
			m_bHasLuaEventData = true;	//We're just using what was passed into us.
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Slider_Action(std::istringstream &in)
		{
			in >> m_id;
			in >> m_evtId;
			in >> m_sliderPos;
		};

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Slider_Action(m_id, m_evtId, m_sliderPos));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Slider_Action() {}

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

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			m_LuaEventData.SetInteger("SliderId", m_id);
			m_LuaEventData.SetInteger("EventTypeId", m_evtId);
			// Encode position as a percentage since LuaPlus cant deal with floats...
			m_LuaEventData.SetNumber("SliderPosition", m_sliderPos);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_id << " ";
			out << m_evtId << " ";
			out << m_sliderPos << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the slider on screen that was adjusted.
		//
		// /////////////////////////////////////////////////////////////////
		U32 GetSliderId() const
		{
			return (m_id);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the slider controls event.
		//
		// /////////////////////////////////////////////////////////////////
		U32 GetSliderEventId() const
		{
			return (m_evtId);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the sliders position.
		//
		// /////////////////////////////////////////////////////////////////
		F32 GetSliderPosition() const
		{
			return (m_sliderPos);
		};

	private:
		U32 m_id;								///< The ID of the slider.
		I32 m_evtId;									///< ID of the event.
		F32 m_sliderPos;								///< The new position of the slider.
		LuaPlus::LuaObject m_LuaEventData;				///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Request_Pause_Game_Event
	// @author PJ O' Halloran
	//
	// Event fired when the a module requests the logic layer to change
	// the game into a paused state.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Request_Pause_Game_Event  : public EmptyEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Request_Pause_Game_Event()
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Request_Pause_Game_Event());
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Request_Pause_Game_Event(std::istringstream &in)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param srcData The LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Request_Pause_Game_Event(LuaPlus::LuaObject srcData)
		{
		}
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Dialog_Open_Request_Event
	// @author PJ O' Halloran
	//
	// Event fired when a module in the system requests a dialog to be
	// opened.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Dialog_Open_Request_Event : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// @enum DialogType
		//
		// The type of dialog that is to be opened.
		//
		// eConfirm:	Confirm dialog.
		// eError:		Error dialog.
		// eInfo:		Information dialog.
		//
		// /////////////////////////////////////////////////////////////////
		enum DialogType
		{
			eConfirm = 0,
			eError,
			eInfo,
			eUnknown
		};

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param dialogType The type of dialog to create.
		// @param text The text to display on the dialog.
		// @param timeout The time in seconds to display the dialog for (0.0
		//					means display until user closes dialog).
		// @param scriptId The script side ID of the dialog.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Dialog_Open_Request_Event(const DialogType dialogType, const std::string &text, const F32 timeout, const U32 scriptId)\
			: m_type(dialogType), m_text(text), m_timeout(timeout), m_scriptId(scriptId)
		{
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param eventData The LUA event data passed in from the script.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Dialog_Open_Request_Event(LuaPlus::LuaObject &eventData)\
			: m_type(eUnknown), m_text(""), m_timeout(0.0f), m_scriptId(0), m_LuaEventData(eventData)
		{
			if(eventData.IsTable())
			{
				LuaPlus::LuaObject typeTable = eventData["DialogType"];
				if(typeTable.IsString())
				{
					if(strcmp(typeTable.GetString(), "confirm") == 0)
					{
						m_type = eConfirm;
					}
					else if(strcmp(typeTable.GetString(), "error") == 0)
					{
						m_type = eError;
					}
					else if(strcmp(typeTable.GetString(), "information") == 0)
					{
						m_type = eInfo;
					}
				}
				LuaPlus::LuaObject textTable = eventData["DialogText"];
				if(textTable.IsString())
				{
					m_text.assign(textTable.GetString());
				}
				LuaPlus::LuaObject timeoutTable = eventData["DialogTimeout"];
				if(timeoutTable.IsNumber())
				{
					m_timeout = static_cast<F32>(timeoutTable.GetNumber());
				}
				LuaPlus::LuaObject sIdTable = eventData["DialogScriptId"];
				if(sIdTable.IsInteger())
				{
					m_scriptId = sIdTable.GetInteger();
				}
			}
			m_bHasLuaEventData = true;	//We're just using what was passed into us.
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Dialog_Open_Request_Event(std::istringstream &in)
		{
			I32 tmp;
			in >> tmp;
			m_type = static_cast<DialogType>(tmp);
			in >> m_text;
			in >> m_timeout;
			in >> m_scriptId;
		};

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Dialog_Open_Request_Event(m_type, m_text, m_timeout, m_scriptId));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Dialog_Open_Request_Event() {}

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

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			switch(m_type)
			{
			case eConfirm:
				{
					m_LuaEventData.SetString("DialogType", "confirm");
				}
				break;
			case eError:
				{
					m_LuaEventData.SetString("DialogType", "error");
				}
				break;
			case eInfo:
				{
					m_LuaEventData.SetString("DialogType", "information");
				}
				break;
			default:
				m_LuaEventData.SetString("DialogType", "UNKNOWN ENUM");
				break;
			}
			m_LuaEventData.SetString("DialogText", m_text.c_str());
			m_LuaEventData.SetNumber("DialogTimeout", m_timeout);
			m_LuaEventData.SetInteger("DialogScriptId", m_scriptId);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << static_cast<I32>(m_type) << " ";
			out << m_text << " ";
			out << m_timeout << " ";
			out << m_scriptId << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the type of dialog.
		//
		// /////////////////////////////////////////////////////////////////
		DialogType GetDialogType() const
		{
			return (m_type);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the text to display on the dialog.
		//
		// /////////////////////////////////////////////////////////////////
		std::string GetText() const
		{
			return (m_text);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the number of seconds to display the dialog before
		// automatically hiding it (0.0 == INFINITE).
		//
		// /////////////////////////////////////////////////////////////////
		F32 GetTimeout() const
		{
			return (m_timeout);
		};

		// /////////////////////////////////////////////////////////////////
		// Script side dialog ID.
		//
		// /////////////////////////////////////////////////////////////////
		U32 GetScriptId() const
		{
			return (m_scriptId);
		};

	private:
		DialogType m_type;								///< Type of dialog to open (this controls the tpe of buttons it will contain and its look and feel plus the result it is capable of returning).
		std::string m_text;								///< Dialog text.
		F32 m_timeout;								///< Number of seconds to display the dialog before automatically hiding it (0.0 == INFINITE).
		U32 m_scriptId;						///< The ID assigned to the dialog so that script listeners may tell which dialog is opening/closing.
		LuaPlus::LuaObject m_LuaEventData;				///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Dialog_Open_Event
	// @author PJ O' Halloran
	//
	// Event fired when a dialog reports that it has opened.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Dialog_Open_Event : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param id The ID of the dialog.
		// @param scriptId Script side ID.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Dialog_Open_Event(const U32 id, const U32 scriptId)\
			: m_id(id), m_scriptId(scriptId)
		{
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param eventData The LUA event data passed in from the script.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Dialog_Open_Event(LuaPlus::LuaObject &eventData)\
			: m_id(0), m_scriptId(0), m_LuaEventData(eventData)
		{
			if(eventData.IsTable())
			{
				LuaPlus::LuaObject idTable = eventData["DialogId"];
				if(idTable.IsInteger())
				{
					m_id = idTable.GetInteger();
				}
				LuaPlus::LuaObject sIdTable = eventData["DialogScriptId"];
				if(sIdTable.IsInteger())
				{
					m_scriptId = sIdTable.GetInteger();
				}
			}
			m_bHasLuaEventData = true;	//We're just using what was passed into us.
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Dialog_Open_Event(std::istringstream &in)
		{
			in >> m_id;
			in >> m_scriptId;
		};

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Dialog_Open_Event(m_id, m_scriptId));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Dialog_Open_Event() {}

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

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			m_LuaEventData.SetInteger("DialogId", m_id);
			m_LuaEventData.SetInteger("DialogScriptId", m_scriptId);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_id << " ";
			out << m_scriptId << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the dialog.
		//
		// /////////////////////////////////////////////////////////////////
		U32 GetId() const
		{
			return (m_id);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the Script side ID of the dialog.
		//
		// /////////////////////////////////////////////////////////////////
		U32 GetScriptId() const
		{
			return (m_scriptId);
		};

	private:
		U32 m_id;								///< The ID of the dialog.
		U32 m_scriptId;						///< Script side ID of dialog.
		LuaPlus::LuaObject m_LuaEventData;				///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Dialog_Close_Event
	// @author PJ O' Halloran
	//
	// Event fired when a dialog reports that it has closed.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Dialog_Close_Event : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// @enum DialogResult
		//
		// The result of the dialog:
		// eOk:			User confirmed action/event.
		// eCancel:		User cancelled action.
		// eYes:		User agreed to action.
		// eNo:			User disallows action to proceed.
		// eNone:		For dialogs that have no valid result (Error/Info dialog).
		//
		// N.B. eYes and eOk mean the same thing and are interchangeable.
		//  The same is true for eCancel and eNo.
		//
		// /////////////////////////////////////////////////////////////////
		enum DialogResult
		{
			eOk = 0,
			eCancel,
			eYes,
			eNo,
			eNone
		};

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param id The ID of the dialog..
		// @param result The result of the dialog.
		// @param scriptId Script side ID of the dialog.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Dialog_Close_Event(const U32 id, const DialogResult result, const U32 scriptId)\
			: m_id(id), m_result(result), m_scriptId(scriptId)
		{
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param eventData The LUA event data passed in from the script.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Dialog_Close_Event(LuaPlus::LuaObject &eventData)\
			: m_id(0), m_result(eNone), m_scriptId(0), m_LuaEventData(eventData)
		{
			if(eventData.IsTable())
			{
				LuaPlus::LuaObject resultTable = eventData["DialogResult"];
				if(resultTable.IsString())
				{
					if(strcmp(resultTable.GetString(), "ok") == 0)
					{
						m_result = eOk;
					}
					else if(strcmp(resultTable.GetString(), "cancel") == 0)
					{
						m_result = eCancel;
					}
					else if(strcmp(resultTable.GetString(), "yes") == 0)
					{
						m_result = eYes;
					}
					else if(strcmp(resultTable.GetString(), "no") == 0)
					{
						m_result = eNo;
					}
					else if(strcmp(resultTable.GetString(), "none") == 0)
					{
						m_result = eNone;
					}
				}
				LuaPlus::LuaObject idTable = eventData["DialogId"];
				if(idTable.IsInteger())
				{
					m_id = idTable.GetInteger();
				}
				LuaPlus::LuaObject sIdTable = eventData["DialogScriptId"];
				if(sIdTable.IsInteger())
				{
					m_scriptId = sIdTable.GetInteger();
				}
			}
			m_bHasLuaEventData = true;	//We're just using what was passed into us.
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Dialog_Close_Event(std::istringstream &in)
		{
			in >> m_id;
			I32 tmp;
			in >> tmp;
			m_result = static_cast<DialogResult>(tmp);
			in >> m_scriptId;
		};

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Dialog_Close_Event(m_id, m_result, m_scriptId));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Dialog_Close_Event() {}

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

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );
			
			// Serialize the data necessary.
			switch(m_result)
			{
			case eOk:
				{
					m_LuaEventData.SetString("DialogResult", "ok");
				}
				break;
			case eCancel:
				{
					m_LuaEventData.SetString("DialogResult", "cancel");
				}
				break;
			case eYes:
				{
					m_LuaEventData.SetString("DialogResult", "yes");
				}
				break;
			case eNo:
				{
					m_LuaEventData.SetString("DialogResult", "no");
				}
				break;
			case eNone:
				{
					m_LuaEventData.SetString("DialogResult", "none");
				}
				break;
			default:
				m_LuaEventData.SetString("DialogResult", "UNKNOWN ENUM");
				break;
			}
			m_LuaEventData.SetInteger("DialogId", m_id);
			m_LuaEventData.SetInteger("DialogScriptId", m_scriptId);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_id << " ";
			out << static_cast<I32>(m_result) << " ";
			out << m_scriptId << " ";
		}

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the dialog.
		//
		// /////////////////////////////////////////////////////////////////
		U32 GetId() const
		{
			return (m_id);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the result of dialog.
		//
		// /////////////////////////////////////////////////////////////////
		DialogResult GetDialogResult() const
		{
			return (m_result);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the script side ID of the dialog.
		//
		// /////////////////////////////////////////////////////////////////
		U32 GetScriptId() const
		{
			return (m_scriptId);
		};

	private:
		U32 m_id;								///< The ID of the dialog.
		DialogResult m_result;							///< Result of dialog.
		U32 m_scriptId;						///< Script side ID of dialog.
		LuaPlus::LuaObject m_LuaEventData;				///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Loading_Progress
	// @author PJ O' Halloran
	//
	// Event fired when a loading operation makes some progress.
	//
	// The progress is measured as a F32 between 0.0f and 1.0f.
	//
	// A negative value indicates some error occurred during the load.
	//
	// This event is intended to notify the application of a loading
	// screens progress but it may be used for any type of loading
	// operation also.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Loading_Progress : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param id The ID of the loading operation.
		// @param progress The progress of the operation.
		// @param errMsg Optional error message in case the loading operation
		//					fails.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Loading_Progress(const I32 id, const F32 progress, boost::optional<std::string> errMsg = boost::optional<std::string>())\
			: m_id(id), m_progress(progress), m_errMsg(errMsg)
		{
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param srcData Script data describing the event.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Loading_Progress(LuaPlus::LuaObject &srcData)\
			: m_id(0), m_progress(0.0f), m_errMsg(), m_LuaEventData(srcData)
		{
			if(srcData.IsTable())
			{
				LuaPlus::LuaObject idTable = srcData["LoadingId"];
				if(idTable.IsInteger())
				{
					m_id = idTable.GetInteger();
				}
				LuaPlus::LuaObject progTable = srcData["Progress"];
				if(progTable.IsNumber())
				{
					m_progress = static_cast<F32>(progTable.GetNumber());
				}
				if(m_progress > 0.0f)
				{
					LuaPlus::LuaObject msgTable = srcData["ErrorMessage"];
					if(msgTable.IsString())
					{
						m_errMsg = boost::optional<std::string>(msgTable.GetString());
					}
				}
			}
			m_bHasLuaEventData = true;
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream to create the event from.
		//
		// /////////////////////////////////////////////////////////////////
		EvtData_Loading_Progress(std::istringstream &in)
		{
			in >> m_id;
			in >> m_progress;
			std::string msg;
			in >> msg;
			m_errMsg = boost::optional<std::string>(msg);
		}

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return IEventDataPtr(GCC_NEW EvtData_Loading_Progress(m_id, m_progress, m_errMsg));
		}

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~EvtData_Loading_Progress() {}

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

			// Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable(pState);
			
			// Serialize the data necessary.
			m_LuaEventData.SetInteger("LoadingId", m_id);
			m_LuaEventData.SetNumber("Progress", m_progress);
			if(m_errMsg.is_initialized())
			{
				m_LuaEventData.SetString("ErrorMessage", (*m_errMsg).c_str());
			}
			else
			{
				m_LuaEventData.SetString("ErrorMessage", "");
			}

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event.
		//
		// @param The stream to serialize the event out to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << I32(m_id) << " ";
			out << m_progress << " ";
			if(m_errMsg.is_initialized())
			{
				out << *m_errMsg << " ";
			}
		}

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the loading operation.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetId() const
		{
			return (m_id);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the loading operations progress (0.0 <= P <= 1.0).
		//
		// /////////////////////////////////////////////////////////////////
		F32 GetProgress() const
		{
			return (m_progress);
		};

		// /////////////////////////////////////////////////////////////////
		// Is the loading operation complete?
		//
		// /////////////////////////////////////////////////////////////////
		bool IsComplete() const
		{
			return (m_progress >= 1.0f);
		};

		// /////////////////////////////////////////////////////////////////
		// Did an error occur on the loading operation.
		//
		// /////////////////////////////////////////////////////////////////
		bool AnyError() const
		{
			return (m_progress < 0.0f);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the error message of the load in the case of some failure
		// (if any).
		//
		// /////////////////////////////////////////////////////////////////
		boost::optional<std::string> GetErrorMsg() const
		{
			return (m_errMsg);
		};

	private:
		I32 m_id;										///< ID of loading operation.
		F32 m_progress;								///< Progress of load.
		boost::optional<std::string> m_errMsg;			///< Optional error message in case an error occurred during the load.
		LuaPlus::LuaObject m_LuaEventData;				///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Move_Kinematic_Actor
	// @author PJ O Halloran
	//
	// Sent from the view layer to the logic layer when the view layer
	// requests to move a game actor manually.  For example the player
	// might move a character/object around the scene using a gamepad.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Move_Kinematic_Actor : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param id The ID of the actor who has moved.
		// @param mat The movement the actor has made.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Move_Kinematic_Actor( ActorId id, const Matrix4 &mat) : m_id(id), m_mat(mat) { };

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream used to create the event.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Move_Kinematic_Actor(std::istringstream &in)
		{
			in >> m_id;
			for (I32 i=0; i < Matrix4::NUMBER_ELEMENTS; ++i)
			{
				in >> m_mat[i];
			}
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
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			//Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable( pState );

			//Now assign the data.
			m_LuaEventData.SetInteger("ActorId", m_id);

			//We don't want a whole 4x4 matrix, so just give us the position.
			Vector4 srcPos;
			m_mat.GetPosition(srcPos);
			LuaPlus::LuaObject posTable = m_LuaEventData.CreateTable("Pos", 3);
			posTable.SetNumber(1, srcPos.GetX());
			posTable.SetNumber(2, srcPos.GetY());
			posTable.SetNumber(3, srcPos.GetZ());

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event to a stream.
		//
		// @param out The stream to serialize the event to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			Matrix4 copy(m_mat);
			out << m_id << " ";
			for (I32 i=0; i < Matrix4::NUMBER_ELEMENTS; ++i)
			{
				out << copy[i] << " ";
			}
		};

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return (IEventDataPtr(GCC_NEW EvtData_Move_Kinematic_Actor(m_id, m_mat)));
		};

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the actor.
		//
		// /////////////////////////////////////////////////////////////////
		ActorId GetActorId() const
		{
			return (m_id);
		};

		// /////////////////////////////////////////////////////////////////
		// Get the movement matrix the actor just made.
		//
		// /////////////////////////////////////////////////////////////////
		Matrix4 GetMovement() const
		{
			return (m_mat);
		};

	private:
		ActorId m_id;										///< The ID of the actor.
		Matrix4 m_mat;										///< The movement the actor made.
		LuaPlus::LuaObject m_LuaEventData;					///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Video_Resolution_Change
	// @author PJ O Halloran
	//
	// Sent by the WindowManager class when the applications video
	// resolution changes.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Video_Resolution_Change : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param oldW The old width of the screen.
		// @param oldH The old height of the screen.
		// @param newW The new width of the screen.
		// @param newH The new height of the screen.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Video_Resolution_Change(const I32 oldW, const I32 oldH, const I32 newW, const I32 newH)\
			: m_oldWidth(oldW), m_oldHeight(oldH), m_newWidth(newW), m_newHeight(newH), m_LuaEventData() { };

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream used to create the event.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Video_Resolution_Change(std::istringstream &in)
		{
			in >> m_oldWidth;
			in >> m_oldHeight;
			in >> m_newWidth;
			in >> m_newHeight;
		}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert((true == m_bHasLuaEventData) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!");
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert((false == m_bHasLuaEventData) && "Already built lua event data!");

			//Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable(pState);

			//Now assign the data.
			m_LuaEventData.SetInteger("OldWidth", m_oldWidth);
			m_LuaEventData.SetInteger("OldHeight", m_oldHeight);
			m_LuaEventData.SetInteger("NewWidth", m_newWidth);
			m_LuaEventData.SetInteger("NewHeight", m_newHeight);

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event to a stream.
		//
		// @param out The stream to serialize the event to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << m_oldWidth << " ";
			out << m_oldHeight << " ";
			out << m_newWidth << " ";
			out << m_newHeight << " ";
		};

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return (IEventDataPtr(GCC_NEW EvtData_Video_Resolution_Change(m_oldWidth, m_oldHeight, m_newWidth, m_newHeight)));
		};

		// /////////////////////////////////////////////////////////////////
		// Get the old video resolution width.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetOldWidth() const { return (m_oldWidth); };

		// /////////////////////////////////////////////////////////////////
		// Get the old video resolution height.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetOldHeight() const { return (m_oldHeight); };

		// /////////////////////////////////////////////////////////////////
		// Get the new video resolution width.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetNewWidth() const { return (m_newWidth); };

		// /////////////////////////////////////////////////////////////////
		// Get the new video resolution width.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetNewHeight() const { return (m_newHeight); };

	private:
		I32 m_oldWidth;										///< Old width of the video resolution.
		I32 m_oldHeight;									///< Old height of the video resolution.
		I32 m_newWidth;										///< New width of the video resolution.
		I32 m_newHeight;									///< New height of the video resolution.
		LuaPlus::LuaObject m_LuaEventData;					///< The LUA event data.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_Physics_Diagnostics
	// @author PJ O Halloran
	//
	// Sent by the View layer of the game to update the status of the
	// physics diagnostics rendering.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_Physics_Diagnostics : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType(void) const
		{
			return sk_EventType;
		}

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param renderDiag Should the logic layer render the physics diags?
		// @param debugMode The new diagnostic mode.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Physics_Diagnostics(const bool renderDiag, const boost::optional<I32> debugMode = boost::optional<I32>())\
			: m_render(renderDiag), m_mode(debugMode), m_LuaEventData() { };

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param in The string stream used to create the event.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_Physics_Diagnostics(std::istringstream &in)
		{
			m_mode.reset();

			I32 tmp;
			in >> tmp;
			m_render = (tmp == 0 ? false : true);
			in >> tmp;
			if(tmp == 1)
			{
				in >> tmp;
				m_mode = tmp;
			}
		}

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert((true == m_bHasLuaEventData) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!");
			return m_LuaEventData;
		}

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert((false == m_bHasLuaEventData) && "Already built lua event data!");

			//Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable(pState);

			//Now assign the data.
			m_LuaEventData.SetBoolean("Render", m_render);
			if(m_mode.is_initialized())
			{
				m_LuaEventData.SetInteger("Mode", *m_mode);
			}

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Serialize the event to a stream.
		//
		// @param out The stream to serialize the event to.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const
		{
			out << (m_render ? 1 : 0) << " ";
			if(m_mode.is_initialized())
			{
				out << "1 " << m_mode << " ";
			}
			else
			{
				out << "0 ";
			}
		};

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return (IEventDataPtr(GCC_NEW EvtData_Physics_Diagnostics(m_render, m_mode)));
		};

		// /////////////////////////////////////////////////////////////////
		// Get the old video resolution width.
		//
		// /////////////////////////////////////////////////////////////////
		bool IsRender() const { return (m_render); };

		// /////////////////////////////////////////////////////////////////
		// Get the new diagnostic mode.
		//
		// /////////////////////////////////////////////////////////////////
		boost::optional<I32> GetMode() const { return (m_mode); };


	private:
		bool m_render;								///< Should the logic layer render diagnostics.
		boost::optional<I32> m_mode;				///< The diagnostic mode.
		LuaPlus::LuaObject m_LuaEventData;			///< The LUA event data.
	};

}

#endif
