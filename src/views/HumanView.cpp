//========================================================================
// GameCode.h : Defines the entry point for the application.
//
// Part of the GameCode3 Application
//
// GameCode3 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 3rd Edition" by Mike McShaffry, published by
// Charles River Media. ISBN-10: 1-58450-680-6   ISBN-13: 978-1-58450-680-5
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the author a favor and buy a new copy of the book - there are
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here:
//    http://www.amazon.com/gp/product/1584506806?ie=UTF8&tag=gamecodecompl-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=1584506806
//
// There's a companion web site at http://www.mcshaffry.com/GameCode/
//
// The source code is managed and maintained through Google Code: http://gamecode3.googlecode.com/svn/trunk/
//
// (c) Copyright 2009 Michael L. McShaffry
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License v2
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================

// /////////////////////////////////////////////////////////////////
// @file HumanView.h
// @author Michael L. McShaffry (edited by PJ O Halloran)
// @date 17/07/2010
//
// File contains the header for the game view layer class that humans
// see and interact with.
//
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry, author of Game
// Coding Complete and games programmer.
//
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// I have added/modified the following functionality:
// - I added the class under the GameHalloran namespace.
// - I put the class in its own header and implementation file.
// - Made class members private.
//
// /////////////////////////////////////////////////////////////////

#include <string.h>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "HumanView.h"
#include "GameBase.h"
#include "GameColors.h"
#include "Timer.h"
#include "GameOptions.h"

#include "GameMain.h"

#include "CAudio.h"
#include "OpenALAudio.h"

using boost::shared_ptr;
using boost::filesystem::path;
using boost::lexical_cast;
using boost::bad_lexical_cast;
using boost::optional;

