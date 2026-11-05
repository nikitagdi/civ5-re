/// origin guess
#include <math.h>
#include <Windows.h>
#include "Civ5/Src/App/StreamlinePump.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/TerrainStencil.h"
#include "Civ5/Src/App/cvCityBuildingSim.h"
#include "FirePlace/FireWorks/FMemHooks.h"

/// 0x087e4196
static void SeedQueueInit(StreamlineSeedQueue* pQueue) {
    pQueue->m_uCount = 0;
    pQueue->m_kPool.m_uFirstEmpty = 0x0fffffff;
    pQueue->m_kPool.m_uDataCapacity = 0;
    pQueue->m_kPool.m_uDataSize = 0;
    pQueue->m_kPool.m_pData = 0;
    pQueue->m_kPool.m_uSize = 0;
    pQueue->m_kPool.m_pData = static_cast<StreamlineSeed*>(FireMallocAlignedNoTracking(2000 * sizeof(StreamlineSeed), 4, "Port/FirePlace/FireWorks/FFastVector.h", 0x55));
    pQueue->m_kPool.m_uDataCapacity = 2000;
    pQueue->m_kLists.m_uListCount = 0;
    pQueue->m_kLists.m_uListCapacity = 0;
    pQueue->m_kLists.m_pLists = 0;
    pQueue->m_kLists.m_pAlloc = &pQueue->m_kPool;
    SeedListsSetMinNumLists(&pQueue->m_kLists, 3);
}

/// 0x087e4544
static StreamlineSeed SeedQueuePopFront(StreamlineSeedQueue* pQueue) {
    StreamlineSeed kSeed;
    for (unsigned int uList = 0; uList < 3; uList++) {
        StreamlineSeedList* pList = &pQueue->m_kLists.m_pLists[uList];
        if (pList->m_uiSize != 0) {
            kSeed = pList->m_pAlloc->m_pData[pList->m_uiFirst];
            SeedListPopFront(pList);
            break;
        }
    }
    pQueue->m_uCount--;
    return kSeed;
}

/// 0x087e5ac8
static void SeedQueueInsertAtRandom(StreamlineSeedQueue* pQueue, StreamlineSeed const& seed, unsigned int uList) {
    pQueue->m_uCount++;
    StreamlineSeedList* pList = &pQueue->m_kLists.m_pLists[uList];
    if (pList->m_uiSize == 0) {
        SeedListPushFrontExisting(pList, SeedPoolAlloc(pList->m_pAlloc, seed));
        return;
    }
    // The new seed lands before the element at a random position, never at the end.
    unsigned int uBefore = pList->m_uiFirst;
    int iSteps = (int)(rand_wincompatible() % pList->m_uiSize);
    for (; iSteps > 0; iSteps--) {
        uBefore = (uBefore == 0x0fffffff) ? pList->m_uiFirst : pList->m_pAlloc->m_pData[uBefore].node.uiNext;
    }
    StreamlineSeedListIterator kIt;
    kIt.m_pList = pList;
    kIt.m_uNode = uBefore;
    SeedListInsertExisting(pList, kIt, SeedPoolAlloc(pList->m_pAlloc, seed));
}

/// 0x087e6588
static float FieldEvalWeight(LayoutTensorFieldEval const* pEval, FGXVector2 const& vPos) {
    const unsigned char ucBlocked = g_TerrainStencil.SampleWorldPosition(vPos, 8);
    if (!FieldEvalContains(pEval, vPos)) {
        return 0.0f;
    }
    return (float)(ucBlocked == 0);
}

/// 0x087e6070
static bool PolicyStepAlongField(StreamlineGenerationPolicy const* pPolicy, LayoutTensorFieldEval const& eval, unsigned int uDirection, unsigned int uFamily, FGXVector2 const& vFrom, FGXVector2& vTo) {
    vTo = vFrom;
    if (!FieldEvalContains(&eval, vTo)) {
        return false;
    }
    FGXVector2 vField = Fun_087bbab8(eval.m_pState, vFrom);
    float fLength = sqrtf(vField.y * vField.y + vField.x * vField.x);
    if (fLength == 0.0f) {
        return false;
    }
    // Family 1 follows the field, family 2 its right-hand normal; direction 8 walks backwards.
    bool bAcross;
    bool bBackward;
    switch (uDirection | uFamily) {
    case 5:
        bAcross = false;
        bBackward = false;
        break;
    case 6:
        bAcross = true;
        bBackward = false;
        break;
    case 9:
        bAcross = false;
        bBackward = true;
        break;
    case 10:
        bAcross = true;
        bBackward = true;
        break;
    default:
        return true;
    }
    FGXVector2 vDir;
    if (bAcross) {
        vDir.x = vField.y / fLength;
        vDir.y = -vField.x / fLength;
    } else {
        vDir.x = vField.x / fLength;
        vDir.y = vField.y / fLength;
    }
    // Four fixed-point passes: each step from vFrom uses the direction sampled at the previous estimate.
    for (int iPass = 0; iPass < 4; iPass++) {
        if (bBackward) {
            vTo.x = vFrom.x - vDir.x * 0.5f * pPolicy->m_fStepLength;
            vTo.y = vFrom.y - vDir.y * 0.5f * pPolicy->m_fStepLength;
        } else {
            vTo.x = vDir.x * 0.5f * pPolicy->m_fStepLength + vFrom.x;
            vTo.y = vDir.y * 0.5f * pPolicy->m_fStepLength + vFrom.y;
        }
        if (!FieldEvalContains(&eval, vTo)) {
            return false;
        }
        vField = Fun_087bbab8(eval.m_pState, vTo);
        fLength = sqrtf(vField.y * vField.y + vField.x * vField.x);
        if (fLength == 0.0f) {
            return false;
        }
        if (bAcross) {
            vDir.x = vField.y / fLength;
            vDir.y = -vField.x / fLength;
        } else {
            vDir.x = vField.x / fLength;
            vDir.y = vField.y / fLength;
        }
    }
    return true;
}

/// 0x087e5f8e
static FGXVector2 PumpNext(StreamlinePump<LayoutTensorFieldEval, StreamlineGenerationPolicy>* pPump) {
    const FGXVector2 vFrom = pPump->m_Points.begin()[pPump->m_Points.size() - 1];
    const LayoutTensorFieldEval kEval = pPump->m_Eval;
    FGXVector2 vTo;
    if (PolicyStepAlongField(&pPump->m_Policy, kEval, pPump->m_ucDirection, pPump->m_ucFamily, vFrom, vTo) && FieldEvalWeight(&pPump->m_Eval, vTo) > 0.0f) {
        pPump->m_Points.push_back(vTo);
    }
    const int iCursor = pPump->m_iCursor++;
    return pPump->m_Points.begin()[iCursor + 1];
}
