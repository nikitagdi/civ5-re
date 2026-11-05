/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFSMConditionVariable.h"

class FContext;

/// size ?
/// vtable 0x0a305da0
class FFSMConditionVariableEqual : public FFSMConditionVariable {
public:
    /// absent
    virtual ~FFSMConditionVariableEqual();
    /// 0x08dad50e
    virtual bool evaluate(FContext& a1);
    /// 0x08daeb20
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08daeb40
    virtual UNK_RET vf_01(UNK_PARAMS);
};
