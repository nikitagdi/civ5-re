/// origin binary
#include "FirePlace/FireWorks/FTimer.h"

/// 0x08dc2180
double FTimer::Read( __int64 offset ) {
    if ( m_fFreq == 0.0 ) {
        QueryPerformanceFrequency( (LARGE_INTEGER*)&m_qRate );
        QueryPerformanceCounter( (LARGE_INTEGER*)&m_qMark );
        m_fFreq = 1.0;
        if ( m_qRate != 0 ) {
            m_fFreq = 1.0 / (double)m_qRate;
        }
    }
    __int64 now;
    QueryPerformanceCounter( (LARGE_INTEGER*)&now );
    if ( offset == 0 ) {
        offset = m_qMark;
    }
    return m_fFreq * (double)( now - offset );
}
