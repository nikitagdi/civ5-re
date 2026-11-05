/// origin binary
#include <cstring>
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainClassGrid.h"
#include "Civ5/Src/App/Graphics/TerrainData.h"
#include "Civ5/Src/App/Graphics/cvTerrainIceSystem.h"
#include "FirePlace/FireGrafix/CCommandStream.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "FirePlace/FireGrafix/TextureData.h"
#include "FirePlace/FireGrafix/WorldCurrents.h"
#include "FirePlace/FireWorks/FLeanMap.h"
#include "FirePlace/FireWorks/FTempHeap.h"

namespace {

const unsigned int kIceShaderDef = 0x3b;
const unsigned long kTextureFormatA8R8G8B8 = 0x15;
const unsigned long kTextureFormatA8 = 0x1c;
const unsigned int kLowSettingsFreeHeap = 0x4000000;

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

}

/// 0x08673c60
cvTerrainIceSystem::cvTerrainIceSystem() {
    Fun_08675cb4(this, 0xff, 0xff, &HEXTransform_i::IDENTITY_TRANSFORM);
    memset(m_aCells, 0, sizeof(m_aCells));
    m_bStarted = false;
    m_bIceBuilt = false;
    m_hShaderPackage = 0;
    field_feac = 0;
    memset(m_Settings.m_afSubsurface, 0, sizeof(m_Settings.m_afSubsurface));
    memset(m_Settings.m_afUnderwater1, 0, sizeof(m_Settings.m_afUnderwater1));
    memset(m_Settings.m_afUnderwater2, 0, sizeof(m_Settings.m_afUnderwater2));
    m_hVertexBuffer = 0;
    m_hIndexBuffer = 0;
    field_fee8 = 0;
    m_hDiffuseTexture = 0;
    m_hEnvironmentCube = 0;
    m_hBumpTexture0 = 0;
    m_hBumpTexture1 = 0;
    m_hSidesDiffuseTexture = 0;
    m_hSidesBumpTexture0 = 0;
    m_hSidesBumpTexture1 = 0;
    memset(m_afBoundsRect, 0, sizeof(m_afBoundsRect));
    m_pSurfaces = 0;
    m_uSurfaceCount = 0;
    m_vShadowDir.x = 0.0f;
    m_vShadowDir.y = 0.0f;
    m_vShadowDir.z = 0.0f;
    m_vSunDir.x = 0.0f;
    m_vSunDir.y = 0.0f;
    m_vSunDir.z = 0.0f;
}

/// 0x08673e20
void cvTerrainIceSystem::SetupMemTracker(FMemTracker* a1) {
}

/// 0x08673e22
void cvTerrainIceSystem::Startup() {
    FireGrafix::dispatchShaderPackageCreateInPlace_ShaderDefDX9(kIceShaderDef, &unk_fea4);
    FGXVector4 kBlendFactor;
    kBlendFactor.x = 0.0f;
    kBlendFactor.y = 0.0f;
    kBlendFactor.z = 0.0f;
    kBlendFactor.w = 0.0f;
    // Blend: src ONE, dest INVSRCALPHA, ADD; alpha src ONE, dest ZERO, ADD; write mask RGBA.
    m_hAlphaState = FireGrafix::g_pRenderer->CreateAlphaState(true, static_cast<_D3DBLEND>(2), static_cast<_D3DBLEND>(6), static_cast<_D3DBLENDOP>(1), static_cast<_D3DBLEND>(2), static_cast<_D3DBLEND>(1), static_cast<_D3DBLENDOP>(1), 0xf, kBlendFactor);
    LoadTextures();
    m_bStarted = true;
}

/// 0x08673ec8
void cvTerrainIceSystem::LoadTextures() {
    // Makes the two DX9 textures of a .lean file; the handles and the scale stay untouched when it is not a .lean image.
    const auto createLeanTexturePair = [](const char* pszFile, unsigned int* phFirst, unsigned int* phSecond, float* pfScale) {
        FileLoadBuffer kFile;
        Fun_08669c48(&kFile, pszFile);
        LeanMapImage kImage;
        FLeanMap& kMap = reinterpret_cast<FLeanMap&>(kImage);
        if (FLeanMap::DeSerializeFromMemory(kFile.m_pData, kMap)) {
            kMap.ConvertToDX9();
            FireGrafix::TextureData kFirst(kTextureFormatA8R8G8B8, static_cast<FireGrafix::_TextureType>(0), kImage.m_iWidth, kImage.m_iHeight, 1, kImage.m_nMips, 0x41, kImage.m_pPixels, 0);
            FireGrafix::TextureData kSecond(kTextureFormatA8, static_cast<FireGrafix::_TextureType>(0), kImage.m_iWidth, kImage.m_iHeight, 1, kImage.m_nMips, 0x41, kImage.m_pConverted, 0);
            *phFirst = FireGrafix::g_pRenderer->CreateTexture(&kFirst);
            *phSecond = FireGrafix::g_pRenderer->CreateTexture(&kSecond);
            *pfScale = kImage.m_fScale;
        }
        Fun_08669d90(&kFile);
    };
    m_hDiffuseTexture = FireGrafix::g_pRenderer->LoadTexture("assets\\terrain\\Ice\\Ice_D.dds");
    m_hSidesDiffuseTexture = FireGrafix::g_pRenderer->LoadTexture("assets\\terrain\\Ice\\Ice_D_Sides.dds");
    m_hEnvironmentCube = FireGrafix::g_pRenderer->LoadCubeTexture("assets\\terrain\\Ice\\Ice_Environment_Diffuse.dds");
    createLeanTexturePair("Ice_Bump.lean", &m_hBumpTexture0, &m_hBumpTexture1, &m_fBumpScale);
    createLeanTexturePair("Ice_Bump_Sides.lean",&m_hSidesBumpTexture0, &m_hSidesBumpTexture1, &m_fSidesBumpScale);
}

