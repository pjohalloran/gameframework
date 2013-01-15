--[[
	@file init.lua
	@author PJ O Halloran

	Performs global application initialization for the lua manager.
	Also is the location of some useful global variables such as those that tell scripts
	that are run later on what to do (e.g. print out debug info, etc.).
]]

INIT_APPLICATION_NAME = "TestApp";
INIT_GAME_ROOT_PATH = "";
INIT_LUA_SCRIPTS_IN_RESOURCE_CACHE = false;
INIT_RUNNING_DEBUG_BUILD = false;
INIT_DEBUG_MODE = true;
INIT_PLAYER_OPTIONS = {};

INIT_BLACK_COLOR = { r = 0.0, g = 0.0, b = 0.0, a = 1.0 };
INIT_RED_COLOR = { r = 1.0, g = 0.0, b = 0.0, a = 1.0 };
INIT_GREEN_COLOR = { r = 0.0, g = 1.0, b = 0.0, a = 1.0 };
INIT_BLUE_COLOR = { r = 0.0, g = 0.0, b = 1.0, a = 1.0 };
INIT_YELLOW_COLOR = { r = 1.0, g = 1.0, b = 0.0, a = 1.0 };
INIT_MAGENTA_COLOR = { r = 1.0, g = 0.0, b = 1.0, a = 1.0 };
INIT_CYAN_COLOR = { r = 0.0, g = 1.0, b = 1.0, a = 1.0 };
INIT_DARKGRAY_COLOR = { r = 0.25, g = 0.25, b = 0.25, a = 1.0 };
INIT_LIGHTGRAY_COLOR = { r = 0.75, g = 0.75, b = 0.75, a = 1.0 };
INIT_BROWN_COLOR = { r = 0.60, g = 0.40, b = 0.12, a = 1.0 };
INIT_ORANGE_COLOR = { r = 0.98, g = 0.625, b = 0.12, a = 1.0 };
INIT_PINK_COLOR = { r = 0.98, g = 0.04, b = 0.7, a = 1.0 };
INIT_PURPLE_COLOR = { r = 0.60, g = 0.40, b = 0.70, a = 1.0 };
INIT_WHITE_COLOR = { r = 1.0, g = 1.0, b = 1.0, a = 1.0 };

INIT_UP_DIRECTION = { x = 0.0, y = 1.0, z = 0.0 };
INIT_RIGHT_DIRECTION = { x = 1.0, y = 0.0, z = 0.0 };
INIT_FORWARD_DIRECTION = { x = 0.0, y = 0.0, z = -1.0 };
INIT_ORIGIN_POINT = { x = 0.0, y = 0.0, z = 0.0 };

