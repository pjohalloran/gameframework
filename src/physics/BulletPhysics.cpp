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
// @file BulletPhysics.cpp
// @author Mike McShaffry & PJ O Halloran
// @date 14/10/2010
//
// Implementation file for the games Bullet API physics class.
//
// /////////////////////////////////////////////////////////////////

#include <string>

#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

#include "BulletPhysics.h"
#include "EventManager.h"
#include "GameMain.h"
#include "GameBase.h"
#include "PhysicsEvents.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // ***** Useful Bullet to gameframework engine conversion utility functions *****
    // /////////////////////////////////////////////////////////////////

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GfTriangleMeshTobtTriangleMesh(const TriangleMesh &myMesh, btTriangleMesh &bulletMesh)
    {
        Vertex currVertex;                                          // Current vertex in GF mesh.
        const U32 NUM_VERTICES(Triangle::eNumberVertices);  // ...
        btVector3 verts[NUM_VERTICES];                              // Three bullet triangle positions.

        for(TriangleMesh::const_iterator i = myMesh.begin(), end = myMesh.end(); i != end; ++i) {
            for(I32 vi = 0; vi < NUM_VERTICES; ++vi) {
                (*i)->GetVertex(Triangle::VertexId(vi), currVertex);
                Point3TobtVector3(currVertex.GetPosition(), verts[vi]);
            }

            bulletMesh.addTriangle(verts[0], verts[1], verts[2]);
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void GfTriangleBatchTobtTriangleMesh(const GLTriangleBatch &myBatch, btTriangleMesh &bulletMesh)
    {
        // Error - TriangleBatch must be completed before we can create a physical mesh!
        if(!myBatch.IsBatchComplete()) {
            return;
        }

        btIndexedMesh indexedMesh;
        indexedMesh.m_numTriangles = myBatch.GetIndexCount();
        indexedMesh.m_numVertices = myBatch.GetVertexCount();
        indexedMesh.m_triangleIndexStride = sizeof(GLushort) * 3;
        indexedMesh.m_vertexStride = sizeof(GLfloat) * 3;
        GLushort *tmpArr = myBatch.GetIndexArray();
        indexedMesh.m_triangleIndexBase = reinterpret_cast<unsigned char *>(tmpArr);
        VertexArr *tmpArr2 = myBatch.GetVertexArray();
        indexedMesh.m_vertexBase = reinterpret_cast<unsigned char *>(tmpArr2);
        bulletMesh.addIndexedMesh(indexedMesh, PHY_SHORT);
    }

    // /////////////////////////////////////////////////////////////////
    // ************************* BulletPhysics *************************
    // /////////////////////////////////////////////////////////////////

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::SendCollisionPairAddEvent(btPersistentManifold const *manifoldPtr, btRigidBody const *body0Ptr, btRigidBody const *body1Ptr)
    {
        // Parameter check
        if(!manifoldPtr || !body0Ptr || !body1Ptr) {
            GF_LOG_TRACE_ERR("BulletPhysics::SendCollisionPairAddEvent()", "Invalid parameters");
            return;
        }

        if(body0Ptr->getUserPointer() || body1Ptr->getUserPointer()) {
            // only triggers have non-NULL userPointers

            // figure out which actor is the trigger
            btRigidBody const * triggerBody, * otherBody;

            if(body0Ptr->getUserPointer()) {
                triggerBody = body0Ptr;
                otherBody = body1Ptr;
            } else {
                otherBody = body0Ptr;
                triggerBody = body1Ptr;
            }

            // send the trigger event.
            const I32 triggerId = *(static_cast<I32*>(triggerBody->getUserPointer()));
            safeQueEvent(IEventDataPtr(GCC_NEW EvtData_PhysTrigger_Enter(triggerId, FindActorID(otherBody))));
        } else {
            boost::optional<ActorId> const id0 = FindActorID(body0Ptr);
            boost::optional<ActorId> const id1 = FindActorID(body1Ptr);

            if(!id0.is_initialized() || !id1.is_initialized()) {
#if defined(DEBUG)
                std::string str;
                if(id0.is_initialized()) {
                    try {
                        str = boost::lexical_cast<std::string, ActorId>(*id0);
                    } catch(...) { }
                } else if(id1.is_initialized()) {
                    try {
                        str = boost::lexical_cast<std::string, ActorId>(*id1);
                    } catch(...) { }
                }

                // something is colliding with a non-actor.  we currently don't send events for that
                GF_LOG_TRACE_DEB("BulletPhysics::SendCollisionPairAddEvent()", "A collision event occurred between actor " + str + " and a game object with no ID assigned (Not handled at present)");
#endif
                return;
            }

            // this pair of colliding objects is new.  send a collision-begun event
            Point3List collisionPoints;
            Vector3 sumNormalForce, sumFrictionForce;
            F32 sumImpulseForce(0.0f);

            boost::shared_ptr<Point3> currPointPtr;
            Vector3 currNormalF, currFrictionF;
            for(I32 i = 0, numContacts = manifoldPtr->getNumContacts(); i < numContacts; ++i) {
                const btManifoldPoint &point = manifoldPtr->getContactPoint(i);

                currPointPtr.reset(GCC_NEW Point3);
                btVector3ToPoint3(point.getPositionWorldOnB(), *(currPointPtr.get()));
                collisionPoints.push_back(currPointPtr);

                btVector3ToVector3(point.m_combinedRestitution * point.m_normalWorldOnB, currNormalF);
                btVector3ToVector3(point.m_combinedFriction * point.m_lateralFrictionDir1, currFrictionF);

                sumNormalForce += currNormalF;
                sumFrictionForce += currFrictionF;
                sumImpulseForce += point.getAppliedImpulse();
            }

            // send the event for the game
            safeQueEvent(IEventDataPtr(GCC_NEW EvtData_PhysCollision(*id0, *id1, sumNormalForce, sumFrictionForce, collisionPoints, sumImpulseForce)));
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::SendCollisionPairRemoveEvent(btRigidBody const *body0Ptr, btRigidBody const *body1Ptr)
    {
        // Parameter check
        if(!body0Ptr || !body1Ptr) {
            GF_LOG_TRACE_ERR("BulletPhysics::SendCollisionPairRemoveEvent()", "Invalid parameters");
            return;
        }

        if(body0Ptr->getUserPointer() || body1Ptr->getUserPointer()) {
            // figure out which actor is the trigger
            btRigidBody const *triggerBody, *otherBody;

            if(body0Ptr->getUserPointer()) {
                triggerBody = body0Ptr;
                otherBody = body1Ptr;
            } else {
                otherBody = body0Ptr;
                triggerBody = body1Ptr;
            }

            // send the trigger event.
            const I32 triggerId = *(static_cast<I32*>(triggerBody->getUserPointer()));
            safeQueEvent(IEventDataPtr(GCC_NEW EvtData_PhysTrigger_Leave(triggerId, FindActorID(otherBody))));
        } else {
            const boost::optional<ActorId> id0 = FindActorID(body0Ptr);
            const boost::optional<ActorId> id1 = FindActorID(body1Ptr);

            if(!id0.is_initialized() || !id1.is_initialized()) {
                // collision is ending between some object(s) that don't have actors.  we don't send events for that.
#if defined(DEBUG)
                std::string str;
                if(id0.is_initialized()) {
                    try {
                        str = boost::lexical_cast<std::string, ActorId>(*id0);
                    } catch(...) { }
                } else if(id1.is_initialized()) {
                    try {
                        str = boost::lexical_cast<std::string, ActorId>(*id1);
                    } catch(...) { }
                }
                GF_LOG_DEB(std::string("BulletPhysics::SendCollisionPairRemoveEvent()", "A collision end event occurred between a game object ") + str + std::string(" and an object that has no ID (Not handled at present)"));
#endif
                return;
            }

            safeQueEvent(IEventDataPtr(GCC_NEW EvtData_PhysSeparation(*id0, *id1)));
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::AddGameActorRigidBody(btCollisionShape *shapePtr, const PhysicsObjectAttributes &physicsObject)
    {
        // Parameter check
        if(!shapePtr || !physicsObject.m_actorId) {
            GF_LOG_TRACE_ERR("BulletPhysics::AddGameActorRigidBody()", "Invalid parameters");
            return;
        }
        if(physicsObject.m_bodyType != eRigidBody) {
            GF_LOG_TRACE_ERR("BulletPhysics::AddGameActorRigidBody()", "Cannot add non rigid body using this function");
            return;
        }

        // actors get one body apiece
        const ActorId actorID = *physicsObject.m_actorId;
        if(m_actorBodies.find(actorID) != m_actorBodies.end()) {
#if DEBUG
            std::string actorStr;
            try {
                actorStr = boost::lexical_cast<std::string, ActorId>(actorID);
            } catch(...) { }
            GF_LOG_TRACE_ERR("BulletPhysics::AddGameActorRigidBody()", "Tried to add another shape for an actor that already has a shape registered, id: " + actorStr);
#endif
            return;
        }

        // localInertia defines how the object's mass is distributed
        btVector3 localInertia(0.0f, 0.0f, 0.0f);
        if(physicsObject.m_mass != 0.0f) {
            // Dynamic (moving) rigidbody (else its a non moving static one)!
            shapePtr->calculateLocalInertia(physicsObject.m_mass, localInertia);
        }

        // set the initial position of the body from the actor
        ActorMotionState * const myMotionState = GCC_NEW ActorMotionState(physicsObject.m_objMatrix);

        btRigidBody::btRigidBodyConstructionInfo rbInfo(physicsObject.m_mass, myMotionState, shapePtr, localInertia);

        // set up the materal properties
        rbInfo.m_restitution = physicsObject.m_restitution;
        rbInfo.m_friction = physicsObject.m_friction;
        rbInfo.m_linearDamping = physicsObject.m_linearDamping;
        rbInfo.m_angularDamping = physicsObject.m_angularDamping;

        btRigidBody * const rigidBodyPtr = new btRigidBody(rbInfo);

        if(physicsObject.m_objectType != eStatic) {
            rigidBodyPtr->setContactProcessingThreshold(0.0f);
        }

        if(physicsObject.m_objectType == eKinematic) {
            rigidBodyPtr->setCollisionFlags(rigidBodyPtr->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
            rigidBodyPtr->setActivationState(DISABLE_DEACTIVATION);
        }

        // To work around Bullets inconsistant interface we will see if a btDiscreteDynamicsWorld is in use.  If it is we will use the custom groups
        //  defined in PhysicsObjectAttributes for collision filtering.  If not, we just use Bullet defaults.
        boost::shared_ptr<btDiscreteDynamicsWorld> discreteDynamicsWorldPtr = boost::static_pointer_cast<btDiscreteDynamicsWorld>(m_dynamicsWorld);
        if(discreteDynamicsWorldPtr) {
            discreteDynamicsWorldPtr->addRigidBody(rigidBodyPtr, physicsObject.m_collisionGroup, physicsObject.m_collisionMask);
        } else {
            m_dynamicsWorld->addRigidBody(rigidBodyPtr);
        }

        // create the BulletActor
        boost::shared_ptr<BulletActor> bulletActorPtr(GCC_NEW BulletActor(rigidBodyPtr));

        // add it to the collection to be checked for changes in VSyncVisibleScene
        m_actorBodies[actorID] = bulletActorPtr;
        m_rigidBodyToActorId[rigidBodyPtr] = actorID;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::RemoveCollisionObject(btCollisionObject *removeMePtr)
    {
        // Parameter check
        if(!removeMePtr) {
            GF_LOG_TRACE_DEB("BulletPhysics::RemoveCollisionObject()", "Invalid parameters");
            return;
        }

        // first remove the object from the physics sim
        m_dynamicsWorld->removeCollisionObject(removeMePtr);

        // then remove the pointer from the ongoing contacts list
        for(CollisionPairs::iterator it = m_previousTickCollisionPairs.begin(); it != m_previousTickCollisionPairs.end();) {
            CollisionPairs::iterator next = it;
            ++next;

            if((it->first == removeMePtr) || (it->second == removeMePtr)) {
                SendCollisionPairRemoveEvent(it->first, it->second);
                m_previousTickCollisionPairs.erase(it);
            }

            it = next;
        }

        // if the object is a RigidBody (all of ours are RigidBodies, but it's good to be safe)
        if(btRigidBody * const body = btRigidBody::upcast(removeMePtr)) {
            // delete the components of the object
            // Created in AddGameActorRigidBody()...
            delete body->getMotionState();
            // Created externally or in the VAddShape() interface routines...
            delete body->getCollisionShape();
            // Created in VCreateTrigger()...
            if(body->getUserPointer()) {
                delete body->getUserPointer();
            }

            for(I32 ii = body->getNumConstraintRefs() - 1; ii >= 0; --ii) {
                btTypedConstraint * const constraint = body->getConstraintRef(ii);
                m_dynamicsWorld->removeConstraint(constraint);
                delete constraint;
            }
        } else {
            GF_LOG_TRACE_ERR("BulletPhysics::RemoveCollisionObject()", "Non btRigidBody found in bullet world!");
        }

        delete removeMePtr;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    btRigidBody *BulletPhysics::FindActorBody(const ActorId id) const
    {
        boost::shared_ptr<BulletActor> bulletActorPtr = FindBulletActor(id);
        if(bulletActorPtr) {
            return (bulletActorPtr->m_rigidBodyPtr);
        }

        return (NULL);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    boost::shared_ptr<BulletPhysics::BulletActor> BulletPhysics::FindBulletActor(const ActorId id) const
    {
        ActorIDToBulletActorMap::const_iterator found = m_actorBodies.find(id);

        if(found != m_actorBodies.end()) {
            return (found->second);
        }

        boost::shared_ptr<BulletActor> null;
        return (null);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    boost::optional<ActorId> BulletPhysics::FindActorID(btRigidBody const *bodyPtr) const
    {
        // Parameter check
        if(!bodyPtr) {
            GF_LOG_TRACE_ERR("BulletPhysics::FindActorID()", "Invalid parameters");
            return (boost::optional<ActorId>());
        }

        RigidBodyToActorIDMap::const_iterator found = m_rigidBodyToActorId.find(bodyPtr);
        if(found != m_rigidBodyToActorId.end()) {
            return (found->second);
        }

        return (boost::optional<ActorId>());
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    btStridingMeshInterface const *BulletPhysics::FindMeshShape(const ActorId aid) const
    {
        BulletMeshMap::const_iterator found = m_meshMap.find(aid);
        if(found != m_meshMap.end()) {
            return (found->second);
        }
        return (NULL);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    PhysicsObjectType BulletPhysics::GetPhysicsObjectType(const btCollisionObject *bodyPtr) const
    {
        if(!bodyPtr) {
            return (eInvalidObject);
        }

        if(bodyPtr->isKinematicObject()) {
            return (eKinematic);
        }

        if(bodyPtr->isStaticObject()) {
            const btRigidBody *rigidBodyPtr = btRigidBody::upcast(bodyPtr);
            if(!rigidBodyPtr) {
                return (eStatic);
            }

            // Get the mass of the rigid body.
            btScalar mass = 0.0f, invMass = rigidBodyPtr->getInvMass();
            if(invMass != 0.0f) {
                mass = 1.0f / invMass;
            }

            if(mass != 0.0f) {
                return (eDynamic);
            } else {
                return (eStatic);
            }
        }

        // Should never get here but included to silence compiler warnings.
        return (eInvalidObject);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    PhysicsBodyType BulletPhysics::GetPhysicsBodyType(const btCollisionObject *bodyPtr) const
    {
        if(!bodyPtr) {
            return (eInvalidBody);
        }

        PhysicsBodyType type;

        switch(bodyPtr->getInternalType()) {
            case btCollisionObject::CO_COLLISION_OBJECT:
                type = eCollisionBody;
                break;
            case btCollisionObject::CO_RIGID_BODY:
                type = eRigidBody;
                break;
            case btCollisionObject::CO_GHOST_OBJECT:
                type = eDummyBody;
                break;
            case btCollisionObject::CO_SOFT_BODY:
                type = eSoftBody;
                break;
            default:
                type = eInvalidBody;
                break;
        }

        return (type);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::BulletInternalTickCallback(btDynamicsWorld * const world, const btScalar timeStep)
    {
        // Parameter check
        if(!world || !world->getWorldUserInfo()) {
            GF_LOG_TRACE_DEB("BulletPhysics::BulletInternalTickCallback()", "Invalid parameters");
            return;
        }


        BulletPhysics * const bulletPhysics = static_cast<BulletPhysics*>(world->getWorldUserInfo());
        CollisionPairs currentTickCollisionPairs;

        // look at all existing contacts
        btDispatcher * const dispatcher = world->getDispatcher();
        for(int manifoldIdx = 0, numManifolds = dispatcher->getNumManifolds(); manifoldIdx < numManifolds; ++manifoldIdx) {
            // get the "manifold", which is the set of data corresponding to a contact point
            //   between two physics objects
            btPersistentManifold const * const manifold = dispatcher->getManifoldByIndexInternal(manifoldIdx);
            if(!manifold) {
                GF_LOG_TRACE_ERR("BulletPhysics::BulletInternalTickCallback()", "Failed to get the manifold pointer");
                return;
            }

            if(manifold->getNumContacts() > 0) {
                // get the two bodies used in the manifold.  Bullet stores them as void*, so we must cast
                //  them back to btRigidBody*s.  Manipulating void* pointers is usually a bad
                //  idea, but we have to work with the environment that we're given.  We know this
                //  is safe because we only ever add btRigidBodys to the simulation
                btRigidBody const * const body0 = static_cast<btRigidBody const *>(manifold->getBody0());
                btRigidBody const * const body1 = static_cast<btRigidBody const *>(manifold->getBody1());

                // always create the pair in a predictable order
                bool const swapped = body0 > body1;

                btRigidBody const * const sortedBodyA = (swapped ? body1 : body0);
                btRigidBody const * const sortedBodyB = (swapped ? body0 : body1);

                CollisionPair const thisPair = std::make_pair(sortedBodyA, sortedBodyB);
                currentTickCollisionPairs.insert(thisPair);

                if(bulletPhysics->m_previousTickCollisionPairs.find(thisPair) == bulletPhysics->m_previousTickCollisionPairs.end()) {
                    // this is a new contact, which wasn't in our list before.  send an event to the game.
                    bulletPhysics->SendCollisionPairAddEvent(manifold, body0, body1);
                }
            }
        }

        CollisionPairs removedCollisionPairs;

        // use the STL set difference function to find collision pairs that existed during the previous tick but not any more
        std::set_difference(bulletPhysics->m_previousTickCollisionPairs.begin(), bulletPhysics->m_previousTickCollisionPairs.end(),
                            currentTickCollisionPairs.begin(), currentTickCollisionPairs.end(),
                            std::inserter(removedCollisionPairs, removedCollisionPairs.begin()));

        for(CollisionPairs::const_iterator it = removedCollisionPairs.begin(), end = removedCollisionPairs.end(); it != end; ++it) {
            btRigidBody const * const body0 = it->first;
            btRigidBody const * const body1 = it->second;
            bulletPhysics->SendCollisionPairRemoveEvent(body0, body1);
        }

        // the current tick becomes the previous tick.  this is the way of all things.
        bulletPhysics->m_previousTickCollisionPairs = currentTickCollisionPairs;

        //// handle actors that want to turn manually
        //for(ActorIDToBulletActorMap::const_iterator it = bulletPhysics->m_actorBodies.begin(); it != bulletPhysics->m_actorBodies.end(); ++it)
        //{
        //  if(it->second->m_desiredDeltaYAngleTime > 0)
        //  {
        //      F32 const amountOfTimeToConsume = std::min(timeStep, it->second->m_desiredDeltaYAngleTime);
        //      F32 const deltaAngle = (amountOfTimeToConsume / it->second->m_desiredDeltaYAngleTime) * it->second->m_desiredDeltaYAngle;
        //
        //      // create a transform to represent the additional turning this frame
        //      btTransform angleTransform;
        //      angleTransform.setIdentity();
        //      angleTransform.getBasis().setEulerYPR(0, deltaAngle, 0); // rotation about body Y-axis
        //
        //      btRigidBody * const body = it->second->m_rigidBodyPtr;
        //
        //      // concatenate the transform onto the body's transform
        //      body->setCenterOfMassTransform(body->getCenterOfMassTransform() * angleTransform);
        //
        //      // tick down the parameters
        //      it->second->m_desiredDeltaYAngle -= deltaAngle;
        //      it->second->m_desiredDeltaYAngleTime -= amountOfTimeToConsume;
        //  }
        //}
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    BulletPhysics::BulletPhysics(boost::shared_ptr<ModelViewProjStackManager> mvpStackManagerPtr)\
:
    m_dynamicsWorld()
    , m_broadphase()
    , m_dispatcher()
    , m_solver()
    , m_collisionConfiguration()
    , m_debugDrawer()
    , m_actorBodies()
    , m_rigidBodyToActorId()
    , m_previousTickCollisionPairs()
    , m_mvpStackManagerPtr(mvpStackManagerPtr)
    , m_meshMap()
    {
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    BulletPhysics::~BulletPhysics()
    {
        try {
            // delete any physics objects which are still in the world
            for(I32 index = m_dynamicsWorld->getNumCollisionObjects() - 1; index >= 0; --index) {
                btCollisionObject * const obj = m_dynamicsWorld->getCollisionObjectArray()[index];
                RemoveCollisionObject(obj);
            }

            // destroy all the BulletActor objects (all shared_ptr so they are cleaned automatically!)
            m_actorBodies.clear();

            // Clean up any meshes created.
            for(BulletMeshMap::iterator i = m_meshMap.begin(), end = m_meshMap.end(); i != end; ++i) {
                if(i->second) {
                    delete i->second;
                    i->second = NULL;
                }
            }
            m_meshMap.clear();
        } catch(...) { }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool BulletPhysics::VInitialize()
    {
        // VInitialize creates the components that Bullet uses
        // this controls how Bullet does internal memory management during the collision pass
        m_collisionConfiguration.reset(GCC_NEW btDefaultCollisionConfiguration());
        if(!m_collisionConfiguration) {
            GF_LOG_TRACE_ERR("BulletPhysics::VInitialize()", "Failed to create the btDefaultCollisionConfiguration");
            return (false);
        }

        // this manages how Bullet detects precise collisions between pairs of objects
        m_dispatcher.reset(GCC_NEW btCollisionDispatcher(m_collisionConfiguration.get()));
        if(!m_dispatcher) {
            GF_LOG_TRACE_ERR("BulletPhysics::VInitialize()", "Failed to create the btCollisionDispatcher");
            return (false);
        }

        // Bullet uses this to quickly (imprecisely) detect collisions between objects.
        //  Once a possible collision passes the broad phase, it will be passed to the
        //   slower but more precise narrow-phase collision detection (btCollisionDispatcher).
        m_broadphase.reset(GCC_NEW btDbvtBroadphase());
        //m_broadphase.reset(new btAxisSweep3(btVector3(-2.0f, -2.0f, -2.0f), btVector3(2.0f, 2.0f, 2.0f)));
        if(!m_broadphase) {
            GF_LOG_TRACE_ERR("BulletPhysics::VInitialize()", "Failed to create the btDbvtBroadphase");
            return (false);
        }

        // Manages constraints which apply forces to the physics simulation.  Used
        //  for e.g. springs, motors.  We don't use any constraints right now.
        m_solver.reset(GCC_NEW btSequentialImpulseConstraintSolver);
        if(!m_solver) {
            GF_LOG_TRACE_ERR("BulletPhysics::VInitialize()", "Failed to create the btSequentialImpulseConstraintSolver");
            return (false);
        }

        // This is the main Bullet interface point.  Pass in all these components to customize its behavior.
        m_dynamicsWorld.reset(GCC_NEW btDiscreteDynamicsWorld(m_dispatcher.get(),
                              m_broadphase.get(),
                              m_solver.get(),
                              m_collisionConfiguration.get()));
        if(!m_dynamicsWorld) {
            GF_LOG_TRACE_ERR("BulletPhysics::VInitialize()", "Failed to create the btDiscreteDynamicsWorld");
            return (false);
        }

        // also set up the functionality for debug drawing
        m_debugDrawer.reset(GCC_NEW BulletPhysicsDebugDrawer(m_mvpStackManagerPtr));
        if(!m_debugDrawer) {
            GF_LOG_TRACE_ERR("BulletPhysics::VInitialize()", "Failed to create the BulletPhysicsDebugDrawer");
            return (false);
        }

        m_dynamicsWorld->setDebugDrawer(m_debugDrawer.get());

        // and set the internal tick callback to our own method "BulletInternalTickCallback"
        m_dynamicsWorld->setInternalTickCallback(BulletInternalTickCallback);
        m_dynamicsWorld->setWorldUserInfo(this);

        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VSyncVisibleScene()
    {
        // Keep physics & graphics in sync

        // check all the existing actor's bodies for changes.
        //  If there is a change, send the appropriate event for the game system.
        for(ActorIDToBulletActorMap::const_iterator it = m_actorBodies.begin(); it != m_actorBodies.end(); ++it) {
            const ActorId id = it->first;

            // get the MotionState.  this object is updated by Bullet.
            // it's safe to cast the btMotionState to ActorMotionState, because all the bodies in m_actorBodies
            //   were created through AddGameActorRigidBody()
            ActorMotionState const * const actorMotionStatePtr = static_cast<ActorMotionState*>(it->second->m_rigidBodyPtr->getMotionState());
            if(!actorMotionStatePtr) {
                GF_LOG_TRACE_DEB("BulletPhysics::VSyncVisibleScene()", "Failed to cast to ActorMotionState");
                continue;
            }

            boost::shared_ptr<IActor> gameActor = g_appPtr->GetLogicPtr()->VGetActor(id);
            if(!gameActor) {
#if DEBUG
                std::string str;
                try {
                    boost::lexical_cast<std::string, ActorId>(id);
                } catch(...) { }
                GF_LOG_TRACE_DEB("BulletPhysics::VSyncVisibleScene()", "Failed to retrieve the actor from the Logic Layer, id: " + str);
#endif
                continue;
            }
            if(gameActor->VGetMat() != actorMotionStatePtr->m_worldToPositionTransform) {
                // bullet has moved the actor's physics object.  update the actor.
                safeQueEvent(IEventDataPtr(GCC_NEW EvtData_Move_Actor(id, actorMotionStatePtr->m_worldToPositionTransform)));
            }
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VOnUpdate(const F32 deltaSeconds)
    {
        m_dynamicsWorld->stepSimulation(deltaSeconds, 12, 1.0f / 240.0f);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VAddSphere(const F32 radius, const F32 specificGravity, PhysicsObjectAttributes &physicsObjectAtt)
    {
        // create the collision body, which specifies the shape of the object
        btSphereShape * const collisionShape = new btSphereShape(btScalar(radius));

        // Sphere volume = 4/3 * PI * r^3
        const F32 volume = (4.0f / 3.0f) * static_cast<F32>(M3D_PI) * radius * radius * radius;

        // DENSITY = MASS / VOLUME (SpecificGravity is Density / 1gm/cm^3)
        physicsObjectAtt.m_mass = volume * specificGravity;

        AddGameActorRigidBody(collisionShape, physicsObjectAtt);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VAddBox(const Vector3 &dimensions, const F32 specificGravity, PhysicsObjectAttributes &physicsObjectAtt)
    {
        // create the collision body, which specifies the shape of the object
        btVector3 bulletVec;
        Vector3TobtVector3(dimensions, bulletVec);
        btBoxShape * const boxShape = new btBoxShape(bulletVec);

        // Volume of a cube is W*H*D
        const F32 volume = dimensions.GetX() * dimensions.GetY() * dimensions.GetZ();

        // DENSITY = MASS / VOLUME (SpecificGravity is Density / 1gm/cm^3)
        physicsObjectAtt.m_mass = volume * specificGravity;

        AddGameActorRigidBody(boxShape, physicsObjectAtt);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VAddCylinder(const Vector3 &dimensions, const F32 specificGravity, PhysicsObjectAttributes &physicsObjectAtt)
    {
        // create the collision body, which specifies the shape of the object
        btVector3 bulletVec;
        Vector3TobtVector3(dimensions, bulletVec);
        btCylinderShapeZ * const cylinderShape = new btCylinderShapeZ(bulletVec);

        // Volume of a cylinder is PI * r^2 * h
        F32 r(cylinderShape->getRadius());
        const F32 volume = F32(M3D_PI) * r * r * dimensions.GetY();

        // DENSITY = MASS / VOLUME (SpecificGravity is Density / 1gm/cm^3)
        physicsObjectAtt.m_mass = volume * specificGravity;

        AddGameActorRigidBody(cylinderShape, physicsObjectAtt);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VAddPointCloud(const Point3List &vertVec, const F32 specificGravity, PhysicsObjectAttributes &physicsObjectAtt)
    {
        btConvexHullShape * const shape = new btConvexHullShape();

        // add the poI32s to the shape one at a time
        btVector3 bulletVec;
        for(Point3List::const_iterator i = vertVec.begin(), end = vertVec.end(); i != end; ++i) {
            Point3TobtVector3(*(*i), bulletVec);
            shape->addPoint(bulletVec);
        }

        // approximate volume (and therefore mass) using bounding box.
        btVector3 aabbMin(0, 0, 0), aabbMax(0, 0, 0);
        shape->getAabb(btTransform::getIdentity(), aabbMin, aabbMax);
        const btVector3 aabbExtents = aabbMax - aabbMin;
        const F32 volume = aabbExtents.x() * aabbExtents.y() * aabbExtents.z();

        // DENSITY = MASS / VOLUME (SpecificGravity is Density / 1gm/cm^3)
        physicsObjectAtt.m_mass = volume * specificGravity;

        AddGameActorRigidBody(shape, physicsObjectAtt);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VAddStaticMesh(const TriangleMesh &mesh, PhysicsObjectAttributes &physicsObjectAtt)
    {
        if(mesh.empty() || !physicsObjectAtt.m_actorId) {
            GF_LOG_TRACE_ERR("BulletPhysics::VAddStaticMesh()", "Invalid parameters");
            return;
        }

        if(FindMeshShape(*physicsObjectAtt.m_actorId) != NULL) {
            GF_LOG_TRACE_ERR("BulletPhysics::VAddStaticMesh()", "Actor already has a mesh");
            return;
        }

        btTriangleMesh * const bulletMesh = new btTriangleMesh(false, false);
        GameHalloran::GfTriangleMeshTobtTriangleMesh(mesh, *bulletMesh);

        m_meshMap[*physicsObjectAtt.m_actorId] = bulletMesh;

        btBvhTriangleMeshShape * const shape = new btBvhTriangleMeshShape(bulletMesh, true);

        // Add the shape - Static object so we use 0 mass.
        physicsObjectAtt.m_mass = 0.0f;
        physicsObjectAtt.m_objectType = eStatic;
        AddGameActorRigidBody(shape, physicsObjectAtt);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VAddStaticMesh(const GLTriangleBatch &batch, PhysicsObjectAttributes &physicsObjectAtt)
    {
        if(!batch.IsBatchComplete() || !physicsObjectAtt.m_actorId) {
            GF_LOG_TRACE_ERR("BulletPhysics::VAddStaticMesh()", "Invalid parameters");
            return;
        }

        if(FindMeshShape(*physicsObjectAtt.m_actorId) != NULL) {
            GF_LOG_TRACE_ERR("BulletPhysics::VAddStaticMesh()", "Actor already has a mesh");
            return;
        }

        btTriangleMesh * const bulletMesh = new btTriangleMesh(false, false);
        GameHalloran::GfTriangleBatchTobtTriangleMesh(batch, *bulletMesh);

        m_meshMap[*physicsObjectAtt.m_actorId] = bulletMesh;

        btBvhTriangleMeshShape * const shape = new btBvhTriangleMeshShape(bulletMesh, false);

        // Add the shape - Static object so we use 0 mass.
        physicsObjectAtt.m_mass = 0.0f;
        physicsObjectAtt.m_objectType = eStatic;
        AddGameActorRigidBody(shape, physicsObjectAtt);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool BulletPhysics::VGetObjectStatus(const PhysicsObjectId id, PhysicsObjectAttributes &status) const
    {
        // TODO: Implement PhysicsObjectId in the BulletPhysics internal data structures.
        return (false);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool BulletPhysics::VGetActorObjectStatus(const ActorId aid, PhysicsObjectAttributes &status) const
    {
        boost::shared_ptr<BulletActor> bulletActor = FindBulletActor(aid);
        if(!bulletActor || !bulletActor->m_rigidBodyPtr) {
            return (false);
        }

        status.m_id = 0;
        status.m_bodyType = GetPhysicsBodyType(dynamic_cast<btCollisionObject *>(bulletActor->m_rigidBodyPtr));
        status.m_objectType = GetPhysicsObjectType(dynamic_cast<btCollisionObject *>(bulletActor->m_rigidBodyPtr));
        status.m_actorId = aid;
        status.m_active = bulletActor->m_rigidBodyPtr->isActive();
        status.m_friction = bulletActor->m_rigidBodyPtr->getFriction();
        status.m_restitution = bulletActor->m_rigidBodyPtr->getRestitution();
        status.m_collisionGroup = bulletActor->m_rigidBodyPtr->getBroadphaseProxy()->m_collisionFilterGroup;
        status.m_collisionMask = bulletActor->m_rigidBodyPtr->getBroadphaseProxy()->m_collisionFilterMask;

        const ActorMotionState *actorMotionPtr = dynamic_cast<const ActorMotionState *>(bulletActor->m_rigidBodyPtr->getMotionState());
        btTransform trans;
        if(actorMotionPtr) {
            actorMotionPtr->getWorldTransform(trans);
        } else {
            trans = bulletActor->m_rigidBodyPtr->getWorldTransform();
        }
        btTransformToMatrix4(trans, status.m_objMatrix);

        btVector3ToVector3(bulletActor->m_rigidBodyPtr->getLinearVelocity(), status.m_linearVelocity);
        status.m_linearDamping = bulletActor->m_rigidBodyPtr->getLinearDamping();
        btVector3ToVector3(bulletActor->m_rigidBodyPtr->getAngularVelocity(), status.m_angularVelocity);
        status.m_angularDamping = bulletActor->m_rigidBodyPtr->getAngularDamping();
        status.m_mass = bulletActor->m_rigidBodyPtr->getInvMass();
        if(status.m_mass != 0.0f) {
            status.m_mass = 1.0f / status.m_mass;
        }
        btVector3ToVector3(bulletActor->m_rigidBodyPtr->getInvInertiaDiagLocal(), status.m_inertia);
        if(status.m_inertia.GetX() != 0.0f) {
            status.m_inertia.SetX(1.0f / status.m_inertia.GetX());
        }
        if(status.m_inertia.GetY() != 0.0f) {
            status.m_inertia.SetY(1.0f / status.m_inertia.GetY());
        }
        if(status.m_inertia.GetZ() != 0.0f) {
            status.m_inertia.SetZ(1.0f / status.m_inertia.GetZ());
        }

        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VRemoveActor(const ActorId id)
    {
        if(btRigidBody * const body = FindActorBody(id)) {
            // destroy the body and all its components
            RemoveCollisionObject(body);

            // clear the relevant elements from the lookup maps
            ActorIDToBulletActorMap::iterator it = m_actorBodies.find(id);
            if(it != m_actorBodies.end()) {
                m_actorBodies.erase(it);
            }
            m_rigidBodyToActorId.erase(body);

            // Check if actor had a physics mesh created for it, if so free it.
            BulletMeshMap::iterator mit = m_meshMap.find(id);
            if(mit != m_meshMap.end()) {
                delete mit->second;
                mit->second = NULL;
                m_meshMap.erase(mit);
            }
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VRenderDiagnostics()
    {
        m_dynamicsWorld->debugDrawWorld();
        m_debugDrawer->BatchDraw();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    I32 BulletPhysics::VGetDiagnosticMode() const
    {
        if(m_dynamicsWorld->getDebugDrawer()) {
            return (m_dynamicsWorld->getDebugDrawer()->getDebugMode());
        }

        return (btIDebugDraw::DBG_NoDebug);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VSetDiagnosticMode(const I32 mode)
    {
        if(m_dynamicsWorld->getDebugDrawer()) {
            m_dynamicsWorld->getDebugDrawer()->setDebugMode(mode);
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VCreateTrigger(const Point3 &pos, const F32 dim, const I32 triggerID)
    {
        // create the collision body, which specifies the shape of the object
        btVector3 bulletVec(dim, dim, dim);
        btBoxShape * const boxShape = new btBoxShape(bulletVec);

        // triggers are immoveable.  0 mass signals this to Bullet.
        const btScalar mass = 0;

        // set the initial position of the body from the actor
        Matrix4 triggerTrans(g_identityMat);
        triggerTrans[Matrix4::M30] = pos.GetX();
        triggerTrans[Matrix4::M31] = pos.GetY();
        triggerTrans[Matrix4::M32] = pos.GetZ();
        triggerTrans[Matrix4::M33] = 1.0f;
        ActorMotionState * const myMotionState = GCC_NEW ActorMotionState(triggerTrans);

        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, boxShape, btVector3(0.0f , 0.0f , 0.0f));
        btRigidBody * const body = new btRigidBody(rbInfo);

        m_dynamicsWorld->addRigidBody(body);

        // a trigger is just a box that doesn't collide with anything.  That's what "CF_NO_CONTACT_RESPONSE" indicates.
        body->setCollisionFlags(body->getCollisionFlags() | btRigidBody::CF_NO_CONTACT_RESPONSE);
        body->setUserPointer(new I32(triggerID));
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VRemoveTrigger(const I32 triggerId)
    {
        bool removed(false);
        btCollisionObjectArray &collisionObjects = m_dynamicsWorld->getCollisionObjectArray();

        for(I32 i = 0, size = collisionObjects.size(); ((!removed) && (i < size)); ++i) {
            // Any rigid bodies that have a user pointer defined are trigger areas so we search for them.
            btRigidBody *currRigidBodyPtr = btRigidBody::upcast(collisionObjects[i]);
            if(currRigidBodyPtr && currRigidBodyPtr->getUserPointer()) {
                I32 *idPtr = reinterpret_cast<I32 *>(currRigidBodyPtr->getUserPointer());
                if(idPtr && *idPtr == triggerId) {
                    RemoveCollisionObject(currRigidBodyPtr);
                    removed = true;
                }
            }
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VApplyForce(const Vector3 &dir, const F32 newtons, const ActorId aid)
    {
        if(btRigidBody * const body = FindActorBody(aid)) {
            btVector3 force;
            Vector3TobtVector3(dir, force);
            force *= newtons;

            // Wake up the body if its in a sleeping state first.
            body->activate();

            body->applyCentralImpulse(force);
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VApplyTorque(const Vector3 &dir, const F32 newtons, const ActorId aid)
    {
        if(btRigidBody * const body = FindActorBody(aid)) {
            btVector3 torque;
            Vector3TobtVector3(dir, torque);
            torque *= newtons;

            // Wake up the body if its in a sleeping state first.
            body->activate();

            body->applyTorqueImpulse(torque);
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool BulletPhysics::VKinematicMove(const Matrix4 &mat, const ActorId aid)
    {
        if(btRigidBody * const body = FindActorBody(aid)) {
            // If we are moving a dynamic body we should wake it up first.
            //if(body->isKinematicObject())
            //{
            //}

            // warp the body to the new position
            btTransform bulletMatrix;
            Matrix4TobtTransform(mat, bulletMatrix);
            body->getMotionState()->setWorldTransform(bulletMatrix);

            //body->activate(true);

            return (true);
        }

        return (false);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VRotateY(const ActorId actorId, const F32 angle, const F32 time)
    {
        if(const boost::shared_ptr<BulletActor> actor = FindBulletActor(actorId)) {
            // set the parameters for the turning that will be handled in BulletInternalTickCallback
            actor->m_desiredDeltaYAngle = angle;
            actor->m_desiredDeltaYAngleTime = time;
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    F32 BulletPhysics::VGetOrientationY(const ActorId actorId)
    {
        boost::shared_ptr<BulletActor> pBulletActor = FindBulletActor(actorId);
        if(!pBulletActor) {
            GF_LOG_TRACE_ERR("BulletPhysics::VGetOrientationY()", "Failed to find the actor in the physics system");
            return (FLT_MAX);
        }

        const btTransform& actorTransform = pBulletActor->m_rigidBodyPtr->getCenterOfMassTransform();
        btMatrix3x3 actorRotationMat(actorTransform.getBasis());  // should be just the rotation information

        btVector3 startingVec(0, 0, 1);
        btVector3 endingVec = actorRotationMat * startingVec; // transform the vector

        endingVec.setY(0);  // we only care about rotation on the XZ plane

        const F32 endingVecLength = endingVec.length();
        if(endingVecLength < 0.001) {
            // gimbal lock (orientation is straight up or down)
            GF_LOG_TRACE_ERR("BulletPhysics::VGetOrientationY()", "Gimbal lock occurred");
            return 0;
        }

        else {
            btVector3 cross = startingVec.cross(endingVec);
            F32 sign = (cross.getY() > 0 ? 1.0f : -1.0f);
            return (acosf(startingVec.dot(endingVec) / endingVecLength) * sign);
        }

        GF_LOG_TRACE_ERR("BulletPhysics::VGetOrientationY()", "Failed to find the Y orientation for the actor");
        return (FLT_MAX);  // fail...
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VStopActor(const ActorId actorId)
    {
        boost::shared_ptr<BulletActor> pBulletActor = FindBulletActor(actorId);
        if(pBulletActor) {
            pBulletActor->m_rigidBodyPtr->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
            pBulletActor->m_rigidBodyPtr->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VSetVelocity(const ActorId actorId, const Vector3 &vel)
    {
        boost::shared_ptr<BulletActor> pBulletActor = FindBulletActor(actorId);
        if(pBulletActor) {
            btVector3 btVel;
            Vector3TobtVector3(vel, btVel);
            pBulletActor->m_rigidBodyPtr->setLinearVelocity(btVel);
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void BulletPhysics::VTranslate(const ActorId actorId, const Vector3 &vec)
    {
        boost::shared_ptr<BulletActor> pBulletActor = FindBulletActor(actorId);
        if(pBulletActor) {
            btVector3 btVec;
            Vector3TobtVector3(vec, btVec);
            pBulletActor->m_rigidBodyPtr->translate(btVec);
        }
    }

}
