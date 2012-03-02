#ifndef _GF_LINKED_LIST_H
#define _GF_LINKED_LIST_H

// ////////////////////////////////////////////////////////////
// @file LinkedList.h
// @author PJ O Halloran
// @date 27/03/2011
//
// Header for the template LinkedList container class(es).
//
// ////////////////////////////////////////////////////////////


// External Headers


// Project Headers
#include "GameBase.h"


// ////////////////////////////////////////////////////////////
//
//
// ////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// ////////////////////////////////////////////////////////////
	// @class LinkedList
	// @author PJ O Halloran
	// 
	// A doubly linked list.
	//
	// Note: List is not circular.
	//
	// advantages:
	// - Ultra fast adding and removing of elements from anywhere in 
	// the container.
	//
	// disadvantages:
	// - The elements are not stored contiguosly in memory.
	// - Requires a little extra memory to manage the elements
	// in the list per list element.
	//
	// ////////////////////////////////////////////////////////////
	template<typename ElementType>
		class LinkedList
	{
	private:

		// ////////////////////////////////////////////////////////////
		// @struct ListNode
		// @author PJ O Halloran
		//
		// A node in a linked list.
		//
		// ////////////////////////////////////////////////////////////
		struct ListNode
		{
			ListNode *m_prev;						///< Previous node in list.
			ListNode *m_next;						///< Next node in list.
			ElementType m_elem;						///< Element.
			U32 m_listId;							///< ID of the list the node was allocated for.

			// ////////////////////////////////////////////////////////////
			// Constructor.
			//
			// ////////////////////////////////////////////////////////////
			ListNode() : m_prev(NULL), m_next(NULL), m_elem(), m_listId(0) {};
		};

	public:

		// ////////////////////////////////////////////////////////////
		// @class Iterator.
		// @author PJ O Halloran
		//
		// Iterator for the doubly linked list.  Similar in
		// functionality to an STL bidirectional iterator.
		//
		// Tip:
		// Be careful about keeping Iterators around too long.
		// In general, if you perform any operations on the LinkedList
		// that are non const then the Iterators will become
		// invalidated.  The result of using invalidated iterators is 
		// undefined.
		//
		// ////////////////////////////////////////////////////////////
		class Iterator
		{
			// Allow LinkedList class only to access Iterators private members.
			friend class LinkedList;

		private:

			ListNode *m_currNode;					///< Node the iterator is pointing at.

		public:

			// ////////////////////////////////////////////////////////////
			// Constructor.
			//
			// ////////////////////////////////////////////////////////////
			Iterator() : m_currNode(NULL) {};

			// ////////////////////////////////////////////////////////////
			// Constructor.
			//
			// ////////////////////////////////////////////////////////////
			explicit Iterator(ListNode *nodePtr) : m_currNode(nodePtr) {};

			// ////////////////////////////////////////////////////////////
			// Copy constructor.
			//
			// ////////////////////////////////////////////////////////////
			Iterator(const Iterator &copyObj) : m_currNode(copyObj.m_currNode) {};

			// ////////////////////////////////////////////////////////////
			// Destructor.
			//
			// ////////////////////////////////////////////////////////////
			~Iterator() {};

			// ////////////////////////////////////////////////////////////
			// Assignment operator.
			//
			// ////////////////////////////////////////////////////////////
			void operator=(const Iterator &rhs)
			{
				if(this == &rhs)
				{
					return;
				}

				m_currNode = rhs.m_currNode;	
			};

			// ////////////////////////////////////////////////////////////
			// Equivalent operator.
			//
			// ////////////////////////////////////////////////////////////
			bool operator==(const Iterator &rhs) const
			{
				if(!m_currNode || !rhs.m_currNode)
				{
					return (false);
				}

				return ((m_currNode->m_listId == rhs.m_currNode->m_listId) &&\
							(m_currNode->m_next == rhs.m_currNode->m_next) &&\
							(m_currNode->m_prev == rhs.m_currNode->m_prev) &&\
							(m_currNode->m_elem == rhs.m_currNode->m_elem));
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
			// Validate current element.  Ensure the current node is not NULL
			// nor the next node is not NULL.
			//
			// Tip:
			// If this test fails then using the Iterators -> or * operators
			// will cause a seg fault!
			//
			// ////////////////////////////////////////////////////////////
			bool operator!() const
			{
				return (NULL == m_currNode || NULL == m_currNode->m_next);
			};

			// ////////////////////////////////////////////////////////////
			// Get a pointer to the current element.
			//
			// Note: Will cause a seg fault if the current node is NULL.
			//
			// ////////////////////////////////////////////////////////////
			ElementType *operator->() const
			{
				return (&m_currNode->m_elem);
			};

			// ////////////////////////////////////////////////////////////
			// Get a reference to the current element.
			//
			// Note: Will cause a seg fault if the current node is NULL.
			//
			// ////////////////////////////////////////////////////////////
			ElementType &operator*() const
			{
				return (m_currNode->m_elem);
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
				// Only inc. if we are not pointing at the TAIL element in the list.
				if(m_currNode && m_currNode->m_next)
				{
					m_currNode = m_currNode->m_next;
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
				// Only dec. if we are not pointing at the first element in the list (we never point to the HEAD, Begin = First element).
				if(m_currNode && m_currNode->m_prev && (NULL != m_currNode->m_prev->m_prev))
				{
					m_currNode = m_currNode->m_prev;
				}
				return (*this);
			};

			// ////////////////////////////////////////////////////////////
			// Increment operator.
			//
			// Complexity: O(N) (N=offset).
			//
			// ////////////////////////////////////////////////////////////
			Iterator operator+(const U64 offset) const
			{
				Iterator copy(*this);
				copy += offset;
				return (copy);
			};

			// ////////////////////////////////////////////////////////////
			// Increment operator.
			//
			// Complexity: O(N) (N=offset).
			//
			// ////////////////////////////////////////////////////////////
			Iterator &operator+=(const U64 offset)
			{
				U64 i(0);
				while(i<offset)
				{
					++(*this);
					++i;
				}
				return (*this);
			};

			// ////////////////////////////////////////////////////////////
			// Decrement operator.
			//
			// Complexity: O(N) (N == offset).
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
			// Complexity: O(N) (N == offset).
			//
			// ////////////////////////////////////////////////////////////
			Iterator &operator-=(const U64 offset)
			{
				U64 i(0);
				while(i<offset)
				{
					--(*this);
					++i;
				}
				return (*this);
			};

			// ////////////////////////////////////////////////////////////
			// Is this iterator earlier in the linked list than the
			// iterator on the rhs?
			//
			// Complexity (Worst case):	O(N^2)
			//	- When this iterator and the rhs iterator both point to the
			//		first element in the list.
			//
			// ////////////////////////////////////////////////////////////
			bool operator<(Iterator &rhs) const
			{
				if(this == &rhs || !m_currNode || m_currNode->m_listId != rhs.m_currNode->m_listId)
				{
					return (false);
				}

				Iterator tmp(*this), rhsCopy(rhs);
				U64 ti(0), ri(0);

				// Check which iterator is the furthest from the end of the list.
				while(tmp.m_currNode->m_next)
				{
					++tmp;
					++ti;
				}
				while(rhsCopy.m_currNode->m_next)
				{
					++rhsCopy;
					++ri;
				}

				// > operator used here as we are looking for the iterator which is furthest from the END...
				return (ti > ri);
			};
		};

	private:

		ListNode *m_headPtr;					///< The first node of the list (special node - no element value).
		ListNode *m_tailPtr;					///< The last node of the list (special node - no element value).
		U32 m_id;											///< Unique id of this list instance.

		static U32 LINKED_LIST_COUNT;						///< Count of the number of instances of LinkedList objects.

		// ////////////////////////////////////////////////////////////
		// Allocate memory for a node.
		//
		// (Right now we allocate off the global OS heap, later we will
		// define a custom allocater to use here - a pool type allocater
		// would be ideal for a linked list).
		//
		// ////////////////////////////////////////////////////////////
		ListNode *AllocateNode() const
		{
			ListNode *nodePtr = GCC_NEW ListNode();
			if(nodePtr)
			{
				nodePtr->m_elem = ElementType();
				nodePtr->m_listId = m_id;
			}
			return (nodePtr);
		};

		// ////////////////////////////////////////////////////////////
		// Free the memory for a node.
		//
		// ////////////////////////////////////////////////////////////
		void DeallocateNode(ListNode *nodePtr) const
		{
			if(nodePtr && nodePtr->m_listId == m_id)
			{
				delete nodePtr;
				nodePtr = NULL;
			}
		};

		// ////////////////////////////////////////////////////////////
		// Constructor helper function.
		//
		// ////////////////////////////////////////////////////////////
		void Init()
		{
			m_id = LINKED_LIST_COUNT;
			++LINKED_LIST_COUNT;

			m_headPtr = AllocateNode();
			m_tailPtr = AllocateNode();

			m_headPtr->m_prev = NULL;
			m_headPtr->m_next = m_tailPtr;

			m_tailPtr->m_prev = m_headPtr;
			m_tailPtr->m_next = NULL;
		};

		// ////////////////////////////////////////////////////////////
		// Desctructor helper function.
		//
		// ////////////////////////////////////////////////////////////
		void Destroy()
		{
			Clear();

			DeallocateNode(m_headPtr);
			DeallocateNode(m_tailPtr);
		};

		// ////////////////////////////////////////////////////////////
		// Helper function for all methods that add/insert elements.
		//
		// @param value The value to insert.
		// @param position Position to insert the new value at.
		//
		// ////////////////////////////////////////////////////////////
		void InsertHelper(ListNode *nodePtr, Iterator &position)
		{
			if(!nodePtr)
			{
				return;
			}

			// Insert the new node before the node at the Iterator position.
			nodePtr->m_next = position.m_currNode;
			nodePtr->m_prev = position.m_currNode->m_prev;

			// Update the next and prev pointers on the nodes around the new node so that it is properly inserted.
			position.m_currNode->m_prev->m_next = nodePtr;
			position.m_currNode->m_prev = nodePtr;
		};

		// ////////////////////////////////////////////////////////////
		// Helper function for all methods that pop/remove elements.
		//
		// @param nodeToRemovePtr The node to remove.
		//
		// ////////////////////////////////////////////////////////////
		void RemoveHelper(ListNode *nodeToRemovePtr)
		{
			if(IsEmpty() || !nodeToRemovePtr || nodeToRemovePtr->m_listId != m_id)
			{
				return;
			}

			// Update the pointers of the nodes around the removed node.
			nodeToRemovePtr->m_prev->m_next = nodeToRemovePtr->m_next;
			nodeToRemovePtr->m_next->m_prev = nodeToRemovePtr->m_prev;

			DeallocateNode(nodeToRemovePtr);
		}

	public:

		// ////////////////////////////////////////////////////////////
		// Default constructor.
		//
		// ////////////////////////////////////////////////////////////
		explicit LinkedList() : m_headPtr(NULL), m_tailPtr(NULL) { Init(); };

		// ////////////////////////////////////////////////////////////
		// Copy constructor.
		//
		// Complexity: O(N).
		//
		// ////////////////////////////////////////////////////////////
		explicit LinkedList(const LinkedList &listObj) : m_headPtr(NULL), m_tailPtr(NULL)
		{
			Init();

			LinkedList<ElementType> &nonConstList = const_cast<LinkedList &>(listObj);
			InsertRange(nonConstList.Begin(), nonConstList.End(), Begin());
		};

		// ////////////////////////////////////////////////////////////
		// Construct a list from all the elements in another list
		// referred to by the iterators.
		//
		/// Complexity: O(N).
		//
		// ////////////////////////////////////////////////////////////
		explicit LinkedList(Iterator &begin, Iterator &end) : m_headPtr(NULL), m_tailPtr(NULL)
		{
			Init();
			InsertRange(begin, end, Begin());
		};

		// ////////////////////////////////////////////////////////////
		// Destructor.
		//
		// ////////////////////////////////////////////////////////////
		~LinkedList() { try { Destroy(); } catch(...) {} };

		// ////////////////////////////////////////////////////////////
		// Assign the contents of a LinkedList to this list.
		//
		// Complexity: O(N^2).
		//
		// ////////////////////////////////////////////////////////////
		void operator=(const LinkedList &rhs)
		{
			Clear();
			LinkedList<ElementType> &nonConstList = const_cast<LinkedList &>(rhs);
			InsertRange(nonConstList.Begin(), nonConstList.End(), Begin());
		};

		// ////////////////////////////////////////////////////////////
		// An iterator referring to the first element in the list.
		//
		// Complexity: O(1).
		//
		// ////////////////////////////////////////////////////////////
		Iterator Begin() { return (Iterator(m_headPtr->m_next)); };

		// ////////////////////////////////////////////////////////////
		// An iterator referring to the end of the list marker (no element).
		//
		// Complexity: O(1).
		//
		// ////////////////////////////////////////////////////////////
		Iterator End() { return (Iterator(m_tailPtr)); };

		// ////////////////////////////////////////////////////////////
		// Insert a new element at a specific position in the list.
		//
		// Complexity: O(1).
		//
		// @param element The value to insert.
		// @param position The position to insert at.
		//
		// @return bool True if the element was inserted and false if there
		//				was a problem.
		//
		// ////////////////////////////////////////////////////////////
		bool Insert(const ElementType &element, Iterator &position)
		{
			// The iterator position has to refer to this list.
			if(position.m_currNode->m_listId != m_id)
			{
				return (false);
			}

			// Create the new element.
			ListNode *nodePtr = AllocateNode();
			if(!nodePtr)
			{
				return (false);
			}
			nodePtr->m_elem = element;

			InsertHelper(nodePtr, position);
			
			return (true);
		};

		// ////////////////////////////////////////////////////////////
		// Copy a range of elements onto the end of the list.
		//
		// Complexity: O(N) (where N is the number of elements between
		// begin and end).
		//
		// @param begin 
		// @param end 
		//
		// ////////////////////////////////////////////////////////////
		void InsertRange(Iterator &begin, Iterator &end, Iterator &position)
		{
			if((end < begin) ||\
				(end.m_currNode->m_listId != begin.m_currNode->m_listId))
			{
				return;
			}

			Iterator beginCopy(begin);
			while(beginCopy != end)
			{
				Insert(*beginCopy, position);
				++beginCopy;
			}
		};

		// ////////////////////////////////////////////////////////////
		// Remove and element from the list from a specific position.
		//
		// Complexity: O(1)
		//
		// @param position The position of the element to remove.
		//
		// ////////////////////////////////////////////////////////////
		void Remove(Iterator &position)
		{
			if(position == End())
			{
				PopBack();
			}
			else
			{
				RemoveHelper(position.m_currNode);
			}
		};

		// ////////////////////////////////////////////////////////////
		// Remove a range of elements from the list.
		//
		// Complexity: O(N) (N = number of elements between begin and end).
		//
		// @param begin The start position to remove.
		// @param end The end position to remove.
		//
		// ////////////////////////////////////////////////////////////
		bool RemoveRange(Iterator &begin, Iterator &end)
		{
			if((end < begin) || (end.m_currNode->m_listId != m_id) || (begin.m_currNode->m_listId != m_id) || begin == End())
			{
				return (false);
			}

			Iterator endCopy(end), beginCopy(begin);

			// Dont remove the tail node ever!
			if(endCopy == End())
			{
				--endCopy;
			}

			ListNode *prevRangeNode(begin.m_currNode->m_prev);
			ListNode *endRangeNode(endCopy.m_currNode->m_next);

			// Break the link outside the range and exclude the nodes to be removed and freed from the list.
			prevRangeNode->m_next = endRangeNode;
			if(endRangeNode)
			{
				endRangeNode->m_prev = prevRangeNode;
			}

			// Deallocate all nodes in the range.
			while(beginCopy != endCopy)
			{
				Iterator i(beginCopy);
				++beginCopy;
				DeallocateNode(i.m_currNode);
			}

			return (true);
		};

		// ////////////////////////////////////////////////////////////
		// Add an element to the end of the list.
		//
		// Complexity: O(1).
		//
		// @param val The value to append to the list.
		//
		// @return bool True|False on success|error.
		//
		// ////////////////////////////////////////////////////////////
		bool PushBack(const ElementType val)
		{
			ListNode *newEndPtr(AllocateNode());
			if(!newEndPtr)
			{
				// Alloc error!
				return (false);
			}

			// Assign the value to the new node.
			newEndPtr->m_elem = val;

			InsertHelper(newEndPtr, End());

			return (true);
		};

		// ////////////////////////////////////////////////////////////
		// Remove the element at the end of the list.
		//
		// Complexity: O(1).
		//
		// @return bool False if there were no elements in the list.
		//
		// ////////////////////////////////////////////////////////////
		bool PopBack()
		{
			// No elements at the beginning of the list.
			if(m_tailPtr->m_prev == m_headPtr)
			{
				return (false);
			}

			RemoveHelper(m_tailPtr->m_prev);

			return (true);
		};

		// ////////////////////////////////////////////////////////////
		// Add an element to the start of the list.
		//
		// Complexity: O(1).
		//
		// @param val The value to prepend to the list.
		//
		// @return bool True|false on success|error.
		//
		// ////////////////////////////////////////////////////////////
		bool PushFront(const ElementType val)
		{
			ListNode *newNodePtr(AllocateNode());
			if(!newNodePtr)
			{
				// Alloc error!
				return (false);
			}

			// Assign the value to the new node.
			newNodePtr->m_elem = val;

			InsertHelper(newNodePtr, Begin());

			return (true);
		};

		// ////////////////////////////////////////////////////////////
		// Remove an element from the beginning of the list.
		//
		// Complexity: O(1).
		//
		// @return bool False if no elements in the list.
		//
		// ////////////////////////////////////////////////////////////
		bool PopFront()
		{
			// No elements at the beginning of the list.
			if(m_headPtr->m_next == m_tailPtr)
			{
				return (false);
			}

			RemoveHelper(m_headPtr->m_next);

			return (true);
		};

		// ////////////////////////////////////////////////////////////
		// Get the element at the start of the list.
		//
		// Complexity: O(1).
		//
		// @param frontVal Contains the value of the node at the front
		//					if the list was not empty.
		//
		// @return bool False if the list was empty.
		//
		// ////////////////////////////////////////////////////////////
		bool Front(ElementType &frontVal)
		{
			if(IsEmpty())
			{
				return (false);
			}

			frontVal = m_headPtr->m_next->m_elem;
			return (true);
		};

		// ////////////////////////////////////////////////////////////
		// Get the element at the end of the list.
		//
		// ////////////////////////////////////////////////////////////
		bool Back(ElementType &frontVal)
		{
			if(IsEmpty())
			{
				return (false);
			}

			frontVal = m_tailPtr->m_prev->m_elem;
			return (true);
		};

		// ////////////////////////////////////////////////////////////
		// Clear the list.
		//
		// Complexity: O(N).
		//
		// ////////////////////////////////////////////////////////////
		void Clear()
		{
			bool removed(true);
			do
			{
				removed = PopBack();
			} while(removed);
		};

		// ////////////////////////////////////////////////////////////
		// Get the size of the list.
		//
		// Complexity: O(N)
		//
		// ////////////////////////////////////////////////////////////
		U64 Size()
		{
			U64 size(0);
			for(Iterator i = Begin(), end = End(); i != end; ++i)
			{
				++size;
			}
			return (size);
		};

		// ////////////////////////////////////////////////////////////
		// Check if the list is empty.
		//
		// Complexity: O(1).
		//
		// ////////////////////////////////////////////////////////////
		bool IsEmpty() const
		{
			return (m_headPtr->m_next == m_tailPtr && m_tailPtr->m_prev == m_headPtr);
		};

	};

	template<typename ElementName>
		U32 LinkedList<ElementName>::LINKED_LIST_COUNT = 1;
}

#endif