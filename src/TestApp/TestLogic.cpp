#include "TestLogic.h"
#include "TextResource.h"

using std::string;

using boost::shared_ptr;
using boost::optional;

namespace GameHalloran {
	ActorParams *TestLogicEventListener::CreateNewActorParams(const LuaPlus::LuaObject &srcData) {
		return (NULL);
	}

	TestLogicEventListener::TestLogicEventListener(TestLogic *logicPtr) throw (GameException &)
		: m_logicPtr(logicPtr) {
	}

	char const *TestLogicEventListener::VGetName(void) {
		return ("TestLogicEventListener");
	}

	bool TestLogicEventListener::VHandleEvent(IEventData const &eventObj) {
		return (false);
	}

	TestLogic::TestLogic(shared_ptr<GameOptions> optionsPtr
						, shared_ptr<GameLog> loggerPtr
						, shared_ptr<ModelViewProjStackManager> stackManagerPtr) throw (GameException &)
						: BaseLuaGameLogic(optionsPtr, loggerPtr, stackManagerPtr)
						, m_listener()
	{
		m_listener.reset(GCC_NEW TestLogicEventListener(this));
	}

	TestLogic::~TestLogic() {
	}

	void TestLogic::VBuildInitialScene() {
		BaseLuaGameLogic::VBuildInitialScene();
	}

	bool TestLogic::VLoadGame(const string &gameNameRef) {
		return (BaseLuaGameLogic::VLoadGame(gameNameRef));
	}

	bool TestLogic::VSaveGame() {
		return (BaseLuaGameLogic::VSaveGame());
	}

	void TestLogic::VOnUpdate(const F64 time, const F32 elapsedTime) {
		BaseLuaGameLogic::VOnUpdate(time, elapsedTime);
	}

	void TestLogic::VChangeState(const enum BaseGameState newState) {
		BaseLuaGameLogic::VChangeState(newState);
	}

}
