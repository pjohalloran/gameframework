#pragma once
#ifndef __POOL_3D_ACTORS_H
#define __POOL_3D_ACTORS_H

// /////////////////////////////////////////////////////////////////
// @file Pool3dActors.h
// @author PJ O Halloran
// @date 01/11/2010
//
// File contains the header for the Pool3d actor parameters classes.
//
// /////////////////////////////////////////////////////////////////

#include <strstream>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include "Actors.h"
#include "GameColors.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // @struct PhysicsActorInfo
    // @author PJ O Halloran
    //
    // Information on each actors physics information which may be set
    // via lua scripts.
    //
    // /////////////////////////////////////////////////////////////////
    struct PhysicsActorInfo {
        F32 m_restitution;          ///< Actors "bounce".
        F32 m_friction;             ///< Actors friction.
        F32 m_density;              ///< Actors density.
        F32 m_linearDamping;        ///< Linear velocity damping factor.
        F32 m_angularDamping;       ///< Angular velocity damping factor.

        PhysicsActorInfo() : m_restitution(0.0f), m_friction(0.0f), m_density(0.0f), m_linearDamping(0.0f), m_angularDamping(0.0f) {};
    };

    // /////////////////////////////////////////////////////////////////
    // @class Pool3dActorParams
    // @author PJ O Halloran
    //
    // General pool game actor parameters.  This should be extended with
    // a parameter class with more specific parameters for dynamic scene
    // nodes.  This class provides enough info to create static scene
    // elements only.
    //
    // /////////////////////////////////////////////////////////////////
    class Pool3dActorParams : public ActorParams {
    public:

        static const U32 MAX_NAME_SIZE = 256;       ///< Maximum length of a texture/shader name string.

        // ID for special collision group for the physics engine so we can move cue about the scene and not have it interfere with objects other than the cue ball.
        static const I32 PLAYER_POOL_PHYSICS_GROUP = 64;    ///< Bullet physics group ID for player interaction physics objects (e.g. Cue and Cue Ball).

    private:

        char m_textureName[MAX_NAME_SIZE];                  ///< Name of texture to apply to the actor.
        char m_shaderName[MAX_NAME_SIZE];                   ///< Name of custom shader to use to render actor.
        char m_meshName[MAX_NAME_SIZE];                     ///< Name of the actors mesh.
        Material m_material;                                ///< Actors Material (for lighting calculations).
        PhysicsActorInfo m_physicsInfo;                     ///< Physics engine information for an actor.

    protected:

        // /////////////////////////////////////////////////////////////////
        // Copy a string into an array of chars as long as the string is less
        // than MAX_NAME_SIZE in length.
        //
        // @param memberArr Array of chars of length MAX_NAME_SIZE.
        // @param str String to copy to the array.
        //
        // /////////////////////////////////////////////////////////////////
        void SetStringMember(char *memberArr, const std::string &str);

    public:

        // /////////////////////////////////////////////////////////////////
        // Default Constructor.
        //
        // /////////////////////////////////////////////////////////////////
        explicit Pool3dActorParams() : ActorParams(), m_material(), m_physicsInfo() {
            ActorParams::VSetSize(sizeof(Pool3dActorParams));
            ActorParams::VSetType(AT_Unknown);
        };

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param textureName The name of the texture to apply to the actor.
        // @param shaderName The name of the shader to use to render the actor.
        // @param meshName The name of the actors mesh model.
        // @param material Actors Material (for lighting calculations).
        // @param physicsInfo Physics information for the actor.
        //
        // /////////////////////////////////////////////////////////////////
        explicit Pool3dActorParams(const std::string &textureName, \
                                   const std::string &shaderName, \
                                   const std::string &meshName, \
                                   const Material &material, \
                                   const PhysicsActorInfo &physicsInfo);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~Pool3dActorParams();

        // /////////////////////////////////////////////////////////////////
        // Initialize the parameters from a stream.
        //
        // @param in The input stream to initialize the parameters from.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VInit(std::istringstream &in);

        // /////////////////////////////////////////////////////////////////
        // Serialize the parameters out to a stream.
        //
        // @param out The output stream
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSerialize(std::ostringstream &out) const;

        // /////////////////////////////////////////////////////////////////
        // Initialize the parameters from a Lua script.
        //
        // @param srcData The Lua object containing the parameter data.
        // @param errorMessages The error messages (maybe passed back from the script?)
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VInit(LuaPlus::LuaObject srcData, TErrorMessageList &errorMessages);

        // /////////////////////////////////////////////////////////////////
        // Get the texture name.
        //
        // /////////////////////////////////////////////////////////////////
        const char * const GetTextureName() const {
            return (m_textureName);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the texture name.
        //
        // /////////////////////////////////////////////////////////////////
        void SetTextureName(const std::string &textureName) {
            SetStringMember(m_textureName, textureName);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the shader name.
        //
        // /////////////////////////////////////////////////////////////////
        const char * const GetShaderName() const {
            return (m_shaderName);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the shader name.
        //
        // /////////////////////////////////////////////////////////////////
        void SetShaderName(const std::string &shaderName) {
            SetStringMember(m_shaderName, shaderName);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the mesh name.
        //
        // /////////////////////////////////////////////////////////////////
        const char * const GetMeshName() const {
            return (m_meshName);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the mesh name.
        //
        // /////////////////////////////////////////////////////////////////
        void SetMeshName(const std::string &meshName) {
            SetStringMember(m_meshName, meshName);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the actors material.
        //
        // /////////////////////////////////////////////////////////////////
        Material GetMaterial() const {
            return (m_material);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the actors material.
        //
        // /////////////////////////////////////////////////////////////////
        void SetMaterial(const Material &material);

        // /////////////////////////////////////////////////////////////////
        // Set the actors physics information.
        //
        // /////////////////////////////////////////////////////////////////
        PhysicsActorInfo GetPhysicsInfo() const {
            return (m_physicsInfo);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the actors physics information.
        //
        // /////////////////////////////////////////////////////////////////
        void SetPhysicsInfo(const PhysicsActorInfo &physicsInfo) {
            m_physicsInfo = physicsInfo;
        };

        // /////////////////////////////////////////////////////////////////
        // Base logic create actor function.  This should be overridden
        // completely by derived classes for special types of scene nodes
        // for the game.
        //
        // The actor created should be added manually to the logic layer.
        //
        // @return boost::shared_ptr<IActor> A pointer to an actor or NULL
        //                                      on error.
        //
        // /////////////////////////////////////////////////////////////////
        virtual boost::shared_ptr<IActor> VCreate();

        // /////////////////////////////////////////////////////////////////
        // The view layers create function.  Creates a general scene node
        // here useful for static scenery.  This should be overridden
        // completely by derived classes for special types of scene nodes
        // for the game.
        //
        // @return boost::shared_ptr<ISceneNode> A pointer to a scene node.
        //
        // /////////////////////////////////////////////////////////////////
        virtual boost::shared_ptr<ISceneNode> VCreateSceneNode();
    };


    // /////////////////////////////////////////////////////////////////
    // @class BallActorParams
    // @author PJ O Halloran
    //
    // Pool ball actor parameters.
    //
    // /////////////////////////////////////////////////////////////////
    class BallActorParams : public Pool3dActorParams {
    public:

        // /////////////////////////////////////////////////////////////////
        // @enum BallGroup
        //
        // The group the ball belongs to.
        //
        // /////////////////////////////////////////////////////////////////
        enum BallGroup {
            eCue = 0,
            eStripes,
            ePlain,
            eBlack,
            eNumberBallGroupTypes,
            eNone
        };

    private:

        F32 m_radius;                                       ///< Radius of the ball.
        I32 m_slices;                                       ///< Number of triangle bands around sphere.
        I32 m_stacks;                                       ///< Number of triangle bands up and down the sphere.
        BallGroup m_group;                                  ///< Ball group the ball belongs to.

    protected:

    public:

        // /////////////////////////////////////////////////////////////////
        // Default constructor.
        //
        // /////////////////////////////////////////////////////////////////
        explicit BallActorParams() : Pool3dActorParams(), m_radius(0.0f), m_slices(0), m_stacks(0), m_group(eCue) {
            ActorParams::VSetSize(sizeof(BallActorParams));
            ActorParams::VSetType(AT_Ball);
        };

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param textureName The name of the texture to apply to the ball.
        // @param shaderName The name of the shader to use to render the ball.
        // @param material Balls material (for lighting).
        // @param physicsInfo Physics engine info for the actor.
        // @param radius Radius of the ball.
        // @param slices Number of triangle bands around sphere.
        // @param stacks Number of triangle bands up and down the sphere.
        // @param group Ball group the ball belongs to.
        //
        // /////////////////////////////////////////////////////////////////
        explicit BallActorParams(const std::string &textureName, const std::string &shaderName, const Material &material, const PhysicsActorInfo &physicsInfo, const F32 radius, const I32 slices, const I32 stacks, const BallGroup group);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~BallActorParams();

        // /////////////////////////////////////////////////////////////////
        // Initialize the parameters from a stream.
        //
        // @param in The input stream to initialize the parameters from.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VInit(std::istringstream &in);

        // /////////////////////////////////////////////////////////////////
        // Serialize the parameters out to a stream.
        //
        // @param out The output stream
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSerialize(std::ostringstream &out) const;

        // /////////////////////////////////////////////////////////////////
        // Initialize the parameters from a Lua script.
        //
        // @param srcData The Lua object containing the parameter data.
        // @param errorMessages The error messages (maybe passed back from the script?)
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VInit(LuaPlus::LuaObject srcData, TErrorMessageList &errorMessages);

        // /////////////////////////////////////////////////////////////////
        // The logic layers create function.  Creates an Actor in respect to
        // the logic layer from the parameters.
        //
        // The actor created should be added manually to the logic layer.
        //
        // @return boost::shared_ptr<IActor> A pointer to an actor or NULL
        //                                      on error.
        //
        // /////////////////////////////////////////////////////////////////
        virtual boost::shared_ptr<IActor> VCreate();

        // /////////////////////////////////////////////////////////////////
        // The view layers create function.  Creates an SceneNode representing
        // the Actor in respect to the view layer from the parameters.
        //
        // @return boost::shared_ptr<ISceneNode> A pointer to a scene node.
        //
        // /////////////////////////////////////////////////////////////////
        virtual boost::shared_ptr<ISceneNode> VCreateSceneNode();

        // /////////////////////////////////////////////////////////////////
        // Get the radius of the ball.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetRadius() const {
            return (m_radius);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the radius of the ball.
        //
        // /////////////////////////////////////////////////////////////////
        void SetRadius(const F32 radius) {
            m_radius = radius;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the number of stacks.
        //
        // /////////////////////////////////////////////////////////////////
        I32 GetStacks() const {
            return (m_stacks);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the number of stacks.
        //
        // /////////////////////////////////////////////////////////////////
        void SetStacks(const I32 stacks) {
            m_stacks = stacks;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the number of slices.
        //
        // /////////////////////////////////////////////////////////////////
        I32 GetSlices() const {
            return (m_slices);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the number of slices.
        //
        // /////////////////////////////////////////////////////////////////
        void SetSlices(const I32 slices) {
            m_slices = slices;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the ballgroup.
        //
        // /////////////////////////////////////////////////////////////////
        BallGroup GetBallGroup() const {
            return (m_group);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the ball group.
        //
        // /////////////////////////////////////////////////////////////////
        void SetBallGroup(const BallGroup group) {
            m_group = group;
        };
    };

    // /////////////////////////////////////////////////////////////////
    // @class TableActorParams
    // @author PJ O Halloran
    //
    // Pool table actor.  Holds RC ids for the various meshes used to
    // render the pool table as well as textures to apply to them and
    // material information for lighting them.
    //
    // /////////////////////////////////////////////////////////////////
    class TableActorParams : public Pool3dActorParams {
    private:

        // Table Side Panels meshes.
        char m_frontPanelMeshName[MAX_NAME_SIZE];   ///< Name of the model file for the front/back panels.
        char m_sidePanelMeshName[MAX_NAME_SIZE];    ///< Name of the model file for the side panels.
        char m_panelTextureName[MAX_NAME_SIZE];     ///< All Panels texture ID.
        Material m_panelMaterial;                   ///< Material for the table panels.

        // Table Pockets meshes.
        char m_middlePocketMeshName[MAX_NAME_SIZE]; ///< Name of the model file for the middle pocket mesh.
        char m_cornerPocketMeshName[MAX_NAME_SIZE]; ///< Name of the model file for the corner pocket mesh.
        Material m_pocketsMaterial;                 ///< Material for the table pockets.
        char m_pocketTextureName[MAX_NAME_SIZE];    ///< Texture to apply to the table poacket.

        F32 m_width;                                ///< Table width (calculated from table mesh).
        F32 m_height;                               ///< Table height (calculated from table mesh).
        F32 m_depth;                                ///< Table depth (calculated from table mesh).
        F32 m_pocketRadius;                     ///< Table pocket radius (calculated from pocket meshes).

        Point3 m_tlpTriggerPos;                     ///< Position of the top left pocket trigger area.
        I32 m_tlPocketId;                           ///< Trigger ID for the top left pocket.
        Point3 m_trpTriggerPos;                     ///< Position of the top right pocket trigger area.
        I32 m_trPocketId;                           ///< Trigger ID for the top right pocket.
        Point3 m_blpTriggerPos;                     ///< Position of the bottom left pocket trigger area.
        I32 m_blPocketId;                           ///< Trigger ID for the bottom left pocket.
        Point3 m_brpTriggerPos;                     ///< Position of the bottom right pocket trigger area.
        I32 m_brPocketId;                           ///< Trigger ID for the bottom right pocket.
        Point3 m_mlpTriggerPos;                     ///< Position of the middle left pocket trigger area.
        I32 m_mlPocketId;                           ///< Trigger ID for the middle left pocket.
        Point3 m_mrpTriggerPos;                     ///< Position of the top left pocket trigger area.
        I32 m_mrPocketId;                           ///< Trigger ID for the middle right pocket.

    public:

        // /////////////////////////////////////////////////////////////////
        // Default constructor.
        //
        // /////////////////////////////////////////////////////////////////
        explicit TableActorParams() : Pool3dActorParams(), m_panelMaterial(), m_pocketsMaterial(), \
            m_width(0.0f), m_height(0.0f), m_depth(0.0f), m_pocketRadius(0.0f), m_tlpTriggerPos(), m_tlPocketId(0), \
            m_trpTriggerPos(), m_trPocketId(1), m_blpTriggerPos(), m_blPocketId(2), m_brpTriggerPos(), m_brPocketId(3), \
            m_mlpTriggerPos(), m_mlPocketId(4), m_mrpTriggerPos(), m_mrPocketId(5) {
            ActorParams::VSetSize(sizeof(TableActorParams));
            ActorParams::VSetType(AT_Table);
        };

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param textureName The name of the texture to apply to the table.
        // @param shaderName The name of the shader to use to render the table.
        // @param meshName The RC ID for the table mesh.
        // @param material Material for lighting.
        // @param physicsInfo The phsics information for the table.
        // @param frontPanelMeshName The RC ID for the front panel mesh.
        // @param sidePanelMeshName The RC ID for the side panel mesh.
        // @param panelTextureName The texture RC ID for the table panel.
        // @param panelMaterial The material info for the table panel.
        // @param middlePocketMeshName The RC ID for the middle pocket mesh.
        // @param cornerPocketMeshName The RC ID for the corner pocket mesh.
        // @param pocketMaterial The pocket material.
        // @param pocketTextureName The texture RC ID for the pockets.
        //
        // /////////////////////////////////////////////////////////////////
        explicit TableActorParams(const std::string &textureName, \
                                  const std::string &shaderName, \
                                  const std::string &meshName, \
                                  const Material &material, \
                                  const PhysicsActorInfo &physicsInfo, \
                                  const std::string &frontPanelMeshName, \
                                  const std::string &sidePanelMeshName, \
                                  const std::string &panelTextureName, \
                                  const Material &panelMaterial, \
                                  const std::string &middlePocketMeshName, \
                                  const std::string &cornerPocketMeshName, \
                                  const Material &pocketMaterial,
                                  const std::string &pocketTextureName);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~TableActorParams();

        // /////////////////////////////////////////////////////////////////
        // Initialize the parameters from a stream.
        //
        // @param in The input stream to initialize the parameters from.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VInit(std::istringstream &in);

        // /////////////////////////////////////////////////////////////////
        // Serialize the parameters out to a stream.
        //
        // @param out The output stream
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSerialize(std::ostringstream &out) const;

        // /////////////////////////////////////////////////////////////////
        // Initialize the parameters from a Lua script.
        //
        // @param srcData The Lua object containing the parameter data.
        // @param errorMessages The error messages (maybe passed back from the script?)
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VInit(LuaPlus::LuaObject srcData, TErrorMessageList &errorMessages);

        // /////////////////////////////////////////////////////////////////
        // The logic layers create function.  Creates an Actor in respect to
        // the logic layer from the parameters.
        //
        // The actor created should be added manually to the logic layer.
        //
        // @return boost::shared_ptr<IActor> A pointer to an actor or NULL
        //                                      on error.
        //
        // /////////////////////////////////////////////////////////////////
        virtual boost::shared_ptr<IActor> VCreate();

        // /////////////////////////////////////////////////////////////////
        // The view layers create function.  Creates an SceneNode representing
        // the Actor in respect to the view layer from the parameters.
        //
        // @return boost::shared_ptr<ISceneNode> A pointer to a scene node.
        //
        // /////////////////////////////////////////////////////////////////
        virtual boost::shared_ptr<ISceneNode> VCreateSceneNode();

        // /////////////////////////////////////////////////////////////////
        // Get the front panel mesh RC ID.
        //
        // /////////////////////////////////////////////////////////////////
        std::string GetFrontPanelMeshName() const {
            return (std::string(m_frontPanelMeshName));
        };

        // /////////////////////////////////////////////////////////////////
        // Set the front panel RC ID.
        //
        // /////////////////////////////////////////////////////////////////
        void SetFrontPanelMeshName(const std::string &str) {
            SetStringMember(m_frontPanelMeshName, str);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the back panel mesh RC ID.
        //
        // /////////////////////////////////////////////////////////////////
        std::string GetSidePanelMeshName() const {
            return (std::string(m_sidePanelMeshName));
        };

        // /////////////////////////////////////////////////////////////////
        // Set the side panel RC ID.
        //
        // /////////////////////////////////////////////////////////////////
        void SetSidePanelMeshName(const std::string &str) {
            SetStringMember(m_sidePanelMeshName, str);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the panel RC texture ID.
        //
        // /////////////////////////////////////////////////////////////////
        std::string GetPanelTextureName() const {
            return (std::string(m_panelTextureName));
        };

        // /////////////////////////////////////////////////////////////////
        // Set the panel RC texture ID.
        //
        // /////////////////////////////////////////////////////////////////
        void SetPanelTextureName(const std::string &str) {
            SetStringMember(m_panelTextureName, str);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the material used to light the table panels.
        //
        // /////////////////////////////////////////////////////////////////
        Material GetTablePanelMaterial() const {
            return (m_panelMaterial);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the material used to light the table panels.
        //
        // /////////////////////////////////////////////////////////////////
        void SetTablePanelMaterial(const Material &material) {
            m_panelMaterial = material;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the middle pocket mesh RC ID.
        //
        // /////////////////////////////////////////////////////////////////
        std::string GetMiddlePocketMeshName() const {
            return (std::string(m_middlePocketMeshName));
        };

        // /////////////////////////////////////////////////////////////////
        // Set the middle pocket mesh RC ID.
        //
        // /////////////////////////////////////////////////////////////////
        void SetMiddlePocketMeshName(const std::string &str) {
            SetStringMember(m_middlePocketMeshName, str);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the corner pocket mesh RC ID.
        //
        // /////////////////////////////////////////////////////////////////
        std::string GetCornerPocketMeshName() const {
            return (std::string(m_cornerPocketMeshName));
        };

        // /////////////////////////////////////////////////////////////////
        // Set the corner pocket mesh RC ID.
        //
        // /////////////////////////////////////////////////////////////////
        void SetCornerPocketMeshName(const std::string &str) {
            SetStringMember(m_cornerPocketMeshName, str);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the material used to light the table pockets.
        //
        // /////////////////////////////////////////////////////////////////
        Material GetTablePocketMaterial() const {
            return (m_pocketsMaterial);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the material used to light the table pockets.
        //
        // /////////////////////////////////////////////////////////////////
        void SetTablePocketMaterial(const Material &material) {
            m_pocketsMaterial = material;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the name of the texture to apply to the pockets.
        //
        // /////////////////////////////////////////////////////////////////
        std::string GetPocketTextureName() const {
            return (std::string(m_pocketTextureName));
        };

        // /////////////////////////////////////////////////////////////////
        // Set the name of the texture to apply to the pockets.
        //
        // /////////////////////////////////////////////////////////////////
        void SetPocketTextureName(const std::string &str) {
            SetStringMember(m_pocketTextureName, str);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the table width.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetWidth() const {
            return (m_width);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the table width.
        //
        // /////////////////////////////////////////////////////////////////
        void SetWidth(const F32 width) {
            m_width = width;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the table height.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetHeight() const {
            return (m_height);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the table width.
        //
        // /////////////////////////////////////////////////////////////////
        void SetHeight(const F32 height) {
            m_height = height;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the table depth.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetDepth() const {
            return (m_depth);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the table depth.
        //
        // /////////////////////////////////////////////////////////////////
        void SetDepth(const F32 depth) {
            m_depth = depth;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the table pocket radius.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetPocketRadius() const {
            return (m_pocketRadius);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the table pocket radius.
        //
        // /////////////////////////////////////////////////////////////////
        void SetPocketRadius(const F32 pocketRadius) {
            m_pocketRadius = pocketRadius;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the location of the trigger area for the top left pocket to
        // notify the app when a ball is potted.
        //
        // /////////////////////////////////////////////////////////////////
        Point3 GetTopLeftPocketTriggerPos() const {
            return (m_tlpTriggerPos);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the ID of the trigger area for the top left pocket to
        // notify the app when a ball is potted.
        //
        // /////////////////////////////////////////////////////////////////
        I32 GetTopLeftPocketTriggerId() const {
            return (m_tlPocketId);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the location of the trigger area for the top right pocket to
        // notify the app when a ball is potted.
        //
        // /////////////////////////////////////////////////////////////////
        Point3 GetTopRightPocketTriggerPos() const {
            return (m_trpTriggerPos);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the ID of the trigger area for the top right pocket to
        // notify the app when a ball is potted.
        //
        // /////////////////////////////////////////////////////////////////
        I32 GetTopRightPocketTriggerId() const {
            return (m_trPocketId);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the location of the trigger area for the bottom left pocket to
        // notify the app when a ball is potted.
        //
        // /////////////////////////////////////////////////////////////////
        Point3 GetBottomLeftPocketTriggerPos() const {
            return (m_blpTriggerPos);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the ID of the trigger area for the bottom left pocket to
        // notify the app when a ball is potted.
        //
        // /////////////////////////////////////////////////////////////////
        const I32 GetBottomLeftPocketTriggerId() const {
            return (m_blPocketId);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the location of the trigger area for the bottom right pocket to
        // notify the app when a ball is potted.
        //
        // /////////////////////////////////////////////////////////////////
        Point3 GetBottomRightPocketTriggerPos() const {
            return (m_brpTriggerPos);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the ID of the trigger area for the bottom right pocket to
        // notify the app when a ball is potted.
        //
        // /////////////////////////////////////////////////////////////////
        I32 GetBottomRightPocketTriggerId() const {
            return (m_brPocketId);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the location of the trigger area for the middle left pocket to
        // notify the app when a ball is potted.
        //
        // /////////////////////////////////////////////////////////////////
        Point3 GetMiddleLeftPocketTriggerPos() const {
            return (m_mlpTriggerPos);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the ID of the trigger area for the middle left pocket to
        // notify the app when a ball is potted.
        //
        // /////////////////////////////////////////////////////////////////
        I32 GetMiddleLeftPocketTriggerId() const {
            return (m_mlPocketId);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the location of the trigger area for the middle right pocket to
        // notify the app when a ball is potted.
        //
        // /////////////////////////////////////////////////////////////////
        Point3 GetMiddleRightPocketTriggerPos() const {
            return (m_mrpTriggerPos);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the ID of the trigger area for the middle right pocket to
        // notify the app when a ball is potted.
        //
        // /////////////////////////////////////////////////////////////////
        I32 GetMiddleRightPocketTriggerId() const {
            return (m_mrPocketId);
        };

    };

    // /////////////////////////////////////////////////////////////////
    // @class CueActorParams
    // @author PJ O Halloran
    //
    // Pool cue actor.
    //
    // /////////////////////////////////////////////////////////////////
    class CueActorParams : public Pool3dActorParams {
    private:

        F32 m_width;                                        ///< Width of cue.
        F32 m_height;                                       ///< Height of cue.
        F32 m_depth;                                        ///< Depth of cue.

    public:

        // /////////////////////////////////////////////////////////////////
        // Default constructor.
        //
        // /////////////////////////////////////////////////////////////////
        explicit CueActorParams() : Pool3dActorParams() {
            ActorParams::VSetSize(sizeof(CueActorParams));
            ActorParams::VSetType(AT_Cue);
        };

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param textureName The name of the texture to apply to the cue.
        // @param shaderName The name of the shader to use to render the cue.
        // @param meshName Name of the cue mesh.
        // @param material Material for lighting.
        // @param physicsInfo Physics information for the cue.
        //
        // /////////////////////////////////////////////////////////////////
        explicit CueActorParams(const std::string &textureName, \
                                const std::string &shaderName, \
                                const std::string &meshName, \
                                const Material &material, \
                                const PhysicsActorInfo &physicsInfo);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~CueActorParams();

        // /////////////////////////////////////////////////////////////////
        // Initialize the parameters from a stream.
        //
        // @param in The input stream to initialize the parameters from.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VInit(std::istringstream &in);

        // /////////////////////////////////////////////////////////////////
        // Serialize the parameters out to a stream.
        //
        // @param out The output stream
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSerialize(std::ostringstream &out) const;

        // /////////////////////////////////////////////////////////////////
        // Initialize the parameters from a Lua script.
        //
        // @param srcData The Lua object containing the parameter data.
        // @param errorMessages The error messages (maybe passed back from the script?)
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VInit(LuaPlus::LuaObject srcData, TErrorMessageList &errorMessages);

        // /////////////////////////////////////////////////////////////////
        // The logic layers create function.  Creates an Actor in respect to
        // the logic layer from the parameters.
        //
        // The actor created should be added manually to the logic layer.
        //
        // @return boost::shared_ptr<IActor> A pointer to an actor or NULL
        //                                      on error.
        //
        // /////////////////////////////////////////////////////////////////
        virtual boost::shared_ptr<IActor> VCreate();

        // /////////////////////////////////////////////////////////////////
        // The view layers create function.  Creates an SceneNode representing
        // the Actor in respect to the view layer from the parameters.
        //
        // @return boost::shared_ptr<ISceneNode> A pointer to a scene node.
        //
        // /////////////////////////////////////////////////////////////////
        virtual boost::shared_ptr<ISceneNode> VCreateSceneNode();

        // /////////////////////////////////////////////////////////////////
        // Get the cue width.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetWidth() const {
            return (m_width);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the cue width.
        //
        // /////////////////////////////////////////////////////////////////
        void SetWidth(const F32 width) {
            m_width = width;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the cue height.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetHeight() const {
            return (m_height);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the cue width.
        //
        // /////////////////////////////////////////////////////////////////
        void SetHeight(const F32 height) {
            m_height = height;
        };

        // /////////////////////////////////////////////////////////////////
        // Get the cue depth.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetDepth() const {
            return (m_depth);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the cue depth.
        //
        // /////////////////////////////////////////////////////////////////
        void SetDepth(const F32 depth) {
            m_depth = depth;
        };

    };
}

#endif
