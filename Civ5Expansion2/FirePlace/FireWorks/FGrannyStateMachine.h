/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFiniteStateMachineReference.h"
#include <granny.h>

class FGrannyAsset;
class FGrannyState;
class FXmlElement;

/// size 0x40
/// vtable 0x0a302770
class FGrannyStateMachine : public FFiniteStateMachineReference<FGrannyState> {
public:
    /// 0x08ce9284
    FGrannyStateMachine();
    /// 0x08ce9ba4
    /// deleting 0x08ce9b50
    /// thunk 0x08ce9b78
    /// thunk 0x08ce9bf4
    virtual ~FGrannyStateMachine();
    /// 0x08cea20e
    virtual UNK_RET get_state_name_size();
    /// 0x08cea204
    virtual UNK_RET get_state_names();
    /// 0x08ce9f92
    /// thunk 0x08ce9f94
    virtual UNK_RET on_load_condition(int a1, int a2, int a3, FXmlElement const& a4);
    /// 0x08ce9c18
    /// thunk 0x08ce9f5e
    virtual UNK_RET on_load_state(int a1, FXmlElement const& a2);
    /// 0x08cea0e8
    /// thunk 0x08cea0ea
    virtual UNK_RET on_save_condition(int a1, int a2, int a3, FXmlElement& a4);
    /// 0x08ce9f96
    /// thunk 0x08cea0b4
    virtual UNK_RET on_save_state(int a1, FXmlElement& a2);
    /// 0x08cea184
    virtual UNK_RET populate_names();
    /// 0x08cea6e0
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08cea700
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08cea0ec
    virtual int get_state_id_by_name(char const* a1);
    /// 0x08cea216
    bool HasAsset(int a1, char const* a2) const;
    /// 0x08cea264
    void CreateStateIDMapping(FGrannyAsset const* a1);
    /// 0x08cea386
    granny_dag2* GetDAG2Source(int a1) const;
    /// 0x08cea3d8
    float GetMinimumDuration(int a1) const;
    /// 0x08cea436
    unsigned int GetDAG2Count() const;
    /// 0x08cea43e
    granny_dag2* GetDAG2Indexed(unsigned int a1) const;

    char unk_8[0x20];                   ///< +0x8
    char const** m_ppszStateNames;      ///< +0x28
    unsigned int m_uiNumStateNames;     ///< +0x2c
    unsigned int m_uiStateNamesCapacity;///< +0x30
    unsigned char m_stateDagMap[0xc];   ///< +0x34
};
