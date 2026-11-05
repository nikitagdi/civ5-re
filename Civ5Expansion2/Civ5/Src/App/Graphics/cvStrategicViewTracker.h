/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"

class ActivePlayerData;
class AppUpdateData;
class CityCaptureData;
class CityCreatedData;
class CityCultureChangedData;
class CityPopulationChangedData;
class CitySetDamageData;
class ClearHexHighlightStyleData;
class DisplayMovementIndicatorData;
class FGXInt2;
class FeatureChangedData;
class ForestChangedData;
class GameplayFXData;
class HexCultureChangedData;
class HexFOWStateChangedData;
class HexHighlightData;
class HexID;
class LandmarkCreatedData;
class LandmarkHandleData;
class LandmarkIconCreatedData;
class LinearEventDispatcher;
class LocalMachineContainer;
class MarshCreatedData;
class MarshRemovedData;
class NewHexRoadData;
class NullEventClass;
class PathFinderData;
class SerialContainer;
class TradeRoutesPublishBuffers_EventData;
class UnitActionData;
class UnitBooleanData;
class UnitCreatedData;
class UnitHandleData;
class UnitMoveToHexesData;
class UnitSelectionChangedData;
class UnitSetDamageData;
class UnitTeleportedToHexData;
class UnitVisibilityChangedData;
class cvEventContext;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;
namespace FGXMathRect {
class Rect;
}
namespace TerrainIce {
class PolygonGraph;
class TriangulationGraph;
template <class T0, int T1> class HeapVector;
}

