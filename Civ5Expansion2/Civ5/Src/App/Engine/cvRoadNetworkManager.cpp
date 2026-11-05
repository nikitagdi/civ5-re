/// origin guess
#include <math.h>
#include <stdlib.h>
#include <Windows.h>
#include "Civ5/Src/App/Engine/cvRoadNetworkManager.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "FirePlace/FireWorks/FMemHooks.h"

/// 0x087e2558
cvRoadNetworkManager::cvRoadNetworkManager() {
    m_iStreamlineCounter = 0;
    m_Network.m_fStepLength = 1.3f;
    m_Network.m_fSeedSpacing = 14.400001f;
    m_Network.field_68 = 2;
    m_Network.m_afSeedRadius[0] = 13.6f;
    m_Network.m_afSeedRadius[1] = 13.6f;
    m_Network.m_afSeedRadius[2] = 4.0f;
    m_Network.field_44 = 2.4f;
    m_Network.field_5c = 9.6f;
    m_Network.m_iFirstUpdateStreamline = 0;
    m_Network.field_78 = 0;
    m_Network.field_74 = 0;
    m_Network.field_80 = 0;
    m_Network.field_7c = 0;
    m_Network.m_iLevel = 0;
    m_Network.m_pRoadSets = 0;
    m_Network.m_iWidth = 0;
    m_Network.m_iHeight = 0;
    m_Network.m_kJunctions.m_uFirstEmpty = 0x0fffffff;
    m_Network.m_kJunctions.m_uDataCapacity = 0;
    m_Network.m_kJunctions.m_uDataSize = 0;
    m_Network.m_kJunctions.m_pData = 0;
    m_Network.m_kJunctions.m_uSize = 0;
    m_Network.m_kPoints.m_uFirstEmpty = 0x0fffffff;
    m_Network.m_kPoints.m_uDataCapacity = 0;
    m_Network.m_kPoints.m_uDataSize = 0;
    m_Network.m_kPoints.m_pData = 0;
    m_Network.m_kPoints.m_uSize = 0;
    m_Network.m_kRoads.m_uFirstEmpty = 0x0fffffff;
    m_Network.m_kRoads.m_uDataCapacity = 0;
    m_Network.m_kRoads.m_uDataSize = 0;
    m_Network.m_kRoads.m_pData = 0;
    m_Network.m_kRoads.m_uSize = 0;
    SeedQueueInit(&m_SeedQueue);
    m_Pump.m_Policy.m_fStepLength = 1.3f;
    m_Pump.m_Policy.m_fSeedSpacing = 14.400001f;
    m_Pump.m_Policy.field_68 = 2;
    m_Pump.m_Policy.m_afSeedRadius[0] = 13.6f;
    m_Pump.m_Policy.m_afSeedRadius[1] = 13.6f;
    m_Pump.m_Policy.m_afSeedRadius[2] = 4.0f;
    m_Pump.m_Policy.field_44 = 2.4f;
    m_Pump.m_Policy.field_5c = 9.6f;
    m_Pump.m_Policy.m_iFirstUpdateStreamline = 0;
    m_Pump.m_iCommitted = 0;
    m_Pump.m_iCursor = -1;
    m_aUpdateRoads.m_pData = 0;
    m_Pump.m_Eval.m_kBounds.m_fMaxY = 0.0f;
    m_Pump.m_Eval.m_kBounds.m_fMaxX = 0.0f;
    m_aUpdateRoads.m_uCapacity = 0;
    m_aUpdateRoads.m_uSize = 0;
    m_Pump.m_Eval.m_kBounds.m_fMinY = 0.0f;
    m_Pump.m_Eval.m_kBounds.m_fMinX = 0.0f;
    m_aUpdateRoads.m_pData = static_cast<unsigned int*>(FireMallocAlignedNoTracking(0x20 * sizeof(unsigned int), 4, "Port/FirePlace/FireWorks/FFastVector.h", 0x55));
    m_aUpdateRoads.m_uCapacity = 0x20;
    m_uSeedTemplateCount = 0;
    m_aStreamlineRoads.m_uSize = 0;
    m_aStreamlineRoads.m_pData = 0;
    m_aStreamlineRoads.m_uCapacity = 0;
    m_uSeedTemplateCapacity = 50;
    m_pSeedTemplates = m_aSeedTemplateStorage;
}

