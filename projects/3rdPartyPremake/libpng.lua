project "png"
	kind "StaticLib"
	language "C"
	location ("../tmp")
	links {"zlib"}
	includedirs {"../../src/3rdParty/libpng", "../../include/zlib", "../../src/3rdParty/zlib"}
	files {
		"../../src/3rdParty/libpng/**.h",
		"../../src/3rdParty/libpng/**.c",
	}
	
	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../../libs/Debug")
		objdir ("../../obj/Debug/" .. "png")
		defines {
			"DEBUG"
		}
	configuration "Release"
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		targetdir ("../../libs/Release")
		objdir ("../../obj/Release/" .. "png")
		defines {
			"RELEASE"
		}
	configuration "windows"
		postbuildcommands {
			"mkdir ..\\..\\include\\png",
			"copy /Y ..\\..\\src\\3rdParty\\libpng\\*.h ..\\..\\include\\png"
		}
	configuration "not windows"
		postbuildcommands {
			"[ -d \"../../include/png\" ] || mkdir ../../include/png",
			"cp ../../src/3rdParty/libpng/*.h ../../include/png"
		}