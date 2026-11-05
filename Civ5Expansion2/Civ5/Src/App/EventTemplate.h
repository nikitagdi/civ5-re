/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/cvUnitVisSystem.h"
#include "Civ5/Src/App/cvUnitFormationLib.h"
#include "Civ5/Src/App/HEXVec_i.h"

class ActionTriggerData;
class AppUpdateData;
class AssetSwappedData;
class AssetTypeChangedData;
class Audio2DSoundData;
class Audio3DSoundData;
class AudioDebugChangeMusicData;
class AudioListenerData;
class AudioSoundTriggerData;
class AudioVolumeData;
class BuildingLibrarySwapData;
class ButtonUpData;
class CameraRephasedData;
class CameraViewData;
class ChangeProjectileTargetData;
class ChangeProjectileTransformData;
class CityBuildingStateData;
class CityContinentChangedData;
class CityFogData;
class ClearHexHighlightStyleData;
class CreateProjectileData;
class DebugHexSpaceStringData;
class DebugStringClearData;
class DebugStringToggleData;
class DebugWorldSpaceStringData;
class DestroyTradeRoute_EventData;
class DisplayMovementIndicatorData;
class EditProjectileData;
class FGXVector2;
class FrameGlobalsData;
class GameMessageDawnOfManData;
class GlobalUnitScaleData;
class HexHighlightData;
class HexID;
class LanguageChangingData;
class LeaderConfigureShadowsData;
class LeaderPlayAnimationData;
class LinearEventDispatcher;
class LoadGameData;
class LoadMapData;
class LocalMachineContainer;
class LuaOutputData;
class MapGlobalsData;
class MinimapAddIndicatorData;
class MinimapClickData;
class MinimapRemoveIndicatorData;
class NewHexRoadData;
class NewTradeRoute_EventData;
class NonSerialContainer;
class NullEventClass;
class OverlaySystem;
class ParticleEffectData;
class ParticleEffectDeSpawnedData;
class ParticleEffectHandleChangedData;
class ParticleEffectInfoResponseData;
class ParticleEffectStatsResponseData;
class PathFinderData;
class PlotData;
class PlotPacketData;
class ProjectileFinishedData;
class RadioSelectionChangedData;
class RemoveArrowEventData;
class RequestYieldDisplayData;
class SequencerContainer;
class SpawnArrowEventData;
class StateMachineChangeStateData;
class StateMachineDumpStatesData;
class StateMachineRequestStatesData;
class SteamLobbyInviteData;
class SteamServerInviteData;
class SystemInitData;
class SystemUpdateUIData;
class TerrainCacheData;
class TerrainStencilUpdatedData;
class ToggleTradeRouteDisplay_EventData;
class ToggleTradeRoutePopup_EventData;
class TradeRoutesPublishBuffers_EventData;
class UnitFormationCreateData;
class UnitFormationQueryAllData;
class UnitFormationQueryData;
class UnitFormationResultData;
class UnitFormationSetData;
class UnitHandleData;
class UnitHexHighlightData;
class UnitPositionChangedData;
class UpdateTradeRouteDirection_EventData;
class UserInputData;
class VisibilityUpdatedData;
class WonderGlobalVarsData;
class WonderSelectData;
class WonderStateChangedData;
class WorldGlobalsData;
class WorldMouseOverData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemNonSerialAccessor;
class cvEventSystemSequencerAccessor;
class cvSubscriptionValidator;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class EventPacketData;
template <int T0> class Event_Int2Type;
namespace FGXMathRect {
class Rect;
}

// Payload of event 105: mode 0 clears the city fog grid, 2 ends city fog, any other value marks one cell.
/// size ?
class CityFogData {
public:
    int m_iMode;    ///< +0x0
    int m_iRow;     ///< +0x4
    int m_iColumn;  ///< +0x8
};

// Payload of event 125.
/// size ?
class CityBuildingStateData {
public:
    float m_fBuildingSize;  ///< +0x0
};

