/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIFSMCondition.h"

class FContext;

/// size ?
/// vtable 0x0a3031a0
class FFSMConditionTrue : public FIFSMCondition {
public:
    /// absent
    virtual ~FFSMConditionTrue();
    /// 0x08cf0108
    virtual bool evaluate(FContext& a1);
    /// 0x08cf008a
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08cf00aa
    virtual UNK_RET vf_01(UNK_PARAMS);
};
