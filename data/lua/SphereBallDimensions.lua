--[[
	@file SphereBallDimensions.lua
	@author PJ O Halloran
	@date 18/12/2010

	This file defines the dimensions for all procedullarly generated
	pool balls in the scene.
]]

-- Global pool data, mainly used to create the pool ball mesh commonly used by all other pool ball actors.
SphereBallDimensions =
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
	OnCreateFunc = "",
	OnDestroyFunc = "",
	TextureName = "",
	ShaderName = "",
	Radius = 0.02125,
	Slices = 20,
	Stacks = 10,
	Group = "cue"
};
