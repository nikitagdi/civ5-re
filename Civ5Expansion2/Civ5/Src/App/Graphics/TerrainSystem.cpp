/// origin binary
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/cvTerrainIceSystem.h"
#include "Civ5/Src/App/cvFloodplainsSystem.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/UI/LuaContext.h"
#include "Civ5/Src/App/UI/TextControl.h"
#include "FirePlace/FireGrafix/CCommandStream.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "Civ5/Src/App/DynamicHexGrid.h"
#include "Civ5/Src/App/TerrainStencil.h"
#include "Civ5/Src/App/Engine/cvGameplayRoadHandler.h"
#include "Civ5/Src/App/Engine/cvMarshSimSystem.h"
#include "Civ5/Src/App/Engine/cvRiverSimSystem.h"
#include "Civ5/Src/App/Graphics/CoastGrid.h"
#include "Civ5/Src/App/Graphics/cvCoastMeshEngine.h"
#include "Civ5/Src/App/Graphics/cvCoastSplineSystem.h"
#include "Civ5/Src/App/Graphics/cvTerrainBlend.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "Civ5/Src/App/ButtonControlBase.h"
#include "Civ5/Src/App/ControlBase.h"
#include "Civ5/Src/App/EventTemplate.h"
#include "Civ5/Src/App/Graphics/ForestVisBuilderContext.h"
#include "Civ5/Src/App/Graphics/RiverVisSystem.h"
#include "Civ5/Src/App/NexusSupport/Listener.h"
#include "Civ5/Src/App/NetProxyConnectionResponseData.h"
#include "Civ5/Src/App/UI/UIRenderBuffer.h"
#include "FirePlace/FireGrafix/FGXShaderPackageInstanceView.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "FirePlace/FireGrafix/ShadowShader.h"
#include "FirePlace/FireGrafix/WaterShader.h"
#include "FirePlace/FireGrafix/WorldCurrents.h"
#include "FirePlace/FireGrafix/TextureData.h"
#include "FirePlace/FireWorks/FFile.h"
#include "FirePlace/FireWorks/FJob.h"
#include "FirePlace/FireWorks/FLeanMap.h"
#include "FirePlace/FireWorks/FMemHooks.h"
#include "FirePlace/FireWorks/FSingleton.h"
#include <math.h>
#include <new>
#include <stdlib.h>
#include <string.h>

static const double kDegreesToRadians = 0.017453292519943295;

namespace {

typedef FFastVector<FGXVector2, true, 43u, 259u> SplinePointVector;
typedef FFastVector<SplineRange, true, 43u, 259u> SplineRangeVector;

// sqrt(3)/2, the hex row spacing.
const float kHexHeightRatio = 0.866f;
// The middle one of the five FinalLevelSplines levels.
const unsigned int kCoastlineLevel = 2;
const double kMinCornerDistance = 0.01;
const int kLoopSearchWindow = 50;
const float kIntersectEpsilon = 1.0e-4f;

}  // namespace

/// 0x0867e2c0
TerrainSystem::TerrainSystem() {
    // The decal layer, the water system's tail and the terrain data have no constructors: they are cleared here.
    memset(&m_DecalLayer.unk_4[8], 0, 12);
    m_DecalLayer.m_grid.m_fMinX = 0.0f;
    m_DecalLayer.m_grid.m_fMinY = 0.0f;
    m_DecalLayer.m_grid.m_fMaxX = 0.0f;
    m_DecalLayer.m_grid.m_fMaxY = 0.0f;
    m_DecalLayer.m_grid.m_fInvCellW = 0.0f;
    m_DecalLayer.m_grid.m_fInvCellH = 0.0f;
    m_DecalLayer.m_grid.m_pCellHeads = nullptr;
    m_DecalLayer.m_grid.m_cellsX = 0;
    m_DecalLayer.m_grid.m_cellsY = 0;
    m_DecalLayer.m_grid.m_pNodePool = nullptr;
    Fun_0867e1d6(&m_DecalLayer.m_grid);
    m_DecalLayer.m_bigDecals.m_freeHead = 0xfffffff;
    m_DecalLayer.m_bigDecals.m_pItems = nullptr;
    m_DecalLayer.m_bigDecals.m_numAllocated = 0;
    m_DecalLayer.m_bigDecals.m_itemCapacity = 0;
    m_DecalLayer.m_bigDecals.field_c = 0;
    m_DecalLayer.m_bigDecals.m_numUsed = 0;
    m_DecalLayer.m_bigDecals.m_usedHead = 0xfffffff;
    m_DecalLayer.m_bigDecals.m_usedTail = 0xfffffff;
    m_DecalLayer.m_numTypes = 0;
    m_DecalLayer.m_numBanks = 0;
    m_DecalLayer.m_pBanks = nullptr;
    m_DecalLayer.field_bc = 0;
    m_DecalLayer.field_c8 = 0x40;
    m_DecalLayer.m_pTypes = reinterpret_cast<DecalType*>(m_DecalLayer.unk_cc);
    m_DecalLayer.m_numDrawQueue = 0;
    m_DecalLayer.m_pDrawQueue = nullptr;
    m_DecalLayer.field_4d8 = 0;
    m_DecalLayer.m_gridNodes.field_0 = 0xfffffff;
    m_DecalLayer.m_gridNodes.field_10 = 0;
    m_DecalLayer.m_gridNodes.field_c = 0;
    m_DecalLayer.m_gridNodes.m_pNodes = nullptr;
    m_DecalLayer.m_gridNodes.field_4 = 0;
    memset(&m_DecalLayer.m_scratchVertices, 0, sizeof(m_DecalLayer.m_scratchVertices));
    memset(&m_DecalLayer.m_scratchIndices, 0, sizeof(m_DecalLayer.m_scratchIndices));
    new (unk_50c) cvRiverSimSystem();
    new (unk_276d8) cvMarshSimSystem();
    field_2771c = nullptr;
    field_27718 = nullptr;
    m_iHeightMapWidth = 0;
    field_27720 = nullptr;
    m_iHeightMapHeight = 0;
    memset(&m_WaterSystem.m_vWaterDir0, 0, 0x18);
    new (unk_27818) cvFloodplainsSystem();
    new (unk_529bc) cvTerrainIceSystem();
    m_TerrainData.m_aTextureNames.m_uiSize = 0;
    m_TerrainData.m_aTextureNames.m_pData = nullptr;
    m_TerrainData.m_aTextureImages.m_pData = nullptr;
    m_TerrainData.m_aTextureNames.m_uiCapacity = 0;
    m_TerrainData.m_aTextureImages.m_uiCapacity = 0;
    m_TerrainData.m_aTextureImages.m_uiSize = 0;
    m_TerrainData.m_bKeepImages = false;
    m_TerrainData.m_bLoaded = false;
    m_vShadowDir.y = 0.0f;
    m_vShadowDir.x = 0.0f;
    m_vSunDir.x = 0.0f;
    m_vShadowDir.z = 0.0f;
    m_vSunDir.z = 0.0f;
    m_vSunDir.y = 0.0f;
    m_vReflectDir.y = 0.0f;
    m_vReflectDir.x = 0.0f;
    m_vReflectDir.z = 0.0f;
    m_iInitialized = 0;
    m_pTextureCommandStream = nullptr;
    m_pCommandStream = nullptr;
    m_pJobSet = nullptr;
    m_pOverlayCommandStream = nullptr;
    m_bDiffuse = true;
    m_bRenderWater = true;
    m_bRenderIce = true;
    m_bGPUInitStarted = false;
    m_bWireframe = false;
}

/// 0x0867e770
UNK_RET TerrainSystem::CompactTerrain(char const* a1, char const* a2, bool a3) {
    // The terrain data and the pieces library are heap objects whose constructors are inlined here.
    TerrainData* pTerrainData = static_cast<TerrainData*>(operator new(sizeof(TerrainData)));
    pTerrainData->m_aTextureNames.m_uiSize = 0;
    pTerrainData->m_aTextureNames.m_pData = nullptr;
    pTerrainData->m_aTextureImages.m_pData = nullptr;
    pTerrainData->m_aTextureNames.m_uiCapacity = 0;
    pTerrainData->m_aTextureImages.m_uiCapacity = 0;
    pTerrainData->m_aTextureImages.m_uiSize = 0;
    pTerrainData->m_bKeepImages = false;
    pTerrainData->m_bLoaded = false;
    pTerrainData->Load(false);
    pTerrainData->LoadTerrainDefinition("assets\\terrain\\TerrainTextures\\TerrainDef.csv", false, false);
    WorldGlobals::SetupTerrainPieceGlobals();

    // The allocation is 4 bytes larger than the declared class; the extra dword is the flag a3.
    cvCompressedTerrainPiecesLibrary* pLibrary = static_cast<cvCompressedTerrainPiecesLibrary*>(operator new(0x80188));
    pLibrary->m_uiFileSize = 0;
    pLibrary->m_pFileBuffer = nullptr;
    pLibrary->m_bFileLoaded = false;
    pLibrary->m_uiFileCapacity = 0;
    memcpy(pLibrary->unk_114, &WorldGlobals::g_TerrainPieceToHex, sizeof(pLibrary->unk_114));
    pLibrary->m_aPieces.m_pData = nullptr;
    pLibrary->m_aPieces.m_uiSize = 0;
    pLibrary->m_aPieces.m_uiCapacity = 0;
    pLibrary->m_aPieces.m_pData = static_cast<TerrainPiece*>(FireMallocAlignedNoTracking(0x100 * sizeof(TerrainPiece), 4, "Port/FirePlace/FireWorks/FFastVector.h", 0x55));
    pLibrary->m_aPieces.m_uiCapacity = 0x100;
    pLibrary->m_aByteRuns.m_uiSize = 0;
    pLibrary->m_aByteRuns.m_pData = nullptr;
    pLibrary->m_aHeightRuns.m_pData = nullptr;
    pLibrary->m_aByteRuns.m_uiCapacity = 0;
    pLibrary->m_aHeightRuns.m_uiCapacity = 0;
    pLibrary->m_aHeightRuns.m_uiSize = 0;
    pLibrary->m_aRowOffsets.m_uiSize = 0;
    pLibrary->m_aRowOffsets.m_pData = nullptr;
    pLibrary->m_aRowOffsets.m_uiCapacity = 0;
    *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(pLibrary) + 0x80184) = a3;

    pLibrary->LoadTerrainPieces(false, a1, a2, reinterpret_cast<unsigned long const*>(pTerrainData->m_aHexCode), pTerrainData->m_uiNumTerrains);
    Fun_08683154(pLibrary);
    operator delete(pLibrary);
    pTerrainData->Unload();
    FireFreeAlignedNoTracking(pTerrainData->m_aTextureImages.m_pData);
    FireFreeAlignedNoTracking(pTerrainData->m_aTextureNames.m_pData);
    operator delete(pTerrainData);
}

