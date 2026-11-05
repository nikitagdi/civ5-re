/// origin guess
#pragma once

/// size ?
/// vtable ?
class FIGrannyTimedTrigger {
public:
    char unk_4[0x8];    ///< +0x4
    /// 0x08ce094e
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08ce096e
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// absent
    virtual ~FIGrannyTimedTrigger();
    /// pure
    virtual UNK_RET SetEventCode(int a1) = 0;
    /// pure
    virtual UNK_RET GetEventCode() const = 0;
    /// 0x087ff4d0
    virtual UNK_RET GetDisplayValue() const;
    /// 0x08a28184
    virtual UNK_RET GetGrannyBoneName() const;
    /// 0x087ff4d8
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x087ff4e0
    virtual UNK_RET vf_09(UNK_PARAMS);
};
