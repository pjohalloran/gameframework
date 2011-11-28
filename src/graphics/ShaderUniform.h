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
    template<typename T>
        class ShaderUniformBase : public ICleanable
    {
    private:
        
        GLint m_location;                       ///< Location of the uniform in the shader.
        ICleanableObserver *m_observer;         ///< Pointer to the shader program, the uniform observer.
        
    protected:
        
        bool m_dirty;                           ///< Dirty flag.
        T *m_value;                             ///< Pointer to the value.
        
        // /////////////////////////////////////////////////////////////////
        //
        // /////////////////////////////////////////////////////////////////
        ShaderUniformBase() {};
        
        // /////////////////////////////////////////////////////////////////
        //
        // /////////////////////////////////////////////////////////////////
        ShaderUniformBase(const ShaderUniformBase &obj) {};
        
    public:
        
        // /////////////////////////////////////////////////////////////////
        // 
        //
        // /////////////////////////////////////////////////////////////////
        inline ShaderUniformBase(GLint location, ICleanableObserver *observer) : m_location(location), m_observer(observer), m_dirty(true), m_value() {};
        
        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~ShaderUniformBase() {};
        
        // /////////////////////////////////////////////////////////////////
        // Get the value
        //
        // /////////////////////////////////////////////////////////////////
        inline T *GetValue() { return (m_value); };
        
        // /////////////////////////////////////////////////////////////////
        // Set the value.
        //
        // /////////////////////////////////////////////////////////////////
        void SetValue(const T &value)
        {
            if((!m_dirty) && (m_value != value))
            {
                m_dirty = true;
                if(m_observer)
                    m_observer->VNotifyDirty(this);
            }
        };
        
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
        virtual void VClean() = 0;
    };
    
    // /////////////////////////////////////////////////////////////////
    // @class ShaderUniformPod
    // @author PJ O Halloran
    //
    // Uniform for a POD type or array (GLint or GLfloat only supported).
    //
    // /////////////////////////////////////////////////////////////////
    template<typename T>
    class ShaderUniformPod : public ShaderUniformBase
    {
    private:
        
        GLsizei m_size;                     ///< Number of elements in the array.
        
    protected:
        
        // /////////////////////////////////////////////////////////////////
        // Set a GLint value.
        //
        // /////////////////////////////////////////////////////////////////
        void SetGlValue(GLint *arr)
        {
            if(arr)
            {
                switch(m_size)
                {
                    case 1: glUniform1iv(m_location, 1, arr); break;
                    case 2: glUniform2iv(m_location, 1, arr); break;
                    case 3: glUniform3iv(m_location, 1, arr); break;
                    case 4: glUniform4iv(m_location, 1, arr); break;
                    default: break;
                }
            }
        };
        
        // /////////////////////////////////////////////////////////////////
        // Set a GLfloat value.
        //
        // /////////////////////////////////////////////////////////////////
        void SetGlValue(GLfloat *arr)
        {
            if(arr)
            {
                switch(m_size)
                {
                    case 1: glUniform1fv(m_location, 1, arr); break;
                    case 2: glUniform2fv(m_location, 1, arr); break;
                    case 3: glUniform3fv(m_location, 1, arr); break;
                    case 4: glUniform4fv(m_location, 1, arr); break;
                    default: break;
                }
            }
        };
        
    public:
        
        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        inline ShaderUniformPod(GLint location, ICleanableObserver *observer, const GLsizei count) : ShaderUniformBase(location, observer), m_size(count) {};
        
        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~ShaderUniformPod() {};
        
        // /////////////////////////////////////////////////////////////////
        // Interface for shader program to update uniform values to GL.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VClean()
        {
            m_dirty = false;
            SetGlValue(m_value);
        };
    };
    
    // /////////////////////////////////////////////////////////////////
    // @class ShaderUniformMatrix
    // @author PJ O Halloran
    //
    // A uniform matrix type (column major Matrix4x4 and 3x3 supported only).
    //
    // /////////////////////////////////////////////////////////////////
    template<typename T>
    class ShaderUniformMatrix : public ShaderUniformBase
    {
    public:
        
        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        enum MatrixType
        {
            eThreeByThree,
            eFourByFour
        };
        
    private:
        
        MatrixType m_type;                      ///< 
        
    protected:
        
        // /////////////////////////////////////////////////////////////////
        // Set the matrix value.
        //
        // /////////////////////////////////////////////////////////////////
        void SetGlValue(GLfloat *arr)
        {
            if(arr)
            {
                switch(m_type)
                {
                    case eThreeByThree: glUniformMatrix3fv(m_location, 1, GL_FALSE, arr); break;
                    case eFourByFour: glUniformMatrix4fv(m_location, 1, GL_FALSE, arr); break;
                    default: break;
                }
            }
        };
        
    public:
        
        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        inline ShaderUniformPod(GLint location, ICleanableObserver *observer, const MatrixType type) : ShaderUniformBase(location, observer), m_type(type) {};
        
        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        virtual ~ShaderUniformPod() {};
        
        // /////////////////////////////////////////////////////////////////
        // Clean the uniform value - send to GL.
        //
        // /////////////////////////////////////////////////////////////////
        virtual void VClean()
        {
            m_dirty = false;
            SetGlValue(m_value);
        };
    };
    
}


#endif
