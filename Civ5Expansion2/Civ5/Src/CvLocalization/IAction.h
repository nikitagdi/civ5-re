/// origin guess
#pragma once

namespace Modding {
class ActionData;
class System;
}

namespace Modding {
/// size ?
/// vtable 0x0a301030
class IAction {
public:
    /// pure
    virtual UNK_RET PerformAction(Modding::System& a1, char const* a2, Modding::ActionData const& a3) = 0;
    /// 0x08c69e4a
    virtual UNK_RET AcquireFileData(Modding::System& a1, char const* a2, int a3, wchar_t const* a4, char*& a5, unsigned int& a6);
};
}
