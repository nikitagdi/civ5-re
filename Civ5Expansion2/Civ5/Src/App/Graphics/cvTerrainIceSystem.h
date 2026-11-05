/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/IceSettings.h"
#include "Civ5/Src/App/HEXTransform_i.h"
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"

class FMemTracker;
class HEXVec_i;
class cvTerrainSampler;
namespace FireGrafix {
class CCommandStream;
}

// The first 0xfea0 bytes are a StaticHexGrid<TerrainIce::HexData, SquareXWrapGridPolicy, 255, 255>: cell byte bit 7 = ice hex.
/// size 0x1001c
class cvTerrainIceSystem {
public:
    // Bounds and surface handle of one ice mesh piece; pieces outside the visible world rect are skipped when drawing.
    /// size 0x14
    struct IceSurface {
        float m_fMinX;                 ///< +0x0
        float m_fMinY;                 ///< +0x4
        float m_fMaxX;                 ///< +0x8
        float m_fMaxY;                 ///< +0xc
        unsigned int m_hSurface;       ///< +0x10
    };

    /// 0x08673c60
    cvTerrainIceSystem();
    /// 0x08673e20
    void SetupMemTracker(FMemTracker* a1);
    /// 0x08673e22
    void Startup();
    /// 0x08673ec8
    void LoadTextures();
    /// 0x08674290
    void Shutdown();
    /// 0x086742e8
    void ReleaseTextures();
    /// 0x086743d0
    void ConfigureGrid(FGXInt2 const& kSize, HEXTransform_i const& kTransform);
    /// 0x086744c8
    void MarkIceHex(HEXVec_i const& kHex);
    /// 0x086744f4
    void BuildIce(cvTerrainSampler const& kSampler);
    /// 0x0867456e
    UNK_RET BuildIceMeshes(TerrainIce::IceSettings const& a1, cvTerrainSampler const& a2);
    /// 0x08674f6a
    void DestroyIce();
    /// 0x0867507a
    void SetSunDir(FGXVector3 const& kShadowDir, FGXVector3 const& kSunDir);
    /// 0x086750b4
    UNK_RET RenderInternal(unsigned int a1, bool a2, bool a3);
    /// 0x08675a18
    void RenderReflections(unsigned int a1);
    /// 0x08675a52
    void Render(unsigned int a1, bool a2);
    /// 0x08675a8c
    void Rebuild(cvTerrainSampler const& kSampler);

    HEXMatrix_i m_matForward;                          ///< +0x0
    HEXMatrix_i m_matInverse;                          ///< +0x40
    HEXTransform_i m_Transform;                        ///< +0x80
    FGXInt2 m_Size;                                    ///< +0x94
    bool m_bWrapX;                                     ///< +0x9c
    unsigned char m_aCells[0xfe01];                    ///< +0x9d
    unsigned char m_ucOutOfRangeCell;                  ///< +0xfe9e
    char unk_fe9f[0x1];                                ///< +0xfe9f
    bool m_bStarted;                                   ///< +0xfea0
    bool m_bIceBuilt;                                  ///< +0xfea1
    char unk_fea2[0x2];                                ///< +0xfea2
    char unk_fea4[0x4];                                ///< +0xfea4
    unsigned int m_hShaderPackage;                     ///< +0xfea8
    unsigned int field_feac;                           ///< +0xfeac
    char unk_feb0[0x24];                               ///< +0xfeb0
    FireGrafix::CCommandStream* m_apCommandStreams[3]; ///< +0xfed4
    unsigned int m_hVertexBuffer;                      ///< +0xfee0
    unsigned int m_hIndexBuffer;                       ///< +0xfee4
    unsigned int field_fee8;                           ///< +0xfee8
    unsigned int m_hDiffuseTexture;                    ///< +0xfeec
    unsigned int m_hEnvironmentCube;                   ///< +0xfef0
    unsigned int m_hBumpTexture0;                      ///< +0xfef4
    unsigned int m_hBumpTexture1;                      ///< +0xfef8
    unsigned int m_hSidesDiffuseTexture;               ///< +0xfefc
    unsigned int m_hSidesBumpTexture0;                 ///< +0xff00
    unsigned int m_hSidesBumpTexture1;                 ///< +0xff04
    float m_fBumpScale;                                ///< +0xff08
    float m_fSidesBumpScale;                           ///< +0xff0c
    float m_fSamplerField4;                            ///< +0xff10
    float m_fMaxHeight;                                ///< +0xff14
    float m_afBoundsRect[4];                           ///< +0xff18
    IceSurface* m_pSurfaces;                           ///< +0xff28
    unsigned int m_uSurfaceCount;                      ///< +0xff2c
    unsigned int m_hAlphaState;                        ///< +0xff30
    FGXVector3 m_vShadowDir;                           ///< +0xff34
    FGXVector3 m_vSunDir;                              ///< +0xff40
    TerrainIce::IceSettings m_Settings;                ///< +0xff4c
};

// Cell byte of a cube hex coordinate; MarkIceHex sets bit 7 in it.
/// 0x08675c2a
unsigned char* Fun_08675c2a(cvTerrainIceSystem* pGrid, HEXVec_i const& kHex);
