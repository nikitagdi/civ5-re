/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIBlockCurve.h"

/// size ?
/// vtable 0x0a3056a0
class FBlockCurveLine : public FIBlockCurve {
public:
    /// 0x08d7b812
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08d7b832
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08d7b868
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08d7b86a
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// 0x08d7b808
    virtual UNK_RET eval(float a1) const;
};
