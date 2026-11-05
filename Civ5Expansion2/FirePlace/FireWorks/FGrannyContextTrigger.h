/// origin guess
#pragma once
#include "FirePlace/FireWorks/FContextTrigger.h"
#include <granny.h>

class FClassId;
class FGrannyAssetInstance;
class FGrannyModelInstanceData;
class FITimedTriggerSystemInstance;

/// size ?
/// vtable 0x0a302510
class FGrannyContextTrigger : public FContextTrigger {
public:
    /// 0x08ce2a84
    FGrannyContextTrigger(FITimedTriggerSystemInstance& a1, float a2, FGrannyAssetInstance* a3, FGrannyModelInstanceData const& a4, granny_animation const* a5);
    /// 0x08ce2b74
    /// deleting 0x08ce2af4
    /// thunk 0x08ce2b1c
    /// thunk 0x08ce2b48
    /// thunk 0x08ce2b88
    /// thunk 0x08ce2bac
    virtual ~FGrannyContextTrigger();
    /// 0x08ce2bd0
    /// thunk 0x08ce2c18
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08ce2c5c
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08ce2c7c
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// 0x08ce2c44
    UNK_RET get_asset_instance() const;
    /// 0x08ce2c4c
    UNK_RET get_extra_data() const;
    /// 0x08ce2c54
    UNK_RET get_animation() const;
};
