/// origin binary
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
#include "Civ5/Src/App/CvInitMgr.h"
#include "Civ5/Src/App/EventTemplate.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/CellBlitJob.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/Graphics/TradeVisSystem.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/NetProxyConnectionResponseData.h"
#include "Civ5/Src/App/UI/UIRenderBuffer.h"
#include "Civ5/Src/App/cvDllUnitList.h"
#include "FirePlace/FireEngine2/FTextureResource.h"
#include "FirePlace/FireGrafix/CCommandStream.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/TextureData.h"
#include "FirePlace/FireGrafix/TradeArrowShader.h"

namespace {

typedef EventTemplate<Event_Int2Type<117>, NewTradeRoute_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass> NewTradeRouteEvent;
typedef EventTemplate<Event_Int2Type<118>, UpdateTradeRouteDirection_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass> UpdateTradeRouteDirectionEvent;
typedef EventTemplate<Event_Int2Type<119>, DestroyTradeRoute_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass> DestroyTradeRouteEvent;
typedef EventTemplate<Event_Int2Type<120>, ToggleTradeRouteDisplay_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass> ToggleTradeRouteDisplayEvent;
typedef EventTemplate<Event_Int2Type<121>, ToggleTradeRoutePopup_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass> ToggleTradeRoutePopupEvent;

const int kMaxTradeRouteSlots = 0x100;
const int kMaxTradeRoutePlots = 0x100;
const unsigned int kTradeRouteVertexCapacity = 0x12000;
const unsigned int kTradeRouteIndexCapacity = 0x48000;
const unsigned int kTradeRouteVertexFormat = 9;
const unsigned int kIndexFormat32 = 1;
const unsigned int kNumTradeArrowTextures = 6;
const unsigned int kTextureLoadFlags = 0x20000;
const unsigned int kShowTradeRoutesOption = 0x11;

// Scroll phase units per second.
const float kTradeArrowScrollSpeed = 0.75f;
// Popup textures follow the three route textures in g_ahTradeArrowTextures.
const int kPopupTextureBase = 3;
const int kPopupSurface = 0;
const int kFirstRouteTypeSurface = 1;
const int kNumTradeRouteSurfaces = 4;
// Strategic view map transform inside g_cvStrategicViewVisSystem: float scale x, y at +0x8, origin x, y at +0x10.
const unsigned int kStrategicViewMapScale = 0x8;
const unsigned int kStrategicViewMapOrigin = 0x10;
const float kStrategicViewTradeRouteZ = 1.0f;
// Event 122 slot of a local machine (8 * id + 8): dispatcher, its context at +4.
const unsigned int kLocalMachineStride = 0x600;
const unsigned int kPublishBuffersEventSlot = 0x3d8;
// TradeArrowShader global constants: overlay projection at +0, scroll phase at +0x40.
const unsigned int kTradeArrowScrollConstant = 0x40;

typedef void (*PfnDispatchEvent)(const void* pEvent, void* pContext);

const unsigned int kTradeRouteSurfaceType = 0x4f;
// Route colour alpha: routes of the active player and the highlighted popup route.
const float kRouteAlpha = 0.4f;
const float kPopupRouteAlpha = 0.95f;
const float kColorScale = 255.0f;
// A strip piece is 9 stations of 4 vertices (feather, edge, edge, feather); 8 rows of 3 quads.
const int kStripStations = 9;
const int kStripVertices = kStripStations * 4;
const int kStripIndices = (kStripStations - 1) * 18;
const float kStripStep = 0.125f;
// The last piece of a route is a short stub that ends in the arrow head.
const float kStubStep = 0.01875f;
const float kShaftHalfWidth = 5.0f;
const float kShaftFeatherHalfWidth = 5.35f;
const float kVPerUnitParam = -2.0f;
const int kArrowHeadVertices = 14;
const int kArrowHeadIndices = 0x33;
const float kHeadHalfWidth = 10.0f;
const float kHeadFeatherHalfWidth = 10.500215f;
const float kHeadBackShift = 0.5f;
const float kHeadFeatherBackShift = 0.15f;
const float kHeadTipLength = 23.8f;
const float kHeadTipFeatherLength = 24.6155f;
const float kStubVAdvance = 0.728125f;

// Triangle template of one strip row; the vertices of the next row are 4 higher.
const unsigned int kStripRowIndices[18] = {0, 4, 5, 0, 5, 1, 1, 5, 6, 1, 6, 2, 2, 6, 7, 2, 7, 3};
const unsigned int kArrowHeadTriangles[kArrowHeadIndices] = {
    0, 1, 4, 4, 1, 3, 3, 1, 2,
    0, 4, 9, 0, 9, 5, 0, 5, 6, 0, 6, 1,
    2, 1, 6, 2, 6, 7, 2, 7, 8, 2, 8, 3,
    3, 8, 10, 3, 10, 11, 4, 3, 11, 4, 11, 12, 4, 12, 13, 4, 13, 9};

const auto IsSameHex = [](const HEXVec_i& kA, const HEXVec_i& kB) {
    return kA.x == kB.x && kA.y == kB.y && kA.z == kB.z;
};

const auto GetHexWorldLocation = [](const HEXVec_i& kHex) {
    HEXVec kPos = {static_cast<float>(kHex.x), static_cast<float>(kHex.y), static_cast<float>(kHex.z), 1.0f};
    return MapGlobals::g_pTerrainSampler->GetWorldLocation2D(kPos);
};

const auto SetRouteVertex = [](TradeRouteVertex& kVertex, float fX, float fY, unsigned int uColor, float fU, float fV) {
    kVertex.m_fX = fX;
    kVertex.m_fY = fY;
    kVertex.m_fZ = 0.0f;
    kVertex.m_uColor = uColor;
    kVertex.m_fU = fU;
    kVertex.m_fV = fV;
};

}

