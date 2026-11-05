/// origin guess
#include <math.h>
#include <new>
#include <Windows.h>
#include "Civ5/Src/App/Graphics/cvCoastMeshEngine.h"
#include "Civ5/Src/App/Graphics/CellBlitJob.h"
#include "Civ5/Src/App/Graphics/CreateLevelSplinesJob.h"
#include "Civ5/Src/App/Graphics/TerrainData.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/cvCityBuildingSim.h"
#include "Civ5/Src/App/cvFloodplainsSystem.h"
#include "FirePlace/FireWorks/FJob.h"

namespace {

const char kJobTag[] = "Port/Civ5/Src/App/Graphics/cvCoastMesh.cpp";

const B_SPLINE::BSPLINE_BEGIN_TYPES kBegin = (B_SPLINE::BSPLINE_BEGIN_TYPES)0;
const B_SPLINE::BSPLINE_END_TYPES kEnd = (B_SPLINE::BSPLINE_END_TYPES)0;

}  // namespace

/// 0x092142b0
unsigned int const g_aCoastQuadIndices[6] = {0, 1, 8, 1, 9, 8};

/// 0x088e96b4
CoastStyleDef Fun_088e96b4(cvCoastMeshLibrary const& kLibrary, unsigned int uiStyle, unsigned int uiContinent) {
    CoastStyleDef kStyle = CoastStyleDef();
    for (unsigned int i = 0; i < kLibrary.m_uiNumStyles; ++i) {
        if (kLibrary.m_pStyles[i].m_uiStyleIndex == uiStyle) {
            kStyle = kLibrary.m_pStyles[i];
        }
    }

    const TerrainData& kTerrain = *kLibrary.m_pTerrainData;
    for (int iStrip = 0; iStrip < 4; ++iStrip) {
        const unsigned int uiType = kStyle.m_aiTerrainType[iStrip];
        if (uiType == 0xffffffff) {
            continue;
        }
        // The continent's own texture first, then the one of continent 0.
        unsigned int i = 0;
        while (i < kTerrain.m_uiNumTerrains && kTerrain.m_aHexCode[i] != (g_aContinentPrefix[uiContinent] | uiType)) {
            ++i;
        }
        if (i < kTerrain.m_uiNumTerrains && (unsigned char)i != 0xff) {
            kStyle.m_aiTerrainType[iStrip] = (unsigned char)i;
            continue;
        }
        unsigned int uiIndex = 0xff;
        for (i = 0; i < kTerrain.m_uiNumTerrains; ++i) {
            if (kTerrain.m_aHexCode[i] == (uiType | g_aContinentPrefix[0])) {
                uiIndex = (unsigned char)i;
                break;
            }
        }
        kStyle.m_aiTerrainType[iStrip] = uiIndex;
    }
    return kStyle;
}

/// 0x088e6df4
UNK_RET cvCoastMeshEngine::GenerateAllLevelSplines() {
    const unsigned int uiNumSplines = m_CoastSplineSystem.m_uiNumSplines;
    CreateLevelSplinesJob* aJobs = static_cast<CreateLevelSplinesJob*>(_alloca(uiNumSplines * sizeof(CreateLevelSplinesJob)));
    FJobSet kJobSet;
    m_aFinalLevelSplines.reserve(uiNumSplines);
    for (unsigned int i = 0; i < uiNumSplines; ++i) {
        new (&aJobs[i]) CreateLevelSplinesJob();
        m_aFinalLevelSplines.push_back();
        aJobs[i].m_pSpline = &m_CoastSplineSystem.m_pSplines[i];
        aJobs[i].m_pLevels = &m_aFinalLevelSplines[i];
        aJobs[i].m_pEngine = this;
        kJobSet.AddJob(&aJobs[i], kJobTag, nullptr);
    }
    kJobSet.Wait();
}

