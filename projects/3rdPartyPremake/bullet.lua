project "bullet"
	kind "StaticLib"
	language "C++"
	location ("../tmp")
	includedirs { "../../src/3rdParty/bullet-2.80-rev2531/src" }
	files {
		"../../src/3rdParty/bullet-2.80-rev2531/**.h",
		"../../src/3rdParty/bullet-2.80-rev2531/**.c",
		"../../src/3rdParty/bullet-2.80-rev2531/**.cpp"
	}
	excludes {
		"../../src/3rdParty/bullet-2.80-rev2531/src/BulletMultiThreaded/**",
		"../../src/3rdParty/bullet-2.80-rev2531/src/ibmsdk/**",
		"../../src/3rdParty/bullet-2.80-rev2531/src/MiniCL/**",
		"../../src/3rdParty/bullet-2.80-rev2531/src/vectormath/**"
	}
	
	configuration "Debug"
		flags { "FloatStrict", "StaticRuntime", "Symbols" }
		targetdir ("../../libs/Debug")
		objdir ("../../obj/Debug/" .. "bullet")
		defines {
			"DEBUG"
		}
	configuration "Release"
		flags { "FloatFast", "OptimizeSpeed", "StaticRuntime" }
		targetdir ("../../libs/Release")
		objdir ("../../obj/Release/" .. "bullet")
		defines {
			"RELEASE"
		}
	configuration "windows"
		postbuildcommands {
			"mkdir ..\\..\\include\\bullet",
			"mkdir ..\\..\\include\\bullet\\BulletCollision",
			"mkdir ..\\..\\include\\bullet\\BulletCollision\\BroadphaseCollision",
			"mkdir ..\\..\\include\\bullet\\BulletCollision\\CollisionDispatch",
			"mkdir ..\\..\\include\\bullet\\BulletCollision\\CollisionShapes",
			"mkdir ..\\..\\include\\bullet\\BulletCollision\\Gimpact",
			"mkdir ..\\..\\include\\bullet\\BulletCollision\\NarrowPhaseCollision",
			"mkdir ..\\..\\include\\bullet\\BulletDynamics",
			"mkdir ..\\..\\include\\bullet\\BulletDynamics\\Character",
			"mkdir ..\\..\\include\\bullet\\BulletDynamics\\ConstraintSolver",
			"mkdir ..\\..\\include\\bullet\\BulletDynamics\\Dynamics",
			"mkdir ..\\..\\include\\bullet\\BulletDynamics\\Vehicle",
			"mkdir ..\\..\\include\\bullet\\BulletSoftBody",
			"mkdir ..\\..\\include\\bullet\\LinearMath",
			"copy /Y ..\\..\\src\\3rdParty\\bullet-2.80-rev2531\\src\\*.h ..\\..\\include\\bullet",
			"copy /Y ..\\..\\src\\3rdParty\\bullet-2.80-rev2531\\src\\BulletCollision\\*.h ..\\..\\include\\bullet\\BulletCollision",
			"copy /Y ..\\..\\src\\3rdParty\\bullet-2.80-rev2531\\src\\BulletCollision\\BroadphaseCollision\\*.h ..\\..\\include\\bullet\\BulletCollision\\BroadphaseCollision",
			"copy /Y ..\\..\\src\\3rdParty\\bullet-2.80-rev2531\\src\\BulletCollision\\CollisionDispatch\\*.h ..\\..\\include\\bullet\\BulletCollision\\CollisionDispatch",
			"copy /Y ..\\..\\src\\3rdParty\\bullet-2.80-rev2531\\src\\BulletCollision\\CollisionShapes\\*.h ..\\..\\include\\bullet\\BulletCollision\\CollisionShapes",
			"copy /Y ..\\..\\src\\3rdParty\\bullet-2.80-rev2531\\src\\BulletCollision\\Gimpact\\*.h ..\\..\\include\\bullet\\BulletCollision\\Gimpact",
			"copy /Y ..\\..\\src\\3rdParty\\bullet-2.80-rev2531\\src\\BulletCollision\\NarrowPhaseCollision\\*.h ..\\..\\include\\bullet\\BulletCollision\\NarrowPhaseCollision",
			"copy /Y ..\\..\\src\\3rdParty\\bullet-2.80-rev2531\\src\\BulletDynamics\\*.h ..\\..\\include\\bullet\\BulletDynamics",
			"copy /Y ..\\..\\src\\3rdParty\\bullet-2.80-rev2531\\src\\BulletDynamics\\Character\\*.h ..\\..\\include\\bullet\\BulletDynamics\\Character",
			"copy /Y ..\\..\\src\\3rdParty\\bullet-2.80-rev2531\\src\\BulletDynamics\\ConstraintSolver\\*.h ..\\..\\include\\bullet\\BulletDynamics\\ConstraintSolver",
			"copy /Y ..\\..\\src\\3rdParty\\bullet-2.80-rev2531\\src\\BulletDynamics\\Dynamics\\*.h ..\\..\\include\\bullet\\BulletDynamics\\Dynamics",
			"copy /Y ..\\..\\src\\3rdParty\\bullet-2.80-rev2531\\src\\BulletDynamics\\Vehicle\\*.h ..\\..\\include\\bullet\\BulletDynamics\\Vehicle",
			"copy /Y ..\\..\\src\\3rdParty\\bullet-2.80-rev2531\\src\\BulletSoftBody\\*.h ..\\..\\include\\bullet\\BulletSoftBody",
			"copy /Y ..\\..\\src\\3rdParty\\bullet-2.80-rev2531\\src\\LinearMath\\*.h ..\\..\\include\\bullet\\LinearMath"
		}
	configuration "not windows"
		postbuildcommands {
			"[ -d \"../../include/boost\" ] || mkdir ../../include/bullet",
			"[ -d \"../../include/boost/BulletCollision\" ] || mkdir ../../include/bullet/BulletCollision",
			"[ -d \"../../include/boost/BulletCollision/BroadphaseCollision\" ] || mkdir ../../include/bullet/BulletCollision/BroadphaseCollision",
			"[ -d \"../../include/boost/BulletCollision/CollisionDispatch\" ] || mkdir ../../include/bullet/BulletCollision/CollisionDispatch",
			"[ -d \"../../include/boost/BulletCollision/CollisionShapes\" ] || mkdir ../../include/bullet/BulletCollision/CollisionShapes",
			"[ -d \"../../include/boost/BulletCollision/Gimpact\" ] || mkdir ../../include/bullet/BulletCollision/Gimpact",
			"[ -d \"../../include/boost/BulletCollision/NarrowPhaseCollision\" ] || mkdir ../../include/bullet/BulletCollision/NarrowPhaseCollision",
			"[ -d \"../../include/boost/BulletDynamics\" ] || mkdir ../../include/bullet/BulletDynamics",
			"[ -d \"../../include/boost/BulletDynamics/Character\" ] || mkdir ../../include/bullet/BulletDynamics/Character",
			"[ -d \"../../include/boost/BulletDynamics/ConstraintSolver\" ] || mkdir ../../include/bullet/BulletDynamics/ConstraintSolver",
			"[ -d \"../../include/boost/BulletDynamics/Dynamics\" ] || mkdir ../../include/bullet/BulletDynamics/Dynamics",
			"[ -d \"../../include/boost/BulletDynamics/Vehicle\" ] || mkdir ../../include/bullet/BulletDynamics/Vehicle",
			"[ -d \"../../include/boost/BulletSoftBody\" ] || mkdir ../../include/bullet/BulletSoftBody",
			"[ -d \"../../include/boost/LinearMath\" ] || mkdir ../../include/bullet/LinearMath",
			"cp ../../src/3rdParty/bullet-2.80-rev2531/src/*.h ../../include/bullet",
			--"cp ../../src/3rdParty/bullet-2.80-rev2531/src/BulletCollision/*.h ../../include/bullet/BulletCollision",
			"cp ../../src/3rdParty/bullet-2.80-rev2531/src/BulletCollision/BroadphaseCollision/*.h ../../include/bullet/BulletCollision/BroadphaseCollision",
			"cp ../../src/3rdParty/bullet-2.80-rev2531/src/BulletCollision/CollisionDispatch/*.h ../../include/bullet/BulletCollision/CollisionDispatch",
			"cp ../../src/3rdParty/bullet-2.80-rev2531/src/BulletCollision/CollisionShapes/*.h ../../include/bullet/BulletCollision/CollisionShapes",
			"cp ../../src/3rdParty/bullet-2.80-rev2531/src/BulletCollision/Gimpact/*.h ../../include/bullet/BulletCollision/Gimpact",
			"cp ../../src/3rdParty/bullet-2.80-rev2531/src/BulletCollision/NarrowPhaseCollision/*.h ../../include/bullet/BulletCollision/NarrowPhaseCollision",
			--"cp ../../src/3rdParty/bullet-2.80-rev2531/src/BulletDynamics/*.h ../../include/bullet/BulletDynamics",
			"cp ../../src/3rdParty/bullet-2.80-rev2531/src/BulletDynamics/Character/*.h ../../include/bullet/BulletDynamics/Character",
			"cp ../../src/3rdParty/bullet-2.80-rev2531/src/BulletDynamics/ConstraintSolver/*.h ../../include/bullet/BulletDynamics/ConstraintSolver",
			"cp ../../src/3rdParty/bullet-2.80-rev2531/src/BulletDynamics/Dynamics/*.h ../../include/bullet/BulletDynamics/Dynamics",
			"cp ../../src/3rdParty/bullet-2.80-rev2531/src/BulletDynamics/Vehicle/*.h ../../include/bullet/BulletDynamics/Vehicle",
			"cp ../../src/3rdParty/bullet-2.80-rev2531/src/BulletSoftBody/*.h ../../include/bullet/BulletSoftBody",
			"cp ../../src/3rdParty/bullet-2.80-rev2531/src/LinearMath/*.h ../../include/bullet/LinearMath"
		}