project "glew"
	kind "StaticLib"
	language "C"
	location ("../tmp")
	includedirs { "../../src/3rdParty/glew-1.8.0/include"}
	files {
		"../../src/3rdParty/glew-1.8.0/include/**.h",
		"../../src/3rdParty/glew-1.8.0/src/glew.c"
	}
	
	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../../libs/Debug")
		objdir ("../../obj/Debug/" .. "glew")
		defines {
			"DEBUG",
			"_LIB",
			"GLEW_STATIC"
		}
	configuration "Release"
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		targetdir ("../../libs/Release")
		objdir ("../../obj/Release/" .. "glew")
		defines {
			"RELEASE",
			"_LIB",
			"GLEW_STATIC"
		}
	configuration "windows"
		defines {
			"WIN32",
			"WIN32_LEAN_AND_MEAN",
			"VC_EXTRALEAN"
		}
		postbuildcommands {
			"IF NOT EXIST ..\\..\\include\\GL mkdir ..\\..\\include\\GL",
			"copy /Y ..\\..\\src\\3rdParty\\glew-1.8.0\\include\\GL\\*.h ..\\..\\include\\GL"
		}
	configuration "not windows"
		postbuildcommands {
			"[ -d \"../../include/GL\" ] || mkdir ../../include/GL",
			"cp ../../src/3rdParty/glew-1.8.0/include/GL/*.h ../../include/GL"
		}