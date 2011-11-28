#ifndef __GAME_PLATFORM_H
#define __GAME_PLATFORM_H

// /////////////////////////////////////////////////////////////////
// @file GamePlatform.h
// @author PJ O Halloran
// @date 17/09/2011
//
// Where platform specific types headers are defined and/or included.
//
// /////////////////////////////////////////////////////////////////


#include "GameTypes.h"


// Building on WINDOWS...
#if _WINDOWS || TARGET_OS_WIN32
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX

    #include <windows.h>
    #include <windowsx.h>
    #include <mmsystem.h>
    #include <direct.h>
    #include <crtdbg.h>

    #include <GL/glew.h>
    #ifdef USE_NEW_GLFW
        #include <GL/glfw3.h>
        #define GLFW_FUNC_PRE GLFWAPI
    #else
        #include <GL/glfw.h>
        #define GLFW_FUNC_PRE GLFWCALL
    #endif

    #define STRCPY char *strcpy_s(char *, size_t, const char *);
#else
    // Defining handy mmioFOURCC macro for non WIN32 platforms...
    #ifndef mmioFOURCC
    #define mmioFOURCC( ch0, ch1, ch2, ch3 )				\
    ( (GameHalloran::I64)(GameHalloran::U8)(ch0) | ( (GameHalloran::I64)(GameHalloran::U8)(ch1) << 8 ) |	\
    ( (GameHalloran::I64)(GameHalloran::U8)(ch2) << 16 ) | ( (GameHalloran::I64)(GameHalloran::U8)(ch3) << 24 ) )
    #endif

    #define STRCPY char *strcpy(char *, const char *);

    // BUILDING ON MACOSX
    #if TARGET_OS_MAC

        #ifdef __APPLE__
            #include <TargetConditionals.h>
            #include <GL/glew.h>
            #include <OpenGL/gl.h>
            #include <OpenGL/glext.h>
            #ifdef USE_NEW_GLFW
                #include <GL/glfw3.h>
                #define GLFW_FUNC_PRE GLFWAPI
            #else
                #include <GL/glfw.h>
                #define GLFW_FUNC_PRE GLFWCALL
            #endif
        #endif

    #elif TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

        #ifdef __APPLE__
            #include <TargetConditionals.h>
            #include <OpenGLES/ES2/gl.h>
            #include <OpenGLES/ES2/glext.h>
        #endif

    #elif TARGET_OS_UNIX
        // TODO: ...
    #else
        #error "Platform not yet supported!"
    #endif
#endif

#endif
