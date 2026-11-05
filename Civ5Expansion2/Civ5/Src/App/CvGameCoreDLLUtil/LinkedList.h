/// origin guess
#pragma once

#include <stddef.h>
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FMemHooks.h"
// LinkedList.h
// A doubly-linked list

#ifndef		LINKEDLIST_H
#define		LINKEDLIST_H
#pragma		once

template <class tVARTYPE> class CLinkList;


/// size ?
/// vtable ?
template <class tVARTYPE> class CLLNode
{

friend class CLinkList<tVARTYPE>;

public:

    /// inline
    CLLNode(const tVARTYPE& val)
          {
	          m_data = val;

	          m_pNext = NULL;
	          m_pPrev = NULL;
          }
	/// inline
	virtual ~CLLNode() {}

	//list of vartype
	tVARTYPE	m_data; ///< +0x4

protected:

	CLLNode<tVARTYPE>*	m_pNext; ///< +0x8
	CLLNode<tVARTYPE>*	m_pPrev; ///< +0xc

};


/// size ?
/// vtable ?
template <class tVARTYPE> class CLinkList
{

public:

	/// inline
	CLinkList();
	/// inline
	virtual ~CLinkList();

	/// inline
	void clear();

	/// inline
	void insertAtBeginning(const tVARTYPE& val);
	/// inline
	void insertAtEnd(const tVARTYPE& val);
	/// inline
	void insertBefore(const tVARTYPE& val, CLLNode<tVARTYPE>* pThisNode);
	/// inline
	void insertAfter(const tVARTYPE& val, CLLNode<tVARTYPE>* pThisNode);
	/// inline
	CLLNode<tVARTYPE>* deleteNode(CLLNode<tVARTYPE>* pNode);
	/// inline
	void moveToEnd(CLLNode<tVARTYPE>* pThisNode);

	/// inline
	const CLLNode<tVARTYPE>* next(const CLLNode<tVARTYPE>* pNode) const;
	/// inline
	CLLNode<tVARTYPE>* next(CLLNode<tVARTYPE>* pNode);

	/// inline
	const CLLNode<tVARTYPE>* prev(const CLLNode<tVARTYPE>* pNode) const;
	/// inline
	CLLNode<tVARTYPE>* prev(CLLNode<tVARTYPE>* pNode);

	/// inline
	CLLNode<tVARTYPE>* nodeNum(int iNum) const;
	/// inline
	void swapUp(int iNum);

	/// inline
	void Read( FDataStream& kStream );
	/// inline
	void Write( FDataStream& kStream ) const;

	/// inline
	int getLength() const
	{
		return m_iLength;
	}

	/// inline
	const CLLNode<tVARTYPE>* head() const
	{
		return m_pHead;
	}

	/// inline
	CLLNode<tVARTYPE>* head()
	{
		return m_pHead;
	}

	/// inline
	const CLLNode<tVARTYPE>* tail() const
	{
		return m_pTail;
	}

	/// inline
	CLLNode<tVARTYPE>* tail()
	{
		return m_pTail;
	}

	/// inline
	bool operator==(const CLinkList & rhs) const;
	/// inline
	bool operator!=(const CLinkList & rhs) const;

protected:
	int m_iLength; ///< +0x4

	CLLNode<tVARTYPE>* m_pHead; ///< +0x8
	CLLNode<tVARTYPE>* m_pTail; ///< +0xc
};



//constructor
//resets local vars
/// inline
template <class tVARTYPE>
inline CLinkList<tVARTYPE>::CLinkList()
{
	m_iLength = 0;

	m_pHead = NULL;
	m_pTail = NULL;
}


//Destructor
//resets local vars
/// inline
template <class tVARTYPE>
inline CLinkList<tVARTYPE>::~CLinkList()
{
  clear();
}


