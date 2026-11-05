/// origin guess
#pragma once

/// size ?
/// vtable 0x0a304c20
class FDataTransfer {
public:
    /// 0x08d64418
    FDataTransfer(int a1, int a2);
    /// 0x08d64564
    /// deleting 0x08d6453c
    virtual ~FDataTransfer();
    /// 0x08d64a52
    virtual UNK_RET OpenData();
    /// 0x08d64a58
    virtual UNK_RET CheckData();
    /// 0x08d64a5e
    virtual UNK_RET ReadData(unsigned int a1);
    /// 0x08d64a62
    virtual UNK_RET GetSendData();
    /// 0x08d64a66
    virtual UNK_RET CheckDataEnd();
    /// 0x08d64a6c
    virtual UNK_RET CloseData();
    /// 0x08d6450c
    UNK_RET GetSession() const;
    /// 0x08d645c4
    UNK_RET Update();
    /// 0x08d64628
    UNK_RET Notify();
    /// 0x08d64816
    UNK_RET Start();
    /// 0x08d648ac
    UNK_RET Continue();
    /// 0x08d649ca
    UNK_RET Finish();
    /// 0x08d649fe
    UNK_RET Ready();
    /// 0x08d64a0a
    UNK_RET Halt();
    /// 0x08d64a3e
    UNK_RET Done();
};
