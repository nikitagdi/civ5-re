/// origin guess
#pragma once
#include "FirePlace/FireWorks/FResourceLoadHelper.h"

class FGrannyAssetLoadFilter;

/// size ?
/// vtable 0x0a301b20
class FGrannyAssetResourceLoadHelper : public FResourceLoadHelper {
public:
    class FailureListener;
    /// 0x08cd431a
    FGrannyAssetResourceLoadHelper(float a1, FGrannyAssetLoadFilter* a2, unsigned int a3, FGrannyAssetResourceLoadHelper::FailureListener* a4);
    /// 0x08cd4a6c
    /// deleting 0x08cd4ab6
    virtual ~FGrannyAssetResourceLoadHelper();
    /// 0x08a2fce2
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08cd4374
    virtual UNK_RET CreateEmptyAsset();
    /// 0x08cd43d2
    UNK_RET Create(float a1, FGrannyAssetLoadFilter* a2, unsigned int a3, FGrannyAssetResourceLoadHelper::FailureListener* a4);
    /// 0x08cd4450
    UNK_RET Create(FGrannyAssetLoadFilter* a1, FGrannyAssetResourceLoadHelper::FailureListener* a2);
    /// 0x0d276c7c
    static UNK_DATA ms_kRTTI[0x8];
};