/// 0x087e2850
void cvRoadNetworkManager::BeginUpdate(FGXMathRect::Rect const& rect) {
    GenerateSeedPoints(rect);
    FGXMathRect::Rect kRect = rect;
    ManagerQueueDeadEndSeeds(this, kRect);
    m_aUpdateRoads.m_uSize = 0;
    const FGXMathRect::Rect kBounds = rect;
    m_Network.m_iFirstUpdateStreamline = m_iStreamlineCounter;

    // Kinds above 2 reuse the previous start point; before the first seed the binary holds (0, max y).
    FGXVector2 vStart;
    vStart.x = 0.0f;
    vStart.y = kBounds.m_fMaxY;
    while (m_SeedQueue.m_kLists.m_pLists[1].m_uiSize + m_SeedQueue.m_kLists.m_pLists[0].m_uiSize + m_SeedQueue.m_kLists.m_pLists[2].m_uiSize != 0) {
        StreamlineSeed kSeed = SeedQueuePopFront(&m_SeedQueue);
        m_Network.m_kSeed = kSeed;
        if (!TestSeedPoint(kSeed)) {
            continue;
        }
        unsigned char ucFamily = kSeed.m_ucFamily;
        if (kSeed.m_ucKind == 1) {
            vStart.x = kSeed.m_vPos.x + kSeed.m_vDir.x * 1.3f * 0.5f;
            vStart.y = kSeed.m_vPos.y + kSeed.m_vDir.y * 1.3f * 0.5f;
        } else if (kSeed.m_ucKind == 0) {
            vStart = kSeed.m_vPos;
            ucFamily = (unsigned char)(2 - (rand_wincompatible() & 1));
        } else if (kSeed.m_ucKind == 2) {
            const unsigned int uJunction = kSeed.m_uJunction;
            RoadJunction const& kJunction = m_Network.m_kJunctions.m_pData[uJunction];
            const unsigned int nRoads = kJunction.m_uCount;
            if (nRoads == 0) {
                continue;
            }
            int nSameFamily = 0;
            for (unsigned int i = 0; i < nRoads; i++) {
                const unsigned char ucRoadFamily = m_Network.m_kRoads.m_pData[kJunction.m_auRoads[i] & 0x0fffffff].m_kKey.m_ucFamily;
                if (ucRoadFamily != 0) {
                    nSameFamily += (ucRoadFamily == ucFamily) ? 1 : 0;
                }
            }
            if (nSameFamily != 1) {
                continue;
            }
            m_aStreamlineRoads.m_uSize = 0;
            const FGXVector2 vPos = kSeed.m_vPos;
            m_Pump.m_Eval.m_pState = reinterpret_cast<LayoutFieldState*>(g_LayoutState);
            m_Pump.m_Eval.m_kBounds = kBounds;
            m_Pump.m_iCursor = -1;
            m_Pump.m_iCommitted = 0;
            m_Pump.m_ucDirection = kSeed.m_ucDirection;
            m_Pump.m_ucFamily = ucFamily;
            m_Pump.m_Points.clear();
            m_Pump.m_Points.push_back(vPos);
            AddStreamlineToRoadNetwork(m_Pump, kSeed.m_uStreamline, uJunction, 0xffff, kSeed.m_ucDirection == 8);
            continue;
        } else {
            ucFamily = 0;
        }

        m_iStreamlineCounter++;
        const unsigned int uJunction = NetworkAllocJunction(&m_Network);

        m_aStreamlineRoads.m_uSize = 0;
        m_Pump.m_Eval.m_pState = reinterpret_cast<LayoutFieldState*>(g_LayoutState);
        m_Pump.m_Eval.m_kBounds = kBounds;
        m_Pump.m_iCursor = -1;
        m_Pump.m_iCommitted = 0;
        m_Pump.m_ucDirection = 4;
        m_Pump.m_ucFamily = ucFamily;
        m_Pump.m_Points.clear();
        m_Pump.m_Points.push_back(vStart);
        unsigned int uForwardRoad = 0x0fffffff;
        if (AddStreamlineToRoadNetwork(m_Pump, (unsigned int)m_iStreamlineCounter, uJunction, 0xffff, false) != 0) {
            uForwardRoad = m_aStreamlineRoads.m_pData[0];
        }

        m_aStreamlineRoads.m_uSize = 0;
        m_Pump.m_Eval.m_pState = reinterpret_cast<LayoutFieldState*>(g_LayoutState);
        m_Pump.m_Eval.m_kBounds = kBounds;
        m_Pump.m_iCursor = -1;
        m_Pump.m_iCommitted = 0;
        m_Pump.m_ucDirection = 8;
        m_Pump.m_ucFamily = ucFamily;
        m_Pump.m_Points.clear();
        m_Pump.m_Points.push_back(vStart);
        if (AddStreamlineToRoadNetwork(m_Pump, (unsigned int)m_iStreamlineCounter, uJunction, 0xffff, true) != 0 && uForwardRoad != 0x0fffffff) {
            NetworkMergeRoads(&m_Network, m_aStreamlineRoads.m_pData[0], uForwardRoad);
        }
        if (m_Network.m_kJunctions.m_pData[uJunction].m_uCount == 0) {
            JunctionPoolFree(&m_Network.m_kJunctions, uJunction);
        }
    }
    ManagerPruneDeadEnds(this);
}

