/// origin guess
#pragma once
#include "FirePlace/FireWorks/FReferenceObject.h"
#include <granny.h>

/// size ?
/// vtable 0x0a301930
class FGrannyFileRefObject : public FReferenceObject {
public:
    /// 0x08cc332c
    FGrannyFileRefObject(granny_file* a1);
    /// 0x08cc33f8
    /// deleting 0x08cc33d0
    virtual ~FGrannyFileRefObject();
    /// 0x08cc3394
    UNK_RET SharedMemoryPush(unsigned int a1);
};
