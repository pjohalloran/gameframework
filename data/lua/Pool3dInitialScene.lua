--[[
	@file Pool3dInitialScene.lua
	@author PJ O Halloran
	@date 27/11/2010

	This file defines and sets up the initial Pool3d scene.

	Please note all global declarations and functions in this file
	begin with the prefix "p3di".  This is to try and avoid global
	lua name clashes with other scripts.

]]

-- Used for debugging statements.
local SCRIPT_NAME = "Pool3dInitialScene.lua: ";
local DEBUG_SCRIPT = false;

-- Check if the global debug flag was set in the init script.
if(INIT_DEBUG_MODE ~= nil and INIT_DEBUG_MODE == true) then
	print("Turning on debugging for " .. SCRIPT_NAME);
	DEBUG_SCRIPT = true;
end


printDebug("Executing " .. SCRIPT_NAME);

-- Ensure all our required global objects exist so we may setup the scene.
if(DEBUG_SCRIPT == true) then
	assert(GameMain ~= nil, "GameMain global object does not exist");
	assert(WindowManager ~= nil, "WindowManager global object does not exist");
	assert(LuaStateManager ~= nil, "LuaStateManager global object does not exist");
	assert(EventManager ~= nil, "EventManager global object does not exist");
	assert(SceneGraphManager ~= nil, "SceneGraphManager global object does not exist");
end

--print("A");

-- Global pool data, mainly used to create the pool ball mesh commonly used by all other pool ball actors.
p3diGlobaPoolBallData =
{
	-- For ball placing function...
	Radius = 0.0243,
};

--print("B");

-- Define all pool ball actors, cue actor and the table actor here.

-- Balls 1 to 15...
p3diBall01Data =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 0.0,
		B = 0.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/ball01.tga",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "plain"
};

p3diBall02Data =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 0.0,
		B = 0.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/ball02.tga",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "plain"
};

p3diBall03Data =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 0.0,
		B = 0.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/ball03.tga",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "plain"
};

p3diBall04Data =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 0.0,
		B = 0.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/ball04.tga",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "plain"
};

p3diBall05Data =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 0.0,
		B = 0.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/ball05.tga",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "plain"
};

p3diBall06Data =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 0.0,
		B = 0.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/ball06.tga",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "plain"
};

p3diBall07Data =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 0.0,
		B = 0.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/ball07.tga",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "plain"
};

p3diBall08Data =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 0.0,
		B = 0.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/ball08.tga",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "black"
};

p3diBall09Data =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 0.0,
		B = 0.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/ball09.tga",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "stripes"
};

p3diBall10Data =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 0.0,
		B = 0.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/ball10.tga",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "stripes"
};

p3diBall11Data =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 0.0,
		B = 0.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/ball11.tga",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "stripes"
};

p3diBall12Data =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 0.0,
		B = 0.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/ball12.tga",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "stripes"
};

p3diBall13Data =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 0.0,
		B = 0.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/ball13.tga",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "stripes"
};

p3diBall14Data =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 0.0,
		B = 0.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/ball14.tga",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "stripes"
};

p3diBall15Data =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 0.0,
		B = 0.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/ball15.tga",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "stripes"
};

p3diBallCueData =
{
	ActorType = "poolball",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 1.0,
		G = 1.0,
		B = 1.0,
		A = 1.0
	},
	Material = INIT_BALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "",
	ShaderName = "",
	MeshName = "",
	PhysicsInformation = INIT_POOLBALL_PHYSICS_INFORMATION,
	Radius = 0.025,
	Slices = 100,
	Stacks = 50,
	Group = "cue"
};

--print("C");

-- Table

