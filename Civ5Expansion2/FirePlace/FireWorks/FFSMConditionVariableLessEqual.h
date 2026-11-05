/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFSMConditionVariable.h"

class FContext;

/// size ?
/// vtable 0x0a305fa0
class FFSMConditionVariableLessEqual : public FFSMConditionVariable {
public:
    /// absent
    virtual ~FFSMConditionVariableLessEqual();
    /// 0x08dad630
    virtual bool evaluate(FContext& a1);
    /// 0x08daf348
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08daf368
    virtual UNK_RET vf_01(UNK_PARAMS);
};
