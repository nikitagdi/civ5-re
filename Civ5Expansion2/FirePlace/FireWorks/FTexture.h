/// origin guess
#pragma once
#include "FirePlace/FireWorks/FReferenceObject.h"
#include "FirePlace/FireWorks/FString.h"

/// size ?
/// vtable 0x0a303230
class FTexture : public FReferenceObject {
public:
    /// 0x08cf0278
    FTexture(bool a1, unsigned int a2, char const* a3);
    /// 0x08cf0340
    /// deleting 0x08cf0318
    virtual ~FTexture();
    /// 0x08cf049e
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08cf02ca
    UNK_RET SetName(char const* a1);
    /// 0x08cf039a
    UNK_RET SetName(FStringA const& a1);
    /// 0x08cf03da
    UNK_RET GetWidth() const;
    /// 0x08cf0414
    UNK_RET GetHeight() const;
    /// 0x08cf044e
    UNK_RET Reload(char const* a1);
    /// 0x0d27bb28
    static UNK_DATA ms_kRTTI[0x8];
};
