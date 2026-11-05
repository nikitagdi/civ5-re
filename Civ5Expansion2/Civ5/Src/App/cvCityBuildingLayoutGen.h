/// origin guess
#pragma once
#include "Civ5/Src/App/HEXVec_i.h"

class FGXVector2;

namespace FTensor {
/// size 0x2
class TensorC {
public:
    signed char m_cX;                 ///< +0x0
    signed char m_cY;                 ///< +0x1
};
}
namespace FGXMathRect {
/// size 0x10
class Rect {
public:
    float m_fMinX;                    ///< +0x0
    float m_fMinY;                    ///< +0x4
    float m_fMaxX;                    ///< +0x8
    float m_fMaxY;                    ///< +0xc
};
}
/// absent
FTensor::TensorC InterpTensorC(FTensor::TensorC const* a1, FGXVector2 const& a2);

/// size 0x20
class FieldFrame {
public:
    int m_nWidth;                     ///< +0x0
    int m_nHeight;                    ///< +0x4
    float m_fMinX;                    ///< +0x8
    float m_fMinY;                    ///< +0xc
    float m_fMaxX;                    ///< +0x10
    float m_fMaxY;                    ///< +0x14
    float m_fScaleX;                  ///< +0x18
    float m_fScaleY;                  ///< +0x1c
};

/// size ?
template <class T0, unsigned int T1, unsigned int T2>
class StaticLinearField {
public:
    T0 m_aCells[T2][T1];              ///< +0x0
};

/// size ?
template <class T0, class T1, T0 (*T2)(T0 const*, FGXVector2 const&)>
class EmbeddedDataField {
public:
    T1 m_Data;                        ///< +0x0
    // FieldFrame follows m_Data at sizeof(T1)
};

typedef EmbeddedDataField<FTensor::TensorC, StaticLinearField<FTensor::TensorC, 64u, 64u>, &InterpTensorC> TensorFieldType;
/// size 0x24
class PlacementParticle {
public:
    unsigned char m_bFree;            ///< +0x0
    unsigned int m_nNextFree;         ///< +0x4
    float m_fX;                       ///< +0x8
    float m_fY;                       ///< +0xc
    float m_fTensorX;                 ///< +0x10
    float m_fTensorY;                 ///< +0x14
    float m_fForceX;                  ///< +0x18
    float m_fForceY;                  ///< +0x1c
    unsigned short m_nHashNext;       ///< +0x20
    unsigned char unk_22[0x2];        ///< +0x22
};

/// size 0x14
class ParticlePool {
public:
    // free-list head; 0x0fffffff when the list is empty
    unsigned int m_nFreeHead;         ///< +0x0
    unsigned int m_nLiveCount;        ///< +0x4
    PlacementParticle* m_pParticles;  ///< +0x8
    unsigned int m_nParticleCount;    ///< +0xc
    unsigned int m_nParticleCap;      ///< +0x10
};

/// size 0x10
class ForceProfile {
public:
    float m_fRadius;                  ///< +0x0
    float m_fRange;                   ///< +0x4
    float m_fStrengthA;               ///< +0x8
    float m_fStrengthB;               ///< +0xc
};

/// size 0x4898
class cvCityBuildingLayoutGen {
public:
    float m_fMinX;                    ///< +0x0
    float m_fMinY;                    ///< +0x4
    float m_fMaxX;                    ///< +0x8
    float m_fMaxY;                    ///< +0xc
    float m_afFrameMatrix[4];         ///< +0x10
    float m_fWrapWidth;               ///< +0x20
    unsigned char unk_24[0x4];        ///< +0x24
    float m_fWrapMin;                 ///< +0x28
    float m_fWrapMax;                 ///< +0x2c
    TensorFieldType const* m_pTensorField; ///< +0x30
    unsigned short m_anHashHeads[32][32]; ///< +0x34
    unsigned char m_abBlockedMask[128 * 128]; ///< +0x834
    ParticlePool m_Particles;         ///< +0x4834
    float m_fSpacing;                 ///< +0x4848
    float m_fMinNeighbourDistance;    ///< +0x484c
    int m_nHashRadius;                ///< +0x4850
    int m_nMaskRadius;                ///< +0x4854
    ForceProfile m_aForceProfiles[4]; ///< +0x4858

    /// 0x08a034d8
    void InitializePopBuildingPlacementSet(FGXMathRect::Rect const& rect, TensorFieldType const& tensorField);
    /// 0x08a036f0
    UNK_RET SolveForFinalPlacements();
    /// 0x08a0374c
    void Tick(unsigned char nFlags);
    /// 0x08a040c6
    UNK_RET FinalizePlacementSet();
};
