--[[
	@file PauseMenu.lua
	@author PJ O Halloran
	@date 13/11/2010

	This file defines and sets up the Pause screen in the application.

	All variables defined here in the global namespace should be prefixed with
	the letters "pm" to help avoid global name clashes and overwrites.

]]

--[[
	*******************************************************************

	Container and Widget definitions.

	*******************************************************************
]]

-- Options Screen container.
pmScreenContainer =
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
		a = 0.1
	},
	Dimension =
	{
		Type = "relative",
		Width = 1.0,
		Height = 1.0,
		Depth = 0.0
	},
	TextureName = "textures/greenfelt.tga",
	--TextureName = "",
	Visible = true
}

pmTitleText =
{
	Type = "TextBox",
	Position =
	{
		Type = "relative",
		x = 0.425,
		y = 0.9,
		z = 0.0
	},
	Color =
	{
		r = 0.0,
		g = 1.0,
		b = 0.0,
		a = 0.1
	},
	Dimension =
	{
		Type = "relative",
		Width = 0.1,
		Height = 0.05,
		Depth = 0.0
	},
	TextureName = "textures/fontBackgroundA.tga",
	Visible = true,
	Enabled = true,
	Text = "Pause Menu"
}

-- Graphics button.
pmResumeButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.45,
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
		Width = 0.1,
		Height = 0.05,
		Depth = 0.0
	},
	TextureName = "textures/dBut.tga",
	Visible = true,
	Enabled = true,
	EventTypeId = 1,
	Text = "Resume",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}

-- Options button.
pmOptionsButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.45,
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
		Width = 0.1,
		Height = 0.05,
		Depth = 0.0
	},
	TextureName = "textures/dBut.tga",
	Visible = true,
	Enabled = true,
	EventTypeId = 1,
	Text = "Options",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}

--[[
-- Save Game button.
pmSaveGameButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.45,
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
	EventTypeId = 1,
	Text = "Save Game",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}
]]

-- QuitGame button.
pmQuitGameButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.45,
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
	EventTypeId = 1,
	Text = "Quit Game",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}

--[[
	*******************************************************************

	Setup screen and event handling from here.

	*******************************************************************
]]


-- Global Pause Menu parameters...
pmScreenType = "PauseMenu";

pmExitDialogId = 99887766;

-- Used for debugging statements.
local SCRIPT_NAME = "PauseMenu.lua: ";
local DEBUG_SCRIPT = false;

-- Check if the global debug flag was set in the init script.
if(INIT_DEBUG_MODE ~= nil and INIT_DEBUG_MODE == true) then
	print("Turning on debugging for " .. SCRIPT_NAME);
	DEBUG_SCRIPT = true;
end

--[[
	Performs some checks to see if the global variables and
	application bindings we require exist.

	@param N/A

	@return bool True if the required lua objects are present.
]]
local function pmPreliminarySystemsCheck()
	local FUNCTION_NAME = "pmPreliminarySystemsCheck(): ";

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
	if (pmScreenType == nil) then
		print(FUNCTION_NAME .. "Missing pmScreenType variable");
		return (false);
	end

	-- Success
	return (true);
end

