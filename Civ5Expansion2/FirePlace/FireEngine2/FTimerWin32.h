/// origin binary
#pragma once
#include "FirePlace/FireEngine2/FTimerBase.h"

/// size ?
/// vtable 0x0a301ee0
class FTimerWin32 : public FTimerBase {
public:
    /// 0x08cdf3a0
    FTimerWin32();
    /// 0x08cdf3fc
    /// deleting 0x08cdf3e8
    virtual ~FTimerWin32();
    /// 0x08cdf3fe
    virtual UNK_RET CalcSystemTime();
    /// 0x0d27ac64
    static UNK_DATA m_pInst[0x4];
};
