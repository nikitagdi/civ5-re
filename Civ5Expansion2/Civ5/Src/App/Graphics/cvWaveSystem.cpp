/// origin binary
#include "Civ5/Src/App/Graphics/cvWaveSystem.h"
#include <math.h>
#include <string.h>
#include <Windows.h>
#include "Civ5/Src/App/Engine/GameSplineDatabase.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "Civ5/Src/App/NetProxyConnectionResponseData.h"
#include "Civ5/Src/App/UI/UIRenderBuffer.h"
#include "Civ5/Src/App/cvCityBuildingSim.h"
#include "Civ5/Src/App/cvFloodplainsSystem.h"
#include "FirePlace/FireGrafix/CCommandStream.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "FirePlace/FireGrafix/SurfaceSet.h"
#include "FirePlace/FireGrafix/TextureData.h"
#include "FirePlace/FireWorks/FRandom.h"
#include "FirePlace/FireWorks/FString.h"
#include "FirePlace/FireWorks/FXmlAttribute.h"
#include "FirePlace/FireWorks/FXmlDocument.h"
#include "FirePlace/FireWorks/FXmlElement.h"

namespace {

// Waves per batch; the batch constants are one float4 each.
const unsigned int kWavesPerBatch = 248;
// Spline samples per wave, spaced kSampleStep apart in [0, 1].
const unsigned int kSamplesPerWave = 9;
const float kSampleStep = 0.125f;
// The crest quad spans from kInnerReach behind the spline to kOuterReach in front of it.
const float kInnerReach = 0.1f;
const float kOuterReach = 1.2f;
// Fraction of size * spline length * crest aspect used as the half-extent of the quad.
const float kExtentScale = 0.4f;
// The LCG shared with FRandom, as inlined by Update.
const unsigned int kRandomModulus = 0xffffcfef;
const double kRandomScale = 2.3283131071116735E-10;
// XML colours are sRGB bytes.
const float kColorGamma = 2.2f;

const auto FloatToUint = [](float fValue) -> unsigned int {
    if (fValue >= 2147483648.0f) {
        return static_cast<unsigned int>(static_cast<int>(fValue - 2147483648.0f) ^ 0x80000000);
    }
    return static_cast<unsigned int>(static_cast<int>(fValue));
};

const auto NextRandom = [](unsigned int uiState) -> unsigned int {
    uiState = uiState * 0x343fd + 0x269ec3;
    return uiState % kRandomModulus;
};

const auto PageToUV = [](unsigned int uiPage, unsigned int uiPagesU, unsigned int uiPagesV, float& fU, float& fV) {
    fU = static_cast<float>(uiPage % uiPagesU) / static_cast<float>(uiPagesU);
    fV = static_cast<float>(uiPage / uiPagesU) / static_cast<float>(uiPagesV);
};

}

/// 0x0893bbf4
cvWaveSystem::cvWaveSystem() {
    m_uShaderPackageInstance = 0;
    unk_84[0] = unk_84[1] = unk_84[2] = unk_84[3] = 0;
    m_pCommandStream = 0;
    m_uiBatchCount = 0;
    m_pBatches = 0;
    m_uiWaveCount = 0;
    m_pWaves = 0;
    m_hCrestTexture = 0;
    m_pSurfaceSet = 0;
}

