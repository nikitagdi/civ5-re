/// origin guess
#pragma once
#include "FirePlace/FireWorks/FArray.h"
#include "FirePlace/FireWorks/FMemHooks.h"
#include "FirePlace/FireWorks/FString.h"

class FXmlElement;

/// size ?
/// vtable ?
class FIFSMStateMachine {
public:
    /// 0x08cecc90
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08ceccb0
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// absent
    virtual ~FIFSMStateMachine();
    /// pure
    virtual UNK_RET get_istate(int a1) = 0;
    /// pure
    virtual UNK_RET get_state_factory() = 0;
    /// pure
    virtual UNK_RET get_condition_factory() = 0;
    /// 0x08cecd16
    virtual UNK_RET on_load_state(int a1, FXmlElement const& a2);
    /// 0x08cecd18
    virtual UNK_RET on_load_condition(int a1, int a2, int a3, FXmlElement const& a4);
    /// 0x08cecd1a
    virtual UNK_RET on_save_state(int a1, FXmlElement& a2);
    /// 0x08cecd1c
    virtual UNK_RET on_save_condition(int a1, int a2, int a3, FXmlElement& a4);
    /// pure
    virtual UNK_RET set_resource_name(FStringA const& a1) = 0;
    /// pure
    virtual UNK_RET get_resource_name() const = 0;
    /// pure
    virtual UNK_RET get_incoming_states(int a1, FArray<int, (eMPoolType)27>& a2) const = 0;
    /// pure
    virtual UNK_RET sort_connections() = 0;
};
