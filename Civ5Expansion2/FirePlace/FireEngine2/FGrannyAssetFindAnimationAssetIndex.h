/// origin guess
#pragma once
#include <granny.h>

class FGrannyAsset;

/// size ?
class FGrannyAssetFindAnimationAssetIndex {
public:
    /// 0x08cc841a
    FGrannyAssetFindAnimationAssetIndex(granny_animation const* a1);
    /// 0x08cc8426
    UNK_RET operator()(FGrannyAsset* a1);
};
