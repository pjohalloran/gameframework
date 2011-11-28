// ////////////////////////////////////////////////////////////////////
// @file OpenALAudio.cpp
// @author PJ O Halloran
// @date 12/10/2010
//
// File contains the implementation for the OpenALAudio class.
//
// ////////////////////////////////////////////////////////////////////

// External Headers
#include <string>
#include <cstring>

#include <boost/lexical_cast.hpp>


// Project Headers
#include "OpenALAudio.h"

#include "GameMain.h"


namespace GameHalloran
{
    
	// Set pointer to NULL initially.
	OpenALAudio *g_oalAudioPtr = NULL;

    // ////////////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////////////
    ALboolean OpenALAudio::AlSanityCheck(void)
    {
        // maybe not needed here????
//        if(!Audio::IsInitialized())
//        {
//            GF_LOG_TRACE_ERR("OpenALAudio::AlSanityCheck()", alGetErrorString(AL_INVALID_OPERATION));
//            return (AL_FALSE);
//        }
        
        ALCcontext *context = alcGetCurrentContext();
        if(context == NULL)
        {
            GF_LOG_TRACE_ERR("OpenALAudio::AlSanityCheck()", alGetErrorString(ALUT_ERROR_NO_CURRENT_CONTEXT));
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
                                 GameLog::ERR,\
                                 std::string("OpenALAudio::AlSanityCheck()"),\
                                 std::string(alGetErrorString(ALUT_ERROR_NO_CURRENT_CONTEXT)));
            return (AL_FALSE);
        }
        
