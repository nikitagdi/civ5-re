/// origin guess
#pragma once
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/Graphics/TerrainDecalLayer.h"
#include "FirePlace/FireGrafix/ForestShader.h"
#include "FirePlace/FireWorks/FFastVector.h"

class FGXVector4;
namespace FGXMathRect {
class Rect;
}
namespace FireGrafix {
class CCommandStream;
}

// One placed tree: hex offset, tile spot, scale (0 hides it).
/// size 0x1c
class ForestVisTreeData {
public:
    float m_fHexX;   ///< +0x0
    float m_fHexY;   ///< +0x4
    float m_fHexZ;   ///< +0x8
    float m_fHexW;   ///< +0xc
    float m_fTileX;  ///< +0x10
    float m_fTileY;  ///< +0x14
    float m_fScale;  ///< +0x18
};

// Pool of 2000 ForestShader entries (0x34 bytes each, free-list link at +0x30) shared by all ForestVis.
/// size 0x10
class ForestShaderPool {
public:
    FireGrafix::ForestShader* m_pShaders; ///< +0x0
    unsigned int m_uiNumShaders;          ///< +0x4
    unsigned int m_uiCapacity;            ///< +0x8
    unsigned int m_uiFreeHead;            ///< +0xc
};

/// 0x08670b40
static void AppendDecalVertices(TerrainDecalVertexScratch& kBuffer, DecalVertex const* pVertices, unsigned int uiCount);
/// 0x08670ba0
static void AppendDecalIndices(TerrainDecalIndexScratch& kBuffer, unsigned short const* pIndices, unsigned int uiCount);
/// 0x08670c00
static void PushDecalId(FStaticVector<unsigned int, 4u, true, 49u, 0u>& kIds, unsigned int const& uiId);
/// 0x08670c52
static void PushShaderId(FStaticVector<unsigned int, 8u, true, 49u, 0u>& kIds, unsigned int const& uiId);

/// size 0x84
class ForestVis {
public:
    enum FOREST_VIS_TYPE : int;
    /// 0x0866fed4
    ForestVis(HEXVec_i const& kHex, ForestVis::FOREST_VIS_TYPE eType);
    /// 0x0866ff5e
    void BuildBuffers(unsigned int hVertexBuffer, unsigned int hIndexBuffer, unsigned int hSurface, FFastVector<ForestVisTreeData, false, 49u, 0u, BaseVector<ForestVisTreeData, false>::FDefaultFastVectorAllocator> const& kTrees, unsigned int uiNumShaders, ForestShaderPool& kPool);
    /// 0x08670342
    UNK_RET AllocateShaders(unsigned int uiNumShaders, ForestShaderPool& kPool);
    /// 0x086703c6
    void BuildShadows(FFastVector<ForestVisTreeData, false, 49u, 0u, BaseVector<ForestVisTreeData, false>::FDefaultFastVectorAllocator> const& kTrees);
    /// 0x08670934
    UNK_RET CheckForRefresh(FGXMathRect::Rect& kChanged);
    /// 0x0867096c
    UNK_RET ReleaseShadows();
    /// 0x086709b8
    UNK_RET ReleaseBuffers(ForestShaderPool& kPool);
    /// 0x086709e6
    UNK_RET DeleteShaders(ForestShaderPool& kPool);
    /// 0x08670a4c
    UNK_RET Render(FireGrafix::CCommandStream* pStream, ForestShaderPool& kPool, FireGrafix::ForestShader::SHADER_METHOD eMethod);
    /// 0x08670aca
    bool IsOnScreen(FGXVector4& kPos, int iCamera);

    // Cleared by CheckForRefresh when a changed rect overlaps the bounds; ForestVisSystem::Update then rebuilds.
    bool m_bUpToDate;                                  ///< +0x0
    float m_fBoundsLeft;                               ///< +0x4
    float m_fBoundsTop;                                ///< +0x8
    float m_fBoundsRight;                              ///< +0xc
    float m_fBoundsBottom;                             ///< +0x10
    unsigned int m_hVertexBuffer;                      ///< +0x14
    unsigned int m_hIndexBuffer;                       ///< +0x18
    unsigned int m_hSurface;                           ///< +0x1c
    FStaticVector<unsigned int, 4u, true, 49u, 0u> m_DecalIds;   ///< +0x20
    FStaticVector<unsigned int, 8u, true, 49u, 0u> m_ShaderIds;  ///< +0x40
    // Index into ForestVisSystem::m_TreeLists, drawn at random when the vis is placed.
    unsigned int m_uiTreeList;                         ///< +0x70
    HEXVec_i m_Hex;                                    ///< +0x74
    ForestVis::FOREST_VIS_TYPE m_eType;                ///< +0x80
};