INIT_PHYSICS_PLAYDOUGH_MATERIAL = { Restitution = 0.05, Friction = 0.9 };
INIT_PHYSICS_NORMAL_MATERIAL = { Restitution = 0.25, Friction = 0.5 };
INIT_PHYSICS_BOUNCY_MATERIAL = { Restitution = 0.95, Friction = 0.5 };
INIT_PHYSICS_SLIPPERY_MATERIAL = { Restitution = 0.25, Friction = 0.0 };
INIT_PHYSICS_DENSITY_AIR = 0.0013;
INIT_PHYSICS_DENSITY_WATER = 1.0;
INIT_PHYSICS_DENSITY_STATIC = 0.0;
INIT_PHYSICS_DENSITY_STYROFOAM = 0.01;
INIT_PHYSICS_DENSITY_BALSA = 0.017;
INIT_PHYSICS_DENSITY_BAMBOO = 0.35;
INIT_PHYSICS_DENSITY_PINE = 0.5;
INIT_PHYSICS_DENSITY_OAK = 0.83;
INIT_PHYSICS_DENSITY_EBONY = 1.1;
INIT_PHYSICS_DENSITY_BLOOD = 1.060;
INIT_PHYSICS_DENSITY_BONE = 1.8;
INIT_PHYSICS_DENSITY_SILICON = 2.4;
INIT_PHYSICS_DENSITY_ALUMINIUM = 2.65;
INIT_PHYSICS_DENSITY_ASBESTOS = 2.45;
INIT_PHYSICS_DENSITY_BARITE = 4.5;
INIT_PHYSICS_DENSITY_BISMUTH = 9.8;
INIT_PHYSICS_DENSITY_BORAX = 1.75;
INIT_PHYSICS_DENSITY_BORON = 2.32;
INIT_PHYSICS_DENSITY_BRASS = 8.55;
INIT_PHYSICS_DENSITY_BRICK = 8.64;
INIT_PHYSICS_DENSITY_BRONZE = 8.4;
INIT_PHYSICS_DENSITY_CALCIUM = 4.58;
INIT_PHYSICS_DENSITY_CARBON = 1.95;
INIT_PHYSICS_DENSITY_CHROMIUM = 7.1;
INIT_PHYSICS_DENSITY_CLAY = 2.2;
INIT_PHYSICS_DENSITY_COAL = 0.8;
INIT_PHYSICS_DENSITY_COBALT = 8.9;
INIT_PHYSICS_DENSITY_COPPER = 8.75;
INIT_PHYSICS_DENSITY_DIAMOND = 3.51;
INIT_PHYSICS_DENSITY_DOLOMITE = 2.9;
INIT_PHYSICS_DENSITY_EPOXY = 1.8;
INIT_PHYSICS_DENSITY_GLASS = 2.6;
INIT_PHYSICS_DENSITY_CRYSTAL = 2.95;
INIT_PHYSICS_DENSITY_GRANITE = 2.55;
INIT_PHYSICS_DENSITY_GOLD = 19.3;
INIT_PHYSICS_DENSITY_HEMATITE = 5.2;
INIT_PHYSICS_DENSITY_IRIDIUM = 21.6;
INIT_PHYSICS_DENSITY_CASTIRON = 7.2;
INIT_PHYSICS_DENSITY_WROUGHTIRON = 7.75;
INIT_PHYSICS_DENSITY_LIMESTONE = 2.4;
INIT_PHYSICS_DENSITY_LEAD = 11.34;
INIT_PHYSICS_DENSITY_MAGNETITE = 3.2;
INIT_PHYSICS_DENSITY_MANGANESE = 7.42;
INIT_PHYSICS_DENSITY_MAGNESIUM = 1.74;
INIT_PHYSICS_DENSITY_MARBLE = 2.72;
INIT_PHYSICS_DENSITY_MERCURY = 13.54;
INIT_PHYSICS_DENSITY_MOLYBDENUM = 10.2;
INIT_PHYSICS_DENSITY_NICKEL = 8.9;
INIT_PHYSICS_DENSITY_PLATINUM = 21.45;
INIT_PHYSICS_DENSITY_POTASSIUM = 8.6;
INIT_PHYSICS_DENSITY_QUARTZ = 2.65;
INIT_PHYSICS_DENSITY_SANDSTONE = 2.3;
INIT_PHYSICS_DENSITY_SERPENTINE = 2.75;
INIT_PHYSICS_DENSITY_SILVER = 10.5;
INIT_PHYSICS_DENSITY_SODIUM = 0.97;
INIT_PHYSICS_DENSITY_STEEL = 7.8;
INIT_PHYSICS_DENSITY_TALC = 2.7;
INIT_PHYSICS_DENSITY_TAR = 1.2;
INIT_PHYSICS_DENSITY_TELLURIUM = 6.12;
INIT_PHYSICS_DENSITY_TIN = 7.35;
INIT_PHYSICS_DENSITY_TITANIUM = 4.5;
INIT_PHYSICS_DENSITY_TUNGSTEN = 19.22;
INIT_PHYSICS_DENSITY_URANIUM = 18.7;
INIT_PHYSICS_DENSITY_VANADIUM = 5.96;
INIT_PHYSICS_DENSITY_VINYL = 1.8;
INIT_PHYSICS_DENSITY_WOOL = 1.32;
INIT_PHYSICS_DENSITY_ZINC = 7.05;

INIT_PHYSICS_BTEST_MATERIAL = { Restitution = 0.25, Friction = 0.5 };
INIT_PHYSICS_TTEST_MATERIAL = { Restitution = 0.1, Friction = 0.5 };

function print(message)
	if(LuaStateManager ~= nil) then
		LuaStateManager:PrintDebugMessage(message);
	end
end

function printDebug(message)
	if(INIT_DEBUG_MODE == true) then
		print(message);
	end
end

function logError(message, showDialog)
	printDebug(message);
	if (showDialog == true) then
		displayDialog(message, "error", 0.0, 1);
	end
end

