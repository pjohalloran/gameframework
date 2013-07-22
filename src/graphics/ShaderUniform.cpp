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
#include "CommonMath.h"

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran {
    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ShaderUniform::VClean()
    {
        GF_CLEAR_GL_ERROR();

        switch(m_type) {
            case eInt: {
                glUniform1i(m_location, m_value.m_intArr[0]);
                break;
            }
            case eFloat: {
                glUniform1f(m_location, m_value.m_floatArr[0]);
                break;
            }
            case eIntArr: {
                switch(m_size) {
                    case 1:
                        glUniform1iv(m_location, m_arrayCount, m_value.m_intArr);
                        break;
                    case 2:
                        glUniform2iv(m_location, m_arrayCount, m_value.m_intArr);
                        break;
                    case 3:
                        glUniform3iv(m_location, m_arrayCount, m_value.m_intArr);
                        break;
                    case 4:
                        glUniform4iv(m_location, m_arrayCount, m_value.m_intArr);
                        break;
                    default:
                        assert(false);
                        break;
                }
                break;
            }
            case eFloatArr: {
                switch(m_size) {
                    case 1:
                        glUniform1fv(m_location, m_arrayCount, m_value.m_floatArr);
                        break;
                    case 2:
                        glUniform2fv(m_location, m_arrayCount, m_value.m_floatArr);
                        break;
                    case 3:
                        glUniform3fv(m_location, m_arrayCount, m_value.m_floatArr);
                        break;
                    case 4:
                        glUniform4fv(m_location, m_arrayCount, m_value.m_floatArr);
                        break;
                    case 9:
                        glUniformMatrix3fv(m_location, m_arrayCount, GL_FALSE, m_value.m_floatArr);
                        break;
                    case 16:
                        glUniformMatrix4fv(m_location, m_arrayCount, GL_FALSE, m_value.m_floatArr);
                        break;
                    default:
                        assert(false);
                        break;
                }
                break;
            }
            default:
                assert(false);
                break;
        }

        GF_CHECK_GL_ERROR();
        m_dirty = false;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ShaderUniform::SetValue(const GLint value, const bool forceCopyToGpu)
    {
        if(!forceCopyToGpu && m_type == eInt && m_size == 1 && value == m_value.m_intArr[0]) {
            return;
        }

        m_type = eInt;
        m_size = 1;
        m_arrayCount = 1;
        m_value.m_intArr[0] = value;
        NotifyShader();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ShaderUniform::SetValue(GLint * const arr, const U32 size, const U32 arrCount, const bool forceCopyToGpu)
    {
        assert(arr != NULL && size > 0 && arrCount > 0);

        if(!forceCopyToGpu && m_type == eIntArr && m_size == size && m_arrayCount == arrCount) {
            bool changed(false);
            for(I32 i = 0; ((!changed) && (i < size)); ++i)
                if(m_value.m_intArr[i] != arr[i]) {
                    changed = true;
                }

            if(!changed) {
                return;
            }
        }

        m_type = eIntArr;
        m_size = size;
        m_arrayCount = arrCount;
        memcpy(m_value.m_intArr, arr, m_size * m_arrayCount * sizeof(GLint));
        NotifyShader();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ShaderUniform::SetValue(const GLfloat value, const bool forceCopyToGpu)
    {
        if(!forceCopyToGpu && m_type == eFloat && m_size == 1 && FloatCmp(value, m_value.m_floatArr[0])) {
            return;
        }

        m_type = eFloat;
        m_size = 1;
        m_arrayCount = 1;
        m_value.m_floatArr[0] = value;
        NotifyShader();
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void ShaderUniform::SetValue(GLfloat * const arr, const U32 size, const U32 arrCount, const bool forceCopyToGpu)
    {
        assert(arr != NULL && size > 0 && arrCount > 0);

        if(!forceCopyToGpu && m_type == eFloatArr && m_size == size && m_arrayCount == arrCount) {
            bool changed(false);
            for(I32 i = 0; ((!changed) && (i < size)); ++i)
                if(!FloatCmp(m_value.m_floatArr[i], arr[i])) {
                    changed = true;
                }

            if(!changed) {
                return;
            }
        }

        m_type = eFloatArr;
        m_size = size;
        m_arrayCount = arrCount;
        memcpy(m_value.m_floatArr, arr, m_size * m_arrayCount * sizeof(GLfloat));
        NotifyShader();
    }
}
