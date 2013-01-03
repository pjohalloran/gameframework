// /////////////////////////////////////////////////////////////////
// @file ObjModelFileLoader.cpp
// @author PJ O Halloran
// @date 06/12/2010
//
// Implementation for loading 3D objects from files using the OBJ 
// wavefront file format.
//
// /////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "ObjModelFileLoader.h"
#include "Triangle.h"
#include "TextResource.h"
#include "GameMain.h"

namespace GameHalloran
{

	// Definition of static const strings.
	const std::string ObjModelFileLoader::COMMENT_STR("#");
	const std::string ObjModelFileLoader::VERTEX_STR("v ");
	const std::string ObjModelFileLoader::NORMAL_STR("vn ");
	const std::string ObjModelFileLoader::TEXCOORD_STR("vt ");
	const std::string ObjModelFileLoader::FACE_STR("f ");
	const std::string ObjModelFileLoader::GROUP_STR("g ");
	const std::string ObjModelFileLoader::USEMTL_STR("usemtl ");
	const std::string ObjModelFileLoader::MATERIAL_GROUP_STR("mtllib ");

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ObjModelFileLoader::GenerateFileStatistics(const std::vector<std::string> &linesVec,\
													U64 &numLines,\
													U64 &numGroups,\
													U64 &numVertices,\
													U64 &numNormals,\
													U64 &numTexCoords,\
													U64 &numTriangles,\
													const bool printStatistics)
	{
		numLines = 0;
		numGroups = 0;
		numVertices = 0;
		numNormals = 0;
		numTexCoords = 0;
		numTriangles = 0;

		for(std::vector<std::string>::const_iterator i = linesVec.begin(), end = linesVec.end(); i != end; ++i)
		{
			++numLines;

			if(boost::algorithm::starts_with(*i, VERTEX_STR))
			{
				++numVertices;
			}
			else if(boost::algorithm::starts_with(*i, NORMAL_STR))
			{
				++numNormals;
			}
			else if(boost::algorithm::starts_with(*i, TEXCOORD_STR))
			{
				++numTexCoords;
			}
			else if(boost::algorithm::starts_with(*i, FACE_STR))
			{
				++numTriangles;
			}
			else if(boost::algorithm::starts_with(*i, GROUP_STR))
			{
				++numGroups;
			}
		}

		if(numVertices > 0 && numGroups == 0)
		{
			numGroups = 1;
		}

#if DEBUG
		if(printStatistics)
		{
			std::cout << "Num lines:\t\t" << numLines << std::endl;
			std::cout << "Num Vertices:\t\t" << numVertices << std::endl;
			std::cout << "Num Normals:\t\t" << numNormals << std::endl;
			std::cout << "Num tex coords:\t\t" << numTexCoords << std::endl;
			std::cout << "Num triangles:\t\t" << numTriangles << std::endl;
			std::cout << "Num groups:\t\t" << numGroups << std::endl;
		}
#endif
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ObjModelFileLoader::BuildTriangleLists(std::vector<std::string> &linesVec)
	{
		U64 count, groups, vertices, normals, texCoords, triangles;
		GenerateFileStatistics(linesVec, count, groups, vertices, normals, texCoords, triangles, true);

		m_vertices.reserve(vertices);
		m_normals.reserve(normals);
		m_texCoords.reserve(texCoords);

		try
		{
			// First pass - Read in triangle vertices, normals and tex Coords first.
			// Second pass - Read in faces and construct triangles from the v,n and tcs its refers to.
			U32 passNumber = 1;
			U64 currIteration = 0;
			U64 totalIterations = vertices + normals + texCoords + triangles;
			bool anyGroups = false;
			bool validLine = true;
			for(std::vector<std::string>::iterator i = linesVec.begin(); ((passNumber <= 2) && (i != linesVec.end())); ++currIteration)
			{
				if((!(*i).empty()) &&\
					(!boost::algorithm::starts_with(*i, MATERIAL_GROUP_STR)) &&\
					(!boost::algorithm::starts_with(*i, COMMENT_STR)) &&\
					(!boost::algorithm::starts_with(*i, USEMTL_STR)))
				{
					if(ParseLine(*i, passNumber, anyGroups))
					{
						i = linesVec.erase(i);
						validLine = false;
					}
					else
					{
						++i;
					}
				}
				else
				{
					i = linesVec.erase(i);
					validLine = false;
				}

				// Check if we should move to the next pass of the file.
				if((i == linesVec.end()) && (passNumber < 2))
				{
					++passNumber;
					i = linesVec.begin();

					if(!anyGroups)
					{
						// Create the one default group now.
						m_currentGroup.assign("defaultgroup");
						m_objectMap[m_currentGroup] = TriangleList();
					}
				}

				if(validLine && m_callbackObjPtr)
				{
					m_callbackObjPtr->VReportProgress(F32(currIteration) / F32(totalIterations));
				}
				validLine = true;
			}
		}
		catch(GameException &ge)
		{
            GF_LOG_TRACE_ERR("ObjModelFileLoader::BuildTriangleLists()", std::string("Failed to build the triangle lists: ") + std::string(ge.what()));
			return (false);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ObjModelFileLoader::ParseLine(const std::string &line, const U32 passNumber, bool &anyGroups) throw (GameException &)
	{	
		bool result = true;

		if(passNumber == 1)
		{
			if(boost::algorithm::starts_with(line, VERTEX_STR))
			{
				ParseVertex(line);
			}
			else if(boost::algorithm::starts_with(line, NORMAL_STR))
			{
				ParseNormal(line);
			}
			else if(boost::algorithm::starts_with(line, TEXCOORD_STR))
			{
				ParseTextureCoordinate(line);
			}
			else if(boost::algorithm::starts_with(line, GROUP_STR))
			{
				anyGroups = true;
				result = false;
			}
			else if(boost::algorithm::starts_with(line, FACE_STR))
			{
				// ignore on this pass
				result = false;
			}
		}
		else if(passNumber == 2)
		{
			if(boost::algorithm::starts_with(line, FACE_STR))
			{
				ParseFace(line);
			}
			else if(boost::algorithm::starts_with(line, GROUP_STR))
			{
				ParseGroup(line);
			}
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ObjModelFileLoader::ParseVertex(const std::string &vertexStr) throw (GameException &)
	{
		std::vector<std::string> tokens;
		boost::algorithm::split(tokens, vertexStr, boost::algorithm::is_any_of(std::string(" ")));

		if(tokens.size() != std::vector<std::string>::size_type(4))
		{
			throw GameException(std::string("Invalid number of components in line ") + vertexStr);
		}

		F32 x, y, z;

		try
		{
			x = boost::lexical_cast<F32, std::string>(tokens[1]);
			y = boost::lexical_cast<F32, std::string>(tokens[2]);
			z = boost::lexical_cast<F32, std::string>(tokens[3]);
		}
		catch(...)
		{
			throw GameException(std::string("Failed to convert a component to a F32 ") + vertexStr);
		}

		m_vertices.push_back(Vector3(x, y, z));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ObjModelFileLoader::ParseNormal(const std::string &normalStr) throw (GameException &)
	{
		std::vector<std::string> tokens;
		boost::algorithm::split(tokens, normalStr, boost::algorithm::is_any_of(std::string(" ")));

		if(tokens.size() != std::vector<std::string>::size_type(4))
		{
			throw GameException(std::string("Invalid number of components in line ") + normalStr);
		}

		F32 x, y, z;

		try
		{
			x = boost::lexical_cast<F32, std::string>(tokens[1]);
			y = boost::lexical_cast<F32, std::string>(tokens[2]);
			z = boost::lexical_cast<F32, std::string>(tokens[3]);
		}
		catch(...)
		{
			throw GameException(std::string("Failed to convert a component to a F32 ") + normalStr);
		}

		m_normals.push_back(Vector3(x, y, z));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ObjModelFileLoader::ParseTextureCoordinate(const std::string &tcStr) throw (GameException &)
	{
		std::vector<std::string> tokens;
		boost::algorithm::split(tokens, tcStr, boost::algorithm::is_any_of(std::string(" ")));

		if(tokens.size() != std::vector<std::string>::size_type(3))
		{
			throw GameException(std::string("Invalid number of components in line ") + tcStr);
		}

		F32 u, v;

		try
		{
			u = boost::lexical_cast<F32, std::string>(tokens[1]);
			v = boost::lexical_cast<F32, std::string>(tokens[2]);
		}
		catch(...)
		{
			throw GameException(std::string("Failed to convert a component to a F32 ") + tcStr);
		}

		m_texCoords.push_back(Vector3(u, v, 0.0f));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ObjModelFileLoader::ParseFace(const std::string &faceStr) throw (GameException &)
	{
		if(m_currentGroup.empty())
		{
			throw GameException(std::string("Current group string is empty"));
		}

		std::vector<std::string> tokens;
		boost::algorithm::split(tokens, faceStr, boost::algorithm::is_any_of(std::string(" ")));

		if(tokens.size() != std::vector<std::string>::size_type(4))
		{
			throw GameException(std::string("Invalid number of components in line ") + faceStr);
		}

		U32 tokenIndex, vertexIndex = 0;
		Vertex vArr[3];
		for(tokenIndex = 1; tokenIndex <= 3; ++tokenIndex, ++vertexIndex)
		{
			std::vector<std::string> triangleTokens;
			boost::algorithm::split(triangleTokens, tokens[tokenIndex], boost::algorithm::is_any_of(std::string("/")));

			if(triangleTokens.size() != std::vector<std::string>::size_type(3))
			{
				throw GameException(std::string("Invalid number of components in vertex ") + tokens[tokenIndex] + std::string(" from line ") + faceStr);
			}

			if(!triangleTokens[0].empty())
			{
				U32 vIndex = 0;
				try { vIndex = boost::lexical_cast<U32, std::string>(triangleTokens[0]); } catch(...) {}
				vArr[vertexIndex].SetPosition(Point3(m_vertices[vIndex-1]));
			}
			else
			{
				// All vertices need at a minimum to have a position!
				throw GameException(std::string("Found a vertex that did not have a position specified in line ") + faceStr);
			}
			// Tex Coord index [optional]
			if(!triangleTokens[1].empty())
			{
				U32 tcIndex = 0;
				try { tcIndex = boost::lexical_cast<U32, std::string>(triangleTokens[1]); } catch(...) {}
				vArr[vertexIndex].AddTextureCoordinate(m_texCoords[tcIndex-1]);
			}
			// Normal index [optional] (we skip this if we are generating our own normals later...)
			if(!triangleTokens[2].empty())
			{
				U32 nIndex = 0;
				try { nIndex = boost::lexical_cast<U32, std::string>(triangleTokens[2]); } catch(...) {}
				vArr[vertexIndex].SetNormal(m_normals[nIndex-1]);
			}
		}

		boost::shared_ptr<Triangle> triPtr(GCC_NEW Triangle(vArr[0], vArr[1], vArr[2]));

		if(m_calculateNormals)
		{
			Vector3 triangleNormal(triPtr->CalculateNormal());
			for(U32 i(0); i < 3; ++i)
			{
				vArr[i].SetNormal(triangleNormal);
				triPtr->SetVertex(static_cast<Triangle::VertexId>(i), vArr[i]);
			}
		}

		m_objectMap[m_currentGroup].push_back(triPtr);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ObjModelFileLoader::ParseGroup(const std::string &groupStr) throw (GameException &)
	{
		std::vector<std::string> tokens;
		boost::algorithm::split(tokens, groupStr, boost::algorithm::is_any_of(std::string(" ")));

		if(tokens.size() != std::vector<std::string>::size_type(2))
		{
			throw GameException(std::string("Invalid number of components in line ") + groupStr);
		}

		if(!tokens[1].empty())
		{
			m_currentGroup.assign(tokens[1]);
			m_objectMap[m_currentGroup] = TriangleList();
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ObjModelFileLoader::VLoad(const std::string &resourceFileKey)
	{
		if(resourceFileKey.empty())
		{
            GF_LOG_TRACE_ERR("ObjModelFileLoader::VLoad(RC)", "Resource cache file key is empty");
			return (false);
		}

		TextResource tr(resourceFileKey);
		boost::shared_ptr<TextResHandle> thPtr = boost::static_pointer_cast<TextResHandle>(g_appPtr->GetResourceCache()->GetHandle(&tr));
		if(!thPtr || !thPtr->VInitialize())
		{
            GF_LOG_TRACE_ERR("ObjModelFileLoader::VLoad(RC)", std::string("Failed to find and initialize the text resource from the resource cache: ") + resourceFileKey);
			return (false);
		}

		std::string objFileData(thPtr->GetTextBuffer());

#if defined(_WINDOWS)
		std::string newLine("\n");
#else
		std::string newLine("\t\n");
#endif
		// Make a large vector to prevent repeating reallocations (we dont know how long the file is, this is a guess to help reduce reallocations).
		std::vector<std::string> linesVec;
		linesVec.reserve(3000);
		boost::algorithm::split(linesVec, objFileData, boost::algorithm::is_any_of(newLine));

		// Trim excess capacity.
		std::vector<std::string>(linesVec.begin(), linesVec.end()).swap(linesVec);

		RemoveTrailingCr fo;
		std::for_each(linesVec.begin(), linesVec.end(), fo);

		if(!BuildTriangleLists(linesVec))
		{
			return (false);
		}

		BaseModelFileLoader::SetFileLoaded(true);
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ObjModelFileLoader::VLoad(const boost::filesystem::path &filePath)
	{
		if(!boost::filesystem::is_regular_file(filePath))
		{
            GF_LOG_TRACE_ERR("ObjModelFileLoader::VLoad(FS)", std::string("File is not a regular file (does it exist? Are you specifing a directory?, etc.): ") + filePath.string());
			return (false);
		}
		if(filePath.extension().string().compare(".obj") != 0)
		{
            GF_LOG_TRACE_ERR("ObjModelFileLoader::VLoad(FS)", std::string("File does not have a .obj extension: ") + filePath.string());
			return (false);
		}

		std::ifstream inputStream(filePath.string().c_str());
		if(!inputStream)
		{
            GF_LOG_TRACE_ERR("ObjModelFileLoader::VLoad(FS)", std::string("Failed to open file: ") + filePath.string());
			return (false);
		}

		const U32 SIZE = 1024;
		char buffer[SIZE];
		memset(buffer, 0, sizeof(char) * SIZE);
		std::vector<std::string> linesVec;
		linesVec.reserve(3000);

		while(!inputStream.eof())
		{
			inputStream.getline(buffer, SIZE);
			linesVec.push_back(std::string(buffer));
		}

		// Trim excess vector capacity
		std::vector<std::string>(linesVec.begin(), linesVec.end()).swap(linesVec);

		if(!BuildTriangleLists(linesVec))
		{
			return (false);
		}

		BaseModelFileLoader::SetFileLoaded(true);
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ObjModelFileLoader::VClear()
	{
		BaseModelFileLoader::VClear();
		m_vertices.clear();
		m_normals.clear();
		m_texCoords.clear();
		m_currentGroup.clear();
	}

}
