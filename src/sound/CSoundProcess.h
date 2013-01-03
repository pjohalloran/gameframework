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

#ifndef __GCC3_C_SOUND_PROCESS_H
#define __GCC3_C_SOUND_PROCESS_H

// External Headers
#include <boost/shared_ptr.hpp>

// Project Headers
#include "CProcess.h"
#include "IAudioBuffer.h"
#include "IAudio.h"
#include "CAudio.h"
#include "CSoundResource.h"

namespace GameHalloran
{

	// ///////////////////////////////////////////////////////////////////////////
	// @class SoundProcess
	// @author Mike McShaffry
	//
	// A Sound Process, not to be confused with a Sound Resource (SoundResource)
	// manages a sound as it is being played. You can use this class to manage
	// timing between sounds & animations.
	//
	// ///////////////////////////////////////////////////////////////////////////
	class SoundProcess : public CProcess  
	{
	protected:

		// ///////////////////////////////////////////////////////////////////////////
		//
		//
		// ///////////////////////////////////////////////////////////////////////////
		void InitializeVolume();

		// ///////////////////////////////////////////////////////////////////////////
		//
		//
		// ///////////////////////////////////////////////////////////////////////////
		void Replay() { SetInitialUpdate(true); };

		boost::shared_ptr<SoundResHandle> m_SoundResource;			///< this is the raw sound data
		boost::shared_ptr<IAudioBuffer> m_AudioBuffer;				///< handle to the implementation independant audio buffer (DSound, Miles, OpenAL) 

		// these hold the initial setting until the sound is actually launched.
		I32 m_Volume;
		bool m_isLooping;

	public:

		// ///////////////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param soundResource The sound resource to manage.
		// @param typeOfSound The type of process.
		// @param volume The initial volume.
		// @param looping Should the sound loop when being played?
		//
		// ///////////////////////////////////////////////////////////////////////////
		SoundProcess(boost::shared_ptr<SoundResHandle> soundResource, I32 typeOfSound = PROC_SOUNDFX, I32 volume = 100, bool looping = false);
		
		// ///////////////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// ///////////////////////////////////////////////////////////////////////////
		virtual ~SoundProcess();

		// ///////////////////////////////////////////////////////////////////////////
		// Update the process.
		//
		// @param elapsedTime The number of seconds since the last update.
		//
		// ///////////////////////////////////////////////////////////////////////////
		virtual void VOnUpdate(const F32 elapsedTime);

		// ///////////////////////////////////////////////////////////////////////////
		// Called to initialize the process.
		//
		// ///////////////////////////////////////////////////////////////////////////
		virtual void VOnInitialize();

		// ///////////////////////////////////////////////////////////////////////////
		// Kill the process.
		//
		// ///////////////////////////////////////////////////////////////////////////
		virtual void VKill();

		// ///////////////////////////////////////////////////////////////////////////
		// Toggle on/off pausing the process.
		//
		// ///////////////////////////////////////////////////////////////////////////
		virtual void VTogglePause();

		// ///////////////////////////////////////////////////////////////////////////
		// Play the sound.
		//
		// @param volume The volume of the sound.
		// @param looping Should the sound file loop?
		//
		// ///////////////////////////////////////////////////////////////////////////
		void Play(const I32 volume, const bool looping);

		// ///////////////////////////////////////////////////////////////////////////
		// Stop the sound.
		//
		// ///////////////////////////////////////////////////////////////////////////
		void Stop();

		// ///////////////////////////////////////////////////////////////////////////
		// Set the volume.
		//
		// @param volume The volume of the sound.
		//
		// ///////////////////////////////////////////////////////////////////////////
		void SetVolume(const I32 volume);

		// ///////////////////////////////////////////////////////////////////////////
		// Get the volume.
		//
		// ///////////////////////////////////////////////////////////////////////////
		I32	GetVolume();

		// ///////////////////////////////////////////////////////////////////////////
		// Get the length in milliseconds of the sound.
		//
		// ///////////////////////////////////////////////////////////////////////////
		I32	GetLengthMilli();

		// ///////////////////////////////////////////////////////////////////////////
		// Does the sound process hold a valid sound resource?
		//
		// ///////////////////////////////////////////////////////////////////////////
		bool IsSoundValid() { return (m_SoundResource != NULL); };

		// ///////////////////////////////////////////////////////////////////////////
		// Is the sound currently playing.
		//
		// ///////////////////////////////////////////////////////////////////////////
		bool IsPlaying();

		// ///////////////////////////////////////////////////////////////////////////
		// Is the sound currently looping.
		//
		// ///////////////////////////////////////////////////////////////////////////
		bool IsLooping() { return (m_AudioBuffer && m_AudioBuffer->VIsLooping()); };

		// ///////////////////////////////////////////////////////////////////////////
		// Get the progress of the sound playback.
		//
		// @return F32 A number between 0.0 and 1.0 indicating the progress of the
		//					playback.
		//
		// ///////////////////////////////////////////////////////////////////////////
		F32 GetProgress();
	};



	// ///////////////////////////////////////////////////////////////////////////
	// @param ExplosionProcess
	// @author Mike McShaffry
	//
	// This is an example of a process that uses a simple state machine
	// to control itself.
	//
	// ///////////////////////////////////////////////////////////////////////////
	class ExplosionProcess : public CProcess
	{
	protected:

		I32 m_Stage;												///< Current state.
		boost::shared_ptr<SoundProcess> m_Sound;					///< The sound process.
	   
		// ///////////////////////////////////////////////////////////////////////////
		// Update the process.
		//
		// ///////////////////////////////////////////////////////////////////////////
  		virtual void VOnUpdate(const F32 elapsedTime);

		// ///////////////////////////////////////////////////////////////////////////
		// Initialize the process.
		//
		// ///////////////////////////////////////////////////////////////////////////
 		virtual void VOnInitialize();

	public:

		// ///////////////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// ///////////////////////////////////////////////////////////////////////////
		ExplosionProcess() : CProcess(PROC_SOUNDFX) { m_Stage=0; }
	};

	/////////////////////////////////////////////////////////////////////////////
	// @class CFadeProcess
	// @author Mike McShaffry
	//
	// Fades sound volume in or out over time and then kills itself.
	// This should be useful for groups of sound effects, too - such as when
	// an AI barks and it must be heard above the other effects like too
	// much freaking thunder.
	//
	/////////////////////////////////////////////////////////////////////////////
	class CFadeProcess : public CProcess
	{

	protected:
		boost::shared_ptr<SoundProcess> m_Sound;					///< The sound process.
		
		F32 m_TotalFadeTime;										///< 
		F32 m_ElapsedTime;										///< 
		I32 m_StartVolume;											///< 
		I32 m_EndVolume;											///< 

	public:

		// ///////////////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// ///////////////////////////////////////////////////////////////////////////
		CFadeProcess(boost::shared_ptr<SoundProcess> sound, const F32 fadeTime, const I32 endVolume);

		// ///////////////////////////////////////////////////////////////////////////
		// Update the process.
		//
		// ///////////////////////////////////////////////////////////////////////////
		virtual void VOnUpdate(const F32 elapsedTime);	
	};

}

#endif
