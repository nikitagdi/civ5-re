/// origin binary
#pragma once
#include "FirePlace/FireWorks/FIFile.h"

/// size ?
class FFileErrorHandler {
public:
    enum EErrorType : int;
    /// 0x08dd6148
    UNK_RET GetLastError();
    /// 0x08dd61fe
    UNK_RET SetLastError(FFileErrorHandler::EErrorType a1, FIFile* a2, ...);
};
