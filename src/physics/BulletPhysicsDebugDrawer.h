#pragma once
#ifndef __GF_BULLET_PHYSICS_DEBUG_DRAWER_H
#define __GF_BULLET_PHYSICS_DEBUG_DRAWER_H

// /////////////////////////////////////////////////////////////////
// @file BulletPhysicsDebugDrawer.h
// @author PJ O Halloran
// @date 14/10/2010
//
// Debug renderer for the Bullet physics API.
//
// /////////////////////////////////////////////////////////////////

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

#include <string>

#include <boost/shared_ptr.hpp>

#include <bullet/btBulletDynamicsCommon.h>

#include "GLSLShader.h"
#include "ModelViewProjStackManager.h"
#include "GameException.h"
#include "ZipFile.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class BulletPhysicsDebugDrawer
	// @author PJ O Halloran
	//
	// Bullet system debug renderer.
	//
	// /////////////////////////////////////////////////////////////////
	class BulletPhysicsDebugDrawer : public btIDebugDraw
	{
	private:
        
        static const U32 MAX_LINES = 5000;

		GLuint m_vaoId;															///< Vertex attribute array object.
		GLuint m_vboId;															///< Vertex buffer object.
		boost::shared_ptr<GLSLShader> m_shaderPtr;								///< Shader program used to render the debug info.
		boost::shared_ptr<ModelViewProjStackManager> m_mvpStackManagerPtr;		///< Pointer to the MVP stack manager.
		I32 m_debugMode;														///< Current debug mode.
		ShaderUniformSPtr m_mvpUniform;                                         ///< 
        ShaderUniformSPtr m_colorUniform;                                       ///< 
        U32 m_numberLines;                                                      ///< 
        
        
		// /////////////////////////////////////////////////////////////////
		// Creates the Vertex Buffer and Vertex attribute objects.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool CreateBuffers();

		// /////////////////////////////////////////////////////////////////
		// Loads the Vertex Buffer with enough data for 1 line.
		//
		// Note that this method assumes the GL VBO has been bound to already.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		bool LoadVertexBuffer(const btVector3 &from, const btVector3 &to);

        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        void ResetLines();
        
	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param Pointer to the MVP stack manager.
		// @param shaderName The name of the shader program to use (default:
		//						"shaders\\flat" the unlight colored, transformed, shader).
		//
		// @throw GameException& If input parameters are invalid.
		// @throw GameException& If we fail to create the GLSL shader program.
		//
		// /////////////////////////////////////////////////////////////////
		BulletPhysicsDebugDrawer(boost::shared_ptr<ModelViewProjStackManager> mvpStackManagerPtr, const std::string shaderName = std::string("shaders") + ZipFile::ZIP_PATH_SEPERATOR + std::string("flat")) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~BulletPhysicsDebugDrawer();

		// /////////////////////////////////////////////////////////////////
		// Draw a line between two 3D points with the color supplied.
		//
		// @param from Point A.
		// @param to Point B.
		// @param color The color of the line.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);

		// /////////////////////////////////////////////////////////////////
		// Draw a contact point  (the normal vector (i think)).
		//
		// @param PointOnB The origin of the normal vector.
		// @param normalOnB The normal vector.
		// @param distance The length of the normal.
		// @param lifetime How long the point will last before it will disappear.
		// @param color The color of the point.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, I32 lifeTime, const btVector3 &color);

		// /////////////////////////////////////////////////////////////////
		// Log an error.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void reportErrorWarning(const char *warningString);

		// /////////////////////////////////////////////////////////////////
		// Draw 3D text at the location in the scene.
		//
		// @param location The position of the text.
		// @param textString The string to render.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void draw3dText(const btVector3 &location, const char *textString);

        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        void BatchDraw();
        
		// /////////////////////////////////////////////////////////////////
		// Set the debug mode.
		//
		// The debug mode is expected to be a bit field therefore some helper
		// debug mode get and set methods are supplied below.  Using them
		// will be less error prone.
		//
		// @param debugMode The debug mode of the drawer.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void setDebugMode(I32 debugMode);

		// /////////////////////////////////////////////////////////////////
		// Get the debug mode.
		//
		// /////////////////////////////////////////////////////////////////
		virtual I32 getDebugMode() const;

		// /////////////////////////////////////////////////////////////////
		// Turn on a Bullet debug mode.
		//
		// /////////////////////////////////////////////////////////////////
		inline void DebugTurnOn(const enum btIDebugDraw::DebugDrawModes debugMode)
		{
			if(debugMode != DBG_MAX_DEBUG_DRAW_MODE)
			{
				m_debugMode = m_debugMode | debugMode;
			}
		};

		// /////////////////////////////////////////////////////////////////
		// Turn off a Bullet debug mode.
		//
		// /////////////////////////////////////////////////////////////////
		inline void DebugTurnOff(const enum btIDebugDraw::DebugDrawModes debugMode)
		{
			if(debugMode != DBG_MAX_DEBUG_DRAW_MODE)
			{
				m_debugMode = m_debugMode & (~debugMode);
			}
		};

		// /////////////////////////////////////////////////////////////////
		// Check if a Bullet debug mode is on or off.
		//
		// /////////////////////////////////////////////////////////////////
		inline bool DebugIsOn(const enum btIDebugDraw::DebugDrawModes debugMode)
		{
			if(debugMode == DBG_MAX_DEBUG_DRAW_MODE)
			{
				// invalid enum!
				return (false);
			}
			return ((m_debugMode & debugMode) > 0);
		};
	};


}

#ifdef WIN32
#	pragma warning( pop )
#endif

#endif