/// 0x0893bd36
bool cvWaveSystem::ParseSettingsFromXML(char const* pszPath, cvWaveSystem::WaveSettings& kSettings) {
    kSettings.m_fWavesPerEdge = 4.6f;
    kSettings.m_uiCrestPagesU = 8;
    kSettings.m_uiCrestPagesV = 1;
    kSettings.m_uiCrestPagesInUse = 8;
    kSettings.m_uiRandomSeed = 0;
    kSettings.m_fMinSize = 0.05f;
    kSettings.m_fMaxSize = 0.1f;
    kSettings.m_fCycleTime = 10.0f;
    kSettings.m_fFadeInStart = 1.0f;
    kSettings.m_fFadeInEnd = 4.5f;
    kSettings.m_fFadeOutStart = 8.0f;
    kSettings.m_fFadeOutEnd = 9.5f;
    kSettings.m_fRestartDelayMin = 0.0f;
    kSettings.m_fRestartDelayMax = 3.0f;
    kSettings.m_fStartDistance = 0.1f;
    kSettings.m_fEndDistance = 0.02f;
    kSettings.m_fCrestAspect = 0.125f;

    FXmlDocument kDocument;
    FXmlElement kRoot;
    if (!kDocument.Load(pszPath)) {
        return false;
    }
    kDocument.GetRootElement(kRoot);
    FStringA kName;
    if (!kRoot.GetName(kName) || strcmp(static_cast<char const*>(kName), "WaveSettings") != 0) {
        return false;
    }
    const unsigned int uiAttributeCount = kRoot.GetAttributeCount();
    FXmlAttribute kAttribute;
    kRoot.GetFirstAttribute(kAttribute);
    float fValue = 0.0f;
    for (unsigned int i = 0; i < uiAttributeCount; ++i) {
        kAttribute.GetName(kName);
        kAttribute.GetVal(fValue);
        kAttribute.GetNextAttribute(kAttribute);
        char const* pszName = static_cast<char const*>(kName);
        if (_stricmp(pszName, "WavesPerEdge") == 0) {
            kSettings.m_fWavesPerEdge = fValue;
        } else if (_stricmp(pszName, "RandomSeed") == 0) {
            kSettings.m_uiRandomSeed = FloatToUint(fValue);
        } else if (_stricmp(pszName, "CrestPagesU") == 0) {
            kSettings.m_uiCrestPagesU = FloatToUint(fValue);
        } else if (_stricmp(pszName, "CrestPagesV") == 0) {
            kSettings.m_uiCrestPagesV = FloatToUint(fValue);
        } else if (_stricmp(pszName, "CrestPagesInUse") == 0) {
            kSettings.m_uiCrestPagesInUse = FloatToUint(fValue);
        } else if (_stricmp(pszName, "MinSize") == 0) {
            kSettings.m_fMinSize = fValue;
        } else if (_stricmp(pszName, "MaxSize") == 0) {
            kSettings.m_fMaxSize = fValue;
        } else if (_stricmp(pszName, "StartDistance") == 0) {
            kSettings.m_fStartDistance = fValue;
        } else if (_stricmp(pszName, "EndDistance") == 0) {
            kSettings.m_fEndDistance = fValue;
        } else if (_stricmp(pszName, "CycleTime") == 0) {
            kSettings.m_fCycleTime = fValue;
        } else if (_stricmp(pszName, "FadeInStart") == 0) {
            kSettings.m_fFadeInStart = fValue;
        } else if (_stricmp(pszName, "FadeInEnd") == 0) {
            kSettings.m_fFadeInEnd = fValue;
        } else if (_stricmp(pszName, "FadeOutStart") == 0) {
            kSettings.m_fFadeOutStart = fValue;
        } else if (_stricmp(pszName, "FadeOutEnd") == 0) {
            kSettings.m_fFadeOutEnd = fValue;
        } else if (_stricmp(pszName, "RestartDelayMin") == 0) {
            kSettings.m_fRestartDelayMin = fValue;
        } else if (_stricmp(pszName, "RestartDelayMax") == 0) {
            kSettings.m_fRestartDelayMax = fValue;
        } else if (_stricmp(pszName, "Color_R") == 0) {
            kSettings.m_fColorR = powf(fValue / 255.0f, kColorGamma);
        } else if (_stricmp(pszName, "Color_G") == 0) {
            kSettings.m_fColorG = powf(fValue / 255.0f, kColorGamma);
        } else if (_stricmp(pszName, "Color_B") == 0) {
            kSettings.m_fColorB = powf(fValue / 255.0f, kColorGamma);
        }
    }
    kSettings.m_fCrestAspect = static_cast<float>(kSettings.m_uiCrestPagesV) / static_cast<float>(kSettings.m_uiCrestPagesU);
    return true;
}

