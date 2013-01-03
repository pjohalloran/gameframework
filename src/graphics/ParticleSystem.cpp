// /////////////////////////////////////////////////////////////////
// @file ParticleSystem.cpp
// @author PJ O Halloran
// @date 29/09/2010
//
// File contains the implementation for the Particle and ParticleSystem 
// classes.
//
// /////////////////////////////////////////////////////////////////

#include <string>

#include "ParticleSystem.h"
#include "GameBase.h"
#include "GameMain.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// ********************** Particle *********************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Particle::GetMatrix(Matrix4 &mat) const
	{
		Matrix4 rotMat;
		GameHalloran::BuildRotationArbitraryMatrix4(rotMat, m_rotateAngle, m_rotationAxis.GetX(), m_rotationAxis.GetY(), m_rotationAxis.GetZ());
		GameHalloran::BuildTranslationMatrix4(mat, m_position.GetX(), m_position.GetY(), m_position.GetZ());
		mat *= rotMat;
	}

	// /////////////////////////////////////////////////////////////////
	// ******************** ParticleSystem *****************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ParticleSystem::VRemoveDeadParticles()
	{
		for(ParticleList::iterator curr = m_list.begin(); curr != m_list.end();)
		{
			if(!(*curr)->IsAlive())
			{
				curr = m_list.erase(curr);
			}
			else
			{
				++curr;
			}
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ParticleSystem::ParticleSystem()
							: m_origin()
							, m_boundBox()
							, m_emitRate(0.0f)
							, m_maxParticles(0)
							, m_windDir()
							, m_gravity(0.0f)
							, m_textureResource(std::string(""))
							, m_textureId(0)
							, m_shaderProg()
							, m_list()
							, m_pointSpritesBatch()
							, m_rng()
	{
		glGenTextures(1, &m_textureId);
		m_rng.Randomize();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ParticleSystem::ParticleSystem(const Point3 &pt,
									const BoundingCube &bb,
									const F32 emitRate,
									const U32 maxParticles,
									const ImageResource &textureResource,
									const Vector3 &windVec,
									const F32 gravity)
									: m_origin(pt)
									, m_boundBox(bb)
									, m_emitRate(emitRate)
									, m_maxParticles(maxParticles)
									, m_windDir(windVec)
									, m_gravity(gravity)
									, m_textureResource(textureResource)
									,  m_textureId(0)
									, m_shaderProg()
									, m_list()
									, m_pointSpritesBatch()
									, m_rng()
	{
		glGenTextures(1, &m_textureId);
		m_rng.Randomize();
		// Load the texture.
		VOnRestore();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ParticleSystem::~ParticleSystem()
	{
		try
		{
			if(IsTextureAssigned())
			{
				glDeleteTextures(1, &m_textureId);
			}

			m_list.clear();
			m_pointSpritesBatch.Reset();
		}
		catch(...)
		{
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ParticleSystem::VOnRestore()
	{
		bool result = true;

		if(m_textureResource.GetName().empty())
		{
            GF_LOG_TRACE_DEB("ParticleSystem::VOnRestore()", "The particle system was not yet assigned a texture to reload");
			result = false;
		}

		if(result)
		{
			boost::shared_ptr<ImageResHandle> imgH = boost::static_pointer_cast<ImageResHandle>(g_appPtr->GetResourceCache()->GetHandle(&m_textureResource));
			if(!imgH || !imgH->VInitialize())
			{
                GF_LOG_TRACE_ERR("ParticleSystem::VOnRestore()", std::string("Failed to load the resource ") + m_textureResource.GetName());
				result = false;
			}

			if(result)
			{
                GF_CLEAR_GL_ERROR();
                
				if(!IsTextureAssigned())
				{
					glGenTextures(1, &m_textureId);
                    GF_CHECK_GL_ERROR();
				}

				glBindTexture(GL_TEXTURE_2D, m_textureId);
                GF_CHECK_GL_ERROR();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                GF_CHECK_GL_ERROR();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                GF_CHECK_GL_ERROR();
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                GF_CHECK_GL_ERROR();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                GF_CHECK_GL_ERROR();
				
				if(FindImageTypeFromFile(m_textureResource.GetName()) == IMAGE_TYPE_TGA)
				{
					glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                    GF_CHECK_GL_ERROR();
				}

				glTexImage2D(GL_TEXTURE_2D,
								0,
								imgH->GetImageComponents(),
								imgH->GetImageWidth(),
								imgH->GetImageHeight(),
								0,
								imgH->GetImageFormat(),
								GL_UNSIGNED_BYTE,
								imgH->GetImageBuffer());
				result = GF_CHECK_GL_ERROR();
			}
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ParticleSystem::VOnLostDevice()
	{
		if(IsTextureAssigned())
		{
            GF_CLEAR_GL_ERROR();
			glDeleteTextures(1, &m_textureId);
            GF_CHECK_GL_ERROR();
            
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ParticleSystem::VOnPreRender()
	{
        GF_CLEAR_GL_ERROR();
		glEnable(GL_PROGRAM_POINT_SIZE);
        GF_CHECK_GL_ERROR();
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ParticleSystem::VOnRender(const F64 time, const F32 elapsedTime)
	{
		if(IsTextureAssigned())
		{
            GF_CLEAR_GL_ERROR();
			glBindTexture(GL_TEXTURE_2D, m_textureId);
            GF_CHECK_GL_ERROR();
		}
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ParticleSystem::VOnPostRender()
	{
        GF_CLEAR_GL_ERROR();
		glDisable(GL_PROGRAM_POINT_SIZE);
        GF_CHECK_GL_ERROR();
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ParticleSystem::VReset()
	{
		for(ParticleList::iterator curr = m_list.begin(), end = m_list.end(); curr != end; ++curr)
		{
			VResetParticle(*curr);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ParticleSystem::VAddParticle()
	{
		boost::shared_ptr<Particle> particle(GCC_NEW Particle());
		if(!particle)
		{
            GF_LOG_FAT("Failed to allocate memory to add a particle");
		}

		VResetParticle(particle);
		m_list.push_back(particle);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ParticleSystem::IsDead()
	{
		bool aliveFound = false;				// Did we find any live particles?

		for(ParticleList::iterator curr = m_list.begin(), end = m_list.end(); ((!aliveFound) && (curr != end)); ++curr)
		{
			if((*curr)->IsAlive())
			{
				aliveFound = true;
			}
		}

		return (!aliveFound);
	}

}
