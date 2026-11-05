/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/cvGeneralRefHandle.h"
#include "Civ5/Src/App/Graphics/cvUnitLibrary.h"
#include "Civ5/Src/App/cvCombatSim.h"
#include <d3d9.h>

class ActionTriggerData;
class EffectTransferTriggeredData;
class FGXVector2;
class FGXVector3;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class ParticleEffectData;
class ProjectileFinishedData;
class SingleSubscriberContainer;
class cvCityCombatSim;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
class cvUnitSimDataDef;
class cvUnitSimSystem;
class cvVisEffectInfoChild;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0> class cvCityCombatSimSystemAccessor;
template <class T0> class cvUnitSimSystemAccessor;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2ffef0
class cvCombatSimMeleeMelee : public cvCombatSim {
public:
    /// 0x08a0e66a
    cvCombatSimMeleeMelee(cvUnitSimSystemAccessor<cvUnitSimSystem> const& a1, cvCityCombatSimSystemAccessor<cvCityCombatSim> const& a2);
    /// 0x08a14d22
    virtual UNK_RET DoActionComplete(cvCombatSim::UnitInfo& a1, int a2);
    /// 0x08a149bc
    virtual UNK_RET DoActionTrigger(cvCombatSim::UnitInfo& a1, int a2);
    /// 0x08a14186
    virtual UNK_RET OnActionTriggered(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor>& a1, EventTemplate<Event_Int2Type<86>, ActionTriggerData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
    /// 0x08a140e4
    virtual UNK_RET OnEffectTransferTriggered(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor>& a1, EventTemplate<Event_Int2Type<50>, EffectTransferTriggeredData, LinearEventDispatcher, SingleSubscriberContainer, unsigned int, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
    /// 0x08a14412
    virtual UNK_RET OnProjectileFinished(EventTemplate<Event_Int2Type<54>, ProjectileFinishedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a0e76c
    virtual UNK_RET Shutdown();
    /// 0x08a0e6a8
    virtual UNK_RET Startup();
    /// 0x08a1345c
    virtual UNK_RET Update();
    /// 0x08a0e6e2
    UNK_RET SetupCombatPositions(int a1);
    /// 0x08a0e7bc
    UNK_RET SetAllToIdle();
    /// 0x08a0eba8
    UNK_RET SetInitialPhaseStatus();
    /// 0x08a0f492
    UNK_RET UpdateDyingUnit(cvCombatSim::UnitInfo& a1, bool a2);
    /// 0x08a0f4f2
    UNK_RET MoveUnit(cvCombatSim::UnitInfo& a1, FGXVector3 const& a2, FGXVector3 const& a3);
    /// 0x08a0fd4a
    UNK_RET CheckFinalPosition(cvCombatSim::UnitInfo& a1);
    /// 0x08a0fed8
    UNK_RET CheckAttackStatus(cvCombatSim::UnitInfo& a1);
    /// 0x08a10480
    UNK_RET KillUnit(cvCombatSim::UnitID const& a1);
    /// 0x08a106a6
    UNK_RET Attack(cvCombatSim::UnitInfo& a1, cvCombatSim::UnitID const& a2, bool a3, cvUnitLibrary::cvUnitLibraryUnitMemberEntry::CombatConstants::WeaponInfo const& a4);
    /// 0x08a10942
    UNK_RET Attack(cvCombatSim::UnitID a1);
    /// 0x08a10ae8
    UNK_RET ApplyPassiveDamage(float a1);
    /// 0x08a10c7e
    UNK_RET SetWeaponFacing(cvCombatSim::UnitInfo& a1, cvCombatSim::UnitID const& a2);
    /// 0x08a10e4a
    UNK_RET UpdateTurn(cvCombatSim::UnitInfo& a1, FGXVector2 const& a2);
    /// 0x08a10e8e
    UNK_RET UpdateTurn(cvCombatSim::UnitInfo& a1, float a2);
    /// 0x08a1144a
    UNK_RET UpdateTurnAnimation(cvCombatSim::UnitInfo& a1);
    /// 0x08a114d4
    UNK_RET ApplyPosition(cvCombatSim::UnitInfo& a1);
    /// 0x08a11722
    UNK_RET UpdatePositions();
    /// 0x08a11d00
    UNK_RET ApplyConstraints(int a1);
    /// 0x08a12076
    UNK_RET MoveUnits();
    /// 0x08a12b22
    UNK_RET UpdateStatus();
    /// 0x08a1361a
    UNK_RET LaunchProjectile(cvCombatSim::UnitInfo& a1, FGXVector3 a2, D3DXMATRIX const& a3, ParticleEffectData const& a4);
    /// 0x08a142ac
    UNK_RET DoWeaponEffect(cvCombatSim::UnitInfo const& a1, cvUnitLibrary::cvUnitLibraryUnitMemberEntry::CombatConstants::WeaponInfo const& a2, cvCombatSim::UnitID const& a3);
    /// 0x08a14716
    UNK_RET DoChildEffect(cvVisEffectInfoChild const* a1, float a2, cvCombatSim::SideInfo const& a3, FGXVector3 const& a4, bool a5);
    /// 0x08a147b2
    UNK_RET DoEffect(unsigned int a1, float a2, cvCombatSim::SideInfo const& a3, FGXVector3 const& a4, bool a5);
    /// 0x08a14d56
    UNK_RET SetupMeleePositions(cvCombatSim::SideInfo& a1);
    /// 0x08a14ef0
    UNK_RET SetupFormationPositionsOnEdge(cvCombatSim::SideInfo& a1, int a2, int a3, float a4, float a5, float a6, int a7);
    /// 0x08a150c0
    UNK_RET SetupDefensiveMeleeScatterPositions(cvCombatSim::SideInfo& a1);
    /// 0x08a1520a
    UNK_RET SetupAttackRushPositions(cvCombatSim::SideInfo& a1);
};

/// 0x08a10be6
UNK_RET PerpDot(FGXVector2 const& a1, FGXVector2 const& a2);