/// inline
template <class tVARTYPE>
inline void CLinkList<tVARTYPE>::clear()
{
	CLLNode<tVARTYPE>* pCurrNode;
	CLLNode<tVARTYPE>* pNextNode;

	pCurrNode = m_pHead;
	while (pCurrNode != NULL)
	{
		pNextNode = pCurrNode->m_pNext;
		SAFE_DELETE(pCurrNode);
		pCurrNode = pNextNode;
	}

	m_iLength = 0;

	m_pHead = NULL;
	m_pTail = NULL;
}


//inserts at the tail of the list
/// inline
template <class tVARTYPE>
inline void CLinkList<tVARTYPE>::insertAtBeginning(const tVARTYPE& val)
{	
	CLLNode<tVARTYPE>* pNode;

	assert((m_pHead == NULL) || (m_iLength > 0));

	pNode = FNEW(CLLNode<tVARTYPE>(val), c_eCiv5GameplayDLL, 0);

	if (m_pHead != NULL)
	{
		m_pHead->m_pPrev = pNode;
		pNode->m_pNext = m_pHead;
		m_pHead = pNode;
	}
	else
	{
		m_pHead = pNode;
		m_pTail = pNode;
	}

	m_iLength++;
}


//inserts at the tail of the list
/// inline
template <class tVARTYPE>
inline void CLinkList<tVARTYPE>::insertAtEnd(const tVARTYPE& val)
{	
	CLLNode<tVARTYPE>* pNode;

	assert((m_pHead == NULL) || (m_iLength > 0));

	pNode = FNEW(CLLNode<tVARTYPE>(val), c_eCiv5GameplayDLL, 0);

	if (m_pTail != NULL)
	{
		m_pTail->m_pNext = pNode;
		pNode->m_pPrev = m_pTail;
		m_pTail = pNode;
	}
	else
	{
		m_pHead = pNode;
		m_pTail = pNode;
	}

	m_iLength++;
}


//inserts before the specified node
/// inline
template <class tVARTYPE>
inline void CLinkList<tVARTYPE>::insertBefore(const tVARTYPE& val, CLLNode<tVARTYPE>* pThisNode)
{
	CLLNode<tVARTYPE>* pNode;

	assert((m_pHead == NULL) || (m_iLength > 0));

	if ((pThisNode == NULL) || (pThisNode->m_pPrev == NULL))
	{
		insertAtBeginning(val);
		return;
	}

	pNode = FNEW(CLLNode<tVARTYPE>(val), c_eCiv5GameplayDLL, 0);

	pThisNode->m_pPrev->m_pNext = pNode;
	pNode->m_pPrev = pThisNode->m_pPrev;
	pThisNode->m_pPrev = pNode;
	pNode->m_pNext = pThisNode;

	m_iLength++;
}


//inserts after the specified node
/// inline
template <class tVARTYPE>
inline void CLinkList<tVARTYPE>::insertAfter(const tVARTYPE& val, CLLNode<tVARTYPE>* pThisNode)
{
	CLLNode<tVARTYPE>* pNode;

	assert((m_pHead == NULL) || (m_iLength > 0));

	if ((pThisNode == NULL) || (pThisNode->m_pNext == NULL))
	{
		insertAtEnd(val);
		return;
	}

	pNode = FNEW(CLLNode<tVARTYPE>(val), c_eCiv5GameplayDLL, 0);

	pThisNode->m_pNext->m_pPrev = pNode;
	pNode->m_pNext              = pThisNode->m_pNext;
	pThisNode->m_pNext          = pNode;
	pNode->m_pPrev			        = pThisNode;

	m_iLength++;
}


