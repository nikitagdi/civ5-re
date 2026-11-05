/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIFSMCondition.h"

class FContext;

/// size ?
/// vtable 0x0a3030c0
class FFSMConditionFalse : public FIFSMCondition {
public:
    /// absent
    virtual ~FFSMConditionFalse();
    /// 0x08cefd0e
    virtual bool evaluate(FContext& a1);
    /// 0x08cefc90
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08cefcb0
    virtual UNK_RET vf_01(UNK_PARAMS);
};
