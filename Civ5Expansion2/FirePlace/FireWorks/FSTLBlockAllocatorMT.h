/// origin binary
//-----------------------------------------------------------------------------
//
// STL Block Allocator (Thread Safe)
//
//-----------------------------------------------------------------------------
//
// This allocator never frees its internal memory pools; however, it does
// reuse memory when it's available.  As with all STL allocators, all memory
// management is stateless.
//
// In most cases, this behavior won't be an issue since the allocated memory
// will be continuously recycled over the course of the program. However, there
// are situations where this allocator is inappropriate.  For example, this
// allocator shouldn't be used with a container that could spike with an
// unusually high memory footprint in situations such as loading.  It also
// shouldn't be used in one-use situations where the memory is intended to be
// released to the OS.
//
//-----------------------------------------------------------------------------
//
// This allocator is thread safe.
//
//-----------------------------------------------------------------------------
// Copyright (c) 2007 Firaxis Games, Inc. All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __FIRAXIS_STL_BLOCK_ALLOCATOR_MT_H__
#define __FIRAXIS_STL_BLOCK_ALLOCATOR_MT_H__

#include "FirePlace/FireWorks/FFireTypes.h"

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

	#include "FirePlace/FireWorks/FMemHooks.h"
	#include "FirePlace/FireWorks/FCriticalSection.h"

//-----------------------------------------------------------------------------
// Expose The Original Memory Management Functions
//-----------------------------------------------------------------------------

	//#include "FUnDefNew.h"

//-----------------------------------------------------------------------------
/// 0x08db3774
FCriticalSection & FSTLBlockAllocatorMTGetCriticalSection();

//-----------------------------------------------------------------------------
// STL Block Allocator
//-----------------------------------------------------------------------------

/// size ?
template <typename T, unsigned int uBlockCountT, unsigned int pool_type, unsigned int uAlignment = __alignof(T)>
class FSTLBlockAllocatorMT
{
	//-------------------------------------------------------------------------
	// Internal Allocator Types (Required)
	//-------------------------------------------------------------------------
	public:

		// Value Type
		typedef T value_type;

		// Pointer Types
		typedef       T * pointer;
		typedef const T * const_pointer;

		// Reference Types
		typedef       T & reference;
		typedef const T & const_reference;

		// Size Types
		typedef size_t    size_type;
		typedef ptrdiff_t difference_type;

	//-------------------------------------------------------------------------
	// Rebind Interface
	//-------------------------------------------------------------------------
	public:

		template <typename U> struct rebind
		{
			typedef FSTLBlockAllocatorMT<U, uBlockCountT, pool_type, uAlignment> other;
		};

	//-------------------------------------------------------------------------
	// Construction
	//-------------------------------------------------------------------------
	public:

		/// inline
		FSTLBlockAllocatorMT() : m_pFreeStack(NULL)
		{
		}

		/// inline
		FSTLBlockAllocatorMT(const FSTLBlockAllocatorMT<T, uBlockCountT, pool_type, uAlignment> & rhs) : m_pFreeStack(NULL)
		{
		}

		template <typename U, unsigned int uBlockCountU, unsigned int pool_typeU, unsigned int uAlignmentU >
		FSTLBlockAllocatorMT(const FSTLBlockAllocatorMT<U, uBlockCountU, pool_typeU, uAlignmentU> & rhs) : m_pFreeStack(NULL)
		{
		}

	//-------------------------------------------------------------------------
	// Destruction
	//-------------------------------------------------------------------------
	public:

		/// inline
		~FSTLBlockAllocatorMT()
		{
		}

	//-------------------------------------------------------------------------
	// Operators
	//-------------------------------------------------------------------------
	public:

		template <typename U, unsigned int uBlockCountU, unsigned int pool_typeU, unsigned int uAlignmentU>
		FSTLBlockAllocatorMT& operator=(const FSTLBlockAllocatorMT<U, uBlockCountU, pool_typeU, uAlignmentU> & rhs)
		{
			return * this;
		}

