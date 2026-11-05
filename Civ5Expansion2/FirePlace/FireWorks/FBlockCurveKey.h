/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIBlockCurve.h"

/// size ?
/// vtable 0x0a306af0
class FBlockCurveKey : public FIBlockCurve {
public:
    /// 0x08dd3772
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08dd3792
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08dd37c8
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08dd37f8
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// pure
    virtual UNK_RET eval(float a1) const = 0;
    /// 0x08dd3048
    UNK_RET add(float a1, float a2);
    /// 0x08dd30ce
    UNK_RET clear();
    /// 0x08dd30ee
    UNK_RET size() const;
};
