// /////////////////////////////////////////////////////////////////
// @file BaseModelFileLoader.cpp
// @author PJ O Halloran
// @date 06/12/2010
//
// Implementation for base file loading functionality for all 
// model loading files classes.
//
// /////////////////////////////////////////////////////////////////

#include <string>
#include <cstring>

#include "BaseModelFileLoader.h"
#include "Triangle.h"
#include "GameMain.h"
#include "Events.h"


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// ********************* BaseModelFileLoader ***********************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseModelFileLoader::VSetLoadingProgressCallback(IModelLoadProgressCallback *callbackObjPtr)
	{
		m_callbackObjPtr = callbackObjPtr;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void BaseModelFileLoader::VClear()
	{
		m_objectMap.clear();
		m_loaded = false;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool BaseModelFileLoader::VGetTriangleList(TriangleList &tList) const
	{
		if(!m_loaded)
		{
            GF_LOG_TRACE_ERR("BaseModelFileLoader::VGetTriangleList()", "No file was loaded yet");
			return (false);
		}

		if(m_objectMap.empty())
		{
            GF_LOG_TRACE_INF("BaseModelFileLoader::VGetTriangleList()", "No models were loaded from the file");
			return (false);
		}

		tList = m_objectMap.begin()->second;
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool BaseModelFileLoader::VGetObjectTriangleList(const std::string &groupId, TriangleList &tList) const
	{
		if(!m_loaded)
		{
            GF_LOG_TRACE_ERR("BaseModelFileLoader::VGetObjectTriangleList()", "No file was loaded yet");
			return (false);
		}
		if(groupId.empty())
		{
            GF_LOG_TRACE_ERR("BaseModelFileLoader::VGetObjectTriangleList()", "GroupId is empty");
			return (false);
		}

		ObjectGroupMap::const_iterator i = m_objectMap.find(groupId);
		if(i == m_objectMap.end())
		{
            GF_LOG_TRACE_ERR("BaseModelFileLoader::VGetObjectTriangleList()", "No object found with the ID " + groupId);
			return (false);
		}

		tList = i->second;
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	// ******************* Misc Model File helper **********************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ModelLoadingProgress::VReportProgress(const F32 progress)
	{
		m_currentProgress = progress;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ModelLoadingProgress::NextStage()
	{
		if(!IsComplete())
		{
			m_currentProgress = 0.0f;
			m_totalProgress += m_totalProgressInc;
			if(m_modelIndex < m_numberModels)
			{
				if(m_currOperation == eMeshBuilding)
				{
					++m_modelIndex;
					m_currOperation = eMeshLoading;
				}
				else
				{
					++m_currOperation;
				}
			}
			else
			{
				m_currOperation = eNumberOperations;
			}
			
			IEventDataPtr eventDataPtr(GCC_NEW EvtData_Loading_Progress(m_loadId, m_totalProgress));
			safeQueEvent(eventDataPtr);
			return (true);
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ModelLoadingProgress::Failure(const std::string &errorMsg)
	{
		// Mark load operation as complete but with an error status.
		m_currOperation = eNumberOperations;
		m_totalProgress = -1.0f;
		m_currentProgress = 0.0f;
		m_modelIndex = 0;

		IEventDataPtr eventDataPtr(GCC_NEW EvtData_Loading_Progress(m_loadId, m_totalProgress, boost::optional<std::string>(errorMsg)));
		safeQueEvent(eventDataPtr);
	}

	// /////////////////////////////////////////////////////////////////
	// ******************* Misc Model File helper **********************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<GLTriangleBatch> ConvertTriangleListToBatch(const TriangleList &tList, IModelLoadProgressCallback *progressCallbackPtr, const bool retainData)
	{
		if(tList.empty())
		{
			return (boost::shared_ptr<GLTriangleBatch>());
		}

		boost::shared_ptr<GLTriangleBatch> tBatch(GCC_NEW GLTriangleBatch);
		if(!tBatch)
		{
			return (boost::shared_ptr<GLTriangleBatch>());
		}

		const U32 NUM_VERTICES(Triangle::eNumberVertices);			// Number of vertices in a triangle.

		tBatch->BeginMesh(tList.size() * NUM_VERTICES);

		Vertex vertexObjArr[NUM_VERTICES];							// My Vertex Objects stored in triangle objects in the list.
		VertexArr vArr[NUM_VERTICES];								// VertexArr type GLTriangleBatch works with.
		NormalArr nArr[NUM_VERTICES];								// NormalArr type GLTriangleBatch works with.
		TextureArr tArr[NUM_VERTICES];								// TextureArr type GLTriangleBatch works with.

		U32 count = 0;
		F32 totalSize(tList.size());
		for(TriangleList::const_iterator i = tList.begin(), end = tList.end(); i != end; ++i, ++count)
		{
			for(U32 index(0); index < NUM_VERTICES; ++index)
			{
				memset(vArr[index], 0, sizeof(F32) * 3);
				memset(nArr[index], 0, sizeof(F32) * 3);
				memset(tArr[index], 0, sizeof(F32) * 2);

				if(!(*i)->GetVertex(Triangle::VertexId(index), vertexObjArr[index]))
				{
					//I32 balls = 0;
				}
				memcpy(vArr[index], vertexObjArr[index].GetPosition().GetComponentsConst(), sizeof(VertexArr));
				if(vertexObjArr[index].HasNormal())
				{
					Vector3 normal;
					vertexObjArr[index].GetNormal(normal);
					memcpy(nArr[index], normal.GetComponentsConst(), sizeof(NormalArr));
				}
				if(vertexObjArr[index].HasAnyTextureCoordinates())
				{
					Vector3 texCoords;
					vertexObjArr[index].GetTextureCoordinate(0, texCoords);
					memcpy(tArr[index], texCoords.GetComponentsConst(), sizeof(TextureArr));
				}
			}

			tBatch->AddTriangle(vArr, nArr, tArr);
			if(progressCallbackPtr)
			{
				progressCallbackPtr->VReportProgress(F32(count) / totalSize);
			}
		}

		//tBatch->PrintDebugInfo();

		tBatch->End(!retainData);

		return (tBatch);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	TriangleMesh &LoadMeshFromResourceCache(const std::string &meshId, BaseModelFileLoader *modelLoadingObjPtr, ModelLoadingProgress &loadProgressObj, TriangleMesh &mesh)
	{
		// Clear incoming mesh list.
		if(!mesh.empty())
		{
			mesh.clear();
		}

		// Verify input paramters.
		if(!modelLoadingObjPtr || meshId.empty())
		{
            GF_LOG_TRACE_ERR("LoadMeshFromResourceCache()", "Invalid parameters");
			return (mesh);
		}

		modelLoadingObjPtr->VSetLoadingProgressCallback(&loadProgressObj);

		// Load the mesh into memory.
		if(!modelLoadingObjPtr->VLoad(meshId))
		{
#if DEBUG
			std::string errMsg(std::string("Failed to load mesh from resource cache: ") + meshId);
			loadProgressObj.Failure(errMsg);
            GF_LOG_TRACE_ERR("LoadMeshFromResourceCache()", errMsg);
#endif
			return (mesh);
		}

		loadProgressObj.NextStage();

		modelLoadingObjPtr->VGetTriangleList(mesh);
		modelLoadingObjPtr->VClear();

		return (mesh);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	boost::shared_ptr<GLTriangleBatch> LoadBatchFromResourceCache(const std::string &meshId, BaseModelFileLoader *modelLoadingObjPtr, ModelLoadingProgress &loadProgressObj, BoundingCube &bb, const bool retainData)
	{
		TriangleMesh mesh;
		LoadMeshFromResourceCache(meshId, modelLoadingObjPtr, loadProgressObj, mesh);
		if(mesh.empty())
		{
			return (boost::shared_ptr<GLTriangleBatch>());
		}

		CalculateTriangleListBoundingBox(mesh, bb);
		
		boost::shared_ptr<GLTriangleBatch> batch = ConvertTriangleListToBatch(mesh, &loadProgressObj, retainData);
		if(!batch)
		{
#if DEBUG
			std::string errMsg(std::string("Failed to build mesh: ") + meshId);
			loadProgressObj.Failure(errMsg);
            GF_LOG_TRACE_ERR("LoadBatchFromResourceCache()", errMsg);
#endif
			return (batch);
		}

		loadProgressObj.NextStage();

		return (batch);
	}

}
