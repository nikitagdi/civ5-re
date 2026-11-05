/// origin guess
#pragma once
#include "Civ5/Src/App/StreamlinePump.h"
#include "Civ5/Src/App/cvCityBuildingLayoutGen.h"
#include "FirePlace/FireGrafix/FGXVector2.h"

// Node of a road polyline, six points per node.
/// size 0x3c
class RoadPointNode {
public:
    unsigned char unk_0[0x4];         ///< +0x0
    unsigned int m_uNext;             ///< +0x4
    unsigned int m_uCount : 31;       ///< +0x8
    unsigned int m_bDeleted : 1;      ///< +0xb
    FGXVector2 m_aPoints[6];          ///< +0xc
};

/// size 0x14
class RoadPointPool {
public:
    unsigned int m_uFirstEmpty;       ///< +0x0
    unsigned int m_uSize;             ///< +0x4
    RoadPointNode* m_pData;           ///< +0x8
    unsigned int m_uDataSize;         ///< +0xc
    unsigned int m_uDataCapacity;     ///< +0x10
};

class Road;

// Position in a road polyline; the end is node 0x0fffffff at index 0.
/// size 0x10
class RoadPointIterator {
public:
    Road* m_pRoad;                   ///< +0x0
    unsigned int m_uNodeCount;        ///< +0x4
    unsigned int m_uNode;             ///< +0x8
    unsigned int m_uPos;              ///< +0xc
};

// One edge of the road graph: a polyline between two junctions.
/// size 0x38
class Road {
public:
    RoadPointPool* m_pPointPool;      ///< +0x0
    unsigned int m_uNodeCount;        ///< +0x4
    unsigned int m_uHead;             ///< +0x8
    unsigned int m_uTail;             ///< +0xc
    unsigned int m_uSize;             ///< +0x10
    unsigned short m_usEndJunction;   ///< +0x14
    unsigned short m_usStartJunction; ///< +0x16
    FGXMathRect::Rect m_kBounds;      ///< +0x18
    unsigned int m_uNextInCell;       ///< +0x28
    // Negative: a free pool slot.
    int field_2c;                     ///< +0x2c
    RoadKey m_kKey;                   ///< +0x30
};

// Up to four roads meet here; each entry is a road index in the low 28 bits.
/// size 0x14
class RoadJunction {
public:
    unsigned int m_auRoads[4];        ///< +0x0
    unsigned int m_uCount : 31;       ///< +0x10
    unsigned int m_bDeleted : 1;      ///< +0x13
};

/// size 0x14
class RoadJunctionPool {
public:
    unsigned int m_uFirstEmpty;       ///< +0x0
    unsigned int m_uSize;             ///< +0x4
    RoadJunction* m_pData;            ///< +0x8
    unsigned int m_uDataSize;         ///< +0xc
    unsigned int m_uDataCapacity;     ///< +0x10
};

/// size 0x14
class RoadPool {
public:
    unsigned int m_uFirstEmpty;       ///< +0x0
    unsigned int m_uSize;             ///< +0x4
    Road* m_pData;                    ///< +0x8
    unsigned int m_uDataSize;         ///< +0xc
    unsigned int m_uDataCapacity;     ///< +0x10
};

// Grid cell of the road network: its roads chained through Road::m_uNextInCell and one terrain decal.
/// size 0x18
class RoadSet {
public:
    RoadPool* m_pRoads;               ///< +0x0
    unsigned char unk_4[0x4];         ///< +0x4
    unsigned int m_uFirstRoad;        ///< +0x8
    unsigned char unk_c[0x4];         ///< +0xc
    // 0xffffffff: none.
    unsigned int m_uDecal;            ///< +0x10
    unsigned char unk_14[0x4];        ///< +0x14
};

/// size 0xc
class RoadIndexVector {
public:
    unsigned int* m_pData;            ///< +0x0
    unsigned int m_uSize;             ///< +0x4
    unsigned int m_uCapacity;         ///< +0x8
};

// Planar graph of streamline roads; the policy part holds the current seed and the generation constants.
/// size 0xd0
class RoadNetwork : public StreamlineGenerationPolicy {
public:
    float field_6c;                  ///< +0x6c
    unsigned char unk_70[0x4];        ///< +0x70
    int field_74;                     ///< +0x74
    int field_78;                     ///< +0x78
    int field_7c;                     ///< +0x7c
    int field_80;                     ///< +0x80
    int m_iHeight;                    ///< +0x84
    int m_iWidth;                     ///< +0x88
    RoadSet* m_pRoadSets;             ///< +0x8c
    int m_iLevel;                     ///< +0x90
    RoadJunctionPool m_kJunctions;    ///< +0x94
    RoadPointPool m_kPoints;          ///< +0xa8
    RoadPool m_kRoads;                ///< +0xbc
};

