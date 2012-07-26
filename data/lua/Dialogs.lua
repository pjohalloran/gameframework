--[[
	@file Dialogs.lua
	@author PJ O Halloran
	@date 15/11/2010

	This file defines and sets up the dialogs used in the application.

	All variables defined here in the global namespace should be prefixed with
	the letters "dlg" to help avoid global name clashes and overwrites.

]]

--[[
	*******************************************************************

	Container and Widget definitions.

	*******************************************************************
]]

-- Options Screen container.
dlgScreenRootContainer =
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
		a = 0.95
	},
	Dimension =
	{
		Type = "relative",
		Width = 1.0,
		Height = 1.0,
		Depth = 0.0
	},
	AtlasName = "UI",
	TextureName = "PanelOutline.tga",
	--TextureName = "",
	Visible = true
}

dlgTitleText =
{
	Type = "TextBox",
	Position =
	{
		Type = "relative",
		x = 0.4,
		y = 0.9,
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
		Width = 0.1,
		Height = 0.05,
		Depth = 0.0
	},
	AtlasName = "UI",
	TextureName = "fontBackground.tga",
	Visible = true,
	Enabled = true,
	-- Title text depends on dialog type!
	Text = ""
}

-- Ok button.
dlgConfirmButton =
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
	AtlasName = "UI",
	TextureName = "dBut.tga",
	Visible = true,
	Enabled = true,
	EventTypeId = 1,
	Text = "Confirm",
	PressedTexture = "pBut.tga",
	HoverTexture = "hBut.tga",
	SendCodeEventOnClick = true
}

-- Cancel button.
dlgCancelButton =
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
	AtlasName = "UI",
	TextureName = "dBut.tga",
	Visible = true,
	Enabled = true,
	EventTypeId = 2,
	Text = "Cancel",
	PressedTexture = "pBut.tga",
	HoverTexture = "hBut.tga",
	SendCodeEventOnClick = true
}



--[[
	*******************************************************************

	Setup screen and event handling from here.

	*******************************************************************
]]

-- Global Dialog variables and functions.

-- Used for debugging statements.
local SCRIPT_NAME = "Dialogs.lua: ";
local DEBUG_SCRIPT = false;

-- Check if the global debug flag was set in the init script.
if(INIT_DEBUG_MODE ~= nil and INIT_DEBUG_MODE == true) then
	print("Turning on debugging for " .. SCRIPT_NAME);
	DEBUG_SCRIPT = true;
end


-- Global event Button Action handling function (Will be set for a duration of a dialogs lifetime to point to a specific event handling function).
dlgGlobalButtonEventHandlingFunction = nil;

-- Global close dialog handling function (Will be set for a duration of a dialogs lifetime to point to a specific event handling function).
dlgGlobalCloseDialogHandlingFunction = nil;

-- The script ID assigned to the current dialog so that other scripts may know when dialogs they are interested in are closing.
dlgGlobalCurrentScriptDialogId = nil;


--[[
	Create dialog function to be called by application.

	@param dialogTypeString The type of dialog to create.
	@param dialogText The text to render on the dialog.
	@param dialogScriptId Script side ID of the dialog.

	@return bool True on success or false on failure.
]]
function dlgCreateDialog(dialogTypeString, dialogText, dialogScriptId)
	local FUNCTION_NAME = "dlgCreateDialog: ";

	-- Validate parameters
	if(dialogTypeString == nil or type(dialogTypeString) ~= "string" or dialogText == nil or type(dialogText) ~= "string" or dialogScriptId == nil or type(dialogScriptId) ~= "number") then
		print(FUNCTION_NAME .. "Invalid arguments");
		return (false);
	end

	-- Set the current script side ID.
	dlgGlobalCurrentScriptDialogId = dialogScriptId;

	local result = true;
	if(dialogTypeString == "confirm") then
		result = dlgCreateConfirmDialog(dialogText);
	elseif(dialogTypeString == "error") then
		result = dlgCreateErrorDialog(dialogText);
	elseif(dialogTypeString == "information") then
		result = dlgCreateInformationDialog(dialogText);
	else
		print(FUNCTION_NAME .. "Unknown dialog type requested: " .. dialogTypeString);
		result = false;
	end

	return (result);
