#include <string>

#include "GameBase.h"
#include "TestApp.h"

#include "TestLogic.h"
#include "TestView.h"
#include "GameColors.h"

using boost::shared_ptr;
using boost::filesystem::path;

using std::vector;
using std::string;

namespace GameHalloran {

	bool TestAppEventListener::VHandleEvent(IEventData const &eventObj) {
		return (false);
	}

	bool TestApp::VInitOpenGL() {
		bool result = true;

        GF_LOG_DEB("Initializing the GLEW library");

#if defined(TARGET_OS_MAC)
        glewExperimental = GL_TRUE;
#endif
		GLenum res = glewInit();
		if (GLEW_OK != res) {
            GF_LOG_ERR(string("Failed to initialize the GLEW library: ") + string(reinterpret_cast<const char *>(glewGetErrorString(res))));
			result = false;
		}
		if(result) {
            GF_LOG_DEB(string("Using GLEW version: ") + string(reinterpret_cast<const char *>(glewGetString(GLEW_VERSION))));
		}

#ifdef GLEW_EXT_texture_filter_anisotropic
		m_isAnisotropicExtPresent = true;
#endif

		if(result) {
			glClearColor(GameHalloran::g_gcBlue.GetX()
							, GameHalloran::g_gcBlue.GetY()
							, GameHalloran::g_gcBlue.GetZ()
							, GameHalloran::g_gcBlue.GetW());
			glEnable(GL_DEPTH_TEST);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);

			m_texManagerPtr.reset(GCC_NEW TextureManager(10, 1024*1024*20));
			m_texManagerPtr->SetTextureFilterMode(TextureManager::eAnisotropic);
			m_texManagerPtr->SetAnisotropicLinearLevel(1.0f);
            
            GameMain::SetupTextureAtlasManager(std::string("atlases") + ZipFile::ZIP_PATH_SEPERATOR + std::string("atlasDictionary.xml"));
		}

		return (result);
	}

	shared_ptr<BaseGameLogic> TestApp::VCreateLogicAndViews() {
		RegisterGameSpecificEvents();

		shared_ptr<BaseGameLogic> logicPtr(GCC_NEW TestLogic(m_optionsPtr
																, m_loggerPtr
																, m_mvProjStackManager));
		shared_ptr<TestView> viewPtr(GCC_NEW TestView(m_optionsPtr
																, m_loggerPtr
																, m_windowManagerPtr
																, m_mvProjStackManager));

		boost::optional<GameViewId> gameViewId = logicPtr->VAddView(viewPtr);
		m_gameId = *gameViewId;

		return (logicPtr);
	}

	void TestApp::RegisterGameSpecificEvents() {
        m_eventsRegistered = true;
	}
	
	TestApp::TestApp(boost::shared_ptr<GameLog> loggerPtr, boost::shared_ptr<GameOptions> optionsPtr) throw (GameException &)
		: GameMain(loggerPtr, optionsPtr)
		, m_mvProjStackManager()
		, m_viewFrustrum()
		, m_isAnisotropicExtPresent(false)
		, m_gameId(VIEW_ID_NO_VIEW_ATTACHED)
		, m_listener()
		, m_eventsRegistered(false) {
		shared_ptr<GLMatrixStack> mvStack(GCC_NEW GLMatrixStack());
		shared_ptr<GLMatrixStack> projStack(GCC_NEW GLMatrixStack(4));
		m_mvProjStackManager.reset(GCC_NEW ModelViewProjStackManager(mvStack, projStack));
		m_listener.reset(GCC_NEW TestAppEventListener(this));
	}

	TestApp::~TestApp() {
		try {
            if(m_eventsRegistered && g_appPtr && g_appPtr->GetEventManager()) {
            }
		}
		catch(...) {}
	}

	void TestApp::VOnResize(const I32 width, const I32 height) {
		GameMain::VOnResize(width, height);
		GLsizei w = static_cast<GLsizei>(width);
		GLsizei h = static_cast<GLsizei>(height);

		glViewport(0, 0, w, h);
		GLfloat aspectRatio = GLfloat(w);
		aspectRatio /= GLfloat(h);
		const GLfloat fov = 35.0f;
		const GLfloat nearPlane = 0.1f;
		const GLfloat farPlane = 500.0f;

		m_viewFrustrum.Init(fov, aspectRatio, nearPlane, farPlane);

		bool popResult = true;
		do
		{
			popResult = m_mvProjStackManager->GetProjectionMatrixStack()->PopMatrix();
		} while(popResult);

		Matrix4 mat;
		GameHalloran::BuildOrthographic(mat, 0.0f, F32(w), 0.0f, F32(h), -1.0f, 1.0f);
		m_mvProjStackManager->GetProjectionMatrixStack()->LoadMatrix(mat);
		m_mvProjStackManager->GetProjectionMatrixStack()->PushMatrix();

		GameHalloran::BuildPerspectiveFov(mat, fov, aspectRatio, nearPlane, farPlane);
		m_mvProjStackManager->GetProjectionMatrixStack()->LoadMatrix(mat);
	}

}
