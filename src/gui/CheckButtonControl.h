#pragma once
#ifndef __GF_CHECK_BUTTON_CONTROL_WIDGET_H
#define __GF_CHECK_BUTTON_CONTROL_WIDGET_H

// /////////////////////////////////////////////////////////////////
// @file CheckButtonControl.h
// @author PJ O Halloran
// @date 21/10/2010
//
// Class for the check button type control widget in
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
    // @class CheckButtonControl
    // @author PJ O Halloran
    //
    // Base class for all button type control UI objects.
    //
    // /////////////////////////////////////////////////////////////////
    class CheckButtonControl : public AbstractButtonControl {
    private:

        bool m_checked;                                     ///< Is the button checked at present.
        TexHandle m_hoverHandle;                            ///< Handle to the hover texture.
        TexHandle m_checkHandle;                            ///< Handle to the checked texture.
        TexHandle m_checkHoverHandle;                       ///< Handle to the checked/hover handle.
        AtlasImage m_hoverDim;                              ///< Atlas image dim for hover image.
        AtlasImage m_checkDim;                              ///< Atlas image dim for checked image.
        AtlasImage m_checkHoverDim;                         ///< Atlas image dim for hover/checked image.

        // /////////////////////////////////////////////////////////////////
        // Constructor helper function.
        //
        // @param table The Checked lua data.
        //
        // /////////////////////////////////////////////////////////////////
        void SetLuaChecked(const LuaPlus::LuaObject &table);

        // /////////////////////////////////////////////////////////////////
        // Constructor helper function.
        //
        // @param hoverTable Hover tex lua data.
        // @param checkTable Checked tex lua data.
        // @param hoverCheckTable Hover, Checked tex lua data.
        // @param hoverNameRef Hover texture Id.
        // @param checkNameRef Checked texture Id.
        // @param checkHoverNameRef Hover, checked texture Id.
        //
        // /////////////////////////////////////////////////////////////////
        void SetLuaCbTextures(const LuaPlus::LuaObject &hoverTable, \
                              const LuaPlus::LuaObject &checkTable, \
                              const LuaPlus::LuaObject &hoverCheckTable, \
                              std::string &hoverNameRef, \
                              std::string &checkNameRef, \
                              std::string &checkHoverNameRef);

        // /////////////////////////////////////////////////////////////////
        // Constructor helper function.
        //
        // @param hoverNameRef Hover texture Id.
        // @param checkNameRef Checked texture Id.
        // @param checkHoverNameRef Hover, checked texture Id.
        //
        // /////////////////////////////////////////////////////////////////
        void Init(const std::string &hoverNameRef, const std::string &checkNameRef, const std::string &checkHoverNameRef);

    protected:

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

        // /////////////////////////////////////////////////////////////////
        // Set the button as checked.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetChecked(const bool c) {
            m_checked = c;
        };

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
        // @param eventTypeId The type of button event the checkbox generates.
        // @param textureNameRef Name of texture to apply. An empty string disables it.
        // @param hoverNameRef Name of texture to apply when the mouse is hovering over.
        // @param checkNameRef Name of texture to apply when its checked.
        // @param checkHoverNameRef Name of the texture to apply when the button is checked
        //                          and the mouse is hovering over it.
        // @param visible Is the widget visible.
        // @param id Unique ID of widget.
        // @param enabled Is the control initially enabled.
        //
        // @throw GameException& If the shader object used is NULL.
        //
        // /////////////////////////////////////////////////////////////////
        explicit CheckButtonControl(const Point3 &posRef, \
                                    const Vector4 &colorRef, \
                                    boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr, \
                                    const F32 width, \
                                    const F32 height, \
                                    boost::shared_ptr<FTFont> fontPtr, \
                                    const boost::shared_ptr<GLSLShader> shaderFlatObj, \
                                    const boost::shared_ptr<GLSLShader> shaderTexObj, \
                                    const I32 eventTypeId,
                                    const std::string &textureNameRef, \
                                    const std::string &atlasNameRef, \
                                    const std::string &hoverNameRef, \
                                    const std::string &checkNameRef, \
                                    const std::string &checkHoverNameRef, \
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
        explicit CheckButtonControl(const LuaPlus::LuaObject &widgetScriptData, \
                                    boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr, \
                                    const boost::shared_ptr<GLSLShader> shaderFlatObj, \
                                    const boost::shared_ptr<GLSLShader> shaderTexObj, \
                                    boost::shared_ptr<FTFont> fontPtr, \
                                    const ScreenElementId id = SCREEN_ELEMENT_ID_NOT_ASSIGNED) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~CheckButtonControl() { };

        // /////////////////////////////////////////////////////////////////
        // Update the screen element.
        //
        // @param elapsedTime The number of seconds since the last update.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VOnUpdate(const F32 elapsedTime);

        // /////////////////////////////////////////////////////////////////
        // Method called when the button is clicked.
        //
        // This will send out an event of type EvtData_Button_Action onto
        // the global event queue to notify the application of a check button
        // status change event.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnAction();

        // /////////////////////////////////////////////////////////////////
        // Check if the button is currently checked.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool IsChecked() const {
            return (m_checked);
        };

        // /////////////////////////////////////////////////////////////////
        // Method that returns the value/setting data of the control.  This
        // data will be in the form of a lua table so lua scripts may react
        // to control events appropriately.
        //
        // A CheckBox contains:
        // { Checked = true|false, ScreenElementId = sid, EventTypeId = eid }
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
