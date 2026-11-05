/// origin guess
#include "Civ5/Src/App/cvFloodplainsSystem.h"
#include <math.h>
#include <string.h>
#include <Windows.h>
#include "Civ5/Src/App/Engine/GameSplineDatabase.h"
#include "Civ5/Src/App/Engine/cvRiverSimSystem.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainDecalLayer.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/HEXMatrix.h"
#include "Civ5/Src/App/HEXTransform_i.h"
#include "Civ5/Src/App/HEXVec.h"
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireGrafix/FGXVector2.h"

namespace {

typedef GameCoreEventTemplate<Event_Int2Type<52>, FeatureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass> FeatureChangedEvent;
typedef FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator> HexEdgeVector;
typedef FFastVector<FGXVector2, true, 51u, 0u, FTempHeapFastVectorAllocator> PointVector;
typedef FFastVector<float, true, 51u, 0u, FTempHeapFastVectorAllocator> FloatVector;

// A serial game-core event is a 4-byte header followed by its payload.
struct FeatureChangedMessage {
    unsigned int m_uHeader;
    FeatureChangedData m_kData;
};

// The same hex side named from the hex on its other side.
const unsigned char kOppositeEdge[12] = {9, 10, 11, 6, 7, 8, 3, 4, 5, 0, 1, 2};
// Rotation of the bank pattern per side, in sixths of a turn.
const float kSideRotation[6] = {1.0f, 3.0f, 5.0f, 4.0f, 0.0f, 2.0f};
const float kSixthTurn = 1.0471667f;
// Bank width in texels for a run of 1..5 sides around one hex.
const float kRunWidth[5] = {35.0f, 30.0f, 25.0f, 20.0f, 15.0f};
// Spline parameter step and the texel-to-u scale of the bank strip.
const float kSplineStep = 0.25f;
const float kTexelToU = 0.00390625f;
// Six triangles fanning around the centre vertex (3) of each quad, relative to the quad's first vertex.
const int kStripIndexOffsets[18] = {-4, 3, -3, -3, 3, -2, -4, 0, 3, -2, 3, 2, 3, 0, 1, 3, 1, 2};

// The helpers below are inlined in the binary.
const auto EdgeAcross = [](HexEdge const& kEdge) -> HexEdge {
    HEXVec_i const& kStep = HEXVec_i::TRANSLATIONS[kEdge.m_iEdge % 6];
    HexEdge kAcross;
    kAcross.m_kHex.x = kEdge.m_kHex.x + kStep.x;
    kAcross.m_kHex.y = kEdge.m_kHex.y + kStep.y;
    kAcross.m_kHex.z = kEdge.m_kHex.z + kStep.z;
    kAcross.m_iEdge = kOppositeEdge[kEdge.m_iEdge];
    return kAcross;
};

const auto SameEdge = [](HexEdge const& kA, HexEdge const& kB) -> bool {
    return kA.m_kHex.x == kB.m_kHex.x && kA.m_kHex.y == kB.m_kHex.y && kA.m_kHex.z == kB.m_kHex.z && kA.m_iEdge == kB.m_iEdge;
};

// True when the side is in the list under either of its two names.
const auto IsVisited = [](HexEdgeVector const& kVisited, HexEdge const& kEdge) -> bool {
    const HexEdge kAcross = EdgeAcross(kEdge);
    for (unsigned int i = 0; i < kVisited.size(); ++i) {
        if (SameEdge(kVisited[i], kEdge) || SameEdge(kVisited[i], kAcross)) {
            return true;
        }
    }
    return false;
};

const auto EvalPoint = [](unsigned int uiCount, FGXVector2* pPoints, float fT) -> FGXVector2 {
    return B_SPLINE::EvalScalingFunc<FGXVector2, FGXVector2*, static_cast<B_SPLINE::BSPLINE_BEGIN_TYPES>(0), static_cast<B_SPLINE::BSPLINE_END_TYPES>(0)>(uiCount, pPoints, fT, 0);
};

const auto EvalWidth = [](unsigned int uiCount, float* pWidths, float fT) -> float {
    return B_SPLINE::EvalScalingFunc<float, float*, static_cast<B_SPLINE::BSPLINE_BEGIN_TYPES>(0), static_cast<B_SPLINE::BSPLINE_END_TYPES>(0)>(uiCount, pWidths, fT, 0);
};

// The point fDistance from kFrom in the direction of kToward.
const auto ExtrudeToward = [](FGXVector2 const& kFrom, FGXVector2 const& kToward, float fDistance) -> FGXVector2 {
    D3DXVECTOR2 kDir = {kToward.x - kFrom.x, kToward.y - kFrom.y};
    D3DXVec2Normalize(&kDir, &kDir);
    FGXVector2 kResult;
    kResult.x = kDir.x * fDistance + kFrom.x;
    kResult.y = fDistance * kDir.y + kFrom.y;
    return kResult;
};

const auto Distance = [](FGXVector2 const& kA, FGXVector2 const& kB) -> float {
    const float fDX = kA.x - kB.x;
    const float fDY = kA.y - kB.y;
    return sqrtf(fDY * fDY + fDX * fDX);
};

const auto GetRiverSim = []() -> cvRiverSimSystem* {
    return reinterpret_cast<cvRiverSimSystem*>(g_TerrainSystem.unk_50c);
};

}

