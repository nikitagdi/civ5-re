/// origin guess
#pragma once

class HEXMatrix;
class HEXTransform;
class HEXVec;

/// size ?
class HEXTransform {
public:
    /// 0x0868d3e8
    UNK_RET ForwardTransform(HEXVec const& a1) const;
    /// 0x0868d510
    UNK_RET InverseTransform(HEXVec const& a1) const;
    /// 0x0868d618
    UNK_RET matrix() const;
    /// 0x0868d644
    UNK_RET inv_matrix() const;
    /// 0x0a3c8b70
    static UNK_DATA IDENTITY_TRANSFORM[0x18];
};

/// 0x0868d44c
UNK_RET HEXMatForwardTransform(HEXMatrix* a1, HEXTransform const& a2);
/// 0x0868d574
UNK_RET HEXMatInverseTransform(HEXMatrix* a1, HEXTransform const& a2);