p3diTableData =
{
	ActorType = "pooltable",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0,
		0.0
	},
	Color =
	{
		R = 0.0,
		G = 1.0,
		B = 0.0,
		A = 1.0
	},
	--Material = INIT_TABLE_MATERIAL,
	Material =
	{
		Ambient = INIT_GREEN_COLOR,
		Diffuse =
		{
			r = 0.6,
			g = 1.0,
			b = 0.6,
			a = 1.0
		},
		Specular = INIT_GREEN_COLOR,
		Emissive = INIT_BLACK_COLOR,
		Shininess = 0
	},
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/PoolTableTex.tga",
	ShaderName = "",
	PhysicsInformation = INIT_POOLTABLE_PHYSICS_INFORMATION,
	MeshName = "models/PoolTableMeshGF.obj",
	FrontPanelMeshName = "models/PoolTableFrontPanelGF.obj",
	SidePanelMeshName = "models/PoolTableSidePanelGF.obj",
	PanelTextureName = "textures/wood3.tga",
	PanelMaterial =
	{
		Ambient = INIT_WHITE_COLOR,
		Diffuse = INIT_WHITE_COLOR,
		Specular = INIT_WHITE_COLOR,
		Emissive = INIT_BLACK_COLOR,
		Shininess = 256
	},
	MiddlePocketMeshName = "models/PoolTablePocketMiddleGF.obj",
	CornerPocketMeshName = "models/PoolTableCornerFix.obj",
	PocketMaterial =
	{
		Ambient = INIT_LIGHTGRAY_COLOR,
		Diffuse = INIT_LIGHTGRAY_COLOR,
		Specular = INIT_WHITE_COLOR,
		Emissive = INIT_BLACK_COLOR,
		Shininess = 256
	},
	PocketTexture = "textures/marble.tga"
};

--print("D");

-- Cue

p3diCueData =
{
	ActorType = "poolcue",
	ActorId = 0,
	Pos =
	{
		0.0,
		0.0100,
		0.0
	},
	Color =
	{
		R = 1.0,
		G = 1.0,
		B = 1.0,
		A = 1.0
	},
	Material = INIT_CUE_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/CueTexNew.tga",
	ShaderName = "",
	MeshName = "models/PoolCue.obj"
};

-- Static room scene decorations.

p3diFloorData =
{
	ActorType = "unknown",
	ActorId = 0,
	Pos =
	{
		0.0,
		-0.65,
		0.0
	},
	Color =
	{
		R = 1.0,
		G = 1.0,
		B = 1.0,
		A = 1.0
	},
	Material = INIT_FLOOR_WALL_MATERIAL,
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "textures/wood3.tga",
	ShaderName = "",
	MeshName = "models/floor.obj"
};

--print("E");

-- Define the lights in the scene.

-- Global ambient illumination in the scene.
p3diGlobalAmbientLight =
{
	LightType = "directional",
	Ambient =
	{
		r = 0.5,
		g = 0.5,
		b = 0.5,
		a = 1.0
	},
	On = true
};

--print("F");

-- Overhead light over pool table.
p3diPoolTableLight =
{
	LightType = "positional",
	Position =
	{
		x = 0.0,
		y = 3.0,
		z = 0.0
	},
	Direction =
	{
		x = 0.0,
		y = -1.0,
		z = 0.0
	},
	Ambient =
	{
		r = 0.2,
		g = 0.2,
		b = 0.2,
		a = 1.0
	},
	Diffuse =
	{
		r = 1.0,
		g = 1.0,
		b = 1.0,
		a = 1.0
	},
	Specular =
	{
		r = 1.0,
		g = 1.0,
		b = 1.0,
		a = 1.0
	},
	ConstantAttenuation = 2.0,
	LinearAttenuation = 1.0,
	QuadraticAttenuation = 0.5,
	SpotlightCutoff = 50.0,
	SpotlightExponent = 32.0,
	On = true
};

--print("G");

-- Test spotlight
p3diTestSpotLight =
{
	LightType = "spotlight",
	Position =
	{
		x = 0.0,
		y = 1.0,
		z = 0.0
	},
	Direction =
	{
		x = 0.0,
		y = -1.0,
		z = 0.0
	},
	Ambient =
	{
		r = 0.2,
		g = 0.2,
		b = 0.2,
		a = 1.0
	},
	Diffuse =
	{
		r = 1.0,
		g = 1.0,
		b = 1.0,
		a = 1.0
	},
	Specular =
	{
		r = 1.0,
		g = 1.0,
		b = 1.0,
		a = 1.0
	},
	ConstantAttenuation = 2.0,
	LinearAttenuation = 1.0,
	QuadraticAttenuation = 0.0,
	SpotlightCutoff = 25.0,
	SpotlightExponent = 32.0,
	On = true
};

