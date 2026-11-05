/// origin guess
#pragma once
#include "FirePlace/FireWorks/FMemoryStream.h"
#include "FirePlace/FireWorks/FString.h"

/// size ?
/// vtable 0x0a303a20
class FDataTransferTarget {
public:
    /// 0x08d402ec
    FDataTransferTarget(int a1, int a2);
    /// 0x08d40398
    /// deleting 0x08d40370
    virtual ~FDataTransferTarget();
    /// 0x08d403fe
    UNK_RET AddFileTransfer(FStringA const& a1);
    /// 0x08d4052c
    UNK_RET AddStreamTransfer(FMemoryStream* a1);
    /// 0x08d405a4
    UNK_RET TransferReady(unsigned int a1);
    /// 0x08d40608
    UNK_RET HaltTransfer(unsigned int a1);
    /// 0x08d4066c
    UNK_RET ClearTransfers();
    /// 0x08d406c6
    UNK_RET Update();
    /// 0x08d408a8
    UNK_RET GetSession() const;
};
