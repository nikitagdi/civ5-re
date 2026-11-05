/// origin guess
#pragma once
#include "Civ5/Src/App/IModdingAsyncTask.h"

namespace Modding {
class SteamWorkshopMonitor;
class System;
}

namespace Modding {
/// size ?
/// vtable 0x0a301158
class RefreshSteamWorkshopDataTask : public Modding::IModdingAsyncTask {
public:
    /// 0x08c75d90
    RefreshSteamWorkshopDataTask(Modding::System& a1, Modding::SteamWorkshopMonitor& a2);
    /// 0x08c75dbc
    virtual UNK_RET Execute();
};
}
