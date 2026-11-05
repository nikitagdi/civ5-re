/// origin guess
#pragma once
#include <d3d9.h>

/// size ?
class CvLeaderHeadModelInstance {
public:
    /// 0x0887f5d0
    UNK_RET Update(float a1);
    /// 0x0887f66c
    UNK_RET RecomputeTransforms();
    /// 0x0887f8b0
    UNK_RET SetFSMPathAction(float a1, bool a2);
    /// 0x0887f90c
    UNK_RET ResetAnimation();
    /// 0x0887f990
    UNK_RET MultiplyRootTransform(D3DXMATRIX const& a1);
};
