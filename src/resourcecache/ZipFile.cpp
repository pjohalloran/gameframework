//========================================================================
// ZipFile.cpp : API to use Zip files
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

// /////////////////////////////////////////////////////////////////
// @file ZipFile.h
// @author Javier Arevalo and Michael L. McShaffry.
// @date 12/07/2010
//
// File contains the implementation of the ZipFile class.  
// This class was extracted from the Game Coding Complete 3 code.
// 
// The declaration of a quick'n dirty ZIP file reader class.
// Original code from Javier Arevalo.
// Get zlib from http://www.cdrom.com/pub/infozip/zlib/
//
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// Please see the header file for details on any changes I have made.
//
// /////////////////////////////////////////////////////////////////

#include <string.h>
#include <regex>

#include <boost/algorithm/string/case_conv.hpp>
#include "zlib/zlib.h"

#include "GameMain.h"
#include "GameLog.h"
#include "ZipFile.h"

using boost::filesystem::path;
using boost::optional;

// --------------------------------------------------------------------------
// ZIP file structures. Note these have to be packed (on windows!).
// --------------------------------------------------------------------------
#pragma pack(1)
struct GameHalloran::ZipFile::TZipLocalHeader
{
    enum
    {
        SIGNATURE = 0x04034b50,
        SIZE = sizeof(DWORD)*4 + sizeof(WORD)*7
    };
    DWORD sig;
    WORD version;
    WORD flag;
    WORD compression;      // Z_NO_COMPRESSION or Z_DEFLATED
    WORD modTime;
    WORD modDate;
    DWORD crc32;
    DWORD cSize;
    DWORD ucSize;
    WORD fnameLen;         // Filename string follows header.
    WORD xtraLen;          // Extra field follows filename.
};

struct GameHalloran::ZipFile::TZipDirHeader
{
    enum
    {
        SIGNATURE = 0x06054b50,
        SIZE = sizeof(DWORD)*3 + sizeof(WORD)*5
    };
    DWORD sig;
    WORD nDisk;
    WORD nStartDisk;
    WORD nDirEntries;
    WORD totalDirEntries;
    DWORD dirSize;
    DWORD dirOffset;
    WORD cmntLen;
};

struct GameHalloran::ZipFile::TZipDirFileHeader
{
    enum
    {
        SIGNATURE   = 0x02014b50,
        // NB Size here does not take into account extra padding info at the end of the header..
        SIZE = sizeof(DWORD)*6 + sizeof(WORD)*11
    };
    DWORD sig;
    WORD verMade;
    WORD verNeeded;
    WORD flag;
    WORD compression;      // COMP_xxxx
    WORD modTime;
    WORD modDate;
    DWORD crc32;
    DWORD cSize;            // Compressed size
    DWORD ucSize;           // Uncompressed size
    WORD fnameLen;         // Filename string follows header.
    WORD xtraLen;          // Extra field follows filename.
    WORD cmntLen;          // Comment field follows extra field.
    WORD diskStart;
    WORD intAttr;
    DWORD extAttr;
    DWORD hdrOffset;

    inline char *GetName() const { return (char *)(this + 1); };
    inline char *GetExtra() const { return GetName() + fnameLen; };
    inline char *GetComment() const { return GetExtra() + xtraLen; };
};

#pragma pack()
//#pragma push(pop)

namespace GameHalloran
{
    