/// 0x0893d21a
UNK_RET cvWaveSystem::Update(float fDeltaTime) {
    if (m_uiWaveCount == 0) {
        return;
    }
    // The state is advanced locally and never written back to the settings.
    unsigned int uiState = m_kSettings.m_uiRandomSeed;
    for (unsigned int i = 0; i < m_uiWaveCount; ++i) {
        WaveInstance& kWave = m_pWaves[i];
        kWave.m_fTime += fDeltaTime;
        if (kWave.m_fTime > m_kSettings.m_fCycleTime) {
            uiState = NextRandom(uiState);
            const float fRandom = static_cast<float>(static_cast<double>(uiState) * kRandomScale);
            kWave.m_fTime = -(fRandom * (m_kSettings.m_fRestartDelayMax - m_kSettings.m_fRestartDelayMin) + m_kSettings.m_fRestartDelayMin);
            uiState = NextRandom(uiState);
            PageToUV(uiState % m_kSettings.m_uiCrestPagesInUse, m_kSettings.m_uiCrestPagesU, m_kSettings.m_uiCrestPagesV, kWave.m_fPageU, kWave.m_fPageV);
        }
    }
}

/// 0x0893d392
UNK_RET cvWaveSystem::ComputePageUV(unsigned int uiPage, float& fU, float& fV) {
    PageToUV(uiPage, m_kSettings.m_uiCrestPagesU, m_kSettings.m_uiCrestPagesV, fU, fV);
}

/// 0x0893d418
UNK_RET cvWaveSystem::Render(unsigned int uiRenderTarget) {
    if (m_pSurfaceSet == 0) {
        return;
    }
    // Per camera: visible world rectangle as left, top, right, bottom.
    float aafCameraRect[4][4] = {};
    for (unsigned int c = 0; c < FrameGlobals::g_nCameras; ++c) {
        aafCameraRect[c][0] = FrameGlobals::g_WorldTopLeftVisibleUnclamped[c].x;
        aafCameraRect[c][1] = FrameGlobals::g_WorldTopLeftVisibleUnclamped[c].y;
        aafCameraRect[c][2] = FrameGlobals::g_WorldBottomRightVisibleUnclamped[c].x;
        aafCameraRect[c][3] = FrameGlobals::g_WorldBottomRightVisibleUnclamped[c].y;
    }
    m_pSurfaceSet->Flush();

    for (unsigned int b = 0; b < m_uiBatchCount; ++b) {
        const WaveBatch& kBatch = m_pBatches[b];
        for (unsigned int c = 0; c < FrameGlobals::g_nCameras; ++c) {
            const float* afRect = aafCameraRect[c];
            if (afRect[0] <= kBatch.m_fMaxX && kBatch.m_fMinX <= afRect[2] && afRect[1] <= kBatch.m_fMaxY && kBatch.m_fMinY <= afRect[3]) {
                void* pDest = static_cast<FireGrafix::WaveData*>(m_pInstanceConsts)->LockDynamic(m_pSurfaceSet);
                memcpy(pDest, m_pWaves + kBatch.m_uiFirstWave, kBatch.m_uiWaveCount * sizeof(WaveInstance));
                m_pInstanceConsts->UnlockDynamic();
                Fun_088c53a6(m_pSurfaceSet, kBatch.m_hSurface, m_uShaderPackageInstance, 0, 0xffffffff);
                break;
            }
        }
    }

    if (FrameGlobals::g_nCameras != 0) {
        const float fWaterLevel = MapGlobals::g_pTerrainSampler->field_4 + 1.0f;
        for (unsigned int c = 0; c < FrameGlobals::g_nCameras; ++c) {
            FireGrafix::FGXConstantSet* pSet = &reinterpret_cast<FireGrafix::FGXConstantSet*>(&FireGrafix::CRenderer9::m_kCSManager[0x2020c8])[m_auGlobalConstSets[c]];
            float* pConsts = static_cast<float*>(static_cast<FireGrafix::WaveGlobalConsts*>(pSet)->LockGlobal(0));
            D3DXMATRIX mViewProj;
            D3DXMatrixMultiply(&mViewProj, &FrameGlobals::g_View[c], &FrameGlobals::g_Proj);
            memcpy(pConsts, &mViewProj, sizeof(D3DXMATRIX));
            pConsts[0x10] = m_fWorldScale * m_kSettings.m_fStartDistance;
            pConsts[0x11] = m_fWorldScale * m_kSettings.m_fEndDistance;
            pConsts[0x12] = 1.0f / m_kSettings.m_fCycleTime;
            pConsts[0x13] = fWaterLevel;
            pConsts[0x14] = m_kSettings.m_fFadeInStart;
            pConsts[0x15] = 1.0f / (m_kSettings.m_fFadeInEnd - m_kSettings.m_fFadeInStart);
            pConsts[0x16] = m_kSettings.m_fFadeOutStart;
            pConsts[0x17] = 1.0f / (m_kSettings.m_fFadeOutEnd - m_kSettings.m_fFadeOutStart);
            pConsts[0x18] = 1.0f / static_cast<float>(m_kSettings.m_uiCrestPagesU);
            pConsts[0x19] = 1.0f / static_cast<float>(m_kSettings.m_uiCrestPagesV);
            pConsts[0x1c] = m_kSettings.m_fColorR;
            pConsts[0x1d] = m_kSettings.m_fColorG;
            pConsts[0x1e] = m_kSettings.m_fColorB;
            pConsts[0x1f] = 1.0f;
            pSet->Unlock(0);
        }
        FGXVector4 vClear;
        vClear.x = 0.0f;
        vClear.y = 0.0f;
        vClear.z = 0.0f;
        vClear.w = 0.0f;
        for (unsigned int c = 0; c < FrameGlobals::g_nCameras; ++c) {
            m_pCommandStream->BeginCommandSet(uiRenderTarget, 4, 10, 0, "Waves", vClear, 1.0f, 0, 0, 0);
            m_pCommandStream->AliasGlobal(FireGrafix::g_pWaveGlobalConsts->field_34, m_auGlobalConstSets[c]);
            // The surface set's handle is at +0xc.
            m_pCommandStream->DrawSurfaceSet(*reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(m_pSurfaceSet) + 0xc), 0xffffffff);
            m_pCommandStream->EndCommandSet();
        }
    }
    m_pCommandStream->AddAllCommandSetsToRenderQueue();
}