--[[
	Takes a string in the format "IntAxIntB" and splits it into two ints.

	Used to split strings in script for the options screen to submit to the
	application when the player changes the options.

	@param resStr The resolution string.

	@return A LUA table (= {IntA, IntB}) or an empty table on error.
]]
function splitResolutionString(resStr)
	local FUNCTION_NAME = "splitResolutionString(): ";	-- Name of function/Debug prints.
	local resolutionTable = {};							-- Output table.

	if (type(resStr) ~= "string") then
		printDebug(FUNCTION_NAME .. "Invalid type for resStr supplied: " .. type(resStr));
		return (resolutionTable);
	end

	-- Get the location of the first "x" character in the string.
	local SPLIT_CHAR = "*";								-- Character to split the resolution string around.
	local sepIndex = string.find(resStr, SPLIT_CHAR);
	if (sepIndex == nil) then
		printDebug(FUNCTION_NAME .. "Failed to find the " .. SPLIT_CHAR .. " character in the string: " .. resStr);
		return (resolutionTable);
	end

	-- Split the string about the character and convert each substring to ints.
	local resWidthStr = string.sub(resStr, 0, sepIndex-1);
	local resHeightStr = string.sub(resStr, sepIndex+1, -1);
	resolutionTable[1] = tonumber(resWidthStr);
	resolutionTable[2] = tonumber(resHeightStr);
	if(#resolutionTable ~= 2 or resolutionTable[1] == nil or resolutionTable[2] == nil) then
		printDebug(FUNCTION_NAME .. "The string " .. resStr .. " contains invalid numbers");
		resolutionTable = {};
		return (resolutionTable);
	end

	-- Success
	return (resolutionTable);
end

--[[
	Display a dialog to the player/developer informing them of some occurence triggered in the scripts
	of the application.

	@param text The text to display on the dialog.
	@param dialogType The type of dialog ("confirm", "information", "error").
	@param timeout The number of seconds to display the dialog before removing it automatically (0 = display forever).
	@param scriptSideDialogId The script side ID of the dialog.

	@return bool true|false on success/failure.
]]
function displayDialog(text, dialogType, timeout, scriptSideDialogId)
	local FUNCTION_NAME = "displayDialog(): ";

	-- Ensure the EventManager is up and running
	if (EventManager == nil) then
		printDebug(FUNCTION_NAME .. "No EventManager found!");
		return (false);
	end

	-- Validate input
	if (type(text) ~= "string") then
		printDebug(FUNCTION_NAME .. "Invalid text");
		return (false);
	end
	if (type(dialogType) ~= "string") then
		printDebug(FUNCTION_NAME .. "Invalid type");
		return (false);
	end
	if (type(scriptSideDialogId) ~= "number") then
		printDebug(FUNCTION_NAME .. "Invalid script side ID");
		return (false);
	end

	-- Correct any invalid non essential parameters.
	if (dialogType ~= "information" and dialogType ~= "confirm" and dialogType ~= "error") then
		-- Set to a default value!
		dialogType = "information";
	end
	if (type(timeout) ~= "number") then
		-- Set to a default value!
		timeout = 0.0;
	end

	-- Create and queue up the dialog request event.
	eventData = {};
	eventData.DialogText = "LUA Msg: " .. text;
	eventData.DialogType = dialogType;
	eventData.DialogTimeout = timeout;
	eventData.DialogScriptId = scriptSideDialogId;
	EventManager:QueueEvent("dialog_open_request_event", eventData);

	-- Success
	return (true);
end

--[[
	Execute a lua script depending on the value of the INIT_LUA_SCRIPTS_IN_RESOURCE_CACHE
	global flag.

	If this is false the normal lua function dofile() will be used.

	If it is true the function LuaStateManager:DoResourceFile() will be used.

	@param scriptName The name of the script.

	@return bool True|False on success or failure.
]]
function executeGameLuaScript(scriptName)
	local FUNCTION_NAME = "executeGameLuaScript(): "	-- For debugging.
	local useLuaCommand = false;						-- Should we use the lua function, dofile()?
	local result = true;								-- Result of function.

	-- Validate parameters.
	if (type(scriptName) ~= "string") then
		printDebug(FUNCTION_NAME .. "Invalid parameters.");
		return (false);
	end

	-- Check how we should run the script.
	if (INIT_LUA_SCRIPTS_IN_RESOURCE_CACHE == nil or INIT_LUA_SCRIPTS_IN_RESOURCE_CACHE == false) then
		useLuaCommand = true;
	else
		useLuaCommand = false;
	end

	-- Execute the script according to the global setting.
	if (useLuaCommand == true and dofile(scriptName) == 1) then
		result = false;
	elseif(useLuaCommand == false) then
		if (LuaStateManager == nil) then
			printDebug(FUNCTION_NAME .. "LuaStateManager binding not found.");
			return (false);
		end
		result = LuaStateManager:DoResourceFile(scriptName);
	end

	return (result);
end

print("Running the init script using version " .. _VERSION .. " of the lua interpreter on " .. os.date());
