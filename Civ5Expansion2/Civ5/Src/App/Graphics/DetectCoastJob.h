/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/CoastGrid.h"
#include "FirePlace/FireWorks/FJob.h"

// Lists the coast edges of one row of CoastGrid cells from the terrain classes of the cell and its neighbours.
/// size 0x18
/// vtable 0x0a2fdc34
class DetectCoastJob : public FJob {
public:
    /// inline
    DetectCoastJob() {
        field_4 = 0;
        for (int i = 0; i < 8; ++i) {
            unk_8[i] = 0;
        }
    }

    /// 0x088e6484
    virtual void Execute(unsigned int uiThreadIndex);

    int m_iRow;    ///< +0x10
    int m_iWidth;  ///< +0x14

    /// 0x0c057f90
    static CoastEdgeSegment* ms_pCoastData;
    /// 0x0c057f94
    static CoastGrid* ms_pCoastGrid;
};
