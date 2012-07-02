// /////////////////////////////////////////////////////////////////
// @file AbstractButtonControl.cpp
// @author PJ O Halloran
// @date 21/10/2010
//
// Base level class for button type control widgets in
// my OpenGL based UI library.
//
// /////////////////////////////////////////////////////////////////

// External Headers


// Project Headers
#include "AbstractButtonControl.h"

#include "OsInputEvents.h"

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
	void AbstractButtonControl::SetLuaEventTypeId(const LuaPlus::LuaObject &table)
	{
		if(table.IsInteger())
		{
			m_eventTypeId = table.GetInteger();
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	AbstractButtonControl::AbstractButtonControl(const Point3 &posRef,\
													const Vector4 &colorRef,\
													boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
													const F32 width,\
													const F32 height,\
													boost::shared_ptr<FTFont> fontPtr,\
													const boost::shared_ptr<GLSLShader> shaderFlatObj,\
													const boost::shared_ptr<GLSLShader> shaderTexObj,\
													const I32 eventTypeId,\
													const std::string &textureNameRef,\
                                                    const std::string &atlasNameRef,\
													const bool visible,\
													const ScreenElementId id,
													const bool enabled) throw (GameException &)\
													: ControlWidget(posRef, colorRef, mvpStackManPtr, width, height, fontPtr, shaderFlatObj, shaderTexObj,\
														textureNameRef, atlasNameRef, visible, id, enabled), m_pressed(false), m_mouseOver(false), m_eventTypeId(eventTypeId)
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	AbstractButtonControl::AbstractButtonControl(const LuaPlus::LuaObject &widgetScriptData,\
												boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
												const boost::shared_ptr<GLSLShader> shaderFlatObj,\
												const boost::shared_ptr<GLSLShader> shaderTexObj,\
												boost::shared_ptr<FTFont> fontPtr,\
												const ScreenElementId id) throw (GameException &)\
												: ControlWidget(widgetScriptData, mvpStackManPtr, shaderFlatObj, shaderTexObj, fontPtr, id),\
													m_pressed(false), m_mouseOver(false), m_eventTypeId(0)
	{
		SetLuaEventTypeId(widgetScriptData.GetByName("EventTypeId"));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool AbstractButtonControl::VOnEvent(GF_Event &eventObj, const F32 elapsedTime)
	{
		bool result = true;						// Result of method

		if(VIsEnabled() && VIsVisible())
		{
			switch(eventObj.id)
			{
				case GF_MOUSE_MOVE_EVENT:
				{
					Point3 cursorPt(static_cast<F32>(eventObj.mouseMove.x), static_cast<F32>(eventObj.mouseMove.y), 0.0f);
					ConvertWindowCoordinates(cursorPt);

					// Test if the cursor was previously NOT over the button AND if it currently is inside the button.
					if(!m_mouseOver && m_bb.IsPointInside(cursorPt))
					{
						m_mouseOver = true;
						result = VOnMouseEnter();
					}
					// If the cursor was previously over the button.
					else if(m_mouseOver && !m_bb.IsPointInside(cursorPt))
					{
						m_mouseOver = false;
						result = VOnMouseLeave();
					}
				}
				break;

				case GF_MOUSE_BUTTON_EVENT:
				{
					Point3 cursorPt(static_cast<F32>(eventObj.mouseButton.x), static_cast<F32>(eventObj.mouseButton.y), 0.0f);
					ConvertWindowCoordinates(cursorPt);

					// Pressed event
					if(eventObj.mouseButton.state == GLFW_PRESS)
					{
						// If the cursor was previously over the button OR if it is over the button now.
						if(m_mouseOver || m_bb.IsPointInside(cursorPt))
						{
							m_mouseOver = true;
							m_pressed = true;
							result = VOnMousePressed(eventObj.mouseButton.buttonId);
						}
					}
					// Release event
					else
					{
						// Check if the mouse was previously over the button AND that it was previously pressed
						//  over the button.
						if(m_mouseOver && m_pressed)
						{
							m_pressed = false;
							VOnMouseReleased(eventObj.mouseButton.buttonId);
							result = VOnAction();
						}
						// else the mouse is being released outside the button (take no action in this case!)
						else if(m_pressed)
						{
							m_pressed = false;
							result = VOnMouseReleasedCancel(eventObj.mouseButton.buttonId);
						}
					}
				}
				break;

				default: // do nothing for other types of events.
				break;
			}
		}

		return (result);
	}

}
