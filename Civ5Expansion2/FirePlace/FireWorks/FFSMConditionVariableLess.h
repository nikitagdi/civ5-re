/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFSMConditionVariable.h"

class FContext;

/// size ?
/// vtable 0x0a305ea0
class FFSMConditionVariableLess : public FFSMConditionVariable {
public:
    /// absent
    virtual ~FFSMConditionVariableLess();
    /// 0x08dad590
    virtual bool evaluate(FContext& a1);
    /// 0x08daef34
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08daef54
    virtual UNK_RET vf_01(UNK_PARAMS);
};
