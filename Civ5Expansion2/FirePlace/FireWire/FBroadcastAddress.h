/// origin guess
#pragma once
#include "FirePlace/FireWire/FInetAddress.h"
#include "FirePlace/FireWorks/FString.h"
#include <memory>
#include <vector>

class FDateTime;

/// size ?
/// vtable 0x0a3046c0
class FBroadcastAddress : public FInetAddress {
public:
    /// 0x08d58ed4
    FBroadcastAddress(FStringA a1);
    /// absent
    virtual ~FBroadcastAddress();
};

namespace FNetMiscUtils {
/// 0x08d58ff4
UNK_RET GetNetworkTime(FDateTime* a1);
/// 0x08d59020
UNK_RET DebugPrintIntVector(std::vector<int, std::allocator<int> > const& a1);
}