end

--[[
	Destroy dialog function to be called to clean up
	the resources allocated by the application on opening
	the dialog.

	It is called automatically by the dialog in script when the user
	clicks on the button(s) to close the dialog or when the application
	closes the dialog due to a dialog timeout.

	@param N/A

	@return bool True on success or false on failure.
]]
function dlgDestroyDialog()
	local FUNCTION_NAME = "dlgDestroyDialog: ";

	print("1");

	-- Remove the event listener for the dialog.
	EventManager:RemoveScriptListener("button_action", dlgGlobalButtonEventHandlingFunction);

	print("2");

	dlgGlobalButtonEventHandlingFunction = nil;

	print("3");

	EventManager:RemoveScriptListener("dialog_close_event", dlgGlobalCloseDialogHandlingFunction);

	print("4");

	dlgGlobalCloseDialogHandlingFunction = nil;

	dlgGlobalCurrentScriptDialogId = nil;

	print("5");

	return (true);
end

--[[
	Function registered for the duration of a dialogs lifetime
	to listen for dialog close events.  The application itself
	might decide to close the dialog if it is open too long
	since the dialog will cause the game to pause.

	All dialogs will share the same event handler for the close
	event.

	@param N/A

	@return bool True on success or false on failure.
]]
function dlgCloseDialogEventHandler(eventData)
	print("GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG");
	local FUNCTION_NAME = "dlgCloseDialogEventHandler: ";

	if (eventData == nil) then
		printDebug(FUNCTION_NAME .. "Got no data");
	elseif (eventData.DialogId == nil) then
		printDebug(FUNCTION_NAME .. "Invalid data");
	else
		printDebug("");

		if (dlgScreenRootContainer.Id == eventData.DialogId) then

			printDebug("The C application closed the dialog prematurely!");
			-- A value of timeout here will ensure the close dialog event is not sent out twice!
			--dlgDestroyDialog("timeout");

		else
			-- We are not interested in other buttons events!
		end
	end
end

-- **********************************************************************************************************
-- **********************************************************************************************************
-- **************************************** Confirm Dialog **************************************************
-- **********************************************************************************************************
-- **********************************************************************************************************

