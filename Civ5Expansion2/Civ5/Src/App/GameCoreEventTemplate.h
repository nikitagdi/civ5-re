/// origin guess
#pragma once
#include <Windows.h>
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/DynamicHexGrid.h"

namespace FireGrafix {
class TextureData;
}
class AILeaderMessageData;
class AbortCombatSimData;
class ActivePlayerData;
class AdvisorDisplayShowData;
class ArrowVisSystem;
class CameraCenterData;
class cvGameplayRoadHandler;
class ChangeGameStateData;
class CityCaptureData;
class CityCreatedData;
class CityCultureChangedData;
class CityGenericData;
class CityPopulationChangedData;
class CitySetDamageData;
class EndGameShowData;
class EraChangedData;
class FGXVector3;
class FeatureChangedData;
class ForestChangedData;
class GameMessagePopupData;
class GameMessagePopupProcessedData;
class GameplayFXData;
class GreatWallData;
class HexCultureChangedData;
class HexFOWStateChangedData;
class HexID;
class LandmarkCreatedData;
class LandmarkHandleData;
class LandmarkIconCreatedData;
class LeaderArtDefineData;
class LinearEventDispatcher;
class MarshCreatedData;
class MarshRemovedData;
class NotificationAddedData;
class NullEventClass;
class RandomSeedSetData;
class RiverTileAddedData;
class RoadCreatedData;
class RunCombatSimData;
class ScreenShotData;
class SerialContainer;
class ShowPlayerChangeUIData;
class TeamMetData;
class TechAcquiredData;
class TerrainOverlayModData;
class TerrainStencil;
class ToggleBridgesData;
class UnitActionData;
class UnitBooleanData;
class UnitCreatedData;
class UnitDataEditedData;
class UnitDataRequestedData;
class UnitDebugFSMData;
class UnitFacingChangedData;
class UnitHandleData;
class UnitMoveToHexesData;
class UnitSelectionChangedData;
class UnitSetDamageData;
class UnitTeleportedToHexData;
class UnitVisibilityChangedData;
class WonderData;
class WonderRemovedData;
class cvDistanceFog;
class cvEventContext;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

// Payload of event 53 (marsh on a hex).
/// size 0x14
class MarshCreatedData {
public:
    HEXVec_i m_kHex;    ///< +0x0
    int m_eTerrain;     ///< +0xc
    int m_iContinent;   ///< +0x10
};

// Payload of event 54 (marsh gone from a hex); cvMarshSimSystem::OnMarshRemoved reads only the hex.
/// size ?
class MarshRemovedData {
public:
    HEXVec_i m_kHex;    ///< +0x0
};

// Payload of events 34 and 35 (city created, ruins created); cvCityBuildingSim::CreateNewCity takes it by reference.
/// size 0x2c
class CityCreatedData {
public:
    int m_iPlayerID;                ///< +0x0
    int m_iCityID;                  ///< +0x4
    int m_iCultureType;             ///< +0x8
    int m_iContinent;               ///< +0xc
    int m_iEraType;                 ///< +0x10
    unsigned char unk_14[0x4];      ///< +0x14
    int m_iPopulation;              ///< +0x18
    int m_iSize;                    ///< +0x1c
    short m_sHexX;                  ///< +0x20
    short m_sHexY;                  ///< +0x22
    unsigned char m_ucFOWState;     ///< +0x24
    unsigned char unk_25[0x7];      ///< +0x25
};

// Payload of event 49.
/// size 0xc
class CityCultureChangedData {
public:
    short m_sHexX;          ///< +0x0
    short m_sHexY;          ///< +0x2
    int m_iCultureType;     ///< +0x4
    unsigned char unk_8[0x4]; ///< +0x8
};

// Payload of event 41 (city destroyed).
/// size 0xc
class CityGenericData {
public:
    short m_sHexX;          ///< +0x0
    short m_sHexY;          ///< +0x2
    int m_iPlayerID;        ///< +0x4
    int m_iCityID;          ///< +0x8
};

// Payload of events 40 and 42 (city destroyed, city captured).
/// size 0x1c
class CityCaptureData {
public:
    short m_sHexX;              ///< +0x0
    short m_sHexY;              ///< +0x2
    int m_iPlayerID;            ///< +0x4
    int m_iCityID;              ///< +0x8
    int field_c;                ///< +0xc
    unsigned char unk_10[0xc];  ///< +0x10
};

