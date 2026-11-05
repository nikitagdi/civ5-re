/// origin guess
#pragma once
#include "Civ5/Src/App/DynamicHexGrid.h"
#include "Civ5/Src/App/Graphics/CoastGrid.h"
#include "FirePlace/FireWorks/FJob.h"

// Fills one row of CoastGrid cells from the game grid flags.
/// size 0x18
/// vtable 0x0a2fdc4c
class InitGridJob : public FJob {
public:
    /// inline
    InitGridJob() {
        field_4 = 0;
        for (int i = 0; i < 8; ++i) {
            unk_8[i] = 0;
        }
    }

    /// 0x088e6b3c
    virtual void Execute(unsigned int uiThreadIndex);

    int m_iRow;    ///< +0x10
    int m_iWidth;  ///< +0x14

    /// 0x0c057f88
    static CoastGrid* ms_pCoastGrid;
    /// 0x0c057f8c
    static DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const* ms_pGameGrid;
};
