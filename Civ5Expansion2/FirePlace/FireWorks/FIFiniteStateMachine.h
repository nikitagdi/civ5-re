/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIUnknown.h"

/// size ?
/// vtable ?
class FIFiniteStateMachine : public FIUnknown {
public:
    /// pure
    virtual UNK_RET vf_05(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_06(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_07(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_08(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_09(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_10(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_11(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_12(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_13(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_14(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET populate_names() = 0;
    /// pure
    virtual UNK_RET get_state_names() = 0;
    /// pure
    virtual UNK_RET get_state_name_size() = 0;
};
