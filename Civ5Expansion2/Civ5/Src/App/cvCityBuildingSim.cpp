/// origin guess
#include "Civ5/Src/App/cvCityBuildingSim.h"
#include "Civ5/Src/App/EventTemplate.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/cvGeneralRefHandle.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "Civ5/Src/App/HEXMatrix.h"
#include "Civ5/Src/App/HEXTransform_i.h"
#include "Civ5/Src/App/HEXVec.h"
#include "Civ5/Src/App/TerrainStencil.h"
#include "Civ5/Src/App/cvCityVisSystem.h"
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include <algorithm>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

void srand_wincompatible(unsigned int uiSeed);

namespace {

typedef cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> CityVisHandle;

const unsigned int kNoCity = 0x7fff;
const unsigned int kPolygonEnd = 0x0fffffffu;
// The wall sample lattice is 64x64 with a step of 2 world units, starting at -64 from the city.
const int kLatticeSize = 64;
const float kLatticeOrigin = -64.0f;
const float kMaxSampleRadius = 40.0f;
const float kMinHullArea = 200.0f;
const float kHexCornerScale = 0.6f;
const float kWallOutwardOffset = 2.0f;
const float kTowerSpacing = 12.0f;
const float kWallScale = 0.05f;
const unsigned char kStencilWaterBit = 8;
// A wall piece holds at most six control points and repeats the last two points of the previous piece.
const unsigned int kMaxSegmentPoints = 6;
const unsigned int kMinSegmentPoints = 4;
const unsigned int kSegmentOverlap = 2;
// Cube coordinate offsets of the six corners of a hex; the table at 0x0a4cc1f8 is filled at static init.
const float kHexCornerOffsets[6][2] = {
    {2.0f / 3.0f, -1.0f / 3.0f}, {1.0f / 3.0f, 1.0f / 3.0f},  {-1.0f / 3.0f, 2.0f / 3.0f},
    {-2.0f / 3.0f, 1.0f / 3.0f}, {-1.0f / 3.0f, -1.0f / 3.0f}, {1.0f / 3.0f, -2.0f / 3.0f},
};

const unsigned int kBuildingListEnd = 0x0fffffffu;
const unsigned int kInCitySlotCount = 4;
const unsigned int kBurningBuildingCount = 8;
const unsigned int kDamageRankThresholds[4] = {3, 4, 6, 8};
// Houses stand on land only: raw height in [0x32c9, 0x32c9 + 22000).
const unsigned short kLandHeightBias = 0xcd37;
const unsigned int kLandHeightRange = 22000;
const float kMinNormalZ = 0.85f;
const float kHexRowStretch = 1.1547344f;
const float kHexRowScale = 0.8660254f;
const unsigned char kBuildingClassNone = 4;
// Neighbour directions (indices into HEXVec_i::TRANSLATIONS) that span the triangle of each hex corner.
const unsigned int kCornerNeighbours[2][2] = {{0, 5}, {3, 2}};

}  // namespace

/// 0x0a4cc088
WONDER::HexCorner cvCityBuildingSim::ms_aWonderCornerCandidates[0xc] = {
    {{0, 0, 0}, 0, {}},   {{-1, 1, 0}, 1, {}},  {{0, -1, 1}, 0, {}},  {{0, 0, 0}, 1, {}},
    {{1, -1, 0}, 0, {}},  {{0, 1, -1}, 1, {}},  {{-1, 2, -1}, 1, {}}, {{-1, 0, 1}, 0, {}},
    {{-1, 0, 1}, 1, {}},  {{1, -2, 1}, 0, {}},  {{1, 0, -1}, 1, {}},  {{1, 0, -1}, 0, {}},
};

/// 0x087b892a
int cvCityBuildingSim::FindCityBuildingPlacementSlot(unsigned int cityIdx, bool allowRandom) {
    const CITY::cvCityData& kCity = m_pCities[cityIdx];
    for (unsigned int uiSlot = 0; uiSlot < kInCitySlotCount; uiSlot++) {
        if (kCity.m_anInCitySlots[uiSlot] == 0xffffffffu) return (int)uiSlot;
    }
    if (!allowRandom) return -1;
    return (int)rand_wincompatible() % (int)kInCitySlotCount;
}