--print("H");

--[[
	Positions pool balls in their initial positions in the scene depending on
	their dimensions and the dimensions of the table.
	(We assume the tables centre is aligned with the world origin here).

	@param N/A

	@return N/A
]]
function p3diPositionPoolBalls()
	--local tW = p3diTableData.TableWidth;			-- Width of table.
	--local tD = p3diTableData.TableDepth;			-- Height of table.
	local tW = 1.0;			-- Width of table.
	local tD = 2.2;			-- Height of table.
	local bR = p3diGlobaPoolBallData.Radius;		-- Radius of a ball.
	local bC = bR * 2.0;							-- Circumference of a ball.
	local twHalf = tW / 2.0;						-- Half table width.
	local tdHalf= tD / 2.0;							-- Half table depth.
	local currX = 0.0;								-- Current X position.
	local currY = 0.0;								-- Current Y position.
	local currZ = 0.0;								-- Current Z position.
	local spacing = 0.00125;							-- Buffer spacing to keep balls slighly apart (as they cause collisions on startup if they are touching...).

	-- Y position of balls is always the same so that the are lying on the table surface.
	currY = bR;

	-- X and Z positions are different for each ball.
	currX = 0.0;
	-- TableDepth / 2.0 + ball radius - (5% of table depth + ball radius * 2 * 5)
	--currZ = -tdHalf + bR + ((tD / 20.0) + (bC  * 5.0));
	currZ = -tdHalf + bR + ((tD / 10.0) + (bC  * 5.0));

	-- Head of triangle.
	p3diBall01Data.Pos[1] = currX;
	p3diBall01Data.Pos[2] = currY;
	p3diBall01Data.Pos[3] = currZ;

	-- Second row.
	currX = -bR - spacing;
	currZ = currZ - bC - spacing;

	p3diBall09Data.Pos[1] = currX;
	p3diBall09Data.Pos[2] = currY;
	p3diBall09Data.Pos[3] = currZ;

	currX = currX + bC + spacing;

	p3diBall02Data.Pos[1] = currX;
	p3diBall02Data.Pos[2] = currY;
	p3diBall02Data.Pos[3] = currZ;

	-- Third row.
	currX = -bR * 2.0 - spacing;
	currZ = currZ - bC - spacing;

	p3diBall03Data.Pos[1] = currX;
	p3diBall03Data.Pos[2] = currY;
	p3diBall03Data.Pos[3] = currZ;

	currX = currX + bC + spacing;

	p3diBall08Data.Pos[1] = currX;
	p3diBall08Data.Pos[2] = currY;
	p3diBall08Data.Pos[3] = currZ;

	currX = currX + bC + spacing;

	p3diBall10Data.Pos[1] = currX;
	p3diBall10Data.Pos[2] = currY;
	p3diBall10Data.Pos[3] = currZ;

	-- Fourth Row.
	currX = -bR * 3.0 - spacing;
	currZ = currZ - bC - spacing;

	p3diBall11Data.Pos[1] = currX;
	p3diBall11Data.Pos[2] = currY;
	p3diBall11Data.Pos[3] = currZ;

	currX = currX + bC + spacing;

	p3diBall04Data.Pos[1] = currX;
	p3diBall04Data.Pos[2] = currY;
	p3diBall04Data.Pos[3] = currZ;

	currX = currX + bC + spacing;

	p3diBall12Data.Pos[1] = currX;
	p3diBall12Data.Pos[2] = currY;
	p3diBall12Data.Pos[3] = currZ;

	currX = currX + bC + spacing;

	p3diBall05Data.Pos[1] = currX;
	p3diBall05Data.Pos[2] = currY;
	p3diBall05Data.Pos[3] = currZ;

	-- Fifth Row.
	currX = -bR * 4.0 - spacing;
	currZ = currZ - bC - spacing;

	p3diBall06Data.Pos[1] = currX;
	p3diBall06Data.Pos[2] = currY;
	p3diBall06Data.Pos[3] = currZ;

	currX = currX + bC + spacing;

	p3diBall13Data.Pos[1] = currX;
	p3diBall13Data.Pos[2] = currY;
	p3diBall13Data.Pos[3] = currZ;

	currX = currX + bC + spacing;

	p3diBall07Data.Pos[1] = currX;
	p3diBall07Data.Pos[2] = currY;
	p3diBall07Data.Pos[3] = currZ;

	currX = currX + bC + spacing;

	p3diBall14Data.Pos[1] = currX;
	p3diBall14Data.Pos[2] = currY;
	p3diBall14Data.Pos[3] = currZ;

	currX = currX + bC + spacing;

	p3diBall15Data.Pos[1] = currX;
	p3diBall15Data.Pos[2] = currY;
	p3diBall15Data.Pos[3] = currZ;

	currX = 0.0;
	currY = bR;
	--currZ = tdHalf - bR - (tD / 20.0);
	currZ = 0.44;

	-- Cue ball
	p3diBallCueData.Pos[1] = currX;
	p3diBallCueData.Pos[2] = currY;
	p3diBallCueData.Pos[3] = currZ;

	p3diCueData.Pos[1] = 0.0;
	p3diCueData.Pos[2] = -bR + 0.5;
	p3diCueData.Pos[3] = 0.0;

