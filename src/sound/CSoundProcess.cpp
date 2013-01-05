//========================================================================
// Various classes and interfaces from the sound system.
//
// Part of the GameCode3 Application
//
// GameCode3 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 3rd Edition" by Mike McShaffry, published by
// Charles River Media. ISBN-10: 1-58450-680-6   ISBN-13: 978-1-58450-680-5
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the author a favor and buy a new copy of the book - there are 
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: 
//    http://www.amazon.com/gp/product/1584506806?ie=UTF8&tag=gamecodecompl-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=1584506806
//
// There's a companion web site at http://www.mcshaffry.com/GameCode/
// 
// The source code is managed and maintained through Google Code:
// http://gamecode3.googlecode.com/svn/trunk/
//
// (c) Copyright 2009 Michael L. McShaffry
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License v2
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================

#include <string>

#include "CAudio.h"
#include "CSoundProcess.h"
#include "CSoundResource.h"
#include "GameLog.h"
#include "GameMain.h"

using boost::shared_ptr;

using std::string;

namespace GameHalloran
{

	// ////////////////////////////////////////////////////////////////////
	// ******************** SoundProcess **********************************
	// ////////////////////////////////////////////////////////////////////

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	SoundProcess::SoundProcess(shared_ptr<SoundResHandle> soundResource, I32 typeOfSound, I32 volume, bool looping)\
		: CProcess(typeOfSound, 0), m_SoundResource(soundResource), m_Volume(volume), m_isLooping(looping)
	{
		InitializeVolume();
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	SoundProcess::~SoundProcess()
	{
		try
		{
			if (m_AudioBuffer)
			{
				if(g_audioPtr)
				{
					g_audioPtr->VReleaseAudioBuffer(m_AudioBuffer);
				}
				else
				{
                    GF_LOG_TRACE_ERR("~SoundProcess()", "The global audio pointer is NULL so cannot free audio buffer");
				}
			}
		}
		catch(...)
		{
		}
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void SoundProcess::InitializeVolume()
	{
		//See CProcess.h for the enum PROCESS_TYPE declaration
		//int typeOfSound = GetType();
		
		// TODO: SOmewhere set an adjusted volume based on game options
		// m_volume = g_GraphicalApp->GetVolume(typeOfSound);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	I32 SoundProcess::GetLengthMilli()
	{
		if(m_SoundResource)
		{
			return (m_SoundResource->GetLengthMilli());
		}
		else
		{
			return (0);
		}
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void SoundProcess::VOnInitialize()
	{
		//If the sound has never been... you know... then Play it for the very first time
		if (!m_SoundResource)
		{
            GF_LOG_TRACE_ERR("SoundProcess::VOnInitialize()", "Sound resource is NULL");
			return;
		}

		if(!g_audioPtr)
		{
            GF_LOG_TRACE_ERR("SoundProcess::VOnInitialize()", "Global audio pointer is NULL");
			return;
		}

		m_SoundResource->VInitialize();

		//This sound will manage it's own handle in the other thread
		m_AudioBuffer = g_audioPtr->VInitAudioBuffer(m_SoundResource);

		if (!m_AudioBuffer)
		{
            GF_LOG_TRACE_ERR("SoundProcess::VOnInitialize()", "Failed to initialize the audio buffer");
			VKill();
			return;
		}

		Play(m_Volume, m_isLooping);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void SoundProcess::VOnUpdate(const F32 elapsedTime)
	{
		// Call base
		CProcess::VOnUpdate(elapsedTime);

		if(!IsInitialUpdate() && !IsPlaying())
		{
			VKill();
		}

		if(IsDead() && IsLooping())
		{
			Replay();
		}
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool SoundProcess::IsPlaying()
	{
		if (!m_SoundResource || !m_AudioBuffer)
		{
			return false;
		}
		
		return (m_AudioBuffer->VIsPlaying());
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void SoundProcess::SetVolume(I32 volume)
	{
		if(m_AudioBuffer==NULL)
		{
            GF_LOG_TRACE_ERR("SoundProcess::SetVolume()", "The audio buffer is NULL");
			return;
		}

		m_Volume = volume;
		m_AudioBuffer->VSetVolume(volume);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	I32 SoundProcess::GetVolume()
	{
		if(m_AudioBuffer==NULL)
		{
            GF_LOG_TRACE_ERR("SoundProcess::GetVolume()", "The audio buffer is NULL");
			return 0;
		}

		m_Volume = m_AudioBuffer->VGetVolume();
		return (m_Volume);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void SoundProcess::VTogglePause()
	{
		if(m_AudioBuffer)
		{
			m_AudioBuffer->VTogglePause();
		}
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void SoundProcess::VKill()
	{
		if(IsPlaying())
		{
			Stop();
		}
		CProcess::VKill();
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void SoundProcess::Play(const I32 volume, const bool looping)
	{
		if(!m_AudioBuffer)
		{
            GF_LOG_TRACE_ERR("SoundProcess::Play()", "audio buffer is NULL");
			return;
		}
		
		m_AudioBuffer->VPlay(volume, looping);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void SoundProcess::Stop()
	{
		if(m_AudioBuffer)
		{
			m_AudioBuffer->VStop();
		}
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	F32 SoundProcess::GetProgress()
	{
		if (m_AudioBuffer)
		{
			return m_AudioBuffer->VGetProgress();
		}

        GF_LOG_TRACE_ERR("SoundProcess::GetProgress()", "audio buffer is NULL");
		return (0.0f);
	}

	// ////////////////////////////////////////////////////////////////////
	// ******************** ExplosionProcess ******************************
	// ////////////////////////////////////////////////////////////////////

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void ExplosionProcess::VOnInitialize()
	{
		CProcess::VOnInitialize();
		SoundResource resource("explosion.wav");
		shared_ptr<SoundResHandle> srh = boost::static_pointer_cast<SoundResHandle>(g_appPtr->GetResourceCache()->GetHandle(&resource));
		m_Sound.reset(GCC_NEW SoundProcess(srh));

		// Imagine cool explosion graphics setup code here!!!!
		//
		//
		//
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void ExplosionProcess::VOnUpdate(const F32 elapsedTime)
	{
		// Since the sound is the real pacing mechanism - we ignore deltaMilliseconds
		F32 progress = m_Sound->GetProgress();
		
		switch (m_Stage)
		{
			case 0:
				if (progress>0.55f)
				{
					++m_Stage;
					// Imagine secondary explosion effect launch right here!
				}
				break;

			case 1:
				if (progress>0.75f)
				{
					++m_Stage;
					// Imagine tertiary explosion effect launch right here!
				}
				break;

			default:
				break;
		}
	}

	// ////////////////////////////////////////////////////////////////////
	// ******************** CFadeProcess **********************************
	// ////////////////////////////////////////////////////////////////////

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	CFadeProcess::CFadeProcess(shared_ptr<SoundProcess> sound, const F32 fadeTime, const I32 endVolume)
	: CProcess(PROC_INTERPOLATOR)
	{
		m_Sound = sound;
		m_TotalFadeTime = fadeTime;
		m_StartVolume = sound->GetVolume();
		m_EndVolume = endVolume;
		m_ElapsedTime = 0.0f;

		VOnUpdate(0);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void CFadeProcess::VOnUpdate(const F32 elapsedTime)
	{
		if (!CProcess::IsInitialUpdate())
		{
			m_ElapsedTime += elapsedTime;
		}

		CProcess::VOnUpdate(elapsedTime);

		if (m_Sound->IsDead())
			VKill();

		F32 cooef = static_cast<F32>(m_ElapsedTime) / static_cast<F32>(m_TotalFadeTime);
		if (cooef>1.0f)
			cooef = 1.0f;
		if (cooef<0.0f)
			cooef = 0.0f;

		I32 newVolume = m_StartVolume + static_cast<I32>(static_cast<F32>(m_EndVolume - m_StartVolume) * cooef);

		if (m_ElapsedTime >= m_TotalFadeTime)
		{
			newVolume = m_EndVolume;
			VKill();
		}

		m_Sound->SetVolume(newVolume);
	}

}
