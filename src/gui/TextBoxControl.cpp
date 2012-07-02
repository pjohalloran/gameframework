// /////////////////////////////////////////////////////////////////
// @file TextBoxControl.cpp
// @author PJ O Halloran
// @date 21/10/2010
//
// Class for a textbox control widget in my OpenGL based UI library.
//
// /////////////////////////////////////////////////////////////////

// External Headers


// Project Headers
#include "TextBoxControl.h"
#include "GameMain.h"


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void TextBoxControl::SetLuaText(const LuaPlus::LuaObject &table)
	{
		if(table.IsString())
		{
			VSetText(table.GetString());
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	TextBoxControl::TextBoxControl(const Point3 &posRef,\
									const Vector4 &colorRef,\
									boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
									const F32 width,\
									const F32 height,\
									boost::shared_ptr<FTFont> fontPtr,\
									const boost::shared_ptr<GLSLShader> shaderFlatObj,\
									const boost::shared_ptr<GLSLShader> shaderTexObj,\
									const std::string &textRef,\
									const std::string &textureNameRef,\
                                    const std::string &atlasNameRef,\
									const bool visible,\
									const ScreenElementId id,\
									const bool enabled) throw (GameException &)\
									: ControlWidget(posRef, colorRef, mvpStackManPtr, width, height, fontPtr, shaderFlatObj, shaderTexObj, textureNameRef, atlasNameRef, visible, id, enabled),\
										m_text(textRef)
	{
		VSetText(textRef);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	TextBoxControl::TextBoxControl(const LuaPlus::LuaObject &widgetScriptData,\
									boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
									const boost::shared_ptr<GLSLShader> shaderFlatObj,\
									const boost::shared_ptr<GLSLShader> shaderTexObj,\
									boost::shared_ptr<FTFont> fontPtr,\
									const ScreenElementId id) throw (GameException &)
									: ControlWidget(widgetScriptData, mvpStackManPtr, shaderFlatObj, shaderTexObj, fontPtr, id),\
										m_text("--No Text--")
	{
		SetLuaText(widgetScriptData.GetByName("Text"));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool TextBoxControl::VOnRender(const F64 time, const F32 elapsedTime)
	{
		bool result = true;
		result = ControlWidget::VOnRender(time, elapsedTime);
		if(result && VIsVisible())
		{
			result = ControlWidget::RenderText(m_text);
		}
		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void TextBoxControl::VOnUpdate(const F32 elapsedTime)
	{
		// TODO: 
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void TextBoxControl::VSetText(const std::string &textRef)
	{
		m_text = textRef;
		if(!m_text.empty())
		{
			BoundingCube bb;
			GetTextRenderBoundingBox(m_text, bb);
			const F32 padding = 5.0f;
			if(bb.GetWidth() + padding > VGetWidth())
			{
				VSetWidth(bb.GetWidth() + padding);
			}
			if(bb.GetHeight() + padding > VGetHeight())
			{
				VSetHeight(bb.GetHeight() + padding);
			}
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	LuaPlus::LuaObject TextBoxControl::VGetLuaData()
	{
		LuaPlus::LuaObject table;
		table.AssignNewTable(g_appPtr->GetLuaStateManager()->GetGlobalState().Get());
		table.SetString("Text", m_text.c_str());
		table.SetInteger("ScreenElementId", VGetId());
		return (table);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool TextBoxControl::VSetLuaData(const LuaPlus::LuaObject &widgetData)
	{
		return (SetStringFromLua(widgetData["Text"], m_text));
	}

}
