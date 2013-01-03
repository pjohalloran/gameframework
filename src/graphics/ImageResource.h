#pragma once
#ifndef _GF_IMAGE_RESOURCE_H
#define _GF_IMAGE_RESOURCE_H

// ////////////////////////////////////////////////////////////////////
// @file ImageResource.h
// @author PJ O Halloran
// @date 27/09/2010
//
// Classes for image resources for loading images using the 
// ResourceCache functionality.
//
// ////////////////////////////////////////////////////////////////////

#include <string>

#include "GameBase.h"
#include "ResCache2.h"

namespace GameHalloran
{

	// ////////////////////////////////////////////////////////////////////
	// @enum ImageMode
	// @author PJ O Halloran
	//
	// Defines various image types.
	//
	// ////////////////////////////////////////////////////////////////////
    enum ImageMode
    {
        eRGB = 0,
        eRGBA,
        eIMAGE_MODE_COUNT,
        eIMAGE_MODE_UNKNOWN
    };
    
	// ////////////////////////////////////////////////////////////////////
	// @enum ImageType
	// @author PJ O Halloran
	//
	// Defines supported image types compatible to be stored in game
	// resource files (zips) which may be loaded from memory.
	//
	// ////////////////////////////////////////////////////////////////////
	enum ImageType
	{
		IMAGE_TYPE_FIRST,
		IMAGE_TYPE_TGA = IMAGE_TYPE_FIRST,
		IMAGE_TYPE_BMP,
		IMAGE_TYPE_PNG,
		IMAGE_TYPE_JPEG,
		IMAGE_TYPE_COUNT,
		IMAGE_TYPE_UNKNOWN
	};

	// Global array of image extensions.
	extern const char * const gImageExtentions[];

	// ////////////////////////////////////////////////////////////////////
	// Given a image file type, return the image files typical file
	// extension.
	//
	// @return const char* const The image file types extension or an
	//								empty string if the file extension
	//								was not found in the global image ext
	//								lookup array.
	//
	// ////////////////////////////////////////////////////////////////////
	char const * const FindExtFromImageType(ImageType type);

	// ////////////////////////////////////////////////////////////////////
	// Given a filename determine the image type from the enum from the 
	// file extension.
	//
	// @param filenameRef The name of the file.
	//
	// @return imageType The type of the image file or IMAGE_TYPE_UNKNOWN
	//						if the file extension is not stored in the global
	//						file extension array.
	//
	// ////////////////////////////////////////////////////////////////////
	ImageType FindImageTypeFromFile(const std::string &filenameRef);
    
    // ////////////////////////////////////////////////////////////////////
    // Convert to ImageMode type from a string.
    //
    // ////////////////////////////////////////////////////////////////////
    ImageMode FindImageModeFromString(const std::string &imageModeRef);
    
	// ////////////////////////////////////////////////////////////////////
	// @class ImageResource
	// @author PJ O Halloran
	//
	// Data on a game image resource.
	//
	// ////////////////////////////////////////////////////////////////////
	class ImageResource : public Resource
	{
	public:

		// ////////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param name The filename of the resource.
		// 
		// ////////////////////////////////////////////////////////////////////
		ImageResource(const std::string &name);

		// ////////////////////////////////////////////////////////////////////
		// Return a handle to the resource.
		//
		// @param buffer A buffer holding the contents of the file in memory.
		// @param size The size of the memory buffer.
		// @param pResCache Pointer to the resource cache manager.
		//
		// @return ResHandle* A pointer to a resource handle for the file.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual ResHandle *VCreateHandle(const char *buffer, U32 size, ResCache *pResCache);

	};

	// ////////////////////////////////////////////////////////////////////
	// @class ImageResHandle
	// @author PJ O Halloran.
	//
	// Stores images loaded by the resource cache manager in a manner which
	// makes it easy to send the image through the OpenGL API as a texture.
	//
	// ////////////////////////////////////////////////////////////////////
	class ImageResHandle : public ResHandle
	{
	private:

		ImageType m_imageType;				///< is this a TGA, PNG, BMP, JPEG... ?
		bool m_initialized;					///< has the image been initialized
		bool m_fromFile;					///< are we reading from a file or a buffer?
		GLbyte *m_imageBuffer;				///< The parsed image data.
		GLint m_imageSize;					///< The size of the image data.
		GLint m_width;						///< The width of the image data.
		GLint m_height;						///< The height of the image data.
		GLint m_depth;						///< The depth of the image data.
		GLenum m_format;					///< The format of the image data.
		GLint m_components;					///< The number of components in the image data.
		std::string m_imageFilename;		///< The name of the file or resource

		// ////////////////////////////////////////////////////////////////////
		// Parse a TGA type image file from a file.
		//
		// Note: We only load 8, 24 or 32 bit RGB or RGBA TGA files at
		// present.
		// 
		// @param fd The file descriptor of the image file.
		//
		// @return bool True on success or false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		bool ParseTga(FILE *fd);

