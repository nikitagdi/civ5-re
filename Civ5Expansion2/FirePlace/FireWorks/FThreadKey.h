/// origin guess
#pragma once

namespace CvPreGame {
class CustomOption;
}

/// size ?
class FThreadKey {
public:
    /// 0x08dc3502
    FThreadKey();
    /// 0x08dc3528
    ~FThreadKey();
    /// 0x08dc2e6a
    UNK_RET SetKey(void* a1);
    /// 0x08dc33cc
    UNK_RET GetKey() const;
};

/// 0x08dc355e
UNK_RET SetThreadName(unsigned long a1, char const* a2);

namespace FSerialization {
/// 0x08dc45d2
UNK_RET toString(CvPreGame::CustomOption const& a1);
}
