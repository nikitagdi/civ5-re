/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/cvUnitVisSystem.h"
#include <d3d9.h>
#include <lua.h>
#include <ostream>
#include <string>

class AILeaderMessageData;
class AIProcessingPlayerData;
class ActivePlayerData;
class AdvisorDisplayShowData;
class AppUpdateData;
class AssetTypeChangedData;
class Audio2DSoundData;
class AudioDebugChangeMusicData;
class AudioVolumeData;
class BuildingLibrarySwapData;
class CameraProjectionData;
class CameraViewData;
class CameraZoomData;
class CityBuildingStateData;
class CityCaptureData;
class CityContinentChangedData;
class CityCreatedData;
class CityCultureChangedData;
class CityGenericData;
class CityHandleCreatedData;
class CityInterfaceUpdateData;
class CityPopulationChangedData;
class CitySetDamageData;
class ClearHexHighlightStyleData;
class DisconnectEventData;
class DisplayMovementIndicatorData;
class DragCameraData;
class EndCombatSimData;
class EndGameShowData;
class EndTurnBlockingChangedData;
class EndTurnTimerUpdateData;
class EraChangedData;
class FGXInt2;
class FGXVector2;
class FGXVector3;
class FGXVector4;
class FXmlElement;
class FeatureChangedData;
class FilePathData;
class FloatData;
class ForestChangedData;
class FrontEndPopupData;
class GameAbandonedEventData;
class GameMessageChatData;
class GameMessageDawnOfManData;
class GameMessagePopupData;
class GameMessagePopupProcessedData;
class GameViewTypeChangedData;
class GameplayAlertMessageData;
class GameplayFXData;
class GenericWorldAnchorData;
class GlobalUnitScaleData;
class HexCultureChangedData;
class HexFOWStateChangedData;
class HexHighlightData;
class HexID;
class HostMigrationEventData;
class InitCityRangeStrikeData;
class InterfaceModeChangedData;
class JoinRoomFailedEventData;
class KeyUpData;
class LandmarkCreatedData;
class LandmarkHandleData;
class LandmarkIconCreatedData;
class LanguageChangingData;
class LeaderArtDefineData;
class Leaderboard_ScoresDownloadedData;
class LinearEventDispatcher;
class LoadGameData;
class LoadMapData;
class LocalMachineContainer;
class MinimapClickData;
class MinimapTextureBroadcastData;
class MultiplayerGameListUpdatedData;
class NewGameTurnData;
class NotificationActivatedData;
class NotificationAddedData;
class NotificationRemovedData;
class NullEventClass;
class OpenInfoCornerData;
class OpenPlayerDealScreenEventData;
class ParticleEffectStatsResponseData;
class PathFinderData;
class PediaHomePageData;
class PlayerVersionMismatchData;
class PopupTextEventData;
class RequestYieldDisplayData;
class RoadCreatedData;
class RunCombatSimData;
class ScreenShotData;
class SerialContainer;
class ShowHexYieldData;
class ShowPlayerChangeUIData;
class SpawnArrowEventData;
class StateMachineDumpStatesData;
class StateMachineRequestStatesData;
class SteamLobbyInviteData;
class SteamServerInviteData;
class StrategicViewStateChangedData;
class SystemUpdateUIData;
class TaskListUpdateData;
class TeamMetData;
class TechAcquiredData;
class TerrainDecalIDData;
class TerrainOverlayModData;
class ToggleTradeRouteDisplay_EventData;
class ToolTipData;
class UnitBooleanData;
class UnitCreatedData;
class UnitDamagedData;
class UnitDataEditedData;
class UnitDataRequestedData;
class UnitDebugFSMData;
class UnitFacingChangedData;
class UnitFlagUpdatedData;
class UnitHandleCreatedData;
class UnitHandleData;
class UnitHealedData;
class UnitHexHighlightData;
class UnitMemberCombatTargetData;
class UnitMemberHandleData;
class UnitMemberOverlayMessageData;
class UnitMemberOverlayStateData;
class UnitMemberPositionData;
class UnitMemberStateData;
class UnitMoveData;
class UnitMoveToHexesData;
class UnitPositionChangedData;
class UnitSelectionChangedData;
class UnitSetDamageData;
class UnitStateChangeDetectedData;
class UnitTeleportedToHexData;
class UnitVisibilityChangedData;
class VisibilityUpdatedData;
class WarStateChangedData;
class WonderStateChangedData;
class WorldMouseOverData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
class cvTerrainSampler;
enum MD5 : int;
enum SMTP_COMMAND : int;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;
namespace FireGrafix {
class CCommandStream;
}

