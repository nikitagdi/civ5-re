/// origin binary
#pragma once

/// size 0x18
class FRandom {
public:
    unsigned int m_uiState;      ///< +0x0
    unsigned int m_uiCallCount;  ///< +0x4
    unsigned int m_uiSeed;       ///< +0x8
    unsigned int m_uiModulus;    ///< +0xc
    double m_dScale;             ///< +0x10
};

/// 0x087fbf02
float Fun_087fbf02(FRandom& rng);
