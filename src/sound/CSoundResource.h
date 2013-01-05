#pragma once
#ifndef __GCC_C_SOUND_RESOURCE_H
#define __GCC_C_SOUND_RESOURCE_H

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

#include "ResCache2.h"
#include "SoundCommon.h"
#include "GameBase.h"


#if !defined(_WINDOWS) || !defined(WIN32)
// Redefine the struct on platforms other than WIN32...
typedef struct {
    GameHalloran::WORD  wFormatTag;
    GameHalloran::WORD  nChannels;
    GameHalloran::DWORD nSamplesPerSec;
    GameHalloran::DWORD nAvgBytesPerSec;
    GameHalloran::WORD  nBlockAlign;
    GameHalloran::WORD  wBitsPerSample;
    GameHalloran::WORD  cbSize;
}WAVEFORMATEX;
#endif


namespace GameHalloran
{

	// ////////////////////////////////////////////////////////////////////
	// @class SoundResource
	// @author Mike McShaffry
	//
	// This class is meant to house the buffer that sound players will read
	// It will encapsulate the concept of "A Sound", not to be confused
	// with "A Sound Process"(SoundProcess)
	//
	// ////////////////////////////////////////////////////////////////////
	class SoundResource : public Resource
	{
	public:
		// ////////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param name The filename of the resource.
		// 
		// ////////////////////////////////////////////////////////////////////
		SoundResource(std::string name) : Resource(name) { };

		// ////////////////////////////////////////////////////////////////////
		// Return a handle to the resource.
		//
		// @param buffer A buffer holding the contents of the file in memory.
		// @param size The size of the memory buffer.
		// @param pResCache Pointer to the resource cache manager.
		//
		// @return ResHandle* A pointer to a resource handle for the file.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual ResHandle *VCreateHandle(const char *buffer, U32 size, ResCache *pResCache);
	};

	// ////////////////////////////////////////////////////////////////////
	// @class SoundResHandle
	// @author Mike McShaffry
	//
	// Stores sounds loaded by the resource cache manager.
	//
	// ////////////////////////////////////////////////////////////////////
	class SoundResHandle : public ResHandle
	{
	private:

		SoundType m_SoundType;				///< is this an Ogg, WAV, etc.?
		bool m_bInitialized;				///< has the sound been initialized
		bool m_bFromFile;					///< are we reading from a file or a buffer?
		char *m_PCMBuffer;					///< the destination PCM buffer of playable sound
		I32 m_PCMBufferSize;				///< the length of the PCM buffer
		WAVEFORMATEX m_WavFormatEx;			///< description of the PCM format
		I32 m_LengthMilli;					///< how long the sound is in milliseconds
		const std::string m_SoundFile;		///< the name of the file or resource

		// ////////////////////////////////////////////////////////////////////
		// Parse a WAV type sound file from a file.
		// 
		// @param fd The file descriptor of the sound file.
		//
		// @return bool True on success or false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		bool ParseWave(FILE *fd);

		// ////////////////////////////////////////////////////////////////////
		// Parse a OGG type sound file from a file.
		// 
		// @param fd The file descriptor of the sound file.
		//
		// @return bool True on success or false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		bool ParseOgg(FILE *fd);

		// ////////////////////////////////////////////////////////////////////
		// Parse a OGG type sound file from a memory stream.
		// 
		// @param oggStream The sound file stream in memory.
		// @param length The length of the sound file stream.
		//
		// @return bool True on success or false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		bool ParseOgg(char *oggStream, const size_t length);

		// ////////////////////////////////////////////////////////////////////
		// Parse a WAV type sound file from a memory stream.
		// 
		// @param wavStream The sound file stream in memory.
		// @param length The length of the sound file stream.
		//
		// @return bool True on success or false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		bool ParseWave(char *wavStream, const size_t length);

	public:
		// ////////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param r The resource file.
		// @param buffer The sound file loaded into memory.
		// @param size The size of the sound buffer.
		// @param pResCache Pointer to the resource cache manager.
		//
		// ////////////////////////////////////////////////////////////////////
		SoundResHandle(Resource &r, char *buffer, const U32 size, ResCache *pResCache);

		// ////////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual ~SoundResHandle();

		// ////////////////////////////////////////////////////////////////////
		// Get the sound buffer.
		//
		// ////////////////////////////////////////////////////////////////////
		char const *GetPCMBuffer() const { return (m_PCMBuffer); };

		// ////////////////////////////////////////////////////////////////////
		// Get the sound buffer size.
		//
		// ////////////////////////////////////////////////////////////////////
		I32 GetPCMBufferSize() const { return (m_PCMBufferSize); };

		// ////////////////////////////////////////////////////////////////////
		// Get the sound buffer/file type.
		//
		// ////////////////////////////////////////////////////////////////////
		SoundType GetSoundType() { return (m_SoundType); };

		// ////////////////////////////////////////////////////////////////////
		// Get the format of the sound buffer.
		// (windows specific.)
		//
		// ////////////////////////////////////////////////////////////////////
		WAVEFORMATEX const *GetFormat() { return (&m_WavFormatEx); };

		// ////////////////////////////////////////////////////////////////////
		// Get the length of the sound in milliseconds.
		//
		// ////////////////////////////////////////////////////////////////////
		I32 GetLengthMilli() const { return (m_LengthMilli); }

		// ////////////////////////////////////////////////////////////////////
		// Initializes the sound depending on the sound type.
		//
		// The initializing of the sound is seperate to the constructor for two
		// reasons.
		// 1) If loading the sound fails, it gives more flexibility for error
		//		handling for the system if initialization is located outisde
		//		the constructor (usually the only error handling available
		//		to a developer in a constructor is to throw an exception).
		// 2) Sound files are relatively large and a an init method gives more
		//		control over when they are loaded into RAM.
		//
		// @return bool True on success or false on failure.
		// 
		// ////////////////////////////////////////////////////////////////////
		virtual bool VInitialize();
	};
}

#endif
