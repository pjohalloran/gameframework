// GLMatrixStack.h
// Matrix stack functionality
/*
Copyright (c) 2009, Richard S. Wright Jr.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list 
of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this list 
of conditions and the following disclaimer in the documentation and/or other 
materials provided with the distribution.

Neither the name of Richard S. Wright Jr. nor the names of other contributors may be used 
to endorse or promote products derived from this software without specific prior 
written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// /////////////////////////////////////////////////////////////////
// @file GLMatrixStack.h
// @author Richard S. Wright Jr & PJ O Halloran
// @date 09/09/2010
//
// A stack of matrices.  Original source from the OpenGL Superbible
// 5th edition.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GLT_MATRIX_STACK
#define __GLT_MATRIX_STACK

#ifdef WIN32
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

// External Headers
#include <string>
#include <boost/shared_ptr.hpp>


// Project Headers
#include "GameException.h"
#include "Matrix.h"
#include "GLFrame.h"

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @enum GLT_STACK_ERROR
	//
	// Types of errors that can occur while using the matrix stack.
	// GLT_STACK_NOERROR:	Stack is ok.
	// GLT_STACK_OVERFLOW:  Maximum stack size reached. No more pushing.
	// GLT_STACK_UNDERFLOW:	Minimum stack size reached. No more popping.
	//
	// /////////////////////////////////////////////////////////////////
	enum GLT_STACK_ERROR { GLT_STACK_NOERROR = 0, GLT_STACK_OVERFLOW, GLT_STACK_UNDERFLOW }; 

	// /////////////////////////////////////////////////////////////////
	// @class GLMatrixStack
	// @author Richard S. Wright Jr. and PJ O Halloran.
	//
	// A stack of matrices.  Useful for saving and restoring transformation
	// state in a 3D application quickly and efficiently.
	//
	// Changes over original implementation:
	// - I added comments to all the class members and I changed it to use
	// the 3D math library I developed.
	//
	// - I added an exception to the constructor to ensure that the object
	// is not created if memory cannot be allocated.
	//
	// - Fixed bug in constructor where if stack size was less than 0, then
	// it would try to allocate memory (undefined C++ behaviour i believe).
	//
	// - Fixed C stack overflow bug in PushMatrix where if the matrix stack
	// is at its maximum depth it would try and write 1 element off the end
	// of the array going into the realm of undefined runtime behaviour.
	//
	// - I have Push and Pop return a bool to indicate success or failure
	// so that we don't have to call GetLastError() after every push or pop 
	// to check if it worked!
	//
	// - Some methods are a bit large to locate in a header and inline
	// so they are moved to a seperate cpp file.
	//
	// /////////////////////////////////////////////////////////////////
	class GLMatrixStack
	{
	private:

		GLT_STACK_ERROR m_lastError;			///< Last error type.
		U32 m_stackDepth;				///< Stack size.
		U32 m_stackPointer;			///< Pointer to current location on stack.
		Matrix4 **m_pStack;						///< Array of matrices.

	public:

		// Default size of the stack.
		static const U32 DEFAULT_STACK_DEPTH = 64;

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param stackDepth The depth/size of the stack.  It has a default
		//						value (64) if not supplied of if given a value of
		//						0.
		//
		// @throw GameException& If we fail to allocate memory for the stack.
		//
		// /////////////////////////////////////////////////////////////////
		explicit GLMatrixStack(I32 stackDepth = DEFAULT_STACK_DEPTH) throw (GameException &);
		
		// /////////////////////////////////////////////////////////////////
		// Destructor (non virtual so do not derive from!).
		//
		// /////////////////////////////////////////////////////////////////
		~GLMatrixStack(void);

		// /////////////////////////////////////////////////////////////////
		// Load the identity matrix into the current matrix.
		//
		// /////////////////////////////////////////////////////////////////
		inline void LoadIdentity(void) { m_pStack[m_stackPointer]->LoadIdentity(); }; 
		
		// /////////////////////////////////////////////////////////////////
		// Set the current matrix.
		//
		// @param matrix The matrix to use as the current matrix.
		//
		// /////////////////////////////////////////////////////////////////
		inline void LoadMatrix(const Matrix4 &matrix) { *m_pStack[m_stackPointer] = matrix; };
        
		// /////////////////////////////////////////////////////////////////
		// Set the current matrix as the frame of reference.
		//
		// @param frame The frame of reference to use as the current matrix.
		//
		// /////////////////////////////////////////////////////////////////
        inline void LoadMatrix(GLFrame &frame)
		{
            Matrix4 m;
            frame.GetMatrix(m);
            LoadMatrix(m);
		};
        
		// /////////////////////////////////////////////////////////////////
		// Multiply a matrix by the matrix on the top of the stack.
		//
		// /////////////////////////////////////////////////////////////////
		inline void MultiplyMatrix(const Matrix4 &matrix) { *m_pStack[m_stackPointer] *= matrix; };
        
		// /////////////////////////////////////////////////////////////////
		// Multiply a frame of reference matrix by the matrix on the top of
		// the stack.
		//
		// /////////////////////////////////////////////////////////////////
        inline void MultiplyMatrix(GLFrame &frame)
		{
            Matrix4 m;
            frame.GetMatrix(m);
            MultiplyMatrix(m);
		};
        
		// /////////////////////////////////////////////////////////////////
		// Save the top most matrix and push a copy of it one place down the
		// stack.
		//
		// @return bool True on success or false on failure (check last 
		//				error on failure).
		//
		// /////////////////////////////////////////////////////////////////
		bool PushMatrix(void);
		
		// /////////////////////////////////////////////////////////////////
		// Restore the top of the stack to the last matrix.
		//
		// @return bool True on success or false on failure (check last 
		//				error on failure).
		//
		// /////////////////////////////////////////////////////////////////
		bool PopMatrix(void);

		// /////////////////////////////////////////////////////////////////
		// Save the top most matrix and push it one place down the stack
		// while setting the top of the stack to be a matrix you supply.
		//
		// @param matrix The matrix to place on top of the stack.
		//
		// @return bool True on success or false on failure (check last 
		//				error on failure).
		//
		// /////////////////////////////////////////////////////////////////
		bool PushMatrix(const Matrix4 &matrix);
		
		// /////////////////////////////////////////////////////////////////
		// Push a frame of reference matrix onto the top of the stack.
		//
		// @param frame The frame of reference to push onto the top of the
		//				stack.
		//
		// @return bool True on success or false on failure (check last 
		//				error on failure).
		//
		// /////////////////////////////////////////////////////////////////
        inline bool PushMatrix(GLFrame &frame)
		{
            Matrix4 m;
            frame.GetMatrix(m);
            return (PushMatrix(m));
        };
        
		// /////////////////////////////////////////////////////////////////
		// Get a read only copy of the matrix on the top of the stack.
		//
		// /////////////////////////////////////////////////////////////////
		inline const Matrix4& GetMatrix(void) const { return (*m_pStack[m_stackPointer]); };

		// /////////////////////////////////////////////////////////////////
		// Get a copy of the matrix on the top of the stack.
		//
		// /////////////////////////////////////////////////////////////////
		inline void GetMatrix(Matrix4 &matrix) const { matrix = *m_pStack[m_stackPointer]; };

		// /////////////////////////////////////////////////////////////////
		// Get the last error to have occurred.
		//
		// /////////////////////////////////////////////////////////////////
		inline GLT_STACK_ERROR GetLastError(void)
		{
			GLT_STACK_ERROR retval = m_lastError;
			m_lastError = GLT_STACK_NOERROR;
			return (retval);
		};
		
		// /////////////////////////////////////////////////////////////////
		// Scale the top most matrix.
		//
		// @param x The number of units on the X axis to scale the matrix by.
		// @param y The number of units on the Y axis to scale the matrix by.
		// @param z The number of units on the Z axis to scale the matrix by.
		//
		// /////////////////////////////////////////////////////////////////
		void Scale(const F32 x, const F32 y, const F32 z);
		
		// /////////////////////////////////////////////////////////////////
		// Translate the top most matrix.
		//
		// @param x The number of units on the X axis to translate the matrix by.
		// @param y The number of units on the Y axis to translate the matrix by.
		// @param z The number of units on the Z axis to translate the matrix by.
		//
		// /////////////////////////////////////////////////////////////////
		void Translate(const F32 x, const F32 y, const F32 z);
		
		// /////////////////////////////////////////////////////////////////
		// Rotate the top most matrix.
		//
		// @param angle The angle to rotate by in degrees.
		// @param x The X axis of rotation.
		// @param y The Y axis of rotation.
		// @param z The Z axis of rotation.
		//
		// /////////////////////////////////////////////////////////////////
		void Rotate(const F32 angle, const F32 x, const F32 y, const F32 z);
		
		// /////////////////////////////////////////////////////////////////
		// Scale the top most matrix by the components of a vector.
		//
		// @param vScale The vector to scale by.
		//
		// /////////////////////////////////////////////////////////////////
		inline void Scalev(const Vector3 &vScale)
		{
			Scale(vScale.GetX(), vScale.GetY(), vScale.GetZ());
		};
		
		// /////////////////////////////////////////////////////////////////
		// Translate the top most matrix by the components of a vector.
		//
		// @param vTranslate The vector to translate by.
		//
		// /////////////////////////////////////////////////////////////////
		inline void Translatev(const Vector3 &vTranslate)
		{
			Translate(vTranslate.GetX(), vTranslate.GetY(), vTranslate.GetZ());
		};
		
		// /////////////////////////////////////////////////////////////////
		// Rotate the top most matrix around the axis described by the vector.
		//
		// @param angle The angle to rotate by in degrees.
		// @param vAxis The axis to rotate around.
		//
		// /////////////////////////////////////////////////////////////////
		inline void Rotatev(const F32 angle, const Vector3 &vAxis)
		{
			Rotate(angle, vAxis.GetX(), vAxis.GetY(), vAxis.GetZ());
		};
	};

	// /////////////////////////////////////////////////////////////////
	// @class GLMatrixStackRaii
	// @author PJ O Halloran
	//
	// The stack state will be restored when instances of this class go
	// out of scope giving the matrix stack more exception safety if it
	// is used carefully in conjunction with this class.
	//
	// /////////////////////////////////////////////////////////////////
	class GLMatrixStackRaii
	{
	private:

		boost::shared_ptr<GLMatrixStack> m_stackPtr;

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// Saves the current state of the matrix stack.
		//
		// @param stackPtr Pointer to the matrix stack.
		//
		// /////////////////////////////////////////////////////////////////
		GLMatrixStackRaii(boost::shared_ptr<GLMatrixStack> stackPtr) : m_stackPtr(stackPtr) { if(m_stackPtr) { m_stackPtr->PushMatrix(); } };

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// Saves the current state of the matrix stack and sets the new
		// matrix state with the matrix supplied.
		//
		// @param stackPtr Pointer to the matrix stack.
		// @param matrix The new matrix to go on top of the stack.
		//
		// /////////////////////////////////////////////////////////////////
		GLMatrixStackRaii(boost::shared_ptr<GLMatrixStack> stackPtr, const Matrix4 &matrix) : m_stackPtr(stackPtr) { if(m_stackPtr) { m_stackPtr->PushMatrix(matrix); } };

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// Restores the previous state of the matrix stack.
		//
		// /////////////////////////////////////////////////////////////////
		~GLMatrixStackRaii() { try { if(m_stackPtr) { m_stackPtr->PopMatrix(); } } catch(...) {} };

	};

#ifdef WIN32
#	pragma warning( pop )
#endif

}
#endif
