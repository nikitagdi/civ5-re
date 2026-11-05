/// origin binary
//------------------------------------------------------------------------------------------------
//
//  ***************** FIRAXIS GAME ENGINE   ********************
//
//! \file		FAtomic.h
//! \author		Bart Muzzin -- 2008/04/14
//! \brief		Platform neutral defines for atomic processor operations
//
//------------------------------------------------------------------------------------------------
//  Copyright (c) 2008 Firaxis Games, Inc. All rights reserved.
//------------------------------------------------------------------------------------------------

#ifndef FAtomic_h
#define FAtomic_h

#include "FirePlace/FireWorks/FFireTypes.h"

// Terrible performance versions for win32, because they don't have these natively
#if defined(_WINPC) 
	#ifndef _WIN64
	//------------------------------------------------------------------------------------------------
	/// absent
	LONGLONG _FAtomicIncr64(LONGLONG volatile* pLLIncr );
	/// absent
	LONGLONG _FAtomicDecr64(LONGLONG volatile* pLLDecr );
	#else // (_WIN32_WINNT < 0x0502)
	#define _FAtomicIncr64(x) InterlockedIncrement64(x)
	#define _FAtomicDecr64(x) InterlockedDecrement64(x)
	#endif
#endif

#if defined(_WINPC) 
	// Increment
	#define FAtomicIncr(x)			InterlockedIncrement(&x)
	#define FAtomicIncr64(x)		_FAtomicIncr64(&x)
	// Decrement
	#define FAtomicDecr(x)			InterlockedDecrement(&x)
	#define FAtomicDecr64(x)		_FAtomicDecr64(&x)
	// Add
	#define FAtomicAdd(x, a)		InterlockedExchangeAdd(&x, a)
	// And
	#define FAtomicAnd(x, a)		InterlockedAnd(&x, a)
	// Or
	#define FAtomicOr(x, a)			InterlockedOr(&x, a)
	// CompareExchange
	#define FAtomicCAX(x, a, b)		InterlockedCompareExchange(&x, a, b)
    #define FAtomicCAXPtr(x, a, b)  InterlockedCompareExchangePointer((void**)&x, a, b)

#elif defined(_XENON)
	// Increment
	#define FAtomicIncr(x)			InterlockedIncrement(&x)
	#define FAtomicIncr64(x)		InterlockedIncrement64(&x)
	// Decrement
	#define FAtomicDecr(x)			InterlockedDecrement(&x)
	#define FAtomicDecr64(x)		InterlockedDecrement64(&x)
	// Add
	#define FAtomicAdd(x, a)		InterlockedExchangeAdd(&x, a)
	// And
	#define FAtomicAnd(x, a)		InterlockedAnd(&x, a)
	// Or
	#define FAtomicOr(x, a)			InterlockedOr(&x, a)
	// CompareExchange
	#define FAtomicCAX(x, a, b)		InterlockedCompareExchange(&x, a, b)
    #define FAtomicCAXPtr(x, a, b)  InterlockedCompareExchangePointer((void**)&x, a, b)
#elif defined(_PS3)
	// Note:  these are PPU versions but there is an SPU version as well
	// Increment
	#define FAtomicIncr(x)			cellAtomicIncr32(&x)
	#define FAtomicIncr64(x)		cellAtomicIncr64((std::uint64_t*)&x)
	// Decrement
	#define FAtomicDecr(x)			cellAtomicDecr32(&x)
	#define FAtomicDecr64(x)		cellAtomicDecr64((std::uint64_t*)&x)
	// Add
	#define FAtomicAdd(x, a)		cellAtomicAdd32(&x, a)
	// And
	#define FAtomicAnd(x, a)		cellAtomicAnd32(&x, a)
	// Or
	#define FAtomicOr(x, a)			cellAtomicOr32(&x, a)
	// CompareExchange
	#define FAtomicCAX(x, a, b)		cellAtomicCompareAndSwap32(&x, b, a)
    #define FAtomicCAXPtr(x, a, b)  cellAtomicCompareAndSwap32(&x, b, a)
#endif

