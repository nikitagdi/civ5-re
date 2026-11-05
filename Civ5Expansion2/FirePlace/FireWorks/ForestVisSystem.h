/// origin guess
#pragma once
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/Graphics/cvGeneralRefHandle.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/ForestVis.h"

class ForestVisBuilderContext;
class ForestVisSystem;
class ForestVisSystemDataDef;
class ForestVisSystem_Init;
class ForestVisSystem_Invalidate;
class ForestVisSystem_RemoveForest;
class ForestVisSystem_RemoveJungle;
template <class> class ForestVisSystem_Accessor;
namespace FireGrafix {
class CCommandStream;
}

// L8 mask of the spots a tree may take, one per forest_tile<N>.dds (128x128 texels).
/// size 0x4008
class ForestVisTileMask {
public:
    unsigned char m_aucMask[0x4008]; ///< +0x0
};

// Event payload of the forest placement; m_ucVariant picks ForestVis type 2 * m_ucVariant + 1.
/// size 0x10
class ForestVisSystem_PlaceForest {
public:
    HEXVec_i m_Hex;            ///< +0x0
    unsigned char m_ucVariant; ///< +0xc
};

// Event payload of the jungle placement; ForestVis type is 2 with m_bVariant clear, 4 with it set.
/// size 0x10
class ForestVisSystem_PlaceJungle {
public:
    HEXVec_i m_Hex;   ///< +0x0
    bool m_bVariant;  ///< +0xc
};

// Mesh vertex as the Granny loader leaves it; BuildTreeBuffers copies the 0x20 bytes and appends the tree slot byte (0x24-byte VB vertex).
/// size 0x20
class ForestVisMeshVertex {
public:
    unsigned char m_aucData[0x20]; ///< +0x0
};

// One tree mesh: ranges of ForestVisSystem::m_Vertices and m_Indices.
/// size 0x10
class ForestVisMeshRange {
public:
    unsigned int m_uiFirstVertex; ///< +0x0
    unsigned int m_uiNumVertices; ///< +0x4
    unsigned int m_uiFirstIndex;  ///< +0x8
    unsigned int m_uiNumIndices;  ///< +0xc
};

/// size 0x1c8
class ForestVisSystem {
public:
    // The meshes of one tree style inside the shared builder context's mesh array.
    /// size 0x8
    class ContinentTreeLibrary {
    public:
        unsigned int m_uiFirstMesh; ///< +0x0
        unsigned int m_uiNumMeshes; ///< +0x4
    };

    // Node of the vis list: m_uiNext is the index of the next node, 0xfffffff ends the list.
    /// size 0x8c
    class VisNode {
    public:
        unsigned int m_uiNext; ///< +0x0
        unsigned int field_4;  ///< +0x4
        ForestVis m_Vis;       ///< +0x8
    };

