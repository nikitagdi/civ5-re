/// origin guess
#include "Civ5/Src/App/LoadGameState.h"
#include <cstring>
#include <memory>
#include "Civ5/Src/App/Audio/cvAudioManager.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/DynamicHexGrid.h"
#include "Civ5/Src/App/Engine/cvGameplayRoadHandler.h"
#include "Civ5/Src/App/Engine/cvMarshSimSystem.h"
#include "Civ5/Src/App/Engine/cvRiverSimSystem.h"
#include "Civ5/Src/App/EventTemplate.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/Graphics/cvTerrainIceSystem.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/cvCityBuildingSim.h"
#include "Civ5/Src/App/cvDllUnitList.h"
#include "Civ5/Src/App/cvFloodplainsSystem.h"
#include "FirePlace/FireGrafix/FGXInt2.h"

/// 0x0865cdd8
UNK_RET LoadGameState::StartInitTerrainFromGameplayMap() {
    typedef GameCoreEventTemplate<Event_Int2Type<53>, MarshCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass> MarshCreatedEvent;
    typedef GameCoreEventTemplate<Event_Int2Type<68>, SpecialTileAddedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass> SpecialTileAddedEvent;
    typedef GameCoreEventTemplate<Event_Int2Type<52>, FeatureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass> FeatureChangedEvent;
    typedef GameCoreEventTemplate<Event_Int2Type<67>, RiverTileAddedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass> RiverTileAddedEvent;

    // A serial game-core event is a 4-byte header followed by its payload.
    struct MarshCreatedMessage { unsigned int m_uHeader; MarshCreatedData m_kData; };
    struct SpecialTileAddedMessage { unsigned int m_uHeader; SpecialTileAddedData m_kData; };
    struct FeatureChangedMessage { unsigned int m_uHeader; FeatureChangedData m_kData; };
    struct RiverTileAddedMessage { unsigned int m_uHeader; RiverTileAddedData m_kData; };

    // Terrain and feature ids in database order.
    const int kTerrainGrass = 0;
    const int kTerrainDesert = 2;
    const int kTerrainTundra = 3;
    const int kTerrainSnow = 4;
    const int kTerrainCoast = 5;
    const int kTerrainOcean = 6;
    const int kFeatureIce = 0;
    const int kFeatureMarsh = 2;
    const int kFeatureFloodPlains = 4;

    // HexPatternCell::m_uFlags: the terrain piece class of the hex.
    const unsigned int kCellGrass = 0x1;
    const unsigned int kCellPlains = 0x2;
    const unsigned int kCellDesert = 0x4;
    const unsigned int kCellTundra = 0x8;
    const unsigned int kCellSnow = 0x10;
    const unsigned int kCellGrassHills = 0x20;
    const unsigned int kCellPlainsHills = 0x40;
    const unsigned int kCellDesertHills = 0x80;
    const unsigned int kCellTundraHills = 0x100;
    const unsigned int kCellSnowHills = 0x200;
    const unsigned int kCellMountain = 0x400;
    const unsigned int kCellCoast = 0x2000;
    const unsigned int kCellOcean = 0x4000;
    const unsigned int kCellFuji = 0x40000;
    const unsigned char kTileWater = 0;
    const unsigned char kTileMountain = 2;

    const SquareGridPolicy& kGameGrid = *reinterpret_cast<const SquareGridPolicy*>(WorldGlobals::g_GameGrid);
    const SquareGridPolicy& kPaddedGrid = *reinterpret_cast<const SquareGridPolicy*>(WorldGlobals::g_PaddedGameGrid);

    // Gameplay plot (x, y) to world hex; the third input component is what the binary passes.
    auto PlotToHex = [&kGameGrid](int iX, int iY) -> HEXVec_i {
        const HEXVec_i kOffset = {iX - (iY >> 1), iY, (iY >> 1) - iX};
        HEXVec_i kHex = Fun_086616f0(kGameGrid.m_mxInverse, kOffset);
        kHex.z = -(kHex.x + kHex.y);
        return kHex;
    };

    // Natural wonders get their own piece class; unlisted ones use the Fuji piece.
    auto NaturalWonderFlags = [kCellFuji](const char* szType, unsigned int uTerrainFlags) -> unsigned int {
        static const struct {
            const char* m_szType;
            unsigned int m_uFlags;
        } kWonders[] = {
            {"FEATURE_GEYSER", 0x8000},          {"FEATURE_CRATER", 0x10000},       {"FEATURE_GIBRALTAR", 0x4000000},
            {"FEATURE_FUJI", 0x40000},           {"FEATURE_MESA", 0x80000},         {"FEATURE_REEF", 0x100000},
            {"FEATURE_VOLCANO", 0x200000},       {"FEATURE_POTOSI", 0x20000},       {"FEATURE_ULURU", 0x400000},
            {"FEATURE_MT_SINAI", 0x800000},      {"FEATURE_MT_KAILASH", 0x1000000}, {"FEATURE_SRI_PADA", 0x2000000},
            {"FEATURE_KILIMANJARO", 0x800},      {"FEATURE_LAKE_VICTORIA", 0x1000}, {"FEATURE_SOLOMONS_MINES", 0x4},
        };
        for (unsigned int i = 0; i < sizeof(kWonders) / sizeof(kWonders[0]); ++i) {
            if (strcmp(szType, kWonders[i].m_szType) == 0) {
                return kWonders[i].m_uFlags;
            }
        }
        if (strcmp(szType, "FEATURE_FOUNTAIN_YOUTH") == 0 || strcmp(szType, "FEATURE_EL_DORADO") == 0) {
            return uTerrainFlags;
        }
        return kCellFuji;
    };

    cvCityBuildingSim& kCityBuildingSim = *reinterpret_cast<cvCityBuildingSim*>(g_cvCityBuildingSim);

    // The hex grid TerrainSystem::Init builds the terrain from, sized like the padded game grid.
    const FGXInt2 kPaddedSize = {kPaddedGrid.m_iWidth, kPaddedGrid.m_iHeight};
    const HEXTransform_i kPaddedTransform = kPaddedGrid.m_kTransform;
    const HexPatternCell kEmptyCell = {};
    DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> kGrid;
    Fun_0865f122(&kGrid, kPaddedSize, kEmptyCell, kPaddedTransform);

    WorldGlobalsData kWorldGlobals;
    memcpy(kWorldGlobals.m_afTexToHex, WorldGlobals::g_TexToHex, sizeof(kWorldGlobals.m_afTexToHex));
    memcpy(kWorldGlobals.m_afHexToTex, WorldGlobals::g_HexToTex, sizeof(kWorldGlobals.m_afHexToTex));
    kWorldGlobals.m_vWorldMin = WorldGlobals::g_WorldMin;
    kWorldGlobals.m_vWorldMax = WorldGlobals::g_WorldMax;
    kWorldGlobals.m_vWorldSize = WorldGlobals::g_WorldSize;
    kWorldGlobals.m_vTerrainScale = WorldGlobals::g_TerrainScale;
    kWorldGlobals.m_kGameGrid = kGameGrid;
    g_GameplayRoadHandler.m_kWorldGlobals = kWorldGlobals;
    g_GameplayRoadHandler.StartupGame();

    const FGXInt2 kGameGridSize = {kGameGrid.m_iWidth, kGameGrid.m_iHeight};
    const HEXTransform_i kGameGridTransform = kGameGrid.m_kTransform;
    reinterpret_cast<cvTerrainIceSystem*>(g_TerrainSystem.unk_529bc)->ConfigureGrid(kGameGridSize, kGameGridTransform);

    // The padding around the map is coast.
    for (int iX = 0; iX < kPaddedGrid.m_iWidth; ++iX) {
        for (int iY = 0; iY < kPaddedGrid.m_iHeight; ++iY) {
            HexPatternCell& kCell = kGrid.m_pCells[iX * kGrid.m_iHeight + iY];
            kCell.m_uFlags = kCellCoast;
            kCell.m_ucContinent = 0xff;
            kCell.m_bCovered = false;
        }
    }

    std::auto_ptr<ICvMap1> pMap = GameCore::GetMap();
    const int iMapWidth = pMap->GetGridWidth();
    const int iMapHeight = pMap->GetGridHeight();

    // Pass 1: piece class and continent of every hex.
    cvMarshSimSystem* pMarshSim = reinterpret_cast<cvMarshSimSystem*>(g_TerrainSystem.unk_276d8);
    for (int iX = 0; iX < iMapWidth; ++iX) {
        for (int iY = 0; iY < iMapHeight; ++iY) {
            std::auto_ptr<ICvPlot1> pPlot(pMap->GetPlot(iX, iY));
            const HEXVec_i kHex = PlotToHex(iX, iY);
            const HEXVec_i kPadded = Fun_086616f0(kPaddedGrid.m_mxForward, kHex);

            SpecialTileAddedMessage kSpecialTile = {};
            kSpecialTile.m_kData.m_sHexX = static_cast<short>(kHex.x);
            kSpecialTile.m_kData.m_sHexY = static_cast<short>(kHex.y);
            unsigned int uFlags = kCellOcean;
            unsigned char ucContinent = 0;

            if (pPlot.get() != NULL) {
                if (pPlot->GetFeatureType() == kFeatureMarsh) {
                    MarshCreatedMessage kMarsh = {};
                    kMarsh.m_kData.m_kHex = kHex;
                    kMarsh.m_kData.m_eTerrain = pPlot->GetTerrainType();
                    kMarsh.m_kData.m_iContinent = pPlot->GetContinentType();
                    pMarshSim->OnMarshAdded(reinterpret_cast<MarshCreatedEvent&>(kMarsh));
                }

                const char cContinent = pPlot->GetContinentType();
                const int eTerrain = pPlot->GetTerrainType();
                if (eTerrain == kTerrainOcean || eTerrain == kTerrainCoast) {
                    kSpecialTile.m_kData.m_ucTileType = kTileWater;
                    kCityBuildingSim.OnSpecialTileAdded(reinterpret_cast<SpecialTileAddedEvent&>(kSpecialTile));
                    // The receiver is the special-case sim inside g_cvLandmarkSimSystem (+0x208cc); the header has it as a namespace.
                    cvSpecialCaseSimSystem::OnSpecialTileAdded(reinterpret_cast<SpecialTileAddedEvent&>(kSpecialTile));
                    uFlags = (eTerrain == kTerrainOcean) ? kCellOcean : kCellCoast;
                    ucContinent = 0;
                } else if (pPlot->IsMountain()) {
                    kSpecialTile.m_kData.m_ucTileType = kTileMountain;
                    kCityBuildingSim.OnSpecialTileAdded(reinterpret_cast<SpecialTileAddedEvent&>(kSpecialTile));
                    cvSpecialCaseSimSystem::OnSpecialTileAdded(reinterpret_cast<SpecialTileAddedEvent&>(kSpecialTile));
                    uFlags = kCellMountain;
                    ucContinent = cContinent;
                } else if (pPlot->IsHills()) {
                    switch (eTerrain) {
                    case kTerrainGrass: uFlags = kCellGrassHills; break;
                    case kTerrainDesert: uFlags = kCellDesertHills; break;
                    case kTerrainSnow: uFlags = kCellSnowHills; break;
                    case kTerrainTundra: uFlags = kCellTundraHills; break;
                    default: uFlags = kCellPlainsHills; break;
                    }
                    ucContinent = cContinent;
                } else {
                    switch (eTerrain) {
                    case kTerrainGrass: uFlags = kCellGrass; break;
                    case kTerrainDesert: uFlags = kCellDesert; break;
                    case kTerrainSnow: uFlags = kCellSnow; break;
                    case kTerrainTundra: uFlags = kCellTundra; break;
                    default: uFlags = kCellPlains; break;
                    }
                    ucContinent = cContinent;
                }

                const FeatureTypes eFeature = pPlot->GetFeatureType();
                if (eFeature != NO_FEATURE) {
                    std::auto_ptr<ICvFeatureInfo1> pFeatureInfo(GameCore::GetContext()->GetFeatureInfo(eFeature));
                    if (pFeatureInfo.get() != NULL && pFeatureInfo->IsNaturalWonder()) {
                        uFlags = NaturalWonderFlags(pFeatureInfo->GetType(), uFlags);
                        // A wonder keeps the plot's continent even on water.
                        ucContinent = cContinent;
                    }
                }
            }

            HexPatternCell& kCell = kGrid.m_pCells[((kPadded.y >> 1) + kPadded.x) * kGrid.m_iHeight + kPadded.y];
            kCell.m_uFlags = uFlags;
            kCell.m_ucContinent = ucContinent;
            kCell.m_bCovered = false;
        }
    }

    // Pass 2: ice, flood plains and river sides.
    cvTerrainIceSystem* pIceSystem = reinterpret_cast<cvTerrainIceSystem*>(g_TerrainSystem.unk_529bc);
    cvFloodplainsSystem* pFloodplains = reinterpret_cast<cvFloodplainsSystem*>(g_TerrainSystem.unk_27818);
    cvRiverSimSystem* pRiverSim = reinterpret_cast<cvRiverSimSystem*>(g_TerrainSystem.unk_50c);
    // Each road cell keeps three river-side bits in m_aucBits (bits 11..13).
    RoadGrid* pRoadGrid = &g_GameplayRoadHandler.m_RoadGrid;
    for (int iX = 0; iX < iMapWidth; ++iX) {
        for (int iY = 0; iY < iMapHeight; ++iY) {
            std::auto_ptr<ICvPlot1> pPlot(pMap->GetPlot(iX, iY));
            const HEXVec_i kHex = PlotToHex(iX, iY);
            if (pPlot.get() == NULL) {
                continue;
            }

            if (pPlot->GetFeatureType() == kFeatureIce) {
                pIceSystem->MarkIceHex(kHex);
            }
            if (pPlot->GetFeatureType() == kFeatureFloodPlains) {
                FeatureChangedMessage kFloodplain = {};
                kFloodplain.m_kData.field_0 = 2;
                kFloodplain.m_kData.field_4 = -1;
                kFloodplain.m_kData.m_kHex = kHex;
                pFloodplains->OnFloodplainAdded(reinterpret_cast<FeatureChangedEvent&>(kFloodplain));
            }
            if (!pPlot->IsRiver()) {
                continue;
            }

            auto AddRiverSide = [&](int iEdge) {
                HexEdge kSide;
                kSide.m_kHex = kHex;
                kSide.m_iEdge = iEdge;
                HexEdge kOwned = {};
                Fun_0865f1be(kSide, kOwned);
                Fun_0865f23e(kOwned.m_kHex);

                RiverTileAddedMessage kRiver = {};
                kRiver.m_kData.m_kHex = kOwned.m_kHex;
                kRiver.m_kData.m_ucEdge = static_cast<unsigned char>(kOwned.m_iEdge);
                pRiverSim->OnAddRiverTile(reinterpret_cast<RiverTileAddedEvent&>(kRiver));

                RoadCell* pRoadCell = Fun_08661606(pRoadGrid, kRiver.m_kData.m_kHex);
                unsigned short usBits;
                memcpy(&usBits, pRoadCell->m_aucBits, sizeof(usBits));
                const unsigned int uSides = ((usBits >> 11) | (1u << (kRiver.m_kData.m_ucEdge % 3))) & 0x7;
                usBits = static_cast<unsigned short>((usBits & 0xc7ff) | (uSides << 11));
                memcpy(pRoadCell->m_aucBits, &usBits, sizeof(usBits));

                kCityBuildingSim.OnRiverTileAdded(reinterpret_cast<RiverTileAddedEvent&>(kRiver));
            };

            // Side ids: the east side is 10 or 4, the south-east 8 or 2, the south-west 9 or 3, by flow direction.
            const bool bWOfRiver = pPlot->IsWOfRiver();
            const bool bEastFlowsNorth = bWOfRiver && pPlot->GetRiverEFlowDirection() == FLOWDIRECTION_NORTH;
            const bool bNWOfRiver = pPlot->IsNWOfRiver();
            const bool bSouthEastFlowsNorthEast = bNWOfRiver && pPlot->GetRiverSEFlowDirection() == FLOWDIRECTION_NORTHEAST;
            const bool bNEOfRiver = pPlot->IsNEOfRiver();
            const bool bSouthWestFlowsNorthWest = bNEOfRiver && pPlot->GetRiverSWFlowDirection() == FLOWDIRECTION_NORTHWEST;
            if (bEastFlowsNorth) {
                AddRiverSide(10);
            }
            if (bWOfRiver && !bEastFlowsNorth) {
                AddRiverSide(4);
            }
            if (bSouthEastFlowsNorthEast) {
                AddRiverSide(8);
            }
            if (bNWOfRiver && !bSouthEastFlowsNorthEast) {
                AddRiverSide(2);
            }
            if (bNEOfRiver && !bSouthWestFlowsNorthWest) {
                AddRiverSide(9);
            }
            if (bSouthWestFlowsNorthWest) {
                AddRiverSide(3);
            }
        }
    }

    // ICvPreGame1::mapRandomSeed: the binary calls slot 0x11c, one past the SDK order.
    g_TerrainSystem.Init(kGrid, true, GameCore::GetPreGame()->mapRandomSeed());
}
