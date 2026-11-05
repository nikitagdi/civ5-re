/// origin binary
/*
Set of classes which plug into FFastList, FFastTree, etc. to control element allocation.
These are not general allocators, but specific to the FFastX data structures.
*/

#include "FirePlace/FireWorks/FFastVector.h"

#ifndef FFAST_ALLOCATOR_H
#define FFAST_ALLOCATOR_H

#include <assert.h>
#include <limits.h>

//
// Simple allocator based on a vector.  Deleted elements are 
// added to a "free" linked list and recycled.
//

//
// Special fast allocation class which allocated homogeneous classes from a vector.
//
/// size ?
template< class T > struct FastAllocatorNodePolicy
{
	/// inline
	FastAllocatorNodePolicy(){};
	/// inline
	FastAllocatorNodePolicy(const T& x):data(x){};

	//Array of list connectivity
	bool m_bDeleted; ///< +0x0
	//The index of the next node
	unsigned int uiNext; ///< +0x4

	//User data
	T data; ///< +0x8

	//These six functions are required for the function to work
	/// inline
	unsigned int ALLOC_GetNext() const{ return uiNext; };
	/// inline
	void ALLOC_SetNext(unsigned int uiNodeIndex){ uiNext = uiNodeIndex; };
	/// inline
	bool ALLOC_GetDeleted() const{ return m_bDeleted; };
	/// inline
	void ALLOC_SetDeleted(bool bDeleted){ m_bDeleted = bDeleted; };
};

/// size 0x8
struct NullFastAllocatorNodePolicy
{
	//Array of list connectivity
	bool m_bDeleted; ///< +0x0
	//The index of the next node
	unsigned int uiNext; ///< +0x4

	//These six functions are required for the function to work
	/// inline
	unsigned int ALLOC_GetNext() const{ return uiNext; };
	/// inline
	void ALLOC_SetNext(unsigned int uiNodeIndex){ uiNext = uiNodeIndex; };
	/// inline
	bool ALLOC_GetDeleted() const{ return m_bDeleted; };
	/// inline
	void ALLOC_SetDeleted(bool bDeleted){ m_bDeleted = bDeleted; };
};

//
// The allocator has the following template parameters to control behavior:
// 1 - The object type to allocate
// 3 - Whether the internal objects have non-trivial copy constructors
// 4 - The allocation pool to put any dynamic allocations in
// 5 - Allocation pool sub ID
//
/// size ?
template< 
	class T, 
	bool bPODType = false, 
	unsigned int AllocPool = c_eMPoolTypeContainer, 
	unsigned int nSubID = 0,
	class BASE_ALLOC = typename BaseVector< T, bPODType >::FDefaultFastVectorAllocator
