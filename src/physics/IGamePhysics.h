#pragma once
#ifndef __GF_I_GAME_PHYSICS_H
#define __GF_I_GAME_PHYSICS_H

//========================================================================
// IGamePhysics.h -  Physics class interface.
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
// @file IGamePhysics.h
// @author Michael L. McShaffry (edited by PJ O Halloran)
// @date 09/07/2010
//
// File contains the header for the physics interface.
// 
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry, author of Game
// Coding Complete and games programmer.
// 
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// I have added/modified the following functionality:
// - Modified the interface to use my own custom 3D math library.
//
// /////////////////////////////////////////////////////////////////

#include <vector>

#include <boost/shared_ptr.hpp>

#include "IActors.h"
#include "Vector.h"
#include "Matrix.h"
#include "Triangle.h"
#include "GLTriangleBatch.h"
#include "PhysicsCommon.h"

namespace GameHalloran
{

	typedef U32 PhysicsObjectId;

	// /////////////////////////////////////////////////////////////////
	// @enum ActorPhysicsBodyType
	// @author PJ O Halloran
	//
	// General physics body types for our interface.
	//
	// eCollisionBody:		Basic collision body.
	// eRigidBody:			Collision body with rigid body dynamics applied.
	// eSoftBody:			Collision body with soft body dynamics applied.
	// eDummyBody:			Some physics engines use proxy types for
	//						different reasons (Bullet has GhostType, etc.).
	//
	// /////////////////////////////////////////////////////////////////
	enum PhysicsBodyType
	{
		eCollisionBody = 0,
		eRigidBody,
		eSoftBody,
		eDummyBody,
		eNumberBodyTypes,
		eInvalidBody
	};

	// /////////////////////////////////////////////////////////////////
	// @enum ActorPhysicsObjectType
	// @author PJ O Halloran
	//
	// General physics body *object* types for our interface.
	//
	// eStatic:			Non moving body with 0 mass.
	// eDynamic:		Moving body.
	// eKinematic:		User controlled moving body.
	//
	// /////////////////////////////////////////////////////////////////
	enum PhysicsObjectType
	{
		eStatic = 0,
		eDynamic,
		eKinematic,
		eNumberObjectTypes,
		eInvalidObject
	};

	// /////////////////////////////////////////////////////////////////
	// @struct PhysicsObjectAttributes
	// @author PJ O Halloran
	//
	// Universal struct to hold general information about physics objects
	// used to query or create physics objects in an API independant
	// manner.
	//
	// Note: A physics object may or may not be related to a Game actor.
	//
	// Note: I have not experimented much with soft body dynamics yet so
	// we *might* add soft body information to this struct later.
	//
	// /////////////////////////////////////////////////////////////////
	struct PhysicsObjectAttributes
	{
		// General parameters.
		PhysicsObjectId m_id;						///< The object Id (assigned by the physics system, never supplied by the programmer).
		enum PhysicsBodyType m_bodyType;			///< Body type.
		enum PhysicsObjectType m_objectType;		///< Object type.
		boost::optional<ActorId> m_actorId;			///< Game actor ID (link to the games logic layer actors).
		bool m_active;								///< Is it currently active/asleep.
		F32 m_friction;								///< Friction parameter.
		F32 m_restitution;							///< Restitution parameter.
		I32 m_collisionGroup;						///< Collision group object belongs to (for high level collision filtering).
		I32 m_collisionMask;						///< Collision group object interacts with (for high level collision filtering).
		Matrix4 m_objMatrix;						///< The orientation and position of the object.

		// Parameters for RigidBody types only.
		Vector3 m_linearVelocity;					///< Linear velocity.
		F32 m_linearDamping;						///< Linear damping.
		Vector3 m_angularVelocity;					///< Angular velocity.
		F32 m_angularDamping;						///< Angular damping.
		F32 m_mass;									///< Body mass.
		Vector3 m_inertia;							///< The bodies inertia tensor.

		// Parameters for SoftBody types only (TODO: Add these later...).

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		PhysicsObjectAttributes() : m_id(0), m_bodyType(eInvalidBody), m_objectType(eInvalidObject), m_actorId(), m_active(false), m_friction(0.0f), m_restitution(0.0f),\
			m_collisionGroup(0), m_collisionMask(0), m_objMatrix(), m_linearVelocity(), m_linearDamping(0.0f), m_angularVelocity(), m_angularDamping(0.0f), m_mass(0.0f), m_inertia() {};

