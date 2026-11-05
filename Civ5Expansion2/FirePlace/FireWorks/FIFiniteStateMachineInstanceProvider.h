/// origin guess
#pragma once

/// size ?
/// vtable ?
class FIFiniteStateMachineInstanceProvider {
public:
    /// 0x08cb7d28
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08cb7d48
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08cb7d82
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08cb7da0
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// 0x08cb7dc4
    virtual UNK_RET vf_04(UNK_PARAMS);
    /// pure
    virtual UNK_RET get_fsm_instance(unsigned int a1) const = 0;
};
