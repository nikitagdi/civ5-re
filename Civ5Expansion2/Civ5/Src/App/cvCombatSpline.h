/// origin guess
#pragma once

class FGXVector3;
class cvCombatSpline_Init;
class cvCombatSpline_Transform;

/// size ?
class cvCombatSpline {
public:
    /// 0x08b77a40
    cvCombatSpline();
    /// 0x08b77b02
    ~cvCombatSpline();
    /// 0x08b77b04
    UNK_RET Update(float a1);
    /// 0x08b77d38
    UNK_RET GetSplineAttributes(bool a1, float a2, FGXVector3& a3, FGXVector3& a4);
    /// 0x08b77ea0
    UNK_RET UpdateData(cvCombatSpline_Transform const& a1);
    /// 0x08b77eb8
    UNK_RET UpdateData(cvCombatSpline_Init const& a1);
    /// 0x08b77ef2
    UNK_RET GetPosition() const;
    /// 0x08b77f24
    UNK_RET GetTransform() const;
};
