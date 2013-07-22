#pragma once
#ifndef __GF_OPEN_AL_AUDIO_SOURCE_H
#define __GF_OPEN_AL_AUDIO_SOURCE_H

// ////////////////////////////////////////////////////////////////////
// @file OpenALAudioSource.h
// @author PJ O Halloran
// @date 12/10/2010
//
// File contains the header for the OpenALAudioSource class.
//
// ////////////////////////////////////////////////////////////////////

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#if defined(_WINDOWS) || defined(WIN32)
#include <al.h>
#include <alc.h>
#elif defined (TARGET_OS_MAC)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#error "Target not supported yet!"
#endif

#include "OpenALAudioBuffer.h"
#include "Vector.h"

namespace GameHalloran {

    // ////////////////////////////////////////////////////////////////////
    // @class OpenALAudioSource
    // @author PJ O Halloran
    //
    // An C++ wrapper to encapsulate the OpenAL source of a sound.
    // Many OpenAL sound buffers may be attached to a source to be played
    // one after the other.
    //
    // ////////////////////////////////////////////////////////////////////
    class OpenALAudioSource {
    private:

        ALuint m_id;                                    ///< Handle/ID of the OpenAL source.
        boost::optional<ALfloat> m_totalDuration;       ///< Total duration of the buffer(s) attached to the source in seconds.
        F32 m_lastUpdateTime;                           ///< Timestamp of the last update call.

        // ////////////////////////////////////////////////////////////////////
        // Get a human readable string of the ALenum param (for logging errors).
        //
        // ////////////////////////////////////////////////////////////////////
        std::string GetALEnumString(const ALenum param) const;

        // ////////////////////////////////////////////////////////////////////
        // Get a single OpenAL F32 value.
        //
        // @param param The ID of the parameter.
        // @param value Will hold the value retrieved on success.
        //
        // @return bool True on success or false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        bool GetFloatParam(const ALenum param, ALfloat &value) const;

        // ////////////////////////////////////////////////////////////////////
        // Set a single OpenAL F32 value.
        //
        // @param param The ID of the parameter.
        // @param value The new value of the parameter.
        //
        // @return bool True on success or false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        bool SetFloatParam(const ALenum param, const ALfloat value) const;

        // ////////////////////////////////////////////////////////////////////
        // Get an array OpenAL F32 value.
        //
        // @param param The ID of the parameter.
        // @param arrSize The size of the array.
        // @param arr Will hold the values retrieved on success.
        //
        // @return bool True on success or false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        bool GetFloatArrayParam(const ALenum param, const ALuint arrSize, ALfloat *arr) const;

        // ////////////////////////////////////////////////////////////////////
        // Set an array OpenAL F32 value.
        //
        // @param param The ID of the parameter.
        // @param arrSize The size of the array.
        // @param arr The new value of the array parameter.
        //
        // @return bool True on success or false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        bool SetFloatArrayParam(const ALenum param, const ALuint arrSize, const ALfloat * const arr) const;

        // ////////////////////////////////////////////////////////////////////
        // Get a single OpenAL int value.
        //
        // @param param The ID of the parameter.
        // @param value Will hold the value retrieved on success.
        //
        // @return bool True on success or false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        bool GetIntParam(const ALenum param, ALint &value) const;

        // ////////////////////////////////////////////////////////////////////
        // Set a single OpenAL int value.
        //
        // @param param The ID of the parameter.
        // @param value The new value of the parameter.
        //
        // @return bool True on success or false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        bool SetIntParam(const ALenum param, const ALint value) const;

        // ////////////////////////////////////////////////////////////////////
        // Get an array OpenAL I32 value.
        //
        // @param param The ID of the parameter.
        // @param arrSize The size of the array.
        // @param arr Will hold the values retrieved on success.
        //
        // @return bool True on success or false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        bool GetIntArrayParam(const ALenum param, const ALuint arrSize, ALint *arr) const;

