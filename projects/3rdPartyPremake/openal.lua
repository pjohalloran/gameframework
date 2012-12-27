project "openal"
	kind "StaticLib"
	language "C"
	location ("../tmp")
	includedirs {"../../src/3rdParty/openal-soft-1.15/OpenAL32/Include", "../../src/3rdParty/openal-soft-1.15/include"}
	files {
		"../../src/3rdParty/openal-soft-1.15/OpenAL32/**.h",
		"../../src/3rdParty/openal-soft-1.15/include/**.h",
		"../../src/3rdParty/openal-soft-1.15/OpenAL32/**.c",
		"../../src/3rdParty/openal-soft-1.15/Alc/**.h",
		"../../src/3rdParty/openal-soft-1.15/Alc/**.c",
	}
	excludes {
		"../../src/3rdParty/openal-soft-1.15/Alc/mixer_inc.c",
		"../../src/3rdParty/openal-soft-1.15/Alc/mixer_neon.c",
		"../../src/3rdParty/openal-soft-1.15/Alc/backends/alsa.c",
		"../../src/3rdParty/openal-soft-1.15/Alc/backends/coreaudio.c",
		"../../src/3rdParty/openal-soft-1.15/Alc/backends/opensl.c",
		"../../src/3rdParty/openal-soft-1.15/Alc/backends/oss.c",
		"../../src/3rdParty/openal-soft-1.15/Alc/backends/portaudio.c",
		"../../src/3rdParty/openal-soft-1.15/Alc/backends/pulseaudio.c",
		"../../src/3rdParty/openal-soft-1.15/Alc/backends/sndio.c",
		"../../src/3rdParty/openal-soft-1.15/Alc/backends/solaris.c"
	}
	defines {
		"AL_ALEXT_PROTOTYPES",
		"AL_BUILD_LIBRARY"
	}
	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../../libs/Debug")
		objdir ("../../obj/Debug/" .. "openal")
		defines {
			"_DEBUG"
		}
	configuration "Release"
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		targetdir ("../../libs/Release")
		objdir ("../../obj/Release/" .. "openal")
		defines {
			"RELEASE",
			"NDEBUG"
		}
	configuration "windows"
		-- Include dir for auto generated openal config.h, you may need to tweak or regenerate
		-- this header to get openal to build on your Windows machine.
		-- I generated this with cmake on a Windows 7 64 bit machine with the Windows 7A SDK installed
		includedirs { "../../src/3rdParty/openal-soft-1.15/build/windows" }
		files { "../../src/3rdParty/openal-soft-1.15/build/windows/config.h" }
		defines {
			"_WIN32",
			"WIN32",
			"_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS",
			"_CRT_NONSTDC_NO_DEPRECATE"
		}
		postbuildcommands {
			"mkdir ..\\..\\include\\AL",
			"copy /Y ..\\..\\src\\3rdParty\\openal-soft-1.15\\include\\AL\\*.h ..\\..\\include\\AL"
		}
	configuration "not windows"
		postbuildcommands {
			"mkdir ../../include/AL",
			"cp ../../src/3rdParty/openal-soft-1.15/include/*.h ../../include/AL"
		}