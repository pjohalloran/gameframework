 		**** gameframework ****

Introduction:

This is my personal educational toolbox for learning how to make 2D and 3D games
primarily using C++ and OpenGL (3.3 only right now).

It started off as a 2D game engine in 2007 using C++ and SDL.  I used it to create very
simple and unremarkable 2D games back then.  A very small chunk of code from back then still exists in the codebase today.
 
In 2010, I decided to get into professional games development and most of the work on it has been done in the past 2 years.  I learned from Game Coding Complete 3rd edition by Mike Mcshaffry et al. and use alot of code from that library.  In fact you could call this a multiplatform port of *some* parts of the GCC3 code using OpenGL and GLFW instead of DirectX and WIN32.

I created a 3D game called Pool3D using this framework on the Windows platform. See:
http://www.youtube.com/watch?v=3gPpkircnBg

In 2011, I ported the framework and Pool3D game to the Mac OSX platform.

Right now, Im doing a big cleanup of the library removing dead code and experimental stuff that did not work out as well as as much GCC3 code as I can.  Also I'm porting to the iOS platform and learning new 3D graphics techniques as I go.


Features:

 - C++ game engine.
 - OpenGL 3.3 support.
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
 
 - Fix the Windows build.
 - Inline all 3rdParty code inside the library to remove the need to install anything.
 - Get rid of dead and experimental code.
 - iOS platform support.
 - More 2D/3D game examples.
 - Custom memory management and allocation.
 - Removal of exceptions.
 - Replace STL with custom data structures and algorithms.
 - Fix the Windows build.
 - Implement shadows in the Pool3D 


Installation:

 - Mac OSX:
On Mac OSX, it is necessary to install certain 3rd party libs in order to build and run the framework and its examples.  

Firstly, you will need to have macports installed.  See http://www.macports.org/

Go into src/build/macosx/ and run the script 3rdPartyInstall.sh at the command line.  It will take a while to run.

 - Windows:
The windows build is currently broken due to my lack of a windows machine!


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


License:


This library is free to use for anyone that wants it under the MIT license.