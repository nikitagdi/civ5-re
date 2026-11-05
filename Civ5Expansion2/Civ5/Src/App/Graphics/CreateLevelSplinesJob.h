/// origin guess
#pragma once
#include "FirePlace/FireWorks/FJob.h"

class CoastlineSpline;
class FinalLevelSplines;
class cvCoastMeshEngine;

/// size 0x1c
/// vtable 0x0a2fdcac
class CreateLevelSplinesJob : public FJob {
public:
    /// 0x088ec0bc
    virtual void Execute(unsigned int a1);

    CoastlineSpline const* m_pSpline;   ///< +0x10
    FinalLevelSplines* m_pLevels;       ///< +0x14
    cvCoastMeshEngine const* m_pEngine; ///< +0x18
};
