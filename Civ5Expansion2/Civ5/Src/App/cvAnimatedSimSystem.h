/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastAllocator.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FUnrolledList_Core.h"
#include "Civ5/Src/App/Graphics/cvLandmarkLibrary.h"
#include "Civ5/Src/App/Graphics/cvLandmarkVisSystem.h"
#include "Civ5/Src/App/Graphics/TerrainDecalLayer.h"
#include "Civ5/Src/App/DynamicHexGrid.h"
#include "Civ5/Src/App/cvCityBuildingSim.h"

class EraChangedData;
class FGXVector2;
class HEXVec_i;
class HexFOWStateChangedData;
class HexID;
class LandmarkCreatedData;
class LandmarkHandleData;
class LinearEventDispatcher;
class LocalMachineContainer;
class MapGlobalsData;
class NullEventClass;
class PlotData;
class PlotPacketData;
class RoadVertex;
class SerialContainer;
class TechAcquiredData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvLandmarkVis_Init;
class cvSubscriptionValidator;
class cvTerrainSampler;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class EventPacketData;
template <class T0, class T1> class FUnrolledList_Tail_Pointer;
template <class T0, class T1> class cvSerialBaseEvent;
template <class T0, unsigned int T1, unsigned int T2> class NodeArray;
template <int T0> class Event_Int2Type;
template <unsigned int T0> class cvInfoIDBitSet;
namespace FGXMathRect {
class Rect;
}
namespace cvLandmark_Properties {
enum eLandmarkEra : int;
enum eLandmarkState : int;
enum eLandmarkType : int;
}

