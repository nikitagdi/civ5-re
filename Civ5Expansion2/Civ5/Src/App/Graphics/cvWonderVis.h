/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/cvWonderLibrary.h"
#include <d3d9.h>

class FGXGrannyCompositeContainerInstance;
class FGXGrannyCompositeContainerTemplate;
class cvCityVis_AddWonderData;
class cvCity_FogState;
namespace FireGrafix {
class FGXModelTemplate;
}

// One wonder placed in a city; linked into the per-city list through m_uNext (0xfffffff ends it).
/// size 0xd0
class cvWonderVis {
public:
    unsigned int m_uNext;                                       ///< +0x0
    unsigned int field_4;                                       ///< +0x4
    // Multiplies the renderer time when the animation is advanced.
    float m_fAnimSpeed;                                         ///< +0x8
    // Rotation about Z that WonderRenderJob adds to the renderer's.
    float m_fRenderRotation;                                    ///< +0xc
    // Time of the last SetAnimationState.
    float m_fAnimTime;                                          ///< +0x10
    unsigned int m_uWonderType;                                 ///< +0x14
    // Model node state bits for SetMeshState and SetAnimationState (bit 7 selects the animation set).
    unsigned int m_uState;                                      ///< +0x18
    // Rotation about Z in radians.
    float m_fRotation;                                          ///< +0x1c
    float field_20;                                             ///< +0x20
    // Continent variant, 0 to 4; selects the surface set of the library entry.
    unsigned int m_uContinent;                                  ///< +0x24
    float m_fPosX;                                              ///< +0x28
    float m_fPosY;                                              ///< +0x2c
    float m_fPosZ;                                              ///< +0x30
    cvWonderLibrary::cvWonderLibraryEntry const* m_pEntry;      ///< +0x34
    FGXGrannyCompositeContainerInstance* m_pInstance;           ///< +0x38
    FireGrafix::FGXModelTemplate* m_pTemplate;                  ///< +0x3c
    unsigned char m_kStateSorter[0x2c];                         ///< +0x40
    unsigned int m_auDecals[8];                                 ///< +0x6c
    unsigned int m_uDecalCount;                                 ///< +0x8c
    D3DXMATRIX m_mWorld;                                        ///< +0x90
    /// 0x08866314
    cvWonderVis();
    /// 0x08866444
    ~cvWonderVis();
    /// 0x08866446
    UNK_RET Init(cvCityVis_AddWonderData const& a1, cvCity_FogState const& a2, FGXGrannyCompositeContainerTemplate* a3, unsigned int a4, unsigned int a5, cvWonderLibrary::cvWonderLibraryEntry const* a6, D3DXMATRIX const& a7, float a8);
    /// 0x08866788
    UNK_RET SetMeshState(unsigned char a1);
    /// 0x088667ec
    UNK_RET SetAnimationState(unsigned char a1, float a2, float a3);
    /// 0x088668fe
    UNK_RET Release();
    /// 0x0886697e
    UNK_RET ClearDecals();
    /// 0x08866aa0
    float const* GetPosition() const;
    /// 0x08866ace
    UNK_RET SortNodes();
    /// 0x08866b0e
    UNK_RET InitDecals();
};
