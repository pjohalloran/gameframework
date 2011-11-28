// /////////////////////////////////////////////////////////////////
// @file Pool3dGame.cpp
// @author PJ O Halloran
// @date 01/11/2010
//
// File contains the implementation for the Pool3dGame class.
//
// /////////////////////////////////////////////////////////////////

// External Headers

#include <string>

// Project Headers
#include "GameBase.h"
#include "Pool3dGame.h"
#include "Pool3dLogic.h"
#include "Pool3dView.h"
#include "Pool3dMenuView.h"
#include "Pool3dActors.h"

using boost::shared_ptr;
using boost::filesystem::path;

using std::vector;
using std::string;

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// ******************** Pool3dGameEventListener ********************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dGameEventListener::VHandleEvent(IEventData const &eventObj)
	{
		bool result = true;

		// NB. Note to self, the event will not be propagated anymore if you return TRUE from this method.
		//		Take care you return FALSE for events that might have script listeners...

		// Log receipt of the event and its type.
        GF_LOG_TRACE_TRC(VGetName(), string("Recevied event type = ") + string(eventObj.VGetEventType().getStr()));
//		SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::TRC, string(VGetName()), string("Recevied event type = ") + string(eventObj.VGetEventType().getStr()));

		// Check what event has occurred and handle it appropriately.
		if(eventObj.VGetEventType() == EvtData_Graphics_Config_Change::sk_EventType)
		{
			const EvtData_Graphics_Config_Change &castEvent = static_cast<const EvtData_Graphics_Config_Change &>(eventObj);
			result = OnGraphicsConfigChange(castEvent);
		}
		else if(eventObj.VGetEventType() == EvtData_Sound_Config_Change::sk_EventType)
		{
			const EvtData_Sound_Config_Change &castEvent = static_cast<const EvtData_Sound_Config_Change &>(eventObj);
			result = OnSoundConfigChange(castEvent);
		}
		// TODO: Handle other UI view events here...
		else
		{
            GF_LOG_DEB(string("View: Unknown game event received: ") + eventObj.VGetEventType().getStr());
//			SafeGameLog(g_appPtr->GetLoggerPtr(), GameLog::DEB, string("View: Unknown game event received: ") + eventObj.VGetEventType().getStr());
			result = false;
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dGameEventListener::OnGraphicsConfigChange(const EvtData_Graphics_Config_Change &eventData)
	{
		if(eventData.GetWidth() != m_poolAppPtr->GetWindowManager()->GetWidth() && eventData.GetHeight() != m_poolAppPtr->GetWindowManager()->GetHeight())
		{
			m_poolAppPtr->GetWindowManager()->SetVideoMode(eventData.GetWidth(), eventData.GetHeight());
		}
		m_poolAppPtr->GetTextureManagerPtr()->SetTextureFilterMode(TextureManager::TextureFilterMode(eventData.GetTexFilteringMode() - 1));
#ifdef GLEW_EXT_texture_filter_anisotropic
		// If the user selected anisotropic filtering, set the ani level to the max allowed.
		if(m_poolAppPtr->GetTextureManagerPtr()->GetTextureFilterMode() == TextureManager::eAnisotropic)
		{
			m_poolAppPtr->GetTextureManagerPtr()->SetAnisotropicLinearLevel(1.0f);
		}
#endif

		// TODO: Update the application to either render shadows nor not (once i have figured out how to render shadows).
		//eventData.UseShadows();

		// TODO: Multisampling update - this requires an WindowManager restart to take effect which itself requires a game restart.


		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dGameEventListener::OnSoundConfigChange(const EvtData_Sound_Config_Change &eventData)
	{
		// Handled in Pool3dView...
		//eventData.GetVolume();
		//eventData.PlayMusic();
		//eventData.PlaySoundFx();
		return (false);
	}

	// /////////////////////////////////////////////////////////////////
	// ************************ Pool3dGame *****************************
	// /////////////////////////////////////////////////////////////////

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool Pool3dGame::VInitOpenGL()
	{
		bool result = true;

        GF_LOG_DEB("Initializing the GLEW library");
//		SafeGameLog(m_loggerPtr, GameLog::DEB, string("Initializing the GLEW library."));
#if TARGET_OS_MAC
        glewExperimental = GL_TRUE;
#endif
		GLenum res = glewInit();
		if (GLEW_OK != res)
		{
			// Problem: glewInit failed, something is seriously wrong.
            GF_LOG_ERR(string("Failed to initialize the GLEW library: ") + string(reinterpret_cast<const char *>(glewGetErrorString(res))));
//			SafeGameLog(m_loggerPtr, GameLog::ERR, string("Failed to initialize the GLEW library: ") + string(reinterpret_cast<const char *>(glewGetErrorString(res))));
			result = false;
		}
		if(result)
		{
            GF_LOG_DEB(string("Using GLEW version: ") + string(reinterpret_cast<const char *>(glewGetString(GLEW_VERSION))));
//			SafeGameLog(m_loggerPtr, GameLog::DEB, string("Using GLEW version: ") + string(reinterpret_cast<const char *>(glewGetString(GLEW_VERSION))));
		}

		// Check for required OpenGL extensions here and set application wide flags if certain features are available.
		// TODO: check for other extensions.
#ifdef GLEW_EXT_texture_filter_anisotropic
		m_isAnisotropicExtPresent = true;
#endif

		// Perform any global OpenGL initialization here.
		if(result)
		{
			// Set the color buffer clear value.
			glClearColor(GameHalloran::g_gcLightGray.GetX(), GameHalloran::g_gcLightGray.GetY(), GameHalloran::g_gcLightGray.GetZ(), GameHalloran::g_gcLightGray.GetW());

			// Enable depth testing.
			glEnable(GL_DEPTH_TEST);

			// Set the alpha blending function.
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Turn on anitaliasing for lines
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

			// Cull all back facing triangles.
			glCullFace(GL_BACK);

			// Set the triangle winding.
			glFrontFace(GL_CCW);

			// Initialize the TextureManager last once all other OpenGL initialization is done.
			m_texManagerPtr.reset(GCC_NEW TextureManager(10, 1024*1024*20));
			m_texManagerPtr->SetTextureFilterMode(TextureManager::eAnisotropic);
			m_texManagerPtr->SetAnisotropicLinearLevel(1.0f);
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	shared_ptr<BaseGameLogic> Pool3dGame::VCreateLogicAndViews()
	{
		RegisterGameSpecificEvents();

		// Create the logic layer for the test app.
		shared_ptr<BaseGameLogic> logicPtr(GCC_NEW Pool3dLogic(m_optionsPtr, m_loggerPtr, m_mvProjStackManager));
		if(!logicPtr)
		{
            GF_LOG_FAT("Failed to allocate memory for the Pool3D logic layer");
//			SafeGameLog(m_loggerPtr, GameLog::FAT, string("Failed to allocate memory for the Pool3D logic layer"));
		}
		else
		{
			// TODO: Load the loading screen/view here first.
			
			// Add the main game view where gameplay is rendered.
			shared_ptr<Pool3dView> viewPtr(GCC_NEW Pool3dView(m_optionsPtr, m_loggerPtr, m_windowManagerPtr, m_mvProjStackManager, &m_viewFrustrum));
			if(!viewPtr)
			{
                GF_LOG_FAT("Failed to allocate memory for the Pool3D view (Removing the Pool3D logic layer as a result)");
//				SafeGameLog(m_loggerPtr, GameLog::FAT, string("Failed to allocate memory for the Pool3D view (Removing the Pool3D logic layer as a result)"));
				logicPtr.reset();
			}
			else
			{
				boost::optional<GameViewId> gameViewId = logicPtr->VAddView(viewPtr);
				if(!gameViewId.is_initialized())
				{
                    GF_LOG_FAT("Failed to add the GameView to the Logic layer");
//					SafeGameLog(m_loggerPtr, GameLog::FAT, string("Failed to add the GameView to the Logic layer"));
				}
				else
				{
					m_gameId = *gameViewId;
				}
			}

			// Add the menu/UI view where the menu screens are displayed overlayed on the game view.
			shared_ptr<Pool3dMenuView> menuViewPtr(GCC_NEW Pool3dMenuView(m_optionsPtr, m_loggerPtr, m_windowManagerPtr, m_mvProjStackManager));
			if(!menuViewPtr)
			{
                GF_LOG_FAT("Failed to allocate memory for the Pool3D UI/Menu view (Removing the Pool3D logic layer as a result)");
//				SafeGameLog(m_loggerPtr, GameLog::FAT, string("Failed to allocate memory for the Pool3D UI/Menu view (Removing the Pool3D logic layer as a result)"));
				logicPtr.reset();
			}
			else
			{
				boost::optional<GameViewId> uiViewId = logicPtr->VAddView(menuViewPtr);
				if(!uiViewId.is_initialized())
				{
                    GF_LOG_FAT("Failed to add the UiView to the Logic layer");
//					SafeGameLog(m_loggerPtr, GameLog::FAT, string("Failed to add the UiView to the Logic layer"));
				}
				else
				{
					m_uiId = *uiViewId;
				}
			}
		}

		return (logicPtr);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dGame::RegisterGameSpecificEvents()
	{
		// Options screen events
		g_appPtr->GetEventManager()->RegisterEvent<EvtData_Graphics_Config_Change>(EvtData_Graphics_Config_Change::sk_EventType);
		g_appPtr->GetEventManager()->RegisterEvent<EvtData_Sound_Config_Change>(EvtData_Sound_Config_Change::sk_EventType);
		g_appPtr->GetEventManager()->RegisterEvent<EvtData_Save_Config_Change>(EvtData_Save_Config_Change::sk_EventType);

		// Loading and saving events.
		g_appPtr->GetEventManager()->RegisterEvent<EvtData_Loading_Progress>(EvtData_Loading_Progress::sk_EventType);
		g_appPtr->GetEventManager()->RegisterCodeOnlyEvent(EvtData_Complex_Mesh_Loaded::sk_EventType);
		g_appPtr->GetEventManager()->RegisterCodeOnlyEvent(EvtData_Save_Game::sk_EventType);
		g_appPtr->GetEventManager()->RegisterCodeOnlyEvent(EvtData_Load_Game::sk_EventType);

		// Gameplay events.
		g_appPtr->GetEventManager()->RegisterEvent<EvtData_Camera_Aim_Mode_Request>(EvtData_Camera_Aim_Mode_Request::sk_EventType);
		g_appPtr->GetEventManager()->RegisterEvent<EvtData_ResetCueBall>(EvtData_ResetCueBall::sk_EventType);
		g_appPtr->GetEventManager()->RegisterCodeOnlyEvent(EvtData_Camera_Aim_Mode_Update::sk_EventType);
		g_appPtr->GetEventManager()->RegisterCodeOnlyEvent(EvtData_Shot_Start::sk_EventType);
		g_appPtr->GetEventManager()->RegisterCodeOnlyEvent(EvtData_Shot_End::sk_EventType);

		// Others
		g_appPtr->GetEventManager()->RegisterEvent<EvtData_Game_Mode>(EvtData_Game_Mode::sk_EventType);

		// Register events now...
		safeAddListener(m_listener, EvtData_Graphics_Config_Change::sk_EventType);
		safeAddListener(m_listener, EvtData_Sound_Config_Change::sk_EventType);
        m_eventsRegistered = true;
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Pool3dGame::Pool3dGame(boost::shared_ptr<GameLog> loggerPtr, boost::shared_ptr<GameOptions> optionsPtr) throw (GameException &)\
		: GameMain(loggerPtr, optionsPtr), m_mvProjStackManager(), m_viewFrustrum(), m_isAnisotropicExtPresent(false), m_gameId(VIEW_ID_NO_VIEW_ATTACHED),\
			m_uiId(VIEW_ID_NO_VIEW_ATTACHED), m_listener(), m_eventsRegistered(false)
	{
		// Build ModelView matrix stack with the default stack size (64).
		// NB:  This sets a limit for the game views scene graph to have a maximum depth of 64.
		shared_ptr<GLMatrixStack> mvStack(GCC_NEW GLMatrixStack());
		
		// Build Projection matrix stack with a small default stack size (Proj stack does not need to be
		//  very large, 2 matrices at most, we will say 4 for flexibility sake).
		shared_ptr<GLMatrixStack> projStack(GCC_NEW GLMatrixStack(4));

		m_mvProjStackManager.reset(GCC_NEW ModelViewProjStackManager(mvStack, projStack));

		m_listener.reset(GCC_NEW Pool3dGameEventListener(this));
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Pool3dGame::~Pool3dGame()
	{
		try
		{
            if(m_eventsRegistered && g_appPtr && g_appPtr->GetEventManager())
            {
                safeDelListener(m_listener, EvtData_Graphics_Config_Change::sk_EventType);
                safeDelListener(m_listener, EvtData_Sound_Config_Change::sk_EventType);
            }
		}
		catch(...) {}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void Pool3dGame::VOnResize(const I32 width, const I32 height)
	{
		// TODO: Make the perspective frustrum parameters configurable...

		GameMain::VOnResize(width, height);

		GLsizei w = static_cast<GLsizei>(width);
		GLsizei h = static_cast<GLsizei>(height);

		// Set the OpenGL viewport matrix for the window clipping stage of the rendering pipeline.
		//  This is still required in OpenGL 3.x and greater as the programmable pipeline does not
		//   alter this matrix.
		glViewport(0, 0, w, h);

		// Calculate the aspect ratio of the screen.
		GLfloat aspectRatio = GLfloat(w);
		if(h != 0)
		{
			aspectRatio /= GLfloat(h);
		}
		const GLfloat fov = 35.0f;
		const GLfloat nearPlane = 0.1f;
		const GLfloat farPlane = 500.0f;

		// Set the applications viewing frustrum to clip unseen geometry.
		m_viewFrustrum.Init(fov, aspectRatio, nearPlane, farPlane);

		// Pop all stored matrices from the Projection stack as they are useless now that the window size has changed.
		bool popResult = true;
		do
		{
			popResult = m_mvProjStackManager->GetProjectionMatrixStack()->PopMatrix();
		} while(popResult);

		Matrix4 mat;

		// Build a new orthographic matrix for the UI view and push it onto the stack for retrieval elsewhere in the app.
		//  Note: This projection matrix is only in use while a UI menu is being displayed.
		GameHalloran::BuildOrthographic(mat, 0.0f, F32(w), 0.0f, F32(h), -1.0f, 1.0f);
		m_mvProjStackManager->GetProjectionMatrixStack()->LoadMatrix(mat);
		m_mvProjStackManager->GetProjectionMatrixStack()->PushMatrix();

		// Build a new perspecitive matrix based on the new window width/height and set the top of the Projection stack.
		GameHalloran::BuildPerspectiveFov(mat, fov, aspectRatio, nearPlane, farPlane);
		m_mvProjStackManager->GetProjectionMatrixStack()->LoadMatrix(mat);

		//// TEST CODE FOLLOWS TO GET FTGL working (AS IT USES OLD STYLE GL matrices)
		//glMatrixMode(GL_PROJECTION);
		////glLoadMatrixf(mat.GetComponentsConst());
		//gluPerspective(fov, aspectRatio, nearPlane, farPlane);
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
	}

}
