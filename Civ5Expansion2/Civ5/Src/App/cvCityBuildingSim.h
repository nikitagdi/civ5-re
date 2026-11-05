/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastAllocator.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FUnrolledList.h"
#include "FirePlace/FireWorks/FUnrolledList_Core.h"
#include "FirePlace/FireWorks/FUnrolledList_Tail_Pointer.h"
#include "Civ5/Src/App/NodeArray.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/cvCityBuildingLayoutGen.h"
#include "FirePlace/FireGrafix/FGXVector2.h"

class AssetSwappedData;
class CityContinentChangedData;
class CityCreatedData;
class CityCultureChangedData;
class CityGenericData;
class CityPopulationChangedData;
class CitySetDamageData;
class CoastCP;
class EraChangedData;
class FGXVector3;
class FGXVector4;
class FloodFillCallback;
class GreatWallData;
class GreatWallHexData;
class GreatWallInstance;
class HEXVec_i;
class HexCultureChangedData;
class HexFOWStateChangedData;
class HexID;
class LinearEventDispatcher;
class LocalMachineContainer;
class NewHexRoadData;
class NullEventClass;
class Particle;
class RiverTileAddedData;
class RoadVertex;
class SerialContainer;
class SpecialTileAddedData;
class SquareXWrapGridPolicy;
class TerrainGenCallback;
class TerrainLineSeedCallback;
class WonderData;
class cvTerrainSampler;
class WonderRemovedData;
class WorldGlobalsData;
class cvCity_FogState;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, bool T3> class FUnrolledList;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1, unsigned int T2, unsigned int T3> class DefaultStaticHexGrid;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class FUnrolledList_Tail_Pointer;
template <class T0, class T1> class cvSerialBaseEvent;
template <class T0, unsigned int T1, unsigned int T2> class NodeArray;
template <class T0> class ArrayCallback;
template <int T0> class Event_Int2Type;
namespace B_SPLINE {
enum BSPLINE_BEGIN_TYPES : int;
enum BSPLINE_END_TYPES : int;
}
namespace CITY {
class cvCityData;

// The cell of the 255x255 city hex grid at cvCityBuildingSim +0x44730.
/// size 0x2
class CityHexData {
public:
    unsigned char unk_0[0x2];         ///< +0x0
};
}
namespace GameSpline {
class ControlPoint;
}

// Node of a polygon list, 30 points per node. A free node keeps the free-list link in m_uNext and has m_bDeleted set.
/// size 0xfc
template <>
class NodeArray<FGXVector2, 30u, 14u> {
public:
    unsigned int m_uPrev;             ///< +0x0
    unsigned int m_uNext;             ///< +0x4
    unsigned int m_uCount : 31;       ///< +0x8
    unsigned int m_bDeleted : 1;      ///< +0xb
    FGXVector2 m_aItems[30];          ///< +0xc

    /// inline
    unsigned int ALLOC_GetNext() const { return m_uNext; }
    /// inline
    void ALLOC_SetNext(unsigned int uNodeIndex) { m_uNext = uNodeIndex; }
    /// inline
    bool ALLOC_GetDeleted() const { return m_bDeleted != 0; }
    /// inline
    void ALLOC_SetDeleted(bool bDeleted) { m_bDeleted = bDeleted ? 1 : 0; }
};

typedef FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> WallPolygonPool;

// A chain of nodes in a shared pool. A chain ends at 0x0fffffff; a position handle is node << 8 | index in the node.
/// size 0x14
template <>
class FUnrolledList_Core<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> > > {
public:
    // Passed to the spline templates through a hidden copy.
    /// size 0x10
    class iterator {
    public:
        FUnrolledList_Core* m_pList;      ///< +0x0
        unsigned int m_uNodeCount;        ///< +0x4
        unsigned int m_uNode;             ///< +0x8
        unsigned int m_uPos;              ///< +0xc

