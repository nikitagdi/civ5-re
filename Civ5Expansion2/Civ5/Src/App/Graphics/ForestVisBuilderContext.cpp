/// origin binary
#include "Civ5/Src/App/Graphics/ForestVisBuilderContext.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/TerrainData.h"
#include "Civ5/Src/App/UI/LuaContext.h"
#include "Civ5/Src/App/UI/TextControl.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/TextureData.h"
#include <stdio.h>
#include <string.h>

// Asset path prefix per system (ColorKeyData::m_iSystem).
static char const* const s_apszKeySystems[5] = {
    "assets\\ColorKeys\\Terrain",
    "assets\\ColorKeys\\TerrainFOW",
    "assets\\ColorKeys\\TerrainShadow",
    "assets\\ColorKeys\\Buildings",
    "assets\\ColorKeys\\Rivers",
};

// Style class suffix per type (ColorKeyData::m_iType).
static char const* const s_apszKeyTypes[5] = {
    "Generic",
    "America",
    "Asia",
    "Africa",
    "Europe",
};

/// 0x086717aa
void ColorKeySystem::LoadKeyTexture(unsigned int a1, unsigned int a2, char* a3) {
    ColorKeyData* pKeys = reinterpret_cast<ColorKeyData*>(this);
    FileLoadBuffer buffer;
    Fun_08669c48(&buffer, a3);
    if (Fun_08669d48(&buffer) != 0) {
        unsigned int uSize = Fun_08669d48(&buffer);
        FireGrafix::TextureData texture(buffer.m_pData, uSize, 0x21);
        memcpy(pKeys->m_apKeyTextures[a1][a2]->m_aMips[0].m_apFaces[0], texture.m_aMips[0].m_apFaces[0], 0x20000);
        FireGrafix::g_pRenderer->RecreateTexture(pKeys->m_auKeyHandles[a1][a2], pKeys->m_apKeyTextures[a1][a2], false);
    }
    Fun_08669d90(&buffer);
}

/// 0x0867166c
void ColorKeySystem::OnScreenShotTaken(GameCoreEventTemplate<Event_Int2Type<28>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1) {
    ColorKeyData* pKeys = reinterpret_cast<ColorKeyData*>(this);
    if (pKeys->m_bSavePending && RenderGlobals::g_pScreenShot != nullptr) {
        FireGrafix::TextureData screen(RenderGlobals::g_pScreenShot, 0x15);
        FireGrafix::TextureData key(pKeys->m_apKeyTextures[pKeys->m_iSystem][pKeys->m_iType], 0x15);
        memcpy(screen.m_aMips[0].m_apFaces[0], key.m_aMips[0].m_apFaces[0], 0x20000);
        char szFile[260];
        sprintf_s(szFile, sizeof(szFile), "%s_%s_ColorKey.tga", s_apszKeySystems[pKeys->m_iSystem], s_apszKeyTypes[pKeys->m_iType]);
        screen.SaveTGA(szFile);
    }
    pKeys->m_bSavePending = false;
}

/// 0x08671afe
void ColorKeySystem::SystemSelect(unsigned int a1, unsigned int a2) {
    ColorKeyData* pKeys = reinterpret_cast<ColorKeyData*>(this);
    unsigned int uSystem = static_cast<unsigned int>(pKeys->m_iSystem) + a1;
    pKeys->m_iSystem = static_cast<int>(uSystem < 5u ? uSystem : 0u);
    ControlBase* pLabel = pKeys->m_pLuaContext->LookUpControl("DebugMenu/ColorKey_Panel/System_Label");
    if (pLabel != nullptr) {
        static_cast<TextControl*>(pLabel)->SetText(s_apszKeySystems[pKeys->m_iSystem]);
    }
}

/// 0x08671b66
void ColorKeySystem::TypeSelect(unsigned int a1, unsigned int a2) {
    ColorKeyData* pKeys = reinterpret_cast<ColorKeyData*>(this);
    unsigned int uType = static_cast<unsigned int>(pKeys->m_iType) + a1;
    pKeys->m_iType = static_cast<int>(uType < 5u ? uType : 0u);
    ControlBase* pLabel = pKeys->m_pLuaContext->LookUpControl("DebugMenu/ColorKey_Panel/Type_Label");
    if (pLabel != nullptr) {
        static_cast<TextControl*>(pLabel)->SetText(s_apszKeyTypes[pKeys->m_iType]);
    }
}

/// 0x08671bce
void ColorKeySystem::ReloadColorKey(unsigned int a1, unsigned int a2) {
    ColorKeyData* pKeys = reinterpret_cast<ColorKeyData*>(this);
    char szFile[260];
    sprintf_s(szFile, sizeof(szFile), "%s_%s_ColorKey.tga", s_apszKeySystems[pKeys->m_iSystem], s_apszKeyTypes[pKeys->m_iType]);
    LoadKeyTexture(pKeys->m_iSystem, pKeys->m_iType, szFile);
}

/// 0x08671ca6
void ColorKeySystem::ResetColorKey(unsigned int a1, unsigned int a2) {
    ColorKeyData* pKeys = reinterpret_cast<ColorKeyData*>(this);
    FireGrafix::TextureData* pTexture = pKeys->m_apKeyTextures[pKeys->m_iSystem][pKeys->m_iType];
    pTexture->Fill3DTexture(ColorIdentity, nullptr);
    FireGrafix::g_pRenderer->RecreateTexture(pKeys->m_auKeyHandles[pKeys->m_iSystem][pKeys->m_iType], pTexture, false);
}
