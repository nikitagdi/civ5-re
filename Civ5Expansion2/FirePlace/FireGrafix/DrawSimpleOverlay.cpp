/// origin guess
#include "FirePlace/FireGrafix/DrawSimpleOverlay.h"
#include "Civ5/Src/App/NetProxyConnectionResponseData.h"
#include "Civ5/Src/App/UI/UIRenderBuffer.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/TextureData.h"

namespace FireGrafix {
/// 0x0877f98a
DrawSimpleOverlay::DrawSimpleOverlay() {
    m_hInstance = 0;
    field_8 = 0;
    unsigned int auParamSetIdsA[6] = {};
    unsigned int auParamSetIdsB[4] = {};
    unsigned int auParamSetIdsC[4] = {};
    m_pConstantSet = g_pOverlayGlobals;
    auParamSetIdsA[0] = g_pOverlayGlobals->field_34;
    unsigned int const uDynamicSet =
        FGXConstantSetManager::CreateConstantSet(static_cast<CONSTANTTYPE>(0), 0xe0, 4, 0xc, 0xc, 0, 2, 0);
    m_pDynamicConstantSet = &reinterpret_cast<FGXConstantSet*>(&CRenderer9::m_kCSManager[0x2020c8])[uDynamicSet];
    m_pszName = "DrawSimpleOverlay";
    m_uPackage = 0x43;
    m_hInstance = g_pRenderer->CreateShaderPackageInstance(auParamSetIdsA, auParamSetIdsB, auParamSetIdsC,
                                                           m_pDynamicConstantSet->field_34, 1, 0, 0, 0x43);
}
}
