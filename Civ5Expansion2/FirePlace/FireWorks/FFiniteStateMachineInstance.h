/// origin binary
#pragma once
#include "FirePlace/FireWorks/FIFiniteStateMachineInstance.h"

class FGrannyState;
class FRandom;
class FIFiniteStateMachineInstance;
template <class T0> class FFiniteStateMachineReferenceState;

/// size ?
/// vtable 0x0a301660 FFiniteStateMachineInstance<FGrannyState>
template <class T0>
class FFiniteStateMachineInstance : public FIFiniteStateMachineInstance {
public:
    /// 0x08cb815e FFiniteStateMachineInstance<FGrannyState>
    /// deleting 0x08cb8182 FFiniteStateMachineInstance<FGrannyState>
    virtual ~FFiniteStateMachineInstance();
    /// 0x08cb8108 FFiniteStateMachineInstance<FGrannyState>
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08cb8128 FFiniteStateMachineInstance<FGrannyState>
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x087fbb6e FFiniteStateMachineInstance<FGrannyState>
    virtual UNK_RET vf_05(UNK_PARAMS);
    /// 0x087fbbea FFiniteStateMachineInstance<FGrannyState>
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x087fbc06 FFiniteStateMachineInstance<FGrannyState>
    virtual UNK_RET vf_07(UNK_PARAMS);
    /// 0x087fbc16 FFiniteStateMachineInstance<FGrannyState>
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x087fbc1e FFiniteStateMachineInstance<FGrannyState>
    virtual UNK_RET vf_09(UNK_PARAMS);
    /// 0x087fbcd2 FFiniteStateMachineInstance<FGrannyState>
    virtual UNK_RET vf_10(UNK_PARAMS);
    /// 0x08cb8196 FFiniteStateMachineInstance<FGrannyState>
    virtual UNK_RET get_context();
    /// 0x087fbdea FFiniteStateMachineInstance<FGrannyState>
    virtual UNK_RET vf_12(UNK_PARAMS);
    /// 0x087fbdf2 FFiniteStateMachineInstance<FGrannyState>
    virtual float get_state_time() const;
    /// 0x087fbdfa FFiniteStateMachineInstance<FGrannyState>
    virtual UNK_RET vf_14(UNK_PARAMS);
    /// 0x087fbe02 FFiniteStateMachineInstance<FGrannyState>
    virtual float get_roll() const;
    /// 0x087fbe0a FFiniteStateMachineInstance<FGrannyState>
    virtual FRandom& get_random();
    /// 0x087fbe12 FFiniteStateMachineInstance<FGrannyState>
    virtual void set_path_option(int a1, float a2);
    /// 0x087fbe28 FFiniteStateMachineInstance<FGrannyState>
    virtual float get_path_option(int a1) const;
    /// 0x08cb7ad8 FFiniteStateMachineInstance<FGrannyState>
    virtual UNK_RET start_log(char const* a1, unsigned int a2);
    /// 0x087fbe36 FFiniteStateMachineInstance<FGrannyState>
    virtual UNK_RET vf_20(UNK_PARAMS);
    /// 0x08cb8226 FFiniteStateMachineInstance<FGrannyState>
    virtual UNK_RET on_enter(int a1, FFiniteStateMachineReferenceState<T0>& a2, int a3);
    /// 0x08cb8228 FFiniteStateMachineInstance<FGrannyState>
    virtual UNK_RET on_leave(int a1, FFiniteStateMachineReferenceState<T0>& a2, int a3);
};