// Payload of event 43.
/// size 0xc
class CityPopulationChangedData {
public:
    short m_sHexX;              ///< +0x0
    short m_sHexY;              ///< +0x2
    unsigned int m_uPopulation; ///< +0x4
    int m_iSize;                ///< +0x8
};

// Payload of event 44.
/// size 0x1c
class CitySetDamageData {
public:
    int m_iPlayerID;            ///< +0x0
    int m_iCityID;              ///< +0x4
    int m_iDamage;              ///< +0x8
    int m_iMaxDamage;           ///< +0xc
    int m_iFOWState;            ///< +0x10
    short m_sHexX;              ///< +0x14
    short m_sHexY;              ///< +0x16
    unsigned char unk_18[0x4];  ///< +0x18
};

// Payload of event 68: a water (0) or mountain (2) hex.
/// size 0x6
class SpecialTileAddedData {
public:
    short m_sHexX;              ///< +0x0
    short m_sHexY;              ///< +0x2
    unsigned char m_ucTileType; ///< +0x4
};

// Payload of event 52.
/// size 0x14
class FeatureChangedData {
public:
    unsigned char field_0;  ///< +0x0
    int field_4;            ///< +0x4
    HEXVec_i m_kHex;        ///< +0x8
};

// Payload of event 67: one river side, already named from its owning hex (see Fun_0865f1be).
/// size 0x10
class RiverTileAddedData {
public:
    HEXVec_i m_kHex;        ///< +0x0
    unsigned char m_ucEdge; ///< +0xc
};

// Payload of event 36; also the copy cvGameplayRoadHandler keeps at +0x14.
/// size 0x14c
class WorldGlobalsData {
public:
    /// inline
    WorldGlobalsData() : m_vWorldMin(), m_vWorldMax(), m_vWorldSize(), m_vTerrainScale() {
        Fun_08661752(&m_kGameGrid, FGXInt2(), HEXTransform_i::IDENTITY_TRANSFORM);
    }

    float m_afTexToHex[16];         ///< +0x0
    float m_afHexToTex[16];         ///< +0x40
    FGXVector3 m_vWorldMin;         ///< +0x80
    FGXVector3 m_vWorldMax;         ///< +0x8c
    FGXVector3 m_vWorldSize;        ///< +0x98
    FGXVector3 m_vTerrainScale;     ///< +0xa4
    SquareGridPolicy m_kGameGrid;   ///< +0xb0
};

