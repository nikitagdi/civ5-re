/// origin binary
#pragma once
#include "FirePlace/FireWorks/FIFile.h"

class FFileIO;

/// size ?
class FIOBuffer {
public:
    /// 0x08da8800
    UNK_RET Init(FFileIO* a1, unsigned int a2, unsigned int a3, unsigned int a4);
    /// 0x08da883a
    UNK_RET CalcNewFilePos(long a1, FIFile::SeekMode a2, unsigned int a3);
};
