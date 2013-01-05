// /////////////////////////////////////////////////////////////////
// @file Frustrum.cpp
// @author PJ O Halloran
// @date 26/08/2010
//
// File contains the implementation of the Frustrum class.
//
// /////////////////////////////////////////////////////////////////

#include "Frustrum.h"
#include "GLBatch.h"
#include "GameColors.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	Frustrum::Frustrum() : m_Fov(M3D_PI/4.0f), m_Aspect(1.0f), m_Near(1.0f), m_Far(1000.0f)
	{
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool Frustrum::Inside(const Point3 &pt) const
	{
		for (I32 i = 0; i < NumPlanes; ++i)
		{
			if (!m_Planes[i].Inside(pt))
			{
				return (false);
			}
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	bool Frustrum::Inside(const Point3 &pt, const F32 radius) const
	{
		for(I32 i = 0; i < NumPlanes; ++i) 
		{
			if(m_Planes[i].Inside(pt, radius))
			{
				return (false);
			}
		}	
		
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void Frustrum::Init(const F32 fov, const F32 aspect, const F32 nearClip, const F32 farClip)
	{
		m_Fov = F32(m3dDegToRad(fov));
		m_Aspect = aspect;
		m_Near = nearClip;
		m_Far = farClip;

		F32 tanFovOver2 = F32(tan(m_Fov/2.0f));
		F32 nearFov2 = m_Near * tanFovOver2;
		F32 farFov2 = m_Far * tanFovOver2;

		// Calculate the near & far right and near & far up vertices of the Frustrum.
		Vector3 nearRight = nearFov2 * m_Aspect * g_right;
		Vector3 farRight = farFov2 * m_Aspect * g_right;
		Vector3 nearUp = nearFov2 * g_up;
		Vector3 farUp = farFov2 * g_up;

		// points start in the upper right and go around clockwise
		m_NearClip[0] = (m_Near * g_forward) - nearRight + nearUp;
		m_NearClip[1] = (m_Near * g_forward) + nearRight + nearUp;
		m_NearClip[2] = (m_Near * g_forward) + nearRight - nearUp;
		m_NearClip[3] = (m_Near * g_forward) - nearRight - nearUp;

		m_FarClip[0] = (m_Far * g_forward) - farRight + farUp;
		m_FarClip[1] = (m_Far * g_forward) + farRight + farUp;
		m_FarClip[2] = (m_Far * g_forward) + farRight - farUp;
		m_FarClip[3] = (m_Far * g_forward) - farRight - farUp;

		m_Planes[Near] = Plane(m_NearClip[2], m_NearClip[1], m_NearClip[0]);
		m_Planes[Far] = Plane(m_FarClip[0], m_FarClip[1], m_FarClip[2]);
		m_Planes[Right] = Plane(m_FarClip[2], m_FarClip[1], g_originPt);
		m_Planes[Top] = Plane(m_FarClip[1], m_FarClip[0], g_originPt);
		m_Planes[Left] = Plane(m_FarClip[0], m_FarClip[3], g_originPt);
		m_Planes[Bottom] = Plane(m_FarClip[3], m_FarClip[2], g_originPt);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void Frustrum::Render()
	{
		// TODO: Include color info and write flat shader that uses vertex color attribute.

		// NB This method is not very efficient but does not need to be since the Frustrum
		//  will only be drawn when a developer is debugging scene culling issues, etc.
		GLBatch frustrumBatch;					// Batch of geometry for the Frustrum.
		//frustrumBatch.Begin(GL_LINE, 36);
		frustrumBatch.Begin(GL_LINES, 24);

			// Near clip plane
			frustrumBatch.Vertex3f(m_NearClip[0].GetX(), m_NearClip[0].GetY(), m_NearClip[0].GetZ());
			frustrumBatch.Vertex3f(m_NearClip[1].GetX(), m_NearClip[1].GetY(), m_NearClip[1].GetZ());

			frustrumBatch.Vertex3f(m_NearClip[1].GetX(), m_NearClip[1].GetY(), m_NearClip[1].GetZ());
			frustrumBatch.Vertex3f(m_NearClip[2].GetX(), m_NearClip[2].GetY(), m_NearClip[2].GetZ());

			frustrumBatch.Vertex3f(m_NearClip[2].GetX(), m_NearClip[2].GetY(), m_NearClip[2].GetZ());
			frustrumBatch.Vertex3f(m_NearClip[3].GetX(), m_NearClip[3].GetY(), m_NearClip[3].GetZ());

			frustrumBatch.Vertex3f(m_NearClip[3].GetX(), m_NearClip[3].GetY(), m_NearClip[3].GetZ());
			frustrumBatch.Vertex3f(m_NearClip[0].GetX(), m_NearClip[0].GetY(), m_NearClip[0].GetZ());

			// Far clip plane.
			frustrumBatch.Vertex3f(m_FarClip[0].GetX(), m_FarClip[0].GetY(), m_FarClip[0].GetZ());
			frustrumBatch.Vertex3f(m_FarClip[1].GetX(), m_FarClip[1].GetY(), m_FarClip[1].GetZ());

			frustrumBatch.Vertex3f(m_FarClip[1].GetX(), m_FarClip[1].GetY(), m_FarClip[1].GetZ());
			frustrumBatch.Vertex3f(m_FarClip[2].GetX(), m_FarClip[2].GetY(), m_FarClip[2].GetZ());

			frustrumBatch.Vertex3f(m_FarClip[2].GetX(), m_FarClip[2].GetY(), m_FarClip[2].GetZ());
			frustrumBatch.Vertex3f(m_FarClip[3].GetX(), m_FarClip[3].GetY(), m_FarClip[3].GetZ());

			frustrumBatch.Vertex3f(m_FarClip[3].GetX(), m_FarClip[3].GetY(), m_FarClip[3].GetZ());
			frustrumBatch.Vertex3f(m_FarClip[0].GetX(), m_FarClip[0].GetY(), m_FarClip[0].GetZ());


			// Edges between near and far planes
			frustrumBatch.Vertex3f(m_NearClip[0].GetX(), m_NearClip[0].GetY(), m_NearClip[0].GetZ());
			frustrumBatch.Vertex3f(m_FarClip[0].GetX(), m_FarClip[0].GetY(), m_FarClip[0].GetZ());

			frustrumBatch.Vertex3f(m_NearClip[1].GetX(), m_NearClip[1].GetY(), m_NearClip[1].GetZ());
			frustrumBatch.Vertex3f(m_FarClip[1].GetX(), m_FarClip[1].GetY(), m_FarClip[1].GetZ());

			frustrumBatch.Vertex3f(m_NearClip[2].GetX(), m_NearClip[2].GetY(), m_NearClip[2].GetZ());
			frustrumBatch.Vertex3f(m_FarClip[2].GetX(), m_FarClip[2].GetY(), m_FarClip[2].GetZ());

			frustrumBatch.Vertex3f(m_NearClip[3].GetX(), m_NearClip[3].GetY(), m_NearClip[3].GetZ());
			frustrumBatch.Vertex3f(m_FarClip[3].GetX(), m_FarClip[3].GetY(), m_FarClip[3].GetZ());

			//// Front
			//frustrumBatch.Vertex3f(m_NearClip[0].GetX(), m_NearClip[0].GetY(), m_NearClip[0].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[1].GetX(), m_NearClip[1].GetY(), m_NearClip[1].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[3].GetX(), m_NearClip[3].GetY(), m_NearClip[3].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[3].GetX(), m_NearClip[3].GetY(), m_NearClip[3].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[1].GetX(), m_NearClip[1].GetY(), m_NearClip[1].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[2].GetX(), m_NearClip[2].GetY(), m_NearClip[2].GetZ());
			//// Left
			//frustrumBatch.Vertex3f(m_FarClip[0].GetX(), m_FarClip[0].GetY(), m_FarClip[0].GetZ());
			//frustrumBatch.Vertex3f(m_FarClip[1].GetX(), m_FarClip[1].GetY(), m_FarClip[1].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[1].GetX(), m_NearClip[1].GetY(), m_NearClip[1].GetZ());
			//frustrumBatch.Vertex3f(m_FarClip[0].GetX(), m_FarClip[0].GetY(), m_FarClip[0].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[2].GetX(), m_NearClip[2].GetY(), m_NearClip[2].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[0].GetX(), m_NearClip[0].GetY(), m_NearClip[0].GetZ());
			//// Right
			//frustrumBatch.Vertex3f(m_FarClip[3].GetX(), m_FarClip[3].GetY(), m_FarClip[3].GetZ());
			//frustrumBatch.Vertex3f(m_FarClip[2].GetX(), m_FarClip[2].GetY(), m_FarClip[2].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[1].GetX(), m_NearClip[1].GetY(), m_NearClip[1].GetZ());
			//frustrumBatch.Vertex3f(m_FarClip[3].GetX(), m_FarClip[3].GetY(), m_FarClip[3].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[2].GetX(), m_NearClip[2].GetY(), m_NearClip[2].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[3].GetX(), m_NearClip[3].GetY(), m_NearClip[3].GetZ());
			//// Top
			//frustrumBatch.Vertex3f(m_FarClip[0].GetX(), m_FarClip[0].GetY(), m_FarClip[0].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[0].GetX(), m_NearClip[0].GetY(), m_NearClip[0].GetZ());
			//frustrumBatch.Vertex3f(m_FarClip[3].GetX(), m_FarClip[3].GetY(), m_FarClip[3].GetZ());
			//frustrumBatch.Vertex3f(m_FarClip[3].GetX(), m_FarClip[3].GetY(), m_FarClip[3].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[0].GetX(), m_NearClip[0].GetY(), m_NearClip[0].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[3].GetX(), m_NearClip[3].GetY(), m_NearClip[3].GetZ());
			//// Bottom
			//frustrumBatch.Vertex3f(m_FarClip[2].GetX(), m_FarClip[2].GetY(), m_FarClip[2].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[2].GetX(), m_NearClip[2].GetY(), m_NearClip[2].GetZ());
			//frustrumBatch.Vertex3f(m_FarClip[1].GetX(), m_FarClip[1].GetY(), m_FarClip[1].GetZ());
			//frustrumBatch.Vertex3f(m_FarClip[1].GetX(), m_FarClip[1].GetY(), m_FarClip[1].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[2].GetX(), m_NearClip[2].GetY(), m_NearClip[2].GetZ());
			//frustrumBatch.Vertex3f(m_NearClip[1].GetX(), m_NearClip[1].GetY(), m_NearClip[1].GetZ());
			//// Back
			//frustrumBatch.Vertex3f(m_FarClip[3].GetX(), m_FarClip[3].GetY(), m_FarClip[3].GetZ());
			//frustrumBatch.Vertex3f(m_FarClip[2].GetX(), m_FarClip[2].GetY(), m_FarClip[2].GetZ());
			//frustrumBatch.Vertex3f(m_FarClip[0].GetX(), m_FarClip[0].GetY(), m_FarClip[0].GetZ());
			//frustrumBatch.Vertex3f(m_FarClip[2].GetX(), m_FarClip[2].GetY(), m_FarClip[2].GetZ());
			//frustrumBatch.Vertex3f(m_FarClip[1].GetX(), m_FarClip[1].GetY(), m_FarClip[1].GetZ());
			//frustrumBatch.Vertex3f(m_FarClip[0].GetX(), m_FarClip[0].GetY(), m_FarClip[0].GetZ());
		frustrumBatch.End();


		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		frustrumBatch.VDraw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}
