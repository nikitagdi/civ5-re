/// origin guess
#pragma once
#include "FirePlace/FireWorks/FMemoryStream.h"
#include "FirePlace/FireWorks/FString.h"

/// size ?
/// vtable 0x0a303a30
class FDataTransferMgr {
public:
    /// 0x08d408d8
    FDataTransferMgr(int a1);
    /// 0x08d4098c
    /// deleting 0x08d40964
    virtual ~FDataTransferMgr();
    /// 0x08d40a36
    UNK_RET TransferFile(FStringA const& a1, int a2);
    /// 0x08d40b1a
    UNK_RET TransferStream(FMemoryStream* a1, int a2);
    /// 0x08d40bfe
    UNK_RET TransferReady(int a1, unsigned int a2);
    /// 0x08d40c5c
    UNK_RET HaltTransfer(int a1, unsigned int a2);
    /// 0x08d40cba
    UNK_RET StopAllTransfers(int a1);
    /// 0x08d40d0c
    UNK_RET Update();
    /// 0x08d40d80
    UNK_RET ReadyForFile(FStringA const& a1, unsigned int a2, int a3);
    /// 0x08d40e00
    UNK_RET GetSession() const;
    /// 0x08d40e30
    UNK_RET AddIncomingFile(unsigned int a1, FStringA const& a2);
    /// 0x08d40f0e
    UNK_RET ReadyForStream(unsigned int a1, unsigned int a2, int a3);
    /// 0x08d40f8e
    UNK_RET AddIncomingStream(unsigned int a1, unsigned int a2);
    /// 0x08d41056
    UNK_RET GetTransferType(unsigned int a1);
    /// 0x08d410a2
    UNK_RET GetIncomingTransfer(unsigned int a1);
    /// 0x08d410ee
    UNK_RET RemoveIncomingTransfer(unsigned int a1);
    /// 0x08d4116e
    UNK_RET AppendIncomingData(unsigned int a1, unsigned char const* a2, unsigned int a3);
    /// 0x08d411f2
    UNK_RET FinishIncomingData(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08d4137e
    UNK_RET IncomingTransferError(unsigned int a1, unsigned int a2);
};
