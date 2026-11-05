/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIBlockCurve.h"

/// size ?
/// vtable 0x0a3056d0
class FBlockCurveSlash : public FIBlockCurve {
public:
    /// 0x08d7b87e
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08d7b89e
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08d7b868
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08d7b8d4
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// 0x08d7b80c
    virtual UNK_RET eval(float a1) const;
};
