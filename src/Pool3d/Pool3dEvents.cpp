// /////////////////////////////////////////////////////////////////
// @file Pool3dEvents.cpp
// @author PJ O Halloran
// @date 01/11/2010
//
// File contains the impl for the Pool3d event classes.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <strstream>
#include <string>

#include <boost/shared_ptr.hpp>


// Project Headers
#include "Pool3dEvents.h"

//using boost::shared_ptr;

namespace GameHalloran
{
	const EventType EvtData_Complex_Mesh_Loaded::sk_EventType("complex_mesh_loaded");
	const EventType EvtData_Camera_Aim_Mode_Update::sk_EventType("camera_mode_update");
	const EventType EvtData_Camera_Aim_Mode_Request::sk_EventType("camera_mode_request");
	const EventType EvtData_Game_Mode::sk_EventType("game_mode");
	const EventType EvtData_Graphics_Config_Change::sk_EventType("graphics_config_change");
	const EventType EvtData_Load_Game::sk_EventType("load_game");
	const EventType EvtData_Save_Game::sk_EventType("save_game");
	const EventType EvtData_Save_Config_Change::sk_EventType("save_config_change");
	const EventType EvtData_Shot_End::sk_EventType("shot_end");
	const EventType EvtData_Shot_Start::sk_EventType("shot_start");
	const EventType EvtData_Sound_Config_Change::sk_EventType("sound_config_change");
	const EventType EvtData_Camera_Auto_Pan_Start::sk_EventType("camera_auto_pan_start");
	const EventType EvtData_Camera_Auto_Pan_Stop::sk_EventType("camera_auto_pan_stop");
	const EventType EvtData_ResetCueBall::sk_EventType("reset_cue_ball");
	//const EventType EvtData_::sk_EventType("");
}
