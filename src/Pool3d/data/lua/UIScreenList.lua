--[[
	@file UiScreenList.lua
	@author PJ O Halloran
	@date 11/11/2010

	This file defines and sets up all the user interface screens for the game. This script
	is called by the application on startup to trigger off creating all the UI screens.

	A local table ("slUiScreenList") contains the names of all the scripts where each screen is
	defined.  This table should contain a string filename of each screen to be added.

	Each screen script should handle defining, creation and event handling for each UI screen.

	If you wish to add more screens then:
	1) Write a script that defines all the screens widgets and creates them and adds them to the
		applications UI view manager.
	2) Append the name of this script to the end of the "slUiScreenList" table below.

]]

--[[
	Main entry point of script.
]]
local function sluiMain()
	local FUNCTION_NAME = "sluiMain(): ";

	-- Ensure we have the required global functions and data...
	if (printDebug == nil) then
		print(FUNCTION_NAME .. "printDebug missing.");
		return (false);
	end
	if (LuaStateManager == nil) then
		print(FUNCTION_NAME .. "LuaStateManager missing.");
		return (false);
	end
	if (INIT_GAME_ROOT_PATH == nil) then
		print(FUNCTION_NAME .. "INIT_GAME_ROOT_PATH missing.");
		return (false);
	end
	if (executeGameLuaScript == nil) then
		print(FUNCTION_NAME .. "executeGameLuaScript missing.");
		return (false);
	end

	-- Table of lua UI scripts to execute.  Add to this if you are adding more screens.
	local slUiScreenList=
	{
		INIT_GAME_ROOT_PATH .. "data/lua/Dialogs.lua",
		INIT_GAME_ROOT_PATH .. "data/lua/MainMenu.lua",
		INIT_GAME_ROOT_PATH .. "data/lua/InstructionsScreen.lua",
		INIT_GAME_ROOT_PATH .. "data/lua/NewMenu.lua",
		INIT_GAME_ROOT_PATH .. "data/lua/OptionsMenu.lua",
		INIT_GAME_ROOT_PATH .. "data/lua/PauseMenu.lua"
	};

	local i = 1;							-- Current loop counter.
	local size = #slUiScreenList;			-- Number of UI screens to setup.
	local result = true;					-- Did we setup all screens correctly?

	-- Execute all the screen UI scripts defined in "slUiScreenList".
	while i <= size do
		printDebug("Executing " .. slUiScreenList[i]);
		if(slUiScreenList[i] == nil) then
			printDebug(slUiScreenList[i] .. " is nil");
			result = false;
		elseif(executeGameLuaScript(slUiScreenList[i]) == false) then
			printDebug("Failed to execute " .. slUiScreenList[i]);
			result = false;
		end
		i = i + 1;
	end

	return (result);
end

-- Main entry point of the script.
sluiMain();
