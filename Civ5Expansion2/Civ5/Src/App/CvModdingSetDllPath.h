/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/IAction.h"

class CvModdingFrameworkAppSide;
namespace Modding {
class ActionData;
class System;
}

/// size ?
/// vtable 0x0a2ffd70
class CvModdingSetDllPath : public Modding::IAction {
public:
    /// 0x089f1858
    CvModdingSetDllPath(CvModdingFrameworkAppSide& a1);
    /// 0x089f187c
    virtual UNK_RET PerformAction(Modding::System& a1, char const* a2, Modding::ActionData const& a3);
};
