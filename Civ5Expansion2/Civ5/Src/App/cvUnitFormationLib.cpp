/// origin guess
#include "Civ5/Src/App/cvUnitFormationLib.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireWorks/FCrc32.h"
#include <math.h>
#include <string.h>

/// 0x08a28322
cvUnitFormationLib::cvUnitFormationLib() {
    m_pFormations = 0;
    m_uiNumFormations = 0;
    m_uiFormationCapacity = 0;
    m_pFormationNames = 0;
    m_uiNumFormationNames = 0;
    m_uiFormationNameCapacity = 0;
    m_rand.m_uiState = 7;
    m_rand.m_uiCallCount = 0;
    m_rand.m_uiSeed = 7;
    m_rand.m_uiModulus = 0xffffcfef;
    m_rand.m_dScale = 0x1.0000301109066p-32;
    m_fSpacingScale = 1.0f;
    m_iRandSeedOverride = -1;
}

/// 0x08a28388
unsigned int cvUnitFormationLib::CalcFormationCrc(char const* a1) const {
    return g_CRC32.Calc(a1, (int)strlen(a1), 0xffffffff);
}

/// 0x08a283ca
void cvUnitFormationLib::CreateFormation(char const* pName, UnitFormation const& formation) {
    static unsigned int s_uiDefaultCrc = g_CRC32.Calc("Default", 7, 0xffffffff);
    unsigned int uiNameCrc = g_CRC32.Calc(pName, (int)strlen(pName), 0xffffffff);
    UnitFormation* pExisting = GetFormation(uiNameCrc);
    if (pExisting->m_uiNameCrc == s_uiDefaultCrc) {
        Fun_08a29890(this, &formation);
        m_pFormations[m_uiNumFormations - 1].m_uiNameCrc = uiNameCrc;
        char name[32];
        name[0] = '\0';
        strcpy_s(name, 32, pName);
        Fun_08a298e2(&m_pFormationNames, name);
    } else {
        memcpy(pExisting, &formation, sizeof(UnitFormation));
        pExisting->m_uiNameCrc = uiNameCrc;
    }
}

/// 0x08a284fa
UnitFormation* cvUnitFormationLib::GetFormation(unsigned int uiNameCrc) const {
    for (unsigned int i = 0; i < m_uiNumFormations; ++i) {
        if (m_pFormations[i].m_uiNameCrc == uiNameCrc) {
            return &m_pFormations[i];
        }
    }
    return m_pFormations;
}

/// 0x08a28bd2
UNK_RET cvUnitFormationLib::Shutdown() {
    m_uiNumFormations = 0;
    m_uiNumFormationNames = 0;
}

