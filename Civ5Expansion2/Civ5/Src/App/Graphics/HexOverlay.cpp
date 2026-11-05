/// origin binary
#include <string.h>
#include <Windows.h>
#include "Civ5/Src/App/EventTemplate.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/CultureBorders.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/HEXTransform_i.h"
#include "Civ5/Src/App/HEXVec.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/NetProxyConnectionResponseData.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "Civ5/Src/App/cvFileChangeNotification.h"
#include "Civ5/Src/App/cvPath.h"
#include "FirePlace/FireEngine2/FTextureResource.h"
#include "FirePlace/FireGrafix/CCommandStream.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/DrawSimpleOverlay.h"
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireGrafix/FGXShaderPackageInstanceView.h"
#include "FirePlace/FireWorks/FString.h"
#include "FirePlace/FireWorks/FXmlDocument.h"
#include "FirePlace/FireWorks/FXmlElement.h"

namespace {

typedef EventTemplate<Event_Int2Type<112>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass> HexGridOnEvent;
typedef EventTemplate<Event_Int2Type<113>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass> HexGridOffEvent;

const char kHexGridXmlPath[] = "Assets/Overlays/HexGrid.xml";
const unsigned int kHexesPerBuffer = 0x400;
const unsigned int kVerticesPerHex = 12;
const unsigned int kIndicesPerHex = 36;
const unsigned int kBufferCount = 3;
const unsigned int kFileMonitorFlags = 0x10;
const float kColorChannelMax = 255.0f;

// Hex rows are squashed to this factor in overlay texture space (sqrt(3) / 2), and stretched back for the grid texture.
const float kHexRowScale = 0.8660254f;
const float kHexTextureRowScale = 1.1547005f;
const unsigned int kHexGridTextureFlags = 0x20000;
const unsigned int kHexGridSurfaceFlags = 0x43;
const unsigned short kHexGridSubset = 4;

// Constants OverlayDynamics holds for the hex grid draw.
struct HexOverlayConstants {
    D3DXMATRIX amIdentity[2];
    D3DXMATRIX mTexture;
};

const float kThird = 1.0f / 3.0f;
const float kTwoThirds = 2.0f / 3.0f;

// Corners of a unit hex in cube coordinates.
const float kHexCorners[6][3] = {
    {kTwoThirds, -kThird, -kThird},
    {kThird, kThird, -kTwoThirds},
    {-kThird, kTwoThirds, -kThird},
    {-kTwoThirds, kThird, kThird},
    {-kThird, -kThird, kTwoThirds},
    {kThird, -kTwoThirds, kThird},
};

}

/// 0x08a4d5ac
void HexOverlay::Startup() {
    m_pShader = new FireGrafix::DrawSimpleOverlay();
    for (int i = kBufferCount - 1; i >= 0; --i) {
        m_ahIndexBuffer[i] = FireGrafix::g_pRenderer->CreateStaticIB(static_cast<FireGrafix::_FGX_INDEXFORMATS>(0), kHexesPerBuffer * kIndicesPerHex, 0, 0);
        m_ahVertexBuffer[i] = FireGrafix::g_pRenderer->CreateDynamicVB(static_cast<FireGrafix::_FGX_VERTEXFORMATS>(5), kHexesPerBuffer * kVerticesPerHex, 0);
    }

    // Outer ring 0..5, inner ring 6..11: one quad (two triangles) per hex side.
    unsigned short aIndices[kIndicesPerHex];
    for (int iSide = 0; iSide < 5; ++iSide) {
        unsigned short* pQuad = &aIndices[iSide * 6];
        pQuad[0] = iSide;
        pQuad[1] = iSide + 1;
        pQuad[2] = iSide + 7;
        pQuad[3] = iSide;
        pQuad[4] = iSide + 7;
        pQuad[5] = iSide + 6;
    }
    aIndices[30] = 5;
    aIndices[31] = 6;
    aIndices[32] = 0;
    aIndices[33] = 5;
    aIndices[34] = 6;
    aIndices[35] = 11;
    for (int i = kBufferCount - 1; i >= 0; --i) {
        unsigned short* pIndices = static_cast<unsigned short*>(FireGrafix::g_pRenderer->LockStaticIB(m_ahIndexBuffer[i], 0));
        for (unsigned int uHex = 0; uHex < kHexesPerBuffer; ++uHex) {
            for (unsigned int uIndex = 0; uIndex < kIndicesPerHex; ++uIndex) {
                pIndices[uHex * kIndicesPerHex + uIndex] = aIndices[uIndex] + uHex * kVerticesPerHex;
            }
        }
        FireGrafix::g_pRenderer->UnlockIB(m_ahIndexBuffer[i], 0, 0);
    }
    LoadXML();

    TerrainEventDelegate kDelegate;
    kDelegate.m_pObject = this;
    void (HexOverlay::*pfnOn)(HexGridOnEvent&) = &HexOverlay::OnHexGridOn;
    memcpy(&kDelegate.m_pfnMethod, &pfnOn, sizeof(pfnOn));
    Fun_08a4f3e6(&kDelegate);
    void (HexOverlay::*pfnOff)(HexGridOffEvent&) = &HexOverlay::OnHexGridOff;
    memcpy(&kDelegate.m_pfnMethod, &pfnOff, sizeof(pfnOff));
    Fun_08a4f054(&kDelegate);
}

