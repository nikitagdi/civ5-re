/// origin guess
#pragma once

class FDDTDecision;
class FDDTSelector;

/// size ?
class FGrannyDecisionSetModelAnimationSampler {
public:
    /// 0x08caccf0
    FGrannyDecisionSetModelAnimationSampler();
    /// 0x08cacd18
    UNK_RET AccumulateModelAnimations(FDDTSelector const& a1, FDDTDecision const* a2, unsigned int a3, float a4, float a5);
};