/// size ?
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6>
class GameCoreEventTemplate {
public:
    static UNK_DATA sm_aContexts[0x8];
};
/// 0x0bff91f8 GameCoreEventTemplate<Event_Int2Type<113>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3d211e GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3d2120 GameCoreEventTemplate<Event_Int2Type<116>, HexID, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03cbe6 GameCoreEventTemplate<Event_Int2Type<117>, LeaderArtDefineData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03cbe8 GameCoreEventTemplate<Event_Int2Type<118>, AILeaderMessageData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccfcd0 GameCoreEventTemplate<Event_Int2Type<119>, TeamMetData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03ddfe GameCoreEventTemplate<Event_Int2Type<11>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c088292 GameCoreEventTemplate<Event_Int2Type<121>, NotificationAddedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff9030 GameCoreEventTemplate<Event_Int2Type<124>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff9032 GameCoreEventTemplate<Event_Int2Type<125>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a6185ec GameCoreEventTemplate<Event_Int2Type<128>, TechAcquiredData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03ddfc GameCoreEventTemplate<Event_Int2Type<12>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03ddec GameCoreEventTemplate<Event_Int2Type<130>, FGXVector3, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e50 GameCoreEventTemplate<Event_Int2Type<131>, ActivePlayerData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb86e GameCoreEventTemplate<Event_Int2Type<132>, UnitHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb86c GameCoreEventTemplate<Event_Int2Type<133>, UnitHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c088290 GameCoreEventTemplate<Event_Int2Type<134>, AdvisorDisplayShowData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c08828e GameCoreEventTemplate<Event_Int2Type<135>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccfcd6 GameCoreEventTemplate<Event_Int2Type<136>, EndGameShowData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e2c GameCoreEventTemplate<Event_Int2Type<138>, RandomSeedSetData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4cc164 GameCoreEventTemplate<Event_Int2Type<141>, GreatWallData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4cc150 GameCoreEventTemplate<Event_Int2Type<142>, HexID, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f365a GameCoreEventTemplate<Event_Int2Type<143>, GameplayFXData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb868 GameCoreEventTemplate<Event_Int2Type<148>, UnitBooleanData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03ddfa GameCoreEventTemplate<Event_Int2Type<14>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb860 GameCoreEventTemplate<Event_Int2Type<151>, ChangeGameStateData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c08828c GameCoreEventTemplate<Event_Int2Type<153>, GameMessagePopupData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb866 GameCoreEventTemplate<Event_Int2Type<154>, GameMessagePopupProcessedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb85e GameCoreEventTemplate<Event_Int2Type<156>, ShowPlayerChangeUIData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3cb85c GameCoreEventTemplate<Event_Int2Type<157>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03ddf8 GameCoreEventTemplate<Event_Int2Type<15>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03ddf6 GameCoreEventTemplate<Event_Int2Type<16>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03ddf4 GameCoreEventTemplate<Event_Int2Type<17>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03ddf2 GameCoreEventTemplate<Event_Int2Type<18>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03ddf0 GameCoreEventTemplate<Event_Int2Type<19>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03ddee GameCoreEventTemplate<Event_Int2Type<26>, CameraCenterData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3d35e6 GameCoreEventTemplate<Event_Int2Type<27>, ScreenShotData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a341408 GameCoreEventTemplate<Event_Int2Type<28>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3d2122 GameCoreEventTemplate<Event_Int2Type<29>, HexCultureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c019b86 GameCoreEventTemplate<Event_Int2Type<2>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c08c480 GameCoreEventTemplate<Event_Int2Type<30>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f3f70 GameCoreEventTemplate<Event_Int2Type<31>, TerrainOverlayModData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7ed908 GameCoreEventTemplate<Event_Int2Type<34>, CityCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4cc160 GameCoreEventTemplate<Event_Int2Type<35>, CityCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4cc156 GameCoreEventTemplate<Event_Int2Type<37>, WonderData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4cc154 GameCoreEventTemplate<Event_Int2Type<38>, WonderRemovedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4cc152 GameCoreEventTemplate<Event_Int2Type<39>, WonderData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0bff91fa GameCoreEventTemplate<Event_Int2Type<3>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03da9e GameCoreEventTemplate<Event_Int2Type<40>, CityCaptureData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4cc15e GameCoreEventTemplate<Event_Int2Type<41>, CityGenericData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03da9c GameCoreEventTemplate<Event_Int2Type<42>, CityCaptureData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4cc15a GameCoreEventTemplate<Event_Int2Type<43>, CityPopulationChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4cc15c GameCoreEventTemplate<Event_Int2Type<44>, CitySetDamageData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7ed90c GameCoreEventTemplate<Event_Int2Type<45>, RoadCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7ed904 GameCoreEventTemplate<Event_Int2Type<46>, HexID, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7ed90a GameCoreEventTemplate<Event_Int2Type<47>, ToggleBridgesData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3e2918 GameCoreEventTemplate<Event_Int2Type<48>, EraChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4cc162 GameCoreEventTemplate<Event_Int2Type<49>, CityCultureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03de0a GameCoreEventTemplate<Event_Int2Type<4>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a6185f0 GameCoreEventTemplate<Event_Int2Type<50>, LandmarkCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a6185ee GameCoreEventTemplate<Event_Int2Type<51>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a340ac8 GameCoreEventTemplate<Event_Int2Type<52>, FeatureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f2dbe GameCoreEventTemplate<Event_Int2Type<53>, MarshCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f2dbc GameCoreEventTemplate<Event_Int2Type<54>, MarshRemovedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a6185f4 GameCoreEventTemplate<Event_Int2Type<55>, LandmarkCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a6185f2 GameCoreEventTemplate<Event_Int2Type<56>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccd926 GameCoreEventTemplate<Event_Int2Type<57>, LandmarkIconCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccd924 GameCoreEventTemplate<Event_Int2Type<58>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3e291c GameCoreEventTemplate<Event_Int2Type<59>, LandmarkCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03de08 GameCoreEventTemplate<Event_Int2Type<5>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a3e291a GameCoreEventTemplate<Event_Int2Type<60>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccd922 GameCoreEventTemplate<Event_Int2Type<61>, LandmarkIconCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccd920 GameCoreEventTemplate<Event_Int2Type<62>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccd92a GameCoreEventTemplate<Event_Int2Type<63>, ForestChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4ccf74 GameCoreEventTemplate<Event_Int2Type<64>, ForestChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccd928 GameCoreEventTemplate<Event_Int2Type<65>, ForestChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a4ccf72 GameCoreEventTemplate<Event_Int2Type<66>, ForestChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7ed90e GameCoreEventTemplate<Event_Int2Type<67>, RiverTileAddedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f3640 GameCoreEventTemplate<Event_Int2Type<69>, UnitCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, cvEventContext>
/// 0x0c03de06 GameCoreEventTemplate<Event_Int2Type<6>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccd91e GameCoreEventTemplate<Event_Int2Type<70>, UnitHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f365c GameCoreEventTemplate<Event_Int2Type<71>, UnitHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e44 GameCoreEventTemplate<Event_Int2Type<72>, UnitSetDamageData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e30 GameCoreEventTemplate<Event_Int2Type<73>, UnitActionData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccd91a GameCoreEventTemplate<Event_Int2Type<74>, UnitBooleanData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccd91c GameCoreEventTemplate<Event_Int2Type<75>, UnitBooleanData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e2e GameCoreEventTemplate<Event_Int2Type<76>, UnitActionData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e4e GameCoreEventTemplate<Event_Int2Type<77>, UnitMoveToHexesData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e4c GameCoreEventTemplate<Event_Int2Type<79>, UnitTeleportedToHexData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03de04 GameCoreEventTemplate<Event_Int2Type<7>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e4a GameCoreEventTemplate<Event_Int2Type<80>, UnitFacingChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e46 GameCoreEventTemplate<Event_Int2Type<81>, UnitVisibilityChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccd918 GameCoreEventTemplate<Event_Int2Type<83>, UnitBooleanData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0cccd916 GameCoreEventTemplate<Event_Int2Type<84>, UnitBooleanData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c075254 GameCoreEventTemplate<Event_Int2Type<86>, UnitDataEditedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c075252 GameCoreEventTemplate<Event_Int2Type<87>, UnitDataRequestedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c075250 GameCoreEventTemplate<Event_Int2Type<88>, UnitDebugFSMData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e26 GameCoreEventTemplate<Event_Int2Type<89>, RunCombatSimData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03de02 GameCoreEventTemplate<Event_Int2Type<8>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e24 GameCoreEventTemplate<Event_Int2Type<91>, AbortCombatSimData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0a7f4e48 GameCoreEventTemplate<Event_Int2Type<92>, UnitSelectionChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
/// 0x0c03de00 GameCoreEventTemplate<Event_Int2Type<9>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6>
UNK_DATA GameCoreEventTemplate<T0, T1, T2, T3, T4, T5, T6>::sm_aContexts[0x8];

