#pragma once
#ifndef __GF_LIST_BUTTON_CONTROL_WIDGET_H
#define __GF_LIST_BUTTON_CONTROL_WIDGET_H

// /////////////////////////////////////////////////////////////////
// @file ListButtonControl.h
// @author PJ O Halloran
// @date 21/10/2010
//
// Class for the list button control widgets in my OpenGL based UI 
// library.
//
// /////////////////////////////////////////////////////////////////

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

#include "ButtonControl.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class ListButtonControl
	// @author PJ O Halloran
	//
	// This is a button that holds a list of strings.  Every time the
	// user clicks on the button, the next string in the list will be
	// displayed on the buttons surface.  This way the user can cycle
	// through a list of options.  For example the user may pick a
	// screen resolution using this button.
	//
	// /////////////////////////////////////////////////////////////////
	class ListButtonControl : public ButtonControl
	{
	private:

		std::list<std::string> m_list;						///< List of list options for the button to display.
		std::list<std::string>::const_iterator m_curr;		///< Position in the list of the currently displayed item.

		// /////////////////////////////////////////////////////////////////
		// Constructor helper function.  Gets the list of text from lua data.
		//
		// /////////////////////////////////////////////////////////////////
		void SetLuaTextList(const LuaPlus::LuaObject &table);

		// /////////////////////////////////////////////////////////////////
		// Constructor helper function.
		//
		// /////////////////////////////////////////////////////////////////
		void Init();

	protected:

		// /////////////////////////////////////////////////////////////////
		// Action to perform when the mouse is released inside the control
		// while previously pressed over the control.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnMouseReleased(const U32 buttonId);

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
		// @param textureNameRef Name of the "default" buttons texture.
		// @param texturePressedRef Name of the buttons "pressed" texture.
		// @param textureHoverRef Name of the buttons "hover" texture.
		// @param list The list of string selections.
		// @param eventTypeId The ID of the event type to generate upon an action.
		// @param visible Is the widget visible.
		// @param id Unique ID of widget.
		// @param enabled Is the control initially enabled.
		//
		// @throw GameException& If the shader object used is NULL.
		//
		// /////////////////////////////////////////////////////////////////
		explicit ListButtonControl(const Point3 &posRef,\
									const Vector4 &colorRef,\
									boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
									const F32 width,\
									const F32 height,\
									boost::shared_ptr<FTFont> fontPtr,\
									const boost::shared_ptr<GLSLShader> shaderFlatObj,\
									const boost::shared_ptr<GLSLShader> shaderTexObj,\
									const I32 eventTypeId,\
									const std::string &textureNameRef,\
									const std::string &texturePressedRef,\
									const std::string &textureHoverRef,\
                                    const std::string &atlasNameRef,\
									const std::list<std::string> &list,\
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
		explicit ListButtonControl(const LuaPlus::LuaObject &widgetScriptData,\
									boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
									const boost::shared_ptr<GLSLShader> shaderFlatObj,\
									const boost::shared_ptr<GLSLShader> shaderTexObj,\
									boost::shared_ptr<FTFont> fontPtr,\
									const ScreenElementId id = SCREEN_ELEMENT_ID_NOT_ASSIGNED) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		// 
		// /////////////////////////////////////////////////////////////////
		virtual ~ListButtonControl() { };

		// /////////////////////////////////////////////////////////////////
		// Method called when the button is clicked.
		//
		// Adds an event of type EvtData_List_Button_Action to the global event
		// queue to notify the application that this button (identified by
		// its event type and screen id) was clicked.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnAction();

		// /////////////////////////////////////////////////////////////////
		// Update the screen element.
		//
		// @param elapsedTime The number of seconds since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnUpdate(const F32 elapsedTime);

		// /////////////////////////////////////////////////////////////////
		// Set the buttons text.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetText(const std::string &textRef);

		// /////////////////////////////////////////////////////////////////
		// Get the current list selection string.
		//
		// NB This gives the same result as calling the base class, 
		// ButtonControl's VGetText() method!
		//
		// /////////////////////////////////////////////////////////////////
		inline std::string GetSelection() { return (ButtonControl::VGetText()); };

		// /////////////////////////////////////////////////////////////////
		// Method that returns the value/setting data of the control.  This
		// data will be in the form of a lua table so lua scripts may react
		// to control events appropriately.
		//
		// A ListButton returns a lua table continaing:
		// "{ Text = "Currently displayed text", ScreenElementId = sid, EventTypeId = eid }"
		//
		// @return LuaPlus::LuaObject The lua script data encapsulating the
		//								controls state.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaData();

		// /////////////////////////////////////////////////////////////////
		// Method that updates the current state(s) of the widget from a
		// LuaObject from application code or from script code.
		//
		// @param widgetData The lua script data encapsulating the
		//								controls new state.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VSetLuaData(const LuaPlus::LuaObject &widgetData);

	};

}

#ifdef WIN32
#	pragma warning( pop )
#endif

#endif
