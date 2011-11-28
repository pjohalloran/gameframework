--[[
	@file OptionsMenu.lua
	@author PJ O Halloran
	@date 11/11/2010

	This file defines and sets up the Options screen in the application.

	All variables defined here in the global namespace should be prefixed with
	the letters "opm" to help avoid global name clashes and overwrites.

]]

--[[
	*******************************************************************

	Container and Widget definitions.

	*******************************************************************
]]

-- Options Screen container.
opmScreenContainer =
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

opmTitleText =
{
	Type = "TextBox",
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
	Position =
	{
		Type = "relative",
		x = 0.4,
		y = 0.9,
		z = 0.0
	},
	TextureName = "textures/fontBackground.tga",
	Visible = true,
	Enabled = true,
	Text = "Game Options"
}

-- Graphics button.
opmGraphicsButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.15,
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
	Text = "Graphics",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}

-- Sound button.
opmSoundButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.15,
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
	Text = "Sound",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}

--[[
-- Contrls button.
opmControlsButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.15,
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
	EventTypeId = 1,
	Text = "Controls",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}

-- SaveGame button.
opmSaveGameButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.15,
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
	EventTypeId = 1,
	Text = "Save Game",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}
]]

-- Confirm button.
opmConfirmButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.35,
		y = 0.1,
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
	Text = "OK",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}