/// 0x087e2e36
void cvRoadNetworkManager::GenerateSeedPoints(FGXMathRect::Rect const& rect) {
    const float fCellX0 = rect.m_fMinX - fmodf(rect.m_fMinX, 64.0f);
    const float fCellY0 = rect.m_fMinY - fmodf(rect.m_fMinY, 64.0f);
    const float fCellX1 = (rect.m_fMaxX - fmodf(rect.m_fMaxX, 64.0f)) + 64.0f;
    const float fCellY1 = (rect.m_fMaxY - fmodf(rect.m_fMaxY, 64.0f)) + 64.0f;
    for (float fCellX = fCellX0; fCellX < fCellX1; fCellX += 64.0f) {
        for (float fCellY = fCellY0; fCellY < fCellY1; fCellY += 64.0f) {
            // All 50 template slots are read, whatever m_uSeedTemplateCount says.
            for (unsigned int i = 0; i < 50; i++) {
                StreamlineSeed const& kTemplate = m_pSeedTemplates[i];
                const float fX = kTemplate.m_vPos.x + fCellX;
                if (!(fX > rect.m_fMinX) || !(rect.m_fMaxX > fX)) {
                    continue;
                }
                const float fY = kTemplate.m_vPos.y + fCellY;
                if (!(fY > rect.m_fMinY) || !(rect.m_fMaxY > fY)) {
                    continue;
                }
                StreamlineSeed kSeed = kTemplate;
                kSeed.m_ucKind = 0;
                kSeed.m_vPos.x = fX;
                kSeed.m_vPos.y = fY;
                SeedQueueInsertAtRandom(&m_SeedQueue, kSeed, 2);
            }
        }
    }
}

