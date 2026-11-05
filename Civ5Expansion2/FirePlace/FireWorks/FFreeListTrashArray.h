/// origin binary
//------------------------------------------------------------------------------------------------
//
//  *****************   FIRAXIS GAME ENGINE   ********************
//
//  FILE:    FFreeListTrashArray.h
//
//  AUTHOR:  Soren Johnson
//
//  PURPOSE: A dynamic array with a free list that keeps track of its own memory...
//
//------------------------------------------------------------------------------------------------
//  Copyright (c) 2007 Firaxis Games, Inc. All rights reserved.
//------------------------------------------------------------------------------------------------

#ifndef		FFREELISTTRASHARRAY_H
#define		FFREELISTTRASHARRAY_H
#pragma		once

#include "FirePlace/FireWorks/FDataStream.h"
#include <assert.h>
#include "FirePlace/FireWorks/FMemHooks.h"

#include	"FirePlace/FireWorks/FFreeListArrayBase.h"

#define FLTA_ID_SHIFT		(13)
#define FLTA_MAX_BUCKETS	(1 << FLTA_ID_SHIFT)
#define FLTA_INDEX_MASK		(FLTA_MAX_BUCKETS - 1)
#define FLTA_ID_MASK		(~(FLTA_INDEX_MASK))
#define FLTA_GROWTH_FACTOR	(2)

/// size ?
/// vtable ?
template <class T>
class FFreeListTrashArray : public FFreeListArrayBase<T>
{
public:

	/// inline
	FFreeListTrashArray();
	/// inline
	virtual ~FFreeListTrashArray();
								
	/// inline
	virtual void Init(int iNumSlots = 8);
	/// inline
	virtual void Uninit();
	/// inline
	virtual T* GetAt(int iID);
	/// inline
	virtual const T* GetAt(int iID) const;

	/// inline
	T* Add();
	/// inline
	bool Remove(T* pData);
	/// inline
	bool RemoveAt(int iID);
	/// inline
	virtual void RemoveAll();

	/// inline
	void Load(T* pData);

	/// inline
	int GetNumSlots() const
	{
		return this->m_iNumSlots;
	}

	/// inline
	int GetLastIndex() const
	{
		return this->m_iLastIndex;
	}
	/// inline
	void SetLastIndex(int iNewValue)
	{
		this->m_iLastIndex = iNewValue;
	}

	/// inline
	int GetFreeListHead() const
	{
		return this->m_iFreeListHead;
	}
	/// inline
	void SetFreeListHead(int iNewValue)
	{
		this->m_iFreeListHead = iNewValue;
	}

	/// inline
	int GetFreeListCount() const
	{
		return this->m_iFreeListCount;
	}
	/// inline
	void SetFreeListCount(int iNewValue)
	{
		this->m_iFreeListCount = iNewValue;
	}

	/// inline
	int GetCurrentID() const
	{
		return m_iCurrentID;
	}
	/// inline
	void SetCurrentID(int iNewValue)
	{
		assert((iNewValue & FLTA_INDEX_MASK) == 0);
		assert((iNewValue & FLTA_ID_MASK) != 0);
		m_iCurrentID = iNewValue;
	}

	/// inline
	int GetNextFreeIndex(int iIndex) const
	{
		if ((iIndex >= GetNumSlots()) || (m_pArray == NULL))
		{
			assert(false);
			return FFreeList::INVALID_INDEX;
		}
		return m_pArray[iIndex].iNextFreeIndex;
	}
	/// inline
	void SetNextFreeIndex(int iIndex, int iNewValue)
	{
		if ((iIndex >= GetNumSlots()) || (m_pArray == NULL))
		{
			assert(false);
			return;
		}
		m_pArray[iIndex].iNextFreeIndex = iNewValue;
	}

	/// inline
	void Read( FDataStream* pStream );
	/// inline
	void Write( FDataStream* pStream ) const;

protected:

	/// size ?
	struct FFreeListTrashArrayNode
	{
		int iNextFreeIndex; ///< +0x0
		T* pData; ///< +0x4
	};

	int m_iCurrentID; ///< +0x14
	FFreeListTrashArrayNode* m_pArray; ///< +0x18

	/// inline
	virtual void GrowArray();
};



// Public functions...

/// inline
template <class T>
FFreeListTrashArray<T>::FFreeListTrashArray()
{
	m_iCurrentID = FLTA_MAX_BUCKETS;
	m_pArray = NULL;
}


/// inline
template <class T>
FFreeListTrashArray<T>::~FFreeListTrashArray()
{
	Uninit();
}


