// /////////////////////////////////////////////////////////////////
// @file ShaderUniform.cpp
// @author PJ O Halloran
// @date 26/11/2011
//
// Contains ShaderUniform class impl.
//
// /////////////////////////////////////////////////////////////////


#include "ShaderUniform.h"
#include "GameBase.h"

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ShaderUniform::VClean()
    {
        GF_CLEAR_GL_ERROR();
        
        switch(m_type)
        {
            case eInt:
            {
                glUniform1i(m_location, m_value.m_intArr[0]);
                break;
            }
            case eFloat:
            {
                glUniform1f(m_location, m_value.m_floatArr[0]);
                break;
            }
            case eIntArr:
            {
                switch(m_size)
                {
                    case 1:     glUniform1iv(m_location, 1, m_value.m_intArr);  break;
                    case 2:     glUniform2iv(m_location, 1, m_value.m_intArr);  break;
                    case 3:     glUniform3iv(m_location, 1, m_value.m_intArr);  break;
                    case 4:     glUniform4iv(m_location, 1, m_value.m_intArr);  break;
                    default:    assert(false);                                  break;
                }
                break;
            }
            case eFloatArr:
            {
                switch(m_size)
                {
                    case 1:     glUniform1fv(m_location, 1, m_value.m_floatArr);                    break;
                    case 2:     glUniform2fv(m_location, 1, m_value.m_floatArr);                    break;
                    case 3:     glUniform3fv(m_location, 1, m_value.m_floatArr);                    break;
                    case 4:     glUniform4fv(m_location, 1, m_value.m_floatArr);                    break;
                    case 9:     glUniformMatrix3fv(m_location, 1, GL_FALSE, m_value.m_floatArr);    break;
                    case 16:    glUniformMatrix3fv(m_location, 1, GL_FALSE, m_value.m_floatArr);    break;
                    default:    assert(false);                                                      break;
                }
                break;
            }
            default: assert(false); break;
        }
        
        GF_CHECK_GL_ERROR();
    }
}
