#include <string>
#include <iostream>
#include <cstring>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include "TestView.h"
#include "GameBase.h"
#include "GameMain.h"
#include "CameraSceneNode.h"
#include "TextResource.h"
#include "ZipFile.h"

using std::string;

using boost::optional;
using boost::shared_ptr;
using boost::filesystem::path;

namespace GameHalloran
{
	TestViewEventListener::TestViewEventListener(TestView *viewPtr) throw (GameException &)
		: m_viewPtr(viewPtr) {
		if(!m_viewPtr) {
			throw GameException(string("Invalid game UI view pointer supplied."));
		}
	}

	char const *TestViewEventListener::VGetName(void) {
		return ("TestViewEventListener");
	}

	bool TestViewEventListener::VHandleEvent(IEventData const &eventObj) {
		return (false);
	}

	TestView::TestView(shared_ptr<GameOptions> optionsPtr
						, shared_ptr<GameLog> loggerPtr
						, shared_ptr<WindowManager> screenManPtr
						, shared_ptr<ModelViewProjStackManager> matStackManager) throw (GameException &)
						: HumanView(optionsPtr, loggerPtr, screenManPtr)
						, m_stackManager(matStackManager)
						, m_modelViewStackPtr()
						, m_projStackPtr()
						, m_listenerPtr()
						, m_flatShader()
						, m_texShader() {
		m_modelViewStackPtr = m_stackManager->GetModelViewMatrixStack();
		m_projStackPtr = m_stackManager->GetProjectionMatrixStack();

		m_listenerPtr.reset(GCC_NEW TestViewEventListener(this));

		VSAttributeNameList flatVs, texVs;
		flatVs.push_back(std::string("vertexPos"));
		texVs.push_back(string("vertexPos"));
		texVs.push_back(g_ignoreShaderSlot);
		texVs.push_back(g_ignoreShaderSlot);
		texVs.push_back(string("texCoords"));
		m_flatShader = GameHalloran::BuildShaderFromResourceCache(std::string("shaders") + ZipFile::ZIP_PATH_SEPERATOR + std::string("flat"), flatVs);
		m_texShader = GameHalloran::BuildShaderFromResourceCache(std::string("shaders") + ZipFile::ZIP_PATH_SEPERATOR + std::string("GuiTextureColor"), texVs);
		if(!m_flatShader || !m_texShader) {
			throw GameException(std::string("Failed to build UI shaders for View UI manager"));
		}
	}

	TestView::~TestView() {
	}

	bool TestView::VOnRestore() {
		return (HumanView::VOnRestore());
	}

	void TestView::VOnRender(const F64 time, const F32 elapsedTime) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
		GF_CLEAR_GL_ERROR();
		
		{
			GLMatrixStackRaii mvSaveStack(m_modelViewStackPtr);

		}

		HumanView::VOnRender(time, elapsedTime);
	}
	
	void TestView::VOnLostDevice() {
		HumanView::VOnLostDevice();
	}

	bool TestView::VOnEvent(GF_Event &eventObj, const F32 elapsedTime) {
		HumanView::VOnEvent(eventObj, elapsedTime);
		
		switch(eventObj.id) {
			case GF_JOYSTICK_MOVE_EVENT:
			case GF_MOUSE_BUTTON_EVENT:
			case GF_MOUSE_WHEEL_EVENT:
			case GF_MOUSE_MOVE_EVENT:
			case GF_KEY_EVENT:
			case GF_KEY_CHARACTER_EVENT:
			case GF_JOYSTICK_BUTTON_EVENT:
			default:
				break;
		}

		return (true);
	}
	
	void TestView::VOnUpdate(const F32 elapsedTime) {
		HumanView::VOnUpdate(elapsedTime);
	}
}
