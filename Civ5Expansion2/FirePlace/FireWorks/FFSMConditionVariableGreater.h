/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFSMConditionVariable.h"

class FContext;

/// size ?
/// vtable 0x0a305f20
class FFSMConditionVariableGreater : public FFSMConditionVariable {
public:
    /// absent
    virtual ~FFSMConditionVariableGreater();
    /// 0x08dad5e0
    virtual bool evaluate(FContext& a1);
    /// 0x08daf13e
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08daf15e
    virtual UNK_RET vf_01(UNK_PARAMS);
};
