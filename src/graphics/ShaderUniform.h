#pragma once
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
#include <string>

#include "ICleanable.h"
#include "ICleanableObserver.h"
#include "HashedString.h"
#include "GameBase.h"

namespace GameHalloran
{
    // /////////////////////////////////////////////////////////////////
    // @class ShaderUniformBase
    // @author PJ O Halloran
    //
    // Base Uniform type.
    //
    // /////////////////////////////////////////////////////////////////
    class ShaderUniform : public ICleanable, public NonCopyable
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
        U32 m_size;                             ///< Number of elements in the uniform (1, 2, 3, 4, 9 or 16 depending on the type).
        U32 m_arrayCount;                       ///< Number of arrays to copy.
        ICleanableObserver *m_observer;         ///< Pointer to the shader program, the uniform observer.
        bool m_dirty;                           ///< Dirty flag.
        HashedString m_id;                      ///< Uniform name/ID.
        Value m_value;                          ///< ...
        
        // /////////////////////////////////////////////////////////////////
        // Notifies the shader of a change of value.
        //
        // /////////////////////////////////////////////////////////////////
        void NotifyShader()
        {
            if(!m_dirty)
            {
                m_observer->VNotifyDirty(this);
                m_dirty = true;
            }
        };
        
    public:
        
        // /////////////////////////////////////////////////////////////////
        // General constructor.
        //
        // /////////////////////////////////////////////////////////////////
        ShaderUniform(const GLint location, const std::string &name, ICleanableObserver *observer)
						: m_type(eInt)
						, m_location(location)
						, m_size(1)
						, m_arrayCount(1)
						, m_observer(observer)
						, m_dirty(true)
						, m_id(name.c_str())
        {
            assert(observer != NULL);
            m_observer->VNotifyDirty(this);
        };
        
        // /////////////////////////////////////////////////////////////////
        // GLint Constructor.
        //
        // /////////////////////////////////////////////////////////////////
        ShaderUniform(const GLint location, const std::string &name, ICleanableObserver * observer, const GLint val)
						: m_type(eInt)
						, m_location(location)
						, m_size(1)
						, m_arrayCount(1)
						, m_observer(observer)
						, m_dirty(true)
						, m_id(name.c_str())
        {
            assert(observer != NULL);
            m_value.m_intArr[0] = val;
            m_observer->VNotifyDirty(this);
        };
        
        // /////////////////////////////////////////////////////////////////
        // GLint [] Constructor.
        //
        // /////////////////////////////////////////////////////////////////
        ShaderUniform(const GLint location, const std::string &name, ICleanableObserver * observer, const U32 size, const U32 arrCount, const GLint *arr)
						: m_type(eIntArr)
						, m_location(location)
						, m_size(size)
						, m_arrayCount(arrCount)
						, m_observer(observer)
						, m_dirty(true)
						, m_id(name.c_str())
        {
            assert(m_size != 0 && m_size <= 4 && m_arrayCount > 0);
            assert(observer != NULL);
            assert(arr != NULL);
            memcpy(m_value.m_intArr, arr, m_size*sizeof(GLint));
            m_observer->VNotifyDirty(this);
        };
        
        // /////////////////////////////////////////////////////////////////
        // GLfloat Constructor.
        //
        // /////////////////////////////////////////////////////////////////
        ShaderUniform(const GLint location, const std::string &name, ICleanableObserver * observer, const U32 size, const GLfloat val)
						: m_type(eFloat)
						, m_location(location)
						, m_size(size)
						, m_arrayCount(1)
						, m_observer(observer)
						, m_dirty(true)
						, m_id(name.c_str())
        {
            assert(m_observer != NULL);
            m_value.m_floatArr[0] = val;
            m_observer->VNotifyDirty(this);
        };
        
        // /////////////////////////////////////////////////////////////////
        // GLfloat [] Constructor.
        //
        // /////////////////////////////////////////////////////////////////
        ShaderUniform(const GLint location, const std::string &name, ICleanableObserver * observer, const U32 size, const U32 arrCount, const GLfloat *arr)
						: m_type(eFloatArr)
						, m_location(location)
						, m_size(size)
						, m_arrayCount(arrCount)
						, m_observer(observer)
						, m_dirty(true)
						, m_id(name.c_str())
        {
            assert(m_size != 0 && m_size <= 16 && m_arrayCount > 0);
            assert(observer != NULL);
            assert(arr != NULL);
            memcpy(m_value.m_floatArr, arr, m_size*sizeof(GLfloat));
            m_observer->VNotifyDirty(this);
        };
        
        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~ShaderUniform() {};
        
        // /////////////////////////////////////////////////////////////////
        // Get the hashed value of the shader name.
        //
        // /////////////////////////////////////////////////////////////////
        U64 GetId() const { return (m_id.getHashValue()); };
        
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
        void SetValue(const GLint value, const bool forceCopyToGpu = false);
        
        // /////////////////////////////////////////////////////////////////
        // Set the value.
        //
        // /////////////////////////////////////////////////////////////////
        void SetValue(GLint * const arr, const U32 size, const U32 arrCount = 1,  const bool forceCopyToGpu = false);
        
        // /////////////////////////////////////////////////////////////////
        // Set the value.
        //
        // /////////////////////////////////////////////////////////////////
        void SetValue(const GLfloat value, const bool forceCopyToGpu = false);
        
        // /////////////////////////////////////////////////////////////////
        // Set the value.
        //
        // /////////////////////////////////////////////////////////////////
        void SetValue(GLfloat * const arr, const U32 size, const U32 arrCount = 1, const bool forceCopyToGpu = false);
        
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
    
    typedef boost::shared_ptr<ShaderUniform> ShaderUniformSPtr;
}

#endif
