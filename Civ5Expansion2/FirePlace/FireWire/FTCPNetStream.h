/// origin guess
#pragma once
#include "FirePlace/FireWire/FTCPConnection.h"

/// size ?
/// vtable 0x0a304700
class FTCPNetStream : public FTCPConnection {
public:
    class SendData;
    /// size ?
    class SendData {
    public:
        /// 0x08d5945e
        SendData();
        /// 0x08d59478
        SendData(unsigned char const* a1, unsigned int a2);
        /// 0x08d594c0
        SendData(FTCPNetStream::SendData const& a1);
        /// 0x08d594ee
        ~SendData();
        /// 0x08d5941a
        UNK_RET Update(unsigned int a1);
    };
    /// 0x08d59516
    /// deleting 0x08d5954c
    virtual ~FTCPNetStream();
    /// 0x08d591e8
    virtual UNK_RET Send(unsigned char const* a1, unsigned int a2);
    /// 0x08d592ee
    UNK_RET TrySend(unsigned char const* a1, unsigned int a2);
    /// 0x08d59372
    UNK_RET SendPending();
    /// 0x08d5942a
    UNK_RET ClearPendingSends();
};
