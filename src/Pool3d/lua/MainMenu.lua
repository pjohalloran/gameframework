--[[
	@file MainMenu.lua
	@author PJ O Halloran
	@date 11/11/2010

	This file defines and sets up the MainMenu screen in the application.

	All variables defined here in the global namespace should be prefixed with
	the letters "mm" to help avoid global name clashes and overwrites.

]]

--[[
	*******************************************************************

	Container and Widget definitions.

	*******************************************************************
]]

-- MainMenu Screen container.
mmScreenContainer =
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
		r = 0.0,
		g = 1.0,
		b = 0.0,
		a = 1.0
	},
	Dimension =
	{
		Type = "relative",
		Width = 1.0,
		Height = 1.0,
		Depth = 0.0
	},
	TextureName = "textures/MainMenuBackground.tga",
	--TextureName = "",
	Visible = true
}

mmGameLogo =
{
	Type = "TextBox",
	Position =
	{
		Type = "relative",
		x = 0.35,
		y = 0.95,
		z = 0.0
	},
	Color =
	{
		r = 0.0,
		g = 0.0,
		b = 0.0,
		a = 0.1
	},
	Dimension =
	{
		Type = "relative",
		Width = 0.3,
		Height = 0.2,
		Depth = 0.0
	},
	TextureName = "textures/Pool3dLogo.tga",
	--TextureName = "",
	Visible = true,
	Enabled = true,
	Text = ""
}


-- New Game button.
mmNewButton =
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
	Text = "New",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}

--[[
-- Load Game Button.
mmLoadButton =
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
	EventTypeId = 2,
	Text = "Load",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}
]]

-- Options Button.
mmOptionsButton =
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
	EventTypeId = 3,
	Text = "Options",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}

-- Credits Button
mmCreditsButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.45,
		y = 0.45,
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
	Text = "Credits",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}

-- Exit Button.
mmExitButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.45,
		y = 0.35,
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
	EventTypeId = 5,
	Text = "Exit",
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
local SCRIPT_NAME = "MainMenu.lua: ";
local DEBUG_SCRIPT = false;

-- Check if the global debug flag was set in the init script.
if(INIT_DEBUG_MODE ~= nil and INIT_DEBUG_MODE == true) then
	print("Turning on debugging for " .. SCRIPT_NAME);
	DEBUG_SCRIPT = true;
end

-- ID of the MainMenu screen
mmScreenType = "MainMenu";

-- ID of the confirm exit dialog
mmExitDialogId = 11223344;

-- Data for the credits screen.
mmCreditsDialogId = 44882211;
mmCreditsText = "Designed and programmed by PJ O Halloran (pjohalloran@gmail.com).";

--[[
	Performs some checks to see if the global variables and
	application bindings we require exist.

	@param N/A

	@return bool True if the required lua objects are present.
]]
local function mmPreliminarySystemsCheck()
	local FUNCTION_NAME = "mmPreliminarySystemsCheck(): ";

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
	if (mmScreenType == nil) then
		print(FUNCTION_NAME .. "Missing mmScreenType variable");
		return (false);
	end
	if (mmExitDialogId == nil) then
		print(FUNCTION_NAME .. "Missing mmExitDialogId variable");
		return (false);
	end

	-- Success
	return (true);
end

