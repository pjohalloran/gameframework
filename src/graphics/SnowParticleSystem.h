#pragma once
#ifndef __GF_SNOW_PARTICLE_SYSTEM_H
#define __GF_SNOW_PARTICLE_SYSTEM_H

// /////////////////////////////////////////////////////////////////
// @file SnowParticleSystem.h
// @author PJ O Halloran
// @date 29/09/2010
//
// File contains the header for the SnowParticleSystem class.
//
// /////////////////////////////////////////////////////////////////

#include "GameBase.h"
#include "Matrix.h"
#include "Vector.h"
#include "BoundingCube.h"
#include "BoundingSphere.h"
#include "GLBatch.h"
#include "GLSLShader.h"
#include "GameColors.h"
#include "ParticleSystem.h"
#include "ModelViewProjStackManager.h"
#include "GLShaderManager.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // @class SnowParticleSystem
    // @author PJ O Halloran
    //
    // Class for rendering a snow effect using the base particle engine
    // and an appropriate GLSL shader program.
    //
    // /////////////////////////////////////////////////////////////////
    class SnowParticleSystem : public ParticleSystem {
    private:

        // /////////////////////////////////////////////////////////////////
        // @enum SnowState
        //
        // Various states of the snow particle system.
        // SNOW_BEGIN:      Snow starting to fall gradually heavier upto...
        // SNOW_CONSTANT:   Snowfall at a constant rate.
        // SNOW_ENDING:     Snowfall beginning to fall off.
        // SNOW_STOP:       Snowfall stopped.
        //
        // /////////////////////////////////////////////////////////////////
        enum SnowState {
            SNOW_BEGIN,
            SNOW_CONSTANT,
            SNOW_ENDING,
            SNOW_STOP
        };

        GLint m_mvpMatrixLoc;                                       ///< Location of the mvp matrix in the GLSL shader src.
        GLint m_snowColorLoc;                                       ///< Location of the snow color in the GLSL shader src.
        GLint m_pointImageLoc;                                      ///< Location of the point image sampler in the GLSL shader src.
        GLint m_mvMatrixLoc;                                        ///< The position of the ModelView matrix in the GLSL shader src.
        GLint m_cameraPosLoc;                                       ///< The position of the camera vector in the GLSL shader src.
        SnowState m_state;                                          ///< The current state of the snowfall.
        boost::shared_ptr<ModelViewProjStackManager> m_mvpStackPtr; ///< Pointer to the MV matrix stack.
        GLShaderManager m_stockShaders;
        const GLFrame m_cameraFrame;                                ///< The camera frame of reference.

        // /////////////////////////////////////////////////////////////////
        // Prepare the GLSL program for execution.
        //
        // /////////////////////////////////////////////////////////////////
        void PrepareShader();

    protected:


    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param mvStackPtr Pointer to the MVP stack manager.
        //
        // /////////////////////////////////////////////////////////////////
        explicit SnowParticleSystem(boost::shared_ptr<ModelViewProjStackManager> mvpStackPtr, const GLFrame &cameraFrame);

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param bb Bounding box of the particle system.
        // @param emitRate The number of particles emitted per second.
        // @param maxParticles The maximum number of particles allowed.
        // @param textureResource The texture resource to apply to all particles.
        // @param windVec The wind vector.
        // @param gravity The strength of gravity in the system.
        // @param vertexShaderRes
        // @param numParticles The initial number of snow particles.
        // @param mvStackPtr Pointer to the MVP stack manager.
        //
        // /////////////////////////////////////////////////////////////////
        explicit SnowParticleSystem(const BoundingCube &bb,
                                    const F32 emitRate,
                                    const U32 maxParticles,
                                    const ImageResource &textureResource,
                                    const Vector3 &windVec,
                                    const F32 gravity,
                                    const U32 numParticles,
                                    boost::shared_ptr<ModelViewProjStackManager> mvpStackPtr,
                                    GLShaderManager &stockShaders,
                                    const GLFrame &cameraFrame);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~SnowParticleSystem();

        // /////////////////////////////////////////////////////////////////
        // Called when the application is restored after a minimize or other
        // such event.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnRestore();

        // /////////////////////////////////////////////////////////////////
        // Called when the application display or device is lost after a
        // display resolution or other such system event.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VOnLostDevice();

        // /////////////////////////////////////////////////////////////////
        // Called before a render to set 3D scene state.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnPreRender();

        // /////////////////////////////////////////////////////////////////
        // Render the particle system.
        //
        // @param time The total time in seconds so far.
        // @param elapsedTime The time elapsed in seconds since the last
        //                      render call.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnRender(const F64 time, const F32 elapsedTime);

        // /////////////////////////////////////////////////////////////////
        // Called after a render to restore 3D scene state.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnPostRender();

        // /////////////////////////////////////////////////////////////////
        // Update the particle system.
        //
        // @param elapsedTime The number of milliseconds since the last update.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VOnUpdate(const F32 elapsedTime);

        // /////////////////////////////////////////////////////////////////
        // Reset a particle back to an initial state.
        // In some particle system types we will want to reuse particles
        // instead of removing them.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VResetParticle(boost::shared_ptr<Particle> particlePtr);

    };

}

#endif
