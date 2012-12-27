project "ogg"
	kind "StaticLib"
	language "C"
	location ("../tmp")
	includedirs {"../../src/3rdParty/libogg-1.3.0/include"}
	files {
		"../../src/3rdParty/libogg-1.3.0/**.h",
		"../../src/3rdParty/libogg-1.3.0/**.c"
	}
	
	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../../libs/Debug")
		objdir ("../../obj/Debug/" .. "ogg")
		defines {
			"_DEBUG"
		}
	configuration "Release"
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		targetdir ("../../libs/Release")
		objdir ("../../obj/Release/" .. "ogg")
		defines {
			"RELEASE"
		}
	configuration "windows"
		defines {
			"WIN32",
			"_WINDOWS",
			"_USRDLL",
			"LIBOGG_EXPORTS"
		}
		files {
			"../../src/3rdParty/libogg-1.3.0/win32/ogg.def"
		}
		postbuildcommands {
			"mkdir ..\\..\\include\\ogg",
			"copy /Y ..\\..\\src\\3rdParty\\libogg-1.3.0\\include\\ogg\\*.h ..\\..\\include\\ogg"
		}
	configuration "not windows"
		postbuildcommands {
			"[ -d \"../../include/ogg\" ] || mkdir ../../include/ogg",
			"cp ../../src/3rdParty/libogg-1.3.0/include/ogg/*.h ../../include/ogg"
		}