        /// inline
        iterator() : m_pList(0), m_uNodeCount(0), m_uNode(0x0fffffffu), m_uPos(0) {}
        /// inline
        iterator(iterator const& rhs) : m_pList(rhs.m_pList), m_uNodeCount(rhs.m_uNodeCount), m_uNode(rhs.m_uNode), m_uPos(rhs.m_uPos) {}
        /// inline
        FGXVector2& operator*() const { return (*m_pList->m_pPool)[m_uNode].m_aItems[m_uPos]; }
        /// inline
        bool operator==(iterator const& rhs) const { return m_uNode == rhs.m_uNode && m_uPos == rhs.m_uPos; }
        /// inline
        bool operator!=(iterator const& rhs) const { return !(*this == rhs); }
    };

    /// size 0x10
    class const_iterator {
    public:
        FUnrolledList_Core const* m_pList; ///< +0x0
        unsigned int m_uNodeCount;        ///< +0x4
        unsigned int m_uNode;             ///< +0x8
        unsigned int m_uPos;              ///< +0xc

        /// inline
        const_iterator() : m_pList(0), m_uNodeCount(0), m_uNode(0x0fffffffu), m_uPos(0) {}
        /// inline
        const_iterator(const_iterator const& rhs) : m_pList(rhs.m_pList), m_uNodeCount(rhs.m_uNodeCount), m_uNode(rhs.m_uNode), m_uPos(rhs.m_uPos) {}
        /// inline
        FGXVector2 const& operator*() const { return (*m_pList->m_pPool)[m_uNode].m_aItems[m_uPos]; }
        /// inline
        bool operator==(const_iterator const& rhs) const { return m_uNode == rhs.m_uNode && m_uPos == rhs.m_uPos; }
        /// inline
        bool operator!=(const_iterator const& rhs) const { return !(*this == rhs); }
    };

    WallPolygonPool* m_pPool;         ///< +0x0
    unsigned int m_uNodeCount;        ///< +0x4
    unsigned int m_uHead;             ///< +0x8
    unsigned int m_uTail;             ///< +0xc
    unsigned int m_uSize;             ///< +0x10

    /// inline
    iterator begin() {
        iterator kIt;
        kIt.m_pList = this;
        if (m_uHead != 0x0fffffffu) {
            kIt.m_uNode = m_uHead & 0x00ffffffu;
            kIt.m_uNodeCount = (*m_pPool)[kIt.m_uNode].m_uCount;
        }
        return kIt;
    }
    /// inline
    iterator end() {
        iterator kIt;
        kIt.m_pList = this;
        return kIt;
    }
    /// inline
    const_iterator begin() const {
        const_iterator kIt;
        kIt.m_pList = this;
        if (m_uHead != 0x0fffffffu) {
            kIt.m_uNode = m_uHead & 0x00ffffffu;
            kIt.m_uNodeCount = (*m_pPool)[kIt.m_uNode].m_uCount;
        }
        return kIt;
    }
    /// inline
    const_iterator end() const {
        const_iterator kIt;
        kIt.m_pList = this;
        return kIt;
    }
};

/// size 0x14
template <>
class FUnrolledList<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, true> : public FUnrolledList_Core<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> > > {
public:
};

typedef FUnrolledList<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, true> WallPolygon;
typedef FUnrolledList_Core<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> > >::iterator WallPolygonIterator;

// The list members below are not exported, so they are internal functions taking the list as their first argument.
// Appends the point; returns its position handle.
/// 0x087bb104
static unsigned int PolygonPushBack(WallPolygon* pPolygon, FGXVector2 const& rPoint);
// Inserts the point at the front; returns its position handle.
/// 0x087c57b6
static unsigned int PolygonPushFront(WallPolygon* pPolygon, FGXVector2 const& rPoint);
// Erases the last point.
/// 0x087c58f8
static void PolygonPopBack(WallPolygon* pPolygon);
// Erases the point at the position handle; returns the iterator behind it.
/// 0x087c5950
static WallPolygonIterator PolygonErase(WallPolygon* pPolygon, unsigned int uPosition);
// Unlinks and frees the nodes from uFirstNode up to uEndNode; the caller then zeroes m_uSize.
/// 0x087bbeec
static void PolygonEraseNodes(WallPolygon* pPolygon, unsigned int uFirstNode, unsigned int uEndNode);
// Moves to the next point, to the next node when the current one is used up.
/// 0x087c5892
static void PolygonIteratorNext(WallPolygonIterator* pIterator);
/// 0x087bb55c
static void PolygonConstIteratorNext(WallPolygon::const_iterator* pIterator);

