#pragma once
#ifndef __GF_OPEN_AL_AUDIO_H
#define __GF_OPEN_AL_AUDIO_H

// ////////////////////////////////////////////////////////////////////
// @file OpenALAudio.h
// @author PJ O Halloran
// @date 12/10/2010
//
// File contains the header for the OpenALAudio class.
//
// ////////////////////////////////////////////////////////////////////

#include <map>

#include <boost/shared_ptr.hpp>
#if defined(_WINDOWS) || defined(WIN32)
#include <al.h>
#include <alc.h>
#elif defined (TARGET_OS_MAC)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#error "Target not supported yet!"
#endif

#include "IAudioBuffer.h"
#include "CSoundResource.h"
#include "CAudio.h"
#include "IActors.h"
#include "OpenALAudioBuffer.h"
#include "OpenALAudioSource.h"
#include "Vector.h"

// Special error defines for alGetErrorString()...
#define ALUT_ERROR_NO_CURRENT_CONTEXT          0x204
#define ALUT_ERROR_AL_ERROR_ON_ENTRY           0x205
#define ALUT_ERROR_ALC_ERROR_ON_ENTRY          0x206
#define ALUT_ERROR_OPEN_DEVICE                 0x207
#define ALUT_ERROR_CLOSE_DEVICE                0x208
#define ALUT_ERROR_CREATE_CONTEXT              0x209
#define ALUT_ERROR_MAKE_CONTEXT_CURRENT        0x20A
#define ALUT_ERROR_DESTROY_CONTEXT             0x20B
#define ALUT_ERROR_GEN_BUFFERS                 0x20C
#define ALUT_ERROR_BUFFER_DATA                 0x20D
#define ALUT_ERROR_IO_ERROR                    0x20E
#define ALUT_ERROR_UNSUPPORTED_FILE_TYPE       0x20F
#define ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE    0x210
#define ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA   0x211

namespace GameHalloran {

    // ////////////////////////////////////////////////////////////////////
    // @class OpenALAudio
    // @author PJ O Halloran
    //
    // An OpenAL implementation of the Audio interface in Games Coding
    // Complete 3 by Mike McShaffry.
    //
    // This class extends his simple sound interface for OpenAL, most notably
    // brings in the idea of a listener and multiple sound sources in a
    // 3D scene playing and sharing a set of buffers loaded by the audio system.
    //
    // A sound source may be created and tied to a game actor or a source
    // may be created and located in an arbituary position in the scene.
    //
    // ////////////////////////////////////////////////////////////////////
    class OpenALAudio : public Audio {
    private:

        // ////////////////////////////////////////////////////////////////////
        // @struct SceneAudioSource
        // @author PJ O Halloran
        //
        // An audio source located at some arbitrary position in the scene.
        // If the temp flag is marked to true the audio manager will free the
        // source immediately once has stopped playing.
        //
        // ////////////////////////////////////////////////////////////////////
        struct SceneAudioSource {
            boost::shared_ptr<OpenALAudioSource> m_src;         ///< The audio source.
            bool m_tempSrc;                                     ///< Is the source a temporary one (to be removed during listener update if its state is stopped)?
        };

        typedef std::vector<SceneAudioSource> SceneAudioSourceVector;
        typedef std::map<ActorId, boost::shared_ptr<OpenALAudioSource> > ActorAudioSourceMap;

        SceneAudioSourceVector m_sourceVec;                                                 ///< List of scene (possibly temporary) audio sources.
        ActorAudioSourceMap m_actorSourceMap;                                               ///< Map of sources associated with game actors.
        ALCcontext *m_contextPtr;                                                           ///< OpenAL context.

        // ////////////////////////////////////////////////////////////////////
        // Checks for a valid context - used in alutInit and Exit....
        //
        // ////////////////////////////////////////////////////////////////////
        ALboolean AlSanityCheck(void);

        // ////////////////////////////////////////////////////////////////////
        // Getting rid of alut 3rd party dependancy - this function performs
        // the same job as alutInit().
        //
        // ////////////////////////////////////////////////////////////////////
        ALboolean AlInit();

        // ////////////////////////////////////////////////////////////////////
        // Getting rid of alut 3rd party dependancy - this function performs
        // the same job as alutExit().
        //
        // ////////////////////////////////////////////////////////////////////
        ALboolean AlExit();