/// 0x087b8988
int cvCityBuildingSim::FindWonderPlacementSlot(unsigned char kind, HEXVec_i hex, WONDER::HexCorner* out, bool allowShared) {
    WONDER::HexCorner aCandidates[0xc] = {};
    unsigned int uiCount;
    unsigned int uiFlagMask;
    if (kind == 2) {
        for (unsigned int i = 0; i < 6; i++) aCandidates[i] = ms_aWonderCornerCandidates[i];
        uiFlagMask = 0x21;
        uiCount = 6;
    } else if (kind == 4) {
        for (unsigned int i = 0; i < 6; i++) aCandidates[i] = ms_aWonderCornerCandidates[6 + i];
        uiFlagMask = 0x21;
        uiCount = 6;
    } else if (kind == 8 || kind == 16) {
        for (unsigned int i = 0; i < 0xc; i++) aCandidates[i] = ms_aWonderCornerCandidates[i];
        uiFlagMask = (kind == 8) ? 0x22 : 0x20;
        uiCount = 12;
    } else {
        return 0;
    }

    // Seeded per city hex so the shuffle is reproducible.
    srand_wincompatible((unsigned int)(hex.y * 0xff + hex.x));
    std::random_shuffle(aCandidates, aCandidates + uiCount);

    // Pass 0 (kinds 2 and 4 only) avoids corners with the 0x10 edge flag, pass 1 takes any free corner, pass 2 shares a used one.
    for (unsigned int uiPass = 0; uiPass < 3; uiPass++) {
        if (uiPass == 0 && kind != 2 && kind != 4) continue;
        if (uiPass == 2 && !allowShared) break;
        for (unsigned int i = 0; i < uiCount; i++) {
            const HEXVec_i kHex = {aCandidates[i].m_Hex.x + hex.x, aCandidates[i].m_Hex.y + hex.y, aCandidates[i].m_Hex.z + hex.z};
            const unsigned char ucCorner = aCandidates[i].m_nCorner;
            WONDER::HexCornerCell* pCell = Fun_087ba112(m_aHexCornerGrid, kHex);
            const unsigned int uiUsedBits = pCell->m_nUsedCornerBits;
            const unsigned char ucFlags = pCell->m_anCornerFlags[ucCorner];
            if (uiPass < 2 && ((uiUsedBits & 7) >> ucCorner & 1) != 0) continue;
            if (uiPass == 0 && (ucFlags & 0x10) != 0) continue;
            if ((ucFlags & uiFlagMask) == 0) continue;
            pCell->m_nUsedCornerBits = (unsigned short)((uiUsedBits & 0xfff8u) | ((uiUsedBits | (1u << ucCorner)) & 7u));
            out->m_Hex = kHex;
            out->m_nCorner = ucCorner;
            return uiPass == 2 ? 2 : 1;
        }
    }
    return 0;
}

/// 0x087b9500
FGXVector4 cvCityBuildingSim::CalculateWonderPositionFromSlot(WONDER::HexCorner const& slot) {
    const HEXVec_i& kNeighbourA = HEXVec_i::TRANSLATIONS[kCornerNeighbours[slot.m_nCorner][0]];
    const HEXVec_i& kNeighbourB = HEXVec_i::TRANSLATIONS[kCornerNeighbours[slot.m_nCorner][1]];

    // Centroid of the three hexes that meet at the corner, in hex coordinates.
    const float fHexX = (float)slot.m_Hex.x + (float)(kNeighbourA.x + slot.m_Hex.x) + (float)(slot.m_Hex.x + kNeighbourB.x);
    const float fHexY = (float)slot.m_Hex.y + (float)(kNeighbourA.y + slot.m_Hex.y) + (float)(slot.m_Hex.y + kNeighbourB.y);
    const float fCentroidX = fHexX / 3.0f;
    const float fCentroidY = fHexY / 3.0f;
    const float fCentroidZ = -fCentroidX - fCentroidY;

    const float* aMatrix = m_HexToWorldMatrix;
    FGXVector2 vPos;
    vPos.x = aMatrix[0] * fCentroidX + aMatrix[4] * fCentroidY + aMatrix[8] * fCentroidZ + aMatrix[12] - m_pTerrainSampler->m_fOffsetX;
    vPos.y = (aMatrix[1] * fCentroidX + aMatrix[5] * fCentroidY + aMatrix[9] * fCentroidZ + aMatrix[13]) * kHexRowScale -
             m_pTerrainSampler->m_fOffsetY;

    const FGXVector2 vHeading = Fun_087bbab8(reinterpret_cast<const LayoutFieldState*>(g_LayoutState), vPos);

    FGXVector4 vResult;
    vResult.x = vPos.x;
    vResult.y = vPos.y;
    vResult.z = vHeading.x;
    vResult.w = vHeading.y;
    return vResult;
}