/// 0x0867e9be
UNK_RET TerrainSystem::Startup() {
    m_pJobSet = new FJobSet();
    m_pCommandStream = FireGrafix::g_pRenderer->CreateCommandStream(0x2000, 0x1000, 0x80000, 0);
    m_pOverlayCommandStream = FireGrafix::g_pRenderer->CreateCommandStream(0x2000, 0x1000, 0x80000, 0);
    m_pTextureCommandStream = FireGrafix::g_pRenderer->CreateCommandStream(0x8000, 0x4000, 0x400000, 0);

    RenderGlobals::g_TexHandles[5] = 0;
    RenderGlobals::g_TexHandles[6] = 0;
    m_bSpecular = GraphicsSettings::TerrainSettings::g_uTerrainDetailLevel == 3;
    m_fSunYaw = 90.0f;
    m_fSunPitch = 25.0f;
    m_vShadowDir.x = 0.4330127f;
    m_vShadowDir.y = -0.25000003f;
    m_vShadowDir.z = 0.8660254f;
    m_vSunDir.x = 1.8473232e-08f;
    m_vSunDir.y = -0.42261827f;
    m_vSunDir.z = 0.90630776f;
    m_fReflectYaw = 80.0f;
    m_fReflectPitch = 135.0f;
    m_vReflectDir.x = -0.12278783f;
    m_vReflectDir.y = -0.6963642f;
    m_vReflectDir.z = -0.70710677f;
    m_fReflectAmount = 0.5f;
    BuildShadowBuffers();

    unsigned int hOceanCurrents = 0;
    if (GraphicsSettings::TerrainSettings::g_uWaterQuality != 0) {
        hOceanCurrents = FireGrafix::g_pRenderer->CreateRenderTexture(0x100, 0x100, 1, 0x15, "Water Currents");
    }
    RenderGlobals::g_TexHandles[8] = hOceanCurrents;
    m_WaterSystem.Startup(13000);
    m_DecalLayer.Startup();
    reinterpret_cast<cvRiverSimSystem*>(unk_50c)->Startup();
    reinterpret_cast<cvMarshSimSystem*>(unk_276d8)->Startup();
    reinterpret_cast<cvFloodplainsSystem*>(unk_27818)->Startup();
    m_pShadowShader = new FireGrafix::ShadowShader();

    // Subscribe OnTerrainCached and the two cell-update hold handlers.
    TerrainEventDelegate kDelegate;
    kDelegate.m_pObject = this;
    UNK_RET (TerrainSystem::*pfnOnTerrainCached)(EventTemplate<Event_Int2Type<28>, TerrainCacheData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>&) = &TerrainSystem::OnTerrainCached;
    memcpy(&kDelegate.m_pfnMethod, &pfnOnTerrainCached, sizeof(pfnOnTerrainCached));
    Fun_08660726(&kDelegate);
    kDelegate.m_pObject = this;
    UNK_RET (TerrainSystem::*pfnSuspend)() = &TerrainSystem::SuspendCellUpdates;
    memcpy(&kDelegate.m_pfnMethod, &pfnSuspend, sizeof(pfnSuspend));
    Fun_08689e88(&kDelegate);
    kDelegate.m_pObject = this;
    UNK_RET (TerrainSystem::*pfnResume)() = &TerrainSystem::ResumeCellUpdates;
    memcpy(&kDelegate.m_pfnMethod, &pfnResume, sizeof(pfnResume));
    Fun_08689af6(&kDelegate);

    m_Stats.m_fGPUInitTime = 0.0f;
    m_Stats.m_fInitTime = 0.0f;
    m_Stats.field_c = 0;
    m_Stats.m_uiNumCacheTextures = 0;
    m_Stats.m_uiCacheMemory = 0;
    m_uiNum64Textures = GraphicsSettings::TerrainSettings::g_n64Textures;
    m_uiNum128Textures = GraphicsSettings::TerrainSettings::g_n128Textures;
    m_uiNum256Textures = GraphicsSettings::TerrainSettings::g_n256Textures;
    m_uiNum512Textures = GraphicsSettings::TerrainSettings::g_n512Textures;
    m_uiNumBCTextures = GraphicsSettings::TerrainSettings::g_nBCTextures;
    for (unsigned int i = 0; i < m_uiNum64Textures; ++i) {
        m_ah64Textures[i] = FireGrafix::g_pRenderer->CreateRenderTexture(0x40, 0x40, 0, 0x17, "Terrain 64 size texture");
    }
    for (unsigned int i = 0; i < m_uiNum128Textures; ++i) {
        m_ah128Textures[i] = FireGrafix::g_pRenderer->CreateRenderTexture(0x80, 0x80, 0, 0x17, "Terrain 128 size texture");
    }
    for (unsigned int i = 0; i < m_uiNum256Textures; ++i) {
        m_ah256Textures[i] = FireGrafix::g_pRenderer->CreateRenderTexture(0x100, 0x100, 0, 0x17, "Terrain 256 size texture");
    }
    for (unsigned int i = 0; i < m_uiNum512Textures; ++i) {
        m_ah512Textures[i] = FireGrafix::g_pRenderer->CreateRenderTexture(0x200, 0x200, 0, 0x17, "Terrain 512 size texture");
    }

    if (GraphicsSettings::TerrainSettings::g_bEnableDXTCompression != 0) {
        const unsigned int uiCacheSize = GraphicsSettings::TerrainSettings::g_iCacheSize;
        int nMips = 4;
        if (uiCacheSize == 0x200) {
            nMips = 7;
        } else if (uiCacheSize == 0x100) {
            nMips = 6;
        } else if (uiCacheSize == 0x80) {
            nMips = 5;
        }
        // Format 0x31545844 is the DXT1 fourcc.
        FireGrafix::TextureData kBCTemplate(0x31545844, static_cast<FireGrafix::_TextureType>(0), uiCacheSize, uiCacheSize, 0, nMips, 0x80, nullptr, 0);
        for (unsigned int i = 0; i < m_uiNumBCTextures; ++i) {
            m_ahBCTextures[i] = FireGrafix::g_pRenderer->CreateTexture(&kBCTemplate);
        }
    }

    reinterpret_cast<cvTerrainIceSystem*>(unk_529bc)->Startup();
    reinterpret_cast<cvTerrainIceSystem*>(unk_529bc)->SetSunDir(m_vShadowDir, m_vSunDir);
}

/// 0x0867ef40
UNK_RET TerrainSystem::BuildShadowBuffers() {
    unsigned int uiBlurSize;
    bool bBlurPass;
    if (GraphicsSettings::GeneralSettings::g_uShadowLevel == 1) {
        uiBlurSize = 0x400;
        bBlurPass = false;
    } else {
        if (GraphicsSettings::GeneralSettings::g_uShadowLevel == 2) {
            uiBlurSize = 0x400;
        } else if (GraphicsSettings::GeneralSettings::g_uShadowLevel == 3) {
            uiBlurSize = 0x800;
        } else {
            // Shadows off: the shadow texture is a 4x4 cleared placeholder.
            if (RenderGlobals::g_TexHandles[6] != 0) {
                return;
            }
            FireGrafix::TextureData kBlank(0x15, static_cast<FireGrafix::_TextureType>(0), 4, 4, 1, 1, 0, nullptr, 0);
            memset(kBlank.m_aMips[0].m_apFaces[0], 0, 4 * 4 * 4);
            RenderGlobals::g_TexHandles[6] = FireGrafix::g_pRenderer->CreateTexture(&kBlank);
            return;
        }
        bBlurPass = true;
    }

    bool bRecreateRaw = true;
    bool bRecreateBlur = true;
    if (RenderGlobals::g_TexHandles[5] != 0) {
        FireGrafix::TextureDesc* pDesc = FireGrafix::g_pRenderer->GetTextureDesc(RenderGlobals::g_TexHandles[5]);
        unsigned int uiSize = FireGrafix::GetTextureSize(FireGrafix::CRenderer9::m_nScreenWidth, FireGrafix::CRenderer9::m_nScreenHeight, 1, 1, 0x71);
        if (pDesc->field_0 == 1 && pDesc->m_uWidth == FireGrafix::CRenderer9::m_nScreenWidth && pDesc->m_uHeight == FireGrafix::CRenderer9::m_nScreenHeight && pDesc->field_c == 1 && pDesc->field_1c == 0x71) {
            bRecreateRaw = pDesc->field_10 != uiSize;
        }
    }
    if (RenderGlobals::g_TexHandles[6] != 0) {
        FireGrafix::TextureDesc* pDesc = FireGrafix::g_pRenderer->GetTextureDesc(RenderGlobals::g_TexHandles[6]);
        unsigned int uiSize = FireGrafix::GetTextureSize(uiBlurSize, uiBlurSize, 1, 1, 0x6f);
        if (pDesc->field_0 == 1 && pDesc->m_uWidth == uiBlurSize && pDesc->m_uHeight == uiBlurSize && pDesc->field_c == 1 && pDesc->field_1c == 0x6f) {
            bRecreateBlur = pDesc->field_10 != uiSize;
        }
    }

    if (RenderGlobals::g_TexHandles[5] == 0) {
        RenderGlobals::g_TexHandles[5] = FireGrafix::g_pRenderer->CreateRenderTexture(FireGrafix::CRenderer9::m_nScreenWidth, FireGrafix::CRenderer9::m_nScreenHeight, 1, 0x71, "Shadow Texture Raw");
    } else if (bRecreateRaw) {
        FireGrafix::g_pRenderer->RecreateRenderTexture(RenderGlobals::g_TexHandles[5], FireGrafix::CRenderer9::m_nScreenWidth, FireGrafix::CRenderer9::m_nScreenHeight, 1, 0x71, "Shadow Texture Raw");
    }
    if (bBlurPass) {
        if (RenderGlobals::g_TexHandles[6] == 0) {
            RenderGlobals::g_TexHandles[6] = FireGrafix::g_pRenderer->CreateRenderTexture(uiBlurSize, uiBlurSize, 1, 0x6f, "Shadow Texture Blur");
        } else if (bRecreateBlur) {
            FireGrafix::g_pRenderer->RecreateRenderTexture(RenderGlobals::g_TexHandles[6], uiBlurSize, uiBlurSize, 1, 0x6f, "Shadow Texture Blur");
        }
    } else if (RenderGlobals::g_TexHandles[6] == 0) {
        // Without a blur pass the raw texture doubles as the blurred one.
        RenderGlobals::g_TexHandles[6] = RenderGlobals::g_TexHandles[5];
        Fun_0868a21a(FireGrafix::CRenderer9::m_TextureList, RenderGlobals::g_TexHandles[5]);
    }

    FireGrafix::g_pRenderer->ReleaseSurface(m_hShadowSurface);
    FireGrafix::FGXSurface* pSurface;
    FireGrafix::g_pRenderer->CreateSurface(&pSurface, &m_hShadowSurface, 0x46);
    pSurface->field_60 = 1;
    pSurface->field_18[0] = static_cast<unsigned short>(RenderGlobals::g_TexHandles[5]);
    pSurface->field_38[0] = 0;
    pSurface->m_hVB = 1;
    pSurface->m_hIB = 1;
    pSurface->field_c = 2;

    // Texel size of the screen: (1/w, 1/h, -1/w, -1/h).
    float* pConstants = static_cast<float*>(FireGrafix::g_pShadowGlobals->LockGlobal(0));
    const float fWidth = static_cast<float>(FireGrafix::CRenderer9::m_nScreenWidth);
    const float fHeight = static_cast<float>(FireGrafix::CRenderer9::m_nScreenHeight);
    pConstants[0] = 1.0f / fWidth;
    pConstants[1] = 1.0f / fHeight;
    pConstants[2] = -1.0f / fWidth;
    pConstants[3] = -1.0f / fHeight;
    FireGrafix::g_pShadowGlobals->Unlock(0);
}

/// 0x0867f416
UNK_RET TerrainSystem::OnTerrainCached(EventTemplate<Event_Int2Type<28>, TerrainCacheData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1) {
    // The event data starts with the cached fraction.
    const float fCachedFraction = *reinterpret_cast<const float*>(&a1);
    if (fCachedFraction >= 0.9999f) {
        m_Stats.m_fGPUInitTime = m_GPUInitTimer.Peek();
        m_GPUInitTimer.Stop();
        m_pTerrain->CalcStats(&m_Stats);
    }
}

/// 0x0867f496
UNK_RET TerrainSystem::Shutdown() {
    m_WaterSystem.Shutdown();
    m_DecalLayer.Shutdown();
    reinterpret_cast<cvRiverSimSystem*>(unk_50c)->Shutdown();
    reinterpret_cast<cvMarshSimSystem*>(unk_276d8)->Shutdown();
    reinterpret_cast<cvFloodplainsSystem*>(unk_27818)->Shutdown();
    FireGrafix::g_pRenderer->ReleaseCommandStream(m_pCommandStream);
    FireGrafix::g_pRenderer->ReleaseCommandStream(m_pOverlayCommandStream);
    FireGrafix::g_pRenderer->ReleaseCommandStream(m_pTextureCommandStream);
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, RenderGlobals::g_TexHandles[5]);
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, RenderGlobals::g_TexHandles[6]);
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, RenderGlobals::g_TexHandles[8]);
    for (unsigned int i = 0; i < m_uiNum64Textures; ++i) {
        Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_ah64Textures[i]);
    }
    for (unsigned int i = 0; i < m_uiNum128Textures; ++i) {
        Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_ah128Textures[i]);
    }
    for (unsigned int i = 0; i < m_uiNum256Textures; ++i) {
        Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_ah256Textures[i]);
    }
    for (unsigned int i = 0; i < m_uiNum512Textures; ++i) {
        Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_ah512Textures[i]);
    }
    if (GraphicsSettings::TerrainSettings::g_bEnableDXTCompression != 0) {
        for (unsigned int i = 0; i < m_uiNumBCTextures; ++i) {
            Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_ahBCTextures[i]);
        }
    }
    if (m_pJobSet != nullptr) {
        delete m_pJobSet;
    }
    m_pJobSet = nullptr;

    TerrainEventDelegate kDelegate;
    kDelegate.m_pObject = this;
    UNK_RET (TerrainSystem::*pfnOnTerrainCached)(EventTemplate<Event_Int2Type<28>, TerrainCacheData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>&) = &TerrainSystem::OnTerrainCached;
    memcpy(&kDelegate.m_pfnMethod, &pfnOnTerrainCached, sizeof(pfnOnTerrainCached));
    Fun_08660236(&kDelegate);
    kDelegate.m_pObject = this;
    UNK_RET (TerrainSystem::*pfnSuspend)() = &TerrainSystem::SuspendCellUpdates;
    memcpy(&kDelegate.m_pfnMethod, &pfnSuspend, sizeof(pfnSuspend));
    Fun_086898ca(&kDelegate);
    kDelegate.m_pObject = this;
    UNK_RET (TerrainSystem::*pfnResume)() = &TerrainSystem::ResumeCellUpdates;
    memcpy(&kDelegate.m_pfnMethod, &pfnResume, sizeof(pfnResume));
    Fun_0868969e(&kDelegate);

    // ShadowShader is a 0x30-byte shader package instance view; only that destructor runs.
    FireGrafix::FGXShaderPackageInstanceView* pShadowView = reinterpret_cast<FireGrafix::FGXShaderPackageInstanceView*>(m_pShadowShader);
    if (pShadowView != nullptr) {
        delete pShadowView;
    }
    m_pShadowShader = nullptr;
    reinterpret_cast<cvTerrainIceSystem*>(unk_529bc)->Shutdown();
}