/// 0x08a4d78e
void HexOverlay::LoadXML() {
    FStringA kColorText;
    FXmlDocument kDocument;
    FXmlElement kRoot;
    if (!kDocument.Load(kHexGridXmlPath)) {
        MessageBoxA(0, "Could not load hex grid xml file", "Hex grid error", MB_ICONEXCLAMATION);
        return;
    }
    kDocument.GetRootElement(kRoot);
    if (kRoot.GetFirstAttributeVal(kColorText, "color", 5)) {
        ExtractColor(kColorText, m_vColor);
        m_vColor.x /= kColorChannelMax;
        m_vColor.y /= kColorChannelMax;
        m_vColor.z /= kColorChannelMax;
        m_vColor.w /= kColorChannelMax;
    } else {
        MessageBoxA(0, "Could not load color information", "Hex grid error", MB_ICONEXCLAMATION);
    }
    if (!kRoot.GetFirstAttributeVal(m_fWidth, "width", 5)) {
        MessageBoxA(0, "Could not load width information", "Hex grid error", MB_ICONEXCLAMATION);
    }
    // Files are monitored for reload only when bit 0 of application byte 0xf4d is set.
    const unsigned char* pApplication = *reinterpret_cast<const unsigned char* const*>(g_pkApplication);
    if ((pApplication[0xf4d] & 1) != 0) {
        cvPath kPath;
        kPath.FromSystemLocale(kHexGridXmlPath);
        reinterpret_cast<cvFileChangeNotification*>(m_kFileMonitor)->SetFileToMonitor(kPath, kFileMonitorFlags);
    }
}

/// 0x08a4da76
void HexOverlay::OnHexGridOn(EventTemplate<Event_Int2Type<112>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    m_iVisible = 1;
}

/// 0x08a4da82
void HexOverlay::OnHexGridOff(EventTemplate<Event_Int2Type<113>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    m_iVisible = 0;
}

/// 0x08a4da8e
void HexOverlay::StartupGame() {
    Fun_08a4daab(m_fWidth);
}

/// 0x08a4daab
void Fun_08a4daab(float fWidth) {
    const D3DXMATRIX* pHexToTex = reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_HexToTex);
    // Corners of the hex-space rectangle that spans the texture of one hex (w = 0: direction vectors).
    D3DXVECTOR4 avRect[4] = {
        {-5.0f / 3.0f, 4.0f / 3.0f, -1.0f / 6.0f, 0.0f},
        {kThird, 4.0f / 3.0f, -5.0f / 3.0f, 0.0f},
        {5.0f / 3.0f, -4.0f / 3.0f, 1.0f / 6.0f, 0.0f},
        {-kThird, -4.0f / 3.0f, 5.0f / 3.0f, 0.0f},
    };
    for (int i = 0; i < 4; ++i) {
        D3DXVECTOR4 vTransformed;
        D3DXVec4Transform(&vTransformed, &avRect[i], pHexToTex);
        avRect[i] = vTransformed;
    }

    // Outer ring, then the inner ring scaled by the line width.
    float aafRing[12][3];
    const float fInnerScale = 1.0f - fWidth;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 3; ++j) {
            aafRing[i][j] = kHexCorners[i][j];
            aafRing[6 + i][j] = kHexCorners[i][j] * fInnerScale;
        }
    }

    for (unsigned int i = 0; i < kVerticesPerHex; ++i) {
        HexOverlayVertex& rkVertex = HexOverlay::s_aVertexTemplate[i];
        rkVertex.x = aafRing[i][0];
        rkVertex.y = aafRing[i][1];
        rkVertex.z = aafRing[i][2];
        rkVertex.uColor = 0xffffffff;
        const D3DXVECTOR4 vCorner = {aafRing[i][0], aafRing[i][1], aafRing[i][2], 0.0f};
        D3DXVECTOR4 vTexel;
        D3DXVec4Transform(&vTexel, &vCorner, pHexToTex);
        rkVertex.u = (vTexel.x - avRect[0].x) / (avRect[1].x - avRect[0].x);
        rkVertex.v = 1.0f - (vTexel.y - avRect[3].y) / (avRect[0].y - avRect[3].y);
    }
}

