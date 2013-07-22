//========================================================================
// Part of the GameCode3 Application
//
// GameCode3 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 3rd Edition" by Mike McShaffry, published by
// Charles River Media. ISBN-10: 1-58450-680-6   ISBN-13: 978-1-58450-680-5
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the author a favor and buy a new copy of the book - there are
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here:
//    http://www.amazon.com/gp/product/1584506806?ie=UTF8&tag=gamecodecompl-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=1584506806
//
// There's a companion web site at:
// http://www.mcshaffry.com/GameCode/
//
// The source code is managed and maintained through Google Code:
// http://gamecode3.googlecode.com/svn/trunk/
//
// (c) Copyright 2009 Michael L. McShaffry
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License v2
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================

// /////////////////////////////////////////////////////////////////
// @file SceneNode.cpp
// @author Mike McShaffry & PJ O Halloran
// @date 04/10/2010
//
// Contains the SceneNode classes implementation.
//
// /////////////////////////////////////////////////////////////////

#include <string>

#include "SceneNode.h"
#include "GameMain.h"
#include "SceneGraphManager.h"
#include "GLSLShader.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    SceneNode::SceneNode(SceneGraphManager *sgPtr,
                         boost::optional<ActorId> actorId,
                         const std::string &name,
                         const RenderPass renderPass,
                         const Material &material,
                         const Matrix4 &toWorld)\
