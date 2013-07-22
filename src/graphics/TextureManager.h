#pragma once
#ifndef __GF_TEXTURE_MANAGER_H
#define __GF_TEXTURE_MANAGER_H

// /////////////////////////////////////////////////////////////////
// @file TextureManager.h
// @author PJ O Halloran
// @date 07/10/2010
//
// Header for the global TextureManager class for managing OpenGL
// textures.
//
// /////////////////////////////////////////////////////////////////

#include <boost/optional.hpp>

#include <map>
#include <vector>
#include <string>

#include "GameBase.h"
#include "GameException.h"

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran {

    // A handle to the texture passed out to users of the texture manager.
    typedef I32 TexHandle;

    // /////////////////////////////////////////////////////////////////
    // @class TextureManager
    // @author PJ O Halloran.
    //
    // Global texture manager used for loading in image files from the
    // resource file.
    //
    // It also manages a series of OpenGL textures, creating them as
    // required and cleaning them up on shutdown.
    //
    // It also optionally has functionality for using only a certain
    // fraction of available GPU memory and swapping textures in and out
    // to maintain available memory.
    // It uses a LRU strategy for removing textures when the memory is full
    // by timestamping texture access and removing the LRU texture(s) from
    // GPU memory when the texture manager detects an overflow while
    // loading in a new texture.
    //
    // TODO:
    // Check the texture being swapped out when the limit is reached to
    // prevent trashing.  Use a MRU strategy temporarily to avoid this.
    //
    // /////////////////////////////////////////////////////////////////
    class TextureManager : public NonCopyable {
    public:

        // /////////////////////////////////////////////////////////////////
        // @enum TextureFilterMode
        // @author PJ O Halloran
        //
        // List of texture filtering modes available.
        // - eBasic         (min = GL_NEAREST, mag = GL_NEAREST) w/o mipmaps.
        // - eBasicMipMap   (min = GL_NEAREST_MIPMAP_NEAREST, mag = GL_NEAREST).
        // - eBilinear      (min = GL_LINEAR_MIPMAP_NEAREST, mag = GL_LINEAR).
        // - eTrilinear     (min = GL_LINEAR_MIPMAP_LINEAR, mag = GL_LINEAR).
        // - eAnisotropic   (min = GL_LINEAR_MIPMAP_LINEAR, mag = GL_LINEAR) + anisotropic filtering applied.
        //
        // NB. eAnisotropic will only be available on hardware with the
        // GLEW_EXT_texture_filter_anisotropic extension defined.
        //
        // /////////////////////////////////////////////////////////////////
        enum TextureFilterMode {
            eBasic = 0,
            eBasicMipMap,
            eBilinear,
            eTrilinear,
#ifdef GLEW_EXT_texture_filter_anisotropic
            eAnisotropic,
#endif
            eNumberModes,
            eUnknown
        };

        // /////////////////////////////////////////////////////////////////
        // @struct TextureElement
        // @author PJ O Halloran
        //
        // Cache of information on a texture stored on the GPU.
        //
        // /////////////////////////////////////////////////////////////////
        struct TextureElement {
            TexHandle m_id;                     ///< The application side ID.
            std::string m_filename;             ///< The filename from where the img was loaded from.
            F32 m_timestamp;                    ///< Timestamp of the last texture access.

            GLuint m_glTexId;                   ///< OpenGL texture ID.
            GLenum m_minFilter;                 ///< Current min filter applied to the texture.
            GLenum m_magFilter;                 ///< Current mag filter applied to the texture.
            GLenum m_wrapMode;                  ///< Wrap mode for the texture.
            GLenum m_glTarget;                  ///< The texture target type.
            GLsizei m_width;                    ///< Width of the image.
            GLsizei m_height;                   ///< Height of the image.
            GLenum m_imgFormat;                 ///< The image format.
            GLenum m_imgType;                   ///< The image type.
            GLsizei m_unpackAlignment;          ///< The unpack alignment used to unpack the texture data onto the GPU memory.
        };

    private:

        typedef std::map<TexHandle, TextureElement> ElementMap;

        ElementMap m_elementsMap;                                   ///< Map of texture handles known externally to Texture information.
        std::vector<GLuint> m_glIdVec;                              ///< Striaght vector of OpenGL texture Ids generated on startup and when required.
        U32 m_usedTextureCount;                                     ///< The number of textures used.
        GLint m_currTexLayer;                                       ///< Cache of the current active texture layer.
        GLint m_maxTexLayers;                                       ///< Cache of the maximum allowed texture layers on this GPU.
        TextureFilterMode m_currTexFilterMode;                      ///< The current texture filter mode.
        GLenum m_currMinFilter;                                     ///< The current minification texture filter mode.
        GLenum m_currMagFilter;                                     ///< The current magnification texture filter mode.
        F32 m_anisotropicLinearLevel;                               ///< The level of anisotropic filtering to use.  Between 0.0 and 1.0.  Used to linearly interpolate the actual value used.
        GLfloat m_maxAnisotropicValue;                              ///< The maximum anisotropic value the hardware is capable of (if at all).
        U32 m_extendSize;                                           ///< The number of OpenGL texture IDs to generate at a time when we run out of free texture IDs when textures are added.
        const U32 m_maxSize;                                        ///< The maximum allowable texture memory in bytes the TextureManager has access to on the GPU.
        U32 m_currSize;                                             ///< Current GPU memory used for textures in bytes.
        GLuint m_curBindTex;                                        ///< Currently binded texture.

        // The number of texture objects to generate if we run out of them by default.
        static const U32 DEFAULT_EXTEND_SIZE = 10;

        // /////////////////////////////////////////////////////////////////
        // Called if we have got to the end of our allocated texture vector.
        // This method will generate 10 new texture objects and extend the
        // internal texture vector by 10 also.
        //
        // /////////////////////////////////////////////////////////////////
        void ResizeTextureVector();

        // /////////////////////////////////////////////////////////////////
        // Loads a 2D texture.
        //
        // The texture must be set/binded beforehand.
        //
        // Target must be:
        // - GL_TEXTURE_2D
        // - GL_TEXTURE_RECTANGLE
        // - GL_TEXTURE_CUBE_MAP_POSITIVE_X
        // - GL_TEXTURE_CUBE_MAP_NEGATIVE_X
        // - GL_TEXTURE_CUBE_MAP_POSITIVE_Y
        // - GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
        // - GL_TEXTURE_CUBE_MAP_POSITIVE_Z
        // - GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
        //
        // @param target Texture type.
        // @param level Mipmap level.
        // @param internalFormat Format of data on GPU.
        // @param width Width of texture.
        // @param height Height of texture.
        // @param border Border width
        // @param format Format of the image data.
        // @param type Type of data.
        // @param data Pointer to the data.
        // @param tightlyPack Should we instruct GL to unpack the image
        //                      data in a 1 byte packed format (for TGA).
        //
        // @return bool True if the image was loaded and false if not.
        //
        // /////////////////////////////////////////////////////////////////
        bool LoadCommon2D(const GLenum target, const GLint level, const GLint internalFormat, const GLsizei width, const GLsizei height, \
                          const GLint border, const GLenum format, const GLenum type, void *data, const bool tightlyPack = false);

        // /////////////////////////////////////////////////////////////////
        // Find a textures private ID by the texture handle.
        //
        // @param tHandle Public texture ID.
        // @param timestampTex Should we apply a timestamp to the texture to
        //                      imply it is being accessed?
        //
        // /////////////////////////////////////////////////////////////////
        boost::optional<GLuint> Find(const TexHandle tHandle, bool timestampTex = false);

        // /////////////////////////////////////////////////////////////////
        // Find a textures public ID by the string name used to load the
        // texture.
        //
        // /////////////////////////////////////////////////////////////////
        boost::optional<TexHandle> Find(const std::string &imgnameRef) const;

        // /////////////////////////////////////////////////////////////////
        // Unload the least recently used texture from the GPU.
        //
        // @param U32 The number of bytes freed. 0 if there are no
        //                      textures loaded or uninitialized if an error
        //                      occurred.
        //
        // /////////////////////////////////////////////////////////////////
        boost::optional<U32> UnloadLRUTexture();

        // /////////////////////////////////////////////////////////////////
        // Unload the texture with the information supplied from the GPU.
        //
        // All the cached member variables and containers are updated without
        // the texture if the texture memor is freed successfully.
        //
        // @param texIter Iterator pointing to the element to remove.
        //
        // @param U32 The number of bytes freed. 0 if there are no
        //                      textures loaded or uninitialized if an error
        //                      occurred.
        //
        // /////////////////////////////////////////////////////////////////
        boost::optional<U32> UnloadTexture(ElementMap::iterator &texIter);

        // /////////////////////////////////////////////////////////////////
        // Update the texture filters for all managed textures according to
        // the current min and mag level and possibly the current anisotropic
        // level.
        //
        // /////////////////////////////////////////////////////////////////
        void UpdateTextureFilters(boost::optional<TextureFilterMode> oldMode = boost::optional<TextureFilterMode>());

    public:

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param expectedNumTextures The number of texture you expect
        //                              to create during the program execution.
        // @param maxSize The maximum number of texture memory (in bytes)
        //                  the manager can use before it will start
        //                  recycling memory by unloading not recently
        //                  used textures and loading new textures.
        //                  if its 0, the manager will assume
        //                  unlimited memory!
        //
        // /////////////////////////////////////////////////////////////////
        explicit TextureManager(const U32 expectedNumTextures, const U32 maxSize) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        ~TextureManager();

        // /////////////////////////////////////////////////////////////////
        // Get the current texture filter mode.
        //
        // /////////////////////////////////////////////////////////////////
        inline TextureFilterMode GetTextureFilterMode() const {
            return (m_currTexFilterMode);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the current texture filter mode.  This will update the filter
        // modes of all textures currently residing on the GPU.
        //
        // N.B. If anisotropic filtering is available and the new filter mode
        // is eAnisotropic then the anisotropic level used will be the current
        // level set previously with SetAnisotropicLinearLevel().
        //
        // /////////////////////////////////////////////////////////////////
        void SetTextureFilterMode(const TextureFilterMode mode);

        // /////////////////////////////////////////////////////////////////
        // Runtime check to query if the hardware we are running on is
        // Anisotropic filtering capable.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool IsAnisotropicAvailable() const {
#ifdef GLEW_EXT_texture_filter_anisotropic
            return (true);
#else
            return (false);
#endif
        };

        // /////////////////////////////////////////////////////////////////
        // Get the current anisotropic linear level.  This means the linearly
        // interpolated level between 0.0 and the value of GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT.
        //
        // /////////////////////////////////////////////////////////////////
        inline F32 GetAnisotropicLinearLevel() const {
            return (m_anisotropicLinearLevel);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the anisotropic linear level.  This means the linearly interpolated
        // level between 0.0 and the value of GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT.
        //
        // N.B. The texture filter mode must be eAnisotropic for this to have
        // an effect.
        // N.B. Only available on certain hardware.
        //
        // /////////////////////////////////////////////////////////////////
        void SetAnisotropicLinearLevel(const F32 anisotropicLinearLevel);

        // /////////////////////////////////////////////////////////////////
        // Get the number of GL texture IDs to generate when we run out of
        // them.
        //
        // /////////////////////////////////////////////////////////////////
        inline U32 GetExtendGlIdSize() const {
            return (m_extendSize);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the number of GL texture IDs to generate when we run out of
        // them.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetExtendGlIdSize(const U32 extendSize) {
            if(extendSize == 0) {
                m_extendSize = 1;
            } else {
                m_extendSize = extendSize;
            }
        };

        // /////////////////////////////////////////////////////////////////
        // Loads a 1D texture with the texture data supplied.
        //
        // Notes:
        // - The texture data supplied must be in the GL_RGB format.
        // - The internal format of the texture on the GPU is GL_RGB.
        // - Mipmaps will be generated automatically if the current filter
        //   level is Bilinear or higher.
        // - The type of data is GL_UNSIGNED_BYTE.
        //
        // @param imgnameRef The string ID of the 1D texture for future
        //                      reference.
        // @param textureData The texture data. A 2D array of width "w" and
        //                      height of 3 (GL_RGB format).
        // @param w The width of the texture.
        // @param wrapMode The texture wrap mode (default: GL_CLAMP_TO_EDGE).
        //
        // @return boost::optional<TexHandle> Uninitialized on failure or will
        //      contain the texture handle (not the GL texture ID!) on success.
        //
        // /////////////////////////////////////////////////////////////////
        boost::optional<TexHandle> Load1D(const std::string &imgnameRef, GLubyte **textureData, const U32 w, const GLenum wrapMode = GL_CLAMP_TO_EDGE);

        // /////////////////////////////////////////////////////////////////
        // Loads a 2D texture from the resource cache identified by the image
        // name string.
        //
        // Notes:
        // - The internal components, format, width and height will be read
        //   from the image.
        // - If the image file to be read in is a TGA file then the
        //   GL_UNPACK_ALIGNMENT parameter will be temporarily set to 1.
        // - Mipmaps will be generated automatically if the current filter
        //   level is Bilinear or higher.
        // - The type of data is GL_UNSIGNED_BYTE.
        //
        // @param imgnameRef The name of the image to load from the ResourceCache.
        // @param wrapMode The texture wrap mode (default: GL_CLAMP_TO_EDGE).
        //
        // @return boost::optional<TexHandle> Uninitialized on failure or will
        //      contain the texture handle (not the GL texture ID!) on success.
        //
        // /////////////////////////////////////////////////////////////////
        boost::optional<TexHandle> Load2D(const std::string &imgnameRef, const GLenum wrapMode = GL_CLAMP_TO_EDGE);

        // /////////////////////////////////////////////////////////////////
        // Loads a Rectangle texture from the resource cache identified by
        // the image name string.
        //
        // Notes:
        // - The internal components, format, width and height will be read
        //   from the image.
        // - If the image file to be read in is a TGA file then the
        //   GL_UNPACK_ALIGNMENT parameter will be temporarily set to 1.
        // - Mipmaps cannot be generated for rectangle textures.
        // - The type of data is GL_UNSIGNED_BYTE.
        // - Only Basic filtering mode is allowed for rectangle textures.
        // - GL_REPEAT and GL_REPEAT_MIRRORED clamp values are not supported.
        //
        // @param imgnameRef The name of the image to load from the ResourceCache.
        // @param wRef The width of the image on success.
        // @param hRef The height of the image on success.
        // @param wrapMode The texture wrap mode (default: GL_CLAMP_TO_EDGE).
        //
        // @return boost::optional<TexHandle> Uninitialized on failure or will
        //      contain the texture handle (not the GL texture ID!) on success.
        //
        // /////////////////////////////////////////////////////////////////
        boost::optional<TexHandle> LoadRectangle(const std::string &imgnameRef, U32 &wRef, U32 &hRef, const GLenum wrapMode = GL_CLAMP_TO_EDGE);

        // /////////////////////////////////////////////////////////////////
        // Loads a CubeMap texture from the resource cache from 6 images
        // loaded in from the resource cache.
        //
        // Notes:
        // - The internal components, format, width and height will be read
        //   from the image.
        // - If the image file to be read in is a TGA file then the
        //   GL_UNPACK_ALIGNMENT parameter will be temporarily set to 1.
        // - Mipmaps will be generated automatically if the current filter
        //   level is Bilinear or higher.
        // - The type of data is GL_UNSIGNED_BYTE.
        //
        // @param cubeImgVec A vector of names of the image to load from
        //                      the ResourceCache.
        // @param minFilter The minification filter (default: GL_NEAREST).
        // @param magFilter The magnification filter (default: GL_NEAREST).
        // @param wrapMode The texture wrap mode (default: GL_CLAMP_TO_EDGE).
        //
        // @return boost::optional<TexHandle> Uninitialized on failure or will
        //      contain the texture handle (not the GL texture ID!) on success.
        //
        // /////////////////////////////////////////////////////////////////
        boost::optional<TexHandle> LoadCubeMap(const std::vector<std::string> &cubeImgVec, const GLenum wrapMode = GL_CLAMP_TO_EDGE);

        // /////////////////////////////////////////////////////////////////
        // Bind a texture for use.
        //
        // @param textureHandle The handle of the texture by which the texture
        //                      is identified outside the manager.
        // @param target The type of texture.
        // @param textureLayer The layer to apply the texture to.  The default
        //                      is layer 0 which is always available.
        //
        // @return bool True on success.
        // @return bool False if no texture exists in the manager associated
        //              with the texture handle.
        // @return bool False if the layer supplied is greater than the number
        //              of textures this implementation supports.
        //
        // /////////////////////////////////////////////////////////////////
        bool Bind(const TexHandle textureHandle, const GLenum target, const GLint textureLayer = GL_TEXTURE0);

        // /////////////////////////////////////////////////////////////////
        // Check if the manager has a texture asociated with a handle.
        //
        // /////////////////////////////////////////////////////////////////
        bool Contains(const TexHandle textureHandle);

        // /////////////////////////////////////////////////////////////////
        // Check if the manager has a texture associated with a name.
        //
        // /////////////////////////////////////////////////////////////////
        bool Contains(const std::string &imgnameRef);

        // /////////////////////////////////////////////////////////////////
        // Sometimes some GL operations require the use of the texture ID/
        // handle.  This searches for and gets the GL texture ID associated
        // with a public texture handle.
        //
        // @param textureHandle Public texture handle.
        //
        // @return boost::optional<GLuint> Private GL texture ID or it is
        //                                  not initialized in the case of a
        //                                  failure.
        //
        // /////////////////////////////////////////////////////////////////
        boost::optional<GLuint> GetGLTextureHandle(const TexHandle textureHandle);

        // /////////////////////////////////////////////////////////////////
        // Manually unload a texture from memory.
        //
        // /////////////////////////////////////////////////////////////////
        bool UnloadTexture(const TexHandle textureHandle);

        // /////////////////////////////////////////////////////////////////
        // Free all textures from GPU memory.
        //
        // /////////////////////////////////////////////////////////////////
        bool FreeAll();

        // /////////////////////////////////////////////////////////////////
        // Get the maximum number of texture units/layers available on this
        // implementation.
        //
        // /////////////////////////////////////////////////////////////////
        GLint GetMaxTextureUnits() const;

        // /////////////////////////////////////////////////////////////////
        // Get the maximum texture size.
        //
        // /////////////////////////////////////////////////////////////////
        GLint GetMaxTextureSize(const GLenum texType) const;

        // /////////////////////////////////////////////////////////////////
        // Load a dummy texture to check if this implementation supports
        // the texture size, format, etc.
        //
        // TODO: Implement this.
        //
        // /////////////////////////////////////////////////////////////////
        bool DummyLoad();

    };

}

#endif