/// 0x08661b2a
cvFloodplainsSystem::cvFloodplainsSystem() {
    field_0 = 4;
    const FGXInt2 kGridSize = {200, 200};
    Fun_08661752(&m_kGrid, kGridSize, HEXTransform_i::IDENTITY_TRANSFORM);
    m_bFloodplainListsReady = false;
    m_bRiverBankListsReady = false;
}

/// 0x08661b88
cvFloodplainsSystem::~cvFloodplainsSystem() {
}

/// 0x08661b8a
UNK_RET cvFloodplainsSystem::Startup() {
    m_kGrid.m_mxForward = m_kGrid.m_kTransform.matrix();
    m_kGrid.m_mxInverse = m_kGrid.m_kTransform.inv_matrix();
    SubscribeToEvents();
}

/// 0x08661c26
UNK_RET cvFloodplainsSystem::LoadFloodplainsDepthTexture() {
}

/// 0x08661c28
UNK_RET cvFloodplainsSystem::SubscribeToEvents() {
    TerrainEventDelegate kDelegate;
    kDelegate.m_pObject = this;
    UNK_RET (cvFloodplainsSystem::*pfnOnFloodplainAdded)(FeatureChangedEvent&) = &cvFloodplainsSystem::OnFloodplainAdded;
    memcpy(&kDelegate.m_pfnMethod, &pfnOnFloodplainAdded, sizeof(pfnOnFloodplainAdded));
    Fun_086655d2(&kDelegate);
}

/// 0x08661c64
UNK_RET cvFloodplainsSystem::Shutdown() {
    UnsubscribeToEvents();
}

/// 0x08661ca0
UNK_RET cvFloodplainsSystem::UnsubscribeToEvents() {
    TerrainEventDelegate kDelegate;
    kDelegate.m_pObject = this;
    UNK_RET (cvFloodplainsSystem::*pfnOnFloodplainAdded)(FeatureChangedEvent&) = &cvFloodplainsSystem::OnFloodplainAdded;
    memcpy(&kDelegate.m_pfnMethod, &pfnOnFloodplainAdded, sizeof(pfnOnFloodplainAdded));
    Fun_0866535c(&kDelegate);
}

/// 0x08661cdc
UNK_RET cvFloodplainsSystem::StartupGame() {
}

/// 0x08661cde
UNK_RET cvFloodplainsSystem::ShutdownGame() {
    m_bFloodplainListsReady = false;
}

// Flood plains first, from the cells marked by OnFloodplainAdded; then river banks along every river side.
/// 0x08661cea
UNK_RET cvFloodplainsSystem::BuildFloodplains(FFastVector<FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator> const& kRivers) {
    FFastVector<cvFloodplainsSim, false, 51u, 0u, FTempHeapFastVectorAllocator> kSims;
    kSims.reserve(2500);
    HexEdgeVector kVisited;
    kVisited.reserve(5000);

    BuildTileLists(kSims, kVisited, RIVERBANK_FLAG_FLOODPLAIN);
    BuildBankVis(kSims, RIVERBANK_FLAG_FLOODPLAIN);
    kSims.clear();
    kVisited.clear();

    AddAllRiverBanks(kRivers);
    BuildTileLists(kSims, kVisited, RIVERBANK_FLAG_RIVER);
    BuildBankVis(kSims, RIVERBANK_FLAG_RIVER);
}

