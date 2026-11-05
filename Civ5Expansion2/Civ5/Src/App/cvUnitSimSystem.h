/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/Graphics/cvGeneralRefHandle.h"
#include "Civ5/Src/App/Graphics/cvUnitVisSystem.h"
#include "Civ5/Src/App/cvUnitSim.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include <lua.h>

class AbortCombatSimData;
class ActivePlayerData;
class FGXVector3;
class HEXVec_i;
class HexFOWStateChangedData;
class HexID;
class LinearEventDispatcher;
class LocalMachineContainer;
class MapGlobalsData;
class NonSerialContainer;
class NullEventClass;
class RandomSeedSetData;
class RunCombatSimData;
class SerialContainer;
class UnitActionData;
class UnitCreatedData;
class UnitFacingChangedData;
class UnitFormationCreateData;
class UnitFormationQueryAllData;
class UnitFormationQueryData;
class UnitFormationSetData;
class UnitHandleData;
class UnitMoveData;
class UnitMoveToHexesData;
class UnitSelectionChangedData;
class UnitTeleportedToHexData;
class UnitVisibilityChangedData;
class WorldGlobalsData;
class cvEventContext;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemNonSerialAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
class cvUnitCurrentDebugStateInfo;
class cvUnitCurrentStateInfo;
class cvUnitOverrideFSMState;
class cvUnitSetActionResultCheck;
class cvUnitSetFSMDebugState;
class cvUnitSimDataDef;
class cvUnitSimSystem;
class cvUnitSim_Add3DMoves;
class cvUnitSim_OverrideFOW;
class cvUnitSim_Set3DTransformation;
class cvUnitSim_SetFrameAnimUpdate;
class cvUnitSim_SetVisibility;
class cvUnitSim_State;
class cvUnitSim_UnitInfo;
class cvUnitSim_UnitStatus;
class cvUnitStateGraphInfo;
class cvUnitVis_State;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <class T0> class cvUnitSimSystemAccessor;
template <class T0> class cvUnitVisSystemAccessor;
template <int T0> class Event_Int2Type;

/// size ?
class UnitSetDamageData {
public:
    // Offsets are those of the whole event object.
    char unk_0[0x4];             ///< +0x0
    PlayerTypes m_ePlayer;       ///< +0x4
    int m_iUnitID;               ///< +0x8
    char unk_c[0xc];             ///< +0xc
    int m_iDamage;               ///< +0x18
    char unk_1c[0x4];            ///< +0x1c
    int m_iMaxHitPoints;         ///< +0x20
};

/// size 0x4
class cvUnitSim_SetHitPct {
public:
    int m_iPct;                  ///< +0x0
};

/// size 0xc
class cvUnitSim_ResetFormation {
public:
    unsigned int m_uiFormationCrc;   ///< +0x0
    unsigned int m_uiFormationCrc2;  ///< +0x4
    bool m_bSetHeading;              ///< +0x8
};

/// size 0x8
class cvUnitSim_DamageState {
public:
    unsigned int m_uiFigure;     ///< +0x0
    unsigned int m_uiHealthPct;  ///< +0x4
};

/// size 0x14
class cvUnitSim_SetTransformation {
public:
    FGXVector2 m_vPos;           ///< +0x0
    float m_fHeading;            ///< +0x8
    unsigned int m_uiFigure;     ///< +0xc
    bool m_bSetHeading;          ///< +0x10
};

typedef cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> cvUnitSimHandle;

/// 0x08808a8c
UNK_RET Fun_08808a8c(UNK_PARAMS);

/// 0x08808f6c
UNK_RET Fun_08808f6c(UNK_PARAMS);

/// 0x088086cc
UNK_RET Fun_088086cc(UNK_PARAMS);

