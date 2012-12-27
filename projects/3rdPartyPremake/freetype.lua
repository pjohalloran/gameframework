project "freetype"
	kind "StaticLib"
	language "C"
	location ("../tmp")
	includedirs { "../../src/3rdParty/freetype-2.4.10/include", "../../src/3rdParty/zlib"}
	files {
		"../../src/3rdParty/freetype-2.4.10/include/f2tbuild.h",
		"../../src/3rdParty/freetype-2.4.10/include/config/*.h",
		"../../src/3rdParty/freetype-2.4.10/src/autofit/autofit.c",
		"../../src/3rdParty/freetype-2.4.10/src/bdf/bdf.c",
		"../../src/3rdParty/freetype-2.4.10/src/cff/cff.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftbase.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftbitmap.c",
		"../../src/3rdParty/freetype-2.4.10/src/cache/ftcache.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftdebug.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftfstype.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftgasp.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftglyph.c",
		"../../src/3rdParty/freetype-2.4.10/src/gzip/ftgzip.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftinit.c",
		"../../src/3rdParty/freetype-2.4.10/src/lzw/ftlzw.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftstroke.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftsystem.c",
		"../../src/3rdParty/freetype-2.4.10/src/smooth/smooth.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftbbox.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftgxval.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftlcdfil.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftmm.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftotval.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftpatent.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftpfr.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftsynth.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/fttype1.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftwinfnt.c",
		"../../src/3rdParty/freetype-2.4.10/src/base/ftxf86.c",
		"../../src/3rdParty/freetype-2.4.10/src/pcf/pcf.c",
		"../../src/3rdParty/freetype-2.4.10/src/pfr/pfr.c",
		"../../src/3rdParty/freetype-2.4.10/src/psaux/psaux.c",
		"../../src/3rdParty/freetype-2.4.10/src/pshinter/pshinter.c",
		"../../src/3rdParty/freetype-2.4.10/src/psnames/psmodule.c",
		"../../src/3rdParty/freetype-2.4.10/src/raster/raster.c",
		"../../src/3rdParty/freetype-2.4.10/src/sfnt/sfnt.c",
		"../../src/3rdParty/freetype-2.4.10/src/truetype/truetype.c",
		"../../src/3rdParty/freetype-2.4.10/src/type1/type1.c",
		"../../src/3rdParty/freetype-2.4.10/src/cid/type1cid.c",
		"../../src/3rdParty/freetype-2.4.10/src/winfonts/winfnt.c",
		"../../src/3rdParty/freetype-2.4.10/src/type42/type42.c"
	}
	
	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../../libs/Debug")
		objdir ("../../obj/Debug/" .. "freetype")
		defines {
			"_DEBUG",
			"_LIB",
			"FT_DEBUG_LEVEL_ERROR",
			"FT_DEBUG_LEVEL_TRACE",
			"FT2_BUILD_LIBRARY"
		}
	configuration "Release"
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		targetdir ("../../libs/Release")
		objdir ("../../obj/Release/" .. "freetype")
		defines {
			"RELEASE",
			"_LIB",
			"FT2_BUILD_LIBRARY"
		}
	configuration "windows"
		defines {
			"WIN32",
			"WIN32_LEAN_AND_MEAN",
			"VC_EXTRALEAN",
			"_CRT_SECURE_NO_WARNINGS"
		}
		postbuildcommands {
			"IF NOT EXIST ..\\..\\include\\freetype mkdir ..\\..\\include\\freetype",
			"copy /Y ..\\..\\src\\3rdParty\\freetype-2.4.10\\include\\ft2build.h ..\\..\\include",
			"xcopy /Y /E ..\\..\\src\\3rdParty\\freetype-2.4.10\\include\\freetype ..\\..\\include\\freetype"
		}
	configuration "not windows"
		postbuildcommands {
			"[ -d \"../../include/freetype\" ] || mkdir ../../include/freetype",
			"cp ../../src/3rdParty/freetype-2.4.10/include/ft2build.h ../../include",
			"cp -R ../../src/3rdParty/freetype-2.4.10/include/freetype ../../include"
		}