/// 0x0893dc6a
UNK_RET cvWaveSystem::CreateWave(unsigned int uiWaveIndex, GameSpline::Segment& kSpline, float fSortKey, FRandom& kRandom, FireGrafix::VStruct<25>* pVertices, unsigned short* pIndices, unsigned int uiFirstVertex) {
    const float fSize = (m_kSettings.m_fMaxSize - m_kSettings.m_fMinSize) * Fun_087fbf02(kRandom) + m_kSettings.m_fMinSize;
    const float fLength = B_SPLINE::CalculateApproxSplineLength<GameSpline::ControlPoint*, static_cast<B_SPLINE::BSPLINE_BEGIN_TYPES>(0), static_cast<B_SPLINE::BSPLINE_END_TYPES>(0)>(kSpline.begin(), kSpline.begin() + kSpline.size());
    const float fCycleTime = m_kSettings.m_fCycleTime;
    const float fExtent = fSize * fLength * m_kSettings.m_fCrestAspect;
    WaveInstance& kWave = m_pWaves[uiWaveIndex];
    kWave.m_fTime = fCycleTime * Fun_087fbf02(kRandom) + 0.0f;

    // The page pick draws the raw state instead of a scaled float.
    kRandom.m_uiCallCount = kRandom.m_uiCallCount + 1;
    unsigned int uiState = kRandom.m_uiState * 0x343fd + 0x269ec3;
    kRandom.m_uiState = uiState;
    if (kRandom.m_uiModulus != 0) {
        uiState = uiState % kRandom.m_uiModulus;
        kRandom.m_uiState = uiState;
    }
    PageToUV(uiState % m_kSettings.m_uiCrestPagesInUse, m_kSettings.m_uiCrestPagesU, m_kSettings.m_uiCrestPagesV, kWave.m_fPageU, kWave.m_fPageV);

    const unsigned char ucSlot = static_cast<unsigned char>(uiWaveIndex % kWavesPerBatch);
    const float fHalfExtent = fExtent * kExtentScale;
    WaveVertex* pVertex = reinterpret_cast<WaveVertex*>(pVertices);
    float fT = 0.0f;
    for (unsigned int i = 0; i < kSamplesPerWave; ++i) {
        FGXVector2 vPos;
        FGXVector2 vNormal;
        EvaluateSpline(kSpline, fT, vPos, vNormal);
        const unsigned char ucNormalX = static_cast<unsigned char>(static_cast<int>((vNormal.x + 1.0f) * 0.5f * 255.0f));
        const unsigned char ucNormalY = static_cast<unsigned char>(static_cast<int>((vNormal.y + 1.0f) * 0.5f * 255.0f));
        const unsigned char ucAlong = static_cast<unsigned char>(static_cast<int>(fT * 255.0f));

        WaveVertex& kOuter = pVertex[2 * i];
        float fCoord = vNormal.x * fHalfExtent * kOuterReach + vPos.x;
        D3DXFloat32To16Array(reinterpret_cast<D3DXFLOAT16*>(&kOuter.m_usX), &fCoord, 1);
        fCoord = vNormal.y * fHalfExtent * kOuterReach + vPos.y;
        D3DXFloat32To16Array(reinterpret_cast<D3DXFLOAT16*>(&kOuter.m_usY), &fCoord, 1);
        kOuter.m_ucSide = 0;
        kOuter.m_ucAlong = ucAlong;
        kOuter.m_ucNormalX = ucNormalX;
        kOuter.m_ucNormalY = ucNormalY;
        kOuter.m_ucSlot = ucSlot;

        WaveVertex& kInner = pVertex[2 * i + 1];
        fCoord = vPos.x - vNormal.x * fHalfExtent * kInnerReach;
        D3DXFloat32To16Array(reinterpret_cast<D3DXFLOAT16*>(&kInner.m_usX), &fCoord, 1);
        fCoord = vPos.y - vNormal.y * fHalfExtent * kInnerReach;
        D3DXFloat32To16Array(reinterpret_cast<D3DXFLOAT16*>(&kInner.m_usY), &fCoord, 1);
        kInner.m_ucSide = 0xff;
        kInner.m_ucAlong = ucAlong;
        kInner.m_ucNormalX = ucNormalX;
        kInner.m_ucNormalY = ucNormalY;
        kInner.m_ucSlot = ucSlot;
        fT += kSampleStep;
    }

    // Eight quads, two triangles each, over the nine vertex pairs.
    for (unsigned int q = 0; q < kSamplesPerWave - 1; ++q) {
        const unsigned short usBase = static_cast<unsigned short>(uiFirstVertex + 2 * q);
        pIndices[0] = usBase;
        pIndices[1] = static_cast<unsigned short>(usBase + 2);
        pIndices[2] = static_cast<unsigned short>(usBase + 1);
        pIndices[3] = static_cast<unsigned short>(usBase + 2);
        pIndices[4] = static_cast<unsigned short>(usBase + 3);
        pIndices[5] = static_cast<unsigned short>(usBase + 1);
        pIndices += 6;
    }
}

