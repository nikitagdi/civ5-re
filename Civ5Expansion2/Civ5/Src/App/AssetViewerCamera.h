/// origin guess
#pragma once
#include "Civ5/Src/App/cvCombatSim.h"
#include "FirePlace/FireEngine2/FCamera.h"

class FGXInt2;
class FGXVector2;
class FGXVector3;
class cvCombatSim;
namespace cvCombatSimTargeting {
class IntersectionResult;
}
namespace cvCombatSimTargetingDefs {
enum BLOCKED_STATUS : int;
}

/// size ?
/// vtable 0x0a300770
class AssetViewerCamera : public FCamera {
public:
    /// 0x08b70468
    AssetViewerCamera();
    /// 0x08b70520
    /// deleting 0x08b704f8
    virtual ~AssetViewerCamera();
    /// 0x08b70534
    UNK_RET ResetCamera();
    /// 0x08b7054e
    UNK_RET ResetCamera(FGXVector3 const& a1, FGXVector3 const& a2, FGXVector3 const& a3);
    /// 0x08b705a6
    UNK_RET Zoom(float a1);
    /// 0x08b705ee
    UNK_RET Pan(float a1, float a2);
    /// 0x08b7065e
    UNK_RET PanFromMouseDelta(int a1, int a2);
    /// 0x08b7088c
    UNK_RET Orbit(float a1, float a2, FGXVector3 const& a3);
    /// 0x08b70a8a
    UNK_RET OnMouseMove(int a1, int a2, bool a3, bool a4, bool a5);
    /// 0x08b70ba8
    UNK_RET OnMouseButtonDown();
    /// 0x08b70baa
    UNK_RET OnMouseWheel(int a1);
    /// 0x08b70c1a
    UNK_RET SetWindowDimensions(FGXInt2 const& a1);
    /// 0x0cccf65c
    static UNK_DATA sm_kCamLocalRot[0x40];
    /// 0x0cccf69c
    static UNK_DATA sm_kInvCamLocalRot[0x40];
};

namespace cvCombatSimTargeting {
/// 0x08b70c70
UNK_RET IsBlockedToTargetRadius(cvCombatSim& a1, cvCombatSim::SideInfo& a2, int a3, unsigned int a4, cvCombatSimTargetingDefs::BLOCKED_STATUS* a5);
/// 0x08b70e84
UNK_RET IsBlockedToTargetRadius(cvCombatSim& a1, cvCombatSim::UnitInfo& a2, FGXVector2 a3, float a4, float a5, unsigned int a6, int a7, cvCombatSimTargeting::IntersectionResult* a8);
/// 0x08b71290
UNK_RET FindBlockingUnits(cvCombatSim& a1, cvCombatSim::UnitInfo& a2, FGXVector2 a3, cvCombatSim::UnitID* a4, int a5, unsigned int a6);
/// 0x08b713da
UNK_RET FindBestFormationTarget(cvCombatSim& a1, cvCombatSim::SideInfo& a2, int a3, unsigned int a4, cvCombatSimTargetingDefs::BLOCKED_STATUS* a5);
/// 0x08b717a0
UNK_RET FindMatchingFormationTarget(cvCombatSim::SideInfo& a1, int a2);
/// 0x08b717e4
UNK_RET FindBestRangedTarget(cvCombatSim& a1, cvCombatSim::SideInfo& a2, int a3);
/// 0x08b71a74
UNK_RET IsBlockedToTarget(cvCombatSim& a1, cvCombatSim::UnitInfo& a2, FGXVector2 a3, float a4, unsigned int a5, int a6, cvCombatSimTargeting::IntersectionResult* a7);
/// 0x08b71cce
UNK_RET UnitOrderSort(cvCombatSim::UnitOrder a1, cvCombatSim::UnitOrder a2);
/// 0x08b71ce0
UNK_RET SortUnitsOnSide(cvCombatSim::SideInfo& a1, cvCombatSim::SideInfo& a2);
/// 0x08b71dea
UNK_RET FindBestTarget(cvCombatSim& a1, cvCombatSim::SideInfo& a2, int a3);
/// 0x08b71f1a
UNK_RET FindBestMeleeTarget(cvCombatSim& a1, cvCombatSim::SideInfo& a2, int a3);
/// 0x08b72496
UNK_RET IsIntersectingOpponent(cvCombatSim::UnitInfo& a1, cvCombatSim::SideInfo& a2, cvCombatSim::UnitID* a3);
/// 0x08b72526
UNK_RET FindClosestInRange(FGXVector2 const& a1, cvCombatSim::SideInfo const& a2, float a3, unsigned int a4);
/// 0x08b725a4
UNK_RET FindTargets(cvCombatSim& a1);
/// 0x08b72cf0
UNK_RET HasAvailableTargets(cvCombatSim::SideInfo const& a1);
}
