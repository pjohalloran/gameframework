// /////////////////////////////////////////////////////////////////
// @file SnowParticleSystem.cpp
// @author PJ O Halloran
// @date 29/09/2010
//
// File contains the implementation for the SnowParticleSystem class.
//
// /////////////////////////////////////////////////////////////////

#include <string>

#include "SnowParticleSystem.h"
#include "TextResource.h"
#include "GameMain.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void SnowParticleSystem::PrepareShader()
    {
        TextResource vpRes(std::string("SnowPointShader.vp"));          // ID of the vertex shader in the resource file.
        TextResource fpRes(std::string("SnowPointShader.fp"));          // ID of the fragment shader in the resource file.

        // Get vertex and fragment shader programs from the resource file.
        boost::shared_ptr<TextResHandle> vpSrcH = boost::static_pointer_cast<TextResHandle>(g_appPtr->GetResourceCache()->GetHandle(&vpRes));
        boost::shared_ptr<TextResHandle> fpSrcH = boost::static_pointer_cast<TextResHandle>(g_appPtr->GetResourceCache()->GetHandle(&fpRes));
        if(!vpSrcH || !fpSrcH) {
            GF_LOG_TRACE_ERR("SnowParticleSystem::PrepareShader()", "Failed to retrieve the SnowPointShader programs from the resource cache");
            return;
        }

        if(!vpSrcH->VInitialize()) {
            GF_LOG_TRACE_ERR("SnowParticleSystem::PrepareShader()", "Failed to initialize the SnowPointShader.vp shader source handle");
            return;
        }
        if(!fpSrcH->VInitialize()) {
            GF_LOG_TRACE_ERR("SnowParticleSystem::PrepareShader()", "Failed to initialize the SnowPointShader.fp shader source handle");
            return;
        }

        // Build shader.
        VSAttributeNameList attList;
        attList.push_back(std::string("vertexPos"));
        std::string errorMsg;
        if(!m_shaderProg.Build(vpSrcH->GetTextBuffer(), fpSrcH->GetTextBuffer(), attList, errorMsg)) {
            GF_LOG_TRACE_ERR("SnowParticleSystem::PrepareShader()", std::string("Failed to build the SnowPointShader: ") + errorMsg);
            return;
        }

        m_shaderProg.Activate();

        // Get uniform locations
        m_mvpMatrixLoc = m_shaderProg.GetUniformLocation("mvpMatrix");
        if(m_mvpMatrixLoc == -1) {
            GF_LOG_TRACE_ERR("SnowParticleSystem::PrepareShader()", "Failed to find the mvpMatrix in the SnowPointShader");
        }
        m_snowColorLoc = m_shaderProg.GetUniformLocation("snowColor");
        if(m_snowColorLoc == -1) {
            GF_LOG_TRACE_ERR("SnowParticleSystem::PrepareShader()", "Failed to find the snowColor in the SnowPointShader");
        }
        m_pointImageLoc = m_shaderProg.GetUniformLocation("pointImage");
        if(m_pointImageLoc == -1) {
            GF_LOG_TRACE_ERR("SnowParticleSystem::PrepareShader()", "Failed to find the pointImage in the SnowPointShader");
        }
        m_mvMatrixLoc = m_shaderProg.GetUniformLocation("mvMatrix");
        if(m_mvMatrixLoc == -1) {
            GF_LOG_TRACE_ERR("SnowParticleSystem::PrepareShader()", "Failed to find the mvMatrix in the SnowPointShader");
        }
        m_cameraPosLoc = m_shaderProg.GetUniformLocation("cameraPos");
        if(m_cameraPosLoc == -1) {
            GF_LOG_TRACE_ERR("SnowParticleSystem::PrepareShader()", "Failed to find the cameraPos in the SnowPointShader");
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    SnowParticleSystem::SnowParticleSystem(boost::shared_ptr<ModelViewProjStackManager> mvpStackPtr,
                                           const GLFrame &cameraFrame)
        : ParticleSystem()
        , m_mvpMatrixLoc(0)
        , m_snowColorLoc(0)
        , m_pointImageLoc(0)
        , m_mvMatrixLoc(0)
        , m_cameraPosLoc(0)
        , m_state(SNOW_CONSTANT)
        , m_mvpStackPtr(mvpStackPtr)
        , m_cameraFrame(cameraFrame)
    {
        PrepareShader();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    SnowParticleSystem::SnowParticleSystem(const BoundingCube &bb,
                                           const F32 emitRate,
                                           const U32 maxParticles,
                                           const ImageResource &textureResource,
                                           const Vector3 &windVec,
                                           const F32 gravity,
                                           const U32 numParticles,
                                           boost::shared_ptr<ModelViewProjStackManager> mvpStackPtr,
                                           GLShaderManager &stockShaders,
                                           const GLFrame &cameraFrame)
        : ParticleSystem(Point3(), bb, emitRate, maxParticles, textureResource, windVec, gravity)
        , m_mvpMatrixLoc(0)
        , m_snowColorLoc(0)
        , m_pointImageLoc(0)
        , m_mvMatrixLoc(0)
        , m_cameraPosLoc(0)
        , m_state(SNOW_CONSTANT)
        , m_mvpStackPtr(mvpStackPtr)
        , m_stockShaders(stockShaders)
        , m_cameraFrame(cameraFrame)
    {
        PrepareShader();

        const U32 COUNT = (numParticles > maxParticles ? maxParticles : numParticles);
        for(U32 i = 0; i < COUNT; ++i) {
            ParticleSystem::VAddParticle();
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    SnowParticleSystem::~SnowParticleSystem()
    {

    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SnowParticleSystem::VOnRestore()
    {
        bool result = ParticleSystem::VOnRestore();
        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void SnowParticleSystem::VOnLostDevice()
    {
        ParticleSystem::VOnLostDevice();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SnowParticleSystem::VOnPreRender()
    {
        bool result = ParticleSystem::VOnPreRender();
        // Snow specific operations...
        m_shaderProg.Activate();

        Matrix4 mvp, mv;
        m_mvpStackPtr->GetModelViewProjectionMatrix(mvp);
        m_mvpStackPtr->GetModelViewMatrixStack()->GetMatrix(mv);

        glUniformMatrix4fv(m_mvpMatrixLoc, 1, GL_FALSE, mvp.GetComponentsConst());
        Vector4 whiteAplha(g_gcWhite);
        whiteAplha.SetW(0.25f);
        glUniform4fv(m_snowColorLoc, 1, whiteAplha.GetComponentsConst());
        glUniformMatrix4fv(m_mvMatrixLoc, 1, GL_FALSE, mv.GetComponentsConst());
        glUniform4fv(m_cameraPosLoc, 1, Vector4(m_cameraFrame.GetPosition()).GetComponentsConst());
        glUniform1i(m_pointImageLoc, 0);

        // Turn on additive blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDisable(GL_DEPTH_TEST);

        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SnowParticleSystem::VOnRender(const F64 time, const F32 elapsedTime)
    {
        bool result = ParticleSystem::VOnRender(time, elapsedTime);
        // Snow specific operations.

        if(!m_list.empty() && m_state != SNOW_STOP) {
            // TODO: Submit batches of geometry more efficiently (1 - Dont add one at a time, 2 - Submit batches of points all together to keep GPU and CPU busy)...
            // TODO: Add texture units...
            GLBatch tmpBatch;
            tmpBatch.Begin(GL_POINTS, static_cast<GLuint>(m_list.size()));

            //m_pointSpritesBatch.Reset();
            tmpBatch.Begin(GL_POINTS, static_cast<GLuint>(m_list.size()));
            U32 batchIndex = 0;
            for(ParticleList::iterator curr = m_list.begin(), end = m_list.end(); curr != end; ++curr, ++batchIndex) {
                if((*curr)->IsAlive()) {
                    Point3 currPos = (*curr)->GetPosition();
                    //Vector4 currColor = (*curr)->GetColor();
                    tmpBatch.Vertex3f(currPos.GetX(), currPos.GetY(), currPos.GetZ());
                    //tmpBatch.Color4f(currColor.GetX(), currColor.GetY(), currColor.GetZ(), currColor.GetW());
                    //tmpBatch.MultiTexCoord2fv(m_textureId, Vector3().GetComponentsConst());
                }
            }
            tmpBatch.End();

            //Matrix4 mvp;
            //m_mvpStackPtr->GetModelViewProjectionMatrix(mvp);
            //m_stockShaders.UseStockShader(GLT_SHADER_FLAT, mvp.GetComponentsConst(), GameHalloran::g_gcWhite.GetComponentsConst());

            tmpBatch.VDraw();
        }

        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SnowParticleSystem::VOnPostRender()
    {
        bool result = ParticleSystem::VOnPostRender();
        // Snow specific operations...
        glDisable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);

        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void SnowParticleSystem::VOnUpdate(const F32 elapsedTime)
    {
        // TODO: Update this method depending on the current state of the snowfall... (m_state)

        for(ParticleList::iterator curr = m_list.begin(), end = m_list.end(); curr != end; ++curr) {
            if((*curr)->IsAlive()) {
                // Get velocity of snow flake and apply the current wind vector to it.
                Vector3 velocityVec((*curr)->GetVelocity());
                velocityVec += ParticleSystem::GetWindDirection();

                // Update the position of the snowflake depending on its calculated velocity and the time elapsed.
                Point3 currPos((*curr)->GetPosition());
                currPos += velocityVec * elapsedTime;
                (*curr)->SetPosition(currPos);

                // If the snowflake leavees the area then recycle it.
                if(!ParticleSystem::GetBoundingCube().IsPointInside(currPos)) {
                    VResetParticle(*curr);
                }
            }
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void SnowParticleSystem::VResetParticle(boost::shared_ptr<Particle> particlePtr)
    {
        if(!particlePtr) {
            GF_LOG_TRACE_ERR("SnowParticleSystem::VResetParticle()", "NULL pointer given to reset method");
            return;
        }

        // Generate random size, position velocity and acceleration and set color to white.
        F32 size = (m_rng.Random() * 4.0f) + 1.0f;
        Vector4 color(GameHalloran::g_gcWhite);

        // Starts from the same height but random X and Z values inside the BB.
        BoundingCube bb = ParticleSystem::GetBoundingCube();
        Point3 pos;
        Point3 minPt(bb.GetMin()), maxPt(bb.GetMax());
        GameHalloran::GenerateRandomPoint3(pos, m_rng, minPt, maxPt);
        pos.SetY(bb.GetMax().GetY());

        // Max velocity of 2.0 in the X or Z directions (Y is taken care of by gravity).
        Vector3 velocity;
        Vector3 minVel(-2.0f, 0.0f, -2.0f), maxVel(0.0f, 0.0f, 0.0f);
        GameHalloran::GenerateRandomVector3(velocity, m_rng, minVel, maxVel);
        // Ensure gravity has a negative effect on the Y position of the snowflake.
        F32 grav = ParticleSystem::GetGravity();
        F32 finalGrav = (grav >= 0.0f ? -grav : grav);
        finalGrav = (finalGrav * m_rng.Random()) + finalGrav;
        velocity.SetY(finalGrav);
        if(velocity.GetX() <= -0.01f) {
            velocity.SetX(-0.01f);
        }
        if(velocity.GetZ() <= -0.01f) {
            velocity.SetZ(-0.01f);
        }

        //// Set maximum acceleration.
        //Vector3 acceleration;
        //Vector3 minAccel, maxAccel(0.1f, 0.0f, 0.1f);
        //GameHalloran::GenerateRandomVector3(acceleration, m_rng, minAccel, maxAccel);

        // Set all relevant particle parameters.
        particlePtr->SetPosition(pos);
        particlePtr->SetVelocity(velocity);
        //particlePtr->SetAcceleration(acceleration);
        particlePtr->SetColor(color);
        particlePtr->SetSize(size);
    }

}
