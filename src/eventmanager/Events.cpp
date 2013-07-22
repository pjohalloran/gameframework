//========================================================================
// Events.cpp : defines common game events
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
// The source code is managed and maintained through Google Code:
// http://gamecode3.googlecode.com/svn/trunk/
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
// @file Events.cpp
// @author Mike McShaffry
// @date 13/07/2010
//
// File contains the header for common game events.
//
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry, author of Game
// Coding Complete and games programmer.
//
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// Please see the associated header for details on any changes I have
// made.
//
// /////////////////////////////////////////////////////////////////

#include "Events.h"

namespace GameHalloran {
    const EventType EvtData_New_Game::sk_EventType("new_game");
    const EventType EvtData_End_Game::sk_EventType("end_game");
    const EventType EvtData_New_Actor::sk_EventType("new_actor");
    const EventType EvtData_Move_Actor::sk_EventType("move_actor");
    const EventType EvtData_Destroy_Actor::sk_EventType("destroy_actor");
    const EventType EvtData_Game_State::sk_EventType("game_state");
    const EventType EvtData_Request_Start_Game::sk_EventType("game_request_start");
    const EventType EvtData_Remote_Client::sk_EventType("remote_client");
    const EventType EvtData_Network_Player_Actor_Assignment::sk_EventType("netplayer_actor_assign");
    const EventType EvtData_Update_Tick::sk_EventType("update_tick");
    const EventType EvtData_Debug_String::sk_EventType("debug_string");
    const EventType EvtData_Decompress_Request::sk_EventType("decompress_request");
    const EventType EvtData_Decompression_Progress::sk_EventType("decompression_progress");
    const EventType EvtData_Request_New_Actor::sk_EventType("request_new_actor");
    const EventType EvtData_UpdateActorParams::sk_EventType("update_actor_params");
    const EventType EvtData_Pause_Game_Event::sk_EventType("pause_game_event");
    const EventType EvtData_Button_Action::sk_EventType("button_action");
    const EventType EvtData_List_Button_Action::sk_EventType("list_button_action");
    const EventType EvtData_Slider_Action::sk_EventType("slider_action");
    const EventType EvtData_Request_Pause_Game_Event::sk_EventType("request_pause_game_event");
    const EventType EvtData_Dialog_Open_Request_Event::sk_EventType("dialog_open_request_event");
    const EventType EvtData_Dialog_Open_Event::sk_EventType("dialog_open_event");
    const EventType EvtData_Dialog_Close_Event::sk_EventType("dialog_close_event");
    const EventType EvtData_Loading_Progress::sk_EventType("loading_progress");
    const EventType EvtData_Move_Kinematic_Actor::sk_EventType("user_move_actor_event");
    const EventType EvtData_Video_Resolution_Change::sk_EventType("video_resolution_change");
    const EventType EvtData_Physics_Diagnostics::sk_EventType("physics_diagnostic_mode");
}
