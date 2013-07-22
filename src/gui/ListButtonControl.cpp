// /////////////////////////////////////////////////////////////////
// @file ListButtonControl.cpp
// @author PJ O Halloran
// @date 21/10/2010
//
// Class for the list button control widgets in my OpenGL based UI
// library.
//
// /////////////////////////////////////////////////////////////////

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "ListButtonControl.h"
#include "GameMain.h"
#include "GameBase.h"
#include "EventManager.h"
#include "Events.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ListButtonControl::SetLuaTextList(const LuaPlus::LuaObject &table)
    {
        bool error = true;
        if(table.IsTable()) {
            I64 size = table.GetTableCount();
            // LUA table indices start at 1!!
            for(I64 i = 1; i <= size; ++i) {
                LuaPlus::LuaObject currTable = table[i];
                if(currTable.IsString()) {
                    error = false;
                    m_list.push_back(std::string(currTable.GetString()));
                }
            }

        }

#if DEBUG
        if(error) {
            GF_LOG_TRACE_ERR("ListButtonControl::SetLuaTextList()", "No List of text strings got from lua data");
        }
#endif
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ListButtonControl::Init()
    {
        if(!m_list.empty()) {
            VSetText(*m_curr);
        }

        BoundingCube bb;
        F32 maxWidth = 0.0f, maxHeight = 0.0f;
        for(std::list<std::string>::const_iterator i = m_list.begin(), end = m_list.end(); i != end; ++i) {
            GetTextRenderBoundingBox(*i, bb);
            F32 w = bb.GetMax().GetX() - bb.GetMin().GetX();
            F32 h = bb.GetMax().GetY() - bb.GetMin().GetY();
            if(w > maxWidth) {
                maxWidth = w;
            }
            if(h > maxHeight) {
                maxHeight = h;
            }
        }
        if((maxWidth > 0.0f) && (maxHeight > 0.0f)) {
            const F32 padding = 5.0f;
            VSetWidth(maxWidth + padding);
            VSetHeight(maxHeight + padding);
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ListButtonControl::VOnMouseReleased(const U32 buttonId)
    {
        bool result = ButtonControl::VOnMouseReleased(buttonId);
        if(!m_list.empty()) {
            ++m_curr;
            if(m_curr == m_list.end()) {
                m_curr = m_list.begin();
            }
            VSetText(*m_curr);
        }
        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    ListButtonControl::ListButtonControl(const Point3 &posRef, \
                                         const Vector4 &colorRef, \
                                         boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr, \
                                         const F32 width, \
                                         const F32 height, \
                                         boost::shared_ptr<FTFont> fontPtr, \
                                         const boost::shared_ptr<GLSLShader> shaderFlatObj, \
                                         const boost::shared_ptr<GLSLShader> shaderTexObj, \
                                         const I32 eventTypeId, \
                                         const std::string &textureNameRef, \
                                         const std::string &texturePressedRef, \
                                         const std::string &textureHoverRef, \
                                         const std::string &atlasNameRef, \
                                         const std::list<std::string> &list, \
                                         const bool visible, \
                                         const ScreenElementId id, \
                                         const bool enabled) throw(GameException &)\
:
    ButtonControl(posRef, colorRef, mvpStackManPtr, width, height, fontPtr, shaderFlatObj, shaderTexObj, eventTypeId, textureNameRef, atlasNameRef, \
                  texturePressedRef, textureHoverRef, std::string(""), visible, id, enabled)
    , m_list(list)
    , m_curr(m_list.begin())
    {
        Init();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    ListButtonControl::ListButtonControl(const LuaPlus::LuaObject &widgetScriptData, \
                                         boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr, \
                                         const boost::shared_ptr<GLSLShader> shaderFlatObj, \
                                         const boost::shared_ptr<GLSLShader> shaderTexObj, \
                                         boost::shared_ptr<FTFont> fontPtr, \
                                         const ScreenElementId id) throw(GameException &)\
:
    ButtonControl(widgetScriptData, mvpStackManPtr, shaderFlatObj, shaderTexObj, fontPtr, id)
    , m_list()
    , m_curr()
    {
        SetLuaTextList(widgetScriptData.GetByName("TextTable"));
        if(!m_list.empty()) {
            m_curr = m_list.begin();
        }
        Init();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ListButtonControl::VOnAction()
    {
        std::string text;
        if(!m_list.empty()) {
            text.assign(*m_curr);
        }
        IEventDataPtr buttonActionEvent(GCC_NEW EvtData_List_Button_Action(AbstractWidget::VGetId(), AbstractButtonControl::GetEventType(), text));
        if(!safeQueEvent(buttonActionEvent)) {
            std::string idStr;
            try {
                idStr = boost::lexical_cast<std::string, ScreenElementId>(AbstractWidget::VGetId());
            } catch(...) {}
            GF_LOG_TRACE_ERR("ListButtonControl::ListButtonControl()", std::string("Failed to send the EvtData_List_Button_Action event for the list button ") + idStr);
            return (false);
        }

        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ListButtonControl::VSetText(const std::string &textRef)
    {
        m_text = textRef;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ListButtonControl::VOnUpdate(const F32 elapsedTime)
    {
        ButtonControl::VOnUpdate(elapsedTime);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    LuaPlus::LuaObject ListButtonControl::VGetLuaData()
    {
        LuaPlus::LuaObject table;
        table.AssignNewTable(g_appPtr->GetLuaStateManager()->GetGlobalState().Get());
        if(m_list.empty()) {
            table.SetString("Text", "");
        } else {
            table.SetString("Text", (*m_curr).c_str());
        }
        table.SetInteger("ScreenElementId", VGetId());
        table.SetInteger("EventTypeId", GetEventType());
        return (table);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool ListButtonControl::VSetLuaData(const LuaPlus::LuaObject &widgetData)
    {
        I32 currEventType(0);
        std::string tmpText;
        bool a = SetStringFromLua(widgetData["Text"], tmpText);
        bool b = SetIntFromLua(widgetData["EventTypeId"], currEventType);
        bool found(false);

        if(a && b) {
            boost::algorithm::to_lower(tmpText);
            for(std::list<std::string>::iterator i = m_list.begin(), end = m_list.end(); ((!found) && (i != end)); ++i) {
                std::string currStr(*i);
                boost::algorithm::to_lower(currStr);
                if(currStr.compare(tmpText.c_str()) == 0) {
                    found = true;
                    m_curr = i;
                    m_text = *i;
                }
            }

            if(found) {
                SetEventType(currEventType);
            }
        }

        return (a && b && found);
    }

}
