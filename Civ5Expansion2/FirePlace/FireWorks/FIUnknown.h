/// origin binary
#pragma once

class FClassId;

/// size ?
/// vtable ?
class FIUnknown {
public:
    /// 0x087fc9da
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x087fc9fa
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// absent
    virtual ~FIUnknown();
    /// 0x087fca76
    virtual UNK_RET QueryInterface(FClassId const& a1);
};
