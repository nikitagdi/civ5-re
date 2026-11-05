/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFSMConditionVariable.h"

class FContext;

/// size ?
/// vtable 0x0a306020
class FFSMConditionVariableGreaterEqual : public FFSMConditionVariable {
public:
    /// absent
    virtual ~FFSMConditionVariableGreaterEqual();
    /// 0x08dad680
    virtual bool evaluate(FContext& a1);
    /// 0x08daf552
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08daf572
    virtual UNK_RET vf_01(UNK_PARAMS);
};