/// inline
template <class tVARTYPE>
inline CLLNode<tVARTYPE>* CLinkList<tVARTYPE>::deleteNode(CLLNode<tVARTYPE>* pNode)
{
	CLLNode<tVARTYPE>* pPrevNode;
	CLLNode<tVARTYPE>* pNextNode;

	assert(pNode != NULL);
	if (!pNode) return NULL;

	pPrevNode = pNode->m_pPrev;
	pNextNode = pNode->m_pNext;

	if ((pPrevNode != NULL) && (pNextNode != NULL))
	{
		pPrevNode->m_pNext = pNextNode;
		pNextNode->m_pPrev = pPrevNode;
	}
	else if (pPrevNode != NULL)
	{
		pPrevNode->m_pNext = NULL;
		m_pTail = pPrevNode;
	}
	else if (pNextNode != NULL)
	{
		pNextNode->m_pPrev = NULL;
		m_pHead = pNextNode;
	}
	else
	{
		m_pHead = NULL;
		m_pTail = NULL;
	}

	SAFE_DELETE(pNode);

	m_iLength--;

	return pNextNode;
}


/// inline
template <class tVARTYPE>
inline void CLinkList<tVARTYPE>::moveToEnd(CLLNode<tVARTYPE>* pNode)
{
	CLLNode<tVARTYPE>* pPrevNode;
	CLLNode<tVARTYPE>* pNextNode;

	assert(pNode != NULL);

	if (getLength() == 1)
	{
	return;
	}

	if (pNode == m_pTail)
	{
		return;
	}

	pPrevNode = pNode->m_pPrev;
	pNextNode = pNode->m_pNext;

	if ((pPrevNode != NULL) && (pNextNode != NULL))
	{
		pPrevNode->m_pNext = pNextNode;
		pNextNode->m_pPrev = pPrevNode;
	}
	else if (pPrevNode != NULL)
	{
		pPrevNode->m_pNext = NULL;
		m_pTail = pPrevNode;
	}
	else if (pNextNode != NULL)
	{
		pNextNode->m_pPrev = NULL;
		m_pHead = pNextNode;
	}
	else
	{
		m_pHead = NULL;
		m_pTail = NULL;
	}

	pNode->m_pNext = NULL;
	m_pTail->m_pNext = pNode;
	pNode->m_pPrev = m_pTail;
	m_pTail = pNode;
}


/// inline
template <class tVARTYPE>
inline const CLLNode<tVARTYPE>* CLinkList<tVARTYPE>::next(const CLLNode<tVARTYPE>* pNode) const
{
	assert(pNode != NULL);
	return (pNode) ? (pNode->m_pNext) : (NULL);
}

/// inline
template <class tVARTYPE>
inline CLLNode<tVARTYPE>* CLinkList<tVARTYPE>::next(CLLNode<tVARTYPE>* pNode)
{
	assert(pNode != NULL);
	return (pNode) ? (pNode->m_pNext) : (NULL);
}


/// inline
template <class tVARTYPE>
inline const CLLNode<tVARTYPE>* CLinkList<tVARTYPE>::prev(const CLLNode<tVARTYPE>* pNode) const
{
	assert(pNode != NULL);
	return (pNode) ? (pNode->m_pPrev) : (NULL);
}

/// inline
template <class tVARTYPE>
inline CLLNode<tVARTYPE>* CLinkList<tVARTYPE>::prev(CLLNode<tVARTYPE>* pNode)
{
	assert(pNode != NULL);
	return (pNode) ? (pNode->m_pPrev) : (NULL);
}

/// inline
template <class tVARTYPE>
inline CLLNode<tVARTYPE>* CLinkList<tVARTYPE>::nodeNum(int iNum) const
{
	CLLNode<tVARTYPE>* pNode;
	int iCount;

	iCount = 0;
	pNode = m_pHead;

	while (pNode != NULL)
	{
		if (iCount == iNum)
		{
			return pNode;
		}

		iCount++;
		pNode = pNode->m_pNext;
	}

	return NULL;
}