/// size ?
class cvRoadNetworkManager {
public:
    /// 0x087e2558
    cvRoadNetworkManager();
    /// 0x087e2850
    void BeginUpdate(FGXMathRect::Rect const& rect);
    /// 0x087e2e36
    void GenerateSeedPoints(FGXMathRect::Rect const& rect);
    /// 0x087e3058
    bool TestSeedPoint(StreamlineSeed const& seed);
    /// 0x087e3462
    unsigned int AddStreamlineToRoadNetwork(StreamlinePump<LayoutTensorFieldEval, StreamlineGenerationPolicy>& pump, unsigned int uStreamline, unsigned int uStartJunction, unsigned int uEndJunction, bool bReverse);
    /// 0x087e3560
    void EndUpdate();
    /// 0x087e3586
    void RemoveOldDecal(RoadSet& a1);
    /// 0x087e35c2
    void AddRoadDecal(RoadSet& a1);
    /// 0x0868a43e
    void Fun_0868a43e();

    int m_iStreamlineCounter;         ///< +0x0
    RoadNetwork m_Network;            ///< +0x4
    StreamlineSeedQueue m_SeedQueue;  ///< +0xd4
    StreamlinePump<LayoutTensorFieldEval, StreamlineGenerationPolicy> m_Pump; ///< +0xfc
    RoadIndexVector m_aUpdateRoads;   ///< +0x194
    RoadIndexVector m_aStreamlineRoads; ///< +0x1a0
    StreamlineSeed* m_pSeedTemplates; ///< +0x1ac
    unsigned int m_uSeedTemplateCount; ///< +0x1b0
    unsigned int m_uSeedTemplateCapacity; ///< +0x1b4
    StreamlineSeed m_aSeedTemplateStorage[50]; ///< +0x1b8
};

/// 0x0868934a
static void RoadPointIteratorAdvance(RoadPointIterator* pIterator);
/// 0x086859f2
static unsigned int JunctionPoolAlloc(RoadJunctionPool* pPool, RoadJunction const& junction);
/// 0x08686c44
static void JunctionPoolFree(RoadJunctionPool* pPool, unsigned int uIndex);
/// 0x087e5a7a
static void RoadIndexPushBack(RoadIndexVector* pVector, unsigned int const& uRoad);
/// 0x087e45be
static unsigned int NetworkAllocJunction(RoadNetwork* pNetwork);
// Integrates the pump, splits it at every crossing into roads between junctions and appends their indices to aNewRoads.
/// 0x087e4ae0
static UNK_RET NetworkTraceStreamline(RoadNetwork* pNetwork, StreamlinePump<LayoutTensorFieldEval, StreamlineGenerationPolicy>& pump, RoadIndexVector& aTouchedRoads, RoadIndexVector& aNewRoads, RoadKey& key, unsigned int uStartJunction, unsigned int uEndJunction, int iLevel);
// Reverses the point order and swaps the two junctions.
/// 0x087e5782
static void NetworkReverseRoad(RoadNetwork* pNetwork, unsigned int uRoad);
// Appends uSecond, minus its first point, to uFirst through their shared junction and frees uSecond.
/// 0x087e4606
static void NetworkMergeRoads(RoadNetwork* pNetwork, unsigned int uFirst, unsigned int uSecond);
/// 0x08686a08
static UNK_RET NetworkRemoveRoad(RoadNetwork* pNetwork, unsigned int uRoad);
// Removes the roads of this update that nothing marked to keep.
/// 0x087e4a4e
static void ManagerRemoveUnkeptNewRoads(cvRoadNetworkManager* pManager);
// Queues kind-2 seeds at the free ends of roads in the rectangle.
/// 0x087e425a
static void ManagerQueueDeadEndSeeds(cvRoadNetworkManager* pManager, FGXMathRect::Rect const& rect);
/// 0x087e559c
static void ManagerQueueSideSeeds(cvRoadNetworkManager* pManager, FGXVector2 const* pBegin, FGXVector2 const* pEnd, unsigned char ucFamily);
/// 0x087e4962
static void ManagerPruneDeadEnds(cvRoadNetworkManager* pManager);
