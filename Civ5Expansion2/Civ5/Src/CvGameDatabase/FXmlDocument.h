/// origin guess
#pragma once

namespace Database {
class FXmlElement;
}

namespace Database {
/// size ?
/// vtable 0x0a300b80
class FXmlDocument {
public:
    /// 0x08c56c40
    FXmlDocument();
    /// 0x08c56cf2
    /// deleting 0x08c56cca
    virtual ~FXmlDocument();
    /// 0x08c56d44
    UNK_RET Clear();
    /// 0x08c56d68
    UNK_RET Parse(char* a1);
    /// 0x08c56d9a
    UNK_RET GetElementCount() const;
    /// 0x08c56dce
    UNK_RET GetRootElement(Database::FXmlElement& a1) const;
    /// 0x08c56e2a
    UNK_RET LocateElement(Database::FXmlElement& a1, char const* a2);
};
}
