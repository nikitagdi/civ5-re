/// origin binary
#pragma once

/// size ?
class FMutex {
public:
    /// 0x08dc2a34
    FMutex(bool a1);
    /// 0x08dc2a7c
    UNK_RET Create(char const* a1);
    /// 0x08dc2af4
    UNK_RET Destroy();
    /// 0x08dc2b22
    UNK_RET Enter();
    /// 0x08dc2b4e
    UNK_RET Try();
    /// 0x08dc2b82
    UNK_RET Leave();
};
