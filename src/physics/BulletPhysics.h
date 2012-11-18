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
// @file BulletPhysics.h
// @author Mike McShaffry & PJ O Halloran
// @date 14/10/2010
//
// Header file for the games Bullet API physics class.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GF_BULLET_PHYSICS_H
#define __GF_BULLET_PHYSICS_H

// External Headers
#include <cstring>

#include <vector>
#include <map>
#include <iterator>
#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include <btBulletDynamicsCommon.h>


// Project Headers
#include "IGamePhysics.h"
#include "BulletPhysicsDebugDrawer.h"
#include "ModelViewProjStackManager.h"
#include "Triangle.h"
#include "GLTriangleBatch.h"


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// makes life easier when trying out different constraints
	// /////////////////////////////////////////////////////////////////
	//typedef btSliderConstraint RotatorConstraint;
	typedef btHingeConstraint RotatorConstraint;

	// /////////////////////////////////////////////////////////////////
	// ***** My Math lib to Bullet LinearMath Conversion Functions *****
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	// Convert from Vector3 to btVector3.
	//
	// @param myVec The Vector3 object to convert.
	// @param bulletVec The Bullet Vector3.
	//
	// /////////////////////////////////////////////////////////////////
	inline void Vector3TobtVector3(const Vector3 &myVec, btVector3 &bulletVec)
	{
		bulletVec.setValue(myVec.GetX(), myVec.GetY(), myVec.GetZ());
	};

	// /////////////////////////////////////////////////////////////////
	// Convert from btVector3 to Vector3.
	//
	// @param bulletVec The Bullet Vector3 to convert.
	// @param myVec The Vector3 object.
	//
	// /////////////////////////////////////////////////////////////////
	inline void btVector3ToVector3(const btVector3 &bulletVec, Vector3 &myVec)
	{
		myVec.Set(bulletVec.x(), bulletVec.y(), bulletVec.z());
	};

	// /////////////////////////////////////////////////////////////////
	// Convert from Point3 to btVector3.
	//
	// @param myPt The Point3 object to convert.
	// @param bulletPt The Bullet Vector3.
	//
	// /////////////////////////////////////////////////////////////////
	inline void Point3TobtVector3(const Point3 &myPt, btVector3 &bulletPt)
	{
		bulletPt.setValue(myPt.GetX(), myPt.GetY(), myPt.GetZ());
	};

	// /////////////////////////////////////////////////////////////////
	// Convert from btVector3 to Point3.
	//
	// @param bulletVec The Bullet Vector3 to convert.
	// @param myVec The Point3 object.
	//
	// /////////////////////////////////////////////////////////////////
	inline void btVector3ToPoint3(const btVector3 &bulletPt, Point3 &myPt)
	{
		myPt.Set(bulletPt.x(), bulletPt.y(), bulletPt.z());
	};

	// /////////////////////////////////////////////////////////////////
	// Convert a Matrix4 to a Bullet type 4x4 matrix, btTransform.
	// Both libraries store matrices in column major format so the
	// conversion is fairly straightforward.
	//
	// @param myMatrix The Matrix4 object to convert.
	// @param bulletMatrix The Bullet 4x4 matrix object.
	//
	// /////////////////////////////////////////////////////////////////
	inline void Matrix4TobtTransform(const Matrix4 &myMatrix, btTransform &bulletMatrix)
	{
		btScalar arr[15];
		memcpy(arr, myMatrix.GetComponentsConst(), 15 * sizeof(F32));
		bulletMatrix.setFromOpenGLMatrix(arr);
	};

	// /////////////////////////////////////////////////////////////////
	// Convert a Bullet type 4x4 matrix, btTransform to my own Matrix4
	// class. Both libraries store matrices in column major format so the
	// conversion is fairly straightforward.
	//
	// @param bulletMatrix The Bullet 4x4 matrix object to convert.
	// @param myMatrix The Matrix4 object.
	//
	// /////////////////////////////////////////////////////////////////
	inline void btTransformToMatrix4(const btTransform &bulletMatrix, Matrix4 &myMatrix)
	{
		btScalar arr[16];
		bulletMatrix.getOpenGLMatrix(arr);
		arr[15] = 1.0f;
		myMatrix.Set(arr);
	};

	// /////////////////////////////////////////////////////////////////
	// ***** Other Useful Bullet to gameframework engine conversion utility functions *****
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	// Convert a gameframework triangle mesh to a Bullet triangle mesh
	// shape object.  This mesh shape object may be used to represent
	// instances of that shape in the physics game world simulation.
	//
	// N.B.
	// In this case btTriangleMesh has its own copy of the mesh data
	// so you may safely clean up the geometrical mesh and still have a
	// physics mesh shape.
	//
	// N.B.
	// As it creates its own copy it runs in O(n) time.
	//
	// @param myMesh Gameframework mesh data structure.
	// @param bulletMesh Bullet library mesh object.
	//
	// @return N/A.
	//
	// /////////////////////////////////////////////////////////////////
	void GfTriangleMeshTobtTriangleMesh(const TriangleMesh &myMesh, btTriangleMesh &bulletMesh);

	// /////////////////////////////////////////////////////////////////
	// Convert a gameframework triangle batch stored in an OpenGL VBO 
	// to a Bullet triangle mesh shape object.  This mesh shape object
	// may be used to represent instances of that shape in the physics
	// game world simulation.
	//
	// N.B.
	// btTriangleMesh refers to the data in the GLTriangleBatch so the
	// batch must be kept in memory for the lifetime of the physics shape.
	//
	// N.B.
	// Runs in constant time.
	//
	// @param myBatch Gameframework mesh data structure.
	// @param bulletMesh Bullet library mesh object.
	//
	// @return N/A.
	//
	// /////////////////////////////////////////////////////////////////
	void GfTriangleBatchTobtTriangleMesh(const GLTriangleBatch &myBatch, btTriangleMesh &bulletMesh);

	// /////////////////////////////////////////////////////////////////
	// @class BulletPhysics
	// @author Mike McShaffry & PJ O Halloran
	//
	// Implementation of the game physics interface using the Bullet API.
	//
	// /////////////////////////////////////////////////////////////////
	class BulletPhysics : public IGamePhysics
	{
	private:

		// /////////////////////////////////////////////////////////////////
		// @class ActorMotionState
		// @author Mike McShaffry & PJ O Halloran
		//
		// Interface that Bullet uses to communicate position and orientation
		// changes back to the game.
		//
		// Note:
		// This assumes that the actor's center of mass and world position
		// are the same point.  If that was not the case, an additional
		// transformation would need to be stored here to represent that
		// difference.
		//
		// Note:
		// Members are public here because the class is private and known
		// only to the BulletPhysics class!
		//
		// /////////////////////////////////////////////////////////////////
		class ActorMotionState : public btMotionState
		{
		public:

			Matrix4 m_worldToPositionTransform;					///< The actors position and orientation info.
			
			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// @param startingTransform The starting position and orientation of
			//							the game object.
			//
			// /////////////////////////////////////////////////////////////////
			inline explicit ActorMotionState(const Matrix4 &startingTransform) : m_worldToPositionTransform(startingTransform) { };
			
			// /////////////////////////////////////////////////////////////////
			// btMotionState interface:  Bullet calls these
			// /////////////////////////////////////////////////////////////////
			inline virtual void getWorldTransform(btTransform &worldTrans) const
			{
				Matrix4TobtTransform(m_worldToPositionTransform, worldTrans);
			};

			// /////////////////////////////////////////////////////////////////
			// btMotionState interface:  Bullet calls these
			// /////////////////////////////////////////////////////////////////
			inline virtual void setWorldTransform(const btTransform &worldTrans)
			{
				btTransformToMatrix4(worldTrans, m_worldToPositionTransform);
			};
		};

		// /////////////////////////////////////////////////////////////////
		// @class BulletActor
		// @author Mike McShaffry & PJ O Halloran
		//
		// Represents a BulletActor and all its associated data internally
		// for BulletPhysics.
		//
		// Note:
		// Members are public here because the class is private and known
		// only to the BulletPhysics class!
		//
		// /////////////////////////////////////////////////////////////////
		class BulletActor
		{
		public:

			btRigidBody *m_rigidBodyPtr;		///< The rigid body associted with this actor (should never be NULL)
			F32 m_desiredDeltaYAngle;			///< The desired Y orientation (to change to over time).
			F32 m_desiredDeltaYAngleTime;		///< The desired timeframe (seconds) to change the orientation.

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// @param rigidBodPtr The pointer to the Bullet rigid body input into
			//						the physics world.
			//
			// /////////////////////////////////////////////////////////////////
			inline explicit BulletActor(btRigidBody *rigidBodyPtr)\
				: m_rigidBodyPtr(rigidBodyPtr), m_desiredDeltaYAngle(0.0f), m_desiredDeltaYAngleTime(0.0f) { };
		};
		
		// these are all of the objects that Bullet uses to do its work.
		//   see BulletPhysics::VInitialize() for some more info.
		boost::shared_ptr<btDynamicsWorld> m_dynamicsWorld;
		boost::shared_ptr<btBroadphaseInterface> m_broadphase;
		boost::shared_ptr<btCollisionDispatcher> m_dispatcher;
		boost::shared_ptr<btConstraintSolver> m_solver;
		boost::shared_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
		boost::shared_ptr<BulletPhysicsDebugDrawer> m_debugDrawer;
		
		// keep track of the existing rigid bodies:  To check them for updates
		//   to the actors' positions, and to remove them when their lives are over.
		typedef std::map<ActorId, boost::shared_ptr<BulletActor> > ActorIDToBulletActorMap;
		ActorIDToBulletActorMap m_actorBodies;
		
		// also keep a map to get the actor id from the btRigidBody*
		typedef std::map<btRigidBody const *, ActorId> RigidBodyToActorIDMap;
		RigidBodyToActorIDMap m_rigidBodyToActorId;
		
		// data used to store which collision pair (bodies that are touching) need
		//   Collision events sent.  When a new pair of touching bodies are detected,
		//   they are added to m_previousTickCollisionPairs and an event is sent.
		//   When the pair is no longer detected, they are removed and another event
		//   is sent.
		typedef std::pair<btRigidBody const *, btRigidBody const *> CollisionPair;
		typedef std::set<CollisionPair> CollisionPairs;
		CollisionPairs m_previousTickCollisionPairs;

		// The MVP stack manager to pass onto the Debug Drawer class in VInitialize() so
		//  that it knows the current mvp matrix for rendering debug info!
		boost::shared_ptr<ModelViewProjStackManager> m_mvpStackManagerPtr;

		typedef std::map<ActorId, btStridingMeshInterface const *> BulletMeshMap;
		BulletMeshMap m_meshMap;
		
		// /////////////////////////////////////////////////////////////////
		// helpers for sending events relating to collision pairs
		//
		// /////////////////////////////////////////////////////////////////
		void SendCollisionPairAddEvent(btPersistentManifold const *manifoldPtr, btRigidBody const *body0Ptr, btRigidBody const *body1Ptr);

		// /////////////////////////////////////////////////////////////////
		// helpers for sending events relating to collision pairs
		//
		// /////////////////////////////////////////////////////////////////
		void SendCollisionPairRemoveEvent(btRigidBody const *body0Ptr, btRigidBody const *body1Ptr);
		
		// /////////////////////////////////////////////////////////////////
		// helper for cleaning up objects
		//
		// /////////////////////////////////////////////////////////////////
		void RemoveCollisionObject(btCollisionObject *removeMePtr);
		
		// /////////////////////////////////////////////////////////////////
		// find the rigid body associated with the given actor ID
		//
		// /////////////////////////////////////////////////////////////////
		btRigidBody *FindActorBody(const ActorId id) const;
		
		// /////////////////////////////////////////////////////////////////
		// find the BulletActor object with the given actor ID
		//
		// /////////////////////////////////////////////////////////////////
		boost::shared_ptr<BulletPhysics::BulletActor> FindBulletActor(const ActorId id) const;
		
		// /////////////////////////////////////////////////////////////////
		// find the actor ID associated with the given body
		//
		// /////////////////////////////////////////////////////////////////
		boost::optional<ActorId> FindActorID(btRigidBody const *bodyPtr) const;

		// /////////////////////////////////////////////////////////////////
		// Find the mesh shape associated with the physics object created
		// for the actor identified.
		//
		// @param aid The actor ID.
		//
		// @return btStridingMeshInterface const * Pointer to bullet mesh
		//											linked to actor or NULL
		//											if the actor has no mesh.
		//
		// /////////////////////////////////////////////////////////////////
		btStridingMeshInterface const *FindMeshShape(const ActorId aid) const;

		// /////////////////////////////////////////////////////////////////
		// Get the API independant physics object type from the bullet
		// object.
		//
		// @param bodyPtr Bullet physics object.
		//
		// @return PhysicsObjectType The physics object type.
		//
		// /////////////////////////////////////////////////////////////////
		PhysicsObjectType GetPhysicsObjectType(const btCollisionObject *bodyPtr) const;

		// /////////////////////////////////////////////////////////////////
		// Get the API independant physics body type from the bullet
		// object.
		//
		// @param bodyPtr Bullet physics object.
		//
		// @return PhysicsBodyType The physics body type.
		//
		// /////////////////////////////////////////////////////////////////
		PhysicsBodyType GetPhysicsBodyType(const btCollisionObject *bodyPtr) const;
		
		// /////////////////////////////////////////////////////////////////
		// callback from bullet for each physics time step.  set in VInitialize
		//
		// /////////////////////////////////////////////////////////////////
		static void BulletInternalTickCallback(btDynamicsWorld * const world, const btScalar timeStep);

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		BulletPhysics(boost::shared_ptr<ModelViewProjStackManager> mvpStackManagerPtr);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~BulletPhysics();

		// /////////////////////////////////////////////////////////////////
		// Initialiazation of physics world.
		//
		// @return bool True on success or false on failure.
		// 
		// /////////////////////////////////////////////////////////////////
		virtual bool VInitialize();

		// /////////////////////////////////////////////////////////////////
		// Propagate reported physics changes throughout the system using
		// the game event framework.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSyncVisibleScene();

		// /////////////////////////////////////////////////////////////////
		// Update the physics world.
		//
		// @param deltaSeconds The number of seconds since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnUpdate(const F32 deltaSeconds); 

		// /////////////////////////////////////////////////////////////////
		// Adds a btRigidBody to the physics simulation linked to the game
		// actor supplied.
		//
		// @param shapePtr Pointer to the game shape for the bullet object.
		// @param physicsObject Information describing the physics object
		//
		// /////////////////////////////////////////////////////////////////
		void AddGameActorRigidBody(btCollisionShape *shapePtr, const struct PhysicsObjectAttributes &physicsObject);

		// /////////////////////////////////////////////////////////////////
		// Add a sphere object to the physics world.
		//
		// @param radius The radius of the sphere.
		// @param specificGravity The gravity for the object.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddSphere(const F32 radius, const F32 specificGravity, struct PhysicsObjectAttributes &physicsObjectAtt);

		// /////////////////////////////////////////////////////////////////
		// Add a box object to the physics world.
		//
		// @param dimensions The dimensions of the box.
		// @param specificGravity The gravity for the object.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddBox(const Vector3 &dimensions, const F32 specificGravity, struct PhysicsObjectAttributes &physicsObjectAtt);

		// /////////////////////////////////////////////////////////////////
		// Add a cylinder object to the physics world.
		//
		// @param dimensions The dimensions of the box.
		// @param specificGravity The gravity for the object.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddCylinder(const Vector3 &dimensions, const F32 specificGravity, struct PhysicsObjectAttributes &physicsObjectAtt);

		// /////////////////////////////////////////////////////////////////
		// Add a point cloud for a convex hull shape to the physics world.
		//
		// @param vertVec The vertices of the point cloud.
		// @param specificGravity The gravity for the object.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddPointCloud(const Point3List &vertVec, const F32 specificGravity, struct PhysicsObjectAttributes &physicsObjectAtt);

		// /////////////////////////////////////////////////////////////////
		// Add a static triangle mesh object (0 mass) to the physics world.
		// 
		// @param mesh The triangle mesh.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddStaticMesh(const TriangleMesh &mesh, struct PhysicsObjectAttributes &physicsObjectAtt);

		// /////////////////////////////////////////////////////////////////
		// Add a static triangle mesh object (0 mass) to the physics world.
		// 
		// @param batch The OpenGL rendering triangle batch.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddStaticMesh(const GLTriangleBatch &batch, struct PhysicsObjectAttributes &physicsObjectAtt);

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
		virtual bool VGetObjectStatus(const PhysicsObjectId id, struct PhysicsObjectAttributes &status) const;

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
		virtual bool VGetActorObjectStatus(const ActorId aid, struct PhysicsObjectAttributes &status) const;

		// /////////////////////////////////////////////////////////////////
		// Remove an object from the physics system associated with the game
		// actor.
		//
		// @param id The ID of the game actor to remove.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRemoveActor(const ActorId id);

		// /////////////////////////////////////////////////////////////////
		// Debugging.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRenderDiagnostics();

		// /////////////////////////////////////////////////////////////////
		// Get the current diagnostic mode.
		//
		// /////////////////////////////////////////////////////////////////
		virtual I32 VGetDiagnosticMode() const;

		// /////////////////////////////////////////////////////////////////
		// Set the current diagnostic mode.
		//
		// @param mode New diagnostic mode.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetDiagnosticMode(const I32 mode);

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
		virtual void VCreateTrigger(const Point3 &pos, const F32 dim, const I32 triggerID);

		// /////////////////////////////////////////////////////////////////
		// Physics world modifiers.
		//
		// Remove a trigger area.
		//
		// @param triggerId The ID of the trigger point.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRemoveTrigger(const I32 triggerId);

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
		virtual void VApplyForce(const Vector3 &dir, const F32 newtons, const ActorId aid);

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
		virtual void VApplyTorque(const Vector3 &dir, const F32 newtons, const ActorId aid);

		// /////////////////////////////////////////////////////////////////
		// Physics world modifiers.
		//
		// Move a static (kinematic) game object.
		//
		// @param mat The objects new position and orientation.
		// @param aid The ID of the game object.
		//
		// @return bool 
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VKinematicMove(const Matrix4 &mat, const ActorId aid);
		
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
		virtual void VRotateY(const ActorId actorId, const F32 angle, const F32 time);

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
		virtual F32 VGetOrientationY(const ActorId actorId);

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
		virtual void VStopActor(const ActorId actorId);

		// /////////////////////////////////////////////////////////////////
		// Physics actor states.
		// (Useful for example, the AI system to tell the physics system to
		// alter a game objects state).
		//
		// @param actorId The ID of the game object.
		// @param vel The new velocity of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetVelocity(const ActorId actorId, const Vector3 &vel);

		// /////////////////////////////////////////////////////////////////
		// Physics actor states.
		// (Useful for example, the AI system to tell the physics system to
		// alter a game objects state).
		//
		// @param actorId The ID of the actor.
		// @param vec The vector to translate the object by.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VTranslate(const ActorId actorId, const Vector3 &vec);
	};

}

#endif