/// 0x089f1ae8
TradeVisSystem::TradeVisSystem() : field_0(8) {
}

/// 0x089f1af4
TradeVisSystem::~TradeVisSystem() {
}

/// 0x089f1af6
void TradeVisSystem::Startup() {
    TerrainEventDelegate kDelegate;
    kDelegate.m_pObject = this;
    void (TradeVisSystem::*pfnNewRoute)(NewTradeRouteEvent&) = &TradeVisSystem::NewTradeRoute_Handler;
    memcpy(&kDelegate.m_pfnMethod, &pfnNewRoute, sizeof(pfnNewRoute));
    Fun_089f5e0c(&kDelegate);
    kDelegate.m_pObject = this;
    void (TradeVisSystem::*pfnDirection)(UpdateTradeRouteDirectionEvent&) = &TradeVisSystem::UpdateTradeRouteDirection_Handler;
    memcpy(&kDelegate.m_pfnMethod, &pfnDirection, sizeof(pfnDirection));
    Fun_089f5a7a(&kDelegate);
    kDelegate.m_pObject = this;
    void (TradeVisSystem::*pfnDestroy)(DestroyTradeRouteEvent&) = &TradeVisSystem::DestroyTradeRoute_Handler;
    memcpy(&kDelegate.m_pfnMethod, &pfnDestroy, sizeof(pfnDestroy));
    Fun_089f56e8(&kDelegate);
    kDelegate.m_pObject = this;
    void (TradeVisSystem::*pfnDisplay)(ToggleTradeRouteDisplayEvent&) = &TradeVisSystem::ToggleDisplay_Handler;
    memcpy(&kDelegate.m_pfnMethod, &pfnDisplay, sizeof(pfnDisplay));
    Fun_089f5356(&kDelegate);
    kDelegate.m_pObject = this;
    void (TradeVisSystem::*pfnPopup)(ToggleTradeRoutePopupEvent&) = &TradeVisSystem::TogglePopup_Handler;
    memcpy(&kDelegate.m_pfnMethod, &pfnPopup, sizeof(pfnPopup));
    Fun_089f4fc4(&kDelegate);

    g_bShowTradeRoutes = *reinterpret_cast<const bool*>(&cvOptionsManager::g_GameOptionsStore[kShowTradeRoutesOption]);
    g_bTradePopupActive = false;
    g_iTradePopupRoute = -1;
    g_hTradeRouteVB = FireGrafix::g_pRenderer->CreateStaticVB(static_cast<FireGrafix::_FGX_VERTEXFORMATS>(kTradeRouteVertexFormat), kTradeRouteVertexCapacity, 0, 1);
    g_hTradeRouteIB = FireGrafix::g_pRenderer->CreateStaticIB(static_cast<FireGrafix::_FGX_INDEXFORMATS>(kIndexFormat32), kTradeRouteIndexCapacity, 0, 1);
    g_uTradeRouteVertexCount = 0;
    g_uTradeRouteIndexCount = 0;
    g_pTradeArrowShader = new FireGrafix::TradeArrowShader();
    for (int iTexture = kNumTradeArrowTextures - 1; iTexture >= 0; --iTexture) {
        g_ahTradeArrowTextures[iTexture] = FTextureResourceMgr::LoadTextureHandle(TEXTURE_PATHS[iTexture], kTextureLoadFlags, NULL);
    }
}

/// 0x089f1cb0
void TradeVisSystem::NewTradeRoute_Handler(EventTemplate<Event_Int2Type<117>, NewTradeRoute_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    const NewTradeRoute_EventData& kData = reinterpret_cast<const NewTradeRoute_EventData&>(kEvent);
    // Reuse the highest free slot; otherwise append one with a fresh plot buffer.
    int iSlot = g_nTradeRouteSlots;
    while (iSlot > 0 && g_aTradeRoutes[iSlot - 1].m_nPlots != 0) {
        --iSlot;
    }
    if (iSlot > 0) {
        --iSlot;
    } else {
        if (g_nTradeRouteSlots >= kMaxTradeRouteSlots) {
            return;
        }
        iSlot = g_nTradeRouteSlots;
        g_aTradeRoutes[iSlot].m_paiPlots = static_cast<int (*)[2]>(malloc(kMaxTradeRoutePlots * sizeof(int[2])));
        g_nTradeRouteSlots = iSlot + 1;
    }
    TradeRouteVis& kRoute = g_aTradeRoutes[iSlot];
    kRoute.m_iRoute = kData.m_iRoute;
    kRoute.m_iPlayer = kData.m_iPlayer;
    kRoute.m_bForwards = true;
    kRoute.m_eType = kData.m_eType;
    const int nPlots = kData.m_nPlots < kMaxTradeRoutePlots ? kData.m_nPlots : kMaxTradeRoutePlots;
    for (int iPlot = 0; iPlot < nPlots; ++iPlot) {
        kRoute.m_paiPlots[iPlot][0] = kData.m_aiPlots[iPlot][0];
        kRoute.m_paiPlots[iPlot][1] = kData.m_aiPlots[iPlot][1];
    }
    kRoute.m_nPlots = nPlots;
    g_bTradeRoutesDirty = true;
}