// One slot per walled city, indexed by cvCityData::m_nWallSlot.
/// size 0x20
class CityWallSlot {
public:
    unsigned char unk_0[0x8];         ///< +0x0
    WallPolygon m_Polygon;            ///< +0x8
    unsigned char unk_1c[0x4];        ///< +0x1c
};

// The wall generator object at cvCityBuildingSim +0x248: the sampler and the jitter of the 64x64 sample lattice of the polygon search.
/// size ?
class cvCityWallGenerator {
public:
    cvTerrainSampler* m_pTerrainSampler; ///< +0x0
    unsigned char unk_4[0x8];         ///< +0x4
    FGXVector2 m_aSampleOffsets[8][8]; ///< +0xc

    /// 0x087c3dc8
    void GenerateWallBetweenPoints(CITY::cvCityData* pCity, FGXVector2* pPoints, unsigned int uStart, unsigned int uEnd, unsigned int uTotal);
    /// 0x087c442e
    void GenerateWallDataFromPolygon(CITY::CityHexData const& hexData, CITY::cvCityData& city, WallPolygon const& polygon);
    /// 0x087c464a
    void GenerateWallPolygonFromCity(HEXVec_i hex, CITY::cvCityData& city, WallPolygon* pPolygon, unsigned char ucEdges);
};

namespace WONDER {
/// size 0x10
class HexCorner {
public:
    HEXVec_i m_Hex;                   ///< +0x0
    unsigned char m_nCorner;          ///< +0xc
    unsigned char unk_d[0x3];         ///< +0xd
};

/// size 0x8
class HexCornerCell {
public:
    unsigned short m_nUsedCornerBits; ///< +0x0
    unsigned char m_anCornerFlags[2]; ///< +0x2
    unsigned char m_anEdgeFlags[4];   ///< +0x4
};
}

/// size 0x24
class CityBuildingRecord {
public:
    unsigned int m_nNext;             ///< +0x0
    unsigned char unk_4[0x4];         ///< +0x4
    FGXVector2 m_vPos;                ///< +0x8
    FGXVector2 m_vDir;                ///< +0x10
    unsigned char m_ucFlags;          ///< +0x18
    unsigned char m_ucDamageRank;     ///< +0x19
    unsigned char unk_1a[0xa];        ///< +0x1a
};

/// size ?
class CityBuildingPool {
public:
    unsigned char unk_0[0x8];         ///< +0x0
    CityBuildingRecord* m_pRecords;   ///< +0x8
};

