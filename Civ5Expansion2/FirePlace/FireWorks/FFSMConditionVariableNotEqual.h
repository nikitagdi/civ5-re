/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFSMConditionVariable.h"

class FContext;

/// size ?
/// vtable 0x0a305e20
class FFSMConditionVariableNotEqual : public FFSMConditionVariable {
public:
    /// absent
    virtual ~FFSMConditionVariableNotEqual();
    /// 0x08dad494
    virtual bool evaluate(FContext& a1);
    /// 0x08daed2a
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08daed4a
    virtual UNK_RET vf_01(UNK_PARAMS);
};