/// 0x08661e2c
UNK_RET cvFloodplainsSystem::BuildTileLists(FFastVector<cvFloodplainsSim, false, 51u, 0u, FTempHeapFastVectorAllocator>& kSims, FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator>& kVisited, cvFloodplainsSystem::RIVERBANK_FLAG eFlag) {
    cvRiverSimSystem* pRiverSim = GetRiverSim();
    const int iWidth = m_kGrid.m_iWidth;
    const int iHeight = m_kGrid.m_iHeight;
    for (int iColumn = 0; iColumn < iWidth; ++iColumn) {
        for (int iRow = 0; iRow < iHeight; ++iRow) {
            if ((m_auBankFlags[iRow][iColumn] & eFlag) == 0) {
                continue;
            }
            const int iOffsetX = iColumn - (iRow >> 1);
            const HEXVec_i kOffset = {iOffsetX, iRow, -(iOffsetX + iRow)};
            const HEXVec_i kGridHex = Fun_086616f0(m_kGrid.m_mxInverse, kOffset);
            HexEdge kEdge;
            kEdge.m_kHex.x = kGridHex.x;
            kEdge.m_kHex.y = kGridHex.y;
            kEdge.m_kHex.z = -(kGridHex.y + kGridHex.x);

            // Sides 3..5 and 9..11 belong to the neighbours.
            for (int iSide = 0; iSide < 12; ++iSide) {
                kEdge.m_iEdge = iSide;
                if (iSide % 6 > 2) {
                    continue;
                }
                if (!pRiverSim->IsRiver(kEdge)) {
                    continue;
                }
                if (IsVisited(kVisited, kEdge)) {
                    continue;
                }
                HexEdge kSource = {};
                if (pRiverSim->TraverseToRiverSource(kEdge, kSource, kVisited)) {
                    AddBankAlongRiver(kSource, kVisited, kSims, eFlag);
                }
            }
        }
    }

    if (eFlag == RIVERBANK_FLAG_RIVER) {
        m_bRiverBankListsReady = true;
    } else if (eFlag == RIVERBANK_FLAG_FLOODPLAIN) {
        m_bFloodplainListsReady = true;
    }
}