:
    m_sgmPtr(sgPtr)
    , m_parentPtr(NULL)
    , m_props()
    , m_useCustomShader(false)
    , m_children()
    , m_shaderPtr()
    {
        m_props.SetActorId(actorId);
        m_props.SetName(name);
        m_props.SetRenderPass(renderPass);
        m_props.SetMaterial(material);
        VSetTransform(toWorld);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    SceneNode::SceneNode(SceneGraphManager *sgPtr,
                         boost::optional<ActorId> actorId,
                         const std::string &name,
                         const RenderPass renderPass,
                         const Material &material,
                         const Matrix4 &toWorld,
                         const Matrix4 &fromWorld)\
:
    m_sgmPtr(sgPtr)
    , m_parentPtr(NULL)
    , m_props()
    , m_useCustomShader(false)
    , m_children()
    , m_shaderPtr()
    {
        m_props.SetActorId(actorId);
        m_props.SetName(name);
        m_props.SetRenderPass(renderPass);
        m_props.SetMaterial(material);
        VSetTransform(toWorld, fromWorld);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    SceneNode::~SceneNode()
    {
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SceneNode::VPreRender()
    {
        // Check if we need to get the shader (On the first time through or else we changed to a different shader recently).
        if(m_useCustomShader) {
            if(!m_shaderPtr) {
                m_shaderPtr = m_sgmPtr->GetShader(m_props.GetShaderName());
                if(!m_shaderPtr) {
                    GF_LOG_TRACE_ERR("SceneNode::VPreRender()", std::string("Failed to get the custom shader from the SGM: ") + m_props.GetShaderName());
                    return (false);
                }
            }

            // Activate the custom GLSL program we will be using to render the node.
            if(!m_shaderPtr->Activate()) {
                GF_LOG_TRACE_ERR("SceneNode::VPreRender()", std::string("Failed to activate the custom shader: ") + m_props.GetShaderName());
                return (false);
            }
        }

        // Save the transformation state of the modelview matrix stack before we render
        //  and set the new state to be the old matrix * this nodes matrix.
        m_sgmPtr->GetStackManager()->GetModelViewMatrixStack()->PushMatrix();
        m_sgmPtr->GetStackManager()->GetModelViewMatrixStack()->MultiplyMatrix(m_props.GetToWorld());
        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SceneNode::VRender()
    {
        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SceneNode::VPostRender()
    {
        m_sgmPtr->GetStackManager()->GetModelViewMatrixStack()->PopMatrix();
        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void SceneNode::VSetTransform(const Matrix4 &toWorld)
    {
        m_props.SetToWorld(toWorld);
        Matrix4 inv;
        if(!toWorld.Inversed(inv)) {
            GF_LOG_TRACE_INF("SceneNode::VSetTransform()", "Failed to calculate an inverse for the nodes new toWorld matrix");
            inv.LoadIdentity();
        }

        m_props.SetFromWorld(inv);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void SceneNode::VSetTransform(const Matrix4 &toWorld, const Matrix4 &fromWorld)
    {
        m_props.SetToWorld(toWorld);
        m_props.SetFromWorld(fromWorld);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SceneNode::VAddChild(boost::shared_ptr<ISceneNode> childNodePtr)
    {
        if(!childNodePtr) {
            return (false);
        }

        m_children.push_back(childNodePtr);
        childNodePtr->VSetParentPtr(this);

        Matrix4 childToWorld = childNodePtr->VGet()->GetToWorld();
        F32 childRadius = childNodePtr->VGet()->GetRadius();

        Point3 childPos, myPos;
        childToWorld.GetPosition(childPos);
        m_props.GetToWorld().GetPosition(myPos);

        Vector3 dirToChild = childPos - myPos;

        F32 newRadius = dirToChild.Magnitude() + childRadius;
        if(newRadius > m_props.GetRadius()) {
            m_props.SetRadius(newRadius);
        }

        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SceneNode::VRemoveChild(const ActorId id)
    {
        bool found = false;
        for(SceneNodeList::iterator i = m_children.begin(); ((!found) && (i != m_children.end()));) {
            const SceneNodeProperties * const propsPtr = (*i)->VGet();
            if((propsPtr->GetActorId().is_initialized()) && (*propsPtr->GetActorId() == id)) {
                i = m_children.erase(i);
                found = true;
            } else {
                ++i;
            }
        }
        return (found);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void SceneNode::RenderSceneNode(ISceneNode *snPtr)
    {
        if(!snPtr) {
            return;
        }

        F32 alpha = snPtr->VGet()->GetAlpha();
        if(FloatCmp(alpha, g_OPAQUE)) {
            if(snPtr->VPreRender()) {
                snPtr->VRender();
            }
            snPtr->VPostRender();
        } else if(!FloatCmp(alpha, g_TRANSPARENT)) {
            Matrix4 mat;
            F32 z;

            m_sgmPtr->GetStackManager()->GetModelViewMatrixStack()->GetMatrix(mat);

            Vector4 worldPos4;
            mat.GetPosition(worldPos4);

            Matrix4 toWorldCamera(m_sgmPtr->GetCamera()->VGet()->GetToWorld());

            Vector4 eyePos4 = toWorldCamera * worldPos4;
            z = eyePos4.GetZ();

            boost::shared_ptr<AlphaSceneNode> asn(GCC_NEW AlphaSceneNode(boost::shared_ptr<ISceneNode>(snPtr), mat, z));
            if(!asn) {
                GF_LOG_TRACE_ERR("SceneNode::VRenderChildren()", "Failed to allocate memory for an alpha scene node");
            } else {
                m_sgmPtr->AddAlphaSceneNode(asn);
            }
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SceneNode::VRenderChildren()
    {
        bool result = true;
        for(SceneNodeList::iterator i = m_children.begin(), end = m_children.end(); i != end; ++i) {
            if((*i)->VIsVisible()) {
                RenderSceneNode((*i).get());
            }

            (*i)->VRenderChildren();
        }
        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SceneNode::VOnRestore()
    {
        bool result = true;
        for(SceneNodeList::iterator i = m_children.begin(), end = m_children.end(); i != end; ++i) {
            if(!(*i)->VOnRestore()) {
                result = false;
            }
        }
        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SceneNode::VOnLostDevice()
    {
        bool result = true;
        for(SceneNodeList::iterator i = m_children.begin(), end = m_children.end(); i != end; ++i) {
            if(!(*i)->VOnLostDevice()) {
                result = false;
            }
        }
        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SceneNode::VIsVisible() const
    {
        Matrix4 cameraTransform(m_sgmPtr->GetCamera()->VGet()->GetToWorld());
        Matrix4 cameraInvTransform(m_sgmPtr->GetCamera()->VGet()->GetFromWorld());

        Vector4 posWorld4;
        m_props.GetToWorld().GetPosition(posWorld4);

        Vector4 posCamEyeSpace4 = cameraTransform * posWorld4;
        Vector3 posCamEyeSpace3(posCamEyeSpace4);
        Point3 eyePt(posCamEyeSpace3);

        return (m_sgmPtr->GetCamera()->GetFrustum()->Inside(eyePt, m_props.GetRadius()));
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SceneNode::VPick(const RayCast &ray)
    {
        if(m_children.empty()) {
            return (false);
        }

        bool result = true;
        for(SceneNodeList::iterator i = m_children.begin(), end = m_children.end(); i != end; ++i) {
            if(!(*i)->VPick(ray)) {
                result = false;
            }
        }
        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool SceneNode::VOnUpdate(const F32 elapsedTime)
    {
        bool result = true;
        for(SceneNodeList::iterator i = m_children.begin(), end = m_children.end(); i != end; ++i) {
            if(!(*i)->VOnUpdate(elapsedTime)) {
                result = false;
            }
        }
        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    Point3 SceneNode::GetPosition() const
    {
        Point3 pos;
        m_props.GetToWorld().GetPosition(pos);
        return (pos);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void SceneNode::SetPosition(const Point3 &pos)
    {
        Matrix4 toWorld(m_props.GetToWorld());
        toWorld[Matrix4::M30] = pos.GetX();
        toWorld[Matrix4::M31] = pos.GetY();
        toWorld[Matrix4::M32] = pos.GetZ();
        toWorld[Matrix4::M33] = 1.0f;

        VSetTransform(toWorld);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void SceneNode::SetShaderName(const std::string &shaderNameRef)
    {
        m_useCustomShader = true;
        m_props.SetShaderName(shaderNameRef);
        m_shaderPtr = m_sgmPtr->GetShader(m_props.GetShaderName());
    };

}