extern "C" {
    /// 0x0c03de0c
    extern cvDistanceFog g_DistanceFog;
    /// 0x0a7f4e80
    extern UNK_DATA g_EventSystemLocalMachine[0x1801080];
    /// 0x0bff6100
    extern UNK_DATA g_EventSystemNonSerial[0x780];
    /// 0x0bff6a80
    extern UNK_DATA g_EventSystemSequencer[0x680];
    /// 0x0bff7300
    extern UNK_DATA g_EventSystemSerial[0xd80];
    /// 0x0a340acc
    extern UNK_DATA g_ForestVisSystem[0x1c8];
    /// 0x0a4ccf78
    extern UNK_DATA g_GameSplineDatabase[0xea6b0];
    /// 0x0a61885c
    extern cvGameplayRoadHandler g_GameplayRoadHandler;
    /// 0x0a7ed910
    extern UNK_DATA g_LayoutState[0x4090];
    /// 0x0a7f3f74
    extern TerrainStencil g_TerrainStencil;
    /// 0x0bff988c
    extern ArrowVisSystem g_cvArrowVisSystem;
    /// 0x0a7f2fbc
    extern UNK_DATA g_cvCityBuildingTypeMap[0x1c];
    /// 0x0a4cc630
    extern UNK_DATA g_cvCityCombatSim[0x1];
    /// 0x0a4cc898
    extern UNK_DATA g_cvCombatSimSystem[0x64];
    /// 0x0a7f2dc0
    extern UNK_DATA g_cvEffectVariantSystem[0x1];
    /// 0x0a5b77cc
    extern UNK_DATA g_cvLandmarkSimSystem[0x60c7c];
    /// 0x0a7f2f68
    extern UNK_DATA g_cvLandmarkTypeMap[0x1c];
    /// 0x0c03e4c0
    extern UNK_DATA g_cvMovementPathVisSystem[0x5e10];
    /// 0x0a7f3660
    extern UNK_DATA g_cvProjectileSimSystem[0x3c];
    /// 0x0a7f1b44
    extern UNK_DATA g_cvRoadLayoutSimSystem[0xd8c];
    /// 0x0a7f2fa0
    extern UNK_DATA g_cvUnitMemberTypeMap[0x1c];
    /// 0x0a7f31b4
    extern UNK_DATA g_cvUnitMovementIndicatorSimSystem[0x8];
    /// 0x0a7f2ff4
    extern UNK_DATA g_cvUnitStatesMap[0x1c];
    /// 0x0a7f2f84
    extern UNK_DATA g_cvUnitTypeMap[0x1c];
    /// 0x0a7f2fd8
    extern UNK_DATA g_cvWonderTypeMap[0x1c];
}

