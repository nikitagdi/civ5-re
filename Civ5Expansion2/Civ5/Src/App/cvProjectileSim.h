/// origin guess
#pragma once
#include "Civ5/Src/App/CreateProjectileData.h"

class FGXVector3;
class cvProjectileSim_Init;
class cvProjectileSim_Transform;

/// size ?
class cvProjectileSim {
public:
    /// 0x08a25bc0
    cvProjectileSim(float a1);
    /// 0x08a25cc2
    ~cvProjectileSim();
    /// 0x08a25cc4
    UNK_RET Release();
    /// 0x08a25d0c
    UNK_RET Finish();
    /// 0x08a25dda
    UNK_RET ChangeTarget(FGXVector3 const& a1);
    /// 0x08a25e0a
    UNK_RET Update();
    /// 0x08a2611a
    UNK_RET GetTrajectoryAttributes(CreateProjectileData::eTrajectoryType a1, float a2, FGXVector3& a3, FGXVector3& a4);
    /// 0x08a26372
    UNK_RET UpdateData(cvProjectileSim_Init const& a1);
    /// 0x08a263fc
    UNK_RET UpdateData(cvProjectileSim_Transform const& a1);
};