/// size 0xe8
class cvAnimatedSimSystem {
public:
    unsigned int m_uSimPoolInfo;                  ///< +0x0
    unsigned int m_uFreeHandleHead;               ///< +0x4
    char unk_8[4];                                ///< +0x8
    char* m_pSims;                                ///< +0xc
    char unk_10[0xc];                             ///< +0x10
    unsigned int m_uFirstSim;                     ///< +0x1c
    unsigned int m_uPoolEnd;                      ///< +0x20
    cvLandmarkLibrary* m_pLandmarkLibrary;        ///< +0x24
    char m_EventQueue[0x10];                      ///< +0x28
    char m_RenderEventQueue[0x10];                ///< +0x38
    cvTerrainSampler* m_pTerrainSampler;          ///< +0x48
    char unk_4c[0x80];                            ///< +0x4c
    bool m_bStartupFlag;                          ///< +0xcc
    FFastVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true, 54u, 0u, BaseVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true>::FDefaultFastVectorAllocator> m_AnimatedPastureRefs; ///< +0xd0
    FFastVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true, 54u, 0u, BaseVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true>::FDefaultFastVectorAllocator> m_AnimatedRotatedRefs; ///< +0xdc
    /// 0x08a18538
    cvAnimatedSimSystem();
    /// 0x08a18604
    ~cvAnimatedSimSystem();
    /// 0x08a18674
    UNK_RET Startup(bool a1);
    /// 0x08a18770
    UNK_RET SubscribeToEvents();
    /// 0x08a18882
    UNK_RET Shutdown();
    /// 0x08a188f2
    UNK_RET UnSubscribeToEvents();
    /// 0x08a18a04
    UNK_RET DestroySims();
    /// 0x08a18a6c
    UNK_RET StartupGame();
    /// 0x08a18a6e
    UNK_RET ShutdownGame();
    /// 0x08a18aa4
    UNK_RET OnMapGlobalsChanged(EventTemplate<Event_Int2Type<37>, MapGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a18ad8
    UNK_RET OnRawResourceCreated(GameCoreEventTemplate<Event_Int2Type<55>, LandmarkCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a18b9e
    UNK_RET OnRawResourceDestroyed(GameCoreEventTemplate<Event_Int2Type<56>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a18c52
    UNK_RET OnImprovementCreated(GameCoreEventTemplate<Event_Int2Type<59>, LandmarkCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a18d18
    UNK_RET OnImprovementDestroyed(GameCoreEventTemplate<Event_Int2Type<60>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a18dcc
    UNK_RET OnEraChanged(GameCoreEventTemplate<Event_Int2Type<48>, EraChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a19258
    UNK_RET OnFOWChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a19422
    UNK_RET OnTechAcquired(GameCoreEventTemplate<Event_Int2Type<128>, TechAcquiredData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a19a5c
    UNK_RET Update(float a1);
    /// 0x08a19a8c
    UNK_RET DestroySim(unsigned int a1);
    /// 0x08a19ade
    int GetLandmarkEntryIndex(int iType, int iSubType, cvLandmark_Properties::eLandmarkEra eEra, cvLandmark_Properties::eLandmarkState eState, cvInfoIDBitSet<256u> const& kTechs, bool& bRandomFallback) const;
    /// 0x08a19b84
    UNK_RET GetBestLandmarkEntryIndex(int a1, int a2, cvLandmark_Properties::eLandmarkEra a3, cvLandmark_Properties::eLandmarkState a4, cvInfoIDBitSet<256u> const& a5, bool& a6) const;
    /// 0x08a19bfc
    UNK_RET IsBestLandmarkEntryIndex(int a1, int a2, int a3, cvLandmark_Properties::eLandmarkEra a4, cvLandmark_Properties::eLandmarkState a5, cvInfoIDBitSet<256u> const& a6) const;
    /// 0x08a19d18
    UNK_RET GetHandleByLocation(HexID const& a1);
    /// 0x08a19d7e
    UNK_RET GetHandleByLocation(HexID const& a1, cvLandmark_Properties::eLandmarkType a2);
};

/// 0x08a1dcfe POLY_Centroid<FUnrolledList_Core<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> > >::iterator>
/// 0x08a23855 POLY_Centroid<FUnrolledList_Core<RoadVertex, NodeArray<RoadVertex, 20u, 9u>, FFastAllocator<NodeArray<RoadVertex, 20u, 9u>, true, 47u, 0u, BaseVector<NodeArray<RoadVertex, 20u, 9u>, true>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<RoadVertex, 20u, 9u>, FFastAllocator<NodeArray<RoadVertex, 20u, 9u>, true, 47u, 0u, BaseVector<NodeArray<RoadVertex, 20u, 9u>, true>::FDefaultFastVectorAllocator> > >::iterator>
template <class T0>
FGXVector2 POLY_Centroid(T0 a1, T0 a2);
/// 0x08a1ea34 POLY_ComputeBounds<FUnrolledList_Core<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> > >::iterator>
template <class T0>
FGXMathRect::Rect POLY_ComputeBounds(T0 a1, unsigned int a2);

typedef FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> FarmPolygonPool;

// Outline of one farm field; the nodes live in cvFarmSimSystem::m_PolygonNodes. A chain ends at 0x0fffffff.
/// size 0x14
template <>
class FUnrolledList_Core<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> > > {
public:
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

    FarmPolygonPool* m_pPool;         ///< +0x0
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

typedef FUnrolledList_Core<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> > > FarmPolygon;
typedef FarmPolygon::iterator FarmPolygonIterator;

// Moves to the next point, to the next node when the current one is used up.
/// 0x08a1eb20
static void FarmPolygonConstIteratorNext(FarmPolygon::const_iterator* pIterator);
/// 0x08a1ecc0
static void FarmPolygonIteratorNext(FarmPolygonIterator* pIterator);
// Element names of the farm_types.xml groups in cvFarmSimSystem::m_aDecalGroups order; the first is "Crops_Half_Built".
/// 0x09224524
extern const char s_aszFarmDecalGroupNames[8][0x20];

// Event 3 payload: one chunk of up to 16 outline points of a plot polygon.
/// size 0xa0
struct PlotPacketRecord {
    HEXVec_i m_kHex;                  ///< +0x0
    // Plot id; the chunks of one outline share it.
    int m_iPacketId;                  ///< +0xc
    // Number of plots sent for the hex.
    int m_iNumHexPlots;               ///< +0x10
    int m_iChunk;                     ///< +0x14
    int m_iNumChunks;                 ///< +0x18
    int m_iNumPoints;                 ///< +0x1c
    FGXVector2 m_aPoints[16];         ///< +0x20
};

// Event 59 as delivered: the serial header word, then the LandmarkCreatedData fields the farm code reads.
/// size ?
struct LandmarkCreatedEventRecord {
    unsigned int m_uHeader;           ///< +0x0
    int m_iResourceType;              ///< +0x4
    int m_iImprovementType;           ///< +0x8
    unsigned char m_ucPlayer;         ///< +0xc
    unsigned char unk_d[0x3];         ///< +0xd
    unsigned short m_usDecalStyle;    ///< +0x10
    unsigned char unk_12[0x2];        ///< +0x12
    unsigned short m_usVisStyle;      ///< +0x14
    unsigned char unk_16[0x2];        ///< +0x16
    short m_sX;                       ///< +0x18
    short m_sY;                       ///< +0x1a
    unsigned char unk_1c[0x4];        ///< +0x1c
    int m_iEra;                       ///< +0x20
    // cvLandmarkLibraryEntry state: 2 under construction, 4 constructed, 8 pillaged.
    unsigned char m_ucState;          ///< +0x24
};

class cvFarmSimSystem;
// Delegate stored with a deferred event: object, member handler, this adjustment.
/// size 0xc
class FarmEventCallback {
public:
    cvFarmSimSystem* m_pObject;       ///< +0x0
    void* m_pfnHandler;               ///< +0x4
    int m_iThisAdjust;                ///< +0x8
};

// Farm visuals: terrain decals on the field polygons sent for each farm hex, plus one landmark model per hex.
/// size 0x207c0
class cvFarmSimSystem {
public:
    // Outline assembly progress: chunks expected (-1 before the first) and chunks received.
    /// size 0x8
    class AssemblyState {
    public:
        int m_iNumChunks;                 ///< +0x0
        int m_iNumReceived;               ///< +0x4
    };

    /// size 0x10
    class PlotAssembly {
    public:
        int m_iPacketId;                  ///< +0x0
        unsigned int m_uPlot;             ///< +0x4
        AssemblyState m_kState;           ///< +0x8
    };

    /// size 0x18
    class AssemblyNode {
    public:
        unsigned int m_uNext;             ///< +0x0
        unsigned int m_uPrev : 31;        ///< +0x4
        unsigned int m_bValid : 1;        ///< +0x7
        PlotAssembly m_kData;             ///< +0x8
    };

    // Outlines still missing chunks: a node pool plus a doubly linked list ending at 0x0fffffff.
    /// size 0x20
    class AssemblyList {
    public:
        unsigned int m_uFreeHead;         ///< +0x0
        unsigned int m_uNumUsed;          ///< +0x4
        AssemblyNode* m_pNodes;           ///< +0x8
        unsigned int m_uNumSlots;         ///< +0xc
        unsigned int m_uCapacity;         ///< +0x10
        unsigned int m_uSize;             ///< +0x14
        unsigned int m_uFirst;            ///< +0x18
        unsigned int m_uLast;             ///< +0x1c
    };

    // First decal type and number of types of one farm_types.xml group.
    /// size 0x4
    class FarmDecalGroup {
    public:
        unsigned short m_usFirst;         ///< +0x0
        unsigned short m_usCount;         ///< +0x2
    };

    // Vector with inline storage for 64 types.
    /// size 0x410
    class DecalTypeVector {
    public:
        DecalType* m_pData;               ///< +0x0
        unsigned int m_uSize;             ///< +0x4
        unsigned int m_uCapacity;         ///< +0x8
        DecalType m_aStorage[64];         ///< +0xc
        bool m_bIsResized;                ///< +0x40c
        unsigned char unk_40d[0x3];       ///< +0x40d
    };

    // Vector with inline storage for 16 farm indices.
    /// size 0x50
    class PendingFarmVector {
    public:
        unsigned int* m_pData;            ///< +0x0
        unsigned int m_uSize;             ///< +0x4
        unsigned int m_uCapacity;         ///< +0x8
        unsigned int m_auStorage[16];     ///< +0xc
        bool m_bIsResized;                ///< +0x4c
        unsigned char unk_4d[0x3];        ///< +0x4d
    };

    // One farm hex, from the improvement event; slot of m_kFarms.
    /// size 0x8c
    class FarmData {
    public:
        // Index into the landmark library entries; a free slot keeps the next free index here.
        int m_iLandmarkEntry;             ///< +0x0
        // cvLandmarkLibraryEntry state of the event: 2 under construction, 4 constructed, 8 pillaged.
        unsigned char m_ucState;          ///< +0x4
        unsigned char m_ucNumPlots;       ///< +0x5
        unsigned char m_ucFOWState;       ///< +0x6
        // 1..4; 0 suppresses the decals.
        unsigned char m_ucEra;            ///< +0x7
        unsigned char m_ucPlayer;         ///< +0x8
        // cvLandmarkVis_Init::m_uiVariant (continent art style, below 5).
        unsigned short m_uVisStyle : 4;   ///< +0x9
        // Selects the decal group, 1..4.
        unsigned short m_uDecalStyle : 4; ///< +0x9
        unsigned short m_bHasVis : 1;     ///< +0xa
        // The raw resource of the hex is ART_DEF_RESOURCE_WHEAT.
        unsigned short m_bOnWheat : 1;    ///< +0xa
        // Set while the pool slot is free.
        unsigned short m_bFree : 1;       ///< +0xa
        // Random index below m_auNumFarmVariants[era].
        unsigned short m_uVariant : 5;    ///< +0xa
        short m_sX;                       ///< +0xc
        short m_sY;                       ///< +0xe
        // World position of the landmark model, written by FillLandmarkSimInfo.
        float m_fAnchorX;                 ///< +0x10
        float m_fAnchorY;                 ///< +0x14
        float m_fAnchorZ;                 ///< +0x18
        // Indices into m_kPlots.
        unsigned short m_ausPlots[50];    ///< +0x1c
        // The cvGeneralRefHandle of the landmark vis; only its first word is stored.
        unsigned int m_hVis;              ///< +0x80
        // Key of the pillage smoke effect: this FarmData's address and 0, or zero while there is none.
        FarmData* m_pSmokeKey;            ///< +0x84
        unsigned int m_uSmokeKeyHigh;     ///< +0x88
    };

    // One field polygon of a farm; slot of m_kPlots.
    /// size 0x2c
    class FarmPlot {
    public:
        bool m_bFree;                     ///< +0x0
        unsigned char unk_1[0x3];         ///< +0x1
        unsigned int m_uNextFree;         ///< +0x4
        FarmPolygon m_Polygon;            ///< +0x8
        int m_iPacketId;                  ///< +0x1c
        unsigned short m_usFarm;          ///< +0x20
        unsigned char unk_22[0x2];        ///< +0x22
        unsigned int m_hDecal;            ///< +0x24
        unsigned int m_hOverlayDecal;     ///< +0x28
    };

    /// size 0x14
    class FarmPlotPool {
    public:
        unsigned int m_uFreeHead;         ///< +0x0
        unsigned int m_uNumUsed;          ///< +0x4
        FarmPlot* m_pData;                ///< +0x8
        unsigned int m_uNumSlots;         ///< +0xc
        unsigned int m_uCapacity;         ///< +0x10
    };

    /// size 0x14
    class FarmDataPool {
    public:
        unsigned int m_uFreeHead;         ///< +0x0
        unsigned int m_uNumUsed;          ///< +0x4
        FarmData* m_pData;                ///< +0x8
        unsigned int m_uNumSlots;         ///< +0xc
        unsigned int m_uCapacity;         ///< +0x10
    };

    AssemblyList m_kAssemblies;           ///< +0x0
    // Counts the outlines completed from plot packets.
    unsigned int m_uNumCompletedPlots;    ///< +0x20
    // Groups in the order of the names at 0x09224524 (stride 0x20), the first being "Crops_Half_Built".
    FarmDecalGroup m_aDecalGroups[8];     ///< +0x24
    DecalTypeVector m_kDecalTypes;        ///< +0x44
    unsigned int m_hBlankTexture;         ///< +0x454
    // Farms waiting for their PlotUpdateEnd event.
    PendingFarmVector m_kPendingFarms;    ///< +0x458
    int m_iWheatLandmarkType;             ///< +0x4a8
    int m_iFarmLandmarkType;              ///< +0x4ac
    // Landmark library records of the farm models, [variant][era].
    int m_aaiFarmEntries[3][5];           ///< +0x4b0
    unsigned int m_auNumFarmVariants[5];  ///< +0x4ec
    unsigned char unk_500[0xb0];          ///< +0x500
    SquareGridPolicy m_kPlotGrid;         ///< +0x5b0
    cvTerrainSampler* m_pTerrainSampler;  ///< +0x64c
    unsigned char unk_650[0x80];          ///< +0x650
    bool m_bStartupFlag;                  ///< +0x6d0
    unsigned char unk_6d1[0x3];           ///< +0x6d1
    // The library embedded in g_cvLandmarkVisSystem at +0xb54.
    cvLandmarkLibrary* m_pLandmarkLibrary; ///< +0x6d4
    FarmPolygonPool m_PolygonNodes;       ///< +0x6d8
    FarmPlotPool m_kPlots;                ///< +0x6ec
    SquareGridPolicy m_kFarmGrid;         ///< +0x700
    // Farm index per grid cell [row][column] of m_kFarmGrid; 0xffff = no farm.
    unsigned short m_aausFarmIndex[256][256]; ///< +0x79c
    FarmDataPool m_kFarms;                ///< +0x2079c
    // Events received while bit 0 of m_ucDeferFlags is set.
    unsigned char m_aucEventQueue[0xc];   ///< +0x207b0
    // Bit 0: the handlers queue their event in m_aucEventQueue instead of handling it.
    unsigned char m_ucDeferFlags;         ///< +0x207bc
    unsigned char unk_207bd[0x3];         ///< +0x207bd

    /// 0x08a1a750
    UNK_RET SubscribeToEvents();
    /// 0x08a1a894
    UNK_RET OnImprovementCreated(GameCoreEventTemplate<Event_Int2Type<59>, LandmarkCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a1ab70
    UNK_RET OnImprovementDestroyed(GameCoreEventTemplate<Event_Int2Type<60>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a1ac82
    UNK_RET OnEraChanged(GameCoreEventTemplate<Event_Int2Type<48>, EraChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a1ae78
    UNK_RET OnFOWChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a1b030
    UNK_RET OnPlotUpdateEnd(EventTemplate<Event_Int2Type<5>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a1b402
    UNK_RET OnPlotPacketReceived(EventTemplate<Event_Int2Type<3>, EventPacketData<FGXVector2, PlotPacketData>, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a1b8f6
    UNK_RET OnPlotRequestReleaseReceived(EventTemplate<Event_Int2Type<6>, PlotData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a1bad2
    UNK_RET UnSubscribeToEvents();
    /// 0x08a1bc12
    UNK_RET Startup(bool bStartupFlag);
    /// 0x08a1bcf6
    UNK_RET LoadFarmDecalTextures(char const* pszFileName);
    /// 0x08a1c150
    UNK_RET Shutdown();
    /// 0x08a1c214
    void FillLandmarkSimInfo(cvFarmSimSystem::FarmData& kFarm, cvLandmarkVis_Init& kInit);
    /// 0x08a1c82c
    UNK_RET StartupGame();
    /// 0x08a1c978
    UNK_RET ShutdownGame();
    /// 0x08a1c99c
    UNK_RET DestroyImprovement(HEXVec_i kHex);
    /// 0x08a1d004
    void ReleaseFarmDecal(unsigned int uFarm, unsigned int uPlot);
    /// 0x08a1d08e
    void PaintRoadNetwork(unsigned int uFarm, unsigned int uPlot, bool bPaint);
    /// 0x08a1d4ac
    void CreateFarmDecal(unsigned int uFarm, unsigned int uPlot);
};

// Copies kType behind the last element, growing the vector first when it is full.
/// 0x08a1df90
static void PushBackDecalType(cvFarmSimSystem::DecalTypeVector* pVector, DecalType const& kType);
/// 0x08a1e0e6
static void PushBackPendingFarm(cvFarmSimSystem::PendingFarmVector* pVector, unsigned int const& uFarm);
// Takes the head of the free list or appends a slot, copies kFarm and clears m_bFree; returns the slot.
/// 0x08a1dff2
static unsigned int AllocFarm(cvFarmSimSystem::FarmDataPool* pPool, cvFarmSimSystem::FarmData const& kFarm);
// Sets m_bFree and links the slot into the free list.
/// 0x08a1e138
static void FreeFarm(cvFarmSimSystem::FarmDataPool* pPool, unsigned int uFarm);
// Takes the head of the free list or appends a slot, copies kPlot and clears m_bFree; returns the slot.
/// 0x08a1f3aa
static unsigned int AllocPlot(cvFarmSimSystem::FarmPlotPool* pPool, cvFarmSimSystem::FarmPlot const& kPlot);
// Sets m_bFree and links the slot into the free list.
/// 0x08a1fbba
static void FreePlot(cvFarmSimSystem::FarmPlotPool* pPool, unsigned int uPlot);
// Takes a node from the pool, copies kAssembly and sets m_bValid; returns the node.
/// 0x08a1f116
static unsigned int AllocAssembly(cvFarmSimSystem::AssemblyList* pList, cvFarmSimSystem::PlotAssembly const& kAssembly);
/// 0x08a1f19c
static void LinkAssemblyAtEnd(cvFarmSimSystem::AssemblyList* pList, unsigned int uNode);
// Unlinks and frees the nodes from uFirst up to uEnd.
/// 0x08a1f5de
static void EraseAssemblies(cvFarmSimSystem::AssemblyList* pList, unsigned int uFirst, unsigned int uEnd);
// Writes the chunk's points from index m_iChunk * 16, growing the outline first; true once every chunk has arrived.
/// 0x08a1e3dc
static bool AppendPlotPacket(cvFarmSimSystem::AssemblyState* pState, PlotPacketRecord const& kPacket, FarmPolygon* pPolygon);
// Appends an event 33 (FGXMathRect::Rect payload) to the active local-machine buffer; returns the payload.
/// 0x08a1fb0c
static FGXMathRect::Rect* QueueRectEvent(void* pQueue);
// Ends the farm's smoke effect, then starts ART_DEF_VEFFECT_FARM_PILLAGED_SMOKE_01 at the anchor when the farm is pillaged.
/// 0x08a1e21a
static void UpdatePillageSmoke(cvFarmSimSystem* pSystem, cvFarmSimSystem::FarmData& kFarm);
// Appends a local-machine event and returns its 16-byte payload; the farm code writes the smoke key and two zero words.
/// 0x087bc5a2
static unsigned int* Fun_087bc5a2(void* pQueue);
// Stores an event 59 payload with its handler while cvFarmSimSystem defers its events.
/// 0x087506be
static UNK_RET Fun_087506be(void* pQueue, void const* pPayload, FarmEventCallback const* pCallback, unsigned int uTag);
// As Fun_087506be, for event 3.
/// 0x08a1e33c
static UNK_RET Fun_08a1e33c(void* pQueue, void const* pPayload, FarmEventCallback const* pCallback, unsigned int uTag);
// As Fun_087506be, for event 5.
/// 0x08a1e17a
static UNK_RET Fun_08a1e17a(void* pQueue, void const* pPayload, FarmEventCallback const* pCallback, unsigned int uTag);
