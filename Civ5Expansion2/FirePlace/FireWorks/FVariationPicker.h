/// origin guess
#pragma once
#include <miles.h>

/// size ?
class FVariationPicker {
public:
    /// 0x08d39620
    FVariationPicker();
    /// 0x08d3962c
    FVariationPicker(int a1);
    /// 0x08d3969c
    ~FVariationPicker();
    /// 0x08d39640
    UNK_RET SetVariations(int a1);
    /// 0x08d3969e
    UNK_RET GetNumVariations();
    /// 0x08d396a6
    UNK_RET ShuffleVariation();
    /// 0x08d39748
    UNK_RET GetNextVariation();
    /// 0x08d39816
    UNK_RET GetRandomVariation();
};

/// 0x08d398a0
UNK_RET FMilesParabolicFalloffFunction(_SAMPLE* a1, float a2, float a3, float a4, float a5);
/// 0x08d3993b
UNK_RET FMilesLinearFalloffFunction(_SAMPLE* a1, float a2, float a3, float a4, float a5);
/// 0x08d39993
UNK_RET FMilesDefaultFalloffFunction(_SAMPLE* a1, float a2, float a3, float a4, float a5);
