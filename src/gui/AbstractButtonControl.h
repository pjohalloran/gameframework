#pragma once
#ifndef __GF_ABSTRACT_BUTTON_CONTROL_WIDGET_H
#define __GF_ABSTRACT_BUTTON_CONTROL_WIDGET_H

// /////////////////////////////////////////////////////////////////
// @file AbstractButtonControl.h
// @author PJ O Halloran
// @date 21/10/2010
//
// Base level class for button type control widgets in
// my OpenGL based UI library.
//
// /////////////////////////////////////////////////////////////////

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

#include "ControlWidget.h"

namespace GameHalloran
{


	// /////////////////////////////////////////////////////////////////
	// @class AbstractButtonControl
	// @author PJ O Halloran
	//
	// Base class for all button type control UI objects.
	//
	// /////////////////////////////////////////////////////////////////
	class AbstractButtonControl : public ControlWidget
	{
	private:

		bool m_pressed;						///< Is the button currently being pressed.
		bool m_mouseOver;					///< Is the cursor currently over the button.
		I32 m_eventTypeId;					///< ID of the event to include in the event generated on an action.

		// /////////////////////////////////////////////////////////////////
		// Constructor helper funtion. Sets the eventTypeId attribute.
		//
		// /////////////////////////////////////////////////////////////////
		void SetLuaEventTypeId(const LuaPlus::LuaObject &table);

	protected:

		// /////////////////////////////////////////////////////////////////
		// Action to perform when the mouse is hovering over the control.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnMouseEnter() = 0;

		// /////////////////////////////////////////////////////////////////
		// Action to perform when the mouse is leaves the controls area.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnMouseLeave() = 0;

		// /////////////////////////////////////////////////////////////////
		// Action to perform when the mouse is pressed while hovering over
		// the control.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnMousePressed(const U32 buttonId) = 0;

		// /////////////////////////////////////////////////////////////////
		// Action to perform when the mouse is released inside the control
		// while previously pressed over the control.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnMouseReleased(const U32 buttonId) = 0;

		// /////////////////////////////////////////////////////////////////
		// Action to perform when the mouse is released outside the control
		// while previously pressed over the control.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnMouseReleasedCancel(const U32 buttonId) = 0;

		// /////////////////////////////////////////////////////////////////
		// Set if the button is currently pressed.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetPressed(const bool p) { m_pressed = p; };

		// /////////////////////////////////////////////////////////////////
		// Set if the mouse is currently over.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetMouseOver(const bool m) { m_mouseOver = m; };

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
		explicit AbstractButtonControl(const Point3 &posRef,\
										const Vector4 &colorRef,\
										boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
										const F32 width,\
										const F32 height,\
										boost::shared_ptr<FTFont> fontPtr,\
										const boost::shared_ptr<GLSLShader> shaderFlatObj,\
										const boost::shared_ptr<GLSLShader> shaderTexObj,\
										const I32 eventTypeId,\
										const std::string &textureNameRef = std::string(),\
                                        const std::string &atlasNameRef = std::string(),\
										const bool visible = true,\
										const ScreenElementId id = SCREEN_ELEMENT_ID_NOT_ASSIGNED,
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
		explicit AbstractButtonControl(const LuaPlus::LuaObject &widgetScriptData,\
										boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
										const boost::shared_ptr<GLSLShader> shaderFlatObj,\
										const boost::shared_ptr<GLSLShader> shaderTexObj,\
										boost::shared_ptr<FTFont> fontPtr,\
										const ScreenElementId id = SCREEN_ELEMENT_ID_NOT_ASSIGNED) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		// 
		// /////////////////////////////////////////////////////////////////
		virtual ~AbstractButtonControl() { };

		// /////////////////////////////////////////////////////////////////
		// Called once per game loop so the element can check if there was
		// any input from the operating system or from the user.
		//
		// @param eventObj Event structure.
		// @param elapsedTime The number of seconds since the last event poll.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnEvent(GF_Event &eventObj, const F32 elapsedTime);

		// /////////////////////////////////////////////////////////////////
		// Method called when the button is clicked.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnAction() = 0;

		// /////////////////////////////////////////////////////////////////
		// Is the button currently pressed.
		//
		// /////////////////////////////////////////////////////////////////
		inline bool IsPressed() const { return (m_pressed); };

		// /////////////////////////////////////////////////////////////////
		// Is the mouse cursor currently on top of the button.
		//
		// /////////////////////////////////////////////////////////////////
		inline bool IsMouseOver() const { return (m_mouseOver); };

		// /////////////////////////////////////////////////////////////////
		// Set the buttons event type ID.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetEventType(const I32 eventTypeId) { m_eventTypeId = eventTypeId; };

		// /////////////////////////////////////////////////////////////////
		// Get the buttons event type ID.
		//
		// /////////////////////////////////////////////////////////////////
		inline I32 GetEventType() const { return (m_eventTypeId); };

	};

}

#ifdef WIN32
#	pragma warning( pop )
#endif

#endif
