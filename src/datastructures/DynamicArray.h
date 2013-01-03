#pragma once
#ifndef _GF_DYNAMIC_ARRAY_H
#define _GF_DYNAMIC_ARRAY_H

// ////////////////////////////////////////////////////////////
// @file DynamicArray.h
// @author PJ O Halloran
// @date 27/03/2011
//
// Header for the template DynamicArray container class(es).
//
// ////////////////////////////////////////////////////////////

#include <cstring>

#include "GameBase.h"

namespace GameHalloran
{

	// ////////////////////////////////////////////////////////////
	// @class DynamicArray
	// @author PJ O Halloran
	// 
	// A dynamically growing and shrinking array.
	//
	// Usage of it and its Iterator type is very similar to std::vector
	// and std::vector::iterator.
	//
	// advatages:
	// - Data stored contiguosly in memory.
	// - Fast iteration over data and element access.
	// - Much simpler memory allocater and alignment support than
	//  STL vector.  Supports using OS heap with default new/delete
	//	or custom allocaters.
	//
	// disadvatages:
	// - Memory allocations required when increasing size past capacity.
	// - Slow insertion and removal of elements in the middle of the
	//	array.
	//
	// ////////////////////////////////////////////////////////////
	template<typename ElementType>
		class DynamicArray
	{
	public:

		// ////////////////////////////////////////////////////////////
		// @class Iterator.
		// @author PJ O Halloran
		//
		// Iterator for the DynamicArray container.  Similar in
		// functionality to an STL bidirectional iterator.
		//
		// ////////////////////////////////////////////////////////////
		class Iterator
		{
			friend class DynamicArray<ElementType>;

		private:

			U64 m_index;						///< Element iterator refers to.
			DynamicArray *m_arr;				///< Pointer for the DynamicArray object the iterator manages.

			// ////////////////////////////////////////////////////////////
			// Default constructor hidden.
			//
			// ////////////////////////////////////////////////////////////
			Iterator() {};

		public:

			// ////////////////////////////////////////////////////////////
			// Constructor.
			//
			// @param index The index of the element to point at.
			// @param arr The DynamicArray.
			//
			// ////////////////////////////////////////////////////////////
			explicit Iterator(const U64 index, DynamicArray *arr) : m_index(index), m_arr(arr) {};

			// ////////////////////////////////////////////////////////////
			// Destructor.
			//
			// ////////////////////////////////////////////////////////////
			~Iterator() {};

			// ////////////////////////////////////////////////////////////
			// Equivalent operator.
			//
			// ////////////////////////////////////////////////////////////
			bool operator==(const Iterator &rhs) const
			{
				return (m_arr && m_arr->GetUniqueId() == rhs.m_arr->GetUniqueId() && m_index == rhs.m_index);
			};

			// ////////////////////////////////////////////////////////////
			// Not equivalent operator.
			//
			// ////////////////////////////////////////////////////////////
			bool operator!=(const Iterator &rhs) const
			{
				return (!(*this == rhs));
			};

			// ////////////////////////////////////////////////////////////
			// Validate current element.
			//
			// Tip:
			// If this test fails then using the Iterators -> or * operators
			// will cause a seg fault!
			//
			// ////////////////////////////////////////////////////////////
			bool operator!() const
			{
				return (m_arr && m_index >= m_arr->GetSize());
			};

			// ////////////////////////////////////////////////////////////
			// Get a pointer to the current element.
			//
			// ////////////////////////////////////////////////////////////
			ElementType *operator->()
			{
				if(m_arr)
				{
					return (&(*m_arr)[m_index]);
				}
				return (NULL);
			};

			// ////////////////////////////////////////////////////////////
			// Get a reference to the current element.
			//
			// ////////////////////////////////////////////////////////////
			ElementType &operator*()
			{
				return ((*m_arr)[m_index]);
			};

			// ////////////////////////////////////////////////////////////
			// Post increment operator.
			//
			// Complexity: O(1).
			//
			// ////////////////////////////////////////////////////////////
			Iterator operator++(I32)
			{
				Iterator tmp(*this);
				++(*this);
				return (tmp);
			};