/// 0x089f1db6
void TradeVisSystem::UpdateTradeRouteDirection_Handler(EventTemplate<Event_Int2Type<118>, UpdateTradeRouteDirection_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    const UpdateTradeRouteDirection_EventData& kData = reinterpret_cast<const UpdateTradeRouteDirection_EventData&>(kEvent);
    // Free slots keep their route id, so they are updated too; the dirty flag is not raised.
    for (int iSlot = g_nTradeRouteSlots - 1; iSlot >= 0; --iSlot) {
        if (g_aTradeRoutes[iSlot].m_iRoute == kData.m_iRoute) {
            g_aTradeRoutes[iSlot].m_bForwards = kData.m_bForwards;
        }
    }
}

/// 0x089f1df4
void TradeVisSystem::DestroyTradeRoute_Handler(EventTemplate<Event_Int2Type<119>, DestroyTradeRoute_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    const DestroyTradeRoute_EventData& kData = reinterpret_cast<const DestroyTradeRoute_EventData&>(kEvent);
    for (int iSlot = g_nTradeRouteSlots - 1; iSlot >= 0; --iSlot) {
        if (g_aTradeRoutes[iSlot].m_iRoute == kData.m_iRoute) {
            g_aTradeRoutes[iSlot].m_nPlots = 0;
            g_bTradeRoutesDirty = true;
        }
    }
    if (g_iTradePopupRoute == kData.m_iRoute) {
        g_iTradePopupRoute = 0;
        g_bTradePopupActive = false;
        g_bTradeRoutesDirty = true;
    }
}

/// 0x089f1e5c
void TradeVisSystem::ToggleDisplay_Handler(EventTemplate<Event_Int2Type<120>, ToggleTradeRouteDisplay_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    const ToggleTradeRouteDisplay_EventData& kData = reinterpret_cast<const ToggleTradeRouteDisplay_EventData&>(kEvent);
    if (g_bShowTradeRoutes != kData.m_bShow) {
        g_bShowTradeRoutes = kData.m_bShow;
        g_bTradeRoutesDirty = true;
    }
}

/// 0x089f1e86
void TradeVisSystem::TogglePopup_Handler(EventTemplate<Event_Int2Type<121>, ToggleTradeRoutePopup_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    const ToggleTradeRoutePopup_EventData& kData = reinterpret_cast<const ToggleTradeRoutePopup_EventData&>(kEvent);
    if (g_bTradePopupActive == kData.m_bActive && g_iTradePopupRoute == kData.m_iRoute) {
        return;
    }
    g_bTradePopupActive = kData.m_bActive;
    g_iTradePopupRoute = kData.m_iRoute;
    g_bTradeRoutesDirty = true;
}

/// 0x089f1eca
void TradeVisSystem::Shutdown() {
    TerrainEventDelegate kDelegate;
    kDelegate.m_pObject = this;
    void (TradeVisSystem::*pfnDisplay)(ToggleTradeRouteDisplayEvent&) = &TradeVisSystem::ToggleDisplay_Handler;
    memcpy(&kDelegate.m_pfnMethod, &pfnDisplay, sizeof(pfnDisplay));
    Fun_089f4d98(&kDelegate);
    kDelegate.m_pObject = this;
    void (TradeVisSystem::*pfnPopup)(ToggleTradeRoutePopupEvent&) = &TradeVisSystem::TogglePopup_Handler;
    memcpy(&kDelegate.m_pfnMethod, &pfnPopup, sizeof(pfnPopup));
    Fun_089f4b6c(&kDelegate);
    kDelegate.m_pObject = this;
    void (TradeVisSystem::*pfnNewRoute)(NewTradeRouteEvent&) = &TradeVisSystem::NewTradeRoute_Handler;
    memcpy(&kDelegate.m_pfnMethod, &pfnNewRoute, sizeof(pfnNewRoute));
    Fun_089f4940(&kDelegate);
    kDelegate.m_pObject = this;
    void (TradeVisSystem::*pfnDirection)(UpdateTradeRouteDirectionEvent&) = &TradeVisSystem::UpdateTradeRouteDirection_Handler;
    memcpy(&kDelegate.m_pfnMethod, &pfnDirection, sizeof(pfnDirection));
    Fun_089f4714(&kDelegate);
    kDelegate.m_pObject = this;
    void (TradeVisSystem::*pfnDestroy)(DestroyTradeRouteEvent&) = &TradeVisSystem::DestroyTradeRoute_Handler;
    memcpy(&kDelegate.m_pfnMethod, &pfnDestroy, sizeof(pfnDestroy));
    Fun_089f44e8(&kDelegate);

    ReleaseTradeRouteSurfaces();
    Fun_0866fb7e(FireGrafix::CRenderer9::m_IBList, g_hTradeRouteIB);
    Fun_0866fd26(FireGrafix::CRenderer9::m_VBList, g_hTradeRouteVB);
    g_hTradeRouteVB = 0;
    g_hTradeRouteIB = 0;
    g_uTradeRouteVertexCount = 0;
    g_uTradeRouteIndexCount = 0;
    delete g_pTradeArrowShader;
    for (int iSlot = g_nTradeRouteSlots - 1; iSlot >= 0; --iSlot) {
        free(g_aTradeRoutes[iSlot].m_paiPlots);
    }
    g_nTradeRouteSlots = 0;
}

