#pragma once
#ifndef __GCC3_C_AUDIO_H
#define __GCC3_C_AUDIO_H

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

#include <boost/shared_ptr.hpp>

#include "IAudioBuffer.h"
#include "CSoundResource.h"
#include "IAudio.h"

namespace GameHalloran
{

	// ////////////////////////////////////////////////////////////////////
	// @class AudioBuffer
	// @author Mike McShaffry
	//
	// Implements IAudiobuffer interface using a smart pointer to
	// SoundResource.
	//
	// This class is a platform/library independant buffer class and needs
	// to be derived from to provide a full working sound buffer.
	//
	// ////////////////////////////////////////////////////////////////////
	class AudioBuffer : public IAudioBuffer
	{
	private:

		bool m_isPaused;							///< Is the sound paused
		bool m_isLooping;							///< Is the sound looping
		I32 m_Volume;								///< the volume

	protected:

		// The sound is guaranteed not to be deleted until it has finished playing 
		//  as the sound resource is held by a shared_ptr
		boost::shared_ptr<SoundResHandle> m_Resource;		///< The sound resource handle.

		// ////////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// Disable public construction.
		//
		// ////////////////////////////////////////////////////////////////////
		AudioBuffer(boost::shared_ptr<SoundResHandle >resource) : m_isPaused(false),\
			m_isLooping(false), m_Volume(0), m_Resource(resource)
		{
		};

		// ////////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual ~AudioBuffer() { };

		// ////////////////////////////////////////////////////////////////////
		// Pause the buffer.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual void VSetPaused(const bool paused) { m_isPaused = paused; };

		// ////////////////////////////////////////////////////////////////////
		// Set the flag so that the sound will loop once played.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual void VSetLooping(const bool looping) { m_isLooping = looping; };

	public:

		// ////////////////////////////////////////////////////////////////////
		// Get the sound resource handle.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual boost::shared_ptr<SoundResHandle> VGetResource() { return m_Resource; };

		// ////////////////////////////////////////////////////////////////////
		// Check if the sound is looping.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual bool VIsLooping() const { return (m_isLooping); };

		// ////////////////////////////////////////////////////////////////////
		// Set the sound volume.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual void VSetVolume(const I32 volume) { m_Volume = volume; };

		// ////////////////////////////////////////////////////////////////////
		// Get the current volume.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual I32 VGetVolume() const { return (m_Volume); };

		// ////////////////////////////////////////////////////////////////////
		// Check if the sound is paused.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual bool VIsPaused() const { return (m_isPaused); };
	};

	// ////////////////////////////////////////////////////////////////////
	// @class Audio
	// @author Mike McShaffry
	//
	// Implements IAudio interface.  This class is meant to be derived from
	// as it does contain the full implementation for an audio manager, only
	// the library/platform independant sections.
	// 
	// ////////////////////////////////////////////////////////////////////
	class Audio : public IAudio
	{
	private:

		bool m_AllPaused;						///< Has the sound system been paused?
		bool m_Initialized;						///< Has the sound system been initialized?

	protected:
		
		AudioBufferList m_AllSamples;			///< List of all currently allocated audio buffers.

		// ////////////////////////////////////////////////////////////////////
		// Set the initialized flag.
		//
		// ////////////////////////////////////////////////////////////////////
		void SetInitialized(const bool initialized) { m_Initialized = initialized; };

		// ////////////////////////////////////////////////////////////////////
		// Are all sound buffers currently paused.
		//
		// ////////////////////////////////////////////////////////////////////
		bool IsAllPaused() const { return (m_AllPaused); };

		// ////////////////////////////////////////////////////////////////////
		// Set all sound buffers to be paused.
		//
		// ////////////////////////////////////////////////////////////////////
		void SetAllPaused(const bool allPaused) { m_AllPaused = allPaused; };

	public:
		// ////////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// ////////////////////////////////////////////////////////////////////
		Audio() : m_AllSamples(), m_AllPaused(false), m_Initialized(false) { };

		// ////////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual ~Audio() { try { VShutdown(); } catch(...){ } };

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

		// ////////////////////////////////////////////////////////////////////
		// Shutdown the audio system.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual void VShutdown();

		// ////////////////////////////////////////////////////////////////////
		// Is the audio system intialized flag set.
		//
		// ////////////////////////////////////////////////////////////////////
		bool IsInitialized() const { return (m_Initialized); };

		// ////////////////////////////////////////////////////////////////////
		// Check if theres a sound card available.
		//
		// ////////////////////////////////////////////////////////////////////
		static bool HasSoundCard(void);
	};

	// Global audio system pointer.
	extern Audio *g_audioPtr;

}

#endif