// Each sim becomes one strip of quads from the river side to the extruded bank, split into a start, middle and end decal.
/// 0x0866217c
UNK_RET cvFloodplainsSystem::BuildBankVis(FFastVector<cvFloodplainsSim, false, 51u, 0u, FTempHeapFastVectorAllocator> const& kSims, cvFloodplainsSystem::RIVERBANK_FLAG eFlag) {
    if (eFlag == RIVERBANK_FLAG_FLOODPLAIN) {
        if (!m_bFloodplainListsReady) {
            return;
        }
    } else if (eFlag == RIVERBANK_FLAG_RIVER && !m_bRiverBankListsReady) {
        return;
    }
    const bool bRiverBank = eFlag == RIVERBANK_FLAG_RIVER;

    FFastVector<DecalVertex, true, 43u, 260u, FTempHeapFastVectorAllocator> kVertices;
    FFastVector<unsigned short, true, 43u, 260u, FTempHeapFastVectorAllocator> kIndices;
    kVertices.reserve(10000);
    kIndices.reserve(10000);
    PointVector kRiverPoints;
    PointVector kBankPoints;
    FloatVector kBankWidths;
    kRiverPoints.reserve(5000);
    kBankPoints.reserve(5000);
    kBankWidths.reserve(5000);
    PointVector kRawBankPoints;
    FloatVector kRawBankWidths;
    kRawBankPoints.reserve(5000);
    kRawBankWidths.reserve(5000);

    // Decal categories of the start, middle and end of a strip.
    const unsigned int auiFloodplainCategories[3] = {5, 4, 6};
    const unsigned int auiRiverBankCategories[3] = {8, 7, 9};

    GameSplineDatabase* pSplines = reinterpret_cast<GameSplineDatabase*>(g_GameSplineDatabase);
    const unsigned int uiNumSims = kSims.size();
    for (unsigned int uiSim = 0; uiSim < uiNumSims; ++uiSim) {
        cvFloodplainsSim const& kSim = kSims[uiSim];
        const unsigned int uiNumEdges = kSim.m_kEdges.size();
        kVertices.clear();
        kIndices.clear();
        kBankWidths.clear();
        kBankPoints.clear();
        kRiverPoints.clear();

        GameSpline::Segment kSegment;
        for (unsigned int uiEdge = 0; uiEdge < uiNumEdges; ++uiEdge) {
            kSegment.clear();
            pSplines->QueryHexSegment(kSim.m_kEdges[uiEdge], &kSegment);
            for (unsigned int i = 0; i < kSegment.size(); ++i) {
                kRiverPoints.push_back(kSegment[i].m_vPos);
            }
        }
        if (kRiverPoints.size() < 2) {
            continue;
        }

        ExtrudeControlPoints(kBankPoints, kBankWidths, kSim, kRawBankPoints, kRawBankWidths);
        AddEndcaps<GameSpline::ControlPoint>(kBankPoints, kRawBankPoints);
        AddEndcaps<GameSpline::ControlPoint>(kRiverPoints, kRawBankPoints);
        AddEndcaps<float>(kBankWidths, kRawBankWidths);

        FGXVector2* pRiver = kRiverPoints.begin();
        const unsigned int uiNumPoints = kRiverPoints.size();
        const float fLastT = static_cast<float>(uiNumPoints) - 2.0f;
        const float fEndT = fLastT + 0.001f;
        FGXVector2* pBank = kBankPoints.begin();
        float* pWidths = kBankWidths.begin();

        // First pass: the mean length of the river side and the bank side, in u.
        FGXVector2 kPrevRiver = EvalPoint(uiNumPoints, pRiver, 0.0f);
        FGXVector2 kPrevBank = EvalPoint(uiNumPoints, pBank, 0.0f);
        float fRiverLength = 0.0f;
        float fBankLength = 0.0f;
        for (float fT = 0.0f; fEndT > fT;) {
            const float fClampedT = fLastT < fT ? fLastT : fT;
            const FGXVector2 kRiver = EvalPoint(uiNumPoints, pRiver, fClampedT);
            const FGXVector2 kToward = EvalPoint(uiNumPoints, pBank, fClampedT);
            const float fWidth = EvalWidth(uiNumPoints, pWidths, fClampedT);
            const FGXVector2 kBank = ExtrudeToward(kRiver, kToward, fWidth);
            fRiverLength += Distance(kRiver, kPrevRiver);
            fBankLength += Distance(kBank, kPrevBank);
            kPrevRiver = kRiver;
            kPrevBank = kBank;
            fT = fClampedT + kSplineStep;
        }
        const float fEndU = (fRiverLength * kTexelToU + fBankLength * kTexelToU) * 0.5f - 0.1f;

        // Second pass: four vertices per sample (river, middle, bank, quad centre), six triangles per quad.
        kPrevRiver = EvalPoint(uiNumPoints, pRiver, 0.0f);
        kPrevBank = EvalPoint(uiNumPoints, pBank, 0.0f);
        float fRiverU = 0.0f;
        float fBankU = 0.0f;
        float fPrevU = 0.0f;
        unsigned int uiStartEnd = 0;
        unsigned int uiTailStart = 0;
        for (float fT = 0.0f; fEndT > fT;) {
            const float fClampedT = fLastT < fT ? fLastT : fT;
            const FGXVector2 kRiver = EvalPoint(uiNumPoints, pRiver, fClampedT);
            const FGXVector2 kToward = EvalPoint(uiNumPoints, pBank, fClampedT);
            const float fWidth = EvalWidth(uiNumPoints, pWidths, fClampedT);
            const FGXVector2 kBank = ExtrudeToward(kRiver, kToward, fWidth);
            fRiverU += Distance(kRiver, kPrevRiver) * kTexelToU;
            fBankU += kTexelToU * Distance(kBank, kPrevBank);
            const float fU = fRiverU * 0.5f + fBankU * 0.5f;

            const DecalVertex kRiverVertex = {kRiver.x, kRiver.y, fU, 0.98f};
            const DecalVertex kMiddleVertex = {kRiver.x * 0.5f + kBank.x * 0.5f, kRiver.y * 0.5f + kBank.y * 0.5f, fU, 0.49f};
            const DecalVertex kBankVertex = {kBank.x, kBank.y, fU, 0.0f};
            const DecalVertex kCentreVertex = {
                ((kPrevBank.x * 0.25f + kBank.x * 0.25f) + kRiver.x * 0.25f) + kPrevRiver.x * 0.25f,
                ((kPrevBank.y * 0.25f + kBank.y * 0.25f) + kRiver.y * 0.25f) + kPrevRiver.y * 0.25f,
                fPrevU * 0.5f + fU * 0.5f,
                0.48f,
            };
            const unsigned int uiFirstVertex = kVertices.size();
            kVertices.push_back(kRiverVertex);
            kVertices.push_back(kMiddleVertex);
            kVertices.push_back(kBankVertex);
            kVertices.push_back(kCentreVertex);

            const float fNextT = fClampedT + kSplineStep;
            // The first sample only opens the strip.
            if (fNextT >= 0.2501f) {
                const unsigned int uiNumIndices = kIndices.size();
                if (uiStartEnd == 0 && fPrevU > 0.1f) {
                    uiStartEnd = uiNumIndices;
                }
                const float fPrevU8 = 8.0f * fPrevU;
                if (uiTailStart == 0 && fPrevU > fEndU && fabsf(fPrevU8 - static_cast<float>(static_cast<int>(fPrevU8))) < 0.5f) {
                    uiTailStart = uiNumIndices;
                }
                for (int i = 0; i < 18; ++i) {
                    kIndices.push_back(static_cast<unsigned short>(uiFirstVertex + kStripIndexOffsets[i]));
                }
            }
            kPrevRiver = kRiver;
            kPrevBank = kBank;
            fPrevU = fU;
            fT = fNextT;
        }

        const unsigned int uiNumIndices = kIndices.size();
        if (uiTailStart == 0) {
            uiTailStart = uiNumIndices;
        }
        if (uiStartEnd > uiTailStart) {
            uiTailStart = uiStartEnd;
        }
        if (uiNumIndices != 0 && (eFlag == RIVERBANK_FLAG_FLOODPLAIN || bRiverBank)) {
            const unsigned int* puiCategories = eFlag == RIVERBANK_FLAG_FLOODPLAIN ? auiFloodplainCategories : auiRiverBankCategories;
            const unsigned int auiSplit[4] = {0, uiStartEnd, uiTailStart, uiNumIndices};
            for (int iPart = 0; iPart < 3; ++iPart) {
                const unsigned int uiPartIndices = auiSplit[iPart + 1] - auiSplit[iPart];
                if (uiPartIndices != 0) {
                    g_TerrainSystem.m_DecalLayer.CreateDecal(puiCategories[iPart], kVertices.begin(), kVertices.size(), kIndices.begin() + auiSplit[iPart], uiPartIndices / 3, false);
                }
            }
        }
        kIndices.clear();
        kVertices.clear();
    }
}

