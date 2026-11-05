/// origin guess
#pragma once
#include "Civ5/Src/App/UnitFormationData.h"
#include "FirePlace/FireWorks/FRandom.h"

class FGXVector2;

/// size 0x38
class cvUnitFormationLib {
public:
    UnitFormation* m_pFormations;            ///< +0x0
    unsigned int m_uiNumFormations;          ///< +0x4
    unsigned int m_uiFormationCapacity;      ///< +0x8
    char* m_pFormationNames;                 ///< +0xc
    unsigned int m_uiNumFormationNames;      ///< +0x10
    unsigned int m_uiFormationNameCapacity;  ///< +0x14
    FRandom m_rand;                          ///< +0x18
    float m_fSpacingScale;                   ///< +0x30
    int m_iRandSeedOverride;                 ///< +0x34
    /// 0x08a28322
    cvUnitFormationLib();
    /// 0x08a28388
    unsigned int CalcFormationCrc(char const* a1) const;
    /// 0x08a283ca
    void CreateFormation(char const* pName, UnitFormation const& formation);
    /// 0x08a284fa
    UnitFormation* GetFormation(unsigned int uiNameCrc) const;
    /// 0x08a28526
    UNK_RET Startup(char const* a1);
    /// 0x08a28bd2
    UNK_RET Shutdown();
    /// 0x08a28be6
    UNK_RET Save(char const* a1) const;
    /// 0x08a290a8
    void GenerateUnitFormation(unsigned int a1, UnitFormation const& a2, unsigned int a3, FGXVector2* a4);
    /// 0x08a29688
    UNK_RET GenerateRowEntries(unsigned int uiNumEntries, UnitFormation const& formation, unsigned int uiGroupSize, unsigned int* puiRowStarts);
    /// 0x08a29772
    char* GetFormationMetaData(unsigned int uiNameCrc) const;
    /// 0x08a297a8
    UnitFormation* GetFormation(char const* a1) const;
};

/// 0x08a29890
void Fun_08a29890(void* pVector, UnitFormation const* pEntry);

/// 0x08a298e2
void Fun_08a298e2(void* pVector, char const* pName32);
