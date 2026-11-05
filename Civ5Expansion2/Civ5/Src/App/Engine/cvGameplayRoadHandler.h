/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include "Civ5/Src/App/Engine/cvRoadNetworkManager.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/DynamicHexGrid.h"
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireGrafix/FGXVector2.h"

class HEXVec_i;
class HexID;
class LandmarkCreatedData;
class LandmarkHandleData;
class LinearEventDispatcher;
class NullEventClass;
class SerialContainer;
class TerrainTensorSurface;
class ToggleBridgesData;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

// One hex of the road grid. m_aucBits (read as a little-endian u16 at +0x2d):
// bits 0..2 segment count, bit 3 has-road, bits 6..7 city mode (0 open country, 1 ring plus spokes, 2 spokes only),
// bits 8..10 route state (AddRoad a3; the eRoadTypes of CvPlot::updateLayout, 4 and 5 are never stored; also byte +0x2e & 7),
// bits 11..13 river on side 0..2 (a bridge handle each).
// m_ucRoadType is AddRoad a2, 0xff when the hex has no road. m_ucSideMask bit i: neighbour HEXVec_i::TRANSLATIONS[i] has a road.
// m_aucSegments[k] indexes cvGameplayRoadHandler::ms_aaiSegmentSides: the pair of sides the k-th spline joins, one decal in m_auiDecals each.
/// size 0x30
class RoadCell {
public:
    unsigned int m_auiGeometryHandles[3];  ///< +0x0
    unsigned int m_auiDecals[5];           ///< +0xc
    unsigned char m_aucVariation[6];       ///< +0x20
    unsigned char m_aucSegments[5];        ///< +0x26
    unsigned char m_ucRoadType;            ///< +0x2b
    unsigned char m_ucSideMask;            ///< +0x2c
    unsigned char m_aucBits[2];            ///< +0x2d
    char unk_2f[0x1];                      ///< +0x2f
};

// The cell array is inline: the handler fields after the grid start 40000 cells later.
/// size 0x1d4ca0
class RoadGrid : public SquareGridPolicy {
public:
    unsigned char field_9c;       ///< +0x9c
    char unk_9d[0x3];             ///< +0x9d
    RoadCell m_aCells[40000];     ///< +0xa0
};

// Cell (iX, iY) of the grid in its column and row; the spare cell at +0x1d4ca0 when off the grid. Rows are 200 cells apart.
/// 0x0866167c
static RoadCell* RoadGridCellAt(RoadGrid* a1, int iX, int iY);

class cvGameplayRoadHandler;

// Appends the 4 control points of the line from side iSideA to side iSideB of kHex to pHandler->m_SplinePoints; always returns 1.
/// 0x087df2f8
static unsigned int BuildSideToSideSpline(cvGameplayRoadHandler* pHandler, HEXVec_i& kHex, int iSideA, int iSideB);