    public:

        // ////////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // ////////////////////////////////////////////////////////////////////
        OpenALAudio();

        // ////////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual ~OpenALAudio();

        // ////////////////////////////////////////////////////////////////////
        // Retrieves the global singleton OpenAL audio manager pointer.
        //
        // Will create the manager the first time it is called.  If we fail to
        // setup the OpenAL audio manager then this method will return NULL so its
        // important to verify the pointer before use.
        //
        // N.B. It is not safe to store this pointer locally!
        //
        // ////////////////////////////////////////////////////////////////////
        static OpenALAudio * const GetGlobalInstance() {
            static OpenALAudio gAudioObj;
            if(!gAudioObj.VInitialize()) {
                return (NULL);
            }
            return (&gAudioObj);
        };

        // ////////////////////////////////////////////////////////////////////
        // Set the master gain/volume in the scene.
        //
        // ////////////////////////////////////////////////////////////////////
        bool SetMasterGain(const F32 masterGain);

        // ////////////////////////////////////////////////////////////////////
        // Get the master gain/volume in the scene.
        //
        // ////////////////////////////////////////////////////////////////////
        bool GetMasterGain(F32 &masterGain) const;

        // ////////////////////////////////////////////////////////////////////
        // Set the position of the listener in the scene.
        //
        // ////////////////////////////////////////////////////////////////////
        bool SetPosition(const Point3 &position);

        // ////////////////////////////////////////////////////////////////////
        // Get the position of the listener in the scene.
        //
        // ////////////////////////////////////////////////////////////////////
        bool GetPosition(Point3 &position) const;

        // ////////////////////////////////////////////////////////////////////
        // Set the velocity of the listener.
        //
        // ////////////////////////////////////////////////////////////////////
        bool SetVelocity(const Vector3 &velocity);

        // ////////////////////////////////////////////////////////////////////
        // Get the velocity of the listener.
        //
        // ////////////////////////////////////////////////////////////////////
        bool GetVelocity(Vector3 &velocity) const;

        // ////////////////////////////////////////////////////////////////////
        // Set the orientation of the listener.
        //
        // @param forward The direction the listener is facing.
        // @param up The up direction relative to the listener.
        //
        // ////////////////////////////////////////////////////////////////////
        bool SetOrientation(const Vector3 &forward, const Vector3 &up);

        // ////////////////////////////////////////////////////////////////////
        // Get the orientation of the listener.
        //
        // @param forward The direction the listener is facing.
        // @param up The up direction relative to the listener.
        //
        // ////////////////////////////////////////////////////////////////////
        bool GetOrientation(Vector3 &forward, Vector3 &up);

        // ////////////////////////////////////////////////////////////////////
        // Convienience function that updates the listeners position, orientation
        // and velocity all at once.
        //
        // HINT: Keep the listener pos and orientation in sync with the 3D scenes
        // camera for a straight forward 3D audio effect!
        //
        // The velocity is calculated using the formula V = (Pn - Po) / (t).
        // V = new velocity.
        // Pn = New position.
        // Po = Old position.
        // t = Number of seconds since the last update.
        //
        // @param newPosition The new position of the listener.
        // @param newForward New facing direction of the listener.
        // @param newUp New up direction of the listener.
        // @param timediff The number of seconds since the last update.
        //
        // @return bool True on success or false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        bool Update(const Point3 &newPosition, const Vector3 &newForward, const Vector3 &newUp, const F32 timediff);

        // ////////////////////////////////////////////////////////////////////
        // Add a source into the OpenAL system.
        //
        // @param srcPtr Pointer to the source.
        // @param actorId ID to tie the sound source to a game actor.
        //
        // @return bool True if the source was added and false if not.
        //
        // ////////////////////////////////////////////////////////////////////
        bool AddActorSource(boost::shared_ptr<OpenALAudioSource> srcPtr, const ActorId actorId);

        // ////////////////////////////////////////////////////////////////////
        // Add a scene source into the OpenAL system.
        //
        // @param srcPtr Pointer to the source.
        // @param freeOnceStopped Should we remove the source once it has completed
        //                          playback.
        //
        // @return bool True if the source was added and false if not.
        //
        // ////////////////////////////////////////////////////////////////////
        bool AddSceneSource(boost::shared_ptr<OpenALAudioSource> srcPtr, const bool freeOnceStopped = true);

