#ifdef TARGET_WINDOWS || _WINDOWS || WIN32

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

// External Headers
#include <string>

#include <DXUT/DXUT.h>
#include <Dxerr.h>

// Project Headers
#include "DirectSoundAudio.h"
#include "CSoundResource.h"

#include "GameBase.h"
#include "GameLog.h"
#include "SoundCommon.h"

// For the global application pointer.
#include "GameMain.h"

using std::string;

using boost::shared_ptr;

namespace GameHalloran
{

	// ////////////////////////////////////////////////////////////////////
	// ***************** DirectSound8Audio ********************************
	// ////////////////////////////////////////////////////////////////////

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool DirectSound8Audio::VInitialize()
	{
		if(Audio::IsInitialized())
		{
			return (true);
		}

		Audio::SetInitialized(false);
		Release(m_pDS);
		HRESULT hr;

		// Create IDirectSound using the primary sound device
		if(FAILED(hr = DirectSoundCreate8(NULL, &m_pDS, NULL)))
		{
            GF_LOG_TRACE_ERR("DirectSound8Audio::VInitialize()", "Failed to initialized the DirectSound interface");
			return (false);
		}

		// Set DirectSound coop level 
		if(FAILED(hr = m_pDS->SetCooperativeLevel(m_hWnd, DSSCL_PRIORITY)))
		{
            GF_LOG_TRACE_ERR("DirectSound8Audio::VInitialize()", "Failed to set the coop level of the DirectSound interface");
			return (false);
		}

		if(FAILED(hr = SetPrimaryBufferFormat(8, 44100, 16)))
		{
            GF_LOG_TRACE_ERR("DirectSound8Audio::VInitialize()", "Failed to set the primary buffer format of the DirectSound interface");
			return (false);
		}

		Audio::SetInitialized(true);
		m_AllSamples.clear();

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	HRESULT DirectSound8Audio::SetPrimaryBufferFormat( 
		DWORD dwPrimaryChannels, 
		DWORD dwPrimaryFreq, 
		DWORD dwPrimaryBitRate )
	{
		// !WARNING! - Setting the primary buffer format and then using this
		// it for DirectMusic messes up DirectMusic! 
		//
		// If you want your primary buffer format to be 22kHz stereo, 16-bit
		// call with these parameters:	SetPrimaryBufferFormat(2, 22050, 16);

		HRESULT             hr;
		LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

		if(m_pDS == NULL)
		{
			return (CO_E_NOTINITIALIZED);
		}

		// Get the primary buffer 
		DSBUFFERDESC dsbd;
		ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
		dsbd.dwSize        = sizeof(DSBUFFERDESC);
		dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
		dsbd.dwBufferBytes = 0;
		dsbd.lpwfxFormat   = NULL;

		if(FAILED(hr = m_pDS->CreateSoundBuffer(&dsbd, &pDSBPrimary, NULL)))
		{
			return (DXUT_ERR(L"CreateSoundBuffer", hr));
		}

		WAVEFORMATEX wfx;
		ZeroMemory(&wfx, sizeof(WAVEFORMATEX));
		wfx.wFormatTag      = (WORD) WAVE_FORMAT_PCM; 
		wfx.nChannels       = (WORD) dwPrimaryChannels; 
		wfx.nSamplesPerSec  = (DWORD) dwPrimaryFreq; 
		wfx.wBitsPerSample  = (WORD) dwPrimaryBitRate; 
		wfx.nBlockAlign     = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
		wfx.nAvgBytesPerSec = (DWORD) (wfx.nSamplesPerSec * wfx.nBlockAlign);

		if(FAILED(hr = pDSBPrimary->SetFormat(&wfx)))
		{
			return (DXUT_ERR(L"SetFormat", hr));
		}

		Release(pDSBPrimary);

		return (S_OK);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void DirectSound8Audio::VShutdown()
	{
		if(Audio::IsInitialized())
		{
			Audio::VShutdown();
			Release(m_pDS);
			Audio::SetInitialized(false);
		}
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	boost::shared_ptr<IAudioBuffer> DirectSound8Audio::VInitAudioBuffer(shared_ptr<SoundResHandle> soundResource)//const
	{
		boost::shared_ptr<IAudioBuffer> null;
		const char *fileExtension = FindExtFromSoundType(soundResource->GetSoundType());

		if(m_pDS == NULL)
		{
			return (null);
		}

		switch(soundResource->GetSoundType())
		{
			case SOUND_TYPE_OGG:
			case SOUND_TYPE_WAVE:
				// We support WAVs and OGGs
				break;

			case SOUND_TYPE_MP3:
			case SOUND_TYPE_MIDI:	//If it's a midi file, then do nothin at this time... maybe we will support this in the future
                GF_LOG_TRACE_ERR("DirectSound8Audio::VInitAudioBuffer()", "MP3s and MIDI are not supported");
				return (null);
				break;

			default:
                GF_LOG_TRACE_ERR("DirectSound8Audio::VInitAudioBuffer()", "Unknown sound file");
				return (null);
				break;
		}//end switch

		LPDIRECTSOUNDBUFFER sampleHandle;

		// Create the direct sound buffer, and only request the flags needed
		// since each requires some overhead and limits if the buffer can 
		// be hardware accelerated
		DSBUFFERDESC dsbd;
		ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
		dsbd.dwSize          = sizeof(DSBUFFERDESC);
		dsbd.dwFlags         = DSBCAPS_CTRLVOLUME;
		dsbd.dwBufferBytes   = soundResource->GetPCMBufferSize();
		dsbd.guid3DAlgorithm = GUID_NULL;
		dsbd.lpwfxFormat     = const_cast<WAVEFORMATEX *>(soundResource->GetFormat());

		HRESULT hr;
		if(FAILED(hr = m_pDS->CreateSoundBuffer(&dsbd, &sampleHandle, NULL)))
		{
            GF_LOG_TRACE_ERR("DirectSound8Audio::VInitAudioBuffer()", "");
			return (null);
		}

		// Add handle to the list
		boost::shared_ptr<IAudioBuffer> audioBuffer(GCC_NEW DirectSound8AudioBuffer(sampleHandle, soundResource));
		if(audioBuffer)
		{
			m_AllSamples.insert(m_AllSamples.begin(), audioBuffer);
		}
		
		return (audioBuffer);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void DirectSound8Audio::VReleaseAudioBuffer(boost::shared_ptr<IAudioBuffer> sampleHandle)//const
	{
		sampleHandle->VStop();
		m_AllSamples.remove(sampleHandle);
	}

	// ////////////////////////////////////////////////////////////////////
	// *******************DirectSound8AudioBuffer**************************
	// ////////////////////////////////////////////////////////////////////

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	DirectSound8AudioBuffer::DirectSound8AudioBuffer(LPDIRECTSOUNDBUFFER sample, shared_ptr<SoundResHandle> resource)\
		: AudioBuffer(resource), m_Sample(sample)
	{
		FillBufferWithSound();
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void *DirectSound8AudioBuffer::VGet()
	{
 		if (!VOnRestore())
		{
            GF_LOG_TRACE_ERR("DirectSound8AudioBuffer::VGet()", "Failed to restore the buffer");
			return (NULL);
		}

		return (m_Sample);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool DirectSound8AudioBuffer::VPlay(const I32 volume, const bool looping)
	{
		VStop();

		// Set base class attributs.
		AudioBuffer::VSetVolume(volume);
		AudioBuffer::VSetLooping(looping);

		// Get DirectSound buffer.
		LPDIRECTSOUNDBUFFER pDSB = static_cast<LPDIRECTSOUNDBUFFER>(VGet());
		if (!pDSB)
		{
            GF_LOG_TRACE_ERR("DirectSound8AudioBuffer::VPlay()", "Failed to cast the buffer to a DirectSound buffer");
			return (false);
		}

		// Set the buffers volume and play it.
		pDSB->SetVolume(volume);
		DWORD dwFlags = looping ? DSBPLAY_LOOPING : 0L;
		return (S_OK == pDSB->Play(0, 0, dwFlags));
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool DirectSound8AudioBuffer::VStop()
	{
		LPDIRECTSOUNDBUFFER pDSB = static_cast<LPDIRECTSOUNDBUFFER>(VGet());

		// If the global audio pointer is NULL OR the sound system is not active.
		if(!g_audioPtr || !g_audioPtr->VActive())
		{
            GF_LOG_TRACE_ERR("DirectSound8AudioBuffer::VStop()", "The sound system is not active");
			return (false);
		}
		if(pDSB == NULL)
		{
            GF_LOG_TRACE_ERR("DirectSound8AudioBuffer::VStop()", "Failed to cast the buffer to a DirectSound buffer");
			return (false);
		}

		// Set the base class attribute and stop playback.
		AudioBuffer::VSetPaused(true);
		pDSB->Stop();
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool DirectSound8AudioBuffer::VPause()
	{
		LPDIRECTSOUNDBUFFER pDSB = static_cast<LPDIRECTSOUNDBUFFER>(VGet());

		// If the global audio pointer is NULL OR the sound system is not active.
		if(!g_audioPtr || !g_audioPtr->VActive())
		{
            GF_LOG_TRACE_ERR("DirectSound8AudioBuffer::VPause()", "The sound system is not active");
			return (false);
		}
		if(pDSB == NULL)
		{
            GF_LOG_TRACE_ERR("DirectSound8AudioBuffer::VPause()", "Failed to cast the buffer to a DirectSound buffe");
			return (false);
		}

		// TODO: See if VPause and VStop are mixed up (SetCurrentPosition here and not in VStop???)

		AudioBuffer::VSetPaused(true);
		pDSB->Stop();
		pDSB->SetCurrentPosition(0);	// rewinds buffer to beginning.
		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool DirectSound8AudioBuffer::VResume()
	{
		AudioBuffer::VSetPaused(false);
		return (VPlay(VGetVolume(), VIsLooping()));
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool DirectSound8AudioBuffer::VTogglePause()
	{
		if(!g_audioPtr || !g_audioPtr->VActive())
		{
			return (false);
		}

		bool result = true;
		if(AudioBuffer::VIsPaused())
		{
			result = VResume();
		}
		else
		{
			result = VPause();				// note that the book code calls VStop().
									//   It's better to call VPause() instead.
		}

		return (result);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool DirectSound8AudioBuffer::VIsPlaying()
	{
		if(!g_audioPtr || !g_audioPtr->VActive())
		{
            GF_LOG_TRACE_ERR("DirectSound8AudioBuffer::VIsPlaying()", "The sound system is not active");
			return (false);
		}

		DWORD dwStatus = 0;
		LPDIRECTSOUNDBUFFER pDSB = static_cast<LPDIRECTSOUNDBUFFER>(VGet());
		if(!pDSB)
		{
            GF_LOG_TRACE_ERR("DirectSound8AudioBuffer::VIsPlaying()", "Failed to cast the buffer to a DirectSound buffer");
			return (false);
		}

		pDSB->GetStatus(&dwStatus);
		bool bIsPlaying = ((dwStatus & DSBSTATUS_PLAYING) != 0);

		return (bIsPlaying);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	void DirectSound8AudioBuffer::VSetVolume(I32 volume)
	{
		if(!g_audioPtr || !g_audioPtr->VActive())
		{
            GF_LOG_TRACE_ERR("DirectSound8AudioBuffer::VSetVolume()", "The sound system is not active");
			return;
		}

		LPDIRECTSOUNDBUFFER pDSB = static_cast<LPDIRECTSOUNDBUFFER>(VGet());
		if(!pDSB)
		{
            GF_LOG_TRACE_ERR("DirectSound8AudioBuffer::VSetVolume()", "Failed to cast the buffer to a DirectSound buffer");
			return;
		}

		if(volume < 0)
		{
            GF_LOG_TRACE_INF("DirectSound8AudioBuffer::VSetVolume()", "Volume cannot be less than 0 so we are setting it to 0");
			volume = 0;
		}
		else if(volume > 100)
		{
            GF_LOG_TRACE_INF("DirectSound8AudioBuffer::VSetVolume()", "The volume cannot be greater than 100 so we are setting it to 100");
			volume = 100;
		}

		// convert volume from 0-100 into range for DirectX - don't forget to use a logarithmic scale!
		F32 coeff = (F32)volume / 100.0f;
		F32 logarithmicProportion = coeff >0.1f  ? 1+log10(coeff)  : 0;
		F32 range = (DSBVOLUME_MAX - GCC_DSBVOLUME_MIN);
		F32 fvolume = (range * logarithmicProportion) + GCC_DSBVOLUME_MIN;
		assert(fvolume>=GCC_DSBVOLUME_MIN && fvolume<=DSBVOLUME_MAX);

		HRESULT hr = pDSB->SetVolume(LONG(fvolume));
		if(hr != S_OK)
		{
            GF_LOG_TRACE_ERR("DirectSound8AudioBuffer::VSetVolume()", "Failed to set the volum of the DirectSound Buffer");
		}
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool DirectSound8AudioBuffer::VOnRestore()
	{
	   HRESULT hr;
	   BOOL bRestored;

		// Restore the buffer if it was lost
		if(FAILED(hr = RestoreBuffer(&bRestored)))
		{
            GF_LOG_TRACE_ERR("DirectSound8AudioBuffer::VOnRestore()", "Failed to restore the buffer");
			return (false);
		}

		if(bRestored)
		{
			// The buffer was restored, so we need to fill it with new data
			if(FAILED(hr = FillBufferWithSound()))
			{
                GF_LOG_TRACE_ERR("DirectSound8AudioBuffer::VOnRestore()", "Failed to fill the buffer with sound");
				return (false);
			}
		}
		else
		{
            GF_LOG_TRACE_ERR("DirectSound8AudioBuffer::VOnRestore()", "Failed to restore the buffer so cannot fill it with sound");
		}

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	HRESULT DirectSound8AudioBuffer::RestoreBuffer(BOOL* pbWasRestored)
	{
		HRESULT hr;

		if( m_Sample == NULL )
		{
			return CO_E_NOTINITIALIZED;
		}

		if(pbWasRestored)
		{
			*pbWasRestored = FALSE;
		}

		DWORD dwStatus;
		if(FAILED(hr = m_Sample->GetStatus( &dwStatus)))
		{
			return DXUT_ERR(L"GetStatus", hr);
		}

		if(dwStatus & DSBSTATUS_BUFFERLOST)
		{
			// Since the app could have just been activated, then
			// DirectSound may not be giving us control yet, so 
			// the restoring the buffer may fail.  
			// If it does, sleep until DirectSound gives us control.

			// We will try to restore it a maxmimum number of times before giving up.
			const I32 NUM_RETRIES = 20;
			I32 i = 1;
			do 
			{
				hr = m_Sample->Restore();
				if(hr == DSERR_BUFFERLOST)
				{
					Sleep(10);
				}

				++i;
			}
			while((i <= NUM_RETRIES) && ((hr = m_Sample->Restore()) == DSERR_BUFFERLOST));

			if(i >= NUM_RETRIES)
			{
                GF_LOG_TRACE_DEB("DirectSound8AudioBuffer::RestoreBuffer()", "Failed to restore the buffer after NUM_RETRIES tries.  DirectSound seems to be in a broken state...");
			}
			else
			{
				if(pbWasRestored)
				{
					*pbWasRestored = TRUE;
				}
			}

			return (S_OK);
		}
		else
		{
			return (S_FALSE);
		}
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	HRESULT DirectSound8AudioBuffer::FillBufferWithSound(void)
	{
		HRESULT hr; 
		VOID	*pDSLockedBuffer = NULL;	 // a pointer to the DirectSound buffer
		DWORD   dwDSLockedBufferSize = 0;    // Size of the locked DirectSound buffer
		DWORD   dwWavDataRead        = 0;    // Amount of data read from the wav file 

		if(m_Sample == NULL)
		{
			return (CO_E_NOTINITIALIZED);
		}

		// Make sure we have focus, and we didn't just switch in from
		// an app which had a DirectSound device
		if(FAILED(hr = RestoreBuffer(NULL))) 
		{
			return DXUT_ERR( L"RestoreBuffer", hr );
		}

		I32 pcmBufferSize = m_Resource->GetPCMBufferSize();
		// Lock the buffer down
		if(FAILED(hr = m_Sample->Lock( 0, pcmBufferSize, 
									 &pDSLockedBuffer, &dwDSLockedBufferSize, 
									 NULL, NULL, 0L)))
		{
			return (DXUT_ERR(L"Lock", hr));
		}

		if(pcmBufferSize == 0)
		{
			// Wav is blank, so just fill with silence
			FillMemory((BYTE*) pDSLockedBuffer, 
						dwDSLockedBufferSize, 
						(BYTE)(m_Resource->GetFormat()->wBitsPerSample == 8 ? 128 : 0));
		}
		else 
		{
			CopyMemory(pDSLockedBuffer, m_Resource->GetPCMBuffer(), pcmBufferSize);
			if(pcmBufferSize < static_cast<I32>(dwDSLockedBufferSize))
			{
				// If the buffer sizes are different fill in the rest with silence 
				FillMemory((BYTE*) pDSLockedBuffer + pcmBufferSize, 
							dwDSLockedBufferSize - pcmBufferSize, 
							(BYTE)(m_Resource->GetFormat()->wBitsPerSample == 8 ? 128 : 0));
			}
		}

		// Unlock the buffer, we don't need it anymore.
		m_Sample->Unlock(pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0);

		return (S_OK);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	F32 DirectSound8AudioBuffer::VGetProgress()
	{
		LPDIRECTSOUNDBUFFER pDSB = static_cast<LPDIRECTSOUNDBUFFER>(VGet());
		if(!pDSB)
		{
            GF_LOG_TRACE_ERR("DirectSound8AudioBuffer::VGetProgress()", "Failed to cast the buffer to a DirectSound buffer");
			return (false);
		}
		DWORD progress = 0;

		pDSB->GetCurrentPosition(&progress, NULL);
		F32 length = static_cast<F32>(m_Resource->GetPCMBufferSize());

		return (static_cast<F32>(progress) / length);
	}

}

#endif
