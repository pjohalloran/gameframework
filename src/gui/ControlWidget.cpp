// /////////////////////////////////////////////////////////////////
// @file ControlWidget.cpp
// @author PJ O Halloran
// @date 21/10/2010
//
// Base level class for all controls in my OpenGL based UI library.
//
// /////////////////////////////////////////////////////////////////

#include "GameBase.h"
#include "ControlWidget.h"
#include "GameMain.h"
#include "GameColors.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ControlWidget::SetLuaEnabled(const LuaPlus::LuaObject &table)
    {
        if(table.IsBoolean()) {
            m_enabled = table.GetBoolean();
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ControlWidget::BuildOutline()
    {
        m_outlineBatch.Reset();

        const Point3 pos = AbstractWidget::VGetPosition();
        m_outlineBatch.Begin(GL_LINES, 2);
        // Defined in CCW order.
        m_outlineBatch.Vertex3f(0.0f, 0.0f, 0.0f);
        m_outlineBatch.Vertex3f(0.0f, 0.0f, 0.0f);
        m_outlineBatch.End();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ControlWidget::GetTextRenderBoundingBox(const std::string &textToRender, BoundingCube &bb)
    {
        if(!m_fontPtr) {
            return (false);
        }
        if(m_fontPtr->Error()) {
            return (false);
        }

        FTBBox ftbb = m_fontPtr->BBox(textToRender.c_str());
        bb.SetMin(Point3(static_cast<F32>(ftbb.Lower().X()), static_cast<F32>(ftbb.Lower().Y()), static_cast<F32>(ftbb.Lower().Z())));
        bb.SetMax(Point3(static_cast<F32>(ftbb.Upper().X()), static_cast<F32>(ftbb.Upper().Y()), static_cast<F32>(ftbb.Upper().Z())));
        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ControlWidget::RenderText(const std::string &textToRender)
    {
        if(!m_fontPtr) {
            return (false);
        }
        if(m_fontPtr->Error()) {
            return (false);
        }

        BoundingCube bb;
        GetTextRenderBoundingBox(textToRender, bb);
        Point3 windowPos;
        windowPos.SetX(VGetPosition().GetX() + (VGetWidth() - bb.GetWidth()) / 2.0f);
        windowPos.SetY(VGetPosition().GetY() - (VGetHeight() - bb.GetHeight()) / 2.0f - bb.GetHeight());
        //m_fontPtr->Render(textToRender.c_str());

        return(true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    ControlWidget::ControlWidget(const Point3 &posRef, \
                                 const Vector4 &colorRef, \
                                 boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr, \
                                 const F32 width, \
                                 const F32 height, \
                                 boost::shared_ptr<FTFont> fontPtr, \
                                 const boost::shared_ptr<GLSLShader> shaderFlatObj, \
                                 const boost::shared_ptr<GLSLShader> shaderTexObj, \
                                 const std::string &textureNameRef, \
                                 const std::string &atlasNameRef, \
                                 const bool visible, \
                                 const ScreenElementId id, \
                                 const bool enabled) throw(GameException &)\
:
    AbstractWidget(posRef, colorRef, mvpStackManPtr, width, height, \
                   shaderFlatObj, shaderTexObj, textureNameRef, atlasNameRef, visible, id)
    , m_enabled(enabled)
    , m_fontPtr(fontPtr)
    , m_ftglLayoutPtr(NULL)
    {
        BuildOutline();

        if(m_fontPtr) {
            m_ftglLayoutPtr = new FTSimpleLayout();
            m_ftglLayoutPtr->SetFont(m_fontPtr.get());
            m_ftglLayoutPtr->SetAlignment(FTGL::ALIGN_LEFT);
            m_ftglLayoutPtr->SetLineLength(600.0f);
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    ControlWidget::ControlWidget(const LuaPlus::LuaObject &widgetScriptData, \
                                 boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr, \
                                 const boost::shared_ptr<GLSLShader> shaderFlatObj, \
                                 const boost::shared_ptr<GLSLShader> shaderTexObj, \
                                 boost::shared_ptr<FTFont> fontPtr, \
                                 const ScreenElementId id) throw(GameException &)
        : AbstractWidget(widgetScriptData, mvpStackManPtr, shaderFlatObj, shaderTexObj, id)\
        , m_enabled(true)
        , m_fontPtr(fontPtr)
        , m_ftglLayoutPtr(NULL)
    {
        SetLuaEnabled(widgetScriptData.GetByName("Enabled"));
        BuildOutline();

        if(m_fontPtr) {
            m_ftglLayoutPtr = new FTSimpleLayout();
            m_ftglLayoutPtr->SetFont(m_fontPtr.get());
            m_ftglLayoutPtr->SetAlignment(FTGL::ALIGN_LEFT);
            m_ftglLayoutPtr->SetLineLength(600.0f);
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ControlWidget::VOnRestore()
    {
        m_outlineBatch.Reset();
        return (AbstractWidget::VOnRestore());
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ControlWidget::VOnLostDevice()
    {
        BuildOutline();
        AbstractWidget::VOnLostDevice();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ControlWidget::VOnRender(const F64 time, const F32 elapsedTime)
    {
        if(!AbstractWidget::VOnRender(time, elapsedTime)) {
            GF_LOG_TRACE_ERR("ControlWidget::VOnRender()", "Failed to render base class");
            return (false);
        }

        // Render using the base classes shader which is still active (draw a black outline)...
        if(m_enabled) {
            //glUniform1i(m_samplerLoc, 0);
            //glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, m_projMatrix.GetComponentsConst());

            //glUniform4fv(m_colorLoc, 1, g_gcBlack.GetComponentsConst());
            //m_outlineBatch.VDraw();
        }
        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ControlWidget::VSetPosition(const Point3 &posRef)
    {
        AbstractWidget::VSetPosition(posRef);
        BuildOutline();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ControlWidget::VSetWidth(const F32 width)
    {
        AbstractWidget::VSetWidth(width);
        BuildOutline();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ControlWidget::VSetHeight(const F32 height)
    {
        AbstractWidget::VSetHeight(height);
        BuildOutline();
    }

}