/// 0x08a290a8
void cvUnitFormationLib::GenerateUnitFormation(unsigned int a1, UnitFormation const& a2, unsigned int a3, FGXVector2* a4) {
    int iWidth = a2.m_iTotalWidth;
    if ((unsigned int)(iWidth - 1) >= 11) {
        iWidth = (int)(unsigned int)ceilf(sqrtf((float)a3)) + 1;
    }
    int iXBlockPeriod = a2.m_iXBlockSize ? a2.m_iXBlockSize * 2 : 0x3ffffffe;
    int iYBlockPeriod = a2.m_iYBlockSize ? a2.m_iYBlockSize : 0x1fffffff;
    int iCenterWidth = a2.m_iCenterWidth;
    int iPerRowInc = a2.m_iPerRowInc;
    int iStagger = a2.m_iStagger;
    int iXBlockSpacingCount = a2.m_iXBlockSpacingCount;
    int iYBlockSpacingCount = a2.m_iYBlockSpacingCount;
    float fYOffsetInc = a2.m_fYOffsetInc;
    int iYOffsetCurved = a2.m_iYOffsetCurved;

    if (m_iRandSeedOverride != -1) {
        m_rand.m_uiState = m_iRandSeedOverride;
        m_rand.m_uiCallCount = 0;
        m_rand.m_uiSeed = m_iRandSeedOverride;
    }

    if (a1 == 0) {
        return;
    }

    float fSpacingX = (float)a2.m_uiSpacingX * a2.m_fSpacingMultiplier * m_fSpacingScale;
    float fSpacingY = (float)a2.m_uiSpacingY * a2.m_fSpacingMultiplier * m_fSpacingScale;
    float fMinX = 0.0f;
    float fMinY = 0.0f;
    float fMaxX = 0.0f;
    float fMaxY = 0.0f;
    float fHalfCenterGap = (iCenterWidth != 0 ? 0.0f : 1.0f) * (float)iXBlockSpacingCount * fSpacingX * 0.5f;
    float fRandomness = a2.m_fUnitRandomness * 0.4f;
    float fBlockGapX = (float)iXBlockSpacingCount * fSpacingX;
    float fBlockGapY = (float)iYBlockSpacingCount * fSpacingY;
    float fSign = 1.0f;
    int iColumn = 0;
    int iRow = 0;
    int iStaggerOffset = 0;

    for (unsigned int i = 0; i < a1; ++i) {
        int iRowLength = iStaggerOffset + iWidth + iPerRowInc * 2 * iRow;
        int iFromCenter = iColumn - iCenterWidth;
        bool bPastCenter = iFromCenter >= 0;
        if (!bPastCenter) {
            iFromCenter = 0;
        }
        int iRowParity = iRowLength % 2;
        int iNextParity = (iRowLength + 1) % 2;
        int iBlock = (int)(bPastCenter && iCenterWidth != 0) + iFromCenter / iXBlockPeriod;
        float fRowOrigin = ((float)iNextParity * 0.5f * fSpacingX - fHalfCenterGap) - (float)iBlock * fBlockGapX;
        float fX = ((float)((iColumn + 2 - iRowParity) / 2) * fSpacingX - fRowOrigin) * fSign;

        float fYOffset;
        if (iYOffsetCurved == 0) {
            fYOffset = (float)((iRowParity + iColumn) / 2) * fYOffsetInc;
        } else {
            float fRowHalfWidth = (float)((iRowLength + 1 - iRowParity) / 2) * fSpacingX - fRowOrigin;
            float fCurve = cosf((fX / fRowHalfWidth) * 1.570795f);
            fYOffset = (float)((iRowParity + iRowLength) / 2) * (1.0f - fCurve) * fYOffsetInc;
            if (fYOffset < 0.0f) {
                fYOffset = -fYOffset;
            }
        }

        fX = fX + (Fun_087fbf02(m_rand) + -0.5f) * 2.0f * fSpacingX * fRandomness;
        float fY = (float)iRow * fSpacingY + (Fun_087fbf02(m_rand) + -0.5f) * 2.0f * fSpacingY * fRandomness + fYOffset;
        fY = fY + (float)(iRow / iYBlockPeriod) * fBlockGapY;
        a4[i].x = fX;
        a4[i].y = fY;

        ++iColumn;
        if (iColumn == iRowLength) {
            ++iRow;
            iStaggerOffset = (iRow % 2) * iStagger;
            iColumn = 0;
        }
        fSign = -fSign;
        if (fX <= fMinX) fMinX = fX;
        if (fY <= fMinY) fMinY = fY;
        if (fX > fMaxX) fMaxX = fX;
        if (fY > fMaxY) fMaxY = fY;
    }

    for (unsigned int i = 0; i < a1; ++i) {
        a4[i].x -= (fMaxX + fMinX) * 0.5f;
        a4[i].y -= (fMaxY + fMinY) * 0.5f;
    }
}

/// 0x08a29688
UNK_RET cvUnitFormationLib::GenerateRowEntries(unsigned int uiNumEntries, UnitFormation const& formation, unsigned int uiGroupSize, unsigned int* puiRowStarts) {
    int iWidth = formation.m_iTotalWidth;
    if ((unsigned int)(iWidth - 1) >= 11) {
        iWidth = (int)(unsigned int)ceilf(sqrtf((float)uiGroupSize)) + 1;
    }
    int iColumn = 0;
    int iRow = 0;
    int iStaggerOffset = 0;
    for (unsigned int i = 0; i < uiNumEntries; ++i) {
        ++iColumn;
        if (iColumn == formation.m_iPerRowInc * 2 * iRow + iWidth + iStaggerOffset) {
            puiRowStarts[iRow] = i;
            ++iRow;
            iStaggerOffset = (iRow % 2) * formation.m_iStagger;
            iColumn = 0;
        }
    }
}

/// 0x08a29772
char* cvUnitFormationLib::GetFormationMetaData(unsigned int uiNameCrc) const {
    for (unsigned int i = 0; i < m_uiNumFormations; ++i) {
        if (m_pFormations[i].m_uiNameCrc == uiNameCrc) {
            return m_pFormationNames + i * 0x20;
        }
    }
    return m_pFormationNames;
}

/// 0x08a297a8
UnitFormation* cvUnitFormationLib::GetFormation(char const* a1) const {
    return GetFormation(CalcFormationCrc(a1));
}
