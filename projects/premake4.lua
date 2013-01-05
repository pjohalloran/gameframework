-- Config variables for the Windows build, you wont have to change these unless you installed OpenAL/boost to a different location
-- on your system.  The windows SDK and the DirectX SDK should already be in your path if your using Visual Studio.  If you are
-- using an environment other than VS then you may have to tweak this file...
if os.is("windows") then
	OPENAL_INCLUDE_DIR="C:/Program Files (x86)/OpenAL 1.1 SDK/include/"
	OPENAL_LIB_DIR="C:/Program Files (x86)/OpenAL 1.1 SDK/libs/WIN32/"
	BOOST_LIB_DIR="C:/Program Files/boost/boost_1_51/lib"
end

solution "gameframework"
  configurations { "Debug", "Release" }

project "gameframework"
	kind "StaticLib"
	language "C++"
	links { "zlib", "tinyxml", "bullet", "png", "jpeg", "luaplus51", "ogg", "vorbis", "glew", "glfw", "freetype", "ftgl" }
	location ("tmp")
	includedirs { "../include", "../include/bullet", "../src/3rdParty" }
	files {
		"../src/**.h",
		"../src/**.cpp"
	}
	excludes {
		"../src/3rdParty/**",
		"../src/GLSLCompiler/**",
		"../src/build/**",
		"../src/Pool3d/**",
		"../src/unittests/**",
		"../src/sound/DirectSoundAudio.h",
		"../src/sound/DirectSoundAudio.cpp"
	}
	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../libs/Debug")
		objdir ("../obj/Debug/" .. "gameframework")
		defines {
			"DEBUG"
		}
	configuration "Release"
		defines {
			"RELEASE",
			"NDEBUG"
		}
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		targetdir ("../libs/Release")
		objdir ("../obj/Release/" .. "gameframework")
	
	configuration "windows"
		defines {
			"WIN32",
			"_WINDOWS",
			"WIN32_LEAN_AND_MEAN",
			"NOMINMAX",
			"GLEW_STATIC",
			"FTGL_LIBRARY_STATIC"
		}
		includedirs { OPENAL_INCLUDE_DIR }
		prebuildcommands {
			"copy /Y ..\\..\\src\\datastructures\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\eventmanager\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\exceptions\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\fileio\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\gamebase\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\graphics\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\gui\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\input\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\logging\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\logic\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\math\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\memory\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\physics\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\resourcecache\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\scripting\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\sound\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\timer\\*.h ..\\..\\include",
			"copy /Y ..\\..\\src\\views\\*.h ..\\..\\include"
		}
	
	configuration "not windows"
		prebuildcommands {
			"cp ../../src/datastructures/*.h ../../include",
			"cp ../../src/eventmanager/*.h ../../include",
			"cp ../../src/exceptions/*.h ../../include",
			"cp ../../src/fileio/*.h ../../include",
			"cp ../../src/gamebase/*.h ../../include",
			"cp ../../src/graphics/*.h ../../include",
			"cp ../../src/gui/*.h ../../include",
			"cp ../../src/input/*.h ../../include",
			"cp ../../src/logging/*.h ../../include",
			"cp ../../src/logic/*.h ../../include",
			"cp ../../src/math/*.h ../../include",
			"cp ../../src/memory/*.h ../../include",
			"cp ../../src/physics/*.h ../../include",
			"cp ../../src/resourcecache/*.h ../../include",
			"cp ../../src/scripting/*.h ../../include",
			"cp ../../src/sound/*.h ../../include",
			"cp ../../src/timer/*.h ../../include",
			"cp ../../src/views/*.h ../../include"
		}
	
	configuration "macosx"
		defines {
			"TARGET_OS_MAC"
		}

project "Pool3d"
	kind "ConsoleApp"
	language "C++"
	location ("tmp")
	includedirs { "../include", "../include/bullet", "../src/3rdParty" }
	targetdir ("../bin")
	links { "zlib", "tinyxml", "bullet", "png", "jpeg", "luaplus51", "ogg", "vorbis", "glew", "glfw", "freetype", "ftgl", "gameframework" }
	files {
		"../src/Pool3d/**.h",
		"../src/Pool3d/**.cpp"
	}
	excludes {
		"../src/data/**",
		"../src/lua/**"
	}
	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		objdir ("../obj/Debug/" .. "Pool3d")
		defines {
			"DEBUG"
		}
		libdirs { "../libs/Debug" }
	configuration "Release"
		defines {
			"RELEASE",
			"NDEBUG"
		}
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		objdir ("../obj/Release/" .. "Pool3d")
		libdirs { "../libs/Release" }
	
	configuration "windows"
		defines {
			"WIN32",
			"_WINDOWS",
			"WIN32_LEAN_AND_MEAN",
			"NOMINMAX",
			"GLEW_STATIC",
			"FTGL_LIBRARY_STATIC"
		}
		includedirs { OPENAL_INCLUDE_DIR }
		libdirs { OPENAL_LIB_DIR, BOOST_LIB_DIR }
		links { "opengl32", "glu32", "dsound", "OpenAL32" }
	configuration { "windows", "Debug" }
		links { "libboost_filesystem-vc100-mt-sgd-1_51", "libboost_system-vc100-mt-sgd-1_51" }
		linkoptions { "/NODEFAULTLIB:\"libcmtd.lib\"" }
	configuration { "windows", "Release" }
		links { "libboost_filesystem-vc100-mt-s-1_51", "libboost_system-vc100-mt-s-1_51" }
		linkoptions { "/NODEFAULTLIB:\"libcmt.lib\"" }
	configuration "macosx"
		defines {
			"TARGET_OS_MAC"
		}

		links { "boost_filesystem-mt", "boost_system-mt", "OpenGL.framework", "OpenAL.framework", "CoreFoundation.framework", "IOKit.framework", "AppKit.framework" }
		postbuildcommands {
			". ../../src/build/macosx/BuildResources.sh ../../src/Pool3d/data/ ../../../data/Pool3D/Pool3D.zip"
		}
		
local ThirdPartyMakeScripts = {
	--"3rdPartyPremake/boost.lua",
	"3rdPartyPremake/zlib.lua",
	"3rdPartyPremake/bullet.lua",
	"3rdPartyPremake/glew.lua",
	"3rdPartyPremake/glfw.lua",
	"3rdPartyPremake/freetype.lua",
	"3rdPartyPremake/ftgl.lua",
	"3rdPartyPremake/libjpeg.lua",
	"3rdPartyPremake/libogg.lua",
	"3rdPartyPremake/libpng.lua",
	"3rdPartyPremake/libvorbis.lua",
	"3rdPartyPremake/luaplus.lua",
	"3rdPartyPremake/tinyxml.lua",
	--"3rdPartyPremake/openal.lua",
};
		
local i = 1;
local size = #ThirdPartyMakeScripts;
-- Execute all the 3rdParty scripts defined in "ThirdPartyMakeScripts".
while i <= size do
	print("Executing " .. ThirdPartyMakeScripts[i]);
	if(ThirdPartyMakeScripts[i] == nil) then
		print(ThirdPartyMakeScripts[i] .. " is nil");
	elseif(dofile(ThirdPartyMakeScripts[i]) == false) then
		print("Failed to execute " .. ThirdPartyMakeScripts[i]);
	end
	i = i + 1;
end