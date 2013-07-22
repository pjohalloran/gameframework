// /////////////////////////////////////////////////////////////////
// @file Pool3dSceneNodes.cpp
// @author PJ O Halloran
// @date 19/11/2010
//
// File contains the impl for the Pool3d scene nodes classes.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <cfloat>


// Project Headers
#include "Pool3dSceneNodes.h"
#include "Pool3dEvents.h"

#include "GameBase.h"
#include "GLTriangleBatch.h"
#include "GLTools.h"
#include "GameMain.h"
#include "SceneGraphManager.h"
#include "RayCast.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // ********************* BallSceneNode *****************************
    // /////////////////////////////////////////////////////////////////

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    boost::shared_ptr<IGLBatchBase> BallSceneNode::CreateBallMeshFromParams(BallActorParams &param)
    {
        boost::shared_ptr<GLTriangleBatch> batch(GCC_NEW GLTriangleBatch());
        if(!batch) {
            return (boost::shared_ptr<IGLBatchBase>());
        }

        // Generate a sphere (this is extremely slow so this mesh should be shared with each ball instance).
        gltMakeSphere(*(batch.get()), param.GetRadius(), param.GetSlices(), param.GetStacks());

        return (boost::static_pointer_cast<IGLBatchBase>(batch));
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    BallSceneNode::BallSceneNode(SceneGraphManager *sgPtr, \
                                 const boost::optional<ActorId> actorId, \
                                 const Material &material, \
                                 const Matrix4 &toWorld, \
                                 const std::string &textureName, \
                                 const std::string &shaderNameRef, \
                                 BallActorParams &param) throw(GameException &)\
:
    CommonBatchSceneNode(sgPtr, \
                         actorId, \
                         std::string("PoolBall"), \
                         RenderPassActor, \
                         material, \
                         toWorld, \
                         textureName, \
                         shaderNameRef, \
                         CreateBallMeshFromParams(param)), m_param(param)
    {
        if(!CommonBatchSceneNode::IsBatchValid()) {
            throw GameException(std::string("Failed to create ball scene node from parameters"));
        }

        SceneNode::SetRadius(m_param.GetRadius());
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    BallSceneNode::BallSceneNode(SceneGraphManager *sgPtr, \
                                 const boost::optional<ActorId> actorId, \
                                 const Material &material, \
                                 const Matrix4 &toWorld, \
                                 const Matrix4 &fromWorld, \
                                 const std::string &textureName, \
                                 const std::string &shaderNameRef, \
                                 BallActorParams &param) throw(GameException &)\
:
    CommonBatchSceneNode(sgPtr, \
                         actorId, \
                         std::string("PoolBall"), \
                         RenderPassActor, \
                         material, \
                         toWorld, \
                         fromWorld, \
                         textureName, \
                         shaderNameRef, \
                         CreateBallMeshFromParams(param)), m_param(param)

    {
        if(!CommonBatchSceneNode::IsBatchValid()) {
            throw GameException(std::string("Failed to create ball scene node from parameters"));
        }

        SceneNode::SetRadius(m_param.GetRadius());
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    BallSceneNode::BallSceneNode(SceneGraphManager *sgPtr, \
                                 const boost::optional<ActorId> actorId, \
                                 const Material &material, \
                                 const Matrix4 &toWorld, \
                                 const std::string &textureName, \
                                 const std::string &shaderNameRef, \
                                 BallActorParams &param, \
                                 boost::shared_ptr<IGLBatchBase> actorMesh) throw(GameException &)\
:
    CommonBatchSceneNode(sgPtr, \
                         actorId, \
                         std::string("PoolBall"), \
                         RenderPassActor, \
                         material, \
                         toWorld, \
                         textureName, \
                         shaderNameRef, \
                         actorMesh), m_param(param)
    {
        if(!CommonBatchSceneNode::IsBatchValid()) {
            throw GameException(std::string("Failed to supply valid mesh to constructor"));
        }

        SceneNode::SetRadius(m_param.GetRadius());
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    BallSceneNode::~BallSceneNode()
    {
        try {
        } catch(...) {}
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool BallSceneNode::VOnRestore()
    {
        bool result = CommonBatchSceneNode::VOnRestore();

        // Actor parameters may have changed so we regenerate the sphere geometry now.
        //  This is to handle the UpdateActorParams events that may be recevied from lua
        //   acripts during runtime.
        if(result) {
            CommonBatchSceneNode::SetBatch(CreateBallMeshFromParams(m_param));
            result = CommonBatchSceneNode::IsBatchValid();
            if(result) {
                SceneNode::SetRadius(m_param.GetRadius());
            }
        }

        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool BallSceneNode::VOnLostDevice()
    {
        return (CommonBatchSceneNode::VOnLostDevice());
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool BallSceneNode::VPick(const RayCast &ray)
    {
        bool result = false;
        result = SceneNode::VPick(ray);

        if(!result) {
            BoundingSphere bs;
            VGet()->GetBoundingSphere(bs);
            result = ray.DoesRaySphereIntersect(bs);
        }

        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    // ******************** TableSceneNode *****************************
    // /////////////////////////////////////////////////////////////////

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void TableSceneNode::Init() throw(GameException &)
    {
        ObjModelFileLoader objLoader;                       // 3D model parsing object.
        ModelLoadingProgress progressObj(ePoolTable, 5);    // Model loading progress reporting interface.
        BoundingCube tableMeshBB;                           // BoundingBoxes for the various pool table meshes.

        TriangleMesh tableMesh;
        LoadMeshFromResourceCache(m_param.GetMeshName(), &objLoader, progressObj, tableMesh);
        CalculateTriangleListBoundingBox(tableMesh, tableMeshBB);

        // Create the child mesh.
        boost::shared_ptr<GLTriangleBatch> tableBatch = ConvertTriangleListToBatch(tableMesh, &progressObj, true);
        if(!tableBatch) {
            throw GameException(std::string("Failed to load pool table mesh"));
        }

        progressObj.NextStage();

        CommonBatchSceneNode::SetBatch(tableBatch);
        // Calculate and set the radius of the table scene node from the table mesh.
        SceneNode::SetRadius(CmMax<F32>(g_originPt.Distance(tableMeshBB.GetMin()), g_originPt.Distance(tableMeshBB.GetMax())));

        // Calculate the actual width of the pool table floor.
        F32 tableFloorWidth = 0.0f, projX = -FLT_MAX;
        Vertex currVertex;
        for(TriangleList::iterator i = tableMesh.begin(), end = tableMesh.end(); i != end; ++i) {
            for(I32 vi = 0; vi < Triangle::eNumberVertices; ++vi) {
                (*i)->GetVertex(Triangle::VertexId(vi), currVertex);
                Vector3 posVec(currVertex.GetPosition());

                F32 projX = posVec.Dot(g_right);
                if(projX > tableFloorWidth) {
                    tableFloorWidth = projX;
                }
            }
        }
        tableFloorWidth *= 2.0f;
        F32 tw = tableMeshBB.GetWidth();

        F32 mpDepth, pr;
        if(!InitPockets(objLoader, progressObj, tableMeshBB, mpDepth, pr)) {
            throw GameException(std::string("Failed to load pool table pocket meshes"));
        }
        if(!InitPanels(objLoader, progressObj, tableMeshBB, mpDepth)) {
            throw GameException(std::string("Failed to load pool table panel meshes"));
        }

        // Notify logic layer that mesh has been read in (it needs some mesh dimensions before it can create the physics representation of the table).
        m_param.SetWidth(tableFloorWidth);
        m_param.SetHeight(tableMeshBB.GetHeight());
        m_param.SetDepth(tableMeshBB.GetDepth() - (tableMeshBB.GetWidth() - tableFloorWidth));
        m_param.SetPocketRadius(pr);
        IEventDataPtr eventPtr(GCC_NEW EvtData_Complex_Mesh_Loaded(&m_param));
        safeQueEvent(eventPtr);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TableSceneNode::InitPanels(ObjModelFileLoader &objLoader, ModelLoadingProgress &progressObj, const BoundingCube &tbb, const F32 mpDepth)
    {
        BoundingCube fpBB, spBB;    // BoundingBoxes for the various pool table meshes.

        // Load meshes into VBOs
        boost::shared_ptr<IGLBatchBase> frontPanelBatch = LoadBatchFromResourceCache(m_param.GetFrontPanelMeshName(), &objLoader, progressObj, fpBB);
        boost::shared_ptr<IGLBatchBase> sidePanelBatch = LoadBatchFromResourceCache(m_param.GetSidePanelMeshName(), &objLoader, progressObj, spBB);
        if(!frontPanelBatch || !sidePanelBatch) {
            return (false);
        }

        // Create and add all the child scene nodes to the Pool tables children list placing them in their appropriate positions around the table in World space.
        boost::optional<ActorId> nullId;                // Null Actor ID, child nodes of pool table..
        Matrix4 childTransMat;                          // Final position and orientation of each child scene node.
        Matrix4 rotMatrix;                              // Rotation matrix to apply to orient each SN mesh.
        F32 tableWHalf = tbb.GetWidth() / 2.0f;     // Half width table mesh.
        F32 tableDHalf = tbb.GetDepth() / 2.0f;     // Half depth table mesh.
        F32 tableHHalf = tbb.GetHeight() / 2.0f;        // Half height table mesh.
        const F32 padding = 0.005f;

        // Add both front and back panel children.
        {
            const F32 halfFpDepth = fpBB.GetDepth() / 2.0f;

            // Create and add FrontPanel child SN.
            GameHalloran::BuildTranslationMatrix4(childTransMat, 0.0f, tableHHalf, tableDHalf + halfFpDepth - padding);
            boost::shared_ptr<ISceneNode> frontPanel(GCC_NEW CommonBatchSceneNode(m_sgmPtr, nullId, std::string("FrontPanel"), RenderPassStatic, m_param.GetTablePanelMaterial(), childTransMat, m_param.GetPanelTextureName(), m_param.GetShaderName(), frontPanelBatch));
            SceneNode::VAddChild(frontPanel);

            // Create and add BackPanel child SN.
            GameHalloran::BuildTranslationMatrix4(childTransMat, 0.0f, tableHHalf, -tableDHalf - halfFpDepth + padding);
            GameHalloran::BuildRotationYMatrix4(rotMatrix, 180.0f);
            childTransMat *= rotMatrix;
            boost::shared_ptr<ISceneNode> backPanel(GCC_NEW CommonBatchSceneNode(m_sgmPtr, nullId, std::string("BackPanel"), RenderPassStatic, m_param.GetTablePanelMaterial(), childTransMat, m_param.GetPanelTextureName(), m_param.GetShaderName(), frontPanelBatch));
            SceneNode::VAddChild(backPanel);
        }

        // Add 4 side panels between table pockets.
        {
            const F32 spHalfW = spBB.GetWidth() / 2.0f;
            const F32 spHalfD = spBB.GetDepth() / 2.0f;

            // Create and add Front Left SidePanel child SN.
            GameHalloran::BuildTranslationMatrix4(childTransMat, -tableWHalf - spHalfW + padding, tableHHalf, spHalfD + mpDepth);
            boost::shared_ptr<ISceneNode> frontLeftPanel(GCC_NEW CommonBatchSceneNode(m_sgmPtr, nullId, std::string("FrontLeftPanel"), RenderPassStatic, m_param.GetTablePanelMaterial(), childTransMat, m_param.GetPanelTextureName(), m_param.GetShaderName(), sidePanelBatch));
            SceneNode::VAddChild(frontLeftPanel);

            // Create and add Back Left SidePanel child SN.
            GameHalloran::BuildTranslationMatrix4(childTransMat, -tableWHalf - spHalfW + padding, tableHHalf, -spHalfD - mpDepth);
            boost::shared_ptr<ISceneNode> backLeftPanel(GCC_NEW CommonBatchSceneNode(m_sgmPtr, nullId, std::string("BackLeftPanel"), RenderPassStatic, m_param.GetTablePanelMaterial(), childTransMat, m_param.GetPanelTextureName(), m_param.GetShaderName(), sidePanelBatch));
            SceneNode::VAddChild(backLeftPanel);

            GameHalloran::BuildRotationYMatrix4(rotMatrix, 180.0f);

            // Create and add Bottom Right SidePanel child SN.
            GameHalloran::BuildTranslationMatrix4(childTransMat, tableWHalf + spHalfW - padding, tableHHalf, spHalfD + mpDepth);
            childTransMat *= rotMatrix;
            boost::shared_ptr<ISceneNode> frontRightPanel(GCC_NEW CommonBatchSceneNode(m_sgmPtr, nullId, std::string("FrontRightPanel"), RenderPassStatic, m_param.GetTablePanelMaterial(), childTransMat, m_param.GetPanelTextureName(), m_param.GetShaderName(), sidePanelBatch));
            SceneNode::VAddChild(frontRightPanel);

            // Create and add Top Right SidePanel child SN.
            GameHalloran::BuildTranslationMatrix4(childTransMat, tableWHalf + spHalfW - padding, tableHHalf, -spHalfD - mpDepth);
            childTransMat *= rotMatrix;
            boost::shared_ptr<ISceneNode> backRightPanel(GCC_NEW CommonBatchSceneNode(m_sgmPtr, nullId, std::string("BackRightPanel"), RenderPassStatic, m_param.GetTablePanelMaterial(), childTransMat, m_param.GetPanelTextureName(), m_param.GetShaderName(), sidePanelBatch));
            SceneNode::VAddChild(backRightPanel);
        }

        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TableSceneNode::InitPockets(ObjModelFileLoader &objLoader, ModelLoadingProgress &progressObj, const BoundingCube &tbb, F32 &mpDepth, F32 &pr)
    {
        BoundingCube mpBB, cpBB;    // BoundingBoxes for the various pool table meshes.

        // Parse and load the middle pocket mesh.
        TriangleMesh mpMesh;
        LoadMeshFromResourceCache(m_param.GetMiddlePocketMeshName(), &objLoader, progressObj, mpMesh);
        if(mpMesh.empty()) {
            return (false);
        }

        // Calculate the BB of the middle pocket.
        GameHalloran::CalculateTriangleListBoundingBox(mpMesh, mpBB);
        boost::shared_ptr<GLTriangleBatch> middlePocketBatch = ConvertTriangleListToBatch(mpMesh, &progressObj);
        progressObj.NextStage();

        F32 pocketRadius;           // Radius of a pool pocket.
        // Calculate the radius of the pool table pocket drop areas.
        Vertex currVertex;
        pocketRadius = -FLT_MAX;
        for(TriangleList::iterator i = mpMesh.begin(), end = mpMesh.end(); i != end; ++i) {
            for(I32 vi = 0; vi < Triangle::eNumberVertices; ++vi) {
                (*i)->GetVertex(Triangle::VertexId(vi), currVertex);
                Vector3 posVec(currVertex.GetPosition());

                F32 projX = posVec.Dot(g_right);
                if(projX > pocketRadius) {
                    pocketRadius = projX;
                }
            }
        }
        pr = pocketRadius;

        // Load the corner and middle pocket batches into VBOs.
        boost::shared_ptr<IGLBatchBase> cornerPocketBatch = LoadBatchFromResourceCache(m_param.GetCornerPocketMeshName(), &objLoader, progressObj, cpBB);
        if(!cornerPocketBatch || !middlePocketBatch) {
            return (false);
        }

        // Create and add all the child scene nodes to the Pool tables children list placing them in their appropriate positions around the table in World space.
        boost::optional<ActorId> nullId;                // Null Actor ID, child nodes of pool table..
        Matrix4 childTransMat;                          // Final position and orientation of each child scene node.
        Matrix4 rotMatrix;                              // Rotation matrix to apply to orient each SN mesh.
        F32 tableWHalf = tbb.GetWidth() / 2.0f;     // Half width table mesh.
        F32 tableDHalf = tbb.GetDepth() / 2.0f;     // Half depth table mesh.
        F32 tableHHalf = tbb.GetHeight() / 2.0f;        // Half height table mesh.
        const F32 padding = 0.005f;

        // Add 2 Middle pockets.
        {
            const F32 mpHalfW = mpBB.GetWidth() / 2.0f;
            const F32 mpHalfH = mpBB.GetHeight() / 2.0f;
            const F32 mpHalfD = mpBB.GetDepth() / 2.0f;
            const F32 diff = mpBB.GetWidth() - (pocketRadius * 2.0f);

            mpDepth = mpHalfD;

            // Create and add Left middle pocket child SN.
            GameHalloran::BuildTranslationMatrix4(childTransMat, -tableWHalf + pocketRadius - diff + padding, -mpHalfH + tbb.GetHeight(), 0.0f);
            boost::shared_ptr<ISceneNode> middleLeftPocket(GCC_NEW CommonBatchSceneNode(m_sgmPtr, nullId, std::string("MiddleLeftPocket"), RenderPassStatic, m_param.GetTablePocketMaterial(), childTransMat, m_param.GetPocketTextureName(), m_param.GetShaderName(), middlePocketBatch));
            SceneNode::VAddChild(middleLeftPocket);

            // Create and add Right middle pocket child SN.
            GameHalloran::BuildTranslationMatrix4(childTransMat, tableWHalf - pocketRadius + diff - padding, -mpHalfH + tbb.GetHeight(), 0.0f);
            GameHalloran::BuildRotationYMatrix4(rotMatrix, 180.0f);
            childTransMat *= rotMatrix;
            boost::shared_ptr<ISceneNode> middleRightPocket(GCC_NEW CommonBatchSceneNode(m_sgmPtr, nullId, std::string("MiddleRightPocket"), RenderPassStatic, m_param.GetTablePocketMaterial(), childTransMat, m_param.GetPocketTextureName(), m_param.GetShaderName(), middlePocketBatch));
            SceneNode::VAddChild(middleRightPocket);
        }

        // Add all 4 corner pockets.
        {
            const F32 cpHalfW = cpBB.GetWidth() / 2.0f;
            const F32 cpHalfH = cpBB.GetHeight() / 2.0f;
            const F32 cpHalfD = cpBB.GetDepth() / 2.0f;

            const F32 diff = cpBB.GetWidth() - (pocketRadius * 2.0f);

            // Create and add bottom Left pocket child SN.
            GameHalloran::BuildTranslationMatrix4(childTransMat, -tableWHalf + cpHalfW - diff + padding, -cpHalfH + tbb.GetHeight(), tableDHalf - cpHalfD + diff);
            boost::shared_ptr<ISceneNode> bottomLeftPocket(GCC_NEW CommonBatchSceneNode(m_sgmPtr, nullId, std::string("BottomLeftPocket"), RenderPassStatic, m_param.GetTablePocketMaterial(), childTransMat, m_param.GetPocketTextureName(), m_param.GetShaderName(), cornerPocketBatch));
            SceneNode::VAddChild(bottomLeftPocket);

            // Create and add top Left pocket child SN.
            GameHalloran::BuildTranslationMatrix4(childTransMat, -tableWHalf + cpHalfW - diff + padding, -cpHalfH + tbb.GetHeight(), -tableDHalf + cpHalfD - diff);
            GameHalloran::BuildRotationYMatrix4(rotMatrix, -90.0f);
            childTransMat *= rotMatrix;
            boost::shared_ptr<ISceneNode> topLeftPocket(GCC_NEW CommonBatchSceneNode(m_sgmPtr, nullId, std::string("TopLeftPocket"), RenderPassStatic, m_param.GetTablePocketMaterial(), childTransMat, m_param.GetPocketTextureName(), m_param.GetShaderName(), cornerPocketBatch));
            SceneNode::VAddChild(topLeftPocket);

            // Create and add bottom Right pocket child SN.
            GameHalloran::BuildTranslationMatrix4(childTransMat, tableWHalf - cpHalfW + diff - padding, -cpHalfH + tbb.GetHeight(), tableDHalf - cpHalfD + diff);
            GameHalloran::BuildRotationYMatrix4(rotMatrix, -270.0f);
            childTransMat *= rotMatrix;
            boost::shared_ptr<ISceneNode> bottomRightPocket(GCC_NEW CommonBatchSceneNode(m_sgmPtr, nullId, std::string("BottomRightPocket"), RenderPassStatic, m_param.GetTablePocketMaterial(), childTransMat, m_param.GetPocketTextureName(), m_param.GetShaderName(), cornerPocketBatch));
            SceneNode::VAddChild(bottomRightPocket);

            // Create and add top Right pocket child SN.
            GameHalloran::BuildTranslationMatrix4(childTransMat, tableWHalf - cpHalfW + diff - padding, -cpHalfH + tbb.GetHeight(), -tableDHalf + cpHalfD - diff);
            GameHalloran::BuildRotationYMatrix4(rotMatrix, -180.0f);
            childTransMat *= rotMatrix;
            boost::shared_ptr<ISceneNode> topRightPocket(GCC_NEW CommonBatchSceneNode(m_sgmPtr, nullId, std::string("TopRightPocket"), RenderPassStatic, m_param.GetTablePocketMaterial(), childTransMat, m_param.GetPocketTextureName(), m_param.GetShaderName(), cornerPocketBatch));
            SceneNode::VAddChild(topRightPocket);
        }

        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    TableSceneNode::TableSceneNode(SceneGraphManager *sgPtr, const boost::optional<ActorId> actorId, const Material &material, const Matrix4 &toWorld, const std::string &textureName, const std::string &shaderNameRef, TableActorParams &param)\
    throw(GameException &) : CommonBatchSceneNode(sgPtr, \
                actorId, \
                std::string("PoolTable"), \
                RenderPassStatic, \
                material, \
                toWorld, \
                textureName, \
                shaderNameRef, \
                boost::shared_ptr<IGLBatchBase>()), m_param(param)
    {
        Init();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    TableSceneNode::TableSceneNode(SceneGraphManager *sgPtr, const boost::optional<ActorId> actorId, const Material &material, const Matrix4 &toWorld, const Matrix4 &fromWorld, const std::string &textureName, const std::string &shaderNameRef, TableActorParams &param)\
    throw(GameException &) : CommonBatchSceneNode(sgPtr, \
                actorId, \
                std::string("PoolTable"), \
                RenderPassStatic, \
                material, \
                toWorld, \
                fromWorld, \
                textureName, \
                shaderNameRef, \
                boost::shared_ptr<IGLBatchBase>()), m_param(param)
    {
        Init();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    TableSceneNode::~TableSceneNode()
    {

    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TableSceneNode::VRenderChildren()
    {
        bool result = true;

        // Render table and all children if table is visible.
        if(VIsVisible()) {
            for(SceneNodeList::iterator i = m_children.begin(), end = m_children.end(); i != end; ++i) {
                SceneNode::RenderSceneNode((*i).get());
                result = (*i)->VRenderChildren();
            }
        }

        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TableSceneNode::VOnRestore()
    {
        bool result = CommonBatchSceneNode::VOnRestore();

        if(result) {
            m_children.clear();
            try {
                Init();
            } catch(...) {
                result = false;
            }
        }

        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TableSceneNode::VOnLostDevice()
    {
        return (CommonBatchSceneNode::VOnLostDevice());
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TableSceneNode::VPick(const RayCast &ray)
    {
        bool result = false;
        result = SceneNode::VPick(ray);

        if(!result) {
            BoundingCube bb;
            VGet()->GetBoundingBox(bb);
            result = ray.DoesRayCubeIntersect(bb);
        }

        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    // ********************* CueSceneNode ******************************
    // /////////////////////////////////////////////////////////////////

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void CueSceneNode::Init() throw(GameException &)
    {
        ObjModelFileLoader objLoader;                       // 3D mesh loading object.
        ModelLoadingProgress loadProgressObj(ePoolCue);     // Proress update interface.
        BoundingCube bb;                                    // BoundingBox for cue mesh.

        boost::shared_ptr<IGLBatchBase> cueBatch = LoadBatchFromResourceCache(m_param.GetMeshName(), &objLoader, loadProgressObj, bb);
        if(!cueBatch) {
            throw GameException(std::string("Failed to load cue mesh"));
        }
        CommonBatchSceneNode::SetBatch(cueBatch);

        // Calculate the radius of the cue scene node.
        F32 maxWH = CmMax<F32>(bb.GetWidth(), bb.GetHeight());
        F32 circumference = CmMax<F32>(maxWH, bb.GetDepth());
        SceneNode::SetRadius(circumference / 2.0f);

        // Inform logic layer that the mesh has been loaded so that it may use the dimensions we found here to create the physics cue shape.
        m_param.SetWidth(bb.GetWidth());
        m_param.SetHeight(bb.GetHeight());
        m_param.SetDepth(bb.GetDepth());
        IEventDataPtr eventPtr(GCC_NEW EvtData_Complex_Mesh_Loaded(&m_param));
        safeQueEvent(eventPtr);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    CueSceneNode::CueSceneNode(SceneGraphManager *sgPtr, const boost::optional<ActorId> actorId, const Material &material, const Matrix4 &toWorld, const std::string &textureName, const std::string &shaderNameRef, CueActorParams &param)\
    throw(GameException &) : CommonBatchSceneNode(sgPtr, \
                actorId, \
                std::string("PoolCue"), \
                RenderPassActor, \
                material, \
                toWorld, \
                textureName, \
                shaderNameRef, \
                boost::shared_ptr<IGLBatchBase>()), m_param(param)
    {
        Init();
    }


    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    CueSceneNode::CueSceneNode(SceneGraphManager *sgPtr, const boost::optional<ActorId> actorId, const Material &material, const Matrix4 &toWorld, const Matrix4 &fromWorld, const std::string &textureName, const std::string &shaderNameRef, CueActorParams &param)\
    throw(GameException &) : CommonBatchSceneNode(sgPtr, \
                actorId, \
                std::string("PoolCue"), \
                RenderPassActor, \
                material, \
                toWorld, \
                fromWorld, \
                textureName, \
                shaderNameRef, \
                boost::shared_ptr<IGLBatchBase>()), m_param(param)
    {
        Init();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    CueSceneNode::~CueSceneNode()
    {

    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool CueSceneNode::VOnRestore()
    {
        bool result = CommonBatchSceneNode::VOnRestore();

        // Actor parameters may have changed so we regenerate the cue geometry now.
        //  This is to handle the UpdateActorParams events that may be recevied from lua
        //   scripts during runtime.
        if(result) {
            try {
                Init();
            } catch(...) {
                result = false;
            }
        }

        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool CueSceneNode::VOnLostDevice()
    {
        return (CommonBatchSceneNode::VOnLostDevice());
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool CueSceneNode::VPick(const RayCast &ray)
    {
        bool result = false;
        result = SceneNode::VPick(ray);

        if(!result) {
            BoundingCube bb;
            VGet()->GetBoundingBox(bb);
            result = ray.DoesRayCubeIntersect(bb);
        }

        return (result);
    }

}
