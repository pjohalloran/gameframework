--[[
	@file InstructionsScreen.lua
	@author PJ O Halloran
	@date 11/11/2010

	This file defines and sets up the Instructions screen in the application.

	All variables defined here in the global namespace should be prefixed with
	the letters "isct" to help avoid global name clashes and overwrites.

]]

--[[
	*******************************************************************

	Container and Widget definitions.

	*******************************************************************
]]

-- NewMenu Screen container.
isctScreenContainer =
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
	AtlasName = "UI",
	TextureName = "MainMenuBackground.tga",
	Visible = true
}

-- Confirm/Quit Button.
isctConfirmButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.45 - 0.025,
		y = 0.15,
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
	AtlasName = "UI",
	TextureName = "dBut.tga",
	Visible = true,
	Enabled = true,
	EventTypeId = 2,
	Text = "OK",
	PressedTexture = "pBut.tga",
	HoverTexture = "hBut.tga",
	SendCodeEventOnClick = true
}

isctInstructionsTextBox =
{
	Id = 0,
	Type = "TextBox",
	Color =
	{
		r = 1.0,
		g = 1.0,
		b = 1.0,
		a = 0.5
	},
	Dimension =
	{
		Type = "relative",
		Width = 0.8,
		Height = 0.75,
		Depth = 0.0
	},
	Position =
	{
		Type = "relative",
		x = 0.1,
		y = 0.925,
		z = 0.0
	},
	AtlasName = "UI",
	TextureName = "Instructions.tga",
	Visible = true,
	Enabled = true,
	Text = ""
}

--[[
	*******************************************************************

	Setup screen and event handling from here.

	*******************************************************************
]]

-- Global lua parameters for the InstructionsScreen screen.

-- Used for debugging statements.
local SCRIPT_NAME = "InstructionsScreen.lua: ";
local DEBUG_SCRIPT = false;

-- Check if the global debug flag was set in the init script.
if(INIT_DEBUG_MODE ~= nil and INIT_DEBUG_MODE == true) then
	print("Turning on debugging for " .. SCRIPT_NAME);
	DEBUG_SCRIPT = true;
end

-- ID of the instructions screen.
isctScreenType = "InstructionsScreen";

--[[
	Performs some checks to see if the global variables and
	application bindings we require exist.

	@param N/A

	@return bool True if the required lua objects are present.
]]
local function isctPreliminarySystemsCheck()
	local FUNCTION_NAME = "isctPreliminarySystemsCheck(): ";

	-- Check for required lua variables, functions and application bindings.
	if (printDebug == nil) then
		print(FUNCTION_NAME .. "Missing printDebug");
		return (false);
	end
	if (logError == nil) then
		print(FUNCTION_NAME .. "Missing logError");
		return (false);
	end
	if (displayDialog == nil) then
		print(FUNCTION_NAME .. "Missing displayDialog");
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

	-- Success
	return (true);
end

--[[
	Create the Instructions Screen and add all its widgets.

	@param N/A

	@return True|False on success or error.
]]
local function isctSetUpMainScreen()
	local FUNCTION_NAME = "isctSetUpMainScreen(): ";	-- Name of the function for debugging.

	-- Register the NewMenu screen ID.
	if (Pool3dMenuView:RegisterScreenType(isctScreenType) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to register the screen type with the View UI manager");
		return (false);
	end
	if (Pool3dMenuView:IsRegistered(isctScreenType) ~= true) then
		printDebug(FUNCTION_NAME .. "Pool3dMenuView reporting that screen was not registered");
		return (false);
	end

	-- Add the new menu screen.
	if (Pool3dMenuView:AddScreen(isctScreenType, isctScreenContainer) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to add the container to the screen " .. isctScreenType);
		return (false);
	end

	-- Add all the widgets to the container.
	isctInstructionsTextBox.Id = Pool3dMenuView:AddWidgetToScreen(isctScreenType, isctInstructionsTextBox);
	if (isctInstructionsTextBox.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the Instructions box to the InstructionsScreen container");
		return (false);
	end
	isctConfirmButton.Id = Pool3dMenuView:AddWidgetToScreen(isctScreenType, isctConfirmButton);
	if (isctConfirmButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the Confirm Button to the InstructionsScreen container");
		return (false);
	end

	return (true);
end

--[[
	Event handler function for button press events for the Instructions screen.

	@param eventData The event data.

	@return void
]]
function isctButtonActionEvent(eventData)
	local FUNCTION_NAME = "isctButtonActionEvent(): ";

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

		-- User Clicks the OK/Confirm Button!
		if (buttonId == isctConfirmButton.Id) then
			printDebug(FUNCTION_NAME .. "Confirm Click");
			-- Close this screen and go back to the previous screen.
			Pool3dMenuView:ShowPreviousScreen();

		else
			-- We are not interested in other buttons events!
		end
	end
end

--[[
	Set up the UI event handlers for the Instructions screen.

	@param N/A

	@return bool True|False on success error.
]]
local function isctSetUpEventHandlers()
	local FUNCTION_NAME = "isctSetUpEventHandlers(): ";	-- Name of the function for debugging.

	EventManager:AddScriptListener("button_action", isctButtonActionEvent);
end

--[[
	Main function.  Creates the NewMenu screen and sets up the event
	handlers for its UI widgets.
]]
local function isctMain()
	if(isctPreliminarySystemsCheck() == true) then
		printDebug("Executing " .. SCRIPT_NAME);

		isctSetUpMainScreen();

		isctSetUpEventHandlers();
	else
		print("Failed to run " .. SCRIPT_NAME);
	end
end

-- Main entry point of SCRIPT_NAME
isctMain();