/// size ?
class cvUnitSimSystem {
public:
    char unk_0[0xd0];          ///< +0x0
    cvUnitSim* m_pSims;        ///< +0xd0
    unsigned int m_uiNumSims;  ///< +0xd4
    char unk_d8[0x3c];         ///< +0xd8
    char unk_114[0xc];         ///< +0x114
    unsigned char m_ucFlags;   ///< +0x120
    class QueuedMove;
    /// size ?
    class QueuedMove {
    public:
        /// 0x08800578
        UNK_RET SetEventID(unsigned int a1);
        /// 0x088005d2
        UNK_RET Clear();
    };
    /// 0x0880063e
    cvUnitSimSystem();
    /// 0x08800902
    ~cvUnitSimSystem();
    /// 0x088009b6
    UNK_RET Startup(cvUnitVisSystemAccessor<cvUnitVisSystem> const& a1);
    /// 0x08800a5c
    UNK_RET SubscribeToEvents();
    /// 0x08800d7e
    UNK_RET Shutdown();
    /// 0x08800df8
    UNK_RET UnSubscribeToEvents();
    /// 0x0880111a
    UNK_RET DestroySims();
    /// 0x088011de
    UNK_RET StartupGame();
    /// 0x08801222
    UNK_RET ShutdownGame();
    /// 0x0880126a
    UNK_RET Update(float a1);
    /// 0x088012e6
    UNK_RET UpdateSim(cvUnitSim& a1, float a2);
    /// 0x088015a4
    UNK_RET OnMapGlobalsChanged(EventTemplate<Event_Int2Type<37>, MapGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088015da
    UNK_RET OnWorldGlobalsChanged(EventTemplate<Event_Int2Type<36>, WorldGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08801610
    UNK_RET OnActivePlayerChanged(GameCoreEventTemplate<Event_Int2Type<131>, ActivePlayerData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0880180e
    UNK_RET OnUnitMoveToHexes(GameCoreEventTemplate<Event_Int2Type<77>, UnitMoveToHexesData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0880201e
    UNK_RET OnUnitTeleportedToHex(GameCoreEventTemplate<Event_Int2Type<79>, UnitTeleportedToHexData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088024b2
    UNK_RET OnUnitFacingChanged(GameCoreEventTemplate<Event_Int2Type<80>, UnitFacingChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08802608
    UNK_RET OnFOWChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088028b4
    UNK_RET OnUnitSelected(GameCoreEventTemplate<Event_Int2Type<92>, UnitSelectionChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08802a74
    UNK_RET OnUnitVisibilityChanged(GameCoreEventTemplate<Event_Int2Type<81>, UnitVisibilityChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08802b8a
    UNK_RET OnUnitSetDamage(GameCoreEventTemplate<Event_Int2Type<72>, UnitSetDamageData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08802c5e
    UNK_RET OnUnitReform(EventTemplate<Event_Int2Type<12>, UnitHandleData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08802d38
    UNK_RET OnUnitFormationQuery(EventTemplate<Event_Int2Type<29>, UnitFormationQueryData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08802e74
    UNK_RET OnUnitFormationQueryAll(EventTemplate<Event_Int2Type<30>, UnitFormationQueryAllData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08802f92
    UNK_RET OnUnitFormationSet(EventTemplate<Event_Int2Type<32>, UnitFormationSetData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088030b4
    UNK_RET OnUnitCreateFormation(EventTemplate<Event_Int2Type<28>, UnitFormationCreateData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088030e6
    UNK_RET OnUnitActionChanged(GameCoreEventTemplate<Event_Int2Type<73>, UnitActionData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0880332a
    UNK_RET OnUnitResetAnimationState(GameCoreEventTemplate<Event_Int2Type<76>, UnitActionData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08803532
    UNK_RET OnRandomSeedSet(GameCoreEventTemplate<Event_Int2Type<138>, RandomSeedSetData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08803556
    UNK_RET OnUnitLibraryInitialized(EventTemplate<Event_Int2Type<21>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088036b6
    UNK_RET OnRunCombatSim(GameCoreEventTemplate<Event_Int2Type<89>, RunCombatSimData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08803768
    UNK_RET OnAbortCombatSim(GameCoreEventTemplate<Event_Int2Type<91>, AbortCombatSimData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088037da
    UNK_RET OnUnitCreated(GameCoreEventTemplate<Event_Int2Type<69>, UnitCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, cvEventContext>& a1);
    /// 0x08803d04
    UNK_RET OnUnitDestroyed(GameCoreEventTemplate<Event_Int2Type<70>, UnitHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08803f78
    UNK_RET OnUnitMove(GameCoreEventTemplate<Event_Int2Type<78>, UnitMoveData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0880412a
    UNK_RET AssignActionStateToMembers(cvUnitSim& a1);
    /// 0x08804154
    cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> GetUnitHandle(PlayerTypes a1, int a2) const;
    /// 0x088041c6
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSetActionResultCheck const& a2);
    /// 0x08804220
    UNK_RET PublishUnitActionStates(cvUnitSim& a1);
    /// 0x088042d6
    UNK_RET GetUnitHandle(HexID a1) const;
    /// 0x0880437e
    UNK_RET GetUnitHandle(FGXVector2 const& a1) const;
    /// 0x088043e8
    UNK_RET FindFirstSimHandleInRange(FGXVector2 const& a1) const;
    /// 0x08804472
    UNK_RET ToUnitHexPosition(FGXVector3 const& a1, HEXVec_i& a2);
    /// 0x0880453e
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSim_SetTransformation const& a2);
    /// 0x08804a8e
    UNK_RET PublishUnitMovementQueueEmpty(cvUnitSim& a1);
    /// 0x08804b2c
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSim_SetHitPct const& a2);
    /// 0x08804e86
    UNK_RET TestAndUpdateFOW(cvUnitSim& a1);
    /// 0x08804fa2
    UNK_RET PublishUnitMoved(cvUnitSim& a1);
    /// 0x08805016
    UNK_RET PublishUnitHexHighlight(cvUnitSim const& a1, bool a2);
    /// 0x088050da
    UNK_RET AddMovesToSim(cvUnitSim& a1, unsigned int a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9, FGXVector3 const* a10, unsigned int a11, bool a12, unsigned int a13);
    /// 0x08805430
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSim_Set3DTransformation const& a2);
    /// 0x08805828
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSim_SetVisibility const& a2);
    /// 0x088058e8
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSim_State const& a2);
    /// 0x088059b6
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSim_ResetFormation const& a2);
    /// 0x08805b22
    UNK_RET PublishUnitTransforms(cvUnitSim& a1, cvUnitSimSystem::QueuedMove* a2);
    /// 0x08805f2a
    UNK_RET GetInfoFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSim_UnitInfo& a2) const;
    /// 0x08805f7c
    UNK_RET GetInfoFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSim_UnitStatus& a2) const;
    /// 0x088060bc
    UNK_RET GetInfoFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitStateGraphInfo& a2) const;
    /// 0x08806130
    UNK_RET GetInfoFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitCurrentStateInfo& a2) const;
    /// 0x088061a4
    UNK_RET GetInfoFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitCurrentDebugStateInfo& a2) const;
    /// 0x08806218
    UNK_RET HasAnimationStates(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1) const;
    /// 0x08806288
    UNK_RET HasQueuedMoves(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1) const;
    /// 0x088062c2
    UNK_RET OffsetUnitFormation(float a1, FGXVector2 const& a2, unsigned int a3, FGXVector2* a4) const;
    /// 0x08806394
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSim_DamageState const& a2);
    /// 0x0880656a
    UNK_RET ToVisState(cvUnitSim& a1, cvUnitVis_State& a2);
    /// 0x088065ea
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSim_SetFrameAnimUpdate const& a2);
    /// 0x08806622
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSetVarListValue const& a2);
    /// 0x0880667c
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitOverrideFSMState const& a2);
    /// 0x088066d6
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSim_Add3DMoves const& a2);
    /// 0x088067b4
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSetFSMDebugState const& a2);
    /// 0x0880680e
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor> a1, cvUnitSim_OverrideFOW const& a2);
    /// 0x08806906
    UNK_RET FindFirstSimHandleInRange(HEXVec_i const& a1) const;
    /// 0x088069a2
    UNK_RET CalculateUnitTransformsFromQueuedPath(cvUnitSim& a1, cvUnitSimSystem::QueuedMove& a2, float a3);
    /// 0x08806fd6
    UNK_RET NormalizeTransform(cvUnitSim& a1);
    /// 0x088071b0
    UNK_RET PublishUnitHeadingDelta(cvUnitSim& a1);
    /// 0x088072f4
    UNK_RET AddMovesToSimMembers(cvUnitSim& a1, cvUnitSimSystem::QueuedMove& a2);
    /// 0x08807a48
    UNK_RET LuaGetUnitSimHandle(int a1, int a2);
};

/// 0x08807989
/// lua ?
int LuaThunk_GetUnitSimHandle(lua_State* a1);
/// 0x08807a8a
int LuaToString_GetUnitSimHandle(lua_State* a1);
/// 0x08807b13
/// lua ?
int LuaThunk_TunerSetUnitState(lua_State* a1);
/// 0x08807bb3
UNK_RET TunerSetUnitState(int a1, int a2, char const* a3);
/// 0x08807d6a
int LuaToString_TunerSetUnitState(lua_State* a1);
/// 0x08816a24
UNK_RET CalculateSplineControlPointsFromTangent(FGXVector3 const& a1, FGXVector3 const& a2, FGXVector3 const& a3, FGXVector3 (&a4)[4]);

namespace cvLocalMachineEventRegistrar {
/// 0x08813048
UNK_RET RegisterEvents();
}

namespace cvNonSerialEventRegistrar {
/// 0x0881be54
UNK_RET RegisterEvents();
}
