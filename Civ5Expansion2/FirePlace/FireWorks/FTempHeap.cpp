/// origin binary
#include "FirePlace/FireWorks/FTempHeap.h"

/// 0x08dbf798
byte* TempHeap::Allocate( const uint uiBytes ) {
    const uint uiSize = ( uiBytes + 0xf ) & 0xfffffff0;
    HeapLock::ReadLock_Infinite lock( m_Lock.Acquire() );
    for (;;) {
        const uint uiCurrent = m_uiCurrentOffset;
        if ( uiCurrent + uiSize > m_uiCapacity ) {
            InterlockedDecrement( &m_Lock.Acquire() );
            return NULL;
        }
        if ( InterlockedCompareExchange( &m_uiCurrentOffset, uiCurrent + uiSize, uiCurrent ) == uiCurrent ) {
            return m_pMemHead + uiCurrent;
        }
    }
}

/// 0x08dbf824
void TempHeap::DeAllocate( const byte* pBytes ) {
    if ( pBytes == NULL ) {
        return;
    }
    volatile LONG& uiLock = m_Lock.Acquire();
    InterlockedDecrement( &uiLock );
    InterlockedCompareExchange( &uiLock, 0x10000, 0 );
    if ( ( uiLock & 0x10000 ) != 0 ) {
        InterlockedCompareExchange( &m_uiCurrentOffset, 0, m_uiCurrentOffset );
        InterlockedCompareExchange( &uiLock, 0, uiLock );
    }
}
