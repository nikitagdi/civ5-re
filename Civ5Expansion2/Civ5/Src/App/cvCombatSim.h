/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/Graphics/cvGeneralRefHandle.h"
#include "Civ5/Src/App/Graphics/cvUnitLibrary.h"
#include "Civ5/Src/App/Graphics/cvUnitVisSystem.h"
#include "Civ5/Src/App/cvCityCombatSim.h"
#include "Civ5/Src/App/cvUnitSetActionResultCheck.h"

class ActionTriggerData;
class EffectTransferTriggeredData;
class FGXVector2;
class FGXVector3;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class ProjectileFinishedData;
class SingleSubscriberContainer;
class cvCityCombatSim;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
class cvUnitSimDataDef;
class cvUnitSimSystem;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0> class cvCityCombatSimSystemAccessor;
template <class T0> class cvUnitSimSystemAccessor;
template <int T0> class Event_Int2Type;
namespace cvCombatSimTargetingDefs {
enum TARGETING_STATUS : int;
}

/// size ?
/// vtable 0x0a2ffeb0
class cvCombatSim {
public:
    class SideInfo;
    class UnitID;
    class UnitInfo;
    enum COMBAT_DIRECTIONS : int;
    enum COMBAT_PHASE : int;
    enum COMBAT_STATUS : int;
    enum COMBAT_TYPE : int;
    class CombatProjectile;
    enum MOVEMENT_PHASE : int;
    /// size ?
    class SideInfo {
    public:
        /// 0x08a045d2
        UNK_RET GetUniformTurnDirection(cvCombatSim::MOVEMENT_PHASE a1, cvCombatSim::COMBAT_DIRECTIONS a2);
    };
    /// size ?
    class UnitID {
    public:
        /// 0x0c0885e0
        static UNK_DATA NO_TARGET[0x4];
    };
    /// size ?
    class UnitInfo {
    public:
        /// 0x08a04478
        UNK_RET SetFinalPosition(FGXVector3 const& a1);
        /// 0x08a044f4
        UNK_RET GetShortMoveRadius() const;
        /// 0x08a0451a
        UNK_RET GetShortMoveRate() const;
        /// 0x08a04540
        UNK_RET GetMoveRate() const;
        /// 0x08a04570
        UNK_RET GetWeaponInfo(cvUnitLibrary::cvUnitLibraryUnitMemberEntry::CombatConstants::WEAPON_TYPES a1, unsigned int a2) const;
    };
    class UnitOrder;
    /// 0x08a045f2
    cvCombatSim(cvUnitSimSystemAccessor<cvUnitSimSystem> const& a1, cvCityCombatSimSystemAccessor<cvCityCombatSim> const& a2);
    /// 0x08a047cc
    ~cvCombatSim();
    /// 0x08a049de
    virtual UNK_RET Startup();
    /// 0x08a0543c
    virtual UNK_RET Shutdown();
    /// pure
    virtual UNK_RET Update() = 0;
    /// 0x08a0bfe6
    virtual UNK_RET OnEffectTransferTriggered(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor>& a1, EventTemplate<Event_Int2Type<50>, EffectTransferTriggeredData, LinearEventDispatcher, SingleSubscriberContainer, unsigned int, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
    /// 0x08a0bfe8
    virtual UNK_RET OnActionTriggered(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor>& a1, EventTemplate<Event_Int2Type<86>, ActionTriggerData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
    /// 0x08a0bfea
    virtual UNK_RET OnProjectileFinished(EventTemplate<Event_Int2Type<54>, ProjectileFinishedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a0bfec
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x08a0bfee
    virtual UNK_RET DoActionTrigger(cvCombatSim::UnitInfo& a1, int a2);
    /// 0x08a0bff0
    virtual UNK_RET DoActionComplete(cvCombatSim::UnitInfo& a1, int a2);
    /// 0x08a04914
    UNK_RET SetEventID(unsigned int a1);
    /// 0x08a05098
    UNK_RET PopulateSimHandles();
    /// 0x08a0524c
    UNK_RET SetupActiveWeapons(cvCombatSim::SideInfo& a1);
    /// 0x08a05412
    UNK_RET SetRangedAttack(cvCombatSim::SideInfo& a1);
    /// 0x08a0574c
    UNK_RET GetUnitSim(int a1);
    /// 0x08a05760
    UNK_RET SetDead(cvCombatSim::UnitInfo& a1);
    /// 0x08a05812
    UNK_RET ApplyActionCode(cvCombatSim::UnitInfo& a1);
    /// 0x08a05ba8
    UNK_RET GetCitySim(int a1) const;
    /// 0x08a05bbc
    UNK_RET GetCitySim(int a1);
    /// 0x08a05bd0
    UNK_RET GetUnitSim(int a1) const;
    /// 0x08a05be4
    UNK_RET InitializeSide(int a1);
    /// 0x08a05d0c
    UNK_RET GetOpposingSide(int a1);
    /// 0x08a05d32
    UNK_RET GetOpposingSideIndex(int a1) const;
    /// 0x08a05d44
    UNK_RET GetOpposingSide(int a1) const;
    /// 0x08a05d6a
    UNK_RET UnitInfoRetrieved();
    /// 0x08a05dac
    UNK_RET SetUnitData(int a1, cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> const& a2, int a3);
    /// 0x08a0657e
    UNK_RET AddToPath(cvCombatSim::UnitInfo& a1, FGXVector3 const& a2, unsigned int a3);
    /// 0x08a0662a
    UNK_RET SetCityData(int a1, cvCityCombatSim::HandleRef const& a2, int a3);
    /// 0x08a06940
    UNK_RET SetPlotData(int a1, PlayerTypes a2, int a3, int a4);
    /// 0x08a06be0
    UNK_RET BeginUpdate(float a1);
    /// 0x08a06c48
    UNK_RET EndUpdate();
    /// 0x08a06c6e
    UNK_RET UpdateActions(cvCombatSim::SideInfo& a1);
    /// 0x08a06d70
    UNK_RET GetUnitVisStates();
    /// 0x08a06f20
    UNK_RET SetVariableValue(cvCombatSim::UnitInfo const& a1, cvUnitSetVarListValue::VARIABLE_TYPE a2, float a3);
    /// 0x08a06fca
    UNK_RET ApplyActionCodeToVis(cvCombatSim::UnitInfo& a1, int a2, int a3, cvUnitSetActionResultCheck::CHECK_TYPE a4);
    /// 0x08a071be
    UNK_RET CombatRanTooLong();
    /// 0x08a071d2
    UNK_RET InitializeCombatData();
    /// 0x08a073e0
    UNK_RET GetPairedTargets(FGXVector2 const* a1, FGXVector2 const* a2, int* a3, int* a4, int a5);
    /// 0x08a0770c
    UNK_RET CalculateFormationPositions(cvCombatSim::SideInfo& a1, FGXVector2 const& a2, FGXVector2 const& a3, unsigned int a4);
    /// 0x08a078c6
    UNK_RET CalculateFormationPositions(unsigned int a1, unsigned int a2, cvCombatSim::SideInfo& a3, FGXVector2 const& a4, FGXVector2 const& a5, bool a6, unsigned int a7);
    /// 0x08a07c7a
    UNK_RET GetHeightAt(cvCombatSim::SideInfo const& a1, FGXVector2 const& a2);
    /// 0x08a07cba
    UNK_RET ClearPath(cvCombatSim::UnitInfo& a1);
    /// 0x08a07ce4
    UNK_RET CalculateFinalPositions();
    /// 0x08a08024
    UNK_RET UpdatePath(cvCombatSim::UnitInfo& a1, FGXVector3 const& a2, unsigned int a3);
    /// 0x08a08094
    UNK_RET ChangeTarget(cvCombatSim::UnitInfo& a1, cvCombatSim::UnitID a2);
    /// 0x08a08186
    UNK_RET GetActiveWeaponByID(cvCombatSim::UnitInfo const& a1, unsigned int a2) const;
    /// 0x08a081b6
    UNK_RET GetActiveWeapon(cvCombatSim::UnitInfo const& a1) const;
    /// 0x08a081c8
    UNK_RET GetActiveWeapon(cvCombatSim::SideInfo const& a1) const;
    /// 0x08a081e0
    UNK_RET GetKillCount(cvCombatSim::UnitInfo& a1, cvCombatSim::UnitID const& a2, cvUnitLibrary::cvUnitLibraryUnitMemberEntry::CombatConstants::WeaponInfo const& a3);
    /// 0x08a082ec
    UNK_RET AssignPendingKills(cvCombatSim::UnitInfo const& a1, cvCombatSim::UnitID const& a2, int a3, int a4, cvCombatSim::UnitID* a5, cvUnitLibrary::cvUnitLibraryUnitMemberEntry::CombatConstants::WeaponInfo const* a6);
    /// 0x08a08580
    UNK_RET GetPosition(cvCombatSim::UnitID const& a1, FGXVector2* a2, float a3);
    /// 0x08a0860e
    UNK_RET SetPosition(cvCombatSim::UnitInfo& a1, FGXVector3 const& a2);
    /// 0x08a08660
    UNK_RET ModTo2PI(float a1);
    /// 0x08a086ce
    UNK_RET ChangeUnitStatus(cvCombatSim::UnitInfo& a1, cvCombatSim::COMBAT_STATUS a2, float a3);
    /// 0x08a08b12
    UNK_RET IsUsingMotionAction(cvCombatSim::UnitInfo const& a1);
    /// 0x08a08b58
    UNK_RET SetSideActionCode(int a1, int a2, unsigned int a3);
    /// 0x08a08bd8
    UNK_RET SetSideTargeting(int a1, cvCombatSimTargetingDefs::TARGETING_STATUS a2, unsigned int a3);
    /// 0x08a08c28
    UNK_RET SetSideUnitStatus(int a1, cvCombatSim::COMBAT_STATUS a2, unsigned int a3);
    /// 0x08a08cb0
    UNK_RET SetSideRangeUnitStatus(int a1, int a2, cvCombatSim::COMBAT_STATUS a3, unsigned int a4);
    /// 0x08a08d02
    UNK_RET ChangeSideUnitStatus(int a1, cvCombatSim::COMBAT_STATUS a2, cvCombatSim::COMBAT_STATUS a3, unsigned int a4);
    /// 0x08a08d96
    UNK_RET IsSideReadyForAttack(int a1, unsigned int a2);
    /// 0x08a08e1e
    UNK_RET SetSideUnitDirectionModifier(int a1, float a2, unsigned int a3);
    /// 0x08a08fb2
    UNK_RET AddRandomVector(FGXVector3* a1, float a2);
    /// 0x08a090ea
    UNK_RET AddRandomVector(FGXVector2* a1, float a2);
    /// 0x08a091a2
    UNK_RET EstimateIntersectionPosition(cvCombatSim::UnitID const& a1, FGXVector3* a2, float a3, float a4, FGXVector3 const& a5);
    /// 0x08a09672
    UNK_RET GetPosition(cvCombatSim::UnitID const& a1, FGXVector3* a2, float a3);
    /// 0x08a09804
    UNK_RET GetPosition(cvCombatSim::UnitID const& a1, cvCombatSim::UnitInfo const& a2, int a3);
    /// 0x08a0999a
    UNK_RET GetTargeterIndex(cvCombatSim::UnitID const& a1, cvCombatSim::UnitInfo const& a2);
    /// 0x08a09a30
    UNK_RET GetUnit(cvCombatSim::UnitID const& a1);
    /// 0x08a09a54
    UNK_RET GetUnit(cvCombatSim::UnitID const& a1) const;
    /// 0x08a09a78
    UNK_RET GetTargetPosition(FGXVector3 const& a1, cvCombatSim::SideInfo const& a2, FGXVector3* a3, float a4);
    /// 0x08a09bbe
    UNK_RET HasReachedKillLimit();
    /// 0x08a09bfa
    UNK_RET HasReachedKillLimit(cvCombatSim::SideInfo const& a1);
    /// 0x08a09c1e
    UNK_RET IsInSim(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> const& a1);
    /// 0x08a09c70
    UNK_RET IsInSim(cvCityCombatSim::HandleRef const& a1);
    /// 0x08a09cbe
    UNK_RET GetSideIndex(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> const& a1) const;
    /// 0x08a09cf6
    UNK_RET GetSideIndex(cvCityCombatSim::HandleRef const& a1) const;
    /// 0x08a09d46
    UNK_RET GetOpponent(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> const& a1, cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor>* a2);
    /// 0x08a09de0
    UNK_RET GetOpponent(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> const& a1, cvCityCombatSim::HandleRef* a2);
    /// 0x08a09e68
    UNK_RET GetOpponent(cvCityCombatSim::HandleRef const& a1, cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor>* a2);
    /// 0x08a09f1a
    UNK_RET GetFormationType(cvCombatSim::SideInfo& a1, cvCombatSim::COMBAT_TYPE a2, cvCombatSim::COMBAT_PHASE a3);
    /// 0x08a09fcc
    UNK_RET MakePath(cvCombatSim::UnitInfo& a1, unsigned int a2, bool a3);
    /// 0x08a0a9d8
    UNK_RET PrepareActivePathNode(cvCombatSim::UnitInfo& a1);
    /// 0x08a0ad2c
    UNK_RET UpdateProjectile(cvCombatSim::CombatProjectile& a1);
    /// 0x08a0b27a
    UNK_RET GetNextCombatPhase();
    /// 0x08a0b3ac
    UNK_RET SetPhaseComplete();
    /// 0x08a0b3b8
    UNK_RET IsPointReachable(cvCombatSim::UnitInfo& a1, FGXVector2 const& a2, float a3);
    /// 0x08a0b50c
    UNK_RET CheckForEndOfCombat(cvCombatSim::UnitInfo& a1);
    /// 0x08a0b7a0
    UNK_RET SideHasDyingUnits(int a1);
    /// 0x08a0b7f0
    UNK_RET WorldToMapPoint(FGXVector2 const& a1, FGXVector2* a2);
    /// 0x08a0b89c
    UNK_RET WorldToMapPoint3D(FGXVector3 const& a1, FGXVector3* a2);
    /// 0x08a0b952
    UNK_RET UpdateRootPosition(cvCombatSim::SideInfo& a1);
    /// 0x08a0bab4
    UNK_RET SideHasAttacked(int a1);
    /// 0x08a0baf8
    UNK_RET IsMotionAction(int a1);
};

/// 0x08a04967
UNK_RET AngleToFacing(float a1);
/// 0x08a0e638
UNK_RET Get2DDistanceSqr(FGXVector3 const& a1, FGXVector3 const& a2);