--[[
	Create the PauseMenu Screen and add all its widgets.

	@param N/A

	@return True|False on success or error.
]]
local function pmSetUpMainScreen()
	local FUNCTION_NAME = "pmSetUpMainScreen(): ";	-- Name of the function for debugging.

	-- Register the NewMenu screen ID.
	if (Pool3dMenuView:RegisterScreenType(pmScreenType) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to register the screen type with the View UI manager");
		return (false);
	end
	if (Pool3dMenuView:IsRegistered(pmScreenType) ~= true) then
		printDebug(FUNCTION_NAME .. "Pool3dMenuView reporting that screen was not registered");
		return (false);
	end

	-- Add the pause menu screen.
	if (Pool3dMenuView:AddScreen(pmScreenType, pmScreenContainer) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to add the container to the screen " .. newmScreenType);
		return (false);
	end

	-- Add all the widgets to the container.
	pmTitleText.Id = Pool3dMenuView:AddWidgetToScreen(pmScreenType, pmTitleText);
	if (pmTitleText.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the pmTitleText to the PauseMenu container");
		return (false);
	end
	pmResumeButton.Id = Pool3dMenuView:AddWidgetToScreen(pmScreenType, pmResumeButton);
	if (pmResumeButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the pmResumeButton to the PauseMenu container");
		return (false);
	end
	pmOptionsButton.Id = Pool3dMenuView:AddWidgetToScreen(pmScreenType, pmOptionsButton);
	if (pmOptionsButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the pmOptionsButton to the PauseMenu container");
		return (false);
	end
	pmQuitGameButton.Id = Pool3dMenuView:AddWidgetToScreen(pmScreenType, pmQuitGameButton);
	if (pmQuitGameButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the pmQuitGameButton to the PauseMenu container");
		return (false);
	end

	return (true);
end

--[[
	Event handler function for the exit button dialog close events for the MainMenu screen.

	@param eventData The event data.

	@return void
]]
function pmExitDialogCloseEvent(eventData)
	local FUNCTION_NAME = "pmExitDialogCloseEvent(): ";

	if (eventData == nil) then
		printDebug(FUNCTION_NAME .. "Got no data");
	elseif (eventData.DialogId == nil or eventData.DialogResult == nil or eventData.DialogScriptId == nil) then
		printDebug(FUNCTION_NAME .. "Invalid data");
	else
		print(FUNCTION_NAME .. "Enter with " .. eventData.DialogScriptId);
		if(eventData.DialogScriptId == pmExitDialogId) then
			if(eventData.DialogResult == "ok") then
				print(FUNCTION_NAME .. "Exiting game due to user confirming quit via Pause menus");
				-- Notify the game to go back to BGS_MainMenu state!
				local endGameEventData = {};
				EventManager:TriggerEvent("end_game", endGameEventData);

			elseif(eventData.DialogResult == "cancel") then
				-- Cancel quit, do nothing!
				print(FUNCTION_NAME .. "The player decided not to quit the game.");
			else
				-- Don't care!
			end
		end
	end
end

--[[
	Event handler function for button press events for the Pause screen.

	@param eventData The event data.

	@return void
]]
function pmButtonActionEvent(eventData)
	local FUNCTION_NAME = "pmButtonActionEvent(): ";

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

		-- On resume game click.
		if (buttonId == pmResumeButton.Id) then
			printDebug(FUNCTION_NAME .. "Resume Game Button Click");
			local requestPauseEventData = {};
			EventManager:TriggerEvent("request_pause_game_event", requestPauseEventData);

		-- On game options click.
		elseif (buttonId == pmOptionsButton.Id) then
			printDebug(FUNCTION_NAME .. "Options Button Click");
			Pool3dMenuView:SetCurrentScreenType(opmScreenType);
			local result = Pool3dMenuView:SetScreenWidgetVisibility(opmScreenType, opmSoundPaneContainer.Id, false);
			result = Pool3dMenuView:SetScreenWidgetVisibility(opmScreenType, opmGraphicsPaneContainer.Id, true);

		--[[
		elseif (buttonId == pmSaveGameButton.Id) then

			printDebug(FUNCTION_NAME .. "Save Game Button Click");
			-- TODO: Trigger saving the current game state or opening the save game menu from here.
		]]

		-- On quit button click.
		elseif (buttonId == pmQuitGameButton.Id) then
			printDebug(FUNCTION_NAME .. "End Game Button Click");
			requestOpenDialogEventData = { DialogType = "confirm", DialogText = "Are you sure you wish to quit?", DialogTimeout = 0.0, DialogScriptId = pmExitDialogId };
			EventManager:QueueEvent("dialog_open_request_event", requestOpenDialogEventData);

		else
			-- We are not interested in other buttons events!
		end
	end
end

--[[
	Event handler function for Game State Change events.

	@param eventData The event data.

	@return void
]]
function pmPauseGameEvent(eventData)
	local FUNCTION_NAME = "pmButtonActionEvent(): ";

	if (eventData == nil) then
		printDebug(FUNCTION_NAME .. "Got no data");
	elseif (eventData.PauseState == nil) then
		printDebug(FUNCTION_NAME .. "Invalid data");
	else
		local paused = eventData.PauseState;

		-- On Pause, display Pause menu, on unpause, hide it!
		if(paused == true) then
			Pool3dMenuView:SetCurrentScreenType(pmScreenType);
		else
			Pool3dMenuView:SetCurrentScreenType("");
		end

	end
end

--[[
	Set up the UI event handlers for the PauseMenu screen.

	@param N/A

	@return bool True|False on success error.
]]
local function pmSetUpEventHandlers()
	local FUNCTION_NAME = "pmSetUpEventHandlers(): ";	-- Name of the function for debugging.

	EventManager:AddScriptListener("button_action", pmButtonActionEvent);
	EventManager:AddScriptListener("pause_game_event", pmPauseGameEvent);
	EventManager:AddScriptListener("dialog_close_event", pmExitDialogCloseEvent);
end

--[[
	Main function.  Creates the PauseMenu screen and sets up the event
	handlers for its UI widgets.
]]
local function pmMain()
	if(pmPreliminarySystemsCheck() == true) then
		printDebug("Executing " .. SCRIPT_NAME);

		pmSetUpMainScreen();

		pmSetUpEventHandlers();
	else
		print("Failed to run " .. SCRIPT_NAME);
	end
end

-- Main entry point of SCRIPT_NAME
pmMain();