/// 0x087e3058
bool cvRoadNetworkManager::TestSeedPoint(StreamlineSeed const& seed) {
    // The radius follows the kind of the copy in m_Network, which BeginUpdate fills from the same seed.
    float const* pRadius;
    if (m_Network.m_kSeed.m_ucKind == 2) {
        pRadius = &m_Network.m_afSeedRadius[2];
    } else if (m_Network.m_kSeed.m_ucKind == 1) {
        pRadius = &m_Network.m_afSeedRadius[1];
    } else {
        pRadius = &m_Network.m_afSeedRadius[0];
    }
    const unsigned int nRoads = m_Network.m_kRoads.m_uDataSize;
    bool bAccepted = true;
    if (nRoads == 0) {
        return bAccepted;
    }
    const float fRadius = *pRadius;
    const float fRadiusSq = fRadius * fRadius;
    const float fMinY = seed.m_vPos.y - fRadius;
    const float fMaxX = fRadius + seed.m_vPos.x;
    const float fMinX = seed.m_vPos.x - fRadius;
    const float fMaxY = fRadius + seed.m_vPos.y;
    for (unsigned int uRoad = 0; uRoad != nRoads; uRoad++) {
        Road& kRoad = m_Network.m_kRoads.m_pData[uRoad];
        if (kRoad.field_2c < 0 || kRoad.m_uSize < 2) {
            continue;
        }
        if (!(kRoad.m_kBounds.m_fMinX <= fMaxX) || !(fMinX <= kRoad.m_kBounds.m_fMaxX) || !(kRoad.m_kBounds.m_fMinY <= fMaxY) || !(fMinY <= kRoad.m_kBounds.m_fMaxY)) {
            continue;
        }
        if (seed.m_ucKind == 2) {
            if (seed.m_ucFamily != kRoad.m_kKey.m_ucFamily || seed.m_uStreamline == kRoad.m_kKey.m_uStreamline) {
                continue;
            }
        } else if (seed.m_ucKind == 1) {
            if (seed.m_ucFamily != kRoad.m_kKey.m_ucFamily) {
                continue;
            }
        }
        RoadPointIterator kPrev;
        kPrev.m_pRoad = &kRoad;
        kPrev.m_uNodeCount = 0;
        kPrev.m_uNode = 0x0fffffff;
        kPrev.m_uPos = 0;
        if (kRoad.m_uHead != 0x0fffffff) {
            kPrev.m_uNode = kRoad.m_uHead & 0x00ffffff;
            kPrev.m_uNodeCount = kRoad.m_pPointPool->m_pData[kPrev.m_uNode].m_uCount;
        }
        RoadPointIterator kNext = kPrev;
        RoadPointIteratorAdvance(&kNext);
        if (!bAccepted) {
            continue;
        }
        FGXVector2 vA = kPrev.m_pRoad->m_pPointPool->m_pData[kPrev.m_uNode].m_aPoints[kPrev.m_uPos];
        while (kNext.m_uNode != 0x0fffffff || kNext.m_uPos != 0) {
            const FGXVector2 vB = kNext.m_pRoad->m_pPointPool->m_pData[kNext.m_uNode].m_aPoints[kNext.m_uPos];
            const float fBX = seed.m_vPos.x - vB.x;
            const float fBY = seed.m_vPos.y - vB.y;
            float fDistSq;
            if (vA.x == vB.x && vA.y == vB.y) {
                fDistSq = fBY * fBY + fBX * fBX;
            } else {
                const float fEX = vB.x - vA.x;
                const float fEY = vB.y - vA.y;
                if (fEY * fBY + fEX * fBX >= 0.0f) {
                    fDistSq = fBY * fBY + fBX * fBX;
                } else {
                    const float fAX = seed.m_vPos.x - vA.x;
                    const float fAY = seed.m_vPos.y - vA.y;
                    if (-(fAY * fEY) - fAX * fEX >= 0.0f) {
                        fDistSq = fAY * fAY + fAX * fAX;
                    } else {
                        // The binary truncates the squared normal distance to an int.
                        const float fCross = fAY * fEX - fAX * fEY;
                        fDistSq = (float)abs((int)(fCross * fCross / (fEY * fEY + fEX * fEX)));
                    }
                }
            }
            if (fRadiusSq > fDistSq) {
                bAccepted = false;
            }
            RoadPointIteratorAdvance(&kNext);
            RoadPointIteratorAdvance(&kPrev);
            vA = vB;
            if (!bAccepted) {
                break;
            }
        }
    }
    return bAccepted;
}