/// 0x089f2070
void TradeVisSystem::StartupGame() {
    std::auto_ptr<ICvGameContext1> pContext = GameCore::GetContext();
    for (int iPlayer = 0; iPlayer < 0x40; ++iPlayer) {
        std::auto_ptr<ICvPlayer1> pPlayer = GameCore::GetPlayer(static_cast<PlayerTypes>(iPlayer));
        FGXVector4& kColor = g_aTradeRoutePlayerColors[iPlayer];
        kColor.x = 1.0f;
        kColor.y = 1.0f;
        kColor.z = 1.0f;
        kColor.w = 1.0f;
        if (pPlayer.get() == NULL || !pPlayer->IsEverAlive()) {
            continue;
        }
        const PlayerColorTypes ePlayerColor = pPlayer->GetPlayerColor();
        if (ePlayerColor <= NO_PLAYERCOLOR) {
            continue;
        }
        std::auto_ptr<ICvPlayerColorInfo1> pPlayerColorInfo(pContext->GetPlayerColorInfo(ePlayerColor));
        if (pPlayerColorInfo.get() == NULL) {
            kColor.x = 0.0f;
            kColor.y = 0.0f;
            kColor.z = 0.0f;
            kColor.w = 1.0f;
            continue;
        }
        float fRed, fGreen, fBlue;
        {
            std::auto_ptr<ICvColorInfo1> pPrimary(pContext->GetColorInfo(pPlayerColorInfo->GetColorTypePrimary()));
            // The secondary colour is fetched and released unused.
            std::auto_ptr<ICvColorInfo1> pSecondary(pContext->GetColorInfo(pPlayerColorInfo->GetColorTypeSecondary()));
            const CvColorA& kPrimary = pPrimary->GetColor();
            fRed = kPrimary.r;
            fGreen = kPrimary.g;
            fBlue = kPrimary.b;
        }
        kColor.x = fRed;
        kColor.y = fGreen;
        kColor.z = fBlue;
        kColor.w = 1.0f;
    }
}

/// 0x089f2328
void TradeVisSystem::ShutdownGame() {
    for (int iSlot = g_nTradeRouteSlots - 1; iSlot >= 0; --iSlot) {
        g_aTradeRoutes[iSlot].m_nPlots = 0;
    }
    g_bTradeRoutesDirty = true;
}

/// 0x089f2026
void ReleaseTradeRouteSurfaces() {
    for (int iSurface = kNumTradeRouteSurfaces - 1; iSurface >= 0; --iSurface) {
        FireGrafix::g_pRenderer->ReleaseSurface(g_ahTradeRouteSurfaces[iSurface]);
        g_ahTradeRouteSurfaces[iSurface] = 0;
    }
}

/// 0x089f235c
HEXVec_i get_unwrapped_pos(HEXVec_i const& a1, HEXVec_i const& a2) {
    // Looks for the copy of a2 that lies next to a1 across the world wrap.
    for (int iTranslation = 5; iTranslation >= 0; --iTranslation) {
        HEXVec_i kCandidate;
        kCandidate.x = HEXVec_i::TRANSLATIONS[iTranslation].x + a1.x;
        kCandidate.y = HEXVec_i::TRANSLATIONS[iTranslation].y + a1.y;
        kCandidate.z = HEXVec_i::TRANSLATIONS[iTranslation].z + a1.z;
        HEXVec_i kWrapped = kCandidate;
        Fun_0865f23e(kWrapped);
        if (IsSameHex(kWrapped, a2)) {
            return kCandidate;
        }
    }
    return a2;
}

/// 0x089f2d95 fastcc
void CreateTradeRouteSurface(unsigned int* phSurface, unsigned int uStartIndex, int nIndices, unsigned short uTexture) {
    FireGrafix::FGXSurface* pSurface;
    FireGrafix::g_pRenderer->CreateSurface(&pSurface, phSurface, kTradeRouteSurfaceType);
    memset(pSurface, 0, sizeof(*pSurface));
    pSurface->field_0 = kTradeRouteSurfaceType;
    pSurface->field_18[0] = uTexture;
    pSurface->field_38[0] = 0;
    pSurface->field_60 = 1;
    pSurface->m_hIB = g_hTradeRouteIB;
    pSurface->m_hVB = g_hTradeRouteVB;
    pSurface->field_10 = uStartIndex;
    pSurface->field_c = nIndices / 3;
}

/// 0x089f2e4a
void UnlockTradeRouteBuffers() {
    FireGrafix::g_pRenderer->UnlockVB(g_hTradeRouteVB, 0, g_uTradeRouteVertexCount);
    FireGrafix::g_pRenderer->UnlockIB(g_hTradeRouteIB, 0, g_uTradeRouteIndexCount);
    g_pTradeRouteVertices = NULL;
    g_pTradeRouteIndices = NULL;
}

/// 0x089f439e
void AllocateRouteGeometry(TradeRouteVertex** ppVertices, unsigned int** ppIndices, unsigned int* puFirstVertex, int nVertices, int nIndices) {
    if (static_cast<int>(g_uTradeRouteVertexCount) + nVertices >= static_cast<int>(kTradeRouteVertexCapacity) ||
        static_cast<int>(g_uTradeRouteIndexCount) + nIndices >= static_cast<int>(kTradeRouteIndexCapacity)) {
        *ppIndices = NULL;
        *ppVertices = NULL;
        return;
    }
    *ppIndices = static_cast<unsigned int*>(g_pTradeRouteIndices) + g_uTradeRouteIndexCount;
    *ppVertices = g_pTradeRouteVertices + g_uTradeRouteVertexCount;
    g_uTradeRouteIndexCount += nIndices;
    *puFirstVertex = g_uTradeRouteVertexCount;
    g_uTradeRouteVertexCount += nVertices;
}