        if(alGetError() != AL_NO_ERROR)
        {
            GF_LOG_TRACE_ERR("OpenALAudio::AlSanityCheck()", alGetErrorString(ALUT_ERROR_AL_ERROR_ON_ENTRY));
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
                                 GameLog::ERR,\
                                 std::string("OpenALAudio::AlSanityCheck()"),\
                                 std::string(alGetErrorString(ALUT_ERROR_AL_ERROR_ON_ENTRY)));
            return (AL_FALSE);
        }
        
        if(alcGetError (alcGetContextsDevice(context)) != ALC_NO_ERROR)
        {
            GF_LOG_TRACE_ERR("OpenALAudio::AlSanityCheck()", alGetErrorString(ALUT_ERROR_ALC_ERROR_ON_ENTRY));
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
                                 GameLog::ERR,\
                                 std::string("OpenALAudio::AlSanityCheck()"),\
                                 std::string(alGetErrorString(ALUT_ERROR_ALC_ERROR_ON_ENTRY)));
            return (AL_FALSE);
        }
        
        return (AL_TRUE);
    }
    
    // ////////////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////////////
    ALboolean OpenALAudio::AlInit()
    {
        if(Audio::IsInitialized())
        {
            GF_LOG_TRACE_ERR("OpenALAudio::AlInit()", alGetErrorString(AL_INVALID_OPERATION));
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
                                 GameLog::ERR,\
                                 std::string("OpenALAudio::AlInit()"),\
                                 std::string(alGetErrorString(AL_INVALID_OPERATION)));
            return (AL_FALSE);
        }
        
        ALCdevice *device = alcOpenDevice(NULL);
        if (device == NULL)
        {
            GF_LOG_TRACE_ERR("OpenALAudio::AlInit()", alGetErrorString(ALUT_ERROR_OPEN_DEVICE));
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
                                 GameLog::ERR,\
                                 std::string("OpenALAudio::AlInit()"),\
                                 std::string(alGetErrorString(ALUT_ERROR_OPEN_DEVICE)));
            return (AL_FALSE);
        }
        
        GF_CLEAR_AL_ERROR();
        
        m_contextPtr = alcCreateContext(device, NULL);
        if(m_contextPtr == NULL)
        {
            alcCloseDevice (device);
            GF_LOG_TRACE_ERR("OpenALAudio::AlInit()", alGetErrorString(ALUT_ERROR_CREATE_CONTEXT));
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
                                 GameLog::ERR,\
                                 std::string("OpenALAudio::AlInit()"),\
                                 std::string(alGetErrorString(ALUT_ERROR_CREATE_CONTEXT)));
            return (AL_FALSE);
        }
        
        if (!alcMakeContextCurrent(m_contextPtr))
        {
            alcDestroyContext (m_contextPtr);
            m_contextPtr = NULL;
            alcCloseDevice (device);
            GF_LOG_TRACE_ERR("OpenALAudio::AlInit()", alGetErrorString(ALUT_ERROR_MAKE_CONTEXT_CURRENT));
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
                                 GameLog::ERR,\
                                 std::string("OpenALAudio::AlInit()"),\
                                 std::string(alGetErrorString(ALUT_ERROR_MAKE_CONTEXT_CURRENT)));
            return (AL_FALSE);
        }
        
        return (AL_TRUE);
    }
    
    // ////////////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////////////
    ALboolean OpenALAudio::AlExit()
    {
        if(!Audio::IsInitialized())
        {
            GF_LOG_TRACE_ERR("OpenALAudio::AlExit()", alGetErrorString(AL_INVALID_OPERATION));
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
                                 GameLog::ERR,\
                                 std::string("OpenALAudio::AlExit()"),\
                                 std::string(alGetErrorString(AL_INVALID_OPERATION)));
            return (AL_FALSE);
        }
        
        GF_CLEAR_AL_ERROR();
        
        if(!AlSanityCheck())
        {
            return (AL_FALSE);
        }
        
        if(!alcMakeContextCurrent(NULL))
        {
            GF_LOG_TRACE_ERR("OpenALAudio::AlExit()", alGetErrorString(ALUT_ERROR_MAKE_CONTEXT_CURRENT));
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
                                 GameLog::ERR,\
                                 std::string("OpenALAudio::AlExit()"),\
                                 std::string(alGetErrorString(ALUT_ERROR_MAKE_CONTEXT_CURRENT)));
            return (AL_FALSE);
        }
        
        ALCdevice *device = alcGetContextsDevice(m_contextPtr);
        alcDestroyContext(m_contextPtr);
        m_contextPtr = NULL;
        
        if(alcGetError(device) != ALC_NO_ERROR)
        {
            GF_LOG_TRACE_ERR("OpenALAudio::AlExit()", alGetErrorString(ALUT_ERROR_DESTROY_CONTEXT));
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
                                 GameLog::ERR,\
                                 std::string("OpenALAudio::AlExit()"),\
                                 std::string(alGetErrorString(ALUT_ERROR_DESTROY_CONTEXT)));
            return (AL_FALSE);
        }
        
        if(!alcCloseDevice(device))
        {
            GF_LOG_TRACE_ERR("OpenALAudio::AlExit()", alGetErrorString(ALUT_ERROR_CLOSE_DEVICE));
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
                                 GameLog::ERR,\
                                 std::string("OpenALAudio::AlExit()"),\
                                 std::string(alGetErrorString(ALUT_ERROR_CLOSE_DEVICE)));
            return (AL_FALSE);
        }
        
        Audio::SetInitialized(false);
        return (AL_TRUE);
    }
    
	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	OpenALAudio::OpenALAudio() : Audio(), m_sourceVec(), m_actorSourceMap(), m_contextPtr(NULL)
	{
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	OpenALAudio::~OpenALAudio()
	{
		try
		{
			VShutdown();
		}
		catch(...)
		{
		}
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudio::SetMasterGain(const F32 masterGain)
	{
		if(!Audio::IsInitialized())
		{
			return (false);
		}

		GF_CLEAR_AL_ERROR();
		
		alListenerf(AL_GAIN, static_cast<ALfloat>(masterGain));

		return (GF_CHECK_AL_ERROR_TRC("OpenALAudio::SetMasterGain(): "));
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudio::GetMasterGain(F32 &masterGain) const
	{
		if(!Audio::IsInitialized())
		{
			return (false);
		}

		GF_CLEAR_AL_ERROR();
		
		ALfloat mg(0.0f);
		alGetListenerf(AL_GAIN, &mg);
		
		bool alError = !GF_CHECK_AL_ERROR_TRC("OpenALAudio::GetMasterGain(): ");

        if(alError)
            masterGain = static_cast<F32>(mg);
		return (alError);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudio::SetPosition(const Point3 &position)
	{
		if(!Audio::IsInitialized())
		{
			return (false);
		}

		GF_CLEAR_AL_ERROR();

		alListenerfv(AL_POSITION, position.GetComponentsConst());

		return (GF_CHECK_AL_ERROR_TRC("OpenALAudio::SetPosition(): "));
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudio::GetPosition(Point3 &position) const
	{
		if(!Audio::IsInitialized())
		{
			return (false);
		}

		GF_CLEAR_AL_ERROR();

		ALfloat arr[3];
		alGetListenerfv(AL_POSITION, arr);

		bool alError = !GF_CHECK_AL_ERROR_TRC("OpenALAudio::GetPosition(): ");

        if(alError)
            position.Set(arr[0], arr[1], arr[2]);
		return (alError);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudio::SetVelocity(const Vector3 &velocity)
	{
		if(!Audio::IsInitialized())
		{
			return (false);
		}

		GF_CLEAR_AL_ERROR();

		alListenerfv(AL_VELOCITY, velocity.GetComponentsConst());

		return (GF_CHECK_AL_ERROR_TRC("OpenALAudio::SetVelocity(): "));
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudio::GetVelocity(Vector3 &velocity) const
	{
		if(!Audio::IsInitialized())
		{
			return (false);
		}

		GF_CLEAR_AL_ERROR();

		ALfloat arr[3];
		alGetListenerfv(AL_VELOCITY, arr);

		bool alError = !GF_CHECK_AL_ERROR_TRC("OpenALAudio::GetVelocity(): ");

        if(alError)
            velocity.Set(arr[0], arr[1], arr[2]);
		return (alError);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudio::SetOrientation(const Vector3 &forward, const Vector3 &up)
	{
		if(!Audio::IsInitialized())
		{
			return (false);
		}

		GF_CLEAR_AL_ERROR();

		ALfloat arr[6];
		memcpy(arr, forward.GetComponentsConst(), sizeof(F32) * 3);
		memcpy(arr + 3, up.GetComponentsConst(), sizeof(F32) * 3);
		alListenerfv(AL_ORIENTATION, arr);

		return (GF_CHECK_AL_ERROR_TRC("OpenALAudio::SetOrientation(): "));
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudio::GetOrientation(Vector3 &forward, Vector3 &up)
	{
		if(!Audio::IsInitialized())
		{
			return (false);
		}

		GF_CLEAR_AL_ERROR();

		ALfloat arr[6];
		alGetListenerfv(AL_ORIENTATION, arr);

		bool alError = !GF_CHECK_AL_ERROR_TRC("OpenALAudio::GetOrientation(): ");

        if(alError)
        {
            forward.Set(arr[0], arr[1], arr[2]);
            up.Set(arr[3], arr[4], arr[5]);
        }
        
		return (alError);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudio::Update(const Point3 &newPosition, const Vector3 &newForward, const Vector3 &newUp, const F32 timediff)
	{
		if(!Audio::IsInitialized())
		{
			return (false);
		}

		// Set orientation.
		SetOrientation(newForward, newUp);
		
		// Get old position and set new one.
		Point3 oldPosition;
		GetPosition(oldPosition);
		SetPosition(newPosition);

		// Update the listeners velocity.
		Vector3 newVelocity(newPosition - oldPosition);
		newVelocity /= timediff;
		SetVelocity(newVelocity);

		// Iterate through all the scene audio sources and update them, finally remove any which are finished playing.
		for(SceneAudioSourceVector::iterator i = m_sourceVec.begin(); i != m_sourceVec.end(); )
		{
			//Point3 oldPosition;
			//Vector3 oldDirection;
			//(*i).m_src->GetPosition(oldPosition);
			//(*i).m_src->GetDirection(oldDirection);
			//(*i).m_src->Update(oldPosition, oldDirection, g_appPtr->GetCurrTime());

			if((*i).m_tempSrc && (*i).m_src->IsStopped())
			{
				i = m_sourceVec.erase(i);
			}
			else
			{
				++i;
			}
		}

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudio::VInitialize()
	{
		if(Audio::IsInitialized())
		{
			return (true);
		}

		// Init OpenAL.
		if(AlInit() != AL_TRUE)
		{
			return (false);
		}
        GF_CHECK_AL_ERROR_TRC("OpenALAudio::VInitialize(): ");

		// Sanity check - ensure default OAL context exists after alutInit()!
		if(!AlSanityCheck())
		{
			return (false);
		}

		// OpenAL is setup correctly at this point.
		Audio::SetInitialized(true);
		if(!m_AllSamples.empty())
		{
			m_AllSamples.clear();
		}

		// Print out OpenAL and ALUT version to log file.
		const char *alVersionStr = static_cast<const char *>(alGetString(AL_VERSION));
        GF_LOG_TRACE_INF("OpenALAudio::VInitialize()", std::string("AL version: ") + std::string(alVersionStr));

		// Initialize listener attributes with default values.
		SetPosition(g_originPt);
		SetVelocity(Vector3(0.0f));
		SetMasterGain(1.0f);
		SetOrientation(g_forward, g_up);

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void OpenALAudio::VShutdown()
	{
		if(Audio::IsInitialized())
		{
            GF_LOG_TRACE_INF("OpenALAudio::VShutdown()", "Shutting down the audio system");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::INF,\
									std::string("OpenALAudio::VShutdown()"),\
									std::string("Shutting down the audio system"));

			ClearAllSources();

			Audio::VShutdown();
			AlExit();
            GF_LOG_TRACE_INF("OpenALAudio::VShutdown()", "OpenAL system shutdown");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::INF,\
									std::string("OpenALAudio::VShutdown()"),\
									std::string("OpenAL system shutdown"));
			Audio::SetInitialized(false);
		}
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudio::AddActorSource(boost::shared_ptr<OpenALAudioSource> srcPtr, const ActorId actorId)
	{
		if(!Audio::IsInitialized())
		{
            GF_LOG_TRACE_ERR("OpenALAudio::AddActorSource()", "Audio system is not initialized");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::AddActorSource()"),\
									std::string("Audio system is not initialized"));
			return (false);
		}

		if(!srcPtr)
		{
            GF_LOG_TRACE_ERR("OpenALAudio::AddActorSource()", "The OpenALAudioSource pointer is NULL");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::AddActorSource()"),\
									std::string("The OpenALAudioSource pointer is NULL"));
			return (false);
		}

		boost::shared_ptr<OpenALAudioSource> actorSrcPtr = FindActorSource(actorId);
		if(actorSrcPtr)
		{
#if DEBUG
			// We found a source already associated with an actor in the list so we will log a trace message and remove it.
			std::string actorIdStr;
			try { actorIdStr = boost::lexical_cast<std::string, ActorId>(actorId); } catch(...) { }
            GF_LOG_TRACE_INF("OpenALAudio::AddActorSource()", std::string("Duplicate source found for actor ") + actorIdStr);
#endif
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::INF,\
									std::string("OpenALAudio::AddActorSource()"),\
									std::string("Duplicate source found for actor ") + actorIdStr);
			m_actorSourceMap.erase(actorId);
			actorSrcPtr.reset();
		}

		// Add the actors source to the map.
		m_actorSourceMap[actorId] = srcPtr;

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudio::AddSceneSource(boost::shared_ptr<OpenALAudioSource> srcPtr, const bool freeOnceStopped)
	{
		if(!Audio::IsInitialized())
		{
            GF_LOG_TRACE_ERR("OpenALAudio::AddSceneSource()", "Audio system is not initialized");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::AddSceneSource()"),\
									std::string("Audio system is not initialized"));
			return (false);
		}

		if(!srcPtr)
		{
            GF_LOG_TRACE_ERR("OpenALAudio::AddSceneSource()", "The OpenALAudioSource pointer is NULL");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::AddSceneSource()"),\
									std::string("The OpenALAudioSource pointer is NULL"));
			return (false);
		}

		if(freeOnceStopped && srcPtr->IsStopped())
		{
            GF_LOG_TRACE_ERR("OpenALAudio::AddSceneSource()", "The audio source cannot be added in a AL_STOPPED state");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::AddSceneSource()"),\
									std::string("The audio source cannot be added in a AL_STOPPED state"));
			return (false);
		}

		SceneAudioSource srcInfo;
		srcInfo.m_src = srcPtr;
		srcInfo.m_tempSrc = freeOnceStopped;
		m_sourceVec.push_back(srcInfo);

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	boost::shared_ptr<OpenALAudioSource> OpenALAudio::FindActorSource(const ActorId actorId)
	{
		if(!Audio::IsInitialized())
		{
            GF_LOG_TRACE_ERR("OpenALAudio::FindActorSource()", "Audio system is not initialized");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::FindActorSource()"),\
									std::string("Audio system is not initialized"));
			return (boost::shared_ptr<OpenALAudioSource>());
		}

		std::map<ActorId, boost::shared_ptr<OpenALAudioSource> >::const_iterator i = m_actorSourceMap.find(actorId);
		if(i == m_actorSourceMap.end())
		{
			return (boost::shared_ptr<OpenALAudioSource>());
		}

		return ((*i).second);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	boost::shared_ptr<OpenALAudioSource> OpenALAudio::GetSceneSource(const ALuint srcId)
	{
		if(!Audio::IsInitialized())
		{
            GF_LOG_TRACE_ERR("OpenALAudio::GetSceneSource()", "Audio system is not initialized");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::GetSceneSource()"),\
									std::string("Audio system is not initialized"));
			return (boost::shared_ptr<OpenALAudioSource>());
		}

		bool found(false);
		boost::shared_ptr<OpenALAudioSource> src;

		// Search for the source in the container.
		for(SceneAudioSourceVector::iterator vecIter = m_sourceVec.begin(), endIter = m_sourceVec.end(); ((!found) && (vecIter != endIter)); )
		{
			if((*vecIter).m_src->GetId() == srcId)
			{
				found = true;
				src = (*vecIter).m_src;
			}
			else
			{
				++vecIter;
			}
		}

		return (src);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudio::RemoveActorSource(const ActorId actorId)
	{
		if(!Audio::IsInitialized())
		{
            GF_LOG_TRACE_ERR("OpenALAudio::RemoveActorSource()", "Audio system is not initialized");
			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::RemoveActorSource()"),\
									std::string("Audio system is not initialized"));
			return (false);
		}

		std::map<ActorId, boost::shared_ptr<OpenALAudioSource> >::iterator i = m_actorSourceMap.find(actorId);
		if(i == m_actorSourceMap.end())
		{
			std::string actorIdStr;
			try { actorIdStr = boost::lexical_cast<std::string, ActorId>(actorId); } catch(...) { }
            GF_LOG_TRACE_ERR("OpenALAudio::RemoveActorSource()", std::string("No actor in container matching id ") + actorIdStr);
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::RemoveActorSource()"),\
									std::string("No actor in container matching id ") + actorIdStr);
			return (false);
		}

		boost::shared_ptr<OpenALAudioSource> srcToRemovePtr(i->second);
		if(!srcToRemovePtr)
		{
            GF_LOG_TRACE_ERR("OpenALAudio::RemoveActorSource()", "NULL pointer found in container");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::RemoveActorSource()"),\
									std::string("NULL pointer found in container"));
			return (false);
		}

		// Clear the map entry
		m_actorSourceMap.erase(i);

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudio::RemoveSceneSource(const ALuint srcId)
	{
		if(!Audio::IsInitialized())
		{
            GF_LOG_TRACE_ERR("OpenALAudio::RemoveSceneSource()", "Audio system is not initialized");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::RemoveSceneSource()"),\
									std::string("Audio system is not initialized"));
			return (false);
		}

		// Search for the source in the container and remove it if its in there.
		bool removed(false);
		for(SceneAudioSourceVector::iterator vecIter = m_sourceVec.begin(), endIter = m_sourceVec.end(); ((!removed) && (vecIter != endIter)); )
		{
			if((*vecIter).m_src->GetId() == srcId)
			{
				removed = true;
				vecIter = m_sourceVec.erase(vecIter);
			}
			else
			{
				++vecIter;
			}
		}

		return (removed);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void OpenALAudio::ClearAllSources()
	{
		for(SceneAudioSourceVector::iterator i = m_sourceVec.begin(), end = m_sourceVec.end(); i != end; ++i)
		{
			(*i).m_src->Stop();
			(*i).m_src->DetachBuffers();
		}
		m_sourceVec.clear();

		for(ActorAudioSourceMap::iterator i = m_actorSourceMap.begin(), end = m_actorSourceMap.end(); i != end; ++i)
		{
			(i->second)->Stop();
			(i->second)->DetachBuffers();
		}
		m_actorSourceMap.clear();
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool OpenALAudio::VActive()
	{
		// Check if the default context has been made.
		return (alcGetCurrentContext() != NULL);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	boost::shared_ptr<IAudioBuffer> OpenALAudio::VInitAudioBuffer(boost::shared_ptr<SoundResHandle> soundResource)
	{
		if(!Audio::IsInitialized())
		{
            GF_LOG_TRACE_ERR("", "");
			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::VInitAudioBuffer()"),\
									std::string("Audio system is not initialized"));
			return (boost::shared_ptr<IAudioBuffer>());
		}

		if(!soundResource)
		{
            GF_LOG_TRACE_ERR("", "");
			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::VInitAudioBuffer()"),\
									std::string("Sound Resource Handle is NULL"));
			return (boost::shared_ptr<IAudioBuffer>());
		}

		if(!soundResource->VInitialize())
		{
            GF_LOG_TRACE_ERR("", "");
			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::VInitAudioBuffer()"),\
									std::string("Failed to initialize the PCM buffers in the Sound Resource"));
			return (boost::shared_ptr<IAudioBuffer>());
		}

		boost::shared_ptr<OpenALAudioBuffer> buffer;
		try
		{
			buffer.reset(GCC_NEW OpenALAudioBuffer(soundResource));
			if(!buffer)
			{
                GF_LOG_TRACE_ERR("", "");
				SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
										GameLog::ERR,\
										std::string("OpenALAudio::VInitAudioBuffer()"),\
										std::string("Failed to create the AudioBuffer"));
				return (boost::shared_ptr<IAudioBuffer>());
			}
		}
		catch (GameException &ge)
		{
            GF_LOG_TRACE_ERR("", "");
			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::VInitAudioBuffer()"),\
									std::string("Failed to create the AudioBuffer: ") + std::string(ge.what()));
			return (boost::shared_ptr<IAudioBuffer>());
		}

		m_AllSamples.push_back(boost::static_pointer_cast<IAudioBuffer>(buffer));
		return (boost::static_pointer_cast<IAudioBuffer>(buffer));
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void OpenALAudio::VReleaseAudioBuffer(boost::shared_ptr<IAudioBuffer> audioBuffer)
	{
		if(!Audio::IsInitialized())
		{
            GF_LOG_TRACE_ERR("OpenALAudio::VReleaseAudioBuffer()", "Audio system is not initialized");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::VReleaseAudioBuffer()"),\
									std::string("Audio system is not initialized"));
			return;
		}

		if(!audioBuffer)
		{
            GF_LOG_TRACE_ERR("OpenALAudio::VReleaseAudioBuffer()", "The audio buffer is NULL");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::VReleaseAudioBuffer()"),\
									std::string("The audio buffer is NULL"));
			return;
		}

		boost::shared_ptr<OpenALAudioBuffer> oalBuffer = boost::static_pointer_cast<OpenALAudioBuffer>(audioBuffer);
		if(!oalBuffer)
		{
            GF_LOG_TRACE_ERR("OpenALAudio::VReleaseAudioBuffer()", "The audio buffer is not an instance of OpenALAudioBuffer");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::VReleaseAudioBuffer()"),\
									std::string("The audio buffer is not an instance of OpenALAudioBuffer"));
			return;
		}

		oalBuffer->VStop();

		// All sources with a reference to this buffer must detach from it before it can be deleted (see OpenAL spec. 1.1)....
		for(SceneAudioSourceVector::iterator i = m_sourceVec.begin(), end = m_sourceVec.end(); i != end; ++i)
		{
			if((*i).m_src->IsBufferAttached(oalBuffer))
			{
				(*i).m_src->Stop();
				(*i).m_src->DetachBuffers();
			}
		}
		for(ActorAudioSourceMap::iterator i = m_actorSourceMap.begin(), end = m_actorSourceMap.end(); i != end; ++i)
		{
			if((i->second)->IsBufferAttached(oalBuffer))
			{
				(i->second)->Stop();
				(i->second)->DetachBuffers();
			}
		}

		// Search for and delete the audio buffer from the base classes data structures.
		bool found(false);
		for(AudioBufferList::iterator i = m_AllSamples.begin(); ((!found) && (i != m_AllSamples.end())); )
		{
			boost::shared_ptr<OpenALAudioBuffer> iOalBuffer = boost::static_pointer_cast<OpenALAudioBuffer>((*i));
			if(iOalBuffer->GetId() == oalBuffer->GetId())
			{
				i = m_AllSamples.erase(i);
				found = true;
			}
			else
			{
				++i;
			}
		}
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void OpenALAudio::VStopAllSounds()
	{
		if(!Audio::IsInitialized())
		{
            GF_LOG_TRACE_ERR("OpenALAudio::VStopAllSounds()", "Audio system is not initialized");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::VStopAllSounds()"),\
									std::string("Audio system is not initialized"));
			return;
		}

		for(SceneAudioSourceVector::iterator i = m_sourceVec.begin(), end = m_sourceVec.end(); i != end; ++i)
		{
			(*i).m_src->Stop();
		}
		for(ActorAudioSourceMap::iterator i = m_actorSourceMap.begin(), end = m_actorSourceMap.end(); i != end; ++i)
		{
			(i->second)->Stop();
		}

		Audio::SetAllPaused(false);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void OpenALAudio::VPauseAllSounds()
	{
		if(!Audio::IsInitialized())
		{
            GF_LOG_TRACE_ERR("OpenALAudio::VPauseAllSounds()", "Audio system is not initialized");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::VPauseAllSounds()"),\
									std::string("Audio system is not initialized"));
			return;
		}

		for(SceneAudioSourceVector::iterator i = m_sourceVec.begin(), end = m_sourceVec.end(); i != end; ++i)
		{
			(*i).m_src->Pause();
		}
		for(ActorAudioSourceMap::iterator i = m_actorSourceMap.begin(), end = m_actorSourceMap.end(); i != end; ++i)
		{
			(i->second)->Pause();
		}

		Audio::SetAllPaused(true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void OpenALAudio::VResumeAllSounds()
	{
		if(!Audio::IsInitialized())
		{
            GF_LOG_TRACE_ERR("OpenALAudio::VResumeAllSounds()", "Audio system is not initialized");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("OpenALAudio::VResumeAllSounds()"),\
									std::string("Audio system is not initialized"));
			return;
		}

		for(SceneAudioSourceVector::iterator i = m_sourceVec.begin(), end = m_sourceVec.end(); i != end; ++i)
		{
			(*i).m_src->Resume();
		}
		for(ActorAudioSourceMap::iterator i = m_actorSourceMap.begin(), end = m_actorSourceMap.end(); i != end; ++i)
		{
			(i->second)->Resume();
		}

		Audio::SetAllPaused(false);
	}
    
	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
    const char *alGetErrorString (ALenum error)
    {
        switch (error)
        {
            case AL_NO_ERROR:
                return "No AL error found";
                
            case AL_OUT_OF_MEMORY:
                return "AL ran out of memory";
                
            case AL_INVALID_ENUM:
                return "AL was given an invalid enumeration token";
                
            case AL_INVALID_VALUE:
                return "AL was given an invalid value";
                
            case AL_INVALID_NAME:
                return "AL was given an invalid name";
                
            case AL_INVALID_OPERATION:
                return "The operation was invalid in the current AL state";
                
            case ALUT_ERROR_NO_CURRENT_CONTEXT:
                return "There is no current AL context";
                
            case ALUT_ERROR_AL_ERROR_ON_ENTRY:
                return "There was already an AL error on entry to an ALUT function";
                
            case ALUT_ERROR_ALC_ERROR_ON_ENTRY:
                return "There was already an ALC error on entry to an ALUT function";
                
            case ALUT_ERROR_OPEN_DEVICE:
                return "There was an error opening the ALC device";
                
            case ALUT_ERROR_CLOSE_DEVICE:
                return "There was an error closing the ALC device";
                
            case ALUT_ERROR_CREATE_CONTEXT:
                return "There was an error creating an ALC context";
                
            case ALUT_ERROR_MAKE_CONTEXT_CURRENT:
                return "Could not change the current ALC context";
                
            case ALUT_ERROR_DESTROY_CONTEXT:
                return "There was an error destroying the ALC context";
                
            case ALUT_ERROR_GEN_BUFFERS:
                return "There was an error generating an AL buffer";
                
            case ALUT_ERROR_BUFFER_DATA:
                return "There was an error passing buffer data to AL";
                
            case ALUT_ERROR_IO_ERROR:
                return "I/O error";
                
            case ALUT_ERROR_UNSUPPORTED_FILE_TYPE:
                return "Unsupported file type";
                
            case ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE:
                return "Unsupported mode within an otherwise usable file type";
                
            case ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA:
                return "The sound data was corrupt or truncated";
                
            default:
                return "An impossible AL error condition was reported?!?";
        }
    }
    
	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
    bool GfCheckAlError()
    {
#if DEBUG
        ALenum err = alGetError();
        if(err != AL_NO_ERROR)
            GF_LOG_ERR(alGetErrorString(err));
        return (err == AL_NO_ERROR);
#else
        return (true);
#endif
    }
    
	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
    bool GfCheckAlErrorTrc(const char *msg)
    {
#if DEBUG
        ALenum err = alGetError();
        if(err != AL_NO_ERROR)
            GF_LOG_ERR(std::string(msg) + alGetErrorString(err));
        return (err == AL_NO_ERROR);
#else
        return (true);
#endif
    }
    
}
