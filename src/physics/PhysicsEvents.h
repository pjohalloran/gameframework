#pragma once
#ifndef __GF_PHYSICS_EVENTS_H
#define __GF_PHYSICS_EVENTS_H

//========================================================================
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
// @file PhysicsEvents.h
// @author Mike McShaffry & PJ O Halloran
// @date 14/10/2010
//
// Header file containing the general purpose physics game events and
// the physics event listener class.
//
// /////////////////////////////////////////////////////////////////

#include <string>
#include <boost/optional.hpp>

#include "EventManager.h"
#include "Events.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_PhysTrigger_Enter
	// @author Mike McShaffry & PJ O Halloran
	//
	// Event that is broadcast when the physics system detects that a
	// game object has entered a trigger area.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_PhysTrigger_Enter : public BaseEventData
	{
	public:

		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type ID.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const { return (sk_EventType); };

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param triggerID The ID of the trigger point.
		// @param other The ID of the game object that entered the trigger
		//				area.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_PhysTrigger_Enter(const I32 triggerID, boost::optional<ActorId> other)\
			: m_triggerID(triggerID), m_other(other) { };

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the data.
		//
		// /////////////////////////////////////////////////////////////////
		IEventDataPtr VCopy() const { return (IEventDataPtr(GCC_NEW EvtData_PhysTrigger_Enter(m_triggerID, m_other))); };

		// /////////////////////////////////////////////////////////////////
		// Get a LUA object containg the event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert((true == m_bHasLuaEventData) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!");
			return (m_LuaEventData);
		};

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert( ( false == m_bHasLuaEventData ) && "Already built lua event data!" );

			//Get the global state.
			LuaPlus::LuaState *pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable(pState);

			m_LuaEventData.SetInteger("TriggerId", m_triggerID);
			if(m_other.is_initialized())
			{
				m_LuaEventData.SetInteger("ActorId", *m_other);
			}

			m_bHasLuaEventData = true;
		}

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the trigger area.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetTriggerId() const { return (m_triggerID); };

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the actor.
		//
		// /////////////////////////////////////////////////////////////////
		boost::optional<ActorId> GetActorId() const { return (m_other); };

	private:

		LuaPlus::LuaObject m_LuaEventData;			///< LUA script link object.
		I32 m_triggerID;					///< ID of the trigger point that an object entered.
		boost::optional<ActorId> m_other;	///< The ID of the game object that entered it.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_PhysTrigger_Leave
	// @author Mike McShaffry & PJ O Halloran
	//
	// Event triggered by the physics system when a game object leaves
	// a trigger area.
	//
	// /////////////////////////////////////////////////////////////////
	struct EvtData_PhysTrigger_Leave : public BaseEventData
	{
	public:

		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event type ID.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType(void) const { return (sk_EventType); };

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param triggerID The ID of the trigger point.
		// @param other The ID of the game object that left the trigger
		//				area.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_PhysTrigger_Leave(const I32 triggerID, boost::optional<ActorId> other)\
			: m_triggerID(triggerID), m_other(other) { };

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const { return (IEventDataPtr(GCC_NEW EvtData_PhysTrigger_Leave(m_triggerID, m_other))); };

		// /////////////////////////////////////////////////////////////////
		// Get a LUA object containg the event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert((true == m_bHasLuaEventData) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!");
			return (m_LuaEventData);
		};

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert((false == m_bHasLuaEventData) && "Already built lua event data!" );

			//Get the global state.
			LuaPlus::LuaState *pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable(pState);

			m_LuaEventData.SetInteger("TriggerId", m_triggerID);
			if(m_other.is_initialized())
			{
				m_LuaEventData.SetInteger("ActorId", *m_other);
			}

			m_bHasLuaEventData = true;
		};

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the trigger area.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetTriggerId() const { return (m_triggerID); };

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the actor.
		//
		// /////////////////////////////////////////////////////////////////
		boost::optional<ActorId> GetActorId() const { return (m_other); };

	private:

		LuaPlus::LuaObject m_LuaEventData;					///< LUA object to link to scripts.
		I32 m_triggerID;							///< ID of the trigger area.
		boost::optional<ActorId> m_other;			///< The ID of the game object who left the trigger area.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_PhysCollision
	// @author Mike McShaffry & PJ O Halloran
	//
	// Event triggered by the physics system when two game objects
	// collide.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_PhysCollision : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event ID.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType & VGetEventType( void ) const { return (sk_EventType); };

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param actorA ID of the first actor.
		// @param actorB ID of the second actor.
		// @param sumNormalForce Sum of the normal forces produced from the
		//							collision.
		// @param sumFrictionForce Sum of the friction forces produced from
		//							the collision.
		// @param collisionPoints List of points involved in the collision.
		// @param sumImpulseForce Sum of the impulse forces to all contact
		//							points invloved in the collision.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_PhysCollision(const ActorId actorA, const ActorId actorB, const Vector3 &sumNormalForce,\
			const Vector3 &sumFrictionForce, const Point3List &collisionPoints, const F32 sumImpulseForce) : m_ActorA(actorA), m_ActorB(actorB),\
				m_SumNormalForce(sumNormalForce), m_SumFrictionForce(sumFrictionForce), m_CollisionPoints(collisionPoints), m_sumImpulseForce(sumImpulseForce)
		{ };

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const
		{
			return (IEventDataPtr(GCC_NEW EvtData_PhysCollision(m_ActorA, m_ActorB, m_SumNormalForce, m_SumFrictionForce, m_CollisionPoints, m_sumImpulseForce)));
		};

		// /////////////////////////////////////////////////////////////////
		// Get the LUA event data object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert((true == m_bHasLuaEventData) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!");
			return (m_LuaEventData);
		};

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

			//Now provide the event data necessary.
			m_LuaEventData.SetInteger("ActorA", m_ActorA);
			m_LuaEventData.SetInteger("ActorB", m_ActorB);

			LuaPlus::LuaObject normalForceObj = m_LuaEventData.CreateTable("NormalForce");
			normalForceObj.SetNumber(1, m_SumNormalForce.GetX());
			normalForceObj.SetNumber(2, m_SumNormalForce.GetY());
			normalForceObj.SetNumber(3, m_SumNormalForce.GetZ());

			LuaPlus::LuaObject frictionForceObj = m_LuaEventData.CreateTable("FrictionForce");
			frictionForceObj.SetNumber(1, m_SumFrictionForce.GetX());
			frictionForceObj.SetNumber(2, m_SumFrictionForce.GetY());
			frictionForceObj.SetNumber(3, m_SumFrictionForce.GetZ());

			m_LuaEventData.SetNumber("ImpulseForce", m_sumImpulseForce);

			// TODO: Determine if i need to include the collision points in the LUA object??

			m_bHasLuaEventData = true;
		};

		// /////////////////////////////////////////////////////////////////
		// Get the first actors ID.
		//
		// /////////////////////////////////////////////////////////////////
		ActorId GetActorAId() const { return (m_ActorA); };

		// /////////////////////////////////////////////////////////////////
		// Get the second actors ID.
		//
		// /////////////////////////////////////////////////////////////////
		ActorId GetActorBId() const { return (m_ActorB); };

		// /////////////////////////////////////////////////////////////////
		// Get the sum of the normal forces produced by the collision
		// event.
		//
		// /////////////////////////////////////////////////////////////////
		Vector3 GetSumNormalForce() const { return (m_SumNormalForce); };

		// /////////////////////////////////////////////////////////////////
		// Get the sum of the friction forces produced by the collision
		// event.
		//
		// /////////////////////////////////////////////////////////////////
		Vector3 GetSumFrictionForce() const { return (m_SumNormalForce); };

		// /////////////////////////////////////////////////////////////////
		// Get a list of points involved in the collision.
		//
		// /////////////////////////////////////////////////////////////////
		void GetCollisionPointsList(Point3List &list) { std::copy(m_CollisionPoints.begin(), m_CollisionPoints.end(), list.begin()); };

		// /////////////////////////////////////////////////////////////////
		// Get the sum of all impulse forces applied to all contact points
		// at the time of the detected collision.
		//
		// /////////////////////////////////////////////////////////////////
		F32 GetSumImpulseForce() const { return (m_sumImpulseForce); };

	private:

		LuaPlus::LuaObject m_LuaEventData;	///< The link to the LUA scripts.
		ActorId m_ActorA;					///< ID of actor A.
		ActorId m_ActorB;					///< ID of actor B.
		Vector3 m_SumNormalForce;			///< The sum of the normal forces produced from the collision.
		Vector3 m_SumFrictionForce;			///< The sum of the friction forces produced from the collision.
		Point3List m_CollisionPoints;		///< List of collision points.
		F32 m_sumImpulseForce;			///< Sum of all impulse forces applied to all contact points at the time of the detected collision.
	};

	// /////////////////////////////////////////////////////////////////
	// @class EvtData_PhysSeparation
	// @author Mike McShaffry & PJ O Halloran
	//
	// Event triggered by the physics system when two game objects
	// seperate after a collision.
	//
	// /////////////////////////////////////////////////////////////////
	class EvtData_PhysSeparation : public BaseEventData
	{
	public:

		static const EventType sk_EventType;

		// /////////////////////////////////////////////////////////////////
		// Get the event ID.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const EventType &VGetEventType(void) const { return (sk_EventType); };

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param actorA The ID of the first actor.
		// @param actorB The ID of the second actor.
		//
		// /////////////////////////////////////////////////////////////////
		explicit EvtData_PhysSeparation(const ActorId actorA, const ActorId actorB) : m_ActorA(actorA), m_ActorB(actorB){ };

		// /////////////////////////////////////////////////////////////////
		// Make a copy of the event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual IEventDataPtr VCopy() const { return (IEventDataPtr(GCC_NEW EvtData_PhysSeparation(m_ActorA, m_ActorB))); };

		// /////////////////////////////////////////////////////////////////
		// Get the LUA data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaEventData(void) const
		{
			assert((true == m_bHasLuaEventData) && "Can't get lua event data because it hasn't been built yet!  Call BulidLuaEventData() first!");
			return (m_LuaEventData);
		};

		// /////////////////////////////////////////////////////////////////
		// Build the LUA event data object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VBuildLuaEventData(void)
		{
			assert((false == m_bHasLuaEventData) && "Already built lua event data!");

			//Get the global state.
			LuaPlus::LuaState * pState = g_appPtr->GetLuaStateManager()->GetGlobalState().Get();
			m_LuaEventData.AssignNewTable(pState);

			// Now provide the event data necessary.
			m_LuaEventData.SetInteger("ActorA", m_ActorA);
			m_LuaEventData.SetInteger("ActorB", m_ActorB);

			m_bHasLuaEventData = true;
		};

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the first actor.
		//
		// /////////////////////////////////////////////////////////////////
		ActorId GetActorA() const { return (m_ActorA); };

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the second actor.
		//
		// /////////////////////////////////////////////////////////////////
		ActorId GetActorB() const { return (m_ActorB); };

	private:

		LuaPlus::LuaObject m_LuaEventData;		///< LUA script link object.
		ActorId m_ActorA;				///< The ID of the first actor.
		ActorId m_ActorB;				///< The ID of the second actor.
	};

}

#endif
