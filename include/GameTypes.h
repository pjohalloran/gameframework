#ifndef __GAME_TYPES_H
#define __GAME_TYPES_H

// /////////////////////////////////////////////////////////////////
// @file GameTypes.h
// @author PJ O Halloran
// @date 17/09/2011
//
// Header for framework types.
//
// /////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{
    typedef char I8;
    typedef unsigned char U8;
    typedef short I16;
    typedef unsigned short U16;
    typedef int I32;
    typedef unsigned int U32;
    typedef long I64;
    typedef unsigned long U64;
    typedef float F32;
    typedef double F64;
    
	// Matrix3x3 aray definition (a F32 array as it is used in GLSL shaders only).
	typedef F32 Matrix3x3[9];
    
    // unsigned long is 32 bits or 64 bits depending on the target architecture...
    // However, 3rd party data chunks used in framework (zlib (.zip), .wav) use 4 bytes for DWORD no matter
    //  what the architeture used is...  So we will explicitly define DWORD and WORD types here!
#if defined(__i386__)
    typedef GameHalloran::U64 DWORD;
    typedef GameHalloran::U16 WORD;
#elif defined(__x86_64__)
    typedef GameHalloran::U32 DWORD;
    typedef GameHalloran::U16 WORD;
#endif
}

#endif
