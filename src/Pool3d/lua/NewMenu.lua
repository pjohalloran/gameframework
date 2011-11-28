--[[
	@file NewMenu.lua
	@author PJ O Halloran
	@date 11/11/2010

	This file defines and sets up the NewMenu screen in the application.

	All variables defined here in the global namespace should be prefixed with
	the letters "newm" to help avoid global name clashes and overwrites.

]]

--[[
	*******************************************************************

	Container and Widget definitions.

	*******************************************************************
]]

-- NewMenu Screen container.
newmScreenContainer =
{
	Id = 0,
	Type = "Container",
	Position =
	{
		Type = "relative",
		x = 0.0,
		y = 1.0,
		z = 0.0
	},
	Color =
	{
		r = 0.75,
		g = 1.0,
		b = 0.75,
		a = 0.7
	},
	Dimension =
	{
		Type = "relative",
		Width = 1.0,
		Height = 1.0,
		Depth = 0.0
	},
	--TextureName = "textures/greenfelt.tga",
	TextureName = "textures/MainMenuBackground.tga",
	--TextureName = "",
	Visible = true
}

-- Tutorial Button.
newmTutorialGameButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.45 - 0.025,
		y = 0.75,
		z = 0.0
	},
	Color =
	{
		r = 0.0,
		g = 0.0,
		b = 0.0,
		a = 1.0
	},
	Dimension =
	{
		Type = "relative",
		Width = 0.15,
		Height = 0.05,
		Depth = 0.0
	},
	TextureName = "textures/dBut.tga",
	Visible = true,
	Enabled = true,
	EventTypeId = 2,
	Text = "How to play",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}

-- Quick Game button.
newmQuickButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.45 - 0.025,
		y = 0.65,
		z = 0.0
	},
	Color =
	{
		r = 0.0,
		g = 0.0,
		b = 0.0,
		a = 1.0
	},
	Dimension =
	{
		Type = "relative",
		Width = 0.15,
		Height = 0.05,
		Depth = 0.0
	},
	TextureName = "textures/dBut.tga",
	Visible = true,
	Enabled = true,
	EventTypeId = 1,
	Text = "Pool",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}

-- Back Button
newmBackButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.475 - 0.025,
		y = 0.55,
		z = 0.0
	},
	Color =
	{
		r = 0.0,
		g = 0.0,
		b = 0.0,
		a = 1.0
	},
	Dimension =
	{
		Type = "relative",
		Width = 0.1,
		Height = 0.05,
		Depth = 0.0
	},
	TextureName = "textures/dBut.tga",
	Visible = true,
	Enabled = true,
	EventTypeId = 4,
	Text = "Back",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}

--[[
	*******************************************************************

	Setup screen and event handling from here.

	*******************************************************************
]]

-- Global lua parameters for the MainMenu screen.

-- Used for debugging statements.
local SCRIPT_NAME = "NewMenu.lua: ";
local DEBUG_SCRIPT = false;

-- Check if the global debug flag was set in the init script.
if(INIT_DEBUG_MODE ~= nil and INIT_DEBUG_MODE == true) then
	print("Turning on debugging for " .. SCRIPT_NAME);
	DEBUG_SCRIPT = true;
end

-- ID of the New Menu screen.
newmScreenType = "NewMenu";

-- Game mode Ids
newmGameModePool = 0;
newmGameModeTutorial = 1;

--[[
	Performs some checks to see if the global variables and
	application bindings we require exist.

	@param N/A

	@return bool True if the required lua objects are present.
]]
local function newmPreliminarySystemsCheck()
	local FUNCTION_NAME = "newmPreliminarySystemsCheck(): ";

	-- Check for required lua variables, functions and application bindings.
	if (printDebug == nil) then
		print(FUNCTION_NAME .. "Missing printDebug");
		return (false);
	end
	if (logError == nil) then
		print(FUNCTION_NAME .. "Missing logError");
		return (false);
	end
	if (Pool3dMenuView == nil) then
		print(FUNCTION_NAME .. "Missing Pool3dMenuView");
		return (false);
	end
	if (GameMain == nil) then
		print(FUNCTION_NAME .. "Missing GameMain");
		return (false);
	end
	if (WindowManager == nil) then
		print(FUNCTION_NAME .. "Missing WindowManager");
		return (false);
	end
	if (LuaStateManager == nil) then
		print(FUNCTION_NAME .. "Missing LuaStateManager");
		return (false);
	end
	if (EventManager == nil) then
		print(FUNCTION_NAME .. "Missing EventManager");
		return (false);
	end
	if (newmScreenType == nil) then
		print(FUNCTION_NAME .. "Missing newmScreenType variable");
		return (false);
	end
	if (newmGameModePool == nil) then
		print(FUNCTION_NAME .. "Missing newmGameModePool variable");
		return (false);
	end
	if (newmGameModeTutorial == nil) then
		print(FUNCTION_NAME .. "Missing newmGameModeTutorial variable");
		return (false);
	end
	if (isctScreenType == nil) then
		print(FUNCTION_NAME .. "Missing isctScreenType variable");
		return (false);
	end
	if (mmScreenType == nil) then
		print(FUNCTION_NAME .. "Missing mmScreenType variable");
		return (false);
	end

	-- Success
	return (true);