//-------------------------------------------------------------------------------------------------------------------------------
// Handy little multiple reader single writer lock class which can be pulled into a system if needed
// currently very "crude" We still need multi lock for writes / reads on the same thread and a better non blocking setup
// as this usage is to spin infinitely.
// ReadWriteLock needs to be renamed to clarify that it's multi read, single write.
#ifndef FXS_IS_DLL
/// size 0x10
class ReadWriteLock
{
public:

    /// inline
    ReadWriteLock() : m_uiLock(0){};

    enum { eWriteShift = 16 };

    //-----------------------------------------------------------
    /// size 0x4
    class WriteLock_Infinite
    {
    public:

        /// inline
        WriteLock_Infinite(volatile LONG& uiLockRef) : m_uiLock(uiLockRef)
        {
            while(1)
            {
                volatile uint uiLock = uiLockRef;
                if( (uiLock == 0) && (FAtomicCAX(uiLockRef, (uiLock+(1<<eWriteShift)), uiLock) == uiLock) )
                    return;
            }
        };
        /// inline
        ~WriteLock_Infinite() {m_uiLock=0;} // one writer allowed at a time so this does not need to be atomic

        volatile LONG& m_uiLock; ///< +0x0
    };

    //-----------------------------------------------------------
    /// size 0x4
    class WriteLock_Single
    {
    public:

        /// inline
        WriteLock_Single(volatile LONG& uiLockRef) : m_uiLock(uiLockRef)
        {
            FAtomicCAX(uiLockRef, (1<<eWriteShift), 0);
        };

        /// inline
        BOOL IsLocked() const { return m_uiLock&(1<<eWriteShift);}

        /// inline
        ~WriteLock_Single() {m_uiLock=0;} // one writer allowed at a time so this does not need to be atomic

        volatile LONG& m_uiLock; ///< +0x0
    };

    //-----------------------------------------------------------
    /// size 0x4
    class ReadLock_Infinite
    {
    public:
        /// inline
        ReadLock_Infinite(volatile LONG& uiLockRef) : m_uiLock(uiLockRef)
        {
            while(1)
            {
                volatile uint uiLock = uiLockRef;
                if( (uiLock<1<<eWriteShift) && (FAtomicCAX(uiLockRef, (uiLock+1), uiLock) == uiLock) )
                    return;
            }
        };

        /// inline
        ~ReadLock_Infinite() {FAtomicDecr(m_uiLock);}

        volatile LONG& m_uiLock; ///< +0x0
    };


    //-----------------------------------------------------------
    //-----------------------------------------------------------
    typedef WriteLock_Infinite WRITE_LOCK_INFINITE;
    typedef WriteLock_Single   WRITE_LOCK_SINGLE;
    typedef ReadLock_Infinite  READ_LOCK_INFINITE;

    //-----------------------------------------------------------
    /// inline
    volatile LONG& Acquire() const { return m_uiLock; }

    __declspec( align(16)) volatile mutable LONG m_uiLock; ///< +0x0
};

// AtomicLock, blocks for read or write
// LOCK_INFINITE the nested class, acquires m_uiLock during its constructor
// and sets it back to 0 in the destructor.
/// size 0x10
class AtomicLock
{
public:

    /// inline
    AtomicLock() : m_uiLock(0){};

    //-----------------------------------------------------------
    /// size 0x4
    class ScopedLock_Infinite
    {
    public:
        /// inline
        ScopedLock_Infinite(volatile LONG& uiLockRef) : m_uiLock(uiLockRef)
        {
            while(FAtomicCAX(uiLockRef, 1, 0) != 0){}
        };

        /// inline
        ~ScopedLock_Infinite() {m_uiLock = 0;}

        volatile LONG& m_uiLock; ///< +0x0
    };


    //-----------------------------------------------------------
    //-----------------------------------------------------------
    typedef volatile ScopedLock_Infinite SCOPED_LOCK_INFINITE;
    typedef volatile LONG LOCK_MEMBER_TYPE;

    //-----------------------------------------------------------
    /// inline
    volatile LONG& Acquire() const { return m_uiLock; }

    __declspec( align(16)) volatile mutable LONG m_uiLock; ///< +0x0
};
#endif

#endif // FAtomic_h
