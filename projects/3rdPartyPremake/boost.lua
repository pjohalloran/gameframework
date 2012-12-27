project "boost-filesystem"
	kind "StaticLib"
	language "C"
	location ("../tmp")
	includedirs { "../../src/3rdParty/boost_1_52_0/"}
	files {
		--"../../src/3rdParty/boost_1_52_0/boost/filesystem.hpp",
		--"../../src/3rdParty/boost_1_52_0/boost/filesystem/**.h",
		--"../../src/3rdParty/boost_1_52_0/boost/filesystem/**.hpp",
		"../../src/3rdParty/boost_1_52_0/libs/filesystem/src/**.cpp"
	}
	
	configuration "Debug"
		flags { "EnableSSE2", "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../../libs/Debug")
		objdir ("../../obj/Debug/" .. "boost-filesystem")
		defines {
			"DEBUG"
		}
	configuration "Release"
		targetdir ("../../libs/Release")
		objdir ("../../obj/Release" .. "boost-filesystem")
		defines {
			"RELEASE"
		}
	configuration "windows"
		postbuildcommands {
			"mkdir ..\\..\\include\\boost\\filesystem",
			"copy /Y ..\\..\\src\\3rdParty\\boost_1_52_0\\boost\\filesystem.hpp ..\\..\\include\\boost\\filesystem",
			"xcopy /Y ..\\..\\src\\3rdParty\\boost_1_52_0\\boost\\filesystem ..\\..\\include\\boost\\filesystem"
		}
	configuration "not windows"
		postbuildcommands {
			"[ -d \"../../include/boost\" ] || mkdir ../../include/boost",
			"cp ../../src/3rdParty/boost_1_52_0/boost/filesystem.hpp ../../include/boost",
			"cp -R ../../src/3rdParty/boost_1_52_0/boost/filesystem ../../include/boost"
		}