/// 0x087baba2
static unsigned short SampleRawHeight(cvTerrainSampler* pSampler, FGXVector2 const& pos) {
    FGXInt2 kCell;
    kCell.y = (int)((pos.y + pSampler->m_fOffsetY) / pSampler->m_fScaleY);
    kCell.x = (int)((pos.x + pSampler->m_fOffsetX) / pSampler->m_fScaleX);
    const FGXInt2 kWrapped = pSampler->GetWrappedPosition(kCell);
    return Fun_08679d0a(&pSampler->m_iHeightMapWidth, &kWrapped);
}

/// 0x087b7de4
void cvCityBuildingSim::ResetPopBuildingsFromPlacementsInCity(unsigned int cityIdx) {
    CITY::cvCityData& kCity = m_pCities[cityIdx];
    const HEXVec_i kCityHex = {kCity.m_nHexX, kCity.m_nHexY, -(kCity.m_nHexY + kCity.m_nHexX)};
    FStaticVector<unsigned int, 256u, true> aPlaced;

    const unsigned char ucEdges = CalculateHexEdgeCoverage(kCityHex);
    LayoutFieldState& kState = *reinterpret_cast<LayoutFieldState*>(g_LayoutState);
    LayoutPattern* pPattern = kState.m_pPatterns;
    for (unsigned int uiPattern = 0; uiPattern < kState.m_nPatternCount; uiPattern++) {
        if (kState.m_pPatterns[uiPattern].m_nEdgeMask == ucEdges) {
            pPattern = &kState.m_pPatterns[uiPattern];
            break;
        }
    }
    const LayoutDensityField* pField = LoadDensityField(&kState, pPattern->m_nFieldSet, kCity.m_nPopulation);

    cvTerrainSampler* pSampler = MapGlobals::g_pTerrainSampler;
    const float fOffsetX = pSampler->m_fOffsetX;
    const float fOffsetY = pSampler->m_fOffsetY;

    // Tex space to the pattern's field space: tex -> hex, move the city to the origin, reflect.
    D3DXMATRIX mToField;
    memcpy(&mToField, WorldGlobals::g_TexToHex, sizeof(mToField));
    const HEXVec kCityOffset = {(float)-kCity.m_nHexX, (float)-kCity.m_nHexY, (float)kCity.m_nHexX + (float)kCity.m_nHexY, 1.0f};
    D3DXMATRIX mCityTranslation;
    HEXMatTranslation(reinterpret_cast<HEXMatrix*>(&mCityTranslation), kCityOffset);
    D3DXMatrixMultiply(&mToField, &mToField, &mCityTranslation);
    D3DXMatrixMultiply(&mToField, &mToField, reinterpret_cast<const D3DXMATRIX*>(pPattern->m_aReflection));

    cvCityVisSystem& kCityVis = *reinterpret_cast<cvCityVisSystem*>(g_cvCityVisSystem);
    const cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor>& hCityVis =
        *reinterpret_cast<const cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor>*>(
            &kCity.m_hCityVis);
    const cvCityVis_BeginUpdate kBegin;
    kCityVis.UpdateFromHandle(hCityVis, kBegin);

    unsigned int uiAdded = 0;
    for (unsigned int uiBuilding = kCity.m_nFirstBuilding; uiBuilding != kBuildingListEnd;
         uiBuilding = kCity.m_pBuildingPool->m_pRecords[uiBuilding].m_nNext) {
        CityBuildingRecord& kRecord = kCity.m_pBuildingPool->m_pRecords[uiBuilding];
        kRecord.m_ucDamageRank = kBuildingClassNone;

        const float fU = kRecord.m_vPos.x + fOffsetX;
        const float fV = (kRecord.m_vPos.y + fOffsetY) * kHexRowStretch;
        const float fFieldX = mToField._11 * fU + mToField._21 * fV + mToField._31 * 0.0f + mToField._41;
        if (!(fFieldX < pField->m_fMaxX) || fFieldX < pField->m_fMinX) continue;
        const float fFieldY = mToField._12 * fU + mToField._22 * fV + mToField._32 * 0.0f + mToField._42;
        if (!(fFieldY < pField->m_fMaxY) || fFieldY < pField->m_fMinY) continue;

        const int iCellX = (int)((fFieldX - pField->m_fMinX) * pField->m_fScaleX);
        const int iCellY = (int)((fFieldY - pField->m_fMinY) * pField->m_fScaleY);
        kRecord.m_ucFlags = (kRecord.m_ucFlags & 0xf8) | (pField->m_aCells[iCellY * pField->m_nWidth + iCellX] & 7);
        if ((kRecord.m_ucFlags & 7) == kBuildingClassNone) continue;

        const unsigned short usHeight = SampleRawHeight(pSampler, kRecord.m_vPos);
        if ((unsigned short)(usHeight + kLandHeightBias) >= kLandHeightRange) {
            kRecord.m_ucFlags = (kRecord.m_ucFlags & 0xf8) | kBuildingClassNone;
            continue;
        }
        const FGXVector3 vNormal = pSampler->GetWorldNormal(kRecord.m_vPos);
        if (vNormal.z < kMinNormalZ) {
            kRecord.m_ucFlags = (kRecord.m_ucFlags & 0xf8) | kBuildingClassNone;
            continue;
        }

        aPlaced.push_back(uiBuilding);
        cvCityVis_AddBuilding kAdd;
        kAdd.m_nBuildingId = uiBuilding;
        kAdd.m_vPos = kRecord.m_vPos;
        kAdd.m_fHeight = pSampler->GetWorldHeight(kRecord.m_vPos);
        kAdd.m_vDir = kRecord.m_vDir;
        kAdd.m_ucClass = kRecord.m_ucFlags & 7;
        kCityVis.UpdateFromHandle(hCityVis, kAdd);
        uiAdded++;
    }

    kState.m_bFieldLoaded = 0;
    cvCityVis_EndUpdate kEnd;
    kEnd.m_nBuildingCount = uiAdded;
    kCityVis.UpdateFromHandle(hCityVis, kEnd);

    // Only the first few shuffled buildings can burn; the rank is the order in which they catch fire.
    std::random_shuffle(aPlaced.begin(), aPlaced.end());
    const unsigned int uiBurning = uiAdded < kBurningBuildingCount ? uiAdded : kBurningBuildingCount;
    for (unsigned int uiPosition = 0; uiPosition < uiBurning; uiPosition++) {
        unsigned char ucRank = 0;
        while (uiPosition >= kDamageRankThresholds[ucRank]) ucRank++;
        m_pBuildingSlots[aPlaced[uiPosition]].m_ucDamageRank = ucRank;
    }
}