	const std::string ZipFile::ZIP_PATH_SEPERATOR("/");
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ZipFile::ReadDirHeader(TZipDirHeader * const headerPtr, I64 &offset)
    {
        if(!headerPtr || !m_pFile)
        {
            GF_LOG_TRACE_ERR("ZipFile::ReadDirHeader()", "Invalid parameters");
            return (false);
        }
        
        I32 tmp = TZipDirHeader::SIZE;
        memset(headerPtr, 0, tmp);
        
        fseek(m_pFile, -TZipDirHeader::SIZE, SEEK_END);
        
        // Save the offset to the Dir Header.
        offset = ftell(m_pFile);

        fread(headerPtr, TZipDirHeader::SIZE, 1, m_pFile);
        
        if(headerPtr->sig != TZipDirHeader::SIGNATURE)
        {
            GF_LOG_TRACE_ERR("ZipFile::ReadDirHeader()", "Invalid TZipDirHeader signature encountered");
            return (false);
        }
        
        return (true);
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ZipFile::ReadDirFileHeader(TZipDirFileHeader * const headerPtr, const I64 offset)
    {
		return (false);
    }
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ZipFile::ReadLocalHeader(TZipLocalHeader * const headerPtr, const I64 offset)
    {
        if(!headerPtr)
        {
            GF_LOG_TRACE_ERR("ZipFile::ReadLocalHeader()", "Invalid parameters");
            return (false);
        }
        
        // Navigate to the byte offset in the file where the local header is stored.
        fseek(m_pFile, offset, SEEK_SET);
        
        memset(headerPtr, 0, TZipLocalHeader::SIZE);
        fread(headerPtr, TZipLocalHeader::SIZE, 1, m_pFile);
        
        if (headerPtr->sig != TZipLocalHeader::SIGNATURE)
        {
            GF_LOG_TRACE_ERR("ZipFile::ReadLocalHeader()", "Local ZIP Header signature is invalid");
            return (false);
        }
        
        return (true);
    }
    
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ZipFile::ZipFile()
			: m_pFile(NULL)
			, m_pDirData(NULL)
			, m_nEntries(0)
			, m_ZipContentsMap()
			, m_papDir(NULL)
			, m_localVec()
	{
	}
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    ZipFile::ZipFile(const path &resFileName)
			: m_pFile(NULL)
			, m_pDirData(NULL)
			, m_nEntries(0)
			, m_ZipContentsMap()
			, m_papDir(NULL)
			, m_localVec()
	{
        Init(resFileName);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ZipFile::~ZipFile()
	{
		try
		{
			End();
		}
		catch(...)
		{
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	I32 ZipFile::GetNumFiles() const
	{
		return (m_nEntries);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ZipFile::Init(const path &resFileName)
	{
        End();

        m_pFile = fopen(resFileName.string().c_str(), "rb");
        if (!m_pFile)
        {
            GF_LOG_TRACE_ERR("ZipFile::Init()", std::string("Failed to open zip file ") + resFileName.string());
            return (false);
        }
        
        TZipDirHeader dirHeader;
        I64 dirOffset;
        if(!ReadDirHeader(&dirHeader, dirOffset))
        {
            return (false);
        }
        
        m_localVec.reserve(dirHeader.nDirEntries);

        // Go to the beginning of the directory.
        fseek(m_pFile, dirOffset - dirHeader.dirSize, SEEK_SET);

        const U64 size(dirHeader.dirSize + dirHeader.nDirEntries*sizeof(*m_papDir)); 
        
        // Allocate the data buffer, and read the whole thing.
        m_pDirData = GCC_NEW char[size];
        if (!m_pDirData)
        {
            GF_LOG_TRACE_ERR("ZipFile::Init()", "Failed to allocate ZIP data buffer");
            return (false);
        }
        memset(m_pDirData, 0, size);
        fread(m_pDirData, dirHeader.dirSize, 1, m_pFile);

        // Now process each entry.
        char *pfh = m_pDirData;
        m_papDir = (const TZipDirFileHeader **)(m_pDirData + dirHeader.dirSize);

        bool success = true;

        for (I32 i = 0; ((i < dirHeader.nDirEntries) && (success)); ++i)
        {
            TZipDirFileHeader &fh = *(TZipDirFileHeader*)pfh;

            m_localVec.push_back(fh);
            
            // Store the address of nth file for quicker access.
            //m_papDir[i] = &fh;
            m_papDir[i] = (TZipDirFileHeader*)pfh;

            
            // Check the directory entry integrity (33639248).
            if (fh.sig != TZipDirFileHeader::SIGNATURE)
            {
                success = false;
            }
            else
            {
                pfh += TZipDirFileHeader::SIZE;

                char fileName[255];
                memcpy(fileName, pfh, fh.fnameLen);
                fileName[fh.fnameLen]=0;

                std::string spath = fileName;
                boost::algorithm::to_lower(spath);
                
                m_ZipContentsMap[spath] = i;

                // Skip name, extra and comment fields.
                pfh += fh.fnameLen + fh.xtraLen + fh.cmntLen;
            }
        }
        
        if (!success)
        {
            DeleteArray(m_pDirData);
        }
        else
        {
            m_nEntries = dirHeader.nDirEntries;
        }

        return success;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	optional<I32> ZipFile::Find(const path &path) const
	{
        if(path.empty())
        {
            return (optional<I32>());
        }
        
        std::string lwrPath(path.string());
        boost::algorithm::to_lower(lwrPath);
		ZipContentsMap::const_iterator i = m_ZipContentsMap.find(lwrPath);
		if (i==m_ZipContentsMap.end())
        {
			return optional<I32>();
        }

		return (*i).second;
	}
    
    bool ZipFile::Find(const std::string &filePattern, std::vector<boost::filesystem::path> &fileList) {
        return false;
    }

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ZipFile::End()
	{
		m_ZipContentsMap.empty();
		DeleteArray(m_pDirData);
		m_nEntries = 0;
        if(m_pFile)
        {
            fclose(m_pFile);
            m_pFile = NULL;
        }
        
        m_localVec.clear();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ZipFile::GetFilename(const U32 i, path &pathRef) const
	{
        if(i >= m_nEntries)
        {
            GF_LOG_TRACE_ERR("ZipFile::GetFilename()", "ZIP index out of bounds");
            return (false);
        }
		
        std::string filename(m_localVec[i].GetName());
        pathRef = path(filename);
        return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ZipFile::GetFileLen(const U32 i, U64 &fileLen) const
	{
        if (i >= m_nEntries)
        {
            GF_LOG_TRACE_ERR("ZipFile::GetFileLen()", "ZIP index out of bounds");
            return (false);
        }
		
        fileLen = m_localVec[i].ucSize;
        return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ZipFile::ReadFile(const U32 i, void *pBuf)
	{
        if ((pBuf == NULL) || (i >= m_nEntries))
        {
            GF_LOG_TRACE_ERR("ZipFile::ReadFile()", "Invalid ReadFile parameters");
            return (false);
        }

        TZipDirFileHeader tmp = m_localVec[i];
        TZipLocalHeader h;
        if(!ReadLocalHeader(&h, m_localVec[i].hdrOffset))
        {
            return (false);
        }
        
        // Skip extra fields
        fseek(m_pFile, h.fnameLen + h.xtraLen, SEEK_CUR);
        
        if (h.compression == Z_NO_COMPRESSION)
        {
            // Simply read in raw stored data.
            fread(pBuf, h.cSize, 1, m_pFile);
            return (true);
        }
        else if (h.compression != Z_DEFLATED)
        {
            GF_LOG_TRACE_ERR("ZipFile::ReadFile()", "Unable to handle non Z_DEFLATED compressed data ZIP fil");
            return (false);
        }

        I32 cSize = (h.cSize != 0 ? h.cSize : ( tmp.cSize != 0 ? tmp.cSize : -1 ));
        I32 ucSize = (h.ucSize != 0 ? h.ucSize : ( tmp.ucSize != 0 ? tmp.ucSize : -1 ));
        
        // Alloc compressed data buffer and read the whole stream
        char *pcData = GCC_NEW char[cSize];
        if (!pcData)
        {
            GF_LOG_TRACE_ERR("ZipFile::ReadFile()", "Failed to allocate memory for local ZIP file");
            return (false);
        }
        
        memset(pcData, 0, cSize);
        fread(pcData, cSize, 1, m_pFile);

        // Setup the inflate stream.
        z_stream stream;
        I32 err = 0;

        stream.next_in = (Bytef*)pcData;
        stream.avail_in = (uInt)cSize;
        stream.next_out = (Bytef*)pBuf;
        stream.avail_out = (uInt)ucSize;
        stream.zalloc = (alloc_func)0;
        stream.zfree = (free_func)0;

        // Perform inflation. wbits < 0 indicates no zlib header inside the data.
        err = inflateInit2(&stream, -MAX_WBITS);
        if (err == Z_OK)
        {
            err = inflate(&stream, Z_FINISH);
            inflateEnd(&stream);
            if (err == Z_STREAM_END)
            {
                err = Z_OK;
            }
            inflateEnd(&stream);
        }

        DeleteArray(pcData);
        return (err == Z_OK);
	}
    
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ZipFile::WriteFile(const bool compress, const void * const pBuf)
    {
        GF_LOG_TRACE_ERR("ZipFile::WriteFile()", "Not yet implemented...");
        return (false);
    }

}