/// 0x0867f6f2
UNK_RET TerrainSystem::OnResolutionChanged() {
    BuildShadowBuffers();
}

/// 0x0867f706
UNK_RET TerrainSystem::RegisterDebugMenu(LuaContext* pDebugMenu) {
    // Binds handler to the click delegate of the named control; null when the control is missing.
    auto BindHandler = [this, pDebugMenu](char const* pszControl, UNK_RET (TerrainSystem::*pfnHandler)(unsigned int, unsigned int)) -> ButtonControlBase* {
        ButtonControlBase* pButton = static_cast<ButtonControlBase*>(pDebugMenu->LookUpControl(pszControl));
        if (pButton != nullptr) {
            memcpy(&pButton->m_aNativeDelegates[0].m_pfnMethod, &pfnHandler, sizeof(pfnHandler));
            pButton->m_aNativeDelegates[0].m_pObject = this;
        }
        return pButton;
    };

    static const char* const kToggleControls[4] = {
        "DebugMenu/Terrain_Panel/ToggleDiffuse",
        "DebugMenu/Terrain_Panel/ToggleSpecular",
        "DebugMenu/Terrain_Panel/ReloadLightEnvironment",
        "DebugMenu/Terrain_Panel/ForceRefresh",
    };
    for (unsigned int i = 0; i < 4; ++i) {
        BindHandler(kToggleControls[i], &TerrainSystem::TerrainSystemToggle);
    }
    BindHandler("DebugMenu/Terrain_Panel/SunYaw_ButtonDown", &TerrainSystem::SunYawButton);
    BindHandler("DebugMenu/Terrain_Panel/SunYaw_ButtonUp", &TerrainSystem::SunYawButton);
    BindHandler("DebugMenu/Terrain_Panel/SunPitch_ButtonDown", &TerrainSystem::SunPitchButton);
    BindHandler("DebugMenu/Terrain_Panel/SunPitch_ButtonUp", &TerrainSystem::SunPitchButton);
    BindHandler("DebugMenu/Terrain_Panel/ReflectPitch_ButtonDown", &TerrainSystem::ReflectPitchButton);
    BindHandler("DebugMenu/Terrain_Panel/ReflectPitch_ButtonUp", &TerrainSystem::ReflectPitchButton);
    BindHandler("DebugMenu/Terrain_Panel/ReflectYaw_ButtonDown", &TerrainSystem::ReflectYawButton);
    BindHandler("DebugMenu/Terrain_Panel/ReflectYaw_ButtonUp", &TerrainSystem::ReflectYawButton);

    // The tessellation controls are disabled on devices below caps 10.
    ButtonControlBase* pButton = BindHandler("DebugMenu/Terrain_Panel/TerrainTessellation", &TerrainSystem::TessellationToggle);
    if (pButton != nullptr) {
        pButton->SetDisabled(FireGrafix::DeviceSettings::g_DeviceCaps < 10);
    }
    BindHandler("DebugMenu/Terrain_Panel/TerrainWireframe", &TerrainSystem::WireframeToggle);
    pButton = BindHandler("DebugMenu/Terrain_Panel/TessLOD_ButtonDown", &TerrainSystem::TessLODButton);
    if (pButton != nullptr) {
        pButton->SetDisabled(FireGrafix::DeviceSettings::g_DeviceCaps < 10);
    }
    pButton = BindHandler("DebugMenu/Terrain_Panel/TessLOD_ButtonUp", &TerrainSystem::TessLODButton);
    if (pButton != nullptr) {
        pButton->SetDisabled(FireGrafix::DeviceSettings::g_DeviceCaps < 10);
    }
    pButton = BindHandler("DebugMenu/Terrain_Panel/TessLevel_ButtonDown", &TerrainSystem::TessLevelButton);
    if (pButton != nullptr) {
        pButton->SetDisabled(FireGrafix::DeviceSettings::g_DeviceCaps < 10);
    }
    pButton = BindHandler("DebugMenu/Terrain_Panel/TessLevel_ButtonUp", &TerrainSystem::TessLevelButton);
    if (pButton != nullptr) {
        pButton->SetDisabled(FireGrafix::DeviceSettings::g_DeviceCaps < 10);
    }

    m_pDebugMenu = pDebugMenu;
    UpdateSettings();
}

/// 0x0867fb06
UNK_RET TerrainSystem::TerrainSystemToggle(unsigned int a1, unsigned int uiToggle) {
    switch (uiToggle) {
    case 1:
        m_bDiffuse = !m_bDiffuse;
        m_pTerrain->SetOptions((LIGHTINGOPTS)2, (SHADOWOPTS)2, 1, 1, 1, 1, 1.0f, m_bDiffuse, m_bSpecular);
        break;
    case 2:
        m_bSpecular = !m_bSpecular;
        m_pTerrain->SetOptions((LIGHTINGOPTS)2, (SHADOWOPTS)2, 1, 1, 1, 1, 1.0f, m_bDiffuse, m_bSpecular);
        break;
    case 3: {
        FireGrafix::CRenderer9* pRenderer = FireGrafix::g_pRenderer;
        pRenderer->ReloadCubeTexture("assets\\terrain\\lighting\\Sunlight_Generic_Terrain.dds", m_ahSunlightCubes[0]);
        pRenderer->ReloadCubeTexture("assets\\terrain\\lighting\\Shadowlight_Generic_Terrain.dds", m_ahShadowlightCubes[0]);
        pRenderer->ReloadCubeTexture("assets\\terrain\\lighting\\Sunlight_America_Terrain.dds", m_ahSunlightCubes[1]);
        pRenderer->ReloadCubeTexture("assets\\terrain\\lighting\\Shadowlight_America_Terrain_2.dds", m_ahShadowlightCubes[1]);
        pRenderer->ReloadCubeTexture("assets\\terrain\\lighting\\Sunlight_Asia_Terrain.dds", m_ahSunlightCubes[2]);
        pRenderer->ReloadCubeTexture("assets\\terrain\\lighting\\Shadowlight_Asia_Terrain_2.dds", m_ahShadowlightCubes[2]);
        pRenderer->ReloadCubeTexture("assets\\terrain\\lighting\\Sunlight_Africa_Terrain.dds", m_ahSunlightCubes[3]);
        pRenderer->ReloadCubeTexture("assets\\terrain\\lighting\\Shadowlight_Africa_Terrain.dds", m_ahShadowlightCubes[3]);
        pRenderer->ReloadCubeTexture("assets\\terrain\\lighting\\Sunlight_Europe_Terrain.dds", m_ahSunlightCubes[4]);
        pRenderer->ReloadCubeTexture("assets\\terrain\\lighting\\Shadowlight_Europe_Terrain_2.dds", m_ahShadowlightCubes[4]);
        // Slot 9 of RenderGlobals::g_TexHandles is the spec screen map loaded in Init.
        pRenderer->ReloadTexture("assets\\terrain\\lighting\\specscreenmap.dds", RenderGlobals::g_TexHandles[9]);
        m_WaterSystem.ReloadTextures();
        m_pTerrain->Invalidate();
        break;
    }
    case 4:
        m_pTerrain->Invalidate();
        break;
    }
}

/// 0x0867fd46
UNK_RET TerrainSystem::SunYawButton(unsigned int a1, unsigned int a2) {
    // Button 2 steps down; the angle wraps through 360 degrees.
    float fAngle = m_fSunYaw + ((a1 == 2) ? -5.0f : 5.0f);
    if (fAngle < 0.0f) {
        fAngle = 360.0f;
    }
    m_fSunYaw = (fAngle > 360.0f) ? 0.0f : fAngle;
    UpdateSettings();
}

/// 0x0867fdaa
UNK_RET TerrainSystem::SunPitchButton(unsigned int a1, unsigned int a2) {
    float fAngle = m_fSunPitch + ((a1 == 2) ? -5.0f : 5.0f);
    if (fAngle < 0.0f) {
        fAngle = 360.0f;
    }
    m_fSunPitch = (fAngle > 360.0f) ? 0.0f : fAngle;
    UpdateSettings();
}

/// 0x0867fe0e
UNK_RET TerrainSystem::ReflectPitchButton(unsigned int a1, unsigned int a2) {
    float fAngle = m_fReflectPitch + ((a1 == 2) ? -5.0f : 5.0f);
    if (fAngle < 0.0f) {
        fAngle = 360.0f;
    }
    m_fReflectPitch = (fAngle > 360.0f) ? 0.0f : fAngle;
    UpdateSettings();
}

/// 0x0867fe72
UNK_RET TerrainSystem::ReflectYawButton(unsigned int a1, unsigned int a2) {
    float fAngle = m_fReflectYaw + ((a1 == 2) ? -5.0f : 5.0f);
    if (fAngle < 0.0f) {
        fAngle = 360.0f;
    }
    m_fReflectYaw = (fAngle > 360.0f) ? 0.0f : fAngle;
    UpdateSettings();
}

/// 0x0867fed6
UNK_RET TerrainSystem::TessellationToggle(unsigned int a1, unsigned int a2) {
    int& bUseTessellation = GraphicsSettings::TerrainSettings::g_bUseTessellation;
    bUseTessellation = !bUseTessellation;
}

/// 0x0867fef4
UNK_RET TerrainSystem::WireframeToggle(unsigned int a1, unsigned int a2) {
    m_bWireframe = !m_bWireframe;
}

/// 0x0867ff00
UNK_RET TerrainSystem::TessLODButton(unsigned int a1, unsigned int a2) {
    unsigned int& uiPixelsPerTri = GraphicsSettings::TerrainSettings::g_iTessellationPixelsPerTri;
    uiPixelsPerTri += (a1 == 0xffffffff) ? -1 : 1;
    if (uiPixelsPerTri == 0) {
        uiPixelsPerTri = 1;
    }
    UpdateSettings();
}

/// 0x0867ff32
UNK_RET TerrainSystem::TessLevelButton(unsigned int a1, unsigned int a2) {
    unsigned int& uiDivisions = GraphicsSettings::TerrainSettings::g_iTessellationPatchDivisions;
    unsigned int uiNewDivisions = (a1 == 0xffffffff) ? uiDivisions >> 1 : uiDivisions * 2;
    if (uiNewDivisions == 0) {
        uiNewDivisions = 1;
    }
    uiDivisions = (uiNewDivisions > 16) ? 16 : uiNewDivisions;
    UpdateSettings();
}

/// 0x0867ff7a
UNK_RET TerrainSystem::UpdateSettings() {
    char szText[128];
    TextControl* pLabel;

    sprintf_s(szText, 128, "%f", m_fSunPitch);
    pLabel = static_cast<TextControl*>(m_pDebugMenu->LookUpControl("DebugMenu/Terrain_Panel/SunPitch_Label"));
    if (pLabel != 0) {
        pLabel->SetText(szText);
    }
    sprintf_s(szText, 128, "%f", m_fSunYaw);
    pLabel = static_cast<TextControl*>(m_pDebugMenu->LookUpControl("DebugMenu/Terrain_Panel/SunYaw_Label"));
    if (pLabel != 0) {
        pLabel->SetText(szText);
    }
    sprintf_s(szText, 128, "%f", m_fReflectPitch);
    pLabel = static_cast<TextControl*>(m_pDebugMenu->LookUpControl("DebugMenu/Terrain_Panel/ReflectPitch_Label"));
    if (pLabel != 0) {
        pLabel->SetText(szText);
    }
    sprintf_s(szText, 128, "%f", m_fReflectYaw);
    pLabel = static_cast<TextControl*>(m_pDebugMenu->LookUpControl("DebugMenu/Terrain_Panel/ReflectYaw_Label"));
    if (pLabel != 0) {
        pLabel->SetText(szText);
    }
    sprintf_s(szText, 128, "%f", m_fReflectAmount);
    pLabel = static_cast<TextControl*>(m_pDebugMenu->LookUpControl("DebugMenu/Terrain_Panel/ReflectAmount_Label"));
    if (pLabel != 0) {
        pLabel->SetText(szText);
    }
    pLabel = static_cast<TextControl*>(m_pDebugMenu->LookUpControl("DebugMenu/Terrain_Panel/TessLOD_Label"));
    if (pLabel != 0) {
        pLabel->SetText("0");
    }
    pLabel = static_cast<TextControl*>(m_pDebugMenu->LookUpControl("DebugMenu/Terrain_Panel/TessLevel_Label"));
    if (pLabel != 0) {
        pLabel->SetText("1");
    }

    // Yaw and pitch are in degrees; pitch is measured from +z (up).
    m_vSunDir.x = -(cosf(m_fSunYaw * kDegreesToRadians) * sinf(m_fSunPitch * kDegreesToRadians));
    m_vSunDir.y = -(sinf(m_fSunYaw * kDegreesToRadians) * sinf(m_fSunPitch * kDegreesToRadians));
    m_vSunDir.z = cosf(m_fSunPitch * kDegreesToRadians);
    m_vReflectDir.x = -(cosf(m_fReflectYaw * kDegreesToRadians) * sinf(m_fReflectPitch * kDegreesToRadians));
    m_vReflectDir.y = -(sinf(m_fReflectYaw * kDegreesToRadians) * sinf(m_fReflectPitch * kDegreesToRadians));
    m_vReflectDir.z = cosf(m_fReflectPitch * kDegreesToRadians);

    m_pTerrain->SetSunDir(m_vShadowDir, m_vSunDir);
    m_WaterSystem.m_fReflectAmount = m_fReflectAmount;
    reinterpret_cast<cvTerrainIceSystem*>(unk_529bc)->SetSunDir(m_vShadowDir, m_vSunDir);
}

