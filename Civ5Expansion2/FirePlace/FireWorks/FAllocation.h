/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"

/// size ?
class FAllocation {
public:
    /// 0x08db619a
    UNK_RET Read(FDataStream& a1);
    /// 0x08db61f4
    UNK_RET Write(FDataStream& a1);
    /// 0x100905b8
    static UNK_DATA ms_nFrameID[0x4];
    /// 0x100905bc
    static UNK_DATA ms_nNextFrameAllocID[0x4];
};

extern "C" {
    /// 0x100a1338
    extern UNK_DATA g_ThreadIDCount[0x4];
    /// 0x100a136c
    extern UNK_DATA g_ThreadObserver[0xc];
    /// 0x100905c0
    extern UNK_DATA g_pkDebugMemTracker[0x4];
}

namespace FAssert {
/// 0x10090860
extern UNK_DATA g_szAssertText[0x10000];
}
