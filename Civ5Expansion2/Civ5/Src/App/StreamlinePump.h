/// origin guess
#pragma once
#include "Civ5/Src/App/cvCityBuildingLayoutGen.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireWorks/FFastList.h"
#include "FirePlace/FireWorks/FFastVector.h"

class LayoutFieldState;

// A streamline start point; the leading list node links it into a seed list.
/// size 0x30
class StreamlineSeed : public NullMultiListNodePolicy {
public:
    // 0 grid seed, 1 side branch, 2 dead-end continuation.
    unsigned char m_ucKind;           ///< +0x8
    // 1 along the field, 2 across it.
    unsigned char m_ucFamily;         ///< +0x9
    // 4 forward, 8 backward.
    unsigned char m_ucDirection;      ///< +0xa
    unsigned char unk_b[0x1];         ///< +0xb
    FGXVector2 m_vPos;                ///< +0xc
    FGXVector2 m_vDir;                ///< +0x14
    unsigned char unk_1c[0x8];        ///< +0x1c
    unsigned int m_uStreamline;       ///< +0x24
    unsigned int m_uJunction;         ///< +0x28
    unsigned char unk_2c[0x4];        ///< +0x2c
};

// FFastAllocator layout over StreamlineSeed; free slots chain through ListNode::uiNext.
/// size 0x14
class StreamlineSeedPool {
public:
    unsigned int m_uFirstEmpty;       ///< +0x0
    unsigned int m_uSize;             ///< +0x4
    StreamlineSeed* m_pData;          ///< +0x8
    unsigned int m_uDataSize;         ///< +0xc
    unsigned int m_uDataCapacity;     ///< +0x10
};

class StreamlineSeedList;

/// size 0x8
class StreamlineSeedListIterator {
public:
    StreamlineSeedList* m_pList;      ///< +0x0
    unsigned int m_uNode;             ///< +0x4
};

// FCustomList over the shared StreamlineSeedPool; 0x0fffffff ends a chain.
/// size 0x10
class StreamlineSeedList {
public:
    StreamlineSeedPool* m_pAlloc;     ///< +0x0
    unsigned int m_uiSize;            ///< +0x4
    unsigned int m_uiFirst;           ///< +0x8
    unsigned int m_uiLast;            ///< +0xc
};

// FMultiList layout: the lists and the pool they share.
/// size 0x10
class StreamlineSeedMultiList {
public:
    StreamlineSeedList* m_pLists;     ///< +0x0
    unsigned int m_uListCount;        ///< +0x4
    unsigned int m_uListCapacity;     ///< +0x8
    StreamlineSeedPool* m_pAlloc;     ///< +0xc
};

// Three seed lists in one pool, drained in list order 0, 1, 2.
/// size 0x28
class StreamlineSeedQueue {
public:
    unsigned int m_uCount;            ///< +0x0
    StreamlineSeedPool m_kPool;       ///< +0x4
    StreamlineSeedMultiList m_kLists; ///< +0x18
};

// Field sampler of the pump: the layout heading field clipped to a rectangle.
/// size 0x14
class LayoutTensorFieldEval {
public:
    LayoutFieldState* m_pState;       ///< +0x0
    FGXMathRect::Rect m_kBounds;      ///< +0x4
};

// Key of a streamline as stored in each road made from it.
/// size 0x8
class RoadKey {
public:
    unsigned int m_uStreamline;       ///< +0x0
    // 0xffff: not in a cell.
    unsigned short m_usCell;          ///< +0x4
    unsigned char m_ucFamily;         ///< +0x6
    unsigned char m_ucKeep;           ///< +0x7
};

/// size 0x6c
class StreamlineGenerationPolicy {
public:
    StreamlineSeed m_kSeed;           ///< +0x0
    RoadKey m_kKey;                   ///< +0x30
    unsigned char unk_38[0x8];        ///< +0x38
    int field_40;                     ///< +0x40
    float field_44;                   ///< +0x44
    float m_fStepLength;              ///< +0x48
    float m_fSeedSpacing;             ///< +0x4c
    // Indexed by the seed kind.
    float m_afSeedRadius[3];          ///< +0x50
    float field_5c;                   ///< +0x5c
    unsigned char unk_60[0x4];        ///< +0x60
    int m_iFirstUpdateStreamline;     ///< +0x64
    int field_68;                     ///< +0x68
};

/// size ?
template <class T0, class T1>
class StreamlinePump {
public:
};

/// size 0x98
template <>
class StreamlinePump<LayoutTensorFieldEval, StreamlineGenerationPolicy> {
public:
    StreamlineGenerationPolicy m_Policy; ///< +0x0
    unsigned char m_ucDirection;      ///< +0x6c
    unsigned char m_ucFamily;         ///< +0x6d
    unsigned char unk_6e[0x2];        ///< +0x6e
    // First point not yet in a road.
    int m_iCommitted;                 ///< +0x70
    // -1 before the first PumpNext.
    int m_iCursor;                    ///< +0x74
    LayoutTensorFieldEval m_Eval;     ///< +0x78
    FFastVector<FGXVector2, true, 47u, 0u, BaseVector<FGXVector2, true>::FDefaultFastVectorAllocator> m_Points; ///< +0x8c
};

// FFastAllocator::Alloc: copies the seed into a free or new slot and marks it valid.
/// 0x087e5c98
static unsigned int SeedPoolAlloc(StreamlineSeedPool* pPool, StreamlineSeed const& seed);
/// 0x087e5d64
static void SeedListPushFrontExisting(StreamlineSeedList* pList, unsigned int uIndex);
/// 0x087e66c4
static void SeedListPushBackExisting(StreamlineSeedList* pList, unsigned int uIndex);
// Links uIndex before the iterator's node; the end iterator appends.
/// 0x087e665e
static void SeedListInsertExisting(StreamlineSeedList* pList, StreamlineSeedListIterator const& it, unsigned int uIndex);
/// 0x087e6798
static void SeedListPopFront(StreamlineSeedList* pList);
/// 0x087e690e
static void SeedListsSetMinNumLists(StreamlineSeedMultiList* pLists, unsigned int uNumLists);
/// 0x087e4196
static void SeedQueueInit(StreamlineSeedQueue* pQueue);
/// 0x087e4544
static StreamlineSeed SeedQueuePopFront(StreamlineSeedQueue* pQueue);
/// 0x087e5ac8
static void SeedQueueInsertAtRandom(StreamlineSeedQueue* pQueue, StreamlineSeed const& seed, unsigned int uList);
// Inside the layout state's world rectangle [min, max) and strictly inside the eval rectangle.
/// 0x087e65f6
static bool FieldEvalContains(LayoutTensorFieldEval const* pEval, FGXVector2 const& vPos);
// 1 where the point is inside and the terrain stencil bit 8 is clear, else 0.
/// 0x087e6588
static float FieldEvalWeight(LayoutTensorFieldEval const* pEval, FGXVector2 const& vPos);
/// 0x087e6070
static bool PolicyStepAlongField(StreamlineGenerationPolicy const* pPolicy, LayoutTensorFieldEval const& eval, unsigned int uDirection, unsigned int uFamily, FGXVector2 const& vFrom, FGXVector2& vTo);
// Grows the polyline by one step when possible, then returns the point at the advanced cursor.
/// 0x087e5f8e
static FGXVector2 PumpNext(StreamlinePump<LayoutTensorFieldEval, StreamlineGenerationPolicy>* pPump);
