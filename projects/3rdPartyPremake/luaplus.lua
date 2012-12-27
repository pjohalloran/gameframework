project "luaplus51"
	kind "StaticLib"
	language "C++"
	location ("../tmp")
	includedirs { "../../src/3rdParty/luaplus51/"}
	files {
		"../../src/3rdParty/luaplus51/Src/**.h",
		"../../src/3rdParty/luaplus51/Src/**.c",
		"../../src/3rdParty/luaplus51/Src/**.cpp",
		"../../src/3rdParty/luaplus51/Src/**.inl"
	}
	excludes {
		"../../src/3rdParty/luaplus51/Src/LuaPlus/LuaCall.inl",
		"../../src/3rdParty/luaplus51/Src/LuaPlus/luace.*",
		"../../src/3rdParty/luaplus51/Src/LuaPlus/LuaPlusLibs.cpp",
		"../../src/3rdParty/luaplus51/Src/LuaPlus/LuaPlusAll.cpp",
		"../../src/3rdParty/luaplus51/Src/LuaPlus/LuaStackTableIterator.inl",
		"../../src/3rdParty/luaplus51/Src/LuaPlus/luathread.*",
		"../../src/3rdParty/luaplus51/Src/LuaPlus/popen.c",
		"../../src/3rdParty/luaplus51/Src/LuaPlus/pt.*",
		"../../src/3rdParty/luaplus51/Src/LuaPlus/srm.*",
		"../../src/3rdParty/luaplus51/Src/LuaPlus/src/luac.c",
		--"../../src/3rdParty/luaplus51/Src/LuaPlus/src/lua.c",
	}
	
	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../../libs/Debug")
		objdir ("../../obj/Debug/" .. "luaplus51")
		defines {
			"DEBUG",
			"LUAPLUS_ENABLE_INLINES"
		}
	configuration "Release"
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		targetdir ("../../libs/Release")
		objdir ("../../obj/Release/" .. "luaplus51")
		defines {
			"RELEASE",
			"LUAPLUS_ENABLE_INLINES"
		}
	configuration "windows"
		postbuildcommands {
			"mkdir ..\\..\\include\\LuaPlus",
			"xcopy /Y /E ..\\..\\src\\3rdParty\\luaplus51\\Src\\LuaPlus ..\\..\\include\\LuaPlus"
		}
	configuration "not windows"
		postbuildcommands {
			"[ -d \"../../include/LuaPlus\" ] || mkdir ../../include/LuaPlus",
			"cp -R ../../src/3rdParty/luaplus51/Src/LuaPlus ../../include"
		}