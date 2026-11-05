/// origin guess
#pragma once

class cvLandmarkVis_Init;

/// size ?
class cvLandmarkVis {
public:
    /// size 0x44
    struct ElementTransform {
        float m_afMatrix[16];         ///< +0x0
        unsigned int m_uiElementDef;  ///< +0x40
    };
    /// 0x08a2c710
    cvLandmarkVis(cvLandmarkVis_Init const& a1);
    /// 0x08a2c7ba
    ~cvLandmarkVis();
    /// 0x08a2c7bc
    bool Init(unsigned int uiSurfaceCapacity);
    /// 0x08a2c83e
    UNK_RET Release();
    /// 0x08a2c8f8
    UNK_RET GetContainerTemplate() const;
    /// 0x08a2c908
    UNK_RET GetContainerInstance() const;
    /// 0x08a2c91c
    UNK_RET GetDecalHandles() const;
    /// 0x08a2c932
    void SetLandmarkState(unsigned char ucState, float fTime, float a3);
    /// 0x08a2cb86
    void SetFOWState(unsigned char ucFOWState);
    char unk_0[0x8];                  ///< +0x0
    float m_fRandomPhase;             ///< +0x8
    float m_fCreationTime;            ///< +0xc
    unsigned int field_10;            ///< +0x10
    unsigned int m_uiSortedTemplate;  ///< +0x14
    void* m_pContainerTemplate;       ///< +0x18
    unsigned int m_uiSurfaceCapacity; ///< +0x1c
    unsigned int m_uiType;            ///< +0x20
    unsigned int m_uiVariantIndex;    ///< +0x24
    // bits 0-1: FOW state; bits 2-5: landmark state; 0x40 not written by either setter
    unsigned char m_ucFlags;          ///< +0x28
    char unk_29[0x3];                 ///< +0x29
    unsigned int m_uiVariant;         ///< +0x2c
    ElementTransform m_aElements[16]; ///< +0x30
    float field_470;                  ///< +0x470
    float field_474;                  ///< +0x474
    unsigned int m_uiElementCount;    ///< +0x478
};

// Argument of cvLandmarkVisSystem::InitHandle; the constructor copies +0x0..+0x44f to cvLandmarkVis +0x20 and the rest to +0x470.
/// size 0x45c
class cvLandmarkVis_Init {
public:
    unsigned int m_uiType;                            ///< +0x0
    unsigned int m_uiVariantIndex;                    ///< +0x4
    unsigned char m_ucFlags;                          ///< +0x8
    char unk_9[0x3];                                  ///< +0x9
    unsigned int m_uiVariant;                         ///< +0xc
    cvLandmarkVis::ElementTransform m_aElements[16];  ///< +0x10
    float field_450;                                  ///< +0x450
    float field_454;                                  ///< +0x454
    unsigned int m_uiElementCount;                    ///< +0x458
};
