/// origin binary
#pragma once
#include "FirePlace/FireWorks/FContext.h"
#include "FirePlace/FireWorks/FIContextTrigger.h"

class FClassId;
class FITimedTriggerSystemInstance;

/// size ?
/// vtable 0x0a3067c0
class FContextTrigger : public FContext, public FIContextTrigger {
public:
    /// 0x08dca490
    FContextTrigger(FITimedTriggerSystemInstance& a1, float a2, int a3);
    /// 0x08dca538
    /// deleting 0x08dca4dc
    /// thunk 0x08dca4f0
    /// thunk 0x08dca514
    /// thunk 0x08dca554
    /// thunk 0x08dca572
    virtual ~FContextTrigger();
    /// 0x08dca712
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08dca732
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// 0x08dca61e
    /// thunk 0x08dca626
    virtual UNK_RET loop_count() const;
    /// 0x08dca62e
    /// thunk 0x08dca636
    virtual UNK_RET fired_time() const;
    /// 0x08dca63e
    /// thunk 0x08dca646
    virtual UNK_RET get_instance() const;
    /// 0x08dca590
    /// thunk 0x08dca5f2
    virtual UNK_RET QueryInterface(FClassId const& a1);
};
