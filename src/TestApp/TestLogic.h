// /////////////////////////////////////////////////////////////////
// @file TestLogic.h
// @author PJ O Halloran.
// @date 01/11/2010
//
// Test logic class.
//
// /////////////////////////////////////////////////////////////////

#ifndef __TEST_LOGIC_H
#define __TEST_LOGIC_H

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>

#include "GameLogic.h"
#include "Actors.h"
#include "CProcess.h"
#include "GameOptions.h"
#include "GameLog.h"
#include "GameException.h"
#include "IGamePhysics.h"
#include "IGameView.h"
#include "CRandom.h"
#include "Matrix.h"
#include "EventManager.h"

namespace GameHalloran {
	class TestLogic;

	class TestLogicEventListener : public IEventListener {
		private:
			TestLogic *m_logicPtr;

			ActorParams *CreateNewActorParams(const LuaPlus::LuaObject &srcData);

		public:
			explicit TestLogicEventListener(TestLogic *logicPtr) throw (GameException &);
			virtual	~TestLogicEventListener() { };

			virtual char const *VGetName(void);
			virtual bool VHandleEvent(IEventData const &eventObj);
	};

	class TestLogic : public BaseLuaGameLogic {
		friend class TestLogicEventListener;
	private:
		boost::shared_ptr<IEventListener> m_listener;

	public:
		explicit TestLogic(boost::shared_ptr<GameOptions> optionsPtr
							, boost::shared_ptr<GameLog> loggerPtr
							, boost::shared_ptr<ModelViewProjStackManager> stackManagerPtr) throw (GameException &);
		virtual ~TestLogic();

		virtual void VBuildInitialScene();
		virtual bool VLoadGame(const std::string &gameNameRef);
		virtual bool VSaveGame();
		virtual void VOnUpdate(const F64 time, const F32 elapsedTime);
		virtual void VChangeState(const enum BaseGameState newState);
	};
}

#endif