/// 0x0866343c
UNK_RET cvFloodplainsSystem::AddAllRiverBanks(FFastVector<FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator> const& kRivers) {
    const unsigned int uiNumRivers = kRivers.size();
    for (unsigned int uiRiver = 0; uiRiver < uiNumRivers; ++uiRiver) {
        const unsigned int uiNumEdges = kRivers[uiRiver].size();
        for (unsigned int uiEdge = 0; uiEdge < uiNumEdges; ++uiEdge) {
            const HexEdge kEdge = kRivers[uiRiver][uiEdge];
            AddRiverBank(kEdge.m_kHex);
            const HexEdge kAcross = EdgeAcross(kEdge);
            AddRiverBank(kAcross.m_kHex);
        }
    }
}

/// 0x08663556
UNK_RET cvFloodplainsSystem::Update(float fDeltaTime) {
}

/// 0x08663558
UNK_RET cvFloodplainsSystem::OnFloodplainAdded(GameCoreEventTemplate<Event_Int2Type<52>, FeatureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    // Any feature change marks the hex; the feature type in the payload is not read.
    HEXVec_i const& kHex = reinterpret_cast<FeatureChangedMessage const&>(kEvent).m_kData.m_kHex;
    if (Fun_08664aae(m_kGrid, kHex)) {
        const HEXVec_i kOffset = Fun_086616f0(m_kGrid.m_mxForward, kHex);
        m_auBankFlags[kOffset.y][kOffset.x + (static_cast<unsigned int>(kOffset.y) >> 1)] |= RIVERBANK_FLAG_FLOODPLAIN;
    }
    m_bFloodplainListsReady = false;
}

/// 0x086635e8
UNK_RET cvFloodplainsSystem::SetBankFlag(HEXVec_i kHex, cvFloodplainsSystem::RIVERBANK_FLAG eFlag, bool bSet) {
    const HEXVec_i kOffset = Fun_086616f0(m_kGrid.m_mxForward, kHex);
    unsigned int& uiCell = m_auBankFlags[kOffset.y][kOffset.x + (kOffset.y >> 1)];
    if (bSet) {
        uiCell |= eFlag;
    } else {
        // The binary ANDs with !eFlag, so clearing any flag empties the cell.
        uiCell &= static_cast<unsigned int>(eFlag == 0);
    }
}

