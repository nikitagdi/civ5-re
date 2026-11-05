/// origin guess
#pragma once

class FGXVector2;
class FGXVector3;

/// size ?
class cvBezier {
public:
    /// 0x08a24e10
    cvBezier();
    /// 0x08a24e28
    ~cvBezier();
    /// 0x08a24e2e
    UNK_RET ReserveControlPoints(unsigned int a1);
    /// 0x08a24e34
    UNK_RET ClearControlPoints();
    /// 0x08a24e40
    UNK_RET FreeControlPoints();
    /// 0x08a24e78
    UNK_RET AddControlPoint(FGXVector2 const& a1);
    /// 0x08a24ec2
    UNK_RET AddControlPoint(FGXVector3 const& a1);
    /// 0x08a24ec8
    UNK_RET GetControlPoint(unsigned int a1) const;
    /// 0x08a24ed4
    UNK_RET GetNumControlPoints() const;
    /// 0x08a24edc
    UNK_RET GetDirectionOnSpline(float a1) const;
    /// 0x08a24faa
    UNK_RET FirstDerivative(float a1) const;
    /// 0x08a24fea
    UNK_RET GetPointOnSpline(float a1) const;
    /// 0x08a2502a
    UNK_RET GeneralDerivative(float a1, int a2) const;
    /// 0x08a25188
    UNK_RET SecondDerivative(float a1) const;
};