> class FFastAllocator
{
	const static unsigned int ms_uiAnchorNodeIndex = 0x0fffffff;
protected:
	typedef FFastVector< T, bPODType, AllocPool, nSubID, BASE_ALLOC > VectorType;
public:

	//
	//Constructor/destructor
	//
	/// inline
	FFastAllocator() 
		: m_uiFirstEmpty(ms_uiAnchorNodeIndex), m_uiSize(0) {};
	/// inline
	FFastAllocator( unsigned int uiReserve ) 
		: m_vec( uiReserve ), m_uiFirstEmpty(ms_uiAnchorNodeIndex), m_uiSize(0) {};
	/// inline
	~FFastAllocator(){ assert( m_uiSize == 0); };

	//
	//Set the allocation pool size to be ( uiResSize * sizeof( T ) )
	//
	/// inline
	void Reserve(unsigned int uiResSize)
	{
		m_vec.reserve(uiResSize);
	}

	//
	//Allocation function - returns index into member array which is to be used
	//
	/// inline
	unsigned int Alloc( const T& x)
	{
		unsigned int uiPos;

		//If there are no pre-allocated spots, get a new one
		if( m_uiFirstEmpty == ms_uiAnchorNodeIndex )
		{
			uiPos = m_vec.size();
			unsigned int uiIndex = m_vec.push_back( x );
			m_vec[uiIndex].ALLOC_SetDeleted(false);
		}

		//Otherwise reuse an old spot after deleting it's previous contents
		else{
			uiPos = m_uiFirstEmpty;
			T* pTemp = &m_vec[uiPos];
			m_uiFirstEmpty = pTemp->ALLOC_GetNext();
			pTemp->~T();
			new( (void*)pTemp )T( x );
			pTemp->ALLOC_SetDeleted(false);
		}
		m_uiSize++;
		return uiPos;
	};

	//
	//Allocation function - returns index into member array which is to be used
	// required if you can't feasibly have a ref to the element
	//
	/// inline
	unsigned int Alloc()
	{
		unsigned int uiPos;

		//If there are no pre-allocated spots, get a new one
		if( m_uiFirstEmpty == ms_uiAnchorNodeIndex )
		{
			uiPos = m_vec.size();
			unsigned int uiIndex = m_vec.push_back();
			m_vec[uiIndex].ALLOC_SetDeleted(false);
		}

		//Otherwise reuse an old spot after deleting it's previous contents
		else{
			uiPos = m_uiFirstEmpty;
			T* pTemp = &m_vec[uiPos];
			m_uiFirstEmpty = pTemp->ALLOC_GetNext();
			pTemp->~T();
			new( (void*)pTemp )T();
			pTemp->ALLOC_SetDeleted(false);
		}
		m_uiSize++;
		return uiPos;
	};

	//
	//Recycle function - similar to Alloc except constructor/destructor isn't called
	//    unless adding to the internal vector
	//
	/// inline
	unsigned int Recycle(const T& x)
	{
		unsigned int uiPos;

		//If there are no pre-allocated spots, get a new one
		if( m_uiFirstEmpty == ms_uiAnchorNodeIndex )
		{
			uiPos = m_vec.size();
			unsigned int uiIndex = m_vec.push_back( x );
			m_vec[uiIndex].ALLOC_SetDeleted(false);
		}

		//Otherwise reuse an old spot
		else{
			uiPos = m_uiFirstEmpty;
			T* pTemp = &m_vec[uiPos];
			m_uiFirstEmpty = pTemp->ALLOC_GetNext();
			pTemp->ALLOC_SetDeleted(false);
		}
		m_uiSize++;
		return uiPos;
	};

	//
	// Test whether a given slot has been allocated
	//
	/// inline
	bool is_element_valid(unsigned int uiIndex) const
	{
		return (uiIndex < m_vec.size()) && !m_vec[uiIndex].ALLOC_GetDeleted();
	};
	//
	// Return the number of elements currently allocated
	//
	/// inline
	unsigned int size() const{
		return m_uiSize;
	};
	//
	// return the number of elements which can be allocated in the current memory chunk
	//
	/// inline
	unsigned int max_active_index() const{
		return m_vec.size();
	};
	//
	// return the total number of elements which can be allocated in the current memory chunk
	//
	/// inline
	unsigned int capacity() const{
		return m_vec.capacity();
	};

	//
	// Remove all elements which have been allocated in this allocator.
	//
	/// inline
	void clear(){
		m_uiFirstEmpty = ms_uiAnchorNodeIndex;
		m_vec.clear();
		m_uiSize = 0;
	};

	//
	// Clear and resize to hold zero elements.
	//
	/// inline
	void destroy(){
		clear();
		m_vec.setsize(0);
	};

	//
	//Free an index which has already been allocated
	//
	/// inline
	void Free( unsigned int uiIndex )
	{
		T& element = m_vec[uiIndex];
		FAssert( !element.ALLOC_GetDeleted() );
		element.ALLOC_SetDeleted(true);

		if( uiIndex < m_uiFirstEmpty){
			element.ALLOC_SetNext(m_uiFirstEmpty);
			m_uiFirstEmpty = uiIndex;
		}else{
			T* pRoot = &m_vec[m_uiFirstEmpty];
			element.ALLOC_SetNext( pRoot->ALLOC_GetNext() );
			pRoot->ALLOC_SetNext(uiIndex);
		}
		m_uiSize--;
	};

	//
	//Conditional free - only frees the element if ALLOC_GetDeleted() returns true.
	// Returns true if the element was deleted.
	//
	/// inline
	bool FreeIfDeleted( unsigned int uiIndex )
	{
		T& element = m_vec[uiIndex];
		if( !element.ALLOC_GetDeleted() ){
			return false;
		}

		if( uiIndex < m_uiFirstEmpty){
			element.ALLOC_SetNext(m_uiFirstEmpty);

			m_uiFirstEmpty = uiIndex;
		}else{
			T* pRoot = &m_vec[m_uiFirstEmpty];
			element.ALLOC_SetNext( pRoot->ALLOC_GetNext() );
			pRoot->ALLOC_SetNext(uiIndex);
		}
		m_uiSize--;

		return true;
	};

	//
	// handy overload
	//
	/// inline
	void operator = (const FFastAllocator& rhs)
	{
		m_uiFirstEmpty = rhs.m_uiFirstEmpty;
		m_uiSize	   = rhs.m_uiSize;
		m_vec          = rhs.m_vec;
	}
	/// inline
	T& operator[] ( unsigned int ui ) 
	{
		return m_vec[ui];
	}

	/// inline
	const T& operator[] ( unsigned int ui ) const
	{
		return m_vec[ui];

	}

	// will return 0xFFFFFFFF if not in range
	/// inline
	unsigned int CalcIndex(void* pLoc)
	{
		const unsigned int uiTypeSize = sizeof(VectorType::TYPE);
		const byte* pHead = reinterpret_cast<byte*>(&m_vec.front());
		const byte* pLast = pHead+(m_vec.capacity()*uiTypeSize);
		if( pLoc < pHead || pLoc >= pLast ) return 0xFFFFFFFF;

		return (reinterpret_cast<byte*>(pLoc)-pHead)/uiTypeSize;
	}

	/// inline
	const char* GetStoragePtr()
	{
		return  reinterpret_cast<char*>(&m_vec.front());
	}

	/// inline
	const uint GetActiveStorageSize()
	{
		return m_vec.capacity()*sizeof(VectorType::TYPE);
	}

protected:

	//
	//Member data
	//

	//The first empty spot
	unsigned int m_uiFirstEmpty; ///< +0x0
	unsigned int m_uiSize; ///< +0x4

	//The actual data
	VectorType m_vec; ///< +0x8

	template< class T_, bool bPODType_, unsigned int AllocPool_, unsigned int nSubID_, class BASE_ALLOC_ > 
	friend void* operator new( size_t uiSize, FFastAllocator< T_, bPODType_, AllocPool_, nSubID_, BASE_ALLOC_ >& kAlloc );
};

