/// origin guess
#pragma once
#include "Civ5/Src/App/Engine/cvRiverSimSystem.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FTempHeap.h"

// One run of river sides along which one bank of a river or flood plain is drawn.
/// size 0x10
class cvFloodplainsSim {
public:
    // Which bank of the river the run follows; left uses GetNextLeftEdge, right GetNextRightEdge.
    enum FloodplainsRiverSide : int {
        FLOODPLAINS_RIVER_SIDE_LEFT = 0,
        FLOODPLAINS_RIVER_SIDE_RIGHT = 1,
    };

    FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator> m_kEdges; ///< +0x0
    FloodplainsRiverSide m_eSide;                                             ///< +0xc
};
