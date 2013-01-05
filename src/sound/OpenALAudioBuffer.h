#pragma once
#ifndef __GF_OPEN_AL_AUDIO_BUFFER_H
#define __GF_OPEN_AL_AUDIO_BUFFER_H

// ////////////////////////////////////////////////////////////////////
// @file OpenALAudioBuffer.h
// @author PJ O Halloran
// @date 12/10/2010
//
// File contains the header for the OpenALAudioBuffer class.
//
// ////////////////////////////////////////////////////////////////////

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

#include <deque>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#ifdef _WINDOWS || TARGET_WINDOWS || WIN32
	#include <al.h>
	#include <alc.h>
#elif defined (TARGET_OS_MAC)
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#else
	#error "Target not supported yet!"
#endif

#include "IAudioBuffer.h"
#include "CAudio.h"
#include "GameException.h"
#include "CSoundResource.h"

namespace GameHalloran
{

	// ////////////////////////////////////////////////////////////////////
	// @class OpenALAudioBuffer
	// @author PJ O Halloran
	//
	// An OpenAL implementation of the IAudioBuffer interface proposed by
	// Mike McShaffry in Games Coding Complete 3.
	// 
	// ////////////////////////////////////////////////////////////////////
	class OpenALAudioBuffer : public AudioBuffer
	{
	private:

		ALuint m_id;													///< Handle/ID of the OpenAL buffer.

	public:

		// ////////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param resource The sound resource.
		//
		// @throw GameException& If we fail to load the sound resource into
		//							an OpenAL sound buffer.
		//
		// ////////////////////////////////////////////////////////////////////
		OpenALAudioBuffer(boost::shared_ptr<SoundResHandle> resource) throw (GameException &);

		// ////////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual ~OpenALAudioBuffer();

		// ////////////////////////////////////////////////////////////////////
		// Get the ID of the audio buffer so you can bypass this class and use
		// the OpenAL C API directly.  Do not call alDeleteBuffers() as this
		// is done automatically by this class when instances go out of scope.
		//
		// ////////////////////////////////////////////////////////////////////
		ALuint GetId() { return (m_id); };

		// ////////////////////////////////////////////////////////////////////
		// Get an implementation specific pointer to the audio buffer.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual void *VGet();

		// ////////////////////////////////////////////////////////////////////
		// Called on an application restore to restore the sound buffer.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual bool VOnRestore();

		// ////////////////////////////////////////////////////////////////////
		// Play the sound buffer.
		//
		// @param volume The volume of the sound.
		// @param looping Should the sound loop when played?
		//
		// @return bool True on success and false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual bool VPlay(const I32 volume, const bool looping);

		// ////////////////////////////////////////////////////////////////////
		// Pause the sound buffer.
		//
		// @return bool True on success and false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual bool VPause();

		// ////////////////////////////////////////////////////////////////////
		// Stop the sound buffer.
		//
		// @return bool True on success and false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual bool VStop();

		// ////////////////////////////////////////////////////////////////////
		// Resume playing the sound buffer.
		//
		// @return bool True on success and false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual bool VResume();

		// ////////////////////////////////////////////////////////////////////
		// Toggle on/off pausing the sound buffer.
		//
		// @return bool True on success and false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual bool VTogglePause();

		// ////////////////////////////////////////////////////////////////////
		// Check if the sound buffer is playing.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual bool VIsPlaying();

		// ////////////////////////////////////////////////////////////////////
		// Set the volume of the sound.
		//
		// @param volume The new volume of the sound.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual void VSetVolume(const I32 volume);

		// ////////////////////////////////////////////////////////////////////
		// Get the progress of the playback.
		//
		// @return F32 A number between 0.0 and 1.0 indicating how much of
		//					the sound has been played.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual F32 VGetProgress();
	};

	// List of OpenAL audio buffers.
	typedef std::deque<boost::shared_ptr<OpenALAudioBuffer> > OpenALAudioBufferQueue;

	// ////////////////////////////////////////////////////////////////////
	// Get the length in seconds of a OpenAL buffer.
	//
	// Please note that this is only accurate for buffers that were loaded
	// into OpenAL from PCM data.
	//
	// @param bufferId The ID of the OpenAL buffer.
	//
	// @return boost::optional<ALfloat> Uninitialized on error.
	// @return boost::optional<ALfloat> The duration of the buffer on success
	//									(in seconds).
	//
	// ////////////////////////////////////////////////////////////////////
	boost::optional<ALfloat> GetOpenALBufferDuration(const ALuint bufferId);

	// /////////////////////////////////////////////////////////////////
	// Loads an OpenAL buffer from the resource cache manager.
	//
	// @param resourceId The ID of the sound resource in the resource file.
	//
	// @return boost::shared_ptr<OpenALAudioBuffer> A shared pointer storing
	//												an OpenALAudioBuffer object
	//												(or NULL on error).
	//
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<OpenALAudioBuffer> LoadOpenALBufferFromResourceCache(const std::string &resourceId);

}

#ifdef WIN32
#	pragma warning( pop )
#endif

#endif
