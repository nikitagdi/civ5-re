/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"

class KickReason;

/// size ?
class FUrlParser {
public:
    /// 0x08d5bd14
    FUrlParser(char const* a1, unsigned int a2);
    /// 0x08d5bd68
    UNK_RET Parse();
    /// 0x08d5bf92
    UNK_RET Error() const;
};

/// 0x08d5bf9c
UNK_RET operator<<(FDataStream& a1, KickReason const& a2);
/// 0x08d5bfd3
UNK_RET operator>>(FDataStream& a1, KickReason& a2);
