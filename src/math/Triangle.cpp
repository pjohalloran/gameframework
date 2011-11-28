// /////////////////////////////////////////////////////////////////
// @file Triangle.cpp
// @author PJ O Halloran
// @date 03/12/2010
//
// File contains the implemenation for the Triangle and Vertex classes.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <algorithm>

#include <boost/lexical_cast.hpp>


// Project Headers
#include "GameBase.h"
#include "Triangle.h"


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// *********************** Vertex **********************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Vertex::operator==(const Vertex &rhs) const
	{
		if(this == &rhs)
		{
			// rhs is an alias for this object!
			return (true);
		}

		if(!m_texCoordsVec.is_initialized() || !rhs.m_texCoordsVec.is_initialized())
		{
			// One of them is not used so they can't be equal!
			return (false);
		}

		TexCoordsVec thisVec = *m_texCoordsVec;
		TexCoordsVec rhsVec = *rhs.m_texCoordsVec;

		//bool result = std::lexicographical_compare(thisVec.begin(), thisVec.end(), rhsVec.begin(), rhsVec.end(), CompareTextureVectorEqual);
		bool result = std::equal(thisVec.begin(), thisVec.end(), rhsVec.begin());

		if(!result)
		{
			return (false);
		}

		return ((m_position == rhs.m_position) && (m_normal == rhs.m_normal) && (m_color == rhs.m_color));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Vertex::operator<(const Vertex &rhs) const
	{
		if(this == &rhs)
		{
			// rhs is an alias for this object so it is NOT less than!
			return (false);
		}

		if(!m_texCoordsVec.is_initialized() || !rhs.m_texCoordsVec.is_initialized())
		{
			// One of them is not used so they can't be less than!
			return (false);
		}

		TexCoordsVec thisVec = *m_texCoordsVec;
		TexCoordsVec rhsVec = *rhs.m_texCoordsVec;

		bool result = std::lexicographical_compare(thisVec.begin(), thisVec.end(), rhsVec.begin(), rhsVec.end());

		if(!result)
		{
			return (false);
		}

		return ((m_position < rhs.m_position) && (m_normal < rhs.m_normal) && (m_color < rhs.m_color));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Vertex::GetNormal(Vector3 &normal) const
	{
		if(HasNormal())
		{
			normal = *m_normal;
			return (true);
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Vertex::SetNormal(const Vector3 &normal)
	{
		m_normal = normal;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Vertex::HasTextureCoordinate(const U32 textureUnit) const
	{
		if(!HasAnyTextureCoordinates())
		{
			return (false);
		}

		if(TexCoordsVec::size_type(textureUnit) > m_texCoordsVec->size() - 1)
		{
			return (false);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Vertex::GetTextureCoordinateContainer(TexCoordsVec &texCoordVec) const
	{
		if(!HasAnyTextureCoordinates())
		{
			return (false);
		}

		texCoordVec.reserve(m_texCoordsVec->size());
		std::copy(m_texCoordsVec->begin(), m_texCoordsVec->end(), texCoordVec.begin());
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	U32 Vertex::GetNumberTextureUnits() const
	{
		if(!HasAnyTextureCoordinates())
		{
			return (0);
		}

		return (U32(m_texCoordsVec->size()));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Vertex::GetTextureCoordinate(const U32 textureUnit, Vector3 &texCoord) const
	{
		if(!HasAnyTextureCoordinates())
		{
			return (false);
		}

		try
		{
			texCoord = m_texCoordsVec->at(textureUnit);
		}
		catch(...)
		{
			// Index out of bounds exception occurred in at()!
			return (false);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	U32 Vertex::AddTextureCoordinate(const Vector3 &texCoords)
	{
		if(!HasAnyTextureCoordinates())
		{
			// Initialize vector.
			m_texCoordsVec = TexCoordsVec();
		}

		m_texCoordsVec->push_back(texCoords);
		return (U32(m_texCoordsVec->size() - 1));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Vertex::GetColor(Vector4 &color) const
	{
		if(HasColor())
		{
			color = *m_color;
			return (true);
		}

		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Vertex::SetColor(const Vector4 &color)
	{
		m_color = color;
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Vertex::IsCompatibleWith(const Vertex &rhs) const
	{
		// Check that both vertices have the same attributes set.
		return ((HasNormal() == rhs.HasNormal()) &&\
					(HasColor() == rhs.HasColor()) &&\
					(HasAnyTextureCoordinates() == rhs.HasAnyTextureCoordinates()) &&\
					(GetNumberTextureUnits() == rhs.GetNumberTextureUnits()));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	std::string Vertex::ToString() const
	{
		const std::string wsp(" "), endln("\n");
		std::string str(std::string("VP: ") + m_position.ToString() + endln);
		if(HasNormal())
		{
			str += wsp + std::string("VN: ") + m_normal->ToString() + endln;
		}
		if(HasColor())
		{
			str += wsp + std::string("VC: ") + m_color->ToString() + endln;
		}
		if(HasAnyTextureCoordinates())
		{
			std::string indexStr;
			I32 index = 0;
			for(TexCoordsVec::const_iterator i = m_texCoordsVec->begin(), end = m_texCoordsVec->end(); i != end; ++i, ++index)
			{
				try { indexStr = boost::lexical_cast<std::string, I32>(index); } catch(...) {}
				str += wsp + std::string("TC ") + indexStr + wsp + (*i).ToString() + endln;
			}
		}
		return (str);
	}

	// /////////////////////////////////////////////////////////////////
	// ********************** Triangle *********************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Triangle::GetVertex(const enum VertexId index, Vertex &vertex) const
	{
		if(index < eNumberVertices)
		{
			vertex = m_vertices[index];
			return (true);
		}
		
		return (false);
	};

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Triangle::SetVertex(const enum VertexId index, const Vertex &vertex)
	{
		if(index < eNumberVertices)
		{
			m_vertices[index] = vertex;
			return (true);
		}
		
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Vector3 Triangle::CalculateNormal() const
	{
		Vector3 normal, oneVec(m_vertices[eOne].GetPosition());
		Vector3 u(Vector3(m_vertices[eTwo].GetPosition()) - oneVec);
		Vector3 v(Vector3(m_vertices[eThree].GetPosition()) - oneVec);

		u.Cross(v, normal);

		return (normal.Normalize());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	F32 Triangle::CalculateArea() const
	{
		Vector3 normal, oneVec(m_vertices[eOne].GetPosition());
		Vector3 u(Vector3(m_vertices[eTwo].GetPosition()) - oneVec);
		Vector3 v(Vector3(m_vertices[eThree].GetPosition()) - oneVec);

		u.Cross(v, normal);

		F32 area = normal.Magnitude() * 0.5f;
		return (area);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Point3 Triangle::CalculateCentrePoint() const
	{
		Point3 onePt(m_vertices[eOne].GetPosition());
		Point3 twoPt(m_vertices[eTwo].GetPosition());
		Point3 threePt(m_vertices[eThree].GetPosition());

		Point3 centre12, centrePt;
		Point3Lerp(onePt, twoPt, 0.5f, centre12);
		Point3Lerp(centre12, threePt, 0.5f, centrePt);

		return (centrePt);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	F32 Triangle::CalculateRadius() const
	{
		Vector3 centreVec(CalculateCentrePoint());
		Vector3 oneVec(m_vertices[eOne].GetPosition());

		Vector3 diff(centreVec - oneVec);
		F32 radius = diff.Magnitude();

		return (radius);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Triangle::IsValid() const
	{
		if(!m_vertices[eOne].IsCompatibleWith(m_vertices[eTwo]))
		{
			return (false);
		}

		if(!m_vertices[eTwo].IsCompatibleWith(m_vertices[eThree]))
		{
			return (false);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	// ******************* MISC Helper Functions ***********************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CalculateTriangleListBoundingSphere(const TriangleMesh &tList, BoundingSphere &bs)
	{
		Vertex currVertex;				// Current vertex in the current triangle.
		F32 currDistance;			// Distance from the origin point to the current vertex.
		F32 maxDistance = 0.0f;	// Maximum distance found from the origin to any vertex in the mesh to date.

		// Ensure the centre point of the bounding sphere is the origin (MODEL space).
		bs.SetCentre(g_originPt);

		if(tList.empty())
		{
			// Invalid sphere!
			bs.SetRadius(-1.0f);
			return;
		}

		// Finds the distance from the origin to the furthest away vertex in the mesh ( = BS radius).

		for(TriangleMesh::const_iterator i = tList.begin(), end = tList.end(); i != end; ++i)
		{
			for(I32 vi = 0; vi < Triangle::eNumberVertices; ++vi)
			{
				(*i)->GetVertex(Triangle::VertexId(vi), currVertex);
				currDistance = g_originPt.Distance(currVertex.GetPosition());
				if(currDistance > maxDistance)
				{
					maxDistance = currDistance;
				}
			}
		}

		bs.SetRadius(maxDistance);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CalculateTriangleListBoundingBox(const TriangleMesh &tList, BoundingCube &bc)
	{
		Vertex currVertex;									// Current vertex in the current triangle.
		F32 minX = 0.0f, minY = 0.0f, minZ = 0.0f;		// Minimum point found.
		F32 maxX = 0.0f, maxY = 0.0f, maxZ = 0.0f;		// Maximum point found.

		if(tList.empty())
		{
			// Invalid box!
			bc.SetMin(g_originPt);
			bc.SetMax(Point3(-1.0f, -1.0f, -1.0f));
			return;
		}

		// Finds the minimum and maximum vertex points in the mesh.

		for(TriangleMesh::const_iterator i = tList.begin(), end = tList.end(); i != end; ++i)
		{
			for(I32 vi = 0; vi < Triangle::eNumberVertices; ++vi)
			{
				(*i)->GetVertex(Triangle::VertexId(vi), currVertex);
				Point3 pos(currVertex.GetPosition());

				if(pos.GetX() < minX)
				{
					minX = pos.GetX();
				}
				if(pos.GetY() < minY)
				{
					minY = pos.GetY();
				}
				if(pos.GetZ() < minZ)
				{
					minZ = pos.GetZ();
				}

				if(pos.GetX() > maxX)
				{
					maxX = pos.GetX();
				}
				if(pos.GetY() > maxY)
				{
					maxY = pos.GetY();
				}
				if(pos.GetZ() > maxZ)
				{
					maxZ = pos.GetZ();
				}
			}
		}

		bc.SetMin(Point3(minX, minY, minZ));
		bc.SetMax(Point3(maxX, maxY, maxZ));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GetPointListFromTriangleMesh(const TriangleMesh &tList, Point3ValList &pList)
	{
		if(!pList.empty())
		{
			pList.clear();
		}

		Vertex currVertex;					// Current vertex in the current triangle.
		for(TriangleMesh::const_iterator i = tList.begin(), end = tList.end(); i != end; ++i)
		{
			for(I32 vi = 0; vi < Triangle::eNumberVertices; ++vi)
			{
				(*i)->GetVertex(Triangle::VertexId(vi), currVertex);
				pList.push_back(currVertex.GetPosition());
			}
		}
	}

}
