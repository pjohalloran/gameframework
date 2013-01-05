#pragma once
#ifndef __GF_CONTROL_WIDGET_H
#define __GF_CONTROL_WIDGET_H

// /////////////////////////////////////////////////////////////////
// @file ControlWidget.h
// @author PJ O Halloran
// @date 21/10/2010
//
// Base level class for control widgets (buttons, sliders, etc.) in
// my OpenGL based UI library.
//
// /////////////////////////////////////////////////////////////////

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

#include <string>

#include <FTGL/ftgl.h>
#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

#include "AbstractWidget.h"
#include "BoundingCube.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class ControlWidget
	// @author PJ O Halloran
	//
	// Base class for all control UI objects.
	//
	// /////////////////////////////////////////////////////////////////
	class ControlWidget : public AbstractWidget
	{
	private:

		bool m_enabled;									///< Is the control enabled or disabled.
		GLBatch m_outlineBatch;							///< Heavy black lines to draw on edges of control when it is enabled.
		boost::shared_ptr<FTFont> m_fontPtr;			///< Font object to render font for any controls that need it.
		FTSimpleLayout *m_ftglLayoutPtr;				///< 

		// /////////////////////////////////////////////////////////////////
		// LUA constructor helper function.  Sets the widgets enabled setting
		// from the LUA table.
		//
		// /////////////////////////////////////////////////////////////////
		void SetLuaEnabled(const LuaPlus::LuaObject &table);

		// /////////////////////////////////////////////////////////////////
		// Build the outline batch of geometry.
		//
		// /////////////////////////////////////////////////////////////////
		void BuildOutline();

	protected:

		// /////////////////////////////////////////////////////////////////
		// Get the bounding box of the the text when its rendered.
		// Useful for ensuring the control is large enough to hold text before
		// you actual render it.
		//
		// /////////////////////////////////////////////////////////////////
		bool GetTextRenderBoundingBox(const std::string &textToRender, BoundingCube &bb);

		// /////////////////////////////////////////////////////////////////
		// Render the text string centered on the control.  The control should
		// be large enough to contain/surround the text string before you
		// call this method.
		//
		// @param textToRender The text to draw.
		//
		// /////////////////////////////////////////////////////////////////
		bool RenderText(const std::string &textToRender);

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param posRef Initial position and z depth.
		// @param colorRef Color of quad.
		// @param mvpStackManPtr Pointer to the MVP stack manager.
		// @param width Width of widget.
		// @param height Height of widget.
		// @param fontPtr Pointer to the FTGL font to use to render text.
		// @param shaderFlatObj The GLSL shader object used to render the un-textured widget.
		// @param shaderTexObj The GLSL shader object used to render the textured widget.
		// @param textureNameRef Name of texture to apply. An empty string disables it.
		// @param visible Is the widget visible.
		// @param id Unique ID of widget.
		// @param enabled Is the control initially enabled.
		//
		// @throw GameException& If the shader object used is NULL.
		//
		// /////////////////////////////////////////////////////////////////
		explicit ControlWidget(const Point3 &posRef,\
								const Vector4 &colorRef,\
								boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
								const F32 width,\
								const F32 height,\
								boost::shared_ptr<FTFont> fontPtr,\
								const boost::shared_ptr<GLSLShader> shaderFlatObj,\
								const boost::shared_ptr<GLSLShader> shaderTexObj,\
								const std::string &textureNameRef = std::string(),\
                                const std::string &atlasNameRef = std::string(),\
								const bool visible = true,\
								const ScreenElementId id = SCREEN_ELEMENT_ID_NOT_ASSIGNED,\
								const bool enabled = true) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Contructor for widgets defined in an external LUA script.
		//
		// @param widgetScriptData Lua object containing the widget data.
		// @param mvpStackManPtr Pointer to the MVP stack manager.
		// @param shaderFlatObj Flat shader to use.  Will be overridden
		//						if the scripter defined a flat shader.
		// @param shaderTexObj Texture shader to use.  Will be overridden
		//						if the scripter defined a texture shader.
		// @param fontPtr Pointer to the FTGL font to use to render text.
		// @param id The screen element ID of the widget.
		//
		// @throw GameException& If the shader object used is NULL.
		// @throw GameException& If Lua data is invalid or incomplete.
		//
		// /////////////////////////////////////////////////////////////////
		explicit ControlWidget(const LuaPlus::LuaObject &widgetScriptData,\
								boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
								const boost::shared_ptr<GLSLShader> shaderFlatObj,\
								const boost::shared_ptr<GLSLShader> shaderTexObj,\
								boost::shared_ptr<FTFont> fontPtr,\
								const ScreenElementId id = SCREEN_ELEMENT_ID_NOT_ASSIGNED) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		// 
		// /////////////////////////////////////////////////////////////////
		virtual ~ControlWidget() { if(m_ftglLayoutPtr) { delete m_ftglLayoutPtr; m_ftglLayoutPtr = NULL; } };

		// /////////////////////////////////////////////////////////////////
		// Called when the application is restored after a minimize or other
		// such event.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnRestore();

		// /////////////////////////////////////////////////////////////////
		// Called when the application display or device is lost after a
		// display resolution or other such system event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnLostDevice();

		// /////////////////////////////////////////////////////////////////
		// Render the screen element.
		//
		// @param time The total time in seconds so far.
		// @param elapsedTime The time elapsed since the last render call.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnRender(const F64 time, const F32 elapsedTime);

		// /////////////////////////////////////////////////////////////////
		// Check if the control is currently enabled.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VIsEnabled() const { return (m_enabled); };

		// /////////////////////////////////////////////////////////////////
		// Set if the control should be enabled.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetEnabled(const bool enabled) { m_enabled = enabled; };

		// /////////////////////////////////////////////////////////////////
		// Set the position of the top left of the widget.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetPosition(const Point3 &posRef);

		// /////////////////////////////////////////////////////////////////
		// Set the width of the widget.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetWidth(const F32 width);

		// /////////////////////////////////////////////////////////////////
		// Set the height of the widget.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetHeight(const F32 height);

	};

}

#ifdef WIN32
#	pragma warning( pop )
#endif

#endif
