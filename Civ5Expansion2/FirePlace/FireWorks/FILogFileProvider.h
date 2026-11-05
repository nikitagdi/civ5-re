/// origin guess
#pragma once

/// size ?
/// vtable ?
class FILogFileProvider {
public:
    /// 0x08cb7fce
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08cb7fee
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08cb8028
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08cb8046
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// pure
    virtual UNK_RET GetLogFile() const = 0;
};