/// 0x08674290
void cvTerrainIceSystem::Shutdown() {
    if (m_bStarted) {
        ReleaseTextures();
        FireGrafix::g_pRenderer->ReleaseShaderPackageInstance(m_hShaderPackage);
        m_hShaderPackage = 0;
        m_bStarted = false;
    }
}

/// 0x086742e8
void cvTerrainIceSystem::ReleaseTextures() {
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_hDiffuseTexture);
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_hEnvironmentCube);
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_hBumpTexture0);
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_hBumpTexture1);
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_hSidesBumpTexture0);
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_hSidesBumpTexture1);
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_hSidesDiffuseTexture);
    m_hEnvironmentCube = 0;
    m_hDiffuseTexture = 0;
    m_hBumpTexture1 = 0;
    m_hBumpTexture0 = 0;
    m_hSidesBumpTexture0 = 0;
    m_hSidesDiffuseTexture = 0;
    m_hSidesBumpTexture1 = 0;
}

/// 0x086743d0
void cvTerrainIceSystem::ConfigureGrid(FGXInt2 const& kSize, HEXTransform_i const& kTransform) {
    m_Size = kSize;
    m_Transform = kTransform;
    m_matForward = m_Transform.matrix();
    m_matInverse = m_Transform.inv_matrix();
    for (int iX = 0; iX < kSize.x; ++iX) {
        for (int iY = 0; iY < kSize.y; ++iY) {
            int iColumn = iX;
            if (m_bWrapX) {
                iColumn = iX % m_Size.x;
            }
            m_aCells[iY * 0xff + iColumn] = 0;
        }
    }
}

/// 0x086744c8
void cvTerrainIceSystem::MarkIceHex(HEXVec_i const& kHex) {
    *Fun_08675c2a(this, kHex) |= 0x80;
}

/// 0x086744f4
void cvTerrainIceSystem::BuildIce(cvTerrainSampler const& kSampler) {
    char const* pszSettingsFile = "IceSettings.xml";
    if (GetTempHeap()->GetAvailable() < kLowSettingsFreeHeap || GraphicsSettings::TerrainSettings::g_uTerrainDetailLevel < 2) {
        pszSettingsFile = "IceSettings_Low.xml";
    }
    m_Settings.ReadXML(pszSettingsFile);
    BuildIceMeshes(m_Settings, kSampler);
    m_bIceBuilt = true;
}

/// 0x08674f6a
void cvTerrainIceSystem::DestroyIce() {
    if (!m_bIceBuilt) {
        return;
    }
    Fun_0866fd26(FireGrafix::CRenderer9::m_VBList, m_hVertexBuffer);
    Fun_0866fb7e(FireGrafix::CRenderer9::m_IBList, m_hIndexBuffer);
    m_hVertexBuffer = 0;
    m_hIndexBuffer = 0;
    for (unsigned int i = 0; i < m_uSurfaceCount; ++i) {
        FireGrafix::g_pRenderer->ReleaseSurface(m_pSurfaces[i].m_hSurface);
    }
    delete[] m_pSurfaces;
    m_pSurfaces = 0;
    m_uSurfaceCount = 0;
    for (int i = 0; i < 3; ++i) {
        FireGrafix::g_pRenderer->ReleaseCommandStream(m_apCommandStreams[i]);
        m_apCommandStreams[i] = 0;
    }
    m_bIceBuilt = false;
}

/// 0x0867507a
void cvTerrainIceSystem::SetSunDir(FGXVector3 const& kShadowDir, FGXVector3 const& kSunDir) {
    m_vShadowDir = kShadowDir;
    m_vSunDir = kSunDir;
}

/// 0x08675a18
void cvTerrainIceSystem::RenderReflections(unsigned int a1) {
    RenderInternal(a1, false, true);
}

/// 0x08675a52
void cvTerrainIceSystem::Render(unsigned int a1, bool a2) {
    RenderInternal(a1, a2, false);
}

/// 0x08675a8c
void cvTerrainIceSystem::Rebuild(cvTerrainSampler const& kSampler) {
    DestroyIce();
    ReleaseTextures();
    LoadTextures();
    BuildIce(kSampler);
}
