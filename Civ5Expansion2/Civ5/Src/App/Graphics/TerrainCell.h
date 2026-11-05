/// origin guess
#pragma once
#include "FirePlace/FireWorks/FJob.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainCellBaseIBVBFallbackJob.h"
#include "Civ5/Src/App/Graphics/TerrainCellJob.h"
#include "Civ5/Src/App/Graphics/TerrainCellStatJob.h"
#include <d3d9.h>

class FGXInt2;
class FGXVector2;
class FGXVector3;
class FGXVector4;
enum TerrainCellStatus : int;
enum TextureProjectionMode : int;
namespace FireGrafix {
class CCommandStream;
template <int T0> class VStruct;
}

// Allocated by Fun_0891116e in one block after the terrain's eight cell-pointer arrays (stride 0x330).
/// size 0x330
class TerrainCell {
public:
    Terrain* m_pTerrain;                    ///< +0x0
    HEXVec_i m_aHexes[16];                  ///< +0x4
    unsigned int m_nHexes;                  ///< +0xc4
    unsigned int m_uCompositeVBSpace;       ///< +0xc8
    unsigned int m_uCompressedVBSpace;      ///< +0xcc
    unsigned int m_uIBSpace;                ///< +0xd0
    int m_iWidth;                           ///< +0xd4
    int m_iHeight;                          ///< +0xd8
    int m_iX;                               ///< +0xdc
    int m_iY;                               ///< +0xe0
    int field_e4;                           ///< +0xe4
    int field_e8;                           ///< +0xe8
    unsigned int field_ec;                  ///< +0xec
    unsigned int field_f0;                  ///< +0xf0
    unsigned int m_uBaseIB;                 ///< +0xf4
    unsigned int m_uBaseIB2;                ///< +0xf8
    float m_afOrigin[4];                    ///< +0xfc
    float m_fMinX;                          ///< +0x10c
    float m_fMinY;                          ///< +0x110
    float m_fMinZ;                          ///< +0x114
    float m_fMaxX;                          ///< +0x118
    float m_fMaxY;                          ///< +0x11c
    float m_fMaxZ;                          ///< +0x120
    float m_fCenterX;                       ///< +0x124
    float m_fCenterY;                       ///< +0x128
    float m_fCenterZ;                       ///< +0x12c
    float m_fRadius;                        ///< +0x130
    bool m_bVisible;                        ///< +0x134
    // Terrain::LaunchJobs clears it each frame so a cell seen by two cameras launches once.
    bool m_bLaunchedThisFrame;              ///< +0x135
    bool m_bJobsLaunched;                   ///< +0x136
    char unk_137[0x1];                  ///< +0x137
    unsigned int m_uClassMask;              ///< +0x138
    Terrain::SharedData* m_pShared;         ///< +0x13c
    TerrainCellBaseIBVBFallbackJob m_kFallbackJob; ///< +0x140
    unsigned int m_uSurface0;               ///< +0x154
    unsigned int m_uSurface1;               ///< +0x158
    unsigned int m_uTessSurface0;           ///< +0x15c
    unsigned int m_uTessSurface1;           ///< +0x160
    unsigned int m_uOverlayTessSurface;     ///< +0x164
    unsigned int m_uBaseVB;                 ///< +0x168
    int m_nBaseVertices;                    ///< +0x16c
    int m_nBaseTriangles;                   ///< +0x170
    // Copied from SharedData::m_nTextures when the cell is initialized.
    int m_nTerrainTextures;                 ///< +0x174
    // Cache page edge in texels (0x40..0x200).
    unsigned int m_uPageSize;               ///< +0x178
    // Actual edge of the cache textures taken in RequestCacheTexture; the size BuildTexture renders at.
    unsigned int field_17c;                 ///< +0x17c
    unsigned int m_uMipSizeNeeded;          ///< +0x180
    unsigned int field_184;                 ///< +0x184
    unsigned int field_188;                 ///< +0x188
    unsigned int field_18c;                 ///< +0x18c
    unsigned int m_uUncompressedTex0;       ///< +0x190
    unsigned int m_uUncompressedTex1;       ///< +0x194
    unsigned int m_uCacheTex0;              ///< +0x198
    unsigned int m_uCacheTex1;              ///< +0x19c
    float m_afPlaneDistance[6];             ///< +0x1a0
    float m_fWeight;                        ///< +0x1b8
    int field_1bc;                          ///< +0x1bc
    // Bit 0: one of its hexes has FOW grid value 0; bit 1: value 1.
    int m_iFOWFlags;                        ///< +0x1c0
    int m_iTessLevel;                       ///< +0x1c4
    int m_iOverlayTessLevel;                ///< +0x1c8
    // Renderer frames of the last invalidation and of the last Build.
    unsigned int m_uInvalidatedFrame;       ///< +0x1cc
    unsigned int m_uBuiltFrame;             ///< +0x1d0
    unsigned int field_1d4;                 ///< +0x1d4
    char unk_1d8[0x8];                  ///< +0x1d8
    long m_lPendingJobs;                    ///< +0x1e0
    TerrainCellStatJob m_kStatJob;          ///< +0x1e4
    unsigned int m_nTextures;               ///< +0x1f8
    unsigned int m_aTextureIds[0x40];       ///< +0x1fc
    int m_bHasSpecular;                     ///< +0x2fc
    int field_300;                          ///< +0x300
    unsigned char* m_pCompositeVertices;    ///< +0x304
    unsigned char* m_pCompressedVertices;   ///< +0x308
    TerrainCellJob* m_pJobs;                ///< +0x30c
    TerrainCellStatus m_eStatus;            ///< +0x310
    TerrainCellStatus m_eTargetStatus;      ///< +0x314
    unsigned char* m_pFallbackVertices;     ///< +0x318
    unsigned char* m_pFallbackIndices;      ///< +0x31c
    float field_320;                        ///< +0x320
    float field_324;                        ///< +0x324
    int field_328;                          ///< +0x328
    char unk_32c[0x4];                      ///< +0x32c