		// ////////////////////////////////////////////////////////////////////
		// Parse a BMP type image file from a file.
		// 
		// Note: We only allow 24 bit bitmaps to be loaded.
		//
		// @param fd The file descriptor of the image file.
		//
		// @return bool True on success or false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		bool ParseBmp(FILE *fd);

		// ////////////////////////////////////////////////////////////////////
		// Parse a PNG type image file from a file.
		// 
		// @param fd The file descriptor of the image file.
		//
		// @return bool True on success or false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		bool ParsePng(FILE *fd);

		// ////////////////////////////////////////////////////////////////////
		// Parse a JPEG type image file from a file.
		// 
		// @param fd The file descriptor of the image file.
		//
		// @return bool True on success or false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		bool ParseJpeg(FILE *fd);

		// ////////////////////////////////////////////////////////////////////
		// Parse a TGA type image file from a memory stream.
		//
		// Note: We only load 8, 24 or 32 bit RGB or RGBA TGA files at
		// present.
		//
		// @param tgaStream The image file stream in memory.
		// @param length The length of the image file stream.
		//
		// @return bool True on success or false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		bool ParseTga(char *tgaStream, const size_t length);

		// ////////////////////////////////////////////////////////////////////
		// Parse a BMP type image file from a memory stream.
		// 
		// Note: We only allow 24 bit bitmaps to be loaded.
		//
		// @param bmpStream The image file stream in memory.
		// @param length The length of the image file stream.
		//
		// @return bool True on success or false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		bool ParseBmp(char *bmpStream, const size_t length);

		// ////////////////////////////////////////////////////////////////////
		// Parse a PNG type image file from a memory stream.
		// 
		// @param pngStream The image file stream in memory.
		// @param length The length of the image file stream.
		//
		// @return bool True on success or false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		bool ParsePng(char *pngStream, const size_t length);

		// ////////////////////////////////////////////////////////////////////
		// Parse a JPEG type image file from a memory stream.
		// 
		// @param jpegStream The image file stream in memory.
		// @param length The length of the image file stream.
		//
		// @return bool True on success or false on failure.
		//
		// ////////////////////////////////////////////////////////////////////
		bool ParseJpeg(char *jpegStream, const size_t length);

	public:
		// ////////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param r The resource file.
		// @param buffer The image file loaded into memory.
		// @param size The size of the image buffer.
		// @param pResCache Pointer to the resource cache manager.
		//
		// ////////////////////////////////////////////////////////////////////
		ImageResHandle(Resource &r, char *buffer, const U32 size, ResCache *pResCache);

		// ////////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual ~ImageResHandle();

		// ////////////////////////////////////////////////////////////////////
		// Get the image buffer.
		//
		// ////////////////////////////////////////////////////////////////////
		const GLbyte *GetImageBuffer() const;

		// ////////////////////////////////////////////////////////////////////
		// Get the image buffer size.
		//
		// ////////////////////////////////////////////////////////////////////
		GLint GetImageSize() const;

		// ////////////////////////////////////////////////////////////////////
		// Get the image buffer/file type.
		//
		// ////////////////////////////////////////////////////////////////////
		ImageType GetImageType();

		// ////////////////////////////////////////////////////////////////////
		// Get image width.
		//
		// ////////////////////////////////////////////////////////////////////
		GLint GetImageWidth() const;

		// ////////////////////////////////////////////////////////////////////
		// Get image height.
		//
		// ////////////////////////////////////////////////////////////////////
		GLint GetImageHeight() const;

		// ////////////////////////////////////////////////////////////////////
		// Get image depth.
		//
		// ////////////////////////////////////////////////////////////////////
		GLint GetImageDepth() const;

		// ////////////////////////////////////////////////////////////////////
		// Get the number of image components.
		//
		// ////////////////////////////////////////////////////////////////////
		GLint GetImageComponents() const;

		// ////////////////////////////////////////////////////////////////////
		// Get the image format.
		//
		// ////////////////////////////////////////////////////////////////////
		GLenum GetImageFormat() const;

		// ////////////////////////////////////////////////////////////////////
		// Get the filename of the image.
		//
		// ////////////////////////////////////////////////////////////////////
		std::string GetImageFilename() const;

		// ////////////////////////////////////////////////////////////////////
		// Initializes the image depending on the image type.
		//
		// The initializing of the image is seperate to the constructor for two
		// reasons.
		// 1) If loading the Image fails, it gives more flexibility for error
		//		handling for the system if initialization is located outisde
		//		the constructor (usually the only error handling available
		//		to a developer in a constructor is to throw an exception).
		// 2) Image files are relatively large and a an init method gives more
		//		control over when they are loaded into RAM.
		//
		// @return bool True on success or false on failure.
		// 
		// ////////////////////////////////////////////////////////////////////
		virtual bool VInitialize();

	};
}

#endif
