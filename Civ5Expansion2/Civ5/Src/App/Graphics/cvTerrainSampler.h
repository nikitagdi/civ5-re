/// origin guess
#pragma once
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "Civ5/Src/App/HEXVec.h"
#include "Civ5/Src/App/HEXTransform_i.h"

class HexPatternCell;
class SquareGridPolicy;
template <class T0, class T1, unsigned int T2, unsigned int T3> class DynamicHexGrid;

// MapGlobals::g_TerrainSampler is the one instance (symbol size 0x10034).
/// size 0x10034
class cvTerrainSampler {
public:
    /// 0x08676154
    void Startup(unsigned int uiCount);
    /// 0x086761a4
    void Shutdown();
    /// 0x086761a6
    void StartupGame(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const* pGameGrid, FGXInt2 kHeightMapSize, FGXVector2 vWorldSize, FGXVector3 vWorldCenter, FGXVector3 vTerrainScale, bool a6, float a7);
    /// 0x08676548
    void SetContinentTypes(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const& a1);
    /// 0x086765b4
    void ShutdownGame();
    /// 0x086765f2
    FGXInt2 GetWrappedPosition(FGXInt2 pos) const;
    /// 0x08676634
    FGXVector4 GetWorldLocation(HEXVec const& hex) const;
    /// 0x086766ee
    float GetWorldHeight(FGXVector2 const& pos) const;
    /// 0x08676754
    FGXVector2 GetWorldLocation2D(HEXVec const& hex) const;
    /// 0x086767f2
    FGXVector4 GetHexLocation2D(FGXVector2 const& pos) const;
    /// 0x08676890
    FGXVector2 TexToWorld(FGXVector2 const& tex) const;
    /// 0x086768d2
    bool IsValidWorldLocation(FGXVector2 const& pos) const;
    /// 0x08676926
    float GetWorldHeight(FGXInt2 const& cell) const;
    /// 0x086769ba
    FGXVector3 GetWorldNormal(FGXVector2 const& pos) const;
    /// 0x08676ae0
    FGXVector3 CalcNormal(FGXInt2 const& cell) const;
    /// 0x08676f0e
    FGXVector3 GetWorldNormal(FGXInt2 const& cell) const;
    /// 0x08676ff4
    void GenerateNormalField(bool a1, float a2);
    /// 0x086772d8
    void CalcNormalBlockMax69x69(FGXInt2 const& origin, FGXInt2 size, FGXVector3* pNormals) const;
    /// 0x08678670
    void CalcNormalBlockMax12x12(FGXInt2 const& origin, FGXInt2 size, FGXVector3* pNormals) const;