// Placement new on a FFastAllocator allows allocation and construction to be combined.
/// inline
template< class T, bool bPODType, unsigned int AllocPool, unsigned int nSubID, class BASE_ALLOC >
void* operator new(size_t uiSize, FFastAllocator< T, bPODType, AllocPool, nSubID, BASE_ALLOC >& kAlloc )
{
	unsigned int uiPos;
	void* pBuf;

	//If there are no pre-allocated spots, get a new one
	if( kAlloc.m_uiFirstEmpty == kAlloc.ms_uiAnchorNodeIndex )
	{
		uiPos = kAlloc.m_vec.size();
		pBuf = operator new( uiSize, kAlloc.m_vec );
		static_cast<T*>(pBuf)->ALLOC_SetDeleted(false);
	}

	//Otherwise reuse an old spot after deleting it's previous contents
	else{
		uiPos = kAlloc.m_uiFirstEmpty;
		T* pTemp = &kAlloc.m_vec[uiPos];
		pBuf = static_cast<void*>(pTemp);
		kAlloc.m_uiFirstEmpty = pTemp->ALLOC_GetNext();
		pTemp->~T();
		pTemp->ALLOC_SetDeleted(false);
	}
	kAlloc.m_uiSize++;
	return pBuf;
}


//
// The allocator has the following template parameters to control behavior:
// 1 - The object type to allocate
// 3 - Whether the internal objects have non-trivial copy constructors
// 4 - The allocation pool to put any dynamic allocations in
// 5 - Allocation pool sub ID
//
/// size ?
template< 
	class T,
	unsigned int AllocPool = c_eMPoolTypeContainer, 
	unsigned int nSubID = 0,
	class BASE_ALLOC = typename BaseVector< T, false >::FDefaultFastVectorAllocator
