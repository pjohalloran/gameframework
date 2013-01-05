#pragma once
#ifndef __ZIP_FILE_H
#define __ZIP_FILE_H

//========================================================================
// ZipFile.h : API to use Zip files
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
// @author Javier Arevalo, Michael L. McShaffry and PJ O Halloran.
// @date 12/07/2010
//
// File contains the header for the ZipFile class.  
// This class was extracted from the Game Coding Complete 3 code.
// 
// The declaration of a quick'n dirty ZIP file reader class.
// Original code from Javier Arevalo.
// Get zlib from http://www.cdrom.com/pub/infozip/zlib/
//
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// I have added/modified the following functionality:
// - Added under the GameHalloran namespace.
// - Using boost::optional instead of M. McShaffrys' version.
// - Made it platform independant.
//
// /////////////////////////////////////////////////////////////////

#include <cstdio>
#include <string>
#include <map>

#include <boost/optional.hpp>
#include <boost/filesystem.hpp>

#include "GameBase.h"

namespace GameHalloran
{
    
	// /////////////////////////////////////////////////////////////////
	// @class ZipFile
	// @author Javier Arevalo and Michael L. McShaffry.
	//
	// Class for opening, reading and writing a ZIP file.
	//
	// /////////////////////////////////////////////////////////////////
	class ZipFile
	{
    private:
        
        struct TZipLocalHeader;
        struct TZipDirHeader;
        struct TZipDirFileHeader;
        
        // /////////////////////////////////////////////////////////////////
        // Read in the main file directory header.
        //
        // @param headerPtr The struct to store the header parameters.
        //
        // @return bool True|False on success|failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool ReadDirHeader(TZipDirHeader * const headerPtr, I64 &offset);
        
        // /////////////////////////////////////////////////////////////////
        // Read in the main file directory header at the end of the file.
        //
        // @param headerPtr
        // @param offset The offset to the header from the beginning of the
        //                  file.
        //
        // @return bool True|False on success|failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool ReadDirFileHeader(TZipDirFileHeader * const headerPtr, const I64 offset);
        
        // /////////////////////////////////////////////////////////////////
        // Read in the local file header.
        //
        // @param headerPtr
        // @param offset The offset to the header from the beginning of the
        //                  file.
        //
        // @return bool True|False on success|failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool ReadLocalHeader(TZipLocalHeader * const headerPtr, const I64 offset);
        
	public:

        static const std::string ZIP_PATH_SEPERATOR;
        
		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		ZipFile();
        
		// /////////////////////////////////////////////////////////////////
		// Constructor.
        //
        // @param resFileName The filename of the zip file to load.
		//
		// /////////////////////////////////////////////////////////////////
		ZipFile(const boost::filesystem::path &resFileName);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~ZipFile();

		// /////////////////////////////////////////////////////////////////
		// Open the zip directory with the path supplied.
		//
		// @param resFileName The filename of the zip file to load.
		//
		// @return bool True on success and false otherwise.
		//
		// /////////////////////////////////////////////////////////////////
		bool Init(const boost::filesystem::path &resFileName);

		// /////////////////////////////////////////////////////////////////
		// Close the file.
		//
		// /////////////////////////////////////////////////////////////////
		void End();

		// /////////////////////////////////////////////////////////////////
		// Get the number of files packed into the zip file.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetNumFiles() const;

		// /////////////////////////////////////////////////////////////////
		// Get the filename of the file at index i.
		//
		// @param i The index of the file.
		// @param path The filename of the file at index i.
		//
		// /////////////////////////////////////////////////////////////////
		bool GetFilename(const U32 i, boost::filesystem::path &path) const;

		// /////////////////////////////////////////////////////////////////
		// Get the length of the file at index i.
		//
		// @param i The index of the file.
        //
        // @return bool True|False on success|failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool GetFileLen(const U32 i, U64 &fileLen) const;

		// /////////////////////////////////////////////////////////////////
		// Read the file at index i and uncompress it.
		//
		// @param i The index of the file.
		// @param pBuf The pointer to the file buffer.
		//
		// @return bool 
		//
		// /////////////////////////////////////////////////////////////////
		bool ReadFile(const U32 i, void *pBuf);
        
		// /////////////////////////////////////////////////////////////////
		// Write the file to the end of the buffer.
		//
		// @param compress Should the file be compressedin the ZIP?
		// @param pBuf The pointer to the file buffer.
		//
		// @return bool 
		//
		// /////////////////////////////////////////////////////////////////
		bool WriteFile(const bool compress, const void * const pBuf);

		// /////////////////////////////////////////////////////////////////
		// Find the index of the file with the filename supplied.
		//
		// @param path The filename of the file to find.
		//
		// @return boost:optional<int> The index of the file, if it was found.
		//
		// /////////////////////////////////////////////////////////////////
		boost::optional<I32> Find(const boost::filesystem::path &filename) const;

    private:

		typedef std::map<std::string, I32> ZipContentsMap;

		FILE *m_pFile;										///< Zip file
		char *m_pDirData;									///< Raw data buffer.
		U32  m_nEntries;									///< Number of entries.
		ZipContentsMap m_ZipContentsMap;					///< The contents of the zip file.
		const TZipDirFileHeader **m_papDir;					///< Pointers to the dir entries in pDirData.
        std::vector<TZipDirFileHeader> m_localVec;
	};

}

#endif