    float field_0;                                 ///< +0x0
    float field_4;                                 ///< +0x4
    bool field_8;                                  ///< +0x8
    bool m_bGameStarted;                           ///< +0x9
    char unk_a[0x2];                               ///< +0xa
    int m_iHeightMapWidth;                         ///< +0xc
    int m_iHeightMapHeight;                        ///< +0x10
    char unk_14[0x14];                             ///< +0x14
    char unk_28[0x8];                              ///< +0x28
    unsigned int field_30;                         ///< +0x30
    float field_34;                                ///< +0x34
    float field_38;                                ///< +0x38
    float field_3c;                                ///< +0x3c
    float field_40;                                ///< +0x40
    float field_44;                                ///< +0x44
    float field_48;                                ///< +0x48
    char unk_4c[0x1c];                             ///< +0x4c
    char unk_68[0x8];                              ///< +0x68
    unsigned int field_70;                         ///< +0x70
    char unk_74[0x8];                              ///< +0x74
    // The normal grid at +0x74 holds 64x64 blocks; its block table has a state and a storage index per block.
    int m_iNormalBlocksX;                          ///< +0x7c
    int m_iNormalBlocksY;                          ///< +0x80
    unsigned int* m_pNormalBlockTable;             ///< +0x84
    char unk_88[0x8];                              ///< +0x88
    unsigned char* m_pNormalBlocks;                ///< +0x90
    char unk_94[0x4];                              ///< +0x94
    unsigned int m_uNormalBlocksCapacity;          ///< +0x98
    unsigned int field_9c;                         ///< +0x9c
    int m_iSizeX;                                  ///< +0xa0
    int m_iSizeY;                                  ///< +0xa4
    float m_fScaleX;                               ///< +0xa8
    float m_fScaleY;                               ///< +0xac
    float m_fHeightScale;                          ///< +0xb0
    float m_fOffsetX;                              ///< +0xb4
    float m_fOffsetY;                              ///< +0xb8
    float field_bc;                                ///< +0xbc
    float field_c0;                                ///< +0xc0
    float field_c4;                                ///< +0xc4
    char unk_c8[0x40];                             ///< +0xc8
    char unk_108[0x40];                            ///< +0x108
    HEXTransform_i m_kTransform;                   ///< +0x148
    char unk_15c[0x8];                             ///< +0x15c
    unsigned char m_aucContinentType[255 * 255];   ///< +0x164
    char unk_ff65[0x3];                            ///< +0xff65
    float m_afNormalWeightX[25];                   ///< +0xff68
    float m_afNormalWeightY[25];                   ///< +0xffcc
    int m_iNormalFieldReady;                       ///< +0x10030
};

/// 0x0867a6ae
UNK_RET Fun_0867a6ae(UNK_PARAMS);
/// 0x0867a388
UNK_RET Fun_0867a388(UNK_PARAMS);
/// 0x08679c66
UNK_RET Fun_08679c66(UNK_PARAMS);
/// 0x08679cb8
UNK_RET Fun_08679cb8(UNK_PARAMS);
/// 0x0867a198
UNK_RET Fun_0867a198(UNK_PARAMS);
// The paged block grids of the sampler (a 16x16 block of 2-byte or 1-byte cells, or a 64x64 block of 2-byte cells):
// pGrid is the grid object at the first field of its block (width, height, blocks per row and column, block table).
// Sets the size padded to whole blocks and fills the block table with one entry per block: state 1, value *pFill.
/// 0x0867a5c8
void Fun_0867a5c8(void* pGrid, FGXInt2 const* pSize, unsigned short const* pFill, unsigned int uReserve);
// Allocates the block of a state 1 table entry (state 0 afterwards), then copies the 0x200-byte page into it.
/// 0x086799fe
void Fun_086799fe(void* pGrid, void const* pPage, FGXInt2 const* pBlock);
// As Fun_0867a5c8 for 1-byte cells.
/// 0x08679ac0
void Fun_08679ac0(void* pGrid, FGXInt2 const* pSize, unsigned char const* pFill, unsigned int uReserve);
// As Fun_086799fe for a 0x100-byte page.
/// 0x08679ba4
void Fun_08679ba4(void* pGrid, void const* pPage, FGXInt2 const* pBlock);
// As Fun_0867a5c8 for 64x64 blocks of 2-byte cells.
/// 0x08679db0
void Fun_08679db0(void* pGrid, FGXInt2 const* pSize, unsigned short const* pFill, unsigned int uReserve);
// As Fun_086799fe for a 0x2000-byte page.
/// 0x08679e96
void Fun_08679e96(void* pGrid, void const* pPage, FGXInt2 const* pBlock);
// Grows the block storage vector of a grid (data pointer, size, capacity) to uCapacity entries.
/// 0x0867a0f6
void Fun_0867a0f6(void* pVector, unsigned int uCapacity);
/// 0x087fb076
unsigned char Fun_087fb076(void* a1, FGXInt2 const* a2);
/// 0x08679d0a
unsigned short Fun_08679d0a(void* a1, FGXInt2 const* a2);
/// 0x08679d58
void Fun_08679d58(unsigned char* a1, void* a2, FGXInt2 const* a3);
