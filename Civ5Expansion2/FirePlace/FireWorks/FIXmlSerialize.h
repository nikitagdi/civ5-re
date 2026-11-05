/// origin binary
#pragma once

class FXmlElement;

/// size ?
/// vtable ?
class FIXmlSerialize {
public:
    /// 0x08ce3a2a
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08ce3a4a
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// absent
    virtual ~FIXmlSerialize();
    /// pure
    virtual UNK_RET OnLoad(FXmlElement const& a1) = 0;
    /// pure
    virtual UNK_RET OnSave(FXmlElement& a1) const = 0;
};
