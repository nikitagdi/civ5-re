/// origin guess
#pragma once

/// size 0x44
class UnitFormation {
public:
    unsigned int m_uiSpacingX;       ///< +0x0
    unsigned int m_uiSpacingY;       ///< +0x4
    float m_fSpacingMultiplier;      ///< +0x8
    float m_fUnitRandomness;         ///< +0xc
    float m_fBlockRandomness;        ///< +0x10
    int m_iPerRowInc;                ///< +0x14
    float m_fYOffsetInc;             ///< +0x18
    int m_iYOffsetCurved;            ///< +0x1c
    int m_iStagger;                  ///< +0x20
    int m_iTotalWidth;               ///< +0x24
    int m_iXBlockSize;               ///< +0x28
    int m_iXBlockSpacingCount;       ///< +0x2c
    int m_iYBlockSize;               ///< +0x30
    int m_iYBlockSpacingCount;       ///< +0x34
    int m_iCenterWidth;              ///< +0x38
    unsigned int m_uiNameCrc;        ///< +0x3c
    char unk_40[0x4];                ///< +0x40
};

/// size ?
class UnitFormationData {
public:
    /// 0x08a282b4
    UnitFormationData();
};
