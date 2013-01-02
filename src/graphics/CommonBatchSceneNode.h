// /////////////////////////////////////////////////////////////////
// @file CommonBatchSceneNode.h
// @author PJ O Halloran
// @date 18/11/2010
//
// Contains the CommonBatchSceneNode classes definition.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GF_COMMON_BATCH_SCENE_NODE_H
#define __GF_COMMON_BATCH_SCENE_NODE_H

// External Headers
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include "GameBase.h"

#include <string>
#include <vector>

// Project Headers
#include "ISceneNode.h"
#include "SceneNodeProperties.h"
#include "SceneNode.h"
#include "TextureManager.h"
#include "IGLBatchBase.h"
#include "GameException.h"

namespace GameHalloran
{

	class SceneGraphManager;

	// /////////////////////////////////////////////////////////////////
	// @class CommonBatchSceneNode
	// @author PJ O Halloran
	//
	// A scene node that renders a batch of geometry.  This should be
	// extended by game objects as it does not do very much apart from
	// render the batch of geometry with whatever shader happens to be
	// set up and activated by the SGM currently.
	//
	// The sub classes should contain specific instances of ActorParam
	// parameter objects.
	//
	// /////////////////////////////////////////////////////////////////
	class CommonBatchSceneNode : public SceneNode
	{
	private:

		boost::shared_ptr<IGLBatchBase> m_batchPtr;				///< Batch of geometry.
		boost::optional<TexHandle> m_texHandle;					///< Handle for the texture to apply to the geometry.

        ShaderUniformSPtr m_mvpUniform;                         ///< 
        ShaderUniformSPtr m_colorUniform;                       ///< Default shader uniforms.
        
		// /////////////////////////////////////////////////////////////////
		// Constructor helper function.
		//
		// @param textureName The name of the texture to apply.
		// @param shaderNameRef The name of the custom shader to use.
		//
		// /////////////////////////////////////////////////////////////////
		void Init(const std::string &textureName, const std::string &shaderNameRef);

	protected:

		// /////////////////////////////////////////////////////////////////
		// Set the batch of geometry.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetBatch(boost::shared_ptr<IGLBatchBase> newBatch) { m_batchPtr = newBatch; };

		// /////////////////////////////////////////////////////////////////
		// Check if we have a valid batch of geometry.
		//
		// /////////////////////////////////////////////////////////////////
		inline bool IsBatchValid() const { return (m_batchPtr.get() != NULL); };

		// /////////////////////////////////////////////////////////////////
		// Set the texture handle.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetTextureHandle(boost::optional<TexHandle> texHandle) { m_texHandle = texHandle; };

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
        // @param sgPtr SG manager pointer.
		// @param actorId The optional Actor ID.
		// @param nodeName The name of the node.
		// @param renderPass The render pass to add the node to.
		// @param material The objects material.
		// @param toWorld Position and orientation of node relative to its
		//					parent.
		// @param textureName Name of texture to apply to the batch.
		// @param shaderNameRef Name of the custom shader to render the node
		//						(SGMs' global shader will be used if this is empty).
		// @param batchPtr Pointer to the batch (created externally).
		//
		// /////////////////////////////////////////////////////////////////
		explicit CommonBatchSceneNode(SceneGraphManager *sgPtr, boost::optional<ActorId> actorId, const std::string &nodeName, const RenderPass renderPass, const Material &material, const Matrix4 &toWorld, const std::string &textureName, const std::string &shaderNameRef, boost::shared_ptr<IGLBatchBase> batchPtr);

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
        // @param sgPtr SG manager pointer.
		// @param actorId The optional Actor ID.
		// @param nodeName The name of the node.
		// @param renderPass The render pass to add the node to.
		// @param material The objects material.
		// @param toWorld Position and orientation of node relative to its
		//					parent.
		// @param fromWorld Inverse of toWorld matrix.
		// @param textureName Name of texture to apply to the batch.
		// @param shaderNameRef Name of the shader to render the node.
		// @param batchPtr Pointer to the batch (created externally).
		//
		// /////////////////////////////////////////////////////////////////
		explicit CommonBatchSceneNode(SceneGraphManager *sgPtr, boost::optional<ActorId> actorId, const std::string &nodeName, const RenderPass renderPass, const Material &material, const Matrix4 &toWorld, const Matrix4 &fromWorld, const std::string &textureName, const std::string &shaderNameRef, boost::shared_ptr<IGLBatchBase> &batchPtr);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~CommonBatchSceneNode();

		// /////////////////////////////////////////////////////////////////
		// Set the render state before rendering.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VPreRender();

		// /////////////////////////////////////////////////////////////////
		// Render the node.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VRender();

	};

}

#endif