/// size ?
class HighlightLibrary {
public:
    enum StyleType : int {
        STYLE_HEX_CONTOUR = 0,
        STYLE_SPLINE_BORDER = 1,
        STYLE_HEX_MODEL = 2,
        STYLE_STATIC_TEXTURE = 3,
        STYLE_FILLED_HEX = 4,
    };
    /// size 0x9c
    class Entry {
    public:
        char const* name; ///< +0x0
        StyleType type; ///< +0x4
        float width; ///< +0x8
        float color[4]; ///< +0xc
        char texture[64]; ///< +0x1c
        char model[64]; ///< +0x5c
    };
    /// 0x08b7e820
    HighlightLibrary();
    /// 0x08b7e848
    ~HighlightLibrary();
    /// 0x08b7e85c
    bool Init(char const* a1);
    /// 0x08b7e9be
    bool AddEntry(FXmlElement const& a1);
    /// 0x08b7edd0
    UNK_RET Release();
    /// 0x08b7ede4
    Entry& GetEntry(char const* a1);
    /// 0x08b7ee3e
    void ExtractColor(char const* a1, FGXVector4& a2) const;
};

/// 0x08b7fa38 lua_toObject<D3DXMATRIX>
/// 0x08b7fb99 lua_toObject<FGXVector2>
/// 0x08b7fce7 lua_toObject<FGXVector3>
template <class T0>
void lua_toObject(lua_State* a1, int a2, T0& a3);
/// 0x08b7fae2 lua_pushObject<D3DXMATRIX>
/// 0x08b7fc45 lua_pushObject<FGXVector2>
/// 0x08b7fdcc lua_pushObject<FGXVector3>
template <class T0>
void lua_pushObject(lua_State* a1, T0& a2);
/// 0x08b88c28
UNK_RET FindCommandEntry(SMTP_COMMAND a1);
/// 0x08b88c54
UNK_RET IsKeywordSupported(char const* a1, char const* a2);
/// 0x08b88d3f
UNK_RET CharToUnsignedChar(char const* a1);
/// 0x08b9118a
UNK_RET operator<<(std::basic_ostream<char, std::char_traits<char> >& a1, MD5 a2);