/// inline
template <class T>
void FFreeListTrashArray<T>::Init(int iNumSlots)
{
	int iCount;
	int iI;

	assert(iNumSlots >= 0);

	// make sure it's binary...
	if ((iNumSlots > 0) && ((iNumSlots - 1) & iNumSlots) != 0)
	{
		// find high bit
		iCount = 0;
		while (iNumSlots != 1)
		{
			iNumSlots >>= 1;
			iCount++;
		}
		iNumSlots = (1 << (iCount + 1));
	}

	assert(((iNumSlots - 1) & iNumSlots) == 0);
	assert((this->m_iNumSlots <= FLTA_MAX_BUCKETS) && "FFreeListTrashArray<T>::init() size too large");

	Uninit();

	this->m_iNumSlots = iNumSlots;
	this->m_iLastIndex = FFreeList::INVALID_INDEX;
	this->m_iFreeListHead = FFreeList::INVALID_INDEX;
	this->m_iFreeListCount = 0;
	m_iCurrentID = FLTA_MAX_BUCKETS;

	if (this->m_iNumSlots > 0)
	{
		m_pArray = FNEW( FFreeListTrashArrayNode[this->m_iNumSlots], c_eMPoolTypeContainer, 0 );

		for (iI = 0; iI < this->m_iNumSlots; iI++)
		{
			m_pArray[iI].iNextFreeIndex = FFreeList::INVALID_INDEX;
			m_pArray[iI].pData = NULL;
		}
	}
}


/// inline
template <class T>
void FFreeListTrashArray<T>::Uninit()
{
	if (m_pArray != NULL)
	{
		RemoveAll();

		SAFE_DELETE_ARRAY(m_pArray);
	}
}


/// inline
template <class T>
T* FFreeListTrashArray<T>::Add()
{
	int iIndex;

	if (m_pArray == NULL) 
	{
		Init();
	}

	if ((this->m_iLastIndex == this->m_iNumSlots - 1) &&
		(this->m_iFreeListCount == 0))
	{
		if ((this->m_iNumSlots * FLTA_GROWTH_FACTOR) > FLTA_MAX_BUCKETS)
		{
			return NULL;
		}

		GrowArray();
	}

	if (this->m_iFreeListCount > 0)
	{
		iIndex = this->m_iFreeListHead;
		this->m_iFreeListHead = m_pArray[this->m_iFreeListHead].iNextFreeIndex;
		this->m_iFreeListCount--;
	}
	else
	{
		this->m_iLastIndex++;
		iIndex = this->m_iLastIndex;
	}

	m_pArray[iIndex].pData = FNEW( T, c_eMPoolTypeContainer, 0 );
	m_pArray[iIndex].iNextFreeIndex = FFreeList::INVALID_INDEX;

	m_pArray[iIndex].pData->SetID(m_iCurrentID + iIndex);
	m_iCurrentID += FLTA_MAX_BUCKETS;

	return m_pArray[iIndex].pData;
}


/// inline
template <class T>
T* FFreeListTrashArray<T>::GetAt(int iID)
{
	int iIndex;

	if ((iID == FFreeList::INVALID_INDEX) || (m_pArray == NULL))
	{
		return NULL;
	}

	iIndex = (iID & FLTA_INDEX_MASK);

	assert(iIndex >= 0);

	if ((iIndex <= this->m_iLastIndex) && 
		(m_pArray[iIndex].pData != NULL))
	{
		if (((iID & FLTA_ID_MASK) == 0) || (m_pArray[iIndex].pData->GetID() == iID))
		{
			return m_pArray[iIndex].pData;
		}
	}

	return NULL;
}

/// inline
template <class T>
const T* FFreeListTrashArray<T>::GetAt(int iID) const
{
	int iIndex;

	if ((iID == FFreeList::INVALID_INDEX) || (m_pArray == NULL))
	{
		return NULL;
	}

	iIndex = (iID & FLTA_INDEX_MASK);

	assert(iIndex >= 0);

	if ((iIndex <= this->m_iLastIndex) && 
		(m_pArray[iIndex].pData != NULL))
	{
		if (((iID & FLTA_ID_MASK) == 0) || (m_pArray[iIndex].pData->GetID() == iID))
		{
			return m_pArray[iIndex].pData;
		}
	}

	return NULL;
}

/// inline
template <class T>
bool FFreeListTrashArray<T>::Remove(T* pData)
{
	int iI;

	assert(m_pArray != NULL);

	if (pData != NULL)
	{
		for (iI = 0; iI <= this->m_iLastIndex; iI++)
		{
			if (m_pArray[iI].pData == pData)
			{
				return RemoveAt(iI);
			}
		}
	}

	return false;
}


