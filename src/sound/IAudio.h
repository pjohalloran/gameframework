#pragma once
#ifndef __GCC3_I_AUDIO_H
#define __GCC3_I_AUDIO_H

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

#include "SoundCommon.h"
#include "IAudioBuffer.h"
#include "CSoundResource.h"

namespace GameHalloran {

    // ////////////////////////////////////////////////////////////////////
    // @class IAudio
    // @author Mike McShaffry
    //
    // This interface class describes the public interface for
    // a game's audio system.
    //
    // Its purpose is create, manage and release audio buffers.
    //
    // ////////////////////////////////////////////////////////////////////
    class IAudio {
    public:

        // ////////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual ~IAudio() { };

        // ////////////////////////////////////////////////////////////////////
        // Check if the system is active.  This function can inform you if the
        // sound card is disabled or not installed.
        //
        // @return bool True is active, false if not.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual bool VActive() = 0;

        // ////////////////////////////////////////////////////////////////////
        // Create an audio buffer from a sound resource file.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual boost::shared_ptr<IAudioBuffer> VInitAudioBuffer(boost::shared_ptr<SoundResHandle> soundResource) = 0;

        // ////////////////////////////////////////////////////////////////////
        // Release an audio buffer.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual void VReleaseAudioBuffer(boost::shared_ptr<IAudioBuffer> audioBuffer) = 0;

        // ////////////////////////////////////////////////////////////////////
        // Stop all sounds playing.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual void VStopAllSounds() = 0;

        // ////////////////////////////////////////////////////////////////////
        // Pause all playing sounds.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual void VPauseAllSounds() = 0;

        // ////////////////////////////////////////////////////////////////////
        // Resume all paused sounds.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual void VResumeAllSounds() = 0;

        // ////////////////////////////////////////////////////////////////////
        // Initialize the audio system.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual bool VInitialize() = 0;

        // ////////////////////////////////////////////////////////////////////
        // Shutdown the audio subsystem.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual void VShutdown() = 0;
    };

}

#endif