			// ////////////////////////////////////////////////////////////
			// Pre increment operator.
			//
			// Complexity: O(1).
			//
			// ////////////////////////////////////////////////////////////
			Iterator &operator++()
			{
				if(m_index <= m_arr->GetSize()-1)
				{
					++m_index;
				}
				return (*this);
			};

			// ////////////////////////////////////////////////////////////
			// Post decrement operator.
			//
			// Complexity: O(1).
			//
			// ////////////////////////////////////////////////////////////
			Iterator operator--(I32)
			{
				Iterator tmp(*this);
				--(*this);
				return (tmp);
			};

			// ////////////////////////////////////////////////////////////
			// Pre decrement operator.
			//
			// Complexity: O(1).
			//
			// ////////////////////////////////////////////////////////////
			Iterator &operator--()
			{
				if(m_index > 1)
				{
					--m_index;
				}
				return (*this);
			};

			// ////////////////////////////////////////////////////////////
			// Increment operator.
			//
			// Complexity: O(1).
			//
			// ////////////////////////////////////////////////////////////
			Iterator operator+(const U64 offset) const
			{
				Iterator copy(*this);
				copy += offset;
				return (copy);
			};

			// ////////////////////////////////////////////////////////////
			// Increment assignment operator.
			//
			// Complexity: O(1).
			//
			// ////////////////////////////////////////////////////////////
			Iterator &operator+=(const U64 offset)
			{
				m_index += offset;
				// Ensure index does not go past end iterator.
				if(m_index > m_arr->GetSize())
				{
					m_index = m_arr->GetSize();
				}
				return (*this);
			};

			// ////////////////////////////////////////////////////////////
			// Decrement operator.
			//
			// Complexity: O(1).
			//
			// ////////////////////////////////////////////////////////////
			Iterator operator-(const U64 offset) const
			{
				Iterator copy(*this);
				copy -= offset;
				return (copy);
			};

			// ////////////////////////////////////////////////////////////
			// Decrement operator.
			//
			// Complexity: O(1).
			//
			// ////////////////////////////////////////////////////////////
			Iterator &operator-=(const U64 offset)
			{
				m_index -= offset;
				return (*this);
			};

			// ////////////////////////////////////////////////////////////
			// Is this iterator earlier in the array than the
			// iterator on the rhs?
			//
			// Complexity:	O(1)
			//
			// ////////////////////////////////////////////////////////////
			bool operator<(Iterator &rhs) const
			{
				if(this == &rhs)
				{
					return (false);
				}
				if(m_arr->GetUniqueId() != rhs.m_arr->GetUniqueId())
				{
					return (false);
				}

				return (m_index < rhs.m_index);
			};
		};

	private:

		ElementType *m_arr;						///< Pointer to the array of elements.
		U64 m_size;								///< Number of elements in the array (not always the allocated size).
		U64 m_capacity;							///< Allocated size of the array.
		U32 m_id;								///< The ID of this instace of the DynamicArray.

		// Running count of the number of DynamicArray instances created to date.
		// TODO: Does using a static value here make using the container in a multi
		//  thread environment dangerous?  Alternatively I could generate a uuid for the ID instead.
		static U32 DYNAMIC_ARRAY_COUNT;

		// ////////////////////////////////////////////////////////////
		// Allocate an array.
		// 
		// Complexity:	O(N)
		//
		// TODO: Allow the programmer to define or supply their own
		// allocater here besides using new/delete.
		//
		// @param size The size of the allocated array.
		//
		// ////////////////////////////////////////////////////////////
		ElementType *AllocateArray(const U64 size)
		{
			if(size == 0)
			{
				return (NULL);
			}

			ElementType *arr = GCC_NEW ElementType[size];
			if(arr)
			{
				memset(arr, 0, size * sizeof(ElementType));
			}

			return (arr);
		};

		// ////////////////////////////////////////////////////////////
		// Deallocate an array.
		//
		// Complexity:	O(1)
		//
		// TODO: Allow the programmer to define or supply their own
		// allocater here besides using new/delete.
		//
		// @param arr PoI32er to the array to deallocate.
		//
		// ////////////////////////////////////////////////////////////
		void DeallocateArray(ElementType *arr)
		{
			if(arr)
			{
				delete [] arr;
				arr = NULL;
			}
		};

