// ////////////////////////////////////////////////////////////////////
// @file ImageResource.cpp
// @author PJ O Halloran
// @date 27/09/2010
//
// 
//
// ////////////////////////////////////////////////////////////////////

// External Headers


// Project Headers
#include "ImageResource.h"

#include "GLTools.h"

#include "GameMain.h"


namespace GameHalloran
{

	// Global array of image extensions.
	const char * const gImageExtentions[] = { ".tga", ".bmp", ".png", ".jpeg" };

	//////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////
	char const * const FindExtFromImageType(ImageType type)
	{
		// Global sound file extension lookup array bounds checking to make
		//  invalid array index access impossible.
		if(type >= IMAGE_TYPE_COUNT)
		{
			return ("");
		}

		return (gImageExtentions[type]);
	}

	//////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////
	ImageType FindImageTypeFromFile(const std::string &filenameRef)
	{
		if(filenameRef.empty())
		{
			return (IMAGE_TYPE_UNKNOWN);
		}

		I32 type = IMAGE_TYPE_FIRST;
		while (type < IMAGE_TYPE_COUNT)
		{
			if(strstr(filenameRef.c_str(), gImageExtentions[type]))
			{
				return (static_cast<ImageType>(type));
			}

			++type;
		}
		
		return (IMAGE_TYPE_UNKNOWN);
	}

	// ////////////////////////////////////////////////////////////////////
	// 
	// ////////////////////////////////////////////////////////////////////