/// 0x088e7918
UNK_RET cvCoastMeshEngine::GenerateLevels(FinalLevelSplines& kLevels, CoastlineSpline const& kSpline) const {
    kLevels.m_eType = kSpline.m_eType;
    const unsigned int uiNumPoints = kSpline.m_uiNumPoints;

    // Three subdivisions per level: n -> 2n-2 -> 4n-6 -> 8n-14 control points.
    const unsigned int uiLevelSize = uiNumPoints * 8 - 14;
    unsigned int uiFirst = 0;
    for (int iLevel = 0; iLevel < 5; ++iLevel) {
        kLevels.m_auiLevelRange[iLevel][0] = uiFirst;
        uiFirst += uiLevelSize;
        kLevels.m_auiLevelRange[iLevel][1] = uiFirst;
    }
    const unsigned int uiTotal = (uiLevelSize * 5) | 1;
    Fun_088e9ff8(reinterpret_cast<CoastControlPointVector*>(&kLevels.m_pControlPoints), uiTotal);
    kLevels.m_uiNumControlPoints = uiTotal < kLevels.m_uiMaxControlPoints ? uiTotal : kLevels.m_uiMaxControlPoints;

    CoastCP* apOut[5];
    for (int iLevel = 0; iLevel < 5; ++iLevel) {
        apOut[iLevel] = kLevels.m_pControlPoints + kLevels.m_auiLevelRange[iLevel][0];
    }

    CoastStyleDef kStyleA;
    CoastStyleDef kStyleB;
    const CoastCP* pSource = kSpline.m_pControlPoints;

    if (kSpline.m_bClosed) {
        // Points 0 and n-1 of each level are the wrap copies written after the loop.
        for (int iLevel = 0; iLevel < 5; ++iLevel) {
            ++apOut[iLevel];
        }
        bool bFlip = true;
        unsigned int uiLastStyle = 0xffffffff;
        float fT = 0.5f;
        for (unsigned int i = 0; i < uiNumPoints - 1; ++i) {
            const CoastCP kGradient = B_SPLINE::EvalGradient<CoastCP, CoastCP const*, kBegin, kEnd>(uiNumPoints, kSpline.m_pControlPoints + i, fT, i);
            const CoastCP kValue = B_SPLINE::EvalScalingFunc<CoastCP, CoastCP const*, kBegin, kEnd>(uiNumPoints, kSpline.m_pControlPoints + i, fT, i);
            fT += 1.0f;
            const float fInvLength = 1.0f / sqrtf(kGradient.m_vPos.x * kGradient.m_vPos.x + kGradient.m_vPos.y * kGradient.m_vPos.y);
            const unsigned int uiStyle = kValue.m_ucStyleA;
            float fBlend = kValue.m_fBlendWeight;
            if (uiStyle != uiLastStyle) {
                kStyleA = Fun_088e96b4(m_CoastMeshLibrary, uiStyle, kSpline.m_iContinent);
                kStyleB = Fun_088e96b4(m_CoastMeshLibrary, kValue.m_ucStyleB, kSpline.m_iContinent);
                bFlip = !bFlip;
                uiLastStyle = uiStyle;
            }
            if (bFlip) {
                fBlend = 1.0f - fBlend;
            }
            const CoastCP& kBase = pSource[i + 1];
            for (int iLevel = 0; iLevel < 5; ++iLevel) {
                const float fOffset = kStyleB.m_aLevels[iLevel].m_fOffset * (1.0f - fBlend) + kStyleA.m_aLevels[iLevel].m_fOffset * fBlend;
                CoastCP& kOut = *apOut[iLevel]++;
                kOut.m_vPos.x = kBase.m_vPos.x + kGradient.m_vPos.y * fInvLength * fOffset;
                kOut.m_vPos.y = kBase.m_vPos.y - kGradient.m_vPos.x * fInvLength * fOffset;
                kOut.m_ucStyleA = kBase.m_ucStyleA;
                kOut.m_ucStyleB = kBase.m_ucStyleB;
                kOut.m_fBlendWeight = kBase.m_fBlendWeight;
            }
        }
        for (int iLevel = 0; iLevel < 5; ++iLevel) {
            CoastCP* pLevel = kLevels.m_pControlPoints + kLevels.m_auiLevelRange[iLevel][0];
            pLevel[0].m_vPos.x = pLevel[uiNumPoints - 2].m_vPos.x - kSpline.m_vWrapOffset.x;
            pLevel[0].m_vPos.y = pLevel[uiNumPoints - 2].m_vPos.y - kSpline.m_vWrapOffset.y;
            pLevel[0].m_fBlendWeight = pLevel[uiNumPoints - 2].m_fBlendWeight;
            pLevel[uiNumPoints - 1].m_vPos.x = pLevel[1].m_vPos.x + kSpline.m_vWrapOffset.x;
            pLevel[uiNumPoints - 1].m_vPos.y = pLevel[1].m_vPos.y + kSpline.m_vWrapOffset.y;
            pLevel[uiNumPoints - 1].m_fBlendWeight = pLevel[1].m_fBlendWeight;
        }
    } else if (uiNumPoints != 0) {
        bool bFlip = true;
        unsigned int uiLastStyle = 0xffffffff;
        const float fLastT = (float)(int)(uiNumPoints - 2) + -0.01f;
        float fT = 0.5f;
        for (unsigned int i = 0; i < uiNumPoints; ++i) {
            CoastCP kGradient;
            CoastCP kValue;
            if (i == 0) {
                kGradient = B_SPLINE::EvalGradient<CoastCP, CoastCP const*, kBegin, kEnd>(uiNumPoints, kSpline.m_pControlPoints, 0.0f, 0);
                kValue = B_SPLINE::EvalScalingFunc<CoastCP, CoastCP const*, kBegin, kEnd>(uiNumPoints, kSpline.m_pControlPoints, 0.0f, 0);
            } else if (i == uiNumPoints - 1) {
                const unsigned int uiLast = kSpline.m_uiNumPoints;
                kGradient = B_SPLINE::EvalGradient<CoastCP, CoastCP const*, kBegin, kEnd>(uiNumPoints, kSpline.m_pControlPoints + uiLast, fLastT, uiLast);
                kValue = B_SPLINE::EvalScalingFunc<CoastCP, CoastCP const*, kBegin, kEnd>(uiNumPoints, kSpline.m_pControlPoints + uiLast, fLastT, uiLast);
            } else {
                kGradient = B_SPLINE::EvalGradient<CoastCP, CoastCP const*, kBegin, kEnd>(uiNumPoints, kSpline.m_pControlPoints + i, fT, i);
                kValue = B_SPLINE::EvalScalingFunc<CoastCP, CoastCP const*, kBegin, kEnd>(uiNumPoints, kSpline.m_pControlPoints + i, fT, i);
                fT += 1.0f;
            }
            const float fInvLength = 1.0f / sqrtf(kGradient.m_vPos.x * kGradient.m_vPos.x + kGradient.m_vPos.y * kGradient.m_vPos.y);
            const unsigned int uiStyle = kValue.m_ucStyleA;
            float fBlend = kValue.m_fBlendWeight;
            if (uiStyle != uiLastStyle) {
                kStyleA = Fun_088e96b4(m_CoastMeshLibrary, uiStyle, kSpline.m_iContinent);
                kStyleB = Fun_088e96b4(m_CoastMeshLibrary, kValue.m_ucStyleB, kSpline.m_iContinent);
                bFlip = !bFlip;
                uiLastStyle = uiStyle;
            }
            if (bFlip) {
                fBlend = 1.0f - fBlend;
            }
            const CoastCP& kBase = pSource[i];
            for (int iLevel = 0; iLevel < 5; ++iLevel) {
                const float fOffset = kStyleB.m_aLevels[iLevel].m_fOffset * (1.0f - fBlend) + kStyleA.m_aLevels[iLevel].m_fOffset * fBlend;
                CoastCP& kOut = *apOut[iLevel]++;
                kOut.m_vPos.x = kBase.m_vPos.x + kGradient.m_vPos.y * fInvLength * fOffset;
                kOut.m_vPos.y = kBase.m_vPos.y - kGradient.m_vPos.x * fInvLength * fOffset;
                kOut.m_ucStyleA = kBase.m_ucStyleA;
                kOut.m_ucStyleB = kBase.m_ucStyleB;
                kOut.m_fBlendWeight = kBase.m_fBlendWeight;
            }
        }
    }

    unsigned int uiCount = uiNumPoints;
    for (int iLevel = 0; iLevel < 5; ++iLevel) {
        uiCount = uiNumPoints;
        for (int iPass = 0; iPass < 2; ++iPass) {
            CoastCP* pLevel = kLevels.m_pControlPoints + kLevels.m_auiLevelRange[iLevel][0];
            B_SPLINE::ReconstructScalingFunc<CoastCP, CoastCP*, CoastCP*, kBegin, kEnd>(pLevel, pLevel + uiCount, pLevel + uiCount * 2 - 2);
            uiCount = uiCount * 2 - 2;
        }
    }

    // Character noise: the style's profile sample is rotated into the tangent frame and added to the point.
    bool bFlip = true;
    unsigned int uiLastStyle = 0xffffffff;
    for (int iLevel = 0; iLevel < 5; ++iLevel) {
        for (unsigned int i = 0; i < uiCount - 1; ++i) {
            CoastCP& kPoint = kLevels.m_pControlPoints[kLevels.m_auiLevelRange[iLevel][0] + i];
            const CoastCP kGradient = B_SPLINE::EvalGradient<CoastCP, CoastCP const*, kBegin, kEnd>(uiCount, &kPoint, (float)(int)i + 0.5f, i);
            const float fInvLength = 1.0f / sqrtf(kGradient.m_vPos.x * kGradient.m_vPos.x + kGradient.m_vPos.y * kGradient.m_vPos.y);
            const unsigned int uiStyle = kPoint.m_ucStyleA;
            if (uiStyle != uiLastStyle) {
                kStyleA = Fun_088e96b4(m_CoastMeshLibrary, uiStyle, kSpline.m_iContinent);
                kStyleB = Fun_088e96b4(m_CoastMeshLibrary, kPoint.m_ucStyleB, kSpline.m_iContinent);
                bFlip = !bFlip;
                uiLastStyle = uiStyle;
            }
            if (i == 0) {
                continue;
            }
            const float fTangentX = kGradient.m_vPos.x * fInvLength;
            const float fTangentY = kGradient.m_vPos.y * fInvLength;
            float fBlend = kPoint.m_fBlendWeight;
            if (bFlip) {
                fBlend = 1.0f - fBlend;
            }
            float fAX = 0.0f;
            float fAY = 0.0f;
            const int iCharacterA = kStyleA.m_aLevels[iLevel].m_iCharacter;
            if (iCharacterA != -1) {
                const CoastCharacterSpline& kCharacter = m_CoastMeshLibrary.m_pSplines[iCharacterA];
                const FGXVector2& vSample = kCharacter.m_pSamples[i % kCharacter.m_uiNumSamples];
                fAX = vSample.x * fTangentX - vSample.y * fTangentY;
                fAY = vSample.x * fTangentY + vSample.y * fTangentX;
            }
            float fBX = 0.0f;
            float fBY = 0.0f;
            const int iCharacterB = kStyleB.m_aLevels[iLevel].m_iCharacter;
            if (iCharacterB != -1) {
                const CoastCharacterSpline& kCharacter = m_CoastMeshLibrary.m_pSplines[iCharacterB];
                const FGXVector2& vSample = kCharacter.m_pSamples[i % kCharacter.m_uiNumSamples];
                fBX = vSample.x * fTangentX - vSample.y * fTangentY;
                fBY = vSample.x * fTangentY + vSample.y * fTangentX;
            }
            kPoint.m_vPos.x += fBX * (1.0f - fBlend) + fAX * fBlend;
            kPoint.m_vPos.y += fBY * (1.0f - fBlend) + fAY * fBlend;
        }
        if (kSpline.m_bClosed) {
            CoastCP* pLevel = kLevels.m_pControlPoints + kLevels.m_auiLevelRange[iLevel][0];
            pLevel[0].m_vPos.x = pLevel[uiCount - 2].m_vPos.x - kSpline.m_vWrapOffset.x;
            pLevel[0].m_vPos.y = pLevel[uiCount - 2].m_vPos.y - kSpline.m_vWrapOffset.y;
            pLevel[0].m_fBlendWeight = pLevel[uiCount - 2].m_fBlendWeight;
            pLevel[uiCount - 1].m_vPos.x = pLevel[1].m_vPos.x + kSpline.m_vWrapOffset.x;
            pLevel[uiCount - 1].m_vPos.y = pLevel[1].m_vPos.y + kSpline.m_vWrapOffset.y;
            pLevel[uiCount - 1].m_fBlendWeight = pLevel[1].m_fBlendWeight;
        }
    }

    for (int iLevel = 0; iLevel < 5; ++iLevel) {
        CoastCP* pLevel = kLevels.m_pControlPoints + kLevels.m_auiLevelRange[iLevel][0];
        B_SPLINE::ReconstructScalingFunc<CoastCP, CoastCP*, CoastCP*, kBegin, kEnd>(pLevel, pLevel + uiCount, pLevel + uiCount * 2 - 2);
    }
}