// Payload of event 126.
/// size ?
class CityContinentChangedData {
public:
    short m_sHexX;      ///< +0x0
    short m_sHexY;      ///< +0x2
    int m_iContinent;   ///< +0x4
};

// Payload of event 114; the Lua marshaller copies the style name with strncpy(0x80).
/// size 0xa0
class HexHighlightData {
public:
    HEXVec_i m_kHex;            ///< +0x0
    float m_afColor[4];         ///< +0xc
    bool m_bEnable;             ///< +0x1c
    char m_szStyle[0x80];       ///< +0x1d
    unsigned char unk_9d[0x3];  ///< +0x9d
};

// Payload of event 11 (also event 79); the allocator initialises m_bEnable=1, m_bUseOffset=1.
/// size 0x20
class UnitHexHighlightData {
public:
    HEXVec_i m_kHex;            ///< +0x0
    float m_fOffsetX;           ///< +0xc
    float m_fOffsetY;           ///< +0x10
    float m_fOffsetZ;           ///< +0x14
    int m_iOwner;               ///< +0x18
    bool m_bEnable;             ///< +0x1c
    bool field_1d;              ///< +0x1d
    bool m_bUseOffset;          ///< +0x1e
    unsigned char unk_1f[0x1]; ///< +0x1f
};

