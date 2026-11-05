/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIFile.h"
#include "FirePlace/FireWorks/FString.h"

class FNetMessage;

/// size ?
class FNetMessageLog {
public:
    /// 0x08d7a1d0
    FNetMessageLog();
    /// 0x08d7a21a
    ~FNetMessageLog();
    /// 0x08d7a26e
    UNK_RET Open(FStringA const& a1, FIFile::OpenMode a2);
    /// 0x08d7a2de
    UNK_RET Close();
    /// 0x08d7a30a
    UNK_RET AppendMessage(FNetMessage& a1);
};