	//-------------------------------------------------------------------------
	// Standard Allocator Interface (C++ Standard 20.1.5)
	//-------------------------------------------------------------------------
	public:

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		pointer allocate(size_type nCount, const void * pHint)
		{
			FScopedCriticalSection Crit(FSTLBlockAllocatorMTGetCriticalSection());

			if (nCount == 1)
			{
				return static_cast<pointer>(AllocateFromPool());
			}
			else
			{
				//return static_cast<pointer>( FMALLOC(nCount*sizeof(T), c_eMPoolTypeSTL, 0) );
				return static_cast<pointer>( FMALLOCALIGNED(nCount*sizeof(T), (uAlignment == 0) ? __alignof(T) : uAlignment, pool_type, 0) );
				//return static_cast<pointer>(FireMalloc(nCount * sizeof(T)));
			}
		}

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		pointer allocate(size_type nCount)
		{
			FScopedCriticalSection Crit(FSTLBlockAllocatorMTGetCriticalSection());
			return allocate(nCount, NULL);
		}

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		void deallocate(pointer pMemory, size_type nCount)
		{
			FSTLBlockAllocatorMTGetCriticalSection().Enter();

			if (nCount == 1)
			{
				FreeFromPool(pMemory);
			}
			else
			{
				//FFREE(pMemory);
				FFREEALIGNED(pMemory);
			}

			FSTLBlockAllocatorMTGetCriticalSection().Leave();
		}

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		void construct(pointer pMemory, const T & rValue)
		{
			::new (pMemory) T(rValue);
		}

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		void destroy(pointer pObject)
		{
			pObject->~T();
		}

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		pointer address(reference rObject) const
		{
			return & rObject;
		}

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		const_pointer address(const_reference rObject) const
		{
			return & rObject;
		}

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		size_type max_size() const
		{
			size_type nCount = static_cast<size_type>(-1) / sizeof (T); 

			if (nCount)
			{
				return nCount;
			}
			else
			{
				return 1;
			}
		}

	//-------------------------------------------------------------------------
	// Implementation (Stateless)
	//-------------------------------------------------------------------------
	private:

		//---------------------------------------------------------------------
		// Allocates a block from the internal memory pool.
		//---------------------------------------------------------------------
		/// inline
		void * AllocateFromPool()
		{
			if (m_pFreeStack == NULL)
			{
				CreatePool();
			}

			return Pop();
		}

		//---------------------------------------------------------------------
		// Frees a block from the internal memory pool.
		//---------------------------------------------------------------------
		/// inline
		void FreeFromPool(void * pBlock)
		{
			Push(pBlock);
		}

		//---------------------------------------------------------------------
		// Calculates the internal block size.
		//---------------------------------------------------------------------
		/// inline
		size_t GetBlockSize() const
		{
			return MAX(sizeof(T), sizeof(void *));
		}

		//---------------------------------------------------------------------
		// Creates a memory pool based on the template parameters.
		//---------------------------------------------------------------------
		/// inline
		void CreatePool()
		{
			// Note: This memory is never freed.
			char * pMemoryPool = reinterpret_cast<char *>(FMALLOCALIGNED(GetBlockSize() * uBlockCountT, (uAlignment == 0) ? __alignof( T ) : uAlignment, pool_type, 0));

			if (pMemoryPool)
			{   
				for (unsigned int i = 0; i < uBlockCountT; i++)
				{
					Push(pMemoryPool);
					pMemoryPool += GetBlockSize();
				}
			}
		}

		//---------------------------------------------------------------------
		// Pushes a block onto the free stack.
		//---------------------------------------------------------------------
		/// inline
		void Push(void * pBlock)
		{
			*(reinterpret_cast<void **>(pBlock)) = m_pFreeStack;
			m_pFreeStack = pBlock;
		}

		//---------------------------------------------------------------------
		// Pops a block off of the free stack.
		//---------------------------------------------------------------------
		/// inline
		void * Pop()
		{
			void * pBlock = m_pFreeStack;

			if (pBlock)
			{
				m_pFreeStack = *(reinterpret_cast<void **>(pBlock));
			}

			return pBlock;
		}

		void * m_pFreeStack; ///< +0x0
};

//-----------------------------------------------------------------------------
// Non-Member Operators
//-----------------------------------------------------------------------------

/// inline
template <typename T, unsigned int uBlockCountT, unsigned int pool_typeT, unsigned int uAlignmentT, typename U, unsigned int uBlockCountU, unsigned int pool_typeU, unsigned int uAlignmentU>
inline bool operator==(const FSTLBlockAllocatorMT<T, uBlockCountT, pool_typeT, uAlignmentT> & lhs, const FSTLBlockAllocatorMT<U, uBlockCountU, pool_typeU, uAlignmentU> & rhs)
{
	return true;
}

/// inline
template <typename T, unsigned int uBlockCountT, unsigned int pool_typeT, unsigned int uAlignmentT, typename U, unsigned int uBlockCountU, unsigned int pool_typeU, unsigned int uAlignmentU>
inline bool operator!=(const FSTLBlockAllocatorMT<T, uBlockCountT, pool_typeT, uAlignmentT> & lhs, const FSTLBlockAllocatorMT<U, uBlockCountU, pool_typeU, uAlignmentU> & rhs)
{
	return false;
}

//-----------------------------------------------------------------------------
// STL Block Allocator
//-----------------------------------------------------------------------------

