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

#ifndef __GCC3_SOUND_COMMON_H
#define __GCC3_SOUND_COMMON_H

// External Headers
#include <string>

// Project Headers


namespace GameHalloran
{

	//////////////////////////////////////////////////////////////////////
	// @enum SoundType
	//
	// This is an enum that represents the different kinds of sound data
	// streams the sound system can handle.
	//
	//////////////////////////////////////////////////////////////////////
	enum SoundType
	{
		SOUND_TYPE_FIRST,
		SOUND_TYPE_MP3 = SOUND_TYPE_FIRST,
		SOUND_TYPE_WAVE,
		SOUND_TYPE_MIDI,
		SOUND_TYPE_OGG,

		// This needs to be the last sound type
		SOUND_TYPE_COUNT,
		SOUND_TYPE_UNKNOWN,
	};

	// Global array of sound extensions.
	extern char *gSoundExtentions[];

	//////////////////////////////////////////////////////////////////////
	// Given a sound file type, return the sound files typical file
	// extension.
	//
	// @return const char* const The sound file types extension or an
	//								empty string if the file extension
	//								was not found in the global sound ext
	//								lookup array (@see SoundCommon.h).
	//
	//////////////////////////////////////////////////////////////////////
	char const * const FindExtFromSoundType(SoundType type);

	//////////////////////////////////////////////////////////////////////
	// Given a filename determine the sound type from the enum from the 
	// file extension.
	//
	// @param filenameRef The name of the file.
	//
	// @return SoundType The type of the sound file or SOUND_TYPE_UNKNOWN
	//						if the file extension is not stored in the global
	//						file extension array (@see SoundCommon.h).
	//
	//////////////////////////////////////////////////////////////////////
	SoundType FindSoundTypeFromFile(const std::string &filenameRef);

}

#endif
