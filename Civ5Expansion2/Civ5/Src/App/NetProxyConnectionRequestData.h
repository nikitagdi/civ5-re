/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"

/// size ?
class NetProxyConnectionRequestData {
public:
    /// 0x087784e0
    NetProxyConnectionRequestData();
    /// 0x087784f4
    UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08778562
    UNK_RET SetFromBuffer(FDataStream& a1);
};