    /// 0x08666428
    ForestVisSystem();
    /// 0x08666592
    UNK_RET Startup();
    /// 0x0866662a
    UNK_RET LoadSettingsFromXML();
    /// 0x0866686e
    UNK_RET LoadFilePaths();
    /// 0x086669c6
    UNK_RET LoadMeshes();
    /// 0x08666d64
    UNK_RET BuildTreeBuffers();
    /// 0x086676d2
    UNK_RET LoadTreeTiles();
    /// 0x086679fe
    UNK_RET Shutdown();
    /// 0x08667b7e
    UNK_RET StartupGame();
    /// 0x08667ba6
    UNK_RET BuildTreeLists();
    /// 0x08667ca0
    UNK_RET CreateTreeShadows();
    /// 0x08667d04
    UNK_RET ShutdownGame();
    /// 0x08667d34
    UNK_RET DestroyTreeLists();
    /// 0x08667d78
    UNK_RET ReleaseAllVises();
    /// 0x08667e0a
    UNK_RET RenderInternal(unsigned int a1, bool a2);
    /// 0x08668284
    UNK_RET OnRenderView(unsigned int a1);
    /// 0x086682b6
    UNK_RET OnRenderReflection(unsigned int a1);
    /// 0x086682e8
    UNK_RET Update();
    /// 0x0866869c
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<ForestVisSystem, unsigned int, ForestVisSystemDataDef, ForestVisSystem_Accessor>& a1, ForestVisSystem_Init a2);
    /// 0x0866869e
    UNK_RET InsertIntoVisList(ForestVis& a1);
    /// 0x0866880c
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<ForestVisSystem, unsigned int, ForestVisSystemDataDef, ForestVisSystem_Accessor>& a1, ForestVisSystem_PlaceForest a2);
    /// 0x086688b8
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<ForestVisSystem, unsigned int, ForestVisSystemDataDef, ForestVisSystem_Accessor>& a1, ForestVisSystem_PlaceJungle a2);
    /// 0x0866896a
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<ForestVisSystem, unsigned int, ForestVisSystemDataDef, ForestVisSystem_Accessor>& a1, ForestVisSystem_RemoveForest a2);
    /// 0x086689ac
    UNK_RET ReleaseVis(HEXVec_i a1);
    /// 0x08668aaa
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<ForestVisSystem, unsigned int, ForestVisSystemDataDef, ForestVisSystem_Accessor>& a1, ForestVisSystem_RemoveJungle a2);
    /// 0x08668aec
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<ForestVisSystem, unsigned int, ForestVisSystemDataDef, ForestVisSystem_Accessor>& a1, ForestVisSystem_Invalidate a2);
    /// 0x08668b48
    UNK_RET DestroyFromHandle(cvGeneralRefHandle<ForestVisSystem, unsigned int, ForestVisSystemDataDef, ForestVisSystem_Accessor>& a1);
    /// 0x08668b4a
    UNK_RET CopyMeshFromTemplateBuilder(ForestVisBuilderContext& a1, ForestVisSystem::ContinentTreeLibrary& a2);
    /// 0x08668cda
    UNK_RET AddRandomLocationsInTile(FFastVector<ForestVisTreeData, false, 49u, 0u, BaseVector<ForestVisTreeData, false>::FDefaultFastVectorAllocator>& kTrees, unsigned int uiTile);

    FireGrafix::CCommandStream* m_pCommandStream; ///< +0x0
    // All tree meshes of the five continent styles, packed back to back; m_aForestMeshes/m_aJungleMeshes index m_MeshRanges.
    FFastVector<ForestVisMeshVertex, false, 49u, 0u, BaseVector<ForestVisMeshVertex, false>::FDefaultFastVectorAllocator> m_Vertices; ///< +0x4
    FFastVector<unsigned int, false, 49u, 0u, BaseVector<unsigned int, false>::FDefaultFastVectorAllocator> m_Indices;                ///< +0x10
    FFastVector<ForestVisMeshRange, false, 49u, 0u, BaseVector<ForestVisMeshRange, false>::FDefaultFastVectorAllocator> m_MeshRanges; ///< +0x1c
    // Five slots per kind: four continent styles plus a fifth that reuses the europe files.
    ContinentTreeLibrary m_aForestMeshes[5];      ///< +0x28
    ContinentTreeLibrary m_aJungleMeshes[5];      ///< +0x50
    unsigned int field_78;                        ///< +0x78
    unsigned int field_7c;                        ///< +0x7c
    VisNode* m_pVisNodes;                         ///< +0x80
    unsigned char unk_84[0xc];                    ///< +0x84
    unsigned int m_uiFirstVis;                    ///< +0x90
    unsigned int field_94;                        ///< +0x94
    // 20 placement lists built once per game; every vis draws one of them.
    FFastVector<FFastVector<ForestVisTreeData, false, 49u, 0u, BaseVector<ForestVisTreeData, false>::FDefaultFastVectorAllocator>, false, 49u, 0u, BaseVector<FFastVector<ForestVisTreeData, false, 49u, 0u, BaseVector<ForestVisTreeData, false>::FDefaultFastVectorAllocator>, false>::FDefaultFastVectorAllocator> m_TreeLists; ///< +0x98
    FFastVector<ForestVisTileMask, false, 49u, 0u, BaseVector<ForestVisTileMask, false>::FDefaultFastVectorAllocator> m_TileMasks; ///< +0xa4
    unsigned int m_ahForestVB[5];                 ///< +0xb0
    unsigned int m_ahForestIB[5];                 ///< +0xc4
    unsigned int m_ahJungleVB[5];                 ///< +0xd8
    unsigned int m_ahJungleIB[5];                 ///< +0xec
    unsigned int m_ahForestSurface[5];            ///< +0x100
    unsigned int m_ahJungleSurface[5];            ///< +0x114
    unsigned int m_uiRandomTreesPerTile;          ///< +0x128
    unsigned int m_uiGriddedTreesPerTile;         ///< +0x12c
    unsigned int m_uiNumTiles;                    ///< +0x130
    float m_fOverlayAlpha;                        ///< +0x134
    float m_fSpaceBetween;                        ///< +0x138
    bool m_bGameStarted;                          ///< +0x13c
    char const* m_apszForestMeshFiles[5];         ///< +0x140
    char const* m_apszJungleMeshFiles[5];         ///< +0x154
    char const* m_apszForestTextureFiles[5];      ///< +0x168
    char const* m_apszJungleTextureFiles[5];      ///< +0x17c
    char const* m_apszForestOverlayFiles[5];      ///< +0x190
    char const* m_apszJungleOverlayFiles[5];      ///< +0x1a4
    ForestShaderPool m_ShaderPool;                ///< +0x1b8
};

/// 0x08668ca1
UNK_RET ReverseBits(unsigned int a1, unsigned int a2);

/// 0x0866a1c5
UNK_RET Fun_0866a1c5(ForestVisTreeData* pFirst, ForestVisTreeData* pLast, int iDepth);
