/// origin guess
#include "FirePlace/FireWorks/FRandom.h"

/// 0x087fbf02
float Fun_087fbf02(FRandom& rng) {
    rng.m_uiCallCount++;
    rng.m_uiState = rng.m_uiState * 0x343fd + 0x269ec3;
    if (rng.m_uiModulus != 0) {
        rng.m_uiState = rng.m_uiState % rng.m_uiModulus;
    }
    return (float)((double)rng.m_uiState * rng.m_dScale);
}