/// 0x0868041a
UNK_RET TerrainSystem::ReflectAmount(unsigned int a1, unsigned int a2) {
    float fAmount = m_fReflectAmount + ((a1 == 2) ? -0.05f : 0.05f);
    if (fAmount < 0.0f) {
        fAmount = 0.0f;
    }
    m_fReflectAmount = (fAmount > 1.0f) ? 1.0f : fAmount;
    UpdateSettings();
}

/// 0x0868046a
UNK_RET TerrainSystem::Init(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u>& kGrid, bool a2, unsigned int uiRandomSeed) {
    srand(uiRandomSeed);
    m_TerrainData.Load(false);
    m_TerrainData.LoadTerrainDefinition("assets\\terrain\\TerrainTextures\\TerrainDef.csv", m_bSpecular, true);

    RenderGlobals::g_TexHandles[9] = FireGrafix::g_pRenderer->LoadTexture("assets\\terrain\\lighting\\specscreenmap.dds");
    m_ahSunlightCubes[0] = FireGrafix::g_pRenderer->LoadCubeTexture("assets\\terrain\\lighting\\Sunlight_Generic_Terrain.dds");
    m_ahShadowlightCubes[0] = FireGrafix::g_pRenderer->LoadCubeTexture("assets\\terrain\\lighting\\Shadowlight_Generic_Terrain.dds");
    m_ahSunlightCubes[1] = FireGrafix::g_pRenderer->LoadCubeTexture("assets\\terrain\\lighting\\Sunlight_America_Terrain.dds");
    m_ahShadowlightCubes[1] = FireGrafix::g_pRenderer->LoadCubeTexture("assets\\terrain\\lighting\\Shadowlight_America_Terrain_2.dds");
    m_ahSunlightCubes[2] = FireGrafix::g_pRenderer->LoadCubeTexture("assets\\terrain\\lighting\\Sunlight_Asia_Terrain.dds");
    m_ahShadowlightCubes[2] = FireGrafix::g_pRenderer->LoadCubeTexture("assets\\terrain\\lighting\\Shadowlight_Asia_Terrain_2.dds");
    m_ahSunlightCubes[3] = FireGrafix::g_pRenderer->LoadCubeTexture("assets\\terrain\\lighting\\Sunlight_Africa_Terrain.dds");
    m_ahShadowlightCubes[3] = FireGrafix::g_pRenderer->LoadCubeTexture("assets\\terrain\\lighting\\Shadowlight_Africa_Terrain.dds");
    m_ahSunlightCubes[4] = FireGrafix::g_pRenderer->LoadCubeTexture("assets\\terrain\\lighting\\Sunlight_Europe_Terrain.dds");
    m_ahShadowlightCubes[4] = FireGrafix::g_pRenderer->LoadCubeTexture("assets\\terrain\\lighting\\Shadowlight_Europe_Terrain_2.dds");

    m_InitTimer.Start();
    m_StageTimer.Start();

    // 64 heightmap texels per hex.
    FGXInt2 kHeightMapSize;
    kHeightMapSize.x = kGrid.m_iWidth << 6;
    kHeightMapSize.y = kGrid.m_iHeight << 6;
    const FGXVector3& vTerrainScale = WorldGlobals::g_TerrainScale;
    FGXVector2 vWorldSize;
    vWorldSize.x = static_cast<float>(kHeightMapSize.x) * vTerrainScale.x;
    vWorldSize.y = static_cast<float>(kHeightMapSize.y) * vTerrainScale.y;
    FGXVector3 vWorldCenter;
    vWorldCenter.x = static_cast<float>(kHeightMapSize.x >> 1) * vTerrainScale.x;
    vWorldCenter.y = static_cast<float>(kHeightMapSize.y >> 1) * vTerrainScale.y;
    vWorldCenter.z = 0.0f;
    MapGlobals::g_TerrainSampler.StartupGame(&kGrid, kHeightMapSize, vWorldSize, vWorldCenter, vTerrainScale, true, 1.0f);
    MapGlobals::g_TerrainSampler.field_4 = m_WaterSystem.m_fSeaLevel * 255.0f * WorldGlobals::g_TerrainScale.z;
    m_DecalLayer.StartupGame();

    field_2771c = MapGlobals::g_pTerrainSampler;
    field_27720 = &m_TerrainData;
    m_iHeightMapWidth = MapGlobals::g_TerrainSampler.m_iHeightMapWidth;
    m_iHeightMapHeight = MapGlobals::g_TerrainSampler.m_iHeightMapHeight;

    cvRoadLayoutSim& kRoadLayout = *reinterpret_cast<cvRoadLayoutSim*>(g_cvRoadLayoutSimSystem);
    kRoadLayout.Fun_086831c6();

    // Coastlines: extract them from the hex grid, then build the five offset levels of each.
    cvCoastMeshEngine kCoastMeshEngine;
    // Placement new on the temp heap; Fun_08683328 is the CoastGrid constructor.
    CoastGrid* pCoastGrid = static_cast<CoastGrid*>(FTempHeapFastVectorAllocator::AllocAligned(sizeof(CoastGrid), __alignof(CoastGrid), 43, 259));
    if (pCoastGrid != nullptr) {
        Fun_08683328(pCoastGrid);
    }
    if (WorldGlobals::g_eWrapMode == 0) {
        pCoastGrid->field_9c = false;
    }
    pCoastGrid->Init(kGrid);
    kCoastMeshEngine.m_CoastMeshLibrary.LoadCoastDefinitions(m_TerrainData);
    kCoastMeshEngine.m_bLibraryLoaded = true;
    kCoastMeshEngine.m_CoastSplineSystem.Startup(*pCoastGrid);
    kCoastMeshEngine.GenerateAllLevelSplines();
    kCoastMeshEngine.m_CoastSplineSystem.AddAllCoastlinesToDatabase(*pCoastGrid, &kCoastMeshEngine.m_aFinalLevelSplines);
    FTempHeapFastVectorAllocator::FreeAligned(pCoastGrid);

    // Coastal roads: points shifted by the sampler offset, y scaled by the hex row spacing.
    {
        FFastVector<CoastalRoad, true, 43u, 267u, FTempHeapFastVectorAllocator> aRoads;
        FFastVector<FGXVector2, true, 43u, 267u, FTempHeapFastVectorAllocator> aRoadPoints;
        kCoastMeshEngine.GenerateAllCoastalRoads(aRoadPoints, aRoads);
        const cvTerrainSampler* pSampler = MapGlobals::g_pTerrainSampler;
        for (unsigned int i = 0; i < aRoadPoints.size(); ++i) {
            aRoadPoints[i].x -= pSampler->m_fOffsetX;
            aRoadPoints[i].y *= kHexHeightRatio;
            aRoadPoints[i].y -= pSampler->m_fOffsetY;
        }
        for (unsigned int i = 0; i < aRoads.size(); ++i) {
            const CoastalRoad& kRoad = aRoads[i];
            kRoadLayout.AddLargeRoad<FGXVector2>(&aRoadPoints[kRoad.m_uiFirstPoint], kRoad.m_uiNumPoints, kRoad.field_8);
        }
        kRoadLayout.m_RoadNetworkManager.Fun_0868a43e();
    }
    m_StageTimer.Stop();

    g_TerrainStencil.StartupGame();
    m_StageTimer.Start();
    {
        FFastVector<CoastStencilMesh, false, 43u, 267u, FTempHeapFastVectorAllocator> aStencilMeshes;
        kCoastMeshEngine.GenerateAllCoastStencil(aStencilMeshes);
        for (unsigned int i = 0; i < aStencilMeshes.size(); ++i) {
            for (unsigned int j = 0; j < aStencilMeshes[i].m_uiNumSurfaces; ++j) {
                const CoastStencilSurface& kSurface = aStencilMeshes[i].m_pSurfaces[j];
                g_TerrainStencil.AddStencil(kSurface.m_Surface, kSurface.m_ucStencilBits, true);
            }
        }
    }
    m_StageTimer.Start();

    // Distance from p to the segment [a, b].
    auto SegmentDistance = [](const FGXVector2& p, const FGXVector2& a, const FGXVector2& b) -> float {
        const float fABx = b.x - a.x;
        const float fABy = b.y - a.y;
        const float fBPx = p.x - b.x;
        const float fBPy = p.y - b.y;
        if ((a.x == b.x && a.y == b.y) || fABy * fBPy + fABx * fBPx >= 0.0f) {
            return sqrtf(fBPy * fBPy + fBPx * fBPx);
        }
        const float fAPx = p.x - a.x;
        const float fAPy = p.y - a.y;
        if (fABy * fAPy + fABx * fAPx <= 0.0f) {
            return sqrtf(fAPy * fAPy + fAPx * fAPx);
        }
        const float fLength = sqrtf(fABy * fABy + fABx * fABx);
        // Integer abs: the perpendicular distance is truncated to whole units.
        return static_cast<float>(abs(static_cast<int>((fAPy * fABx - fAPx * fABy) / fLength)));
    };

    // One point per segment of pSpline: the segment midpoint moved one unit along fSide * normal.
    auto PushOffsetPoints = [](SplinePointVector& aOut, FGXVector2* pSpline, int iNumPoints, float fSide) {
        const unsigned int uiNumSegments = iNumPoints - 1;
        for (unsigned int i = 0; i < uiNumSegments; ++i) {
            const FGXVector2 vGradient = B_SPLINE::EvalGradient<FGXVector2, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>(
                iNumPoints, pSpline, static_cast<float>(static_cast<int>(i)), 0);
            const float fLength = sqrtf(vGradient.y * vGradient.y + vGradient.x * vGradient.x);
            const float fNormalX = vGradient.y / fLength;
            const float fNormalY = -(vGradient.x / fLength);
            FGXVector2 vPoint;
            vPoint.x = (pSpline[i].x + pSpline[i + 1].x) * 0.5f + fSide * fNormalX;
            vPoint.y = (pSpline[i].y + pSpline[i + 1].y) * 0.5f + fSide * fNormalY;
            aOut.push_back(vPoint);
        }
    };

    // A segment of [iBegin, iEnd) that crosses one of the next kLoopSearchWindow segments pulls every
    // point in between onto the crossing; a near-degenerate corner drops its middle point.
    auto RemoveSplineLoops = [&SegmentDistance](FGXVector2* pPoints, int iBegin, int iEnd) {
        for (int i = iBegin; i < iEnd - 2; ++i) {
            const FGXVector2 vA = pPoints[i];
            const FGXVector2 vB = pPoints[i + 1];
            const FGXVector2 vC = pPoints[i + 2];
            const float fDistC = SegmentDistance(vC, vA, vB);
            const float fDistA = SegmentDistance(vA, vB, vC);
            const float fMinDist = (fDistA <= fDistC) ? fDistA : fDistC;
            if (fMinDist < kMinCornerDistance) {
                pPoints[i + 1] = pPoints[i + 2];
                continue;
            }

            // Line AB as a1 * x + b1 * y = c1.
            const float fA1 = vB.y - vA.y;
            const float fB1 = vA.x - vB.x;
            const float fC1 = vA.y * fB1 + vA.x * fA1;
            const float fMaxABx = (vA.x <= vB.x) ? vB.x : vA.x;
            const float fMinABx = (vB.x <= vA.x) ? vB.x : vA.x;
            const float fMaxABy = (vA.y <= vB.y) ? vB.y : vA.y;
            const float fMinABy = (vB.y <= vA.y) ? vB.y : vA.y;
            const int iLast = (iEnd - 1 < i + kLoopSearchWindow) ? iEnd - 1 : i + kLoopSearchWindow;
            for (int j = i + 2; j < iLast; ++j) {
                const FGXVector2 vP = pPoints[j];
                const FGXVector2 vQ = pPoints[j + 1];
                const float fA2 = vQ.y - vP.y;
                const float fB2 = vP.x - vQ.x;
                const float fDet = fA1 * fB2 - fB1 * fA2;
                if (static_cast<float>(abs(static_cast<int>(fDet))) < kIntersectEpsilon) {
                    continue;
                }
                const float fC2 = vP.y * fB2 + vP.x * fA2;
                const float fInvDet = 1.0f / fDet;
                const float fMaxPQx = (vP.x <= vQ.x) ? vQ.x : vP.x;
                const float fMinPQx = (vQ.x <= vP.x) ? vQ.x : vP.x;
                const float fMaxPQy = (vP.y <= vQ.y) ? vQ.y : vP.y;
                const float fMinPQy = (vQ.y <= vP.y) ? vQ.y : vP.y;
                FGXVector2 vCross;
                vCross.x = (fB2 * fC1 - fB1 * fC2) * fInvDet;
                if (vCross.x > ((fMaxPQx <= fMaxABx) ? fMaxPQx : fMaxABx) + kIntersectEpsilon) {
                    continue;
                }
                if (((fMinABx <= fMinPQx) ? fMinPQx : fMinABx) - kIntersectEpsilon > vCross.x) {
                    continue;
                }
                vCross.y = (fC2 * fA1 - fA2 * fC1) * fInvDet;
                if (vCross.y > ((fMaxPQy <= fMaxABy) ? fMaxPQy : fMaxABy) + kIntersectEpsilon) {
                    continue;
                }
                if (((fMinABy <= fMinPQy) ? fMinPQy : fMinABy) - kIntersectEpsilon > vCross.y) {
                    continue;
                }
                for (int k = j; k > i; --k) {
                    pPoints[k] = vCross;
                }
                break;
            }
        }
    };

    // Blend splines: the coastline level of every coast, then one offset polyline on each side of it.
    SplinePointVector aPoints;
    SplineRangeVector aCoastRanges;
    SplineRangeVector aOffsetRanges;
    FFastVector<FinalLevelSplines, false, 43u, 259u, BaseVector<FinalLevelSplines, false>::FDefaultFastVectorAllocator>& aFinalLevelSplines = kCoastMeshEngine.m_aFinalLevelSplines;
    const unsigned int uiNumCoasts = aFinalLevelSplines.size();
    unsigned int uiMaxPoints = 0;
    for (unsigned int i = 0; i < uiNumCoasts; ++i) {
        uiMaxPoints += (aFinalLevelSplines[i].m_auiLevelRange[kCoastlineLevel][1] - aFinalLevelSplines[i].m_auiLevelRange[kCoastlineLevel][0]) * 3;
    }
    aCoastRanges.reserve(uiNumCoasts);
    aOffsetRanges.reserve(uiNumCoasts * 2);
    aPoints.reserve(uiMaxPoints);

    SplineRange kRange;
    int iCursor = 0;
    for (FinalLevelSplines* pCoast = aFinalLevelSplines.begin(); pCoast != aFinalLevelSplines.end(); ++pCoast) {
        switch (pCoast->m_eType) {
        case 1:
            kRange.field_8 = 1;
            break;
        case 0:
            kRange.field_8 = 0;
            break;
        }
        const unsigned int uiFirst = pCoast->m_auiLevelRange[kCoastlineLevel][0];
        const unsigned int uiLast = pCoast->m_auiLevelRange[kCoastlineLevel][1];
        kRange.m_iBegin = iCursor;
        kRange.m_iEnd = iCursor - uiFirst + uiLast;
        for (const CoastCP* pCP = &pCoast->m_pControlPoints[uiFirst]; pCP != &pCoast->m_pControlPoints[uiLast]; ++pCP) {
            aPoints.push_back(pCP->m_vPos);
        }
        aCoastRanges.push_back(kRange);
        iCursor = kRange.m_iEnd;
    }

    for (unsigned int i = 0; i < aCoastRanges.size(); ++i) {
        const SplineRange kCoast = aCoastRanges[i];
        FGXVector2* pSpline = &aPoints[kCoast.m_iBegin];
        const int iNumPoints = kCoast.m_iEnd - kCoast.m_iBegin;
        const unsigned int uiNumSegments = iNumPoints - 1;

        kRange.m_iBegin = iCursor;
        kRange.m_iEnd = iCursor + uiNumSegments;
        kRange.field_8 = kCoast.field_8;
        PushOffsetPoints(aPoints, pSpline, iNumPoints, -1.0f);
        RemoveSplineLoops(aPoints.begin(), kRange.m_iBegin, kRange.m_iEnd);
        aOffsetRanges.push_back(kRange);

        switch (kCoast.field_8) {
        case 1:
            kRange.field_8 = 2;
            break;
        case 0:
            kRange.field_8 = 1;
            break;
        }
        kRange.m_iBegin = kRange.m_iEnd;
        kRange.m_iEnd = kRange.m_iBegin + uiNumSegments;
        PushOffsetPoints(aPoints, pSpline, iNumPoints, 1.0f);
        RemoveSplineLoops(aPoints.begin(), kRange.m_iBegin, kRange.m_iEnd);
        aOffsetRanges.push_back(kRange);
        iCursor = kRange.m_iEnd;
    }
    m_StageTimer.Stop();

    // Heightmap: terrain pieces blended along the splines.
    {
        cvTerrainBlend kTerrainBlend(kGrid);
        m_StageTimer.Start();
        kTerrainBlend.LoadTerrainPieces(a2, "assets\\terrain\\pieces", reinterpret_cast<unsigned long*>(m_TerrainData.m_aHexCode), m_TerrainData.m_uiNumTerrains);
        m_StageTimer.Stop();
        m_StageTimer.Start();
        kTerrainBlend.BlendTerrainHeightmap(kGrid, kHeightMapSize, aPoints, aCoastRanges, aOffsetRanges, m_DecalLayer);
        kTerrainBlend.ClearTerrainPieces();
        m_StageTimer.Stop();
    }

    reinterpret_cast<cvMarshSimSystem*>(unk_276d8)->StartupGame();
    m_StageTimer.Start();
    {
        FFastVector<CoastMesh, false, 43u, 267u, FTempHeapFastVectorAllocator> aCoastMeshes;
        kCoastMeshEngine.GenerateAllCoastBlendMeshes(aCoastMeshes);
        kCoastMeshEngine.BlendAllCoastMeshes(kHeightMapSize, 0x3c3f, aCoastMeshes, &m_TerrainData);
        kCoastMeshEngine.m_CoastSplineSystem.Shutdown();
        aFinalLevelSplines.clear();
        kCoastMeshEngine.m_CoastMeshLibrary.UnLoadCoastDefinitions();
        m_StageTimer.Stop();
    }

    m_StageTimer.Start();
    cvRiverSimSystem* pRiverSim = reinterpret_cast<cvRiverSimSystem*>(unk_50c);
    {
        FFastVector<FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator> aRiverEdges;
        pRiverSim->StartupGame(aRiverEdges);
        reinterpret_cast<cvFloodplainsSystem*>(unk_27818)->BuildFloodplains(aRiverEdges);
    }
    pRiverSim->BlendIntoTerrain(&m_TerrainData);
    pRiverSim->ClearInternalBuffers();
    m_StageTimer.Stop();

    reinterpret_cast<cvTerrainIceSystem*>(unk_529bc)->BuildIce(MapGlobals::g_TerrainSampler);
    m_StageTimer.Start();
    CreateTerrain(kGrid);
    m_iInitialized = 1;
    field_27718 = m_pTerrain;
    m_StageTimer.Stop();
    m_StageTimer.Stop();
    m_Stats.m_fInitTime = m_InitTimer.Peek();
    m_InitTimer.Stop();
}

