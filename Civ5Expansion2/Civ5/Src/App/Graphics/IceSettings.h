/// origin guess
#pragma once

namespace TerrainIce {
// Tuning values of assets/terrain/Ice/IceSettings.xml; every XML attribute is one field, the defaults are set by ReadXML before the file is read.
/// size 0xd0
class IceSettings {
public:
    // Returns false when the file does not load or its root element is not IceSettings.
    /// 0x0890a86c
    bool ReadXML(char const* pszFile);

    bool m_bDumpInput;                    ///< +0x0
    unsigned int m_uSplineSubdivision;    ///< +0x4
    unsigned int field_8;                 ///< +0x8
    unsigned int m_uNearSamples;          ///< +0xc
    unsigned int m_uFarSamples;           ///< +0x10
    float m_fNearSampleDist;              ///< +0x14
    float m_fFarSampleDist;               ///< +0x18
    float m_fInitialPotential;            ///< +0x1c
    float m_fCrackCost;                   ///< +0x20
    float m_fBoundaryKillRate;            ///< +0x24
    float m_fInteriorKillRate;            ///< +0x28
    float m_fCrackMinClip;                ///< +0x2c
    float m_fCrackMaxClip;                ///< +0x30
    float m_fEdgePullAmount;              ///< +0x34
    float m_fSmallPolyShrinkAmount;       ///< +0x38
    float m_fLatticeVertsPerHex;          ///< +0x3c
    float m_fSmallPolyHeightMin;          ///< +0x40
    float m_fSmallPolyHeightMax;          ///< +0x44
    float m_fLargePolyHeight;             ///< +0x48
    float m_fLargePolyEdgeHeightMin;      ///< +0x4c
    float m_fLargePolyEdgeHeightMax;      ///< +0x50
    float m_fLargePolyEdgeHeightFreq;     ///< +0x54
    float m_fLargePolyDepth;              ///< +0x58
    float m_fSmallPolyDepthMin;           ///< +0x5c
    float m_fSmallPolyDepthMax;           ///< +0x60
    float m_fSkirtSlopeSmall;             ///< +0x64
    float m_fSkirtSlopeLarge;             ///< +0x68
    float field_6c;                       ///< +0x6c
    float m_fMaxHeight;                   ///< +0x70
    float m_fMaxDepth;                    ///< +0x74
    float m_fDisplaceFrequency;           ///< +0x78
    float m_fDisplaceMin;                 ///< +0x7c
    float m_fDisplaceMax;                 ///< +0x80
    float m_fDisplaceExponent;            ///< +0x84
    float m_fTextureRepeat;               ///< +0x88
    float m_fBumpRepeat;                  ///< +0x8c
    float m_fSpecularPower;               ///< +0x90
    float m_fSideTextureRepeatU;          ///< +0x94
    float m_fSideTextureRepeatV;          ///< +0x98
    float m_fSideBumpRepeatU;             ///< +0x9c
    float m_fSideBumpRepeatV;             ///< +0xa0
    float m_afSubsurface[3];              ///< +0xa4
    float m_afUnderwater1[3];             ///< +0xb0
    float m_afUnderwater2[3];             ///< +0xbc
    float m_fUnderwaterColorRamp;         ///< +0xc8
    float m_fUnderwaterAlphaRamp;         ///< +0xcc
};
}
