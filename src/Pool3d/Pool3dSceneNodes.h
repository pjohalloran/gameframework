// /////////////////////////////////////////////////////////////////
// @file Pool3dSceneNodes.h
// @author PJ O Halloran
// @date 19/11/2010
//
// File contains the header for the Pool3d 3D scene node objects.
// - BallSceneNode
// - TableSceneNode
// - CueSceneNode
//
// /////////////////////////////////////////////////////////////////

#ifndef __POOL_3D_SCENE_NODES_H
#define __POOL_3D_SCENE_NODES_H

#ifdef WIN32
#   pragma warning( push )
#   pragma warning( disable:4290 )
#endif

// External Headers
#include <strstream>

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>


// Project Headers
#include "CommonBatchSceneNode.h"
#include "GameColors.h"
#include "ObjModelFileLoader.h"

#include "Pool3dActors.h"


namespace GameHalloran {

    class SceneGraphManager;

    // /////////////////////////////////////////////////////////////////
    // @enum Pool3dModelLoadingId
    //
    // Distinguish between different pool3d model loading process events.
    // ePoolTable:  Loading the pool table meshes and textures.
    // ePoolCue:    Loading the pool cue mesh and texture.
    // eStatic:     Loading static level mesh data.
    //
    // /////////////////////////////////////////////////////////////////
    enum Pool3dModelLoadingId {
        ePoolTable = 0,
        ePoolCue,
        eStaticMesh,
        eTotal
    };

    // /////////////////////////////////////////////////////////////////
    // @class BallSceneNode
    // @author PJ O Halloran
    //
    // A 3D scene object representing a pool ball game actor.
    //
    // /////////////////////////////////////////////////////////////////
    class BallSceneNode : public CommonBatchSceneNode {
    private:

        BallActorParams m_param;                        ///< Ball actors parameters.

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor helper function.
        //
        // Declared static to ensure the programmer knows that class members
        // are not available at this time.  This is because it may be called
        // during the constructors member initialization list.
        //
        // Creates a ball mesh from the attributes of the ball actor defined
        // in the parameters struct.
        //
        // @param param The Ball actor parameters object.
        //
        // @return boost::shared_ptr<IGLBatchBase> Pointer to a GLTools mesh
        //                                          or NULL on error.
        //
        // /////////////////////////////////////////////////////////////////
        static boost::shared_ptr<IGLBatchBase> CreateBallMeshFromParams(BallActorParams &param);

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param sgPtr SG manager pointer.
        // @param actorId The Actor ID.
        // @param material The objects material.
        // @param toWorld Position and orientation of node relative to its
        //                  parent.
        // @param textureName Name of texture to apply to the batch.
        // @param shaderNameRef Name of the custom shader to render the node
        //                      (SGMs' global shader will be used if this is empty).
        // @param param Actors parameters.
        //
        // @throw GameException& If we fail to create the geometry mesh from
        //                          the actors parameters.
        //
        // /////////////////////////////////////////////////////////////////
        explicit BallSceneNode(SceneGraphManager *sgPtr, const boost::optional<ActorId> actorId, const Material &material, const Matrix4 &toWorld, const std::string &textureName, const std::string &shaderNameRef, BallActorParams &param) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param sgPtr SG manager pointer.
        // @param actorId The Actor ID.
        // @param material The objects material.
        // @param toWorld Position and orientation of node relative to its
        //                  parent.
        // @param fromWorld Inverse of toWorld matrix.
        // @param textureName Name of texture to apply to the batch.
        // @param shaderNameRef Name of the shader to render the node.
        // @param param Actors parameters.
        //
        // @throw GameException& If we fail to create the geometry mesh from
        //                          the actors parameters.
        //
        // /////////////////////////////////////////////////////////////////
        explicit BallSceneNode(SceneGraphManager *sgPtr, const boost::optional<ActorId> actorId, const Material &material, const Matrix4 &toWorld, const Matrix4 &fromWorld, const std::string &textureName, const std::string &shaderNameRef, BallActorParams &param) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param sgPtr SG manager pointer.
        // @param actorId The Actor ID.
        // @param material The objects material.
        // @param toWorld Position and orientation of node relative to its
        //                  parent.
        // @param textureName Name of texture to apply to the batch.
        // @param shaderNameRef Name of the custom shader to render the node
        //                      (SGMs' global shader will be used if this is empty).
        // @param param Actors parameters.
        // @param actorMesh Pointer to the ball mesh geometry data seeing as
        //                      there will be lots of instances of cue balls
        //                      and we dont want to be generating a spherical
        //                      mesh over and over again.
        //
        // @throw GameException& If we fail to create the geometry mesh from
        //                          the actors parameters.
        //
        // /////////////////////////////////////////////////////////////////
        explicit BallSceneNode(SceneGraphManager *sgPtr, const boost::optional<ActorId> actorId, const Material &material, const Matrix4 &toWorld, const std::string &textureName, const std::string &shaderNameRef, BallActorParams &param, boost::shared_ptr<IGLBatchBase> actorMesh) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~BallSceneNode();

