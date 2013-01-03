#pragma once
#ifndef __GF_OBJ_MODEL_FILE_LOADER_H
#define __GF_OBJ_MODEL_FILE_LOADER_H

// /////////////////////////////////////////////////////////////////
// @file ObjModelFileLoader.h
// @author PJ O Halloran
// @date 06/12/2010
//
// Header for loading 3D objects from files using the OBJ wavefront
// file format.
//
// /////////////////////////////////////////////////////////////////

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

#include <string>

#include "BaseModelFileLoader.h"
#include "Triangle.h"
#include "GameException.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class ObjModelFileLoader
	// @author PJ O Halloran
	//
	// Interface for loading OBJ wavefront 3D models files.
	//
	// We do not support loading curves or other such complicated
	// data as described in the obj wavefront file format.  We only
	// support loading triangulated meshes at this time as thats all we
	// need.  Any such data found in the file will cause the VLoad calls
	// to fail.
	//
	// Also, all mesh faces must be triangulated as quads are not supported in
	// OpenGL 3.x and above.  Any model files with any faces != 3 vertices
	// will cause the VLoad method to fail.
	//
	// /////////////////////////////////////////////////////////////////
	class ObjModelFileLoader : public BaseModelFileLoader
	{
	private:

		std::vector<Vector3> m_vertices;								///< List of all vertices found in file.
		std::vector<Vector3> m_normals;									///< List of all normals found in file.
		std::vector<Vector3> m_texCoords;								///< List of all tex coords found in file.
		std::string m_currentGroup;										///< Name of the current TriangleList being built up.
		bool m_calculateNormals;										///< Should we calculate the normals ourselves or use those in the file?

		static const std::string COMMENT_STR;							///< Line beginning with a comment.
		static const std::string VERTEX_STR;							///< Vertex description.
		static const std::string NORMAL_STR;							///< Vertex normal description.
		static const std::string TEXCOORD_STR;							///< Texture coordinate description.
		static const std::string FACE_STR;								///< Face description.
		static const std::string GROUP_STR;								///< Model group description.
		static const std::string USEMTL_STR;							///< Material file description.
		static const std::string MATERIAL_GROUP_STR;					///< Material group file.

		// /////////////////////////////////////////////////////////////////
		// Reads through the file and counts up statistics.  These statistics
		// may be used for debugging or for reporting progress.
		//
		// @param numLines Total number of lines in the file.
		// @param numGroups Number of 3D models defined.
		// @param numVertices Number of vertices.
		// @param numNormals Number of normals.
		// @param numTexCoords Number of texture coordinates.
		// @param numTriangles Number of faces/triangles defined.
		//
		// /////////////////////////////////////////////////////////////////
		void GenerateFileStatistics(const std::vector<std::string> &linesVec,\
									U64 &numLines,\
									U64 &numGroups,\
									U64 &numVertices,\
									U64 &numNormals,\
									U64 &numTexCoords,\
									U64 &numTriangles,
									const bool printStatistics = true);

		// /////////////////////////////////////////////////////////////////
		// Build up the triangle list for all the 3D models in the file.
		//
		// @param linesVec The obj file split into a vector by lines.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool BuildTriangleLists(std::vector<std::string> &linesVec);

		// /////////////////////////////////////////////////////////////////
		// Parses a new line of the file and deals with it appropriately
		// depending on the pass through index.
		//
		// On the first pass we look for vertices, normals and tex coordinates.
		//
		// On the second pass we look for and construct models and objects from
		// face information.
		//
		// @param line The next line to parse.
		// @param passNumber The current pass number.  On the first pass we
		//						build up all the vertices, on the second pass
		//						we create the triangles from the vertices.
		// @param anyGroups Set to true during the first pass if the file contains
		//					any specific group id(s). Not used during second pass.
		//
		// @throw GameException& If any bad file data was encountered.
		//
		// /////////////////////////////////////////////////////////////////
		bool ParseLine(const std::string &line, const U32 passNumber, bool &anyGroups) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Parses a vertex line of the file.
		//
		// @throw GameException& If any bad file data was encountered.
		//
		// /////////////////////////////////////////////////////////////////
		void ParseVertex(const std::string &vertexStr) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Parses a normal line of the file.
		//
		// @throw GameException& If any bad file data was encountered.
		//
		// /////////////////////////////////////////////////////////////////
		void ParseNormal(const std::string &normalStr) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Parses a tex coord line of the file.
		//
		// @throw GameException& If any bad file data was encountered.
		//
		// /////////////////////////////////////////////////////////////////
		void ParseTextureCoordinate(const std::string &tcStr) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Parses a face line of the file.
		//
		// @throw GameException& If any bad file data was encountered.
		//
		// /////////////////////////////////////////////////////////////////
		void ParseFace(const std::string &faceStr) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Parses a group line of the file.
		//
		// @throw GameException& If any bad file data was encountered.
		//
		// /////////////////////////////////////////////////////////////////
		void ParseGroup(const std::string &groupStr) throw (GameException &);

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param calculateNormals Should we generate normals for every triangle
		//							or not?
		//
		// /////////////////////////////////////////////////////////////////
		explicit ObjModelFileLoader(const bool calculateNormals = false) : m_vertices(), m_normals(), m_texCoords(), m_currentGroup(), m_calculateNormals(calculateNormals) {};

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~ObjModelFileLoader() { try { VClear(); } catch(...) {} };

		// /////////////////////////////////////////////////////////////////
		// Open a 3D model file from the resource cache and parse its data.
		//
		// @param resourceFileKey Key/filename of the 3D model file in the
		//							resource cache.
		//
		// @return bool True on success or false on failure (check log file).
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VLoad(const std::string &resourceFileKey);

		// /////////////////////////////////////////////////////////////////
		// Open a 3D model file from the filesystem and parse its data.
		//
		// @param filePath Filepath of the 3D model file on the filesystem.
		//
		// @return bool True on success or false on failure (check log file).
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VLoad(const boost::filesystem::path &filePath);

		// /////////////////////////////////////////////////////////////////
		// Clear any and all previously loaded data.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VClear();

	};

}

#ifdef WIN32
#	pragma warning( pop )
#endif

#endif