> class FFixedBlockAllocator
{
	const static unsigned int ms_uiAnchorNodeIndex = 0x0fffffff;
public:

	//
	//Constructor/destructor
	//
	/// inline
	FFixedBlockAllocator()
		: m_uiFirstEmpty(ms_uiAnchorNodeIndex), m_uiSize(0), m_pData(NULL), m_uiCapacity(0), m_uiMaxActiveIndex(0)
	{};

	/// inline
	FFixedBlockAllocator( unsigned int uiCapacity ) 
		: m_uiFirstEmpty(ms_uiAnchorNodeIndex), m_uiSize(0), m_pData(NULL), m_uiCapacity(uiCapacity), m_uiMaxActiveIndex(0)
	{
		m_pData = BASE_ALLOC::AllocAligned( sizeof(T) * m_uiCapacity, __alignof(T), AllocPool, nSubID );
	};
	/// inline
	~FFixedBlockAllocator()
	{
		FAssert( m_uiSize == 0);
		clear();
		BASE_ALLOC::FreeAligned( m_pData );
		m_pData = NULL;
	};

	/// inline
	void Reserve( unsigned int uiCapacity )
	{
		if( m_pData )
		{
			FAssert( m_uiSize == 0);
			clear();
			BASE_ALLOC::FreeAligned( m_pData );
			m_pData = NULL;
		}

		m_uiCapacity = uiCapacity;
		m_pData = BASE_ALLOC::AllocAligned( sizeof(T) * m_uiCapacity, __alignof(T), AllocPool, nSubID );
	}

	//
	//Allocation function - returns index into member array which is to be used
	//
	/// inline
	unsigned int Alloc( const T& x)
	{
		if( m_uiSize == m_uiCapacity )
		{
			FAssertMsg(false, "Fixed-size allocator is full!" );
			return UINT_MAX;
		}

		unsigned int uiPos;

		//If there are no pre-allocated spots, get a new one
		if( m_uiFirstEmpty == ms_uiAnchorNodeIndex )
		{
			uiPos = m_uiSize+1;
			new( (void*)&m_pData[uiPos]) T( x );
			m_pData[uiPos].ALLOC_SetDeleted(false);
		}

		//Otherwise reuse an old spot after deleting it's previous contents
		else{
			uiPos = m_uiFirstEmpty;
			T* pTemp = &m_pData[uiPos];
			m_uiFirstEmpty = pTemp->ALLOC_GetNext();
			pTemp->~T();
			new( (void*)pTemp )T( x );
			pTemp->ALLOC_SetDeleted(false);
		}
		m_uiMaxActiveIndex++;
		m_uiSize++;
		return uiPos;
	};

	//
	//Allocation function - returns index into member array which is to be used
	// required if you can't feasibly have a ref to the element
	//
	/// inline
	unsigned int Alloc()
	{
		if( m_uiSize == m_uiCapacity )
		{
			FAssertMsg(false, "Fixed-size allocator is full!" );
			return UINT_MAX;
		}

		unsigned int uiPos;

		//If there are no pre-allocated spots, get a new one
		if( m_uiFirstEmpty == ms_uiAnchorNodeIndex )
		{
			uiPos = m_uiSize+1;
			new( (void*)&m_pData[uiPos]) T();
			m_pData[uiPos].ALLOC_SetDeleted(false);
		}

		//Otherwise reuse an old spot after deleting it's previous contents
		else{
			uiPos = m_uiFirstEmpty;
			T* pTemp = &m_pData[uiPos];
			m_uiFirstEmpty = pTemp->ALLOC_GetNext();
			new( (void*)pTemp )T();
			pTemp->ALLOC_SetDeleted(false);
		}
		m_uiMaxActiveIndex++;
		m_uiSize++;
		return uiPos;
	};

	//
	// Test whether a given slot has been allocated
	//
	/// inline
	bool is_element_valid(unsigned int uiIndex) const
	{
		return (uiIndex < m_uiSize) && !m_pData[uiIndex].ALLOC_GetDeleted();
	};
	//
	// Return the number of elements currently allocated
	//
	/// inline
	unsigned int size() const{
		return m_uiSize;
	};
	//
	// Return the max index value that could be allocated
	//
	/// inline
	unsigned int max_active_index() const{
		return m_uiMaxActiveIndex;
	};
	//
	// return the total number of elements which can be allocated in the current memory chunk
	//
	/// inline
	unsigned int capacity() const{
		return m_uiCapacity;
	};

	//
	// Remove all elements which have been allocated in this allocator.
	//
	/// inline
	void clear(){
		while( m_uiFirstEmpty != ms_uiAnchorNodeIndex )
			Free( m_uiFirstEmpty );
	};

	//
	//Free an index which has already been allocated
	//
	/// inline
	void Free( unsigned int uiIndex )
	{
		if( uiIndex >= m_uiMaxActiveIndex )
		{
			FAssert(false);
			return;
		}

		T& element = m_pData[uiIndex];
		FAssert( !element.ALLOC_GetDeleted() );
		element.ALLOC_SetDeleted(true);
		element.~T();

		if( uiIndex < m_uiFirstEmpty){
			element.ALLOC_SetNext(m_uiFirstEmpty);
			m_uiFirstEmpty = uiIndex;
		}else{
			T* pRoot = &m_pData[m_uiFirstEmpty];
			element.ALLOC_SetNext( pRoot->ALLOC_GetNext() );
			pRoot->ALLOC_SetNext(uiIndex);
		}
		m_uiSize--;
		if( uiIndex == m_uiMaxActiveIndex-1 ) m_uiMaxActiveIndex--;
	};

	//
	//Conditional free - only frees the element if ALLOC_GetDeleted() returns true.
	// Returns true if the element was deleted.
	//
	/// inline
	bool FreeIfDeleted( unsigned int uiIndex )
	{
		T& element = m_pData[uiIndex];
		if( !element.ALLOC_GetDeleted() ){
			return false;
		}

		if( uiIndex < m_uiFirstEmpty){
			element.ALLOC_SetNext(m_uiFirstEmpty);

			m_uiFirstEmpty = uiIndex;
		}else{
			T* pRoot = &m_pData[m_uiFirstEmpty];
			element.ALLOC_SetNext( pRoot->ALLOC_GetNext() );
			pRoot->ALLOC_SetNext(uiIndex);
		}
		m_uiSize--;
		if( uiIndex == m_uiMaxActiveIndex-1 ) m_uiMaxActiveIndex--;

		return true;
	};


	//Don't allow copying
private:
	/// inline
	void operator = (const FFixedBlockAllocator& rhs){}


public:
	/// inline
	T& operator[] ( unsigned int ui ) 
	{
		FAssert( ui < m_uiMaxActiveIndex );
		FAssert( !m_pData[ui].ALLOC_GetDeleted() );
		return m_pData[ui];
	}

	/// inline
	const T& operator[] ( unsigned int ui ) const
	{
		FAssert( ui < m_uiMaxActiveIndex );
		FAssert( !m_pData[ui].ALLOC_GetDeleted() );
		return m_pData[ui];
	}

	// will return 0xFFFFFFFF if not in range
	/*unsigned int CalcIndex(void* pLoc)
	{
		const unsigned int uiTypeSize = sizeof(VectorType::TYPE);
		const byte* pHead = reinterpret_cast<byte*>(&m_vec.front());
		const byte* pLast = pHead+(m_vec.capacity()*uiTypeSize);
		if( pLoc < pHead || pLoc >= pLast ) return 0xFFFFFFFF;

		return (reinterpret_cast<byte*>(pLoc)-pHead)/uiTypeSize;
	}*/

protected:

	//
	//Member data
	//

	//The first empty spot
	unsigned int m_uiSize; ///< +0x0
	unsigned int m_uiFirstEmpty; ///< +0x4
	unsigned int m_uiCapacity; ///< +0x8
	unsigned int m_uiMaxActiveIndex; ///< +0xc

	//The actual data
	T* m_pData; ///< +0x10

	template< class T_, unsigned int AllocPool_, unsigned int nSubID_, class BASE_ALLOC_ >
	friend void* operator new( size_t uiSize, FFixedBlockAllocator< T_, AllocPool_, nSubID_, BASE_ALLOC_ >& kAlloc );
};