        // /////////////////////////////////////////////////////////////////
        // Get the Ball Actor parameters.
        //
        // /////////////////////////////////////////////////////////////////
        virtual IActorParams *VGetActorParams(void) {
            return (&m_param);
        };

        // /////////////////////////////////////////////////////////////////
        // Set a nodes actor parameters.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSetActorParams(IActorParams *actorParamsPtr) {
            if(actorParamsPtr && actorParamsPtr->VGetType() == AT_Ball) {
                m_param = *(static_cast<BallActorParams *>(actorParamsPtr));
            }
        };

        // /////////////////////////////////////////////////////////////////
        // Called when application is restored.
        //
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnRestore();

        // /////////////////////////////////////////////////////////////////
        // Called when application loses focus.
        //
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnLostDevice();

        // /////////////////////////////////////////////////////////////////
        // Check if the ray intersects with this SceneNode.
        //
        // @param ray Raycast.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VPick(const RayCast &ray);
    };

    // /////////////////////////////////////////////////////////////////
    // @class TableSceneNode
    // @author PJ O Halloran
    //
    // A 3D scene object representing a pool table game actor.
    //
    // /////////////////////////////////////////////////////////////////
    class TableSceneNode : public CommonBatchSceneNode {
    private:

        TableActorParams m_param;                       ///< Table actors parameters.

        // /////////////////////////////////////////////////////////////////
        // Constructor helper function.
        //
        // /////////////////////////////////////////////////////////////////
        void Init() throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Initiaize the table panel child scene nodes.
        //
        // @param objLoader 3D mesh file loader.
        // @param progressObj Loading progress reporting object.
        // @param tbb Table mesh bounding box.
        // @param mpDepth The depth of the middle pocket
        //
        // @return bool True = success, false = failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool InitPanels(ObjModelFileLoader &objLoader, ModelLoadingProgress &progressObj, const BoundingCube &tbb, const F32 mpDepth);

        // /////////////////////////////////////////////////////////////////
        // Initialize the table pocket child scene nodes.
        //
        // @param objLoader 3D mesh file loader.
        // @param progressObj Loading progress reporting object.
        // @param tbb Table mesh bounding box.
        // @param mpDepth Stores the depth of the middle pocket mesh on exit.
        // @param pr Holds table pocket radius on exit.
        //
        // @return bool True = success, false = failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool InitPockets(ObjModelFileLoader &objLoader, ModelLoadingProgress &progressObj, const BoundingCube &tbb, F32 &mpDepth, F32 &pr);

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param sgPtr SG manager pointer.
        // @param actorId The Actor ID.
        // @param material The objects material.
        // @param toWorld Position and orientation of node relative to its
        //                  parent.
        // @param textureName Name of texture to apply to the batch.
        // @param shaderNameRef Name of the custom shader to render the node
        //                      (SGMs' global shader will be used if this is empty).
        // @param param Actors parameters.
        //
        // @throw GameException& If we fail to create the geometry mesh from
        //                          the actors parameters.
        //
        // /////////////////////////////////////////////////////////////////
        explicit TableSceneNode(SceneGraphManager *sgPtr, const boost::optional<ActorId> actorId, const Material &material, const Matrix4 &toWorld, const std::string &textureName, const std::string &shaderNameRef, TableActorParams &param) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param sgPtr SG manager pointer.
        // @param actorId The Actor ID.
        // @param material The objects material.
        // @param toWorld Position and orientation of node relative to its
        //                  parent.
        // @param fromWorld Inverse of toWorld matrix.
        // @param textureName Name of texture to apply to the batch.
        // @param shaderNameRef Name of the shader to render the node.
        // @param param Actors parameters.
        //
        // @throw GameException& If we fail to create the geometry mesh from
        //                          the actors parameters.
        //
        // /////////////////////////////////////////////////////////////////
        explicit TableSceneNode(SceneGraphManager *sgPtr, const boost::optional<ActorId> actorId, const Material &material, const Matrix4 &toWorld, const Matrix4 &fromWorld, const std::string &textureName, const std::string &shaderNameRef, TableActorParams &param) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~TableSceneNode();

        // /////////////////////////////////////////////////////////////////
        // Get the Ball Actor parameters.
        //
        // /////////////////////////////////////////////////////////////////
        virtual IActorParams *VGetActorParams(void) {
            return (&m_param);
        };

        // /////////////////////////////////////////////////////////////////
        // Set a nodes actor parameters.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSetActorParams(IActorParams *actorParamsPtr) {
            if(actorParamsPtr && actorParamsPtr->VGetType() == AT_Table) {
                m_param = *(static_cast<TableActorParams *>(actorParamsPtr));
            }
        };

        // /////////////////////////////////////////////////////////////////
        // Called when application is restored.
        //
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnRestore();

        // /////////////////////////////////////////////////////////////////
        // Called when application loses focus.
        //
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnLostDevice();

        // /////////////////////////////////////////////////////////////////
        // Check if the ray intersects with this SceneNode.
        //
        // @param ray Raycast.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VPick(const RayCast &ray);

        // /////////////////////////////////////////////////////////////////
        // Render all child nodes. Overriding all base class versions here
        // to ensure that all children attached to the pool table are rendered
        // if the pool table is visible to elimiate popping effect.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VRenderChildren();
    };

    // /////////////////////////////////////////////////////////////////
    // @class CueSceneNode
    // @author PJ O Halloran
    //
    // A 3D scene object representing a pool cue game actor.
    //
    // /////////////////////////////////////////////////////////////////
    class CueSceneNode : public CommonBatchSceneNode {
    private:

        CueActorParams m_param;                     ///< Cue actors parameters.

        // /////////////////////////////////////////////////////////////////
        // Constructor helper function.
        //
        // /////////////////////////////////////////////////////////////////
        void Init() throw(GameException &);

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param sgPtr SG manager pointer.
        // @param actorId The Actor ID.
        // @param material The objects material.
        // @param toWorld Position and orientation of node relative to its
        //                  parent.
        // @param textureName Name of texture to apply to the batch.
        // @param shaderNameRef Name of the custom shader to render the node
        //                      (SGMs' global shader will be used if this is empty).
        // @param param Actors parameters.
        //
        // @throw GameException& If we fail to create the geometry mesh from
        //                          the actors parameters.
        //
        // /////////////////////////////////////////////////////////////////
        explicit CueSceneNode(SceneGraphManager *sgPtr, const boost::optional<ActorId> actorId, const Material &material, const Matrix4 &toWorld, const std::string &textureName, const std::string &shaderNameRef, CueActorParams &param) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param sgPtr SG manager pointer.
        // @param actorId The Actor ID.
        // @param material The objects material.
        // @param toWorld Position and orientation of node relative to its
        //                  parent.
        // @param fromWorld Inverse of toWorld matrix.
        // @param textureName Name of texture to apply to the batch.
        // @param shaderNameRef Name of the shader to render the node.
        // @param param Actors parameters.
        //
        // @throw GameException& If we fail to create the geometry mesh from
        //                          the actors parameters.
        //
        // /////////////////////////////////////////////////////////////////
        explicit CueSceneNode(SceneGraphManager *sgPtr, const boost::optional<ActorId> actorId, const Material &material, const Matrix4 &toWorld, const Matrix4 &fromWorld, const std::string &textureName, const std::string &shaderNameRef, CueActorParams &param) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~CueSceneNode();

        // /////////////////////////////////////////////////////////////////
        // Get the Cue Actor parameters.
        //
        // /////////////////////////////////////////////////////////////////
        virtual IActorParams *VGetActorParams(void) {
            return (&m_param);
        };

        // /////////////////////////////////////////////////////////////////
        // Set a nodes actor parameters.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSetActorParams(IActorParams *actorParamsPtr) {
            if(actorParamsPtr && actorParamsPtr->VGetType() == AT_Cue) {
                m_param = *(static_cast<CueActorParams *>(actorParamsPtr));
            }
        };

        // /////////////////////////////////////////////////////////////////
        // Called when application is restored.
        //
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnRestore();

        // /////////////////////////////////////////////////////////////////
        // Called when application loses focus.
        //
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnLostDevice();

        // /////////////////////////////////////////////////////////////////
        // Check if the ray intersects with this SceneNode.
        //
        // @param ray Raycast.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VPick(const RayCast &ray);
    };
}

#ifdef WIN32
#   pragma warning( pop )
#endif

#endif
