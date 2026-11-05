/// origin binary
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"

/// size ?
template <class T0, class T1, unsigned int T2>
class FRangeCoder {
public:
    /// 0x08cfc122 FRangeCoder<FFastVector<unsigned char, true, 10u, 0u, BaseVector<unsigned char, true>::FDefaultFastVectorAllocator>, unsigned int, 4u>
    UNK_RET EncodeShift(T1 a1, T1 a2, T1 a3);
    /// 0x08cfc2ec FRangeCoder<FFastVector<unsigned char, true, 10u, 0u, BaseVector<unsigned char, true>::FDefaultFastVectorAllocator>, unsigned int, 4u>
    UNK_RET EncodeRange(T1 a1, T1 a2, T1 a3);
    /// 0x08cfc4b8 FRangeCoder<FFastVector<unsigned char, true, 10u, 0u, BaseVector<unsigned char, true>::FDefaultFastVectorAllocator>, unsigned int, 4u>
    UNK_RET SyncEncoder();
    /// 0x08cfc62e FRangeCoder<FFastVector<unsigned char, true, 10u, 0u, BaseVector<unsigned char, true>::FDefaultFastVectorAllocator>, unsigned int, 4u>
    UNK_RET RestartDecoder();
};
