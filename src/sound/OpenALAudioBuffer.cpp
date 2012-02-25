// ////////////////////////////////////////////////////////////////////
// @file OpenALAudioBuffer.cpp
// @author PJ O Halloran
// @date 12/10/2010
//
// File contains the implementation for the OpenALAudioBuffer class.
//
// ////////////////////////////////////////////////////////////////////


// External Headers
#include <string>


// Project Headers
#include "OpenALAudioBuffer.h"

#include "OpenALAudio.h"
#include "GameMain.h"


// ////////////////////////////////////////////////////////////////////
//
//
// ////////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// ////////////////////////////////////////////////////////////////////
	// ***************** OpenALAudioBuffer class functions ****************
	// ////////////////////////////////////////////////////////////////////

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	OpenALAudioBuffer::OpenALAudioBuffer(boost::shared_ptr<SoundResHandle> resource) throw (GameException &)\
		: AudioBuffer(resource), m_id(0)
	{
		alGenBuffers(1, &m_id);
		if(!GF_CHECK_AL_ERROR())
		{
			throw GameException(std::string("Failed to generate the OpenAL Buffer ID"));
		}

		if(!resource)
		{
			throw GameException("Sound Resource is NULL");
		}
		// Ensure PCM data is initialized.
		if(!resource->VInitialize())
		{
			throw GameException("Failed to initlaize the PCM buffer data from the SoundResource");
		}

		const WAVEFORMATEX * const rInfo = resource->GetFormat();
		ALsizei freq = rInfo->nSamplesPerSec;

		ALenum fmt;
		// 8 bit sample rate
		if(rInfo->wBitsPerSample == 8)
		{
			// MONO
			if(rInfo->nChannels == 1)
			{
				fmt = AL_FORMAT_MONO8;
			}
			// STEREO
			else if(rInfo->nChannels == 2)
			{
				fmt = AL_FORMAT_STEREO8;
			}
			else
			{
				// error
				throw GameException(std::string("Unknown number of channels for 8 bit data"));
			}
		}
		// 16 bit sample rate
		else if(rInfo->wBitsPerSample == 16)
		{
			// MONO
			if(rInfo->nChannels == 1)
			{
				fmt = AL_FORMAT_MONO16;
			}
			// STEREO
			else if(rInfo->nChannels == 2)
			{
				fmt = AL_FORMAT_STEREO16;
			}
			else
			{
				// error
				throw GameException(std::string("Unknown number of channels for 16 bit data"));
			}
		}
		else
		{
			// error
			throw GameException(std::string("Unknown number of bits data"));
		}

		alBufferData(m_id, fmt, resource->GetPCMBuffer(), resource->GetPCMBufferSize(), freq);
		if(!GF_CHECK_AL_ERROR())
		{
			throw GameException(std::string("Failed to fill the OpenAL Buffer with data"));
		}
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	OpenALAudioBuffer::~OpenALAudioBuffer()
	{
		try
		{
			if(alIsBuffer(m_id) == AL_TRUE)
			{
				GF_CLEAR_AL_ERROR();
				alDeleteBuffers(1, &m_id);
                GF_CHECK_GL_ERROR_TRC("OpenALAudioBuffer::~OpenALAudioBuffer(): ");
			}
		}
		catch(...)
		{
		}
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void *OpenALAudioBuffer::VGet()
	{
		return (static_cast<void *>(&m_id));
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioBuffer::VOnRestore()
	{
		// No need to restore an OpenAL buffer...
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioBuffer::VPlay(const I32 volume, const bool looping)
	{
		// Playing of buffers is done via the OpenALAudioSource inteface in the OpenAL audio system.
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioBuffer::VPause()
	{
		// Pausing of buffers is done via the OpenALAudioSource inteface in the OpenAL audio system.
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioBuffer::VStop()
	{
		// Stopping playing of buffers is done via the OpenALAudioSource inteface in the OpenAL audio system.
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioBuffer::VResume()
	{
		// Resuming of buffers is done via the OpenALAudioSource inteface in the OpenAL audio system.
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioBuffer::VTogglePause()
	{
		// Pausing of buffers is done via the OpenALAudioSource inteface in the OpenAL audio system.
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioBuffer::VIsPlaying()
	{
		// Checking if buffers are playing is done via the OpenALAudioSource inteface in the OpenAL audio system.
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void OpenALAudioBuffer::VSetVolume(const I32 volume)
	{
		// Set volume of buffers is done via the OpenALAudioSource inteface in the OpenAL audio system.
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	F32 OpenALAudioBuffer::VGetProgress()
	{
		// Getting progress of buffers is done via the OpenALAudioSource inteface in the OpenAL audio system.
		return (0.0f);
	}

	// ////////////////////////////////////////////////////////////////////
	// **************** OpenALAudioBuffer helper functions ****************
	// ////////////////////////////////////////////////////////////////////

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	boost::optional<ALfloat> GetOpenALBufferDuration(const ALuint bufferId)
	{
		boost::optional<ALfloat> duration;

		GF_CLEAR_AL_ERROR();

		// Check we have a valid buffer ID.
		if(alIsBuffer(bufferId) == AL_FALSE)
		{
			// Error, not a valid buffer ID.
			return (duration);
		}

		ALint size, bits, channels, freq;

		alGetBufferi(bufferId, AL_SIZE, &size);
		if(!GF_CHECK_AL_ERROR())
		{
			// Error, failed to get the AL_SIZE
			return (duration);
		}
		alGetBufferi(bufferId, AL_BITS, &bits);
		if(!GF_CHECK_AL_ERROR())
		{
			// Error, failed to get the AL_BITS
			return (duration);
		}
		alGetBufferi(bufferId, AL_CHANNELS, &channels);
		if(!GF_CHECK_AL_ERROR())
		{
			// Error, failed to get the AL_CHANNELS
			return (duration);
		}
		alGetBufferi(bufferId, AL_FREQUENCY, &freq);
		if(!GF_CHECK_AL_ERROR())
		{
			// Error, failed to get the AL_FREQUENCY
			return (duration);
		}

		duration.reset((ALfloat(size) / ALfloat(channels) / (ALfloat(bits) / 8.0f)) / ALfloat(freq));
		return (duration);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<OpenALAudioBuffer> LoadOpenALBufferFromResourceCache(const std::string &resourceId)
	{
		boost::shared_ptr<OpenALAudioBuffer> oalBufferPtr;			///< OpenAL audio buffer shared pointer.

		// Ensure OpenAL audio system is initialized and available.
		if(!g_oalAudioPtr || !g_oalAudioPtr->IsInitialized() || !g_oalAudioPtr->HasSoundCard())
		{
            GF_LOG_TRACE_ERR("LoadOpenALBufferFromResourceCache()", std::string("Cannot load ") + resourceId + std::string(" as the OpenAL audio system either is not initialized or there is no sound card available"));
			return (oalBufferPtr);
		}

		// Load raw sound data from RC.
        SoundResource tmpRes(resourceId);
		boost::shared_ptr<SoundResHandle> soundHandle(boost::static_pointer_cast<SoundResHandle>(g_appPtr->GetResourceCache()->GetHandle(&tmpRes)));
		if(!soundHandle)
		{
            GF_LOG_TRACE_ERR("LoadOpenALBufferFromResourceCache()", std::string("Failed to retrieve the sound fx handle from the resource cache: ") + resourceId);
			return (oalBufferPtr);
		}

		// Load the sound data using the appropriate third party sound lib (based on the sound file type).
		if(!soundHandle->VInitialize())
		{
            GF_LOG_TRACE_ERR("LoadOpenALBufferFromResourceCache()", std::string("Failed to load the sound file type: ") + resourceId);
			return (oalBufferPtr);
		}

		// Load sound data into an OpenAL audio buffer.
		try
		{
			oalBufferPtr = boost::static_pointer_cast<OpenALAudioBuffer>(g_oalAudioPtr->VInitAudioBuffer(soundHandle));
			if(!oalBufferPtr)
			{
                GF_LOG_TRACE_ERR("LoadOpenALBufferFromResourceCache()", std::string("Failed to create OpenAL audio buffer with ") + resourceId);
				return (oalBufferPtr);
			}
		}
		catch(GameException &ge)
		{
            GF_LOG_TRACE_ERR("LoadOpenALBufferFromResourceCache()", std::string("Failed to create OpenAL audio buffer with ") + resourceId + std::string(", Reason: ") + std::string(ge.what()));
			return (oalBufferPtr);
		}

		// Success - return valid OAL buffer.
		return (oalBufferPtr);
	}

}
