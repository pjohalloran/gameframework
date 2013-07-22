#pragma once
#ifndef __GF_CONTAINER_WIDGET_H
#define __GF_CONTAINER_WIDGET_H

// /////////////////////////////////////////////////////////////////
// @file ContainerWidget.h
// @author PJ O Halloran
// @date 09/07/2010
//
// Base level container for control widgets in my OpenGL based UI
// library.
//
// /////////////////////////////////////////////////////////////////

#ifdef WIN32
#   pragma warning( push )
#   pragma warning( disable:4290 )
#endif

#include <vector>
#include <boost/shared_ptr.hpp>
#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

#include "AbstractWidget.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // @class ContainerWidget
    // @author PJ O Halloran
    //
    // Container widget for a group of control widgets (buttons,
    // textboxes, etc.).  The container manages how all control widgets
    // are displayed.  Each control widgets position is relative to the
    // container widget it is managed by.
    //
    // /////////////////////////////////////////////////////////////////
    class ContainerWidget : public AbstractWidget {
    private:

        WidgetList m_list;                                  ///< List of controls to manage.

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
        // @param shaderFlatObj The GLSL shader object used to render the un-textured widget.
        // @param shaderTexObj The GLSL shader object used to render the textured widget.
        // @param textureNameRef Name of texture to apply. An empty string disables it.
        // @param visible Is the widget visible.
        // @param id Unique ID of widget.
        //
        // @throw GameException& If the shader object used is NULL.
        //
        // /////////////////////////////////////////////////////////////////
        explicit ContainerWidget(const Point3 &posRef, \
                                 const Vector4 &colorRef, \
                                 boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr, \
                                 const F32 width, \
                                 const F32 height, \
                                 const boost::shared_ptr<GLSLShader> shaderFlatObj, \
                                 const boost::shared_ptr<GLSLShader> shaderTexObj, \
                                 const std::string &textureNameRef = std::string(), \
                                 const std::string &atlasNameRef = std::string(), \
                                 const bool visible = true, \
                                 const ScreenElementId id = SCREEN_ELEMENT_ID_NOT_ASSIGNED) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Contructor for widgets defined in an external LUA script.
        //
        // If LUA data is missing then the Widget will just use default
        // values.  i.e. If position data is missing then the widget will
        // just be located at 0, 0, 0!
        //
        // @param widgetScriptData Lua object containing the widget data.
        // @param mvpStackManPtr Pointer to the MVP stack manager.
        // @param shaderFlatObj Flat shader to use.  Will be overridden
        //                      if the scripter defined a flat shader.
        // @param shaderTexObj Texture shader to use.  Will be overridden
        //                      if the scripter defined a texture shader.
        // @param id The screen element ID of the widget.
        //
        // @throw GameException& If the shader object used is NULL.
        // @throw GameException& If Lua data is invalid or incomplete.
        //
        // /////////////////////////////////////////////////////////////////
        explicit ContainerWidget(const LuaPlus::LuaObject &widgetScriptData, \
                                 boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr, \
                                 const boost::shared_ptr<GLSLShader> shaderFlatObj, \
                                 const boost::shared_ptr<GLSLShader> shaderTexObj, \
                                 const ScreenElementId id = SCREEN_ELEMENT_ID_NOT_ASSIGNED) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~ContainerWidget() { };

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
        // Called when the applications screen resolution is changed.
        //
        // @param oldWidth The old screen width.
        // @param oldHeight The old screen height.
        // @param newWidth The new screen width.
        // @param newHeight The new screen height.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VOnResolutionChange(const I32 oldWidth, const I32 oldHeight, const I32 newWidth, const I32 newHeight);

        // /////////////////////////////////////////////////////////////////
        // Render the screen element.
        //
        // @param fTime The total time in seconds so far.
        // @param fElapsedTime The time elapsed since the last render call.
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
        // Called once per game loop so the element can check if there was
        // any input from the operating system or from the user.
        //
        // @param eventObj Event structure.
        // @param elapsedTime Number of seconds since the last event poll.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VOnEvent(GF_Event &eventObj, const F32 elapsedTime);

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

        // /////////////////////////////////////////////////////////////////
        // Set the position of the top left of the widget.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSetPosition(const Point3 &posRef);

        // /////////////////////////////////////////////////////////////////
        // Set the color of the widget.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSetColor(const Vector4 &colorRef);

        // /////////////////////////////////////////////////////////////////
        // Set if the element is currently visible.
        //
        // @param visible True if visible, false if not.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VSetVisible(const bool visible);

        // /////////////////////////////////////////////////////////////////
        // Add a control to the container.
        //
        // @param controlPtr Pointer to the control widget to add.
        //
        // @return bool True if it was added and false if not.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VAddControl(boost::shared_ptr<AbstractWidget> controlPtr);

        // /////////////////////////////////////////////////////////////////
        // Remove a control from the container.
        //
        // @param controlId The screen ID of the control to remove.
        //
        // @return bool True if the control was found and removed and false
        //              if not.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VRemoveControl(const ScreenElementId controlId);

        // /////////////////////////////////////////////////////////////////
        // Method that returns the value/setting data of the widget.  This
        // data will be in the form of a lua table.
        //
        // @return LuaPlus::LuaObject The lua script data encapsulating the
        //                              controls state.
        //
        // /////////////////////////////////////////////////////////////////
        virtual LuaPlus::LuaObject VGetLuaData() {
            return (LuaPlus::LuaObject());
        };

        // /////////////////////////////////////////////////////////////////
        // Method that updates the current state(s) of the widget from a
        // LuaObject from application code or from script code.
        //
        // @param widgetData The lua script data encapsulating the
        //                              controls new state.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VSetLuaData(const LuaPlus::LuaObject &widgetData) {
            return (false);
        };

        // /////////////////////////////////////////////////////////////////
        // Get the lua data for a child managed by the container.
        //
        // @param childId The ID of the child to retrieve the lua data for.
        // @param controlData The LuaObject to hold the childs lua data.  Will
        //                      only contain valid data if this method returns
        //                      true.
        //
        // @return bool True if the container contains the child and false
        //                  if not.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VGetChildLuaData(const ScreenElementId childId, LuaPlus::LuaObject &controlData);

        // /////////////////////////////////////////////////////////////////
        // Set the lua data for a child managed by the container.
        //
        // @param childId The ID of the child to set the lua data for.
        // @param widgetData The script data containing the new state of the
        //                      widget.
        //
        // @return bool True if the container contains the child and false
        //                  if not or false if an error occurred setting the
        //                  widget data.
        //
        // /////////////////////////////////////////////////////////////////
        virtual bool VSetChildLuaData(const ScreenElementId childId, const LuaPlus::LuaObject &widgetData);

        // /////////////////////////////////////////////////////////////////
        // Set visibility status of a child widget of the container.
        //
        // @param childId The ID of the child.
        // @param visible Show/Hide the child?
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool SetChildVisibility(const ScreenElementId childId, const bool visible);

    };

}

#ifdef WIN32
#   pragma warning( pop )
#endif

#endif
