/// origin guess
#include "FirePlace/FireWorks/FGrannyState.h"

/// 0x08cb65a4
FGrannyState::FGrannyState() {
    m_pContinue = reinterpret_cast<ContinueInfo*>(m_continueStorage);
    m_uiContinueCount = 0;
    m_uiContinueCapacity = 32;
    m_pszName = nullptr;
    m_pszAsset = nullptr;
    m_iX = 0;
    m_iY = 0;
    m_iActionCode = 0;
}

/// 0x08cb666c
void FGrannyState::SetPosition(int a1, int a2) {
    m_iX = a1;
    m_iY = a2;
}

/// 0x08cb6686
int FGrannyState::GetX() const {
    return m_iX;
}

/// 0x08cb6692
int FGrannyState::GetY() const {
    return m_iY;
}

/// 0x08cb669e
bool FGrannyState::operator==(FGrannyState const& a1) const {
    return m_iX == a1.m_iX && m_iY == a1.m_iY;
}

/// 0x08cb6728
char const* FGrannyState::GetName() const {
    return m_pszName;
}

/// 0x08cb6790
char const* FGrannyState::GetAsset() const {
    return m_pszAsset;
}

/// 0x08cb67ec
FGrannyState::ContinueInfo const* FGrannyState::FindContinueInfo(granny_animation const* a1) const {
    for (unsigned int i = 0; i < m_uiContinueCount; ++i) {
        if (m_pContinue[i].m_pAnimation == a1) {
            return &m_pContinue[i];
        }
    }
    return nullptr;
}

/// 0x08cb6876
FGrannyState::ContinueInfo const* FGrannyState::FindContinueInfo(int a1) const {
    for (unsigned int i = 0; i < m_uiContinueCount; ++i) {
        if (m_pContinue[i].m_iNextState == a1) {
            return &m_pContinue[i];
        }
    }
    return nullptr;
}
