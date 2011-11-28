// /////////////////////////////////////////////////////////////////
// @file IModelFileLoader.h
// @author PJ O Halloran
// @date 03/12/2010
//
// Interface for all model loading files.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GF_I_MODEL_FILE_LOADER_H
#define __GF_I_MODEL_FILE_LOADER_H

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

// External Headers
#include <string>

#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>

// Project Headers
#include "GameTypes.h"
#include "Triangle.h"

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class IModelLoadProgressCallback
	// @author PJ O Halloran
	//
	// Base interface for a model loading progress callback class that
	// is called as the models loading progress is updated.
	//
	// /////////////////////////////////////////////////////////////////
	class IModelLoadProgressCallback
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~IModelLoadProgressCallback() {};

		// /////////////////////////////////////////////////////////////////
		// Function to report model loading progress.
		//
		// This function should be short so as not to slow down model loading.
		//
		// @param progress The progress of the load (0.0 <= progress <= 1.0).
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VReportProgress(const F32 progress) = 0;
	};

	// /////////////////////////////////////////////////////////////////
	// @class IModelFileLoader
	// @author PJ O Halloran
	//
	// Base level interface for all 3D model loading classes.  Each class
	// should be capable of parsing a 3D model file, either located on
	// disk or in compressed format in the resource cache.
	//
	// It should also cache the list of triangles it loads for retrieval
	// later on.
	//
	// /////////////////////////////////////////////////////////////////
	class IModelFileLoader
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~IModelFileLoader() { };

		// /////////////////////////////////////////////////////////////////
		// Set the model loading progress callback object.  During VLoad()
		// this will be called periodically to report model loading progress.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetLoadingProgressCallback(IModelLoadProgressCallback *callbackObjPtr) = 0;

		// /////////////////////////////////////////////////////////////////
		// Open a 3D model file from the resource cache and parse its data.
		//
		// @param resourceFileKey Key/filename of the 3D model file in the
		//							resource cache.
		//
		// @return bool True on success or false on failure (check log file).
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VLoad(const std::string &resourceFileKey) = 0;

		// /////////////////////////////////////////////////////////////////
		// Open a 3D model file from the filesystem and parse its data.
		//
		// @param filePath Filepath of the 3D model file on the filesystem.
		//
		// @return bool True on success or false on failure (check log file).
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VLoad(const boost::filesystem::path &filePath) = 0;

		// /////////////////////////////////////////////////////////////////
		// Has a model file been successfully loaded?
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VIsLoaded() const = 0;

		// /////////////////////////////////////////////////////////////////
		// Clear any and all previously loaded data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VClear() = 0;

		// /////////////////////////////////////////////////////////////////
		// Get all the triangles found in the model file.
		//
		// N.B. If the model file contains more than one model than you
		// should use VGetObjectTriangleList to specify which list of 
		// triangles you require.  In this case the first object in the file
		// will be returned in the TriangleList.
		//
		// @param tList List of triangles from file (on success).
		//
		// @return bool True on success or false on failure (check log file).
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VGetTriangleList(TriangleList &tList) const = 0;

		// /////////////////////////////////////////////////////////////////
		// Get all the triangles found in the model file belonging to a
		// particular object in the file.
		//
		// @param tList List of triangles from file (on success).
		//
		// @return bool True on success or false on failure (check log file).
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VGetObjectTriangleList(const std::string &groupId, TriangleList &tList) const = 0;

		// /////////////////////////////////////////////////////////////////
		// Get the number of 3D objects loaded into memory from the file.
		//
		// @return U32 Will return 0 if no objects loaded.
		//
		// /////////////////////////////////////////////////////////////////
		virtual U64 VGetNumberObjects() const = 0;

	};

}

#ifdef WIN32
#	pragma warning( pop )
#endif

#endif
