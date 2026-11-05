/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataTransfer.h"
#include "FirePlace/FireWorks/FMemoryStream.h"

/// size ?
/// vtable 0x0a304c90
class FStreamTransfer : public FDataTransfer {
public:
    /// 0x08d64cb8
    FStreamTransfer(int a1, FMemoryStream* a2, int a3);
    /// 0x08d64d70
    FStreamTransfer(int a1, unsigned char* a2, unsigned int a3, int a4);
    /// 0x08d64e4e
    /// deleting 0x08d64e26
    virtual ~FStreamTransfer();
    /// 0x08d64e90
    virtual UNK_RET CheckData();
    /// 0x08d64eb2
    virtual UNK_RET CheckDataEnd();
    /// 0x08d64ea6
    virtual UNK_RET GetSendData();
    /// 0x08d64ea0
    virtual UNK_RET ReadData(unsigned int a1);
};
