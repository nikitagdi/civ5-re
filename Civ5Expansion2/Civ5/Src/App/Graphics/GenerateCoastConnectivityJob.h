/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/CoastGrid.h"
#include "FirePlace/FireWorks/FJob.h"

// Links each coast edge of one row of CoastGrid cells to the next and previous edge along the coastline.
/// size 0x18
/// vtable 0x0a2fdc1c
class GenerateCoastConnectivityJob : public FJob {
public:
    /// inline
    GenerateCoastConnectivityJob() {
        field_4 = 0;
        for (int i = 0; i < 8; ++i) {
            unk_8[i] = 0;
        }
    }

    /// 0x088e6038
    virtual void Execute(unsigned int uiThreadIndex);

    int m_iRow;    ///< +0x10
    int m_iWidth;  ///< +0x14

    /// 0x0c057f98
    static CoastGrid* ms_pCoastGrid;
};
