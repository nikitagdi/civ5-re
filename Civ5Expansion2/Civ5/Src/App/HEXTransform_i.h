/// origin guess
#pragma once
#include "Civ5/Src/App/HEXMatrix_i.h"

class HEXMatrix;
class HEXTransform_i;
class HEXVec;
class HEXVec_i;

/// size 0x14
class HEXTransform_i {
public:
    /// 0x0868d670
    UNK_RET ForwardTransform(HEXVec_i const& a1) const;
    /// 0x0868d988
    UNK_RET InverseTransform(HEXVec_i const& a1) const;
    /// 0x0868dc8e
    HEXMatrix_i matrix() const;
    /// 0x0868dd3c
    HEXMatrix_i inv_matrix() const;
    /// 0x0a3c8b5c
    static HEXTransform_i IDENTITY_TRANSFORM;

    int field_0;   ///< +0x0
    int field_4;   ///< +0x4
    int field_8;   ///< +0x8
    int field_c;   ///< +0xc
    int field_10;  ///< +0x10
};

/// 0x0868d6e8
UNK_RET HEXMat_iForwardTransform(HEXMatrix_i* a1, HEXTransform_i const& a2);
/// 0x0868da00
UNK_RET HEXMat_iInverseTransform(HEXMatrix_i* a1, HEXTransform_i const& a2);
/// 0x0868ddea
UNK_RET HEXMatIdentity(HEXMatrix* a1);
/// 0x0868de49
UNK_RET HEXMatNormalize(HEXMatrix* a1);
/// 0x0868dec4
UNK_RET HEXMatTranslation(HEXMatrix* a1, HEXVec const& a2);
/// 0x0868df09
UNK_RET HEXMatTranslation(HEXMatrix* a1, float a2, float a3, float a4);
/// 0x0868df1d
UNK_RET HEXMatRotate(HEXMatrix* a1, float a2);
/// 0x0868e227
UNK_RET HEXMat_iIdentity(HEXMatrix_i* a1);
/// 0x0868e2af
UNK_RET HEXMat_iRotate(HEXMatrix_i* a1, int a2);
/// 0x0868e39c
UNK_RET HEXMat_iTranslation(HEXMatrix_i* a1, HEXVec_i const& a2);
// Row-major product a1 = a1 * a2; the product is also returned.
/// 0x0868e43a
HEXMatrix_i Fun_0868e43a(HEXMatrix_i* a1, HEXMatrix_i const& a2);
