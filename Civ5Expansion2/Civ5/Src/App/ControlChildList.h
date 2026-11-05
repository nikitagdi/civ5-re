/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"

class ControlBase;

/// size ?
/// vtable 0x0a2f913c
class ControlChildList : public FStaticVector<ControlBase*, 4u, true, 19u, 7u> {
public:
    /// 0x08698ec0
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x0a3c970c
    static UNK_DATA s_Cache[0x1c];
};