/// 0x08682312
UNK_RET TerrainSystem::CreateTerrain(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const& kGrid) {
    Terrain* pTerrain = new Terrain();
    ExternalDataReferences kExternalData;
    kExternalData.m_pTerrainSystem = &g_TerrainSystem;
    kExternalData.m_pTerrainSampler = MapGlobals::g_pTerrainSampler;
    pTerrain->Init(kGrid, m_TerrainData.m_uiNumTerrains, *reinterpret_cast<ExternalTextureHandles const*>(RenderGlobals::g_TexHandles), kExternalData);
    pTerrain->SetTextures(m_TerrainData.m_aTextureAHandle, m_TerrainData.m_aTextureCHandle, m_TerrainData.m_aTextureBHandle,
                          reinterpret_cast<float const*>(m_TerrainData.field_308), m_TerrainData.m_afFileFloat,
                          reinterpret_cast<TextureProjectionUserMode const*>(m_TerrainData.m_aFileInt), m_TerrainData.m_uiNumTerrains);
    pTerrain->SetLightingEnv(m_ahSunlightCubes, m_ahShadowlightCubes);
    pTerrain->m_kShared.field_520 = 2048.0f;
    pTerrain->SetSunDir(m_vShadowDir, m_vSunDir);
    pTerrain->SetOptions((LIGHTINGOPTS)2, (SHADOWOPTS)2, 1, 1, 1, 1, 1.0f, m_bDiffuse, m_bSpecular);
    pTerrain->m_kShared.m_vTerrainScale = WorldGlobals::g_TerrainScale;
    pTerrain->SetRenderTextures(m_ah64Textures, m_uiNum64Textures, m_ah128Textures, m_uiNum128Textures,
                                m_ah256Textures, m_uiNum256Textures, m_ah512Textures, m_uiNum512Textures,
                                m_ahBCTextures, m_uiNumBCTextures);
    pTerrain->PruneWork();
    m_pTerrain = pTerrain;

    // Queue index and record array of the event sequencer, as laid out inside g_EventSystemSequencer.
    unsigned char* pSequencer = g_EventSystemSequencer;
    Fun_086839fe(pSequencer + 0x588 + *reinterpret_cast<int*>(pSequencer + 0x500) * 0x20);
}

/// 0x086825e4
UNK_RET TerrainSystem::StartGPUInitWork(bool a1) {
    m_GPUInitTimer.Start();
    m_DecalLayer.Update();
    m_pTerrain->PopulateTextureBuildList(a1);
    m_bGPUInitStarted = true;
}

/// 0x0868263e
UNK_RET TerrainSystem::PerformGPUInitWork() {
    if (m_pTerrain != 0 && m_bGPUInitStarted) {
        m_pTerrain->NonBlockingTextureInit(m_pTextureCommandStream);
        m_pTextureCommandStream->AddAllCommandSetsToRenderQueue();
        m_pCommandStream->AddAllCommandSetsToRenderQueue();
    }
}

/// 0x0868269a
UNK_RET TerrainSystem::StartupGame() {
    m_WaterSystem.StartupGame();
    reinterpret_cast<cvFloodplainsSystem*>(unk_27818)->StartupGame();
    m_iSuspendCellUpdates = 0;
}

/// 0x086826da
UNK_RET TerrainSystem::ShutdownGame() {
    m_TerrainData.Unload();
    for (unsigned int i = 0; i < 5; ++i) {
        Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_ahSunlightCubes[i]);
        Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_ahShadowlightCubes[i]);
        m_ahSunlightCubes[i] = 0;
        m_ahShadowlightCubes[i] = 0;
    }
    if (m_pTerrain != nullptr) {
        delete m_pTerrain;
    }
    m_pTerrain = nullptr;
    m_DecalLayer.ShutdownGame();
    field_2771c = nullptr;
    field_27718 = nullptr;
    m_iHeightMapWidth = 0;
    field_27720 = nullptr;
    m_iHeightMapHeight = 0;
    MapGlobals::g_pTerrainSampler = nullptr;
    MapGlobals::g_TerrainSampler.ShutdownGame();
    reinterpret_cast<cvRiverSimSystem*>(unk_50c)->ShutdownGame();
    reinterpret_cast<cvMarshSimSystem*>(unk_276d8)->ShutdownGame();
    reinterpret_cast<cvFloodplainsSystem*>(unk_27818)->ShutdownGame();
    reinterpret_cast<cvTerrainIceSystem*>(unk_529bc)->DestroyIce();
    g_TerrainStencil.ShutdownGame();

    // The road layout sim keeps its state in the global; its pools go back to the empty free list.
    unsigned char* pRoadLayout = g_cvRoadLayoutSimSystem;
    Fun_0868a26e(pRoadLayout + 0x158);
    *reinterpret_cast<unsigned int*>(pRoadLayout + 0xd54) = 0;
    const unsigned int kRoadPoolOffsets[3] = {0xc98, 0xc84, 0xc70};
    for (unsigned int i = 0; i < 3; ++i) {
        unsigned int* pPool = reinterpret_cast<unsigned int*>(pRoadLayout + kRoadPoolOffsets[i]);
        pPool[0] = 0xfffffff;
        pPool[3] = 0;
        pPool[1] = 0;
    }
    pRoadLayout[4] = 0;
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, RenderGlobals::g_TexHandles[9]);
    RenderGlobals::g_TexHandles[9] = 0;
    m_iInitialized = 0;
}

