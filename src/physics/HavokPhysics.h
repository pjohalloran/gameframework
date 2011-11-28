// /////////////////////////////////////////////////////////////////
// @file HavokPhysics.h
// @author PJ O Halloran
// @date 14/10/2010
//
// Header file for the Havok Physics API physics game class.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GF_HAVOK_PHYSICS_H
#define __GF_HAVOK_PHYSICS_H

// External Headers

// Project Headers
#include "IGamePhysics.h"

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class HavokPhysics
	// @author PJ O Halloran
	//
	// Implementation of the physics interface using the Havok physics
	// system.
	//
	// /////////////////////////////////////////////////////////////////
	class HavokPhysics : public IGamePhysics
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		HavokPhysics() { };

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~HavokPhysics() { };

		// /////////////////////////////////////////////////////////////////
		// Initialiazation of physics world.
		//
		// @return bool True on success or false on failure.
		// 
		// /////////////////////////////////////////////////////////////////
		virtual bool VInitialize() { return (true); };

		// /////////////////////////////////////////////////////////////////
		// Propagate reported physics changes throughout the system using
		// the game event framework.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSyncVisibleScene() { };

		// /////////////////////////////////////////////////////////////////
		// Update the physics world.
		//
		// @param deltaSeconds The number of seconds since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnUpdate(const float deltaSeconds) { }; 

		// /////////////////////////////////////////////////////////////////
		// Add a sphere object to the physics world.
		//
		// @param radius The radius of the sphere.
		// @param specificGravity The gravity for the object.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddSphere(const float radius, const float specificGravity, struct PhysicsObjectAttributes &physicsObjectAtt) {};

		// /////////////////////////////////////////////////////////////////
		// Add a box object to the physics world.
		//
		// @param dimensions The dimensions of the box.
		// @param specificGravity The gravity for the object.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddBox(const Vector3 &dimensions, const float specificGravity, struct PhysicsObjectAttributes &physicsObjectAtt) {};

		// /////////////////////////////////////////////////////////////////
		// Add a cylinder object to the physics world.
		//
		// @param dimensions The dimensions of the box.
		// @param specificGravity The gravity for the object.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddCylinder(const Vector3 &dimensions, const float specificGravity, struct PhysicsObjectAttributes &physicsObjectAtt) {};

		// /////////////////////////////////////////////////////////////////
		// Add a point cloud for a convex hull shape to the physics world.
		//
		// @param vertVec The vertices of the point cloud.
		// @param specificGravity The gravity for the object.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddPointCloud(const Point3List &vertVec, const float specificGravity, struct PhysicsObjectAttributes &physicsObjectAtt) {};

		// /////////////////////////////////////////////////////////////////
		// Add a static triangle mesh object (0 mass) to the physics world.
		// 
		// @param mesh The triangle mesh.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddStaticMesh(const TriangleMesh &mesh, struct PhysicsObjectAttributes &physicsObjectAtt) {};

		// /////////////////////////////////////////////////////////////////
		// Add a static triangle mesh object (0 mass) to the physics world.
		// 
		// @param batch The OpenGL rendering triangle batch.
		// @param physicsObjectAtt The physics object attributes of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddStaticMesh(const GLTriangleBatch &batch, struct PhysicsObjectAttributes &physicsObjectAtt) {};

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
		virtual bool VGetObjectStatus(const PhysicsObjectId id, struct PhysicsObjectAttributes &status) const { return (false); };

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
		virtual bool VGetActorObjectStatus(const ActorId aid, struct PhysicsObjectAttributes &status) const { return (false); };

		// /////////////////////////////////////////////////////////////////
		// Remove an object from the physics system associated with the game
		// actor.
		//
		// @param id The ID of the game actor to remove.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRemoveActor(const ActorId id) { };

		// /////////////////////////////////////////////////////////////////
		// Debugging.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRenderDiagnostics() { };

		// /////////////////////////////////////////////////////////////////
		// Get the current diagnostic mode.
		//
		// /////////////////////////////////////////////////////////////////
		virtual I32 VGetDiagnosticMode() const { return (0); };

		// /////////////////////////////////////////////////////////////////
		// Set the current diagnostic mode.
		//
		// @param mode New diagnostic mode.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetDiagnosticMode(const I32 mode) { };

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
		virtual void VCreateTrigger(const Point3 &pos, const float dim, const I32 triggerID) { };

		// /////////////////////////////////////////////////////////////////
		// Physics world modifiers.
		//
		// Remove a trigger area.
		//
		// @param triggerId The ID of the trigger point.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRemoveTrigger(const I32 triggerId) { };

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
		virtual void VApplyForce(const Vector3 &dir, const float newtons, const ActorId aid) { };

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
		virtual void VApplyTorque(const Vector3 &dir, const float newtons, const ActorId aid) { };

		// /////////////////////////////////////////////////////////////////
		// Physics world modifiers.
		//
		// Move a static (kinematic) game object.
		//
		// @param mat The objects new position and orientation.
		// @param aid The ID of the game object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VKinematicMove(const Matrix4 &mat, const ActorId aid) { return (true); };
		
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
		virtual void VRotateY(const ActorId actorId, const float angle, const float time) { };

		// /////////////////////////////////////////////////////////////////
		// Physics actor states.
		// (Useful for example, the AI system to tell the physics system to
		// alter a game objects state).
		// 
		// Get the current Y axis orientation of the game object.
		//
		// @param actorId The ID of the game object.
		//
		// @return float The orientation of the game actor in the physics
		//					system.
		//
		// /////////////////////////////////////////////////////////////////
		virtual float VGetOrientationY(const ActorId actorId) { return (0.0f); };

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
		virtual void VStopActor(const ActorId actorId) { };

		// /////////////////////////////////////////////////////////////////
		// Physics actor states.
		// (Useful for example, the AI system to tell the physics system to
		// alter a game objects state).
		//
		// @param actorId The ID of the game object.
		// @param vel The new velocity of the object.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetVelocity(const ActorId actorId, const Vector3 &vel) { };

		// /////////////////////////////////////////////////////////////////
		// Physics actor states.
		// (Useful for example, the AI system to tell the physics system to
		// alter a game objects state).
		//
		// @param actorId The ID of the actor.
		// @param vec The vector to translate the object by.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VTranslate(const ActorId actorId, const Vector3 &vec) { };
	};


}

#endif
