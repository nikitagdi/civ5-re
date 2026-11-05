/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"
#include <Windows.h>
#include <d3d9.h>

class FGXInt2;
class FGXVector4;
class IDirect3D9;
class IDirect3DBaseTexture9;
class IDirect3DCubeTexture9;
class IDirect3DSwapChain9;
class IDirect3DTexture9;
namespace FireGrafix {
class CCommandStream;
class CRenderer9;
class CommandHeader;
class FGXSurface;
class RenderStats;
class StartupParams;
class SurfaceSet;
class TextureData;
class Viewport;
enum _FGX_INDEXFORMATS : int;
enum _FGX_VERTEXFORMATS : int;
}

namespace FireGrafix {
// Element of CRenderer9::m_SurfaceList from +0x81088 (stride 0x64); CreateSurface clears it, GetSurface copies it.
/// size 0x64
class FGXSurface {
public:
    unsigned int field_0;          ///< +0x0
    unsigned int m_hIB;            ///< +0x4
    unsigned int m_hVB;            ///< +0x8
    unsigned int field_c;          ///< +0xc
    unsigned int field_10;         ///< +0x10
    unsigned int field_14;         ///< +0x14
    unsigned short field_18[8];    ///< +0x18
    unsigned short field_28[8];    ///< +0x28
    unsigned short field_38[8];    ///< +0x38
    unsigned short field_48[8];    ///< +0x48
    unsigned short field_58;       ///< +0x58
    char unk_5a[0x2];              ///< +0x5a
    unsigned short field_5c;       ///< +0x5c
    char unk_5e[0x2];              ///< +0x5e
    unsigned char field_60;        ///< +0x60
    unsigned char field_61;        ///< +0x61
    char unk_62[0x2];              ///< +0x62
};

// Element of CRenderer9::m_TextureList from +0x1a8bc (stride 0x24).
/// size 0x24
class TextureDesc {
public:
    unsigned int field_0;   ///< +0x0
    unsigned int m_uWidth;  ///< +0x4
    unsigned int m_uHeight; ///< +0x8
    unsigned int field_c;   ///< +0xc
    unsigned int field_10;  ///< +0x10
    char unk_14[0x8];       ///< +0x14
    unsigned int field_1c;  ///< +0x1c
    char unk_20[0x4];       ///< +0x20
};

// CreateRenderer allocates 0x251cc bytes.
/// size 0x251cc
class CRenderer9 {
public:
    /// 0x08d0a6c4
    CRenderer9(_D3DPRESENT_PARAMETERS_* a1);
    /// 0x08d0b0a4
    ~CRenderer9();
    /// 0x08d09fb4
    UNK_RET CreatePixelCounter();
    /// 0x08d0a02e
    UNK_RET ReleasePixelCounter(unsigned int a1);
    /// 0x08d0a090
    UNK_RET ReadPixelCounter(unsigned int a1, unsigned long long* a2);
    /// 0x08d0aee0
    UNK_RET CreateDefaultTextures();
    /// 0x08d0af70
    UNK_RET CreateVertexDecls();
    /// 0x08d0b302
    UNK_RET PostShaderInit();
    /// 0x08d0b524
    unsigned int CreateStaticIB(FireGrafix::_FGX_INDEXFORMATS a1, unsigned int a2, unsigned int a3, unsigned int a4);
    /// 0x08d0b556
    unsigned int CreateStaticVB(FireGrafix::_FGX_VERTEXFORMATS a1, unsigned int a2, unsigned int a3, unsigned int a4);
    /// 0x08d0b590
    void* LockStaticVB(unsigned int a1, unsigned int a2);
    /// 0x08d0b5ea
    UNK_RET UnlockVB(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08d0b618
    void* LockStaticIB(unsigned int a1, unsigned int a2);
    /// 0x08d0b672
    UNK_RET UnlockIB(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08d0b6a0
    UNK_RET IncFrame();
    /// 0x08d0b758
    UNK_RET Reap();
    /// 0x08d0bb5c
    UNK_RET CreateVB(FireGrafix::_FGX_VERTEXFORMATS a1, unsigned int a2, unsigned int a3);
    /// 0x08d0be56
    unsigned int CreateDynamicVB(FireGrafix::_FGX_VERTEXFORMATS a1, unsigned int a2, unsigned int a3);
    /// 0x08d0be90
    unsigned int CreateAndFillStaticVB(FireGrafix::_FGX_VERTEXFORMATS a1, unsigned int a2, void* a3, unsigned int a4, unsigned int a5);
    /// 0x08d0bf48
    unsigned int CreateAndFillStaticIB(FireGrafix::_FGX_INDEXFORMATS a1, unsigned int a2, void* a3, unsigned int a4, unsigned int a5);
    /// 0x08d0c00c
    UNK_RET CreateIB(FireGrafix::_FGX_INDEXFORMATS a1, unsigned int a2, unsigned int a3);
    /// 0x08d0c312
    unsigned int CreateDynamicIB(FireGrafix::_FGX_INDEXFORMATS a1, unsigned int a2, unsigned int a3);
    /// 0x08d0c344
    UNK_RET CreateViewport(FireGrafix::Viewport const& a1);
    /// 0x08d0c41c
    UNK_RET ModifyViewport(unsigned int a1, FireGrafix::Viewport const& a2);
    /// 0x08d0c4f4
    unsigned int CreatePixelShaderWithName(char const* a1, unsigned long const* a2, unsigned int a3);
    /// 0x08d0c56e
    UNK_RET ReloadPixelShader(unsigned int a1, unsigned long const* a2, unsigned int a3);
    /// 0x08d0c5e0
    UNK_RET ReloadVertexShader(unsigned int a1, unsigned long const* a2, unsigned int a3, unsigned long a4);
    /// 0x08d0c652
    unsigned int CreateVertexShaderWithName(char const* a1, unsigned long const* a2, unsigned int a3, unsigned long a4);
    /// 0x08d0c6cc
    UNK_RET EnableRendering(bool a1);
    /// 0x08d0c6dc
    UNK_RET CreateRenderTextureAtHandle(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, char const* a6);
    /// 0x08d0c7ea
    unsigned int CreateRenderTexture(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, char const* a5);
    /// 0x08d0c848
    UNK_RET RecreateRenderTexture(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, char const* a6);
    /// 0x08d0c8cc
    UNK_RET CreateRenderCubemap(unsigned int a1, unsigned int a2, unsigned int a3, char const* a4);
    /// 0x08d0c9da
    UNK_RET CreateRenderTexture(IDirect3DTexture9* a1, char const* a2);
    /// 0x08d0cab6
    UNK_RET AddTexture(IDirect3DTexture9* a1, char const* a2);
    /// 0x08d0cb8c
    UNK_RET AddCubeTexture(IDirect3DCubeTexture9* a1, char const* a2);
    /// 0x08d0cc62
    UNK_RET CreateDepthStencilResourceAtHandle(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4);
    /// 0x08d0cd5e
    UNK_RET CreateDepthStencilResource(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08d0ce6a
    UNK_RET RecreateDepthStencilResource(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4);
    /// 0x08d0cf7e
    UNK_RET RecreateTexture(unsigned int a1, FireGrafix::TextureData* a2, bool a3);
    /// 0x08d0d38c
    unsigned int CreateTexture(FireGrafix::TextureData* a1);
    /// 0x08d0d5f0
    unsigned int LoadTexture(char const* a1);
    /// 0x08d0d692
    UNK_RET GetTexture(unsigned int a1);
    /// 0x08d0d6b8
    FireGrafix::TextureDesc* GetTextureDesc(unsigned int a1);
    /// 0x08d0d6e0
    unsigned int LoadCubeTexture(char const* a1);
    /// 0x08d0d8d6
    UNK_RET ReloadTexture(char const* a1, unsigned int a2);
    /// 0x08d0db74
    UNK_RET ReloadTexture(IDirect3DBaseTexture9* a1, unsigned int a2);
    /// 0x08d0dbc0
    UNK_RET ReloadCubeTexture(char const* a1, unsigned int a2);
    /// 0x08d0de56
    UNK_RET ReleaseSurface(unsigned int a1);
    /// 0x08d0de82
    UNK_RET CreateSurface(FireGrafix::FGXSurface** a1, unsigned int* a2);
    /// 0x08d0ded0
    UNK_RET CreateSurface(FireGrafix::FGXSurface** a1, unsigned int* a2, unsigned int a3);
    /// 0x08d0df28
    UNK_RET CreateCopiedSurface(FireGrafix::FGXSurface** a1, unsigned int* a2, unsigned int a3);
    /// 0x08d0df8a
    unsigned int GetFreeSurfaceCount();
    /// 0x08d0dfae
    UNK_RET GetSurface(unsigned int a1, FireGrafix::FGXSurface* a2);
    /// 0x08d0dfdc
    unsigned int CreateShaderPackageInstance(unsigned int* a1, unsigned int* a2, unsigned int* a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int a8);
    /// 0x08d0e0be
    UNK_RET ReleaseShaderPackageInstance(unsigned int a1);
    /// 0x08d0e0ea
    UNK_RET AddRefShaderPackageInstance(unsigned int a1);
    /// 0x08d0e116
    UNK_RET Render_CleanUpRenderedSurfaces();
    /// 0x08d0e16a
    UNK_RET RenderImmediateBegin();
    /// 0x08d0e2b2
    UNK_RET SetBlendStatesImmediate(unsigned int a1, unsigned int a2);
    /// 0x08d0e32c
    UNK_RET RenderSurface(FireGrafix::FGXSurface* a1, void* a2, unsigned int a3, unsigned int a4, unsigned int a5);
    /// 0x08d0e970
    UNK_RET UpdateSubResource(FireGrafix::CommandHeader* a1);
    /// 0x08d0ea78
    UNK_RET CopyRenderTexture(FireGrafix::CommandHeader* a1);
    /// 0x08d0ec4a
    UNK_RET ResolveRenderTexture(FireGrafix::CommandHeader* a1);
    /// 0x08d0ed02
    UNK_RET GenerateMips(FireGrafix::CommandHeader* a1);
    /// 0x08d0ed34
    UNK_RET Render(IDirect3DSwapChain9* a1);
    /// 0x08d0f7de
    UNK_RET Present();
    /// 0x08d0f83c
    UNK_RET AquireDevice();
    /// 0x08d0f89c
    UNK_RET OnDeviceReset();
    /// 0x08d0ff38
    UNK_RET CreateSampler(_D3DTEXTUREADDRESS a1, _D3DTEXTUREADDRESS a2, _D3DTEXTUREADDRESS a3, _D3DTEXTUREFILTERTYPE a4, _D3DTEXTUREFILTERTYPE a5, _D3DTEXTUREFILTERTYPE a6, unsigned int a7, bool a8);
    /// 0x08d0ffc2
    UNK_RET CreateAlphaState(bool a1, _D3DBLEND a2, _D3DBLEND a3, _D3DBLENDOP a4, _D3DBLEND a5, _D3DBLEND a6, _D3DBLENDOP a7, unsigned long a8, unsigned long a9);
    /// 0x08d10050
    unsigned int CreateAlphaState(bool a1, _D3DBLEND a2, _D3DBLEND a3, _D3DBLENDOP a4, _D3DBLEND a5, _D3DBLEND a6, _D3DBLENDOP a7, unsigned long a8, FGXVector4 const& a9);
    /// 0x08d10130
    unsigned int CreateZState(bool a1, bool a2, _D3DCMPFUNC a3, _D3DCULL a4, float a5);
    /// 0x08d101a6
    UNK_RET CreateZStencilState(bool a1, bool a2, _D3DCMPFUNC a3, _D3DCULL a4, float a5, unsigned char a6, unsigned char a7, _D3DSTENCILOP a8, _D3DSTENCILOP a9, _D3DSTENCILOP a10, _D3DCMPFUNC a11, _D3DSTENCILOP a12, _D3DSTENCILOP a13, _D3DSTENCILOP a14, _D3DCMPFUNC a15);
    /// 0x08d10282
    void* LockDynamicIB(unsigned int a1, unsigned int a2);
    /// 0x08d102dc
    UNK_RET LockAndFillStaticIB(unsigned int a1, void* a2, unsigned int a3);
    /// 0x08d10362
    UNK_RET LockAndFillStaticVB(unsigned int a1, void* a2, unsigned int a3);
    /// 0x08d103e8
    void* LockDynamicVB(unsigned int a1, unsigned int a2);
    /// 0x08d10442
    UNK_RET GetVBSize(FireGrafix::_FGX_VERTEXFORMATS a1, unsigned int a2, unsigned int a3);
    /// 0x08d104c4
    UNK_RET ValidateVertexFormatForShaderMethod(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08d104ca
    UNK_RET ValidateSurface(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08d104d0
    UNK_RET ReleaseDepthStencilResource(unsigned int a1);
    /// 0x08d104fc
    UNK_RET AddCommandSetToRenderList(FireGrafix::CCommandStream* a1, unsigned int a2);
    /// 0x08d1052c
    FireGrafix::CCommandStream* CreateCommandStream(unsigned int a1, unsigned int a2, unsigned int a3, unsigned long a4);
    /// 0x08d10592
    UNK_RET ReleaseCommandStream(FireGrafix::CCommandStream* a1);
    /// 0x08d105c2
    UNK_RET CreateSurfaceSet(unsigned int a1, unsigned int a2);
    /// 0x08d10614
    UNK_RET ReleaseSurfaceSet(FireGrafix::SurfaceSet* a1);
    /// 0x08d1064a
    UNK_RET SetScreenMode(FGXInt2 a1, int a2, int a3, int a4, HWND__* a5);
    /// 0x08d106f2
    UNK_RET GetStats(FireGrafix::RenderStats* a1);
    /// 0x08d1084c
    UNK_RET GetDeclarationElements(FireGrafix::_FGX_VERTEXFORMATS a1, _D3DVERTEXELEMENT9* a2, unsigned int* a3);
    /// 0x08d110e8
    UNK_RET SaveTextureToFile(unsigned int a1, char const* a2, unsigned int a3);
    /// 0x08d11140
    UNK_RET GetBackbufferFormat(unsigned int* a1, unsigned int* a2);
    /// 0x08d11192
    UNK_RET FullReap();

    char unk_0[0x42c];          ///< +0x0
    // Compared with 10 like DeviceSettings::g_DeviceCaps (shadow setup in TerrainSystem::PerformShadowBlur).
    int field_42c;              ///< +0x42c
    char unk_430[0x24d84];      ///< +0x430
    // Incremented by IncFrame.
    unsigned int m_uFrame;      ///< +0x251b4
    char unk_251b8[0x14];       ///< +0x251b8
    /// 0x0e040d80
    static UNK_DATA m_DepthStencilList[0x31c0];
    /// 0x0d29aa40
    static UNK_DATA m_IBList[0x1e5c0];
    /// 0x0e043f40
    static UNK_DATA m_SPIList[0xc10c0];
    /// 0x0ff3a2f0
    static UNK_DATA m_ShaderPackages[0x41800];
    /// 0x0d2b9000
    static UNK_DATA m_SurfaceList[0xd014c0];
    /// 0x0e105000
    static UNK_DATA m_SurfaceSets[0x302c0];
    /// 0x0dfba4c0
    static UNK_DATA m_TextureList[0x868c0];
    /// 0x0d27c480
    static UNK_DATA m_VBList[0x1e5c0];
    /// 0x0ff37c00
    static UNK_DATA m_Viewports[0x600];
    /// 0x0d27c449
    static UNK_DATA m_bFullScreen[0x1];
    /// 0x0e1352c0
    static UNK_DATA m_bNullDevice[0x1];
    /// 0x0d27c448
    static UNK_DATA m_bVSync[0x1];
    /// 0x0e135300
    static UNK_DATA m_kCSManager[0x1e02900];
    /// 0x0d27c444
    static unsigned int m_nScreenHeight;
    /// 0x0d27c440
    static unsigned int m_nScreenWidth;
    /// 0x0ff38200
    static UNK_DATA m_nViewports[0x4];
    /// 0x0ff3824c
    static UNK_DATA m_pCurrentIndexBuffer[0x4];
    /// 0x0ff38240
    static UNK_DATA m_pCurrentPShader[0x4];
    /// 0x0ff38258
    static UNK_DATA m_pCurrentTextures[0x40];
    /// 0x0ff38244
    static UNK_DATA m_pCurrentVShader[0x4];
    /// 0x0ff38298
    static UNK_DATA m_pCurrentVTextures[0x8];
    /// 0x0ff38250
    static UNK_DATA m_pCurrentVertexBuffer[0x4];
    /// 0x0ff38248
    static UNK_DATA m_pCurrentVertexDecl[0x4];
    /// 0x0ff38204
    static UNK_DATA m_pDevice[0x4];
    /// 0x0ff392f0
    static UNK_DATA m_pPShaders[0x1000];
    /// 0x0ff382f0
    static UNK_DATA m_pVShaders[0x1000];
    /// 0x0ff382e8
    static UNK_DATA m_uCurrentAlphaState[0x4];
    /// 0x0ff382a0
    static UNK_DATA m_uCurrentSamplers[0x40];
    /// 0x0ff382e0
    static UNK_DATA m_uCurrentVSamplers[0x8];
    /// 0x0ff382ec
    static UNK_DATA m_uCurrentZState[0x4];
    /// 0x0ff38254
    static UNK_DATA m_uiCurrentVertexBufferSize[0x4];
    /// 0x0ff38208
    static UNK_DATA ms_PresentParams[0x38];
};
}

namespace FireGrafix {
/// 0x08d0a0c1
UNK_RET SetWindowDim(int a1, int a2);
/// 0x08d0a0f3
UNK_RET FireGrafixGetDevice();
/// 0x08d0a108
UNK_RET DeviceMeetsMinimumRequirements(IDirect3D9* a1, int a2, _D3DDEVTYPE a3);
/// 0x08d0a17a
UNK_RET FireGrafixStartup(FireGrafix::StartupParams const& a1);
/// 0x08d0a550
UNK_RET CreateRenderer();
/// 0x08d0a5a5
UNK_RET FireGrafixShutdown();
/// 0x08d0a628
UNK_RET DestroyRenderer(FireGrafix::CRenderer9* a1);
/// 0x08d0a659
UNK_RET ColorFill(D3DXVECTOR4* a1, D3DXVECTOR2 const* a2, D3DXVECTOR2 const* a3, void* a4);
/// 0x08d108af
UNK_RET Enum32BitDisplayModes();
/// 0x08d10b26
UNK_RET GetDriverInfo(FStringA* a1, unsigned int* a2, unsigned int* a3, unsigned int* a4, unsigned int* a5, unsigned int* a6, unsigned int* a7, unsigned int* a8, unsigned int* a9, unsigned int* a10);
/// 0x08d10d41
UNK_RET GetVector3(FStringA* a1);
/// 0x0ff7bba0
extern UNK_DATA g_DisplayModes[0x8000];
/// 0x0ff7bb9c
extern UNK_DATA g_bLargeDepthStencil[0x4];
/// 0x0ff83ba0
extern UNK_DATA g_cDisplayModes[0x4];
/// 0x0ff7bb90
extern UNK_DATA g_pD3D[0x4];
/// 0x0ff7bb94
extern UNK_DATA g_pD3DDevice[0x4];
/// 0x0ff7bb98
extern UNK_DATA g_pDepthStencil[0x4];
/// 0x0ff7bb8c
extern CRenderer9* g_pRenderer;
/// 0x0ff83ba4
extern UNK_DATA g_uMSAASupport[0x4];
}

namespace FireGrafix { namespace DeviceSettings {
/// 0x0ff7bb80
extern UNK_DATA g_DedicatedSystemMemory[0x4];
/// 0x0ff7bb7c
extern UNK_DATA g_DedicatedVideoMemory[0x4];
/// 0x0ff7bb78
extern int g_DeviceCaps;
/// 0x0ff7bb74
extern UNK_DATA g_DeviceId[0x4];
/// 0x0ff7baf0
extern UNK_DATA g_DeviceName[0x80];
/// 0x0ff7bb84
extern UNK_DATA g_SharedMemory[0x4];
/// 0x0ff7bb88
extern UNK_DATA g_StereoEnabled[0x4];
/// 0x0ff7bb70
extern UNK_DATA g_VendorId[0x4];
}}
