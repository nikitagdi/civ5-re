/// origin binary
#pragma once
#include "FirePlace/FireWorks/FArray.h"
#include "FirePlace/FireWorks/FIFSMStateMachine.h"
#include "FirePlace/FireWorks/FIFiniteStateMachine.h"
#include "FirePlace/FireWorks/FMemHooks.h"
#include "FirePlace/FireWorks/FString.h"

class FClassId;
class FGrannyState;
class FIFSMStateMachine;
class FIFiniteStateMachine;
class FXmlElement;

/// size ?
/// vtable 0x0a302910 FFiniteStateMachineReference<FGrannyState>
template <class T0>
class FFiniteStateMachineReference : public FIFiniteStateMachine, public FIFSMStateMachine {
public:
    /// 0x08cecb48 FFiniteStateMachineReference<FGrannyState>
    /// deleting 0x08cecc60 FFiniteStateMachineReference<FGrannyState>
    virtual ~FFiniteStateMachineReference();
    /// 0x08cea736 FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08cecc0a FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08cecc2a FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08cea778 FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET vf_05(UNK_PARAMS);
    /// 0x08cea844 FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x08cea8fe FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET vf_07(UNK_PARAMS);
    /// 0x08cea93e FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x08cea970 FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET vf_09(UNK_PARAMS);
    /// 0x08cea97e FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET vf_10(UNK_PARAMS);
    /// 0x08cea9da FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET vf_11(UNK_PARAMS);
    /// 0x08ceaa14 FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET vf_12(UNK_PARAMS);
    /// 0x08ceaa1c FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET vf_13(UNK_PARAMS);
    /// 0x08ceaa7a FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET vf_14(UNK_PARAMS);
    /// 0x08ceaaa6 FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET vf_18(UNK_PARAMS);
    /// 0x08ceabda FFiniteStateMachineReference<FGrannyState>
    /// thunk 0x08ceadba FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET get_istate(int a1);
    /// 0x08ceabe0 FFiniteStateMachineReference<FGrannyState>
    /// thunk 0x08ceade6 FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET get_state_factory();
    /// 0x08ceabf4 FFiniteStateMachineReference<FGrannyState>
    /// thunk 0x08ceadfa FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET get_condition_factory();
    /// 0x08cecc88 FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET on_load_state(int a1, FXmlElement const& a2);
    /// 0x08cecc8a FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET on_load_condition(int a1, int a2, int a3, FXmlElement const& a4);
    /// 0x08cecc8c FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET on_save_state(int a1, FXmlElement& a2);
    /// 0x08cecc8e FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET on_save_condition(int a1, int a2, int a3, FXmlElement& a4);
    /// 0x08ceac08 FFiniteStateMachineReference<FGrannyState>
    /// thunk 0x08ceae0e FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET set_resource_name(FStringA const& a1);
    /// 0x08ceac46 FFiniteStateMachineReference<FGrannyState>
    /// thunk 0x08ceae54 FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET get_incoming_states(int a1, FArray<int, (eMPoolType)27>& a2) const;
    /// 0x08ceac4e FFiniteStateMachineReference<FGrannyState>
    /// thunk 0x08ceae88 FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET sort_connections();
    /// 0x08ceaca6 FFiniteStateMachineReference<FGrannyState>
    /// thunk 0x08ceae4c FFiniteStateMachineReference<FGrannyState>
    virtual UNK_RET get_resource_name() const;
    static UNK_DATA s_conditonFactory[0xc];
    static UNK_DATA s_stateFactory[0xc];
};
/// 0x0a33e490 FFiniteStateMachineReference<FGrannyState>
template <class T0>
UNK_DATA FFiniteStateMachineReference<T0>::s_conditonFactory[0xc];
/// 0x0a33e47c FFiniteStateMachineReference<FGrannyState>
template <class T0>
UNK_DATA FFiniteStateMachineReference<T0>::s_stateFactory[0xc];