namespace CITY {
/// size 0x6f0
class cvCityData {
public:
    CityBuildingPool* m_pBuildingPool; ///< +0x0
    unsigned char unk_4[0x4];         ///< +0x4
    // Head of the building list chained through CityBuildingRecord::m_nNext; 0x0fffffff ends it.
    unsigned int m_nFirstBuilding;    ///< +0x8
    unsigned char unk_c[0xc];         ///< +0xc
    int field_18;                   ///< +0x18
    int m_nCulture;                   ///< +0x1c
    int field_20;                     ///< +0x20
    int m_nContinent;                 ///< +0x24
    int m_nPopulation;                ///< +0x28
    short m_nHexX;                    ///< +0x2c
    short m_nHexY;                    ///< +0x2e
    unsigned char field_30;           ///< +0x30
    unsigned char unk_31[0x3];        ///< +0x31
    int m_nWallSlot;                  ///< +0x34
    unsigned char unk_38[0x4ac];      ///< +0x38
    unsigned char unk_4e4[0x46];      ///< +0x4e4
    unsigned char unk_52a[0x4];       ///< +0x52a
    unsigned short field_52e;         ///< +0x52e
    unsigned char field_530;          ///< +0x530
    unsigned char unk_531[0x1];       ///< +0x531
    unsigned char m_nDamage;          ///< +0x532
    unsigned char m_nFlags533;        ///< +0x533
    unsigned int field_534;           ///< +0x534
    unsigned int field_538;           ///< +0x538
    unsigned int field_53c;           ///< +0x53c
    unsigned char unk_540[0x38];      ///< +0x540
    unsigned char unk_578[0x10];      ///< +0x578
    unsigned int field_588;           ///< +0x588
    unsigned char unk_58c[0xc8];      ///< +0x58c
    unsigned char unk_654[0x28];      ///< +0x654
    unsigned char unk_67c[0x14];      ///< +0x67c
    unsigned char field_690;          ///< +0x690
    unsigned char unk_691[0x43];      ///< +0x691
    unsigned int m_anInCitySlots[4];  ///< +0x6d4
    unsigned char unk_6e4[0x8];       ///< +0x6e4
    unsigned int m_hCityVis;          ///< +0x6ec
};
}


/// size 0x4020
class LayoutDensityField {
public:
    unsigned char m_aCells[128 * 128]; ///< +0x0
    int m_nWidth;                     ///< +0x4000
    int m_nHeight;                    ///< +0x4004
    float m_fMinX;                    ///< +0x4008
    float m_fMinY;                    ///< +0x400c
    float m_fMaxX;                    ///< +0x4010
    float m_fMaxY;                    ///< +0x4014
    float m_fScaleX;                  ///< +0x4018
    float m_fScaleY;                  ///< +0x401c
};

/// size 0x48
class LayoutPattern {
public:
    int m_nFieldSet;                  ///< +0x0
    unsigned char m_nEdgeMask;        ///< +0x4
    unsigned char unk_5[0x3];         ///< +0x5
    float m_aReflection[16];          ///< +0x8
};

// g_LayoutState: authored density fields of the city layout system.
/// size 0x4090
class LayoutFieldState {
public:
    unsigned char unk_0[0x1c];        ///< +0x0
    LayoutPattern* m_pPatterns;       ///< +0x1c
    unsigned int m_nPatternCount;     ///< +0x20
    unsigned char unk_24[0x4];        ///< +0x24
    LayoutDensityField* m_pField;     ///< +0x28
    int m_bFieldLoaded;               ///< +0x2c
    unsigned char unk_30[0xc];        ///< +0x30
    unsigned char* m_pFieldData;      ///< +0x3c
    unsigned char unk_40[0x4028];     ///< +0x40
    float m_fHeadingBase;             ///< +0x4068
    float m_fNoiseOffsetX;            ///< +0x406c
    float m_fNoiseOffsetY;            ///< +0x4070
    // cvFarmSimSystem::FillLandmarkSimInfo clamps its heading sample to 1 unit inside it.
    FGXMathRect::Rect m_kFieldBounds; ///< +0x4074
    unsigned char unk_4084[0xc];      ///< +0x4084
};

// Decompresses the field of (pattern set, population) into m_pField and sets its rect to 0..128.
/// 0x087bb370
static LayoutDensityField* LoadDensityField(LayoutFieldState* pState, int patternSet, int population);

// World position to a heading vector of length 64 from two octaves of noise.
/// 0x087bbab8
FGXVector2 Fun_087bbab8(LayoutFieldState const* pState, FGXVector2 const& worldPos);

/// size 0x20
class cvCityVis_AddBuilding {
public:
    unsigned int m_nBuildingId;       ///< +0x0
    FGXVector2 m_vPos;                ///< +0x4
    float m_fHeight;                  ///< +0xc
    FGXVector2 m_vDir;                ///< +0x10
    unsigned char unk_18[0x4];        ///< +0x18
    unsigned char m_ucClass;          ///< +0x1c
    unsigned char unk_1d[0x3];        ///< +0x1d
};

/// size ?
class cvCityVis_BeginUpdate {
};

