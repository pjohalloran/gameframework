#pragma once
#ifndef __GF_BUTTON_CONTROL_WIDGET_H
#define __GF_BUTTON_CONTROL_WIDGET_H

// /////////////////////////////////////////////////////////////////
// @file ButtonControl.h
// @author PJ O Halloran
// @date 21/10/2010
//
// Class for the standerd button control widgets in
// my OpenGL based UI library.
//
// /////////////////////////////////////////////////////////////////

#ifdef WIN32
#   pragma warning( push )
#   pragma warning( disable:4290 )
#endif

#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

#include "AbstractButtonControl.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // @class ButtonControl
    // @author PJ O Halloran
    //
    // Class for the standerd button control UI object.
    //
    // /////////////////////////////////////////////////////////////////
    class ButtonControl : public AbstractButtonControl {
    private:

        TexHandle m_hoverHandle;                            ///< ID of texture while the mouse is hovering over.
        TexHandle m_pressedHandle;                          ///< ID of texture while the button is pressed.
        bool m_sendEvent;                                   ///< Should this button send an event when the button is clicked?
        AtlasImage m_hoverDim;                              ///< Tex coords for hover image.
        AtlasImage m_pressedDim;                            ///< Tex coords for pressed image.

        // /////////////////////////////////////////////////////////////////
        // Constructor helper function.
        //
        // @param textureHoverRef Name of the buttons "hover" texture.
        // @param texturePressedRef Name of the buttons "pressed" texture.
        //
        // @throw GameException& If the shader object used is NULL.
        //
        // /////////////////////////////////////////////////////////////////
        void Init(const std::string &textureHoverRef, const std::string &texturePressedRef) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Constructor helper function. Gets the hover and pressed tex names
        // from the Lua object.
        //
        // /////////////////////////////////////////////////////////////////
        bool SetLuaTextures(const LuaPlus::LuaObject &hoverTable, const LuaPlus::LuaObject &pressTable, std::string &hoverRef, std::string &pressedRef);

        // /////////////////////////////////////////////////////////////////
        // Constructor helper function. Gets the send event
        // from the Lua object.
        //
        // /////////////////////////////////////////////////////////////////
        void SetLuaSendEvent(const LuaPlus::LuaObject &table);

        // /////////////////////////////////////////////////////////////////
        // Constructor helper function. Gets the button text
        // from the Lua object.
        //
        // /////////////////////////////////////////////////////////////////
        void SetLuaText(const LuaPlus::LuaObject &table);

    protected:

        std::string m_text;                                 ///< Text of the button.

        // /////////////////////////////////////////////////////////////////
        // Action to perform when the mouse is hovering over the control.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnMouseEnter();

        // /////////////////////////////////////////////////////////////////
        // Action to perform when the mouse is leaves the controls area.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnMouseLeave();

        // /////////////////////////////////////////////////////////////////
        // Action to perform when the mouse is pressed while hovering over
        // the control.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnMousePressed(const U32 buttonId);

        // /////////////////////////////////////////////////////////////////
        // Action to perform when the mouse is released inside the control
        // while previously pressed over the control.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnMouseReleased(const U32 buttonId);

        // /////////////////////////////////////////////////////////////////
        // Action to perform when the mouse is released outside the control
        // while previously pressed over the control.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnMouseReleasedCancel(const U32 buttonId);

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
        // @param textRef The buttons text to render centered over the button.
        // @param eventTypeId The ID of the event type to generate upon an action.
        // @param visible Is the widget visible.
        // @param id Unique ID of widget.
        // @param enabled Is the control initially enabled.
        // @param sendEvent Should we send a EvtData_Button_Action event into
        //                  the global event queue when the button is clicked?
        //
        // @throw GameException& If the shader object used is NULL.
        //
        // /////////////////////////////////////////////////////////////////
        explicit ButtonControl(const Point3 &posRef,
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
                               const bool visible = true,
                               const ScreenElementId id = SCREEN_ELEMENT_ID_NOT_ASSIGNED,
                               const bool enabled = true,
                               const bool sendEvent = true) throw(GameException &);

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
        explicit ButtonControl(const LuaPlus::LuaObject &widgetScriptData,
                               boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,
                               const boost::shared_ptr<GLSLShader> shaderFlatObj,
                               const boost::shared_ptr<GLSLShader> shaderTexObj,
                               boost::shared_ptr<FTFont> fontPtr,
                               const ScreenElementId id = SCREEN_ELEMENT_ID_NOT_ASSIGNED) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~ButtonControl() { };

        // /////////////////////////////////////////////////////////////////
        // Method called when the button is clicked.
        //
        // Adds an event of type EvtData_Button_Action to the global event
        // queue to notify the application that this button (identified by
        // its event type and screen id) was clicked.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnAction();

        // /////////////////////////////////////////////////////////////////
        // Set the buttons text.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSetText(const std::string &textRef);

        // /////////////////////////////////////////////////////////////////
        // Get the buttons text.
        //
        // /////////////////////////////////////////////////////////////////
        virtual std::string VGetText() const {
            return (m_text);
        };

        // /////////////////////////////////////////////////////////////////
        // Update the screen element.
        //
        // @param elapsedTime The number of seconds since the last update.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VOnUpdate(const F32 elapsedTime);

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
        // Enable or disable hooking up the button to the GCC event system.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetSendEvent(const bool se) {
            m_sendEvent = se;
        };

        // /////////////////////////////////////////////////////////////////
        // Method that returns the value/setting data of the widget.  This
        // data will be in the form of a lua table.
        //
        // A buttons lua data will contain:
        // { Text = "Button text", ScreenElementId = sid }
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