/// 0x086828b4
UNK_RET TerrainSystem::PerformShadowBlur() {
    if (GraphicsSettings::GeneralSettings::g_uShadowLevel > 1) {
        const FGXVector4 vClearZero = {0.0f, 0.0f, 0.0f, 0.0f};
        const unsigned int uCommandSet = m_pCommandStream->BeginCommandSet(RenderGlobals::g_TexHandles[6], 0, 9, 0x80, "Shadow Blur", vClearZero, 1.0f, 0, 0, 0);
        const bool bCheapBlur = GraphicsSettings::GeneralSettings::g_bAllowCheapDX11ShadowBlur[0] != 0
            && GraphicsSettings::GeneralSettings::g_uShadowLevel <= 2
            && FireGrafix::g_pRenderer->field_42c == 10;
        FireGrafix::FGXShaderPackageInstanceView* pShadowView = reinterpret_cast<FireGrafix::FGXShaderPackageInstanceView*>(m_pShadowShader);
        m_pCommandStream->DrawSurface(m_hShadowSurface, pShadowView->m_hInstance, bCheapBlur);
        m_pCommandStream->EndCommandSet();
        m_pCommandStream->AddCommandSetToRenderQueue(uCommandSet);
    }
}

/// 0x086829c8
UNK_RET TerrainSystem::Render(unsigned int a1) {
    m_pTerrain->Render(m_pCommandStream, m_pTextureCommandStream, a1,
                       m_iSuspendCellUpdates == 0 && GraphicsSettings::TerrainSettings::g_bAutoUpdateCells != 0, false, m_bWireframe);
    if (m_bRenderWater) {
        m_WaterSystem.Render(a1);
    }
    // Byte 0x1ad of the application's options object (at +0xb8), bit 0: the river overlay is shown.
    unsigned char* pApplication = *reinterpret_cast<unsigned char**>(g_pkApplication);
    unsigned char* pOptions = *reinterpret_cast<unsigned char**>(pApplication + 0xb8);
    if ((pOptions[0x1ad] & 1) != 0) {
        reinterpret_cast<RiverVisSystem*>(g_RiverVisSystem)->Render(a1);
    }
    if (m_bRenderIce) {
        reinterpret_cast<cvTerrainIceSystem*>(unk_529bc)->Render(a1, false);
    }
}

/// 0x08682aa6
UNK_RET TerrainSystem::RenderOverlay() {
    unsigned char* pApplication = *reinterpret_cast<unsigned char**>(g_pkApplication);
    if (GraphicsSettings::GeneralSettings::g_uOverlayLevel == 2 && pApplication[0xb19] != 0) {
        m_pTerrain->RenderOverlay(m_pOverlayCommandStream);
    }
}

/// 0x08682af4
UNK_RET TerrainSystem::LaunchJobs() {
    m_DecalLayer.Update();
    m_pTerrain->LaunchJobs(m_iSuspendCellUpdates == 0 && GraphicsSettings::TerrainSettings::g_bAutoUpdateCells != 0, false, false);
}

/// 0x08682b50
UNK_RET TerrainSystem::Update(float fDeltaTime) {
    m_WaterSystem.Update(fDeltaTime);
    reinterpret_cast<cvFloodplainsSystem*>(unk_27818)->Update(fDeltaTime);
}

/// 0x08682ba0
UNK_RET TerrainSystem::SetupShadowFrameGlobals() {
    // The light looks at the middle of the visible world from 5000 units away; the shadow view is the
    // sun's view and the reflection view the mirrored one, both orthographic over the visible rectangle.
    const FGXVector3& vTopLeft = FrameGlobals::g_WorldTopLeftVisibleUnclamped[0];
    const FGXVector3& vBottomRight = FrameGlobals::g_WorldBottomRightVisibleUnclamped[0];
    const float kLightDistance = 5000.0f;

    D3DXVECTOR3 vCenter;
    vCenter.x = (vTopLeft.x + vBottomRight.x) * 0.5f;
    vCenter.y = (vTopLeft.y + vBottomRight.y) * 0.5f;
    vCenter.z = (vTopLeft.z + vBottomRight.z) * 0.5f;
    const float fWidth = vTopLeft.x - vBottomRight.x;
    const float fHeight = vTopLeft.y - vBottomRight.y;

    D3DXVECTOR3 vEye;
    vEye.x = m_vShadowDir.x * kLightDistance + vCenter.x;
    vEye.y = m_vShadowDir.y * kLightDistance + vCenter.y;
    vEye.z = m_vShadowDir.z * kLightDistance + vCenter.z;
    D3DXVECTOR3 vUp;
    vUp.x = 0.0f;
    vUp.y = 1.0f;
    vUp.z = 0.0f;

    D3DXMATRIX mView;
    D3DXMATRIX mProjection;
    D3DXMATRIX mRelative;
    D3DXMatrixLookAtRH(&mView, &vEye, &vCenter, &vUp);
    D3DXMatrixOrthoOffCenterRH(&mProjection, fWidth * 0.5f, fWidth * -0.5f, fHeight * 0.5f, fHeight * -0.5f, 30000.0f, -30000.0f);
    D3DXMatrixMultiply(&FrameGlobals::g_ShadowViewProj[0], &mView, &mProjection);
    for (unsigned int i = 1; i < FrameGlobals::g_nCameras; ++i) {
        D3DXMatrixMultiply(&mRelative, &FrameGlobals::g_View[i], &FrameGlobals::g_InvView[0]);
        D3DXMatrixMultiply(&FrameGlobals::g_ShadowViewProj[i], &mRelative, &FrameGlobals::g_ShadowViewProj[0]);
    }

    vEye.x = vCenter.x - m_vReflectDir.x * kLightDistance;
    vEye.y = vCenter.y - m_vReflectDir.y * kLightDistance;
    vEye.z = vCenter.z - m_vReflectDir.z * kLightDistance;
    D3DXMatrixLookAtRH(&mView, &vEye, &vCenter, &vUp);
    D3DXMatrixMultiply(&FrameGlobals::g_ReflectionViewProj[0], &mView, &mProjection);
    FrameGlobals::g_ReflectionProj = mProjection;
    FrameGlobals::g_ReflectionView[0] = mView;
    for (unsigned int i = 1; i < FrameGlobals::g_nCameras; ++i) {
        D3DXMatrixMultiply(&mRelative, &FrameGlobals::g_View[i], &FrameGlobals::g_InvView[0]);
        D3DXMatrixMultiply(&FrameGlobals::g_ReflectionViewProj[i], &mRelative, &FrameGlobals::g_ReflectionViewProj[0]);
        D3DXMatrixMultiply(&FrameGlobals::g_ReflectionView[i], &mRelative, &FrameGlobals::g_ReflectionView[0]);
    }
}

/// 0x0868300a
UNK_RET TerrainSystem::ToggleCellUpdates() {
    int& bAutoUpdateCells = GraphicsSettings::TerrainSettings::g_bAutoUpdateCells;
    bAutoUpdateCells = !bAutoUpdateCells;
}

/// 0x08683028
UNK_RET TerrainSystem::ToggleWater() {
    m_bRenderWater = !m_bRenderWater;
}

/// 0x08683034
UNK_RET TerrainSystem::ToggleIceRendering() {
    m_bRenderIce = !m_bRenderIce;
}

// Event handler code at 0x086831a6; no function symbol covers it.
/// inline
UNK_RET TerrainSystem::SuspendCellUpdates() {
    m_iSuspendCellUpdates = 1;
}

// Event handler code at 0x086831b6; no function symbol covers it.
/// inline
UNK_RET TerrainSystem::ResumeCellUpdates() {
    m_iSuspendCellUpdates = 0;
}

/// 0x0868b536
UNK_RET WaterSystem::LoadSunSky() {
    // Float blocks of watersky.dds: xyz divided by w, or xyz biased by -0.5 and normalized, plus a raw w.
    FileLoadBuffer kFile;
    Fun_08669c48(&kFile, "watersky.dds");
    FireGrafix::TextureData kSkyDds(kFile.m_pData, Fun_08669d48(&kFile), 1);
    FireGrafix::TextureData kSky(&kSkyDds, 0x74);
    const float* pfSky = reinterpret_cast<const float*>(kSky.m_aMips[0].m_apFaces[0]);

    // The w of the two specular colors is set by Startup (the powers).
    m_vSpecColorNarrow.x = pfSky[0] / pfSky[3];
    m_vSpecColorNarrow.y = pfSky[1] / pfSky[3];
    m_vSpecColorNarrow.z = pfSky[2] / pfSky[3];
    m_vSpecColorWide.x = pfSky[4] / pfSky[7];
    m_vSpecColorWide.y = pfSky[5] / pfSky[7];
    m_vSpecColorWide.z = pfSky[6] / pfSky[7];

    m_vWaterDir0.x = pfSky[8] - 0.5f;
    m_vWaterDir0.y = pfSky[9] - 0.5f;
    m_vWaterDir0.z = pfSky[10] - 0.5f;
    D3DXVec3Normalize(reinterpret_cast<D3DXVECTOR3*>(&m_vWaterDir0), reinterpret_cast<const D3DXVECTOR3*>(&m_vWaterDir0));
    m_vWaterDir1.x = pfSky[12] - 0.5f;
    m_vWaterDir1.y = pfSky[13] - 0.5f;
    m_vWaterDir1.z = pfSky[14] - 0.5f;
    D3DXVec3Normalize(reinterpret_cast<D3DXVECTOR3*>(&m_vWaterDir1), reinterpret_cast<const D3DXVECTOR3*>(&m_vWaterDir1));
    m_vWaterDir2.x = pfSky[16] - 0.5f;
    m_vWaterDir2.y = pfSky[17] - 0.5f;
    m_vWaterDir2.z = pfSky[18] - 0.5f;
    D3DXVec3Normalize(reinterpret_cast<D3DXVECTOR3*>(&m_vWaterDir2), reinterpret_cast<const D3DXVECTOR3*>(&m_vWaterDir2));
    m_fWaterDir2W = pfSky[19];
    m_vWaterDir3.x = pfSky[20] - 0.5f;
    m_vWaterDir3.y = pfSky[21] - 0.5f;
    m_vWaterDir3.z = pfSky[22] - 0.5f;
    D3DXVec3Normalize(reinterpret_cast<D3DXVECTOR3*>(&m_vWaterDir3), reinterpret_cast<const D3DXVECTOR3*>(&m_vWaterDir3));
    m_fWaterDir3W = pfSky[23];
}

/// 0x0868ae38
UNK_RET WaterSystem::Startup(unsigned short a1) {
    // Unpacked .lean image: two pixel blocks, then dimensions (see FLeanMap::DeSerializeFromMemory).
    struct LeanMapImage {
        unsigned char* m_pPixels;
        unsigned char* m_pConverted;
        float m_fScale;
        int m_iWidth;
        int m_iHeight;
        int m_nMips;
        int m_iFormat;
        unsigned char m_bNeedsSwap;
    };
    // Makes the two DX9 textures of a .lean file; the handles stay untouched when it is not a .lean image.
    const auto createLeanTexturePair = [](const char* pszFile, unsigned int* phFirst, unsigned int* phSecond) {
        FileLoadBuffer kFile;
        Fun_08669c48(&kFile, pszFile);
        LeanMapImage kImage;
        FLeanMap& kMap = reinterpret_cast<FLeanMap&>(kImage);
        if (FLeanMap::DeSerializeFromMemory(kFile.m_pData, kMap)) {
            FireGrafix::TextureData kFirst(0x24, static_cast<FireGrafix::_TextureType>(0), kImage.m_iWidth, kImage.m_iHeight, 1, kImage.m_nMips, 0x45, kImage.m_pPixels, 0);
            kMap.ConvertToDX9();
            FireGrafix::TextureData kSecond(0x51, static_cast<FireGrafix::_TextureType>(0), kImage.m_iWidth, kImage.m_iHeight, 1, kImage.m_nMips, 0x45, kImage.m_pConverted, 0);
            *phFirst = FireGrafix::g_pRenderer->CreateTexture(&kFirst);
            *phSecond = FireGrafix::g_pRenderer->CreateTexture(&kSecond);
        }
        Fun_08669d90(&kFile);
    };
    m_fSeaLevel = static_cast<float>(a1) / 65535.0f;
    m_pCommandStream = FireGrafix::g_pRenderer->CreateCommandStream(0x401, 5, 0, 0);
    m_pWaterShader = new FireGrafix::WaterShader();
    m_pWorldCurrents = new FireGrafix::WorldCurrents();

    createLeanTexturePair("waterbumps16.lean", &m_hBumpA, &m_hBumpB);

    if (GraphicsSettings::TerrainSettings::g_uWaterQuality == 0) {
        m_ahRiverBumpA[0] = 0;
        m_ahRiverBumpA[1] = 0;
        m_ahRiverBumpB[0] = 0;
        m_ahRiverBumpB[1] = 0;
    } else {
        static const char* const apszRiverMaps[2] = { "riversmooth16.lean", "riverrough_new16.lean" };
        for (unsigned int i = 0; i < 2; ++i) {
            createLeanTexturePair(apszRiverMaps[i], &m_ahRiverBumpA[i], &m_ahRiverBumpB[i]);
        }
    }

    m_hDepthColor = FireGrafix::g_pRenderer->LoadTexture("waterdepthcolor_new.dds");
    m_hRiverDiffuse = FireGrafix::g_pRenderer->LoadTexture("river_diffuse_new.dds");
    m_hRiverAlpha = FireGrafix::g_pRenderer->LoadTexture("river_alpha_new.dds");

    // Pass 0/2/3 of the water package are the Low/Medium/High ocean pixel shaders; only Medium and High reflect.
    RenderGlobals::g_TexHandles[7] = 0;
    bool bReflect = false;
    if (GraphicsSettings::TerrainSettings::g_uWaterQuality == 1) {
        m_uPass = 2;
        bReflect = true;
    } else if (GraphicsSettings::TerrainSettings::g_uWaterQuality == 2) {
        m_uPass = 3;
        bReflect = true;
    } else {
        m_uPass = 0;
    }
    if (bReflect && GraphicsSettings::GeneralSettings::g_uReflectionLevel == 1) {
        RenderGlobals::g_TexHandles[7] = FireGrafix::g_pRenderer->CreateRenderTexture(0x400, 0x400, 1, 0x15, "Reflection Texture Raw");
    } else {
        FireGrafix::TextureData kBlack(0x15, static_cast<FireGrafix::_TextureType>(0), 0x20, 0x20, 1, 1, 0, nullptr, 0);
        memset(kBlack.m_aMips[0].m_apFaces[0], 0, 0x1000);
        RenderGlobals::g_TexHandles[7] = FireGrafix::g_pRenderer->CreateTexture(&kBlack);
    }

    LoadSunSky();
    field_cc = 1.43f;
    m_fWaveStrength = 5.0f;
    m_fWaveTiling = 0.008f;
    m_fScrollSpeedA = 0.18f;
    m_fScrollSpeedB = 0.03f;
    m_fScrollPhaseA = 0.0f;
    m_fScrollPhaseB = 0.0f;
    m_fCurrentParamA = 0.1f;
    m_fCurrentParamB = 0.1f;
    m_fCurrentParamC = 2.0f;
    m_fReflectAmount = 0.5f;
    m_bWatchSkyFiles = false;
    m_fSpecPowerNarrow = exp2f(11.75f);
    m_fSpecPowerWide = exp2f(5.75f);
    RenderOceanCurrentMap();

    // Camera 0 draws with the global set itself, the other cameras with copies.
    m_auConstantSetIds[0] = FireGrafix::g_pWaterGlobalData->field_34;
    for (unsigned int i = 1; i < 4; ++i) {
        m_auConstantSetIds[i] = FireGrafix::g_pWaterGlobalData->Copy();
    }
    m_uMaxQuads = 0x800;
    m_hQuadVB = FireGrafix::g_pRenderer->CreateDynamicVB(static_cast<FireGrafix::_FGX_VERTEXFORMATS>(0xe), 0x2000, 0);
    m_hQuadIB = FireGrafix::g_pRenderer->CreateDynamicIB(static_cast<FireGrafix::_FGX_INDEXFORMATS>(0), m_uMaxQuads * 6, 0);
}