/// 0x0893e3e2
UNK_RET cvWaveSystem::EvaluateSpline(GameSpline::Segment const& kSpline, float fT, FGXVector2& vPos, FGXVector2& vNormal) {
    const int iCount = static_cast<int>(kSpline.size());
    const float fParam = (static_cast<float>(iCount) + -2.0f) * fT;
    const GameSpline::ControlPoint kValue = B_SPLINE::EvalScalingFunc<GameSpline::ControlPoint, GameSpline::ControlPoint const*, static_cast<B_SPLINE::BSPLINE_BEGIN_TYPES>(0), static_cast<B_SPLINE::BSPLINE_END_TYPES>(0)>(iCount, kSpline.begin(), fParam, 0);
    const GameSpline::ControlPoint kGradient = B_SPLINE::EvalGradient<GameSpline::ControlPoint, GameSpline::ControlPoint const*, static_cast<B_SPLINE::BSPLINE_BEGIN_TYPES>(0), static_cast<B_SPLINE::BSPLINE_END_TYPES>(0)>(iCount, kSpline.begin(), fParam, 0);
    vPos = kValue.m_vPos;
    vNormal.x = kGradient.m_vPos.y;
    vNormal.y = -kGradient.m_vPos.x;
    const float fLength = sqrtf(kGradient.m_vPos.y * kGradient.m_vPos.y + kGradient.m_vPos.x * kGradient.m_vPos.x);
    vNormal.x = vNormal.x / fLength;
    vNormal.y = vNormal.y / fLength;
}
