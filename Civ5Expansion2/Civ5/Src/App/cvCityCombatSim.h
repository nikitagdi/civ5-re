/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"

class HEXVec_i;
class cvCityCombatSim_CityInfo;

/// size ?
class cvCityCombatSim {
public:
    class HandleRef;
    /// 0x087c9dd0
    cvCityCombatSim();
    /// 0x087c9dd2
    UNK_RET GetCityHandle(PlayerTypes a1, int a2, HEXVec_i const& a3) const;
    /// 0x087c9e2c
    UNK_RET GetInfoFromHandle(cvCityCombatSim::HandleRef a1, cvCityCombatSim_CityInfo& a2) const;
};