/// size ?
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6>
class EventTemplate {
public:
    static UNK_DATA sm_aContexts[0x4];
};
/// 0x0a3408fb EventTemplate<Event_Int2Type<101>, SystemUpdateUIData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3d35e0 EventTemplate<Event_Int2Type<102>, SystemInitData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3d1f74 EventTemplate<Event_Int2Type<103>, WorldMouseOverData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3d35d8 EventTemplate<Event_Int2Type<104>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c01b5d0 EventTemplate<Event_Int2Type<105>, CityFogData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3e1f28 EventTemplate<Event_Int2Type<106>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3d35b0 EventTemplate<Event_Int2Type<107>, UserInputData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb87c EventTemplate<Event_Int2Type<110>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccd92c EventTemplate<Event_Int2Type<111>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c08c80a EventTemplate<Event_Int2Type<112>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c08c808 EventTemplate<Event_Int2Type<113>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c08ca04 EventTemplate<Event_Int2Type<114>, HexHighlightData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c08ca02 EventTemplate<Event_Int2Type<115>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c08ca00 EventTemplate<Event_Int2Type<116>, ClearHexHighlightStyleData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c087848 EventTemplate<Event_Int2Type<117>, NewTradeRoute_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c087846 EventTemplate<Event_Int2Type<118>, UpdateTradeRouteDirection_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c087844 EventTemplate<Event_Int2Type<119>, DestroyTradeRoute_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c044474 EventTemplate<Event_Int2Type<11>, UnitHexHighlightData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c087842 EventTemplate<Event_Int2Type<120>, ToggleTradeRouteDisplay_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c087840 EventTemplate<Event_Int2Type<121>, ToggleTradeRoutePopup_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccd914 EventTemplate<Event_Int2Type<122>, TradeRoutesPublishBuffers_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c08828a EventTemplate<Event_Int2Type<123>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c088288 EventTemplate<Event_Int2Type<124>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c019b84 EventTemplate<Event_Int2Type<125>, CityBuildingStateData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4cc148 EventTemplate<Event_Int2Type<126>, CityContinentChangedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4cc158 EventTemplate<Event_Int2Type<128>, AssetSwappedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03cbec EventTemplate<Event_Int2Type<129>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff9042 EventTemplate<Event_Int2Type<12>, AssetTypeChangedData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e42 EventTemplate<Event_Int2Type<12>, UnitHandleData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03cbea EventTemplate<Event_Int2Type<130>, LeaderPlayAnimationData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03cbe4 EventTemplate<Event_Int2Type<131>, LeaderConfigureShadowsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff91f6 EventTemplate<Event_Int2Type<134>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff91f4 EventTemplate<Event_Int2Type<137>, SteamLobbyInviteData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff91f2 EventTemplate<Event_Int2Type<138>, SteamServerInviteData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c077d80 EventTemplate<Event_Int2Type<139>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4cc14e EventTemplate<Event_Int2Type<13>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c07525e EventTemplate<Event_Int2Type<13>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c07525a EventTemplate<Event_Int2Type<14>, GlobalUnitScaleData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4cc14c EventTemplate<Event_Int2Type<14>, NewHexRoadData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb864 EventTemplate<Event_Int2Type<156>, GameMessageDawnOfManData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb862 EventTemplate<Event_Int2Type<157>, GameMessageDawnOfManData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb87a EventTemplate<Event_Int2Type<158>, UnitHandleData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4cc14a EventTemplate<Event_Int2Type<15>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c035a18 EventTemplate<Event_Int2Type<15>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3e7882 EventTemplate<Event_Int2Type<160>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb878 EventTemplate<Event_Int2Type<161>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb876 EventTemplate<Event_Int2Type<162>, LoadGameData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb874 EventTemplate<Event_Int2Type<163>, LoadMapData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f3362 EventTemplate<Event_Int2Type<164>, DisplayMovementIndicatorData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff9efc EventTemplate<Event_Int2Type<165>, SpawnArrowEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff9efa EventTemplate<Event_Int2Type<166>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff9ef8 EventTemplate<Event_Int2Type<167>, RemoveArrowEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7ed906 EventTemplate<Event_Int2Type<16>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c075256 EventTemplate<Event_Int2Type<16>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff9034 EventTemplate<Event_Int2Type<170>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3e873e EventTemplate<Event_Int2Type<17>, AudioVolumeData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c019b90 EventTemplate<Event_Int2Type<17>, BuildingLibrarySwapData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3e873c EventTemplate<Event_Int2Type<18>, Audio2DSoundData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3e873a EventTemplate<Event_Int2Type<19>, Audio3DSoundData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c019b88 EventTemplate<Event_Int2Type<19>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb86a EventTemplate<Event_Int2Type<1>, UnitPositionChangedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c075c20 EventTemplate<Event_Int2Type<20>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3e8736 EventTemplate<Event_Int2Type<21>, AudioSoundTriggerData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e28 EventTemplate<Event_Int2Type<21>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3e8734 EventTemplate<Event_Int2Type<22>, AudioSoundTriggerData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c075c1c EventTemplate<Event_Int2Type<22>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff903e EventTemplate<Event_Int2Type<23>, AssetTypeChangedData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c087b94 EventTemplate<Event_Int2Type<23>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccfcd8 EventTemplate<Event_Int2Type<24>, AudioDebugChangeMusicData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff903a EventTemplate<Event_Int2Type<24>, WonderStateChangedData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3e8738 EventTemplate<Event_Int2Type<25>, AudioListenerData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c019b8c EventTemplate<Event_Int2Type<25>, WonderGlobalVarsData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccfcd4 EventTemplate<Event_Int2Type<26>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff9036 EventTemplate<Event_Int2Type<26>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccfcd2 EventTemplate<Event_Int2Type<27>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c019b80 EventTemplate<Event_Int2Type<27>, WonderSelectData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3408f8 EventTemplate<Event_Int2Type<28>, TerrainCacheData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e32 EventTemplate<Event_Int2Type<28>, UnitFormationCreateData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e3e EventTemplate<Event_Int2Type<29>, UnitFormationQueryData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c077382 EventTemplate<Event_Int2Type<2>, AppUpdateData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3408fa EventTemplate<Event_Int2Type<2>, NullEventClass, LinearEventDispatcher, SequencerContainer, NullEventClass, BaseEvent<cvEventSystemSequencerAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e3a EventTemplate<Event_Int2Type<30>, UnitFormationQueryAllData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccf656 EventTemplate<Event_Int2Type<31>, UnitFormationResultData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4ccf70 EventTemplate<Event_Int2Type<32>, TerrainStencilUpdatedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e36 EventTemplate<Event_Int2Type<32>, UnitFormationSetData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff904a EventTemplate<Event_Int2Type<33>, FGXMathRect::Rect, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c01b5d2 EventTemplate<Event_Int2Type<34>, FrameGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3d2124 EventTemplate<Event_Int2Type<35>, VisibilityUpdatedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3e2916 EventTemplate<Event_Int2Type<36>, WorldGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3e2914 EventTemplate<Event_Int2Type<37>, MapGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c080c28 EventTemplate<Event_Int2Type<38>, RadioSelectionChangedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c088294 EventTemplate<Event_Int2Type<39>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c089ed6 EventTemplate<Event_Int2Type<3>, EventPacketData<FGXVector2, PlotPacketData>, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3d35da EventTemplate<Event_Int2Type<3>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c078768 EventTemplate<Event_Int2Type<3>, NullEventClass, LinearEventDispatcher, SequencerContainer, NullEventClass, BaseEvent<cvEventSystemSequencerAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f3658 EventTemplate<Event_Int2Type<40>, ParticleEffectData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f3656 EventTemplate<Event_Int2Type<41>, ParticleEffectData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f3654 EventTemplate<Event_Int2Type<42>, ParticleEffectDeSpawnedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f3652 EventTemplate<Event_Int2Type<43>, ParticleEffectHandleChangedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c056964 EventTemplate<Event_Int2Type<44>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c056962 EventTemplate<Event_Int2Type<45>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff8734 EventTemplate<Event_Int2Type<46>, ParticleEffectStatsResponseData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c056960 EventTemplate<Event_Int2Type<47>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0ccce9e0 EventTemplate<Event_Int2Type<48>, ParticleEffectInfoResponseData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f3650 EventTemplate<Event_Int2Type<49>, ParticleEffectData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff9046 EventTemplate<Event_Int2Type<4>, CameraViewData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f3846 EventTemplate<Event_Int2Type<51>, CreateProjectileData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f3844 EventTemplate<Event_Int2Type<52>, EditProjectileData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4ccaa4 EventTemplate<Event_Int2Type<53>, EditProjectileData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4ccaa8 EventTemplate<Event_Int2Type<54>, ProjectileFinishedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f3842 EventTemplate<Event_Int2Type<55>, ChangeProjectileTargetData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f3840 EventTemplate<Event_Int2Type<56>, ChangeProjectileTransformData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb872 EventTemplate<Event_Int2Type<57>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb870 EventTemplate<Event_Int2Type<58>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3d211c EventTemplate<Event_Int2Type<59>, CameraRephasedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c089ed8 EventTemplate<Event_Int2Type<5>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccf654 EventTemplate<Event_Int2Type<60>, StateMachineChangeStateData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccf652 EventTemplate<Event_Int2Type<61>, StateMachineChangeStateData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f3bc0 EventTemplate<Event_Int2Type<62>, StateMachineRequestStatesData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccf650 EventTemplate<Event_Int2Type<63>, StateMachineDumpStatesData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c07c495 EventTemplate<Event_Int2Type<65>, ButtonUpData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c077380 EventTemplate<Event_Int2Type<67>, LuaOutputData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c075262 EventTemplate<Event_Int2Type<6>, AnimationSamplingChangedData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c089ed4 EventTemplate<Event_Int2Type<6>, PlotData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c079238 EventTemplate<Event_Int2Type<76>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c019b98 EventTemplate<Event_Int2Type<7>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f3360 EventTemplate<Event_Int2Type<81>, PathFinderData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0ccd0baa EventTemplate<Event_Int2Type<82>, DebugHexSpaceStringData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0ccd0ba8 EventTemplate<Event_Int2Type<83>, DebugWorldSpaceStringData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0ccd0ba6 EventTemplate<Event_Int2Type<84>, DebugStringToggleData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0ccd0ba4 EventTemplate<Event_Int2Type<85>, DebugStringClearData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4ccaa6 EventTemplate<Event_Int2Type<86>, ActionTriggerData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3d2126 EventTemplate<Event_Int2Type<87>, RequestYieldDisplayData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3d35de EventTemplate<Event_Int2Type<89>, LanguageChangingData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3d35e4 EventTemplate<Event_Int2Type<8>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c019b94 EventTemplate<Event_Int2Type<8>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3e7880 EventTemplate<Event_Int2Type<91>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff91f0 EventTemplate<Event_Int2Type<92>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3e25c8 EventTemplate<Event_Int2Type<93>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03daa0 EventTemplate<Event_Int2Type<94>, MinimapClickData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03da9a EventTemplate<Event_Int2Type<96>, MinimapAddIndicatorData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03da98 EventTemplate<Event_Int2Type<97>, MinimapRemoveIndicatorData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3d35e2 EventTemplate<Event_Int2Type<9>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c088284 EventTemplate<Event_Int2Type<9>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6>
UNK_DATA EventTemplate<T0, T1, T2, T3, T4, T5, T6>::sm_aContexts[0x4];

