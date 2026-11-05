/// origin binary
#include "FirePlace/FireWorks/FCrc32.h"

/// 0x08dc00ac
dword FCRC::Calc( const void *pBuf, int nLen, dword crc ) const {
    const byte *pData = static_cast<const byte *>(pBuf);
    while ( nLen != 0 ) {
        crc = (crc >> 8) ^ m_table[(crc & 0xff) ^ *pData];
        ++pData;
        --nLen;
    }
    return crc;
}
