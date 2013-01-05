#pragma once
#ifndef _GF_TEXT_RESOURCE_H
#define _GF_TEXT_RESOURCE_H

// ////////////////////////////////////////////////////////////////////
// @file TextResource.h
// @author PJ O Halloran
// @date 28/09/2010
//
// Classes for text resources.
//
// ////////////////////////////////////////////////////////////////////

#include <string>
#include <functional>

#include "ResCache2.h"

namespace GameHalloran
{

	// ////////////////////////////////////////////////////////////////////
	// @class TextResource
	// @author PJ O Halloran
	//
	// Data on a game text resource.
	//
	// ////////////////////////////////////////////////////////////////////
	class TextResource : public Resource
	{
	public:

		// ////////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param name The filename of the resource.
		// 
		// ////////////////////////////////////////////////////////////////////
		TextResource(const std::string &name);

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
	// @class TextResHandle
	// @author PJ O Halloran.
	//
	// Stores text files loaded by the resource cache manager.
	//
	// ////////////////////////////////////////////////////////////////////
	class TextResHandle : public ResHandle
	{
	private:

		char *m_textBuffer;				///< The parsed text data.
		U32 m_textSize;					///< The size of the text data.
		std::string m_filename;			///< The name of the file or resource.
		bool m_initialized;				///< Has the resource been initialized with proper data yet from the resource cache?

	public:

		// ////////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param r The resource file.
		// @param buffer The text file loaded into memory.
		// @param size The size of the text buffer.
		// @param pResCache Pointer to the resource cache manager.
		//
		// ////////////////////////////////////////////////////////////////////
		TextResHandle(Resource &r, char *buffer, const U32 size, ResCache *pResCache);

		// ////////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// ////////////////////////////////////////////////////////////////////
		virtual ~TextResHandle();

		// ////////////////////////////////////////////////////////////////////
		// Get the text buffer.
		//
		// ////////////////////////////////////////////////////////////////////
		const char *GetTextBuffer() const;

		// ////////////////////////////////////////////////////////////////////
		// Get the text buffer size.
		//
		// ////////////////////////////////////////////////////////////////////
		U32 GetTextSize() const;

		// ////////////////////////////////////////////////////////////////////
		// Get the filename.
		//
		// ////////////////////////////////////////////////////////////////////
		std::string GetFilename() const;

		// ////////////////////////////////////////////////////////////////////
		// Empty method. All initialization for text files loaded in from the
		// resource cache is done in the constructor.
		// 
		// ////////////////////////////////////////////////////////////////////
		virtual bool VInitialize();

	};

	// /////////////////////////////////////////////////////////////////
	// @class RemoveTrailingCr
	// @author PJ O Halloran
	//
	// Functor object that removes a trailing "cr" character (ascii code
	// of 13) from a string.
	//
	// Text files stored in the resource cache may have a trailing "cr"
	// character appended to each line during compression of the file.
	// Most of the time this does not matter but if it interferes with
	// some task you are trying to do then copy all the lines into a
	// vector<std::string> and do:
	//		RemoveTrailingCr fo;
	//		std::for_each(vec.begin(), vec.end(), fo);
	//
	// /////////////////////////////////////////////////////////////////
	class RemoveTrailingCr : public std::unary_function<std::string&, void>
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Functor callback.
		//
		// /////////////////////////////////////////////////////////////////
		void operator()(std::string &arg)
		{
			if(arg.empty())
			{
				return;
			}

			if(I32(arg[arg.size()-1]) == 13)
			{
				arg.assign(arg.begin(), arg.end()-1);
			}
		};
	};
}

#endif