-- Cancel Button.
opmCancelButton =
{
	Id = 0,
	Type = "Button",
	Position =
	{
		Type = "relative",
		x = 0.55,
		y = 0.1,
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
	Text = "Cancel",
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true
}

-- Options Graphics Sub Pane container.
opmGraphicsPaneContainer =
{
	Id = 0,
	Type = "Container",
	Position =
	{
		Type = "relative",
		x = 0.4,
		y = 0.8,
		z = 0.0
	},
	Color =
	{
		r = 1.0,
		g = 1.0,
		b = 1.0,
		a = 0.9
	},
	Dimension =
	{
		Type = "relative",
		Width = 0.5,
		Height = 0.6,
		Depth = 0.0
	},
	TextureName = "textures/PanelOutline.tga",
	Visible = true
}

opmGPTitleText =
{
	Type = "TextBox",
	Position =
	{
		Type = "relative",
		x = 0.6,
		y = 0.75,
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
	TextureName = "textures/fontBackground.tga",
	Visible = true,
	Enabled = true,
	Text = "Graphics"
}

opmGPResolutionText =
{
	Type = "TextBox",
	Position =
	{
		Type = "relative",
		x = 0.5,
		y = 0.65,
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
	TextureName = "textures/fontBackground.tga",
	Visible = true,
	Enabled = true,
	Text = "Screen Resolution"
}

opmGPResolutionListButton =
{
	Type = "ListButton",
	Position =
	{
		Type = "relative",
		x = 0.7,
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
	EventTypeId = 111111,
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true,
	TextTable =
	{
		"640*480",
		"1280*1024",
		"1440*900"
	}
}

opmGPTexFilterText =
{
	Type = "TextBox",
	Position =
	{
		Type = "relative",
		x = 0.5,
		y = 0.55,
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
	TextureName = "textures/fontBackground.tga",
	Visible = true,
	Enabled = true,
	Text = "Texture Filter Mode"
}

opmGPTexFilterListButton =
{
	Type = "ListButton",
	Position =
	{
		Type = "relative",
		x = 0.7,
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
	EventTypeId = 111111,
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true,
	TextTable =
	{
		"Basic",
		"BasicMipmap",
		"BiLinear",
		"TriLinear",
		"Anisotropic"
	}
}

opmGPFsaaText =
{
	Type = "TextBox",
	Position =
	{
		Type = "relative",
		x = 0.5,
		y = 0.45,
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
	TextureName = "textures/fontBackground.tga",
	Visible = true,
	Enabled = true,
	Text = "FSAA Level"
}

opmGPFsaaListButton =
{
	Type = "ListButton",
	Position =
	{
		Type = "relative",
		x = 0.7,
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
	EventTypeId = 111111,
	PressedTexture = "textures/pBut.tga",
	HoverTexture = "textures/hBut.tga",
	SendCodeEventOnClick = true,
	TextTable =
	{
		"x2",
		"x4",
		"x8",
		"x16",
		"x32"
	}
}

opmGPShadowsText =
{
	Type = "TextBox",
	Position =
	{
		Type = "relative",
		x = 0.5,
		y = 0.35,
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
	TextureName = "textures/fontBackground.tga",
	Visible = true,
	Enabled = true,
	Text = "Render Shadows?"
}

opmGPShadowsCheckButton =
{
	Type = "CheckButton",
	Position =
	{
		Type = "relative",
		x = 0.7,
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
		Width = 0.025,
		Height = 0.025,
		Depth = 0.0
	},
	TextureName = "textures/checkDefault.tga",
	Visible = true,
	Enabled = true,
	EventTypeId = 771168,
	CheckedTexture = "textures/checkDefaultChecked.tga",
	HoverTexture = "textures/checkHover.tga",
	HoverCheckedTexture = "textures/checkHoverChecked.tga",
	Checked = true,
	SendCodeEventOnClick = true
}

-- Options Sound Sub Pane container.
opmSoundPaneContainer =
{
	Id = 0,
	Type = "Container",
	Position =
	{
		Type = "relative",
		x = 0.4,
		y = 0.8,
		z = 0.0
	},
	Color =
	{
		r = 1.0,
		g = 1.0,
		b = 1.0,
		a = 1.0
	},
	Dimension =
	{
		Type = "relative",
		Width = 0.5,
		Height = 0.6,
		Depth = 0.0
	},
	TextureName = "textures/PanelOutline.tga",
	Visible = true
}

opmSPTitleText =
{
	Type = "TextBox",
	Position =
	{
		Type = "relative",
		x = 0.6,
		y = 0.75,
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
	TextureName = "textures/fontBackground.tga",
	Visible = true,
	Enabled = true,
	Text = "Sound"
}

opmSPVolumeText =
{
	Type = "TextBox",
	Position =
	{
		Type = "relative",
		x = 0.5,
		y = 0.65,
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
	TextureName = "textures/fontBackground.tga",
	Visible = true,
	Enabled = true,
	Text = "Volume"
}

opmSPVolumeSlider =
{
	Type = "Slider",
	Position =
	{
		Type = "relative",
		x = 0.7,
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
	TextureName = "textures/fontBackground.tga",
	Visible = true,
	Enabled = true,
	SliderPosition = 0.5,
	ButtonTableId = "ButtonTableName",
	EventTypeId = 948737,
	ButtonTableName =
	{
		Type = "Button",
		Color =
		{
			r = 0.0,
			g = 0.0,
			b = 0.0,
			a = 1.0
		},
		TextureName = "textures/dBut.tga",
		PressedTexture = "textures/dBut.tga",
		HoverTexture = "textures/dBut.tga",
		SendCodeEventOnClick = false
	}
}

opmSPMusicText =
{
	Type = "TextBox",
	Position =
	{
		Type = "relative",
		x = 0.5,
		y = 0.50,
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
	TextureName = "textures/fontBackground.tga",
	Visible = true,
	Enabled = true,
	Text = "Music"
}

opmSPMusicCheckButton =
{
	Type = "CheckButton",
	Position =
	{
		Type = "relative",
		x = 0.7,
		y = 0.5,
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
		Width = 0.025,
		Height = 0.025,
		Depth = 0.0
	},
	TextureName = "textures/checkDefault.tga",
	Visible = true,
	Enabled = true,
	EventTypeId = 92763,
	CheckedTexture = "textures/checkDefaultChecked.tga",
	HoverTexture = "textures/checkHover.tga",
	HoverCheckedTexture = "textures/checkHoverChecked.tga",
	Checked = true,
	SendCodeEventOnClick = true
}

opmSPSoundText =
{
	Type = "TextBox",
	Position =
	{
		Type = "relative",
		x = 0.5,
		y = 0.35,
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
	TextureName = "textures/fontBackground.tga",
	Visible = true,
	Enabled = true,
	Text = "Sound"
}

opmSPSoundCheckButton =
{
	Type = "CheckButton",
	Position =
	{
		Type = "relative",
		x = 0.7,
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
		Width = 0.025,
		Height = 0.025,
		Depth = 0.0
	},
	TextureName = "textures/checkDefault.tga",
	Visible = true,
	Enabled = true,
	EventTypeId = 88383,
	CheckedTexture = "textures/checkDefaultChecked.tga",
	HoverTexture = "textures/checkHover.tga",
	HoverCheckedTexture = "textures/checkHoverChecked.tga",
	Checked = true,
	SendCodeEventOnClick = true
}

-- TODO: Implement controls and save game screens if i need them...

--[[
-- Options Controls Sub Pane container.
opmControlsPaneContainer =
{
	Id = 0,
	Type = "Container",
	Position =
	{
		Type = "relative",
		x = 0.5,
		y = 1.0,
		z = 0.0
	},
	Color =
	{
		r = 0.75,
		g = 1.0,
		b = 0.75,
		a = 0.4
	},
	Dimension =
	{
		Type = "relative",
		Width = 0.5,
		Height = 0.85,
		Depth = 0.0
	},
	--TextureName = "textures/Clouds.tga",
	TextureName = "",
	Visible = false
}

-- Options SaveGame Sub Pane container.
opmSaveGamePaneContainer =
{
	Id = 0,
	Type = "Container",
	Position =
	{
		Type = "relative",
		x = 0.5,
		y = 1.0,
		z = 0.0
	},
	Color =
	{
		r = 0.15,
		g = 1.0,
		b = 0.15,
		a = 0.4
	},
	Dimension =
	{
		Type = "relative",
		Width = 0.5,
		Height = 0.85,
		Depth = 0.0
	},
	--TextureName = "textures/Clouds.tga",
	TextureName = "",
	Visible = false
}

]]

--[[
	*******************************************************************

	Setup screen and event handling from here.

	*******************************************************************
]]


-- Global lua parameters for the MainMenu screen.

local SCRIPT_NAME = "OptionsMenu.lua: ";
local DEBUG_SCRIPT = false;

-- Check if the global debug flag was set in the init script.
if(INIT_DEBUG_MODE ~= nil and INIT_DEBUG_MODE == true) then
	print("Turning on debugging for " .. SCRIPT_NAME);
	DEBUG_SCRIPT = true;
end

-- Current values of the options various ListButtons...
opmCurrentResolutionSelection = nil;
opmCurrentTexureFilterSelection = nil;
opmCurrentFsaaSelection = nil;

opmScreenType = "OptionsMenu";




--[[
	Reset the game option widgets back to their default values according to the GameOptions.

	@param N/A

	@return N/A
]]
local function opmResetGameOptions()
	local FUNCTION_NAME = "opmResetGameOptions(): ";

	local widgetData = {};				-- New data for the options screen widget to display.

	--printDebug(FUNCTION_NAME .. "A");

	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.MasterVolume=" .. tostring(INIT_PLAYER_OPTIONS.MasterVolume));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.Music=" .. tostring(INIT_PLAYER_OPTIONS.Music));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.SoundFx=" .. tostring(INIT_PLAYER_OPTIONS.SoundFx));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.ScreenResolution=" .. tostring(INIT_PLAYER_OPTIONS.ScreenResolution));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.TextureFilteringType=" .. tostring(INIT_PLAYER_OPTIONS.TextureFilteringType));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.Multisampling=" .. tostring(INIT_PLAYER_OPTIONS.Multisampling));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.RenderShadows=" .. tostring(INIT_PLAYER_OPTIONS.RenderShadows));

	-- Reset sound widgets (Volume, Music, Fx)
	widgetData = {SliderPosition = INIT_PLAYER_OPTIONS.MasterVolume, EventTypeId = opmSPVolumeSlider.EventTypeId};
	if (Pool3dMenuView:SetControlData(opmScreenType, opmSPVolumeSlider.Id, widgetData) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to update the MasterVolume sound widget");
	end

	--printDebug(FUNCTION_NAME .. "B");

	widgetData = {Checked = INIT_PLAYER_OPTIONS.Music, EventTypeId = opmSPMusicCheckButton.EventTypeId};
	if (Pool3dMenuView:SetControlData(opmScreenType, opmSPMusicCheckButton.Id, widgetData) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to update the Music CheckBox sound widget");
	end

	--printDebug(FUNCTION_NAME .. "C");

	widgetData = {Checked = INIT_PLAYER_OPTIONS.SoundFx, EventTypeId = opmSPSoundCheckButton.EventTypeId};
	if (Pool3dMenuView:SetControlData(opmScreenType, opmSPSoundCheckButton.Id, widgetData) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to update the SoundFx CheckBox sound widget");
	end

	--printDebug(FUNCTION_NAME .. "D");

	-- Reset graphics widgets (Resolution, Tex Filter, FSAA, Shadows)
	widgetData = {Text = INIT_PLAYER_OPTIONS.ScreenResolution, EventTypeId = opmGPResolutionListButton.EventTypeId};
	if (Pool3dMenuView:SetControlData(opmScreenType, opmGPResolutionListButton.Id, widgetData) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to update the Screen Resolution List graphics widget");
	end

	--printDebug(FUNCTION_NAME .. "E");

	widgetData = {Text = INIT_PLAYER_OPTIONS.TextureFilteringType, EventTypeId = opmGPTexFilterListButton.EventTypeId};
	if (Pool3dMenuView:SetControlData(opmScreenType, opmGPTexFilterListButton.Id, widgetData) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to update the Texture Filter List graphics widget");
	end

	--printDebug(FUNCTION_NAME .. "F");

	widgetData = {Text = INIT_PLAYER_OPTIONS.Multisampling, EventTypeId = opmGPFsaaListButton.EventTypeId};
	if (Pool3dMenuView:SetControlData(opmScreenType, opmGPFsaaListButton.Id, widgetData) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to update the FSAA List graphics widget");
	end

	--printDebug(FUNCTION_NAME .. "G");

	widgetData = {Checked = INIT_PLAYER_OPTIONS.RenderShadows, EventTypeId = opmGPShadowsCheckButton.EventTypeId};
	if (Pool3dMenuView:SetControlData(opmScreenType, opmGPShadowsCheckButton.Id, widgetData) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to update the Shadows Check Box graphics widget");
	end

	--printDebug(FUNCTION_NAME .. "H");
end

--[[
	Sends out the options change events back to the application.

	Also updates the lua table INIT_PLAYER_OPTIONS with the latest
	player options.  The values in this table will be saved in the global
	options file when the application shuts down so that player options
	persist accross application restarts.

	@param N/A

	@return N/A

]]
local function opmBroadcastOptionsSaveEvent()
	local FUNCTION_NAME = "opmBroadcastOptionsSaveEvent(): ";

	printDebug(FUNCTION_NAME .. "On Entry Values:");
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.MasterVolume=" .. tostring(INIT_PLAYER_OPTIONS.MasterVolume));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.Music=" .. tostring(INIT_PLAYER_OPTIONS.Music));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.SoundFx=" .. tostring(INIT_PLAYER_OPTIONS.SoundFx));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.ScreenResolution=" .. tostring(INIT_PLAYER_OPTIONS.ScreenResolution));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.TextureFilteringType=" .. tostring(INIT_PLAYER_OPTIONS.TextureFilteringType));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.Multisampling=" .. tostring(INIT_PLAYER_OPTIONS.Multisampling));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.RenderShadows=" .. tostring(INIT_PLAYER_OPTIONS.RenderShadows));

	-- Create the "graphics_config_change" event:
		-- Width int, Height int, Filtering int, Multisampling int, Shadows b
	local GraphicsConfigEventData = {};


	--printDebug(FUNCTION_NAME .. "A");

	-- Retrieve the current resolution.
	GraphicsConfigEventData.Width = nil;
	GraphicsConfigEventData.Height = nil;

	if (type(opmCurrentResolutionSelection) ~= "string") then
		printDebug("No data");
	else
		printDebug("Got data: " .. opmCurrentResolutionSelection);
	end
	if (type(splitResolutionString) ~= "function") then
		printDebug("No function");
	else
		printDebug("Got function");
	end

	--printDebug(FUNCTION_NAME .. "B: F-" .. splitResolutionString .. " S-" .. opmCurrentResolutionSelection);

	local resTable = splitResolutionString(opmCurrentResolutionSelection);

	--printDebug(FUNCTION_NAME .. "C");

	if (#resTable == 2) then
		--printDebug(FUNCTION_NAME .. "C1");
		-- Use defaults or the present resolution...
		GraphicsConfigEventData.Width = resTable[1];
		GraphicsConfigEventData.Height = resTable[2];
		printDebug("Setting Res to " .. GraphicsConfigEventData.Width .. " " .. GraphicsConfigEventData.Height);
		--printDebug(FUNCTION_NAME .. "C2");

		-- Update PLAYER global Options
		INIT_PLAYER_OPTIONS.ScreenResolution = opmCurrentResolutionSelection;
	end

	--printDebug(FUNCTION_NAME .. "D");

	-- Retrieve the current texture filtering level.
	GraphicsConfigEventData.Filtering = nil;
	for i = 1, #opmGPTexFilterListButton.TextTable, 1 do
		if (opmCurrentTexureFilterSelection == opmGPTexFilterListButton.TextTable[i]) then
			--printDebug(FUNCTION_NAME .. "D" .. i);
			GraphicsConfigEventData.Filtering = i;
			printDebug("Setting Tex Filter to " .. GraphicsConfigEventData.Filtering .. " or " .. opmGPTexFilterListButton.TextTable[i]);
			-- Update PLAYER global Options
			INIT_PLAYER_OPTIONS.TextureFilteringType = opmGPTexFilterListButton.TextTable[i];
			break;
		end
	end

	--printDebug(FUNCTION_NAME .. "E");

	-- Retrieve the current Multisampling level.
	GraphicsConfigEventData.Multisampling = nil;
	local msIndex = string.find(opmCurrentFsaaSelection, "x");
	local fsaaNumberStr = string.sub(opmCurrentFsaaSelection, msIndex+1, -1);
	local fsaaNumber = tonumber(fsaaNumberStr);
	if (fsaaNumber ~= nil) then
		GraphicsConfigEventData.Multisampling = fsaaNumber;
		-- Update PLAYER global Options
		INIT_PLAYER_OPTIONS.Multisampling = opmCurrentFsaaSelection;
		printDebug("Setting FSAA to " .. GraphicsConfigEventData.Multisampling);
	end

	--printDebug(FUNCTION_NAME .. "F");

	-- Retrieve the current render shadows flag.
	GraphicsConfigEventData.Shadows = opmGPShadowsCheckButton.Checked;
	-- Update PLAYER global Options
	INIT_PLAYER_OPTIONS.RenderShadows = opmGPShadowsCheckButton.Checked;
	--printDebug(FUNCTION_NAME .. "F1");
	printDebug("Setting Shadows to " .. tostring(GraphicsConfigEventData.Shadows));

	--printDebug(FUNCTION_NAME .. "G");

	-- Send off the "graphics_config_change" event to the application.
	EventManager:TriggerEvent("graphics_config_change", GraphicsConfigEventData);

	--printDebug(FUNCTION_NAME .. "H");

	-- Create the "sound_config_change" event:
		-- MasterVolume f Music b, Fx b
	SoundConfigEventData = {};

	--printDebug(FUNCTION_NAME .. "I");

	SoundConfigEventData.MasterVolume = opmSPVolumeSlider.SliderPosition;
	-- Update PLAYER global Options
	INIT_PLAYER_OPTIONS.MasterVolume = opmSPVolumeSlider.SliderPosition;
	printDebug("Setting Volume to " .. SoundConfigEventData.MasterVolume);

	--printDebug(FUNCTION_NAME .. "J");

	SoundConfigEventData.Music = opmSPMusicCheckButton.Checked;
	-- Update PLAYER global Options
	INIT_PLAYER_OPTIONS.Music = opmSPMusicCheckButton.Checked;
	printDebug("Setting Music to " .. tostring(SoundConfigEventData.Music));

	--printDebug(FUNCTION_NAME .. "K");

	SoundConfigEventData.Fx = opmSPSoundCheckButton.Checked;
	-- Update PLAYER global Options
	INIT_PLAYER_OPTIONS.SoundFx = opmSPSoundCheckButton.Checked;
	printDebug("Setting Fx to " .. tostring(SoundConfigEventData.Fx));

	--printDebug(FUNCTION_NAME .. "L");

	-- Send off the "sound_config_change" event to the application.
	EventManager:TriggerEvent("sound_config_change", SoundConfigEventData);

	--printDebug(FUNCTION_NAME .. "M");

	printDebug(FUNCTION_NAME .. "On Exit Values:");
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.MasterVolume=" .. tostring(INIT_PLAYER_OPTIONS.MasterVolume));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.Music=" .. tostring(INIT_PLAYER_OPTIONS.Music));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.SoundFx=" .. tostring(INIT_PLAYER_OPTIONS.SoundFx));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.ScreenResolution=" .. tostring(INIT_PLAYER_OPTIONS.ScreenResolution));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.TextureFilteringType=" .. tostring(INIT_PLAYER_OPTIONS.TextureFilteringType));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.Multisampling=" .. tostring(INIT_PLAYER_OPTIONS.Multisampling));
	printDebug(FUNCTION_NAME .. "INIT_PLAYER_OPTIONS.RenderShadows=" .. tostring(INIT_PLAYER_OPTIONS.RenderShadows));
end

--[[
	Performs some checks to see if the global variables and
	application bindings we require exist.

	@param N/A

	@return bool True if the required lua objects are present.
]]
local function opmPreliminarySystemsCheck()
	local FUNCTION_NAME = "opmPreliminarySystemsCheck(): ";

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
	if (opmScreenType == nil) then
		print(FUNCTION_NAME .. "Missing opmScreenType");
		return (false);
	end
	WindowManager:SetLuaVideoResolutionsTable();
	if (wmVideoResolutionTable == nil) then
		print(FUNCTION_NAME .. "Missing wmVideoResolutionTable");
		return (false);
	end
	if (INIT_PLAYER_OPTIONS == nil) then
		print(FUNCTION_NAME .. "Missing INIT_PLAYER_OPTIONS");
		return (false);
	end

	opmCurrentResolutionSelection = INIT_PLAYER_OPTIONS.ScreenResolution;
	opmCurrentTexureFilterSelection = INIT_PLAYER_OPTIONS.TextureFilteringType;
	opmCurrentFsaaSelection = INIT_PLAYER_OPTIONS.Multisampling;

	--[[
	opmCurrentResolutionSelection = wmVideoResolutionTable[1];
	if (opmCurrentResolutionSelection == nil) then
		print(FUNCTION_NAME .. "Missing opmCurrentResolutionSelection");
		return (false);
	end
	]]

	-- Success
	return (true);
end

--[[
	Create and add a widget to an already created container.

	Helper function for opmSetUpMainScreen().

	@param containerId The ID of the container to add the widget to.
	@param widgetData The widget data.

	@return bool True on success, false on failure.
]]
local function AddWidgetToContainer(containerId, widgetData)
	local FUNCTION_NAME = "AddWidgetToContainer(): ";

	-- Check input parameters
	if(type(widgetData) ~= "table" or type(containerId) ~= "number") then
		printDebug(FUNCTION_NAME .. "Invalid type(s) supplied");
		return (false);
	end

	-- Create the widget with the data supplied.
	widgetData.Id = Pool3dMenuView:CreateWidget(widgetData);
	if(widgetData.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to create the widget");
		return (false);
	end

	-- Add the widget to the container.
	result = Pool3dMenuView:AddWidgetToContainer(containerId, widgetData.Id);

	if(result == false) then
		printDebug(FUNCTION_NAME .. "Failed to create the widget");
		return (false);
	end

	return (true);
end

--[[
	Create the OptionsMenu Screen and add all its widgets.

	@param N/A

	@return True|False on success or error.
]]
local function opmSetUpMainScreen()
	local FUNCTION_NAME = "opmSetUpMainScreen(): ";	-- Name of the function for debugging.

	-- Register the Options Screen.
	if (Pool3dMenuView:RegisterScreenType(opmScreenType) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to register the screen type with the View UI manager");
		return (false);
	end
	if (Pool3dMenuView:IsRegistered(opmScreenType) ~= true) then
		printDebug(FUNCTION_NAME .. "Pool3dMenuView reporting that screen was not registered");
		return (false);
	end

	--printDebug("E");

	-- Add the Options container to the screen manager.
	if (Pool3dMenuView:AddScreen(opmScreenType, opmScreenContainer) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to add the OptionsMenu container");
		return (false);
	end

	-- Add all the widgets to the container.

	--printDebug("F");

	-- General Options Buttons (Confirm, Cancel, Graphics, Sound, Controls, SaveGame Buttons)
	opmConfirmButton.Id = Pool3dMenuView:AddWidgetToScreen(opmScreenType, opmConfirmButton);
	if(opmConfirmButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the opmConfirmButton to the container");
		return (false);
	end
	opmCancelButton.Id = Pool3dMenuView:AddWidgetToScreen(opmScreenType, opmCancelButton);
	if(opmCancelButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the opmCancelButton to the container");
		return (false);
	end
	opmGraphicsButton.Id = Pool3dMenuView:AddWidgetToScreen(opmScreenType, opmGraphicsButton);
	if(opmGraphicsButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the opmGraphicsButton to the container");
		return (false);
	end
	opmSoundButton.Id = Pool3dMenuView:AddWidgetToScreen(opmScreenType, opmSoundButton);
	if(opmSoundButton.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the opmSoundButton to the container");
		return (false);
	end
	opmTitleText.Id = Pool3dMenuView:AddWidgetToScreen(opmScreenType, opmTitleText);
	if(opmTitleText.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the opmTitleText to the container");
		return (false);
	end

	-- 3B) Create Graphics Sub Pane and all widgets
	opmGraphicsPaneContainer.Id = Pool3dMenuView:CreateContainer(opmGraphicsPaneContainer);
	if(opmGraphicsPaneContainer.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the opmGraphicsPaneContainer to the container");
		return (false);
	end

	--printDebug("G");

	if(opmGraphicsPaneContainer.Id ~= 0) then
		AddWidgetToContainer(opmGraphicsPaneContainer.Id, opmGPTitleText);

		opmGPResolutionListButton.TextTable = wmVideoResolutionTable;

		AddWidgetToContainer(opmGraphicsPaneContainer.Id, opmGPResolutionText);
		AddWidgetToContainer(opmGraphicsPaneContainer.Id, opmGPResolutionListButton);

		AddWidgetToContainer(opmGraphicsPaneContainer.Id, opmGPTexFilterText);
		AddWidgetToContainer(opmGraphicsPaneContainer.Id, opmGPTexFilterListButton);

		AddWidgetToContainer(opmGraphicsPaneContainer.Id, opmGPFsaaText);
		AddWidgetToContainer(opmGraphicsPaneContainer.Id, opmGPFsaaListButton);

		AddWidgetToContainer(opmGraphicsPaneContainer.Id, opmGPShadowsText);
		AddWidgetToContainer(opmGraphicsPaneContainer.Id, opmGPShadowsCheckButton);
	end

	--printDebug("H");

	-- Show graphics pane.
	if (Pool3dMenuView:AddContainerToScreen(opmScreenType, opmGraphicsPaneContainer.Id) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to add the Graphics sub pane to the screen");
		return (false);
	end
	if (Pool3dMenuView:SetScreenWidgetVisibility(opmScreenType, opmGraphicsPaneContainer.Id, true) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to add the Graphics sub pane to the screen");
		return (false);
	end

	-- 3C) Create Sound Sub Pane and all widgets

	--printDebug("I");

	opmSoundPaneContainer.Id = Pool3dMenuView:CreateContainer(opmSoundPaneContainer);
	if(opmSoundPaneContainer.Id == 0) then
		printDebug(FUNCTION_NAME .. "Failed to add the opmSoundPaneContainer to the container");
		return (false);
	end

	if(opmSoundPaneContainer.Id ~= 0) then
		AddWidgetToContainer(opmSoundPaneContainer.Id, opmSPTitleText);

		opmSPVolumeSlider.SliderPosition = INIT_PLAYER_OPTIONS.MasterVolume;
		AddWidgetToContainer(opmSoundPaneContainer.Id, opmSPVolumeText);
		AddWidgetToContainer(opmSoundPaneContainer.Id, opmSPVolumeSlider);

		opmSPMusicCheckButton.Checked = INIT_PLAYER_OPTIONS.Music;
		AddWidgetToContainer(opmSoundPaneContainer.Id, opmSPMusicText);
		AddWidgetToContainer(opmSoundPaneContainer.Id, opmSPMusicCheckButton);

		opmSPSoundCheckButton.Checked = INIT_PLAYER_OPTIONS.SoundFx;
		AddWidgetToContainer(opmSoundPaneContainer.Id, opmSPSoundText);
		AddWidgetToContainer(opmSoundPaneContainer.Id, opmSPSoundCheckButton);
	end

	if (Pool3dMenuView:AddContainerToScreen(opmScreenType, opmSoundPaneContainer.Id) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to add the Sound sub pane to the screen");
		return (false);
	end
	if (Pool3dMenuView:SetScreenWidgetVisibility(opmScreenType, opmSoundPaneContainer.Id, false) ~= true) then
		printDebug(FUNCTION_NAME .. "Failed to hide the Sound sub pane");
		return (false);
	end

	return (true);
end

--[[
	Event handler function for button press events for the Options screen.

	@param eventData The event data.

	@return void
]]
function opmButtonActionEvent(eventData)
	local FUNCTION_NAME = "opmButtonActionEvent(): ";

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
			printDebug(FUNCTION_NAME .. "ButtonData: " .. buttonIdStr .. " " .. eventIdStr .. " " .. checkedStr);
		end

		-- Confirm button click.
		if (eventData.ButtonId == opmConfirmButton.Id) then
			printDebug(FUNCTION_NAME .. "Confirm Button Click");
			opmBroadcastOptionsSaveEvent();
			opmResetGameOptions();
			Pool3dMenuView:ShowPreviousScreen();

		-- Cancel Button Click.
		elseif (eventData.ButtonId == opmCancelButton.Id) then
			printDebug(FUNCTION_NAME .. "Cancel Button Click");
			opmResetGameOptions();
			Pool3dMenuView:ShowPreviousScreen();

		-- Graphics Pane button click.
		elseif (eventData.ButtonId == opmGraphicsButton.Id) then
			printDebug(FUNCTION_NAME .. "Graphics Button Click");
			local result = Pool3dMenuView:SetScreenWidgetVisibility(opmScreenType, opmSoundPaneContainer.Id, false);
			result = Pool3dMenuView:SetScreenWidgetVisibility(opmScreenType, opmGraphicsPaneContainer.Id, true);

		-- Sound Pane button click.
		elseif (eventData.ButtonId == opmSoundButton.Id) then
			printDebug(FUNCTION_NAME .. "Sound Button Click");
			local result = Pool3dMenuView:SetScreenWidgetVisibility(opmScreenType, opmSoundPaneContainer.Id, true);
			result = Pool3dMenuView:SetScreenWidgetVisibility(opmScreenType, opmGraphicsPaneContainer.Id, false);

		-- Shadows check status change.
		elseif (eventData.ButtonId == opmGPShadowsCheckButton.Id) then
			opmGPShadowsCheckButton.Checked = eventData.Checked;

		-- Music check status change.
		elseif (eventData.ButtonId == opmSPMusicCheckButton.Id) then
			opmSPMusicCheckButton.Checked = eventData.Checked;

		-- SoundFx check status change.
		elseif (eventData.ButtonId == opmSPSoundCheckButton.Id) then
			opmSPSoundCheckButton.Checked = eventData.Checked;

		else
			-- We are not interested in other buttons events!
		end

	end
end

--printDebug("M");

--[[
	Event handler function for slider button action events on the options screen.

	@param eventData The event data.

	@return void
]]
function opmSliderMoveEvent(eventData)
	local FUNCTION_NAME = "opmSliderMoveEvent(): ";

	if (eventData == nil) then
		printDebug(FUNCTION_NAME .. "Got no data");
	elseif (eventData.SliderId == nil or eventData.EventTypeId == nil or eventData.SliderPosition == nil) then
		printDebug(FUNCTION_NAME .. "Invalid data");
	else
		local sliderId = eventData.SliderId;
		local eventId = eventData.EventTypeId;
		local position = eventData.SliderPosition;

		if(INIT_DEBUG_MODE == true) then
			local sliderIdStr = tostring(sliderId);
			local eventIdStr = tostring(eventId);
			local positionStr = tostring(position);
			printDebug(FUNCTION_NAME .. "SliderData: " .. sliderIdStr .. " " .. eventIdStr .. " " .. positionStr);
		end

		-- The music volume slider.
		if (eventData.SliderId == opmSPVolumeSlider.Id) then
			printDebug(FUNCTION_NAME .. "Setting music slider to " .. position);
			opmSPVolumeSlider.SliderPosition = eventData.SliderPosition;
		end
	end
end

--printDebug("N");

--[[
	Event handler function for List button state change events for the Options screen.

	@param eventData The event data.

	@return void
]]
function opmListButtonEvent(eventData)
	local FUNCTION_NAME = "opmListButtonEvent(): ";

	if (eventData == nil) then
		printDebug(FUNCTION_NAME .. "Got no data");
	elseif (eventData.ButtonId == nil or eventData.EventTypeId == nil or eventData.Checked == nil or eventData.Selection == nil) then
		printDebug(FUNCTION_NAME .. "Invalid data");
	else
		local buttonId = eventData.ButtonId;
		local eventId = eventData.EventTypeId;
		local checked = eventData.Checked;
		local selection = eventData.Selection;

		if(INIT_DEBUG_MODE == true) then
			local buttonIdStr = tostring(buttonId);
			local eventIdStr = tostring(eventId);
			local checkedStr = tostring(checked);
			local selectionStr = tostring(selection);
			printDebug(FUNCTION_NAME .. "ListButtonData: " .. buttonIdStr .. " " .. eventIdStr .. " " .. checkedStr .. " " .. selectionStr);
		end

		if (eventData.ButtonId == opmGPResolutionListButton.Id) then
			printDebug(FUNCTION_NAME .. "Current Res Selection: " .. selection);
			opmCurrentResolutionSelection = eventData.Selection;
		elseif (eventData.ButtonId == opmGPTexFilterListButton.Id) then
			printDebug(FUNCTION_NAME .. "Current Tex Selection: " .. selection);
			opmCurrentTexureFilterSelection = eventData.Selection;
		elseif (eventData.ButtonId == opmGPFsaaListButton.Id) then
			printDebug(FUNCTION_NAME .. "Current FSAA Selection: " .. selection);
			opmCurrentFsaaSelection = eventData.Selection;
		else
			-- We are not interested in other buttons events!
		end
	end
end

--[[
	Set up the UI event handlers for the OptionsMenu screen.

	@param N/A

	@return bool True|False on success error.
]]
local function opmSetUpEventHandlers()
	local FUNCTION_NAME = "opmSetUpEventHandlers(): ";	-- Name of the function for debugging.

	EventManager:AddScriptListener("button_action", opmButtonActionEvent);
	EventManager:AddScriptListener("slider_action", opmSliderMoveEvent);
	EventManager:AddScriptListener("list_button_action", opmListButtonEvent);
end

--[[
	Main function.  Creates the NewMenu screen and sets up the event
	handlers for its UI widgets.
]]
local function opmMain()
	if(opmPreliminarySystemsCheck() == true) then
		printDebug("Executing " .. SCRIPT_NAME);

		opmSetUpMainScreen();

		opmSetUpEventHandlers();

		-- Reset option widgets to match player options from the xml file.
		opmResetGameOptions();
	else
		print("Failed to run " .. SCRIPT_NAME);
	end
end

-- Main entry point of SCRIPT_NAME
opmMain();
