/// origin guess
#pragma once

/// size ?
/// vtable ?
class FISoundEventName {
public:
    /// 0x08ce1bdc
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08ce1bfc
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// absent
    virtual ~FISoundEventName();
    /// pure
    virtual UNK_RET GetEventName() const = 0;
};
