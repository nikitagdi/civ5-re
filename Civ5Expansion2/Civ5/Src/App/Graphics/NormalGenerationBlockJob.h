/// origin guess
#pragma once
#include "FirePlace/FireWorks/FJob.h"
#include "FirePlace/FireGrafix/FGXVector3.h"

class cvTerrainSampler;

// One 64x64 block of the normal field; x and y count blocks.
/// size 0x20
/// vtable 0x0a2f900c
class NormalGenerationBlockJob : public FJob {
public:
    /// 0x0867a750
    virtual void Execute(unsigned int a1);
    // One 65x65 FGXVector3 scratch grid per job thread, allocated by cvTerrainSampler::GenerateNormalField.
    /// 0x0a3524f0
    static FGXVector3* pThreadLocalStorage;

    int m_iBlockX;                ///< +0x10
    int m_iBlockY;                ///< +0x14
    unsigned short* m_pNormals;   ///< +0x18
    cvTerrainSampler* m_pSampler; ///< +0x1c
};
