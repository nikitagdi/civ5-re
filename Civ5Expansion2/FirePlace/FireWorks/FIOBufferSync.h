/// origin binary
#pragma once
#include "FirePlace/FireWorks/FIFile.h"
#include "FirePlace/FireWorks/FIOBuffer.h"

class FFileIO;

/// size ?
/// vtable 0x0a305ad0
class FIOBufferSync : public FIOBuffer {
public:
    /// 0x08da37f6
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08da37ac
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08da3b2a
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08da8dae
    virtual UNK_RET Read(void* a1, unsigned int a2);
    /// 0x08da8ee0
    virtual UNK_RET Write(void const* a1, unsigned int a2);
    /// 0x08da8fb2
    virtual UNK_RET Seek(long a1, FIFile::SeekMode a2);
    /// 0x08da9044
    virtual UNK_RET Stream(FFileIO& a1);
    /// 0x08da8d46
    virtual UNK_RET Flush();
    /// 0x08da8d04
    virtual UNK_RET InitBuffer();
};
