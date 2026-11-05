/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/IAction.h"

namespace Modding {
class ActionData;
class System;
}

/// size ?
/// vtable 0x0a2fde78
class CvModdingUpdateUserData : public Modding::IAction {
public:
    /// 0x0898f514
    CvModdingUpdateUserData();
    /// 0x0898f530
    virtual UNK_RET PerformAction(Modding::System& a1, char const* a2, Modding::ActionData const& a3);
    /// 0x0898f6ba
    UNK_RET LoadXML(Modding::System& a1, char const* a2, int a3, wchar_t const* a4);
    /// 0x0898f834
    UNK_RET LoadSQL(Modding::System& a1, char const* a2, int a3, wchar_t const* a4);
};
