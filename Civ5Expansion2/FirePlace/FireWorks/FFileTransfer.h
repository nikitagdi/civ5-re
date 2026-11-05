/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataTransfer.h"
#include "FirePlace/FireWorks/FString.h"

/// size ?
/// vtable 0x0a304c50
class FFileTransfer : public FDataTransfer {
public:
    /// 0x08d64a70
    FFileTransfer(int a1, FStringA a2, int a3);
    /// 0x08d64c44
    /// deleting 0x08d64c7a
    virtual ~FFileTransfer();
    /// 0x08d64b92
    virtual UNK_RET CheckData();
    /// 0x08d64be2
    virtual UNK_RET CheckDataEnd();
    /// 0x08d64c20
    virtual UNK_RET CloseData();
    /// 0x08d64bd8
    virtual UNK_RET GetSendData();
    /// 0x08d64b38
    virtual UNK_RET OpenData();
    /// 0x08d64ba2
    virtual UNK_RET ReadData(unsigned int a1);
};