		// ////////////////////////////////////////////////////////////
		// Constructor helper function.
		//
		// Complexity:	O(N)
		//
		// ////////////////////////////////////////////////////////////
		void Init()
		{
			m_arr = AllocateArray(m_capacity);
			m_id = DYNAMIC_ARRAY_COUNT;
			++DYNAMIC_ARRAY_COUNT;
		};

		// ////////////////////////////////////////////////////////////
		// Destructor helper function.
		//
		// Complexity:	O(1)
		//
		// ////////////////////////////////////////////////////////////
		void Destroy()
		{
			DeallocateArray(m_arr);
		};

		// ////////////////////////////////////////////////////////////
		// Copies contents of 1 array to another.
		//
		// Used by SetCapacity and C Array constructor.
		// 
		// Complexity: O(N)
		//
		// @param src The C array.
		// @param srcSize The number of elements in the src C array.
		// @param dest The destination C array.  The size of the dest
		//				array should be at least "srcSize".
		//
		// @return bool False if the C arrays are NULL or size is 0. True
		//					otherwise.
		//
		// ////////////////////////////////////////////////////////////
		bool ArrayCopy(const ElementType * const src, const U64 srcSize, ElementType * const dest)
		{
			if(!src || !dest || srcSize == 0)
			{
				return (false);
			}

			// Add the C array to this array.
			//  N.B. memcpy() is not used since we don't know the element type until
			//   runtime.  memcpy is not safe to use with arrays of C++ objects
			//    in general  Its only safe with POD types.
			for(U64 i(0); i < srcSize; ++i)
			{
				dest[i] = src[i];
			}

			return (true);
		};

	public:

		// ////////////////////////////////////////////////////////////
		// Default constructor.
		//
		// Has a default initial capacity of 10 elements!
		//
		// Complexity:	O(N)
		//
		// ////////////////////////////////////////////////////////////
		explicit DynamicArray() : m_arr(NULL), m_size(0), m_capacity(10), m_id(0) { Init(); };

		// ////////////////////////////////////////////////////////////
		// Constructor.
		//
		// Create an empty array with a specified capacity.
		//
		// @param capacity The initial capacity of the array.
		//
		// Complexity:	O(N)
		//
		// ////////////////////////////////////////////////////////////
		explicit DynamicArray(const U64 capacity) : m_arr(NULL), m_size(0), m_capacity(capacity), m_id(0) { Init(); };

		// ////////////////////////////////////////////////////////////
		// Constructor.
		//
		// Create an array with a specified size and a default value for
		// each element.  Array capacity will be twice the array size.
		//
		// @param capacity The initial capacity of the array.
		//
		// Complexity:	O(N)
		//
		// ////////////////////////////////////////////////////////////
		explicit DynamicArray(const U64 size, const ElementType &defaultVal) : m_arr(NULL), m_size(size), m_capacity(size*2), m_id(0)
		{
			Init();

			if(m_arr)
			{
				for(U64 i(0); i < m_size; ++i)
				{
					m_arr[i] = defaultVal;
				}
			}
		};

		// ////////////////////////////////////////////////////////////
		// Array constructor.
		//
		// Complexity:	O(N)
		//
		// ////////////////////////////////////////////////////////////
		explicit DynamicArray(const ElementType * const arr, const U64 size) : m_arr(NULL), m_size(size), m_capacity(size*2), m_id(0)
		{
			Init();
			ArrayCopy(arr, size, m_arr);
		};

		// ////////////////////////////////////////////////////////////
		// Constructor.
		//
		// Complexity:	O(N)
		//
		// ////////////////////////////////////////////////////////////
		explicit DynamicArray(const DynamicArray &arrObj) : m_arr(NULL), m_size(arrObj.GetSize()), m_capacity(arrObj.GetCapacity()), m_id(0)
		{
			DynamicArray &nonConstArrObj = const_cast<DynamicArray &>(arrObj);
			Iterator begin(nonConstArrObj.Begin()), end(nonConstArrObj.End());

			Init();
			if(m_capacity > 0)
			{
				CopyRange(begin, end, Begin());
			}
		};

