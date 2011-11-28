// /////////////////////////////////////////////////////////////////
// @file Pool3dCommon.h
// @author PJ O Halloran.
// @date 11/01/2011
//
// Common defines, data structures, etc. for the Pool3d app.
//
// /////////////////////////////////////////////////////////////////

#ifndef __POOL_3D_COMMON_H
#define __POOL_3D_COMMON_H

// External Headers



// Project Headers


namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @enum ViewMode
	// 
	// Various view modes the game may be in.
	//
	// eShotAim:	Camera behind the cue ball.  Cue and view rotates
	//				around cue ball on player input.
	// eTable:		Camera faces down and onto the centre of the table.
	//				User may rotate and move about table and zoom in/out
	//				on the current table focus spot.
	// eShotDisplay:Camera pans up from current position to a position
	//				above the table facing down to display the result
	//				of the shot.
	// eDebugFree:	User may move freely about the scene (debug mode).
	//
	// /////////////////////////////////////////////////////////////////
	enum ViewMode
	{
		eShotAim = 0,
		eTable,
		eShotDisplay,
		eDebugFree,
		eNumberViewModes
	};

	// /////////////////////////////////////////////////////////////////
	// @enum ShotAimMode
	//
	// Various shot modes the player must go through before taking a
	// shot.
	//
	// /////////////////////////////////////////////////////////////////
	enum ShotAimMode
	{
		eShotSetup = 0,
		eShotInProgress,
		eNumberAimModes
	};

	// ID for the EvtData_Loading_Progress event when the game has completed loading all game actors and their data.
	static const I32 POOL_GAME_LOADED_ID = 51;

}

#endif
