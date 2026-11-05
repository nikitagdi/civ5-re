/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/IAction.h"

class cvPath;
namespace Modding {
class ActionData;
class System;
}

/// size ?
/// vtable 0x0a3005d0
class CvModdingExecuteScript : public Modding::IAction {
public:
    /// 0x08b55b00
    virtual UNK_RET PerformAction(Modding::System& a1, char const* a2, Modding::ActionData const& a3);
    /// 0x08b55ca0
    UNK_RET FileExists(cvPath const& a1);
};
