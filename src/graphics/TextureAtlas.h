#pragma once
#ifndef __GF_TEXTURE_ATLAS_H
#define __GF_TEXTURE_ATLAS_H

// /////////////////////////////////////////////////////////////////
// @file TextureAtlas.h
// @author PJ O Halloran
// @date 07/10/2010
//
// Header for TextureAtlas class for defining a texture atlas used
// to batch up images and minimize GPU state changes.
//
// Used to read in atlases and their data files generated by the
// "AtlasGenerator" build script located in $ROOT/src/build/tools/.
//
// /////////////////////////////////////////////////////////////////

#include <map>

#include "GameBase.h"
#include "HashedString.h"
#include "TextureManager.h"

class TiXmlElement;

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran {
    struct TextureAtlas;

    // /////////////////////////////////////////////////////////////////
    // @struct TextureAtlasImage
    // @author PJ O Halloran
    //
    // Data on a sub image in a texture atlas.
    //
    // /////////////////////////////////////////////////////////////////
    struct AtlasImage {
        F32 m_x;                        ///< X coordinate of image inside atlas.
        F32 m_y;                        ///< Y coordinate of image inside atlas.
        F32 m_width;                    ///< Width of image inside atlas.
        F32 m_height;                   ///< Height of image inside atlas.
        HashedString m_id;              ///< ID of the image.
        bool m_flipped;                 ///< Is the tex coordinates flipped 90 degrees.

        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        AtlasImage(const char *name) : m_x(0.0f), m_y(0.0f), m_width(0.0f), m_height(0.0f), m_id(name), m_flipped(false) {};
    };

    typedef boost::shared_ptr<AtlasImage> AtlasImageSPtr;
    typedef std::map<U64, AtlasImageSPtr > AtlasImageMap;

    struct TextureAtlas {
        U32 m_borderSize;       ///< Number of pixels space between images in the atlas.
        U32 m_width;            ///< Width of atlas.
        U32 m_height;           ///< Height of atlas.
        HashedString m_id;      ///< Unique id.
        I32 m_mode;             ///< Image mode.
        I32 m_type;             ///< Image type.
        AtlasImageMap m_images; ///< Collection of atlas images.
        TexHandle m_atlasId;    ///< ID of the atlas the image belongs to.

        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        TextureAtlas(const char *name) : m_borderSize(0), m_width(0), m_height(0), m_id(name), m_mode(0), m_type(0), m_images(), m_atlasId(0) {};
    };

    typedef boost::shared_ptr<TextureAtlas> TextureAtlasSPtr;
    typedef std::map<U64, TextureAtlasSPtr > TextureAtlasMap;

    // /////////////////////////////////////////////////////////////////
    // @class TextureAtlasManager
    // @author PJ O Halloran
    //
    // Takes care of loading an image atlas and using the atlas.
    //
    // /////////////////////////////////////////////////////////////////
    class TextureAtlasManager {
    private:

        bool m_loadingFromFilesystem;           ///< ...
        bool m_loaded;                          ///< ...
        TextureAtlasMap m_atlasMap;             ///< Our list of atlases.
        TextureAtlas *m_currAtlasPtr;           ///< Currently selected atlas.
        AtlasImage *m_currImagePtr;             ///< Currently selected atlas image.

        // /////////////////////////////////////////////////////////////////
        // Parse the xml dictionary.
        //
        // /////////////////////////////////////////////////////////////////
        bool ParseXml(TiXmlElement *rootNodePtr);

        // /////////////////////////////////////////////////////////////////
        // Parse an <Atlas> element.
        //
        // /////////////////////////////////////////////////////////////////
        bool ParseAtlasElement(TiXmlElement *atlasNodePtr);

    public:

        // /////////////////////////////////////////////////////////////////
        // Default.
        //
        // /////////////////////////////////////////////////////////////////
        TextureAtlasManager();

        // /////////////////////////////////////////////////////////////////
        // Create and load a texture atlas from a file.
        //
        // @param atlasFilename Name of the atlas file.
        //
        // /////////////////////////////////////////////////////////////////
        TextureAtlasManager(const boost::filesystem::path &atlasFilename);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        ~TextureAtlasManager() {
            Clear();
        };

        // /////////////////////////////////////////////////////////////////
        // Did the texture atlas description file and image data get loaded?
        //
        // /////////////////////////////////////////////////////////////////
        bool IsLoaded() const {
            return (m_loaded);
        };

        // /////////////////////////////////////////////////////////////////
        // Clear the altas.
        //
        // /////////////////////////////////////////////////////////////////
        void Clear();

        // /////////////////////////////////////////////////////////////////
        // Load a texture atlas from a file.
        //
        // @param atlasFilename The filepath of the atlas xml dictionary.
        //
        // @return bool True|False on success|failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool LoadFromFile(const boost::filesystem::path &atlasFilename);

        // /////////////////////////////////////////////////////////////////
        // Load all texture atlases from the resource cache.
        //
        // @return bool True|False on success|failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool LoadFromResourceCache();

        // /////////////////////////////////////////////////////////////////
        // Make a texture atlas the currently bound image on the GPU.
        //
        // @param textureLevel
        //
        // @return bool True|False on success|failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool UseAtlas(const std::string &atlasId);

        // /////////////////////////////////////////////////////////////////
        // Set the currently selected atlas sub image.
        //
        // @param imgName The ID of the sub image.
        //
        // @return bool True|False on success|failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool UseImage(const std::string &imgName);

        // /////////////////////////////////////////////////////////////////
        // Get the currently selected atlas data.
        //
        // /////////////////////////////////////////////////////////////////
        const TextureAtlas * const GetCurrentAtlasData() const {
            return (m_currAtlasPtr);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the currently selected atlas image data.
        //
        // /////////////////////////////////////////////////////////////////
        const AtlasImage * const GetCurrentAtlasImage() const {
            return (m_currImagePtr);
        };

    };

}

#endif // END __GF_TEXTURE_ATLAS_H