/// size ?
class cvStrategicViewTracker {
public:
    class UnitMoveData;
    /// size ?
    class UnitMoveData {
    public:
        /// 0x08a5d3c2
        UNK_RET GetCurrentLocation(FGXInt2& a1, FGXInt2& a2, float& a3);
        /// 0x08a5d558
        UNK_RET GetFinalLocation(FGXInt2& a1) const;
    };
    /// 0x08a5be80
    cvStrategicViewTracker();
    /// 0x08a5c13c
    ~cvStrategicViewTracker();
    /// 0x08a5c210
    UNK_RET StartupGame(unsigned int a1, unsigned int a2);
    /// 0x08a5c5ca
    UNK_RET ShutdownGame();
    /// 0x08a5c766
    UNK_RET AddUnit(FGXInt2 const& a1, int a2);
    /// 0x08a5c9fc
    UNK_RET DeleteUnit(int a1);
    /// 0x08a5cb84
    UNK_RET UpdateUnitVisiblity(int a1);
    /// 0x08a5ccaa
    UNK_RET RemoveUnit(FGXInt2 const& a1, int a2);
    /// 0x08a5cdca
    UNK_RET HasUnit(FGXInt2 const& a1, int a2);
    /// 0x08a5cee4
    UNK_RET HasUnitStack(FGXInt2 const& a1, bool a2);
    /// 0x08a5cfa2
    UNK_RET UpdateFeature(FGXInt2 const& a1);
    /// 0x08a5d226
    UNK_RET SetResourceType(FGXInt2 const& a1, ResourceTypes a2);
    /// 0x08a5d684
    UNK_RET OnLocalMachineAppUpdate(EventTemplate<Event_Int2Type<2>, AppUpdateData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5da7c
    UNK_RET OnSerialEventMouseOverHex(EventTemplate<Event_Int2Type<111>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5db4e
    UNK_RET OnGameplaySetActivePlayer(GameCoreEventTemplate<Event_Int2Type<131>, ActivePlayerData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5dc0c
    UNK_RET OnHexFOWStateChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5dda6
    UNK_RET OnSerialEventHexGridOn(EventTemplate<Event_Int2Type<112>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5de7c
    UNK_RET OnSerialEventHexGridOff(EventTemplate<Event_Int2Type<113>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5df52
    UNK_RET OnSerialEventEnterCityScreen(EventTemplate<Event_Int2Type<123>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5e01a
    UNK_RET OnSerialEventExitCityScreen(EventTemplate<Event_Int2Type<124>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5e0e2
    UNK_RET OnSerialEventFeatureCreated(GameCoreEventTemplate<Event_Int2Type<50>, LandmarkCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5e21a
    UNK_RET OnSerialEventFeatureDestroyed(GameCoreEventTemplate<Event_Int2Type<51>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5e382
    UNK_RET OnSerialEventForestCreated(GameCoreEventTemplate<Event_Int2Type<63>, ForestChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5e4b8
    UNK_RET OnSerialEventForestRemoved(GameCoreEventTemplate<Event_Int2Type<64>, ForestChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5e61e
    UNK_RET OnSerialEventJungleCreated(GameCoreEventTemplate<Event_Int2Type<65>, ForestChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5e754
    UNK_RET OnSerialEventJungleRemoved(GameCoreEventTemplate<Event_Int2Type<66>, ForestChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5e8ba
    UNK_RET OnMarshCreated(GameCoreEventTemplate<Event_Int2Type<53>, MarshCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5e9f0
    UNK_RET OnMarshRemoved(GameCoreEventTemplate<Event_Int2Type<54>, MarshRemovedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5eb56
    UNK_RET OnFloodplainCreated(GameCoreEventTemplate<Event_Int2Type<52>, FeatureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5ec8c
    UNK_RET OnSerialEventRawResourceIconCreated(GameCoreEventTemplate<Event_Int2Type<57>, LandmarkIconCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5edce
    UNK_RET OnSerialEventRawResourceIconDestroyed(GameCoreEventTemplate<Event_Int2Type<58>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5ef36
    UNK_RET OnSerialEventImprovementIconCreated(GameCoreEventTemplate<Event_Int2Type<61>, LandmarkIconCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5f2c4
    UNK_RET OnSerialEventImprovementIconDestroyed(GameCoreEventTemplate<Event_Int2Type<62>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5f434
    UNK_RET OnBeginRoadUpdate(EventTemplate<Event_Int2Type<13>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5f59e
    UNK_RET OnNewHexRoad(EventTemplate<Event_Int2Type<14>, NewHexRoadData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5f738
    UNK_RET OnEndRoadUpdate(EventTemplate<Event_Int2Type<15>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5f8a2
    UNK_RET OnSerialEventRoadDestroyed(GameCoreEventTemplate<Event_Int2Type<46>, HexID, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5fa38
    UNK_RET OnSerialEventHexCultureChanged(GameCoreEventTemplate<Event_Int2Type<29>, HexCultureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5fba4
    UNK_RET OnSerialEventCityCreated(GameCoreEventTemplate<Event_Int2Type<34>, CityCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5fd86
    UNK_RET OnSerialEventCityPopulationChanged(GameCoreEventTemplate<Event_Int2Type<43>, CityPopulationChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5ff22
    UNK_RET OnSerialEventCityCaptured(GameCoreEventTemplate<Event_Int2Type<42>, CityCaptureData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a600b8
    UNK_RET OnSerialEventCityDestroyed(GameCoreEventTemplate<Event_Int2Type<40>, CityCaptureData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a6024e
    UNK_RET OnSerialEventCityCultureChanged(GameCoreEventTemplate<Event_Int2Type<49>, CityCultureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a603ea
    UNK_RET OnSerialEventCitySetDamage(GameCoreEventTemplate<Event_Int2Type<44>, CitySetDamageData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a60616
    UNK_RET OnSerialEventUnitCreated(GameCoreEventTemplate<Event_Int2Type<69>, UnitCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, cvEventContext>& a1);
    /// 0x08a60b7e
    UNK_RET OnSerialEventUnitDestroyed(GameCoreEventTemplate<Event_Int2Type<70>, UnitHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a60d7a
    UNK_RET OnSerialEventUnitMoveToHexes(GameCoreEventTemplate<Event_Int2Type<77>, UnitMoveToHexesData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a612f8
    UNK_RET OnSerialEventUnitTeleportedToHex(GameCoreEventTemplate<Event_Int2Type<79>, UnitTeleportedToHexData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a61506
    UNK_RET OnUnitSelectionChanged(GameCoreEventTemplate<Event_Int2Type<92>, UnitSelectionChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a61778
    UNK_RET OnSerialEventUnitSetDamage(GameCoreEventTemplate<Event_Int2Type<72>, UnitSetDamageData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a61896
    UNK_RET OnUnitGarrison(GameCoreEventTemplate<Event_Int2Type<75>, UnitBooleanData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a61a6e
    UNK_RET OnUnitActionChanged(GameCoreEventTemplate<Event_Int2Type<73>, UnitActionData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a61bac
    UNK_RET OnUnitEmbark(GameCoreEventTemplate<Event_Int2Type<74>, UnitBooleanData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a61e2e
    UNK_RET OnUnitShouldDimFlag(GameCoreEventTemplate<Event_Int2Type<83>, UnitBooleanData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a61fb6
    UNK_RET OnUnitMarkThreatening(GameCoreEventTemplate<Event_Int2Type<84>, UnitBooleanData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a620d8
    UNK_RET OnUnitVisibilityChanged(GameCoreEventTemplate<Event_Int2Type<81>, UnitVisibilityChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a622be
    UNK_RET OnSerialEventHexHighlight(EventTemplate<Event_Int2Type<114>, HexHighlightData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a624de
    UNK_RET OnClearHexHighlights(EventTemplate<Event_Int2Type<115>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a625ba
    UNK_RET OnClearHexHighlightStyle(EventTemplate<Event_Int2Type<116>, ClearHexHighlightStyleData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a626dc
    UNK_RET OnDisplayMovementIndicator(EventTemplate<Event_Int2Type<164>, DisplayMovementIndicatorData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a627aa
    UNK_RET OnUIPathFinderUpdate(EventTemplate<Event_Int2Type<81>, PathFinderData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a6289c
    UNK_RET OnGameplayFX(GameCoreEventTemplate<Event_Int2Type<143>, GameplayFXData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a62a80
    UNK_RET OnEvent_TradeRoutesPublishBuffers(EventTemplate<Event_Int2Type<122>, TradeRoutesPublishBuffers_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
};

/// 0x08a5d5c8
UNK_RET RegisterLocalMachineAppUpdate();
/// 0x08a5d9c0
UNK_RET RegisterSerialEventMouseOverHex();
/// 0x08a5da92
UNK_RET RegisterGameplaySetActivePlayer();
/// 0x08a5db4f
UNK_RET RegisterHexFOWStateChanged();
/// 0x08a5dce9
UNK_RET RegisterSerialEventHexGridOn();
/// 0x08a5ddc0
UNK_RET RegisterSerialEventHexGridOff();
/// 0x08a5de96
UNK_RET RegisterSerialEventEnterCityScreen();
/// 0x08a5df5e
UNK_RET RegisterSerialEventExitCityScreen();
/// 0x08a5e026
UNK_RET RegisterSerialEventFeatureCreated();
/// 0x08a5e15e
UNK_RET RegisterSerialEventFeatureDestroyed();
/// 0x08a5e2c6
UNK_RET RegisterSerialEventForestCreated();
/// 0x08a5e3fb
UNK_RET RegisterSerialEventForestRemoved();
/// 0x08a5e561
UNK_RET RegisterSerialEventJungleCreated();
/// 0x08a5e697
UNK_RET RegisterSerialEventJungleRemoved();
/// 0x08a5e7fd
UNK_RET RegisterMarshCreated();
/// 0x08a5e933
UNK_RET RegisterMarshRemoved();
/// 0x08a5ea99
UNK_RET RegisterFloodplainCreated();
/// 0x08a5ebcf
UNK_RET RegisterSerialEventRawResourceIconCreated();
/// 0x08a5ed11
UNK_RET RegisterSerialEventRawResourceIconDestroyed();
/// 0x08a5ee7a
UNK_RET RegisterSerialEventImprovementIconCreated();
/// 0x08a5f208
UNK_RET RegisterSerialEventImprovementIconDestroyed();
/// 0x08a5f377
UNK_RET RegisterBeginRoadUpdate();
/// 0x08a5f4e2
UNK_RET RegisterNewHexRoad();
/// 0x08a5f67c
UNK_RET RegisterEndRoadUpdate();
/// 0x08a5f7e6
UNK_RET RegisterSerialEventRoadDestroyed();
/// 0x08a5f97b
UNK_RET RegisterSerialEventHexCultureChanged();
/// 0x08a5fae8
UNK_RET RegisterSerialEventCityCreated();
/// 0x08a5fcc9
UNK_RET RegisterSerialEventCityPopulationChanged();
/// 0x08a5fe65
UNK_RET RegisterSerialEventCityCaptured();
/// 0x08a5fffb
UNK_RET RegisterSerialEventCityDestroyed();
/// 0x08a60191
UNK_RET RegisterSerialEventCityCultureChanged();
/// 0x08a6032d
UNK_RET RegisterSerialEventCitySetDamage();
/// 0x08a60545
UNK_RET RegisterSerialEventUnitCreated();
/// 0x08a60ac2
UNK_RET RegisterSerialEventUnitDestroyed();
/// 0x08a60cbe
UNK_RET RegisterSerialEventUnitMoveToHexes();
/// 0x08a6123b
UNK_RET RegisterSerialEventUnitTeleportedToHex();
/// 0x08a61449
UNK_RET RegisterUnitSelectionChanged();
/// 0x08a616bc
UNK_RET RegisterSerialEventUnitSetDamage();
/// 0x08a617da
UNK_RET RegisterUnitGarrison();
/// 0x08a619b1
UNK_RET RegisterUnitActionChanged();
/// 0x08a61af0
UNK_RET RegisterUnitEmbark();
/// 0x08a61d72
UNK_RET RegisterUnitShouldDimFlag();
/// 0x08a61ef9
UNK_RET RegisterUnitMarkThreatening();
/// 0x08a6201b
UNK_RET RegisterUnitVisibilityChanged();
/// 0x08a62202
UNK_RET RegisterSerialEventHexHighlight();
/// 0x08a62422
UNK_RET RegisterClearHexHighlights();
/// 0x08a624fe
UNK_RET RegisterClearHexHighlightStyle();
/// 0x08a6261f
UNK_RET RegisterDisplayMovementIndicator();
/// 0x08a626ed
UNK_RET RegisterUIPathFinderUpdate();
/// 0x08a627e0
UNK_RET RegisterGameplayFX();
/// 0x08a629c4
UNK_RET RegisterEvent_TradeRoutesPublishBuffers();

namespace TerrainIce {
/// 0x08a67cc8
UNK_RET SweepTriangulate(float a1, FGXMathRect::Rect const& a2, TerrainIce::PolygonGraph const& a3, TerrainIce::TriangulationGraph& a4, unsigned int a5, TerrainIce::HeapVector<unsigned int, 14>& a6);
}