/// 0x08a4df64
void HexOverlay::Shutdown() {
    TerrainEventDelegate kDelegate;
    kDelegate.m_pObject = this;
    void (HexOverlay::*pfnOn)(HexGridOnEvent&) = &HexOverlay::OnHexGridOn;
    memcpy(&kDelegate.m_pfnMethod, &pfnOn, sizeof(pfnOn));
    Fun_08a4ee28(&kDelegate);
    void (HexOverlay::*pfnOff)(HexGridOffEvent&) = &HexOverlay::OnHexGridOff;
    memcpy(&kDelegate.m_pfnMethod, &pfnOff, sizeof(pfnOff));
    Fun_08a4ebfc(&kDelegate);

    // DrawSimpleOverlay is a 0x30-byte shader package instance view; only that destructor runs.
    FireGrafix::FGXShaderPackageInstanceView* pView = reinterpret_cast<FireGrafix::FGXShaderPackageInstanceView*>(m_pShader);
    if (pView != nullptr) {
        delete pView;
    }
    for (int i = kBufferCount - 1; i >= 0; --i) {
        Fun_0866fd26(FireGrafix::CRenderer9::m_VBList, m_ahVertexBuffer[i]);
        Fun_0866fb7e(FireGrafix::CRenderer9::m_IBList, m_ahIndexBuffer[i]);
        m_ahVertexBuffer[i] = 0;
        m_ahIndexBuffer[i] = 0;
    }
}

/// 0x08a4e026
void HexOverlay::Update(float fDeltaTime) {
    cvFileChangeNotification* pMonitor = reinterpret_cast<cvFileChangeNotification*>(m_kFileMonitor);
    if (pMonitor->HasChanged()) {
        LoadXML();
        Fun_08a4daab(m_fWidth);
        pMonitor->ResetChange();
    }
}

/// 0x08a4e06c
void HexOverlay::ExtractColor(FStringA const& kText, FGXVector4& vColor) {
    FStringA kList;
    kList.Copy(kText.GetLength(), kText, 0);
    FStringA kToken;
    int iPosition = 0;
    kList.TrimLeft();
    kList.TrimRight();
    const auto clampChannel = [](int iValue) {
        const int iNonNegative = iValue < 0 ? 0 : iValue;
        return iNonNegative < 0x100 ? static_cast<float>(iNonNegative) : kColorChannelMax;
    };
    if (kList.GetLength() > 0) {
        float* pfChannels = &vColor.x;
        kList.Extract(kToken, iPosition, ",");
        pfChannels[0] = clampChannel(kToken.ConvertToInt(10, true));
        for (int iChannel = 1; iChannel < 4; ++iChannel) {
            do {
                kList.Extract(kToken, iPosition, ",");
            } while (kToken.GetLength() < 1);
            pfChannels[iChannel] = clampChannel(kToken.ConvertToInt(10, true));
        }
    }
}

