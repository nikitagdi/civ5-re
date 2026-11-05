/// origin binary
#include "FirePlace/FireWorks/FCriticalSection.h"

/// 0x08dc23e4
FCriticalSection::FCriticalSection( void ) {
    InitializeCriticalSection( &m_kCriticalSection );
}

/// 0x08dc23f8
FCriticalSection::~FCriticalSection( void ) {
    DeleteCriticalSection( &m_kCriticalSection );
}

/// 0x08dc245c
void FCriticalSection::Enter( void ) {
    EnterCriticalSection( &m_kCriticalSection );
}

/// 0x08dc2470
void FCriticalSection::Leave( void ) {
    LeaveCriticalSection( &m_kCriticalSection );
}
