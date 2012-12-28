project "ftgl"
	kind "StaticLib"
	language "C++"
	location ("../tmp")
	includedirs {"../../src/3rdParty/ftgl-2.1.3_rc5/src", "../../src/3rdParty/freetype-2.4.10/include"}
	files {
		"../../src/3rdParty/ftgl-2.1.3_rc5/src/**.h",
		"../../src/3rdParty/ftgl-2.1.3_rc5/src/**.cpp"
	}

	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../../libs/Debug")
		objdir ("../../obj/Debug/" .. "ftgl")
		defines {
			"DEBUG",
			"_LIB",
			"FTGL_LIBRARY_STATIC"
		}
	configuration "Release"
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		targetdir ("../../libs/Release")
		objdir ("../../obj/Release/" .. "ftgl")
		defines {
			"RELEASE",
			"_LIB",
			"FTGL_LIBRARY_STATIC"
		}
	configuration "windows"
		files {
			"../../src/3rdParty/ftgl-2.1.3_rc5/msvc/config.h"
		}
		excludes {
			"../../src/3rdParty/ftgl-2.1.3_rc5/src/config.h"
		}
		includedirs {"../../src/3rdParty/ftgl-2.1.3_rc5/msvc"}
		defines {
			"WIN32"
		}
		postbuildcommands {
			"mkdir ..\\..\\include\\FTGL",
			"copy /Y ..\\..\\src\\3rdParty\\ftgl-2.1.3_rc5\\src\\FTGL\\*.h ..\\..\\include\\FTGL"
		}
	configuration "macosx"
		files {
			"../../src/3rdParty/ftgl-2.1.3_rc5/config.h"
		}
	configuration "not windows"
		postbuildcommands {
			"[ -d \"../../include/FTGL\" ] || mkdir ../../include/FTGL",
			"cp ../../src/3rdParty/ftgl-2.1.3_rc5/src/FTGL/*.h ../../include/FTGL"
		}