// Placement new on a FFastAllocator allows allocation and construction to be combined.
/// inline
template< class T, unsigned int AllocPool, unsigned int nSubID, class BASE_ALLOC >
void* operator new(size_t uiSize, FFixedBlockAllocator< T, AllocPool, nSubID, BASE_ALLOC >& kAlloc )
{
	if( kAlloc.m_uiSize == kAlloc.m_uiCapacity )
	{
		FAssertMsg(false, "Fixed-size allocator is full!" );
		return (void*)UINT_MAX;
	}

	unsigned int uiPos;

	//If there are no pre-allocated spots, get a new one
	if( kAlloc.m_uiFirstEmpty == kAlloc.ms_uiAnchorNodeIndex )
	{
		uiPos = kAlloc.m_uiSize+1;
		kAlloc.m_pData[uiPos].ALLOC_SetDeleted(false);
	}

	//Otherwise reuse an old spot after deleting it's previous contents
	else{
		uiPos = kAlloc.m_uiFirstEmpty;
		T* pTemp = &kAlloc.m_pData[uiPos];
		kAlloc.m_uiFirstEmpty = pTemp->ALLOC_GetNext();
		pTemp->ALLOC_SetDeleted(false);
	}
	kAlloc.m_uiMaxActiveIndex++;
	kAlloc.m_uiSize++;

	return &kAlloc.m_pData[uiPos];
}

#endif