namespace LuaSupport {
/// 0x08b7fea3
UNK_RET FromLua(lua_State* a1, AnimationSamplingChangedData& a2);
/// 0x08b7fed4
UNK_RET ToLua(lua_State* a1, AnimationSamplingChangedData& a2);
/// 0x08b7feff
UNK_RET FromLua(lua_State* a1, AppUpdateData& a2);
/// 0x08b7ff45
UNK_RET ToLua(lua_State* a1, AppUpdateData& a2);
/// 0x08b7ff87
UNK_RET FromLua(lua_State* a1, CityHandleCreatedData& a2);
/// 0x08b7ffcd
UNK_RET ToLua(lua_State* a1, CityHandleCreatedData& a2);
/// 0x08b8000b
UNK_RET FromLua(lua_State* a1, HexFOWStateChangedData& a2);
/// 0x08b80205
UNK_RET ToLua(lua_State* a1, HexFOWStateChangedData& a2);
/// 0x08b80397
UNK_RET FromLua(lua_State* a1, HexID& a2);
/// 0x08b803df
UNK_RET ToLua(lua_State* a1, HexID& a2);
/// 0x08b8041f
UNK_RET FromLua(lua_State* a1, FGXInt2& a2);
/// 0x08b80465
UNK_RET ToLua(lua_State* a1, FGXInt2& a2);
/// 0x08b804a3
UNK_RET FromLua(lua_State* a1, CameraProjectionData& a2);
/// 0x08b804d4
UNK_RET ToLua(lua_State* a1, CameraProjectionData& a2);
/// 0x08b804e8
UNK_RET FromLua(lua_State* a1, CameraViewData& a2);
/// 0x08b80519
UNK_RET ToLua(lua_State* a1, CameraViewData& a2);
/// 0x08b8052d
UNK_RET FromLua(lua_State* a1, CityCreatedData& a2);
/// 0x08b80604
UNK_RET ToLua(lua_State* a1, CityCreatedData& a2);
/// 0x08b806c0
UNK_RET FromLua(lua_State* a1, CityCultureChangedData& a2);
/// 0x08b8072f
UNK_RET ToLua(lua_State* a1, CityCultureChangedData& a2);
/// 0x08b80791
UNK_RET FromLua(lua_State* a1, CityGenericData& a2);
/// 0x08b807f6
UNK_RET ToLua(lua_State* a1, CityGenericData& a2);
/// 0x08b80857
UNK_RET FromLua(lua_State* a1, CityCaptureData& a2);
/// 0x08b808cf
UNK_RET ToLua(lua_State* a1, CityCaptureData& a2);
/// 0x08b8093f
UNK_RET FromLua(lua_State* a1, CityInterfaceUpdateData& a2);
/// 0x08b80998
UNK_RET ToLua(lua_State* a1, CityInterfaceUpdateData& a2);
/// 0x08b809e5
UNK_RET FromLua(lua_State* a1, CityPopulationChangedData& a2);
/// 0x08b80a80
UNK_RET ToLua(lua_State* a1, CityPopulationChangedData& a2);
/// 0x08b80ade
UNK_RET FromLua(lua_State* a1, CityContinentChangedData& a2);
/// 0x08b80b39
UNK_RET ToLua(lua_State* a1, CityContinentChangedData& a2);
/// 0x08b80b88
UNK_RET FromLua(lua_State* a1, CitySetDamageData& a2);
/// 0x08b80c2f
UNK_RET ToLua(lua_State* a1, CitySetDamageData& a2);
/// 0x08b80cba
UNK_RET FromLua(lua_State* a1, CityBuildingStateData& a2);
/// 0x08b80ceb
UNK_RET ToLua(lua_State* a1, CityBuildingStateData& a2);
/// 0x08b80d1a
UNK_RET FromLua(lua_State* a1, LanguageChangingData& a2);
/// 0x08b80e16
UNK_RET ToLua(lua_State* a1, LanguageChangingData& a2);
/// 0x08b80e90
UNK_RET FromLua(lua_State* a1, RoadCreatedData& a2);
/// 0x08b80f12
UNK_RET ToLua(lua_State* a1, RoadCreatedData& a2);
/// 0x08b80f8d
UNK_RET FromLua(lua_State* a1, EraChangedData& a2);
/// 0x08b80fd3
UNK_RET ToLua(lua_State* a1, EraChangedData& a2);
/// 0x08b81011
UNK_RET FromLua(lua_State* a1, FGXVector2& a2);
/// 0x08b81042
UNK_RET ToLua(lua_State* a1, FGXVector2& a2);
/// 0x08b81056
UNK_RET FromLua(lua_State* a1, FGXVector3& a2);
/// 0x08b81087
UNK_RET ToLua(lua_State* a1, FGXVector3& a2);
/// 0x08b8109b
UNK_RET FromLua(lua_State* a1, ForestChangedData& a2);
/// 0x08b810e1
UNK_RET ToLua(lua_State* a1, ForestChangedData& a2);
/// 0x08b8111f
UNK_RET FromLua(lua_State* a1, AssetTypeChangedData& a2);
/// 0x08b8117a
UNK_RET ToLua(lua_State* a1, AssetTypeChangedData& a2);
/// 0x08b811ab
UNK_RET FromLua(lua_State* a1, WonderStateChangedData& a2);
/// 0x08b811dc
UNK_RET ToLua(lua_State* a1, WonderStateChangedData& a2);
/// 0x08b81207
UNK_RET FromLua(lua_State* a1, GlobalUnitScaleData& a2);
/// 0x08b81238
UNK_RET ToLua(lua_State* a1, GlobalUnitScaleData& a2);
/// 0x08b81263
UNK_RET FromLua(lua_State* a1, BuildingLibrarySwapData& a2);
/// 0x08b812a9
UNK_RET ToLua(lua_State* a1, BuildingLibrarySwapData& a2);
/// 0x08b812e7
UNK_RET FromLua(lua_State* a1, HexCultureChangedData& a2);
/// 0x08b81359
UNK_RET ToLua(lua_State* a1, HexCultureChangedData& a2);
/// 0x08b813b8
UNK_RET FromLua(lua_State* a1, HexHighlightData& a2);
/// 0x08b81754
UNK_RET ToLua(lua_State* a1, HexHighlightData& a2);
/// 0x08b81918
UNK_RET FromLua(lua_State* a1, ClearHexHighlightStyleData& a2);
/// 0x08b81988
UNK_RET ToLua(lua_State* a1, ClearHexHighlightStyleData& a2);
/// 0x08b8199c
UNK_RET FromLua(lua_State* a1, SpawnArrowEventData& a2);
/// 0x08b81b14
UNK_RET ToLua(lua_State* a1, SpawnArrowEventData& a2);
/// 0x08b81b15
UNK_RET FromLua(lua_State* a1, FilePathData& a2);
/// 0x08b81b82
UNK_RET ToLua(lua_State* a1, FilePathData& a2);
/// 0x08b81c27
UNK_RET FromLua(lua_State* a1, LoadGameData& a2);
/// 0x08b81ccb
UNK_RET ToLua(lua_State* a1, LoadGameData& a2);
/// 0x08b81d85
UNK_RET FromLua(lua_State* a1, LoadMapData& a2);
/// 0x08b81e29
UNK_RET ToLua(lua_State* a1, LoadMapData& a2);
/// 0x08b81ee3
UNK_RET FromLua(lua_State* a1, LeaderArtDefineData& a2);
/// 0x08b81f6e
UNK_RET ToLua(lua_State* a1, LeaderArtDefineData& a2);
/// 0x08b81fb4
UNK_RET FromLua(lua_State* a1, AILeaderMessageData& a2);
/// 0x08b81fb5
UNK_RET ToLua(lua_State* a1, AILeaderMessageData& a2);
/// 0x08b82026
UNK_RET FromLua(lua_State* a1, TeamMetData& a2);
/// 0x08b8206c
UNK_RET ToLua(lua_State* a1, TeamMetData& a2);
/// 0x08b820aa
UNK_RET FromLua(lua_State* a1, WarStateChangedData& a2);
/// 0x08b82106
UNK_RET ToLua(lua_State* a1, WarStateChangedData& a2);
/// 0x08b82154
UNK_RET FromLua(lua_State* a1, LandmarkCreatedData& a2);
/// 0x08b82221
UNK_RET ToLua(lua_State* a1, LandmarkCreatedData& a2);
/// 0x08b822ca
UNK_RET FromLua(lua_State* a1, LandmarkIconCreatedData& a2);
/// 0x08b8234b
UNK_RET ToLua(lua_State* a1, LandmarkIconCreatedData& a2);
/// 0x08b823b8
UNK_RET FromLua(lua_State* a1, LandmarkHandleData& a2);
/// 0x08b8242e
UNK_RET ToLua(lua_State* a1, LandmarkHandleData& a2);
/// 0x08b8246e
UNK_RET FromLua(lua_State* a1, NullEventClass& a2);
/// 0x08b8246f
UNK_RET ToLua(lua_State* a1, NullEventClass& a2);
/// 0x08b82470
UNK_RET FromLua(lua_State* a1, FeatureChangedData& a2);
/// 0x08b824d5
UNK_RET ToLua(lua_State* a1, FeatureChangedData& a2);
/// 0x08b82523
UNK_RET FromLua(lua_State* a1, TerrainDecalIDData& a2);
/// 0x08b82569
UNK_RET ToLua(lua_State* a1, TerrainDecalIDData& a2);
/// 0x08b825a7
UNK_RET FromLua(lua_State* a1, UnitCreatedData& a2);
/// 0x08b82833
UNK_RET ToLua(lua_State* a1, UnitCreatedData& a2);
/// 0x08b82a4e
UNK_RET FromLua(lua_State* a1, UnitPositionChangedData& a2);
/// 0x08b82abc
UNK_RET ToLua(lua_State* a1, UnitPositionChangedData& a2);
/// 0x08b82b0e
UNK_RET FromLua(lua_State* a1, TerrainOverlayModData& a2);
/// 0x08b82b6b
UNK_RET ToLua(lua_State* a1, TerrainOverlayModData& a2);
/// 0x08b82bb8
UNK_RET FromLua(lua_State* a1, UnitDamagedData& a2);
/// 0x08b82c11
UNK_RET ToLua(lua_State* a1, UnitDamagedData& a2);
/// 0x08b82c5e
UNK_RET FromLua(lua_State* a1, UnitFacingChangedData& a2);
/// 0x08b82cb7
UNK_RET ToLua(lua_State* a1, UnitFacingChangedData& a2);
/// 0x08b82d08
UNK_RET FromLua(lua_State* a1, UnitVisibilityChangedData& a2);
/// 0x08b82d8d
UNK_RET ToLua(lua_State* a1, UnitVisibilityChangedData& a2);
/// 0x08b82dfe
UNK_RET FromLua(lua_State* a1, UnitHandleCreatedData& a2);
/// 0x08b82e44
UNK_RET ToLua(lua_State* a1, UnitHandleCreatedData& a2);
/// 0x08b82e82
UNK_RET FromLua(lua_State* a1, UnitHandleData& a2);
/// 0x08b82ec8
UNK_RET ToLua(lua_State* a1, UnitHandleData& a2);
/// 0x08b82f06
UNK_RET FromLua(lua_State* a1, UnitHealedData& a2);
/// 0x08b82f72
UNK_RET ToLua(lua_State* a1, UnitHealedData& a2);
/// 0x08b82fce
UNK_RET FromLua(lua_State* a1, UnitSetDamageData& a2);
/// 0x08b8303a
UNK_RET ToLua(lua_State* a1, UnitSetDamageData& a2);
/// 0x08b83096
UNK_RET FromLua(lua_State* a1, UnitBooleanData& a2);
/// 0x08b830ef
UNK_RET ToLua(lua_State* a1, UnitBooleanData& a2);
/// 0x08b83143
UNK_RET FromLua(lua_State* a1, UnitMoveToHexesData& a2);
/// 0x08b83189
UNK_RET ToLua(lua_State* a1, UnitMoveToHexesData& a2);
/// 0x08b831c7
UNK_RET FromLua(lua_State* a1, UnitMoveData& a2);
/// 0x08b8320d
UNK_RET ToLua(lua_State* a1, UnitMoveData& a2);
/// 0x08b8324b
UNK_RET FromLua(lua_State* a1, UnitTeleportedToHexData& a2);
/// 0x08b832c2
UNK_RET ToLua(lua_State* a1, UnitTeleportedToHexData& a2);
/// 0x08b8331e
UNK_RET FromLua(lua_State* a1, UnitFlagUpdatedData& a2);
/// 0x08b8338e
UNK_RET ToLua(lua_State* a1, UnitFlagUpdatedData& a2);
/// 0x08b833ef
UNK_RET FromLua(lua_State* a1, UnitSelectionChangedData& a2);
/// 0x08b834a8
UNK_RET ToLua(lua_State* a1, UnitSelectionChangedData& a2);
/// 0x08b83533
UNK_RET FromLua(lua_State* a1, UnitDataEditedData& a2);
/// 0x08b83806
UNK_RET ToLua(lua_State* a1, UnitDataEditedData& a2);
/// 0x08b83934
UNK_RET FromLua(lua_State* a1, UnitDataRequestedData& a2);
/// 0x08b83965
UNK_RET ToLua(lua_State* a1, UnitDataRequestedData& a2);
/// 0x08b83990
UNK_RET FromLua(lua_State* a1, UnitDebugFSMData& a2);
/// 0x08b839c1
UNK_RET ToLua(lua_State* a1, UnitDebugFSMData& a2);
/// 0x08b839ec
UNK_RET FromLua(lua_State* a1, GameMessagePopupData& a2);
/// 0x08b83ef0
UNK_RET ToLua(lua_State* a1, GameMessagePopupData& a2);
/// 0x08b8435c
UNK_RET FromLua(lua_State* a1, GameMessagePopupProcessedData& a2);
/// 0x08b843a2
UNK_RET ToLua(lua_State* a1, GameMessagePopupProcessedData& a2);
/// 0x08b843e0
UNK_RET FromLua(lua_State* a1, GameMessageDawnOfManData& a2);
/// 0x08b84411
UNK_RET ToLua(lua_State* a1, GameMessageDawnOfManData& a2);
/// 0x08b8443c
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<111>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b84488
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<111>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b844c8
UNK_RET FromLua(lua_State* a1, AudioDebugChangeMusicData& a2);
/// 0x08b8452a
UNK_RET ToLua(lua_State* a1, AudioDebugChangeMusicData& a2);
/// 0x08b8457a
UNK_RET FromLua(lua_State* a1, AudioVolumeData& a2);
/// 0x08b845c0
UNK_RET ToLua(lua_State* a1, AudioVolumeData& a2);
/// 0x08b84602
UNK_RET FromLua(lua_State* a1, GameplayAlertMessageData& a2);
/// 0x08b8464d
UNK_RET ToLua(lua_State* a1, GameplayAlertMessageData& a2);
/// 0x08b84661
UNK_RET FromLua(lua_State* a1, UnitStateChangeDetectedData& a2);
/// 0x08b846be
UNK_RET ToLua(lua_State* a1, UnitStateChangeDetectedData& a2);
/// 0x08b8470d
UNK_RET FromLua(lua_State* a1, UnitHexHighlightData& a2);
/// 0x08b84797
UNK_RET ToLua(lua_State* a1, UnitHexHighlightData& a2);
/// 0x08b8480d
UNK_RET FromLua(lua_State* a1, InterfaceModeChangedData& a2);
/// 0x08b84853
UNK_RET ToLua(lua_State* a1, InterfaceModeChangedData& a2);
/// 0x08b84891
UNK_RET FromLua(lua_State* a1, EndTurnTimerUpdateData& a2);
/// 0x08b848c2
UNK_RET ToLua(lua_State* a1, EndTurnTimerUpdateData& a2);
/// 0x08b848f1
UNK_RET FromLua(lua_State* a1, ParticleEffectStatsResponseData& a2);
/// 0x08b84aa9
UNK_RET ToLua(lua_State* a1, ParticleEffectStatsResponseData& a2);
/// 0x08b84c9f
UNK_RET FromLua(lua_State* a1, PathFinderData& a2);
/// 0x08b84cca
UNK_RET ToLua(lua_State* a1, PathFinderData& a2);
/// 0x08b84dec
UNK_RET FromLua(lua_State* a1, DisplayMovementIndicatorData& a2);
/// 0x08b84e20
UNK_RET ToLua(lua_State* a1, DisplayMovementIndicatorData& a2);
/// 0x08b84e4c
UNK_RET FromLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<121>, NotificationAddedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b84e77
UNK_RET ToLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<121>, NotificationAddedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b84f21
UNK_RET FromLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<122>, NotificationRemovedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b84f68
UNK_RET ToLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<122>, NotificationRemovedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b84faf
UNK_RET FromLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<123>, NotificationActivatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b84fda
UNK_RET ToLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<123>, NotificationActivatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b85090
UNK_RET FromLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<128>, TechAcquiredData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b850bb
UNK_RET ToLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<128>, TechAcquiredData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b85102
UNK_RET FromLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<111>, CityInterfaceUpdateData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b8515c
UNK_RET ToLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<111>, CityInterfaceUpdateData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b851aa
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<64>, KeyUpData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b851d5
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<64>, KeyUpData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b85200
UNK_RET FromLua(lua_State* a1, Audio2DSoundData& a2);
/// 0x08b85284
UNK_RET ToLua(lua_State* a1, Audio2DSoundData& a2);
/// 0x08b852af
UNK_RET FromLua(lua_State* a1, AdvisorDisplayShowData& a2);
/// 0x08b85609
UNK_RET ToLua(lua_State* a1, AdvisorDisplayShowData& a2);
/// 0x08b858ed
UNK_RET FromLua(lua_State* a1, EndGameShowData& a2);
/// 0x08b85933
UNK_RET ToLua(lua_State* a1, EndGameShowData& a2);
/// 0x08b85971
UNK_RET FromLua(lua_State* a1, TaskListUpdateData& a2);
/// 0x08b85abb
UNK_RET ToLua(lua_State* a1, TaskListUpdateData& a2);
/// 0x08b85bf2
UNK_RET FromLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<129>, PopupTextEventData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b85c8c
UNK_RET ToLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<129>, PopupTextEventData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b85cd9
UNK_RET FromLua(lua_State* a1, FloatData& a2);
/// 0x08b85d0a
UNK_RET ToLua(lua_State* a1, FloatData& a2);
/// 0x08b85d39
UNK_RET FromLua(lua_State* a1, UnitMemberOverlayStateData& a2);
/// 0x08b85dc6
UNK_RET ToLua(lua_State* a1, UnitMemberOverlayStateData& a2);
/// 0x08b85e2f
UNK_RET FromLua(lua_State* a1, UnitMemberOverlayMessageData& a2);
/// 0x08b85ed5
UNK_RET ToLua(lua_State* a1, UnitMemberOverlayMessageData& a2);
/// 0x08b85f3d
UNK_RET FromLua(lua_State* a1, UnitMemberPositionData& a2);
/// 0x08b85fdc
UNK_RET ToLua(lua_State* a1, UnitMemberPositionData& a2);
/// 0x08b86041
UNK_RET FromLua(lua_State* a1, UnitMemberCombatTargetData& a2);
/// 0x08b8610f
UNK_RET ToLua(lua_State* a1, UnitMemberCombatTargetData& a2);
/// 0x08b861a1
UNK_RET FromLua(lua_State* a1, UnitMemberStateData& a2);
/// 0x08b86235
UNK_RET ToLua(lua_State* a1, UnitMemberStateData& a2);
/// 0x08b862a1
UNK_RET FromLua(lua_State* a1, UnitMemberHandleData& a2);
/// 0x08b86318
UNK_RET ToLua(lua_State* a1, UnitMemberHandleData& a2);
/// 0x08b86371
UNK_RET FromLua(lua_State* a1, StateMachineRequestStatesData& a2);
/// 0x08b863cf
UNK_RET ToLua(lua_State* a1, StateMachineRequestStatesData& a2);
/// 0x08b8640f
UNK_RET FromLua(lua_State* a1, StateMachineDumpStatesData& a2);
/// 0x08b86432
UNK_RET ToLua(lua_State* a1, StateMachineDumpStatesData& a2);
/// 0x08b8654d
UNK_RET FromLua(lua_State* a1, ScreenShotData& a2);
/// 0x08b86581
UNK_RET ToLua(lua_State* a1, ScreenShotData& a2);
/// 0x08b865ad
UNK_RET FromLua(lua_State* a1, ToolTipData& a2);
/// 0x08b865d8
UNK_RET ToLua(lua_State* a1, ToolTipData& a2);
/// 0x08b865ec
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<87>, RequestYieldDisplayData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86687
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<87>, RequestYieldDisplayData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b866e8
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<35>, VisibilityUpdatedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86713
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<35>, VisibilityUpdatedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b8677d
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<88>, ShowHexYieldData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b867a8
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<88>, ShowHexYieldData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b867f8
UNK_RET FromLua(lua_State* a1, ActivePlayerData& a2);
/// 0x08b86823
UNK_RET ToLua(lua_State* a1, ActivePlayerData& a2);
/// 0x08b86861
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<155>, GameMessageChatData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b8688c
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<155>, GameMessageChatData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b868eb
UNK_RET FromLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<146>, OpenPlayerDealScreenEventData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b8691d
UNK_RET ToLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<146>, OpenPlayerDealScreenEventData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86949
UNK_RET FromLua(lua_State* a1, StrategicViewStateChangedData& a2);
/// 0x08b869a8
UNK_RET ToLua(lua_State* a1, StrategicViewStateChangedData& a2);
/// 0x08b869f0
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<95>, MinimapTextureBroadcastData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86a75
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<95>, MinimapTextureBroadcastData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86b09
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<94>, MinimapClickData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86b4f
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<94>, MinimapClickData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86b95
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<120>, ToggleTradeRouteDisplay_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86bc9
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<120>, ToggleTradeRouteDisplay_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86bf5
UNK_RET FromLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<89>, RunCombatSimData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86c20
UNK_RET ToLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<89>, RunCombatSimData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86d73
UNK_RET FromLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<90>, EndCombatSimData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86d9e
UNK_RET ToLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<90>, EndCombatSimData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86ede
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<100>, DragCameraData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86f09
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<100>, DragCameraData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86f75
UNK_RET FromLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<147>, OpenInfoCornerData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b86fd4
UNK_RET ToLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<147>, OpenInfoCornerData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b87015
UNK_RET FromLua(lua_State* a1, GameplayFXData& a2);
/// 0x08b87077
UNK_RET ToLua(lua_State* a1, GameplayFXData& a2);
/// 0x08b87078
UNK_RET FromLua(lua_State* a1, PediaHomePageData& a2);
/// 0x08b870a9
UNK_RET ToLua(lua_State* a1, PediaHomePageData& a2);
/// 0x08b870d8
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<101>, SystemUpdateUIData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b87289
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<101>, SystemUpdateUIData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b87306
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<169>, FrontEndPopupData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b87351
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<169>, FrontEndPopupData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b87365
UNK_RET FromLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<149>, GenericWorldAnchorData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b873e8
UNK_RET ToLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<149>, GenericWorldAnchorData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b87455
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<153>, DisconnectEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b87486
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<153>, DisconnectEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b874b1
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<154>, HostMigrationEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b874e2
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<154>, HostMigrationEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b8750d
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<103>, WorldMouseOverData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b87541
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<103>, WorldMouseOverData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b8756d
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<149>, DisconnectEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b8759e
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<149>, DisconnectEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b875c9
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<98>, PlayerVersionMismatchData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b8762d
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<98>, PlayerVersionMismatchData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b8767b
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<99>, Leaderboard_ScoresDownloadedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b876da
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<99>, Leaderboard_ScoresDownloadedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b87729
UNK_RET FromLua(lua_State* a1, InitCityRangeStrikeData& a2);
/// 0x08b8776f
UNK_RET ToLua(lua_State* a1, InitCityRangeStrikeData& a2);
/// 0x08b877ad
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<137>, SteamLobbyInviteData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b877ae
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<137>, SteamLobbyInviteData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b877af
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<138>, SteamServerInviteData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b877b0
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<138>, SteamServerInviteData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b877b1
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<136>, JoinRoomFailedEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b8781e
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<136>, JoinRoomFailedEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b87c7a
UNK_RET FromLua(lua_State* a1, ShowPlayerChangeUIData& a2);
/// 0x08b87cab
UNK_RET ToLua(lua_State* a1, ShowPlayerChangeUIData& a2);
/// 0x08b87cd6
UNK_RET FromLua(lua_State* a1, NewGameTurnData& a2);
/// 0x08b87d07
UNK_RET ToLua(lua_State* a1, NewGameTurnData& a2);
/// 0x08b87d32
UNK_RET FromLua(lua_State* a1, EventTemplate<Event_Int2Type<147>, GameAbandonedEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b87d63
UNK_RET ToLua(lua_State* a1, EventTemplate<Event_Int2Type<147>, GameAbandonedEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b87d8e
UNK_RET FromLua(lua_State* a1, EndTurnBlockingChangedData& a2);
/// 0x08b87dd4
UNK_RET ToLua(lua_State* a1, EndTurnBlockingChangedData& a2);
/// 0x08b87e1a
UNK_RET FromLua(lua_State* a1, AIProcessingPlayerData& a2);
/// 0x08b87e8c
UNK_RET ToLua(lua_State* a1, AIProcessingPlayerData& a2);
/// 0x08b87ece
UNK_RET FromLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<24>, CameraZoomData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b87f2f
UNK_RET ToLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<24>, CameraZoomData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b87f72
UNK_RET FromLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<25>, CameraZoomData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b87fd3
UNK_RET ToLua(lua_State* a1, GameCoreEventTemplate<Event_Int2Type<25>, CameraZoomData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a2);
/// 0x08b88016
UNK_RET FromLua(lua_State* a1, GameViewTypeChangedData& a2);
/// 0x08b8805c
UNK_RET ToLua(lua_State* a1, GameViewTypeChangedData& a2);
/// 0x08b8809a
UNK_RET FromLua(lua_State* a1, MultiplayerGameListUpdatedData& a2);
/// 0x08b88106
UNK_RET ToLua(lua_State* a1, MultiplayerGameListUpdatedData& a2);
}

namespace TerrainEditorOverlay {
/// 0x08b88308
UNK_RET Startup(cvTerrainSampler const* a1);
/// 0x08b88616
UNK_RET Shutdown();
/// 0x08b8869c
UNK_RET Render(FireGrafix::CCommandStream* a1, D3DXMATRIX* a2);
}