/// 0x0868b832
UNK_RET WaterSystem::Shutdown() {
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_hBumpA);
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_hBumpB);
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_hDepthColor);
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, RenderGlobals::g_TexHandles[7]);
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_hRiverDiffuse);
    for (unsigned int i = 0; i < 2; ++i) {
        Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_ahRiverBumpA[i]);
        Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_ahRiverBumpB[i]);
    }
    // WaterShader and WorldCurrents are 0x30-byte shader package instance views; only that destructor runs.
    FireGrafix::FGXShaderPackageInstanceView* pCurrentsView = reinterpret_cast<FireGrafix::FGXShaderPackageInstanceView*>(m_pWorldCurrents);
    if (pCurrentsView != nullptr) {
        pCurrentsView->~FGXShaderPackageInstanceView();
        operator delete(pCurrentsView);
    }
    FireGrafix::FGXShaderPackageInstanceView* pWaterView = reinterpret_cast<FireGrafix::FGXShaderPackageInstanceView*>(m_pWaterShader);
    if (pWaterView != nullptr) {
        pWaterView->~FGXShaderPackageInstanceView();
        operator delete(pWaterView);
    }
    FireGrafix::g_pRenderer->ReleaseCommandStream(m_pCommandStream);
    Fun_0866fd26(FireGrafix::CRenderer9::m_VBList, m_hQuadVB);
    Fun_0866fb7e(FireGrafix::CRenderer9::m_IBList, m_hQuadIB);
    for (unsigned int i = 1; i < 4; ++i) {
        FireGrafix::FGXConstantSetManager::ReleaseConstantSet(m_auConstantSetIds[i]);
        m_auConstantSetIds[i] = 0;
    }
}

/// 0x0868b9ee
UNK_RET WaterSystem::RenderOceanCurrentMap() {
    if (m_hCurrentSurface == 0) {
        return;
    }
    FireGrafix::g_pRenderer->GetTextureDesc(RenderGlobals::g_TexHandles[8]);
    WorldCurrentsConstants* pConstants = static_cast<WorldCurrentsConstants*>(FireGrafix::g_pWorldCurrentsData->LockGlobal(0));
    pConstants->m_fCurrentParamA = m_fCurrentParamA;
    pConstants->m_fCurrentParamB = m_fCurrentParamB;
    pConstants->m_fCurrentParamC = m_fCurrentParamC;
    pConstants->m_afGridSize[0] = static_cast<float>(*reinterpret_cast<const int*>(WorldGlobals::g_PaddedGameGrid + 0x94));
    pConstants->m_afGridSize[1] = static_cast<float>(*reinterpret_cast<const int*>(WorldGlobals::g_PaddedGameGrid + 0x98));
    FireGrafix::g_pWorldCurrentsData->Unlock(0);

    const FGXVector4 vClear = {0.0f, 0.0f, 0.0f, 0.0f};
    m_pCommandStream->BeginCommandSet(RenderGlobals::g_TexHandles[8], 0, 9, 0x80, "WaterCurrent", vClear, 1.0f, 0, 0, 0);
    m_pCommandStream->DrawSurface(m_hCurrentSurface, reinterpret_cast<FireGrafix::FGXShaderPackageInstanceView*>(m_pWorldCurrents)->m_hInstance, 3);
    m_pCommandStream->EndCommandSet();
    m_pCommandStream->AddAllCommandSetsToRenderQueue();
}
/// 0x0868b966
UNK_RET WaterSystem::StartupGame() {
    FireGrafix::FGXSurface* pSurface;
    FireGrafix::g_pRenderer->CreateSurface(&pSurface, &m_hCurrentSurface, 0x5d);
    pSurface->field_c = 2;
    pSurface->m_hIB = 1;
    pSurface->m_hVB = 1;
    pSurface->field_60 = 1;
    pSurface->field_18[0] = 0;
    pSurface->field_38[0] = 0;
    RenderOceanCurrentMap();
}

/// 0x0868bb38
UNK_RET WaterSystem::GenerateBumpTextures() {
}

/// 0x0868bb3a
UNK_RET WaterSystem::ShutdownGame() {
    FireGrafix::g_pRenderer->ReleaseSurface(field_44);
}

/// 0x0868bb6a
UNK_RET WaterSystem::ReloadTextures() {
    LoadSunSky();
    FireGrafix::g_pRenderer->ReloadTexture("waterdepthcolor_new.dds", m_hDepthColor);
}

/// 0x0868bbae
UNK_RET WaterSystem::Update(float fDeltaTime) {
    m_fScrollPhaseA += m_fScrollSpeedA * 4.0f * fDeltaTime;
    m_fScrollPhaseB += fDeltaTime * m_fScrollSpeedB;
}

