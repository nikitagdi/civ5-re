/// origin guess
#pragma once
#include "FirePlace/FireWorks/FJob.h"

class TerrainCell;
namespace FireGrafix {
template <int T0> class VStruct;
}

/// size 0x28
/// vtable 0x0a2fdcfc
class TerrainCellJob : public FJob {
public:
    /// 0x08911d16
    virtual void Execute(unsigned int a1);
    /// 0x08911cde
    UNK_RET Init(TerrainCell* a1, FireGrafix::VStruct<16>* a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6);

    TerrainCell* m_pCell;                    ///< +0x10
    FireGrafix::VStruct<16>* m_pVertices;    ///< +0x14
    unsigned int field_18;                   ///< +0x18
    unsigned int field_1c;                   ///< +0x1c
    unsigned int field_20;                   ///< +0x20
    unsigned int field_24;                   ///< +0x24
};