--[[
	Event handling function for the Confirm dialog.

	@param eventData The table of event data.

	@return bool True on success or false on failure.
]]
function dlgConfirmDialogEventHandler(eventData)
	print("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

	local FUNCTION_NAME = "dlgConfirmDialogEventHandler: ";

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

		if (buttonId == dlgConfirmButton.Id) then

			printDebug(FUNCTION_NAME .. "OK Button Click");
			-- Create and send out the dialog closed event.
			local closeEventData = { DialogId = dlgScreenRootContainer.Id, DialogResult = "ok", DialogScriptId = dlgGlobalCurrentScriptDialogId };
			EventManager:QueueEvent("dialog_close_event", closeEventData);

		elseif (buttonId == dlgCancelButton.Id) then

			printDebug(FUNCTION_NAME .. "Cancel Button Click");
			-- Create and send out the dialog closed event.
			local closeEventData = { DialogId = dlgScreenRootContainer.Id, DialogResult = "cancel", DialogScriptId = dlgGlobalCurrentScriptDialogId };
			EventManager:QueueEvent("dialog_close_event", closeEventData);

		else
			-- We are not interested in other buttons events!
		end
	end
end

--[[
	Creates and adds a confirm dialog container to the UI view list of
	recently script created containers.

	@param text

	@return bool True on success or false on failure.
]]
function dlgCreateConfirmDialog(text)
	local FUNCTION_NAME = "dlgCreateConfirmDialog: ";

	-- Validate parameters
	if(text == nil or type(text) ~= "string") then
		print(FUNCTION_NAME .. "Invalid arguments");
		return (false);
	end

	-- Create dialog container.
	dlgScreenRootContainer.Position.x = 0.25;
	dlgScreenRootContainer.Position.y = 0.75;
	dlgScreenRootContainer.Dimension.Width = 0.5;
	dlgScreenRootContainer.Dimension.Height = 0.5;
	dlgScreenRootContainer.Id = Pool3dMenuView:CreateContainer(dlgScreenRootContainer);
	if(dlgScreenRootContainer.Id == 0) then
		print(FUNCTION_NAME .. "Failed to create container");
		return (false);
	end

	-- Create text widget.
	dlgTitleText.Position.x = 0.35;
	dlgTitleText.Position.y = 0.65;
	dlgTitleText.Text = text;
	dlgTitleText.Id = Pool3dMenuView:CreateWidget(dlgTitleText);
	if(dlgTitleText.Id == 0) then
		print(FUNCTION_NAME .. "Failed to create text widget");
		return (false);
	end

	-- Create OK button.
	dlgConfirmButton.Position.x = 0.35;
	dlgConfirmButton.Position.y = 0.45;
	dlgConfirmButton.Text = "OK";
	dlgConfirmButton.Id = Pool3dMenuView:CreateWidget(dlgConfirmButton);
	if(dlgConfirmButton.Id == 0) then
		print(FUNCTION_NAME .. "Failed to create OK button");
		return (false);
	end

	-- Create cancel button.
	dlgCancelButton.Position.x = 0.50;
	dlgCancelButton.Position.y = 0.45;
	dlgCancelButton.Text = "Cancel";
	dlgCancelButton.Id = Pool3dMenuView:CreateWidget(dlgCancelButton);
	if(dlgCancelButton.Id == 0) then
		print(FUNCTION_NAME .. "Failed to create Cancel button");
		return (false);
	end

	-- Add all widgets to the container we just created.
	local result = Pool3dMenuView:AddWidgetToContainer(dlgScreenRootContainer.Id, dlgTitleText.Id);
	if(result == false) then
		print(FUNCTION_NAME .. "Failed to add Text widget");
		return (false);
	end
	local result = Pool3dMenuView:AddWidgetToContainer(dlgScreenRootContainer.Id, dlgConfirmButton.Id);
	if(result == false) then
		print(FUNCTION_NAME .. "Failed to add Confirm button");
		return (false);
	end
	local result = Pool3dMenuView:AddWidgetToContainer(dlgScreenRootContainer.Id, dlgCancelButton.Id);
	if(result == false) then
		print(FUNCTION_NAME .. "Failed to add Cancel button");
		return (false);
	end

	print(" ************* Registering functions for Confirm Dialog ************************* ");

	-- Set the global event handling function
	dlgGlobalButtonEventHandlingFunction = dlgConfirmDialogEventHandler;
	EventManager:AddScriptListener("button_action", dlgGlobalButtonEventHandlingFunction);

	dlgGlobalCloseDialogHandlingFunction = dlgCloseDialogEventHandler;
	EventManager:AddScriptListener("dialog_close_event", dlgGlobalCloseDialogHandlingFunction);

	return (true);
end

-- **********************************************************************************************************
-- **********************************************************************************************************
-- ***************************************** Error Dialog ***************************************************
-- **********************************************************************************************************
-- **********************************************************************************************************

--[[
	Event handling function for the Error dialog.

	@param eventData The table of event data.

	@return bool True on success or false on failure.
]]
function dlgErrorDialogEventHandler(eventData)
	local FUNCTION_NAME = "dlgErrorDialogEventHandler: ";

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

		if (buttonId == dlgConfirmButton.Id) then

			printDebug(FUNCTION_NAME .. "OK Button Click");
			-- Create and send out the dialog closed event.
			local closeEventData = { DialogId = dlgScreenRootContainer.Id, DialogResult = "ok", DialogScriptId = dlgGlobalCurrentScriptDialogId };
			EventManager:QueueEvent("dialog_close_event", closeEventData);

		else
			-- We are not interested in other buttons events!
		end
	end
end

--[[
	Creates and adds a Error dialog container to the UI view list of
	recently script created containers.

	@param text

	@return bool True on success or false on failure.
]]
function dlgCreateErrorDialog(text)
	local FUNCTION_NAME = "dlgCreateErrorDialog: ";

	print("A");

	-- Validate parameters
	if(text == nil or type(text) ~= "string") then
		print("A1");
		print(FUNCTION_NAME .. "Invalid arguments");
		return (false);
	end

	print("B");

	-- Create dialog container.
	dlgScreenRootContainer.Position.x = 0.25;
	dlgScreenRootContainer.Position.y = 0.75;
	dlgScreenRootContainer.Dimension.Width = 0.5;
	dlgScreenRootContainer.Dimension.Height = 0.5;
	dlgScreenRootContainer.Id = Pool3dMenuView:CreateContainer(dlgScreenRootContainer);
	if(dlgScreenRootContainer.Id == 0) then
		print("B1");
		print(FUNCTION_NAME .. "Failed to create container");
		return (false);
	end

	print("C");

	-- Create text widget.
	dlgTitleText.Position.x = 0.3;
	dlgTitleText.Position.y = 0.65;
	dlgTitleText.Text = text;
	dlgTitleText.Id = Pool3dMenuView:CreateWidget(dlgTitleText);
	if(dlgTitleText.Id == 0) then
		print("C1");
		print(FUNCTION_NAME .. "Failed to create text widget");
		return (false);
	end

	print("D");

	-- Create OK button.
	dlgConfirmButton.Position.x = 0.45;
	dlgConfirmButton.Position.y = 0.45;
	dlgConfirmButton.Text = "OK";
	dlgConfirmButton.Id = Pool3dMenuView:CreateWidget(dlgConfirmButton);
	if(dlgConfirmButton.Id == 0) then
		print("D1");
		print(FUNCTION_NAME .. "Failed to create OK button");
		return (false);
	end

	print("E");

	-- Add all widgets to the container we just created.
	local result = Pool3dMenuView:AddWidgetToContainer(dlgScreenRootContainer.Id, dlgTitleText.Id);
	if(result == false) then
		print("E1");
		print(FUNCTION_NAME .. "Failed to add Text widget");
		return (false);
	end

	print("F");

	local result = Pool3dMenuView:AddWidgetToContainer(dlgScreenRootContainer.Id, dlgConfirmButton.Id);
	if(result == false) then
		print("F1");
		print(FUNCTION_NAME .. "Failed to add Confirm button");
		return (false);
	end

	print("G");

	-- Set the global event handling function
	dlgGlobalButtonEventHandlingFunction = dlgErrorDialogEventHandler;
	EventManager:AddScriptListener("button_action", dlgGlobalButtonEventHandlingFunction);

	print("H");

	dlgGlobalCloseDialogHandlingFunction = dlgCloseDialogEventHandler;
	EventManager:AddScriptListener("dialog_close_event", dlgGlobalCloseDialogHandlingFunction);

	print("I");

	return (true);
end

-- **********************************************************************************************************
-- **********************************************************************************************************
-- ************************************** Information Dialog ************************************************
-- **********************************************************************************************************
-- **********************************************************************************************************

--[[
	Event handling function for the Information dialog.

	@param eventData The table of event data.

	@return bool True on success or false on failure.
]]
function dlgInformationDialogEventHandler(eventData)
	local FUNCTION_NAME = "dlgInformationDialogEventHandler: ";

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

		if (buttonId == dlgConfirmButton.Id) then

			printDebug(FUNCTION_NAME .. "OK Button Click");
			-- Create and send out the dialog closed event.
			local closeEventData = { DialogId = dlgScreenRootContainer.Id, DialogResult = "ok", DialogScriptId = dlgGlobalCurrentScriptDialogId };
			EventManager:QueueEvent("dialog_close_event", closeEventData);

		else
			-- We are not interested in other buttons events!
		end
	end
end

--[[
	Creates and adds a Information dialog container to the UI view list of
	recently script created containers.

	@param text

	@return bool True on success or false on failure.
]]
function dlgCreateInformationDialog(text)
	local FUNCTION_NAME = "dlgCreateInformationDialog: ";

	-- Validate parameters
	if(text == nil or type(text) ~= "string") then
		print(FUNCTION_NAME .. "Invalid arguments");
		return (false);
	end

	-- Create dialog container.
	dlgScreenRootContainer.Position.x = 0.25;
	dlgScreenRootContainer.Position.y = 0.75;
	dlgScreenRootContainer.Dimension.Width = 0.5;
	dlgScreenRootContainer.Dimension.Height = 0.5;
	dlgScreenRootContainer.Id = Pool3dMenuView:CreateContainer(dlgScreenRootContainer);
	if(dlgScreenRootContainer.Id == 0) then
		print(FUNCTION_NAME .. "Failed to create container");
		return (false);
	end

	-- Create text widget.
	dlgTitleText.Position.x = 0.3;
	dlgTitleText.Position.y = 0.65;
	dlgTitleText.Text = text;
	dlgTitleText.Id = Pool3dMenuView:CreateWidget(dlgTitleText);
	if(dlgTitleText.Id == 0) then
		print(FUNCTION_NAME .. "Failed to create text widget");
		return (false);
	end

	-- Create OK button.
	dlgConfirmButton.Position.x = 0.45;
	dlgConfirmButton.Position.y = 0.45;
	dlgConfirmButton.Text = "OK";
	dlgConfirmButton.Id = Pool3dMenuView:CreateWidget(dlgConfirmButton);
	if(dlgConfirmButton.Id == 0) then
		print(FUNCTION_NAME .. "Failed to create OK button");
		return (false);
	end

	-- Add all widgets to the container we just created.
	local result = Pool3dMenuView:AddWidgetToContainer(dlgScreenRootContainer.Id, dlgTitleText.Id);
	if(result == false) then
		print(FUNCTION_NAME .. "Failed to add Text widget");
		return (false);
	end
	local result = Pool3dMenuView:AddWidgetToContainer(dlgScreenRootContainer.Id, dlgConfirmButton.Id);
	if(result == false) then
		print(FUNCTION_NAME .. "Failed to add Confirm button");
		return (false);
	end

	-- Set the global event handling function
	dlgGlobalButtonEventHandlingFunction = dlgInformationDialogEventHandler;
	EventManager:AddScriptListener("button_action", dlgGlobalButtonEventHandlingFunction);

	dlgGlobalCloseDialogHandlingFunction = dlgCloseDialogEventHandler;
	EventManager:AddScriptListener("dialog_close_event", dlgGlobalCloseDialogHandlingFunction);

	return (true);
end

--[[
	Performs some checks to see if the global variables and
	application bindings we require exist.

	@param N/A

	@return bool True if the required lua objects are present.
]]
local function dlgPreliminarySystemsCheck()
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

	-- Success
	return (true);
end

--[[
	Main function.  Creates the NewMenu screen and sets up the event
	handlers for its UI widgets.
]]
local function dlgMain()
	if(dlgPreliminarySystemsCheck() == true) then
		printDebug("Executing " .. SCRIPT_NAME);
	else
		print("Failed to run " .. SCRIPT_NAME);
	end
end

-- Main entry point of SCRIPT_NAME
dlgMain();