using std::string;

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    optional<ActorId> HumanView::GetActorId() const
    {
        return (m_actorId);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    shared_ptr<CProcessManager> HumanView::GetProcessManager()
    {
        return (m_processManagerPtr);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool HumanView::IsRunFullSpeed() const
    {
        return (m_runFullSpeed);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    F64 HumanView::GetLastDrawTime() const
    {
        return (m_lastDraw);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    HumanView::HumanView(shared_ptr<GameOptions> optionsPtr,
                         shared_ptr<GameLog> loggerPtr,
                         shared_ptr<WindowManager> screenManPtr) throw(GameException &)\
:
    m_viewId(0)
    , m_actorId()
    , m_processManagerPtr()
    , m_timer()
    , m_lastDraw(0.0)
    , m_runFullSpeed(true)
    , m_ScreenElements()
    , m_currElemId(0)
    , m_mouseHandler(NULL)
    , m_keyboardHandler(NULL)
    , m_optionsPtr(optionsPtr)
    , m_loggerPtr(loggerPtr)
    , m_screenManPtr(screenManPtr)
    {
        if(!m_optionsPtr || !m_loggerPtr || !m_screenManPtr) {
            throw GameException(string("One of the pointers supplied to the view was NULL."));
        }

        m_processManagerPtr.reset(GCC_NEW CProcessManager);
        if(!m_processManagerPtr) {
            GF_LOG_FAT("Failed to create the ProcessManager");
            throw GameException(string("Failed to allocate memory off the heap."));
        }

        if(!InitAudio()) {
            throw GameException(string("Failed to initialze the audio subsystem."));
        }

        m_timer = shared_ptr<Timer>(GCC_NEW Timer());
        m_timer->Start();
    }


    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    HumanView::~HumanView()
    {
        try {
            while(!m_ScreenElements.empty()) {
                m_ScreenElements.pop_front();
            }

            if(g_audioPtr) {
                // Free audio system resources.
                delete g_audioPtr;
                g_audioPtr = NULL;

                // Set the OpenAL global derived pointer to NULL now as the global sound pointer which it also
                // refers to has been clean up.
                if(g_oalAudioPtr) {
                    g_oalAudioPtr = NULL;
                }
            }
        } catch(...) {
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void HumanView::VOnRender(const F64 time, const F32 elapsedTime)
    {
        // Exit early, if no time has elapsed since the last render.
        if(m_timer->GetTime() == m_lastDraw) {
            return;
        }

        // Ensure the screen elements are sorted in increasing Z order and render them.
        m_ScreenElements.sort(SortBy_SharedPtr_Content<IScreenElement>());
        for(ScreenElementList::iterator i = m_ScreenElements.begin(); i != m_ScreenElements.end(); ++i) {
            if((*i)->VIsVisible() && !(*i)->VOnRender(time, elapsedTime)) {
                try {
                    string idStr = lexical_cast<string, ScreenElementId>((*i)->VGetId());
                    GF_LOG_ERR("Failed to create the ProcessManager");
                } catch(bad_lexical_cast &) {
                    GF_LOG_ERR("Some screen element failed to Render");
                }
            }
        }

        // record the last successful paint
        m_lastDraw = m_timer->GetTime();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool HumanView::VOnRestore()
    {
        bool result = true;             // Result of method.

        // Call restore on all the screen elements
        if(result) {
            for(ScreenElementList::iterator i = m_ScreenElements.begin(); i != m_ScreenElements.end(); ++i) {
                if(!(*i)->VOnRestore()) {
                    result = false;
                    try {
                        string idStr = lexical_cast<string, ScreenElementId>((*i)->VGetId());
                        GF_LOG_ERR(string("The screen element (id=") + idStr + string(") failed to Restore."));
                    } catch(bad_lexical_cast &) {
                        GF_LOG_ERR("Some screen element failed to Restore");
                    }
                }
            }
        }

        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void HumanView::VOnLostDevice()
    {
        // Call lost device on all screen elements.
        for(ScreenElementList::iterator i = m_ScreenElements.begin(); i != m_ScreenElements.end(); ++i) {
            (*i)->VOnLostDevice();
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    GameViewType HumanView::VGetType() const
    {
        return (GameView_Human);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    GameViewId HumanView::VGetId() const
    {
        return (m_viewId);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool HumanView::InitAudio()
    {
        // Bug Fix: When the app creates more than one view object the global audio pointer used to be created repeatedly.
        // This results in the old global audio pointer not having any reference and therefore becoming a memory leak.
        // Instead we block multiple instances of a HumanView object from creating more than one global audio pointer here!
        if(g_audioPtr && g_audioPtr->VActive()) {
            GF_LOG_TRACE_INF("HumanView::InitAudio()", "The global audio system has already been initialized");
            return (true);
        }

        string audioSystem;                             // Name of audio system to use retrieved from options file.
        const string optionName("AudioSystem");         // ID of audio system in options file.
        const string OAL_SYS("OpenAL");                 // OpenAL system name.
        const string DS_SYS("DirectSound");             // DirectSound system name.
        bool useOpenAL = false;                         // Are we setting up an OpenAL system?

        boost::shared_ptr<GameOptions> opPtr = g_appPtr->GetGameOptions();
        if(!RetrieveAndConvertOption<string>(opPtr, optionName, GameOptions::PROGRAMMER, audioSystem)) {
            GF_LOG_TRACE_INF("HumanView::InitAudio()", string("Failed to get the ") + optionName + string(" so we will use the OpenAL system by default!"));
            audioSystem.assign(OAL_SYS);
        }

        // OpenAL
        if(audioSystem.compare(OAL_SYS.c_str()) == 0) {
            GF_LOG_TRACE_INF("HumanView::InitAudio()", "Using OpenAL as the audio system");
            g_audioPtr = GCC_NEW OpenALAudio;
            useOpenAL = true;
        }
        //// DirectSound
        //else if(audioSystem.compare(DS_SYS.c_str()) == 0)
        //{
        //  GF_LOG_TRACE_INF("HumanView::InitAudio()", "Using DirectSound as the audio system");
        //  // TODO: HWND is a windows specific handle, I cant get this at present from my GLFW window manager...
        //  g_audioPtr = GCC_NEW DirectSound8Audio(g_appPtr->GetWindowManager()->GetHwnd());
        //}
        // Default to OpenAL if audio system is not in file or its invalid...
        else {
            GF_LOG_TRACE_INF("HumanView::InitAudio()", "Using OpenAL as the audio system");
            g_audioPtr = GCC_NEW OpenALAudio;
            useOpenAL = true;
        }

        if(!g_audioPtr) {
            GF_LOG_TRACE_ERR("HumanView::InitAudio()", "Failed to create the audio system");
            return (false);
        }

        if(useOpenAL) {
            g_oalAudioPtr = dynamic_cast<OpenALAudio *>(g_audioPtr);
            if(!g_oalAudioPtr) {
                GF_LOG_TRACE_ERR("HumanView::InitAudio()", "Failed to set the Global OpenAL audio system pointer");
                return (false);
            }
        }

        if(!g_audioPtr->VInitialize()) {
            GF_LOG_TRACE_ERR("HumanView::InitAudio()", "Failed to initialize the audio system");
            return (false);
        }

        return (true);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void HumanView::VOnAttach(const GameViewId vid, boost::optional<ActorId> aid)
    {
        m_viewId = vid;
        m_actorId = aid;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool HumanView::VOnEvent(GF_Event &eventObj, const F32 elapsedTime)
    {
        bool result = true;             // Result of method.

        for(ScreenElementList::reverse_iterator i = m_ScreenElements.rbegin(); i != m_ScreenElements.rend(); ++i) {
            if((*i)->VIsVisible() && !(*i)->VOnEvent(eventObj, elapsedTime)) {
                result = false;
                try {
                    string idStr = lexical_cast<string, ScreenElementId>((*i)->VGetId());
                    GF_LOG_ERR(string("The screen element (id=") + idStr + string(") failed to process the event queue."));
                } catch(bad_lexical_cast &) {
                    GF_LOG_ERR("Some screen element failed to process the event queue");
                }
            }
        }

        // Look for the events that the View is interested in.
        switch(eventObj.id) {
            case GF_KEY_EVENT: {
                if(m_keyboardHandler) {
                    if(eventObj.key.keyState == GLFW_PRESS) {
                        m_keyboardHandler->VOnKeyDown(eventObj.key.keyId);
                    } else if(eventObj.key.keyState == GLFW_RELEASE) {
                        m_keyboardHandler->VOnKeyUp(eventObj.key.keyId);
                    }
                }
            }
            break;

            case GF_MOUSE_MOVE_EVENT: {
                if(m_mouseHandler) {
                    m_mouseHandler->VOnMouseMove(Point(eventObj.mouseMove.x, eventObj.mouseMove.y), \
                                                 Point(eventObj.mouseMove.xrel, eventObj.mouseMove.yrel));
                }
            }
            break;

            case GF_MOUSE_BUTTON_EVENT: {
                if(m_mouseHandler) {
                    if(eventObj.mouseButton.state == GLFW_PRESS) {
                        m_mouseHandler->VOnMouseButtonDown(eventObj.mouseButton.buttonId, Point(eventObj.mouseButton.x, eventObj.mouseButton.y));
                    } else if(eventObj.mouseButton.state == GLFW_RELEASE) {
                        m_mouseHandler->VOnMouseButtonUp(eventObj.mouseButton.buttonId, Point(eventObj.mouseButton.x, eventObj.mouseButton.y));
                    }
                }
            }
            break;

            case GF_MOUSE_WHEEL_EVENT: {
                if(m_mouseHandler) {
#ifdef USE_NEW_GLFW
                    m_mouseHandler->VOnMouseWheelMove(eventObj.mouseWheel.x, eventObj.mouseWheel.y);
#else
                    m_mouseHandler->VOnMouseWheelMove(eventObj.mouseWheel.pos);
#endif
                }
            }
            break;

            default: {
                // ignore all other events in the views for now
            }
            break;
        }

        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void HumanView::VOnUpdate(const F32 elapsedTime)
    {
        m_processManagerPtr->UpdateProcesses(elapsedTime);
        for(ScreenElementList::iterator i = m_ScreenElements.begin(); i != m_ScreenElements.end(); ++i) {
            (*i)->VOnUpdate(elapsedTime);
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void HumanView::VPushElement(shared_ptr<IScreenElement> pElement)
    {
        if(pElement) {
            pElement->VSetId(GetNextElementId());
            m_ScreenElements.push_front(pElement);
        }
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void HumanView::VPopElement(shared_ptr<IScreenElement> pElement)
    {
        if(pElement) {
            m_ScreenElements.remove(pElement);
        }
    }

}
