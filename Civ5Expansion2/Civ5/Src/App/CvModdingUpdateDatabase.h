/// origin guess
#pragma once
#include "Civ5/Src/CvGameDatabase/Database.h"
#include "Civ5/Src/CvLocalization/IAction.h"

namespace Modding {
class ActionData;
class System;
}

/// size ?
/// vtable 0x0a2fde58
class CvModdingUpdateDatabase : public Modding::IAction {
public:
    /// 0x0898f15c
    CvModdingUpdateDatabase(Database::Connection& a1);
    /// 0x0898f180
    virtual UNK_RET PerformAction(Modding::System& a1, char const* a2, Modding::ActionData const& a3);
    /// 0x0898f34e
    UNK_RET LoadXML(Modding::System& a1, char const* a2, int a3, wchar_t const* a4);
    /// 0x0898f454
    UNK_RET LoadSQL(Modding::System& a1, char const* a2, int a3, wchar_t const* a4);
};
