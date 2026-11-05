/// origin guess
#pragma once
#include "FirePlace/FireWorks/FTimedTriggerSystemInstance.h"
#include <granny.h>

class FClassId;
class FGrannyAssetInstance;
class FGrannyModelInstanceData;
class FITimedTrigger;
class FITimedTriggerSystem;

/// size ?
/// vtable 0x0a302640
class FGrannyTimedTriggerSystemInstance : public FTimedTriggerSystemInstance {
public:
    /// 0x08ce5d50
    FGrannyTimedTriggerSystemInstance(FITimedTriggerSystem& a1, int a2);
    /// 0x08ce5e16
    /// deleting 0x08ce5dee
    virtual ~FGrannyTimedTriggerSystemInstance();
    /// 0x08ce5e2c
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08ce5e2a
    virtual UNK_RET process(float a1, float a2);
    /// 0x08ce5fd8
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08ce5ff8
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08ce5e94
    virtual UNK_RET FireTrigger(FITimedTrigger* a1, FGrannyAssetInstance* a2, FGrannyModelInstanceData const& a3, granny_animation const* a4, granny_sample_context* a5);
    /// 0x08ce5f10
    UNK_RET OnControlledAnimation(FGrannyAssetInstance* a1, FGrannyModelInstanceData const& a2, granny_animation const* a3, granny_sample_context* a4);
};
