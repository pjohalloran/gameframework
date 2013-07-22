// /////////////////////////////////////////////////////////////////
// @file TextureManager.cpp
// @author PJ O Halloran
// @date 07/10/2010
//
// Implementation for the global TextureManager class for creating and
// loading OpenGL textures.
//
// /////////////////////////////////////////////////////////////////

#include <cstring>

#include "TextureManager.h"
#include "ResCache2.h"
#include "ImageResource.h"
#include "GameMain.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void TextureManager::ResizeTextureVector()
    {
        // First ensure that we do need to generate more texture objects.
        if(m_usedTextureCount < m_glIdVec.size()) {
            return;
        }

        GLuint newTexObjects[DEFAULT_EXTEND_SIZE];              // Array of new texture objects.
        const U64 oldSize(m_glIdVec.size());                    // Old size of the vector of texture IDs.

        GF_CLEAR_GL_ERROR();

        glGenTextures(DEFAULT_EXTEND_SIZE, newTexObjects);

        if(!GF_CHECK_GL_ERROR_TRC("TextureManager::ResizeTextureVector(): ")) {
            glDeleteTextures(DEFAULT_EXTEND_SIZE, newTexObjects);
            return;
        }

        m_glIdVec.resize(oldSize + DEFAULT_EXTEND_SIZE);
        if(oldSize > 0) {
            memcpy(&m_glIdVec[oldSize], newTexObjects, DEFAULT_EXTEND_SIZE * sizeof(GLuint));
        } else {
            memcpy(&m_glIdVec[0], newTexObjects, DEFAULT_EXTEND_SIZE * sizeof(GLuint));
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    TextureManager::TextureManager(const U32 expectedNumTextures,
                                   const U32 maxSize) throw(GameException &)
        : m_elementsMap()
        , m_glIdVec()
        , m_usedTextureCount(0)
        , m_currTexLayer(0)
        , m_maxTexLayers(0)
        , m_currTexFilterMode(eBasic)
        , m_currMinFilter(GL_NEAREST)
        , m_currMagFilter(GL_NEAREST)
        , m_anisotropicLinearLevel(0.0f)
        , m_maxAnisotropicValue(0.0f)
        , m_extendSize(DEFAULT_EXTEND_SIZE)
        , m_maxSize(maxSize)
        , m_currSize(0)
        , m_curBindTex(99999)
    {
        m_glIdVec.resize(expectedNumTextures);

        GF_CLEAR_GL_ERROR();

        glGenTextures(static_cast<GLsizei>(m_glIdVec.size()), &m_glIdVec[0]);
        GF_CHECK_GL_ERROR_TRC("TextureManager::TextureManager(): ");

        glGetIntegerv(GL_ACTIVE_TEXTURE, &m_currTexLayer);
        GF_CHECK_GL_ERROR_TRC("TextureManager::TextureManager(): ");

        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_maxTexLayers);
        GF_CHECK_GL_ERROR_TRC("TextureManager::TextureManager(): ");

#ifdef GLEW_EXT_texture_filter_anisotropic
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_maxAnisotropicValue);
        GF_CHECK_GL_ERROR_TRC("TextureManager::TextureManager(): ");
#endif
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    TextureManager::~TextureManager()
    {
        try {
            FreeAll();
        } catch(...) {
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void TextureManager::SetTextureFilterMode(const TextureFilterMode mode)
    {
        if(mode < eNumberModes && mode != m_currTexFilterMode) {
            TextureFilterMode oldMode(m_currTexFilterMode);
            m_currTexFilterMode = mode;

            switch(m_currTexFilterMode) {
                case eBasic:
                    m_currMinFilter = GL_NEAREST;
                    m_currMagFilter = GL_NEAREST;
                    break;
                case eBasicMipMap:
                    m_currMinFilter = GL_NEAREST_MIPMAP_NEAREST;
                    m_currMagFilter = GL_LINEAR;
                    break;
                case eBilinear:
                    m_currMinFilter = GL_LINEAR_MIPMAP_NEAREST;
                    m_currMagFilter = GL_LINEAR;
                    break;
                case eTrilinear:
#ifdef GLEW_EXT_texture_filter_anisotropic
                case eAnisotropic:
#endif
                    m_currMinFilter = GL_LINEAR_MIPMAP_LINEAR;
                    m_currMagFilter = GL_LINEAR;
                    break;
            }

            UpdateTextureFilters(boost::optional<TextureFilterMode>(oldMode));
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void TextureManager::SetAnisotropicLinearLevel(const F32 anisotropicLinearLevel)
    {
#ifdef GLEW_EXT_texture_filter_anisotropic
        m_anisotropicLinearLevel = anisotropicLinearLevel;
        Clamp<F32>(m_anisotropicLinearLevel, 0.0f, 1.0f);

        if(m_currTexFilterMode == eAnisotropic) {
            UpdateTextureFilters();
        }
#endif
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void TextureManager::UpdateTextureFilters(boost::optional<TextureFilterMode> oldMode)
    {
#ifdef GLEW_EXT_texture_filter_anisotropic
        GLfloat aniLevelVal(0.0f);

        if(m_currTexFilterMode == eAnisotropic) {
            aniLevelVal = InterpolateFloat(m_anisotropicLinearLevel, 0.0f, m_maxAnisotropicValue);
        }
#endif

        GF_CLEAR_GL_ERROR();

        for(ElementMap::iterator i = m_elementsMap.begin(), end = m_elementsMap.end(); i != end; ++i) {
            GLenum currTarget(((*i).second).m_glTarget);
            if(currTarget != GL_TEXTURE_RECTANGLE) {
                Bind(((*i).second).m_id, currTarget);
                glTexParameteri(currTarget, GL_TEXTURE_MIN_FILTER, m_currMinFilter);
                GF_CHECK_GL_ERROR_TRC("TextureManager::UpdateTextureFilters(): ");

                glTexParameteri(currTarget, GL_TEXTURE_MAG_FILTER, m_currMagFilter);
                GF_CHECK_GL_ERROR_TRC("TextureManager::UpdateTextureFilters(): ");

#ifdef GLEW_EXT_texture_filter_anisotropic
                if(m_currTexFilterMode == eAnisotropic) {
                    glTexParameterf(currTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniLevelVal);
                    GF_CHECK_GL_ERROR_TRC("TextureManager::UpdateTextureFilters(): ");
                } else if(oldMode.is_initialized() && *oldMode == eAnisotropic) {
                    glTexParameterf(currTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 0.0f);
                    GF_CHECK_GL_ERROR_TRC("TextureManager::UpdateTextureFilters(): ");
                }
#endif
            }
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    boost::optional<GLuint> TextureManager::Find(const TexHandle tHandle, bool timestampTex)
    {
        boost::optional<GLuint> texturePrivateId;

        ElementMap::iterator i = m_elementsMap.find(tHandle);
        if(i == m_elementsMap.end()) {
            if(timestampTex) {
                (*i).second.m_timestamp = static_cast<F32>(g_appPtr->GetCurrTime());
            }
            return (texturePrivateId);
        }

        texturePrivateId = ((*i).second).m_glTexId;
        return (texturePrivateId);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    boost::optional<TexHandle> TextureManager::Find(const std::string &imgnameRef) const
    {
        boost::optional<TexHandle> texturePublicId;

        if(!imgnameRef.empty()) {
            for(ElementMap::const_iterator i = m_elementsMap.begin(), end = m_elementsMap.end(); ((!texturePublicId.is_initialized()) && (i != end)); ++i) {
                if(((*i).second).m_filename.compare(imgnameRef.c_str()) == 0) {
                    texturePublicId = ((*i).second).m_id;
                }
            }
        }

        return (texturePublicId);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    boost::optional<U32> TextureManager::UnloadLRUTexture()
    {
        boost::optional<U32> bytesFreed;                // Number of bytes freed.

        if(!m_elementsMap.empty()) {
            //F32 minTs(FLT_MAX);                                       // Record of minimum ts found to date.
            F32 minTs(999999999.9999);                                  // Record of minimum ts found to date.
            ElementMap::iterator lruElementIter(m_elementsMap.end());   // Iterator into element map.

            for(ElementMap::iterator i = m_elementsMap.begin(), end = m_elementsMap.end(); (i != end); ++i) {
                if(((*i).second).m_timestamp < minTs) {
                    minTs = ((*i).second).m_timestamp;
                    lruElementIter = i;
                }
            }

            bytesFreed = UnloadTexture(lruElementIter);
        }

        return (bytesFreed);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    boost::optional<U32> TextureManager::UnloadTexture(ElementMap::iterator &texIter)
    {
        boost::optional<U32> bytesFreed;                // Number of bytes freed.

        if(texIter != m_elementsMap.end()) {
            GF_CLEAR_GL_ERROR();

            glDeleteTextures(1, &((*texIter).second).m_glTexId);
            if(!GF_CHECK_GL_ERROR_TRC("TextureManager::UnloadTexture(): ")) {
                GF_LOG_TRACE_ERR("TextureManager::UnloadTexture()", std::string("An OpenGL error occurred freeing the texture data for the image ") + ((*texIter).second).m_filename);
                return (bytesFreed);
            }

            bytesFreed = ((*texIter).second).m_width * ((*texIter).second).m_height;
            m_currSize -= *bytesFreed;
            m_elementsMap.erase(texIter);

            bool found(false);
            for(std::vector<GLuint>::iterator i = m_glIdVec.begin(); ((!found) && (i != m_glIdVec.end()));) {
                if((*i) == ((*texIter).second).m_glTexId) {
                    i = m_glIdVec.erase(i);
                    found = true;
                } else {
                    ++i;
                }
            }
            if(found) {
                --m_usedTextureCount;
            }
        }

        return (bytesFreed);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TextureManager::UnloadTexture(const TexHandle textureHandle)
    {
        boost::optional<U32> bytesFreed;                // Number of bytes freed.

        if(!m_elementsMap.empty()) {
            ElementMap::iterator elementIter(m_elementsMap.end());  // Iterator into element map.

            for(ElementMap::iterator i = m_elementsMap.begin(), end = m_elementsMap.end(); (i != end); ++i) {
                if(((*i).second).m_id < textureHandle) {
                    elementIter = i;
                }
            }

            bytesFreed = UnloadTexture(elementIter);
        }

        return (bytesFreed.is_initialized());
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    boost::optional<TexHandle> TextureManager::Load1D(const std::string &imgnameRef, GLubyte **textureData, const U32 w, const GLenum wrapMode)
    {
        boost::optional<TexHandle> tHandle;             // The texture handle.

        if((textureData == NULL) || (w == 0) || (imgnameRef.empty())) {
            GF_LOG_TRACE_ERR("TextureManager::Load1D()", "Invalid parameters");
            return (tHandle);
        }

        tHandle = Find(imgnameRef);
        if(tHandle.is_initialized()) {
            return (tHandle);
        }

        if(m_maxSize != 0) {
            if(w > m_maxSize) {
                GF_LOG_TRACE_ERR("TextureManager::Load1D()", "Cannot load image! It is bigger than the entire size of the TextureManagers memory budget!");
                return (tHandle);
            }

            while(w + m_currSize > m_maxSize) {
                UnloadLRUTexture();
            }
        }

        // Ensure we have generated enough texture objects.
        if(m_usedTextureCount >= m_glIdVec.size()) {
            ResizeTextureVector();
        }

        GF_CLEAR_GL_ERROR();

        // Bind to the next available texture object.
        glBindTexture(GL_TEXTURE_1D, m_glIdVec[m_usedTextureCount]);
        GF_CHECK_GL_ERROR_TRC("TextureManager::Load1D(): ");

        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, m_currMinFilter);
        GF_CHECK_GL_ERROR_TRC("TextureManager::Load1D(): ");
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, m_currMagFilter);
        GF_CHECK_GL_ERROR_TRC("TextureManager::Load1D(): ");
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, wrapMode);
        GF_CHECK_GL_ERROR_TRC("TextureManager::Load1D(): ");

        // Send the texture data to the GPU (using set format values).
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, w, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        if(!GF_CHECK_GL_ERROR_TRC("TextureManager::Load1D(): ")) {
            return (tHandle);
        }

        if(m_currTexFilterMode >= eBasicMipMap) {
            glGenerateMipmap(GL_TEXTURE_1D);
            if(!GF_CHECK_GL_ERROR_TRC("TextureManager::Load1D(): ")) {
                return (tHandle);
            }
        }

        // increment the current size of the textures stored in the GPU.
        m_currSize += w;
        tHandle = m_usedTextureCount;

        // Fill out the data struct for the texture we have loaded onto the GPU.
        TextureElement newTexElement;
        newTexElement.m_id = *tHandle;
        newTexElement.m_filename.assign(imgnameRef);
        newTexElement.m_timestamp = static_cast<F32>(g_appPtr->GetCurrTime());
        newTexElement.m_glTexId = m_glIdVec[*tHandle];
        newTexElement.m_minFilter = m_currMinFilter;
        newTexElement.m_magFilter = m_currMagFilter;
        newTexElement.m_wrapMode = wrapMode;
        newTexElement.m_glTarget = GL_TEXTURE_1D;
        newTexElement.m_width = w;
        newTexElement.m_height = 0;
        newTexElement.m_imgFormat = GL_RGB;
        newTexElement.m_imgType = GL_UNSIGNED_BYTE;
        newTexElement.m_unpackAlignment = -1;

        m_elementsMap[*tHandle] = newTexElement;
        ++m_usedTextureCount;

        return (tHandle);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TextureManager::LoadCommon2D(const GLenum target, const GLint level, const GLint internalFormat, const GLsizei width, const GLsizei height, \
                                      const GLint border, const GLenum format, const GLenum type, void *data, const bool tightlyPack)
    {
        if((target != GL_TEXTURE_2D)\
                && (target != GL_TEXTURE_RECTANGLE)\
                && (target != GL_TEXTURE_CUBE_MAP_POSITIVE_X)\
                && (target != GL_TEXTURE_CUBE_MAP_NEGATIVE_X)\
                && (target != GL_TEXTURE_CUBE_MAP_POSITIVE_Y)\
                && (target != GL_TEXTURE_CUBE_MAP_NEGATIVE_Y)\
                && (target != GL_TEXTURE_CUBE_MAP_POSITIVE_Z)\
                && (target != GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)) {
            GF_LOG_TRACE_ERR("TextureManager::LoadCommon2D()", "Target is not a 2D, RECTANGLE or CubeMap texture");
            return (false);
        }

        GF_CLEAR_GL_ERROR();

        GLint oldUnpackAlignment(0);
        if(tightlyPack) {
            glGetIntegerv(GL_UNPACK_ALIGNMENT, &oldUnpackAlignment);
            if(!GF_CHECK_GL_ERROR_TRC("TextureManager::LoadCommon2D(): ")) {
                return (false);
            }
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            if(!GF_CHECK_GL_ERROR_TRC("TextureManager::LoadCommon2D(): ")) {
                return (false);
            }
        }

        glTexImage2D(target, level, internalFormat, width, height, border, format, type, data);
        bool glError = !GF_CHECK_GL_ERROR_TRC("TextureManager::LoadCommon2D(): ");

        if(tightlyPack) {
            glPixelStorei(GL_UNPACK_ALIGNMENT, oldUnpackAlignment);
            GF_CHECK_GL_ERROR_TRC("TextureManager::LoadCommon2D(): ");
        }

        if(!glError) {
            m_currSize += width * height;
        }

        return (!glError);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    boost::optional<TexHandle> TextureManager::Load2D(const std::string &imgnameRef, const GLenum wrapMode)
    {
        boost::optional<TexHandle> tHandle;             // The texture handle.

        if(imgnameRef.empty()) {
            GF_LOG_TRACE_ERR("TextureManager::Load2D()", "Invalid parameters");
            return (tHandle);
        }

        tHandle = Find(imgnameRef);
        if(tHandle.is_initialized()) {
            return (tHandle);
        }

        ImageResource imgRes(imgnameRef);
        boost::shared_ptr<ImageResHandle> imgResHandle = boost::static_pointer_cast<ImageResHandle>(g_appPtr->GetResourceCache()->GetHandle(&imgRes));
        if(!imgResHandle || !imgResHandle->VInitialize()) {
            GF_LOG_TRACE_ERR("TextureManager::Load2D()", std::string("Failed to retrieve and/or initialize the resource ") + imgnameRef);
            return (tHandle);
        }

        // Check if we need to swap out textures.
        if(m_maxSize != 0) {
            U32 imgsize(imgResHandle->GetImageWidth() * imgResHandle->GetImageHeight());
            if(imgsize > m_maxSize) {
                GF_LOG_TRACE_ERR("TextureManager::Load2D()", "Cannot load image! It is bigger than the entire size of the TextureManagers memory budget!");
                return (tHandle);
            }
            while(imgsize + m_currSize > m_maxSize) {
                UnloadLRUTexture();
            }
        }

        if(m_usedTextureCount >= m_glIdVec.size()) {
            ResizeTextureVector();
        }

        GF_CLEAR_GL_ERROR();

        glBindTexture(GL_TEXTURE_2D, m_glIdVec[m_usedTextureCount]);
        GF_CHECK_GL_ERROR_TRC("TextureManager::Load2D(): ");

        bool pack = (FindImageTypeFromFile(imgnameRef) == IMAGE_TYPE_TGA);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_currMinFilter);
        GF_CHECK_GL_ERROR_TRC("TextureManager::Load2D(): ");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_currMagFilter);
        GF_CHECK_GL_ERROR_TRC("TextureManager::Load2D(): ");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        GF_CHECK_GL_ERROR_TRC("TextureManager::Load2D(): ");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
        GF_CHECK_GL_ERROR_TRC("TextureManager::Load2D(): ");

        if(!LoadCommon2D(GL_TEXTURE_2D, 0, imgResHandle->GetImageComponents(), imgResHandle->GetImageWidth(), imgResHandle->GetImageHeight(), \
                         0, imgResHandle->GetImageFormat(), GL_UNSIGNED_BYTE, const_cast<GLbyte *>(imgResHandle->GetImageBuffer()), pack)) {
            return (tHandle);
        }

        if(m_currTexFilterMode >= eBasicMipMap) {
            glGenerateMipmap(GL_TEXTURE_2D);
            if(!GF_CHECK_GL_ERROR_TRC("TextureManager::Load2D(): ")) {
                return (tHandle);
            }
        }

        // Set the public texture ID.
        tHandle = m_usedTextureCount;

        GLint unpackAlignment(1);
        if(!pack) {
            glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpackAlignment);
            GF_CHECK_GL_ERROR_TRC("TextureManager::Load2D(): ");
        }

        // Fill out the data struct for the texture we have loaded onto the GPU.
        TextureElement newTexElement;
        newTexElement.m_id = *tHandle;
        newTexElement.m_filename.assign(imgnameRef);
        newTexElement.m_timestamp = static_cast<F32>(g_appPtr->GetCurrTime());
        newTexElement.m_glTexId = m_glIdVec[*tHandle];
        newTexElement.m_minFilter = m_currMinFilter;
        newTexElement.m_magFilter = m_currMagFilter;
        newTexElement.m_wrapMode = wrapMode;
        newTexElement.m_glTarget = GL_TEXTURE_2D;
        newTexElement.m_width = imgResHandle->GetImageWidth();
        newTexElement.m_height = imgResHandle->GetImageHeight();
        newTexElement.m_imgFormat = imgResHandle->GetImageFormat();
        newTexElement.m_imgType = GL_UNSIGNED_BYTE;
        newTexElement.m_unpackAlignment = unpackAlignment;

        // Append the struct to the map.
        m_elementsMap[*tHandle] = newTexElement;
        ++m_usedTextureCount;

        return (tHandle);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    boost::optional<TexHandle> TextureManager::LoadRectangle(const std::string &imgnameRef, U32 &wRef, U32 &hRef, const GLenum wrapMode)
    {
        boost::optional<TexHandle> tHandle;             // The texture handle.

        if(imgnameRef.empty() || wrapMode == GL_REPEAT) {
            GF_LOG_TRACE_ERR("TextureManager::LoadRectangle()", "Invalid parameters");
            return (tHandle);
        }

        tHandle = Find(imgnameRef);
        if(tHandle.is_initialized()) {
            return (tHandle);
        }

        ImageResource imgRes(imgnameRef);
        boost::shared_ptr<ImageResHandle> imgResHandle = boost::static_pointer_cast<ImageResHandle>(g_appPtr->GetResourceCache()->GetHandle(&imgRes));
        if(!imgResHandle || !imgResHandle->VInitialize()) {
            GF_LOG_TRACE_ERR("TextureManager::LoadRectangle()", std::string("Failed to retrieve and/or initialize the resource ") + imgnameRef);
            return (tHandle);
        }

        wRef = imgResHandle->GetImageWidth();
        hRef = imgResHandle->GetImageHeight();

        // Check if we need to swap out old textures to make room.
        if(m_maxSize != 0) {
            U32 imgsize(wRef * hRef);
            if(imgsize > m_maxSize) {
                GF_LOG_TRACE_ERR("TextureManager::LoadRectangle()", "Cannot load image! It is bigger than the entire size of the TextureManagers memory budget!");
                return (tHandle);
            }
            while(imgsize + m_currSize > m_maxSize) {
                UnloadLRUTexture();
            }
        }

        // Ensure we have generated enough texture objects.
        if(m_usedTextureCount >= m_glIdVec.size()) {
            ResizeTextureVector();
        }

        GF_CLEAR_GL_ERROR();

        glBindTexture(GL_TEXTURE_RECTANGLE, m_glIdVec[m_usedTextureCount]);
        GF_CHECK_GL_ERROR_TRC("TextureManager::LoadRectangle(): ");

        bool pack = (FindImageTypeFromFile(imgnameRef) == IMAGE_TYPE_TGA);

        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        GF_CHECK_GL_ERROR_TRC("TextureManager::LoadRectangle(): ");
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        GF_CHECK_GL_ERROR_TRC("TextureManager::LoadRectangle(): ");
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, wrapMode);
        GF_CHECK_GL_ERROR_TRC("TextureManager::LoadRectangle(): ");
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, wrapMode);
        GF_CHECK_GL_ERROR_TRC("TextureManager::LoadRectangle(): ");

        if(!LoadCommon2D(GL_TEXTURE_RECTANGLE, 0, imgResHandle->GetImageComponents(), wRef, hRef, \
                         0, imgResHandle->GetImageFormat(), GL_UNSIGNED_BYTE, const_cast<GLbyte *>(imgResHandle->GetImageBuffer()), pack)) {
            return (tHandle);
        }

        // Set the public texture ID.
        tHandle = m_usedTextureCount;

        GLint unpackAlignment(1);
        if(!pack) {
            glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpackAlignment);
            GF_CHECK_GL_ERROR_TRC("TextureManager::LoadRectangle(): ");
        }

        // Fill out the data struct for the texture we have loaded onto the GPU.
        TextureElement newTexElement;
        newTexElement.m_id = *tHandle;
        newTexElement.m_filename.assign(imgnameRef);
        newTexElement.m_timestamp = static_cast<F32>(g_appPtr->GetCurrTime());
        newTexElement.m_glTexId = m_glIdVec[*tHandle];
        newTexElement.m_minFilter = GL_NEAREST;
        newTexElement.m_magFilter = GL_NEAREST;
        newTexElement.m_wrapMode = wrapMode;
        newTexElement.m_glTarget = GL_TEXTURE_RECTANGLE;
        newTexElement.m_width = wRef;
        newTexElement.m_height = hRef;
        newTexElement.m_imgFormat = imgResHandle->GetImageFormat();
        newTexElement.m_imgType = GL_UNSIGNED_BYTE;
        newTexElement.m_unpackAlignment = unpackAlignment;

        // Append the struct to the map.
        m_elementsMap[*tHandle] = newTexElement;
        ++m_usedTextureCount;

        return (tHandle);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    boost::optional<TexHandle> TextureManager::LoadCubeMap(const std::vector<std::string> &cubeImgVec, const GLenum wrapMode)
    {
        boost::optional<TexHandle> tHandle;             // The texture handle.
        const U32 CUBE_SIDES = 6;                       // Number of sides in a 3D cube.
        std::string concatStr;                          // string ID for string lookup map.

        if(cubeImgVec.size() != CUBE_SIDES) {
            GF_LOG_TRACE_ERR("TextureManager::LoadCubeMap()", "Invalid parameters");
            return (tHandle);
        }
        for(std::vector<std::string>::const_iterator i = cubeImgVec.begin(), end = cubeImgVec.end(); i != end; ++i) {
            if((*i).empty()) {
                GF_LOG_TRACE_ERR("TextureManager::LoadCubeMap()", "Invalid parameters");
                return (tHandle);
            } else {
                concatStr += *i;
            }
        }

        tHandle = Find(concatStr);
        if(tHandle.is_initialized()) {
            return (tHandle);
        }

        GLenum cubeEnum[CUBE_SIDES] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                                        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                                        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                                        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                                        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                                        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
                                      };

        // Ensure we have generated enough texture objects.
        if(m_usedTextureCount >= m_glIdVec.size()) {
            ResizeTextureVector();
        }

        GF_CLEAR_GL_ERROR();

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_glIdVec[m_usedTextureCount]);
        GF_CHECK_GL_ERROR_TRC("TextureManager::LoadCubeMap(): ");

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_currMinFilter);
        GF_CHECK_GL_ERROR_TRC("TextureManager::LoadCubeMap(): ");
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_currMagFilter);
        GF_CHECK_GL_ERROR_TRC("TextureManager::LoadCubeMap(): ");
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapMode);
        GF_CHECK_GL_ERROR_TRC("TextureManager::LoadCubeMap(): ");
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapMode);
        GF_CHECK_GL_ERROR_TRC("TextureManager::LoadCubeMap(): ");
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapMode);
        GF_CHECK_GL_ERROR_TRC("TextureManager::LoadCubeMap(): ");

        U32 totalSize(0);
        boost::shared_ptr<ImageResHandle> imgResArr[6];

        // Initialize cubemap images and calculate total size required.
        for(U32 i = 0; i < CUBE_SIDES; ++i) {
            ImageResource imgRes(cubeImgVec[i]);
            imgResArr[i] = boost::static_pointer_cast<ImageResHandle>(g_appPtr->GetResourceCache()->GetHandle(&imgRes));
            if(!imgResArr[i] || !imgResArr[i]->VInitialize()) {
                GF_LOG_TRACE_ERR("TextureManager::LoadCubeMap()", std::string("Failed to retrieve and/or initialize the resource ") + cubeImgVec[i]);
                return (tHandle);
            }
            totalSize += imgResArr[i]->GetImageWidth() * imgResArr[i]->GetImageHeight();
        }

        if(m_maxSize != 0) {
            if(totalSize > m_maxSize) {
                GF_LOG_TRACE_ERR("TextureManager::LoadCubeMap()", "Cannot load image! It is bigger than the entire size of the TextureManagers memory budget!");
                return (tHandle);
            }
            while(totalSize + m_currSize > m_maxSize) {
                UnloadLRUTexture();
            }
        }

        for(U32 i = 0; i < CUBE_SIDES; ++i) {
            bool pack = (FindImageTypeFromFile(cubeImgVec[i]) == IMAGE_TYPE_TGA);
            if(!LoadCommon2D(cubeEnum[i], 0, imgResArr[i]->GetImageComponents(), imgResArr[i]->GetImageWidth(), imgResArr[i]->GetImageHeight(), \
                             0, imgResArr[i]->GetImageFormat(), GL_UNSIGNED_BYTE, const_cast<GLbyte *>(imgResArr[i]->GetImageBuffer()), pack)) {
                return (tHandle);
            }
        }

        if(m_currMinFilter >= eBasicMipMap) {
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
            if(!GF_CHECK_GL_ERROR_TRC("TextureManager::LoadCubeMap(): ")) {
                return (tHandle);
            }
        }

        // Set the public texture ID.
        tHandle = m_usedTextureCount;

        GLint unpackAlignment(1);
        if(FindImageTypeFromFile(cubeImgVec[0]) != IMAGE_TYPE_TGA) {
            glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpackAlignment);
            GF_CHECK_GL_ERROR_TRC("TextureManager::LoadCubeMap(): ");
        }

        // Fill out the data struct for the texture we have loaded onto the GPU.
        TextureElement newTexElement;
        newTexElement.m_id = *tHandle;
        newTexElement.m_filename.assign(concatStr);
        newTexElement.m_timestamp = static_cast<F32>(g_appPtr->GetCurrTime());
        newTexElement.m_glTexId = m_glIdVec[*tHandle];
        newTexElement.m_minFilter = m_currMinFilter;
        newTexElement.m_magFilter = m_currMagFilter;
        newTexElement.m_wrapMode = wrapMode;
        newTexElement.m_glTarget = GL_TEXTURE_CUBE_MAP;
        newTexElement.m_width = imgResArr[0]->GetImageWidth();
        newTexElement.m_height = imgResArr[0]->GetImageHeight();
        newTexElement.m_imgFormat = imgResArr[0]->GetImageFormat();
        newTexElement.m_imgType = GL_UNSIGNED_BYTE;
        newTexElement.m_unpackAlignment = unpackAlignment;

        // Append the struct to the map.
        m_elementsMap[*tHandle] = newTexElement;
        ++m_usedTextureCount;

        return (tHandle);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TextureManager::Bind(const TexHandle textureHandle, const GLenum target, const GLint textureLayer)
    {
        GF_CLEAR_GL_ERROR();

        if(textureLayer != m_currTexLayer) {
            glActiveTexture(m_currTexLayer);
            if(!GF_CHECK_GL_ERROR_TRC("TextureManager::Bind(): ")) {
                m_currTexLayer = textureLayer;
            }
        }

        boost::optional<GLuint> textureId = Find(textureHandle, true);
        if(!textureId.is_initialized()) {
            GF_LOG_TRACE_ERR("TextureManager::Bind()", "The texture with the public ID has not been loaded into the TextureManager");
            return (false);
        }

        if(m_curBindTex == *textureId) {
            return (true);
        }

        glBindTexture(target, *textureId);
        if(!GF_CHECK_GL_ERROR_TRC("TextureManager::Bind(): ")) {
            return (false);
        }
        m_curBindTex = *textureId;

        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TextureManager::Contains(const TexHandle textureHandle)
    {
        boost::optional<GLuint> texturePrivateId = Find(textureHandle);
        if(!texturePrivateId.is_initialized()) {
            return (false);
        }

        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TextureManager::Contains(const std::string &imgnameRef)
    {
        boost::optional<TexHandle> texturePublicId = Find(imgnameRef);
        if(!texturePublicId.is_initialized()) {
            return (false);
        }

        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    boost::optional<GLuint> TextureManager::GetGLTextureHandle(const TexHandle texturePublicHandle)
    {
        return (Find(texturePublicHandle));
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TextureManager::FreeAll()
    {
        GF_CLEAR_GL_ERROR();

        bool glError(false);
        if(!m_glIdVec.empty()) {
            glDeleteTextures(static_cast<GLsizei>(m_glIdVec.size()), &m_glIdVec[0]);
            glError = GF_CHECK_GL_ERROR_TRC("TextureManager::FreeAll(): ");
        }

        if(glError) {
            return (false);
        }

        m_glIdVec.clear();
        m_elementsMap.clear();
        m_usedTextureCount = 0;
        m_currTexLayer = GL_TEXTURE0;
        SetTextureFilterMode(eBasic);
        m_anisotropicLinearLevel = 0.0f;
        m_currSize = 0;
        m_curBindTex = 9999;

        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    GLint TextureManager::GetMaxTextureUnits() const
    {
        GLint units;
        GF_CLEAR_GL_ERROR();
        glGetIntegerv(GL_MAX_TEXTURE_UNITS, &units);
        if(!GF_CHECK_GL_ERROR_TRC("TextureManager::GetMaxTextureUnits(): ")) {
            return (0);
        }
        return (units);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    GLint TextureManager::GetMaxTextureSize(const GLenum texType) const
    {
        if((texType != GL_MAX_TEXTURE_SIZE) || (texType != GL_MAX_CUBE_MAP_TEXTURE_SIZE)) {
            GF_LOG_TRACE_INF("TextureManager::GetMaxTextureSize()", "Invalid parameters");
            return (-1);
        }

        GLint maxSize;
        GF_CLEAR_GL_ERROR();
        glGetIntegerv(texType, &maxSize);
        if(!GF_CHECK_GL_ERROR_TRC("TextureManager::GetMaxTextureSize(): ")) {
            return (0);
        }
        return (maxSize);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TextureManager::DummyLoad()
    {
        // TODO:
        GF_LOG_INF("TextureManager::DummyLoad() not yet implemented");
        return (false);
    }

}