/// 0x087b6f9e
void cvCityBuildingSim::UpdateCityWall(unsigned int cityIdx) {
    if (cityIdx == kNoCity) return;

    CITY::cvCityData& kCity = m_pCities[cityIdx];
    cvCityVisSystem& kCityVis = *reinterpret_cast<cvCityVisSystem*>(g_cvCityVisSystem);
    const CityVisHandle& hCityVis = *reinterpret_cast<const CityVisHandle*>(&kCity.m_hCityVis);

    const cvCityVis_ClearCityWall kClear;
    kCityVis.UpdateFromHandle(hCityVis, kClear);
    if (kCity.m_nWallSlot == -1) return;

    CityWallSlot& kSlot = m_pWallSlots[kCity.m_nWallSlot];
    PolygonEraseNodes(&kSlot.m_Polygon, kSlot.m_Polygon.m_uHead, kPolygonEnd);
    kSlot.m_Polygon.m_uSize = 0;

    const HEXVec_i kCityHex = {kCity.m_nHexX, kCity.m_nHexY, -(kCity.m_nHexY + kCity.m_nHexX)};
    const unsigned char ucEdges = CalculateHexEdgeCoverage(kCityHex);
    cvCityWallGenerator& kWallGenerator = *reinterpret_cast<cvCityWallGenerator*>(unk_248);
    kWallGenerator.GenerateWallPolygonFromCity(kCityHex, kCity, &kSlot.m_Polygon, ucEdges);
    kWallGenerator.GenerateWallDataFromPolygon(*GetCityHexCell(unk_44730, kCityHex), kCity, kSlot.m_Polygon);

    const cvCityVis_BuildWall kBuild;
    kCityVis.UpdateFromHandle(hCityVis, kBuild);
}

