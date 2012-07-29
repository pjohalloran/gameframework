// /////////////////////////////////////////////////////////////////
// @file TextureAtlas.cpp
// @author PJ O Halloran
// @date 07/10/2010
//
// Implementation of the TextureAtlas class.
//
// /////////////////////////////////////////////////////////////////


#include "TextureAtlas.h"

#include "GameMain.h"

#include <tinyxml/tinyxml.h>
#include <string>

#include "ImageResource.h"
#include "TextResource.h"

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TextureAtlasManager::ParseXml(TiXmlElement *rootNodePtr)
    {
        assert(rootNodePtr != NULL);
        
		for(TiXmlElement *currElemPtr = rootNodePtr->FirstChildElement(); currElemPtr; currElemPtr = currElemPtr->NextSiblingElement())
		{
			if(currElemPtr && !currElemPtr->ToComment())
            {
				if(!ParseAtlasElement(currElemPtr))
                    return (false);
            }
		}
        
        return (true);
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TextureAtlasManager::ParseAtlasElement(TiXmlElement *atlasNodePtr)
    {
        assert(atlasNodePtr != NULL && strcmp(atlasNodePtr->Value(), "Atlas") == 0);
        
        // <Atlas border="1" height="1024" mode="RGBA" name="INGAME" type="png" width="1024">
        
        TextureAtlasSPtr atlas(new TextureAtlas(atlasNodePtr->Attribute("name")));
        std::string mode(atlasNodePtr->Attribute("mode"));
        std::string type(atlasNodePtr->Attribute("type"));
        atlasNodePtr->Attribute("border", (int *)&atlas->m_borderSize);
        atlasNodePtr->Attribute("width", (int *)&atlas->m_width);
        atlasNodePtr->Attribute("height", (int *)&atlas->m_height);
        atlas->m_type = GameHalloran::FindImageTypeFromFile(type);
        atlas->m_mode = FindImageModeFromString(mode);
        
        // Load atlas image from location using TextureManager.
        if(m_loadingFromFilesystem)
        {
            // TODO:
            assert(false);
        }
        else
        {
            boost::optional<TexHandle> handle = g_appPtr->GetTextureManagerPtr()->Load2D(std::string("atlases") + ZipFile::ZIP_PATH_SEPERATOR + atlas->m_id.getStr() + std::string(".") + type);
            if(!handle)
                return (false);
            
            atlas->m_atlasId = *handle;
        }
        
        m_atlasMap[atlas->m_id.getHashValue()] = atlas;
        
        for(TiXmlElement *currChildPtr = atlasNodePtr->FirstChildElement(); currChildPtr; currChildPtr = currChildPtr->NextSiblingElement())
        {
            // If the current element pointer is valid and it is not an xml comment, then parse it further.
            if(currChildPtr && !currChildPtr->ToComment() && strcmp(currChildPtr->Value(), "image") == 0)
            {
                // <image flipped="False" height="0.125" imagefile="ball01.tga" width="0.125" x="0.0009765625" y="0.0009765625"/>
                double tmp(0.0);
                
                AtlasImageSPtr image(new AtlasImage(currChildPtr->Attribute("imagefile")));
                
                if(image->m_id.getStr().compare("MainMenuBackground.tga") == 0)
                    int tmp = 0;
                
                currChildPtr->Attribute("x", &tmp);
                image->m_x = (float)tmp / atlas->m_width;
                currChildPtr->Attribute("y", &tmp);
                image->m_y = (float)tmp / atlas->m_height;
                image->m_y = 1.0f - image->m_y;
                currChildPtr->Attribute("width", &tmp);
                image->m_width = (float)tmp / atlas->m_width;
                currChildPtr->Attribute("height", &tmp);
                image->m_height = (float)tmp / atlas->m_height;
                if(strcmp("True", currChildPtr->Attribute("flipped")) == 0)
                    image->m_flipped = true;
                
                atlas->m_images[image->m_id.getHashValue()] = image;
            }
        }
        
        return (true);
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    TextureAtlasManager::TextureAtlasManager() : m_loadingFromFilesystem(false), m_loaded(false), m_atlasMap(), m_currAtlasPtr(NULL), m_currImagePtr(NULL)
    {
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    TextureAtlasManager::TextureAtlasManager(const boost::filesystem::path &atlasFilename) : m_loadingFromFilesystem(false), m_loaded(false), m_atlasMap(), m_currAtlasPtr(NULL), m_currImagePtr(NULL)
    {
        LoadFromFile(atlasFilename);
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    TextureAtlasManager::TextureAtlasManager(const std::string &resourceId) : m_loadingFromFilesystem(false), m_loaded(false), m_atlasMap(), m_currAtlasPtr(NULL), m_currImagePtr(NULL)
    {
        LoadFromResourceCache(resourceId);
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void TextureAtlasManager::Clear()
    {
        m_atlasMap.clear();
        m_loaded = false;
        m_loadingFromFilesystem = false;
        m_currAtlasPtr = NULL;
        m_currImagePtr = NULL;
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TextureAtlasManager::LoadFromFile(const boost::filesystem::path &atlasFilename)
    {
        if(!boost::filesystem::exists(atlasFilename))
        {
            GF_LOG_TRACE_ERR("TextureAtlasManager::LoadFromFile()", atlasFilename.string() + std::string(" file does not exist"));
            return (false);
        }
        
        TiXmlDocument xmlDoc(atlasFilename.string().c_str());
        
        if(!xmlDoc.LoadFile() || xmlDoc.Error())
        {
            GF_LOG_TRACE_ERR("TextureAtlasManager::LoadFromFile()", std::string("Failed to load or parse atlas xml file: ") + atlasFilename.string());
            return (false);
        }
        
        TiXmlHandle docHandle(&xmlDoc);
        
		TiXmlElement *rootElemPtr = docHandle.FirstChild("Root").ToElement();
		if(!rootElemPtr)
		{
            GF_LOG_TRACE_ERR("TextureAtlasManager::LoadFromFile()", std::string("Failed to find Root element node in atlas xml file: ") + atlasFilename.string());
            return (false);
		}
        
        m_loadingFromFilesystem = true;
        return (m_loaded = ParseXml(rootElemPtr));
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TextureAtlasManager::LoadFromResourceCache(const std::string &resourceId)
    {
        TextResource atlasRes(resourceId);
        boost::shared_ptr<TextResHandle> atlasHandle = boost::static_pointer_cast<TextResHandle>(g_appPtr->GetResourceCache()->GetHandle(&atlasRes));
        if(!atlasHandle || !atlasHandle->VInitialize())
        {
            GF_LOG_TRACE_ERR("TextureAtlasManager::LoadFromResourceCache()", std::string("Failed to retrieve ") + resourceId + std::string(" from the resource cache"));
            return (false);
        }
        
        TiXmlDocument xmlDoc;
        xmlDoc.Parse(atlasHandle->GetTextBuffer(), 0, TIXML_DEFAULT_ENCODING);
        
        if(xmlDoc.Error())
        {
            GF_LOG_TRACE_ERR("TextureAtlasManager::LoadFromResourceCache()", std::string("Failed to parse atlas xml file: ") + resourceId);
            return (false);
        }
        
        TiXmlHandle docHandle(&xmlDoc);
        
		TiXmlElement *rootElemPtr = docHandle.FirstChild("Root").ToElement();
		if(!rootElemPtr)
		{
            GF_LOG_TRACE_ERR("TextureAtlasManager::LoadFromResourceCache()", std::string("Failed to find Root element node in atlas xml file: ") + resourceId);
            return (false);
		}
        
        m_loadingFromFilesystem = false;
        return (m_loaded = ParseXml(rootElemPtr));
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TextureAtlasManager::UseAtlas(const std::string &atlasId)
    {
        HashedString id(atlasId.c_str());
        
        TextureAtlasMap::iterator atlasIter = m_atlasMap.find(id.getHashValue());
        if(atlasIter == m_atlasMap.end())
            return (false);
        
        m_currAtlasPtr = (atlasIter->second).get();
        
        return (g_appPtr->GetTextureManagerPtr()->Bind(m_currAtlasPtr->m_atlasId, GL_TEXTURE_2D));
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool TextureAtlasManager::UseImage(const std::string &imgName)
    {
        if(!m_currAtlasPtr)
            return (false);
        
        HashedString id(imgName.c_str());
        
        AtlasImageMap::iterator imageIter = m_currAtlasPtr->m_images.find(id.getHashValue());
        if(imageIter == m_currAtlasPtr->m_images.end())
            return (false);
        
        m_currImagePtr = (imageIter->second).get();
        
        return (true);
    }
    
}
