/// origin guess
#pragma once
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FJob.h"
#include "FirePlace/FireWorks/FTempHeap.h"

class cvCompressedTerrainPiecesLibrary;
class cvTerrainSampler;

// A run [m_iBegin, m_iEnd) of points in the coast point list handed to BlendTerrainHeightmap.
/// size 0xc
class SplineRange {
public:
    /// inline
    SplineRange() : m_iBegin(0), m_iEnd(0) {}

    int m_iBegin;  ///< +0x0
    int m_iEnd;    ///< +0x4
    int field_8;   ///< +0x8
};

// One terrain piece to blit; the corners are texel positions.
/// size 0x30
class BlendCell {
public:
    unsigned int m_bSmall;            ///< +0x0
    int m_iIDMapIndex;                ///< +0x4
    int m_iHeightmapIndex;            ///< +0x8
    int m_iBlendmap1Index;            ///< +0xc
    int m_iBlendmap2Index;            ///< +0x10
    FGXInt2 m_kCorner0;               ///< +0x14
    FGXInt2 m_kCorner1;               ///< +0x1c
    FGXInt2 m_kSize;                  ///< +0x24
    unsigned int m_uiPatternType;     ///< +0x2c
};

// One heightmap texel of the cell being blended; m_ucClass 4 marks a texel no class has reached yet.
/// size 0xc
struct BlendTexel {
    float m_fHeightSum;             ///< +0x0
    float m_fWeightSum;             ///< +0x4
    unsigned char m_ucClass;        ///< +0x8
    unsigned char m_ucPieceID;      ///< +0x9
    unsigned char m_ucPieceWeight;  ///< +0xa
    unsigned char field_b;          ///< +0xb
};

/// size 0xc008
struct CellBlitThreadStorage {
    BlendTexel m_aTexels[64][64];  ///< +0x0
    unsigned char unk_c000[0x8];   ///< +0xc000
};

// A texel outside [m_kMin, m_kMax) after wrapping counts as classified, so the flood fill stays in the cell.
/// size 0x24
class TerrainGenCallback {
public:
    /// 0x0891ca60
    bool Fun_0891ca60(FGXInt2 kPos) const;
    /// 0x0891cb4c
    void Fun_0891cb4c(FGXInt2 kPos);

    FGXInt2 m_kMin;                   ///< +0x0
    FGXInt2 m_kMax;                   ///< +0x8
    FGXInt2 m_kMapSize;               ///< +0x10
    int m_iWidth;                     ///< +0x18
    unsigned int m_uiClass;           ///< +0x1c
    CellBlitThreadStorage* m_pTexels; ///< +0x20
};

/// size 0x8
class TerrainFloodFill {
public:
    /// 0x0891cad8
    void Fun_0891cad8(FGXInt2 const& kPos);
    /// 0x0891c9a6
    void Fun_0891c9a6();

    TerrainGenCallback* m_pCallback;                                                ///< +0x0
    FFastVector<FGXInt2, true, 43u, 259u, FTempHeapFastVectorAllocator>* m_pStack;  ///< +0x4
};

/// size 0x20
class TerrainLineSeedCallback {
public:
    /// 0x0891ccc6
    void Fun_0891ccc6(FGXInt2 kPos);

    FGXInt2 m_kMin;                    ///< +0x0
    FGXInt2 m_kMax;                    ///< +0x8
    FGXInt2 m_kMapSize;                ///< +0x10
    TerrainFloodFill* m_pFloodFill;    ///< +0x18
    CellBlitThreadStorage* m_pTexels;  ///< +0x1c
};

// Paged grid setters of cvTerrainSampler: pGrid is the grid's first field, pPos a texel; a uniform 16x16 page is split on write.
/// 0x087fa84a
void Fun_087fa84a(void* pGrid, unsigned short const* pValue, FGXInt2 const* pPos);
/// 0x087e75d2
void Fun_087e75d2(void* pGrid, unsigned char const* pValue, FGXInt2 const* pPos);