        // ////////////////////////////////////////////////////////////////////
        // Set an array OpenAL int value.
        //
        // @param param The ID of the parameter.
        // @param arrSize The size of the array.
        // @param arr The new value of the array parameter.
        //
        // @return bool True on success or false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        bool SetIntArrayParam(const ALenum param, const ALuint arrSize, const ALint * const arr) const;

    public:

        // ////////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // Check if the source is valid after creation with IsValid() to ensure
        // it was created correctly.
        //
        // @param positionRef The initial position of the source.
        // @param velRef The initial velocity of the source.
        // @param directionRef The initial direction the source is facing.
        //
        // ////////////////////////////////////////////////////////////////////
        OpenALAudioSource(const Point3 &positionRef, const Vector3 &velRef, const Vector3 &directionRef);

        // ////////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual ~OpenALAudioSource();

        // ////////////////////////////////////////////////////////////////////
        // Is the source a valid OpenAL source.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool IsValid() const {
            return (alIsSource(m_id) == AL_TRUE ? true : false);
        };

        // ////////////////////////////////////////////////////////////////////
        // Return the ID of the OpenAL source so you can use the OpenAL C API
        // directly if you wish.  Do not call alDeleteSource on this ID as the
        // source class will do that when instances goes out of scope.
        //
        // ////////////////////////////////////////////////////////////////////
        inline ALuint GetId() const {
            return (m_id);
        };

        // ////////////////////////////////////////////////////////////////////
        // Get the current position of the source.
        //
        // @param position The object which will contain the sources current
        //                  position on success.
        //
        // @param bool True if we got the position.
        //
        // ////////////////////////////////////////////////////////////////////
        bool GetPosition(Point3 &position) const;