/// 0x08a4e3f2
void HexOverlay::Render(FireGrafix::CCommandStream* pStream, int iCamera) {
    // World to overlay texture: squash the rows, then remove the sampler's world offset.
    D3DXMATRIX mWorldToOverlay;
    memset(&mWorldToOverlay, 0, sizeof(mWorldToOverlay));
    mWorldToOverlay._11 = 1.0f;
    mWorldToOverlay._22 = 1.0f;
    mWorldToOverlay._33 = 1.0f;
    mWorldToOverlay._44 = 1.0f;
    D3DXMATRIX mStep;
    D3DXMatrixMultiply(&mWorldToOverlay, &mWorldToOverlay, D3DXMatrixScaling(&mStep, 1.0f, kHexRowScale, 1.0f));
    D3DXMatrixMultiply(&mWorldToOverlay, &mWorldToOverlay,
                       D3DXMatrixTranslation(&mStep, -MapGlobals::g_pTerrainSampler->m_fOffsetX, -MapGlobals::g_pTerrainSampler->m_fOffsetY, 0.0f));

    // Grid texture space: recentre the stretched rows on the texel centre.
    D3DXMATRIX mTexture;
    memset(&mTexture, 0, sizeof(mTexture));
    mTexture._11 = 1.0f;
    mTexture._22 = 1.0f;
    mTexture._33 = 1.0f;
    mTexture._44 = 1.0f;
    D3DXMatrixMultiply(&mTexture, &mTexture, D3DXMatrixTranslation(&mStep, -0.5f, -0.5f, 0.0f));
    D3DXMatrixMultiply(&mTexture, &mTexture, D3DXMatrixScaling(&mStep, 1.0f, kHexTextureRowScale, 1.0f));
    D3DXMatrixMultiply(&mTexture, &mTexture, D3DXMatrixTranslation(&mStep, 0.5f, 0.5f, 0.0f));

    if (m_iVisible == 0 || iCamera > 2) {
        return;
    }

    HexOverlayVertex* pVertices = static_cast<HexOverlayVertex*>(FireGrafix::g_pRenderer->LockDynamicVB(m_ahVertexBuffer[iCamera], 0));
    const FGXInt2& kTopLeft = reinterpret_cast<const FGXInt2*>(FrameGlobals::g_GameGridTopLeftVisible)[iCamera];
    const FGXInt2& kBottomRight = reinterpret_cast<const FGXInt2*>(FrameGlobals::g_GameGridBottomRightVisible)[iCamera];
    const HEXMatrix_i& mxGameGridInverse = *reinterpret_cast<const HEXMatrix_i*>(WorldGlobals::g_GameGrid + 0x40);
    const D3DXMATRIX* pHexToTex = reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_HexToTex);

    unsigned int uHexes = 0;
    for (int iRow = kTopLeft.x; iRow < kBottomRight.x && uHexes < kHexesPerBuffer; ++iRow) {
        for (int iColumn = kTopLeft.y; iColumn < kBottomRight.y && uHexes < kHexesPerBuffer; ++iColumn) {
            const int iHexX = iRow - (iColumn >> 1);
            const HEXVec_i kOffsetHex = {iHexX, iColumn, -(iHexX + iColumn)};
            const HEXVec_i kHex = Fun_086616f0(mxGameGridInverse, kOffsetHex);
            const HEXVec vHex = {static_cast<float>(kHex.x), static_cast<float>(kHex.y), -static_cast<float>(kHex.x) - static_cast<float>(kHex.y), 1.0f};
            D3DXMATRIX mHex;
            HEXMatTranslation(reinterpret_cast<HEXMatrix*>(&mHex), vHex);
            D3DXMATRIX mTex;
            D3DXMATRIX mHexToOverlay;
            D3DXMatrixMultiply(&mTex, &mHex, pHexToTex);
            D3DXMatrixMultiply(&mHexToOverlay, &mTex, &mWorldToOverlay);

            HexOverlayVertex* pHexVertices = pVertices + uHexes * kVerticesPerHex;
            for (unsigned int i = 0; i < kVerticesPerHex; ++i) {
                HexOverlayVertex& rkVertex = pHexVertices[i];
                rkVertex = HexOverlay::s_aVertexTemplate[i];
                const float fX = rkVertex.x;
                const float fY = rkVertex.y;
                const float fZ = rkVertex.z;
                rkVertex.x = mHexToOverlay._11 * fX + mHexToOverlay._21 * fY + mHexToOverlay._31 * fZ + mHexToOverlay._41;
                rkVertex.y = mHexToOverlay._12 * fX + mHexToOverlay._22 * fY + mHexToOverlay._32 * fZ + mHexToOverlay._42;
                rkVertex.z = mHexToOverlay._13 * fX + mHexToOverlay._23 * fY + mHexToOverlay._33 * fZ + mHexToOverlay._43;
            }
            ++uHexes;
        }
    }
    FireGrafix::g_pRenderer->UnlockVB(m_ahVertexBuffer[iCamera], 0, uHexes * kVerticesPerHex);

    D3DXMATRIX mIdentity;
    memset(&mIdentity, 0, sizeof(mIdentity));
    mIdentity._11 = 1.0f;
    mIdentity._22 = 1.0f;
    mIdentity._33 = 1.0f;
    mIdentity._44 = 1.0f;
    FireGrafix::OverlayDynamics* pDynamics = static_cast<FireGrafix::OverlayDynamics*>(m_pShader->m_pDynamicConstantSet);
    HexOverlayConstants* pConstants = static_cast<HexOverlayConstants*>(pDynamics->LockDynamic(pStream));
    pConstants->amIdentity[0] = mIdentity;
    pConstants->amIdentity[1] = mIdentity;
    pConstants->mTexture = mTexture;
    pDynamics->UnlockDynamic();

    FireGrafix::FGXSurface* pSurface;
    unsigned int hSurface;
    FireGrafix::g_pRenderer->CreateSurface(&pSurface, &hSurface, kHexGridSurfaceFlags);
    pSurface->m_hIB = m_ahIndexBuffer[iCamera];
    pSurface->m_hVB = m_ahVertexBuffer[iCamera];
    pSurface->field_18[0] = static_cast<unsigned short>(FTextureResourceMgr::LoadTextureHandle("Assets/Overlays/HexGrid.dds", kHexGridTextureFlags, nullptr));
    pSurface->field_38[0] = 5;
    pSurface->field_60 = 3;
    pSurface->field_c = uHexes * kVerticesPerHex;
    pStream->DrawSurface(hSurface, m_pShader->m_hInstance, kHexGridSubset);
    FireGrafix::g_pRenderer->ReleaseSurface(hSurface);
}
