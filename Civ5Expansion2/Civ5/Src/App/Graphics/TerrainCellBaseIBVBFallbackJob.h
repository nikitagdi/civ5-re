/// origin guess
#pragma once
#include "FirePlace/FireWorks/FJob.h"

class TerrainCell;

/// size 0x14
/// vtable 0x0a2fdcc8
class TerrainCellBaseIBVBFallbackJob : public FJob {
public:
    /// 0x089126c8
    virtual void Execute(unsigned int a1);
    /// 0x089126bc
    UNK_RET Init(TerrainCell* a1);

    TerrainCell* m_pCell; ///< +0x10
};

/// 0x089138df
UNK_RET FillIndexLattice(unsigned short* a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int* a5);
