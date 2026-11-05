/// origin guess
#pragma once
#include "FirePlace/FireWorks/FJob.h"

class TerrainCell;

/// size 0x14
/// vtable 0x0a2fdce4
class TerrainCellStatJob : public FJob {
public:
    /// 0x089159ae
    virtual void Execute(unsigned int a1);
    /// 0x089159a2
    UNK_RET Init(TerrainCell* a1);

    TerrainCell* m_pCell; ///< +0x10
};

/// 0x089172d2
UNK_RET IsSmallest(float* a1, int a2, int a3);
/// 0x08917883
unsigned int CreatePatchIndexLattice(unsigned int a1, unsigned int a2);
/// 0x08917a20
unsigned int CreateIndexLattice(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int* a4);
/// 0x08917ad8
unsigned int CreateBaseTerrainCellVB(unsigned int uWidth, unsigned int uHeight, unsigned int uUvBorder);
