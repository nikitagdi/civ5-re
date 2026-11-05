/// origin binary
#pragma once

/// size ?
/// vtable ?
class FITimedTriggerEdit {
public:
    /// 0x08dcaec4
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08dcaee4
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// absent
    virtual ~FITimedTriggerEdit();
    /// pure
    virtual UNK_RET set_time(float a1) = 0;
    /// pure
    virtual UNK_RET set_duration(float a1) = 0;
    /// pure
    virtual UNK_RET set_repeat_count(int a1) = 0;
};