/// 0x089f4420
void EvaluateHermite(FGXVector2* pvOut, FGXVector2 const* pvP0, FGXVector2 const* pvP1, FGXVector2 const* pvT0, FGXVector2 const* pvT1, float fT) {
    const float fT2 = fT * fT;
    const float fT3 = fT2 * fT;
    const float fH00 = (fT3 + fT3) - 3.0f * fT2 + 1.0f;
    const float fH01 = -2.0f * fT3 + 3.0f * fT2;
    const float fH10 = (fT3 - (fT2 + fT2)) + fT;
    const float fH11 = fT3 - fT2;
    pvOut->x = pvT1->x * fH11 + (pvT0->x * fH10 + (pvP1->x * fH01 + pvP0->x * fH00));
    pvOut->y = fH11 * pvT1->y + (fH10 * pvT0->y + (fH01 * pvP1->y + fH00 * pvP0->y));
}

/// 0x089f3eba fastcc
bool BuildRouteStrip(float* pfV, FGXVector2* pvEnd, FGXVector2* pvNormal, FGXVector2 const* pvP0, FGXVector2 const* pvP1, FGXVector2 const* pvT0, FGXVector2 const* pvT1, FGXVector4 const* pColor, float fStep) {
    unsigned int uFirstVertex;
    unsigned int* puIndices;
    TradeRouteVertex* pVertices;
    AllocateRouteGeometry(&pVertices, &puIndices, &uFirstVertex, kStripVertices, kStripIndices);
    if (pVertices == NULL || puIndices == NULL) {
        return true;
    }
    const unsigned int uRgb = (static_cast<int>(pColor->z * kColorScale) & 0xff) << 16 |
                              (static_cast<int>(pColor->y * kColorScale) & 0xff) << 8 |
                              (static_cast<int>(pColor->x * kColorScale) & 0xff);
    const unsigned int uRgba = static_cast<int>(pColor->w * kColorScale) << 24 | uRgb;
    FGXVector2 vCenter = {0.0f, 0.0f};
    float fEdgeX = 0.0f;
    float fEdgeY = 0.0f;
    float fT = 0.0f;
    for (int iStation = 0; iStation < kStripStations; ++iStation) {
        EvaluateHermite(&vCenter, pvP0, pvP1, pvT0, pvT1, fT);
        const float fT2 = fT * fT;
        const float fWeightP0 = fT2 * 6.0f - fT * 6.0f;
        const float fWeightP1 = fT2 * -6.0f + fT * 6.0f;
        const float fWeightT0 = fT * -4.0f + fT2 * 3.0f + 1.0f;
        const float fWeightT1 = fT2 * 3.0f + fT * -2.0f;
        const float fTangentX = pvT1->x * fWeightT1 + pvT0->x * fWeightT0 + pvP1->x * fWeightP1 + pvP0->x * fWeightP0;
        const float fTangentY = fWeightT1 * pvT1->y + fWeightT0 * pvT0->y + fWeightP1 * pvP1->y + fWeightP0 * pvP0->y;
        const float fLength = sqrtf(fTangentX * fTangentX + fTangentY * fTangentY);
        const float fNormalX = -fTangentY / fLength;
        const float fNormalY = fTangentX / fLength;
        fEdgeX = fNormalX * kShaftHalfWidth;
        fEdgeY = fNormalY * kShaftHalfWidth;
        const float fFeatherX = fNormalX * kShaftFeatherHalfWidth;
        const float fFeatherY = fNormalY * kShaftFeatherHalfWidth;
        const float fV = fT * kVPerUnitParam;
        TradeRouteVertex* pRow = pVertices + iStation * 4;
        SetRouteVertex(pRow[0], vCenter.x + fFeatherX, vCenter.y + fFeatherY, uRgb, 0.66724855f, fV);
        SetRouteVertex(pRow[1], vCenter.x + fEdgeX, vCenter.y + fEdgeY, uRgba, 0.65625f, fV);
        SetRouteVertex(pRow[2], vCenter.x - fEdgeX, vCenter.y - fEdgeY, uRgba, 0.34375f, fV);
        SetRouteVertex(pRow[3], vCenter.x - fFeatherX, vCenter.y - fFeatherY, uRgb, 0.3328125f, fV);
        fT += fStep;
    }
    for (int iRow = 0; iRow < kStripStations - 1; ++iRow) {
        for (int iIndex = 0; iIndex < 18; ++iIndex) {
            puIndices[iRow * 18 + iIndex] = kStripRowIndices[iIndex] + uFirstVertex + iRow * 4;
        }
    }
    *pvEnd = vCenter;
    const float fEdgeLength = sqrtf(fEdgeY * fEdgeY + fEdgeX * fEdgeX);
    pvNormal->y = fEdgeY / fEdgeLength;
    pvNormal->x = fEdgeX / fEdgeLength;
    *pfV = -fT;
    return false;
}