/// 0x0866367a
UNK_RET cvFloodplainsSystem::AddRiverBank(HEXVec_i const& kHex) {
    if (Fun_08664aae(m_kGrid, kHex)) {
        const HEXVec_i kOffset = Fun_086616f0(m_kGrid.m_mxForward, kHex);
        m_auBankFlags[kOffset.y][kOffset.x + (static_cast<unsigned int>(kOffset.y) >> 1)] |= RIVERBANK_FLAG_RIVER;
    }
    m_bRiverBankListsReady = false;
}

// Walks the river downstream from kStart; consecutive sides with a marked hex on one bank form one sim per bank.
/// 0x08663704
UNK_RET cvFloodplainsSystem::AddBankAlongRiver(HexEdge const& kStart, FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator>& kVisited, FFastVector<cvFloodplainsSim, false, 51u, 0u, FTempHeapFastVectorAllocator>& kSims, cvFloodplainsSystem::RIVERBANK_FLAG eFlag) {
    cvFloodplainsSim kLeft;
    kLeft.m_eSide = cvFloodplainsSim::FLOODPLAINS_RIVER_SIDE_LEFT;
    cvFloodplainsSim kRight;
    kRight.m_eSide = cvFloodplainsSim::FLOODPLAINS_RIVER_SIDE_RIGHT;
    cvRiverSimSystem* pRiverSim = GetRiverSim();

    HexEdge kEdge = kStart;
    do {
        if (kEdge.m_iEdge % 6 >= 3) {
            kEdge = EdgeAcross(kEdge);
        }
        if (IsVisited(kVisited, kEdge)) {
            break;
        }
        const cvFloodplainsSim::FloodplainsRiverSide eLeft = cvFloodplainsSim::FLOODPLAINS_RIVER_SIDE_LEFT;
        const bool bLeftBank = CheckForBank(kEdge, eLeft, eFlag);
        const cvFloodplainsSim::FloodplainsRiverSide eRight = cvFloodplainsSim::FLOODPLAINS_RIVER_SIDE_RIGHT;
        const bool bRightBank = CheckForBank(kEdge, eRight, eFlag);

        if (bLeftBank) {
            kLeft.m_kEdges.push_back(kEdge);
        } else if (kLeft.m_kEdges.size() != 0) {
            kSims.push_back(kLeft);
            kLeft.m_kEdges.clear();
        }
        if (bRightBank) {
            kRight.m_kEdges.push_back(kEdge);
        } else if (kRight.m_kEdges.size() != 0) {
            kSims.push_back(kRight);
            kRight.m_kEdges.clear();
        }
        kVisited.push_back(kEdge);
    } while (pRiverSim->GetNextEdge(kEdge, kEdge));

    if (kLeft.m_kEdges.size() != 0) {
        kSims.push_back(kLeft);
    }
    if (kRight.m_kEdges.size() != 0) {
        kSims.push_back(kRight);
    }
}

// The bank hex of a side: the hex the side is named from, or its neighbour across, by the side's direction.
/// 0x08663b40
bool cvFloodplainsSystem::CheckForBank(HexEdge kEdge, cvFloodplainsSim::FloodplainsRiverSide const& eSide, cvFloodplainsSystem::RIVERBANK_FLAG eFlag) const {
    const int iDirection = kEdge.m_iEdge % 12;
    const bool bAcross = eSide == cvFloodplainsSim::FLOODPLAINS_RIVER_SIDE_RIGHT ? iDirection >= 6 : iDirection <= 5;
    if (bAcross) {
        kEdge = EdgeAcross(kEdge);
    }
    if (!Fun_08664aae(m_kGrid, kEdge.m_kHex)) {
        return false;
    }
    const HEXVec_i kOffset = Fun_086616f0(m_kGrid.m_mxForward, kEdge.m_kHex);
    return (m_auBankFlags[kOffset.y][kOffset.x + (static_cast<unsigned int>(kOffset.y) >> 1)] & eFlag) != 0;
}

/// 0x08663c4e
bool cvFloodplainsSystem::TestBankFlag(HEXVec_i kHex, cvFloodplainsSystem::RIVERBANK_FLAG eFlag) const {
    const HEXVec_i kOffset = Fun_086616f0(m_kGrid.m_mxForward, kHex);
    return (m_auBankFlags[kOffset.y][kOffset.x + (static_cast<unsigned int>(kOffset.y) >> 1)] & eFlag) != 0;
}