		// ////////////////////////////////////////////////////////////
		// Constructor.
		//
		// Complexity:	O(N)
		//
		// ////////////////////////////////////////////////////////////
		explicit DynamicArray(Iterator &begin, Iterator &end) : m_arr(NULL), m_size(0), m_capacity(0), m_id(0)
		{
			if(!end)
			{
				m_capacity = (end.m_index - begin.m_index) * 2;
			}
			else
			{
				m_capacity = (end.m_index - begin.m_index + 1) * 2;
			}
			Init();
			if(m_capacity > 0)
			{
				CopyRange(begin, end, Begin());
			}
		};

		// ////////////////////////////////////////////////////////////
		// Destructor.
		//
		// ////////////////////////////////////////////////////////////
		~DynamicArray() { try { Destroy(); } catch(...) {} };

		// ////////////////////////////////////////////////////////////
		// Assignment operator.
		//
		// Complexity:	average O(N), worst O(N^2).
		//
		// ////////////////////////////////////////////////////////////
		void operator=(const DynamicArray &rhs)
		{
			if(this == &rhs)
			{
				return;
			}

			// Clear the previous contents.
			Clear();

			m_size = rhs.m_size;
			SetCapacity(rhs.m_capacity);
			if(m_capacity > 0)
			{
				DynamicArray &nonConstRhs = const_cast<DynamicArray &>(rhs);
				CopyRange(nonConstRhs.Begin(), nonConstRhs.End(), Begin());
			}
		};

		// ////////////////////////////////////////////////////////////
		// An Iterator pointing to the beginning of the array.
		//
		// Complexity:	O(1)
		//
		// ////////////////////////////////////////////////////////////
		Iterator Begin() { return (Iterator(0, this)); };

		// ////////////////////////////////////////////////////////////
		// An iterator pointing to the end of the array (no element here).
		//
		// Complexity:	O(1)
		//
		// ////////////////////////////////////////////////////////////
		Iterator End() { return (Iterator(m_size, this));	};

		// ////////////////////////////////////////////////////////////
		// Get the total capacity of the array.
		//
		// Complexity:	O(1)
		//
		// ////////////////////////////////////////////////////////////
		U64 GetCapacity() const { return (m_capacity); };

		// ////////////////////////////////////////////////////////////
		// Sets a new capacity for the array.
		//
		// If the capacity is greater than the current capacity, an
		// array of the new size will be allocated and the
		// existing data will be copied over.
		//
		// If the capacity is smaller than the prevous array, data at the
		// end of the array will be lost, however no allocations will take
		// place and the I32ernal array will not be resized.
		//
		// Complexity:	best O(1), worst O(N)
		// - Will perform a memory allocation and an array copy in the
		// worst case.
		//
		// ////////////////////////////////////////////////////////////
		void SetCapacity(const U64 capacity)
		{
			if(capacity > m_capacity)
			{
				// Allocate new array, copy over data from old array and deallocate the old array.
				m_capacity = capacity;

				ElementType *tmpArr = AllocateArray(m_capacity);
				if(m_size > 0)
				{
					ArrayCopy(m_arr, m_size, tmpArr);
				}

				DeallocateArray(m_arr);
				m_arr = tmpArr;
			}
			else if(capacity < m_capacity)
			{
				// we wont allocate a smaller array in this case but we will reduce the array size.
				if(capacity < m_size)
				{
					m_size = capacity;
				}
			}
		};

		// ////////////////////////////////////////////////////////////
		// Get the current size of the array.
		//
		// Complexity:	O(1)
		//
		// ////////////////////////////////////////////////////////////
		U64 GetSize() const { return (m_size); };

		// ////////////////////////////////////////////////////////////
		// Sets the new size of the array.  If the new size is greater
		// than the current capacity, then an array copy and reallocation
		// will take place.
		//
		// Complexity: O(1) Best, O(N) worst
		//
		// @param size The new size of the array.
		//
		// ////////////////////////////////////////////////////////////
		void SetSize(const U64 size)
		{
			if(size > m_capacity)
			{
				SetCapacity(size*2);
			}
			m_size = size;
		};