/// 0x089f3ac8 fastcc
bool BuildRouteSegment(float* pfV, HEXVec_i* pPrev, HEXVec_i* pCur, HEXVec_i* pNext, HEXVec_i* pNext2, FGXVector4 const* pColor) {
    if (!IsSameHex(*pPrev, *pCur)) {
        *pPrev = get_unwrapped_pos(*pCur, *pPrev);
    }
    if (IsSameHex(*pNext, *pNext2)) {
        *pNext = get_unwrapped_pos(*pCur, *pNext);
        *pNext2 = *pNext;
    } else {
        *pNext = get_unwrapped_pos(*pCur, *pNext);
        *pNext2 = get_unwrapped_pos(*pNext, *pNext2);
    }
    const FGXVector2 vCur = GetHexWorldLocation(*pCur);
    const FGXVector2 vNext = GetHexWorldLocation(*pNext);
    const FGXVector2 vPrev = GetHexWorldLocation(*pPrev);
    const FGXVector2 vNext2 = GetHexWorldLocation(*pNext2);
    // Catmull-Rom tangents of the piece from vCur to vNext.
    const FGXVector2 vTangentStart = {(vNext.x - vPrev.x) * 0.5f, (vNext.y - vPrev.y) * 0.5f};
    const FGXVector2 vTangentEnd = {(vNext2.x - vCur.x) * 0.5f, (vNext2.y - vCur.y) * 0.5f};
    FGXVector2 vEnd;
    FGXVector2 vNormal;
    return BuildRouteStrip(pfV, &vEnd, &vNormal, &vCur, &vNext, &vTangentStart, &vTangentEnd, pColor, kStripStep);
}

/// 0x089f2faa fastcc
void BuildRouteEndCap(float* pfV, HEXVec_i* pPrev, HEXVec_i* pCur, HEXVec_i* pEnd, FGXVector4 const* pColor) {
    if (IsSameHex(*pPrev, *pCur)) {
        *pEnd = get_unwrapped_pos(*pCur, *pEnd);
    } else {
        *pPrev = get_unwrapped_pos(*pCur, *pPrev);
    }
    const FGXVector2 vCur = GetHexWorldLocation(*pCur);
    const FGXVector2 vEnd = GetHexWorldLocation(*pEnd);
    const FGXVector2 vPrev = GetHexWorldLocation(*pPrev);
    const FGXVector2 vTangentStart = {(vEnd.x - vPrev.x) * 0.5f, (vEnd.y - vPrev.y) * 0.5f};
    const FGXVector2 vTangentEnd = {(vEnd.x - vCur.x) * 0.5f, (vEnd.y - vCur.y) * 0.5f};
    FGXVector2 vStubEnd;
    FGXVector2 vNormal;
    if (BuildRouteStrip(pfV, &vStubEnd, &vNormal, &vCur, &vEnd, &vTangentStart, &vTangentEnd, pColor, kStubStep)) {
        return;
    }
    float fForwardX = vEnd.x - vStubEnd.x;
    float fForwardY = vEnd.y - vStubEnd.y;
    const float fForwardLength = sqrtf(fForwardY * fForwardY + fForwardX * fForwardX);
    unsigned int uFirstVertex;
    unsigned int* puIndices;
    TradeRouteVertex* pVertices;
    AllocateRouteGeometry(&pVertices, &puIndices, &uFirstVertex, kArrowHeadVertices, kArrowHeadIndices);
    if (pVertices == NULL || puIndices == NULL) {
        return;
    }
    fForwardX /= fForwardLength;
    fForwardY /= fForwardLength;
    const unsigned int uRgb = (static_cast<int>(pColor->z * kColorScale) & 0xff) << 16 |
                              (static_cast<int>(pColor->y * kColorScale) & 0xff) << 8 |
                              (static_cast<int>(pColor->x * kColorScale) & 0xff);
    const unsigned int uRgba = static_cast<int>(pColor->w * kColorScale) << 24 | uRgb;
    const float fV = *pfV + *pfV;
    const float fCenterX = vStubEnd.x;
    const float fCenterY = vStubEnd.y;
    // A vertex at (fAcross, fAlong) in the frame of the stub end: across the route along the normal, along it on the tangent.
    const auto setVertex = [&](int iVertex, float fAcross, float fAlong, unsigned int uColor, float fU, float fVOffset) {
        SetRouteVertex(pVertices[iVertex],
                       fCenterX + fAcross * vNormal.x + fAlong * vNormal.y,
                       fCenterY + fAcross * vNormal.y - fAlong * vNormal.x,
                       uColor, fU, fV + fVOffset);
    };
    setVertex(0, kHeadHalfWidth, kHeadBackShift, uRgba, 0.8125f, -0.015625f);
    SetRouteVertex(pVertices[1], fCenterX + fForwardX * kHeadTipLength, fCenterY + fForwardY * kHeadTipLength, uRgba, 0.5f, fV - 0.74375f);
    setVertex(2, -kHeadHalfWidth, kHeadBackShift, uRgba, 0.1875f, -0.015625f);
    setVertex(3, -kShaftHalfWidth, kHeadBackShift, uRgba, 0.34375f, -0.015625f);
    setVertex(4, kShaftHalfWidth, kHeadBackShift, uRgba, 0.65625f, -0.015625f);
    setVertex(5, kHeadFeatherHalfWidth, kHeadFeatherBackShift, uRgb, 0.8281317f, -0.0046875f);
    SetRouteVertex(pVertices[6], fCenterX + fForwardX * kHeadTipFeatherLength, fCenterY + fForwardY * kHeadTipFeatherLength, uRgb, 0.5f, fV - 0.76923436f);
    setVertex(7, -kHeadFeatherHalfWidth, kHeadFeatherBackShift, uRgb, 0.1718683f, -0.0046875f);
    setVertex(8, -kShaftFeatherHalfWidth, kHeadFeatherBackShift, uRgb, 0.3328125f, -0.0046875f);
    setVertex(9, kShaftFeatherHalfWidth, kHeadFeatherBackShift, uRgb, 0.6671875f, -0.0046875f);
    setVertex(10, -kShaftFeatherHalfWidth, 0.0f, uRgb, 0.3328125f, 0.0f);
    setVertex(11, -kShaftHalfWidth, 0.0f, uRgba, 0.34375f, 0.0f);
    setVertex(12, kShaftHalfWidth, 0.0f, uRgba, 0.65625f, 0.0f);
    setVertex(13, kShaftFeatherHalfWidth, 0.0f, uRgb, 0.6671875f, 0.0f);
    for (int iIndex = 0; iIndex < kArrowHeadIndices; ++iIndex) {
        puIndices[iIndex] = kArrowHeadTriangles[iIndex] + uFirstVertex;
    }
    *pfV += kStubVAdvance;
}