/// inline
template <class tVARTYPE>
inline void CLinkList<tVARTYPE>::swapUp(int iNum)
{
	CLLNode<tVARTYPE>* pPrevNode;
	CLLNode<tVARTYPE>* pNextNode;
	CLLNode<tVARTYPE>* pNextNextNode;

	if ( iNum > m_iLength - 1 )
		return;

	int iCount = 0;
	CLLNode<tVARTYPE>*pNode = m_pHead;

	while (pNode != NULL)
	{
		if (iCount == iNum)
		{
			break;
		}
		iCount++;
		pNode = pNode->m_pNext;
	}

	pPrevNode = pNode->m_pPrev;
	pNextNode = pNode->m_pNext;
	if (!pNextNode)
		return;
	pNextNextNode = pNextNode->m_pNext;

	if ((pPrevNode != NULL) && (pNextNextNode != NULL))
	{
		pPrevNode->m_pNext = pNextNode;
		pNextNode->m_pPrev = pPrevNode;
		pNextNode->m_pNext = pNode;
		pNode->m_pPrev = pNextNode;
		pNode->m_pNext = pNextNextNode;
		pNextNextNode->m_pPrev = pNode;
	}
	else if ((pPrevNode == NULL) && (pNextNextNode != NULL))
	{
		pNextNode->m_pPrev = NULL;
		pNextNode->m_pNext = pNode;
		pNode->m_pPrev = pNextNode;
		pNode->m_pNext = pNextNextNode;
		pNextNextNode->m_pPrev = pNode;
	}
	else if ((pPrevNode != NULL) && (pNextNextNode == NULL))
	{
		pPrevNode->m_pNext = pNextNode;
		pNextNode->m_pPrev = pPrevNode;
		pNextNode->m_pNext = pNode;
		pNode->m_pPrev = pNextNode;
		pNode->m_pNext = NULL;
	}
	else // ((pPrevNode == NULL) && (pNextNextNode == NULL))
	{
		pNextNode->m_pPrev = NULL;
		pNextNode->m_pNext = pNode;
		pNode->m_pPrev = pNextNode;
		pNode->m_pNext = NULL;
	}
	if (m_pHead == pNode)
	{
		m_pHead = pNextNode;
	}
	if (m_pTail == pNextNode)
	{
		m_pTail = pNode;
	}
}



//
// use when linked list contains non-streamable types
//
/// inline
template < class T >
inline void CLinkList< T >::Read( FDataStream& kStream )
{
	int iLength;
	kStream >>  iLength ;
	clear();

	if ( iLength )
	{
		T* pData = FNEW(T, c_eCiv5GameplayDLL, 0);;
		for ( int i = 0; i < iLength; i++ )
		{
			kStream >>  *pData ;
			insertAtEnd( *pData );
		}
		SAFE_DELETE( pData );
	}
}

/// inline
template < class T >
inline void CLinkList< T >::Write( FDataStream& kStream ) const
{
	int iLength = getLength();
	kStream <<  iLength ;
	const CLLNode< T >* pNode = head();
	while ( pNode )
	{
		kStream << pNode->m_data ;
		pNode = next( pNode );
	}
}

/// inline
template<typename ValueType>
bool CLinkList<ValueType>::operator==(const CLinkList<ValueType> & rhs) const
{
	if(&rhs != this)
	{
		const CLLNode<ValueType> * myNode = head();
		const CLLNode<ValueType> * theirNode = rhs.head();
		while(myNode && theirNode)
		{
			if(myNode->m_data != theirNode->m_data)
				return false;
			myNode = next(myNode);
			theirNode = rhs.next(theirNode);
		}
	}
	return true;
}

/// inline
template<typename ValueType>
bool CLinkList<ValueType>::operator!=(const CLinkList<ValueType> & rhs) const
{
	return !(*this == rhs);
}

/// inline
template<typename ValueType>
FDataStream & operator<<(FDataStream & saveTo, const CLinkList<ValueType> & readFrom)
{
	readFrom.Write(saveTo);
	return saveTo;
}

/// inline
template<typename ValueType>
FDataStream & operator>>(FDataStream & loadFrom, CLinkList<ValueType> & writeTo)
{
	writeTo.Read(loadFrom);
	return loadFrom;
}
#endif	//LINKEDLIST_H