		// ////////////////////////////////////////////////////////////
		// Get the unique ID of the array.
		//
		// Complexity:	O(1)
		//
		// ////////////////////////////////////////////////////////////
		U64 GetUniqueId() const { return (m_id); };

		// ////////////////////////////////////////////////////////////
		// Append an element to the end of the array.
		//
		// Complexity: O(1) average, O(N) worst case.
		// - Will perform a memory allocation and an array copy in the
		// worst case.  Prevent this with proper usage of SetCapacity()!
		//
		// @param val The new value to append.
		//
		// @return n/a
		//
		// ////////////////////////////////////////////////////////////
		void PushBack(const ElementType &val)
		{
			++m_size;
			if(m_size >= m_capacity)
			{
				SetCapacity(m_capacity * 2);
			}
			m_arr[m_size-1] = val;
		};

		// ////////////////////////////////////////////////////////////
		// Pop the last element from the end of the array.
		//
		// Complexity:	O(1)
		//
		// ////////////////////////////////////////////////////////////
		void PopBack()
		{
			if(m_size > 0)
			{
				--m_size;
			}
		};

		// ////////////////////////////////////////////////////////////
		// Gets the element at the back of the array.
		//
		// Complexity:	O(1)
		//
		// ////////////////////////////////////////////////////////////
		ElementType Back() const { return (m_arr[m_size-1]); };

		// ////////////////////////////////////////////////////////////
		// Gets the element at the front of the array.
		//
		// Complexity:	O(1)
		//
		// ////////////////////////////////////////////////////////////
		ElementType Front() const {	return (m_arr[0]); };

		// ////////////////////////////////////////////////////////////
		// Gets the element at the index specified.  Index bounds checking
		// is performed here.
		//
		// Complexity:	O(1)
		//
		// @param index The index of the element you wish to retrieve.
		//
		// @return bool True|False on success|error.
		//
		// ////////////////////////////////////////////////////////////
		bool At(const U64 index, ElementType &val) const
		{
			if(index >= m_size)
			{
				return (false);
			}

			val = m_arr[index];
			return (true);
		};

		// ////////////////////////////////////////////////////////////
		// Gets or sets the element at the index specified.  No bounds
		// checking is performed here.
		//
		// Accessing an element beyond the size of the array leads to
		// undefined behaviour.
		//
		// Complexity:	O(1)
		//
		// @param index The index of the array.
		//
		// @return ElementType& Readable/writeable element alias.
		//
		// ////////////////////////////////////////////////////////////
		ElementType &operator[](const U64 index) { return (m_arr[index]); };

		// ////////////////////////////////////////////////////////////
		// Clears the array.  Does not modify the capacity!
		//
		// Complexity:	O(1)
		//
		// ////////////////////////////////////////////////////////////
		void Clear() { m_size = 0; };

		// ////////////////////////////////////////////////////////////
		// Is the array empty?
		//
		// Complexity:	O(1)
		//
		// ////////////////////////////////////////////////////////////
		bool IsEmpty() const { return (m_size == 0); };

		// ////////////////////////////////////////////////////////////
		// Inserts the element into the array at the position specified.
		//
		// Will attempt to save processing time by taking the old value
		// at the iterator position and adding it onto the back of the
		// array.
		//
		// If you want behaviour like std::vector::insert() or you want
		// to keep the array sorted then you should use InsertAndMove()
		// which inserts the new value and pushes all the elements after
		// up by 1.
		//
		// Note: Invalidates all exising iterators!
		//
		// Complexity:	average: O(1), worst: O(N) plus a reallocation.
		//
		// @param val The value to insert.
		// @param position The position to insert the value to.
		//
		// @return bool True|False on success|failure.
		//
		// ////////////////////////////////////////////////////////////
		bool Insert(const ElementType &val, Iterator &position)
		{
			// That iterator points to a different DynamicArray!
			if(position.m_arr->GetUniqueId() != m_id)
			{
				return (false);
			}

			bool result(false);
			if(position < End())
			{
				ElementType prevVal = *position;
				*position = val;
				PushBack(prevVal);
				result = true;
			}
			// Insert element at the end, no swap or memmove required.
			else
			{
				PushBack(val);
				result = true;
			}

			return (result);
		};