/// 0x08663cc6
bool cvFloodplainsSystem::TestBankFlag(FGXInt2 kCell, cvFloodplainsSystem::RIVERBANK_FLAG eFlag) const {
    return (m_auBankFlags[kCell.y][kCell.x] & eFlag) != 0;
}

// Places a fixed bank pattern on each run of sides around one hex, then corner-cuts the outline (Chaikin, 1/4 and 3/4).
/// 0x08663ce8
UNK_RET cvFloodplainsSystem::ExtrudeControlPoints(FFastVector<FGXVector2, true, 51u, 0u, FTempHeapFastVectorAllocator>& kPoints, FFastVector<float, true, 51u, 0u, FTempHeapFastVectorAllocator>& kWidths, cvFloodplainsSim const& kSim, FFastVector<FGXVector2, true, 51u, 0u, FTempHeapFastVectorAllocator>& kRawPoints, FFastVector<float, true, 51u, 0u, FTempHeapFastVectorAllocator>& kRawWidths) const {
    kRawPoints.clear();
    kRawWidths.clear();

    // Left-bank outline in hex space for a run of 1..5 sides; unused points stay zero.
    const D3DXVECTOR4 aaLeftPattern[5][6] = {
        {{0.5f, -0.5f, 0.0f, 1.0f}, {0.0f, 0.5f, -0.5f, 1.0f}},
        {{0.5f, -0.5f, 0.0f, 1.0f}, {0.099999994f, 0.099999994f, -0.19999999f, 1.0f}, {-0.5f, 0.5f, 0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f, 1.0f}, {0.15416667f, -0.0083333552f, -0.14583331f, 1.0f}, {-0.15416667f, 0.14583328f, 0.008333385f, 1.0f}, {-0.5f, 0.0f, 0.5f, 1.0f}},
        {{0.5f, -0.5f, 0.0f, 1.0f}, {0.15416667f, -0.0083333552f, -0.14583331f, 1.0f}, {-0.099999994f, 0.19999999f, -0.099999994f, 1.0f}, {-0.14583328f, -0.0083333552f, 0.15416664f, 1.0f}, {0.0f, -0.5f, 0.5f, 1.0f}},
        {{0.5f, -0.5f, 0.0f, 1.0f}, {0.15416667f, -0.0083333552f, -0.14583331f, 1.0f}, {-0.099999994f, 0.19999999f, -0.099999994f, 1.0f}, {-0.19999999f, 0.099999994f, 0.099999994f, 1.0f}, {0.0083333552f, -0.15416667f, 0.14583331f, 1.0f}, {0.5f, -0.5f, 0.0f, 1.0f}},
    };
    // The right bank mirrors the left with reflection 6.
    D3DXVECTOR4 aaRightPattern[5][6];
    const D3DXMATRIX* pMirror = reinterpret_cast<const D3DXMATRIX*>(HEXMatrix::REFLECTIONS) + 6;
    for (int iRun = 0; iRun < 5; ++iRun) {
        for (int iPoint = 0; iPoint < 6; ++iPoint) {
            D3DXVec4Transform(&aaRightPattern[iRun][iPoint], &aaLeftPattern[iRun][iPoint], pMirror);
        }
    }

    const unsigned int uiNumEdges = kSim.m_kEdges.size();
    unsigned int uiRun = 0;
    for (unsigned int uiEdge = 0; uiEdge < uiNumEdges; uiEdge += uiRun) {
        HexEdge kEdge = kSim.m_kEdges[uiEdge];
        if (kEdge.m_iEdge % 12 <= 5) {
            kEdge = EdgeAcross(kEdge);
        }
        const float fAngle = kSideRotation[kEdge.m_iEdge % 6] * kSixthTurn;
        uiRun = CountInternalEdges(kSim, uiEdge);
        // Later runs share their first point with the previous run's last.
        const unsigned int uiFirstPoint = uiEdge != 0 ? 1 : 0;
        const float fHexX = static_cast<float>(kEdge.m_kHex.x);
        const float fHexY = static_cast<float>(kEdge.m_kHex.y);
        const float fHexZ = static_cast<float>(kEdge.m_kHex.z);
        for (unsigned int uiPoint = uiFirstPoint; uiPoint < uiRun + 1; ++uiPoint) {
            HEXVec kSideOffset = {0.0f, 0.0f, 0.0f, 1.0f};
            if (kSim.m_eSide == cvFloodplainsSim::FLOODPLAINS_RIVER_SIDE_RIGHT) {
                kSideOffset.x = static_cast<float>(HEXVec_i::TRANSLATIONS[4].x);
                kSideOffset.y = static_cast<float>(HEXVec_i::TRANSLATIONS[4].y);
                kSideOffset.z = static_cast<float>(HEXVec_i::TRANSLATIONS[4].z);
            }
            const HEXVec kHexPosition = {fHexX, fHexY, fHexZ, 1.0f};
            D3DXMATRIX kHexTranslation;
            HEXMatTranslation(reinterpret_cast<HEXMatrix*>(&kHexTranslation), kHexPosition);
            D3DXMATRIX kSideTranslation;
            HEXMatTranslation(reinterpret_cast<HEXMatrix*>(&kSideTranslation), kSideOffset);
            D3DXMATRIX kRotation;
            HEXMatRotate(reinterpret_cast<HEXMatrix*>(&kRotation), fAngle);
            D3DXMATRIX kRotated;
            D3DXMatrixMultiply(&kRotated, &kSideTranslation, &kRotation);
            D3DXMATRIX kPlaced;
            D3DXMatrixMultiply(&kPlaced, &kRotated, &kHexTranslation);
            D3DXMATRIX kToTexture;
            D3DXMatrixMultiply(&kToTexture, &kPlaced, reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_HexToTex));

            D3DXVECTOR4 kTexel;
            if (kSim.m_eSide == cvFloodplainsSim::FLOODPLAINS_RIVER_SIDE_LEFT) {
                D3DXVec4Transform(&kTexel, &aaLeftPattern[uiRun - 1][uiPoint], &kToTexture);
            } else {
                D3DXVec4Transform(&kTexel, &aaRightPattern[uiRun - 1][uiPoint], &kToTexture);
            }
            FGXVector2 kPoint;
            kPoint.x = kTexel.x;
            kPoint.y = kTexel.y;
            kRawPoints.push_back(kPoint);
            kRawWidths.push_back(kRunWidth[uiRun - 1]);
        }
    }

    const unsigned int uiNumRaw = kRawPoints.size();
    for (unsigned int i = 1; i != uiNumRaw; ++i) {
        FGXVector2 kNear;
        kNear.x = kRawPoints[i].x * 0.25f + kRawPoints[i - 1].x * 0.75f;
        kNear.y = kRawPoints[i].y * 0.25f + kRawPoints[i - 1].y * 0.75f;
        kPoints.push_back(kNear);
        FGXVector2 kFar;
        kFar.x = kRawPoints[i].x * 0.75f + kRawPoints[i - 1].x * 0.25f;
        kFar.y = kRawPoints[i].y * 0.75f + kRawPoints[i - 1].y * 0.25f;
        kPoints.push_back(kFar);
        kWidths.push_back(kRawWidths[i] * 0.25f + kRawWidths[i - 1] * 0.75f);
        kWidths.push_back(kRawWidths[i] * 0.75f + kRawWidths[i - 1] * 0.25f);
    }
}

