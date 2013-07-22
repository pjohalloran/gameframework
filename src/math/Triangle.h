#pragma once
#ifndef __GF_TRIANGLE_H
#define __GF_TRIANGLE_H

// /////////////////////////////////////////////////////////////////
// @file Triangle.h
// @author PJ O Halloran
// @date 03/12/2010
//
// File contains the header for the Triangle class.
//
// This is part of a simple 3D math library I wrote in order to try
// to gain a great understanding of 3D math.
//
// I am using OpenGL so the libs are aimed at being a help for this
// but they should in theory be useful for Direct3D or others also.
// The only difference that I know of is that OpenGL uses the column
// major matrix format while Direct3D uses row major matrix format.
//
// As you will see below, I was influenced by other well written math
// libraries freely available but this code was mostly written by myself
// as a learning experience.  Therefore the code is geared towards
// understanding rather than effiency.
//
// If I wasn't learning 3D math, then I would not write my own version.
// Instead I would use something like the Bullet VectorMath library or the
// Direct3D math library.
//
// I would like to cite the following math textbooks/web resources I used
// to learn the math required to write this simple 3D math library:
// - Mathematics for Computer Graphics 2nd Edition by John Vince.
// - Fundamentals of Computer Graphics 3rd Edition by Shirley et al.
// - Mathworld (http://www.mathworld.com).
// - Wolfgang Engels blog and oolong engine 2
//      (http://wolfgang-engel.info/blogs).
// - OpenGL Superbible, 5th Edition by Wright et al.
// - OpenGL Programming Guide (red), 7th Edition by Shreiner et al.
// - Bullet v2.76 Vector Math Library by Edwin Cousins
//      (http://www.bulletphysics.com).
// - Game Coding Complete, 3rd Edition by Mike McShaffry et al.
//
// /////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <list>
#include <ostream>
#include <sstream>

#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

#include "CommonMath.h"
#include "Vector.h"
#include "BoundingSphere.h"
#include "BoundingCube.h"

namespace GameHalloran {

    // Typedef for a texture coordinates vector for a Vertex.
    typedef std::vector<Vector3> TexCoordsVec;

    // /////////////////////////////////////////////////////////////////
    // @class Vertex
    // @author PJ O Halloran
    //
    // A vertex in 3D space.  It has a position at the very least.
    // It may also have a normal and color defined as well as multiple
    // texture coordinates.
    //
    // /////////////////////////////////////////////////////////////////
    class Vertex {
    public:

        // Typedefs for optional class members to save typing.
        typedef boost::optional<Vector3> OpVector3;
        typedef boost::optional<Vector4> OpVector4;
        typedef boost::optional<TexCoordsVec > OpTexCoordsVec;

    private:

        Point3 m_position;                      ///< Position.
        OpVector3 m_normal;                     ///< Optional normal.
        OpTexCoordsVec m_texCoordsVec;          ///< Optional texture coordinate(s).
        OpVector4 m_color;                      ///< Optional color.

    public:

        // /////////////////////////////////////////////////////////////////
        // Default constructor.
        //
        // /////////////////////////////////////////////////////////////////
        explicit inline Vertex() : m_position(), m_normal(), m_texCoordsVec(), m_color() {};

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param pos The position of the vertex.
        // @param texCoord Container of texture coordinates [optional].
        // @param color Color of vectex [optional].
        // @param normal Normal of vertex [optional].
        //
        // /////////////////////////////////////////////////////////////////
        explicit inline Vertex(const Point3 &pos, const OpTexCoordsVec &texCoord = OpTexCoordsVec(), const OpVector4 &color = OpVector4(), const OpVector3 &normal = OpVector3())\
    :
        m_position(pos)
        , m_normal(normal)
        , m_texCoordsVec(texCoord)
        , m_color(color) {
        };

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        ~Vertex() {};

        // /////////////////////////////////////////////////////////////////
        // Equivalent operator.
        //
        // @param rhs
        //
        // /////////////////////////////////////////////////////////////////
        bool operator==(const Vertex &rhs) const;

        // /////////////////////////////////////////////////////////////////
        // Not equivalent operator.
        //
        // @param rhs
        //
        // /////////////////////////////////////////////////////////////////
        inline bool operator!=(const Vertex &rhs) const {
            return (!(*this == rhs));
        };

        // /////////////////////////////////////////////////////////////////
        // Less than operator.
        //
        // @param rhs
        //
        // /////////////////////////////////////////////////////////////////
        bool operator<(const Vertex &rhs) const;

        // /////////////////////////////////////////////////////////////////
        // Get the position.
        //
        // /////////////////////////////////////////////////////////////////
        inline Point3 GetPosition() const {
            return (m_position);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the position.
        //
        // /////////////////////////////////////////////////////////////////
        inline void GetPosition(Point3 &position) const {
            position = m_position;
        };

        // /////////////////////////////////////////////////////////////////
        // Set the position.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetPosition(const Point3 &position) {
            m_position = position;
        };

