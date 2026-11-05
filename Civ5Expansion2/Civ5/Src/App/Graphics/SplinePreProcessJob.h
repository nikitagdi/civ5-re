/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/CoastGrid.h"
#include "FirePlace/FireWorks/FJob.h"

// One pass of the coast smoothing state machine over one row of CoastGrid cells; the pass is ms_uCurrState.
/// size 0x2c
/// vtable 0x0a2fdc04
class SplinePreProcessJob : public FJob {
public:
    /// inline
    SplinePreProcessJob() {
        field_4 = 0;
        for (int i = 0; i < 8; ++i) {
            unk_8[i] = 0;
        }
        for (int i = 0; i < 4; ++i) {
            m_afSmoothed[i] = 0.0f;
        }
        m_iRow = 0;
    }

    /// 0x088e578e
    virtual void Execute(unsigned int uiThreadIndex);

    float m_afSmoothed[4];          ///< +0x10
    int m_iRow;                     ///< +0x20
    int m_iWidth;                   ///< +0x24
    CoastEdgeSegment* m_pScratch;   ///< +0x28

    // Pass kinds, in the order the passes run: smooth, commit, shift mountain edges, commit, ...
    /// 0x0a30fca0
    static int ms_aStateList[10];
    /// 0x0a30fcc8
    static int ms_nStateList;
    /// 0x0c057fa0
    static CoastGrid* ms_pCoastGrid;
    /// 0x0c057f9c
    static unsigned int ms_uCurrState;
};
