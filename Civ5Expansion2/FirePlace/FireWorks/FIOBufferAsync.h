/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIFile.h"
#include "FirePlace/FireWorks/FIOBuffer.h"

class FFileIO;

/// size ?
/// vtable 0x0a305b00
class FIOBufferAsync : public FIOBuffer {
public:
    /// 0x08da38fe
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08da3840
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08da3b70
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08da8a9c
    virtual UNK_RET Read(void* a1, unsigned int a2);
    /// 0x08da8b8c
    virtual UNK_RET Write(void const* a1, unsigned int a2);
    /// 0x08da8b90
    virtual UNK_RET Seek(long a1, FIFile::SeekMode a2);
    /// 0x08da8c56
    virtual UNK_RET Stream(FFileIO& a1);
    /// 0x08da3bd2
    virtual UNK_RET vf_07(UNK_PARAMS);
    /// 0x08da8908
    virtual UNK_RET InitBuffer();
    /// 0x08da89c2
    UNK_RET StartAsyncRead(bool a1);
    /// 0x08da8a10
    UNK_RET FinishAsyncRead(bool a1);
};