/// size ?
template <typename T, unsigned int uBlockCountT, unsigned int pool_type, unsigned int uAlignment = __alignof(T)>
class FSTL_Tagged_Allocator
{
	//-------------------------------------------------------------------------
	// Internal Allocator Types (Required)
	//-------------------------------------------------------------------------
	public:

		// Value Type
		typedef T value_type;

		// Pointer Types
		typedef       T * pointer;
		typedef const T * const_pointer;

		// Reference Types
		typedef       T & reference;
		typedef const T & const_reference;

		// Size Types
		typedef size_t    size_type;
		typedef ptrdiff_t difference_type;

	//-------------------------------------------------------------------------
	// Rebind Interface
	//-------------------------------------------------------------------------
	public:

		template <typename U> struct rebind
		{
			typedef FSTL_Tagged_Allocator<U, uBlockCountT, pool_type, uAlignment> other;
		};

	//-------------------------------------------------------------------------
	// Construction
	//-------------------------------------------------------------------------
	public:

		/// inline
		FSTL_Tagged_Allocator()
		{
		}

		/// inline
		FSTL_Tagged_Allocator(const FSTL_Tagged_Allocator<T, uBlockCountT, pool_type, uAlignment> & rhs)
		{
		}

		template <typename U, unsigned int uBlockCountU, unsigned int pool_typeU, unsigned int uAlignmentU >
		FSTL_Tagged_Allocator(const FSTL_Tagged_Allocator<U, uBlockCountU, pool_typeU, uAlignmentU> & rhs)
		{
		}

	//-------------------------------------------------------------------------
	// Destruction
	//-------------------------------------------------------------------------
	public:

		/// inline
		~FSTL_Tagged_Allocator()
		{
		}

	//-------------------------------------------------------------------------
	// Operators
	//-------------------------------------------------------------------------
	public:

		template <typename U, unsigned int uBlockCountU, unsigned int pool_typeU, unsigned int uAlignmentU>
		FSTL_Tagged_Allocator& operator=(const FSTL_Tagged_Allocator<U, uBlockCountU, pool_typeU, uAlignmentU> & rhs)
		{
			return * this;
		}

	//-------------------------------------------------------------------------
	// Standard Allocator Interface (C++ Standard 20.1.5)
	//-------------------------------------------------------------------------
	public:

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		pointer allocate(size_type nCount, const void * pHint)
		{
			return static_cast<pointer>( FMALLOCALIGNED(nCount*sizeof(T), (uAlignment == 0) ? __alignof(T) : uAlignment, pool_type, 0) );
		}

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		pointer allocate(size_type nCount)
		{
			return static_cast<pointer>( FMALLOCALIGNED(nCount*sizeof(T), (uAlignment == 0) ? __alignof(T) : uAlignment, pool_type, 0) );
		}

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		void deallocate(pointer pMemory, size_type nCount)
		{
			FFREEALIGNED(pMemory);
		}

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		void construct(pointer pMemory, const T & rValue)
		{
			::new (pMemory) T(rValue);
		}

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		void destroy(pointer pObject)
		{
			pObject->~T();
		}

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		pointer address(reference rObject) const
		{
			return & rObject;
		}

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		const_pointer address(const_reference rObject) const
		{
			return & rObject;
		}

		//---------------------------------------------------------------------
		//
		//---------------------------------------------------------------------
		/// inline
		size_type max_size() const
		{
			size_type nCount = static_cast<size_type>(-1) / sizeof (T); 

			if (nCount)
			{
				return nCount;
			}
			else
			{
				return 1;
			}
		}

};

//-----------------------------------------------------------------------------
// Non-Member Operators
//-----------------------------------------------------------------------------

/// inline
template <typename T, unsigned int uBlockCountT, unsigned int pool_typeT, unsigned int uAlignmentT, typename U, unsigned int uBlockCountU, unsigned int pool_typeU, unsigned int uAlignmentU>
inline bool operator==(const FSTL_Tagged_Allocator<T, uBlockCountT, pool_typeT, uAlignmentT> & lhs, const FSTL_Tagged_Allocator<U, uBlockCountU, pool_typeU, uAlignmentU> & rhs)
{
	return true;
}

/// inline
template <typename T, unsigned int uBlockCountT, unsigned int pool_typeT, unsigned int uAlignmentT, typename U, unsigned int uBlockCountU, unsigned int pool_typeU, unsigned int uAlignmentU>
inline bool operator!=(const FSTL_Tagged_Allocator<T, uBlockCountT, pool_typeT, uAlignmentT> & lhs, const FSTL_Tagged_Allocator<U, uBlockCountU, pool_typeU, uAlignmentU> & rhs)
{
	return false;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#endif // Include Guard