/// 0x087c3dc8
void cvCityWallGenerator::GenerateWallBetweenPoints(CITY::cvCityData* pCity, FGXVector2* pPoints, unsigned int uStart, unsigned int uEnd, unsigned int uTotal) {
    cvCityVisSystem& kCityVis = *reinterpret_cast<cvCityVisSystem*>(g_cvCityVisSystem);
    const CityVisHandle& hCityVis = *reinterpret_cast<const CityVisHandle*>(&pCity->m_hCityVis);

    // The range [uStart, uEnd) of the ring of uTotal points, wrapping at its end.
    const unsigned int uCount = uEnd - uStart + (uEnd < uStart ? uTotal : 0);
    if (uCount < 4) return;
    std::vector<FGXVector2> aCurve(uCount);
    if (uEnd != uStart) {
        FGXVector2* pSrc = pPoints + uStart;
        unsigned int uCopied = 0;
        do {
            if (pSrc == pPoints + uTotal) pSrc = pPoints;
            aCurve[uCopied++] = *pSrc++;
        } while (pSrc != pPoints + uEnd);
    }

    cvCityVis_AddWallSegment kSegment = {};
    unsigned int uFilled = 0;
    unsigned int uLeft = uCount;
    for (unsigned int i = 0; i < uCount; i++) {
        if (uFilled == kMaxSegmentPoints || (uFilled >= kMinSegmentPoints && uLeft >= 3 && uLeft <= kMaxSegmentPoints)) {
            kSegment.m_ucCount = (unsigned char)uFilled;
            kCityVis.UpdateFromHandle(hCityVis, kSegment);
            kSegment.m_aPoints[0] = kSegment.m_aPoints[uFilled - kSegmentOverlap];
            kSegment.m_aPoints[1] = kSegment.m_aPoints[uFilled - 1];
            uFilled = kSegmentOverlap;
        }
        kSegment.m_aPoints[uFilled].x = aCurve[i].x;
        kSegment.m_aPoints[uFilled].y = aCurve[i].y;
        kSegment.m_aPoints[uFilled].z = m_pTerrainSampler->GetWorldHeight(aCurve[i]);
        uFilled++;
        uLeft--;
    }
    if (uFilled == 0) return;
    if (uFilled >= kMinSegmentPoints) {
        kSegment.m_ucCount = (unsigned char)uFilled;
        kCityVis.UpdateFromHandle(hCityVis, kSegment);
    }

    // Arc length up to the midpoint of each pair of control points; the last entry is the whole curve.
    const float fLength = B_SPLINE::CalculateApproxSplineLength<FGXVector2*, static_cast<B_SPLINE::BSPLINE_BEGIN_TYPES>(0), static_cast<B_SPLINE::BSPLINE_END_TYPES>(0)>(&aCurve[0], &aCurve[0] + uCount);
    std::vector<float> aArcLength(uCount - 1);
    aArcLength[0] = 0.0f;
    float fPrevMidX = (aCurve[0].x + aCurve[1].x) * 0.5f;
    float fPrevMidY = (aCurve[0].y + aCurve[1].y) * 0.5f;
    float fRunning = 0.0f;
    for (unsigned int i = 1; i + 2 < uCount; i++) {
        const float fMidX = (aCurve[i].x + aCurve[i + 1].x) * 0.5f;
        const float fMidY = (aCurve[i].y + aCurve[i + 1].y) * 0.5f;
        const float fDx = fMidX - fPrevMidX;
        const float fDy = fMidY - fPrevMidY;
        fRunning += sqrtf(fDy * fDy + fDx * fDx);
        aArcLength[i] = fRunning;
        fPrevMidX = fMidX;
        fPrevMidY = fMidY;
    }
    aArcLength[uCount - 2] = fLength;

    const int iTowers = (int)floorf(fLength / kTowerSpacing);
    if (iTowers == 0) return;
    const float fSpacing = fLength / (float)iTowers;
    const float fMaxT = (float)(uCount - 2);

    // The tower at the start of the curve is left out.
    float fArc = 0.0f;
    for (int iTower = 0; iTower != iTowers; iTower++) {
        float fT = FLT_MAX;
        for (unsigned int j = 0; j < uCount - 1; j++) {
            if (aArcLength[j] > fArc) {
                const float fFraction = (aArcLength[j] - fArc) / (aArcLength[j] - aArcLength[j - 1]);
                fT = (float)((int)j - 1) + std::max(0.0f, std::min(1.0f, 1.0f - fFraction));
                break;
            }
        }
        if (fT == FLT_MAX) fT = fMaxT;
        fT = std::max(0.0f, std::min(fMaxT, fT));

        const FGXVector2 vPos = B_SPLINE::EvalScalingFunc<FGXVector2, FGXVector2*, static_cast<B_SPLINE::BSPLINE_BEGIN_TYPES>(0), static_cast<B_SPLINE::BSPLINE_END_TYPES>(0)>((int)uCount, &aCurve[0], fT, 0);
        const FGXVector2 vGradient = B_SPLINE::EvalGradient<FGXVector2, FGXVector2*, static_cast<B_SPLINE::BSPLINE_BEGIN_TYPES>(0), static_cast<B_SPLINE::BSPLINE_END_TYPES>(0)>((int)uCount, &aCurve[0], fT, 0);
        if (iTower != 0) {
            cvCityVis_AddWallTower kTower;
            kTower.m_vPos = vPos;
            kTower.m_fHeight = m_pTerrainSampler->GetWorldHeight(vPos);
            kTower.m_fHeading = atan2f(vGradient.y, vGradient.x);
            kCityVis.UpdateFromHandle(hCityVis, kTower);
        }
        fArc += fSpacing;
    }
}