		// ////////////////////////////////////////////////////////////
		// Inserts the element into the array at the requested position
		// and moves all the following elements up by 1 place.
		//
		// Consider using Insert() instead if you don't need to preserve
		// the current order of elements.
		//
		// Complexity:	average: O(N/2), worst: O(N)
		//
		// Note: Invalidates all exising iterators!
		//
		// @param val The value to insert.
		// @param position The position to insert the value to.
		//
		// @return bool True|False on success|failure.
		//
		// ////////////////////////////////////////////////////////////
		bool InsertAndMove(const ElementType &val, Iterator &position)
		{
			// That iterator points to a different DynamicArray!
			if(position.m_arr->GetUniqueId() != m_id)
			{
				return (false);
			}
            
			bool result(false);
			Iterator end(End());
			if(position < end)
			{
				// Allow room for 1 more element in the current array (resizes array capacity if nessecary).
				SetSize(m_size + 1);

				// Create a new array of the required size.
				ElementType *arrCopy = AllocateArray(m_capacity);

				U64 firstHalfSize = position.m_index - Begin().m_index + 1;
				U64 secondHalfSize = end.m_index - position.m_index;

				// Copy all elements from old array to the new from Begin() to "position"-1
				ArrayCopy(m_arr, firstHalfSize, arrCopy);

				// Copy all elements from old array to the new from "position" to End().
				ArrayCopy(m_arr+(firstHalfSize-1), secondHalfSize, arrCopy+(firstHalfSize));

				// Insert new element at position, "position"
				arrCopy[position.m_index] = val;

				DeallocateArray(m_arr);
				m_arr = arrCopy;
				result = true;
			}
			else
			{
				PushBack(val);
				result = true;
			}

			return (result);
		};

		// ////////////////////////////////////////////////////////////
		// Remove the element at the position specified and moves all
		// elements afterwards back by 1.
		// 
		// Note: Invalidates all exising iterators!
		//
		// Complexity: average: O(N/2).
		//
		// @param position The element to remove.
		//
		// @return bool True|False on success|error.
		//
		// ////////////////////////////////////////////////////////////
		bool Remove(Iterator &position)
		{
			if(position < End())
			{
				ElementType tmp;
				for(U64 i(position.m_index), end(m_size - 1); i < end; ++i)
				{
					ElementType a = m_arr[i], b = m_arr[i+1];
					m_arr[i] = m_arr[i+1];
				}
			}
			PopBack();
			return (true);
		};

		// ////////////////////////////////////////////////////////////
		// Copies a range of elements from this array or another array
		// into this array starting from a positon you specify.  Will 
		// overwrite data within the range limits in the original array.
		//
		// Note: Invalidates all exising iterators!
		//
		// Complexity: average O(N), worst O(N^2).
		//
		// @param begin The begin position to copy.
		// @param end The last position to copy.
		// @param startPos The starting position in this array to copy
		// the range of data into.
		//
		// @return bool True|False on success|error.
		//
		// ////////////////////////////////////////////////////////////
		bool CopyRange(Iterator &begin, Iterator &end, Iterator &startPos)
		{
			// Starting position must be somewhere in this array!
			if(startPos.m_arr->GetUniqueId() != m_id)
			{
				return (false);
			}

			// The range must be in the same array!
			if(begin.m_arr->GetUniqueId() != end.m_arr->GetUniqueId())
			{
				return (false);
			}

			if(end < begin)
			{
				// invalid range.
				return (false);
			}

			// Ensure the dest array is the minimum size and has enough capacity to store the range to copy.
			U64 requiredSize(0);
			if(!end)
			{
				requiredSize = end.m_index - begin.m_index;
			}
			else
			{
				requiredSize = end.m_index - begin.m_index + 1;
			}
			startPos.m_arr->SetSize(requiredSize);

			Iterator i(begin), endRange(end+1), startPosCopy(startPos);
			while(i != endRange)
			{
				*startPosCopy = *i;
				++i;
				++startPosCopy;
			}

			return (true);
		};
		
	};

	template<typename ElementType>
		U32 DynamicArray<ElementType>::DYNAMIC_ARRAY_COUNT = 1;
}

#endif