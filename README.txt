 		**** gameframework ****

Introduction:

This is my personal educational toolbox for learning how to make 2D and 3D games
primarily using C++ and OpenGL (3.3 only right now).

I created a 3D game called Pool3D using this framework on the Windows and Mac OSX platform.
See: http://www.youtube.com/watch?v=3gPpkircnBg

Features:

 - C++ game engine.
 - OpenGL 3.3 support (3.2 on Mac OSX).
 - A SceneGraph API with an Ambient/Diffuse/Specular shader framework with multiple point, directional and spotlights support.
 - Delayed GLSL uniform framework to minimize GL state changes. 
 - Bullet physics integration.
 - LuaPlus support for scripting game logic.
 - Simple 2D GUI library.
 - Wavefront .obj 3D model loading support.
 - Texture atlas generation.
 - Very useful and simple 3D math library of my own creation.
 - WIN32 and MacOSX support.
 - OpenAL support.
 
 TODO:
 
 - Get rid of dead and experimental code.
 - iOS platform support.
 - More 2D/3D game examples.
 - Custom memory management and allocation.
 - Removal of exceptions.
 - Replace STL with custom data structures and algorithms.
 - Implement shadows in the Pool3D.

Dependencies:
You may need to install some dependancies depending on your platform.

- All:
 * premake4
	http://industriousone.com/premake/download

- Windows:
* boost v1.51
	http://www.boostpro.com/download/

* OpenAL SDK v1.1
	https://bitbucket.org/pjohalloran/openalwin/downloads/OpenAL11CoreSDK.zipOpenAL11CoreSDK.zip

- Mac OSX:
	(dependancies are included in project)

Building:

To build the project we first must generate the project files.
Currently Visual Studio for Windows and Xcode for Mac OSX 
are supported via the premake project generation command line tool.

Get the latest source from GitHub:
git clone https://github.com/pjohalloran/gameframework.git

 - MacOSX:
Generate an xcode project by:
 * cd $PROJECT_ROOT\projects
 * premake4 xcode3

Then open the Pool3D project inside in projects/tmp and build and run.

 - Windows:
Generate a Visual Studio project (e.g. 2010) by:
 * cd $PROJECT_ROOT/project
 * premake4 vs2010

Then open the gameframework solution file in projects/ and build and run.

- Pool3D

To run the Windows version of Pool3D, you will need to generate the games resources manually.
Open explorer and navigate to src/Pool3d/data. Select all the directories inside and right click.
Select "Send to" and then "compressed file".  Rename this zip to be "Pool3D.zip" and copy it
into $PROJECT_ROOT/data/Pool3D/

Credits:

This library contains code from the following projects and books:

- Bullet
- zlib
- libpng
- libogg, libvorbis
- OpenAL
- boost
- glew
- glfw
- ftgl
- lua and LuaPlus

- Game Coding Complete (3rd Edition) http://www.mcshaffry.com/GameCode/
- OpenGL Superbible (5th Edition) http://www.starstonesoftware.com/OpenGL/

- It also uses premake to generate and mantain the project files.

License:

This library is free to use for anyone that wants it under the MIT license.
Please see the 3rd party libraries themselves for their individual licences.