/// size 0x4
class cvCityVis_EndUpdate {
public:
    unsigned int m_nBuildingCount;    ///< +0x0
};

/// size ?
class cvCityBuildingSim {
public:
    unsigned char unk_0[0x28];        ///< +0x0
    cvTerrainSampler* m_pTerrainSampler; ///< +0x28
    unsigned char unk_2c[0xc0];       ///< +0x2c
    float m_HexToWorldMatrix[16];     ///< +0xec
    unsigned char unk_12c[0xc4];      ///< +0x12c
    int m_nHexGridWidth;              ///< +0x1f0
    int m_nHexGridHeight;             ///< +0x1f4
    unsigned char unk_1f8[0x1c];      ///< +0x1f8
    CityWallSlot* m_pWallSlots;       ///< +0x214
    unsigned char unk_218[0x10];      ///< +0x218
    CityBuildingRecord* m_pBuildingSlots; ///< +0x228
    unsigned char unk_22c[0x10];      ///< +0x22c
    CITY::cvCityData* m_pCities;      ///< +0x23c
    unsigned int m_nCityCount;        ///< +0x240
    unsigned char unk_244[0x4];       ///< +0x244
    // A cvCityWallGenerator starts here; its extent is not known.
    unsigned char unk_248[0x3fc28];   ///< +0x248
    unsigned int field_3fe70;         ///< +0x3fe70
    unsigned int m_aDamageEffectHandles[9]; ///< +0x3fe74
    cvCityBuildingLayoutGen m_LayoutState; ///< +0x3fe98
    unsigned char unk_44730[0x94];    ///< +0x44730
    int m_nWrapWidth;                 ///< +0x447c4
    unsigned char unk_447c8[0x4];     ///< +0x447c8
    unsigned char m_bWrapX;           ///< +0x447cc
    unsigned char unk_447cd[0x1];     ///< +0x447cd
    unsigned char unk_447ce[0x1fc02]; ///< +0x447ce
    // Grid object of HexCornerCell at the start; its extent is not known.
    unsigned char m_aHexCornerGrid[0x7f0d0]; ///< +0x643d0
    unsigned char unk_e34a0[0xc];     ///< +0xe34a0
    unsigned char m_nQueueFlags;      ///< +0xe34ac
    unsigned char unk_e34ad[0x3];     ///< +0xe34ad
    unsigned char* m_pDirtyQueue;     ///< +0xe34b0
    unsigned int m_nDirtyCount;       ///< +0xe34b4

