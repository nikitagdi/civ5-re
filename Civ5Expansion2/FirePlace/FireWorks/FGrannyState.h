/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include <granny.h>

class FGrannyState;

/// size 0x228
/// vtable 0x0a301400
class FGrannyState {
public:
    /// size 0x10
    class ContinueInfo {
    public:
        granny_animation const* m_pAnimation;   ///< +0x0
        int m_iNextState;                       ///< +0x4
        int m_iDagNode;                         ///< +0x8
        int m_iClockId;                         ///< +0xc
    };
    /// 0x08cb65a4
    FGrannyState();
    /// 0x08cb6a50
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08cb6a70
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08cb663c
    /// deleting 0x08cb6606
    virtual ~FGrannyState();
    /// 0x08cb666c
    void SetPosition(int a1, int a2);
    /// 0x08cb6686
    int GetX() const;
    /// 0x08cb6692
    int GetY() const;
    /// 0x08cb669e
    bool operator==(FGrannyState const& a1) const;
    /// 0x08cb66cc
    void SetName(char const* a1);
    /// 0x08cb6728
    char const* GetName() const;
    /// 0x08cb6734
    void SetAsset(char const* a1);
    /// 0x08cb6790
    char const* GetAsset() const;
    /// 0x08cb679c
    void AddContinueAnimation(granny_animation const* a1, int a2, int a3, int a4);
    /// 0x08cb67ec
    FGrannyState::ContinueInfo const* FindContinueInfo(granny_animation const* a1) const;
    /// 0x08cb6816
    void CollectNextStateInfo(int a1, FStaticVector<FGrannyState::ContinueInfo const*, 32u, true, 36u, 0u>& a2);
    /// 0x08cb6876
    FGrannyState::ContinueInfo const* FindContinueInfo(int a1) const;

    FGrannyState::ContinueInfo* m_pContinue;    ///< +0x4
    unsigned int m_uiContinueCount;             ///< +0x8
    unsigned int m_uiContinueCapacity;          ///< +0xc
    char m_continueStorage[0x200];              ///< +0x10
    unsigned int field_210;                     ///< +0x210
    char const* m_pszName;                      ///< +0x214
    char const* m_pszAsset;                     ///< +0x218
    int m_iX;                                   ///< +0x21c
    int m_iY;                                   ///< +0x220
    int m_iActionCode;                          ///< +0x224
};