        // ////////////////////////////////////////////////////////////////////
        // Find a source in the audio system by the game actor id.
        //
        // @param actorId The ID of the actor.
        //
        // @return boost::shared_ptr<OpenALAudioSource> Pointer to the sound
        //                                                  source or NULL if not
        //                                                  found.
        //
        // ////////////////////////////////////////////////////////////////////
        boost::shared_ptr<OpenALAudioSource> FindActorSource(const ActorId actorId);

        // ////////////////////////////////////////////////////////////////////
        // Get the scene source object.
        //
        // @param srcId The ID of the source.
        //
        // @return boost::shared_ptr<OpenALAudioSource> Pointer to the sound
        //                                                  source or NULL if not
        //                                                  found.
        //
        // ////////////////////////////////////////////////////////////////////
        boost::shared_ptr<OpenALAudioSource> GetSceneSource(const ALuint srcId);

        // ////////////////////////////////////////////////////////////////////
        // Remove an audio source from the OpenAL system.
        //
        // @param actorId The ID of the actor to remove.
        //
        // @return bool True If source found and removed, false if not.
        //
        // ////////////////////////////////////////////////////////////////////
        bool RemoveActorSource(const ActorId actorId);

        // ////////////////////////////////////////////////////////////////////
        // Remove an scene audio source from the OpenAL system.
        //
        // @param srcId The ID of the actor to remove.
        //
        // @return bool True If source found and removed, false if not.
        //
        // ////////////////////////////////////////////////////////////////////
        bool RemoveSceneSource(const ALuint srcId);

        // ////////////////////////////////////////////////////////////////////
        // Clear all audio sources (both actor and scene sources).
        //
        // ////////////////////////////////////////////////////////////////////
        void ClearAllSources();

        // ////////////////////////////////////////////////////////////////////
        // Check if the system is active.  This function can inform you if the
        // sound card is disabled or not installed.
        //
        // @return bool True is active, false if not.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual bool VActive();

        // ////////////////////////////////////////////////////////////////////
        // Initialize an audio buffer from a sound resource.
        //
        // @param soundResource The sound resource handle.
        //
        // @return boost::shared_ptr<IAudioBuffer> A pointer to an audio buffer.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual boost::shared_ptr<IAudioBuffer> VInitAudioBuffer(boost::shared_ptr<SoundResHandle> soundResource);

        // ////////////////////////////////////////////////////////////////////
        // Release resources for an audio buffer.
        //
        // @param audioBuffer Pointer to an audio buffer.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual void VReleaseAudioBuffer(boost::shared_ptr<IAudioBuffer> audioBuffer);

        // ////////////////////////////////////////////////////////////////////
        // Shutdown the audio subsystem.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual void VShutdown();

        // ////////////////////////////////////////////////////////////////////
        // Initialize the audio subsystem.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual bool VInitialize();

        // ////////////////////////////////////////////////////////////////////
        // Stop all sound buffers playing.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual void VStopAllSounds();

        // ////////////////////////////////////////////////////////////////////
        // Pause all playing sound buffers.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual void VPauseAllSounds();

        // ////////////////////////////////////////////////////////////////////
        // Resume playing all paused sound buffers.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual void VResumeAllSounds();
    };

    // ////////////////////////////////////////////////////////////////////
    // Converts an AL error code to a human readable string.
    //
    // ////////////////////////////////////////////////////////////////////
    const char *alGetErrorString(ALenum error);

    // Define OpenAL check and clear error macros so that this slow error checking can be compiled out for the release build!
    bool GfCheckAlError();
    bool GfCheckAlErrorTrc(const char *msg);
#define GF_CHECK_AL_ERROR() GfCheckAlError()
#define GF_CHECK_AL_ERROR_TRC(msg) GfCheckAlErrorTrc(msg)
#if DEBUG
#define GF_CLEAR_AL_ERROR() { ALenum err; do { err = alGetError(); } while(err != AL_NO_ERROR); }
#else
#define GF_CLEAR_AL_ERROR()
#endif

    // Global OpenAL audio pointer.
    extern OpenALAudio *g_oalAudioPtr;

}

#endif