/// 0x0868bc02
UNK_RET WaterSystem::Render(unsigned int a1) {
    struct WaterChunkRange {
        int m_iX0;
        int m_iY0;
        int m_iX1;
        int m_iY1;
    };
    const int kWaterChunkCells = 64;
    const int kWaterTileCells = 0x380;
    const int kWaterTilesPerRow = 16;
    const unsigned int kWaterMaxCameras = 4;
    // One camera's visible window in 64-cell chunks of the heightmap.
    const auto getVisibleChunks = [](unsigned int uCamera, int iMapWidth, int iMapHeight) {
        const FGXVector3& vTopLeft = FrameGlobals::g_WorldTopLeftVisible[uCamera];
        const FGXVector3& vBottomRight = reinterpret_cast<const FGXVector3*>(FrameGlobals::g_WorldBottomRightVisible)[uCamera];
        const float fOriginX = WorldGlobals::g_WorldMin.x;
        const float fOriginY = WorldGlobals::g_WorldMin.y;
        const int iLeft = static_cast<int>(roundf((vTopLeft.x - fOriginX) / WorldGlobals::g_TerrainScale.x));
        const int iTop = static_cast<int>(roundf((vTopLeft.y - fOriginY) / WorldGlobals::g_TerrainScale.y));
        const int iRight = static_cast<int>(roundf((vBottomRight.x - fOriginX) / WorldGlobals::g_TerrainScale.x));
        const int iBottom = static_cast<int>(roundf((vBottomRight.y - fOriginY) / WorldGlobals::g_TerrainScale.y));
        WaterChunkRange kRange;
        kRange.m_iX0 = iLeft / 64;
        kRange.m_iY0 = iTop / 64;
        kRange.m_iX1 = iRight / 64 + 1;
        kRange.m_iY1 = iBottom / 64 + 1;
        if (kRange.m_iX0 < 0) {
            kRange.m_iX0 = 0;
        }
        if (kRange.m_iY0 < 0) {
            kRange.m_iY0 = 0;
        }
        if (kRange.m_iX1 > iMapWidth - 1) {
            kRange.m_iX1 = iMapWidth - 1;
        }
        if (kRange.m_iY1 > iMapHeight - 1) {
            kRange.m_iY1 = iMapHeight - 1;
        }
        return kRange;
    };
    if (m_bWatchSkyFiles) {
        // Reload the sky and depth colour when one of the source files is newer than the last seen write time.
        const auto isNewer = [this](const char* pszFile) {
            const FFileTimestamp kWritten = FFile::GetLastWriteTime(pszFile);
            FILETIME kSeen;
            FILETIME kNow;
            memcpy(&kSeen, &m_ullSkyFileTime, sizeof(kSeen));
            memcpy(&kNow, &kWritten, sizeof(kNow));
            if (CompareFileTime(&kSeen, &kNow) == -1) {
                memcpy(&m_ullSkyFileTime, &kNow, sizeof(kNow));
                return true;
            }
            return false;
        };
        const bool bSkyNewer = isNewer("watersky.dds");
        const bool bDepthNewer = isNewer("waterdepthcolor_new.dds");
        const bool bBumpsNewer = isNewer("waterbumps.dds");
        if (bSkyNewer || bDepthNewer || bBumpsNewer) {
            LoadSunSky();
            FireGrafix::g_pRenderer->ReloadTexture("waterdepthcolor_new.dds", m_hDepthColor);
        }
    }

    const float fMinX = WorldGlobals::g_WorldMin.x;
    const float fMinY = WorldGlobals::g_WorldMin.y;
    const int iMapWidth = MapGlobals::g_pTerrainSampler->m_iHeightMapWidth;
    const int iMapHeight = MapGlobals::g_pTerrainSampler->m_iHeightMapHeight;
    const float fMapScaleX = static_cast<float>(iMapWidth) / WorldGlobals::g_WorldSize.x;
    const float fMapScaleY = static_cast<float>(iMapHeight) / WorldGlobals::g_WorldSize.y;

    // Bump tile counts per world width: integer repeats keep the texture seamless.
    const float fWorldWidth = WorldGlobals::g_WorldMax.x - fMinX;
    const float fTileCount = static_cast<float>(static_cast<int>(fWorldWidth * m_fWaveTiling));
    float afTileScale[4];
    afTileScale[0] = fTileCount / fWorldWidth;
    afTileScale[1] = (1.0f + fTileCount) / fWorldWidth;
    afTileScale[2] = (2.0f + fTileCount) / fWorldWidth;
    afTileScale[3] = (3.0f + fTileCount) / fWorldWidth;

    // Quads are bucketed per heightmap tile: count, then prefix sums, then fill.
    unsigned short auCount[kWaterMaxCameras * 256];
    unsigned short auStart[kWaterMaxCameras * 256];
    memset(auCount, 0, sizeof(auCount));
    unsigned int uTotalQuads = 0;
    const unsigned int nCameras = FrameGlobals::g_nCameras;
    if (nCameras != 0) {
        for (unsigned int uCamera = 0; uCamera < nCameras; ++uCamera) {
            const WaterChunkRange kRange = getVisibleChunks(uCamera, iMapWidth, iMapHeight);
            for (int iX = kRange.m_iX0; iX < kRange.m_iX1; ++iX) {
                for (int iY = kRange.m_iY0; iY < kRange.m_iY1; ++iY) {
                    const int iTile = ((iY * kWaterChunkCells) / kWaterTileCells) * kWaterTilesPerRow + (iX * kWaterChunkCells) / kWaterTileCells;
                    ++auCount[uCamera * 256 + iTile];
                }
            }
        }
        for (unsigned int uCamera = 0; uCamera < nCameras; ++uCamera) {
            for (unsigned int uTile = 0; uTile < 256; ++uTile) {
                const unsigned short uTileCount = auCount[uCamera * 256 + uTile];
                auCount[uCamera * 256 + uTile] = 0;
                auStart[uCamera * 256 + uTile] = static_cast<unsigned short>(uTotalQuads);
                uTotalQuads += uTileCount;
            }
        }
        if (m_uMaxQuads < uTotalQuads) {
            m_uMaxQuads = static_cast<unsigned int>(static_cast<float>(uTotalQuads) * 1.2f);
            Fun_0866fd26(FireGrafix::CRenderer9::m_VBList, m_hQuadVB);
            Fun_0866fb7e(FireGrafix::CRenderer9::m_IBList, m_hQuadIB);
            m_hQuadVB = FireGrafix::g_pRenderer->CreateDynamicVB(static_cast<FireGrafix::_FGX_VERTEXFORMATS>(0xe), m_uMaxQuads << 2, 0);
            m_hQuadIB = FireGrafix::g_pRenderer->CreateDynamicIB(static_cast<FireGrafix::_FGX_INDEXFORMATS>(0), m_uMaxQuads * 6, 0);
        }
    }

    // Vertex: x, y, u, v. Each quad covers one 64x64-cell chunk; z comes from the water height constant.
    float* pfVertices = static_cast<float*>(FireGrafix::g_pRenderer->LockDynamicVB(m_hQuadVB, 0));
    unsigned short* pusIndices = static_cast<unsigned short*>(FireGrafix::g_pRenderer->LockDynamicIB(m_hQuadIB, 0));
    const float fHalfChunk = 32.0f;
    for (unsigned int uCamera = 0; uCamera < nCameras; ++uCamera) {
        const WaterChunkRange kRange = getVisibleChunks(uCamera, iMapWidth, iMapHeight);
        const float fHalfX = WorldGlobals::g_TerrainScale.x * fHalfChunk;
        const float fHalfY = WorldGlobals::g_TerrainScale.y * fHalfChunk;
        for (int iX = kRange.m_iX0; iX < kRange.m_iX1; ++iX) {
            const int iTileX = (iX * kWaterChunkCells) / kWaterTileCells;
            const float fTexelBaseX = static_cast<float>(64 - iTileX * kWaterTileCells);
            for (int iY = kRange.m_iY0; iY < kRange.m_iY1; ++iY) {
                const int iRow = iY * kWaterChunkCells;
                const int iTileY = iRow / kWaterTileCells;
                const float fTexelBaseY = static_cast<float>(64 - iTileY * kWaterTileCells);
                const float fCenterX = WorldGlobals::g_TerrainScale.x * static_cast<float>(iX * kWaterChunkCells) + fMinX + fHalfX;
                const float fCenterY = static_cast<float>(iRow) * WorldGlobals::g_TerrainScale.y + fMinY + fHalfY;
                const float fRight = fHalfX + fCenterX;
                const float fLeft = fCenterX - fHalfX;
                const float fTop = fHalfY + fCenterY;
                const float fBottom = fCenterY - fHalfY;
                const float fULeft = ((fLeft - fMinX) * fMapScaleX + fTexelBaseX) * (1.0f / 1024.0f);
                const float fURight = ((fRight - fMinX) * fMapScaleX + fTexelBaseX) * (1.0f / 1024.0f);
                const float fVBottom = ((fBottom - fMinY) * fMapScaleY + fTexelBaseY) * (-1.0f / 1024.0f);
                const float fVTop = ((fTop - fMinY) * fMapScaleY + fTexelBaseY) * (-1.0f / 1024.0f);
                const unsigned int uBucket = uCamera * 256 + iTileY * kWaterTilesPerRow + iTileX;
                const unsigned int uQuad = static_cast<unsigned int>(auStart[uBucket]) + auCount[uBucket];
                ++auCount[uBucket];
                float* pfQuad = pfVertices + uQuad * 16;
                pfQuad[0] = fLeft;
                pfQuad[1] = fTop;
                pfQuad[2] = fULeft;
                pfQuad[3] = fVTop;
                pfQuad[4] = fRight;
                pfQuad[5] = fTop;
                pfQuad[6] = fURight;
                pfQuad[7] = fVTop;
                pfQuad[8] = fRight;
                pfQuad[9] = fBottom;
                pfQuad[10] = fURight;
                pfQuad[11] = fVBottom;
                pfQuad[12] = fLeft;
                pfQuad[13] = fBottom;
                pfQuad[14] = fULeft;
                pfQuad[15] = fVBottom;
                const unsigned short usBase = static_cast<unsigned short>(uQuad * 4);
                unsigned short* pusQuad = pusIndices + uQuad * 6;
                pusQuad[0] = usBase;
                pusQuad[1] = usBase + 1;
                pusQuad[2] = usBase + 2;
                pusQuad[3] = usBase;
                pusQuad[4] = usBase + 2;
                pusQuad[5] = usBase + 3;
            }
        }
    }
    FireGrafix::g_pRenderer->UnlockIB(m_hQuadIB, 0, uTotalQuads * 6);
    FireGrafix::g_pRenderer->UnlockVB(m_hQuadVB, 0, uTotalQuads << 2);

    // World to heightmap texture space, shared by every camera.
    D3DXMATRIX mScale;
    D3DXMATRIX mTranslate;
    D3DXMATRIX mWorldToHeightmap;
    D3DXMatrixScaling(&mScale, 1.0f / WorldGlobals::g_WorldSize.x, -1.0f / WorldGlobals::g_WorldSize.y, 255.0f / (WorldGlobals::g_TerrainScale.z * 65535.0f));
    D3DXMatrixTranslation(&mTranslate, -fMinX, -fMinY, 0.0f);
    D3DXMatrixMultiply(&mWorldToHeightmap, &mTranslate, &mScale);

    const float* pfFogControl = &FrameGlobals::g_v4FogControl.x;
    const FGXVector4 vClear = {0.0f, 0.0f, 0.0f, 0.0f};
    for (unsigned int uCamera = 0; uCamera < nCameras; ++uCamera) {
        const unsigned int uSetId = m_auConstantSetIds[uCamera];
        FireGrafix::WaterGlobalData* pGlobalData = static_cast<FireGrafix::WaterGlobalData*>(&reinterpret_cast<FireGrafix::FGXConstantSet*>(&FireGrafix::CRenderer9::m_kCSManager[0x2020c8])[uSetId]);
        WaterGlobalConstants* pConstants = static_cast<WaterGlobalConstants*>(pGlobalData->LockGlobal(0));
        const D3DXMATRIX& mInvView = FrameGlobals::g_InvView[uCamera];
        const FGXVector3 vCameraPos = {mInvView._41, mInvView._42, mInvView._43};

        D3DXMatrixMultiply(&pConstants->m_mViewProj, &FrameGlobals::g_View[uCamera], &FrameGlobals::g_Proj);
        pConstants->m_mOverlayProj = FrameGlobals::g_OverlayProjection[uCamera];
        pConstants->m_afScrollOffset[0] = m_fScrollPhaseB * 1.4142135f / 1.8f;
        pConstants->m_afScrollOffset[1] = m_fScrollPhaseB * 1.4142135f / 1.5f;
        pConstants->m_afScrollOffset[2] = m_fScrollPhaseB * 1.4142135f / 7.2f;
        pConstants->m_afScrollOffset[3] = m_fScrollPhaseB * -1.4142135f / 1.2f;
        for (unsigned int i = 0; i < 4; ++i) {
            pConstants->m_afTileScale[i] = afTileScale[i];
        }
        pConstants->m_afBumpWeights[0] = 1.7f;
        pConstants->m_afBumpWeights[1] = 1.2f;
        pConstants->m_afBumpWeights[2] = 1.7f;
        pConstants->m_afBumpWeights[3] = 1.2f;
        pConstants->m_fScrollTile = m_fScrollPhaseA * afTileScale[0];
        pConstants->m_fWaterHeight = m_fSeaLevel * 255.0f * WorldGlobals::g_TerrainScale.z;
        pConstants->m_fWaveStrength = m_fWaveStrength;
        pConstants->m_vDir0 = m_vWaterDir0;
        pConstants->m_vDir1 = m_vWaterDir1;
        pConstants->m_vSpecColorNarrow = m_vSpecColorNarrow;
        pConstants->m_fSpecPowerNarrow = m_fSpecPowerNarrow;
        pConstants->m_vSpecColorWide = m_vSpecColorWide;
        pConstants->m_fSpecPowerWide = m_fSpecPowerWide;
        pConstants->m_vDir2 = m_vWaterDir2;
        pConstants->m_fDir2W = m_fWaterDir2W;
        pConstants->m_vDir3 = m_vWaterDir3;
        pConstants->m_fDir3W = m_fWaterDir3W;
        memcpy(pConstants->m_afFogColor, &FrameGlobals::g_v4FogColor,sizeof(pConstants->m_afFogColor));
        pConstants->m_fFogNegFalloff = -pfFogControl[0];
        pConstants->m_fFogIntegral = -(pfFogControl[1] * expf(-(pfFogControl[0] * vCameraPos.z))) / pfFogControl[0];
        pConstants->field_148 = 0.0f;
        pConstants->m_fFogZoom = pfFogControl[3];
        pConstants->m_fReflectAmount = m_fReflectAmount;
        pConstants->m_mReflectionViewProj = FrameGlobals::g_ReflectionViewProj[uCamera];
        pConstants->m_vCameraPos = vCameraPos;
        pConstants->m_mShadowViewProj = FrameGlobals::g_ShadowViewProj[uCamera];
        pConstants->m_fShadowFade = FrameGlobals::fShadowFade;
        pConstants->m_mWorldToHeightmap = mWorldToHeightmap;
        pGlobalData->Unlock(0);

        m_pCommandStream->BeginCommandSet(a1, 8, 3, 0, "Water", vClear, 1.0f, 0, 0, 0);
        m_pCommandStream->AliasGlobal(FireGrafix::g_pWaterGlobalData->field_34, uSetId);
        for (unsigned int uTile = 0; uTile < 256; ++uTile) {
            const unsigned short usQuads = auCount[uCamera * 256 + uTile];
            if (usQuads == 0) {
                continue;
            }
            FireGrafix::FGXSurface* pSurface;
            unsigned int hSurface;
            FireGrafix::g_pRenderer->CreateSurface(&pSurface, &hSurface, 0x5b);
            pSurface->field_c = usQuads * 2;
            pSurface->m_hVB = m_hQuadVB;
            pSurface->m_hIB = m_hQuadIB;
            pSurface->field_10 = auStart[uCamera * 256 + uTile] * 6;
            pSurface->field_60 = 0xe;
            pSurface->field_18[5] = static_cast<unsigned short>(RenderGlobals::g_TexHandles[8]);
            pSurface->field_38[5] = 0;
            pSurface->field_18[4] = static_cast<unsigned short>(RenderGlobals::g_TexHandles[7]);
            pSurface->field_38[4] = 0;
            pSurface->field_18[0] = static_cast<unsigned short>(m_hBumpA);
            pSurface->field_38[0] = 0;
            pSurface->field_18[1] = static_cast<unsigned short>(m_hBumpB);
            pSurface->field_38[1] = 0;
            pSurface->field_18[2] = 0;
            pSurface->field_38[2] = 0;
            pSurface->field_18[3] = 0;
            pSurface->field_38[3] = 0;
            pSurface->field_18[6] = static_cast<unsigned short>(m_hDepthColor);
            pSurface->field_38[6] = 3;
            pSurface->field_18[7] = static_cast<unsigned short>(g_TerrainSystem.m_pTerrain->m_kShared.m_ahHeightmapTiles[uTile]);
            pSurface->field_38[7] = 0;
            pSurface->field_28[0] = static_cast<unsigned short>(RenderGlobals::g_TexHandles[4]);
            pSurface->field_48[0] = 5;
            pSurface->field_28[1] = static_cast<unsigned short>(RenderGlobals::g_TexHandles[0]);
            pSurface->field_48[1] = 0;
            pSurface->field_28[2] = static_cast<unsigned short>(RenderGlobals::g_TexHandles[2]);
            pSurface->field_48[2] = 0;
            pSurface->field_28[3] = static_cast<unsigned short>(g_ColorKeySystem.m_auKeyHandles[1][0]);
            pSurface->field_48[3] = 5;
            pSurface->field_28[5] = static_cast<unsigned short>(RenderGlobals::g_TexHandles[6]);
            pSurface->field_48[3] = 5;
            m_pCommandStream->DrawSurface(hSurface, reinterpret_cast<FireGrafix::FGXShaderPackageInstanceView*>(m_pWaterShader)->m_hInstance, static_cast<unsigned short>(m_uPass));
            FireGrafix::g_pRenderer->ReleaseSurface(hSurface);
        }
        m_pCommandStream->EndCommandSet();
    }
    m_pCommandStream->AddAllCommandSetsToRenderQueue();
}