		// /////////////////////////////////////////////////////////////////
		// Is the object currently moving in the simulation.
		//
		// /////////////////////////////////////////////////////////////////
		bool IsMoving() const
		{
			bool moving = false;
			if(m_objectType != eStatic && (m_linearVelocity.MagnitudeSqr() > 0.01f || m_angularVelocity.MagnitudeSqr() > 0.01f))
			{
				moving = true;
			}
			return (moving);
		};

		// /////////////////////////////////////////////////////////////////
		// Is the object currently active or moving in the simulation.
		//
		// ("Active" means if the physics simulation is currently including
		// the object in the solvers run - non moving objects are set as
		// inactive to save CPU time)
		//
		// /////////////////////////////////////////////////////////////////
		bool IsActiveOrMoving() const
		{
			return (m_active || IsMoving());
		};
	};

	// /////////////////////////////////////////////////////////////////
	// @class IGamePhysics
	// @author Michael L. McShaffry (edited by PJ O Halloran).
	//
	// This is a class is the base interface that all physics 
	// classes must implement.
	//
	// /////////////////////////////////////////////////////////////////
	class IGamePhysics
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~IGamePhysics() { };

		// /////////////////////////////////////////////////////////////////
		// Initialiazation of physics world.
		//
		// @return bool True on success or false on failure.
		// 
		// /////////////////////////////////////////////////////////////////
		virtual bool VInitialize() = 0;

		// /////////////////////////////////////////////////////////////////
		// Propagate reported physics changes throughout the system using
		// the game event framework.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSyncVisibleScene() = 0;

		// /////////////////////////////////////////////////////////////////
		// Update the physics world.
		//
		// @param deltaSeconds The number of seconds since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnUpdate(const F32 deltaSeconds) = 0; 

		// /////////////////////////////////////////////////////////////////
		// Add a sphere object to the physics world.
		//
		// @param radius The radius of the sphere.
		// @param specificGravity The gravity for the object.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddSphere(const F32 radius, const F32 specificGravity, struct PhysicsObjectAttributes &physicsObjectAtt) = 0;

		// /////////////////////////////////////////////////////////////////
		// Add a box object to the physics world.
		//
		// @param dimensions The dimensions of the box.
		// @param specificGravity The gravity for the object.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddBox(const Vector3 &dimensions, const F32 specificGravity, struct PhysicsObjectAttributes &physicsObjectAtt) = 0;

		// /////////////////////////////////////////////////////////////////
		// Add a cylinder object to the physics world.
		//
		// @param dimensions The dimensions of the box.
		// @param specificGravity The gravity for the object.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddCylinder(const Vector3 &dimensions, const F32 specificGravity, struct PhysicsObjectAttributes &physicsObjectAtt) = 0;

		// /////////////////////////////////////////////////////////////////
		// Add a point cloud for a convex hull shape to the physics world.
		//
		// @param vertVec The vertices of the point cloud.
		// @param specificGravity The gravity for the object.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddPointCloud(const Point3List &vertVec, const F32 specificGravity, struct PhysicsObjectAttributes &physicsObjectAtt) = 0;

		// /////////////////////////////////////////////////////////////////
		// Add a static triangle mesh object (0 mass) to the physics world.
		// 
		// @param mesh The triangle mesh.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddStaticMesh(const TriangleMesh &mesh, struct PhysicsObjectAttributes &physicsObjectAtt) = 0;

		// /////////////////////////////////////////////////////////////////
		// Add a static triangle mesh object (0 mass) to the physics world.
		// 
		// @param batch The OpenGL rendering triangle batch.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddStaticMesh(const GLTriangleBatch &batch, struct PhysicsObjectAttributes &physicsObjectAtt) = 0;

		// /////////////////////////////////////////////////////////////////
		// Get the current status of a physics object in the simulation.
		//
		// @param id The physics object Id.
		// @param status Physics object information.
		//
		// @return bool True if an object is in the simulation with an ID
		//				matching "id".
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VGetObjectStatus(const PhysicsObjectId id, struct PhysicsObjectAttributes &status) const = 0;

		// /////////////////////////////////////////////////////////////////
		// Get the current status of a game actors related physics object
		// in the simulation.
		//
		// @param aid The game actors Id.
		// @param status The struct holding the game actors information (if it
		//					exists).
		//
		// @return bool True if there was an actor with the id "aid" in the
		//				physics simulation and false if not.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VGetActorObjectStatus(const ActorId aid, struct PhysicsObjectAttributes &status) const = 0;