--[[
	Create the MainMenu Screen and add all its widgets.

	@param N/A

	@return True|False on success or error.
]]
local function mmSetUpMainScreen()
	local FUNCTION_NAME = "mmSetUpMainScreen(): ";	-- Name of the function for debugging.
	local result = false;							-- Result of various PoolMenuView operations.

	-- Register the screen container widget ID.
	if (Pool3dMenuView:RegisterScreenType(mmScreenType) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to register the " .. mmScreenType .. " screen type with the View UI manager");
		return (false);
	end
	if (Pool3dMenuView:IsRegistered(mmScreenType) ~= true) then
		printDebug(FUNCTION_NAME .. "Pool3dMenuView reporting that the " .. mmScreenType .. " screen was not registered");
		return (false);
	end

	-- Add the main menu container to the screen manager.
	if (Pool3dMenuView:AddScreen(mmScreenType, mmScreenContainer) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to add the " .. mmScreenType .. " root container");
		return (false);
	end

	-- Add all the widgets to the container.
	mmGameLogo.Id = Pool3dMenuView:AddWidgetToScreen(mmScreenType, mmGameLogo);
	if (mmGameLogo.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the Game Logo to the " .. mmScreenType .. " container");
		return (false);
	end

	mmNewButton.Id = Pool3dMenuView:AddWidgetToScreen(mmScreenType, mmNewButton);
	if (mmNewButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the mmNewButton to the " .. mmScreenType .. " container");
		return (false);
	end

	--[[
	mmLoadButton.Id = Pool3dMenuView:AddWidgetToScreen(mmScreenType, mmLoadButton);
	if (mmLoadButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the mmLoadButton to the " .. mmScreenType .. " container");
		return (false);
	end
	]]

	mmOptionsButton.Id = Pool3dMenuView:AddWidgetToScreen(mmScreenType, mmOptionsButton);
	if (mmOptionsButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the mmOptionsButton to the " .. mmScreenType .. " container");
		return (false);
	end

	mmCreditsButton.Id = Pool3dMenuView:AddWidgetToScreen(mmScreenType, mmCreditsButton);
	if(mmCreditsButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the mmCreditsButton to the " .. mmScreenType .. " container");
		return (false);
	end

	mmExitButton.Id = Pool3dMenuView:AddWidgetToScreen(mmScreenType, mmExitButton);
	if (mmExitButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the mmExitButton to the " .. mmScreenType .. " container");
		return (false);
	end

	-- 4) Since this is the MainMenu script, set this menu as the first active screen!
	result = Pool3dMenuView:SetCurrentScreenType(mmScreenType);
	if (Pool3dMenuView:SetCurrentScreenType(mmScreenType) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to set the " .. mmScreenType .. " as the currently displayed UI screen");
		return (false);
	end

	-- Success
	return (true);
end

--[[
	Event handler function for the exit button dialog close events for the MainMenu screen.

	@param eventData The event data.

	@return void
]]
function mmExitDialogCloseEvent(eventData)
	local FUNCTION_NAME = "mmExitDialogCloseEvent(): ";

	if (eventData == nil) then
		printDebug(FUNCTION_NAME .. "Got no data");
	elseif (eventData.DialogId == nil or eventData.DialogResult == nil or eventData.DialogScriptId == nil) then
		printDebug(FUNCTION_NAME .. "Invalid data");
	else
		--print(FUNCTION_NAME .. "Enter with " .. eventData.DialogScriptId);
		if(eventData.DialogScriptId == mmExitDialogId) then
			if(eventData.DialogResult == "ok") then
				printDebug(FUNCTION_NAME .. "Exiting application due to user confirming quit via UI menus");
				GameMain:SetRunning(false);
			elseif(eventData.DialogResult == "cancel") then
				-- Cancel quit, do nothing!
				printDebug(FUNCTION_NAME .. "User decided to cancel quit action!");
			else
				-- Don't care!
			end
		elseif (eventData.DialogScriptId == mmCreditsDialogId) then
			local requestOpenDialogEventData = { DialogType = "information", DialogText = mmCreditsText, DialogTimeout = 0.0, DialogScriptId = 99 };
			EventManager:QueueEvent("dialog_open_request_event", requestOpenDialogEventData);
		end
	end
end

--[[
	Event handler function for button press events for the MainMenu screen.

	@param eventData The event data.

	@return void
]]
function mmButtonActionEvent(eventData)
	local FUNCTION_NAME = "mmButtonActionEvent(): ";

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

		-- On NewMenu Click.
		if (buttonId == mmNewButton.Id) then
			printDebug(FUNCTION_NAME .. "New Button Click");
			if (newmScreenType ~= nil) then
				-- Set the NewMenu as the current screen.
				Pool3dMenuView:SetCurrentScreenType(newmScreenType);
				--logError(FUNCTION_NAME .. "Click " .. newmScreenType, true);
			else
				logError(FUNCTION_NAME .. "Can't switch to NewMenu\\n, no NewMenu lua ID!", true);
			end

		--[[
		-- On LoadGame Click.
		elseif (buttonId == mmLoadButton.Id) then

			printDebug(FUNCTION_NAME .. "Load Button Click");
			-- TODO: Send event to trigger load menu opening...
		]]

		-- On OptionsMenu Click.
		elseif (buttonId == mmOptionsButton.Id) then
			printDebug(FUNCTION_NAME .. "Options Button Click " .. opmScreenType);
			if (opmScreenType ~= nil) then
				-- Show the Options menu.
				Pool3dMenuView:SetCurrentScreenType(opmScreenType);
				-- Ensure the graphics sub pane is always the first pane visible.
				local result = Pool3dMenuView:SetScreenWidgetVisibility(opmScreenType, opmSoundPaneContainer.Id, false);
				result = Pool3dMenuView:SetScreenWidgetVisibility(opmScreenType, opmGraphicsPaneContainer.Id, true);
			else
				logError(FUNCTION_NAME .. "Can't switch to OptionsMenu, no OptionsMenu lua ID!", true);
			end

		-- On Credits Button Click.
		elseif (buttonId == mmCreditsButton.Id) then
			printDebug(FUNCTION_NAME .. "Credits Button Click");
			displayDialog("Under Construction!", "information", 2.0, mmCreditsDialogId);
			-- TODO: Send event to start credits...

		-- On Exit Button Click.
		elseif (buttonId == mmExitButton.Id) then
			printDebug(FUNCTION_NAME .. "Exit Button Click");
			local requestOpenDialogEventData = { DialogType = "confirm", DialogText = "Are you sure you wish to exit?", DialogTimeout = 0.0, DialogScriptId = mmExitDialogId };
			EventManager:QueueEvent("dialog_open_request_event", requestOpenDialogEventData);

		else
			-- We are not interested in other buttons events!
		end
	end
end

--[[
	Set up the UI event handlers for the MainMenu screen.

	@param N/A

	@return bool True|False on success error.
]]
local function mmSetUpEventHandlers()
	EventManager:AddScriptListener("button_action", mmButtonActionEvent);
	EventManager:AddScriptListener("dialog_close_event", mmExitDialogCloseEvent);
end

--[[
	The main entry point of the script.  Creates the MainMenu screen
	and sets up the event handlers for its widgets.

	@param N/A

	@return bool True/False on success or error.
]]
local function mmMain()
	if(mmPreliminarySystemsCheck() == true) then
		printDebug("Executing " .. SCRIPT_NAME);

		mmSetUpMainScreen();

		mmSetUpEventHandlers();
	else
		print("Failed to run " .. SCRIPT_NAME);
	end
end

-- Main entry point of script...
mmMain();
