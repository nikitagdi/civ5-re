/// origin guess
#pragma once

class FContext;

/// size ?
/// vtable ?
class FIConditionEvent {
public:
    /// 0x08daf9ac
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08daf9cc
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08dafa06
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08dafa46
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// pure
    virtual UNK_RET fire_event(FContext& a1) const = 0;
};
