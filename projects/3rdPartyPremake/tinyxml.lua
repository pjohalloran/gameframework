project "tinyxml"
	kind "StaticLib"
	language "C++"
	location ("../tmp")
	includedirs { "../../src/3rdParty/tinyxml/"}
	files {
		"../../src/3rdParty/tinyxml/**.h",
		"../../src/3rdParty/tinyxml/**.c",
		"../../src/3rdParty/tinyxml/**.cpp"
	}
	
	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../../libs/Debug")
		objdir ("../../obj/Debug/" .. "tinyxml")
		defines {
			"DEBUG"
		}
	configuration "Release"
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		targetdir ("../../libs/Release")
		objdir ("../../obj/Release/" .. "tinyxml")
		defines {
			"NDEBUG"
		}
	configuration "windows"
		postbuildcommands {
			"mkdir ..\\..\\include\\tinyxml",
			"copy /Y ..\\..\\src\\3rdParty\\tinyxml\\*.h ..\\..\\include\\tinyxml"
		}
	configuration "not windows"
		postbuildcommands {
			"[ -d \"../../include/tinyxml\" ] || mkdir ../../include/tinyxml",
			"cp ../../src/3rdParty/tinyxml/*.h ../../include/tinyxml"
		}