/// origin guess
#pragma once
#include "Civ5/Src/App/DynamicHexGrid.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/cvFloodplainsSim.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FTempHeap.h"

class CoastCP;
class FGXInt2;
class FGXVector2;
class FGXVector3;
class FeatureChangedData;
class HexEdge;
class LinearEventDispatcher;
class NullEventClass;
class SerialContainer;
class cvEventSystemSerialAccessor;
class cvFloodplainsSim;
class cvSubscriptionValidator;
struct TerrainEventDelegate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;
namespace B_SPLINE {
enum BSPLINE_BEGIN_TYPES : int;
enum BSPLINE_END_TYPES : int;
}
namespace GameSpline {
class ControlPoint;
}

// Draws the river-bank and flood-plain decals along rivers; owned by TerrainSystem at +0x27818.
/// size 0x2b1a4
class cvFloodplainsSystem {
public:
    // Bits of m_auBankFlags, also the kind of bank a pass builds.
    enum RIVERBANK_FLAG : int {
        RIVERBANK_FLAG_RIVER = 1,
        RIVERBANK_FLAG_FLOODPLAIN = 2,
    };

    /// 0x08661b2a
    cvFloodplainsSystem();
    /// 0x08661b88
    ~cvFloodplainsSystem();
    /// 0x08661b8a
    UNK_RET Startup();
    /// 0x08661c26
    UNK_RET LoadFloodplainsDepthTexture();
    /// 0x08661c28
    UNK_RET SubscribeToEvents();
    /// 0x08661c64
    UNK_RET Shutdown();
    /// 0x08661ca0
    UNK_RET UnsubscribeToEvents();
    /// 0x08661cdc
    UNK_RET StartupGame();
    /// 0x08661cde
    UNK_RET ShutdownGame();
    /// 0x08661cea
    UNK_RET BuildFloodplains(FFastVector<FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator> const& kRivers);
    /// 0x08661e2c
    UNK_RET BuildTileLists(FFastVector<cvFloodplainsSim, false, 51u, 0u, FTempHeapFastVectorAllocator>& kSims, FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator>& kVisited, cvFloodplainsSystem::RIVERBANK_FLAG eFlag);
    /// 0x0866217c
    UNK_RET BuildBankVis(FFastVector<cvFloodplainsSim, false, 51u, 0u, FTempHeapFastVectorAllocator> const& kSims, cvFloodplainsSystem::RIVERBANK_FLAG eFlag);
    /// 0x0866343c
    UNK_RET AddAllRiverBanks(FFastVector<FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator> const& kRivers);
    /// 0x08663556
    UNK_RET Update(float fDeltaTime);
    /// 0x08663558
    UNK_RET OnFloodplainAdded(GameCoreEventTemplate<Event_Int2Type<52>, FeatureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x086635e8
    UNK_RET SetBankFlag(HEXVec_i kHex, cvFloodplainsSystem::RIVERBANK_FLAG eFlag, bool bSet);
    /// 0x0866367a
    UNK_RET AddRiverBank(HEXVec_i const& kHex);
    /// 0x08663704
    UNK_RET AddBankAlongRiver(HexEdge const& kStart, FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator>& kVisited, FFastVector<cvFloodplainsSim, false, 51u, 0u, FTempHeapFastVectorAllocator>& kSims, cvFloodplainsSystem::RIVERBANK_FLAG eFlag);
    /// 0x08663b40
    bool CheckForBank(HexEdge kEdge, cvFloodplainsSim::FloodplainsRiverSide const& eSide, cvFloodplainsSystem::RIVERBANK_FLAG eFlag) const;
    /// 0x08663c4e
    bool TestBankFlag(HEXVec_i kHex, cvFloodplainsSystem::RIVERBANK_FLAG eFlag) const;
    /// 0x08663cc6
    bool TestBankFlag(FGXInt2 kCell, cvFloodplainsSystem::RIVERBANK_FLAG eFlag) const;
    // kPoints/kWidths get the corner-cut bank outline; kRawPoints/kRawWidths are cleared and hold the uncut one.
    /// 0x08663ce8
    UNK_RET ExtrudeControlPoints(FFastVector<FGXVector2, true, 51u, 0u, FTempHeapFastVectorAllocator>& kPoints, FFastVector<float, true, 51u, 0u, FTempHeapFastVectorAllocator>& kWidths, cvFloodplainsSim const& kSim, FFastVector<FGXVector2, true, 51u, 0u, FTempHeapFastVectorAllocator>& kRawPoints, FFastVector<float, true, 51u, 0u, FTempHeapFastVectorAllocator>& kRawWidths) const;
    // Number of sides from uiStart on that turn around one hex on the sim's side; at least 1.
    /// 0x08664806
    unsigned int CountInternalEdges(cvFloodplainsSim const& kSim, unsigned int uiStart) const;

    int field_0;                           ///< +0x0
    unsigned char unk_4[0x4000];           ///< +0x4
    SquareGridPolicy m_kGrid;              ///< +0x4004
    // RIVERBANK_FLAG bits per grid cell, [row][column].
    unsigned int m_auBankFlags[200][200];  ///< +0x40a0
    // Set by BuildTileLists, cleared when a flood plain or river bank is added.
    bool m_bFloodplainListsReady;          ///< +0x2b1a0
    bool m_bRiverBankListsReady;           ///< +0x2b1a1
};

// True when a2 lies inside the grid a1.
/// 0x08664aae
bool Fun_08664aae(SquareGridPolicy const& a1, HEXVec_i const& a2);
// Subscribes the delegate to event 52 (FeatureChangedData).
/// 0x086655d2
UNK_RET Fun_086655d2(TerrainEventDelegate* a1);
// Unsubscribes the delegate from event 52.
/// 0x0866535c
UNK_RET Fun_0866535c(TerrainEventDelegate* a1);

/// 0x08664cc2 AddEndcaps<GameSpline::ControlPoint, FFastVector<FGXVector2, true, 51u, 0u, FTempHeapFastVectorAllocator> >
/// 0x08664e23 AddEndcaps<float, FFastVector<float, true, 51u, 0u, FTempHeapFastVectorAllocator> >
template <class T0, class T1>
void AddEndcaps(T1& a1, T1& a2);

namespace B_SPLINE {
/// 0x088ebd91 EvalScalingFunc<CoastCP, CoastCP const*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x087dfcd9 EvalScalingFunc<FGXVector2, FGXVector2 const*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x08664f40 EvalScalingFunc<FGXVector2, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x08845c18 EvalScalingFunc<FGXVector3, FGXVector3 const*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x0884601c EvalScalingFunc<FGXVector3, FGXVector3 const*, (B_SPLINE::BSPLINE_BEGIN_TYPES)1, (B_SPLINE::BSPLINE_END_TYPES)1>
/// 0x087c9578 EvalScalingFunc<FGXVector3, FGXVector3*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x087fb45f EvalScalingFunc<FGXVector3, FGXVector3*, (B_SPLINE::BSPLINE_BEGIN_TYPES)1, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x0893e908 EvalScalingFunc<GameSpline::ControlPoint, GameSpline::ControlPoint const*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x0875056a EvalScalingFunc<GameSpline::ControlPoint, GameSpline::ControlPoint*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x08b7a195 EvalScalingFunc<float, float const*, (B_SPLINE::BSPLINE_BEGIN_TYPES)1, (B_SPLINE::BSPLINE_END_TYPES)1>
/// 0x08665067 EvalScalingFunc<float, float*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
template <class T0, class T1, B_SPLINE::BSPLINE_BEGIN_TYPES T2, B_SPLINE::BSPLINE_END_TYPES T3>
T0 EvalScalingFunc(int a1, T1 a2, float a3, int a4);
}
