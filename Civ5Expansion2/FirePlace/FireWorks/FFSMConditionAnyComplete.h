/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIFSMCondition.h"

class FContext;

/// size ?
/// vtable 0x0a303260
class FFSMConditionAnyComplete : public FIFSMCondition {
public:
    /// absent
    virtual ~FFSMConditionAnyComplete();
    /// 0x08cf10b0
    virtual bool evaluate(FContext& a1);
    /// 0x08cf1106
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08cf1126
    virtual UNK_RET vf_01(UNK_PARAMS);
};
