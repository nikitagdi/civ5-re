/// origin binary
#pragma once
#include "FirePlace/FireWorks/FSingleton.h"

class FBlockCurveInfo;
class FBlockCurveLibrary;
class FIBlockCurve;
enum FBlockCurveTypes : int;

/// size ?
/// vtable 0x0a305660
class FBlockCurveLibrary : public FSingleton<FBlockCurveLibrary> {
public:
    /// 0x08d7b120
    FBlockCurveLibrary();
    /// 0x08d7b360
    /// deleting 0x08d7b322
    virtual ~FBlockCurveLibrary();
    /// 0x08d7b196
    UNK_RET create_curves();
    /// 0x08d7b398
    UNK_RET destroy_curves();
    /// 0x08d7b3ee
    UNK_RET get_curve(FBlockCurveTypes a1) const;
    /// 0x08d7b3fe
    UNK_RET add_curve(FIBlockCurve* a1);
    /// 0x08d7b450
    UNK_RET get_value(float a1, FBlockCurveTypes a2, int a3, int a4) const;
    /// 0x08d7b4e2
    UNK_RET get_value(float a1, FBlockCurveInfo const* a2, int a3) const;
};
