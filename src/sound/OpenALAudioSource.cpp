// ////////////////////////////////////////////////////////////////////
// @file OpenALAudioSource.cpp
// @author PJ O Halloran
// @date 12/10/2010
//
// File contains the implementation for the OpenALAudioSource class.
//
// ////////////////////////////////////////////////////////////////////

// External Headers
#include <string>


// Project Headers
#include "OpenALAudio.h"
#include "OpenALAudioSource.h"

#include "GameMain.h"


// ////////////////////////////////////////////////////////////////////
//
//
// ////////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	std::string OpenALAudioSource::GetALEnumString(const ALenum param) const
	{
		std::string paramStr;
		switch(param)
		{
            case AL_PITCH:				paramStr.assign("AL_PITCH");				break;
            case AL_GAIN:				paramStr.assign("AL_GAIN");					break;
            case AL_MAX_DISTANCE:		paramStr.assign("AL_MAX_DISTANCE");			break;
            case AL_ROLLOFF_FACTOR:		paramStr.assign("AL_ROLLOFF_FACTOR");		break;
            case AL_REFERENCE_DISTANCE:	paramStr.assign("AL_REFERENCE_DISTANCE");	break;
            case AL_MIN_GAIN:			paramStr.assign("AL_MIN_GAIN");				break;
            case AL_MAX_GAIN:			paramStr.assign("AL_MAX_GAIN");				break;
            case AL_CONE_OUTER_GAIN:	paramStr.assign("AL_CONE_OUTER_GAIN");		break;
            case AL_CONE_INNER_ANGLE:	paramStr.assign("AL_CONE_INNER_ANGLE");		break;
            case AL_CONE_OUTER_ANGLE:	paramStr.assign("AL_CONE_OUTER_ANGLE");		break;
            case AL_POSITION:			paramStr.assign("AL_POSITION");				break;
            case AL_VELOCITY:			paramStr.assign("AL_VELOCITY");				break;
            case AL_DIRECTION:			paramStr.assign("AL_DIRECTION");			break;
            case AL_SOURCE_RELATIVE:	paramStr.assign("AL_SOURCE_RELATIVE");		break;
            case AL_SOURCE_TYPE:		paramStr.assign("AL_SOURCE_TYPE");			break;
            case AL_LOOPING:			paramStr.assign("AL_LOOPING");				break;
            case AL_BUFFER:				paramStr.assign("AL_BUFFER");				break;
            case AL_SOURCE_STATE:		paramStr.assign("AL_SOURCE_STATE");			break;
            case AL_BUFFERS_QUEUED:		paramStr.assign("AL_BUFFERS_QUEUED");		break;
            case AL_BUFFERS_PROCESSED:	paramStr.assign("AL_BUFFERS_PROCESSED");	break;
            case AL_SEC_OFFSET:			paramStr.assign("AL_SEC_OFFSET");			break;
            case AL_SAMPLE_OFFSET:		paramStr.assign("AL_SAMPLE_OFFSET");		break;
            case AL_BYTE_OFFSET:		paramStr.assign("AL_BYTE_OFFSET");			break;
            default:					paramStr.assign("- Unknown AL enum -");		break;
		}
		return (paramStr);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::GetFloatParam(const ALenum param, ALfloat &value) const
	{
		GF_CLEAR_AL_ERROR();

		alGetSourcef(m_id, param, &value);
		if(!GF_CHECK_AL_ERROR())
		{
            GF_LOG_TRACE_ERR("OpenALAudioSource::GetFloatParam()", GetALEnumString(param));
			return (false);
		}

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::SetFloatParam(const ALenum param, const ALfloat value) const
	{
		GF_CLEAR_AL_ERROR();

		alSourcef(m_id, param, value);
		if(!GF_CHECK_AL_ERROR())
		{
            GF_LOG_TRACE_ERR("OpenALAudioSource::SetFloatParam()", GetALEnumString(param));
			return (false);
		}

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::GetFloatArrayParam(const ALenum param, const ALuint arrSize, ALfloat *arr) const
	{
		GF_CLEAR_AL_ERROR();

		alGetSourcefv(m_id, param, arr);
		if(!GF_CHECK_AL_ERROR())
		{
            GF_LOG_TRACE_ERR("OpenALAudioSource::GetFloatArrayParam()", GetALEnumString(param));
			return (false);
		}

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::SetFloatArrayParam(const ALenum param, const ALuint arrSize, const ALfloat * const arr) const
	{
		GF_CLEAR_AL_ERROR();

		alSourcefv(m_id, param, arr);
		if(!GF_CHECK_AL_ERROR())
		{
            GF_LOG_TRACE_ERR("OpenALAudioSource::SetFloatArrayParam()", GetALEnumString(param));
			return (false);
		}

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::GetIntParam(const ALenum param, ALint &value) const
	{
		GF_CLEAR_AL_ERROR();

		alGetSourcei(m_id, param, &value);
		if(!GF_CHECK_AL_ERROR())
		{
            GF_LOG_TRACE_ERR("OpenALAudioSource::GetIntParam()", GetALEnumString(param));
			return (false);
		}

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::SetIntParam(const ALenum param, const ALint value) const
	{
		GF_CLEAR_AL_ERROR();

		alSourcei(m_id, param, value);
		if(!GF_CHECK_AL_ERROR())
		{
            GF_LOG_TRACE_ERR("OpenALAudioSource::SetIntParam()", GetALEnumString(param));
			return (false);
		}

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::GetIntArrayParam(const ALenum param, const ALuint arrSize, ALint *arr) const
	{
		GF_CLEAR_AL_ERROR();

		alGetSourceiv(m_id, param, arr);
		if(!GF_CHECK_AL_ERROR())
		{
            GF_LOG_TRACE_ERR("OpenALAudioSource::GetIntArrayParam()", GetALEnumString(param));
			return (false);
		}

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::SetIntArrayParam(const ALenum param, const ALuint arrSize, const ALint * const arr) const
	{
		GF_CLEAR_AL_ERROR();

		alSourceiv(m_id, param, arr);
		if(!GF_CHECK_AL_ERROR())
		{
            GF_LOG_TRACE_ERR("OpenALAudioSource::SetIntArrayParam()", GetALEnumString(param));
			return (false);
		}

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	OpenALAudioSource::OpenALAudioSource(const Point3 &positionRef, const Vector3 &velRef, const Vector3 &directionRef)\
		: m_id(0), m_totalDuration(), m_lastUpdateTime(0.0f)
	{
		GF_CLEAR_AL_ERROR();

		alGenSources(1, &m_id);
		if(!GF_CHECK_AL_ERROR())
		{
            GF_LOG_TRACE_ERR("OpenALAudioSource::OpenALAudioSource()", "Failed to generate the OpenAL source ID");
			m_id = 0;
		}

		SetPosition(positionRef);
		SetVelocity(velRef);
		SetDirection(directionRef);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	OpenALAudioSource::~OpenALAudioSource()
	{
		try
		{
			// Stop playing buffers and detach all queued buffers
			if(IsPlaying())
			{
				Stop();
			}
			DetachBuffers();

			// Delete source name.
			if(IsValid())
			{
				GF_CLEAR_AL_ERROR();
				alDeleteSources(1, &m_id);
				GF_CHECK_AL_ERROR_TRC("OpenALAudioSource::~OpenALAudioSource(): ");
			}
		}
		catch(...)
		{
		}
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::GetPosition(Point3 &position) const
	{
		F32 arr[3];
		if(!GetFloatArrayParam(AL_POSITION, 3, arr))
		{
			return (false);
		}
		position.Set(arr[0], arr[1], arr[2]);
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::GetVelocity(Vector3 &velocity) const
	{
		F32 arr[3];
		if(!GetFloatArrayParam(AL_VELOCITY, 3, arr))
		{
			return (false);
		}
		velocity.Set(arr[0], arr[1], arr[2]);
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::GetDirection(Vector3 &direction) const
	{
		F32 arr[3];
		if(!GetFloatArrayParam(AL_DIRECTION, 3, arr))
		{
			return (false);
		}
		direction.Set(arr[0], arr[1], arr[2]);
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::Update(const Point3 &newPosition, const Vector3 &newDirection, const F32 timestamp)
	{
		// Set direction.
		SetDirection(newDirection);
		
		// Get old position and set new one.
		Point3 oldPosition;
		GetPosition(oldPosition);
		SetPosition(newPosition);

		// Update the sources velocity.
		Vector3 newVelocity(newPosition - oldPosition);
		F32 timediff = timestamp - m_lastUpdateTime;
		newVelocity /= timediff;
		SetVelocity(newVelocity);

		// Update last position change.
		m_lastUpdateTime = timestamp;
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::AttachBuffer(boost::shared_ptr<OpenALAudioBuffer> oalAudioBufferPtr)
	{
		if(!oalAudioBufferPtr)
		{
            GF_LOG_TRACE_ERR("OpenALAudioSource::AttachBuffer()", "The buffer supplied is NULL");
			return (false);
		}

		if(!SetIntParam(AL_BUFFER, oalAudioBufferPtr->GetId()))
		{
			return (false);
		}

		m_totalDuration = GetOpenALBufferDuration(oalAudioBufferPtr->GetId());
		if(!m_totalDuration.is_initialized())
		{
            GF_LOG_TRACE_ERR("OpenALAudioSource::AttachBuffer()", "Failed to calculate the duration for the buffer");
			return (false);
		}
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::AttachBufferList(const OpenALAudioBufferQueue &oalAudioBufferQueue)
	{
		if(oalAudioBufferQueue.empty())
		{
            GF_LOG_TRACE_ERR("OpenALAudioSource::AttachBufferList()", "The buffer queue is empty");
			return (false);
		}

		GF_CLEAR_AL_ERROR();

		ALuint *bufferIdArr = GCC_NEW ALuint[oalAudioBufferQueue.size()];
		if(!bufferIdArr)
		{
            GF_LOG_TRACE_ERR("OpenALAudioSource::AttachBufferList()", "Failed to allocate memory for the array of buffer IDs");
			return (false);
		}

		// Create ALuint buffer ID array.
		U32 index = 0;
		for(OpenALAudioBufferQueue::const_iterator i = oalAudioBufferQueue.begin(), end = oalAudioBufferQueue.end(); i != end; ++i, ++index)
		{
			if(!(*i))
			{
                GF_LOG_TRACE_ERR("OpenALAudioSource::AttachBufferList()", "One of the OpenALAudioBuffer objects in the queue was NULL");
				m_totalDuration.reset();
				delete [] bufferIdArr;
				return (false);
			}

			bufferIdArr[index] = (*i)->GetId();
			boost::optional<ALfloat> td = GetOpenALBufferDuration((*i)->GetId());
			if(td.is_initialized())
			{
				if(!m_totalDuration.is_initialized())
				{
					*m_totalDuration = 0.0f;
				}
				*m_totalDuration = *m_totalDuration + *td;
			}
			else
			{
                GF_LOG_TRACE_ERR("OpenALAudioSource::AttachBufferList()", "Failed to calculate the duration for the buffer");
			}
		}

		// Queue up the buffers on the source.
		alSourceQueueBuffers(m_id, static_cast<ALsizei>(oalAudioBufferQueue.size()), bufferIdArr);
		
		delete [] bufferIdArr;
		bufferIdArr = NULL;

		return (GF_CHECK_AL_ERROR_TRC("OpenALAudioSource::AttachBufferList(): "));
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::DetachBuffers()
	{
		if(!SetIntParam(AL_BUFFER, 0))
		{
			return (false);
		}

		m_totalDuration.reset();
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::IsBufferAttached(boost::shared_ptr<OpenALAudioBuffer> audioBufferPtr) const
	{
		if(!audioBufferPtr)
		{
			return (false);
		}

		ALint bufId;
		if(!GetIntParam(AL_BUFFER, bufId))
		{
			return (false);
		}

		if(bufId != audioBufferPtr->GetId())
		{
			return (false);
		}

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::Play(const F32 volume, const bool looping) const
	{
		if(!SetFloatParam(AL_GAIN, volume))
		{
			return (false);
		}

		if(!SetIntParam(AL_LOOPING, (looping ? AL_TRUE : AL_FALSE)))
		{
			return (false);
		}

		GF_CLEAR_AL_ERROR();
		alSourcePlay(m_id);
		return (GF_CHECK_AL_ERROR_TRC("OpenALAudioSource::Play(): "));
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::Pause() const
	{
		GF_CLEAR_AL_ERROR();
		alSourcePause(m_id);
		return (GF_CHECK_AL_ERROR_TRC("OpenALAudioSource::Pause(): "));
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::Stop() const
	{
		GF_CLEAR_AL_ERROR();
		alSourceStop(m_id);
		return (!GF_CHECK_AL_ERROR_TRC("OpenALAudioSource::Stop(): "));
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::Resume() const
	{
		GF_CLEAR_AL_ERROR();
		alSourcePlay(m_id);
		return (GF_CHECK_AL_ERROR_TRC("OpenALAudioSource::Resume(): "));
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudioSource::TogglePause() const
	{
		if(IsPlaying())
		{
			return (Pause());
		}
		else if (IsPaused())
		{
			return (Resume());
		}

		// State is stopped or undetermined so we just return true.
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	F32 OpenALAudioSource::GetProgress() const
	{
		if(!m_totalDuration.is_initialized())
		{
            GF_LOG_TRACE_ERR("OpenALAudioSource::GetProgress()", "The total duration of the source buffer(s) is not known");
			return (0.0f);
		}

		ALfloat secOffset;					// Time in seconds into playback.
		F32 x, y;
		GetPlaybackPosition(secOffset, x, y);

		return (secOffset / *m_totalDuration);
	}

}