	// ////////////////////////////////////////////////////////////////////
	// 
	// ////////////////////////////////////////////////////////////////////
	ImageResource::ImageResource(const std::string &name) : Resource(name)
	{
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	ResHandle *ImageResource::VCreateHandle(const char *buffer, U32 size, ResCache *pResCache)
	{
		// Cast away the const-ness of the buffer for the ImageResHandle.
		char *bufferNoConst = const_cast<char *>(buffer);
		return (GCC_NEW ImageResHandle(*this, bufferNoConst, size, pResCache)); 
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool ImageResHandle::ParseTga(FILE *fd)
	{
		TGAHEADER tgaHeader;				// TGA file header
	    
		if(fd == NULL)
		{
			return (false);
		}
		
		// Read in header (binary)
		//fread(&tgaHeader, sizeof(TGAHEADER), 1, fd);
		fread(&tgaHeader, 18, 1, fd);
	    
		// Put some validity checks here. Very simply, I only understand
		// or care about 8, 24, or 32 bit targa's.
		if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		{
			return (false);
		}

		// Do byte swap for big vs little endian on APPLE platforms.
		#ifdef __APPLE__
			LITTLE_ENDIAN_WORD(&tgaHeader.colorMapStart);
			LITTLE_ENDIAN_WORD(&tgaHeader.colorMapLength);
			LITTLE_ENDIAN_WORD(&tgaHeader.xstart);
			LITTLE_ENDIAN_WORD(&tgaHeader.ystart);
			LITTLE_ENDIAN_WORD(&tgaHeader.width);
			LITTLE_ENDIAN_WORD(&tgaHeader.height);
		#endif
		
		// Get width, height, and depth of texture
		m_width = tgaHeader.width;
		m_height = tgaHeader.height;
		m_depth = tgaHeader.bits / 8;
		
		// Calculate size of image buffer
		m_imageSize = m_width * m_height * m_depth;
	    
		// Allocate memory and check for success
		m_imageBuffer = GCC_NEW GLbyte[m_imageSize];
		if(m_imageBuffer == NULL)
		{
			return (false);
		}

		// Read in the bits
		// Check for read error. This should catch RLE or other 
		// weird formats that I don't want to recognize
		if(fread(m_imageBuffer, m_imageSize, 1, fd) != 1)
		{
			DeleteArray(m_imageBuffer);
			return (false);
		}
	    
		// Set OpenGL format expected
		switch(m_depth)
		{
			#ifndef OPENGL_ES
			case 3:     // Most likely case
				m_format = GL_BGR;
				m_components = GL_RGB;
				break;
			#endif
			case 4:
				m_format = GL_BGRA;
				m_components = GL_RGBA;
				break;
			case 1:
				m_format = GL_LUMINANCE;
				m_components = GL_LUMINANCE;
				break;
			default:        // RGB
				// If on the iPhone, TGA's are BGR, and the iPhone does not 
				// support BGR without alpha, but it does support RGB,
				// so a simple swizzle of the red and blue bytes will suffice.
				// For faster iPhone loads however, save your TGA's with an Alpha!
				#ifdef OPENGL_ES
					for(I32 i = 0; i < m_imageSize; i+=3)
					{
						GLbyte temp = m_imageBuffer[i];
						m_imageBuffer[i] = m_imageBuffer[i+2];
						m_imageBuffer[i+2] = temp;
					}
				#endif
			break;
		}

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool ImageResHandle::ParseBmp(FILE *fd)
	{
		BMPInfo *pBitmapInfo = NULL;				/// BMP information.
		U64 lInfoSize = 0;				/// Size of BMP information.
		BMPHeader bitmapHeader;						/// BMP Header

		if(fd == NULL)
		{
			return (false);
		}

		// File is Open. Read in bitmap header information
		fread(&bitmapHeader, sizeof(BMPHeader), 1, fd);

		// Read in bitmap information structure
		lInfoSize = bitmapHeader.offset - sizeof(BMPHeader);
		pBitmapInfo = (BMPInfo *) malloc(sizeof(GLbyte)*lInfoSize);
		if(fread(pBitmapInfo, lInfoSize, 1, fd) != 1)
		{
			free(pBitmapInfo);
			return (false);
		}

		// Save the size and dimensions of the bitmap
		m_width = pBitmapInfo->header.width;
		m_height = pBitmapInfo->header.height;
		m_imageSize = pBitmapInfo->header.imageSize;

		// If the size isn't specified, calculate it anyway	
		if(pBitmapInfo->header.bits != 24)
		{
			free(pBitmapInfo);
			return (false);
		}
		if(m_imageSize == 0)
		{
			m_imageSize = (m_width * pBitmapInfo->header.bits + 7) / 8 * abs(m_height);
		}

		free(pBitmapInfo);

		// Allocate space for the actual bitmap
		m_imageBuffer = GCC_NEW GLbyte[m_imageSize];

		// Read in the bitmap bits, check for corruption
		if(fread(m_imageBuffer, m_imageSize, 1, fd) != 1)
		{
			DeleteArray(m_imageBuffer);
		}

		m_components = GL_RGB;
		m_format = GL_BGR;

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool ImageResHandle::ParsePng(FILE *fd)
	{
		// TODO: 
		return (false);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool ImageResHandle::ParseJpeg(FILE *fd)
	{
		// TODO: 
		return (false);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool ImageResHandle::ParseTga(char *tgaStream, const size_t length)
	{
		TGAHEADER tgaHeader;				// TGA file header
	    size_t pos = 0;						// Stream pointer.

		//if(length < sizeof(TGAHEADER) || tgaStream == NULL)
		if(length < 18 || tgaStream == NULL)
		{
			return (false);
		}
		
		// Read in header.
		tgaHeader.identsize = *(reinterpret_cast<GLbyte *>(tgaStream + pos));
		pos += sizeof(tgaHeader.identsize);
		tgaHeader.colorMapType = *(reinterpret_cast<GLbyte *>(tgaStream + pos));
		pos += sizeof(tgaHeader.colorMapType);
		tgaHeader.imageType = *(reinterpret_cast<GLbyte *>(tgaStream + pos));
		pos += sizeof(tgaHeader.imageType);
		tgaHeader.colorMapStart = *(reinterpret_cast<U16 *>(tgaStream + pos));
		pos += sizeof(tgaHeader.colorMapStart);
		tgaHeader.colorMapLength = *(reinterpret_cast<U16 *>(tgaStream + pos));
		pos += sizeof(tgaHeader.colorMapLength);
		tgaHeader.colorMapBits = *(reinterpret_cast<unsigned char *>(tgaStream + pos));
		pos += sizeof(tgaHeader.colorMapBits);
		tgaHeader.xstart = *(reinterpret_cast<U16 *>(tgaStream + pos));
		pos += sizeof(tgaHeader.xstart);
		tgaHeader.ystart = *(reinterpret_cast<U16 *>(tgaStream + pos));
		pos += sizeof(tgaHeader.ystart);
		tgaHeader.width = *(reinterpret_cast<U16 *>(tgaStream + pos));
		pos += sizeof(tgaHeader.width);
		tgaHeader.height = *(reinterpret_cast<U16 *>(tgaStream + pos));
		pos += sizeof(tgaHeader.height);
		tgaHeader.bits = *(reinterpret_cast<GLbyte *>(tgaStream + pos));
		pos += sizeof(tgaHeader.bits);
		tgaHeader.descriptor = *(reinterpret_cast<GLbyte *>(tgaStream + pos));
		pos += sizeof(tgaHeader.descriptor);	// Now we should be pointing at the beginning of the image data...
	    
		// Put some validity checks here. Very simply, I only understand
		// or care about 8, 24, or 32 bit targa's.
		if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		{
			return (false);
		}

		// Do byte swap for big vs little endian on APPLE platforms.
		#ifdef __APPLE__
			LITTLE_ENDIAN_WORD(&tgaHeader.colorMapStart);
			LITTLE_ENDIAN_WORD(&tgaHeader.colorMapLength);
			LITTLE_ENDIAN_WORD(&tgaHeader.xstart);
			LITTLE_ENDIAN_WORD(&tgaHeader.ystart);
			LITTLE_ENDIAN_WORD(&tgaHeader.width);
			LITTLE_ENDIAN_WORD(&tgaHeader.height);
		#endif
		
		// Get width, height, and depth of texture
		m_width = tgaHeader.width;
		m_height = tgaHeader.height;
		m_depth = tgaHeader.bits / 8;
		
		// Calculate size of image buffer
		m_imageSize = m_width * m_height * m_depth;

		//// Sanity check before we allocate the buffer!!
		//if(size_t(m_imageSize) + pos + 1 != length)
		//{
		//	return (false);
		//}
	    
		// Allocate memory and check for success
		m_imageBuffer = GCC_NEW GLbyte[m_imageSize];
		if(m_imageBuffer == NULL)
		{
			return (false);
		}

		// Read in the bits into the buffer.
		memcpy(m_imageBuffer, tgaStream + pos, m_imageSize);
	    
		// Set OpenGL format expected
		switch(m_depth)
		{
			#ifndef OPENGL_ES
			case 3:     // Most likely case
				m_format = GL_BGR;
				m_components = GL_RGB;
				break;
			#endif
			case 4:
				m_format = GL_BGRA;
				m_components = GL_RGBA;
				break;
			case 1:
				m_format = GL_LUMINANCE;
				m_components = GL_LUMINANCE;
				break;
			default:        // RGB
				// If on the iPhone, TGA's are BGR, and the iPhone does not 
				// support BGR without alpha, but it does support RGB,
				// so a simple swizzle of the red and blue bytes will suffice.
				// For faster iPhone loads however, save your TGA's with an Alpha!
				#ifdef OPENGL_ES
					for(I32 i = 0; i < m_imageSize; i+=3)
					{
						GLbyte temp = m_imageBuffer[i];
						m_imageBuffer[i] = m_imageBuffer[i+2];
						m_imageBuffer[i+2] = temp;
					}
				#endif
			break;
		}

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool ImageResHandle::ParseBmp(char *bmpStream, const size_t length)
	{
		BMPInfo *pBitmapInfo = NULL;				// BMP information.
		U64 lInfoSize = 0;				// Size of BMP information.
		BMPHeader bitmapHeader;						// BMP Header
		size_t pos = 0;								// Stream pointer.

		if(bmpStream == NULL || length < sizeof(BMPHeader))
		{
			return (false);
		}

		// Read in bitmap header information.
		bitmapHeader.type = *(reinterpret_cast<GLushort *>(bmpStream + pos));
		pos += sizeof(bitmapHeader.type);
		bitmapHeader.size = *(reinterpret_cast<GLuint *>(bmpStream + pos));
		pos += sizeof(bitmapHeader.size);
		bitmapHeader.unused = *(reinterpret_cast<GLushort *>(bmpStream + pos));
		pos += sizeof(bitmapHeader.unused);
		bitmapHeader.unused2 = *(reinterpret_cast<GLushort *>(bmpStream + pos));
		pos += sizeof(bitmapHeader.unused2);
		bitmapHeader.offset = *(reinterpret_cast<GLuint *>(bmpStream + pos));
		pos += sizeof(bitmapHeader.offset);

		// Read in bitmap information structure
		lInfoSize = bitmapHeader.offset - sizeof(BMPHeader);
		pBitmapInfo = (BMPInfo *) malloc(sizeof(GLbyte)*lInfoSize);
		if(pBitmapInfo == NULL)
		{
			return (false);
		}
		memcpy(pBitmapInfo, bmpStream + pos, lInfoSize);
		pos += size_t(lInfoSize);

		// Save the size and dimensions of the bitmap
		m_width = pBitmapInfo->header.width;
		m_height = pBitmapInfo->header.height;
		m_imageSize = pBitmapInfo->header.imageSize;

		// If the size isn't specified, calculate it anyway	
		if(pBitmapInfo->header.bits != 24)
		{
			free(pBitmapInfo);
			return (false);
		}
		if(m_imageSize == 0)
		{
			m_imageSize = (m_width * pBitmapInfo->header.bits + 7) / 8 * abs(m_height);
		}

		free(pBitmapInfo);

		//// Sanity check before we allocate the buffer!
		//if(size_t(m_imageSize) + pos + 1 != length)
		//{
		//	return (false);
		//}

		// Allocate space for the actual bitmap
		m_imageBuffer = GCC_NEW GLbyte[m_imageSize];

		// Read in the bitmap bits
		memcpy(m_imageBuffer, bmpStream + pos, m_imageSize);

		m_components = GL_RGB;
		m_format = GL_BGR;

		return (true);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool ImageResHandle::ParsePng(char *pngStream, const size_t length)
	{
		// TODO: 
		return (false);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	bool ImageResHandle::ParseJpeg(char *jpegStream, const size_t length)
	{
		// TODO: 
		return (false);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	ImageResHandle::ImageResHandle(Resource &r, char *buffer, const U32 size, ResCache *pResCache)\
		: ResHandle(r, buffer, size, pResCache), m_imageType(IMAGE_TYPE_UNKNOWN), m_initialized(false), m_fromFile(false),\
			m_imageBuffer(NULL), m_imageSize(0), m_width(0), m_height(0), m_depth(0), m_format(GL_RGB), m_components(GL_RGB),\
				m_imageFilename(r.GetName())
	{
		m_fromFile = (buffer == NULL);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	ImageResHandle::~ImageResHandle()
	{
		// Note - since ImageResources have a lifetime controlled by shared_ptr, 
		// it's safe for us to nuke the memory without checking first.
		try
		{
            GF_LOG_DEB(std::string("Freeing the image resource ") + m_imageFilename);
//			SafeGameLog(g_appPtr->GetLoggerPtr(), GameLog::DEB, std::string("Freeing the image resource ") + m_imageFilename);
			DeleteArray(m_imageBuffer);
		}
		catch(...)
		{
		}
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	const GLbyte *ImageResHandle::GetImageBuffer() const
	{
		return (m_imageBuffer);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	GLint ImageResHandle::GetImageSize() const
	{
		return (m_imageSize);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	ImageType ImageResHandle::GetImageType()
	{
		return (m_imageType);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	GLint ImageResHandle::GetImageWidth() const
	{
		return (m_width);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	GLint ImageResHandle::GetImageHeight() const
	{
		return (m_height);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	GLint ImageResHandle::GetImageDepth() const
	{
		return (m_depth);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	GLint ImageResHandle::GetImageComponents() const
	{
		return (m_components);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	GLenum ImageResHandle::GetImageFormat() const
	{
		return (m_format);
	}

	// ////////////////////////////////////////////////////////////////////
	//
	// ////////////////////////////////////////////////////////////////////
	std::string ImageResHandle::GetImageFilename() const
	{
		return (m_imageFilename);
	}

	// ////////////////////////////////////////////////////////////////////
	// 
	// ////////////////////////////////////////////////////////////////////
	bool ImageResHandle::VInitialize()
	{
		bool result = true;

		if (!m_initialized)
		{
			m_imageType = FindImageTypeFromFile(m_imageFilename);
			if (m_fromFile)
			{
				FILE *file = NULL;
				file = fopen(m_imageFilename.c_str(), "rb");
				if(file == NULL)
				{
					result = false;
				}

				if(result)
				{
					switch (m_imageType)
					{
						case IMAGE_TYPE_TGA:
							result = ParseTga(file);
							break;

						case IMAGE_TYPE_BMP:
							result = ParseBmp(file);
							break;

						case IMAGE_TYPE_PNG:
						case IMAGE_TYPE_JPEG:
							// TODO:
						default:
                            GF_LOG_ERR("Image Type Not Supported");
//							SafeGameLog(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("Image Type Not Supported"));
							result = false;
							break;
					}

					fclose(file);
				}
			}
			else 
			{
				// initializing from a memory buffer
				switch (m_imageType)
				{
					case IMAGE_TYPE_TGA:
						result = ParseTga(ResHandle::Buffer(), ResHandle::Size());
						break;

					case IMAGE_TYPE_BMP:
						result = ParseBmp(ResHandle::Buffer(), ResHandle::Size());
						break;

					case IMAGE_TYPE_PNG:
					case IMAGE_TYPE_JPEG:
						// TODO:
					default:
						result = false;
                        GF_LOG_ERR("Image Type Not Supported");
//						SafeGameLog(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("Image Type Not Supported"));
						break;
				}

			}
			m_initialized = true;
		}

		return (result);
	}

}