		// /////////////////////////////////////////////////////////////////
		// Remove an object from the physics system associated with the game
		// actor.
		//
		// @param id The ID of the game actor to remove.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRemoveActor(const ActorId id) = 0;

		// /////////////////////////////////////////////////////////////////
		// Debugging.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRenderDiagnostics() = 0;

		// /////////////////////////////////////////////////////////////////
		// Get the current diagnostic mode.
		//
		// /////////////////////////////////////////////////////////////////
		virtual I32 VGetDiagnosticMode() const = 0;

		// /////////////////////////////////////////////////////////////////
		// Set the current diagnostic mode.
		//
		// @param mode New diagnostic mode.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetDiagnosticMode(const I32 mode) = 0;

		// /////////////////////////////////////////////////////////////////
		// Physics world modifiers.
		//
		// Create a trigger area which will generate events when game objects
		// strike it.
		//
		// @param pos The position of the trigger point in world space.
		// @param dim The dimensions of the trigger area (box shaped).
		// @param triggerId The ID of the trigger point to identify when an
		//					object has collided/entered a trigger area.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VCreateTrigger(const Point3 &pos, const F32 dim, const I32 triggerID) = 0;

		// /////////////////////////////////////////////////////////////////
		// Physics world modifiers.
		//
		// Remove a trigger area.
		//
		// @param triggerId The ID of the trigger point.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRemoveTrigger(const int triggerId) = 0;

		// /////////////////////////////////////////////////////////////////
		// Physics world modifiers.
		//
		// Apply Force to a game object.  i.e. Push an object off in a
		// certain direction.
		//
		// @param dir The direction to send the game object off in.
		// @param newtons The amount of force in newtons to apply to the
		//					object.
		// @param aid The ID of the actor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VApplyForce(const Vector3 &dir, const F32 newtons, const ActorId aid) = 0;

		// /////////////////////////////////////////////////////////////////
		// Physics world modifiers.
		//
		// Apply torque to a game object.  i.e. Send an object off in a spin.
		//
		// @param dir The direction to send the game object off in.
		// @param newtons The amount of force in newtons to apply to the
		//					object.
		// @param aid The ID of the actor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VApplyTorque(const Vector3 &dir, const F32 newtons, const ActorId aid) = 0;

		// /////////////////////////////////////////////////////////////////
		// Physics world modifiers.
		//
		// Move a kinematic game object.
		//
		// @param mat The objects new position and orientation.
		// @param aid The ID of the game object.
		//
		// @return bool 
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VKinematicMove(const Matrix4 &mat, const ActorId aid) = 0;
		
		// /////////////////////////////////////////////////////////////////
		// Physics actor states.
		// (Useful for example, the AI system to tell the physics system to
		// alter a game objects state).
		//
		// Rotate a game object around the Y axis for "time" seconds
		//
		// @param actorId The ID of the game object.
		// @param angle The angle to rotate by in radians.
		// @param time The number of seconds to rotate for.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRotateY(const ActorId actorId, const F32 angle, const F32 time) = 0;

		// /////////////////////////////////////////////////////////////////
		// Physics actor states.
		// (Useful for example, the AI system to tell the physics system to
		// alter a game objects state).
		// 
		// Get the current Y axis orientation of the game object.
		//
		// @param actorId The ID of the game object.
		//
		// @return F32 The orientation of the game actor in the physics
		//					system.
		//
		// /////////////////////////////////////////////////////////////////
		virtual F32 VGetOrientationY(const ActorId actorId) = 0;

		// /////////////////////////////////////////////////////////////////
		// Physics actor states.
		// (Useful for example, the AI system to tell the physics system to
		// alter a game objects state).
		//
		// Stop an actor where it is.
		//
		// @param actorId The ID of the game object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VStopActor(const ActorId actorId) = 0;

		// /////////////////////////////////////////////////////////////////
		// Physics actor states.
		// (Useful for example, the AI system to tell the physics system to
		// alter a game objects state).
		//
		// @param actorId The ID of the game object.
		// @param vel The new velocity of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetVelocity(const ActorId actorId, const Vector3 &vel) = 0;

		// /////////////////////////////////////////////////////////////////
		// Physics actor states.
		// (Useful for example, the AI system to tell the physics system to
		// alter a game objects state).
		//
		// @param actorId The ID of the actor.
		// @param vec The vector to translate the object by.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VTranslate(const ActorId actorId, const Vector3 &vec) = 0;
	};
}

#endif
