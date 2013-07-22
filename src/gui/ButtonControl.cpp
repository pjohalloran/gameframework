// /////////////////////////////////////////////////////////////////
// @file ButtonControl.cpp
// @author PJ O Halloran
// @date 21/10/2010
//
// Class for the standerd button control widgets in
// my OpenGL based UI library.
//
// /////////////////////////////////////////////////////////////////

#include "ButtonControl.h"
#include "EventManager.h"
#include "Events.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ButtonControl::Init(const std::string &textureHoverRef, const std::string &texturePressedRef) throw(GameException &)
    {
        boost::optional<TexHandle> th;

        if(!IsAtlased()) {
            th = g_appPtr->GetTextureManagerPtr()->Load2D(texturePressedRef, GL_CLAMP_TO_EDGE);
            if(!th.is_initialized()) {
                GF_LOG_TRACE_ERR("ButtonControl::Init()", std::string("Failed to create the texture: ") + texturePressedRef);
            } else {
                m_pressedHandle = *th;
            }
        } else {
            if(!g_appPtr->GetAtlasManagerPtr()->UseAtlas(m_atlasName) || !g_appPtr->GetAtlasManagerPtr()->UseImage(texturePressedRef)) {
#if DEBUG
                std::string idStr;
                try {
                    idStr = boost::lexical_cast<std::string, ScreenElementId>(GetId());
                } catch(...) { }
                GF_LOG_TRACE_ERR("AbstractWidget::Init()", "Failed to get the atlas for the widget " + idStr);
                return;
#endif
            }

            m_pressedDim = *g_appPtr->GetAtlasManagerPtr()->GetCurrentAtlasImage();
        }

        if(!IsAtlased()) {
            th = g_appPtr->GetTextureManagerPtr()->Load2D(textureHoverRef, GL_CLAMP_TO_EDGE);
            if(!th.is_initialized()) {
                GF_LOG_TRACE_ERR("ButtonControl::Init()", std::string("Failed to create the texture: ") + textureHoverRef);
            } else {
                m_hoverHandle = *th;
            }
        } else {
            if(!g_appPtr->GetAtlasManagerPtr()->UseAtlas(m_atlasName) || !g_appPtr->GetAtlasManagerPtr()->UseImage(textureHoverRef)) {
#if DEBUG
                std::string idStr;
                try {
                    idStr = boost::lexical_cast<std::string, ScreenElementId>(GetId());
                } catch(...) { }
                GF_LOG_TRACE_ERR("AbstractWidget::Init()", "Failed to get the atlas for the widget " + idStr);
                return;
#endif
            }

            m_hoverDim = *g_appPtr->GetAtlasManagerPtr()->GetCurrentAtlasImage();
        }

        // Trigger the change size to ensure the widget is big enough to display all the text.
        std::string copy(m_text);
        VSetText(copy);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ButtonControl::SetLuaTextures(const LuaPlus::LuaObject &hoverTable, const LuaPlus::LuaObject &pressTable, std::string &hoverRef, std::string &pressedRef)
    {
        bool result = false;
        if(hoverTable.IsString()) {
            hoverRef = hoverTable.GetString();
            result = true;
        }
        if(pressTable.IsString()) {
            pressedRef.assign(pressTable.GetString());
            result = true;
        }
        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ButtonControl::SetLuaSendEvent(const LuaPlus::LuaObject &table)
    {
        if(table.IsBoolean()) {
            m_sendEvent = table.GetBoolean();
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ButtonControl::SetLuaText(const LuaPlus::LuaObject &table)
    {
        if(table.IsString()) {
            m_text.assign(table.GetString());
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ButtonControl::VOnMouseEnter()
    {
        if(IsAtlased()) {
            SetQuadDim(m_hoverDim);
        } else {
            AbstractWidget::SetCurrentTexture(m_hoverHandle);
        }
        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ButtonControl::VOnMouseLeave()
    {
        if(IsAtlased()) {
            SetQuadDim(GetQuadDim());
        } else {
            AbstractWidget::SetCurrentTexture(m_tHandle);
        }
        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ButtonControl::VOnMousePressed(const U32 buttonId)
    {
        if(IsAtlased()) {
            SetQuadDim(m_pressedDim);
        } else {
            AbstractWidget::SetCurrentTexture(m_pressedHandle);
        }
        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ButtonControl::VOnMouseReleased(const U32 buttonId)
    {
        if(AbstractButtonControl::IsMouseOver()) {
            if(IsAtlased()) {
                SetQuadDim(m_hoverDim);
            } else {
                AbstractWidget::SetCurrentTexture(m_hoverHandle);
            }
        } else {
            if(IsAtlased()) {
                SetQuadDim(GetQuadDim());
            } else {
                AbstractWidget::SetCurrentTexture(m_tHandle);
            }
        }
        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ButtonControl::VOnMouseReleasedCancel(const U32 buttonId)
    {
        if(IsAtlased()) {
            SetQuadDim(GetQuadDim());
        } else {
            AbstractWidget::SetCurrentTexture(m_tHandle);
        }
        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    ButtonControl::ButtonControl(const Point3 &posRef,
                                 const Vector4 &colorRef,
                                 boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,
                                 const F32 width,
                                 const F32 height,
                                 boost::shared_ptr<FTFont> fontPtr,
                                 const boost::shared_ptr<GLSLShader> shaderFlatObj,
                                 const boost::shared_ptr<GLSLShader> shaderTexObj,
                                 const I32 eventTypeId,
                                 const std::string &textureNameRef,
                                 const std::string &texturePressedRef,
                                 const std::string &textureHoverRef,
                                 const std::string &atlasNameRef,
                                 const std::string &textRef,
                                 const bool visible,
                                 const ScreenElementId id,
                                 const bool enabled,
                                 const bool sendEvent) throw(GameException &)
        : AbstractButtonControl(posRef, colorRef, mvpStackManPtr, width, height, fontPtr, shaderFlatObj, shaderTexObj, eventTypeId
                                , textureNameRef, atlasNameRef, visible, id, enabled)
        , m_hoverHandle(0)
        , m_pressedHandle(0)
        , m_sendEvent(sendEvent)
        , m_hoverDim(textureHoverRef.c_str())
        , m_pressedDim(texturePressedRef.c_str())
        , m_text(textRef)
    {
        Init(textureHoverRef, texturePressedRef);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    ButtonControl::ButtonControl(const LuaPlus::LuaObject &widgetScriptData,
                                 boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,
                                 const boost::shared_ptr<GLSLShader> shaderFlatObj,
                                 const boost::shared_ptr<GLSLShader> shaderTexObj,
                                 boost::shared_ptr<FTFont> fontPtr,
                                 const ScreenElementId id) throw(GameException &)
        : AbstractButtonControl(widgetScriptData, mvpStackManPtr, shaderFlatObj, shaderTexObj, fontPtr, id)
        , m_hoverHandle(0)
        , m_pressedHandle(0)
        , m_sendEvent(true)
        , m_hoverDim("")
        , m_pressedDim("")
        , m_text("--Text Not Set--")
    {
        std::string textureHoverRef, texturePressedRef;
        SetLuaTextures(widgetScriptData.GetByName("HoverTexture"), \
                       widgetScriptData.GetByName("PressedTexture"), \
                       textureHoverRef, \
                       texturePressedRef);
        SetLuaSendEvent(widgetScriptData.GetByName("SendCodeEventOnClick"));
        SetLuaText(widgetScriptData.GetByName("Text"));
        Init(textureHoverRef, texturePressedRef);
    }


    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ButtonControl::VOnAction()
    {
        if(m_sendEvent) {
            IEventDataPtr buttonActionEvent(GCC_NEW EvtData_Button_Action(AbstractWidget::VGetId(), AbstractButtonControl::GetEventType()));
            if(!safeQueEvent(buttonActionEvent)) {
#if DEBUG
                std::string idStr;
                try {
                    idStr = boost::lexical_cast<std::string, ScreenElementId>(AbstractWidget::VGetId());
                } catch(...) {}
                GF_LOG_TRACE_ERR("ButtonControl::VOnAction()", std::string("Failed to send the EvtData_Button_Action event for the button ") + idStr);
#endif
                return (false);
            }
        }
        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ButtonControl::VOnUpdate(const F32 elapsedTime)
    {
        // TODO:
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ButtonControl::VSetText(const std::string &textRef)
    {
        m_text = textRef;
        if(!m_text.empty()) {
            BoundingCube bb;
            GetTextRenderBoundingBox(m_text, bb);
            const F32 padding = 5.0f;
            if(bb.GetWidth() + padding > VGetWidth()) {
                VSetWidth(bb.GetWidth() + padding);
            }
            if(bb.GetHeight() + padding > VGetHeight()) {
                VSetHeight(bb.GetHeight() + padding);
            }
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ButtonControl::VOnRender(const F64 time, const F32 elapsedTime)
    {
        bool result = true;
        result = ControlWidget::VOnRender(time, elapsedTime);
        if(result && VGetColor().GetW() >= 0.25f) {
            result = ControlWidget::RenderText(m_text);
        }
        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    LuaPlus::LuaObject ButtonControl::VGetLuaData()
    {
        LuaPlus::LuaObject table;
        table.AssignNewTable(g_appPtr->GetLuaStateManager()->GetGlobalState().Get());
        table.SetString("Text", m_text.c_str());
        table.SetInteger("EventTypeId", GetEventType());
        table.SetInteger("ScreenElementId", VGetId());
        return (table);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ButtonControl::VSetLuaData(const LuaPlus::LuaObject &widgetData)
    {
        I32 currEventType(0);
        bool a = SetStringFromLua(widgetData["Text"], m_text);
        bool b = SetIntFromLua(widgetData["EventTypeId"], currEventType);
        if(a && b) {
            SetEventType(currEventType);
        }
        return (a && b);
    }

}
