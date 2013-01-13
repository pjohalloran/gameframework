project "freetype-gl"
	kind "StaticLib"
	language "C++"
	location ("../tmp")
	includedirs {
		"../../src/3rdParty/freetype-gl",
		"../../src/3rdParty/freetype-2.4.10/include",
		"../../src/3rdParty/glew-1.8.0/include"
	}
	files {
		"../../src/3rdParty/freetype-gl/edtaa3func.h",
		"../../src/3rdParty/freetype-gl/edtaa3func.c",
		"../../src/3rdParty/freetype-gl/font-manager.h",
		"../../src/3rdParty/freetype-gl/font-manager.c",
		"../../src/3rdParty/freetype-gl/freetype-gl.h",
		"../../src/3rdParty/freetype-gl/markup.h",
		"../../src/3rdParty/freetype-gl/mat4.h",
		"../../src/3rdParty/freetype-gl/mat4.c",
		"../../src/3rdParty/freetype-gl/opengl.h",
		"../../src/3rdParty/freetype-gl/platform.h",
		"../../src/3rdParty/freetype-gl/platform.c",
		"../../src/3rdParty/freetype-gl/shader.h",
		"../../src/3rdParty/freetype-gl/shader.c",
		"../../src/3rdParty/freetype-gl/text-buffer.h",
		"../../src/3rdParty/freetype-gl/text-buffer.c",
		"../../src/3rdParty/freetype-gl/texture-atlas.h",
		"../../src/3rdParty/freetype-gl/texture-atlas.c",
		"../../src/3rdParty/freetype-gl/texture-font.h",
		"../../src/3rdParty/freetype-gl/texture-font.c",
		"../../src/3rdParty/freetype-gl/vec234.h",
		"../../src/3rdParty/freetype-gl/vector.h",
		"../../src/3rdParty/freetype-gl/vector.c",
		"../../src/3rdParty/freetype-gl/vertex-attribute.h",
		"../../src/3rdParty/freetype-gl/vertex-attribute.c",
		"../../src/3rdParty/freetype-gl/vertex-buffer.h",
		"../../src/3rdParty/freetype-gl/vertex-buffer.c"
	}

	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../../libs/Debug")
		objdir ("../../obj/Debug/" .. "freetype-gl")
		defines {
			"DEBUG"
		}
	configuration "Release"
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		targetdir ("../../libs/Release")
		objdir ("../../obj/Release/" .. "freetype-gl")
		defines {
			"NDEBUG"
		}
	configuration "windows"
		defines {
			"WIN32",
			"_WINDOWS"
		}
		postbuildcommands {
			"mkdir ..\\..\\include\\freetype-gl",
			"copy /Y ..\\..\\src\\3rdParty\\freetype-gl\\*.h ..\\..\\include\\freetype-gl"
		}
	configuration "not windows"
		postbuildcommands {
			"[ -d \"../../include/freetype-gl\" ] || mkdir ../../include/freetype-gl",
			"cp ../../src/3rdParty/freetype-gl/*.h ../../include/freetype-gl"
		}