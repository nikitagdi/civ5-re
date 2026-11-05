/// origin guess
#pragma once
#include "FirePlace/FireEngine2/FGrannyAssetInstance.h"
#include <granny.h>

/// size ?
/// vtable 0x0a300430
class LightRigAssetInstance : public FGrannyAssetInstance {
public:
    /// absent
    virtual ~LightRigAssetInstance();
    /// 0x08a337d6
    virtual UNK_RET OnControlledAnimation(unsigned int a1, granny_animation const* a2, granny_sample_context* a3);
};