/// inline
template <class T>
bool FFreeListTrashArray<T>::RemoveAt(int iID)
{
	int iIndex;

	if ((iID == FFreeList::INVALID_INDEX) || (m_pArray == NULL))
	{
		return false;
	}

	iIndex = (iID & FLTA_INDEX_MASK);

	assert(iIndex >= 0);

	if ((iIndex <= this->m_iLastIndex) && 
		(m_pArray[iIndex].pData != NULL))
	{
		if (((iID & FLTA_ID_MASK) == 0) || (m_pArray[iIndex].pData->GetID() == iID))
		{
			delete m_pArray[iIndex].pData;
			m_pArray[iIndex].pData = NULL;

			m_pArray[iIndex].iNextFreeIndex = this->m_iFreeListHead;
			this->m_iFreeListHead = iIndex;
			this->m_iFreeListCount++;

			return true;
		}
		else
		{
			assert(false);
		}
	}

	return false;
}


/// inline
template <class T>
void FFreeListTrashArray<T>::RemoveAll()
{
	int iI;

	if (m_pArray == NULL)
	{
		return;
	}

	this->m_iLastIndex = FFreeList::INVALID_INDEX;
	this->m_iFreeListHead = FFreeList::INVALID_INDEX;
	this->m_iFreeListCount = 0;

	for (iI = 0; iI < this->m_iNumSlots; iI++)
	{
		m_pArray[iI].iNextFreeIndex = FFreeList::INVALID_INDEX;
		if (m_pArray[iI].pData != NULL)
		{
			delete m_pArray[iI].pData;
		}
		m_pArray[iI].pData = NULL;
	}
}


/// inline
template <class T>
void FFreeListTrashArray<T>::Load(T* pData)
{
	assert(pData != NULL);
	assert(m_pArray != NULL);
	if (!pData || !m_pArray) return;

	assert((pData->GetID() & FLTA_ID_MASK) < m_iCurrentID);

	int iIndex = (pData->GetID() & FLTA_INDEX_MASK);

	assert(iIndex < FLTA_MAX_BUCKETS);
	assert(iIndex <= this->m_iLastIndex);
	assert(m_pArray[iIndex].pData == NULL);
	assert(m_pArray[iIndex].iNextFreeIndex == FFreeList::INVALID_INDEX);

	m_pArray[iIndex].pData = pData;
}

// Protected functions...

/// inline
template <class T>
void FFreeListTrashArray<T>::GrowArray()
{
	FFreeListTrashArrayNode* pOldArray;
	int iOldNumSlots;
	int iI;

	assert(m_pArray != NULL);
	
	if(!m_pArray) return;

	pOldArray = m_pArray;
	iOldNumSlots = this->m_iNumSlots;

	this->m_iNumSlots *= FLTA_GROWTH_FACTOR;
	assert((this->m_iNumSlots <= FLTA_MAX_BUCKETS) && "FFreeListTrashArray<T>::growArray() size too large");
	m_pArray = FNEW( FFreeListTrashArrayNode[this->m_iNumSlots], c_eMPoolTypeContainer, 0 );

	for (iI = 0; iI < this->m_iNumSlots; iI++)
	{
		if (iI < iOldNumSlots)
		{
			m_pArray[iI] = pOldArray[iI];
		}
		else
		{
			m_pArray[iI].iNextFreeIndex = FFreeList::INVALID_INDEX;
			m_pArray[iI].pData = NULL;
		}
	}

	delete [] pOldArray;
}

//
// use when list contains non-streamable types
//
/// inline
template < class T >
inline void FFreeListTrashArray< T >::Read( FDataStream* pStream )
{
	int iTemp;
	*pStream >> iTemp;
	Init( iTemp );
	*pStream >> iTemp;
	SetLastIndex( iTemp );
	*pStream >> iTemp;
	SetFreeListHead( iTemp );
	*pStream >> iTemp;
	SetFreeListCount( iTemp );
	*pStream >> iTemp;
	SetCurrentID( iTemp );

	int i;

	for ( i = 0; i < GetNumSlots(); i++ )
	{
		*pStream >> iTemp;
		SetNextFreeIndex( i, iTemp );
	}

	int iCount;
	*pStream >> iCount;

	for ( i = 0; i < iCount; i++ )
	{
		T* pData = FNEW( T, c_eMPoolTypeContainer, 0 );
		pStream->Read( sizeof ( T ), ( byte* )pData );
		Load( pData );
	}
}