/// 0x089f2983
void BuildRouteGeometry(TradeRouteVis* pRoute, bool bOwned) {
    HEXVec_i aHexes[kMaxTradeRoutePlots] = {};
    const int nPlots = pRoute->m_nPlots;
    if (nPlots < 2) {
        return;
    }
    FGXVector4 kColor = {1.0f, 1.0f, 1.0f, 1.0f};
    if (pRoute->m_iPlayer < 0x40) {
        kColor = g_aTradeRoutePlayerColors[pRoute->m_iPlayer];
    }
    kColor.w = bOwned ? kRouteAlpha : kPopupRouteAlpha;

    HEXMatrix_i const& kGridInverse = *reinterpret_cast<const HEXMatrix_i*>(WorldGlobals::g_GameGrid + 0x40);
    for (int iPlot = 0; iPlot < nPlots; ++iPlot) {
        HEXVec_i kCube;
        kCube.y = pRoute->m_paiPlots[iPlot][1];
        kCube.x = pRoute->m_paiPlots[iPlot][0] - (kCube.y >> 1);
        kCube.z = -(kCube.x + kCube.y);
        const HEXVec_i kHex = Fun_086616f0(kGridInverse, kCube);
        aHexes[iPlot].x = kHex.x;
        aHexes[iPlot].y = kHex.y;
        aHexes[iPlot].z = -(kHex.x + kHex.y);
    }

    // Texture phase carried between the pieces of one route.
    float fV = 0.0f;
    if (nPlots == 2) {
        HEXVec_i kPrev = aHexes[0];
        HEXVec_i kCur = aHexes[0];
        HEXVec_i kEnd = aHexes[1];
        BuildRouteEndCap(&fV, &kPrev, &kCur, &kEnd, &kColor);
        return;
    }
    {
        HEXVec_i kPrev = aHexes[0];
        HEXVec_i kCur = aHexes[0];
        HEXVec_i kNext = aHexes[1];
        HEXVec_i kNext2 = aHexes[2];
        if (BuildRouteSegment(&fV, &kPrev, &kCur, &kNext, &kNext2, &kColor)) {
            return;
        }
    }
    for (int iPlot = 1; iPlot < nPlots - 2; ++iPlot) {
        HEXVec_i kPrev = aHexes[iPlot - 1];
        HEXVec_i kCur = aHexes[iPlot];
        HEXVec_i kNext = aHexes[iPlot + 1];
        HEXVec_i kNext2 = aHexes[iPlot + 2];
        if (BuildRouteSegment(&fV, &kPrev, &kCur, &kNext, &kNext2, &kColor)) {
            return;
        }
    }
    HEXVec_i kPrev = aHexes[nPlots - 3];
    HEXVec_i kCur = aHexes[nPlots - 2];
    HEXVec_i kEnd = aHexes[nPlots - 1];
    BuildRouteEndCap(&fV, &kPrev, &kCur, &kEnd, &kColor);
}