namespace FrameGlobals {
/// 0x0a351728
extern float fCurrZoom;
/// 0x0a35172c
extern float fCurrZoomNorm;
/// 0x0a351720
extern float fMaxZoom;
/// 0x0a351724
extern float fMinZoom;
/// 0x0a351730
extern float fShadowFade;
/// 0x0a3520a8
extern FGXVector4 g_FOWBaseScale;
/// 0x0a352048
extern UNK_DATA g_GameGridBottomRightVisible[0x20];
/// 0x0a352088
extern UNK_DATA g_GameGridBottomRightVisibleUnclamped[0x20];
/// 0x0a352028
extern UNK_DATA g_GameGridTopLeftVisible[0x20];
/// 0x0a352068
extern UNK_DATA g_GameGridTopLeftVisibleUnclamped[0x20];
/// 0x0a351878
extern D3DXMATRIX g_InvView[4];
/// 0x0a351af8
extern D3DXMATRIX g_OverlayProjection[4];
/// 0x0a351738
extern D3DXMATRIX g_Proj;
/// 0x0a351ef8
extern D3DXMATRIX g_ReflectionProj;
/// 0x0a351df8
extern D3DXMATRIX g_ReflectionView[4];
/// 0x0a351cf8
extern D3DXMATRIX g_ReflectionViewProj[4];
/// 0x0a351f38
extern UNK_DATA g_RelativeCameraPosition[0x30];
/// 0x0a351bf8
extern D3DXMATRIX g_ShadowViewProj[4];
/// 0x0a351778
extern D3DXMATRIX g_View[4];
/// 0x0a351978
extern FGXVector4 g_ViewClipPlanes[4][6];
/// 0x0a351f98
extern UNK_DATA g_WorldBottomRightVisible[0x30];
/// 0x0a351ff8
extern FGXVector3 g_WorldBottomRightVisibleUnclamped[4];
/// 0x0a351f68
extern FGXVector3 g_WorldTopLeftVisible[4];
/// 0x0a351fc8
extern FGXVector3 g_WorldTopLeftVisibleUnclamped[4];
/// 0x0a351734
extern unsigned int g_nCameras;
/// 0x0a3520b8
extern FGXVector4 g_v4FogColor;
/// 0x0a3520c8
extern FGXVector4 g_v4FogControl;
}

namespace GraphicsSettings { namespace GeneralSettings {
/// 0x0a3520fd
extern UNK_DATA g_bConstantRebase[0x1];
/// 0x0a3520fe
extern UNK_DATA g_bFP16AAMode[0x1];
/// 0x0a352105
extern UNK_DATA g_bFadeShadows[0x1];
/// 0x0a3520fc
extern UNK_DATA g_bFullscreen[0x1];
/// 0x0a3520d8
extern UNK_DATA g_bHDStrategicView[0x1];
/// 0x0a352104
extern UNK_DATA g_bVSync[0x1];
/// 0x0a3520e8
extern unsigned int g_uFOWLevel;
/// 0x0a3520dc
extern unsigned int g_uOverlayLevel;
/// 0x0a3520e4
extern unsigned int g_uReflectionLevel;
/// 0x0a3520f0
extern unsigned int g_uResolutionID;
/// 0x0a35210c
extern unsigned int g_uScreenShotHeight;
/// 0x0a352108
extern unsigned int g_uScreenShotWidth;
/// 0x0a3520e0
extern unsigned int g_uShadowLevel;
/// 0x0a3520ec
extern unsigned int g_uTextureQuality;
/// 0x0a352100
extern unsigned int g_uiAA;
/// 0x0a3520f4
extern UNK_DATA g_vWindowResolution[0x8];
}}

