#pragma once
#ifndef __GCC3_I_AUDIO_BUFFER_H
#define __GCC3_I_AUDIO_BUFFER_H

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

#include <list>

#include <boost/shared_ptr.hpp>

#include "SoundCommon.h"
#include "CSoundResource.h"

namespace GameHalloran
{
	//////////////////////////////////////////////////////////////////////
	// @class IAudioBuffer
	// @author Mike McShaffry
	//
	// The interface class that defines the public API for audio buffers.
	// An audio buffer maps to one instance of a sound being played, 
	// which ISNT the sound data. Two different sounds can be played from
	// the same source data - such as two explosions in two different places.
	// 
	//////////////////////////////////////////////////////////////////////
	class IAudioBuffer
	{
	public:

		//////////////////////////////////////////////////////////////////////
		// Destructor.
		// 
		//////////////////////////////////////////////////////////////////////
		virtual ~IAudioBuffer() { };

		//////////////////////////////////////////////////////////////////////
		// Get implementation specific handle to the sound.
		// 
		//////////////////////////////////////////////////////////////////////
		virtual void *VGet() = 0;

		//////////////////////////////////////////////////////////////////////
		// Get the sound resource handle.
		// 
		//////////////////////////////////////////////////////////////////////
		virtual boost::shared_ptr<SoundResHandle> VGetResource() = 0;

		//////////////////////////////////////////////////////////////////////
		// Called on an application restore to restore the sound buffer.
		// 
		// @return bool True on success, false on failure.
		//
		//////////////////////////////////////////////////////////////////////
		virtual bool VOnRestore() = 0;

		//////////////////////////////////////////////////////////////////////
		// Play a sound.
		// 
		// @param volume The volume of the sound.
		// @param looping Should the sound play again when it has completed?
		//
		// @return bool True on success and false on failure.
		//
		//////////////////////////////////////////////////////////////////////
		virtual bool VPlay(const I32 volume, const bool looping) = 0;

		//////////////////////////////////////////////////////////////////////
		// Pause the sound, if it is playing.
		//
		// @return bool True on success and false on failure.
		//
		//////////////////////////////////////////////////////////////////////
		virtual bool VPause() = 0;

		//////////////////////////////////////////////////////////////////////
		// Stop the sound, if it is playing.
		//
		// @return bool True on success and false on failure.
		//
		//////////////////////////////////////////////////////////////////////
		virtual bool VStop() = 0;

		//////////////////////////////////////////////////////////////////////
		// Resume the sound, if it is paused.
		//
		// @return bool True on success and false on failure.
		//
		//////////////////////////////////////////////////////////////////////
		virtual bool VResume() = 0;

		//////////////////////////////////////////////////////////////////////
		// Toggle pausing the sound, if it is playing.
		//
		// @return bool True on success and false on failure.
		//
		//////////////////////////////////////////////////////////////////////
		virtual bool VTogglePause() = 0;

		//////////////////////////////////////////////////////////////////////
		// Check if the sound is playing.
		// 
		//////////////////////////////////////////////////////////////////////
		virtual bool VIsPlaying() = 0;

		//////////////////////////////////////////////////////////////////////
		// Check if the sound is set to loop indefinetly.
		// 
		//////////////////////////////////////////////////////////////////////
		virtual bool VIsLooping() const = 0;

		//////////////////////////////////////////////////////////////////////
		// Set the volume.
		// 
		// @param volume The new volume of the sound.
		//
		//////////////////////////////////////////////////////////////////////
		virtual void VSetVolume(const I32 volume) = 0;

		//////////////////////////////////////////////////////////////////////
		// Get the current volume level.
		// 
		//////////////////////////////////////////////////////////////////////
		virtual I32 VGetVolume() const = 0;

		//////////////////////////////////////////////////////////////////////
		// Get the current progress of the playback.
		// 
		// @return F32 A number between 0.0 and 1.0 indicating the progress
		//					of the playback.
		//
		//////////////////////////////////////////////////////////////////////
		virtual F32 VGetProgress() = 0;
	};

	typedef std::list<boost::shared_ptr<IAudioBuffer> > AudioBufferList;

}

#endif
