/// origin guess
#pragma once

namespace Modding {
/// size ?
class ActionData {
public:
    /// 0x08c69e10
    ActionData(char const* a1, int a2, char const* a3);
    /// 0x08c69e2c
    UNK_RET GetModId() const;
    /// 0x08c69e34
    UNK_RET GetModVersion() const;
    /// 0x08c69e3c
    UNK_RET GetParameter(int a1) const;
    /// 0x08c69e44
    UNK_RET GetNumParameters() const;
};
}
