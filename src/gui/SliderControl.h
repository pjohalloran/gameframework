#ifndef __GF_SLIDER_CONTROL_WIDGET_H
#define __GF_SLIDER_CONTROL_WIDGET_H

// /////////////////////////////////////////////////////////////////
// @file SliderControl.h
// @author PJ O Halloran
// @date 21/10/2010
//
// Class for a slider control widget in my OpenGL based UI library.
//
// /////////////////////////////////////////////////////////////////

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

// External Headers
#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>


// Project Headers
#include "ControlWidget.h"
#include "CommonMath.h"
#include "ButtonControl.h"

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class SliderControl
	// @author PJ O Halloran
	//
	// Class that implements a slider control for the UI.
	//
	// /////////////////////////////////////////////////////////////////
	class SliderControl : public ControlWidget
	{
	private:

		F32 m_sliderPos;									///< Position of the slider along the x axis.  Number is clamped between 0.0 and 1.0.
		boost::shared_ptr<ButtonControl> m_sliderButPtr;	///< Slider button.
		boost::shared_ptr<GLBatch> m_sliderLineBatch;		///< The slider line geometry batch;
		bool m_sliding;										///< Is the slider button currently being dragged by a user?
		I32 m_eventTypeId;									///< ID of the sliders event type.
		Vector4 m_lineColor;								///< The color of the slider line.

		// /////////////////////////////////////////////////////////////////
		// Calculate the position of the slider button based on the current
		// slider position interpolated value.
		//
		// /////////////////////////////////////////////////////////////////
		Point3 CalculateButtonPositionFromSlider();
		
		// /////////////////////////////////////////////////////////////////
		// Calculate the new slider value from the current position of the
		// button on the slider line.
		//
		// /////////////////////////////////////////////////////////////////
		F32 CalculateSliderPositionFromButton();

		// /////////////////////////////////////////////////////////////////
		// Rebuild the slider line geometry.
		//
		// /////////////////////////////////////////////////////////////////
		void RebuildSliderLine();

		// /////////////////////////////////////////////////////////////////
		// Formula for calculating how wide the button should be in relation
		// to the width of the slider control widget.
		//
		// 10% of slider width.
		//
		// /////////////////////////////////////////////////////////////////
		inline F32 GetProjectedButtonWidth()
		{
			return (VGetWidth() / 100.0f * 10.0f);
		};

		// /////////////////////////////////////////////////////////////////
		// Formula for calculating how high the button should be in relation
		// to the height of the slider control widget.
		//
		// 100% of slider height.
		//
		// /////////////////////////////////////////////////////////////////
		inline F32 GetProjectedButtonHeight()
		{
			return (VGetHeight());
		};

		// /////////////////////////////////////////////////////////////////
		// Constructor helper function.  Gets slider position from lua data.
		//
		// /////////////////////////////////////////////////////////////////
		void SetLuaSliderPosition(const LuaPlus::LuaObject &table);

		// /////////////////////////////////////////////////////////////////
		// Constructor helper function.  Gets event type ID from lua data.
		//
		// /////////////////////////////////////////////////////////////////
		void SetLuaEventId(const LuaPlus::LuaObject &table);

		// /////////////////////////////////////////////////////////////////
		// Constructor helper funtion. Create a default button for the slider.
		//
		// @param colorRef Color of quad.
		// @param mvpStackManPtr Pointer to the MVP stack manager.
		// @param shaderFlatObj The GLSL shader object used to render the un-textured widget.
		// @param shaderTexObj The GLSL shader object used to render the textured widget.
		// @param fontPtr Pointer to the FTGL font to use to render text.
		// @param visible Is the widget visible.
		// @param enabled Is the control initially enabled.
		//
		// @throw GameException& If we fail to create the button for any reason.
		//
		// /////////////////////////////////////////////////////////////////
		void CreateDefaultButton(const Vector4 &colorRef,\
									boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
									boost::shared_ptr<FTFont> fontPtr,\
									const boost::shared_ptr<GLSLShader> shaderFlatObj,\
									const boost::shared_ptr<GLSLShader> shaderTexObj,\
									const bool visible,\
									const bool enabled) throw (GameException &);

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
		// @param sliderPos The initial position of the slider (>= 0 && <= 1).
		// @param eventTypeId The ID of the event that is sent when the slider position
		//						is adjusted.
		// @param textureNameRef Name of texture to apply. An empty string disables it.
		// @param visible Is the widget visible.
		// @param id Unique ID of widget.
		// @param enabled Is the control initially enabled.
		//
		// @throw GameException& If the shader object used is NULL.
		//
		// /////////////////////////////////////////////////////////////////
		explicit SliderControl(const Point3 &posRef,\
								const Vector4 &colorRef,\
								boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
								const F32 width,\
								const F32 height,\
								boost::shared_ptr<FTFont> fontPtr,\
								const boost::shared_ptr<GLSLShader> shaderFlatObj,\
								const boost::shared_ptr<GLSLShader> shaderTexObj,\
								const F32 sliderPos,\
								const I32 eventTypeId,\
								const std::string &textureNameRef = std::string(),\
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
		explicit SliderControl(const LuaPlus::LuaObject &widgetScriptData,\
								boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
								const boost::shared_ptr<GLSLShader> shaderFlatObj,\
								const boost::shared_ptr<GLSLShader> shaderTexObj,\
								boost::shared_ptr<FTFont> fontPtr,\
								const ScreenElementId id = SCREEN_ELEMENT_ID_NOT_ASSIGNED) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		// 
		// /////////////////////////////////////////////////////////////////
		virtual ~SliderControl() { };

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
		// Get the sliders position.
		//
		// /////////////////////////////////////////////////////////////////
		virtual F32 VGetSliderPosition() const { return (m_sliderPos); };

		// /////////////////////////////////////////////////////////////////
		// Set the sliders position.  The new position must be between 0.0
		// and 1.0.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetSliderPosition(const F32 sliderPos)
		{
			F32 tmp = sliderPos;
			Clamp(tmp, 0.0f, 1.0f);
			m_sliderPos = tmp;
			CalculateButtonPositionFromSlider();
		};

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

		// /////////////////////////////////////////////////////////////////
		// Method called when the slider position is updated.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnAction();

		// /////////////////////////////////////////////////////////////////
		// Get the event type ID this slider includes in its event when the
		// slider position is updated.
		//
		// /////////////////////////////////////////////////////////////////
		inline I32 GetEventType() const { return (m_eventTypeId); };

		// /////////////////////////////////////////////////////////////////
		// Set the event type ID this slider includes in its event when the
		// slider position is updated.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetEventType(const I32 eventTypeId) { m_eventTypeId = eventTypeId; };

		// /////////////////////////////////////////////////////////////////
		// Set the color of the widget.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetColor(const Vector4 &colorRef);

		// /////////////////////////////////////////////////////////////////
		// Change enabled status.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetEnabled(const bool enabled);

		// /////////////////////////////////////////////////////////////////
		// Change visible status.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetVisible(const bool visible);

		// /////////////////////////////////////////////////////////////////
		// Method that returns the value/setting data of the control.  This
		// data will be in the form of a lua table so lua scripts may react
		// to control events appropriately.
		//
		// A Slider returns a lua table containing:
		// "{SliderPosition = val, EventTypeId = eid, ScreenElementId = sid }".
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
