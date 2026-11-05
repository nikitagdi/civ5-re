/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/cvUnitVisSystem.h"
#include "FirePlace/FireGrafix/FGXVector2.h"

class cvUnitSim;
template <class T0> class cvUnitVisSystemAccessor;

/// size 0x2c
class cvUnitSimFigure {
public:
    char unk_0[0x24];             ///< +0x0
    unsigned int m_uiFigureTag;   ///< +0x24
    unsigned int m_uiHealthPct;   ///< +0x28
};

/// size 0x344
class cvUnitSim {
public:
    char unk_0[0x4];                ///< +0x0
    int m_iLiveTag;                 ///< +0x4
    cvUnitSimFigure* m_pFigures;    ///< +0x8
    int m_iNumFigures;              ///< +0xc
    char unk_10[0x2c8];             ///< +0x10
    char unk_2d8[0x8];              ///< +0x2d8
    FGXVector2 m_vPos;              ///< +0x2e0
    float m_fHeight;                ///< +0x2e8
    float m_fHeading;               ///< +0x2ec
    char unk_2f0[0x10];             ///< +0x2f0
    unsigned int m_uiDamageStep;    ///< +0x300
    char unk_304[0x4];              ///< +0x304
    unsigned int m_uiFormationCrc;  ///< +0x308
    unsigned int m_uiFormationSeed; ///< +0x30c
    unsigned int m_uiFigureTag;     ///< +0x310
    float m_fActionEventTime;       ///< +0x314
    char unk_318[0x2c];             ///< +0x318

    /// 0x08a2abec
    cvUnitSim(cvUnitVisSystemAccessor<cvUnitVisSystem> const& a1);
    /// 0x08a2ad08
    cvUnitSim(cvUnitSim const& a1);
    /// 0x08a2ae5e
    ~cvUnitSim();
    /// 0x08a2af10
    UNK_RET SetActionEventID(unsigned int a1);
    /// 0x08a2af74
    UNK_RET UpdateActionEventTime(float a1);
    /// 0x08a2afae
    UNK_RET DestroyData();
    /// 0x0a30ffbc
    static UNK_DATA ms_eActionXRef[0x38];
};

extern "C" {
    /// 0x0a3113d0
    extern UNK_DATA IdentifierCharacters[0x100];
    /// 0x0a310524
    extern UNK_DATA command_list[0x140];
    /// 0x0a3114d4
    extern UNK_DATA szModsDatabaseSchemaSQL[0x4];
    /// 0x0a3114d0
    extern UNK_DATA szModsDatabaseSchemaVersion[0x4];
}