end

--[[
	Create the NewMenu Screen and add all its widgets.

	@param N/A

	@return True|False on success or error.
]]
local function newmSetUpMainScreen()
	local FUNCTION_NAME = "newmSetUpMainScreen(): ";	-- Name of the function for debugging.

	-- Register the NewMenu screen ID.
	if (Pool3dMenuView:RegisterScreenType(newmScreenType) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to register the screen type with the View UI manager");
		return (false);
	end
	if (Pool3dMenuView:IsRegistered(newmScreenType) ~= true) then
		printDebug(FUNCTION_NAME .. "Pool3dMenuView reporting that screen was not registered");
		return (false);
	end

	-- Add the new menu screen.
	if (Pool3dMenuView:AddScreen(newmScreenType, newmScreenContainer) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to add the container to the screen " .. newmScreenType);
		return (false);
	end

	-- Add all the widgets to the container.
	newmTutorialGameButton.Id = Pool3dMenuView:AddWidgetToScreen(newmScreenType, newmTutorialGameButton);
	if (newmTutorialGameButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the Tutorial Demo Button to the NewMenu container");
		return (false);
	end
	newmQuickButton.Id = Pool3dMenuView:AddWidgetToScreen(newmScreenType, newmQuickButton);
	if (newmQuickButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the Pool Button to the NewMenu container");
		return (false);
	end
	newmBackButton.Id = Pool3dMenuView:AddWidgetToScreen(newmScreenType, newmBackButton);
	if (newmBackButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the Back Button to the NewMenu container");
		return (false);
	end

	return (true);
end

--[[
	Event handler function for button press events for the NewMenu screen.

	@param eventData The event data.

	@return void
]]
function newmButtonActionEvent(eventData)
	local FUNCTION_NAME = "newmButtonActionEvent(): ";

	if (eventData == nil) then
		printDebug(FUNCTION_NAME .. "Got no data");
	elseif (eventData.ButtonId == nil or eventData.EventTypeId == nil or eventData.Checked == nil) then
		printDebug(FUNCTION_NAME .. "Invalid data");
	else
		local buttonId = eventData.ButtonId;
		local eventId = eventData.EventTypeId;
		local checked = eventData.Checked;

		if(INIT_DEBUG_MODE == true) then
			local buttonIdStr = tostring(buttonId);
			local eventIdStr = tostring(eventId);
			local checkedStr = tostring(checked);
			printDebug(FUNCTION_NAME .. "Button Data: " .. buttonIdStr .. " " .. eventIdStr .. " " .. checkedStr);
		end

		-- User Clicks the Pool Game Button!
		if (buttonId == newmQuickButton.Id) then
			printDebug(FUNCTION_NAME .. "Pool Game Button Click");
			-- Hide the current screen and broadcast an event to request startup of the game.
			Pool3dMenuView:SetCurrentScreenType("");
			local eventData = { Mode = newmGameModePool, HumanPlayers = 2};
			EventManager:TriggerEvent("game_mode", eventData);
			local requestStartGameEvtData = {};
			EventManager:TriggerEvent("game_request_start", requestStartGameEvtData);

		-- User clicks the Graphics Demo Button.
		elseif (buttonId == newmTutorialGameButton.Id) then
			--displayDialog("Under Construction!", "information", 2.0, 1);
			--local eventData = { Mode = newmGameModeTutorial, HumanPlayers = 1};
			--EventManager:QueueEvent("game_mode", eventData);
			Pool3dMenuView:SetCurrentScreenType(isctScreenType);
			printDebug(FUNCTION_NAME .. "Tutorial Button Click");

		-- User clicks the Back Button!
		elseif (buttonId == newmBackButton.Id) then
			-- Go back to the previous screen (The Main Menu!).
			printDebug(FUNCTION_NAME .. "Back Button Click");
			--Pool3dMenuView:ShowPreviousScreen();
			Pool3dMenuView:SetCurrentScreenType(mmScreenType);

		else
			-- We are not interested in other buttons events!
		end
	end
end

--[[
	Set up the UI event handlers for the NewMenu screen.

	@param N/A

	@return bool True|False on success error.
]]
local function newmSetUpEventHandlers()
	local FUNCTION_NAME = "newmSetUpEventHandlers(): ";	-- Name of the function for debugging.

	EventManager:AddScriptListener("button_action", newmButtonActionEvent);
end

--[[
	Main function.  Creates the NewMenu screen and sets up the event
	handlers for its UI widgets.
]]
local function newmMain()
	if(newmPreliminarySystemsCheck() == true) then
		printDebug("Executing " .. SCRIPT_NAME);

		newmSetUpMainScreen();

		newmSetUpEventHandlers();
	else
		print("Failed to run " .. SCRIPT_NAME);
	end
end

-- Main entry point of SCRIPT_NAME
newmMain();
