/// origin binary
#pragma once

/// size ?
class FSemaphore {
public:
    /// 0x08dc2ba8
    FSemaphore(unsigned int a1, unsigned int a2);
    /// 0x08dc2bf4
    ~FSemaphore();
    /// 0x08dc2c20
    UNK_RET Wait(unsigned int a1);
    /// 0x08dc2c54
    UNK_RET Try();
    /// 0x08dc2c88
    UNK_RET Post();
};

/// 0x08dc2cbc
UNK_RET GetThreadTime(void* a1);
