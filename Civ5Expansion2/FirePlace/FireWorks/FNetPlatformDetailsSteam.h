/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include <memory>
#include <string>

class FNetPlatformDetailsSteam;

/// size ?
class FNetPlatformDetailsSteam {
public:
    /// 0x08d4b0dc
    FNetPlatformDetailsSteam(int a1);
    /// 0x08d4b1d4
    FNetPlatformDetailsSteam(FNetPlatformDetailsSteam const& a1);
    /// 0x08d4b158
    UNK_RET Uninit();
    /// 0x08d4b286
    UNK_RET operator=(FNetPlatformDetailsSteam const& a1);
    /// 0x08d4b344
    UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x08d4b39a
    UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d4b3e4
    UNK_RET SetUsername(char const* a1);
};

namespace FNetMessageDebug {
/// 0x08d4b494
UNK_RET GetNetMessageDebugLog();
/// 0x08d4b525
UNK_RET OutputNetMessageDebug(char const* a1);
/// 0x08d4b559
UNK_RET OutputNetMessageDebug(std::string const& a1);
/// 0x08d4b592
UNK_RET SetMessageDebugging(bool a1);
}