/// 0x087e3462
unsigned int cvRoadNetworkManager::AddStreamlineToRoadNetwork(StreamlinePump<LayoutTensorFieldEval, StreamlineGenerationPolicy>& pump, unsigned int uStreamline, unsigned int uStartJunction, unsigned int uEndJunction, bool bReverse) {
    RoadKey kKey;
    kKey.m_ucFamily = pump.m_ucFamily;
    kKey.m_uStreamline = uStreamline;
    NetworkTraceStreamline(&m_Network, pump, m_aUpdateRoads, m_aStreamlineRoads, kKey, uStartJunction, uEndJunction, 0);
    FGXVector2 const* pBegin = pump.m_Points.begin();
    FGXVector2 const* pEnd = pBegin;
    if (pump.m_iCursor != -1) {
        pEnd = pBegin + pump.m_Points.size();
    }
    ManagerQueueSideSeeds(this, pBegin, pEnd, pump.m_ucFamily);
    const unsigned int nRoads = m_aStreamlineRoads.m_uSize;
    if (nRoads == 0) {
        return 0;
    }
    unsigned int const* pRoad = m_aStreamlineRoads.m_pData;
    for (unsigned int i = 0; i < nRoads; i++, pRoad++) {
        const unsigned int uRoad = *pRoad;
        if (bReverse) {
            NetworkReverseRoad(&m_Network, uRoad);
        }
        RoadIndexPushBack(&m_aUpdateRoads, uRoad);
    }
    return m_aStreamlineRoads.m_uSize;
}

/// 0x087e3560
void cvRoadNetworkManager::EndUpdate() {
    ManagerRemoveUnkeptNewRoads(this);
    Fun_0868a43e();
}

/// 0x087e4a4e
static void ManagerRemoveUnkeptNewRoads(cvRoadNetworkManager* pManager) {
    RoadNetwork& kNetwork = pManager->m_Network;
    RoadIndexVector& aUpdateRoads = pManager->m_aUpdateRoads;
    for (unsigned int i = 0; i < aUpdateRoads.m_uSize; i++) {
        const unsigned int uRoad = aUpdateRoads.m_pData[i];
        if (uRoad == 0x0fffffff || uRoad >= kNetwork.m_kRoads.m_uDataSize) {
            continue;
        }
        Road const& kRoad = kNetwork.m_kRoads.m_pData[uRoad];
        if (kRoad.field_2c < 0 || kRoad.m_kKey.m_ucFamily == 0 || kRoad.m_kKey.m_ucKeep != 0) {
            continue;
        }
        NetworkRemoveRoad(&kNetwork, uRoad);
        aUpdateRoads.m_pData[i] = 0x0fffffff;
    }
}