/// 0x088e8614
UNK_RET cvCoastMeshEngine::GenerateFinalMesh(CoastMesh& kMesh, FinalLevelSplines const& kLevels, unsigned int uiContinent) const {
    const unsigned int uiNumLevelPoints = kLevels.m_auiLevelRange[0][1] - kLevels.m_auiLevelRange[0][0];
    const unsigned int uiNumQuadPairs = uiNumLevelPoints * 2 - 4;
    const unsigned int uiNumTriangles = uiNumLevelPoints * 8 - 16;
    kMesh.m_aVertices.setsize(uiNumLevelPoints * 8 - 8);
    kMesh.m_aIndices.setsize(uiNumQuadPairs * 12);
    kMesh.field_18.reserve(uiNumTriangles);

    unsigned int uiTrisPerBatch = 64;
    unsigned int uiNumBatches = (uiNumTriangles - 1) / uiTrisPerBatch;
    unsigned int uiRemainder = uiNumTriangles % uiTrisPerBatch;
    while ((++uiNumBatches > 1 && (uiNumBatches & 1) != 0) || (uiRemainder != 0 && uiRemainder < 8)) {
        uiTrisPerBatch += 8;
        uiRemainder = uiNumTriangles % uiTrisPerBatch;
        uiNumBatches = (uiNumTriangles - 1) / uiTrisPerBatch;
    }
    kMesh.m_aBatches.setsize(uiNumBatches);
    CoastMeshBatch* pBatch = kMesh.m_aBatches.begin();
    pBatch->m_uiNumTriangles = 0;
    pBatch->m_uiNumVertices = 8;
    pBatch->m_pVertices = kMesh.m_aVertices.begin();
    pBatch->m_pIndices = kMesh.m_aIndices.begin();

    // Strip j runs from level j (inner vertex) to level j+1 (outer vertex).
    unsigned int auiNumPoints[4];
    const CoastCP* apInner[4];
    const CoastCP* apOuter[4];
    unsigned int uiBegin = kLevels.m_auiLevelRange[0][0];
    for (int iStrip = 0; iStrip < 4; ++iStrip) {
        auiNumPoints[iStrip] = kLevels.m_auiLevelRange[iStrip][1] - uiBegin;
        apInner[iStrip] = kLevels.m_pControlPoints + uiBegin;
        uiBegin = kLevels.m_auiLevelRange[iStrip + 1][0];
        apOuter[iStrip] = kLevels.m_pControlPoints + uiBegin;
    }

    CoastMeshVertex* pVertex = kMesh.m_aVertices.begin();
    const unsigned int uiNumSamples = uiNumLevelPoints - 1;
    if (uiNumSamples != 0) {
        unsigned int uiLastStyle = 0xffffffff;
        bool bFlip = true;
        CoastStyleDef kStyleA;
        CoastStyleDef kStyleB;
        for (unsigned int i = 0; i != uiNumSamples; ++i) {
            const float fT = (float)(int)i;
            for (int iStrip = 0; iStrip < 4; ++iStrip) {
                const CoastCP kInner = B_SPLINE::EvalScalingFunc<CoastCP, CoastCP const*, kBegin, kEnd>(auiNumPoints[iStrip], apInner[iStrip] + i, fT, i);
                const unsigned int uiStyle = kInner.m_ucStyleA;
                float fBlend = kInner.m_fBlendWeight;
                if (uiStyle != uiLastStyle) {
                    kStyleA = Fun_088e96b4(m_CoastMeshLibrary, uiStyle, uiContinent);
                    kStyleB = Fun_088e96b4(m_CoastMeshLibrary, kInner.m_ucStyleB, uiContinent);
                    bFlip = !bFlip;
                    uiLastStyle = uiStyle;
                }
                if (bFlip) {
                    fBlend = 1.0f - fBlend;
                }
                const float fInvBlend = 1.0f - fBlend;
                const CoastStyleDef& kTerrainStyle = fBlend > 0.5f ? kStyleA : kStyleB;

                pVertex[0].m_vPos = kInner.m_vPos;
                pVertex[0].m_ucTerrainIndex = (unsigned char)kTerrainStyle.m_aiTerrainType[iStrip];
                pVertex[0].m_fHeight = kStyleB.m_aLevels[iStrip].m_fHeight * fInvBlend + kStyleA.m_aLevels[iStrip].m_fHeight * fBlend;
                pVertex[0].field_10 = kStyleB.m_aLevels[iStrip].m_fValue * fInvBlend + kStyleA.m_aLevels[iStrip].m_fValue * fBlend;

                const CoastCP kOuter = B_SPLINE::EvalScalingFunc<CoastCP, CoastCP const*, kBegin, kEnd>(auiNumPoints[iStrip], apOuter[iStrip] + i, fT, i);
                pVertex[1].m_vPos = kOuter.m_vPos;
                pVertex[1].m_ucTerrainIndex = (unsigned char)kTerrainStyle.m_aiTerrainType[iStrip];
                pVertex[1].m_fHeight = kStyleB.m_aLevels[iStrip + 1].m_fHeight * fInvBlend + kStyleA.m_aLevels[iStrip + 1].m_fHeight * fBlend;
                pVertex[1].field_10 = kStyleB.m_aLevels[iStrip + 1].m_fValue * fInvBlend + kStyleA.m_aLevels[iStrip + 1].m_fValue * fBlend;
                pVertex += 2;
            }
        }
    }

    const FGXVector3 kZero = {0.0f, 0.0f, 0.0f};
    const FGXVector3 kX = {1.0f, 0.0f, 0.0f};
    const FGXVector3 kOne = {1.0f, 1.0f, 1.0f};
    const FGXVector3 kZ = {0.0f, 0.0f, 1.0f};
    const unsigned int uiNumSegments = uiNumQuadPairs >> 1;
    unsigned short* pIndex = kMesh.m_aIndices.begin();
    unsigned int uiBatch = 0;
    unsigned int uiFirstVertex = 0;
    unsigned int uiFirstIndex = 0;
    unsigned int uiBaseVertex = 0;
    for (unsigned int uiSegment = 0; uiSegment < uiNumSegments; ++uiSegment) {
        if (pBatch->m_uiNumTriangles == uiTrisPerBatch) {
            ++uiBatch;
            pBatch = kMesh.m_aBatches.begin() + uiBatch;
            pBatch->m_uiNumTriangles = 0;
            pBatch->m_uiNumVertices = 8;
            pBatch->m_pVertices = kMesh.m_aVertices.begin() + uiFirstVertex;
            pBatch->m_pIndices = kMesh.m_aIndices.begin() + uiFirstIndex;
            uiBaseVertex = 0;
        }
        for (unsigned int uiStrip = 0; uiStrip < 4; ++uiStrip) {
            for (int k = 0; k < 6; ++k) {
                pIndex[k] = (unsigned short)(g_aCoastQuadIndices[k] + uiBaseVertex + uiStrip * 2);
            }
            pIndex += 6;
        }
        uiFirstVertex += 8;
        uiFirstIndex += 24;
        uiBaseVertex += 8;
        pBatch->m_uiNumVertices += 8;
        pBatch->m_uiNumTriangles += 8;
        kMesh.field_18.push_back(kZero);
        kMesh.field_18.push_back(kX);
        kMesh.field_18.push_back(kOne);
        kMesh.field_18.push_back(kOne);
        kMesh.field_18.push_back(kOne);
        kMesh.field_18.push_back(kOne);
        kMesh.field_18.push_back(kZ);
        kMesh.field_18.push_back(kZero);
    }
}
