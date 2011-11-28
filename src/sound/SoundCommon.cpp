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
#include <cstring>

// Project Headers
#include "GameTypes.h"
#include "SoundCommon.h"

using std::string;

namespace GameHalloran
{

	// Sound file extensions array, indexed by SoundType enum.
	char *gSoundExtentions[] = { ".mp3", ".wav", ".midi", ".ogg" };

	//////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////
	char const * const FindExtFromSoundType(SoundType type)
	{
		// Global sound file extension lookup array bounds checking to make
		//  invalid array index access impossible.
		if(type >= SOUND_TYPE_COUNT)
		{
			return ("");
		}

		return (gSoundExtentions[type]);
	}

	//////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////
	SoundType FindSoundTypeFromFile(const string &filenameRef)
	{
		if(filenameRef.empty())
		{
			return (SOUND_TYPE_UNKNOWN);
		}

		I32 type = SOUND_TYPE_FIRST;
		while (type < SOUND_TYPE_COUNT)
		{
			if(strstr(filenameRef.c_str(), gSoundExtentions[type]))
			{
				return (static_cast<SoundType>(type));
			}

			++type;
		}
		
		return (SOUND_TYPE_UNKNOWN);
	}

}