end

--[[
	Gets the starting position of the cue ball.

	@param N/A

	@return LuaObject/LuaTable containing the cue
	balls starting position.
]]
function p3diGetCueBallStartingPosition()
	Pos = {x = 0.0, y = 0.0, z = 0.0};

	if(p3diBallCueData == nil) then
		printDebug("p3diGetCueBallStartingPosition(): No Cue ball data available");
	else
		Pos.x = p3diBallCueData.Pos[1];
		Pos.y = p3diBallCueData.Pos[2];
		Pos.z = p3diBallCueData.Pos[3];
	end

	return (Pos);
end

--print("I");

-- Set the positions for the pool balls
p3diPositionPoolBalls();

--print("J");

--Generate the new actor events to load all the actors defined here into the 3D scene.
EventManager:QueueEvent("request_new_actor", p3diBall01Data);
EventManager:QueueEvent("request_new_actor", p3diBall02Data);
EventManager:QueueEvent("request_new_actor", p3diBall03Data);
EventManager:QueueEvent("request_new_actor", p3diBall04Data);
EventManager:QueueEvent("request_new_actor", p3diBall05Data);
EventManager:QueueEvent("request_new_actor", p3diBall06Data);
EventManager:QueueEvent("request_new_actor", p3diBall07Data);
EventManager:QueueEvent("request_new_actor", p3diBall08Data);
EventManager:QueueEvent("request_new_actor", p3diBall09Data);
EventManager:QueueEvent("request_new_actor", p3diBall10Data);
EventManager:QueueEvent("request_new_actor", p3diBall11Data);
EventManager:QueueEvent("request_new_actor", p3diBall12Data);
EventManager:QueueEvent("request_new_actor", p3diBall13Data);
EventManager:QueueEvent("request_new_actor", p3diBall14Data);
EventManager:QueueEvent("request_new_actor", p3diBall15Data);
EventManager:QueueEvent("request_new_actor", p3diBallCueData);

--print("K");

EventManager:QueueEvent("request_new_actor", p3diTableData);

--print("L");

EventManager:QueueEvent("request_new_actor", p3diCueData);

--print("M");

EventManager:QueueEvent("request_new_actor", p3diFloorData);

-- Set the lights in the scene.
SceneGraphManager:SetGlobalIllumination(p3diGlobalAmbientLight);
SceneGraphManager:AddDynamicLight(p3diPoolTableLight);
SceneGraphManager:AddDynamicLight(p3diTestSpotLight);

print("END");
