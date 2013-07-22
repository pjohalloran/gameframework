// /////////////////////////////////////////////////////////////////
// @file EnvironmentSceneNode.cpp
// @author PJ O Halloran
// @date 06/10/2010
//
// Contains the EnvironmentSceneNode classes implementaion.
//
// /////////////////////////////////////////////////////////////////

#include <boost/optional.hpp>

#include "EnvironmentSceneNode.h"
#include "SceneGraphManager.h"
#include "GameMain.h"
#include "GLTools.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void EnvironmentSceneNode::Init(const std::vector<std::string> &cubemapTextureNameVec,
                                    const std::string &shaderNameRef,
                                    const F32 cmRadius) throw(GameException &)
    {
        // Load the textures for the sides of the cubemap.
        boost::optional<TexHandle> tHandle = g_appPtr->GetTextureManagerPtr()->LoadCubeMap(cubemapTextureNameVec, GL_CLAMP_TO_EDGE);
        if(!tHandle.is_initialized()) {
            throw GameException(std::string("Failed to load the textures for the EnvironmentMap"));
        }

        m_texHandle = *tHandle;

        SceneNode::SetRadius(cmRadius);

        // TODO: Remove dependancy on GLTools external lib.
        gltMakeCube(m_cubeBatch, cmRadius);

        // Set the shader name (The SGM should already have this shader built and included).
        SceneNode::SetShaderName(shaderNameRef);
        if(m_shaderPtr) {
            m_mvpUniform = m_shaderPtr->GetUniform("mvpMatrix");
            m_cmUniform = m_shaderPtr->GetUniform("cubeMap");
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    EnvironmentSceneNode::EnvironmentSceneNode(SceneGraphManager *sgPtr,
            boost::optional<ActorId> actorId,
            const Matrix4 &toWorld,
            const std::vector<std::string> &cubemapTextureNameVec,
            const std::string &shaderNameRef,
            const F32 cmRadius) throw(GameException &)
        : SceneNode(sgPtr, actorId, std::string("EnvironmentSceneNode"), RenderPassSky, Material(), toWorld)
        , m_texHandle(0)
        , m_mvpUniform()
        , m_cmUniform()
    {
        Init(cubemapTextureNameVec, shaderNameRef, cmRadius);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    EnvironmentSceneNode::EnvironmentSceneNode(SceneGraphManager *sgPtr,
            boost::optional<ActorId> actorId,
            const Matrix4 &toWorld,
            const Matrix4 &fromWorld,
            const std::vector<std::string> &cubemapTextureNameVec,
            const std::string &shaderNameRef,
            const F32 cmRadius) throw(GameException &)
        : SceneNode(sgPtr, actorId, std::string("EnvironmentSceneNode"), RenderPassSky, Material(), toWorld, fromWorld)
        , m_texHandle(0)
        , m_mvpUniform()
        , m_cmUniform()
    {
        Init(cubemapTextureNameVec, shaderNameRef, cmRadius);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    EnvironmentSceneNode::~EnvironmentSceneNode()
    {
        // TextureManager takes care of freeing the Textures.
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool EnvironmentSceneNode::VPreRender()
    {
        bool result = false;

        result = SceneNode::VPreRender();

        if(result && !g_appPtr->GetTextureManagerPtr()->Bind(m_texHandle, GL_TEXTURE_CUBE_MAP, GL_TEXTURE0)) {
            GF_LOG_TRACE_ERR("EnvironmentSceneNode::VPreRender()", "Failed to activate the CubeMap texture");
            result = false;
        }

        if(result) {
            glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

            // Get the camera matrix and clear the cameras position (we want to be able to rotate the environment box but not move it!).
            Matrix4 camMatrix(m_sgmPtr->GetCamera()->VGet()->GetToWorld());
            camMatrix[Matrix4::M30] = 0.0f;
            camMatrix[Matrix4::M31] = 0.0f;
            camMatrix[Matrix4::M32] = 0.0f;
            camMatrix[Matrix4::M33] = 1.0f;
            m_sgmPtr->GetStackManager()->GetModelViewMatrixStack()->LoadMatrix(camMatrix);
        }

        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool EnvironmentSceneNode::VRender()
    {
        bool result = SceneNode::VRender();

        if(result) {
            Matrix4 mvp;
            m_sgmPtr->GetStackManager()->GetModelViewProjectionMatrix(mvp);

            m_mvpUniform->SetValue((GLfloat * const)mvp.GetComponentsConst(), 16);
            m_cmUniform->SetValue(0);

            m_shaderPtr->Activate();
            m_cubeBatch.VDraw();

#if defined(DEBUG)
            if(g_appPtr->GetLoggerPtr() && g_appPtr->GetLoggerPtr()->GetLogLevel() >= GameLog::DEB) {
                GLenum error = glGetError();
                if(error != GL_NO_ERROR) {
                    GF_LOG_TRACE_ERR("EnvironmentSceneNode::VRender()", "The rendering of the cubemap failed");
                    result = false;
                }
            }
#endif
        }

        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool EnvironmentSceneNode::VPostRender()
    {
        glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        return (SceneNode::VPostRender());
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool EnvironmentSceneNode::VOnRestore()
    {
        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool EnvironmentSceneNode::VOnLostDevice()
    {
        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool EnvironmentSceneNode::VOnUpdate(const F32 elapsedTime)
    {
        return (true);
    }

}
