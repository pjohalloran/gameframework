project "jpeg"
	kind "StaticLib"
	language "C"
	location ("../tmp")
	includedirs { "../../src/3rdParty/jpeg-8d/"}
	files {
		"../../src/3rdParty/jpeg-8d/jaricom.c",
		"../../src/3rdParty/jpeg-8d/jcapimin.c",
		"../../src/3rdParty/jpeg-8d/jcapistd.c",
		"../../src/3rdParty/jpeg-8d/jcarith.c",
		"../../src/3rdParty/jpeg-8d/jccoefct.c",
		"../../src/3rdParty/jpeg-8d/jccolor.c",
		"../../src/3rdParty/jpeg-8d/jcdctmgr.c",
		"../../src/3rdParty/jpeg-8d/jchuff.c",
		"../../src/3rdParty/jpeg-8d/jcinit.c",
		"../../src/3rdParty/jpeg-8d/jcmainct.c",
		"../../src/3rdParty/jpeg-8d/jcmarker.c",
		"../../src/3rdParty/jpeg-8d/jcmaster.c",
		"../../src/3rdParty/jpeg-8d/jcomapi.c",
		"../../src/3rdParty/jpeg-8d/jconfig.h", -- TODO: windows, linux, mac version
		"../../src/3rdParty/jpeg-8d/jcparam.c",
		"../../src/3rdParty/jpeg-8d/jcprepct.c",
		"../../src/3rdParty/jpeg-8d/jcsample.c",
		"../../src/3rdParty/jpeg-8d/jctrans.c",
		"../../src/3rdParty/jpeg-8d/jdapimin.c",
		"../../src/3rdParty/jpeg-8d/jdapistd.c",
		"../../src/3rdParty/jpeg-8d/jdarith.c",
		"../../src/3rdParty/jpeg-8d/jdatadst.c",
		"../../src/3rdParty/jpeg-8d/jdatasrc.c",
		"../../src/3rdParty/jpeg-8d/jdcoefct.c",
		"../../src/3rdParty/jpeg-8d/jdcolor.c",
		"../../src/3rdParty/jpeg-8d/jdct.h",
		"../../src/3rdParty/jpeg-8d/jddctmgr.c",
		"../../src/3rdParty/jpeg-8d/jdhuff.c",
		"../../src/3rdParty/jpeg-8d/jdinput.c",
		"../../src/3rdParty/jpeg-8d/jdmainct.c",
		"../../src/3rdParty/jpeg-8d/jdmarker.c",
		"../../src/3rdParty/jpeg-8d/jdmaster.c",
		"../../src/3rdParty/jpeg-8d/jdmerge.c",
		"../../src/3rdParty/jpeg-8d/jdpostct.c",
		"../../src/3rdParty/jpeg-8d/jdsample.c",
		"../../src/3rdParty/jpeg-8d/jdtrans.c",
		"../../src/3rdParty/jpeg-8d/jerror.c",
		"../../src/3rdParty/jpeg-8d/jerror.h",
		"../../src/3rdParty/jpeg-8d/jfdctflt.c",
		"../../src/3rdParty/jpeg-8d/jfdctfst.c",
		"../../src/3rdParty/jpeg-8d/jfdctint.c",
		"../../src/3rdParty/jpeg-8d/jidctflt.c",
		"../../src/3rdParty/jpeg-8d/jidctfst.c",
		"../../src/3rdParty/jpeg-8d/jidctint.c",
		"../../src/3rdParty/jpeg-8d/jinclude.h",
		"../../src/3rdParty/jpeg-8d/jmemmgr.c",
		"../../src/3rdParty/jpeg-8d/jmemnobs.c",
		"../../src/3rdParty/jpeg-8d/jmemsys.h",
		"../../src/3rdParty/jpeg-8d/jmorecfg.h",
		"../../src/3rdParty/jpeg-8d/jpegint.h",
		"../../src/3rdParty/jpeg-8d/jpeglib.h",
		"../../src/3rdParty/jpeg-8d/jquant1.c",
		"../../src/3rdParty/jpeg-8d/jquant2.c",
		"../../src/3rdParty/jpeg-8d/jutils.c",
		"../../src/3rdParty/jpeg-8d/jversion.h",
	}
	
	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../../libs/Debug")
		objdir ("../../obj/Debug/" .. "jpeg")
		defines {
			"DEBUG",
			"_LIB"
		}
	configuration "Release"
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		targetdir ("../../libs/Release")
		objdir ("../../obj/Release/" .. "jpeg")
		defines {
			"NDEBUG",
			"_LIB"
		}
	configuration "windows"
		defines {
			"WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}
		postbuildcommands {
			"mkdir ..\\..\\include\\jpeg",
			"copy /Y ..\\..\\src\\3rdParty\\jpeg-8d\\*.h ..\\..\\include\\jpeg"
		}
	configuration "not windows"
		postbuildcommands {
			"[ -d \"../../include/jpeg\" ] || mkdir ../../include/jpeg",
			"cp ../../src/3rdParty/jpeg-8d/*.h ../../include/jpeg"
		}