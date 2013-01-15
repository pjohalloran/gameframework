#ifndef __TEST_APP_H
#define __TEST_APP_H

#include <stdexcept>

#include <boost/shared_ptr.hpp>

#include "GameBase.h"
#include "GameException.h"
#include "GameLog.h"
#include "GameMain.h"
#include "Frustrum.h"
#include "TestView.h"

namespace GameHalloran {

	class TestApp;

	class TestAppEventListener : public IEventListener {
		private:
			TestApp *m_appPtr;

		protected:

		public:
			
			explicit TestAppEventListener(TestApp *appPtr) : m_appPtr(appPtr) {
				if(m_appPtr == NULL) throw GameException(std::string(" is NULL"));
			};
			virtual	~TestAppEventListener() { };

			virtual char const *VGetName(void) { return ("TestAppEventListener"); };
			virtual bool VHandleEvent(IEventData const &eventObj);
	};

	class TestApp : public GameMain {
		private:
			boost::shared_ptr<ModelViewProjStackManager> m_mvProjStackManager;
			Frustrum m_viewFrustrum;
			bool m_isAnisotropicExtPresent;
			GameViewId m_gameId;
			boost::shared_ptr<TestAppEventListener> m_listener;
            bool m_eventsRegistered;

			virtual bool VInitOpenGL();
			virtual boost::shared_ptr<BaseGameLogic> VCreateLogicAndViews();

			void RegisterGameSpecificEvents();

		public:
			
			TestApp(boost::shared_ptr<GameLog> loggerPtr, boost::shared_ptr<GameOptions> optionsPtr) throw (GameException &);
			virtual	~TestApp();

			virtual void VOnResize(const I32 width, const I32 height);
			bool IsAnisotropicTexturingAvailable() const { return (m_isAnisotropicExtPresent); };
			virtual bool VGetLoadFilenames(const boost::filesystem::path &directoryRef, std::vector<boost::filesystem::path> &fileListRef) { return (true); };

	};

}

#endif