extern "C" {
    /// 0x0cccd74d
    extern UNK_DATA bRegisteredBeginRoadUpdate[0x1];
    /// 0x0cccd766
    extern UNK_DATA bRegisteredClearHexHighlightStyle[0x1];
    /// 0x0cccd765
    extern UNK_DATA bRegisteredClearHexHighlights[0x1];
    /// 0x0cccd767
    extern UNK_DATA bRegisteredDisplayMovementIndicator[0x1];
    /// 0x0cccd74f
    extern UNK_DATA bRegisteredEndRoadUpdate[0x1];
    /// 0x0cccd76a
    extern UNK_DATA bRegisteredEvent_TradeRoutesPublishBuffers[0x1];
    /// 0x0cccd748
    extern UNK_DATA bRegisteredFloodplainCreated[0x1];
    /// 0x0cccd769
    extern UNK_DATA bRegisteredGameplayFX[0x1];
    /// 0x0cccd73a
    extern UNK_DATA bRegisteredGameplaySetActivePlayer[0x1];
    /// 0x0cccd73b
    extern UNK_DATA bRegisteredHexFOWStateChanged[0x1];
    /// 0x0cccd738
    extern UNK_DATA bRegisteredLocalMachineAppUpdate[0x1];
    /// 0x0cccd746
    extern UNK_DATA bRegisteredMarshCreated[0x1];
    /// 0x0cccd747
    extern UNK_DATA bRegisteredMarshRemoved[0x1];
    /// 0x0cccd74e
    extern UNK_DATA bRegisteredNewHexRoad[0x1];
    /// 0x0cccd754
    extern UNK_DATA bRegisteredSerialEventCityCaptured[0x1];
    /// 0x0cccd752
    extern UNK_DATA bRegisteredSerialEventCityCreated[0x1];
    /// 0x0cccd756
    extern UNK_DATA bRegisteredSerialEventCityCultureChanged[0x1];
    /// 0x0cccd755
    extern UNK_DATA bRegisteredSerialEventCityDestroyed[0x1];
    /// 0x0cccd753
    extern UNK_DATA bRegisteredSerialEventCityPopulationChanged[0x1];
    /// 0x0cccd757
    extern UNK_DATA bRegisteredSerialEventCitySetDamage[0x1];
    /// 0x0cccd73e
    extern UNK_DATA bRegisteredSerialEventEnterCityScreen[0x1];
    /// 0x0cccd73f
    extern UNK_DATA bRegisteredSerialEventExitCityScreen[0x1];
    /// 0x0cccd740
    extern UNK_DATA bRegisteredSerialEventFeatureCreated[0x1];
    /// 0x0cccd741
    extern UNK_DATA bRegisteredSerialEventFeatureDestroyed[0x1];
    /// 0x0cccd742
    extern UNK_DATA bRegisteredSerialEventForestCreated[0x1];
    /// 0x0cccd743
    extern UNK_DATA bRegisteredSerialEventForestRemoved[0x1];
    /// 0x0cccd751
    extern UNK_DATA bRegisteredSerialEventHexCultureChanged[0x1];
    /// 0x0cccd73d
    extern UNK_DATA bRegisteredSerialEventHexGridOff[0x1];
    /// 0x0cccd73c
    extern UNK_DATA bRegisteredSerialEventHexGridOn[0x1];
    /// 0x0cccd764
    extern UNK_DATA bRegisteredSerialEventHexHighlight[0x1];
    /// 0x0cccd74b
    extern UNK_DATA bRegisteredSerialEventImprovementIconCreated[0x1];
    /// 0x0cccd74c
    extern UNK_DATA bRegisteredSerialEventImprovementIconDestroyed[0x1];
    /// 0x0cccd744
    extern UNK_DATA bRegisteredSerialEventJungleCreated[0x1];
    /// 0x0cccd745
    extern UNK_DATA bRegisteredSerialEventJungleRemoved[0x1];
    /// 0x0cccd739
    extern UNK_DATA bRegisteredSerialEventMouseOverHex[0x1];
    /// 0x0cccd749
    extern UNK_DATA bRegisteredSerialEventRawResourceIconCreated[0x1];
    /// 0x0cccd74a
    extern UNK_DATA bRegisteredSerialEventRawResourceIconDestroyed[0x1];
    /// 0x0cccd750
    extern UNK_DATA bRegisteredSerialEventRoadDestroyed[0x1];
    /// 0x0cccd758
    extern UNK_DATA bRegisteredSerialEventUnitCreated[0x1];
    /// 0x0cccd759
    extern UNK_DATA bRegisteredSerialEventUnitDestroyed[0x1];
    /// 0x0cccd75a
    extern UNK_DATA bRegisteredSerialEventUnitMoveToHexes[0x1];
    /// 0x0cccd75d
    extern UNK_DATA bRegisteredSerialEventUnitSetDamage[0x1];
    /// 0x0cccd75b
    extern UNK_DATA bRegisteredSerialEventUnitTeleportedToHex[0x1];
    /// 0x0cccd768
    extern UNK_DATA bRegisteredUIPathFinderUpdate[0x1];
    /// 0x0cccd75f
    extern UNK_DATA bRegisteredUnitActionChanged[0x1];
    /// 0x0cccd760
    extern UNK_DATA bRegisteredUnitEmbark[0x1];
    /// 0x0cccd75e
    extern UNK_DATA bRegisteredUnitGarrison[0x1];
    /// 0x0cccd762
    extern UNK_DATA bRegisteredUnitMarkThreatening[0x1];
    /// 0x0cccd75c
    extern UNK_DATA bRegisteredUnitSelectionChanged[0x1];
    /// 0x0cccd761
    extern UNK_DATA bRegisteredUnitShouldDimFlag[0x1];
    /// 0x0cccd763
    extern UNK_DATA bRegisteredUnitVisibilityChanged[0x1];
    /// 0x0c088cb8
    extern UNK_DATA g_DynamicGeometryLibrary[0xd30];
    /// 0x0ccd06b4
    extern UNK_DATA g_EventChannelManager[0x4];
    /// 0x0c01b778
    extern UNK_DATA g_FOWSystem[0xa6c];
    /// 0x0c019ee4
    extern UNK_DATA g_FluidFOWSystem[0x1548];
    /// 0x0a4ccdc4
    extern UNK_DATA g_ForestSimSystem[0x8];
    /// 0x0ccd0bac
    extern UNK_DATA g_HighlightLibrary[0x1c];
    /// 0x0c036000
    extern UNK_DATA g_LeaderHeadSystem[0x6a40];
    /// 0x0c03d288
    extern UNK_DATA g_MinimapSystem[0x20];
    /// 0x0c044478
    extern OverlaySystem g_OverlaySystem;
    /// 0x0c056968
    extern UNK_DATA g_RiverVisSystem[0x30];
    /// 0x0c08d328
    extern UNK_DATA g_StrategicViewTracker[0xc40410];
    /// 0x0c08b018
    extern cvUnitFormationLib g_UnitFormationLib;
    /// 0x0c075268
    extern UNK_DATA g_WaveSystem[0xb0];
    /// 0x0a3d22d4
    extern UNK_DATA g_bEnableTuner[0x1];
    /// 0x0a3e7890
    extern UNK_DATA g_cvAudioSimSystem[0xb30];
    /// 0x0a3e8564
    extern UNK_DATA g_cvAudioSystem[0x2c];
    /// 0x0a3e8924
    extern UNK_DATA g_cvCityBuildingSim[0xe35c0];
    /// 0x0bff9f00
    extern UNK_DATA g_cvCityVisSystem[0x1fad4];
    /// 0x0a7f3dc0
    extern UNK_DATA g_cvDecalSim[0xc];
    /// 0x0a4ccaac
    extern UNK_DATA g_cvDynamicGeometryVisSystem[0x174];
    /// 0x0c01c52c
    extern UNK_DATA g_cvLandmarkVisSystem[0x19344];
    /// 0x0a7f3370
    extern UNK_DATA g_cvParticleSimSystem[0xf0];
    /// 0x0c0566a0
    extern UNK_DATA g_cvParticleVisSystem[0x11c];
    /// 0x0c08cf54
    extern UNK_DATA g_cvStrategicViewFeatureMap[0xc];
    /// 0x0c08cf10
    extern UNK_DATA g_cvStrategicViewFeatureTypeMap[0x1c];
    /// 0x0c08cf6c
    extern UNK_DATA g_cvStrategicViewImprovementMap[0xc];
    /// 0x0c08cf2c
    extern UNK_DATA g_cvStrategicViewImprovementTypeMap[0x1c];
    /// 0x0c08cf60
    extern UNK_DATA g_cvStrategicViewRawResourceMap[0xc];
    /// 0x0c08cf48
    extern UNK_DATA g_cvStrategicViewTerrainMap[0xc];
    /// 0x0c08cef4
    extern UNK_DATA g_cvStrategicViewTerrainTypeMap[0x1c];
    /// 0x0c08cf78
    extern UNK_DATA g_cvStrategicViewUnitMap[0xc];
    /// 0x0c056b3c
    extern UNK_DATA g_cvStrategicViewVisSystem[0x12a4];
    /// 0x0a3d22d0
    extern UNK_DATA g_pkApplication[0x4];
    /// 0x0c08d128
    extern UNK_DATA m_aStrategicViewTrackerSubscribeFuncs[0x100];
    /// 0x0c08d228
    extern UNK_DATA m_aStrategicViewTrackerUnsubscribeFuncs[0x100];
    /// 0x0d2650f4
    extern UNK_DATA s_MemoryUsage[0x1c];
    /// 0x0c03cf38
    extern UNK_DATA s_SortTime[0x4];
    /// 0x0c03cf3c
    extern UNK_DATA s_nSortTris[0x4];
    /// 0x0d2658ec
    extern UNK_DATA s_pkLogger[0x4];
}

namespace Localization {
/// 0x0d265cac
extern UNK_DATA m_iCurrentLanguage[0x4];
/// 0x0d265ca0
extern UNK_DATA m_languages[0xc];
/// 0x0d265c98
extern UNK_DATA m_szICUDataFileBuffer[0x4];
}

namespace cvContentPackageManager {
/// 0x0a3e1f2c
extern UNK_DATA ms_kPackages[0xc];
}

namespace cvProfileManager {
/// 0x0c07876c
extern UNK_DATA ms_kPendingReports[0xc];
}
