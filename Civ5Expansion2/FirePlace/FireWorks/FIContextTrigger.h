/// origin binary
#pragma once

/// size ?
/// vtable ?
class FIContextTrigger {
public:
    /// 0x08dca7c2
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08dca7e2
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// absent
    virtual ~FIContextTrigger();
    /// pure
    virtual UNK_RET loop_count() const = 0;
    /// pure
    virtual UNK_RET fired_time() const = 0;
    /// pure
    virtual UNK_RET get_instance() const = 0;
};
