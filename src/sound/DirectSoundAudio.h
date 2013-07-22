#pragma once
#ifndef __GCC3_DIRECT_SOUND_AUDIO_H
#define __GCC3_DIRECT_SOUND_AUDIO_H

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

#include "GameBase.h"

#include <boost/shared_ptr.hpp>
#include <dsound.h>
#include <mmsystem.h>

#include "CAudio.h"

// Guarantees that the directsound lib is included during build.
#pragma comment( lib, "dsound" )

namespace GameHalloran {

    //#define GCC_DSBVOLUME_MIN (-2000)
    // GCC_DSBVOLUME_MIN is defined as -5000 - why? Because DSBVOLUME_MIN, defined in dsound.h
    // is set to as -10000, which is just way too silent for a lower bound and many programmers find
    // -5000 to be a better minimum bound for the volume range to avoid an annoying silence for the
    // lower 50% of a volume slider that uses a logarithmic scale.
    // This was contributed by BystanderKain!
#define GCC_DSBVOLUME_MIN (DSBVOLUME_MIN)

    // ////////////////////////////////////////////////////////////////////
    // @class DirectSound8AudioBuffer
    // @author Mike McShaffry
    //
    // Implements the rest of the IAudioBuffer interface left out by AudioBuffer.
    //
    // ////////////////////////////////////////////////////////////////////
    class DirectSound8AudioBuffer : public AudioBuffer {
    private:

        // ////////////////////////////////////////////////////////////////////
        // Fill the sound buffer with sound data.
        //
        // ////////////////////////////////////////////////////////////////////
        HRESULT FillBufferWithSound();

        // ////////////////////////////////////////////////////////////////////
        // Restore the sound buffer.
        //
        // @param pbWasRestored True if the sound buffer was restored and false
        //                          otherwise.
        //
        // @return HRESULT
        //
        // ////////////////////////////////////////////////////////////////////
        HRESULT RestoreBuffer(BOOL *pbWasRestored);

    protected:

        LPDIRECTSOUNDBUFFER m_Sample;                                       ///< DirectSound sound buffer.

    public:

        // ////////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param sample
        // @param resource The sound resource.
        //
        // ////////////////////////////////////////////////////////////////////
        DirectSound8AudioBuffer(LPDIRECTSOUNDBUFFER sample, boost::shared_ptr<SoundResHandle> resource);

        // ////////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual ~DirectSound8AudioBuffer() { };

        // ////////////////////////////////////////////////////////////////////
        // Get an implementation specific pointer to the audio buffer.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual void *VGet();

        // ////////////////////////////////////////////////////////////////////
        // Called on an application restore to restore the sound buffer.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual bool VOnRestore();

        // ////////////////////////////////////////////////////////////////////
        // Play the sound buffer.
        //
        // @param volume The volume of the sound.
        // @param looping Should the sound loop when played?
        //
        // @return bool True on success and false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual bool VPlay(const I32 volume, const bool looping);

        // ////////////////////////////////////////////////////////////////////
        // Pause the sound buffer.
        //
        // @return bool True on success and false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual bool VPause();

        // ////////////////////////////////////////////////////////////////////
        // Stop the sound buffer.
        //
        // @return bool True on success and false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual bool VStop();

        // ////////////////////////////////////////////////////////////////////
        // Resume playing the sound buffer.
        //
        // @return bool True on success and false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual bool VResume();

        // ////////////////////////////////////////////////////////////////////
        // Toggle on/off pausing the sound buffer.
        //
        // @return bool True on success and false on failure.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual bool VTogglePause();

        // ////////////////////////////////////////////////////////////////////
        // Check if the sound buffer is playing.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual bool VIsPlaying();

        // ////////////////////////////////////////////////////////////////////
        // Set the volume of the sound.
        //
        // @param volume The new volume of the sound.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual void VSetVolume(const I32 volume);

        // ////////////////////////////////////////////////////////////////////
        // Get the progress of the playback.
        //
        // @return F32 A number between 0.0 and 1.0 indicating how much of
        //                  the sound has been played.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual F32 VGetProgress();
    };



    //////////////////////////////////////////////////////////////////////
    // @class DirectSound8Audio
    // @author Mike McShaffry
    //
    // Implements the rest of the IAudio interface left out by Audio.
    //
    //////////////////////////////////////////////////////////////////////
    class DirectSound8Audio : public Audio {
    private:



    protected:

        IDirectSound8* m_pDS;                                       ///< Pointer to the DirectSound8 interface.
        HWND           m_hWnd;

        // ////////////////////////////////////////////////////////////////////
        // Set the buffer format.
        //
        // @param dwPrimaryChannels The number of audio channels.
        // @param dwPrimaryFreq The frequency.
        // @param dwPrimaryBitRate The sampling bit rate.
        //
        // ////////////////////////////////////////////////////////////////////
        HRESULT SetPrimaryBufferFormat(DWORD dwPrimaryChannels,
                                       DWORD dwPrimaryFreq,
                                       DWORD dwPrimaryBitRate);

    public:

        // ////////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // ////////////////////////////////////////////////////////////////////
        DirectSound8Audio(HWND hWnd) {
            m_pDS = NULL;
            m_hWnd = hWnd;
        };

        // ////////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual ~DirectSound8Audio() { };

        // ////////////////////////////////////////////////////////////////////
        // Check if the sound system has been initialized.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual bool VActive() {
            return (m_pDS != NULL);
        }

        // ////////////////////////////////////////////////////////////////////
        // Initialize an audio buffer from a sound resource.
        //
        // @param soundResource The sound resource handle.
        //
        // @return boost::shared_ptr<IAudioBuffer> A pointer to an audio buffer.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual boost::shared_ptr<IAudioBuffer> VInitAudioBuffer(boost::shared_ptr<SoundResHandle> soundResource);

        // ////////////////////////////////////////////////////////////////////
        // Release resources for an audio buffer.
        //
        // @param audioBuffer Pointer to an audio buffer.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual void VReleaseAudioBuffer(boost::shared_ptr<IAudioBuffer> audioBuffer);

        // ////////////////////////////////////////////////////////////////////
        // Shutdown the audio subsystem.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual void VShutdown();

        // ////////////////////////////////////////////////////////////////////
        // Initialize the audio subsystem.
        //
        // ////////////////////////////////////////////////////////////////////
        virtual bool VInitialize();
    };

}

#endif
