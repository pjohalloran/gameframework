// /////////////////////////////////////////////////////////////////
// @file GameBase.cpp
// @author PJ O Halloran
// @date 06/07/2010
//
// File contains the implementation of the various utility functions
// and classes.
//
// /////////////////////////////////////////////////////////////////

#include "GameBase.h"
#include "GameMain.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    std::string GetOpenGLError(const GLenum errorCode)
    {
        std::string str;

        switch(errorCode) {
            case GL_INVALID_ENUM: {
                str.assign("GL_INVALID_ENUM: unacceptable value for a enumerated argument");
            }
            break;
            case GL_INVALID_VALUE: {
                str.assign("GL_INVALID_VALUE: numeric argument out of range");
            }
            break;
            case GL_INVALID_OPERATION: {
                str.assign("GL_INVALID_OPERATION: operation is not allowed in the current state");
            }
            break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: {
                str.assign("GL_INVALID_FRAMEBUFFER_OPERATION: framebuffer object is not complete");
            }
            break;
            case GL_OUT_OF_MEMORY: {
                str.assign("GL_OUT_OF_MEMORY: not enough memory left to execute the command");
            }
            break;
            // Pre GL 3.x errors for completeness
            case GL_STACK_OVERFLOW: {
                str.assign("GL_STACK_OVERFLOW: command would cause a stack overflow");
            }
            break;
            case GL_STACK_UNDERFLOW: {
                str.assign("GL_STACK_UNDERFLOW: command would cause a stack underflow");
            }
            break;
            case GL_TABLE_TOO_LARGE: {
                str.assign("GL_TABLE_TOO_LARGE: specified table is too large");
            }
            break;
            case GL_NO_ERROR: {
                str.assign("No OpenGL error occurred");
            }
            break;
            default: {
                str.assign("Unknown OpenGL error occurred");
            }
            break;
        }

        return (str);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool GfCheckGlError()
    {
#if DEBUG
        GLenum err = glGetError();
        if(err != GL_NO_ERROR) {
            GF_LOG_ERR(GetOpenGLError(err));
        }
        return (err == GL_NO_ERROR);
#else
        return (true);
#endif
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool GfCheckGlErrorTrc(const char *msg)
    {
#if DEBUG
        GLenum err = glGetError();
        if(err != GL_NO_ERROR) {
            GF_LOG_ERR(msg + GetOpenGLError(err));
        }
        return (err == GL_NO_ERROR);
#else
        return (true);
#endif
    }

}