/// 0x089f2428
void TradeVisSystem::Update(float fDeltaTime, bool bStrategicView) {
    const float fScroll = fDeltaTime * kTradeArrowScrollSpeed + g_fTradeArrowScroll;
    g_fTradeArrowScroll = fScroll - floorf(fScroll);
    if (g_bTradeStrategicView != bStrategicView) {
        g_bTradeStrategicView = bStrategicView;
        g_bTradeRoutesDirty = true;
    } else if (!g_bTradeRoutesDirty) {
        return;
    }

    PlayerTypes eActivePlayer;
    {
        std::auto_ptr<ICvGameAsynch1> pGame = GameCore::GetGameAsynch();
        eActivePlayer = pGame->GetActivePlayer();
    }

    // The whole buffer is rebuilt from the route slots on every change.
    g_pTradeRouteVertices = static_cast<TradeRouteVertex*>(FireGrafix::g_pRenderer->LockStaticVB(g_hTradeRouteVB, 0));
    g_pTradeRouteIndices = FireGrafix::g_pRenderer->LockStaticIB(g_hTradeRouteIB, 0);
    g_uTradeRouteVertexCount = 0;
    g_uTradeRouteIndexCount = 0;
    ReleaseTradeRouteSurfaces();

    TradeRoutesPublishBuffers_EventData kBuffers;
    memset(&kBuffers, 0, sizeof(kBuffers));
    const auto addBatch = [&kBuffers](unsigned int& hSurface, unsigned int uStartIndex, unsigned int hTexture) {
        const unsigned int uIndexCount = g_uTradeRouteIndexCount - uStartIndex;
        if (uIndexCount == 0) {
            return;
        }
        CreateTradeRouteSurface(&hSurface, uStartIndex, uIndexCount, hTexture);
        kBuffers.m_hVB = g_hTradeRouteVB;
        kBuffers.m_hIB = g_hTradeRouteIB;
        kBuffers.m_ahTextures[kBuffers.m_nBatches] = hTexture;
        kBuffers.m_auStartIndex[kBuffers.m_nBatches] = uStartIndex;
        kBuffers.m_auTriangleCount[kBuffers.m_nBatches] = static_cast<int>(g_uTradeRouteIndexCount - uStartIndex) / 3;
        ++kBuffers.m_nBatches;
    };

    // Only the active player's routes, one batch per route type; the popup route is left for its own batch.
    if (g_bShowTradeRoutes) {
        for (int iType = TRADE_CONNECTION_INTERNATIONAL; iType < NUM_TRADE_CONNECTION_TYPES && g_nTradeRouteSlots != 0; ++iType) {
            const unsigned int uStartIndex = g_uTradeRouteIndexCount;
            for (int iSlot = g_nTradeRouteSlots - 1; iSlot >= 0; --iSlot) {
                TradeRouteVis& kRoute = g_aTradeRoutes[iSlot];
                if (g_bTradePopupActive && kRoute.m_iRoute == g_iTradePopupRoute) {
                    continue;
                }
                if (kRoute.m_eType == iType && kRoute.m_iPlayer == eActivePlayer) {
                    BuildRouteGeometry(&kRoute, true);
                }
            }
            addBatch(g_ahTradeRouteSurfaces[kFirstRouteTypeSurface + iType], uStartIndex, g_ahTradeArrowTextures[iType]);
        }
    }

    // The popup route is drawn whoever owns it, and even when the display toggle is off.
    if (g_bTradePopupActive) {
        for (int iSlot = g_nTradeRouteSlots - 1; iSlot >= 0; --iSlot) {
            TradeRouteVis& kRoute = g_aTradeRoutes[iSlot];
            if (kRoute.m_iRoute != g_iTradePopupRoute) {
                continue;
            }
            const unsigned int uStartIndex = g_uTradeRouteIndexCount;
            BuildRouteGeometry(&kRoute, false);
            int iTexture = kPopupTextureBase + TRADE_CONNECTION_INTERNATIONAL;
            if (kRoute.m_eType == TRADE_CONNECTION_FOOD) {
                iTexture = kPopupTextureBase + TRADE_CONNECTION_FOOD;
            } else if (kRoute.m_eType == TRADE_CONNECTION_PRODUCTION) {
                iTexture = kPopupTextureBase + TRADE_CONNECTION_PRODUCTION;
            }
            addBatch(g_ahTradeRouteSurfaces[kPopupSurface], uStartIndex, g_ahTradeArrowTextures[iTexture]);
            break;
        }
    }

    if (bStrategicView) {
        const float* pfScale = reinterpret_cast<const float*>(g_cvStrategicViewVisSystem + kStrategicViewMapScale);
        const float* pfOrigin = reinterpret_cast<const float*>(g_cvStrategicViewVisSystem + kStrategicViewMapOrigin);
        for (unsigned int uVertex = 0; uVertex < g_uTradeRouteVertexCount; ++uVertex) {
            TradeRouteVertex& kVertex = g_pTradeRouteVertices[uVertex];
            kVertex.m_fX = (pfOrigin[0] - kVertex.m_fX) / -pfScale[0];
            kVertex.m_fY = (pfOrigin[1] - kVertex.m_fY) / -pfScale[1];
            kVertex.m_fZ = kStrategicViewTradeRouteZ;
        }
        UnlockTradeRouteBuffers();
        // The strategic view draws the same batches itself (cvStrategicViewTracker, event 122).
        const unsigned int uOffset = *reinterpret_cast<const int*>(g_EventSystemLocalMachine) * kLocalMachineStride;
        PfnDispatchEvent pfnDispatch = *reinterpret_cast<PfnDispatchEvent*>(g_EventSystemLocalMachine + uOffset + kPublishBuffersEventSlot);
        void* pContext = *reinterpret_cast<void**>(g_EventSystemLocalMachine + uOffset + kPublishBuffersEventSlot + 4);
        pfnDispatch(&kBuffers, pContext);
    } else {
        UnlockTradeRouteBuffers();
    }
    g_bTradeRoutesDirty = false;
}

/// 0x089f2ec8
void TradeVisSystem::Render(FireGrafix::CCommandStream* pCommandStream, int iCamera) {
    if (!g_bTradePopupActive && !g_bShowTradeRoutes) {
        return;
    }
    FireGrafix::TradeArrowGlobals* pGlobals = static_cast<FireGrafix::TradeArrowGlobals*>(FireGrafix::g_pTradeArrowGlobals->AliasGlobal(pCommandStream));
    char* pConstants = static_cast<char*>(pGlobals->LockGlobal(0));
    memcpy(pConstants, &FrameGlobals::g_OverlayProjection[iCamera], sizeof(D3DXMATRIX));
    *reinterpret_cast<float*>(pConstants + kTradeArrowScrollConstant) = g_fTradeArrowScroll;
    pGlobals->Unlock(0);
    // Route types 2, 1, 0, then the popup route last.
    for (int iSurface = kNumTradeRouteSurfaces - 1; iSurface >= 0; --iSurface) {
        if (g_ahTradeRouteSurfaces[iSurface] != 0) {
            pCommandStream->DrawSurface(g_ahTradeRouteSurfaces[iSurface], g_pTradeArrowShader->m_hInstance, 0);
        }
    }
    FireGrafix::FGXConstantSetManager::ReleaseConstantSet(pGlobals->field_34);
}
