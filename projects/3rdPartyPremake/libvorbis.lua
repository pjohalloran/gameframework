project "vorbis"
	kind "StaticLib"
	language "C"
	location ("../tmp")
	includedirs {"../../src/3rdParty/libvorbis-1.3.3/include", "../../src/3rdParty/libvorbis-1.3.3/lib", "../../src/3rdParty/libogg-1.3.0/include"}
	
	files {
		"../../src/3rdParty/libvorbis-1.3.3/include/**.h",
		"../../src/3rdParty/libvorbis-1.3.3/lib/**.h",
		"../../src/3rdParty/libvorbis-1.3.3/lib/**.c",
	}
	excludes {
		"../../src/3rdParty/libvorbis-1.3.3/lib/barkmel.c",
		"../../src/3rdParty/libvorbis-1.3.3/lib/psytune.c",
		"../../src/3rdParty/libvorbis-1.3.3/lib/tone.c",
		"../../src/3rdParty/libvorbis-1.3.3/lib/books/coupled\res_books_51.h",
		"../../src/3rdParty/libvorbis-1.3.3/lib/modes/residue_44p51.h",
		"../../src/3rdParty/libvorbis-1.3.3/lib/modes/setup_44p51.h"
	}
	
	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../../libs/Debug")
		objdir ("../../obj/Debug/" .. "vorbis")
		defines {
			"_DEBUG"
		}
	configuration "Release"
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		targetdir ("../../libs/Release")
		objdir ("../../obj/Release/" .. "vorbis")
		defines {
			"RELEASE"
		}
	configuration "windows"
		defines {
			"WIN32",
			"_WINDOWS",
			"_USRDLL",
			"LIBVORBIS_EXPORTS"
		}
		postbuildcommands {
			"mkdir ..\\..\\include\\vorbis",
			"copy /Y ..\\..\\src\\3rdParty\\libvorbis-1.3.3\\include\\vorbis\\*.h ..\\..\\include\\vorbis"
		}
	configuration "not windows"
		postbuildcommands {
			"[ -d \"../../include/vorbis\" ] || mkdir ../../include/vorbis",
			"cp ../../src/3rdParty/libvorbis-1.3.3/include/vorbis/*.h ../../include/vorbis"
		}