    /// 0x08911a68
    void DeleteSurfacesAndBuffers(bool bBuffers, bool bCacheTextures, bool bRenderTextures);
    /// 0x08911b76
    void RemoveBase();
    /// 0x08911d5a
    void BuildTextureVertexBatch(int iXBegin, int iXEnd, int iYBegin, int iYEnd, FireGrafix::VStruct<16>* a5);
    /// 0x089126ec
    void BuildBaseVBandIBFallback(unsigned int a1);
    /// 0x08912bba
    void CalcBlendVector(float* afBlend, TextureProjectionMode eMode, FGXInt2 const& kHex) const;
    /// 0x08912e0a
    void BuildTextureVBs(FJobSet* pJobSet);
    /// 0x08913044
    void CreateTerrainRenderSurface(unsigned int* puHandle, unsigned int hTexture0, unsigned int hTexture1, int iClass, bool bTessellation, unsigned int a6, unsigned int a7, unsigned int a8);
    /// 0x08913238
    void BuildBase();
    /// 0x089135ec
    void RecreateTessellationSurfaces(unsigned int uLevel);
    /// 0x08913766
    void CreateBuffers();
    /// 0x08913ab4
    void CreateNormalCompositeSurface(unsigned int* a1, unsigned int a2);
    /// 0x08913b5c
    void CreateResolveSurface(unsigned int* a1, unsigned int a2);
    /// 0x08913bf2
    void CreateMicroShadowSurface(unsigned int* a1, unsigned int a2, unsigned int a3);
    /// 0x08913bf4
    void CreateCachedCompSurface(unsigned int* puHandle, unsigned int hNormal, unsigned int hRender, unsigned int hSpec, unsigned int hShadow, unsigned int uTriangleCount);
    /// 0x08913dfc
    void BuildTexture(TerrainCellStatus eStatus, FireGrafix::CCommandStream* pStream);
    /// 0x0891568c
    void DemoteToCacheOnly();
    /// 0x089156da
    float GetDistanceFromFrustum(D3DXMATRIX* a1, FGXVector4* a2);
    /// 0x089157c8
    void CalcWeight();
    /// 0x089158ca
    bool IsVisible(FGXVector4* pFrustumPlanes);
    /// 0x089159de
    void CalculateStats();
    /// 0x08915d1c
    void CalculateTextureContributions();
    /// 0x08916132
    void LaunchStatsBuild(FJobSet* pJobSet);
    /// 0x0891617a
    bool IntersectRay(FGXVector3& vOrigin, FGXVector3& vDir, FGXVector2* pTexCoord, FGXInt2* pHit, float* pfDistance);
    /// 0x08916510
    void SetBuildParameters(int iX, int iY, int iWidth, int iHeight);
    /// 0x0891674c
    void LaunchJobs(FJobSet* pJobSet);
    /// 0x089168d0
    bool RequestCacheTexture();
    /// 0x08916a7e
    bool RequestUncompressedTextures();
    /// 0x08916d20
    void CleanuponFail();
    /// 0x08916de0
    void Build(unsigned int uFrame, FireGrafix::CCommandStream* pStream);
    /// 0x08916f30
    float CalcMaxWidth(D3DXMATRIX* pViewProj);
    /// 0x08917166
    unsigned int CalcMIPLevelNeeded(D3DXMATRIX* pViewProj);
    /// 0x089171d6
    float CalcTessLODNeeded(D3DXMATRIX* pViewProj);
    /// 0x08917300
    void Render(FireGrafix::CCommandStream* pStream, D3DXMATRIX* a2, D3DXMATRIX* a3);
    /// 0x0891759c
    void RenderOverlay(FireGrafix::CCommandStream* pStream, D3DXMATRIX* a2, D3DXMATRIX* a3);
    /// 0x08917780
    void CalcTexCoords(FGXVector2* a1, int a2, FGXVector3& a3) const;
    /// 0x08917f50
    bool IntersectLine(FGXVector2* a1, FGXVector2* a2);

    // Static base mesh buffers shared by the cells when tessellation is off (g_uTerrainTessLevel 0).
    // A slot is reusable when its in-use flag is clear and its size is large enough; a handle of 0 ends the list.
    /// 0x0c059a10
    static unsigned int s_auBaseVBHandle[0x600];
    /// 0x0c05b210
    static unsigned int s_auBaseIBHandle[0x600];
    // Vertex count of the slot's vertex buffer.
    /// 0x0c05ca10
    static int s_aiBaseVBSize[0x600];
    // Index count of the slot's index buffer.
    /// 0x0c05e210
    static int s_aiBaseIBSize[0x600];
    /// 0x0c05fa10
    static char s_abBaseVBInUse[0x600];
    /// 0x0c060010
    static char s_abBaseIBInUse[0x600];
};
