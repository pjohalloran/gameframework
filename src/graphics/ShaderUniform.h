#ifndef GF_SHADER_UNIFORM_H
#define GF_SHADER_UNIFORM_H

// /////////////////////////////////////////////////////////////////
// @file ShaderUniform.h
// @author PJ O Halloran
// @date 26/11/2011
//
// Contains ShaderUniform class definition.
//
// /////////////////////////////////////////////////////////////////

#include <cstring>

#include "GamePlatform.h"
#include "ICleanable.h"
#include "ICleanableObserver.h"

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{
    // /////////////////////////////////////////////////////////////////
    // @class ShaderUniformBase
    // @author PJ O Halloran
    //
    // Base Uniform type.
    //
    // /////////////////////////////////////////////////////////////////
    class ShaderUniform : public ICleanable
    {
    public:
        
        enum UniformType
        {
            eInt = 0,
            eFloat,
            eIntArr,
            eFloatArr
        };
        
    private:
        
        union Value
        {
            GLint m_intArr[4];
            GLfloat m_floatArr[16];
        };
        
        UniformType m_type;                     ///< ...
        GLint m_location;                       ///< Location of the uniform in the shader.
        U32 m_size;                             ///< Number of elements in the uniform (1, 2, 3, 4, 9 or 16 depending on the type)
        ICleanableObserver *m_observer;         ///< Pointer to the shader program, the uniform observer.
        bool m_dirty;                           ///< Dirty flag.
        Value m_value;                          ///< ...
        
    public:
        
        // /////////////////////////////////////////////////////////////////
        // GLint Constructor.
        //
        // /////////////////////////////////////////////////////////////////
        ShaderUniform(const GLint location, ICleanableObserver * observer, const GLint val) : m_type(eInt), m_location(location), m_size(1), m_observer(observer), m_dirty(true)
        {
            assert(observer != NULL);
            m_value.m_intArr[0] = val;
        };
        
        // /////////////////////////////////////////////////////////////////
        // GLint [] Constructor.
        //
        // /////////////////////////////////////////////////////////////////
        ShaderUniform(const GLint location, ICleanableObserver * observer, const U32 size, const GLint *arr) : m_type(eIntArr), m_location(location), m_size(size), m_observer(observer), m_dirty(true)
        {
            assert(m_size != 0 && m_size <= 4);
            assert(observer != NULL);
            assert(arr != NULL);
            memcpy(m_value.m_intArr, arr, m_size*sizeof(GLint));
        };
        
        // /////////////////////////////////////////////////////////////////
        // GLfloat Constructor.
        //
        // /////////////////////////////////////////////////////////////////
        ShaderUniform(const GLint location, ICleanableObserver * observer, const U32 size, const GLfloat val) : m_type(eFloat), m_location(location), m_size(size), m_observer(observer), m_dirty(true)
        {
            assert(m_observer != NULL);
            m_value.m_floatArr[0] = val;
        };
        
        // /////////////////////////////////////////////////////////////////
        // GLfloat [] Constructor.
        //
        // /////////////////////////////////////////////////////////////////
        ShaderUniform(const GLint location, ICleanableObserver * observer, const U32 size, const GLfloat *arr) : m_type(eFloatArr), m_location(location), m_size(size), m_observer(observer), m_dirty(true)
        {
            assert(m_size != 0 && m_size <= 16);
            assert(observer != NULL);
            assert(arr != NULL);
            memcpy(m_value.m_floatArr, arr, m_size*sizeof(GLfloat));
        };
        
        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~ShaderUniform() {};
        
        // /////////////////////////////////////////////////////////////////
        // Get the value.
        //
        // /////////////////////////////////////////////////////////////////
        inline void GetValue(GLint &val) { assert(m_type == eInt); val = m_value.m_intArr[0]; };
        
        // /////////////////////////////////////////////////////////////////
        // Get the value.
        //
        // /////////////////////////////////////////////////////////////////
        inline void GetValue(GLint *arr) { assert(m_type == eIntArr); assert(arr != NULL); memcpy(arr, m_value.m_intArr, m_size*sizeof(GLint)); };
        
        // /////////////////////////////////////////////////////////////////
        // Get the value.
        //
        // /////////////////////////////////////////////////////////////////
        inline void GetValue(GLfloat &val) { assert(m_type == eFloat); val = m_value.m_floatArr[0]; };
        
        // /////////////////////////////////////////////////////////////////
        // Get the value.
        //
        // /////////////////////////////////////////////////////////////////
        inline void GetValue(GLfloat *arr) { assert(m_type == eFloatArr); assert(arr != NULL); memcpy(arr, m_value.m_floatArr, m_size*sizeof(GLfloat)); };
        
        // /////////////////////////////////////////////////////////////////
        // Set the value.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetValue(const GLint value) { assert(m_type == eInt); m_value.m_intArr[0] = value; };
        
        // /////////////////////////////////////////////////////////////////
        // Set the value.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetValue(GLint * const arr) { assert(m_type == eIntArr); assert(arr != NULL);  memcpy(arr, m_value.m_intArr, m_size*sizeof(GLint)); };
        
        // /////////////////////////////////////////////////////////////////
        // Set the value.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetValue(const GLfloat value) { assert(m_type == eFloat); m_value.m_floatArr[0] = value; };
        
        // /////////////////////////////////////////////////////////////////
        // Set the value.
        //
        // /////////////////////////////////////////////////////////////////
        inline void SetValue(GLfloat * const arr) { assert(m_type == eFloatArr); assert(arr != NULL);  memcpy(arr, m_value.m_floatArr, m_size*sizeof(GLfloat)); };
        
        // /////////////////////////////////////////////////////////////////
        // Get uniform location.
        //
        // /////////////////////////////////////////////////////////////////
        inline GLint GetLocation() { return (m_location); };
        
        // /////////////////////////////////////////////////////////////////
        // Is the uniform marked for updating before the OpenGL draw?
        //
        // /////////////////////////////////////////////////////////////////
        inline bool IsDirty() { return (m_dirty); };
        
        // /////////////////////////////////////////////////////////////////
        // Clean the uniform (Update its value into the GL state machine).
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VClean();
    };
}

#endif
