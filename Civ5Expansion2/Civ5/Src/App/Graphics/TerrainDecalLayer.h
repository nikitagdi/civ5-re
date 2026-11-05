/// origin guess
#pragma once
#include <d3d9.h>

class FGXVector2;
class TerrainDecalBank;
class TerrainDecalGrid;
class TerrainDecalPool;
namespace FireGrafix {
class CCommandStream;
class TerrainDecalShader;
}

/// size 0x10
class DecalType {
public:
    float m_fPriority; ///< +0x0
    unsigned int m_hAlbedo; ///< +0x4
    unsigned int m_hHeightmap; ///< +0x8
    unsigned int m_hSpec; ///< +0xc
};

/// size 0x10
class DecalVertex {
public:
    float m_fX; ///< +0x0
    float m_fY; ///< +0x4
    float field_8; ///< +0x8
    float field_c; ///< +0xc
};

// Growable buffer of decal vertices (data pointer, count, capacity) filled before CreateDecal.
/// size 0xc
class TerrainDecalVertexScratch {
public:
    DecalVertex* m_pVertices; ///< +0x0
    unsigned int m_uiNumVertices; ///< +0x4
    unsigned int m_uiMaxVertices; ///< +0x8
};

// Growable buffer of 16-bit decal indices (data pointer, count, capacity) filled before CreateDecal.
/// size 0xc
class TerrainDecalIndexScratch {
public:
    unsigned short* m_pIndices; ///< +0x0
    unsigned int m_uiNumIndices; ///< +0x4
    unsigned int m_uiMaxIndices; ///< +0x8
};

/// size 0x8
class TerrainDecalTypeRange {
public:
    unsigned int m_begin; ///< +0x0
    unsigned int m_end; ///< +0x4
};

/// size 0xc
class TerrainDecalDrawItem {
public:
    unsigned int m_bankIndex; ///< +0x0
    unsigned int m_decalIndex; ///< +0x4
    float m_fPriority; ///< +0x8
};

/// size 0x34
class TerrainDecalEntry {
public:
    unsigned int m_next; ///< +0x0
    unsigned int field_4; ///< +0x4
    unsigned char m_flags; ///< +0x8
    unsigned int m_firstVertex; ///< +0xc
    unsigned int m_numTriangles; ///< +0x10
    unsigned int m_hAlbedo; ///< +0x14
    unsigned int m_hHeightmap; ///< +0x18
    unsigned int m_hSpec; ///< +0x1c
    float m_fPriority; ///< +0x20
    float m_fMinX; ///< +0x24
    float m_fMinY; ///< +0x28
    float m_fMaxX; ///< +0x2c
    float m_fMaxY; ///< +0x30
};

/// size 0x10
class TerrainDecalBigItem {
public:
    unsigned int m_next; ///< +0x0
    unsigned int field_4; ///< +0x4
    unsigned int m_bankIndex; ///< +0x8
    unsigned int m_decalIndex; ///< +0xc
};

/// size 0x18
class TerrainDecalGridNode {
public:
    unsigned int m_packedNext; ///< +0x0
    unsigned int m_prev; ///< +0x4
    float m_fX; ///< +0x8
    float m_fY; ///< +0xc
    unsigned int m_bankIndex; ///< +0x10
    unsigned int m_decalIndex; ///< +0x14
};

/// size 0x14
class TerrainDecalNodePool {
public:
    unsigned int field_0; ///< +0x0
    unsigned int field_4; ///< +0x4
    TerrainDecalGridNode* m_pNodes; ///< +0x8
    unsigned int field_c; ///< +0xc
    unsigned int field_10; ///< +0x10
};

/// size 0x20
class TerrainDecalPool {
public:
    unsigned int m_freeHead; ///< +0x0
    unsigned int m_numAllocated; ///< +0x4
    void* m_pItems; ///< +0x8
    unsigned int field_c; ///< +0xc
    unsigned int m_itemCapacity; ///< +0x10
    unsigned int m_numUsed; ///< +0x14
    unsigned int m_usedHead; ///< +0x18
    unsigned int m_usedTail; ///< +0x1c
};

/// size 0x34
class TerrainDecalBank {
public:
    TerrainDecalPool m_decals; ///< +0x0
    unsigned int m_hVertexBuffer; ///< +0x20
    unsigned int m_numVertices; ///< +0x24
    unsigned long long* m_pVertices; ///< +0x28
    int m_bNeedsRepack; ///< +0x2c
    int m_bNeedsUpload; ///< +0x30
};

/// size 0x28
class TerrainDecalGrid {
public:
    float m_fMinX; ///< +0x0
    float m_fMinY; ///< +0x4
    float m_fMaxX; ///< +0x8
    float m_fMaxY; ///< +0xc
    float m_fInvCellW; ///< +0x10
    float m_fInvCellH; ///< +0x14
    unsigned int* m_pCellHeads; ///< +0x18
    int m_cellsX; ///< +0x1c
    int m_cellsY; ///< +0x20
    TerrainDecalNodePool* m_pNodePool; ///< +0x24
};

// Walks the grid nodes whose position lies in a rectangle; Fun_0867d050 starts it, Fun_0867c2a2 advances it.
/// size 0x30
class TerrainDecalGridIterator {
public:
    int m_cellX; ///< +0x0
    int m_cellY; ///< +0x4
    int m_endX; ///< +0x8
    int m_endY; ///< +0xc
    int m_startX; ///< +0x10
    int m_startY; ///< +0x14
    float m_fMinX; ///< +0x18
    float m_fMinY; ///< +0x1c
    float m_fMaxX; ///< +0x20
    float m_fMaxY; ///< +0x24
    TerrainDecalGrid* m_pGrid; ///< +0x28
    unsigned int m_node; ///< +0x2c
};

