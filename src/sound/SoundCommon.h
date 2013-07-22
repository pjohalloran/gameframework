#pragma once
#ifndef __GCC3_SOUND_COMMON_H
#define __GCC3_SOUND_COMMON_H

#include <string>

namespace GameHalloran {

    //////////////////////////////////////////////////////////////////////
    // @enum SoundType
    //
    // This is an enum that represents the different kinds of sound data
    // streams the sound system can handle.
    //
    //////////////////////////////////////////////////////////////////////
    enum SoundType {
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
    //                              empty string if the file extension
    //                              was not found in the global sound ext
    //                              lookup array (@see SoundCommon.h).
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
    //                      if the file extension is not stored in the global
    //                      file extension array (@see SoundCommon.h).
    //
    //////////////////////////////////////////////////////////////////////
    SoundType FindSoundTypeFromFile(const std::string &filenameRef);

}

#endif
