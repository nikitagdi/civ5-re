/// origin guess
#pragma once

/// size ?
/// vtable ?
class FIFiniteStateMachineState {
public:
    /// 0x08cb7f32
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08cb7f52
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08cb7f8c
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08cb7faa
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// pure
    virtual UNK_RET GetCurrentState() const = 0;
    /// pure
    virtual UNK_RET GetMinimumDuration(int a1) const = 0;
    /// pure
    virtual UNK_RET GetCurrentStateMinimumDuration() const = 0;
};
