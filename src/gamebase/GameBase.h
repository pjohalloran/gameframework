#ifndef __GAME_BASE_H
#define __GAME_BASE_H

// /////////////////////////////////////////////////////////////////
// @file GameBase.h
// @author PJ O Halloran
// @date 06/07/2010
//
// File for base game defines and utility functions.
//
// /////////////////////////////////////////////////////////////////


// STD C/C++ Headers
#include <string>
#include <new>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "GameTypes.h"
#include "GamePlatform.h"
#include "GameLog.h"

#include <boost/shared_ptr.hpp>
#include <memory>

// Define the version of new we are going to use depending on the build mode.
#if _WINDOWS && (defined(_DEBUG) || defined(DEBUG))
	#define GCC_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
	#define GCC_NEW new
#endif


// Global includes for removing some unnessecary and/or risky functionality from the builds for submission to games companies.
// Comment out for test builds!
#define GF_DEMO_BUILD


namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// Clean up memory dynamically allocated with the new operator.
	//
	// This method is exception safe.
	//
	// @param ptr A pointer to a type dynamically allocated with new.
	//
	// /////////////////////////////////////////////////////////////////
	template<class T>
		void Delete(T *ptr) throw ()
	{
		if(ptr)
		{
			delete ptr;
			ptr = NULL;
		}
	};

	// /////////////////////////////////////////////////////////////////
	// Clean up an array dynamically allocated with the new[] operator.
	//
	// This method is exception safe.
	//
	// @param arr A pointer to a type dynamically allocated with new[].
	//
	// /////////////////////////////////////////////////////////////////
	template<class T>
		void DeleteArray(T *arr) throw ()
	{
		if(arr)
		{
			delete [] arr;
			arr = NULL;
		}
	};

#if defined(WIN32) || defined (TARGET_OS_WIN32) || defined(_WINDOWS)
	// /////////////////////////////////////////////////////////////////
	// Clean up a COM object.
	//
	// @param ptr A pointer to a COM object.
	//
	// /////////////////////////////////////////////////////////////////
	template<class T>
		void Release(T *ptr)
	{
		if(ptr)
		{
			ptr->Release();
			ptr = NULL;
		}
	};
#endif

	// /////////////////////////////////////////////////////////////////
	// @class SortBy_SharedPtr_Content
	// @author Mike McShaffry
	//
	// This is a functor which is a helper function for sorting elements
	// stored in shared pointers.
	//
	// /////////////////////////////////////////////////////////////////
	template<class T>
		class SortBy_SharedPtr_Content
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Get the lesser object.
		//
		// @return bool True if the left object is less than the right object
		//				or false otherwise.
		//
		// /////////////////////////////////////////////////////////////////
		bool operator()(const boost::shared_ptr<T> &lhs, const boost::shared_ptr<T> &rhs) const
		{
			return (*lhs < *rhs);
		};
	};
    
    // /////////////////////////////////////////////////////////////////
    // Copied from boost to reduce dependancies - please don't sue!
    //
    // /////////////////////////////////////////////////////////////////
    class NonCopyable
    {
    protected:
        NonCopyable() {};
        ~NonCopyable() {};
    private:
        NonCopyable(const NonCopyable&);
        const NonCopyable& operator=(const NonCopyable&);
    };

	// /////////////////////////////////////////////////////////////////
	// Get a string giving us a OpenGL error code in a human readable 
	// format.
	//
	// @param errorCode The OpenGL error code.
	//
	// @return std::string Human readable string.
	//
	// /////////////////////////////////////////////////////////////////
	std::string GetOpenGLError(const GLenum errorCode);
    
// Define OpenGL check and clear error macros so that this slow error checking can be compiled out for the release build!
    bool GfCheckGlError();
    bool GfCheckGlErrorTrc(const char *msg);
#define GF_CHECK_GL_ERROR() GfCheckGlError()
#define GF_CHECK_GL_ERROR_TRC(msg) GfCheckGlErrorTrc(msg)
    
#if DEBUG
#define GF_CLEAR_GL_ERROR() { GLenum err; do { err = glGetError(); } while(err != GL_NO_ERROR); }
#else
#define GF_CLEAR_GL_ERROR()
#endif

}

#endif
