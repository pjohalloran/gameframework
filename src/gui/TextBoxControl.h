#pragma once
#ifndef __GF_TEXT_BOX_CONTROL_WIDGET_H
#define __GF_TEXT_BOX_CONTROL_WIDGET_H

// /////////////////////////////////////////////////////////////////
// @file TextBoxControl.h
// @author PJ O Halloran
// @date 21/10/2010
//
// Class for a textbox control widget in my OpenGL based UI library.
//
// /////////////////////////////////////////////////////////////////

#ifdef WIN32
#   pragma warning( push )
#   pragma warning( disable:4290 )
#endif

#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

#include "ControlWidget.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // @class TextBoxControl
    // @author PJ O Halloran
    //
    // Class that implements a textbox control for the UI.
    //
    // /////////////////////////////////////////////////////////////////
    class TextBoxControl : public ControlWidget {
    private:

        std::string m_text;                     ///< The text to display in the textbox.

        // /////////////////////////////////////////////////////////////////
        // Constructor helper funtion.  Sets the text from a Lua object.
        //
        // /////////////////////////////////////////////////////////////////
        void SetLuaText(const LuaPlus::LuaObject &table);

    protected:


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
        // @param textRef The text to display in the textbox.
        // @param textureNameRef Name of texture to apply. An empty string disables it.
        // @param visible Is the widget visible.
        // @param id Unique ID of widget.
        // @param enabled Is the control initially enabled.
        //
        // @throw GameException& If the shader object used is NULL.
        //
        // /////////////////////////////////////////////////////////////////
        explicit TextBoxControl(const Point3 &posRef, \
                                const Vector4 &colorRef, \
                                boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr, \
                                const F32 width, \
                                const F32 height, \
                                boost::shared_ptr<FTFont> fontPtr, \
                                const boost::shared_ptr<GLSLShader> shaderFlatObj, \
                                const boost::shared_ptr<GLSLShader> shaderTexObj, \
                                const std::string &textRef, \
                                const std::string &textureNameRef = std::string(), \
                                const std::string &atlasNameRef = std::string(), \
                                const bool visible = true, \
                                const ScreenElementId id = SCREEN_ELEMENT_ID_NOT_ASSIGNED, \
                                const bool enabled = true) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Contructor for widgets defined in an external LUA script.
        //
        // @param widgetScriptData Lua object containing the widget data.
        // @param mvpStackManPtr Pointer to the MVP stack manager.
        // @param shaderFlatObj Flat shader to use.  Will be overridden
        //                      if the scripter defined a flat shader.
        // @param shaderTexObj Texture shader to use.  Will be overridden
        //                      if the scripter defined a texture shader.
        // @param fontPtr Pointer to the FTGL font to use to render text.
        // @param id The screen element ID of the widget.
        //
        // @throw GameException& If the shader object used is NULL.
        // @throw GameException& If Lua data is invalid or incomplete.
        //
        // /////////////////////////////////////////////////////////////////
        explicit TextBoxControl(const LuaPlus::LuaObject &widgetScriptData, \
                                boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr, \
                                const boost::shared_ptr<GLSLShader> shaderFlatObj, \
                                const boost::shared_ptr<GLSLShader> shaderTexObj, \
                                boost::shared_ptr<FTFont> fontPtr, \
                                const ScreenElementId id = SCREEN_ELEMENT_ID_NOT_ASSIGNED) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~TextBoxControl() { };

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
        // Update the screen element.
        //
        // @param elapsedTime The number of seconds since the last update.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VOnUpdate(const F32 elapsedTime);

        // /////////////////////////////////////////////////////////////////
        // Get the text currently displayed.
        //
        // /////////////////////////////////////////////////////////////////
        virtual std::string VGetText() const {
            return (m_text);
        };

        // /////////////////////////////////////////////////////////////////
        // Set the text.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSetText(const std::string &textRef);

        // /////////////////////////////////////////////////////////////////
        // No event handling required for a textbox....
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnEvent(GF_Event &eventObj, const F32 elapsedTime) {
            return (true);
        };

        // /////////////////////////////////////////////////////////////////
        // Method that returns the value/setting data of the control.  This
        // data will be in the form of a lua table so lua scripts may react
        // to control events appropriately.
        //
        // A textbox returns a lua table containing a string:
        // "{Text = "TextBox Text", ScreenElementId = sid }".
        //
        // @return LuaPlus::LuaObject The lua script data encapsulating the
        //                              controls state.
        //
        // /////////////////////////////////////////////////////////////////
        virtual LuaPlus::LuaObject VGetLuaData();

        // /////////////////////////////////////////////////////////////////
        // Method that updates the current state(s) of the widget from a
        // LuaObject from application code or from script code.
        //
        // @param widgetData The lua script data encapsulating the
        //                              controls new state.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VSetLuaData(const LuaPlus::LuaObject &widgetData);

    };

}

#ifdef WIN32
#   pragma warning( pop )
#endif

#endif
