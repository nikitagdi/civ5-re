/// origin guess
#pragma once

namespace FireGrafix {
class FGXModelInstance;
class FGXModelNode;
}

/// size ?
class CvLeaderHeadFurPhysics {
public:
    class FurPhysicsNode;
    class PhysicsBuffer;
    /// 0x08a2fcf8
    ~CvLeaderHeadFurPhysics();
    /// 0x08a2fd3e
    UNK_RET Clear();
    /// 0x08a2fda2
    UNK_RET Init(FireGrafix::FGXModelInstance& a1, CvLeaderHeadFurPhysics::FurPhysicsNode const* a2, unsigned int a3);
    /// 0x08a2ffee
    UNK_RET ReplaceSurfaceVBs(FireGrafix::FGXModelNode const* a1, unsigned int* a2, unsigned int a3);
    /// 0x08a3007c
    UNK_RET Update(float a1);
    /// 0x08a30542
    UNK_RET FillVB(CvLeaderHeadFurPhysics::PhysicsBuffer const& a1, unsigned int a2);
    /// 0x08a30666
    UNK_RET ClearVelocities();
};
