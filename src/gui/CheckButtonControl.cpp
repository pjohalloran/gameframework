// /////////////////////////////////////////////////////////////////
// @file CheckButtonControl.cpp
// @author PJ O Halloran
// @date 21/10/2010
//
// Class for the check button type control widget in
// my OpenGL based UI library.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <boost/lexical_cast.hpp>


// Project Headers
#include "CheckButtonControl.h"

#include "GameMain.h"
#include "GameBase.h"
#include "EventManager.h"
#include "Events.h"


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CheckButtonControl::SetLuaChecked(const LuaPlus::LuaObject &table)
	{
		if(table.IsBoolean())
		{
			m_checked = table.GetBoolean();
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CheckButtonControl::SetLuaCbTextures(const LuaPlus::LuaObject &hoverTable,\
												const LuaPlus::LuaObject &checkTable,\
												const LuaPlus::LuaObject &hoverCheckTable,\
												std::string &hoverNameRef,\
												std::string &checkNameRef,\
												std::string &checkHoverNameRef)
	{
		if(hoverTable.IsString())
		{
			hoverNameRef.assign(hoverTable.GetString());
		}
		if(checkTable.IsString())
		{
			checkNameRef.assign(checkTable.GetString());
		}
		if(hoverCheckTable.IsString())
		{
			checkHoverNameRef.assign(hoverCheckTable.GetString());
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CheckButtonControl::Init(const std::string &hoverNameRef, const std::string &checkNameRef, const std::string &checkHoverNameRef)
	{
		boost::optional<TexHandle> th;
        
        if(!IsAtlased())
        {
            th = g_appPtr->GetTextureManagerPtr()->Load2D(hoverNameRef, GL_CLAMP_TO_EDGE);
            if(!th.is_initialized())
            {
                GF_LOG_TRACE_ERR("CheckButtonControl::Init()", std::string("Failed to create the texture: ") + hoverNameRef);
            }
            else
            {
                m_hoverHandle = *th;
            }
        }
        else
        {
            if(!g_appPtr->GetAtlasManagerPtr()->UseAtlas(m_atlasName) || !g_appPtr->GetAtlasManagerPtr()->UseImage(hoverNameRef))
            {
    #if DEBUG
                std::string idStr;
                try { idStr = boost::lexical_cast<std::string, ScreenElementId>(GetId()); } catch(...) { }
                GF_LOG_TRACE_ERR("AbstractWidget::Init()", "Failed to get the atlas for the widget " + idStr);
                return;
    #endif
            }
            
            m_hoverDim = *g_appPtr->GetAtlasManagerPtr()->GetCurrentAtlasImage();
        }
        
        if(!IsAtlased())
        {
            th = g_appPtr->GetTextureManagerPtr()->Load2D(checkNameRef, GL_CLAMP_TO_EDGE);
            if(!th.is_initialized())
            {
                GF_LOG_TRACE_ERR("CheckButtonControl::Init()", std::string("Failed to create the texture: ") + checkNameRef);
            }
            else
            {
                m_checkHandle = *th;
            }
        }
        else
        {
            if(!g_appPtr->GetAtlasManagerPtr()->UseAtlas(m_atlasName) || !g_appPtr->GetAtlasManagerPtr()->UseImage(checkNameRef))
            {
    #if DEBUG
                std::string idStr;
                try { idStr = boost::lexical_cast<std::string, ScreenElementId>(GetId()); } catch(...) { }
                GF_LOG_TRACE_ERR("AbstractWidget::Init()", "Failed to get the atlas for the widget " + idStr);
                return;
    #endif
            }
            
            m_checkDim = *g_appPtr->GetAtlasManagerPtr()->GetCurrentAtlasImage();
        }
        
        if(!IsAtlased())
        {
            th = g_appPtr->GetTextureManagerPtr()->Load2D(checkHoverNameRef, GL_CLAMP_TO_EDGE);
            if(!th.is_initialized())
            {
                GF_LOG_TRACE_ERR("CheckButtonControl::Init()", std::string("Failed to create the texture: ") + checkHoverNameRef);
            }
            else
            {
                m_checkHoverHandle = *th;
            }
        }
        else
        {
            if(!g_appPtr->GetAtlasManagerPtr()->UseAtlas(m_atlasName) || !g_appPtr->GetAtlasManagerPtr()->UseImage(checkHoverNameRef))
            {
    #if DEBUG
                std::string idStr;
                try { idStr = boost::lexical_cast<std::string, ScreenElementId>(GetId()); } catch(...) { }
                GF_LOG_TRACE_ERR("AbstractWidget::Init()", "Failed to get the atlas for the widget " + idStr);
                return;
    #endif
            }
            
            m_checkHoverDim = *g_appPtr->GetAtlasManagerPtr()->GetCurrentAtlasImage();
        }

		if(m_checked)
			AbstractWidget::SetCurrentTexture(m_checkHandle);
		else
			AbstractWidget::SetCurrentTexture(m_tHandle);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CheckButtonControl::VOnMouseEnter()
	{
		if(m_checked)
		{
            if(IsAtlased())
                SetQuadDim(m_checkHoverDim);
            else
                AbstractWidget::SetCurrentTexture(m_checkHoverHandle);
		}
		else
		{
            if(IsAtlased())
                SetQuadDim(m_hoverDim);
            else
                AbstractWidget::SetCurrentTexture(m_hoverHandle);
		}
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CheckButtonControl::VOnMouseLeave()
	{
		if(m_checked)
		{
            if(IsAtlased())
                SetQuadDim(m_checkDim);
            else
                AbstractWidget::SetCurrentTexture(m_checkHandle);
		}
		else
		{
            if(IsAtlased())
                SetQuadDim(GetQuadDim());
            else
                AbstractWidget::SetCurrentTexture(m_tHandle);
		}
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CheckButtonControl::VOnMousePressed(const U32 buttonId)
	{
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CheckButtonControl::VOnMouseReleased(const U32 buttonId)
	{
		m_checked = !m_checked;
		if(m_checked)
		{
            if(IsAtlased())
                SetQuadDim(m_checkHoverDim);
            else
                AbstractWidget::SetCurrentTexture(m_checkHoverHandle);
		}
		else
		{
            if(IsAtlased())
                SetQuadDim(GetQuadDim());
            else
                AbstractWidget::SetCurrentTexture(m_tHandle);
		}
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CheckButtonControl::VOnMouseReleasedCancel(const U32 buttonId)
	{
		if(m_checked)
		{
            if(IsAtlased())
                SetQuadDim(m_checkDim);
            else
                AbstractWidget::SetCurrentTexture(m_checkHandle);
		}
		else
		{
            if(IsAtlased())
                SetQuadDim(GetQuadDim());
            else
                AbstractWidget::SetCurrentTexture(m_tHandle);
		}
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	CheckButtonControl::CheckButtonControl(const Point3 &posRef,\
								const Vector4 &colorRef,\
								boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
								const F32 width,\
								const F32 height,\
								boost::shared_ptr<FTFont> fontPtr,\
								const boost::shared_ptr<GLSLShader> shaderFlatObj,\
								const boost::shared_ptr<GLSLShader> shaderTexObj,\
								const I32 eventTypeId,
								const std::string &textureNameRef,\
                                const std::string &atlasNameRef,\
								const std::string &hoverNameRef,\
								const std::string &checkNameRef,\
								const std::string &checkHoverNameRef,\
								const bool visible,\
								const ScreenElementId id,\
								const bool enabled) throw (GameException &)\
								: AbstractButtonControl(posRef, colorRef, mvpStackManPtr, width, height, fontPtr, shaderFlatObj, shaderTexObj, eventTypeId, textureNameRef, atlasNameRef,\
									visible, id, enabled), m_checked(false), m_hoverHandle(0), m_checkHandle(0), m_checkHoverHandle(0), m_hoverDim(""),
                                        m_checkDim(""), m_checkHoverDim("")
	{
		Init(hoverNameRef, checkNameRef, checkHoverNameRef);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	CheckButtonControl::CheckButtonControl(const LuaPlus::LuaObject &widgetScriptData,\
											boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
											const boost::shared_ptr<GLSLShader> shaderFlatObj,\
											const boost::shared_ptr<GLSLShader> shaderTexObj,\
											boost::shared_ptr<FTFont> fontPtr,\
											const ScreenElementId id) throw (GameException &)
											: AbstractButtonControl(widgetScriptData, mvpStackManPtr, shaderFlatObj, shaderTexObj, fontPtr, id),\
                                            m_checked(false), m_hoverHandle(0), m_checkHandle(0), m_checkHoverHandle(0), m_hoverDim(""),\
                                            m_checkDim(""), m_checkHoverDim("")
	{
		SetLuaChecked(widgetScriptData.GetByName("Checked"));
		std::string hover, check, hoverCheck;
		SetLuaCbTextures(widgetScriptData.GetByName("HoverTexture"),\
							widgetScriptData.GetByName("CheckedTexture"),\
							widgetScriptData.GetByName("HoverCheckedTexture"),\
							hover,\
							check,\
							hoverCheck);
		Init(hover, check, hoverCheck);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void CheckButtonControl::VOnUpdate(const F32 elapsedTime)
	{
		// TODO:
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CheckButtonControl::VOnAction()
	{
		IEventDataPtr buttonActionEvent(GCC_NEW EvtData_Button_Action(AbstractWidget::VGetId(), AbstractButtonControl::GetEventType(), m_checked));
		if(!safeQueEvent(buttonActionEvent))
		{
#if DEBUG
			std::string idStr;
			try { idStr = boost::lexical_cast<std::string, ScreenElementId>(AbstractWidget::VGetId()); } catch(...) {}
            GF_LOG_TRACE_ERR("CheckButtonControl::CheckButtonControl()", std::string("Failed to send the EvtData_Button_Action event for the checkbox ") + idStr);
#endif
			return (false);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	LuaPlus::LuaObject CheckButtonControl::VGetLuaData()
	{
		LuaPlus::LuaObject table;
		table.AssignNewTable(g_appPtr->GetLuaStateManager()->GetGlobalState().Get());
		table.SetBoolean("Checked", m_checked);
		table.SetInteger("ScreenElementId", VGetId());
		table.SetInteger("EventTypeId", GetEventType());
		return (table);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool CheckButtonControl::VSetLuaData(const LuaPlus::LuaObject &widgetData)
	{
		I32 currEventType(0);
		bool a = SetBoolFromLua(widgetData["Checked"], m_checked);
		bool b = SetIntFromLua(widgetData["EventTypeId"], currEventType);
		if(a && b)
		{
			// Update the current UI texture and new script event ID.
			if(IsMouseOver())
			{
				if(m_checked)
				{
					AbstractWidget::SetCurrentTexture(m_checkHoverHandle);
				}
				else
				{
					AbstractWidget::SetCurrentTexture(m_hoverHandle);
				}
			}
			else
			{
				if(m_checked)
				{
					AbstractWidget::SetCurrentTexture(m_checkHandle);
				}
				else
				{
					AbstractWidget::SetCurrentTexture(m_tHandle);
				}
			}
			SetEventType(currEventType);
		}
		return (a && b);
	}

}