        // ////////////////////////////////////////////////////////////////////
        // Set the position of the source.
        //
        // @param positionRef The initial position of the source.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool SetPosition(const Point3 &positionRef) const {
            return (SetFloatArrayParam(AL_POSITION, 3, positionRef.GetComponentsConst()));
        };

        // ////////////////////////////////////////////////////////////////////
        // Get the current velocity of the source.
        //
        // @param velocity The current velocity.
        //
        // @param bool True if we got the velocity.
        //
        // ////////////////////////////////////////////////////////////////////
        bool GetVelocity(Vector3 &velocity) const;

        // ////////////////////////////////////////////////////////////////////
        // Set the velocity of the source.
        //
        // @param velRef The initial velocity of the source.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool SetVelocity(const Vector3 &velRef) const {
            return (SetFloatArrayParam(AL_VELOCITY, 3, velRef.GetComponentsConst()));
        };

        // ////////////////////////////////////////////////////////////////////
        // Get the current direction of the source.
        //
        // @param direction The current velocity.
        //
        // @param bool True if we got the velocity.
        //
        // ////////////////////////////////////////////////////////////////////
        bool GetDirection(Vector3 &direction) const;

        // ////////////////////////////////////////////////////////////////////
        // Set the direction of the source.
        //
        // As per OpenAL documentation, if you set the direction to (0,0,0)
        // the source will be a directionless sound source.  This can speed up
        // sound calculations as attenuation and the doppler effect
        // will be factored out.
        //
        // @param directionRef The initial direction the source is facing.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool SetDirection(const Vector3 &directionRef) const {
            return (SetFloatArrayParam(AL_DIRECTION, 3, directionRef.GetComponentsConst()));
        };

        // ////////////////////////////////////////////////////////////////////
        // Is the sound source a directional one?
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool IsDirectional() const {
            Vector3 dir;
            GetDirection(dir);
            if(dir != Vector3(0, 0, 0)) {
                return (true);
            }
            return (false);
        };

        // ////////////////////////////////////////////////////////////////////
        // Convienience function that updates the sources position, facing
        // direction and velocity all at once.
        //
        // The velocity is calculated using the formula V = (Pn - Po) / (Tn - To).
        // V = new velocity.
        // Pn = New position.
        // Po = Old position.
        // Tn = Time of this position change.
        // To = Time of last position change.
        //
        // @param newPosition The new position of the source.
        // @param newDirection New facing direction of the source.
        // @param timestamp The time when the sources position changed.
        //
        // @return bool True on success or false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        bool Update(const Point3 &newPosition, const Vector3 &newDirection, const F32 timestamp);

        // ////////////////////////////////////////////////////////////////////
        // Attach a single buffer to the source.
        //
        // @param oalAudioBufferPtr Pointer to the audio buffer to attach to the
        //                          source.
        //
        // @return bool True on success.
        // @return bool False if the pointer is NULL.
        // @return bool False if OpenAL fails to attach the buffer to the source
        //              (check error log).
        //
        // ////////////////////////////////////////////////////////////////////
        bool AttachBuffer(boost::shared_ptr<OpenALAudioBuffer> oalAudioBufferPtr);

        // ////////////////////////////////////////////////////////////////////
        // Attach a queue of buffers to the source.
        //
        // @param oalAudioBufferQueue Queue of audio buffers to attach to the
        //                          source.
        //
        // @return bool True on success.
        // @return bool False if the queue is empty or any elements are NULL.
        // @return bool False if OpenAL fails to attach the queue to the source
        //              (check error log).
        //
        // ////////////////////////////////////////////////////////////////////
        bool AttachBufferList(const OpenALAudioBufferQueue &oalAudioBufferQueue);

        // ////////////////////////////////////////////////////////////////////
        // Detach the source from all buffers.
        //
        // ////////////////////////////////////////////////////////////////////
        bool DetachBuffers();

        // ////////////////////////////////////////////////////////////////////
        // Check if an audio buffer is attached to the source.
        //
        // @param audioBufferPtr Pointer to the audio buffer.
        //
        // @return bool True if the audio buffer is attached and false if not.
        //
        // ////////////////////////////////////////////////////////////////////
        bool IsBufferAttached(boost::shared_ptr<OpenALAudioBuffer> audioBufferPtr) const;

        // ////////////////////////////////////////////////////////////////////
        // Get the number of buffers attached.
        //
        // ////////////////////////////////////////////////////////////////////
        inline U32 GetBufferQueueSize() const {
            ALint size(0);
            GetIntParam(AL_BUFFERS_QUEUED, size);
            return (U32(size));
        };

        // ////////////////////////////////////////////////////////////////////
        // Get the number of buffers processed.
        //
        // ////////////////////////////////////////////////////////////////////
        inline U32 GetNumberBuffersProcessed() const {
            ALint size(0);
            GetIntParam(AL_BUFFERS_PROCESSED, size);
            return (U32(size));
        };

        // ////////////////////////////////////////////////////////////////////
        // Get the pitch.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool GetPitch(F32 &pitch) const {
            return (GetFloatParam(AL_PITCH, pitch));
        };

        // ////////////////////////////////////////////////////////////////////
        // Set the pitch.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool SetPitch(const F32 pitch) const {
            return (SetFloatParam(AL_PITCH, pitch));
        };

        // ////////////////////////////////////////////////////////////////////
        // Get the 3D gain/volume attributes.
        //
        // @param maxAttDistance Distance where there will be no further attenuation.
        // @param rolloffFactor Rollof rate.
        // @param refDistance Distance where the volume will drop by half (before
        //                      influence by rolloff or attenuation kicks in).
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool Get3dVolumeAttributes(F32 &maxAttDistance, F32 &rolloffFactor, F32 &refDistance) const {
            return (GetFloatParam(AL_MAX_DISTANCE, maxAttDistance) && \
                    GetFloatParam(AL_ROLLOFF_FACTOR, rolloffFactor) && \
                    GetFloatParam(AL_REFERENCE_DISTANCE, refDistance));
        };

        // ////////////////////////////////////////////////////////////////////
        // Set all 3D gain/volume attributes.
        //
        // @param maxAttDistance Distance where there will be no further attenuation.
        // @param rolloffFactor Rollof rate.
        // @param refDistance Distance where the volume will drop by half (before
        //                      influence by rolloff or attenuation kicks in).
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool Set3dVolumeAttributes(const F32 maxAttDistance, const F32 rolloffFactor, const F32 refDistance) const {
            return (SetFloatParam(AL_MAX_DISTANCE, maxAttDistance) && \
                    SetFloatParam(AL_ROLLOFF_FACTOR, rolloffFactor) && \
                    SetFloatParam(AL_REFERENCE_DISTANCE, refDistance));
        };

        // ////////////////////////////////////////////////////////////////////
        // Get the min and max source gain/volume.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool GetVolumeLimits(F32 &minVolume, F32 &maxVolume) const {
            return (GetFloatParam(AL_MIN_GAIN, minVolume) && \
                    GetFloatParam(AL_MAX_GAIN, maxVolume));
        };

        // ////////////////////////////////////////////////////////////////////
        // Set the min and max source gain/volume.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool SetVolumeLimits(const F32 minVolume, const F32 maxVolume) const {
            return (SetFloatParam(AL_MIN_GAIN, minVolume) && \
                    SetFloatParam(AL_MAX_GAIN, maxVolume));
        };

        // ////////////////////////////////////////////////////////////////////
        // Get the directional sound cone attributes.
        //
        // @param outerGain Gain/Volume when outside the sound cone.
        // @param innerAngle Inner sound cone angle.
        // @param outerAngle Outer sound cone angle.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool GetAudibleConeAttributes(F32 &outerGain, F32 &innerAngle, F32 &outerAngle) const {
            return (GetFloatParam(AL_CONE_OUTER_GAIN, outerGain) && \
                    GetFloatParam(AL_CONE_INNER_ANGLE, innerAngle) && \
                    GetFloatParam(AL_CONE_OUTER_ANGLE, outerAngle));
        };

        // ////////////////////////////////////////////////////////////////////
        // Set the directional sound cone attributes.
        //
        // @param outerGain Gain/Volume when outside the sound cone.
        // @param innerAngle Inner sound cone angle.
        // @param outerAngle Outer sound cone angle.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool SetAudibleConeAttributes(const F32 outerGain, const F32 innerAngle, const F32 outerAngle) const {
            return (SetFloatParam(AL_CONE_OUTER_GAIN, outerGain) && \
                    SetFloatParam(AL_CONE_INNER_ANGLE, innerAngle) && \
                    SetFloatParam(AL_CONE_OUTER_ANGLE, outerAngle));
        };

        // ////////////////////////////////////////////////////////////////////
        // Is the source streaming multiple buffers?
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool IsStreaming() const {
            ALint st;
            return (GetIntParam(AL_SOURCE_TYPE, st) && (st == AL_STREAMING));
        };

        // ////////////////////////////////////////////////////////////////////
        // Is the source playing only one buffer?
        //
        // ////////////////////////////////////////////////////////////////////
        bool IsStatic() const {
            ALint st;
            return (GetIntParam(AL_SOURCE_TYPE, st) && (st == AL_STATIC));
        };

        // ////////////////////////////////////////////////////////////////////
        // Set the current playback position in seconds.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool SetPlaybackPositionTime(const F32 seconds) const {
            return (SetFloatParam(AL_SEC_OFFSET, seconds));
        };

        // ////////////////////////////////////////////////////////////////////
        // Set the current playback position in samples.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool SetPlaybackPositionSamples(const F32 samples) const {
            return (SetFloatParam(AL_SAMPLE_OFFSET, samples));
        };

        // ////////////////////////////////////////////////////////////////////
        // Set the current playback position in byte offset from the beginning
        // of the buffer.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool SetPlaybackPositionByteOffset(const F32 byteOffset) const {
            return (SetFloatParam(AL_BYTE_OFFSET, byteOffset));
        };

        // ////////////////////////////////////////////////////////////////////
        // Get the current playback position.
        //
        // @param seconds Current playback time.
        // @param samples Current buffer sample.
        // @param byteOffset Current buffer byte offset.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool GetPlaybackPosition(F32 &seconds, F32 &samples, F32 &byteOffset) const {
            return (SetFloatParam(AL_SEC_OFFSET, seconds) && \
                    SetFloatParam(AL_SAMPLE_OFFSET, samples) && \
                    SetFloatParam(AL_BYTE_OFFSET, byteOffset));
        };

        // ////////////////////////////////////////////////////////////////////
        // Is the sources position relative to the listener?
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool IsRelativePosition() const {
            ALint r;
            return (GetIntParam(AL_SOURCE_RELATIVE, r) && (r == AL_TRUE));
        };

        // ////////////////////////////////////////////////////////////////////
        // Make the sources position either relative or not relative to the
        // listeners position.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool UseRelativePosition(const bool relative) const {
            return (SetIntParam(AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE));
        };

        // ////////////////////////////////////////////////////////////////////
        // Play the sound buffer the source is attached to from the
        // beginning.
        //
        // @param volume The volume of the sound (between 0.0 and 1.0).
        // @param looping Should the sound loop when played?
        //
        // @return bool True on success and false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        bool Play(const F32 volume, const bool looping) const;

        // ////////////////////////////////////////////////////////////////////
        // Pause the sound buffer(s) the source is attached to.
        //
        // @return bool True on success and false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        bool Pause() const;

        // ////////////////////////////////////////////////////////////////////
        // Stop the sound buffer the source is attached to.
        //
        // @return bool True on success and false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        bool Stop() const;

        // ////////////////////////////////////////////////////////////////////
        // Resume playing the sound buffer the source is attached to.
        //
        // @return bool True on success and false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        bool Resume() const;

        // ////////////////////////////////////////////////////////////////////
        // Toggle on/off pausing the sound buffer the source is attached to.
        //
        // @return bool True on success and false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        bool TogglePause() const;

        // ////////////////////////////////////////////////////////////////////
        // Check if the sound buffer is playing the source is attached to.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool IsPlaying() const {
            ALint state;
            return (GetIntParam(AL_SOURCE_STATE, state) && state == AL_PLAYING);
        };

        // ////////////////////////////////////////////////////////////////////
        // Check if the source state is currently stopped.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool IsStopped() const {
            ALint state;
            return (GetIntParam(AL_SOURCE_STATE, state) && state == AL_STOPPED);
        };

        // ////////////////////////////////////////////////////////////////////
        // Check if the source state is currently paused.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool IsPaused() const {
            ALint state;
            return (GetIntParam(AL_SOURCE_STATE, state) && state == AL_PAUSED);
        };

        // ////////////////////////////////////////////////////////////////////
        // Set the volume/gain of the sound the source is attached to.
        //
        // @param volume The new volume of the sound.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool SetVolume(const F32 volume) const {
            return (SetFloatParam(AL_GAIN, volume));
        };

        // ////////////////////////////////////////////////////////////////////
        // Get the volume/gain of the sound the source is attached to.
        //
        // ////////////////////////////////////////////////////////////////////
        inline bool GetVolume(F32 &volume) const {
            return (GetFloatParam(AL_GAIN, volume));
        };

        // ////////////////////////////////////////////////////////////////////
        // Get the progress of the playback the source is attached to.
        //
        // @return F32 A number between 0.0 and 1.0 indicating how much of
        //                  the sound has been played.
        //
        // ////////////////////////////////////////////////////////////////////
        F32 GetProgress() const;
    };

    // List of OpenAL sources.
    typedef std::vector<boost::shared_ptr<OpenALAudioSource> > OpenALAudioSourceList;

}

#endif
