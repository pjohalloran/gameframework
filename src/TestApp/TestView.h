// /////////////////////////////////////////////////////////////////
// @file TestView.h
// @author PJ O Halloran.
// @date 04/11/2010
//
// File contains the header for the Pool3d UI Menu screens view 
// layer class.
//
// /////////////////////////////////////////////////////////////////

#ifndef __TEST_VIEW_H
#define __TEST_VIEW_H

#include <string>
#include <map>
#include <list>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include "GameBase.h"
#include "HumanView.h"
#include "WindowManager.h"
#include "GameLog.h"
#include "GameOptions.h"
#include "GameException.h"
#include "OsInputEvents.h"
#include "GLMatrixStack.h"
#include "ModelViewProjStackManager.h"
#include "MyOpenGLUI.h"
#include "HashedString.h"
#include "Events.h"
#include "GLSLShader.h"

namespace GameHalloran {
	class TestView;

	class TestViewEventListener : public IEventListener
	{
		private:
			TestView *m_viewPtr;

		public:
			explicit TestViewEventListener(TestView *viewPtr) throw (GameException &);
			virtual	~TestViewEventListener() { };

			virtual char const *VGetName(void);
			virtual bool VHandleEvent(IEventData const &eventObj);
	};

	class TestView : public HumanView {
		friend class TestViewEventListener;
	private:
		boost::shared_ptr<ModelViewProjStackManager> m_stackManager;
		boost::shared_ptr<GLMatrixStack> m_modelViewStackPtr;
		boost::shared_ptr<GLMatrixStack> m_projStackPtr;
		boost::shared_ptr<IEventListener> m_listenerPtr;
		boost::shared_ptr<GLSLShader> m_flatShader;
		boost::shared_ptr<GLSLShader> m_texShader;

	public:
		TestView(boost::shared_ptr<GameOptions> optionsPtr
					, boost::shared_ptr<GameLog> loggerPtr
					, boost::shared_ptr<WindowManager> screenManPtr
					, boost::shared_ptr<ModelViewProjStackManager> matStackManager) throw (GameException &);
		virtual ~TestView();

		virtual bool VOnRestore();
		virtual void VOnRender(const F64 time, const F32 elapsedTime);
		virtual void VOnLostDevice();
		virtual bool VOnEvent(GF_Event &eventObj, const F32 elapsedTime);
		virtual void VOnUpdate(const F32 elapsedTime);
	};
}

#endif
