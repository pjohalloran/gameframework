// /////////////////////////////////////////////////////////////////
// @file GLMatrixStack.cpp
// @author Richard S. Wright Jr & PJ O Halloran
// @date 09/09/2010
//
// A stack of matrices.
//
// /////////////////////////////////////////////////////////////////

// External Headers


// Project Headers
#include "GLMatrixStack.h"
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
	GLMatrixStack::GLMatrixStack(I32 stackDepth) throw (GameException &)\
		: m_lastError(GLT_STACK_NOERROR), m_stackDepth(stackDepth), m_stackPointer(0), m_pStack(NULL)
	{
		// Don't allow the stack depth to be 0!
		if(m_stackDepth == 0)
		{
			m_stackDepth = DEFAULT_STACK_DEPTH;
		}

		m_pStack = GCC_NEW Matrix4*[m_stackDepth];
		if(!m_pStack)
		{
			throw GameException(std::string("Failed to allocate memory for the stack."));
		}
		for(U32 i = 0; i < m_stackDepth; ++i)
		{
			m_pStack[i] = GCC_NEW Matrix4();
			if(!m_pStack[i])
			{
				throw GameException(std::string("Failed to allocate memory for an element of the stack."));
			}
		}
		m_pStack[0]->LoadIdentity();
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	GLMatrixStack::~GLMatrixStack(void)
	{
		try
		{
			for(U32 i = 0; i < m_stackDepth; ++i)
			{
				if(m_pStack[i])
				{
					delete m_pStack[i];
					m_pStack[i] = NULL;
				}
			}
			if(m_pStack)
			{
				delete [] m_pStack;
				m_pStack = NULL;
			}
		}
		catch(...)
		{
		}
	}
        				
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GLMatrixStack::PushMatrix(void)
	{
		bool result = true;
		if(m_stackPointer < m_stackDepth-1)
		{
			++m_stackPointer;
			*m_pStack[m_stackPointer] = *m_pStack[m_stackPointer-1];
		}
		else
		{
			result = false;
			m_lastError = GLT_STACK_OVERFLOW;
		}
		return (result);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GLMatrixStack::PopMatrix(void)
	{
		bool result = true;
		if(m_stackPointer > 0)
		{
			--m_stackPointer;
		}
		else
		{
			m_lastError = GLT_STACK_UNDERFLOW;
			result = false;
		}
		return (result);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool GLMatrixStack::PushMatrix(const Matrix4 &matrix)
	{
		bool result = true;
		if(m_stackPointer < m_stackDepth-1)
		{
			++m_stackPointer;
			*m_pStack[m_stackPointer] = matrix;
		}
		else
		{
			result = false;
			m_lastError = GLT_STACK_OVERFLOW;
		}
		return (result);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLMatrixStack::Scale(const F32 x, const F32 y, const F32 z)
	{
		Matrix4 scaleMat;
		BuildScaleMatrix4(scaleMat, x, y, z);
		MultiplyMatrix(scaleMat);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLMatrixStack::Translate(const F32 x, const F32 y, const F32 z)
	{
		Matrix4 translateMat;
		GameHalloran::BuildTranslationMatrix4(translateMat, x, y, z);
		MultiplyMatrix(translateMat);
	}
	
	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void GLMatrixStack::Rotate(const F32 angle, const F32 x, const F32 y, const F32 z)
	{
		Matrix4 rotateMat;
		GameHalloran::BuildRotationArbitraryMatrix4(rotateMat, angle, x, y, z);
		MultiplyMatrix(rotateMat);
	}

}