/// 0x087e425a
static void ManagerQueueDeadEndSeeds(cvRoadNetworkManager* pManager, FGXMathRect::Rect const& rect) {
    RoadNetwork& kNetwork = pManager->m_Network;
    StreamlineSeedQueue& kQueue = pManager->m_SeedQueue;
    // Only the kind is set up front; the other fields carry over between queued seeds.
    StreamlineSeed kSeed;
    kSeed.m_ucKind = 2;
    const unsigned int nRoads = kNetwork.m_kRoads.m_uDataSize;
    if (nRoads == 0) {
        return;
    }
    for (unsigned int uRoad = 0; uRoad != nRoads; uRoad++) {
        if (uRoad >= kNetwork.m_kRoads.m_uDataSize) {
            continue;
        }
        Road const& kRoad = kNetwork.m_kRoads.m_pData[uRoad];
        if (kRoad.field_2c < 0) {
            continue;
        }
        if (!(kRoad.m_kBounds.m_fMinX <= rect.m_fMaxX) || !(rect.m_fMinX <= kRoad.m_kBounds.m_fMaxX) || !(kRoad.m_kBounds.m_fMinY <= rect.m_fMaxY) || !(rect.m_fMinY <= kRoad.m_kBounds.m_fMaxY)) {
            continue;
        }
        const unsigned char ucFamily = kRoad.m_kKey.m_ucFamily;
        if (ucFamily == 0) {
            continue;
        }
        RoadJunction const& kStart = kNetwork.m_kJunctions.m_pData[kRoad.m_usStartJunction];
        if (kStart.m_uCount != 0) {
            int nSameFamily = 0;
            for (unsigned int i = 0; i < kStart.m_uCount; i++) {
                nSameFamily += (kNetwork.m_kRoads.m_pData[kStart.m_auRoads[i] & 0x0fffffff].m_kKey.m_ucFamily == ucFamily) ? 1 : 0;
            }
            if (nSameFamily == 1) {
                kSeed.m_ucDirection = 8;
                kSeed.m_ucFamily = ucFamily;
                kSeed.m_uStreamline = kRoad.m_kKey.m_uStreamline;
                kSeed.m_vPos = kRoad.m_pPointPool->m_pData[kRoad.m_uHead].m_aPoints[0];
                kSeed.m_uJunction = kRoad.m_usStartJunction;
                StreamlineSeedList* pList = &kQueue.m_kLists.m_pLists[0];
                SeedListPushFrontExisting(pList, SeedPoolAlloc(pList->m_pAlloc, kSeed));
                kQueue.m_uCount++;
            }
        }
        RoadJunction const& kEnd = kNetwork.m_kJunctions.m_pData[kRoad.m_usEndJunction];
        if (kEnd.m_uCount == 0) {
            continue;
        }
        int nSameFamily = 0;
        for (unsigned int i = 0; i < kEnd.m_uCount; i++) {
            nSameFamily += (kNetwork.m_kRoads.m_pData[kEnd.m_auRoads[i] & 0x0fffffff].m_kKey.m_ucFamily == kRoad.m_kKey.m_ucFamily) ? 1 : 0;
        }
        if (nSameFamily != 1) {
            continue;
        }
        kSeed.m_ucDirection = 4;
        kSeed.m_ucFamily = kRoad.m_kKey.m_ucFamily;
        kSeed.m_uStreamline = kRoad.m_kKey.m_uStreamline;
        RoadPointNode const& kTail = kRoad.m_pPointPool->m_pData[kRoad.m_uTail];
        kSeed.m_vPos = kTail.m_aPoints[kTail.m_uCount - 1];
        kSeed.m_uJunction = kRoad.m_usEndJunction;
        StreamlineSeedList* pList = &kQueue.m_kLists.m_pLists[0];
        SeedListPushBackExisting(pList, SeedPoolAlloc(pList->m_pAlloc, kSeed));
        kQueue.m_uCount++;
    }
}

/// 0x087e559c
static void ManagerQueueSideSeeds(cvRoadNetworkManager* pManager, FGXVector2 const* pBegin, FGXVector2 const* pEnd, unsigned char ucFamily) {
    if (pBegin == pEnd || pBegin + 1 == pEnd) {
        return;
    }
    StreamlineSeedQueue& kQueue = pManager->m_SeedQueue;
    const float fSpacing = pManager->m_Network.m_fSeedSpacing;
    const unsigned char ucSideFamily = (unsigned char)((ucFamily != 2 ? 1 : 0) + 1);
    float fTravelled = 0.0f;
    FGXVector2 vPrev = *pBegin;
    for (FGXVector2 const* pPoint = pBegin + 1; pPoint != pEnd; pPoint++) {
        const float fDX = pPoint->x - vPrev.x;
        const float fDY = pPoint->y - vPrev.y;
        const float fLengthSq = fDY * fDY + fDX * fDX;
        fTravelled = fTravelled + sqrtf(fLengthSq);
        // At most one side seed per segment, at its midpoint, pointing to the segment's right.
        if (fTravelled > fSpacing) {
            StreamlineSeed kSeed = StreamlineSeed();
            kSeed.m_ucFamily = ucSideFamily;
            kSeed.m_ucKind = 1;
            kSeed.m_vPos.y = (vPrev.y + pPoint->y) * 0.5f;
            kSeed.m_vPos.x = (vPrev.x + pPoint->x) * 0.5f;
            fTravelled = fTravelled - fSpacing;
            const float fLength = sqrtf(fLengthSq);
            kSeed.m_vDir.y = -fDX / fLength;
            kSeed.m_vDir.x = fDY / fLength;
            StreamlineSeedList* pList = &kQueue.m_kLists.m_pLists[1];
            SeedListPushFrontExisting(pList, SeedPoolAlloc(pList->m_pAlloc, kSeed));
            kQueue.m_uCount++;
        }
        vPrev = *pPoint;
    }
}