/// size 0x1d4f04
class cvGameplayRoadHandler {
public:
    /// 0x087dc654
    UNK_RET SubscribeToEvents();
    /// 0x087dc7a8
    UNK_RET OnSetBridgeState(GameCoreEventTemplate<Event_Int2Type<47>, ToggleBridgesData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087dc906
    UNK_RET OnRoadDestroyedEvil(GameCoreEventTemplate<Event_Int2Type<46>, HexID, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087dc998
    UNK_RET DeleteRoad(HEXVec_i const& kHex);
    /// 0x087dcb1a
    UNK_RET StartupGame();
    /// 0x087dce50
    UNK_RET AddRoad(HEXVec_i const& kHex, unsigned char ucRouteType, unsigned char ucRouteState);
    /// 0x087dd026
    void ReleaseRoadsOnHex(HEXVec_i const& kHex);
    /// 0x087dd0f6
    void RefreshRoadsOnHex(HEXVec_i const& kHex);
    /// 0x087dd2f2
    void CalculateRoadSpline(HEXVec_i const& kHex, unsigned int uiSegment);
    /// 0x087dd3e2
    unsigned int UpdateRoadGeometry(FFastVector<FGXVector2, true, 47u, 0u, BaseVector<FGXVector2, true>::FDefaultFastVectorAllocator> const& kSpline, unsigned char ucRouteState, bool bCreate) const;
    /// 0x087dd8b8
    unsigned int CalculateRoadCoverage(HEXVec_i const& kHex);
    /// 0x087dd9c8
    unsigned int UpdateRoadTopology(unsigned char* pucSegments, unsigned int uiSideMask, unsigned char ucCityMode);
    /// 0x087ddcb0
    void CalculateRoadSplineCity(HEXVec_i const& kHex, unsigned int uiSegment);
    /// 0x087de366
    void TightenRoadCurve(FFastVector<FGXVector2, true, 47u, 0u, BaseVector<FGXVector2, true>::FDefaultFastVectorAllocator>& kSpline);
    /// 0x087de446
    unsigned int UpdateCityRoadTopology(unsigned char* pucSegments, unsigned int uiSideMask, unsigned char ucCityMode);
    /// 0x087de4a6
    UNK_RET Update(float a1);
    /// 0x087df262
    UNK_RET Fun_087df262(HEXVec_i const& a1);
    /// 0x0a30f9f4
    static UNK_DATA ms_aRoadPriority[0x6];
    // The 15 unordered pairs of the 6 sides: three straight lines, then twelve bends; RoadCell::m_aucSegments indexes it.
    /// 0x0920c448
    static int ms_aaiSegmentSides[0xf][0x2];

    // Update pops one (x, y) short pair per call from the end of this list.
    short* m_pPendingHexes;                 ///< +0x0
    unsigned int m_uiNumPendingHexes;       ///< +0x4
    unsigned int field_8;                   ///< +0x8
    int m_iBridgeEnabled;                   ///< +0xc
    int field_10;                           ///< +0x10
    WorldGlobalsData m_kWorldGlobals;       ///< +0x14
    FGXInt2 m_iGridSize;                    ///< +0x160
    RoadGrid m_RoadGrid;                    ///< +0x168
    RoadCell m_kEmptyCell;                  ///< +0x1d4e08
    FGXVector2 m_avSideMid[6][2];           ///< +0x1d4e38
    FGXVector2 m_vHexOrigin;                ///< +0x1d4e98
    FGXVector2 m_avNeighborDelta[6];        ///< +0x1d4ea0
    int m_aiSideBit[6];                     ///< +0x1d4ed0
    unsigned char m_ucBridgeState;          ///< +0x1d4ee8
    char unk_1d4ee9[0x3];                   ///< +0x1d4ee9
    FFastVector<FGXVector2, true, 47u, 0u, BaseVector<FGXVector2, true>::FDefaultFastVectorAllocator> m_SplinePoints; ///< +0x1d4eec
    void* field_1d4ef8;                     ///< +0x1d4ef8
    unsigned int field_1d4efc;              ///< +0x1d4efc
    int field_1d4f00;                       ///< +0x1d4f00
};

namespace cvLayoutState {
/// 0x087e17f0
UNK_RET BlendLayoutMesh(TerrainTensorSurface a1);
}

// The one instance is g_cvRoadLayoutSimSystem (symbol size 0xd8c).
/// size ?
class cvRoadLayoutSim {
public:
    /// 0x086831c6
    UNK_RET Fun_086831c6();
    /// 0x086833ba AddLargeRoad<FGXVector2>
    template <class T0>
    void AddLargeRoad(T0 const* a1, unsigned int a2, bool a3);
    /// 0x087e1974
    void OnImprovementCreated(GameCoreEventTemplate<Event_Int2Type<59>, LandmarkCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087e1c78
    void OnImprovementDestroyed(GameCoreEventTemplate<Event_Int2Type<60>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);

    char unk_0[0x154];                           ///< +0x0
    cvRoadNetworkManager m_RoadNetworkManager;   ///< +0x154
};
