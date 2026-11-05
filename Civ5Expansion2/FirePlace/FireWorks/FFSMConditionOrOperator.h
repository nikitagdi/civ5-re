/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIConditionOperator.h"
#include "FirePlace/FireWorks/FIFSMCondition.h"

class FClassId;
class FContext;

/// size ?
/// vtable 0x0a3063f0
class FFSMConditionOrOperator : public FIFSMCondition, public FIConditionOperator {
public:
    /// 0x08db0cf0
    /// deleting 0x08db0d30
    virtual ~FFSMConditionOrOperator();
    /// 0x08db0d76
    virtual bool evaluate(FContext& a1);
    /// 0x08db0c9a
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08db0cba
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08dad7de
    /// thunk 0x08dad820
    virtual UNK_RET QueryInterface(FClassId const& a1);
};
