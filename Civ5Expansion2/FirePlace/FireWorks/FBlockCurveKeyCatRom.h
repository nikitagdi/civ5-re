/// origin guess
#pragma once
#include "FirePlace/FireWorks/FBlockCurveKey.h"

/// size ?
/// vtable 0x0a306ab0
class FBlockCurveKeyCatRom : public FBlockCurveKey {
public:
    /// 0x08dd30fc
    virtual UNK_RET eval(float a1) const;
    /// 0x08dd3414
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08dd3434
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08dd346a
    virtual UNK_RET vf_03(UNK_PARAMS);
};

/// 0x08dd39b8
UNK_RET nHash(unsigned int a1, unsigned int a2);
