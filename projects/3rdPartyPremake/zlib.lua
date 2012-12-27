project "zlib"
	kind "StaticLib"
	language "C"
	location ("../tmp")
	includedirs { "../../src/3rdParty/zlib/"}
	files {
		"../../src/3rdParty/zlib/**.h",
		"../../src/3rdParty/zlib/**.c"
	}
	
	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../../libs/Debug")
		objdir ("../../obj/Debug/" .. "zlib")
		defines {
			"DEBUG"
		}
	configuration "Release"
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		targetdir ("../../libs/Release")
		objdir ("../../obj/Release/" .. "zlib")
		defines {
			"RELEASE"
		}
	configuration "windows"
		postbuildcommands {
			"mkdir ..\\..\\include\\zlib",
			"copy /Y ..\\..\\src\\3rdParty\\zlib\\zlib.h ..\\..\\include\\zlib",
			"copy /Y ..\\..\\src\\3rdParty\\zlib\\zconf.h ..\\..\\include\\zlib"
		}
	configuration "not windows"
		postbuildcommands {
			"[ -d \"../../include/zlib\" ] || mkdir ../../include/zlib",
			"cp ../../src/3rdParty/zlib/zlib.h ../../include/zlib",
			"cp ../../src/3rdParty/zlib/zconf.h ../../include/zlib"
		}