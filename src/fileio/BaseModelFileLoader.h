#pragma once
#ifndef __GF_BASE_MODEL_FILE_LOADER_H
#define __GF_BASE_MODEL_FILE_LOADER_H

// /////////////////////////////////////////////////////////////////
// @file BaseModelFileLoader.h
// @author PJ O Halloran
// @date 06/12/2010
//
// Header for base file loading functionality for all 
// model loading files classes.
//
// /////////////////////////////////////////////////////////////////

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

// External Headers
#include <string>
#include <map>

// Project Headers
#include "GameBase.h"
#include "IModelFileLoader.h"
#include "Triangle.h"
#include "GLTriangleBatch.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class BaseModelFileLoader
	// @author PJ O Halloran
	//
	// Base interface and functionality for all 3D model file format
	// loading classes.
	//
	// Derived classes should implement the VLoad() methods.
	//
	// /////////////////////////////////////////////////////////////////
	class BaseModelFileLoader : public IModelFileLoader
	{
	private:

		typedef std::map<std::string, TriangleList > ObjectGroupMap;

		bool m_loaded;										///< Has the file been loaded successfully?

	protected:

		ObjectGroupMap m_objectMap;							///< Map of TriangleLists to object IDs (for files with multiple objects).
		IModelLoadProgressCallback *m_callbackObjPtr;		///< Pointer to the callback object class to call as the model is loaded.

		// /////////////////////////////////////////////////////////////////
		// Set the file loaded status.
		//
		// /////////////////////////////////////////////////////////////////
		void SetFileLoaded(const bool loadStatus) { m_loaded = loadStatus; };

	public:

		// /////////////////////////////////////////////////////////////////
		// Default constructor.
		//
		// /////////////////////////////////////////////////////////////////
		explicit BaseModelFileLoader() : m_loaded(false), m_objectMap(), m_callbackObjPtr(NULL) {};

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~BaseModelFileLoader() {};

		// /////////////////////////////////////////////////////////////////
		// Set the model loading progress callback object.  During VLoad()
		// this will be called periodically to report model loading progress.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetLoadingProgressCallback(IModelLoadProgressCallback *callbackObjPtr);

		// /////////////////////////////////////////////////////////////////
		// Has a model file been successfully loaded?
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VIsLoaded() const { return (m_loaded); };

		// /////////////////////////////////////////////////////////////////
		// Clear any and all previously loaded data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VClear();

		// /////////////////////////////////////////////////////////////////
		// Get all the triangles found in the model file.
		//
		// N.B. If the model file contains more than one model than you
		// should use VGetObjectTriangleList to specify which list of 
		// triangles you require.
		//
		// @param tList List of triangles from file (on success).
		//
		// @return bool True on success or false on failure (check log file).
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VGetTriangleList(TriangleList &tList) const;

		// /////////////////////////////////////////////////////////////////
		// Get all the triangles found in the model file belonging to a
		// particular object in the file.
		//
		// @param tList List of triangles from file (on success).
		//
		// @return bool True on success or false on failure (check log file).
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VGetObjectTriangleList(const std::string &groupId, TriangleList &tList) const;

		// /////////////////////////////////////////////////////////////////
		// Get the number of 3D objects loaded into memory from the file.
		//
		// @return U32 Will return 0 if no objects loaded.
		//
		// /////////////////////////////////////////////////////////////////
		virtual U64 VGetNumberObjects() const { return (U64(m_objectMap.size())); };

	};

	// /////////////////////////////////////////////////////////////////
	// @class ModelLoadingProgress
	// @author PJ O Halloran
	//
	// Class for reporting the loading progress of a group of one or
	// more 3D models.
	//
	// /////////////////////////////////////////////////////////////////
	class ModelLoadingProgress : public IModelLoadProgressCallback
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// @enum LoadingOperation
		//
		// ID of each stage in the progress of loading a single model.
		//
		// eMeshLoading:	Parsing a model from a 3D file.
		// eMeshBuilding:	Building a VBO from the 3D model.
		//
		// /////////////////////////////////////////////////////////////////
		enum LoadingOperation
		{
			eMeshLoading = 0,
			eMeshBuilding,
			eNumberOperations
		};

	private:

		I32 m_currOperation;							///< Current stage.
		F32 m_totalProgress;							///< Total progress to date for complete table loading progress (0.0 to 1.0).
		U32 m_numberModels;								///< Number of 3D models to load during this operation.
		I32 m_modelIndex;								///< Index of the current model being loaded.
		const I32 m_loadId;								///< ID of the model loading operation.
		const F32 m_totalProgressInc;					///< When a stage is finished, increment total by this value.
		F32 m_currentProgress;                          ///< Progress for the current stage.

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param loadId The ID of the loading process.
		// @param numberModels The number of 3D models to load.
		//
		// /////////////////////////////////////////////////////////////////
		ModelLoadingProgress(const I32 loadId, const U32 numberModels = 1) : m_currOperation(0), m_totalProgress(0.0f), m_numberModels(numberModels),\
			m_modelIndex(0), m_loadId(loadId), m_totalProgressInc((1.0f / F32(eNumberOperations)) * F32(numberModels)), m_currentProgress(0.0f) {};

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~ModelLoadingProgress() {};

		// /////////////////////////////////////////////////////////////////
		// Function to report model loading progress.
		//
		// This function should be short so as not to slow down model loading.
		//
		// @param progress The progress of the load (0.0 <= progress <= 1.0).
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VReportProgress(const F32 progress);

		// /////////////////////////////////////////////////////////////////
		// Move to the next loading stage.
		//
		// @return bool True if there is a next stage to move to.
		//
		// /////////////////////////////////////////////////////////////////
		bool NextStage();

		// /////////////////////////////////////////////////////////////////
		// Called if a failure occurs in the loading operation, triggers
		// completion of the loading progress with an error return value.
		//
		// @param errorMsg The error message you would like included in the
		//					event which will be reported.
		//
		// /////////////////////////////////////////////////////////////////
		void Failure(const std::string &errorMsg);

		// /////////////////////////////////////////////////////////////////
		// Get the current stage.
		// 
		// /////////////////////////////////////////////////////////////////
		LoadingOperation GetStage() const { return (LoadingOperation(m_currOperation)); };

		// /////////////////////////////////////////////////////////////////
		// Check if the model loading progress is completed.
		//
		// /////////////////////////////////////////////////////////////////
		bool IsComplete() const { return (LoadingOperation(m_currOperation) == eNumberOperations); };

		// /////////////////////////////////////////////////////////////////
		// Get the total progress.
		//
		// /////////////////////////////////////////////////////////////////
		F32 GetTotalProgress() const { return (m_totalProgress); };

		// /////////////////////////////////////////////////////////////////
		// Get the number of models loaded to date.
		//
		// /////////////////////////////////////////////////////////////////
		U32 GetModelsLoadedCount() const { return (m_modelIndex+1); };

		// /////////////////////////////////////////////////////////////////
		// Get the loading process ID.
		//
		// /////////////////////////////////////////////////////////////////
		I32 GetLoadingId() const { return (m_loadId); };
	};

	// /////////////////////////////////////////////////////////////////
	// Given a list of triangle, add them to an object of type
	// GLTriangleBatch so that they may be rendered in OpenGL.
	//
	// @param tList List of triangles.
	// @param progressCallbackPtr Pointer to a progress callback object.
	//								If it is NULL then no progress is
	//								reported.
	// @param retainData Retain the CPU side triangle vertex and index data?
	//
	// @return boost::shared_ptr<GLTriangleBatch> Null on error or a
	//												batch of triangles
	//												ready for rendering.
	//
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<GLTriangleBatch> ConvertTriangleListToBatch(const TriangleList &tList, IModelLoadProgressCallback *progressCallbackPtr = NULL, const bool retainData = false);

	// /////////////////////////////////////////////////////////////////
	// Loads a 3D mesh from a file stored in the RC file and loads the
	// mesh into a GL VBO.
	//
	// @param meshId The RC ID/name of the 3D mesh file.
	// @param modelLoadingObjPtr Pointer to the model loading object.
	// @param loadProgressObj Loading progress reporting object.
	// @param mesh The TriangleMesh returned.
	//
	// @return TriangleMesh& Reference to same mesh as "mesh" parameter.
	//
	// /////////////////////////////////////////////////////////////////
	TriangleMesh &LoadMeshFromResourceCache(const std::string &meshId, BaseModelFileLoader *modelLoadingObjPtr, ModelLoadingProgress &loadProgressObj, TriangleMesh &mesh);

	// /////////////////////////////////////////////////////////////////
	// Loads a 3D mesh from a file stored in the RC file and loads the
	// mesh into a GL VBO.
	//
	// @param meshId The RC ID/name of the 3D mesh file.
	// @param modelLoadingObjPtr Pointer to the model loading object.
	// @param loadProgressObj Loading progress reporting object.
	// @param bb BoundingBox calculated from the 3D mesh.
	// @param retainData Retain the CPU side triangle vertex and index data?
	//
	// @return boost::shared_ptr<GLTriangleBatch> A Pointer to a batch of
	//											geometry or NULL on error.
	//
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<GLTriangleBatch> LoadBatchFromResourceCache(const std::string &meshId, BaseModelFileLoader *modelLoadingObjPtr, ModelLoadingProgress &loadProgressObj, BoundingCube &bb, const bool retainData = false);

}

#ifdef WIN32
#	pragma warning( pop )
#endif

#endif
