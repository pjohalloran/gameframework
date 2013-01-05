// /////////////////////////////////////////////////////////////////
// @file ContainerWidget.cpp
// @author PJ O Halloran
// @date 09/07/2010
//
// Base level container for control widgets in my OpenGL based UI
// library.
//
// /////////////////////////////////////////////////////////////////

#include <boost/lexical_cast.hpp>

#include "ContainerWidget.h"
#include "GameMain.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ContainerWidget::ContainerWidget(const Point3 &posRef,\
										const Vector4 &colorRef,\
										boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
										const F32 width,\
										const F32 height,\
										const boost::shared_ptr<GLSLShader> shaderFlatObj,\
										const boost::shared_ptr<GLSLShader> shaderTexObj,\
										const std::string &textureNameRef,\
                                        const std::string &atlasNameRef,\
										const bool visible,\
										const ScreenElementId id) throw (GameException &)\
										: AbstractWidget(posRef, colorRef, mvpStackManPtr, width, height, shaderFlatObj
											, shaderTexObj, textureNameRef, atlasNameRef, visible, id)
										, m_list()
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	ContainerWidget::ContainerWidget(const LuaPlus::LuaObject &widgetScriptData,\
										boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
										const boost::shared_ptr<GLSLShader> shaderFlatObj,\
										const boost::shared_ptr<GLSLShader> shaderTexObj,\
										const ScreenElementId id) throw (GameException &)
										: AbstractWidget(widgetScriptData, mvpStackManPtr, shaderFlatObj, shaderTexObj, id)
										, m_list()
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ContainerWidget::VOnRestore()
	{
		bool result = AbstractWidget::VOnRestore();
		if(result)
		{
			for(WidgetList::iterator i = m_list.begin(), end = m_list.end(); ((result) && (i != end)); ++i)
			{
				result = (*i)->VOnRestore();
#if DEBUG
				if(!result)
				{
					std::string idStr, containerIdStr;
					try
					{
						idStr = boost::lexical_cast<std::string, ScreenElementId>((*i)->AbstractWidget::VGetId());
						containerIdStr = boost::lexical_cast<std::string, ScreenElementId>(AbstractWidget::VGetId());
					}
					catch(...)
					{
					}
                    GF_LOG_TRACE_ERR("ContainerWidget::VOnRestore()", std::string("Failed to restore screen element ") + idStr + std::string(" held by the container ") + containerIdStr);
				}
#endif
			}
		}
#if DEBUG
		else
		{
			std::string idStr;
			try { idStr = boost::lexical_cast<std::string, ScreenElementId>(AbstractWidget::VGetId()); } catch(...) {}
            GF_LOG_TRACE_ERR("ContainerWidget::VOnRestore()", std::string("Failed to restore the container ") + idStr);
		}
#endif

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ContainerWidget::VOnLostDevice()
	{
		AbstractWidget::VOnLostDevice();
		for(WidgetList::iterator i = m_list.begin(), end = m_list.end(); i != end; ++i)
		{
			(*i)->VOnLostDevice();
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ContainerWidget::VOnResolutionChange(const I32 oldWidth, const I32 oldHeight, const I32 newWidth, const I32 newHeight)
	{
		AbstractWidget::VOnResolutionChange(oldWidth, oldHeight, newWidth, newHeight);

		for(WidgetList::iterator i = m_list.begin(), end = m_list.end(); i != end; ++i)
		{
			(*i)->VOnResolutionChange(oldWidth, oldHeight, newWidth, newHeight);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ContainerWidget::VOnRender(const F64 time, const F32 elapsedTime)
	{
		bool result = AbstractWidget::VOnRender(time, elapsedTime);
		if(result)
		{
			for(WidgetList::iterator i = m_list.begin(), end = m_list.end(); ((result) && (i != end)); ++i)
			{
				// Render the control if the top left of the widget is inside this containers region.
				if(AbstractWidget::m_bb.IsPointInside((*i)->AbstractWidget::VGetPosition()))
				{
                    GF_CLEAR_GL_ERROR();
					result = (*i)->VOnRender(time, elapsedTime) && GF_CHECK_GL_ERROR();
					if(!result)
					{
						std::string idStr, containerIdStr;
						try
						{
							idStr = boost::lexical_cast<std::string, ScreenElementId>((*i)->AbstractWidget::VGetId());
							containerIdStr = boost::lexical_cast<std::string, ScreenElementId>(AbstractWidget::VGetId());
						}
						catch(...)
						{
						}
                        GF_LOG_TRACE_ERR("ContainerWidget::VOnRender()", std::string("Failed to render screen element ") + idStr + std::string(" held by the container ") + containerIdStr);
					}
				}
			}
		}
		else
		{
			std::string idStr;
			try { idStr = boost::lexical_cast<std::string, ScreenElementId>(AbstractWidget::VGetId()); } catch(...) {}
            GF_LOG_TRACE_ERR("ContainerWidget::VOnRender()", std::string("Failed to render the container ") + idStr);
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ContainerWidget::VOnUpdate(const F32 elapsedTime)
	{
		for(WidgetList::iterator i = m_list.begin(), end = m_list.end(); i != end; ++i)
		{
			(*i)->VOnUpdate(elapsedTime);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ContainerWidget::VOnEvent(GF_Event &eventObj, const F32 elapsedTime)
	{
		bool result = true;
		for(WidgetList::iterator i = m_list.begin(), end = m_list.end(); ((result) && (i != end)); ++i)
		{
			result = (*i)->VOnEvent(eventObj, elapsedTime);
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ContainerWidget::VSetWidth(const F32 width)
	{
		AbstractWidget::VSetWidth(width);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ContainerWidget::VSetHeight(const F32 height)
	{
		AbstractWidget::VSetHeight(height);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ContainerWidget::VSetPosition(const Point3 &posRef)
	{
		Point3 oldPos = AbstractWidget::VGetPosition();

		AbstractWidget::VSetPosition(posRef);

		Vector3 diffVec = oldPos - posRef;
		for(WidgetList::iterator i = m_list.begin(), end = m_list.end(); i != end; ++i)
		{
			Point3 currOldPos((*i)->VGetPosition());
			(*i)->VSetPosition(currOldPos - diffVec);
		}

	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ContainerWidget::VSetColor(const Vector4 &colorRef)
	{
		AbstractWidget::VSetColor(colorRef);

		// Set all the widgets inside the container to have the same alpha value as the container,
		//  that way they all fade or become clearer together.
		const F32 alpha = colorRef.GetW();
		for(WidgetList::iterator i = m_list.begin(), end = m_list.end(); i != end; ++i)
		{
			Vector4 oldColor = (*i)->VGetColor();
			oldColor.SetW(alpha);
			(*i)->VSetColor(oldColor);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void ContainerWidget::VSetVisible(const bool visible)
	{
		AbstractWidget::VSetVisible(visible);

		for(WidgetList::iterator i = m_list.begin(), end = m_list.end(); i != end; ++i)
		{
			(*i)->VSetVisible(visible);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ContainerWidget::VAddControl(boost::shared_ptr<AbstractWidget> controlPtr)
	{
		if(!controlPtr)
		{
			return (false);
		}

		bool found = false;
		for(WidgetList::iterator i = m_list.begin(), end = m_list.end(); ((!found) && (i != end)); ++i)
		{
			if((*i) && (*i)->VGetId() == controlPtr->VGetId())
			{
				found = true;
			}
		}

		if(!found)
		{
			// TODO: Some housekeeping to ensure the control is inside the containers drawing region maybe??
			//  and maybe some rejigging of other controls to accomodate the new one??
			Point3 controlPos = controlPtr->VGetPosition();
			if(!m_bb.IsPointInside(controlPos))
			{
				controlPtr->VSetPosition(AbstractWidget::VGetPosition());
			}

			m_list.push_back(controlPtr);
		}
#if DEBUG
		else
		{
			std::string idStr, containerIdStr;
			try
			{
				idStr = boost::lexical_cast<std::string, ScreenElementId>(controlPtr->VGetId());
				containerIdStr = boost::lexical_cast<std::string, ScreenElementId>(AbstractWidget::VGetId());
			}
			catch(...)
			{
			}
            GF_LOG_TRACE_DEB("ContainerWidget::VAddControl()", std::string("Failed to add the screen element ") + idStr + std::string(" as it was already in the container ") + containerIdStr);
		}
#endif

		return (!found);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ContainerWidget::VRemoveControl(const ScreenElementId controlId)
	{
		bool found = false;

		for(WidgetList::iterator i = m_list.begin(), end = m_list.end(); ((!found) && (i != end)); ++i)
		{
			if((*i)->VGetId() == controlId)
			{
				i = m_list.erase(i);
				found = true;
			}
		}

#if DEBUG
		if(!found)
		{
			std::string idStr, containerIdStr;
			try
			{
				idStr = boost::lexical_cast<std::string, ScreenElementId>(controlId);
				containerIdStr = boost::lexical_cast<std::string, ScreenElementId>(AbstractWidget::VGetId());
			}
			catch(...)
			{
			}
            GF_LOG_TRACE_DEB("ContainerWidget::VRemoveControl()", std::string("Failed to remove the screen element ") + idStr + std::string(" held by the container ") + containerIdStr);
		}
#endif

		return (found);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ContainerWidget::VGetChildLuaData(const ScreenElementId childId, LuaPlus::LuaObject &controlData)
	{
		bool found = false;

		for(WidgetList::iterator i = m_list.begin(), end = m_list.end(); ((!found) && (i != end)); ++i)
		{
			if((*i)->VGetId() == childId)
			{
				controlData = (*i)->VGetLuaData();
				found = true;
			}
		}

		return(found);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ContainerWidget::VSetChildLuaData(const ScreenElementId childId, const LuaPlus::LuaObject &widgetData)
	{
		bool found(false);			// Was the widget found?
		bool updated(false);		// Did we update the widgets data?

		for(WidgetList::iterator i = m_list.begin(), end = m_list.end(); ((!found) && (i != end)); ++i)
		{
			if((*i)->VGetId() == childId)
			{
				updated = (*i)->VSetLuaData(widgetData);
				found = true;
			}
			else
			{
				// Check if the child is itself a container and see if the child we are searching for is inside.
				ContainerWidget *childContainerWidget = dynamic_cast<ContainerWidget *>((*i).get());
				if(childContainerWidget)
				{
					found = childContainerWidget->VSetChildLuaData(childId, widgetData);
					if(found)
					{
						updated = true;
					}
				}
			}
		}

		return (found && updated);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool ContainerWidget::SetChildVisibility(const ScreenElementId childId, const bool visible)
	{
		bool found = false;

		for(WidgetList::iterator i = m_list.begin(), end = m_list.end(); ((!found) && (i != end)); ++i)
		{
			if((*i)->VGetId() == childId)
			{
				(*i)->VSetVisible(visible);
				found = true;
			}
		}

		return(found);
	}

}
