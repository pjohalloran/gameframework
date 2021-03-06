// ////////////////////////////////////////////////////////////////////
// @file ImageResource.cpp
// @author PJ O Halloran
// @date 27/09/2010
//
// Text resource cache handle class.
//
// ////////////////////////////////////////////////////////////////////

#include "TextResource.h"
#include "GameMain.h"
#include "GameBase.h"

namespace GameHalloran {
    // ////////////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////////////
    TextResource::TextResource(const std::string &name) : Resource(name)
    {
    }

    // ////////////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////////////
    ResHandle *TextResource::VCreateHandle(const char *buffer, U32 size, ResCache *pResCache)
    {
        // Cast away the const-ness of the buffer for the TextResHandle.
        char *bufferNoConst = const_cast<char *>(buffer);
        return (GCC_NEW TextResHandle(*this, bufferNoConst, size, pResCache));
    }

    // ////////////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////////////

    // ////////////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////////////
    TextResHandle::TextResHandle(Resource &r, char *buffer, const U32 size, ResCache *pResCache)\
:
    ResHandle(r, buffer, size, pResCache)
    , m_textBuffer(NULL)
    , m_textSize(0)
    , m_filename(r.GetName())
    , m_initialized(false)
    {
    }

    // ////////////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////////////
    TextResHandle::~TextResHandle()
    {
        // Note - since TextResources have a lifetime controlled by shared_ptr,
        // it's safe for us to nuke the memory without checking first.
        try {
            GF_LOG_DEB(std::string("Freeing the text resource ") + m_filename);
            DeleteArray(m_textBuffer);
        } catch(...) {
        }
    }

    // ////////////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////////////
    const char *TextResHandle::GetTextBuffer() const
    {
        return (m_textBuffer);
    }

    // ////////////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////////////
    U32 TextResHandle::GetTextSize() const
    {
        return (m_textSize);
    }

    // ////////////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////////////
    std::string TextResHandle::GetFilename() const
    {
        return (m_filename);
    }

    // ////////////////////////////////////////////////////////////////////
    //
    // ////////////////////////////////////////////////////////////////////
    bool TextResHandle::VInitialize()
    {
        if(!m_initialized) {
            m_textSize = ResHandle::Size() + 1;
            m_textBuffer = GCC_NEW char[m_textSize];
            memcpy(m_textBuffer, ResHandle::Buffer(), ResHandle::Size());
            // Ensure text buffer ends with a NULL character.
            m_textBuffer[ResHandle::Size()] = '\0';
            m_initialized = true;
        }
        return (m_initialized);
    }

}
