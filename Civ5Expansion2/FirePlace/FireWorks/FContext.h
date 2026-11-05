/// origin binary
#pragma once
#include "FirePlace/FireWorks/FIUnknown.h"
#include "FirePlace/FireWorks/FReferenceObject.h"

/// size ?
/// vtable 0x0a3016d0
class FContext : public FReferenceObject, public FIUnknown {
public:
    /// 0x08cb822a
    /// deleting 0x08cb8812
    virtual ~FContext();
    /// 0x08cb8826
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08cb8846
    virtual UNK_RET vf_03(UNK_PARAMS);
};