// Counts while the next side on the sim's bank is the next listed side.
/// 0x08664806
unsigned int cvFloodplainsSystem::CountInternalEdges(cvFloodplainsSim const& kSim, unsigned int uiStart) const {
    cvRiverSimSystem const* pRiverSim = GetRiverSim();
    HexEdge kNext = {};
    const unsigned int uiLast = kSim.m_kEdges.size() - 1;
    if (uiStart >= uiLast) {
        return 1;
    }
    unsigned int uiEdge = uiStart;
    do {
        bool bFound;
        if (kSim.m_eSide == cvFloodplainsSim::FLOODPLAINS_RIVER_SIDE_LEFT) {
            bFound = pRiverSim->GetNextLeftEdge(kSim.m_kEdges[uiEdge], kNext);
        } else {
            bFound = pRiverSim->GetNextRightEdge(kSim.m_kEdges[uiEdge], kNext);
        }
        if (!bFound) {
            break;
        }
        HexEdge const& kFollowing = kSim.m_kEdges[uiEdge + 1];
        if (!SameEdge(kFollowing, kNext) && !SameEdge(kFollowing, EdgeAcross(kNext))) {
            break;
        }
        ++uiEdge;
    } while (uiEdge < uiLast);
    return uiEdge - uiStart + 1;
}