/// 0x087c442e
void cvCityWallGenerator::GenerateWallDataFromPolygon(CITY::CityHexData const& hexData, CITY::cvCityData& city, WallPolygon const& polygon) {
    std::vector<FGXVector2> aPoints;
    aPoints.reserve(polygon.m_uSize);
    for (WallPolygon::const_iterator kIt = polygon.begin(); kIt != polygon.end(); PolygonConstIteratorNext(&kIt)) aPoints.push_back(*kIt);
    const unsigned int uCount = (unsigned int)aPoints.size();
    if (uCount < 3) return;

    cvCityVisSystem& kCityVis = *reinterpret_cast<cvCityVisSystem*>(g_cvCityVisSystem);
    const CityVisHandle& hCityVis = *reinterpret_cast<const CityVisHandle*>(&city.m_hCityVis);
    cvCityVis_SetWallScale kScale;
    kScale.m_fScale = kWallScale;
    kCityVis.UpdateFromHandle(hCityVis, kScale);

    // The whole ring, then the seam: two points before the ring end and two after its start.
    GenerateWallBetweenPoints(&city, &aPoints[0], 0, uCount, uCount);
    GenerateWallBetweenPoints(&city, &aPoints[0], uCount - 2, 2, uCount);
}

/// 0x087c464a
void cvCityWallGenerator::GenerateWallPolygonFromCity(HEXVec_i hex, CITY::cvCityData& city, WallPolygon* pPolygon, unsigned char ucEdges) {
    PolygonEraseNodes(pPolygon, pPolygon->m_uHead, kPolygonEnd);
    pPolygon->m_uSize = 0;

    const float fOffsetX = m_pTerrainSampler->m_fOffsetX;
    const float fOffsetY = m_pTerrainSampler->m_fOffsetY;
    const float* aHexToTex = WorldGlobals::g_HexToTex;
    const float fHexX = (float)hex.x;
    const float fHexY = (float)hex.y;
    const float fHexZ = -fHexX - fHexY;

    LayoutFieldState& kState = *reinterpret_cast<LayoutFieldState*>(g_LayoutState);
    LayoutPattern* pPattern = kState.m_pPatterns;
    for (unsigned int uPattern = 0; uPattern < kState.m_nPatternCount; uPattern++) {
        if (kState.m_pPatterns[uPattern].m_nEdgeMask == ucEdges) {
            pPattern = &kState.m_pPatterns[uPattern];
            break;
        }
    }
    const LayoutDensityField* pField = LoadDensityField(&kState, pPattern->m_nFieldSet, city.m_nPopulation);

    // Tex space to the field space of the pattern.
    D3DXMATRIX mToField;
    memcpy(&mToField, WorldGlobals::g_TexToHex, sizeof(mToField));
    D3DXMatrixMultiply(&mToField, &mToField, reinterpret_cast<const D3DXMATRIX*>(pPattern->m_aReflection));

    // The lattice offsets are relative to the city; fOriginTex is the tex position of hex (0, 0, 0).
    const float fOriginTexX = aHexToTex[12];
    const float fOriginTexY = aHexToTex[13];
    const float fCityTexX = aHexToTex[8] * fHexZ + (aHexToTex[4] * fHexY + aHexToTex[0] * fHexX) + aHexToTex[12];
    const float fCityTexY = aHexToTex[9] * fHexZ + (aHexToTex[5] * fHexY + aHexToTex[1] * fHexX) + aHexToTex[13];

    const unsigned int uPopulation = (unsigned int)city.m_nPopulation;
    cvTerrainSampler* pSampler = MapGlobals::g_pTerrainSampler;
    FStaticVector<FGXVector2, 512u, true> aSamples;
    for (int j = 0; j < kLatticeSize; j++) {
        const float fRowBase = (float)(2 * j) + kLatticeOrigin;
        for (int i = 0; i < kLatticeSize; i++) {
            const FGXVector2& vJitter = m_aSampleOffsets[i % 8][j % 8];
            float fTexX = (float)(2 * i) + kLatticeOrigin;
            fTexX += vJitter.x;
            fTexX += fOriginTexX;
            float fTexY = vJitter.y + fRowBase;
            fTexY += fOriginTexY;

            const float fFieldX = mToField._31 * 0.0f + (mToField._21 * fTexY + mToField._11 * fTexX) + mToField._41;
            const float fFieldY = mToField._32 * 0.0f + (mToField._22 * fTexY + mToField._12 * fTexX) + mToField._42;
            if (!(fFieldX < pField->m_fMaxX) || fFieldX < pField->m_fMinX) continue;
            if (!(fFieldY < pField->m_fMaxY) || fFieldY < pField->m_fMinY) continue;
            const int iCellX = (int)((fFieldX - pField->m_fMinX) * pField->m_fScaleX);
            const int iCellY = (int)((fFieldY - pField->m_fMinY) * pField->m_fScaleY);
            const unsigned char ucCell = pField->m_aCells[iCellY * pField->m_nWidth + iCellX];

            // The cell classes that count as built-up grow with the population.
            bool bBuiltUp;
            if (uPopulation <= 3) bBuiltUp = ucCell != kBuildingClassNone;
            else if (uPopulation <= 5) bBuiltUp = (ucCell | 4) != kBuildingClassNone;
            else bBuiltUp = ucCell > kBuildingClassNone || ((0x13u >> ucCell) & 1u) == 0;
            if (!bBuiltUp) continue;

            const float fRelX = fTexX - fOriginTexX;
            const float fRelY = fTexY - fOriginTexY;
            FGXVector2 vPos;
            vPos.x = (fCityTexX + fRelX) - fOffsetX;
            vPos.y = (fCityTexY + fRelY) * kHexRowScale - fOffsetY;

            const unsigned short usHeight = SampleRawHeight(pSampler, vPos);
            if ((unsigned short)(usHeight + kLandHeightBias) >= kLandHeightRange) continue;
            if (g_TerrainStencil.SampleWorldPosition(vPos, kStencilWaterBit) != 0) continue;
            if (sqrtf(fRelY * fRelY + fRelX * fRelX) > kMaxSampleRadius) continue;
            const FGXVector3 vNormal = pSampler->GetWorldNormal(vPos);
            if (vNormal.z < kMinNormalZ) continue;
            aSamples.push_back(vPos);
        }
    }
    kState.m_bFieldLoaded = 0;

    PolygonEraseNodes(pPolygon, pPolygon->m_uHead, kPolygonEnd);
    pPolygon->m_uSize = 0;
    POLY_ComputeBoundingHull<FGXVector2*, WallPolygon>(aSamples.begin(), aSamples.size(), *pPolygon);

    // A hull this small is redone with the corners of the city hex added to the samples.
    if (fabsf(POLY_SignedArea<WallPolygonIterator>(pPolygon->begin(), pPolygon->m_uSize)) < kMinHullArea) {
        for (unsigned int uCorner = 0; uCorner < 6; uCorner++) {
            D3DXVECTOR4 vCorner;
            vCorner.x = kHexCornerOffsets[uCorner][0] * kHexCornerScale + fHexX;
            vCorner.y = kHexCornerOffsets[uCorner][1] * kHexCornerScale + fHexY;
            vCorner.z = -vCorner.x - vCorner.y;
            vCorner.w = 1.0f;
            D3DXVECTOR4 vTex;
            D3DXVec4Transform(&vTex, &vCorner, reinterpret_cast<const D3DXMATRIX*>(aHexToTex));
            FGXVector2 vPos;
            vPos.x = vTex.x - fOffsetX;
            vPos.y = vTex.y * kHexRowScale - fOffsetY;
            aSamples.push_back(vPos);
        }
        PolygonEraseNodes(pPolygon, pPolygon->m_uHead, kPolygonEnd);
        pPolygon->m_uSize = 0;
        POLY_ComputeBoundingHull<FGXVector2*, WallPolygon>(aSamples.begin(), aSamples.size(), *pPolygon);
    }

    // Close the ring with the last point in front and the first behind, then push every point outward
    // along the normal of the spline through the closed ring.
    const unsigned int uHullCount = pPolygon->m_uSize;
    std::vector<FGXVector2> aOffsets(uHullCount + 2);
    WallPolygonPool& rPool = *pPolygon->m_pPool;
    const FGXVector2 vFirst = rPool[pPolygon->m_uHead].m_aItems[0];
    const FGXVector2 vLast = rPool[pPolygon->m_uTail].m_aItems[rPool[pPolygon->m_uTail].m_uCount - 1];
    PolygonPushFront(pPolygon, vLast);
    PolygonPushBack(pPolygon, vFirst);

    if (pPolygon->m_uSize != 2) {
        WallPolygonIterator kIt = pPolygon->begin();
        float fT = 0.5f;
        for (unsigned int i = 0; i < pPolygon->m_uSize - 2; i++) {
            const FGXVector2 vGradient = B_SPLINE::EvalGradient<FGXVector2, WallPolygonIterator, static_cast<B_SPLINE::BSPLINE_BEGIN_TYPES>(1), static_cast<B_SPLINE::BSPLINE_END_TYPES>(1)>((int)pPolygon->m_uSize, kIt, fT, (int)i);
            const float fGradientLength = sqrtf(vGradient.y * vGradient.y + vGradient.x * vGradient.x);
            const float fNormalX = vGradient.x / fGradientLength;
            const float fNormalY = vGradient.y / fGradientLength;
            aOffsets[i].x = fNormalY * -kWallOutwardOffset;
            aOffsets[i].y = fNormalX + fNormalX;
            PolygonIteratorNext(&kIt);
            fT += 1.0f;
        }
    }
    if (pPolygon->m_uSize != 0) PolygonErase(pPolygon, pPolygon->m_uHead << 8);
    PolygonPopBack(pPolygon);

    unsigned int uPoints = 0;
    for (WallPolygonIterator kIt = pPolygon->begin(); kIt != pPolygon->end(); PolygonIteratorNext(&kIt)) {
        aOffsets[uPoints].x = (*kIt).x + aOffsets[uPoints].x;
        aOffsets[uPoints].y = (*kIt).y + aOffsets[uPoints].y;
        uPoints++;
    }

    PolygonEraseNodes(pPolygon, pPolygon->m_uHead, kPolygonEnd);
    pPolygon->m_uSize = 0;
    for (unsigned int i = 0; i < uPoints; i++) PolygonPushBack(pPolygon, aOffsets[i]);
}