namespace GraphicsSettings { namespace LeaderSettings {
/// 0x0a352174
extern UNK_DATA Settings[0x34];
}}

namespace GraphicsSettings { namespace TerrainSettings {
/// 0x0a35212c
extern int g_bAutoUpdateCells;
/// 0x0a352130
extern int g_bBlockOnLoad;
/// 0x0a352138
extern int g_bEnableDXTCompression;
/// 0x0a352164
extern int g_bMinimizeGrayTiles;
/// 0x0a35215c
extern int g_bSpecEnable;
/// 0x0a352134
extern unsigned int g_bUse40Features;
/// 0x0a352160
extern int g_bUseGPUWeightBlending;
/// 0x0a352168
extern int g_bUseTessellation;
/// 0x0a352154
extern unsigned int g_iCacheSize;
/// 0x0a352158
extern unsigned int g_iMaxTileSize;
/// 0x0a352150
extern unsigned int g_iRootCacheSize;
/// 0x0a352170
extern unsigned int g_iTessellationPatchDivisions;
/// 0x0a35216c
extern unsigned int g_iTessellationPixelsPerTri;
/// 0x0a352140
extern unsigned int g_n128Textures;
/// 0x0a352144
extern unsigned int g_n256Textures;
/// 0x0a352148
extern unsigned int g_n512Textures;
/// 0x0a35213c
extern unsigned int g_n64Textures;
/// 0x0a35214c
extern unsigned int g_nBCTextures;
/// 0x0a352128
extern unsigned int g_uNumContinents;
/// 0x0a352118
extern unsigned int g_uShadowQuality;
/// 0x0a352110
extern unsigned int g_uTerrainDetailLevel;
/// 0x0a352114
extern unsigned int g_uTerrainTessLevel;
/// 0x0a352120
extern unsigned int g_uTilesToCalcPerFrame;
/// 0x0a352124
extern unsigned int g_uTilesToCalcPerFrameMoving;
/// 0x0a35211c
extern unsigned int g_uWaterQuality;
}}

namespace MapGlobals {
/// 0x0a3416bc
extern cvTerrainSampler g_TerrainSampler;
/// 0x0a3516f0
extern cvTerrainSampler* g_pTerrainSampler;
}

namespace RenderGlobals {
// Passed whole to Terrain::Init as ExternalTextureHandles; slot 1 is the overlay target, slot 9 the spec screen map.
/// 0x0a3516f4
extern unsigned int g_TexHandles[10];
/// 0x0a35171c
extern FireGrafix::TextureData* g_pScreenShot;
}

namespace WorldGlobals {
/// 0x0a341580
extern UNK_DATA g_GameGrid[0x9c];
/// 0x0a341500
extern UNK_DATA g_HexToTerrainPiece[0x40];
/// 0x0a341450
extern float g_HexToTex[16];
/// 0x0a34161c
extern UNK_DATA g_PaddedGameGrid[0x9c];
/// 0x0a3414c0
extern UNK_DATA g_TerrainPieceToHex[0x40];
/// 0x0a3414b4
extern FGXVector3 g_TerrainScale;
/// 0x0a341410
extern float g_TexToHex[16];
/// 0x0a34149c
extern FGXVector3 g_WorldMax;
/// 0x0a341490
extern FGXVector3 g_WorldMin;
/// 0x0a3414a8
extern FGXVector3 g_WorldSize;
/// 0x0a341560
extern UNK_DATA g_ahLargeCorners[0x20];
/// 0x0a341540
extern UNK_DATA g_ahSmallCorners[0x20];
/// 0x0a34140c
extern UNK_DATA g_bNamespaceValid[0x4];
/// 0x0a3416b8
extern int g_eWrapMode;
}

namespace cvSerialEventLogging {
/// 0x0bff808c
extern UNK_DATA ms_bLoggingEnabled[0x1];
/// 0x0bff808d
extern UNK_DATA ms_bProfilingEnabled[0x1];
}

namespace cvSerialEventRegistrar {
/// 0x0bff8080
extern UNK_DATA ms_kEventNames[0xc];
}
