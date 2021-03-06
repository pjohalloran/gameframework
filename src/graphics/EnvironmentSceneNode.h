#pragma once
#ifndef __GF_ENVIRONMENT_SCENE_NODE_H
#define __GF_ENVIRONMENT_SCENE_NODE_H

// /////////////////////////////////////////////////////////////////
// @file EnvironmentSceneNode.h
// @author PJ O Halloran
// @date 06/10/2010
//
// Contains the EnvironmentSceneNode classes definition.
//
// /////////////////////////////////////////////////////////////////

#ifdef WIN32
#   pragma warning( push )
#   pragma warning( disable:4290 )
#endif

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include <string>
#include <vector>

#include "GameBase.h"
#include "ISceneNode.h"
#include "SceneNodeProperties.h"
#include "SceneNode.h"
#include "TextureManager.h"
#include "GLBatch.h"
#include "GameException.h"
#include "GLSLShader.h"

namespace GameHalloran {
    class SceneGraphManager;

    // /////////////////////////////////////////////////////////////////
    // @class EnvironmentSceneNode
    // @author PJ O Halloran
    //
    // A scene node that renders an environment using a CubeMap.
    // The environment is intended to be off in the very far distance.
    // The camera may rotate around and look at the cubemap from different
    // angles but the camera can never translate or move closer to the edge
    // of the cubemap.  This is to give the appearence of an environment
    // that is infinitely far away such as mountains in the distance or
    // the sky.
    //
    // It uses the Environment vertex and point shader which is located
    // in the resource cache and is loaded and compiled at runtime.
    //
    // This shader and class is adpated from the SkyBox shader and example
    // from the OpenGL Superbible, 5th Edition, chapter 7, pg. 297.
    //
    // /////////////////////////////////////////////////////////////////
    class EnvironmentSceneNode : public SceneNode {
    private:
        TexHandle m_texHandle;                                      ///< Handle to the CubeMap texture.
        GLBatch m_cubeBatch;                                        ///< Batch of geometry describing a 3D cube.
        ShaderUniformSPtr m_mvpUniform;                             ///< MVP uniform location.
        ShaderUniformSPtr m_cmUniform;                              ///< Cubemap texture location.

        // /////////////////////////////////////////////////////////////////
        // Constructor helper function.
        //
        // @param cubemapTextureNameVec Vector of 6 texture names to apply to
        //                              the sides of the cubemap.
        // @param shaderNameRef Name of the shader to render the node.
        // @param cmRadius Radius of the cubemaps 3D cube.
        //
        // @throw GameException& If the vector does not contain 6 valid texture
        //                          names.
        //
        // /////////////////////////////////////////////////////////////////
        void Init(const std::vector<std::string> &cubemapTextureNameVec, const std::string &shaderNameRef, const F32 cmRadius) throw(GameException &);

    protected:



    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param sgPtr SG manager pointer.
        // @param actorId The optional Actor ID.
        // @param toWorld Position and orientation of node relative to its
        //                  parent.
        // @param cubemapTextureNameVec Vector of 6 texture names to apply to
        //                              the sides of the cubemap.
        // @param shaderNameRef Name of the shader to render the node.
        // @param cmRadius Radius of the cubemaps 3D cube.
        //
        // @throw GameException& If the vector does not contain 6 valid texture
        //                          names.
        //
        // /////////////////////////////////////////////////////////////////
        explicit EnvironmentSceneNode(SceneGraphManager *sgPtr,
                                      boost::optional<ActorId> actorId,
                                      const Matrix4 &toWorld,
                                      const std::vector<std::string> &cubemapTextureNameVec,
                                      const std::string &shaderNameRef,
                                      const F32 cmRadius) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param sgPtr SG manager pointer.
        // @param actorId The optional Actor ID.
        // @param toWorld Position and orientation of node relative to its
        //                  parent.
        // @param fromWorld Inverse of toWorld matrix.
        // @param cubemapTextureNameVec Vector of 6 texture names to apply to
        //                              the sides of the cubemap.
        // @param shaderNameRef Name of the shader to render the node.
        // @param cmRadius Radius of the cubemaps 3D cube.
        //
        // @throw GameException& If the vector does not contain 6 valid texture
        //                          names.
        //
        // /////////////////////////////////////////////////////////////////
        explicit EnvironmentSceneNode(SceneGraphManager *sgPtr,
                                      boost::optional<ActorId> actorId,
                                      const Matrix4 &toWorld,
                                      const Matrix4 &fromWorld,
                                      const std::vector<std::string> &cubemapTextureNameVec,
                                      const std::string &shaderNameRef,
                                      const F32 cmRadius) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~EnvironmentSceneNode();

        // /////////////////////////////////////////////////////////////////
        // Set the render state before rendering.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VPreRender();

        // /////////////////////////////////////////////////////////////////
        // Render the node.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VRender();

        // /////////////////////////////////////////////////////////////////
        // Reset the render state after rendering.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VPostRender();

        // /////////////////////////////////////////////////////////////////
        // Overridden and disabled for environment node.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VAddChild(boost::shared_ptr<ISceneNode> childNodePtr) {
            return (true);
        };

        // /////////////////////////////////////////////////////////////////
        // Overridden and disabled for environment node.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VRemoveChild(const ActorId id) {
            return (true);
        };

        // /////////////////////////////////////////////////////////////////
        // Overridden and disabled for environment node.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VRenderChildren() {
            return (true);
        };

        // /////////////////////////////////////////////////////////////////
        // Called when application is restored.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnRestore();

        // /////////////////////////////////////////////////////////////////
        // Called when application loses focus.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnLostDevice();

        // /////////////////////////////////////////////////////////////////
        // The Environment node is rendering the extremities of the environment
        // and so should always be visible.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VIsVisible() const {
            return (true);
        };

        // /////////////////////////////////////////////////////////////////
        // Always returns false as you can't "pick" the background!
        //
        // @param ray Raycast.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VPick(const RayCast &ray) {
            return (false);
        };

        // /////////////////////////////////////////////////////////////////
        // Updates the node once per main loop.
        //
        // @param elapsedTime The number of seconds since the last update.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnUpdate(const F32 elapsedTime);

    };

}

#ifdef WIN32
#   pragma warning( pop )
#endif

#endif