// The vector parameters point at {data, size, capacity}; Fun_0867c100 returns the index of the new bank.
/// 0x0867c09e
void Fun_0867c09e(DecalType** vec, DecalType const* item);
/// 0x0867c100
unsigned int Fun_0867c100(TerrainDecalBank** vec, TerrainDecalBank const* bank);
/// 0x0867c24e
void Fun_0867c24e(TerrainDecalDrawItem** vec, TerrainDecalDrawItem const* item);
/// 0x0867c2a2
void Fun_0867c2a2(TerrainDecalGridIterator* it);
/// 0x0867d050
void Fun_0867d050(TerrainDecalGridIterator* it, TerrainDecalGrid* grid, float* rect);
/// 0x0867d4e8
unsigned int Fun_0867d4e8(TerrainDecalNodePool* pool, TerrainDecalGridNode const* node);
/// 0x0867d586
void Fun_0867d586(TerrainDecalGrid* grid, unsigned int nodeIndex);
/// 0x0867d77c
unsigned int Fun_0867d77c(TerrainDecalPool* pool, TerrainDecalBigItem const* item);
/// 0x0867d7fa
void Fun_0867d7fa(TerrainDecalPool* pool, unsigned int index);
/// 0x0867d9f8
unsigned int Fun_0867d9f8(TerrainDecalPool* pool, TerrainDecalEntry const* entry);
/// 0x0867dabe
void Fun_0867dabe(TerrainDecalPool* pool, unsigned int index);
/// 0x0867dd5e
void Fun_0867dd5e(void** vec, unsigned int capacity);
/// 0x0867e292
void Fun_0867e292(void** vec);
/// 0x0867c02c
void Fun_0867c02c(TerrainDecalGrid* grid);
/// 0x0867c1a0
void Fun_0867c1a0(TerrainDecalGrid* grid, unsigned int nodeIndex);
/// 0x0867d294
void Fun_0867d294(TerrainDecalPool* pool, unsigned int index, unsigned int next);
/// 0x0867d3bc
void Fun_0867d3bc(TerrainDecalPool* pool, unsigned int index, unsigned int next);
/// 0x0867e0c6
void Fun_0867e0c6(TerrainDecalBank** vec, TerrainDecalBank* begin, unsigned int count);
/// 0x0867e1d6
void Fun_0867e1d6(TerrainDecalGrid* grid);

// TerrainSystem and TerrainEditorSystem both embed it at +0x0; their constructors clear +0x4f4..+0x508.
/// size 0x50c
class TerrainDecalLayer {
public:
    unsigned int m_hIndexBuffer; ///< +0x0
    char unk_4[0x14]; ///< +0x4
    FireGrafix::TerrainDecalShader* m_pShader; ///< +0x18
    TerrainDecalTypeRange m_typeRanges[10]; ///< +0x1c
    TerrainDecalGrid m_grid; ///< +0x6c
    TerrainDecalPool m_bigDecals; ///< +0x94
    TerrainDecalBank* m_pBanks; ///< +0xb4
    unsigned int m_numBanks; ///< +0xb8
    unsigned int field_bc; ///< +0xbc
    DecalType* m_pTypes; ///< +0xc0
    unsigned int m_numTypes; ///< +0xc4
    unsigned int field_c8; ///< +0xc8
    char unk_cc[0x400]; ///< +0xcc
    unsigned char field_4cc; ///< +0x4cc
    char unk_4cd[0x3]; ///< +0x4cd
    TerrainDecalDrawItem* m_pDrawQueue; ///< +0x4d0
    unsigned int m_numDrawQueue; ///< +0x4d4
    unsigned int field_4d8; ///< +0x4d8
    TerrainDecalNodePool m_gridNodes; ///< +0x4dc
    unsigned int m_hBlankSpec; ///< +0x4f0
    TerrainDecalVertexScratch m_scratchVertices; ///< +0x4f4
    TerrainDecalIndexScratch m_scratchIndices; ///< +0x500

    /// 0x0867a8dc
    void Startup();
    /// 0x0867aa1c
    UNK_RET LoadTerrainDecals(char const* path);
    /// 0x0867ae88
    void Shutdown();
    /// 0x0867aed2
    void StartupGame();
    /// 0x0867afb6
    void ShutdownGame();
    /// 0x0867b0b2
    unsigned int CreateDecal(DecalType const& type, DecalVertex const* pVerts, unsigned int numVerts, unsigned short const* pIndices, unsigned int numTriangles, bool bPassFlag);
    /// 0x0867b79e
    void RepackDirtyBank(TerrainDecalBank& bank);
    /// 0x0867b85e
    unsigned int CreateDecal(unsigned int typeCategory, DecalVertex const* pVerts, unsigned int numVerts, unsigned short const* pIndices, unsigned int numTriangles, bool bPassFlag);
    /// 0x0867b8e6
    void DestroyDecal(unsigned int handle);
    /// 0x0867ba16
    void RepackAllDirtyBanks();
    /// 0x0867ba6a
    void Update();
    /// 0x0867bb5c
    void Render(FireGrafix::CCommandStream* stream, FGXVector2 const& minCorner, FGXVector2 const& maxCorner, D3DXMATRIX* pMatrix, int bPassFlag);
};
