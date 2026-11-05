/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvStructs.h"
#include "Civ5/Src/App/Graphics/cvGeneralRefHandle.h"
#include "Civ5/Src/App/cvCityCombatSim.h"
#include <memory>
#include <vector>

class AbortCombatSimData;
class ActionTriggerData;
class CombatSimData;
class EditProjectileData;
class EffectTransferTriggeredData;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class ProjectileFinishedData;
class RunCombatSimData;
class SerialContainer;
class SingleSubscriberContainer;
class cvCityCombatSim;
class cvCombatPathControlPoint;
class cvCombatSim;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
class cvUnitSimDataDef;
class cvUnitSimSystem;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <class T0> class cvCityCombatSimSystemAccessor;
template <class T0> class cvUnitSimSystemAccessor;
template <int T0> class Event_Int2Type;

/// size ?
class cvCombatSimSystem {
public:
    enum COMBAT_ERA_TYPE : int;
    /// 0x087c9f00
    cvCombatSimSystem();
    /// 0x087ca03c
    UNK_RET Startup(cvUnitSimSystemAccessor<cvUnitSimSystem> const& a1, cvCityCombatSimSystemAccessor<cvCityCombatSim> const& a2);
    /// 0x087cb242
    UNK_RET AddPath(unsigned int a1, std::vector<cvCombatPathControlPoint, std::allocator<cvCombatPathControlPoint> > const& a2);
    /// 0x087cb33e
    UNK_RET SubscribeToEvents();
    /// 0x087cb340
    UNK_RET Shutdown();
    /// 0x087cb39c
    UNK_RET UnSubscribeFromEvents();
    /// 0x087cb39e
    UNK_RET ShutdownSims();
    /// 0x087cb3ea
    UNK_RET IsInCombat() const;
    /// 0x087cb3fc
    UNK_RET FindCombatSim();
    /// 0x087cb3fe
    UNK_RET Update(float a1);
    /// 0x087cb508
    UNK_RET UnSubscribeFromCombatSimEvents();
    /// 0x087cb5d4
    UNK_RET ReleaseProjectiles(cvCombatSim& a1);
    /// 0x087cb636
    UNK_RET SubscribeToCombatSimEvents();
    /// 0x087cb6ea
    UNK_RET OnEffectTransferTriggered(EventTemplate<Event_Int2Type<50>, EffectTransferTriggeredData, LinearEventDispatcher, SingleSubscriberContainer, unsigned int, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087cb784
    UNK_RET OnProjectileFinished(EventTemplate<Event_Int2Type<54>, ProjectileFinishedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087cb7d2
    UNK_RET OnActionTriggered(EventTemplate<Event_Int2Type<86>, ActionTriggerData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087cb868
    UNK_RET OnProjectileCreated(EventTemplate<Event_Int2Type<53>, EditProjectileData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087cb8ee
    UNK_RET GetUnitFlags(CvCombatMemberEntry const& a1);
    /// 0x087cba4e
    UNK_RET ResolveCombat(CombatSimData const& a1);
    /// 0x087cba50
    UNK_RET ExecuteCombat(GameCoreEventTemplate<Event_Int2Type<89>, RunCombatSimData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087cc63a
    UNK_RET AddSim(cvCombatSim* a1);
    /// 0x087cc6b4
    UNK_RET OnAbortCombatSim(GameCoreEventTemplate<Event_Int2Type<91>, AbortCombatSimData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087cc7a6
    UNK_RET OnUnitMoveQueueEmpty(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1);
    /// 0x087cc7a8
    UNK_RET IsInCombat(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> const& a1) const;
    /// 0x087cc7f4
    UNK_RET IsInCombat(cvCityCombatSim::HandleRef const& a1) const;
    /// 0x087cc840
    UNK_RET GetOpponent(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> const& a1, cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor>* a2) const;
    /// 0x087cc894
    UNK_RET GetOpponent(cvCityCombatSim::HandleRef const& a1, cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor>* a2) const;
    /// 0x087cc8e8
    UNK_RET GetOpponent(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> const& a1, cvCityCombatSim::HandleRef* a2) const;
    /// 0x087cc93c
    UNK_RET GetProjectile(int a1);
    /// 0x087cc96a
    UNK_RET GetPath(unsigned int a1);
    /// 0x087cc9e6
    UNK_RET AddPath(unsigned int a1, std::vector<std::vector<cvCombatPathControlPoint, std::allocator<cvCombatPathControlPoint> >, std::allocator<std::vector<cvCombatPathControlPoint, std::allocator<cvCombatPathControlPoint> > > > const& a2);
    /// 0x087cca3a
    UNK_RET GetCombatSim(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> const& a1) const;
    /// 0x087cca88
    UNK_RET GetCombatSim(cvCityCombatSim::HandleRef const& a1) const;
    /// 0x087ccad6
    UNK_RET GetCombatSim(unsigned int a1) const;
    /// 0x087ccafc
    UNK_RET GetCombatSim(unsigned int a1);
    /// 0x087ccb22
    UNK_RET GetCombatSimWeaponInfo(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> const& a1, int a2) const;
    /// 0x087ccbba
    UNK_RET GetCombatSimWeaponInfo(cvCityCombatSim::HandleRef const& a1, int a2) const;
    /// 0x087ccc52
    UNK_RET GetCombatSimImpactWeaponInfo(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> const& a1, int a2) const;
    /// 0x087cccde
    UNK_RET GetCombatSimImpactWeaponInfo(cvCityCombatSim::HandleRef const& a1, int a2) const;
    /// 0x087ccd6a
    UNK_RET GetDefaultCityCombatConstants(cvCombatSimSystem::COMBAT_ERA_TYPE a1);
    /// 0x087ccd84
    UNK_RET AllocateProjectile(unsigned int a1);
    /// 0x087ccf2a
    UNK_RET DeallocateProjectile(int a1);
    /// 0x087ccf54
    UNK_RET UpdateProjectiles(cvCombatSim& a1);
    /// 0x0a30f9ac
    static UNK_DATA m_szCombatTypes[0x3c];
    /// 0x0a4cc8fc
    static UNK_DATA ms_bCombatDebug[0x1];
};