/// 0x087e4962
static void ManagerPruneDeadEnds(cvRoadNetworkManager* pManager) {
    RoadNetwork& kNetwork = pManager->m_Network;
    const unsigned int nRoads = kNetwork.m_kRoads.m_uDataSize;
    if (nRoads == 0) {
        return;
    }
    // Repeat full passes until one removes nothing: each removal can open a new dead end.
    bool bStable = true;
    unsigned int uRoad = 0;
    for (;;) {
        Road const& kRoad = kNetwork.m_kRoads.m_pData[uRoad];
        if (kRoad.field_2c >= 0) {
            bool bRemove = kRoad.m_uSize == 1;
            if (!bRemove && kRoad.m_kKey.m_ucFamily != 0 && kRoad.m_kKey.m_ucKeep == 0) {
                RoadJunction const* pJunctions = kNetwork.m_kJunctions.m_pData;
                bRemove = pJunctions[kRoad.m_usStartJunction].m_uCount == 1 || pJunctions[kRoad.m_usEndJunction].m_uCount == 1;
            }
            if (bRemove) {
                NetworkRemoveRoad(&kNetwork, uRoad);
                bStable = false;
            }
        }
        uRoad++;
        if (uRoad != nRoads) {
            continue;
        }
        if (bStable) {
            return;
        }
        bStable = true;
        uRoad = 0;
    }
}

/// 0x0868934a
static void RoadPointIteratorAdvance(RoadPointIterator* pIterator) {
    pIterator->m_uPos++;
    if (pIterator->m_uPos < pIterator->m_uNodeCount) {
        return;
    }
    RoadPointNode const* pNodes = pIterator->m_pRoad->m_pPointPool->m_pData;
    const unsigned int uNext = pNodes[pIterator->m_uNode].m_uNext;
    if (uNext == 0x0fffffff) {
        pIterator->m_uNode = 0x0fffffff;
        pIterator->m_uPos = 0;
        pIterator->m_uNodeCount = 0;
    } else {
        pIterator->m_uNode = uNext;
        pIterator->m_uPos = 0;
        pIterator->m_uNodeCount = pNodes[uNext].m_uCount;
    }
}

/// 0x087e45be
static unsigned int NetworkAllocJunction(RoadNetwork* pNetwork) {
    // The binary ORs these bits into an uninitialised record; the bits it leaves are never read.
    RoadJunction kJunction;
    for (unsigned int i = 0; i < 4; i++) {
        kJunction.m_auRoads[i] = 0x1fffffff;
    }
    kJunction.m_uCount = 0;
    kJunction.m_bDeleted = 0;
    return JunctionPoolAlloc(&pNetwork->m_kJunctions, kJunction);
}

/// 0x08686c44
static void JunctionPoolFree(RoadJunctionPool* pPool, unsigned int uIndex) {
    RoadJunction& kJunction = pPool->m_pData[uIndex];
    kJunction.m_bDeleted = 1;
    // A lower index becomes the free head; otherwise the slot goes right after the head.
    if (uIndex < pPool->m_uFirstEmpty) {
        kJunction.m_uCount = pPool->m_uFirstEmpty;
        kJunction.m_bDeleted = 1;
        pPool->m_uFirstEmpty = uIndex;
    } else {
        RoadJunction& kHead = pPool->m_pData[pPool->m_uFirstEmpty];
        kJunction.m_uCount = kHead.m_uCount;
        kJunction.m_bDeleted = 1;
        kHead.m_uCount = uIndex;
    }
    pPool->m_uSize--;
}
