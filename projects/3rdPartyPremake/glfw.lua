project "glfw"
	kind "StaticLib"
	language "C"
	location ("../tmp")
	includedirs { "../../src/3rdParty/glfw/include", "../../src/3rdParty/glfw/lib" }
	if os.is("windows") then
		files {
			"../../src/3rdParty/glfw/**.h",
			"../../src/3rdParty/glfw/**.c"
		}
		excludes {
			"../../src/3rdParty/glfw/lib/cocoa/**",
			"../../src/3rdParty/glfw/lib/x11/**"
		}
	elseif os.is("macosx") then
		files {
			"../../src/3rdParty/glfw/**.h",
			"../../src/3rdParty/glfw/**.c",
			"../../src/3rdParty/glfw/**.m"
		}
		excludes {
			"../../src/3rdParty/glfw/lib/win32/**",
			"../../src/3rdParty/glfw/lib/x11/**"
		}
	elseif os.is("linux") then
		files {
			"../../src/3rdParty/glfw/**.h",
			"../../src/3rdParty/glfw/**.c"
		}
		excludes {
			"../../src/3rdParty/glfw/lib/cocoa/**",
			"../../src/3rdParty/glfw/lib/win32/**"
		}
	else
		print("OS " .. os.getversion().description .. " is not supported yet!")
	end
	
	os.mkdir("../../include/GL")
	
	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../../libs/Debug")
		objdir ("../../obj/Debug/" .. "glfw")
		defines {
			"DEBUG"
		}
	configuration "Release"
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		targetdir ("../../libs/Release")
		objdir ("../../obj/Release/" .. "glfw")
		defines {
			"RELEASE"
		}
	configuration "windows"
		postbuildcommands {
			"IF NOT EXIST ..\\..\\include\\GL mkdir ..\\..\\include\\GL",
			"copy /Y ..\\..\\src\\3rdParty\\glfw\\include\\GL\\*.h ..\\..\\include\\GL"
		}
		includedirs { "../../src/3rdParty/glfw/lib/win32" }
	configuration "macosx"
		includedirs { "../../src/3rdParty/glfw/lib/macosx" }
	configuration "linux"
		includedirs { "../../src/3rdParty/glfw/lib/x11" }
	configuration "not windows"
		postbuildcommands {
			"[ -d \"../../include/GL\" ] || mkdir ../../include/GL",
			"cp ../../src/3rdParty/glfw/include/GL/*.h ../../include/GL"
		}