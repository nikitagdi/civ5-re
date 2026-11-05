/// origin binary
#pragma once
#include "FirePlace/FireWorks/FFileIO.h"

/// size ?
/// vtable 0x0a306a40
class FTempFile : public FFileIO {
public:
    /// 0x08dd6348
    FTempFile(wchar_t const* a1);
    /// 0x08dcde00
    /// deleting 0x08dcddd8
    virtual ~FTempFile();
    /// 0x100b2890
    static UNK_DATA ms_iNumTempFiles[0x4];
};
