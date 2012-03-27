// /////////////////////////////////////////////////////////////////
// @file SliderControl.cpp
// @author PJ O Halloran
// @date 21/10/2010
//
// Class for a slider control widget in my OpenGL based UI library.
//
// /////////////////////////////////////////////////////////////////

// External Headers


// Project Headers
#include "SliderControl.h"

#include "GameBase.h"
#include "Events.h"
#include "ZipFile.h"

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Point3 SliderControl::CalculateButtonPositionFromSlider()
	{
		Point3 butPos(VGetPosition());
		F32 sliderLeftX, sliderRightX;
		F32 butW = GetProjectedButtonWidth();
		//F32 butH = GetProjectedButtonHeight();

		sliderLeftX = butPos.GetX();
		sliderRightX = sliderLeftX + VGetWidth() - butW;

		F32 buttonX = InterpolateFloat(m_sliderPos, sliderLeftX, sliderRightX);

		if(buttonX + butW > sliderRightX)
		{
			buttonX -= (buttonX + butW) - sliderRightX;
		}

		butPos.SetX(buttonX);

		return (butPos);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	F32 SliderControl::CalculateSliderPositionFromButton()
	{
		F32 halfButtonWidth = m_sliderButPtr->VGetWidth() / 2.0f;

		// Width of the slider line
		F32 sliderLineWidth = VGetWidth() - halfButtonWidth;

		// Current X position of the button.
		F32 currButtonX = m_sliderButPtr->VGetPosition().GetX() + halfButtonWidth;

		// Position of the slider button relative to the slider.
		F32 sliderXPos = currButtonX - VGetPosition().GetX();

		// If the button is right at either end of the slider line...
		if(FloatCmp(sliderXPos, halfButtonWidth))
		{
			return (0.0f);
		}
		else if(FloatCmp(sliderXPos, sliderLineWidth))
		{
			return (1.0f);
		}

		F32 finalPos = sliderXPos / sliderLineWidth;
		Clamp(finalPos, 0.0f, 1.0f);
		return (finalPos);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SliderControl::RebuildSliderLine()
	{
		const F32 halfHeight = VGetHeight() / 2.0f;
		Point3 beginPos(VGetPosition()), endPos(VGetPosition());
		beginPos.SetY(beginPos.GetY() - halfHeight);
		endPos.SetX(endPos.GetX() + VGetWidth());
		endPos.SetY(endPos.GetY() - halfHeight);

		m_sliderLineBatch.reset(GCC_NEW GLBatch());
		m_sliderLineBatch->Begin(GL_LINES, 2);
			m_sliderLineBatch->Vertex3f(beginPos.GetX(), beginPos.GetY(), beginPos.GetZ());
			m_sliderLineBatch->Vertex3f(endPos.GetX(), endPos.GetY(), endPos.GetZ());
		m_sliderLineBatch->End();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SliderControl::SetLuaSliderPosition(const LuaPlus::LuaObject &table)
	{
		if(table.IsNumber())
		{
			m_sliderPos = static_cast<F32>(table.GetNumber());
			Clamp(m_sliderPos, 0.0f, 1.0f);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SliderControl::SetLuaEventId(const LuaPlus::LuaObject &table)
	{
		if(table.IsInteger())
		{
			m_eventTypeId = table.GetInteger();
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SliderControl::CreateDefaultButton(const Vector4 &colorRef,\
											boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
											boost::shared_ptr<FTFont> fontPtr,\
											const boost::shared_ptr<GLSLShader> shaderFlatObj,\
											const boost::shared_ptr<GLSLShader> shaderTexObj,\
											const bool visible,\
											const bool enabled) throw (GameException &)
	{
		// Create a button (but disable events when a user clicks on it)
		m_sliderButPtr.reset(GCC_NEW ButtonControl(CalculateButtonPositionFromSlider(),\
													colorRef,\
													mvpStackManPtr,\
													GetProjectedButtonWidth(),\
													GetProjectedButtonHeight(),\
													fontPtr,\
													shaderFlatObj,\
													shaderTexObj,\
													0,\
													std::string("textures") + ZipFile::ZIP_PATH_SEPERATOR + std::string("sliderBut.tga"),\
													std::string("textures") + ZipFile::ZIP_PATH_SEPERATOR + std::string("sliderBut.tga"),\
													std::string("textures") + ZipFile::ZIP_PATH_SEPERATOR + std::string("sliderBut.tga"),\
													std::string(""),\
													visible,\
													0,\
													enabled,\
													false));
		// Ensure slider button has same visible/enabled state as slider control itself.
		m_sliderButPtr->VSetVisible(VIsVisible());
		m_sliderButPtr->VSetEnabled(VIsEnabled());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	SliderControl::SliderControl(const Point3 &posRef,\
									const Vector4 &colorRef,\
									boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
									const F32 width,\
									const F32 height,\
									boost::shared_ptr<FTFont> fontPtr,\
									const boost::shared_ptr<GLSLShader> shaderFlatObj,\
									const boost::shared_ptr<GLSLShader> shaderTexObj,\
									const F32 sliderPos,\
									const I32 eventTypeId,\
									const std::string &textureNameRef,\
									const bool visible,\
									const ScreenElementId id,\
									const bool enabled) throw (GameException &)\
									: ControlWidget(posRef, colorRef, mvpStackManPtr, width, height, fontPtr, shaderFlatObj, shaderTexObj, textureNameRef, visible, id, enabled),
										m_sliderPos(sliderPos), m_sliderButPtr(), m_sliderLineBatch(), m_sliding(false), m_eventTypeId(eventTypeId), m_lineColor(0.0f, 0.0f, 0.0f, 1.0f)
	{
		Clamp(m_sliderPos, 0.0f, 1.0f);

		CreateDefaultButton(colorRef, mvpStackManPtr, fontPtr, shaderFlatObj, shaderTexObj, visible, enabled);

		RebuildSliderLine();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	SliderControl::SliderControl(const LuaPlus::LuaObject &widgetScriptData,\
									boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
									const boost::shared_ptr<GLSLShader> shaderFlatObj,\
									const boost::shared_ptr<GLSLShader> shaderTexObj,\
									boost::shared_ptr<FTFont> fontPtr,\
									const ScreenElementId id) throw (GameException &)\
									: ControlWidget(widgetScriptData, mvpStackManPtr, shaderFlatObj, shaderTexObj, fontPtr, id),\
										m_sliderPos(0.5f), m_sliderButPtr(), m_sliderLineBatch(), m_sliding(false), m_eventTypeId(0), m_lineColor(0.0f, 0.0f, 0.0f, 1.0f)
	{
		SetLuaSliderPosition(widgetScriptData.GetByName("SliderPosition"));
		SetLuaEventId(widgetScriptData.GetByName("EventTypeId"));
		std::string buttonTableName;
		LuaPlus::LuaObject tableName = widgetScriptData.GetByName("ButtonTableId");
		if(tableName.IsString())
		{
			LuaPlus::LuaObject buttonData = widgetScriptData.GetByName(tableName.GetString());
			if(buttonData.IsTable())
			{
				// Create a button (but disable events when a user clicks on it)
				m_sliderButPtr.reset(GCC_NEW ButtonControl(buttonData, mvpStackManPtr, shaderFlatObj, shaderTexObj, fontPtr, 0));

				// We need to explicitly set some important slider button attributes as we cannot rely on the scripter to input them correctly.
				m_sliderButPtr->VSetPosition(CalculateButtonPositionFromSlider());
				m_sliderButPtr->VSetText("");
				m_sliderButPtr->VSetWidth(GetProjectedButtonWidth());
				m_sliderButPtr->VSetHeight(GetProjectedButtonHeight());
				m_sliderButPtr->VSetVisible(VIsVisible());
				m_sliderButPtr->VSetEnabled(VIsEnabled());
				m_sliderButPtr->SetSendEvent(false);
			}
			else
			{
				// We have to create a default button in code!
                GF_LOG_TRACE_ERR("SliderControl::SliderControl()", "Creation of scripted slider button failed.  Creating default button");
				CreateDefaultButton(VGetColor(), mvpStackManPtr, fontPtr, shaderFlatObj, shaderTexObj, VIsVisible(), VIsEnabled());
			}
		}
		else
		{
			// We have to create a default button in code!
            GF_LOG_TRACE_ERR("SliderControl::SliderControl()", "Missing slider button information from script so creating default button");
			CreateDefaultButton(VGetColor(), mvpStackManPtr, fontPtr, shaderFlatObj, shaderTexObj, VIsVisible(), VIsEnabled());
		}

		RebuildSliderLine();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SliderControl::VOnRestore()
	{
		m_sliderButPtr->VOnRestore();
		return (ControlWidget::VOnRestore());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SliderControl::VOnLostDevice()
	{
		m_sliderButPtr->VOnLostDevice();
		ControlWidget::VOnLostDevice();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SliderControl::VOnRender(const F64 time, const F32 elapsedTime)
	{
		bool result = ControlWidget::VOnRender(time, elapsedTime);

		// Render the slider line and then the button.
		if(result && VIsVisible())
		{
			PreRenderFlatWidget(boost::optional<Vector4>(m_lineColor));
			m_sliderLineBatch->VDraw();

			result = m_sliderButPtr->VOnRender(time, elapsedTime);
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SliderControl::VOnUpdate(const F32 elapsedTime)
	{
		// TODO: 
		m_sliderButPtr->VOnUpdate(elapsedTime);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SliderControl::VOnEvent(GF_Event &eventObj, const F32 elapsedTime)
	{
		bool result = true;

		// Propagate the event to the sliders button before we update the slider.
		result = m_sliderButPtr->VOnEvent(eventObj, elapsedTime);

		if(result)
		{
			switch(eventObj.id)
			{
				case GF_MOUSE_MOVE_EVENT:
				{
					const F32 mouseX = static_cast<F32>(eventObj.mouseMove.x);
					const F32 leftSide = VGetPosition().GetX();
					const F32 rightSide = leftSide + VGetWidth() - m_sliderButPtr->VGetWidth();

					//if(m_sliding && ((mouseX >= leftSide) && (mouseX <= rightSide)))
					if(m_sliding)
					{
						F32 newX = mouseX;
						if(newX < leftSide)
						{
							newX = leftSide;
						}
						else if(newX > rightSide)
						{
							newX = rightSide;
						}
						m_sliderButPtr->VSetPosition(Point3(newX, VGetPosition().GetY(), 0.0f));
					}
				}
				break;

				case GF_MOUSE_BUTTON_EVENT:
				{
					// Mouse button press
					if((eventObj.mouseButton.buttonId == GLFW_MOUSE_BUTTON_LEFT) && (eventObj.mouseButton.state == GLFW_PRESS))
					{
						// Change the position of the button on its x axis
						if(m_sliderButPtr->IsPressed())
						{
							m_sliding = true;
						}
					}
					// Mouse button release
					else if ((eventObj.mouseButton.buttonId == GLFW_MOUSE_BUTTON_LEFT) && (eventObj.mouseButton.state == GLFW_RELEASE))
					{
						if(!m_sliderButPtr->IsPressed() && m_sliding)
						{
							m_sliding = false;
							// Trigger final slider pos update event here to notify the app that the sliders position has changed.
							result = VOnAction();
						}
					}
				}
				break;

				default: // do nothing for all other events
				break;
			}
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SliderControl::VOnAction()
	{
		//bool result = true;
		m_sliderPos = CalculateSliderPositionFromButton();
		//std::cout << "Slider Pos: " << m_sliderPos << std::endl;
		IEventDataPtr sliderActionEvent(GCC_NEW EvtData_Slider_Action(AbstractWidget::VGetId(), m_eventTypeId, m_sliderPos));
		if(!safeQueEvent(sliderActionEvent))
		{
			std::string idStr;
			try { idStr = boost::lexical_cast<std::string, ScreenElementId>(AbstractWidget::VGetId()); } catch(...) {}
            GF_LOG_TRACE_ERR("SliderControl::VOnAction()", std::string("Failed to send the EvtData_Slider_Action event for the button ") + idStr);
			return (false);
		}
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SliderControl::VSetPosition(const Point3 &posRef)
	{
		ControlWidget::VSetPosition(posRef);
		RebuildSliderLine();
		m_sliderButPtr->VSetPosition(CalculateButtonPositionFromSlider());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SliderControl::VSetWidth(const F32 width)
	{
		ControlWidget::VSetWidth(width);
		RebuildSliderLine();
		m_sliderButPtr->VSetWidth(GetProjectedButtonWidth());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SliderControl::VSetHeight(const F32 height)
	{
		ControlWidget::VSetHeight(height);
		RebuildSliderLine();
		m_sliderButPtr->VSetHeight(GetProjectedButtonHeight());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SliderControl::VSetColor(const Vector4 &colorRef)
	{
		ControlWidget::VSetColor(colorRef);
		Vector4 oldButtonColor = m_sliderButPtr->VGetColor();
		oldButtonColor.SetW(colorRef.GetW());
		m_sliderButPtr->VSetColor(oldButtonColor);
		m_lineColor.SetW(colorRef.GetW());
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SliderControl::VSetEnabled(const bool enabled)
	{
		ControlWidget::VSetEnabled(enabled);
		m_sliderButPtr->VSetEnabled(enabled);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SliderControl::VSetVisible(const bool visible)
	{
		AbstractWidget::VSetVisible(visible);
		m_sliderButPtr->VSetVisible(visible);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	LuaPlus::LuaObject SliderControl::VGetLuaData()
	{
		LuaPlus::LuaObject table;
		table.AssignNewTable(g_appPtr->GetLuaStateManager()->GetGlobalState().Get());
		table.SetNumber("SliderPosition", m_sliderPos);
		table.SetInteger("EventTypeId", m_eventTypeId);
		table.SetInteger("ScreenElementId", VGetId());
		return (table);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SliderControl::VSetLuaData(const LuaPlus::LuaObject &widgetData)
	{
		I32 currEventType(0);
		bool a = SetFloatFromLua(widgetData["SliderPosition"], m_sliderPos);
		bool b = SetIntFromLua(widgetData["EventTypeId"], currEventType);
		if(a && b)
		{
			m_sliderButPtr->VSetPosition(CalculateButtonPositionFromSlider());
			SetEventType(currEventType);
		}
		return (a && b);
	}

}