    /// 0x087b2e78
    cvCityBuildingSim();
    /// 0x087b31ea
    ~cvCityBuildingSim();
    /// 0x087b32a2
    UNK_RET Startup();
    /// 0x087b3550
    UNK_RET SubscribeToEvents();
    /// 0x087b37bc
    UNK_RET ResetGrids();
    /// 0x087b3888
    UNK_RET Shutdown();
    /// 0x087b38ca
    UNK_RET UnSubscribeToEvents();
    /// 0x087b3b26
    UNK_RET OnEraChanged(GameCoreEventTemplate<Event_Int2Type<48>, EraChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b3c6a
    UNK_RET OnCityCultureChanged(GameCoreEventTemplate<Event_Int2Type<49>, CityCultureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b3da4
    UNK_RET OnRuinsCreated(GameCoreEventTemplate<Event_Int2Type<35>, CityCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b3f28
    UNK_RET OnCityDestroyed(GameCoreEventTemplate<Event_Int2Type<41>, CityGenericData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b4062
    UNK_RET OnCitySetDamage(GameCoreEventTemplate<Event_Int2Type<44>, CitySetDamageData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b4152
    UNK_RET OnCityPopulationChanged(GameCoreEventTemplate<Event_Int2Type<43>, CityPopulationChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b4284
    UNK_RET OnAssetSwap(EventTemplate<Event_Int2Type<128>, AssetSwappedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b4368
    UNK_RET OnWonderCreated(GameCoreEventTemplate<Event_Int2Type<37>, WonderData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b52d0
    UNK_RET OnWonderRemoved(GameCoreEventTemplate<Event_Int2Type<38>, WonderRemovedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b5634
    UNK_RET OnWonderEdited(GameCoreEventTemplate<Event_Int2Type<39>, WonderData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b579a
    UNK_RET OnWallCreated(GameCoreEventTemplate<Event_Int2Type<142>, HexID, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b5944
    UNK_RET OnBeginRoadUpdate(EventTemplate<Event_Int2Type<13>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b5b54
    UNK_RET OnNewRoad(EventTemplate<Event_Int2Type<14>, NewHexRoadData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b6070
    UNK_RET OnEndRoadUpdate(EventTemplate<Event_Int2Type<15>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b6132
    UNK_RET OnFOWChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b6318
    UNK_RET OnCityContinentChanged(EventTemplate<Event_Int2Type<126>, CityContinentChangedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b6420
    UNK_RET OnCityCreated(GameCoreEventTemplate<Event_Int2Type<34>, CityCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b648e
    UNK_RET StartupGame();
    /// 0x087b650e
    UNK_RET InitHexCorners();
    /// 0x087b67ae
    UNK_RET ShutdownGame();
    /// 0x087b6872
    unsigned char CalculateHexEdgeCoverage(HEXVec_i const& hex) const;
    /// 0x087b6964
    UNK_RET Update(float a1);
    /// 0x087b6ba8
    UNK_RET UpdateInCityBuildings(unsigned int cityIdx);
    /// 0x087b6f9e
    void UpdateCityWall(unsigned int cityIdx);
    /// 0x087b7166
    UNK_RET UpdateWonders(unsigned int a1);
    /// 0x087b73a4
    UNK_RET GeneratePopBuildingPlacements(unsigned int cityIdx);
    /// 0x087b7de4
    UNK_RET ResetPopBuildingsFromPlacementsInCity(unsigned int cityIdx);
    /// 0x087b8412
    UNK_RET UpdateDamageState(unsigned int a1);
    /// 0x087b861c
    UNK_RET UpdateCityBase(unsigned int a1);
    /// 0x087b86a6
    UNK_RET SetVisStateFromCurrentSimState(cvCity_FogState const& a1, cvCity_FogState& a2);
    /// 0x087b86b4
    UNK_RET CreateNewCity(CityCreatedData& a1);
    /// 0x087b892a
    int FindCityBuildingPlacementSlot(unsigned int cityIdx, bool allowRandom);
    // returns 0 none, 1 free slot, 2 shared corner (allowShared only)
    /// 0x087b8988
    int FindWonderPlacementSlot(unsigned char kind, HEXVec_i hex, WONDER::HexCorner* out, bool allowShared);
    /// 0x087b8ddc
    UNK_RET OnSpecialTileAdded(GameCoreEventTemplate<Event_Int2Type<68>, SpecialTileAddedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b8ff8
    UNK_RET OnRiverTileAdded(GameCoreEventTemplate<Event_Int2Type<67>, RiverTileAddedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087b90b8
    UNK_RET UpdateBuildingBase(unsigned int a1, unsigned int a2);
    /// 0x087b923c
    UNK_RET StorePopBuildingPlacementsInCity(unsigned int a1, FFastAllocator<Particle, true, 47u, 0u, BaseVector<Particle, true>::FDefaultFastVectorAllocator> const& a2);
    /// 0x087b9500
    FGXVector4 CalculateWonderPositionFromSlot(WONDER::HexCorner const& slot);
    /// 0x087b9852
    CITY::cvCityData* GetCityData(HEXVec_i const& a1);
    // Entries 0..5 are the candidates of wonder kind 2, 6..11 of kind 4; kinds 8 and 16 take all twelve.
    /// 0x0a4cc088
    static WONDER::HexCorner ms_aWonderCornerCandidates[0xc];
    /// 0x0920c118
    static UNK_DATA m_BaseTextureIDsByEra[0x10];
};

// The cell of the hex corner grid (m_aHexCornerGrid) that holds hex.
/// 0x087ba112
WONDER::HexCornerCell* Fun_087ba112(unsigned char* pGrid, HEXVec_i const& hex);

// Raw height-map sample at a world position; land lies in [0x32c9, 0x32c9 + 22000).
/// 0x087baba2
static unsigned short SampleRawHeight(cvTerrainSampler* pSampler, FGXVector2 const& pos);

/// 0x087bc32e RasterizeLine<ArrayCallback<unsigned char> >
/// 0x0891c76c RasterizeLine<TerrainGenCallback>
/// 0x0891c889 RasterizeLine<TerrainLineSeedCallback>
template <class T0>
void RasterizeLine(int a1, int a2, int a3, int a4, T0& a5);
/// 0x087c2a67 POLY_Triangulate<FGXVector2, FGXVector2*, FStaticVector<unsigned short, 32u, true, 55u, 0u> >
/// 0x08a1e6ae POLY_Triangulate<FGXVector2, FUnrolledList_Core<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> > >::const_iterator, FFastVector<unsigned short, true, 43u, 260u, BaseVector<unsigned short, true>::FDefaultFastVectorAllocator> >
template <class T0, class T1, class T2>
unsigned int POLY_Triangulate(T1 a1, T1 a2, T2& a3);
/// 0x087c31b0 POLY_IsPointContained<FGXVector2*>
/// 0x08a23ae6 POLY_IsPointContained<FUnrolledList_Core<RoadVertex, NodeArray<RoadVertex, 20u, 9u>, FFastAllocator<NodeArray<RoadVertex, 20u, 9u>, true, 47u, 0u, BaseVector<NodeArray<RoadVertex, 20u, 9u>, true>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<RoadVertex, 20u, 9u>, FFastAllocator<NodeArray<RoadVertex, 20u, 9u>, true, 47u, 0u, BaseVector<NodeArray<RoadVertex, 20u, 9u>, true>::FDefaultFastVectorAllocator> > >::iterator>
template <class T0>
bool POLY_IsPointContained(T0 a1, T0 a2, FGXVector2 const& a3);
/// 0x087c5489 POLY_ComputeBoundingHull<FGXVector2*, FUnrolledList<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, true> >
template <class T0, class T1>
unsigned int POLY_ComputeBoundingHull(T0 a1, unsigned int a2, T1& a3);
/// 0x087c56e1 POLY_SignedArea<FUnrolledList_Core<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> > >::iterator>
template <class T0>
float POLY_SignedArea(T0 a1, unsigned int a2);
/// 0x08a1edfe POLY_SignedArea<FUnrolledList_Core<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> > >::const_iterator>
/// 0x08a1de97 POLY_SignedArea<FUnrolledList_Core<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> > >::iterator>
/// 0x087c6275 POLY_SignedArea<FUnrolledList_Core<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> > >::iterator>
/// 0x08a239ee POLY_SignedArea<FUnrolledList_Core<RoadVertex, NodeArray<RoadVertex, 20u, 9u>, FFastAllocator<NodeArray<RoadVertex, 20u, 9u>, true, 47u, 0u, BaseVector<NodeArray<RoadVertex, 20u, 9u>, true>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<RoadVertex, 20u, 9u>, FFastAllocator<NodeArray<RoadVertex, 20u, 9u>, true, 47u, 0u, BaseVector<NodeArray<RoadVertex, 20u, 9u>, true>::FDefaultFastVectorAllocator> > >::iterator>
template <class T0>
float POLY_SignedArea(T0 a1, T0 a2);
/// 0x087c90cd HexGridFloodFill<DefaultStaticHexGrid<GreatWallHexData, SquareXWrapGridPolicy, 255u, 255u>, FloodFillCallback>
template <class T0, class T1>
unsigned int HexGridFloodFill(T0& a1, T1& a2, HEXVec_i* a3, HEXVec_i const& a4);
extern "C" {
    /// 0x0914ed74
    extern UNK_DATA JOB_EVENT_END[0x10];
    /// 0x0914ed64
    extern UNK_DATA JOB_EVENT_START[0x10];
    /// 0x0913e064
    extern UNK_DATA ZONEPROPERTY_GameNewHost[0x10];
    /// 0x0913e044
    extern UNK_DATA ZONEPROPERTY_GameOptions[0x10];
    /// 0x0913e054
    extern UNK_DATA ZONEPROPERTY_GameState[0x10];
    /// 0x0913e034
    extern UNK_DATA ZONEPROPERTY_LobbyGuid3[0x10];
    /// 0x0914aba0
    extern UNK_DATA g_fHeightScale[0x4];
}

namespace B_SPLINE {
/// 0x088ebab2 ReconstructScalingFunc<CoastCP, CoastCP*, CoastCP*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x087df919 ReconstructScalingFunc<FGXVector2, FGXVector2 const*, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x087bb5c1 ReconstructScalingFunc<FGXVector2, FGXVector2*, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x087c9333 ReconstructScalingFunc<FGXVector3, FGXVector3*, FGXVector3*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
template <class T0, class T1, class T2, B_SPLINE::BSPLINE_BEGIN_TYPES T3, B_SPLINE::BSPLINE_END_TYPES T4>
void ReconstructScalingFunc(T1 a1, T1 a2, T2 a3);
/// 0x08a17e02 CalculateApproxSplineLength<FGXVector2 const*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x087c8291 CalculateApproxSplineLength<FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x08a17ea7 CalculateApproxSplineLength<FGXVector3 const*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x0893ea5b CalculateApproxSplineLength<GameSpline::ControlPoint*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
template <class T0, B_SPLINE::BSPLINE_BEGIN_TYPES T1, B_SPLINE::BSPLINE_END_TYPES T2>
float CalculateApproxSplineLength(T0 a1, T0 a2);
// The point at parameter fT of the spline over nPointCount control points; kAt points at control point iAtIndex.
/// 0x08664f40 EvalScalingFunc<FGXVector2, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
template <class T0, class T1, B_SPLINE::BSPLINE_BEGIN_TYPES T2, B_SPLINE::BSPLINE_END_TYPES T3>
T0 EvalScalingFunc(int nPointCount, T1 kAt, float fT, int iAtIndex);
// The tangent at parameter fT; the arguments are those of EvalScalingFunc. A list iterator is advanced in its own copy.
/// 0x08683676 EvalGradient<FGXVector2, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x087c5bc9 EvalGradient<FGXVector2, FUnrolledList_Core<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> > >::iterator, (B_SPLINE::BSPLINE_BEGIN_TYPES)1, (B_SPLINE::BSPLINE_END_TYPES)1>
template <class T0, class T1, B_SPLINE::BSPLINE_BEGIN_TYPES T2, B_SPLINE::BSPLINE_END_TYPES T3>
T0 EvalGradient(int nPointCount, T1 kAt, float fT, int iAtIndex);
}

// The cell of the city hex grid (the grid object starts at cvCityBuildingSim +0x44730) that holds hex.
/// 0x087bd0cc
static CITY::CityHexData* GetCityHexCell(unsigned char* pGrid, HEXVec_i const& hex);

namespace cvGreatWallSim {
/// 0x087c8338
UNK_RET Update(float a1);
/// 0x087c83e8
UNK_RET BuildGreatWall(GreatWallInstance* a1);
/// 0x087c8c80
UNK_RET OnGreatWallCreated(GameCoreEventTemplate<Event_Int2Type<141>, GreatWallData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
/// 0x087c8e58
UNK_RET OnHexCultureChanged(GameCoreEventTemplate<Event_Int2Type<29>, HexCultureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
/// 0x087c8f18
UNK_RET OnWorldGlobalsChanged(EventTemplate<Event_Int2Type<36>, WorldGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
}

namespace cvLayoutFieldManager {
/// 0x0920c378
extern UNK_DATA ms_amCityHexPatterns[0x7];
/// 0x0920c374
extern UNK_DATA ms_nCityHexPatterns[0x4];
}