        // /////////////////////////////////////////////////////////////////
        // Check if it has a normal.
        //
        // @return bool True if there was a normal and false if not.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool HasNormal() const {
            return (m_normal.is_initialized());
        };

        // /////////////////////////////////////////////////////////////////
        // Get the normal.
        //
        // @param normal Object that holds the normal (on success).
        //
        // @return bool True if there was a normal and false if not.
        //
        // /////////////////////////////////////////////////////////////////
        bool GetNormal(Vector3 &normal) const;

        // /////////////////////////////////////////////////////////////////
        // Set the normal.
        //
        // @param normal The new normal.
        //
        // /////////////////////////////////////////////////////////////////
        void SetNormal(const Vector3 &normal);

        // /////////////////////////////////////////////////////////////////
        // Check if it has any texture coordinates.
        //
        // @return bool True if there was any texture coordinates and false
        //              if not.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool HasAnyTextureCoordinates() const {
            return ((m_texCoordsVec.is_initialized()) && (!m_texCoordsVec->empty()));
        };

        // /////////////////////////////////////////////////////////////////
        // Check if it has texture coordinate for the texture unit supplied.
        //
        // @param textureUnit
        //
        // @return bool True if there was a texture coordinate and false if not.
        //
        // /////////////////////////////////////////////////////////////////
        bool HasTextureCoordinate(const U32 textureUnit) const;

        // /////////////////////////////////////////////////////////////////
        // Get the container of texture coordinates.
        //
        // @param texCoordVec Container to store any texture coordinates in
        //                      (on success).
        //
        // @return bool True if there was any texture coordinates and false if not.
        //
        // /////////////////////////////////////////////////////////////////
        bool GetTextureCoordinateContainer(TexCoordsVec &texCoordVec) const;

        // /////////////////////////////////////////////////////////////////
        // Check how many texture coordinates the vertex has.
        //
        // /////////////////////////////////////////////////////////////////
        U32 GetNumberTextureUnits() const;

        // /////////////////////////////////////////////////////////////////
        // Get a texture coordinate.
        //
        // @param textureUnit
        // @param texCoord Vector to store the tex coordinate in (on success).
        //
        // @return bool True if there was a texture coordinate and false if not.
        //
        // /////////////////////////////////////////////////////////////////
        bool GetTextureCoordinate(const U32 textureUnit, Vector3 &texCoord) const;

        // /////////////////////////////////////////////////////////////////
        // Add a texture coordinate.  You may add as many as you want.  Each
        // one is for successiv texture units.
        //
        // @return U32 The texture unit the tex coord is assigned to.
        //
        // /////////////////////////////////////////////////////////////////
        U32 AddTextureCoordinate(const Vector3 &texCoords);

        // /////////////////////////////////////////////////////////////////
        // Does the vertex have a color attribute?
        //
        // /////////////////////////////////////////////////////////////////
        inline bool HasColor() const {
            return (m_color.is_initialized());
        };

        // /////////////////////////////////////////////////////////////////
        // Get the vertex color.
        //
        // @param color Object to store the color in.
        //
        // @return bool True if the vertex has a color and false if not.
        //
        // /////////////////////////////////////////////////////////////////
        bool GetColor(Vector4 &color) const;

        // /////////////////////////////////////////////////////////////////
        // Set the vertex color.
        //
        // @param color The new color.
        //
        // /////////////////////////////////////////////////////////////////
        void SetColor(const Vector4 &color);

        // /////////////////////////////////////////////////////////////////
        // All vertices sent to a shader should all have the same attributes.
        // E.g. They should all have positions.  If one has texture coordinates
        // then they should all have texture coordinates, same for normals or
        // colors.  If not the shader *probably* won't render properly
        // and certainly won't work as intended.
        //
        // This checks if two vertex objects are compatible with each other
        // or in other words if they are eligible to be sent to the GPU in the
        // same batch for rendering.
        //
        // /////////////////////////////////////////////////////////////////
        bool IsCompatibleWith(const Vertex &rhs) const;

        // /////////////////////////////////////////////////////////////////
        // Produces a human readable string of the vertex.
        //
        // /////////////////////////////////////////////////////////////////
        std::string ToString() const;

    };

    // /////////////////////////////////////////////////////////////////
    // @class Triangle
    // @author PJ O Halloran
    //
    // This class represents a 3D Triangle.
    //
    // /////////////////////////////////////////////////////////////////
    class Triangle {
    private:

        Vertex m_vertices[3];

    public:

        // /////////////////////////////////////////////////////////////////
        // @enum VertexId
        //
        // Safer vertex access enum.
        // eOne:            Index to vertex one.
        // eTwo:            Index to vertex two.
        // eThree:          Index to vertex three.
        // eNumberVertices: Number of vertices in the triangle (3!).
        //
        // /////////////////////////////////////////////////////////////////
        enum VertexId {
            eOne = 0,
            eTwo,
            eThree,
            eNumberVertices
        };

        // /////////////////////////////////////////////////////////////////
        // Default constructor.
        //
        // /////////////////////////////////////////////////////////////////
        explicit inline Triangle() : m_vertices() {};

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // Vertices should be supplied in counter clockwise order.
        //
        // @param one First vertex.
        // @param two Second vertex.
        // @param three Third vertex.
        //
        // /////////////////////////////////////////////////////////////////
        explicit inline Triangle(const Vertex &one, const Vertex &two, const Vertex &three) : m_vertices() {
            m_vertices[0] = one;
            m_vertices[1] = two;
            m_vertices[2] = three;
        };

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        ~Triangle() {};

        // /////////////////////////////////////////////////////////////////
        // Get a specific vertex.
        //
        // @param index The vertex Id/index.
        // @param vertex Object to copy the vertex to.
        //
        // @return bool False if the index is unknown/out of bounds.
        //
        // /////////////////////////////////////////////////////////////////
        bool GetVertex(const enum VertexId index, Vertex &vertex) const;

        // /////////////////////////////////////////////////////////////////
        // Set a specific vertex.
        //
        // @param index The vertex Id/index.
        // @param vertex Object to set as the vertex.
        //
        // @return bool False if the index is unknown/out of bounds.
        //
        // /////////////////////////////////////////////////////////////////
        bool SetVertex(const enum VertexId index, const Vertex &vertex);

        // /////////////////////////////////////////////////////////////////
        // Calculate and return the triangles unit normal vector.
        //
        // /////////////////////////////////////////////////////////////////
        Vector3 CalculateNormal() const;

        // /////////////////////////////////////////////////////////////////
        // Calculate the triangles surface area.
        //
        // /////////////////////////////////////////////////////////////////
        F32 CalculateArea() const;

        // /////////////////////////////////////////////////////////////////
        // Calculate the centre point of the triangle.
        //
        // /////////////////////////////////////////////////////////////////
        Point3 CalculateCentrePoint() const;

        // /////////////////////////////////////////////////////////////////
        // Calculate the radius of the triangle.
        //
        // /////////////////////////////////////////////////////////////////
        F32 CalculateRadius() const;

        // /////////////////////////////////////////////////////////////////
        // Checks that the triangle is valid.  That all its vertices are
        // compatible to be sent in the same batch of geometry.
        //
        // /////////////////////////////////////////////////////////////////
        bool IsValid() const;

        // /////////////////////////////////////////////////////////////////
        // Produces a human readable string of the triangle.
        //
        // /////////////////////////////////////////////////////////////////
        std::string ToString() const {
            const std::string wsp(" ");
            return (std::string(m_vertices[0].ToString() + wsp + m_vertices[1].ToString() + wsp + m_vertices[2].ToString()));
        }

    };

    // Typedef for a container of triangles and a shared pointer.
    typedef boost::shared_ptr<Triangle> TriangleSharePtr;
    typedef std::list<TriangleSharePtr > TriangleList;
    // TriangleList & TriangleMesh are interchangeable terms...
    typedef TriangleList TriangleMesh;

    // /////////////////////////////////////////////////////////////////
    // Given a list of triangles (a mesh), calculate the bounding sphere.
    // This assumes the centre point of the model is at 0, 0, 0.
    //
    // The bounding sphere retrieved here is in MODEL space coordinates
    // and should be transformed into world coordinates before typical
    // use.
    //
    // @param tList Triangle list/mesh.
    // @param bs The BoundingSphere.
    //
    // /////////////////////////////////////////////////////////////////
    void CalculateTriangleListBoundingSphere(const TriangleMesh &tList, BoundingSphere &bs);

    // /////////////////////////////////////////////////////////////////
    // Given a list of triangles (a mesh), calculate a tight fitting
    // bounding box.
    // This assumes the centre point of the model is at 0, 0, 0.
    //
    // The bounding box retrieved here is in MODEL space coordinates
    // and should be transformed into world coordinates before typical
    // use.
    //
    // @param tList Triangle list/mesh.
    // @param bs The BoundingCube.
    //
    // /////////////////////////////////////////////////////////////////
    void CalculateTriangleListBoundingBox(const TriangleMesh &tList, BoundingCube &bc);

    // /////////////////////////////////////////////////////////////////
    // Extracts a list of points from a triangle mesh.
    //
    // @param tList Traingle List/Mesh.
    // @param pList List of points.
    //
    // /////////////////////////////////////////////////////////////////
    void GetPointListFromTriangleMesh(const TriangleMesh &tList, Point3ValList &pList);

}

#endif