/// inline
template < class T >
inline void FFreeListTrashArray< T >::Write( FDataStream* pStream ) const
{
	pStream->Write( GetNumSlots() );
	pStream->Write( GetLastIndex() );
	pStream->Write( GetFreeListHead() );
	pStream->Write( GetFreeListCount() );
	pStream->Write( GetCurrentID() );

	int i;

	for ( i = 0; i < GetNumSlots(); i++ )
	{
		pStream->Write( GetNextFreeIndex( i ) );
	}

	pStream->Write( FFreeListTrashArray<T>::GetCount() );

	for ( i = 0; i < FFreeListTrashArray<T>::GetIndexAfterLast(); i++ )
	{
		if ( GetAt( i ) )
		{
			pStream->Write( sizeof ( T ), ( byte* )GetAt( i ) );
		}
	}
}

//-------------------------------
// Serialization helper templates:
//-------------------------------

//
// use when list contains streamable types
//
/// inline
template < class T >
inline void ReadStreamableFFreeListTrashArray( FFreeListTrashArray< T >& flist, FDataStream* pStream )
{
	int iTemp;
	*pStream >> iTemp;
	flist.Init( iTemp );
	*pStream >> iTemp;
	flist.SetLastIndex( iTemp );
	*pStream >> iTemp;
	flist.SetFreeListHead( iTemp );
	*pStream >> iTemp;
	flist.SetFreeListCount( iTemp );
	*pStream >> iTemp;
	flist.SetCurrentID( iTemp );

	int i;

	for ( i = 0; i < flist.GetNumSlots(); i++ )
	{
		*pStream >> iTemp;
		flist.SetNextFreeIndex( i, iTemp );
	}

	int iCount;
	*pStream >> iCount;

	for ( i = 0; i < iCount; i++ )
	{
		T* pData = FNEW( T, c_eMPoolTypeContainer, 0 );
		pData->Read( pStream );
		flist.Load( pData );
	}
}

/// inline
template < class T >
inline void WriteStreamableFFreeListTrashArray( FFreeListTrashArray< T >& flist, FDataStream* pStream )
{
	pStream->Write( flist.GetNumSlots() );
	pStream->Write( flist.GetLastIndex() );
	pStream->Write( flist.GetFreeListHead() );
	pStream->Write( flist.GetFreeListCount() );
	pStream->Write( flist.GetCurrentID() );

	int i;

	for ( i = 0; i < flist.GetNumSlots(); i++ )
	{
		pStream->Write( flist.GetNextFreeIndex( i ) );
	}

	pStream->Write( flist.GetCount() );

	for ( i = 0; i < flist.GetIndexAfterLast(); i++ )
	{
		if ( flist[ i ] )
		{
			flist[ i ]->Write( pStream );
		}
	}
}


/// inline
template<typename T>
FDataStream & operator<<(FDataStream & writeTo, const FFreeListTrashArray<T> & readFrom)
{
	writeTo << readFrom.GetNumSlots();
	writeTo << readFrom.GetLastIndex();
	writeTo << readFrom.GetFreeListHead();
	writeTo << readFrom.GetFreeListCount();
	writeTo << readFrom.GetCurrentID();

	int i;

	for ( i = 0; i < readFrom.GetNumSlots(); i++ )
	{
		writeTo << readFrom.GetNextFreeIndex( i );
	}

	writeTo << readFrom.GetCount();

	for ( i = 0; i < readFrom.GetIndexAfterLast(); i++ )
	{
		if ( readFrom[ i ] )
		{
			const T & entry = *(readFrom[i]);
			writeTo << entry;
		}
	}
	return writeTo;
}

/// inline
template<typename T>
FDataStream & operator>>(FDataStream & readFrom, FFreeListTrashArray<T> & writeTo)
{
	int iTemp;
	readFrom >> iTemp;
	writeTo.Init( iTemp );
	readFrom >> iTemp;
	writeTo.SetLastIndex(iTemp);
	readFrom >> iTemp;
	writeTo.SetFreeListHead(iTemp);
	readFrom >> iTemp;
	writeTo.SetFreeListCount(iTemp);
	readFrom >> iTemp;
	writeTo.SetCurrentID(iTemp);

	int i;

	for ( i = 0; i < writeTo.GetNumSlots(); i++ )
	{
		readFrom >> iTemp;
		writeTo.SetNextFreeIndex(i, iTemp);
	}

	int iCount;
	readFrom >> iCount;

	for ( i = 0; i < iCount; i++ )
	{
		T* pData = new T;
		readFrom >> *pData;
		writeTo.Load(pData);
	}
	return readFrom;
}

#endif	//	FFREELISTTRASHARRAY_H