// One job per 64x64 texel cell of the terrain; all jobs of a launch share one BlendCell vector.
/// size 0x810
/// vtable 0x0a2fdd14
class CellBlitJob : public FJob {
public:
    /// inline
    CellBlitJob() {
        field_4 = 0;
        for (int i = 0; i < 16; ++i) {
            m_aCornerPositions[i].x = 0;
            m_aCornerPositions[i].y = 0;
        }
        m_kMapSize.x = 0;
        m_kMapSize.y = 0;
        m_kMin.x = 0;
        m_kMin.y = 0;
        m_kMax.x = 0;
        m_kMax.y = 0;
    }

    /// 0x0891bc02
    virtual void Execute(unsigned int uiThreadIndex);
    /// 0x0891bc58
    void Fun_0891bc58();
    /// 0x0891bcd2
    void Fun_0891bcd2(unsigned int uiThreadIndex);
    /// 0x0891bfd0
    void Fun_0891bfd0();
    /// 0x0891c5e8
    void Fun_0891c5e8();

    unsigned int m_uiNumBlendCells;                                                         ///< +0x10
    unsigned int m_auiBlendCells[256];                                                      ///< +0x14
    unsigned int m_uiNumSplinesA;                                                           ///< +0x414
    unsigned int m_uiNumSplinesB;                                                           ///< +0x418
    SplineRange m_aSplinesA[32];                                                            ///< +0x41c
    SplineRange m_aSplinesB[32];                                                            ///< +0x59c
    FGXVector2 const* m_pSplinePoints;                                                      ///< +0x71c
    unsigned int m_uiNumCornerPositions;                                                    ///< +0x720
    unsigned int m_auiCornerClasses[16];                                                    ///< +0x724
    FGXInt2 m_aCornerPositions[16];                                                         ///< +0x764
    CellBlitThreadStorage* m_pThreadStorage;                                                ///< +0x7e4
    FFastVector<BlendCell, true, 43u, 259u, FTempHeapFastVectorAllocator>* m_pBlendCells;   ///< +0x7e8
    float field_7ec;                                                                        ///< +0x7ec
    unsigned int m_uiNumPieceIDs;                                                           ///< +0x7f0
    FGXInt2 m_kMapSize;                                                                     ///< +0x7f4
    FGXInt2 m_kMin;                                                                         ///< +0x7fc
    FGXInt2 m_kMax;                                                                         ///< +0x804
    cvTerrainSampler* m_pTerrainSampler;                                                    ///< +0x80c

    /// 0x0a30fcd0
    static unsigned int ms_BlendCellSize;
    /// 0x0c060610
    static FFastVector<CellBlitThreadStorage, true, 43u, 259u> ms_kThreadLocalStorage;
    /// 0x0c060620
    static cvCompressedTerrainPiecesLibrary* ms_pPiecesLibrary;
    /// 0x0c06061c
    static FFastVector<FGXInt2, true, 43u, 259u, FTempHeapFastVectorAllocator>* ms_pThreadLocalFloodFillScratch;
};

extern "C" {
    /// 0x0a30ff78
    extern char const* TEXTURE_PATHS[6];
    /// 0x0a30fd0c
    extern unsigned int g_aContinentPrefix[5];
    /// 0x0a30fcd8
    extern unsigned int g_aTTPostfix[13];
    /// 0x0a30fcd4
    extern unsigned int g_mTextureMask;
    /// 0x0a30fd9c
    extern char const* g_pBaseTypeNames[0x1f];
    /// 0x0a30fe18
    extern char const* g_pContinentNames[5];
    /// 0x0a30ff90
    extern char const* g_pDynamicGeometryLibFilename[1];
    /// 0x0a30fd20
    extern unsigned int g_pHexColorCodes[31];
    /// 0x0a30fe2c
    extern char const* g_pTerrainPatternClassNames[5];
}

namespace cvProfileManager {
/// 0x0a30feb8
extern UNK_DATA ms_bEnabled[0x1];
}
